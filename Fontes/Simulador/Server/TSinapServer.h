//-----------------------------------------------------------------------------
#ifndef TSinapServerH
#define TSinapServerH

//-----------------------------------------------------------------------------
#include <Classes.hpp>
#include <System.Win.ScktComp.hpp>
#include "VTSinapServer.h"
#include "..\Protocolo\VTProtocolo.h"
#include "..\RedeCC\ResCurto.h"

//---------------------------------------------------------------------------
class VTMsg;
class VTSinapUser;
class VTProtocolo;
class VTBarra;
class VTEqpto;
class VTTrecho;
class VTPatamar;
class VTCurtocircuito;

//---------------------------------------------------------------------------
class TSinapServer : public VTSinapServer
   {
   public:
                 __fastcall  TSinapServer(VTApl *apl_owner);
			        __fastcall ~TSinapServer(void);
		bool       __fastcall  Conecta(int server_port);
		void       __fastcall  Desconecta(void);
      int        __fastcall  LisEqptoSupervisionado(TList *lisEXT, int eqpto_tipo);
		VTPatamar* __fastcall  PatamarAtual(void);
		VTPatamar* __fastcall  SelecionaProximoPatamar(void);
		bool       __fastcall  SupervisaoAutomaticaStart(TList *lisEXT);
		bool       __fastcall  SupervisaoAutomaticaStop(void);

   protected:
		bool  __fastcall  PM_GetAtivo(void);

   private: //métodos de tratamento de eventos do ServerSocket
		void __fastcall ServerSocketAccept(TObject *Sender, TCustomWinSocket *Socket);
      void __fastcall ServerSocketClientConnect(TObject *Sender, TCustomWinSocket *Socket);
      void __fastcall ServerSocketClientDisconnect(TObject *Sender, TCustomWinSocket *Socket);
      void __fastcall ServerSocketClientRead(TObject *Sender, TCustomWinSocket *Socket);

	private:	//métodos
		void       __fastcall ActionRedeChangedExecute(TObject *Sender);
		void       __fastcall EnviaMensagem(VTMsg *msg);
		void       __fastcall EnviaMsgAck(int funcao_acked, bool sucesso);
		void       __fastcall EnviaMsgNAck(int funcao_nacked);
		bool       __fastcall EnviaMsgRegistro(int eqpto_tipo, int eqpto_id);
		void       __fastcall EnviaMsgRegistroBarra(int eqpto_id);
      void       __fastcall EnviaMsgRegistroCapacitor(int eqpto_id);
		void       __fastcall EnviaMsgRegistroChave(int eqpto_id);
		void       __fastcall EnviaMsgRegistroTrafo(int eqpto_id);
		void       __fastcall EnviaMsgRegistroTrecho(int eqpto_id);
		bool       __fastcall ExecutaCalculo(void);
		VTEqpto*   __fastcall ExisteEqpto(int eqpto_tipo, AnsiString codigo);
      void       __fastcall HabilitaPatamarUnico(strHMS &hms);
		void       __fastcall IniciaStrHMS(TDateTime datetime, strHMS &hms);
		void       __fastcall IniciaZdef(void);
		void       __fastcall LogPatamar(AnsiString txt);
		void       __fastcall LogRx(AnsiString txt);
		void       __fastcall LogTx(AnsiString txt);
		void       __fastcall ReordenaLisEqptoSupervisionado(VTEqpto *eqpto);
		bool       __fastcall SupervisaoAutomatica(void);
		void       __fastcall TrataMsgAlteraCapacitor(VTMsg *msg);
		void       __fastcall TrataMsgAlteraCarga(VTMsg *msg);
      void       __fastcall TrataMsgAlteraGerador(VTMsg *msg);
		void       __fastcall TrataMsgAlteraSuprimento(VTMsg *msg);
		void       __fastcall TrataMsgAlteraTrafo(VTMsg *msg);
		void       __fastcall TrataMsgEliminaFalta(VTMsg *msg);
		void       __fastcall TrataMsgExisteEqpto(VTMsg *msg);
      void       __fastcall TrataMsgManobraChave(VTMsg *msg);
      void       __fastcall TrataMsgSimulaFalta(VTMsg *msg);
		void       __fastcall TrataMsgSolicitacaoRegistro(VTMsg *msg);
		void       __fastcall TrataMsgSupervisaoAutomatica(VTMsg *msg);
		int        __fastcall TraduzTipoFalha_TipoDefeito(int tipo_falha);

	private: //objetos externos
      VTApl    *apl;

	private: //dados locais
		TServerSocket   *ServerSocket;
		VTSinapUser     *user;
		VTProtocolo     *protocolo;
		VTCurtocircuito *curtocircuito;
		TList           *lisEQP;
		TList           *lisMSG;

	private: //dados locais
		bool        client_connected;
		int         estado;
		int         calculo_executado;
		strHMS      hms;
		strBUF      buffer;
		struct{
				bool      server_enabled;
				bool      client_enabled;
				TAction   *ActionRedeChanged;
				VTPatamar *patamar;
				TList     *lisSAE;
				}supervisao_auto;
		struct{
            VTBarra   *barra;
            VTTrecho  *trecho;
				VTPatamar *patamar;
				double    dist_bar1_km;
            int       tipo;
            STR_ZDef  Zdef;
            bool      pre_falta;
            }defeito;
   };

#endif
//---------------------------------------------------------------------------
//eof
