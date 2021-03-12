//---------------------------------------------------------------------------
#ifndef VTWebPlanWSH
#define VTWebPlanWSH

//---------------------------------------------------------------------------
#include <Classes.hpp>
class VTApl;
//---------------------------------------------------------------------------
class VTWebPlanWS: public TObject
   {
   public:
                   __fastcall  VTWebPlanWS(void) {};
      virtual      __fastcall ~VTWebPlanWS(void) {};
      virtual bool __fastcall  ImportaPadrao(AnsiString arq_txt) = 0;
      virtual bool __fastcall  ImportaGrupo(AnsiString arq_txt) = 0;
   };

//---------------------------------------------------------------------------
//função para criar objeto VTWebPlanPadrao
VTWebPlanWS *NewObjWebPlanWS(VTApl *apl);

#endif
//---------------------------------------------------------------------------
//eof
