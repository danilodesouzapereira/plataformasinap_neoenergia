//---------------------------------------------------------------------------
#ifndef VTSpyH
#define VTSpyH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <ComCtrls.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTEqpto;

//---------------------------------------------------------------------------
class VTSpy : public TObject
   {
   public:
                   __fastcall VTSpy(void) {};
      virtual      __fastcall ~VTSpy(void) {};
      virtual void __fastcall ExibeFormEqpto(TComponent *Owner, TWinControl *parent) = 0;
      virtual void __fastcall DefineEqpto(VTEqpto *eqpto) = 0;
   };

//---------------------------------------------------------------------------
//função para criar objeto VTSpy
VTSpy* __fastcall NewObjSpy(VTApl *AplOwner);

#endif
//---------------------------------------------------------------------------
//eof
