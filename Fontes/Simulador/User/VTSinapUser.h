//-----------------------------------------------------------------------------
#ifndef VTSinapUserH
#define VTSinapUserH

//-----------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
//#include <StdCtrls.hpp>

//-----------------------------------------------------------------------------
class VTApl;
class VTMsg;

//-----------------------------------------------------------------------------
class VTSinapUser  : public TObject
   {
   public:
                    __fastcall  VTSinapUser(void) {};
      virtual       __fastcall ~VTSinapUser(void) {};
		virtual void  __fastcall  HabilitaLog(TStrings *lines_rx, TStrings *lines_tx, TStatusPanel *nome_patamar=NULL) = 0;
		virtual void  __fastcall  LogPatamar(AnsiString txt) = 0;
		virtual void  __fastcall  LogRx(AnsiString txt) = 0;
		virtual void  __fastcall  LogTx(AnsiString txt) = 0;
		virtual void  __fastcall  SinapCapacitorEstado(AnsiString codigo, int estado) = 0;
		virtual void  __fastcall  SinapCapacitorQmvar(AnsiString codigo, double q_mvar) = 0;
		virtual void  __fastcall  SinapChaveCBR(AnsiString codigo, int cbr) = 0;
		virtual void  __fastcall  SinapChaveCBTL(AnsiString codigo, int cbtl) = 0;
		virtual void  __fastcall  SinapChaveCorrente(AnsiString codigo, int fase, double i_amp) = 0;
		virtual void  __fastcall  SinapChaveEstado(AnsiString codigo, int estado) = 0;
		virtual void  __fastcall  SinapChaveLockout(AnsiString codigo, int lockout) = 0;
		virtual void  __fastcall  SinapChaveS1f_mva(AnsiString codigo, int fase, double p_mw, double q_mvar) = 0;
		virtual void  __fastcall  SinapChaveS3f_mva(AnsiString codigo, double p_mw, double q_mvar) = 0;
		virtual void  __fastcall  SinapChaveTensao(AnsiString codigo, int fase, double vfn_kv) = 0;
		virtual void  __fastcall  SinapTrafoCorrente(AnsiString codigo, int enrol, int fase, double i_amp) = 0;
		virtual void  __fastcall  SinapTrafoTap(AnsiString codigo, int tap) = 0;
		virtual void  __fastcall  SinapTrafoTensao(AnsiString codigo, int enrol, int fase, double vfn_kv) = 0;
		virtual bool  __fastcall  TrataMsg(AnsiString eqpto_codigo, VTMsg *msg) = 0;
   };

//-----------------------------------------------------------------------------
// função para criar objeto VTSinapUser
//-----------------------------------------------------------------------------
VTSinapUser* __fastcall NewObjSinapUser(VTApl *apl_owner);

#endif
//---------------------------------------------------------------------------
//eof
