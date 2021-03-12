//---------------------------------------------------------------------------
#ifndef VTWebPlanPadraoH
#define VTWebPlanPadraoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
class VTApl;
//---------------------------------------------------------------------------
class VTWebPlanPadrao : public TObject
   {
   public:
                   __fastcall  VTWebPlanPadrao(void) {};
      virtual      __fastcall ~VTWebPlanPadrao(void) {};
      virtual bool __fastcall  ImportaPadrao(AnsiString arq_txt) = 0;
   };

//---------------------------------------------------------------------------
//função para criar objeto VTWebPlanPadrao
VTWebPlanPadrao *NewObjWebPlanPadrao(VTApl *apl);

#endif
//---------------------------------------------------------------------------
//eof
