//-----------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include<DateUtils.hpp>
#include <PlataformaSinap\Fontes\Apl\VTApl.h>
#include <PlataformaSinap\DLL_Inc\Funcao.h>
#include "TSinapClient.h"
#include "TEqp.h"
#include "..\Protocolo\VTMsg.h"
#include "..\Protocolo\TMsgExisteEqpto.h"
#include "..\Protocolo\TMsgRegistroCapacitor.h"
#include "..\Protocolo\TMsgRegistroChave.h"
#include "..\Protocolo\TMsgRegistroTrafo.h"
#include "..\Protocolo\VTProtocolo.h"
#include "..\User\VTSinapUser.h"

//-----------------------------------------------------------------------------
VTSinapClient* __fastcall NewObjSinapClient(VTApl *apl_owner)
   {
   try{
      return(new TSinapClient(apl_owner));
      }catch(Exception &e)
         {//nada a fazer
         }
   //erro na criação do objeto
   return(NULL);
   }

//-----------------------------------------------------------------------------
__fastcall TSinapClient::TSinapClient(VTApl *apl_owner)
   {
   //salva ponteiro p/ objetos
   this->apl = apl_owner;
   user      = (VTSinapUser*)apl->GetObject(__classid(VTSinapUser));
   //cria objetos
   lisEQP       = new TList();
   lisMSG       = new TList();
   protocolo    = NewObjProtocolo();
   ClientSocket = new TClientSocket(NULL);
   //configura tratamento de eventos do ClientSocket
   ClientSocket->OnConnect    = ClientSocketConnect;
   ClientSocket->OnDisconnect = ClientSocketDisconnect;
   ClientSocket->OnError      = ClientSocketError;
   ClientSocket->OnRead       = ClientSocketRead;
   //ClientSocket->OnWrite      = ;
   }

//-----------------------------------------------------------------------------
__fastcall TSinapClient::~TSinapClient(void)
   {
   //cancela conexão
   Desconecta();
   //destrói objetos
   if (protocolo)    {delete protocolo;    protocolo    = NULL;}
   if (ClientSocket) {delete ClientSocket; ClientSocket = NULL;}
   //destró lista e seus objetos
   if (lisEQP) {LimpaTList(lisEQP); delete lisEQP; lisEQP = NULL;}
   if (lisMSG) {LimpaTList(lisMSG); delete lisMSG; lisMSG = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TSinapClient::ClientSocketConnect(TObject *Sender, TCustomWinSocket *Socket)
   {
   LogRx("Connect to: " + Socket->RemoteHost);
   }

//---------------------------------------------------------------------------
void __fastcall TSinapClient::ClientSocketDisconnect(TObject *Sender, TCustomWinSocket *Socket)
   {
   LogRx("Disconnect from: " + Socket->RemoteHost);
   }

//---------------------------------------------------------------------------
void __fastcall TSinapClient::ClientSocketError(TObject *Sender,
      TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &ErrorCode)
   {
   LogRx("ClientSocketError: ErrorCode=" + IntToStr(ErrorCode));
   }

//---------------------------------------------------------------------------
void __fastcall TSinapClient::ClientSocketRead(TObject *Sender, TCustomWinSocket *Socket)
   {
   //variáveis locais
   int    length;
   char   buffer[MAX_MSG_LEN];
   strBUF buf;
   VTMsg *msg;

   //atualiza LogRx
   length = sizeof(buffer);
   length = Socket->ReceiveBuf(buffer, length);
   //monta mensagem recebida
   buf.memory = buffer;
   buf.length = length;
   //identifica a Msgs recebidas
   if (protocolo->MontaMsgRecebida(&buf, lisMSG) == 0)
      {
      LogRx("Mensagem recebida não pode ser identificada");
      return;
      }
   //loop p/ todas Msg recebidas
   for (int n = 0; n < lisMSG->Count; n++)
      {
      msg = (VTMsg*)lisMSG->Items[n];
      //atualiza Log
      LogRx(msg->MsgAsString());
      //verifica se é mensagem com identificação de Eqpto
      if (msg->Funcao == funcaoEXISTE_EQPTO)
         {//envia mensagem que estava à espera da identificação do Eqpto
         TrataRespostaExisteEqpto(msg);
         }
      else
         {//envia Msg para SinapUser
         EnviaMsgToUser(msg);
         }
      }
   //destrói todas Msg recebidas em lisMSG
   LimpaTList(lisMSG);
   }

//---------------------------------------------------------------------------
bool __fastcall TSinapClient::Conecta(AnsiString ip_address, int server_port)
   {
   try{//proteção: se necessário, fecha conexão atual
      Desconecta();
      //inicia conexção
      ClientSocket->ClientType = ctNonBlocking;   // ctBlocking;
      ClientSocket->Host       = ip_address;
      ClientSocket->Port       = server_port;
      //ativa o ClientSocket
      ClientSocket->Open();
      }catch(Exception &e)
         {
         return(false);
         }
   return(Ativo);
   }

//---------------------------------------------------------------------------
void __fastcall TSinapClient::Desconecta(void)
   {
  //cancela conexão
   if (ClientSocket->Active) ClientSocket->Close();
    }

//---------------------------------------------------------------------------
bool __fastcall TSinapClient::EnviaMensagem(VTMsg *msg)
	{
   //variáveis locais
   strBUF *buf = msg->ConverteMsgToBuf();

   ClientSocket->Socket->SendBuf(buf->memory, buf->length);
   //atualiza LogTx
   LogTx(msg->MsgAsString());
   //destrói mensagem enviada
   delete msg;

	return(true);
   }
//---------------------------------------------------------------------------
bool __fastcall TSinapClient::EnviaAtributosCapacitorToUser(AnsiString eqp_cod, TMsgRegistroCapacitor *msg)
   {
	//envia etado
	user->SinapCapacitorEstado(eqp_cod, msg->pcte.estado);
	//envia potência reativa
	user->SinapCapacitorQmvar(eqp_cod, msg->pcte.q_mvar);
	return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TSinapClient::EnviaAtributosChaveToUser(AnsiString eqp_cod, TMsgRegistroChave *msg)
	{
	//variáveis locais
	int     vet_fase[MAX_FASE] = {faseA, faseB, faseC, faseN};
	double  i_amp, vfn_kv;

	//envia correntes da chave
	for (int ind_fase = indFASE_A; ind_fase <= indFASE_C; ind_fase++)
		{
		i_amp = msg->pcte.chv_if[ind_fase].mod_amp;
		user->SinapChaveCorrente(eqp_cod, vet_fase[ind_fase], i_amp);
		}
	//envia tensões da chave
	for (int ind_fase = indFASE_A; ind_fase <= indFASE_C; ind_fase++)
		{
		vfn_kv = msg->pcte.bar1_vf[ind_fase].mod_volt * 0.001;
		user->SinapChaveTensao(eqp_cod, vet_fase[ind_fase], vfn_kv);
		}
	//envia potência ativa e reativa trifásica
	user->SinapChaveS3f_mva(eqp_cod, msg->pcte.p3f_mw, msg->pcte.q3f_mvar);
	//envia potência ativa e reativa por fase
	user->SinapChaveS1f_mva(eqp_cod, faseA, msg->pcte.pa_mw, msg->pcte.qa_mvar);
	user->SinapChaveS1f_mva(eqp_cod, faseB, msg->pcte.pb_mw, msg->pcte.qb_mvar);
	user->SinapChaveS1f_mva(eqp_cod, faseC, msg->pcte.pc_mw, msg->pcte.qc_mvar);
	//envia estado da chave
	user->SinapChaveEstado(eqp_cod, msg->pcte.estado);
	//envia cbr, cbtl e lockout
	user->SinapChaveCBR(eqp_cod, msg->pcte.cbr);
	user->SinapChaveCBTL(eqp_cod, msg->pcte.cbtl);
	user->SinapChaveLockout(eqp_cod, msg->pcte.lockout);
	return(true);
	}

//---------------------------------------------------------------------------
bool __fastcall TSinapClient::EnviaAtributosTrafoToUser(AnsiString eqp_cod, TMsgRegistroTrafo *msg)
	{
	//variáveis locais
	int     vet_fase[MAX_FASE] = {faseA, faseB, faseC, faseN};
	double  i_amp, vfn_kv;

	//envia tap do Trafo
	user->SinapTrafoTap(eqp_cod, msg->pcte.tap);
	//envia correntes do primário do Trafo
	for (int ind_fase = indFASE_A; ind_fase <= indFASE_C; ind_fase++)
		{
		i_amp = msg->pcte.pri_if[ind_fase].mod_amp;
		user->SinapTrafoCorrente(eqp_cod, 1, vet_fase[ind_fase], i_amp);
		}
	//envia correntes do secundário do Trafo
	for (int ind_fase = indFASE_A; ind_fase <= indFASE_C; ind_fase++)
		{
		i_amp = msg->pcte.pri_if[ind_fase].mod_amp;
		user->SinapTrafoCorrente(eqp_cod, 2, vet_fase[ind_fase], i_amp);
		}
	//envia tensões do primário Trafo
	for (int ind_fase = indFASE_A; ind_fase <= indFASE_C; ind_fase++)
		{
		vfn_kv = msg->pcte.bar1_vf[ind_fase].mod_volt * 0.001;
		user->SinapTrafoTensao(eqp_cod, 1, vet_fase[ind_fase], vfn_kv);
		}
	//envia tensões do secundário Trafo
	for (int ind_fase = indFASE_A; ind_fase <= indFASE_C; ind_fase++)
		{
		vfn_kv = msg->pcte.bar2_vf[ind_fase].mod_volt * 0.001;
		user->SinapTrafoTensao(eqp_cod, 2, vet_fase[ind_fase], vfn_kv);
		}
	return(true);
	}

//---------------------------------------------------------------------------
bool __fastcall TSinapClient::EnviaMsgToUser(VTMsg *msg)
	{
	//variáveis locais
	TEqp              *eqp;
	TMsgRegistroChave *reg_chv;

   //verifica se foi definido objeto VTUser
   if (user)
      {//determina Eqp pelo ID do Eqpto indicado na msg
      if ((eqp = ExisteEqp(msg->EqptoTipo, msg->EqptoId)) != NULL)
         {//envia mensagem para VTUser
			user->TrataMsg(eqp->codigo, msg);
			//verifica o tipo da mensagem
         if (msg->Funcao == funcaoENVIO_REGISTRO)
            {
				if (msg->EqptoTipo == teCHAVE)
					{//transfere todos os atributos da Chave
					EnviaAtributosChaveToUser(eqp->codigo, (TMsgRegistroChave*)msg);
					}
				else if (msg->EqptoTipo == teTRANSFORMADOR)
					{//transfere todos os atributos do Trafo
					EnviaAtributosTrafoToUser(eqp->codigo, (TMsgRegistroTrafo*)msg);
					}
				else if (msg->EqptoTipo == teCAPACITOR)
					{//transfere todos os atributos do Trafo
					EnviaAtributosCapacitorToUser(eqp->codigo, (TMsgRegistroCapacitor*)msg);
					}
				}
			}
		}
	return(true);
	}

//-----------------------------------------------------------------------------
TEqp* __fastcall TSinapClient::ExisteEqp(int eqpto_tipo,int eqpto_id)
   {
   //variáveis locais
   TEqp *eqp;

   for (int n = 0; n < lisEQP->Count; n++)
      {
      eqp = (TEqp*)lisEQP->Items[n];
      if ((eqp->tipo == eqpto_tipo)&&(eqp->id == eqpto_id)) return(eqp);
      }
   return(NULL);
   }

//-----------------------------------------------------------------------------
TEqp* __fastcall TSinapClient::ExisteEqp(int eqpto_tipo, AnsiString codigo)
   {
   //variáveis locais
	TEqp *eqp;

   for (int n = 0; n < lisEQP->Count; n++)
      {
      eqp = (TEqp*)lisEQP->Items[n];
      if ((eqp->tipo == eqpto_tipo)&&(eqp->codigo.AnsiCompareIC(codigo) == 0)) return(eqp);
      }
   return(NULL);
   }

//-----------------------------------------------------------------------------
bool __fastcall TSinapClient::GetEqp(int eqpto_tipo, AnsiString codigo, TDateTime dh)
   {
   //variáveis locais
   VTMsg     *msg;
   bool      sucesso;
   strHMS    hms;
   int       eqpto_id = 0;

   try{//verifica se conexão ativa
      if (! ClientSocket->Active) return(false);
      //monta mensagem de solicitação de eqpto
      IniciaStrHMS(dh, hms);
      msg = protocolo->MontaMsgExisteEqpto(eqpto_tipo, eqpto_id, codigo, &hms);
      if (msg == NULL) return(false);
      //envia mensagem para o Server
      sucesso = EnviaMensagem(msg);
      //destrói mensagem enviada
      //delete msg;
      }catch(Exception &e)
         {
         sucesso = false;
         }
   return(sucesso);
   }

//-----------------------------------------------------------------------------
bool __fastcall TSinapClient::GetRegistro(int eqpto_tipo, int eqpto_id, TDateTime dh)
   {
   //variáveis locais
   bool   sucesso;
   strHMS hms;
   VTMsg *msg;

   try{//verifica se conexão ativa
      if (! ClientSocket->Active) return(false);
      //monta mensagem de solicitação de eqpto
      IniciaStrHMS(dh, hms);
		msg = protocolo->MontaMsgSolicitaRegistro(eqpto_tipo, eqpto_id, &hms);
      if (msg == NULL) return(false);
      //envia mensagem para o Server
      sucesso = EnviaMensagem(msg);
//TESTES
//msg = protocolo->MontaMsgSolicitaRegistro(eqpto_tipo, eqpto_id, &hms);
//sucesso = EnviaMensagem(msg);
      //destrói mensagem enviada
      //delete msg;
      }catch(Exception &e)
         {
         sucesso = false;
         }
   return(sucesso);
   }
/*
//-----------------------------------------------------------------------------
bool __fastcall TSinapClient:: GetRegistro(int eqpto_tipo, AnsiString eqpto_cod,  TDateTime dh)
   {
   //variáveis locais
   strHMS hms;
   TEqp  *eqp;
   VTMsg *msg;

   try{//verifica se conexão ativa
      if (! ClientSocket->Active) return(false);
      //verifica se o Eqp já existe em lisEQP
      if ((eqp = ExisteEqp(eqpto_tipo, eqpto_cod)) != NULL)
         {//verifica se o Eqp já tem o ID definido
         if (eqp->enabled)
            {//obtém estado da Chave, usando seu Id
            return(GetRegistro(eqp->tipo, eqp->id, dh));
            }
         }
      //cria novo eqp se ainda não existir
      if (eqp == NULL)
         {//cria novo Eqp e insere na lista
         lisEQP->Add(eqp = new TEqp(eqpto_tipo, eqpto_cod));
         }
      //cria mensagem que será enviada quando o ID do eqpto for recebido
      IniciaStrHMS(Now(), hms);
      msg = protocolo->MontaMsgSolicitaRegistro(eqp->tipo, eqp->id, &hms);
      if (msg == NULL) return(false);
      //insere Msg em Eqp
      eqp->InsereMsg(msg);
      //envia Msg solicitando Id do Eqpto
      GetEqp(eqpto_tipo, eqpto_cod);
      }catch(Exception &e)
         {
         return(false);
         }
   return(true);
   }
*/
//-----------------------------------------------------------------------------
bool __fastcall TSinapClient:: GetRegistro(int eqpto_tipo, AnsiString eqpto_cod,  TDateTime dh)
   {
   //variáveis locais
   strHMS hms;
   TEqp  *eqp;
   VTMsg *msg;

   try{//verifica se conexão ativa
      if (! ClientSocket->Active) return(false);
      //verifica se o Eqp já existe em lisEQP
      if ((eqp = ExisteEqp(eqpto_tipo, eqpto_cod)) == NULL)
			{//cria novo Eqp e insere na lista
			lisEQP->Add(eqp = new TEqp(eqpto_tipo, eqpto_cod));
			}
		//verifica se o Eqp já tem o ID definido
      if (eqp->enabled)
         {//obtém estado da Chave, usando seu Id
         return(GetRegistro(eqp->tipo, eqp->id, dh));
         }
      //cria mensagem que será enviada quando o ID do eqpto for recebido
      IniciaStrHMS(dh, hms);
      msg = protocolo->MontaMsgSolicitaRegistro(eqp->tipo, eqp->id, &hms);
      if (msg == NULL) return(false);
      //insere Msg em Eqp
      eqp->InsereMsg(msg);
      //envia Msg solicitando Id do Eqpto
      GetEqp(eqpto_tipo, eqpto_cod, dh);
      }catch(Exception &e)
         {
         return(false);
         }
   return(true);
   }


//-----------------------------------------------------------------------------
void __fastcall TSinapClient::IniciaStrHMS(TDateTime datetime, strHMS &hms)
   {
   hms.hora    = HourOf(datetime);
   hms.minuto  = MinuteOf(datetime);
   hms.segundo = SecondOf(datetime);
   }

//---------------------------------------------------------------------------
void __fastcall TSinapClient::LogRx(AnsiString txt)
   {
   if (user) user->LogRx(txt);
   }

//---------------------------------------------------------------------------
void __fastcall TSinapClient::LogTx(AnsiString txt)
   {
   if (user) user->LogTx(txt);
   }

//-----------------------------------------------------------------------------
bool __fastcall TSinapClient::PM_GetAtivo(void)
   {
   return(ClientSocket->Active);
   }

//---------------------------------------------------------------------------
bool __fastcall TSinapClient::ResetFalta(void)
	{
	//variáveis locais
	bool   sucesso;
	strHMS hms;
	VTMsg *msg;

	try{//verifica se conexão ativa
		if (! ClientSocket->Active) return(false);
		//monta mensagem de solicitação de eqpto
		IniciaStrHMS(Now(), hms);
		msg = protocolo->MontaMsgEliminaFalta(&hms);
		if (msg == NULL) return(false);
		//envia mensagem para o Server
		sucesso = EnviaMensagem(msg);
		//destrói mensagem enviada
		//delete msg;
		}catch(Exception &e)
			{
			sucesso = false;
			}
	return(sucesso);
	}

//-----------------------------------------------------------------------------
bool __fastcall TSinapClient::SetCapacitorEstado(int eqpto_tipo, AnsiString eqpto_cod, bool ligado, TDateTime dh)
	{
	//variáveis locais
	strHMS hms;
	TEqp  *eqp;
	VTMsg *msg;

	try{//verifica se conexão ativa
		if (! ClientSocket->Active) return(false);
		//verifica se o Eqp já existe em lisEQP
		if ((eqp = ExisteEqp(eqpto_tipo, eqpto_cod)) == NULL)
			{//cria novo Eqp e insere na lista
			lisEQP->Add(eqp = new TEqp(eqpto_tipo, eqpto_cod));
			}
		//verifica se o Eqp já tem o ID definido
		if (eqp->enabled)
			{//atualiza estado do Capacitor, usando seu Id
			return(SetCapacitorEstado(eqp->tipo, eqp->id, ligado, dh));
			}
	  //cria mensagem que será enviada quando o ID do eqpto for recebido
		IniciaStrHMS(dh, hms);
		msg = protocolo->MontaMsgAlteraCapacitor(eqpto_tipo, eqp->id, &hms, ligado);
		if (msg == NULL) return(false);
		//insere Msg em Eqp
		eqp->InsereMsg(msg);
		//envia Msg solicitando Id do Eqpto
		GetEqp(eqpto_tipo, eqpto_cod, dh);
		}catch(Exception &e)
			{
			return(false);
			}
	return(true);
	}

//-----------------------------------------------------------------------------
bool __fastcall TSinapClient::SetCapacitorEstado(int eqpto_tipo, int eqpto_id, bool ligado, TDateTime dh)
	{
	//variáveis locais
	bool   sucesso;
	strHMS hms;
	VTMsg *msg;

	try{//verifica se conexão ativa
		if (! ClientSocket->Active) return(false);
		//monta mensagem de solicitação de eqpto
		IniciaStrHMS(dh, hms);
		msg = protocolo->MontaMsgAlteraCapacitor(eqpto_tipo, eqpto_id, &hms, ligado);
		if (msg == NULL) return(false);
		//envia mensagem para o Server
		sucesso = EnviaMensagem(msg);
		//destrói mensagem enviada
		//delete msg;
		}catch(Exception &e)
			{
			sucesso = false;
			}
	return(sucesso);
	}

//-----------------------------------------------------------------------------
bool __fastcall TSinapClient::SetFalta(int eqpto_tipo, AnsiString eqpto_cod, int tipo_falta, double rdef_ohm,
													double dist_bar1_pu, bool pre_falta, TDateTime dh)
	{
	//variáveis locais
	strHMS hms;
	TEqp  *eqp;
	VTMsg *msg;

	try{//verifica se conexão ativa
		if (! ClientSocket->Active) return(false);
		//verifica se o Eqp já existe em lisEQP
		if ((eqp = ExisteEqp(eqpto_tipo, eqpto_cod)) == NULL)
			{//cria novo Eqp e insere na lista
			lisEQP->Add(eqp = new TEqp(eqpto_tipo, eqpto_cod));
			}
		//verifica se o Eqp já tem o ID definido
		if (eqp->enabled)
			{//atualiza estado da Chave, usando seu Id
			return(SetFalta(eqpto_tipo, eqp->id, tipo_falta, rdef_ohm, dist_bar1_pu, pre_falta, dh));
			}
	  //cria mensagem que será enviada quando o ID do eqpto for recebido
		IniciaStrHMS(dh, hms);
		msg = protocolo->MontaMsgSimulaFalta(eqpto_tipo, eqp->id, tipo_falta, rdef_ohm, dist_bar1_pu, pre_falta, &hms);
		if (msg == NULL) return(false);
		//insere Msg em Eqp
		eqp->InsereMsg(msg);
		//envia Msg solicitando Id do Eqpto
		GetEqp(eqpto_tipo, eqpto_cod, dh);
		}catch(Exception &e)
			{
			return(false);
			}
	return(true);
	}

//-----------------------------------------------------------------------------
bool __fastcall TSinapClient::SetFalta(int eqpto_tipo, int eqpto_id, int tipo_falta, double rdef_ohm,
													double dist_bar1_pu, bool pre_falta, TDateTime dh)
	{
	//variáveis locais
	bool   sucesso;
	strHMS hms;
	VTMsg *msg;

	try{//verifica se conexão ativa
		if (! ClientSocket->Active) return(false);
		//monta mensagem de solicitação de eqpto
		IniciaStrHMS(dh, hms);
		msg = protocolo->MontaMsgSimulaFalta(eqpto_tipo, eqpto_id, tipo_falta, rdef_ohm, dist_bar1_pu, pre_falta, &hms);
		if (msg == NULL) return(false);
		//envia mensagem para o Server
		sucesso = EnviaMensagem(msg);
		//destrói mensagem enviada
		//delete msg;
		}catch(Exception &e)
			{
			sucesso = false;
			}
	return(sucesso);
	}

//-----------------------------------------------------------------------------
bool __fastcall TSinapClient::SetManobraChave(int eqpto_tipo, AnsiString eqpto_cod, bool abrir, TDateTime dh)
	{
	//variáveis locais
	strHMS hms;
	TEqp  *eqp;
	VTMsg *msg;

	try{//verifica se conexão ativa
		if (! ClientSocket->Active) return(false);
		//verifica se o Eqp já existe em lisEQP
		if ((eqp = ExisteEqp(eqpto_tipo, eqpto_cod)) == NULL)
			{//cria novo Eqp e insere na lista
			lisEQP->Add(eqp = new TEqp(eqpto_tipo, eqpto_cod));
			}
		//verifica se o Eqp já tem o ID definido
		if (eqp->enabled)
			{//atualiza estado da Chave, usando seu Id
			return(SetManobraChave(eqp->tipo, eqp->id, abrir, dh));
			}
	  //cria mensagem que será enviada quando o ID do eqpto for recebido
		IniciaStrHMS(dh, hms);
		msg = protocolo->MontaMsgManobraChave(eqpto_tipo, eqp->id, &hms, abrir);
		if (msg == NULL) return(false);
		//insere Msg em Eqp
		eqp->InsereMsg(msg);
		//envia Msg solicitando Id do Eqpto
		GetEqp(eqpto_tipo, eqpto_cod, dh);
		}catch(Exception &e)
			{
			return(false);
			}
	return(true);
	}

//-----------------------------------------------------------------------------
bool __fastcall TSinapClient::SetManobraChave(int eqpto_tipo, int eqpto_id, bool abrir, TDateTime dh)
	{
	//variáveis locais
	bool   sucesso;
	strHMS hms;
	VTMsg *msg;

	try{//verifica se conexão ativa
		if (! ClientSocket->Active) return(false);
		//monta mensagem de solicitação de eqpto
		IniciaStrHMS(dh, hms);
		msg = protocolo->MontaMsgManobraChave(eqpto_tipo, eqpto_id, &hms, abrir);
		if (msg == NULL) return(false);
		//envia mensagem para o Server
		sucesso = EnviaMensagem(msg);
		//destrói mensagem enviada
		//delete msg;
		}catch(Exception &e)
			{
			sucesso = false;
			}
	return(sucesso);
	}

//-----------------------------------------------------------------------------
bool __fastcall TSinapClient::SetSupervisaoAutomatica(bool enabled)
	{
	//variáveis locais
	strHMS hms;
	TEqp  *eqp;
	VTMsg *msg;

	try{//verifica se conexão ativa
		if (! ClientSocket->Active) return(false);
	  //cria mensagem
		IniciaStrHMS(Now(), hms);
		msg = protocolo->MontaMsgSupervisaoAutomatica(&hms, enabled);
		if (msg == NULL) return(false);
		//envia Msg
		EnviaMensagem(msg);
		}catch(Exception &e)
			{
			return(false);
			}
	return(true);
	}

//-----------------------------------------------------------------------------
bool __fastcall TSinapClient::SetTrafoTap(int eqpto_tipo, AnsiString eqpto_cod, int variacao_tap, TDateTime dh)
	{
	//variáveis locais
	strHMS hms;
	TEqp  *eqp;
	VTMsg *msg;

	try{//verifica se conexão ativa
		if (! ClientSocket->Active) return(false);
		//verifica se o Eqp já existe em lisEQP
		if ((eqp = ExisteEqp(eqpto_tipo, eqpto_cod)) == NULL)
			{//cria novo Eqp e insere na lista
			lisEQP->Add(eqp = new TEqp(eqpto_tipo, eqpto_cod));
			}
		//verifica se o Eqp já tem o ID definido
		if (eqp->enabled)
			{//atualiza tap do Trafo, usando seu Id
			return(SetTrafoTap(eqp->tipo, eqp->id, variacao_tap, dh));
			}
	  //cria mensagem que será enviada quando o ID do eqpto for recebido
		IniciaStrHMS(dh, hms);
		msg = protocolo->MontaMsgAlteraTrafo(eqpto_tipo, eqp->id, &hms, variacao_tap);
		if (msg == NULL) return(false);
		//insere Msg em Eqp
		eqp->InsereMsg(msg);
		//envia Msg solicitando Id do Eqpto
		GetEqp(eqpto_tipo, eqpto_cod, dh);
		}catch(Exception &e)
			{
			return(false);
			}
	return(true);
	}

//-----------------------------------------------------------------------------
bool __fastcall TSinapClient::SetTrafoTap(int eqpto_tipo, int eqpto_id, int variacao_tap, TDateTime dh)
	{
	//variáveis locais
	bool   sucesso;
	strHMS hms;
	VTMsg *msg;

	try{//verifica se conexão ativa
		if (! ClientSocket->Active) return(false);
		//monta mensagem de solicitação de eqpto
		IniciaStrHMS(dh, hms);
		msg = protocolo->MontaMsgAlteraTrafo(eqpto_tipo, eqpto_id, &hms, variacao_tap);
		if (msg == NULL) return(false);
		//envia mensagem para o Server
		sucesso = EnviaMensagem(msg);
		//destrói mensagem enviada
		//delete msg;
		}catch(Exception &e)
			{
			sucesso = false;
			}
	return(sucesso);
	}

//-----------------------------------------------------------------------------
bool __fastcall TSinapClient::TrataRespostaExisteEqpto(VTMsg *msg_rx)
   {
   //variáveis locais
   VTMsg           *msg_tx;
   TEqp            *eqp;
   TMsgExisteEqpto *msg_existe = (TMsgExisteEqpto*)msg_rx;
   int             eqpto_tipo  = msg_existe->pcte.eqpto_tipo;
   int             eqpto_id    = msg_existe->pcte.eqpto_id;
   AnsiString      eqpto_cod   = msg_existe->pcte.codigo;

   //determina Eqp relativo ao eqpto da msg
	if ((eqp = ExisteEqp(eqpto_tipo, eqpto_cod)) == NULL)
		{//cria um novo Eqp para tratar de mensagens geradas a partir de supervisão automática
		lisEQP->Add(eqp = new TEqp(eqpto_tipo, eqpto_cod));
		}
   //atualiza Eqp: salva ID e atualiza status
   eqp->id      = eqpto_id;
   eqp->enabled = true;
   //envia todas as mensagens que estão na lista de Eqp
   for (int n = 0; n < eqp->lisMSG->Count; n++)
      {
      msg_tx = (VTMsg*)eqp->lisMSG->Items[n];
      //atualiza eqpto_id da Msg
      msg_tx->EqptoId = eqp->id;
      //envia a mensagem
      EnviaMensagem(msg_tx);
      //inclui um delay
      //if (n < (eqp->lisMSG->Count-1)) Sleep(100);
      }
   //descarta Msg de Eqp sem destrui-las porque EnviaMensagem() destrói Msg enviada
   eqp->lisMSG->Clear();
   return(true);
   }


//---------------------------------------------------------------------------
//eof
