//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include<DateUtils.hpp>
#include <PlataformaSinap\Fontes\Apl\VTApl.h>
#include <PlataformaSinap\DLL_Inc\Funcao.h>
#include "TFormClient.h"
#include "VTSinapClient.h"
#include "..\User\VTSinapUser.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormClient *FormClient;

//---------------------------------------------------------------------------
__fastcall TFormClient::TFormClient(TComponent* Owner, VTApl *apl_owner, TWinControl *parent)
                       : TForm(Owner)
   {
   //NÃO insere Form em seu Parent porque há campos editáveis que ficam desabilitados
   //if (parent != NULL) Parent = parent;
   //cria objeto Apl
   apl = NewObjApl(this, apl_owner);
	//cria objetos VTSinapUser e VTSinapClient
	apl->Add(user   = NewObjSinapUser(apl));
	apl->Add(client = NewObjSinapClient(apl));
   //habilita log do objeto SinapUser
	user->HabilitaLog(RichEditRx->Lines, RichEditTx->Lines, NULL);
   //garante que o SinapClient esteja desconectado
	ActionStop->Execute();
	//inicia controle de supervisão automática
	supervisao_auto = false;
   //inicia CBoxIP
   CBoxIP_Inicia();
   //configura Actions de falta
   ActionSimulaFalta->Enabled  = true;
   ActionEliminaFalta->Enabled = false;
   }

//---------------------------------------------------------------------------
__fastcall TFormClient::~TFormClient(void)
   {
   //desconecta ClientSocket
   ActionStop->Execute();
   //destrói objeto Apl
   if (apl) {delete apl; apl = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormClient::ActionCapacitorOffExecute(TObject *Sender)
	{
	AlteraCapacitorEstado(false);
	}

//---------------------------------------------------------------------------
void __fastcall TFormClient::ActionCapacitorOnExecute(TObject *Sender)
	{
	AlteraCapacitorEstado(true);
	}

//---------------------------------------------------------------------------
void __fastcall TFormClient::ActionChaveAbreExecute(TObject *Sender)
	{
	ManobraChave(true);
	}

//---------------------------------------------------------------------------
void __fastcall TFormClient::ActionChaveFechaExecute(TObject *Sender)
   {
   ManobraChave(false);
   }

//---------------------------------------------------------------------------
void __fastcall TFormClient::ActionClearLogExecute(TObject *Sender)
   {
   RichEditRx->Lines->Clear();
   RichEditTx->Lines->Clear();
   }

//---------------------------------------------------------------------------
void __fastcall TFormClient::ActionEliminaFaltaExecute(TObject *Sender)
   {
   //variáveis locais
   strHMS hms;
   VTMsg *msg;

   try{//verifica se conexão ativa
      if (! client->Ativo)
         {
         Aviso("Não existe conexão com o servidor");
         return;
         }
      //verifica se há uma falta ativa
      if (ActionSimulaFalta->Enabled)
         {
         Aviso("Não existe uma falta ativa");
         return;
         }
      //solicita operação ao SinapClient
      if (! client->ResetFalta())
         {
         Aviso("Erro na operação de 'Elimina Falta'");
         return;
         }
      //configura Actions de falta
      ActionSimulaFalta->Enabled  = true;
      ActionEliminaFalta->Enabled = false;
      }catch(Exception &e)
         {
         Aviso("Erro na criação da mensagem 'Elimina Falta'");
         }
	}
/*
//---------------------------------------------------------------------------
void __fastcall TFormClient::ActionSimulaFaltaExecute(TObject *Sender)
	{
	Aviso("SimulaFalta - função bloqueada");
	}
*/

//---------------------------------------------------------------------------
void __fastcall TFormClient::ActionSimulaFaltaExecute(TObject *Sender)
   {
   //variáveis locais
	bool       pre_falta = false;
	int        eqpto_tipo, tipo_falta;
	AnsiString eqpto_cod;
	double     dist_bar1_pu, rdef_ohm;
	strHMS     hms;
	VTMsg      *msg;

   try{//verifica se conexão ativa
      if (! client->Ativo)
         {
         Aviso("Não existe conexão com o servidor");
         return;
         }
      //verifica se existe uma falta ativa
      if (ActionEliminaFalta->Enabled)
         {
         Aviso("Já existe uma falta simulada");
         return;
         }
      //verifica se o tipo de equipamento está selecionado
      if (CBoxEqptoTipo->ItemIndex < 0)
         {
         Aviso("Selecione o tipo de equipamento");
         return;
         }
		//determina código do Eqpto
		if (EditEqptoCodigo->Text.IsEmpty())
         {
			Aviso("Defina o código do equipamento");
         return;
         }
      //determina configuração da falta
      eqpto_tipo = CBoxEqptoTipo->ItemIndex + 1;
		eqpto_cod  = EditEqptoCodigo->Text;
      tipo_falta = falta3F;
      rdef_ohm   = 0.;
      //verifica se Eqpto selecionado é Barra ou Trecho
      if (eqpto_tipo == teBARRA)
         {
         dist_bar1_pu = 0.;
         }
      else if (eqpto_tipo == teTRECHO)
         {
         dist_bar1_pu = 0.5;
         }
      else
         {
         Aviso("Selecione uma Barra ou Trecho para simulação de falha");
         return;
         }
      //solicita operação ao SinapClient
		if (! client->SetFalta(eqpto_tipo, eqpto_cod, tipo_falta, rdef_ohm,
                             dist_bar1_pu, pre_falta, DateTimePicker->DateTime))
         {
         Aviso("Erro na operação de 'Simulação de Falta'");
         return;
         }
      //configura Actions de falta
      ActionSimulaFalta->Enabled  = false;
      ActionEliminaFalta->Enabled = true;
      }catch(Exception &e)
         {
         Aviso("Erro na operação de 'Simulação de Falta'");
         }
   }

//---------------------------------------------------------------------------
void __fastcall TFormClient::ActionSolicitaRegistroExecute(TObject *Sender)
   {
	//variáveis locais
	int        eqpto_tipo;
	AnsiString eqpto_cod;
	strHMS     hms;
	VTMsg     *msg;

   try{//verifica se conexão ativa
      if (! client->Ativo)
         {
         Aviso("Não existe conexão com o servidor");
         return;
         }
      //verifica se o tipo de equipamento está selecionado
      if (CBoxEqptoTipo->ItemIndex < 0)
         {
         Aviso("Selecione o tipo de equipamento");
         return;
         }
		//determina código do Eqpto
		if (EditEqptoCodigo->Text.IsEmpty())
         {
			Aviso("Defina o código do equipamento");
         return;
         }
      //solicita operação ao SinapClient
      eqpto_tipo = CBoxEqptoTipo->ItemIndex + 1;
		eqpto_cod = EditEqptoCodigo->Text;
		if (! client->GetRegistro(eqpto_tipo, eqpto_cod, DateTimePicker->DateTime))
         {
         Aviso("Erro na operação de 'Solicitação de Registro'");
         return;
         }
      }catch(Exception &e)
         {
         Aviso("Erro na operação de 'Solicitação de Registro'");
         }
   }


//---------------------------------------------------------------------------
void __fastcall TFormClient::ActionStartExecute(TObject *Sender)
   {
   try{//verifica se foi selecionado um endereço de IP
      if (CBoxIP->Text.IsEmpty())
         {
         Aviso("Indique o endereço de IP do Servidor");
         return;
         }
      //garante que o ClientSocket esteja inativo
      client->Desconecta();
      //configura SinapClient
      //client->HabilitaLog(RichEditRx->Lines, RichEditTx->Lines);
      client->Conecta(CBoxIP->Text, SERVER_PORT);
      //indica que a conexão está ativa
      StatusBar->SimpleText = "Ativo";
      }catch(Exception &e)
         {
         Aviso("Erro ao iniciar conexão");
         }
   }

//---------------------------------------------------------------------------
void __fastcall TFormClient::ActionStopExecute(TObject *Sender)
   {
   //cancela conexão
   client->Desconecta();
   //atualiza StatusBar
   if (client->Ativo)
      {
      Aviso("A conexão continua ativa");
      StatusBar->SimpleText = "Ativo";
      }
   else
      {
      StatusBar->SimpleText = "Inativo";
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormClient::ActionSupervisaoAutoExecute(TObject *Sender)
	{
	try{//verifica se conexão ativa
		if (! client->Ativo)
			{
			Aviso("Não existe conexão com o servidor");
			return;
			}
		//habilita/desabilita supervisão automática
		supervisao_auto = ! supervisao_auto;
		//envia comando
		client->SetSupervisaoAutomatica(supervisao_auto);
		//altera Caption da Action
		if (supervisao_auto) ActionSupervisaoAuto->Caption = "Desativar Supervisão Automática";
		else                 ActionSupervisaoAuto->Caption = "Ativar Supervisão Automática";
		}catch(Exception &e)
			{
			Aviso("Erro na configuração de 'Supervisão Automática'");
			}
	}
//---------------------------------------------------------------------------
void __fastcall TFormClient::AlteraCapacitorEstado(bool ligado)
	{
	//variáveis locais
	int        eqpto_tipo;
	AnsiString eqpto_cod;

	try{//verifica se conexão ativa
		if (! client->Ativo)
			{
			Aviso("Não existe conexão com o servidor");
			return;
			}
		//verifica se o tipo de Eqpto é Capacitor
		if ((eqpto_tipo = CBoxEqptoTipo->ItemIndex + 1) != teCAPACITOR)
			{
			Aviso("O tipo de equipamento deve ser 'capacitor'");
			return;
			}
		//determina código do Eqpto
		if (EditEqptoCodigo->Text.IsEmpty())
			{
			Aviso("Defina o código do capacitor");
			return;
			}
		//monta mensagem de solicitação de registro
		eqpto_tipo = CBoxEqptoTipo->ItemIndex + 1;
		eqpto_cod  = EditEqptoCodigo->Text;
		if (! client->SetCapacitorEstado(eqpto_tipo, eqpto_cod, ligado, Now()))
			{
			Aviso("Erro na operação de 'Alteração de Capacitor'");
			return;
			}
		}catch(Exception &e)
			{
			Aviso("Erro na operação de 'Alteração de Capacitor'");
			}
	}

//---------------------------------------------------------------------------
void __fastcall TFormClient::ActionTrafoDecTapExecute(TObject *Sender)
	{
	AlteraTrafoTap(-1);
	}

//---------------------------------------------------------------------------
void __fastcall TFormClient::ActionTrafoIncTapExecute(TObject *Sender)
	{
	AlteraTrafoTap(1);
	}

//---------------------------------------------------------------------------
void __fastcall TFormClient::AlteraTrafoTap(int variacao_tap)
	{
	//variáveis locais
	int        eqpto_tipo;
	AnsiString eqpto_cod;

	try{//verifica se conexão ativa
		if (! client->Ativo)
			{
			Aviso("Não existe conexão com o servidor");
			return;
			}
		//verifica se o tipo de Eqpto é chave
		if ((eqpto_tipo = CBoxEqptoTipo->ItemIndex + 1) != teTRANSFORMADOR)
			{
			Aviso("O tipo de equipamento deve ser 'Transformador'");
			return;
			}
		//determina código do Eqpto
		if (EditEqptoCodigo->Text.IsEmpty())
			{
			Aviso("Defina o código do transformador");
			return;
			}
		//monta mensagem de alteração de trafo
		eqpto_tipo = CBoxEqptoTipo->ItemIndex + 1;
		eqpto_cod  = EditEqptoCodigo->Text;
		if (! client->SetTrafoTap(eqpto_tipo, eqpto_cod, variacao_tap, Now()))
			{
			Aviso("Erro na operação de 'Alteração de Trafo'");
			return;
			}
		}catch(Exception &e)
			{
			Aviso("Erro na operação de 'Alteração de Trafo'");
			}
	}

//---------------------------------------------------------------------------
void __fastcall TFormClient::CBoxIP_Inicia(void)
	{
	//variáveis locais
	char               szHostName[1024] = "";
	struct sockaddr_in SocketAddress;
	struct hostent     *pHost        = 0;
	WSADATA            WSAData;

	//reinicia CBoxIP_Inicia
   CBoxIP->Clear();
   // Initialize winsock dll
   if (WSAStartup(MAKEWORD(1, 0), &WSAData) != 0)
      {//erro
      return;
      }
   if (::gethostname(szHostName, sizeof(szHostName)) != 0)
      {//erro
      return;
      }
   // Get local IP addresses
   if ((pHost = ::gethostbyname(szHostName)) == NULL)
      {//erro
      return;
      }
   for(int iCnt = 0; ((pHost->h_addr_list[iCnt]) && (iCnt < 10)); ++iCnt)
      {
      memcpy(&SocketAddress.sin_addr, pHost->h_addr_list[iCnt], pHost->h_length);
      CBoxIP->Items->Add(inet_ntoa(SocketAddress.sin_addr));
      }
   // Cleanup
   WSACleanup();
   //pré-seleciona primeiro IP
   if (CBoxIP->Items->Count > 0) CBoxIP->ItemIndex = 0;
   }

//---------------------------------------------------------------------------
void __fastcall TFormClient::CheckBoxTempoRealClick(TObject *Sender)
   {
   //habilita/desabilita o Timer
   Timer->Enabled = CheckBoxTempoReal->Checked;
   }

//---------------------------------------------------------------------------
void __fastcall TFormClient::ManobraChave(bool abrir)
	{
	//variáveis locais
	int        eqpto_tipo;
	AnsiString eqpto_cod;

	try{//verifica se conexão ativa
		if (! client->Ativo)
			{
			Aviso("Não existe conexão com o servidor");
			return;
			}
		//verifica se o tipo de Eqpto é chave
		if ((eqpto_tipo = CBoxEqptoTipo->ItemIndex + 1) != teCHAVE)
			{
			Aviso("O tipo de equipamento deve ser 'chave'");
			return;
			}
		//determina código do Eqpto
		if (EditEqptoCodigo->Text.IsEmpty())
			{
			Aviso("Defina o código da chave");
			return;
			}
		//monta mensagem de solicitação de registro
		eqpto_tipo = CBoxEqptoTipo->ItemIndex + 1;
		eqpto_cod  = EditEqptoCodigo->Text;
		if (! client->SetManobraChave(eqpto_tipo, eqpto_cod, abrir, Now()))
			{
			Aviso("Erro na operação de 'Manobra de Chave'");
			return;
			}
		}catch(Exception &e)
			{
			Aviso("Erro na operação de 'Manobra de Chave'");
			}
	}

//---------------------------------------------------------------------------
void __fastcall TFormClient::TimerTimer(TObject *Sender)
	{
	//atualiza DateTimePicker
	DateTimePicker->DateTime = Now();
	}

//---------------------------------------------------------------------------
//eof


