//---------------------------------------------------------------------------
#ifndef TCaboH
#define TCaboH

//arquivos inclu�dos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTCabo.h"

//---------------------------------------------------------------------------
class TCabo : public VTCabo
   {
   protected: //dados acessados via property
      struct   {
               int    tipo_cfg;    //ver enumCFG_CABO acima
               double iadm;        //corrente admiss�vel (A)
               double vmin;        //tens�o m�nima em que � utilizado (kV)
               double vmax;        //tens�o m�xima em que � utilizado (kV)
               } PD;

   public: //m�todos
                 __fastcall TCabo(void);
                 __fastcall ~TCabo(void);
      VTCabo*    __fastcall Clone(void);
		void       __fastcall CopiaAtributosDe(VTEqpto &ref);
		int        __fastcall Tipo(void);
      AnsiString __fastcall TipoAsString(void);

   protected: //m�todos das classe base redefinidos
      void      __fastcall PM_SetCodigo(AnsiString codigo);

   protected: //m�todos acessados via property
      double   __fastcall PM_GetIadm_A(void);
      int      __fastcall PM_GetTipoCfg(void);
      double   __fastcall PM_GetVmax_KV(void);
      double   __fastcall PM_GetVmin_KV(void);
      void     __fastcall PM_SetIAdm_A(double iadm);
      void     __fastcall PM_SetTipoCfg(int tipo_cfg);
      void     __fastcall PM_SetVmax_KV(double vmax);
      void     __fastcall PM_SetVmin_KV(double vmin);
   };

//---------------------------------------------------------------------------
#endif
//eof

