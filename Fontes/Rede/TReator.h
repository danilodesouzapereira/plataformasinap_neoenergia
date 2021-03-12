//---------------------------------------------------------------------------
#ifndef TReatorH
#define TReatorH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTReator.h"

//---------------------------------------------------------------------------
class TReator : public VTReator
   {
   public:
                 __fastcall  TReator(int num_pat);
                 __fastcall ~TReator(void);
      VTReator*  __fastcall  Clone(void);
      void       __fastcall  CopiaAtributosDe(VTEqpto &ref);
      void       __fastcall  DefineCurva(VTCurva *curva);
      VTCurva*   __fastcall  ExisteCurva(void);
      int        __fastcall  Tipo(void);
      AnsiString __fastcall  TipoAsString(void);
      void       __fastcall  Z0_pu(double Sbase, double Vbase, strIMP &z0_pu);
      void       __fastcall  Z1_pu(double Sbase, double Vbase, strIMP &z1_pu);

   private: //métodos acessados via property
      AnsiString  __fastcall PM_GetPadrao(void);
      double      __fastcall PM_GetQ_mvar(void);
      void        __fastcall PM_SetPadrao(AnsiString padrao);

   private:
      AnsiString padrao;
   };
   
//---------------------------------------------------------------------------
#endif
//eof

 