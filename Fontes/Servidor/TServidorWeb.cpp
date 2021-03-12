//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DateUtils.hpp>
#include "TServidorWeb.h"
#include "..\Apl\VTApl.h"
#include "..\Diretorio\DirArq.h"
#include "..\Licenca\VTLicenca.h"
#include "..\Micro\VTPendrive.h"
#include "..\Micro\VTPenFinder.h"
#include "..\..\DLL_Inc\BDlicenca.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Micro.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//-----------------------------------------------------------------------------
//VTServidor* __fastcall NewObjServidorWeb(void)
VTServidor* __fastcall NewObjServidorRemoto(void)
   {
   return(new TServidorWeb());
   }

//---------------------------------------------------------------------------
__fastcall TServidorWeb::TServidorWeb(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
__fastcall TServidorWeb::~TServidorWeb(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
bool __fastcall TServidorWeb::AlocaLicenca(VTLicenca *licenca, AnsiString &diagnostico)
   {
   //variáveis locais
   Licenca                *lic_nova, *lic_web;
   _di_SinapServerPortType servico;
   int status;

   //verifica se existe usuário/senha para produto
   if(ConectaWS(licenca, diagnostico) <= 0) return false;

   //cria licença web
   lic_nova = new Licenca();

   //atualiza Licenca
   licenca->CtrlRemoto = true;
   //salva ponteiro p/ licença do micro
   lic_micro = licenca;
   //inicia dados da licença web
   lic_nova->cpu_id      = lic_micro->CPU_ID;
   lic_nova->produto     = lic_micro->Produto;
   lic_nova->login       = lic_micro->Login;
   lic_nova->password    = lic_micro->Password;
   lic_nova->loginLocal  = lic_micro->LoginLocal;
   lic_nova->ctrlRemoto  = lic_micro->CtrlRemoto;
   lic_nova->dh_alocada  = lic_micro->DH_alocada.FormatString("dd/mm/yyyy");
   lic_nova->dh_inicio   = lic_micro->DH_inicio.FormatString("dd/mm/yyyy");
   lic_nova->dh_controle = lic_micro->DH_controle.FormatString("dd/mm/yyyy");

   //chama serviço web para lê licença
   try{
	  servico = GetSinapServerPortType(false, "");
      if (servico) lic_web = servico->LeLicenca(lic_nova);
      }
      catch(...)
         {
         Erro("Falha de comunicação com o Servidor de Licenças.\n");
         if(lic_nova) {delete lic_nova; lic_nova = NULL;}
         return(false);
         }

   //valida a licença retornada pelo webservice: verifica resposta da alocação WEB
   status = ValidaAlocaLicencaWeb(lic_web, lic_micro);
   if (status != sttALOCADA)
      {//monta Licenca de resposta p/ o micro c/ indicação do erro
      DefineLicencaInvalida(lic_micro, status);
      if(lic_nova) {delete lic_nova; lic_nova = NULL;}
      return(false);
      }
   //atualiza demais dados da Licenca a ser retornada p/ o micro do usuário
   lic_micro->ID          = lic_web->id;
   lic_micro->Empresa     = lic_web->empresa_id;
   lic_micro->Status      = lic_web->status;
   lic_micro->Tipo        = lic_web->tipo;
   lic_micro->Validade    = lic_web->validade;
   lic_micro->DH_termino  = IncDay(lic_micro->DH_inicio, lic_micro->Validade);  //calcula a data de término
   lic_micro->Status      = sttALOCADA;

   //alocação válida: atualiza Licenca na base de dados
   lic_nova->id          = lic_micro->ID;
   lic_nova->status      = lic_micro->Status;
   lic_nova->tipo        = lic_micro->Tipo;
   lic_nova->dh_termino  = lic_micro->DH_termino.FormatString("dd/mm/yyyy");
   lic_nova->validade    = lic_micro->Validade;
   lic_nova->status      = sttALOCADA;

   //chama serviço web para atualizar licença
   try{
      servico = GetSinapServerPortType(false, "");
      if (servico) status = servico->AtualizaLicenca(lic_nova);
      }
      catch(...)
         {
         Erro("Falha de comunicação com o Servidor de Licenças.\n");
         if(lic_nova) {delete lic_nova; lic_nova = NULL;}
         return(false);
         }

   //destrói objeto
   if(lic_nova) {delete lic_nova; lic_nova = NULL;}

   return(status > 0);
   }
//---------------------------------------------------------------------------
int __fastcall TServidorWeb::CalculaValidade(VTLicenca *licenca)
   {
   //verifica se há controle de período de validade
   if(licenca->Validade < 0) return licenca->Validade;

   if (licenca->DH_controle < licenca->DH_termino)
      {//licença está livre mas com menor tempo de validade
      licenca->Validade = abs(DaysBetween(licenca->DH_termino, licenca->DH_controle));
      //por segurança, sempre decrementa um dia
      if (licenca->Validade > 0) licenca->Validade--;
      }
   else
      {
      licenca->Validade = 0;
      }
   return (licenca->Validade);
   }
//---------------------------------------------------------------------------
bool __fastcall TServidorWeb::ConectaWS(VTLicenca *licenca, AnsiString &diagnostico)
   {
   //variáveis locais
   _di_SinapServerPortType servico;
   int status = 0;

   //chama serviço web para alocar licença
   try{
      servico = GetSinapServerPortType(false, "");
	  if(servico) status = servico->Conecta(licenca->Login, licenca->Password);
	  if(! status)
		 {
		 diagnostico = "login/senha inválido(a)";
		 Erro(diagnostico);
		 }
      }
      catch(...)
		 {
		 diagnostico = "Falha na conexão com o servidor de licenças";
		 Erro(diagnostico);
         return(false);
         }
   return(status);
   }

//---------------------------------------------------------------------------
bool __fastcall TServidorWeb::LiberaLicenca(VTLicenca *licenca)
   {
   //variáveis locais
   int       status;
   Licenca  *lic_aloc, *lic_web;
   _di_SinapServerPortType servico;
   AnsiString diagnostico;

   //verifica se existe usuário/senha para produto
   if(ConectaWS(licenca, diagnostico) <= 0) return false;

   //salva ponteiro p/ objeto
   lic_micro = licenca;

   //cria licença para copiar os dados da licença alocada
   lic_aloc = new Licenca();

   //inicia dados da licença web
   CalculaValidade(lic_micro);
   lic_aloc->id          = lic_micro->ID;
   lic_aloc->cpu_id      = lic_micro->CPU_ID;
   lic_aloc->produto     = lic_micro->Produto;
   lic_aloc->login       = lic_micro->Login;
   lic_aloc->password    = lic_micro->Password;
   lic_aloc->loginLocal  = lic_micro->LoginLocal;
   lic_aloc->ctrlRemoto  = lic_micro->CtrlRemoto;
   lic_aloc->dh_alocada  = lic_micro->DH_alocada.FormatString("dd/mm/yyyy");
   lic_aloc->dh_inicio   = lic_micro->DH_inicio.FormatString("dd/mm/yyyy");
   lic_aloc->dh_controle = lic_micro->DH_controle.FormatString("dd/mm/yyyy");
   lic_aloc->validade    = lic_micro->Validade;

   //chama serviço web para ler licença existente
   try{
      servico = GetSinapServerPortType(false, "");
      if (servico) lic_web = servico->LeLicenca(lic_aloc);
      }
      catch(...)
         {
         Erro("Falha de comunicação com o Servidor de Licenças.\n");
         //destrói objeto
         if(lic_aloc) {delete lic_aloc; lic_aloc = NULL;}
         return(false);
         }

   //valida liberação da Licenca: verifica resposta da alocação WEB
   status = ValidaLiberaLicencaWeb(lic_web, lic_micro);
   if (status != sttLIVRE)
      {
      //monta Licenca de resposta p/ o micro c/ indicação do erro
      DefineLicencaInvalida(lic_micro, status);
      //se tiver CPU diferente ? bloquear no servidor...

      if(lic_aloc) {delete lic_aloc; lic_aloc = NULL;}
      return(false);
      }

   //verifica se há controle de período de validade
   if(lic_micro->Validade < 0)
      {//sem validade
      lic_micro->Status   = sttLIVRE;
      }
   else
      {//verifica se a Licenca deve ser liberada ou bloqueada
      if (lic_micro->DH_controle < lic_micro->DH_termino)
         {//licença está livre mas com menor tempo de validade
         lic_micro->Status   = sttLIVRE;
         lic_micro->Validade = abs(DaysBetween(lic_micro->DH_termino, lic_micro->DH_controle));
         //por segurança, sempre decrementa um dia
         if (lic_micro->Validade > 0) lic_micro->Validade--;
         }
      else
         {
         lic_micro->Status   = sttEXPIRADA;
         lic_micro->Validade = 0;
         }
      }
   //liberação válida: atualiza Licenca a ser retornada p/ o micro do usuário
   //lic_micro->DH_alocada  = DateOf(Now());
   lic_micro->DH_inicio   = DateOf(Now());
   lic_micro->DH_controle = DateOf(Now());
   lic_micro->DH_termino  = IncDay(lic_micro->DH_inicio, lic_micro->Validade);  //calcula a data de término
   lic_micro->Renovada    = 0;

   //atualiza licença na base
   lic_aloc->cpu_id      = "indefinido";
   lic_aloc->dh_alocada  = lic_micro->DH_alocada.FormatString("dd/mm/yyyy");
   lic_aloc->dh_inicio   = lic_micro->DH_inicio.FormatString("dd/mm/yyyy");
   lic_aloc->dh_termino  = lic_micro->DH_termino.FormatString("dd/mm/yyyy");
   lic_aloc->dh_controle = lic_micro->DH_controle.FormatString("dd/mm/yyyy");
   lic_aloc->renovada    = lic_micro->Renovada;
   lic_aloc->status      = lic_micro->Status;
   lic_aloc->validade    = lic_micro->Validade;

   //chama serviço web para atualizar licença
   try{
      servico = GetSinapServerPortType(false, "");
      if (servico) status = servico->AtualizaLicenca(lic_aloc);
      }
      catch(...)
         {
         Erro("Falha de comunicação com o Servidor de Licenças.\n");
         if(lic_aloc) {delete lic_aloc; lic_aloc = NULL;}
         return(false);
         }

   //destrói objeto
   if(lic_aloc) {delete lic_aloc; lic_aloc = NULL;}

   return(status > 0);
   }
//---------------------------------------------------------------------------
AnsiString  __fastcall TServidorWeb::PathDatabase(VTPendrive *pendrive)
   {
   return(pendrive->DiskUnit + "\\SinapsisInovacaoEmEnergia\\Data\\" + DBNAME_LICENCA);
   }

//---------------------------------------------------------------------------
bool __fastcall TServidorWeb::RenovaLicenca(VTLicenca *licenca)
   {

   return(false);
   }

//-----------------------------------------------------------------------------
int __fastcall TServidorWeb::ValidaAlocaLicencaWeb(Licenca *lic_web, VTLicenca *lic_micro)
   {
   //variáveis locais
   int status;

   //proteção
   if (lic_web == NULL) return(sttINEXISTENTE);
   if (! lic_web->id)   return(sttINEXISTENTE);

   //verifica status da Licenca na base de dados
   switch(status = lic_web->status)
      {
      case sttLIVRE: //alocação válida
         status = sttALOCADA;
         //log
         break;
      case sttALOCADA:  //verifica se alocada p/ um micro diferente
         if (lic_micro->CPU_ID.AnsiCompareIC(lic_web->cpu_id) != 0)
            {//Licenca instalada em outro micro
            status = sttBLOQUEADA;
            Erro("A licença já foi alocada em outro micro");
            //log
            }
         else
            {
            //log
            }
         break;
      case sttEXPIRADA:    //alocação inválida
         Erro("Licença expirada");
         break;
      case sttBLOQUEADA:   //alocação inválida
         Erro("Licença bloqueada");
         break;
      case sttINEXISTENTE: //alocação inválida
         //log
         Erro("Usuário/senha inválido");
         break;
      }
   return(status);
   }
//-----------------------------------------------------------------------------
int __fastcall TServidorWeb::ValidaLiberaLicencaWeb(Licenca *lic_web, VTLicenca *lic_micro)
   {
   //variáveis locais
   int status;

   //proteção
   if (lic_web == NULL) return(sttINEXISTENTE);
   if (! lic_web->id)   return(sttINEXISTENTE);

   //verifica status da Licenca na base de dados
   switch(status = lic_web->status)
      {
      case sttLIVRE: //liberação inconsistente
         status = sttBLOQUEADA;
         //grava mensagem de log na base de dados
         break;
      case sttEXPIRADA:   //Licenca c/ validade experidada
         status = sttEXPIRADA;
         //grava mensagem de log na base de dados
         break;
      case sttALOCADA:  //verifica se alocada p/ o mesmo micro
         if (lic_micro->CPU_ID.AnsiCompareIC(lic_web->cpu_id) == 0)
            {//liberação válida
            status = sttLIVRE;
            //grava mensagem de log na base de dados
            }
         else
            {//Licenca instalada em outro micro
            status = sttBLOQUEADA;
            //grava mensagem de log na base de dados
            }
         break;
      case sttBLOQUEADA:   //alocação inválida
      case sttINEXISTENTE: //alocação inválida
         //grava mensagem de log na base de dados
         break;
      }
   return(status);

   }
//-----------------------------------------------------------------------------
//eof



