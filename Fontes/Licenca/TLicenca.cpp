//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DateUtils.hpp>
#include "TLicenca.h"
#include "VTCampo.h"
#include "VTPacote.h"
#include "..\..\DLL_Inc\SinapToolKit.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe
//---------------------------------------------------------------------------
VTLicenca* __fastcall NewObjLicenca(void)
   {
   return(new TLicenca());
   }

//---------------------------------------------------------------------------
__fastcall TLicenca::TLicenca(void)
   {
   //inicia dados p/ estado seguro (bloqueada)
   ID          = -1;
   Empresa     = DLL_Empresa_ID();
   CPU_ID      = "indefinido";
   Produto     = "indefinido";
   Login       = "indefinido";
   Password    = "indefinido";
   DH_alocada  = 0.;
   DH_inicio   = 0.;
   DH_termino  = 0.;
   DH_controle = Now();
   Status      = sttBLOQUEADA;
   Tipo        = tipoDEMO;
   Renovada    = 0;
   //assume necessidade de Login local p/ uso do programa
   //LoginLocal  = false;
   //assume que a Licenca é controlada localmente
   CtrlRemoto  = false;
   //inicia array de operacoes
   PD.operacoes.Length = 0;
   }

//---------------------------------------------------------------------------
__fastcall TLicenca::~TLicenca(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TLicenca::Diagnostico(void)
   {
   //variáveis locais
   int  num_dia;

   switch(Status)
	  {
	  case sttALOCADA:
		 //verifica se validade sem limite
		 if (Validade < 0) return("Licenca de Uso habilitada.\nO programa " + Produto + " pode ser utilizado normalmente.");
		 //determina período de validade
		 num_dia = DaysBetween(DH_termino, DH_controle);
		 if (num_dia > 1)
			return("Licenca de Uso habilitada.\nO programa " + Produto + " pode ser utilizado por "  + IntToStr(num_dia) + " dias");
		 else
			return("Licenca de Uso habilitada.\nO programa " + Produto + " pode ser utilizado por "  + IntToStr(num_dia) + " dia");

	  case sttEXPIRADA:
		 return("Licenca de Uso expirada.\nO programa " + Produto + " não pode ser utilizado.");
	  case sttBLOQUEADA:
		 return("Licenca de Uso bloqueada.\nO programa " + Produto + " não pode ser utilizado.");
	  case sttINEXISTENTE:
	  default:
		 return("Não existe Licenca de Uso para o usuário.\nO programa " + Produto + " não pode ser utilizado.");
	  }
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TLicenca::PM_GetCPU_ID(void)
   {
   return(PD.cpu_id);
   }

//---------------------------------------------------------------------------
bool __fastcall TLicenca::PM_GetCtrlRemoto(void)
   {
   return(PD.ctrl_remoto);
   }

//---------------------------------------------------------------------------
TDate __fastcall TLicenca::PM_GetDH_alocada(void)
   {
   return(PD.dh_alocada);
   }

//---------------------------------------------------------------------------
TDate __fastcall TLicenca::PM_GetDH_controle(void)
   {
   return(PD.dh_controle);
   }

//---------------------------------------------------------------------------
TDate __fastcall TLicenca::PM_GetDH_inicio(void)
   {
   return(PD.dh_inicio);
   }

//---------------------------------------------------------------------------
TDate __fastcall TLicenca::PM_GetDH_termino(void)
   {
   return(PD.dh_termino);
   }

//---------------------------------------------------------------------------
int __fastcall TLicenca::PM_GetEmpresa(void)
   {
   return(PD.empresa);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TLicenca::PM_GetEmpresaStr(void)
   {
   //variaveis locais
   UnicodeString nome_empresa;
   VTEmpresa *empresa = NewObjEmpresa();

   //localiza nome da empresa
   nome_empresa = empresa->EmpresaAsStr(PD.empresa);
   delete empresa;
   if(nome_empresa.IsEmpty())
	  return("Não cadastrada");
   return(nome_empresa);
   }

//---------------------------------------------------------------------------
int __fastcall TLicenca::PM_GetID(void)
   {
   return(PD.id);
   }

//---------------------------------------------------------------------------
bool __fastcall TLicenca::PM_GetGoogle(void)
   {
   return(PD.google);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TLicenca::PM_GetLogin(void)
   {
   return(PD.login);
   }

//---------------------------------------------------------------------------
bool __fastcall TLicenca::PM_GetLoginLocal(void)
   {
   return(PD.login_local);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TLicenca::PM_GetPassword(void)
   {
   return(PD.password);
   }

//---------------------------------------------------------------------------
DynamicArray<int> __fastcall TLicenca::PM_GetOperacoes(void)
   {
   return(PD.operacoes);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TLicenca::PM_GetProduto(void)
   {
   return(PD.produto);
   }

//---------------------------------------------------------------------------
int __fastcall TLicenca::PM_GetRenovada(void)
   {
   return(PD.renovada);
   }

//---------------------------------------------------------------------------
int __fastcall TLicenca::PM_GetStatus(void)
   {
   //retorna status
   return(PD.status);
   }

//---------------------------------------------------------------------------
int __fastcall TLicenca::PM_GetTipo(void)
   {
   return(PD.tipo);
   }

//---------------------------------------------------------------------------
int __fastcall TLicenca::PM_GetValidade(void)
   {
   return(PD.validade);
   }

//---------------------------------------------------------------------------
void __fastcall TLicenca::PM_SetCPU_ID(AnsiString cpu_id)
   {
   PD.cpu_id = cpu_id;
   }

//---------------------------------------------------------------------------
void __fastcall TLicenca::PM_SetCtrlRemoto(bool ctrl_remoto)
   {
   PD.ctrl_remoto = ctrl_remoto;
   }

//---------------------------------------------------------------------------
void __fastcall TLicenca::PM_SetDH_alocada(TDate dh_alocada)
   {
   PD.dh_alocada = dh_alocada;
   }

//---------------------------------------------------------------------------
void __fastcall TLicenca::PM_SetDH_controle(TDate dh_controle)
   {
   //variáveis locais
   __int64 num_seg;

   //proteção contra tentativa do usuário atrasar o relógio do micro p/ burlar a Licenca
   if (dh_controle < PD.dh_controle)
      {//relógio do micro foi atrasado: redefine DH_inicio e DH_termino
      num_seg = SecondsBetween(dh_controle, PD.dh_controle);
      //proteção: num_seg PRECISA ser um valor negativo
      if (num_seg > 0) num_seg = -num_seg;
      //redefine DH_inicio e DH_termino
      IncSecond(PD.dh_inicio,  num_seg);
      IncSecond(PD.dh_termino, num_seg);
      }
   //salva nova data/hora de controle
   PD.dh_controle = dh_controle;
   }

//---------------------------------------------------------------------------
void __fastcall TLicenca::PM_SetDH_inicio(TDate dh_inicio)
   {
   PD.dh_inicio = dh_inicio;
   }

//---------------------------------------------------------------------------
void __fastcall TLicenca::PM_SetDH_termino(TDate dh_termino)
   {
   PD.dh_termino = dh_termino;
   }

//---------------------------------------------------------------------------
void __fastcall TLicenca::PM_SetEmpresa(int empresa)
   {
   PD.empresa = empresa;
   }

//---------------------------------------------------------------------------
void __fastcall TLicenca::PM_SetID(int id)
   {
   PD.id = id;
   }

//---------------------------------------------------------------------------
void __fastcall TLicenca::PM_SetGoogle(bool google)
   {
   PD.google = google;
   }

//---------------------------------------------------------------------------
void __fastcall TLicenca::PM_SetLogin(AnsiString login)
   {
   PD.login = login;
   }

//---------------------------------------------------------------------------
void __fastcall TLicenca::PM_SetLoginLocal(bool login_local)
   {
   PD.login_local = login_local;
   }

//---------------------------------------------------------------------------
void __fastcall TLicenca::PM_SetPassword(AnsiString password)
   {
   PD.password = password;
   }

//---------------------------------------------------------------------------
void __fastcall TLicenca::PM_SetOperacoes(DynamicArray<int> operacoes)
   {
   PD.operacoes = operacoes;
   }

//---------------------------------------------------------------------------
void __fastcall TLicenca::PM_SetProduto(AnsiString produto)
   {
   PD.produto = produto;
   }

//---------------------------------------------------------------------------
void __fastcall TLicenca::PM_SetRenovada(int renovada)
   {
   PD.renovada = renovada;
   }

//---------------------------------------------------------------------------
void __fastcall TLicenca::PM_SetStatus(int status)
   {
   PD.status = status;
   }

//---------------------------------------------------------------------------
void __fastcall TLicenca::PM_SetTipo(int tipo)
   {
   PD.tipo = tipo;
   }

//---------------------------------------------------------------------------
void __fastcall TLicenca::PM_SetValidade(int validade)
   {
   PD.validade = validade;
   }

//---------------------------------------------------------------------------
void __fastcall TLicenca::ReadFromPacote(VTPacote *pacote)
   {
   //variáveis locais
   int     campo_tipo, total_operacoes;
   VTCampo *campo;

   ID          = pacote->Campo("ID")->AsInteger;
   CPU_ID      = pacote->Campo("CPU_ID")->AsString;
   Produto     = pacote->Campo("Produto")->AsString;
   Empresa     = pacote->Campo("Empresa")->AsInteger;
   Login       = pacote->Campo("Login")->AsString;

   Password    = pacote->Campo("Password")->AsString;
   DH_alocada  = pacote->Campo("DH_alocada")->AsDate;
   DH_inicio   = pacote->Campo("DH_inicio")->AsDate;
   DH_termino  = pacote->Campo("DH_termino")->AsDate;
   DH_controle = pacote->Campo("DH_controle")->AsDate;
   Status      = pacote->Campo("Status")->AsInteger;
   //lê campo tipo com inormação do tipo da licenca e uso do Google
   campo_tipo  = pacote->Campo("Tipo")->AsInteger;
   Tipo        = campo_tipo & (~ tipoGOOGLE);
   Google      = ((campo_tipo & tipoGOOGLE) == tipoGOOGLE);
   Validade    = pacote->Campo("Validade")->AsInteger;
   Renovada    = pacote->Campo("Renovada")->AsInteger;
   //cuidado: o campo LoginLocal pode não estar disponível pacote
   campo = pacote->Campo("LoginLocal");
   if (campo) LoginLocal = campo->AsBoolean;
   else       LoginLocal = false;
   //cuidado: o campo CtrlRemoto pode não estar disponível pacote
   campo = pacote->Campo("CtrlRemoto");
   if (campo) CtrlRemoto = campo->AsBoolean;
   else       CtrlRemoto = false;
   //operacoes
   campo = pacote->Campo("TotalOperacoes");
   if(campo == NULL)
	  return;
   total_operacoes = campo->AsInteger;
   PD.operacoes.Length = total_operacoes;
   for(int n = 0; n < total_operacoes; n++)
	  {
	  PD.operacoes[n] = pacote->Campo("Op"+IntToStr(n))->AsInteger;
	  }
   }

//---------------------------------------------------------------------------
int __fastcall TLicenca::VerificaCPU_ID(AnsiString cpu_id)
   {
   //verificação do CPU_ID indisponivel em 64 bits (temporariamente)
   //se CPU_ID diferente, bloqueia a Licenca
//   if (CPU_ID.AnsiCompareIC(cpu_id) != 0)
//	  {
//	  Status = sttBLOQUEADA;
//	  }
   return(Status);
   }
/*
//---------------------------------------------------------------------------
int __fastcall TLicenca::VerificaPeriodoValidade(void)
   {
   //
   //OBS: o controle de período de validade é feito apenas p/ Licenca Demo
   //
   //verifica se status indica Licenca alocada
   if (Status == sttALOCADA)
     {//verifica se é uma Licença DEMO
     if (Tipo == tipoDEMO)
       {//atualiza data/hora de controle da Licenca e verifica término da validade
       DH_controle = DateOf(Now());
       if (DH_controle > DH_termino) Status = sttEXPIRADA;
       }
     }
   return(Status);
   }
*/
//---------------------------------------------------------------------------
int __fastcall TLicenca::VerificaPeriodoValidade(void)
   {
   //
   //OBS: o controle de período de validade é feito apenas p/ Licenca Demo
   //
   //verifica se status indica Licenca alocada
   if (Status == sttALOCADA)
     {//verifica se é uma Licença c/ período de validade
     if (Validade >= 0)
       {//atualiza data/hora de controle da Licenca e verifica término da validade
       DH_controle = DateOf(Now());
       if (DH_controle > DH_termino) Status = sttEXPIRADA;
       }
     }
   return(Status);
   }

//---------------------------------------------------------------------------
void __fastcall TLicenca::WriteToPacote(VTPacote *pacote)
   {
   //variáveis locais
   int campo_tipo;

   //define tipo da Licenca c/ informação de acesso ao Google
   campo_tipo = (Google) ? (Tipo | tipoGOOGLE) : (Tipo);
   //monta pacote
   pacote->Clear();
   pacote->Campo("ID")->AsInteger         = ID;
   pacote->Campo("CPU_ID")->AsString      = CPU_ID;
   pacote->Campo("Produto")->AsString     = Produto;
   pacote->Campo("Empresa")->AsInteger    = Empresa;
   pacote->Campo("Login")->AsString       = Login;
   pacote->Campo("Password")->AsString    = Password;
   pacote->Campo("DH_alocada")->AsDate    = DH_alocada;
   pacote->Campo("DH_inicio")->AsDate     = DH_inicio;
   pacote->Campo("DH_controle")->AsDate   = DH_controle;
   pacote->Campo("DH_termino")->AsDate    = DH_termino;
   pacote->Campo("Status")->AsInteger     = Status;
   pacote->Campo("Tipo")->AsInteger       = campo_tipo;
   pacote->Campo("Validade")->AsInteger   = Validade;
   pacote->Campo("Renovada")->AsInteger   = Renovada;
   pacote->Campo("LoginLocal")->AsBoolean = LoginLocal;
   pacote->Campo("CtrlRemoto")->AsBoolean = CtrlRemoto;
   //operacoes
   pacote->Campo("TotalOperacoes")->AsInteger = PD.operacoes.Length;
   for(int n = 0; n < PD.operacoes.Length; n++)
	  {
	  pacote->Campo("Op"+IntToStr(n))->AsInteger = PD.operacoes[n];
	  }
   }

//---------------------------------------------------------------------------
//eof
