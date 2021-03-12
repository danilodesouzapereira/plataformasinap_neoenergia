//---------------------------------------------------------------------------
#ifndef TCaboH
#define TCaboH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTCabo.h"

//---------------------------------------------------------------------------
class TCabo : public VTCabo
   {
   protected: //dados acessados via property
      struct   {
               int    tipo_cfg;    //ver enumCFG_CABO acima
               double iadm;        //corrente admissível (A)
               double vmin;        //tensão mínima em que é utilizado (kV)
               double vmax;        //tensão máxima em que é utilizado (kV)
               } PD;

   public: //métodos
                 __fastcall TCabo(void);
                 __fastcall ~TCabo(void);
      VTCabo*    __fastcall Clone(void);
		void       __fastcall CopiaAtributosDe(VTEqpto &ref);
		int        __fastcall Tipo(void);
      AnsiString __fastcall TipoAsString(void);

   protected: //métodos das classe base redefinidos
      void      __fastcall PM_SetCodigo(AnsiString codigo);

   protected: //métodos acessados via property
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

