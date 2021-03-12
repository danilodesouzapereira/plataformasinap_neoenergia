//-----------------------------------------------------------------------------
#ifndef TSinapUserH
#define TSinapUserH

//-----------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTSinapUser.h"

//---------------------------------------------------------------------------
//class VTMsg;

//---------------------------------------------------------------------------
class TSinapUser : public VTSinapUser
   {
   public:
            __fastcall  TSinapUser(VTApl *apl_owner);
            __fastcall ~TSinapUser(void);
		void  __fastcall  HabilitaLog(TStrings *lines_rx, TStrings *lines_tx, TStatusPanel *nome_patamar);
		void  __fastcall  LogPatamar(AnsiString txt);
		void  __fastcall  LogRx(AnsiString txt);
		void  __fastcall  LogTx(AnsiString txt);
		void  __fastcall  SinapCapacitorEstado(AnsiString codigo, int estado);
		void  __fastcall  SinapCapacitorQmvar(AnsiString codigo, double q_mvar);
		void  __fastcall  SinapChaveCBR(AnsiString codigo, int cbr);
		void  __fastcall  SinapChaveCBTL(AnsiString codigo, int cbtl);
		void  __fastcall  SinapChaveCorrente(AnsiString codigo, int fase, double i_amp);
		void  __fastcall  SinapChaveEstado(AnsiString codigo, int estado);
		void  __fastcall  SinapChaveLockout(AnsiString codigo, int lockout);
		void  __fastcall  SinapChaveS1f_mva(AnsiString codigo, int fase, double p_mw, double q_mvar);
		void  __fastcall  SinapChaveS3f_mva(AnsiString codigo, double p_mw, double q_mvar);
		void  __fastcall  SinapChaveTensao(AnsiString codigo, int fase, double vfn_kv);
		void  __fastcall  SinapTrafoCorrente(AnsiString codigo, int enrol, int fase, double i_amp);
		void  __fastcall  SinapTrafoTap(AnsiString codigo, int tap);
		void  __fastcall  SinapTrafoTensao(AnsiString codigo, int enrol, int fase, double vfn_kv);
		bool  __fastcall  TrataMsg(AnsiString eqpto_codigo, VTMsg *msg);

   private: //métodos

   private: //objetos externos
      VTApl *apl;
		struct{
				TStatusPanel *nome_patamar;
				TStrings     *lines_rx;
				TStrings     *lines_tx;
            }log;

   private: //dados locais
      #define LOG_COUNT_MAX   200
   };

#endif
//---------------------------------------------------------------------------
//eof
