//-----------------------------------------------------------------------------
#ifndef VTWizardH
#define VTWizardH

//-----------------------------------------------------------------------------
#include <Classes.hpp>

//-----------------------------------------------------------------------------
class VTApl;

//-----------------------------------------------------------------------------
class VTWizard  : public TObject
   {
   public:
                    __fastcall VTWizard(void) {};
      virtual       __fastcall ~VTWizard(void) {};
      virtual bool  __fastcall Executa(TWinControl *wincrtl, int x, int y, TColor cor_fundo=clBlack) = 0;
   };

//-----------------------------------------------------------------------------
// função para criar objeto VTWizard
//-----------------------------------------------------------------------------
VTWizard* __fastcall NewObjWizard(VTApl *apl);

#endif
//---------------------------------------------------------------------------
//eof
