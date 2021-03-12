//-----------------------------------------------------------------------------
#ifndef TSinapClientH
#define TSinapClientH

//-----------------------------------------------------------------------------
#include <Classes.hpp>
#include <System.Win.ScktComp.hpp>
#include "VTSinapClient.h"

//---------------------------------------------------------------------------
class TMsgRegistroCapacitor;
class TMsgRegistroChave;
class TMsgRegistroTrafo;
class VTMsg;
class VTProtocolo;
class VTSinapUser;
class TEqp;
struct strHMS;

//---------------------------------------------------------------------------
class TSinapClient : public VTSinapClient
	{
   public:
            __fastcall  TSinapClient(VTApl *apl_owner);
            __fastcall ~TSinapClient(void);
      bool  __fastcall  Conecta(AnsiString ip_address, int server_port);
      void  __fastcall  Desconecta(void);
		bool  __fastcall  GetRegistro(int eqpto_tipo, AnsiString eqpto_cod, TDateTime dh);
		bool  __fastcall  GetRegistro(int eqpto_tipo, int eqpto_id,  TDateTime dh);
		bool  __fastcall  ResetFalta(void);
		bool  __fastcall  SetCapacitorEstado(int eqpto_tipo, AnsiString eqpto_cod, bool ligado, TDateTime dh);
		bool __fastcall   SetCapacitorEstado(int eqpto_tipo, int eqpto_id, bool ligado, TDateTime dh);
		bool  __fastcall  SetFalta(int eqpto_tipo, AnsiString eqpto_cod, int tipo_falta, double rdef_ohm,
											 double dist_bar1_pu, bool pre_falta, TDateTime dh);
		bool  __fastcall  SetFalta(int eqpto_tipo, int eqpto_id, int tipo_falta, double rdef_ohm,
											double dist_bar1_pu, bool pre_falta, TDateTime dh);
		bool  __fastcall  SetManobraChave(int eqpto_tipo, AnsiString eqpto_cod, bool abrir, TDateTime dh);
		bool  __fastcall  SetManobraChave(int eqpto_tipo, int eqpto_id, bool abrir, TDateTime dh);
		bool  __fastcall  SetSupervisaoAutomatica(bool enabled);
		bool  __fastcall  SetTrafoTap(int eqpto_tipo, AnsiString eqpto_cod, int variacao_tap, TDateTime dh);
		bool  __fastcall  SetTrafoTap(int eqpto_tipo, int eqpto_id, int variacao_tap, TDateTime dh);

	protected:
		bool  __fastcall  PM_GetAtivo(void);

	private: //métodos de tratamento de eventos do ClientSocket
		void __fastcall ClientSocketConnect(TObject *Sender, TCustomWinSocket *Socket);
		void __fastcall ClientSocketDisconnect(TObject *Sender, TCustomWinSocket *Socket);
		void __fastcall ClientSocketError(TObject *Sender, TCustomWinSocket *Socket,
													 TErrorEvent ErrorEvent, int &ErrorCode);
		void __fastcall ClientSocketRead(TObject *Sender, TCustomWinSocket *Socket);

	private: //métodos
      bool  __fastcall  EnviaAtributosCapacitorToUser(AnsiString eqp_cod, TMsgRegistroCapacitor *msg);
		bool  __fastcall  EnviaAtributosChaveToUser(AnsiString eqp_cod, TMsgRegistroChave *msg);
		bool  __fastcall  EnviaAtributosTrafoToUser(AnsiString eqp_cod, TMsgRegistroTrafo *msg);
		bool  __fastcall  EnviaMensagem(VTMsg *msg);
		bool  __fastcall  EnviaMsgToUser(VTMsg *msg);
		TEqp* __fastcall  ExisteEqp(int eqpto_tipo,int eqpto_id);
		TEqp* __fastcall  ExisteEqp(int eqpto_tipo, AnsiString codigo);
		bool  __fastcall  GetEqp(int eqpto_tipo,AnsiString codigo, TDateTime dh);
		void  __fastcall  IniciaStrHMS(TDateTime datetime, strHMS &hms);
		void  __fastcall  LogRx(AnsiString txt);
		void  __fastcall  LogTx(AnsiString txt);
		bool  __fastcall  TrataRespostaExisteEqpto(VTMsg *msg_rx);

	private: //objetos externos
		VTApl       *apl;
      VTSinapUser *user;

   private: //dados locais
      bool          server_respondeu;
      VTProtocolo   *protocolo;
      TClientSocket *ClientSocket;
      TList         *lisEQP;
      TList         *lisMSG;
   };

#endif
//---------------------------------------------------------------------------
//eof
