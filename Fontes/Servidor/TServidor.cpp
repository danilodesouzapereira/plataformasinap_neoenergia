//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DateUtils.hpp>
#include "TServidor.h"
#include "..\BDlicenca\VTBDlicenca.h"
#include "..\Licenca\VTCampo.h"
#include "..\Licenca\VTLicenca.h"
#include "..\Licenca\VTPacote.h"
#include "..\Servidor\VTLoglic.h"
#include "..\..\DLL_Inc\BDlicenca.h"
#include "..\..\DLL_Inc\Licenca.h"
#include "..\..\DLL_Inc\Servidor.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//-----------------------------------------------------------------------------
VTServidor* __fastcall NewObjServidor(void)
   {
   return(new TServidor());
   }

//---------------------------------------------------------------------------
__fastcall TServidor::TServidor(void)
   {
   try{//cria objetos
      bdlicenca = DLL_NewObjBDlicenca();
      loglic    = NewObjLoglic(bdlicenca);
      }catch(Exception &e)
         {
         }
   }

//---------------------------------------------------------------------------
__fastcall TServidor::~TServidor(void)
   {
   //destr�i objetos
   if (bdlicenca) {delete bdlicenca; bdlicenca = NULL;}
   if (loglic)    {delete loglic;    loglic    = NULL;}
   }

//-----------------------------------------------------------------------------
bool __fastcall TServidor::AlocaLicenca(VTLicenca *licenca, AnsiString &diagnostico)
   {
   //vari�veis locais
   int status;

   //salva ponteiro p/ objeto
   lic_micro = licenca;
   //verifica se � aloca��o de Licenca Demo
   if (lic_micro->Tipo == tipoDEMO)
      {//se necess�rio, cria Licen�a Demo p/ o usu�rio
      if (! CriaLicencaDemo(lic_micro))
         {//grava mensagem de log na base de dados
         loglic->ErroAlocaDemo_LoginRepetido(lic_micro);
         //monta Licenca de resposta p/ o micro c/ indica��o do erro
         DefineLicencaInvalida(lic_micro, sttINEXISTENTE);
         return(false);
         }
      }
   //verifica na base de dados se h� licen�a cadastrada p/ o usu�rio indicado
   lic_bdado = bdlicenca->ExisteLicenca(lic_micro->Produto, lic_micro->Login, lic_micro->Password);
   if (lic_bdado == NULL)
      {//grava mensagem de log na base de dados
      loglic->ErroAloca_LicInexistente(lic_micro);
      //monta Licenca de resposta p/ o micro c/ indica��o do erro
      DefineLicencaInvalida(lic_micro, sttINEXISTENTE);
      return(false);
      }
   //valida a aloca��o da Licenca
   if ((status = ValidaAlocaLicenca(lic_bdado, lic_micro)) != sttALOCADA)
      {//monta Licenca de resposta p/ o micro c/ indica��o do erro
      DefineLicencaInvalida(lic_micro, status);
      return(false);
      }
   //aloca��o v�lida: atualiza Licenca na base de dados
   lic_bdado->CPU_ID      = lic_micro->CPU_ID;
   lic_bdado->DH_alocada  = DateOf(Now());
   lic_bdado->DH_inicio   = DateOf(Now());
   lic_bdado->DH_termino  = IncDay(lic_bdado->DH_inicio, lic_bdado->Validade);
   lic_bdado->DH_controle = lic_bdado->DH_inicio;
   lic_bdado->Status      = sttALOCADA;
   lic_bdado->Renovada    = 0;
   lic_bdado->LoginLocal  = false;
   //lic_bdado->CtrlRemoto  = true;
   bdlicenca->AtualizaLicenca(lic_bdado);
   //atualiza Licenca a ser retornada p/ o micro do usu�rio
   lic_micro->ID          = lic_bdado->ID;
   lic_micro->DH_alocada  = lic_bdado->DH_alocada;
   lic_micro->DH_inicio   = lic_bdado->DH_inicio;
   lic_micro->DH_termino  = lic_bdado->DH_termino;
   lic_micro->DH_controle = lic_bdado->DH_controle;
   lic_micro->Status      = lic_bdado->Status;
   lic_micro->Tipo        = lic_bdado->Tipo;
   lic_micro->Validade    = lic_bdado->Validade;
   lic_micro->Renovada    = lic_bdado->Renovada;
   lic_micro->LoginLocal  = lic_bdado->LoginLocal;
   lic_micro->CtrlRemoto  = lic_bdado->CtrlRemoto;
   return(true);
   }

//-----------------------------------------------------------------------------
void __fastcall TServidor::BloqueiaLicencaNaBaseDado(VTLicenca *licenca)
   {
   licenca->CPU_ID      = "indefinido";
   licenca->DH_alocada  = DateOf(Now());
   licenca->DH_inicio   = licenca->DH_alocada;
   licenca->DH_termino  = licenca->DH_alocada;
   licenca->DH_controle = licenca->DH_alocada;
   licenca->Status      = sttBLOQUEADA;
   licenca->Validade    = 0;
   licenca->Renovada    = 0;
   bdlicenca->AtualizaLicenca(licenca);
   //grava mensagem de log na base de dados
   loglic->LicencaBloqueada(licenca);
   }

//-----------------------------------------------------------------------------
bool __fastcall TServidor::Conecta(AnsiString database_name)
   {
   return(bdlicenca->Conecta(database_name));
   }

//---------------------------------------------------------------------------
bool __fastcall TServidor::ConsultaPendrive(VTPendrive *pendrive)
   {
   return(bdlicenca->ConsultaPendrive(pendrive));
   }

//-----------------------------------------------------------------------------
bool __fastcall TServidor::CriaLicencaDemo(VTLicenca *licenca)
   {
   //verifica se existe Licenca na base de dados
   lic_bdado = bdlicenca->ExisteLicenca(lic_micro->Produto, lic_micro->Login, lic_micro->Password);
   if (lic_bdado != NULL)
      {//verifica se � uma Licenca demo
      return(lic_bdado->Tipo == tipoDEMO);
      }
   //cria Licenca demo na base de dados p/ o usu�rio
   return(bdlicenca->InsereLicenca(licenca));
   }

//-----------------------------------------------------------------------------
void __fastcall TServidor::DefineLicencaInvalida(VTLicenca *licenca, int status)
   {
   //solicita��o inv�lida
   licenca->ID          = -1;
   licenca->CPU_ID      = "indefinido";
   licenca->DH_alocada  = 0.;
   licenca->DH_inicio   = 0.;
   licenca->DH_termino  = 0.;
   licenca->DH_controle = DateOf(Now());
   licenca->Status      = status;
   licenca->Tipo        = tipoDEMO;
   licenca->Validade    = 0;
   }

//-----------------------------------------------------------------------------
void __fastcall TServidor::Desconecta(void)
   {
   bdlicenca->Desconecta();
   }

//-----------------------------------------------------------------------------
bool __fastcall TServidor::LiberaLicenca(VTLicenca *licenca)
   {
   //vari�veis locais
   int  status;

   //salva ponteiro p/ objeto
	lic_micro = licenca;
   //verifica na base de dados se h� licen�a alocada p/ o usu�rio indicado
   lic_bdado = bdlicenca->ExisteLicenca(lic_micro->Produto, lic_micro->Login, lic_micro->Password);
   if (lic_bdado == NULL)
      {//grava mensagem de log na base de dados
      loglic->ErroLibera_LicInexistente(lic_micro);
      //monta Licenca de resposta p/ o micro c/ indica��o do erro
      DefineLicencaInvalida(lic_micro, sttINEXISTENTE);
	  return(false);
      }
   //valida libera��o da Licenca
	if ((status = ValidaLiberaLicenca(lic_bdado, lic_micro)) != sttLIVRE)
      {//verifica necessidade de bloquear Licenca na base de dados
		if (status == sttBLOQUEADA) BloqueiaLicencaNaBaseDado(lic_bdado);
      //monta Licenca de resposta p/ o micro c/ indica��o do erro
      DefineLicencaInvalida(lic_micro, status);
      return(false);
      }
   //libera��o v�lida: atualiza Licenca na base de dado
   lic_bdado->CPU_ID      = "indefinido";
   lic_bdado->DH_alocada  = DateOf(Now());
   lic_bdado->DH_inicio   = lic_bdado->DH_alocada;
   lic_bdado->DH_termino  = lic_bdado->DH_alocada;
   lic_bdado->DH_controle = lic_bdado->DH_alocada;
	lic_bdado->Renovada    = 0;
	//verifica se h� controle de per�odo de validade
	if (lic_bdado->Validade < 0)
      {//licen�a sem limite de validade
      lic_bdado->Status   = sttLIVRE;
      }
   else
		{//verifica se a Licenca deve ser liberada ou bloqueada
		if (lic_micro->DH_controle < lic_micro->DH_termino)
			{//licen�a est� livre mas com menor tempo de validade
			lic_bdado->Status   = sttLIVRE;
			lic_bdado->Validade = abs(DaysBetween(lic_micro->DH_termino, lic_micro->DH_controle));
         //por seguran�a, sempre decrementa um dia
			if (lic_bdado->Validade > 0) lic_bdado->Validade--;
			}
		else
			{
			lic_bdado->Status   = sttEXPIRADA;
			lic_bdado->Validade = 0;
			}
		}
   bdlicenca->AtualizaLicenca(lic_bdado);
   //atualiza Licenca a ser retornada p/ o micro do usu�rio
   lic_micro->ID          = lic_bdado->ID;
   lic_micro->DH_alocada  = lic_bdado->DH_alocada;
   lic_micro->DH_inicio   = lic_bdado->DH_inicio;
   lic_micro->DH_termino  = lic_bdado->DH_termino;
   lic_micro->DH_controle = lic_bdado->DH_controle;
   lic_micro->Status      = lic_bdado->Status;
   lic_micro->Tipo        = lic_bdado->Tipo;
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TServidor::RenovaLicenca(VTLicenca *licenca)
   {
   //vari�veis locais
   int  status;

   //salva ponteiro p/ objeto
	lic_micro = licenca;
   //verifica na base de dados se h� licen�a alocada p/ o usu�rio indicado
   lic_bdado = bdlicenca->ExisteLicenca(lic_micro->Produto, lic_micro->Login, lic_micro->Password);
   if (lic_bdado == NULL)
      {//grava mensagem de log na base de dados
      loglic->ErroRenova_LicInexistente(lic_micro);
      //monta Licenca de resposta p/ o micro c/ indica��o do erro
      DefineLicencaInvalida(lic_micro, sttINEXISTENTE);
      return(false);
      }
   //valida a renova��o da Licenca
	if ((status = ValidaRenovaLicenca(lic_bdado, lic_micro)) != sttALOCADA)
		{//verifica necessidade de bloquear Licenca na base de dados
		if (status == sttBLOQUEADA) BloqueiaLicencaNaBaseDado(lic_bdado);
      //monta Licenca de resposta c/ indica��o do erro
      DefineLicencaInvalida(lic_micro, status);
      return(false);
      }
   //renova��o v�lida: atualiza Licenca na base de dado
   //lic_bdado->CPU_ID       = licenca->CPU_ID;
   //lic_bdado->DH_alocada   = DateOf(Now());
   lic_bdado->DH_inicio   = DateOf(Now());;
   lic_bdado->DH_termino  = IncDay(lic_bdado->DH_inicio, lic_bdado->Validade);
   lic_bdado->DH_controle = lic_bdado->DH_inicio;
   lic_bdado->Status      = sttALOCADA;
   lic_bdado->Renovada    = lic_bdado->Renovada + 1;
   bdlicenca->AtualizaLicenca(lic_bdado);
   //atualiza Licenca a ser retornada p/ o micro do usu�rio
   lic_micro->ID          = lic_bdado->ID;
   lic_micro->DH_alocada  = lic_bdado->DH_alocada;
   lic_micro->DH_inicio   = lic_bdado->DH_inicio;
   lic_micro->DH_termino  = lic_bdado->DH_termino;
   lic_micro->DH_controle = lic_bdado->DH_controle;
   lic_micro->Status      = lic_bdado->Status;
   lic_micro->Tipo        = lic_bdado->Tipo;
   lic_micro->Validade    = lic_bdado->Validade;
   lic_micro->Renovada    = lic_bdado->Renovada;
   return(true);
	}

//-----------------------------------------------------------------------------
int __fastcall TServidor::ValidaAlocaLicenca(VTLicenca *lic_bdado, VTLicenca *lic_micro)
	{
   //vari�veis locais
   int status;

   //prote��o
	if (lic_bdado == NULL) return(sttINEXISTENTE);
   //verifica status da Licenca na base de dados
	switch(status = lic_bdado->Status)
      {
      case sttLIVRE: //aloca��o v�lida
         status = sttALOCADA;
         //grava mensagem de log na base de dados
			loglic->SucessoAloca(lic_bdado);
         break;
      case sttALOCADA:  //verifica se alocada p/ um micro diferente
			if (lic_bdado->CPU_ID.AnsiCompareIC(lic_micro->CPU_ID) != 0)
            {//Licenca instalada em outro micro
            status = sttBLOQUEADA;
            //grava mensagem de log na base de dados
				loglic->ErroAloca_LicAlocada(lic_bdado);
            }
         else
            {
            //grava mensagem de log na base de dados
				loglic->SucessoAloca(lic_bdado);
            }
         break;
      case sttEXPIRADA:    //aloca��o inv�lida
      case sttBLOQUEADA:   //aloca��o inv�lida
      case sttINEXISTENTE: //aloca��o inv�lida
         //grava mensagem de log na base de dados
			loglic->ErroAloca_LicBloqueada(lic_bdado);
         break;
      }
   return(status);
   }

//-----------------------------------------------------------------------------
int __fastcall TServidor::ValidaLiberaLicenca(VTLicenca *lic_bdado, VTLicenca *lic_micro)
   {
   //vari�veis locais
   int status;

   //prote��o
	if (lic_bdado == NULL) return(sttINEXISTENTE);
   //verifica status da Licenca na base de dados
	switch(status = lic_bdado->Status)
      {
      case sttLIVRE: //libera��o inconsistente
         status = sttBLOQUEADA;
         //grava mensagem de log na base de dados
			loglic->ErroLibera_LicLivre(lic_bdado);
         break;
		case sttEXPIRADA:	//Licenca c/ validade experidada
			status = sttEXPIRADA;
			//grava mensagem de log na base de dados
			loglic->ErroLibera_LicAlocada(lic_bdado);
			break;
		case sttALOCADA:  //verifica se alocada p/ o mesmo micro
			if (lic_bdado->CPU_ID.AnsiCompareIC(lic_micro->CPU_ID) == 0)
            {//libera��o v�lida
            status = sttLIVRE;
            //grava mensagem de log na base de dados
				loglic->SucessoLibera(lic_bdado);
            }
         else
            {//Licenca instalada em outro micro
            status = sttBLOQUEADA;
            //grava mensagem de log na base de dados
				loglic->ErroLibera_LicAlocada(lic_bdado);
            }
         break;
      case sttBLOQUEADA:   //aloca��o inv�lida
      case sttINEXISTENTE: //aloca��o inv�lida
         //grava mensagem de log na base de dados
			loglic->ErroLibera_LicBloqueada(lic_bdado);
         break;
      }
   return(status);
   }

//-----------------------------------------------------------------------------
int __fastcall TServidor::ValidaRenovaLicenca(VTLicenca *lic_bdado, VTLicenca *lic_micro)
   {
   //vari�veis locais
   int status;

   //prote��o
	if (lic_bdado == NULL) return(sttINEXISTENTE);
   //verifica status da Licenca na base de dados
	switch(status = lic_bdado->Status)
      {
      case sttLIVRE: //renova��o inconsistente
         status = sttBLOQUEADA;
         //grava mensagem de log na base de dados
			loglic->ErroRenova_LicLivre(lic_bdado);
         break;
      case sttEXPIRADA:
      case sttALOCADA:  //verifica se alocada p/ o mesmo micro
			if (lic_bdado->CPU_ID.AnsiCompareIC(lic_micro->CPU_ID) == 0)
            {//renova��o v�lida
            status = sttALOCADA;
            //grava mensagem de log na base de dados
				loglic->SucessoRenova(lic_bdado);
            }
         else
            {//Licenca instalada em outro micro
            status = sttBLOQUEADA;
            //grava mensagem de log na base de dados
				loglic->ErroRenova_LicAlocada(lic_bdado);
            }
         break;
      case sttBLOQUEADA:   //aloca��o inv�lida
      case sttINEXISTENTE: //aloca��o inv�lida
         //grava mensagem de log na base de dados
         loglic->ErroRenova_LicBloqueada(lic_bdado);
         break;
      }
   return(status);
   }

//-----------------------------------------------------------------------------
//eof



