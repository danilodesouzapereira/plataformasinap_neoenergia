//---------------------------------------------------------------------------
#ifndef TSpyH
#define TSpyH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTSpy.h"

//---------------------------------------------------------------------------
class TFormEqpto;

//---------------------------------------------------------------------------
class TSpy : public VTSpy
   {
   public:
           __fastcall TSpy(VTApl *AplOwner);
           __fastcall ~TSpy(void);
      void __fastcall ExibeFormEqpto(TComponent *Owner, TWinControl *parent);
      void __fastcall DefineEqpto(VTEqpto *eqpto);

   private: //métodos

   private: //objetos externos
      VTApl *apl;

   private: //dados locais
      TFormEqpto *FormEqpto;
   };

#endif
//---------------------------------------------------------------------------
//eof
