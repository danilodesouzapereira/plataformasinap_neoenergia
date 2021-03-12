//-----------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include<DateUtils.hpp>
#include <PlataformaSinap\Fontes\Apl\VTApl.h>
#include <PlataformaSinap\Fontes\Curtocircuito\VTCurtocircuito.h>
#include <PlataformaSinap\Fontes\Edita\VTEdita.h>
#include <PlataformaSinap\Fontes\Flow\VTFlow.h>
#include <PlataformaSinap\Fontes\Geral\VTGeral.h>
#include <PlataformaSinap\Fontes\Geral\VTInfoset.h>
#include <PlataformaSinap\Fontes\Grafico\VTGrafico.h>
#include <PlataformaSinap\Fontes\Patamar\VTPatamar.h>
#include <PlataformaSinap\Fontes\Patamar\VTPatamares.h>
#include <PlataformaSinap\Fontes\Rede\VTBarra.h>
#include <PlataformaSinap\Fontes\Rede\VTChave.h>
#include <PlataformaSinap\Fontes\Rede\VTCapacitor.h>
#include <PlataformaSinap\Fontes\Rede\VTRedes.h>
#include <PlataformaSinap\Fontes\Rede\VTResFlowLig.h>
#include <PlataformaSinap\Fontes\Rede\VTTrafo.h>
#include <PlataformaSinap\Fontes\Rede\VTTrecho.h>
#include <PlataformaSinap\Fontes\RedeCC\ResCurto.h>
#include <PlataformaSinap\Fontes\RedeCC\VTBarCC.h>
#include <PlataformaSinap\Fontes\RedeCC\VTLigCC.h>
#include <PlataformaSinap\Fontes\RedeCC\VTRedeCC.h>
#include <PlataformaSinap\DLL_Inc\Funcao.h>
#include <PlataformaSinap\DLL_Inc\Curtocircuito.h>
#include "TSinapServer.h"
#include "..\Protocolo\TMsgAlteraCapacitor.h"
#include "..\Protocolo\TMsgAlteraTrafo.h"
#include "..\Protocolo\TMsgExisteEqpto.h"
#include "..\Protocolo\TMsgManobraChave.h"
#include "..\Protocolo\TMsgSimulaFalta.h"
#include "..\Protocolo\TMsgSupervisaoAuto.h"
#include "..\Protocolo\VTMsg.h"
#include "..\Protocolo\VTProtocolo.h"
#include "..\User\VTSinapUser.h"

//-----------------------------------------------------------------------------
VTSinapServer* __fastcall NewObjSinapServer(VTApl *apl_owner)
	{
   try{
      return(new TSinapServer(apl_owner));
      }catch(Exception &e)
         {//nada a fazer
			}
	//erro na criação do objeto
   return(NULL);
	}

//-----------------------------------------------------------------------------
__fastcall TSinapServer::TSinapServer(VTApl *apl_owner)
	{
	//salva ponteiro p/ objetos
   this->apl = apl_owner;
   user      = (VTSinapUser*)apl->GetObject(__classid(VTSinapUser));
	//cria listas
   lisEQP = new TList();
	lisMSG = new TList();
	//indica que não há um Client conectado
	client_connected = false;
	//cria objeto Protocolo
   protocolo    = NewObjProtocolo();
   //cria objeto ServerSocket e configura tratamento de eventos
   ServerSocket                     = new TServerSocket(NULL);
   ServerSocket->OnAccept           = ServerSocketAccept;
   ServerSocket->OnClientConnect    = ServerSocketClientConnect;
   ServerSocket->OnClientDisconnect = ServerSocketClientDisconnect;
   ServerSocket->OnClientRead       = ServerSocketClientRead;
   //aloca buffer para recepção de mensagens
   buffer.length = MAX_MSG_LEN;
   buffer.memory = new char[buffer.length];
   //cria objeto Curtocircuito
   curtocircuito = DLL_NewObjCurtocircuito(apl_owner);
	curtocircuito->AvisoEnabled = false;
	//desabilita supervisão automática
	SupervisaoAutomaticaStop();
	//inicia estado: cálculo de fluxo
   estado = calculoFLUXO;
   //inicia controle de cálculo de curto
   defeito.barra        = NULL;
   defeito.trecho       = NULL;
	defeito.patamar      = NULL;
   defeito.dist_bar1_km = 0.;
   defeito.tipo         = defeito3F;
   defeito.pre_falta    = false;
   //inicia valores de impedância de defeito
	IniciaZdef();
	}

//-----------------------------------------------------------------------------
__fastcall TSinapServer::~TSinapServer(void)
	{
	//cancela supervisão automática
	SupervisaoAutomaticaStop();
	//cancela conexão
   Desconecta();
   //destrói objetos
	if (curtocircuito) {delete curtocircuito;    curtocircuito = NULL;}
   if (protocolo)     {delete protocolo;        protocolo     = NULL;}
   if (ServerSocket)  {delete ServerSocket;     ServerSocket  = NULL;}
   if (buffer.memory)  {delete[] buffer.memory; buffer.memory = NULL;}
   //destrói lista sem destruir seus objetos
	if (lisEQP) {delete lisEQP; lisEQP = NULL;}
	//destrói lista e seus objetos
   if (lisMSG) {LimpaTList(lisMSG); delete lisMSG; lisMSG = NULL;}
	}

//---------------------------------------------------------------------------
void __fastcall TSinapServer::ActionRedeChangedExecute(TObject *Sender)
	{
	//variáveis locais
	int      eqpto_tipo;
	VTEqpto *eqpto;

	//verifica se existe um Client conectado
	if (! client_connected) return;
	//verifica se supervisão atuomática está habilitada
	if ((! supervisao_auto.server_enabled) || (! supervisao_auto.client_enabled)) return;
   //executa cálculo selecionado
   ExecutaCalculo();
	//loop p/ todos os Eqptos selecionados para supervisão automática
	for (int n = 0; n < supervisao_auto.lisSAE->Count; n++)
		{
		eqpto = (VTEqpto*)supervisao_auto.lisSAE->Items[n];
		//determina tipo do eqpto usado pelo protocolo
		if (! protocolo->ConverteTipoEqptoSinapParaProtocolo(eqpto->Tipo(), eqpto_tipo)) continue;
		EnviaMsgRegistro(eqpto_tipo, eqpto->Id);
		}
	}

//---------------------------------------------------------------------------
bool __fastcall TSinapServer::Conecta(int server_port)
   {
   try{//proteção: se necessário, fecha conexão atual
      Desconecta();
      //inicia conexção
      ServerSocket->ServerType = stNonBlocking;
      ServerSocket->Port       = server_port;
      //ativa o ServerSocket
      ServerSocket->Open();
      }catch(Exception &e)
         {
         return(false);
         }
   return(Ativo);
   }

//---------------------------------------------------------------------------
void __fastcall TSinapServer::Desconecta(void)
   {
  //cancela conexão
   if (ServerSocket->Active) ServerSocket->Close();
    }

//---------------------------------------------------------------------------
void __fastcall TSinapServer::EnviaMensagem(VTMsg *msg)
   {
   //variáveis locais
   strBUF *buf = msg->ConverteMsgToBuf();

   //envia a mensagem
   ServerSocket->Socket->Connections[0]->SendBuf(buf->memory, buf->length);
   //atualiza log Tx
   LogTx(msg->MsgAsString());
   //destrói a Msg enviada
   delete msg;
   }

//---------------------------------------------------------------------------
void __fastcall TSinapServer::EnviaMsgAck(int funcao_acked, bool sucesso)
   {
   //variáveis locais
   VTMsg      *msg;
   AnsiString aviso = "Erro na criação da mensagem 'Ack'";

   try{//monta mensagem de Ack
      if ((msg = protocolo->MontaMsgAck(funcao_acked, sucesso)) == NULL)
         {
         Aviso(aviso);
         return;
         }
      //envia mensagem para o Client
		EnviaMensagem(msg);
      //destrói a Msg enviada
      //delete msg;
      }catch(Exception &e)
         {
         Aviso(aviso);
         }
   }

//---------------------------------------------------------------------------
void __fastcall TSinapServer::EnviaMsgNAck(int funcao_nacked)
   {
   //variáveis locais
   VTMsg      *msg;
   AnsiString aviso = "Erro na criação da mensagem 'NAck'";

   try{//monta mensagem de Ack
      if ((msg = protocolo->MontaMsgNAck(funcao_nacked)) == NULL)
         {
         Aviso(aviso);
         return;
         }
      //envia mensagem para o Client
      EnviaMensagem(msg);
      //destrói a Msg enviada
      //delete msg;
      }catch(Exception &e)
         {
         Aviso(aviso);
         }
   }

//---------------------------------------------------------------------------
bool __fastcall TSinapServer::EnviaMsgRegistro(int eqpto_tipo, int eqpto_id)
	{
	//variáveis locais
	bool sucesso = true;

	//monta MsgRegistro de acordo com tipo de eqpto
	switch(eqpto_tipo)
		{
		case teBARRA:         EnviaMsgRegistroBarra(eqpto_id);  break;
		case teTRECHO:        EnviaMsgRegistroTrecho(eqpto_id); break;
		case teCHAVE:         EnviaMsgRegistroChave(eqpto_id);  break;
		case teTRANSFORMADOR: EnviaMsgRegistroTrafo(eqpto_id);  break;
		//case teREGULADOR:
		//case teCAPACITORSERIE:
		case teCAPACITOR:     EnviaMsgRegistroCapacitor(eqpto_id); break;
		//case teGERADOR:
		//case teSUPRIMENTO:
		//case teCARGA:
		//case teFILTRO:
		//case teREATOR:
		default: //tipo de eqpto inválido - envia mensagem de NAck
			sucesso = false;
		}
	return(sucesso);
	}

//---------------------------------------------------------------------------
void __fastcall TSinapServer::EnviaMsgRegistroBarra(int eqpto_id)
   {
   //variáveis locais
   AnsiString   aviso = "Erro na criação da mensagem 'Registro de Barra'";
   VTBarra     *barra;
   VTBarCC     *barCC;
   VTMsg       *msg;
   VTPatamar   *patamar;
   VTRedes     *redes     = (VTRedes*)apl->GetObject(__classid(VTRedes));
   VTRedeCC    *redeCC    = (VTRedeCC*)apl->GetObject(__classid(VTRedeCC));
   VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));


   try{//determina Barra solicitada
      if ((barra = redes->ExisteBarra(eqpto_id)) == NULL)
         {//não existe Barra com o ID indicado: retorna Ack/false
         EnviaMsgAck(msg->Funcao, false);
         return;
         }
      //determina Patamar da hora solicitada
      if ((patamar = patamares->ExistePatamar(hms.hora, hms.minuto)) == NULL)
         {
         EnviaMsgAck(funcaoENVIO_REGISTRO, false);
         Aviso("Não existe patamar da hora solicitada");
         return;
         }
      //monta mensagem de Registro de Barra
      if (calculo_executado == calculoFLUXO)
         {//dados de fluxo
         if ((msg = protocolo->MontaMsgRegistroBarraFluxo(barra, &hms, patamar->Index)) == NULL)
            {
            EnviaMsgAck(funcaoENVIO_REGISTRO, false);
            Aviso(aviso);
            return;
            }
         }
      else
         {//dados de curto
         barCC = (VTBarCC*)barra->Obj;
         if ((msg = protocolo->MontaMsgRegistroBarraCurto(barCC, &hms, defeito.tipo)) == NULL)
            {
            EnviaMsgAck(funcaoENVIO_REGISTRO, false);
            Aviso(aviso);
            return;
            }
         }
      //envia mensagem para o Client
      EnviaMensagem(msg);
      //destrói a Msg enviada
      //delete msg;
      }catch(Exception &e)
         {
         Aviso(aviso);
         }
   }

//---------------------------------------------------------------------------
void __fastcall TSinapServer::EnviaMsgRegistroCapacitor(int eqpto_id)
	{
	//variáveis locais
	AnsiString  aviso = "Erro na criação da mensagem 'Registro de Capacitor'";
	VTMsg       *msg;
   VTCapacitor *capacitor;
	VTPatamar   *patamar;
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   VTRedes     *redes     = (VTRedes*)apl->GetObject(__classid(VTRedes));

	try{//determina Capacitor
		if ((capacitor = (VTCapacitor*)redes->ExisteEqpto(eqptoCAPACITOR, eqpto_id)) == NULL)
			{//não existe Caapcitor com o ID indicado: retorna Ack/false
			EnviaMsgAck(msg->Funcao, false);
			return;
			}
		//determina Patamar da hora solicitada
		if ((patamar = patamares->ExistePatamar(hms.hora, hms.minuto)) == NULL)
			{
			EnviaMsgAck(funcaoENVIO_REGISTRO, false);
			Aviso("Não existe patamar da hora solicitada");
			return;
			}
	   //monta mensagem de Registro de Capacitor
		if ((msg = protocolo->MontaMsgRegistroCapacitor(capacitor, &hms, patamar->Index)) == NULL)
		   {
         Aviso(aviso);
         return;
         }
		//envia mensagem para o Client
		EnviaMensagem(msg);
		}catch(Exception &e)
			{
			Aviso(aviso);
			}
   }

//---------------------------------------------------------------------------
void __fastcall TSinapServer::EnviaMsgRegistroChave(int eqpto_id)
	{
	//variáveis locais
	AnsiString   aviso = "Erro na criação da mensagem 'Registro de Chave'";
	VTLigCC     *ligCC;
	VTBarCC     *bar1CC, *bar2CC;
	VTLigacao   *ligacao;
	VTMsg       *msg;
	VTPatamar   *patamar;
	VTRedeCC    *redeCC    = (VTRedeCC*)apl->GetObject(__classid(VTRedeCC));
	VTRedes     *redes     = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));


	try{//determina Barra solicitada
		if ((ligacao = (VTLigacao*)redes->ExisteEqpto(eqptoCHAVE, eqpto_id)) == NULL)
			{//não existe Chave com o ID indicado: retorna Ack/false
			EnviaMsgAck(msg->Funcao, false);
			return;
			}
		//determina Patamar da hora solicitada
		if ((patamar = patamares->ExistePatamar(hms.hora, hms.minuto)) == NULL)
			{
			EnviaMsgAck(funcaoENVIO_REGISTRO, false);
			Aviso("Não existe patamar da hora solicitada");
			return;
			}
	  //monta mensagem de Registro de Chave
	  if (calculo_executado == calculoFLUXO)
			{//dados de fluxo
			if ((msg = protocolo->MontaMsgRegistroChaveFluxo((VTChave*)ligacao, &hms, patamar->Index)) == NULL)
				{
				EnviaMsgAck(funcaoENVIO_REGISTRO, false);
				Aviso(aviso);
				return;
				}
			}
		else
			{//dados de curto
         ligCC  = (VTLigCC*)(ligacao->Obj);
			bar1CC = (VTBarCC*)(ligacao->pbarra1->Obj);
			bar2CC = (VTBarCC*)(ligacao->pbarra2->Obj);
			if ((msg = protocolo->MontaMsgRegistroChaveCurto(ligCC, bar1CC, bar2CC, &hms, defeito.tipo)) == NULL)
				{
				EnviaMsgAck(funcaoENVIO_REGISTRO, false);
				Aviso(aviso);
				return;
				}
			}
		//envia mensagem para o Client
		EnviaMensagem(msg);
		//destrói a Msg enviada
		//delete msg;
		}catch(Exception &e)
			{
			Aviso(aviso);
			}
	}

//---------------------------------------------------------------------------
void __fastcall TSinapServer::EnviaMsgRegistroTrafo(int eqpto_id)
	{
	//variáveis locais
	AnsiString   aviso = "Erro na criação da mensagem 'Registro de Trafo'";
	VTLigCC     *ligCC;
	VTBarCC     *bar1CC, *bar2CC;
	VTLigacao   *ligacao;
	VTMsg       *msg;
	VTPatamar   *patamar;
	VTRedeCC    *redeCC    = (VTRedeCC*)apl->GetObject(__classid(VTRedeCC));
	VTRedes     *redes     = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));


	try{//determina Barra solicitada
		if ((ligacao = (VTLigacao*)redes->ExisteEqpto(eqptoTRAFO, eqpto_id)) == NULL)
			{//não existe Trafo com o ID indicado: retorna Ack/false
			EnviaMsgAck(msg->Funcao, false);
			return;
			}
		//determina Patamar da hora solicitada
		if ((patamar = patamares->ExistePatamar(hms.hora, hms.minuto)) == NULL)
			{
			EnviaMsgAck(funcaoENVIO_REGISTRO, false);
			Aviso("Não existe patamar da hora solicitada");
			return;
			}
	  //monta mensagem de Registro de Chave
	  if (calculo_executado == calculoFLUXO)
			{//dados de fluxo
			if ((msg = protocolo->MontaMsgRegistroTrafoFluxo((VTTrafo*)ligacao, &hms, patamar->Index)) == NULL)
				{
				EnviaMsgAck(funcaoENVIO_REGISTRO, false);
				Aviso(aviso);
				return;
				}
			}
		else
			{//dados de curto
         ligCC  = (VTLigCC*)(ligacao->Obj);
			bar1CC = (VTBarCC*)(ligacao->pbarra1->Obj);
			bar2CC = (VTBarCC*)(ligacao->pbarra2->Obj);
			if ((msg = protocolo->MontaMsgRegistroTrafoCurto(ligCC, bar1CC, bar2CC, &hms, defeito.tipo)) == NULL)
				{
				EnviaMsgAck(funcaoENVIO_REGISTRO, false);
				Aviso(aviso);
				return;
				}
			}
		//envia mensagem para o Client
		EnviaMensagem(msg);
		//destrói a Msg enviada
		//delete msg;
		}catch(Exception &e)
			{
			Aviso(aviso);
			}
	}

//---------------------------------------------------------------------------
void __fastcall TSinapServer::EnviaMsgRegistroTrecho(int eqpto_id)
	{
	//variáveis locais
	AnsiString   aviso = "Erro na criação da mensagem 'Registro de Trecho'";
	VTLigCC     *ligCC;
	VTBarCC     *bar1CC, *bar2CC;
	VTLigacao   *ligacao;
	VTMsg       *msg;
	VTPatamar   *patamar;
	VTRedeCC    *redeCC    = (VTRedeCC*)apl->GetObject(__classid(VTRedeCC));
	VTRedes     *redes     = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));


	try{//determina Barra solicitada
		if ((ligacao = (VTLigacao*)redes->ExisteEqpto(eqptoTRECHO, eqpto_id)) == NULL)
			{//não existe Trecho com o ID indicado: retorna Ack/false
			EnviaMsgAck(msg->Funcao, false);
			return;
			}
		//determina Patamar da hora solicitada
		if ((patamar = patamares->ExistePatamar(hms.hora, hms.minuto)) == NULL)
			{
			EnviaMsgAck(funcaoENVIO_REGISTRO, false);
			Aviso("Não existe patamar da hora solicitada");
			return;
			}
	  //monta mensagem de Registro de Trecho
	  if (calculo_executado == calculoFLUXO)
			{//dados de fluxo
			if ((msg = protocolo->MontaMsgRegistroTrechoFluxo((VTTrecho*)ligacao, &hms, patamar->Index)) == NULL)
				{
				EnviaMsgAck(funcaoENVIO_REGISTRO, false);
				Aviso(aviso);
				return;
				}
			}
		else
			{//dados de curto
         ligCC  = (VTLigCC*)(ligacao->Obj);
			bar1CC = (VTBarCC*)(ligacao->pbarra1->Obj);
			bar2CC = (VTBarCC*)(ligacao->pbarra2->Obj);
			if ((msg = protocolo->MontaMsgRegistroTrechoCurto(ligCC, bar1CC, bar2CC, &hms, defeito.tipo)) == NULL)
				{
				EnviaMsgAck(funcaoENVIO_REGISTRO, false);
				Aviso(aviso);
				return;
				}
			}
		//envia mensagem para o Client
		EnviaMensagem(msg);
		//destrói a Msg enviada
		//delete msg;
		}catch(Exception &e)
			{
			Aviso(aviso);
			}
	}

//---------------------------------------------------------------------------
bool __fastcall TSinapServer::ExecutaCalculo(void)
	{
	//variáveis locais
	bool            sucesso;
   bool            rede_alterada  = true;
   VTFlow          *flow           = (VTFlow*)apl->GetObject(__classid(VTFlow));
   VTGrafico       *grafico        = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
   VTRedes         *redes          = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //atualiza indicação de cálculo executado
   calculo_executado = estado;
   //executa cálculo em função do estado
   if (estado == calculoFLUXO)
      {//calculo de fluxo de potência
      //flow->UpToDate = false;
      flow->ProgressoEnabled = false;
      sucesso = flow->CalculaPorRede(redes);
      //atualiza gráfico
      if (sucesso) grafico->Show(grafFLOW);
      }
   else if ((estado == calculoCURTO)&&(defeito.barra != NULL))
      {//calcula curto no ponto de defeito
      sucesso = curtocircuito->Executa(redes, defeito.Zdef, rede_alterada, defeito.barra,
                                       defeito.patamar->Index, defeito.pre_falta);
      //atualiza gráfico
      if (sucesso) grafico->Show(grafCURTO);
      }
   else if ((estado == calculoCURTO)&&(defeito.trecho != NULL))
      {//calcula curto no ponto de defeito
      sucesso = curtocircuito->Executa(redes, defeito.Zdef, rede_alterada, defeito.trecho, defeito.dist_bar1_km,
                                       defeito.patamar->Index, defeito.pre_falta);
      //atualiza gráfico
      if (sucesso) grafico->Show(grafCURTO);
      }
   //se o cálculo é de curto e o defeito foi isolado, calcula fluxo
   if ((estado == calculoCURTO)&&(curtocircuito->DefeitoIsolado))
      {
      sucesso = flow->CalculaPorRede(redes);
      //atualiza indicação de calculo executado
      calculo_executado = calculoFLUXO;
      //atualiza gráfico
      if (sucesso) grafico->Show(grafFLOW);
      }
   //estado indefinido: não executa cálculo
   return(sucesso);
   }

//---------------------------------------------------------------------------
VTEqpto* __fastcall TSinapServer::ExisteEqpto(int eqpto_tipo, AnsiString codigo)
   {
   //variáveis locais
   VTEqpto *eqpto;
   VTRedes  *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //obtem lista de Eqptos do tipo indicado
   lisEQP->Clear();
   redes->LisEqpto(lisEQP,eqpto_tipo);
   //procura Eqpto pelo seu código
   for (int n = 0; n < lisEQP->Count; n++)
      {
      eqpto = (VTEqpto*)lisEQP->Items[n];
      if (eqpto->Codigo.AnsiCompareIC(codigo) == 0) return(eqpto);
      }
   return(NULL);
   }

//-----------------------------------------------------------------------------
void __fastcall TSinapServer::HabilitaPatamarUnico(strHMS &hms)
   {

   }

//-----------------------------------------------------------------------------
void __fastcall TSinapServer::IniciaStrHMS(TDateTime datetime, strHMS &hms)
   {
   hms.hora    = HourOf(datetime);
   hms.minuto  = MinuteOf(datetime);
   hms.segundo = SecondOf(datetime);
   }

//---------------------------------------------------------------------------
void __fastcall TSinapServer::IniciaZdef(void)
   {
   //zera impedância de defeito
   defeito.Zdef.Zdef3F_ff.r  = defeito.Zdef.Zdef3F_ff.x  = 0;
   defeito.Zdef.Zdef2F_ff.r  = defeito.Zdef.Zdef2F_ff.x  = 0;
   defeito.Zdef.Zdef2FT_ff.r = defeito.Zdef.Zdef2FT_ff.x = 0;
   defeito.Zdef.Zdef2FT_ft.r = defeito.Zdef.Zdef2FT_ft.x = 0;
   defeito.Zdef.Zdef1FT_ft.r = defeito.Zdef.Zdef1FT_ft.x = 0;
   }
//---------------------------------------------------------------------------
int __fastcall TSinapServer::LisEqptoSupervisionado(TList *lisEXT, int eqpto_tipo)
   {
   //variáveis locais
   VTEqpto *eqpto;

   //copia Chaves da lista de objetos supervisionados
   for (int n = 0; n < supervisao_auto.lisSAE->Count; n++)
      {
      eqpto = (VTEqpto*)supervisao_auto.lisSAE->Items[n];
      if ((eqpto_tipo == eqptoINDEF) || (eqpto->Tipo() == eqpto_tipo))
         {
         if (lisEXT->IndexOf(eqpto) < 0) lisEXT->Add(eqpto);
         }
      }
	return(lisEXT->Count);
   }

//---------------------------------------------------------------------------
void __fastcall TSinapServer::LogPatamar(AnsiString txt)
	{
	if (user) user->LogPatamar(txt);
	}

//---------------------------------------------------------------------------
void __fastcall TSinapServer::LogRx(AnsiString txt)
	{
	if (user) user->LogRx(txt);
	}

//---------------------------------------------------------------------------
void __fastcall TSinapServer::LogTx(AnsiString txt)
   {
   if (user) user->LogTx(txt);
	}

//---------------------------------------------------------------------------
VTPatamar* __fastcall TSinapServer::PatamarAtual(void)
   {
	//variáveis locais
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

	//verifica se o Patamar está definido
	if (supervisao_auto.patamar == NULL)
		{//define Patamar da hora corrente para supervisão automática
		supervisao_auto.patamar   = patamares->ExistePatamar(HourOf(Now()), 0);
		if (supervisao_auto.patamar == NULL)
			{//seleciona primeiro Patamar habilitado
			supervisao_auto.patamar = patamares->PrimeiroPatamarHabilitado();
			}
		}
	return(supervisao_auto.patamar);
   }

//-----------------------------------------------------------------------------
bool __fastcall TSinapServer::PM_GetAtivo(void)
   {
	return(ServerSocket->Active);
	}

//-----------------------------------------------------------------------------
void __fastcall TSinapServer::ReordenaLisEqptoSupervisionado(VTEqpto *eqpto)
	{
	//variáveis locais
	int index;

	//verifica se supervisãoautomática está habilitada
	if (supervisao_auto.lisSAE->Count == 0) return;
	//se o Eqpto existir em lisSAE, move para o fim da lista
	if ((index = supervisao_auto.lisSAE->IndexOf(eqpto)) >= 0)
		{
		supervisao_auto.lisSAE->Delete(index);
		supervisao_auto.lisSAE->Add(eqpto);
		}
	}

//---------------------------------------------------------------------------
VTPatamar* __fastcall TSinapServer::SelecionaProximoPatamar(void)
	{
	//variáveis locais
	int         ind_pat;
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

	//define Patamar
	if (supervisao_auto.patamar == NULL)
		{//define Patamar da hora corrente para supervisão automática
		supervisao_auto.patamar   = patamares->ExistePatamar(HourOf(Now()), 0);
		if (supervisao_auto.patamar == NULL)
			{//seleciona primeiro Patamar habilitado
			supervisao_auto.patamar = patamares->PrimeiroPatamarHabilitado();
			}
		}
	else
		{//seleciona próximo patamar
		ind_pat = supervisao_auto.patamar->Index + 1;
		if (ind_pat >= patamares->NumPatamar()) ind_pat = 0;
		//define Patamar ativo
		supervisao_auto.patamar = patamares->Patamar[ind_pat];
		}
	//define o mesmo Patamar para cálculo de defeito
	defeito.patamar = supervisao_auto.patamar;
	//indica Patamar selecionado no Log
	LogPatamar(supervisao_auto.patamar->Nome);
	//proteção
	if ((supervisao_auto.server_enabled) && (supervisao_auto.client_enabled))
		{//define hora/minuto/segundo das Msgs a partir do Patamar selecionado
		hms.hora    = char(supervisao_auto.patamar->HoraCal);
		hms.minuto  = char(supervisao_auto.patamar->MinutoCal);
		hms.segundo = 0;
		//executa cálculo
		//ExecutaCalculo();
		//executa cálculo e envia dados dos Eqptos configurados para supervisão automática
		supervisao_auto.ActionRedeChanged->Execute();
      }
	return(supervisao_auto.patamar);
   }

//---------------------------------------------------------------------------
void __fastcall TSinapServer::ServerSocketAccept(TObject *Sender, TCustomWinSocket *Socket)
   {
   //atualiza log Rx
   LogRx("Accept to: " + Socket->RemoteHost);
   }

//---------------------------------------------------------------------------
void __fastcall TSinapServer::ServerSocketClientConnect(TObject *Sender, TCustomWinSocket *Socket)
   {
	//indica que há um Client conectado mas ainda sem supervisão automática habilitada
	client_connected               = true;
	supervisao_auto.client_enabled = false;
	//inicia supervisão automática
	SupervisaoAutomatica();
	//atualiza log Rx
   LogRx("Connect to: " + Socket->RemoteHost);
   }

//---------------------------------------------------------------------------
void __fastcall TSinapServer::ServerSocketClientDisconnect(TObject *Sender, TCustomWinSocket *Socket)
   {
	//indica que não há um Client conectado
	client_connected               = false;
	supervisao_auto.client_enabled = false;
	LogRx("Disconnect from: " + Socket->RemoteHost);
   }

//---------------------------------------------------------------------------
void __fastcall TSinapServer::ServerSocketClientRead(TObject *Sender, TCustomWinSocket *Socket)
   {
   //variáveis locais
   int     length;
   int     msg_count;
   VTMsg  *msg;
   strBUF buf_rx;

   //atualiza log Rx
   buf_rx.memory = buffer.memory;
   buf_rx.length = buffer.length;
   buf_rx.length = Socket->ReceiveBuf(buf_rx.memory, buf_rx.length);
   //salva número de mensagens na lista
   msg_count = lisMSG->Count;
   //identifica a Msgs recebidas
   if (protocolo->MontaMsgRecebida(&buf_rx, lisMSG) == 0)
      {
      Aviso("Mensagem recebida não pode ser identificada");
      return;
      }
   //verifica se já havia mensagens na lista
   if (msg_count > 0)
      {//as novas mensagens serão tratadas após o tratamento das mensagens que já estavam na lista
      return;
      }
   //loop p/ todas Msg recebidas
   for (int n = 0; n < lisMSG->Count; n++)
      {
      msg = (VTMsg*)lisMSG->Items[n];
      //atualiza log Rx
      LogRx(msg->MsgAsString());
      //verifica função solicitada
      switch(msg->Funcao)
         {
         case funcaoACK :  //função não esperada pelo servidor
            EnviaMsgNAck(msg->Funcao);
            break;
         case funcaoALTERA_CARGA:
            TrataMsgAlteraCarga(msg);
            break;
			case funcaoALTERA_GERADOR:
				TrataMsgAlteraGerador(msg);
				break;
         case funcaoALTERA_SUPRIMENTO:
            TrataMsgAlteraSuprimento(msg);
            break;
			case funcaoALTERA_CAPACITOR:
				TrataMsgAlteraCapacitor(msg);
            break;
			case funcaoALTERA_TRAFO:
				TrataMsgAlteraTrafo(msg);
            break;
			case funcaoELIMINA_FALTA:
            TrataMsgEliminaFalta(msg);
            break;
			case funcaoENVIO_REGISTRO:
            EnviaMsgNAck(msg->Funcao);
            break;
         case funcaoMANOBRA_CHAVE:
            TrataMsgManobraChave(msg);
            break;
			case funcaoSIMULA_FALTA:
            TrataMsgSimulaFalta(msg);
				break;
			case funcaoSOLICITA_REGISTRO:
            TrataMsgSolicitacaoRegistro(msg);
				break;
         case funcaoEXISTE_EQPTO:
				TrataMsgExisteEqpto(msg);
            break;
			case funcaoSUPERVISAO_AUTO:
				TrataMsgSupervisaoAutomatica(msg);
            break;
			default: //função desconhecida - retorna Erro
            EnviaMsgNAck(msg->Funcao);
         }
      }
	//destrói todas Msg recebidas em lisMSG
   LimpaTList(lisMSG);
	}

/*
//---------------------------------------------------------------------------
bool __fastcall TSinapServer::SimulaTempoReal(void)
	{
	//atualiza Patamar atual
	int         ind_pat;
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

	//define Patamar
	if (supervisao_auto.patamar == NULL)
		{//define Patamar da hora corrente para supervisão automática
		supervisao_auto.patamar   = patamares->ExistePatamar(HourOf(Now()), 0);
		if (supervisao_auto.patamar == NULL)
			{//seleciona primeiro Patamar habilitado
			supervisao_auto.patamar = patamares->PrimeiroPatamarHabilitado();
			}
		}
	else
		{//seleciona próximo patamar
		ind_pat = supervisao_auto.patamar->Index + 1;
		if (ind_pat >= patamares->NumPatamar()) ind_pat = 0;
		//define Patamar ativo
		supervisao_auto.patamar = patamares->Patamar[ind_pat];
		}
	//define o mesmo Patamar para cálculo de defeito
	defeito.patamar = supervisao_auto.patamar;
	//indica Patamar selecionado no Log
	LogPatamar(supervisao_auto.patamar->Nome);
	//proteção
	if ((! supervisao_auto.server_enabled)|| (! supervisao_auto.client_enabled)) return(true);
	//define hora/minuto/segundo das Msgs a partir do Patamar selecionado
	hms.hora    = char(supervisao_auto.patamar->HoraCal);
	hms.minuto  = char(supervisao_auto.patamar->MinutoCal);
	hms.segundo = 0;
	//executa cálculo
	ExecutaCalculo();
	//envia dados dos Eqptos configurados para supervisão automática
	supervisao_auto.ActionRedeChanged->Execute();

	return(true);
	}
 */

//---------------------------------------------------------------------------
bool __fastcall TSinapServer::SupervisaoAutomatica(void)
	{
	//variáveis locais
	strHMS  hms;
	int     eqptipo_proto;
	VTEqpto *eqpto;
	VTMsg   *msg_tx;

	//verifica se supervisão automática está habilitada
	if ((! supervisao_auto.server_enabled) || (! supervisao_auto.client_enabled)) return(true);
	//verifica se existe um Client conectado
	if (! client_connected) return(true);
	//envia mensagens para o Client informando o ID dos Eqptos selecionados para supervisão automática
	for (int n = 0; n < supervisao_auto.lisSAE->Count; n++)
		{
		eqpto = (VTEqpto*)supervisao_auto.lisSAE->Items[n];
		//monta mensagem com identificação do eqpto (OBS: tipo de eqpto do protocolo)
		protocolo->ConverteTipoEqptoSinapParaProtocolo(eqpto->Tipo(), eqptipo_proto);
		IniciaStrHMS(Now(), hms);
		msg_tx = protocolo->MontaMsgExisteEqpto(eqptipo_proto, eqpto->Id, eqpto->Codigo, &hms);
		//envia mensagem para o Client
		EnviaMensagem(msg_tx);
		}
	//simula alteração da Rede para enviar dados dos Eqptos selecionados para supervisão automática
	supervisao_auto.ActionRedeChanged->Execute();

	return(true);
	}

//---------------------------------------------------------------------------
bool __fastcall TSinapServer::SupervisaoAutomaticaStart(TList *lisEXT)
	{
	//variáveis locais
	VTEdita     *edita     = (VTEdita*)apl->GetObject(__classid(VTEdita));
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

	//habilita supervisão automática
	supervisao_auto.server_enabled = true;
	//define Patamar da hora corrente para supervisão automática
	supervisao_auto.patamar   = patamares->ExistePatamar(HourOf(Now()), 0);
	if (supervisao_auto.patamar == NULL)
		{
		supervisao_auto.patamar = patamares->PrimeiroPatamarHabilitado();
		}
	//define hora/minuto/segundo das Msgs a partir do Patamar selecionado
	hms.hora    = char(supervisao_auto.patamar->HoraCal);
	hms.minuto  = char(supervisao_auto.patamar->MinutoCal);
	hms.segundo = 0;
	//indica Patamar selecionado no Log
	LogPatamar(supervisao_auto.patamar->Nome);
	//cria lista dos Eqptos supervisionados
	if (supervisao_auto.lisSAE == NULL)
		{
		supervisao_auto.lisSAE  = new TList();
		}
	//reinicia lista de eqptos com supervisão automática
	supervisao_auto.lisSAE->Clear();
	CopiaTList(lisEXT, supervisao_auto.lisSAE);
	//cria Action para receber eventos de alteração da rede
	if (supervisao_auto.ActionRedeChanged == NULL)
		{
		supervisao_auto.ActionRedeChanged            = new TAction(NULL);
		supervisao_auto.ActionRedeChanged->OnExecute = ActionRedeChangedExecute;
		//configura VTEdita para notificar alterações na rede
		edita->ActionOnChangeRedeInsere(supervisao_auto.ActionRedeChanged);
		}
	//informa ID de todos os Eqptos para o Client
	SupervisaoAutomatica();
	return(true);
	}

//---------------------------------------------------------------------------
bool __fastcall TSinapServer::SupervisaoAutomaticaStop(void)
	{
	//variáveis locais
	VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));

	//desabilita supervisão atuomática
	supervisao_auto.server_enabled = false;
	supervisao_auto.patamar        = NULL;
	//verifica se existe Action
	if (supervisao_auto.ActionRedeChanged)
		{//configura VTEdita para não notificar alterações na rede
		edita->ActionOnChangeRedeRetira(supervisao_auto.ActionRedeChanged);
		//destrói Action
		delete supervisao_auto.ActionRedeChanged;
		supervisao_auto.ActionRedeChanged = NULL;
		}
	//destrói lista de equipamentos supervisionados
	if (supervisao_auto.lisSAE)
		{
		delete supervisao_auto.lisSAE;
		supervisao_auto.lisSAE = NULL;
		}
	return(true);
	}

//---------------------------------------------------------------------------
void __fastcall TSinapServer::TrataMsgAlteraCapacitor(VTMsg *msg)
	{
	//variáveis locais
	bool                enabled;
	VTCapacitor         *capacitor, *clone;
	TMsgAlteraCapacitor *msg_altera  = (TMsgAlteraCapacitor*)msg;
	VTEdita             *edita       = (VTEdita*)apl->GetObject(__classid(VTEdita));
	VTRedes             *redes       = (VTRedes*)apl->GetObject(__classid(VTRedes));

	//salva hora/minuto/segundo indicado na msg
	msg->HoraMinutoSegundo(hms);
	//determina Capacitor  a ser alterado
	capacitor = (VTCapacitor*)redes->ExisteEqpto(eqptoCAPACITOR, msg_altera->EqptoId());
	enabled   = (msg_altera->pcte.ligado == 1);
	if (capacitor != NULL)
		{//verifica se o estado do Capacitor deve ser alterado
		if (capacitor->Status[sttENABLED] != enabled)
			{//altera Status do Capacitor
			clone = capacitor->Clone();
			clone->Status[sttENABLED] = enabled;
			edita->Altera(capacitor, clone);
			}
		}
	//destrói a Msg recebida
	//delete msg;
	}


//---------------------------------------------------------------------------
void __fastcall TSinapServer::TrataMsgAlteraCarga(VTMsg *msg)
	{
	//variáveis locais

	}

//---------------------------------------------------------------------------
void __fastcall TSinapServer::TrataMsgAlteraGerador(VTMsg *msg)
	{
	//variáveis locais

   }

//---------------------------------------------------------------------------
void __fastcall TSinapServer::TrataMsgAlteraSuprimento(VTMsg *msg)
	{
	//variáveis locais

	}

//---------------------------------------------------------------------------
void __fastcall TSinapServer::TrataMsgAlteraTrafo(VTMsg *msg)
	{
	//variáveis locais
	int              variacao_tap;
	VTTrafo          *trafo;
	TMsgAlteraTrafo  *msg_altera  = (TMsgAlteraTrafo*)msg;
	VTEdita          *edita       = (VTEdita*)apl->GetObject(__classid(VTEdita));
	VTRedes          *redes       = (VTRedes*)apl->GetObject(__classid(VTRedes));

	//salva hora/minuto/segundo indicado na msg
	msg->HoraMinutoSegundo(hms);
	//determina Trafo a ser alterafo
	trafo = (VTTrafo*)redes->ExisteEqpto(eqptoTRAFO, msg_altera->EqptoId());
	if (trafo != NULL)
		{//verifica se foi definida uma variação de tap
		variacao_tap = msg_altera->pcte.variacao_tap;
		if (variacao_tap != 0)
			{//altera tap do Trafo
			edita->AlteraTapTrafo(trafo, variacao_tap);
			//executa cálculo de fluxo ou curto
			//ExecutaCalculo();
			}
		}
	//destrói a Msg recebida
	//delete msg;
	}

//---------------------------------------------------------------------------
void __fastcall TSinapServer::TrataMsgEliminaFalta(VTMsg *msg)
	{
	//salva hora/minuto/segundo indicado na msg
	msg->HoraMinutoSegundo(hms);
	//atualiza estado de cálculo
	estado = calculoFLUXO;
	//executa cálculo de curto
	//ExecutaCalculo();
   //executa cálculo e envia dados dos Eqptos configurados para supervisão automática
   supervisao_auto.ActionRedeChanged->Execute();
	}

//---------------------------------------------------------------------------
void __fastcall TSinapServer::TrataMsgExisteEqpto(VTMsg *msg)
	{
	//variáveis locais
	strHMS            hms;
	VTEqpto          *eqpto;
	TMsgExisteEqpto  *msg_existe = (TMsgExisteEqpto*)msg;
	VTMsg            *msg_tx;
	int               eqptipo_sinap;
	int               eqptipo_proto = msg_existe->EqptoTipo();
	AnsiString        eqpto_cod      = msg_existe->pcte.codigo;

   //converte tipo de Eqpto da Msg para tipo de eqpto Sinap
	if (! protocolo->ConverteTipoEqptoProtocoloParaSinap(eqptipo_proto, eqptipo_sinap))
		{//envia mensagem de ACK indicando erro
      EnviaMsgAck(msg->Funcao, false);
      //destrói a Msg recebida
      //delete msg;
		return;
      }
   //verifica se existe o eqpto indicado na rede
	eqpto = ExisteEqpto(eqptipo_sinap, eqpto_cod);
   if (eqpto == NULL)
      {//envia mensagem de ACK indicando erro
      EnviaMsgAck(msg->Funcao, false);
      //destrói a Msg recebida
      //delete msg;
      return;
      }
	//monta mensagem com identificação do eqpto (OBS: tipo de eqpto do protocolo)
	IniciaStrHMS(Now(), hms);
	msg_tx = protocolo->MontaMsgExisteEqpto(eqptipo_proto, eqpto->Id, eqpto_cod, &hms);
	//envia mensagem para o Client
	EnviaMensagem(msg_tx);
	//destrói a Msg recebida
   //delete msg;
	}
/*
//---------------------------------------------------------------------------
void __fastcall TSinapServer::TrataMsgManobraChave(VTMsg *msg)
	{
	//variáveis locais
	VTChave          *chave;
	TMsgManobraChave *msg_manobra = (TMsgManobraChave*)msg;
	VTEdita          *edita       = (VTEdita*)apl->GetObject(__classid(VTEdita));
	VTRedes          *redes       = (VTRedes*)apl->GetObject(__classid(VTRedes));

	//salva hora/minuto/segundo indicado na msg
	msg->HoraMinutoSegundo(hms);
	//determina Chave a ser manobrada
	chave = (VTChave*)redes->ExisteEqpto(eqptoCHAVE, msg_manobra->EqptoId());
	if (chave != NULL)
		{//verifica se o estado da Chave é diferente do indicado na manobra
		if (chave->Estado != msg_manobra->pcte.estado)
			{//IMPORTANTE: reordena lisSAE da supervisão automática
			ReordenaLisEqptoSupervisionado(chave);
			//altera estado da Chave
			edita->Manobra(chave);
			//executa cálculo de fluxo ou curto
			//ExecutaCalculo();
			}
		}
	//destrói a Msg recebida
   //delete msg;
   }
*/
//---------------------------------------------------------------------------
void __fastcall TSinapServer::TrataMsgManobraChave(VTMsg *msg)
	{
	//variáveis locais
	VTChave          *chave, *clone;
	TMsgManobraChave *msg_manobra = (TMsgManobraChave*)msg;
	VTEdita          *edita       = (VTEdita*)apl->GetObject(__classid(VTEdita));
	VTRedes          *redes       = (VTRedes*)apl->GetObject(__classid(VTRedes));

	//salva hora/minuto/segundo indicado na msg
	msg->HoraMinutoSegundo(hms);
	//determina Chave a ser manobrada
	chave = (VTChave*)redes->ExisteEqpto(eqptoCHAVE, msg_manobra->EqptoId());
	if (chave != NULL)
		{//verifica se o estado da Chave é diferente do indicado na manobra
		if (chave->Estado != msg_manobra->pcte.estado)
			{//IMPORTANTE: reordena lisSAE da supervisão automática
			ReordenaLisEqptoSupervisionado(chave);
	      //configura estado da Chave
	      clone = chave->Clone();
         if (chave->Fechada)
            {//abre a Chave, com indicação de lcokout
	         clone->Estado                   = chvABERTA;
	         clone->Status[sttCHAVE_LOCKOUT] = 1;
	         clone->Status[sttCHAVE_CBR]     = 0;
	         clone->Status[sttCHAVE_CBTL]    = 0;
            }
         else
            {//fecha a Chave
	         clone->Estado                   = chvFECHADA;
	         clone->Status[sttCHAVE_LOCKOUT] = 0;
	         clone->Status[sttCHAVE_CBR]     = 0;
	         clone->Status[sttCHAVE_CBTL]    = 0;
            }
	      //altera a Chave na rede
	      edita->Altera(chave, clone);
			}
		}
	//destrói a Msg recebida
   //delete msg;
   }

//---------------------------------------------------------------------------
void __fastcall TSinapServer::TrataMsgSimulaFalta(VTMsg *msg)
	{
	//variáveis locais
	TMsgSimulaFalta *msg_falta = (TMsgSimulaFalta*)msg;
	VTPatamares     *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
	VTRedes         *redes     = (VTRedes*)apl->GetObject(__classid(VTRedes));

	//salva hora/minuto/segundo indicado na msg
	msg->HoraMinutoSegundo(hms);
	//determina tipo de eqpto
	if (msg->EqptoTipo == teBARRA)
      {//define defeito em Barra
      defeito.trecho       = NULL;
      defeito.dist_bar1_km = 0.;
      defeito.barra        = (VTBarra*)redes->ExisteEqpto(eqptoBARRA, msg->EqptoId);
      defeito.patamar      = patamares->ExistePatamar(hms.hora, hms.minuto);
      defeito.tipo         = TraduzTipoFalha_TipoDefeito(msg_falta->pcte.tipo_falha);
      defeito.pre_falta    = (msg_falta->pcte.pre_falta == 1);
      }
   else if (msg->EqptoTipo == teTRECHO)
      {//define defeito em Trecho
      defeito.barra        = NULL;
      defeito.trecho       = (VTTrecho*)redes->ExisteEqpto(eqptoTRECHO, msg->EqptoId);
      defeito.dist_bar1_km = msg_falta->pcte.dist_bar1_pu * defeito.trecho->Comprimento_km;
      defeito.patamar      = patamares->ExistePatamar(hms.hora, hms.minuto);
      defeito.tipo         = TraduzTipoFalha_TipoDefeito(msg_falta->pcte.tipo_falha);
      defeito.pre_falta    = (msg_falta->pcte.pre_falta == 1);
      }
   else
      {//envia mensagem de ACK indicando erro
      EnviaMsgAck(msg->Funcao, false);
      }
   //proteção
   if (((defeito.barra == NULL)&&(defeito.trecho == NULL)) || (defeito.patamar == NULL))
      {//envia mensagem de ACK indicando erro
      EnviaMsgAck(msg->Funcao, false);
      }
	//atualiza estado de cálculo
   estado = calculoCURTO;
   //executa cálculo de curto
   //ExecutaCalculo();
   //executa cálculo e envia dados dos Eqptos configurados para supervisão automática
   supervisao_auto.ActionRedeChanged->Execute();
	//destrói a Msg recebida
	//delete msg;
	}

//---------------------------------------------------------------------------
void __fastcall TSinapServer::TrataMsgSolicitacaoRegistro(VTMsg *msg)
	{
	//salva hora/minuto/segundo indicado na msg
	msg->HoraMinutoSegundo(hms);
	//envia msg com dados do Eqpto
	if (! EnviaMsgRegistro(msg->EqptoTipo, msg->EqptoId))
		{
		EnviaMsgAck(msg->Funcao, false);
		}
	//destrói a Msg recebida
	//delete msg;
	}

//---------------------------------------------------------------------------
void __fastcall TSinapServer::TrataMsgSupervisaoAutomatica(VTMsg *msg)
	{
	//variáveis locais
	TMsgSupervisaoAuto *msg_supervisao = (TMsgSupervisaoAuto*)msg;

	//salva hora/minuto/segundo indicado na msg
	msg->HoraMinutoSegundo(hms);
	//habilita/desabilita supervisão Enabled
	supervisao_auto.client_enabled = msg_supervisao->Enabled;
	//trata supervisão
	SupervisaoAutomatica();
	}

//---------------------------------------------------------------------------
int __fastcall TSinapServer::TraduzTipoFalha_TipoDefeito(int tipo_falha)
	{
	switch(tipo_falha)
      {
      case falta3F:      return(defeito3F);
      case falta2F_AB:   return(defeito2F);
      case falta1FT_AT:  return(defeitoFT);
      case falta1FTz_AT: return(defeitoFTZ);
      case falta2FT_BCT: return(defeito2FT);
      default:           return(defeito3F);
      }
   }
//---------------------------------------------------------------------------
//eof
