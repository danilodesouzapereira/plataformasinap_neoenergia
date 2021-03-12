//---------------------------------------------------------------------------
#ifndef VTWebPlanH
#define VTWebPlanH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;

//---------------------------------------------------------------------------
class VTWebPlan : public TObject
   {
   public:
                         __fastcall  VTWebPlan(void) {};
      virtual            __fastcall ~VTWebPlan(void) {};
      virtual bool       __fastcall  EnviaEstudo(AnsiString arq_txt) = 0;
      virtual AnsiString __fastcall  MsgErroWebService(void) = 0;
   };

//---------------------------------------------------------------------------
//função para criar objeto VTWebPlan
VTWebPlan *NewObjWebPlan(VTApl *apl);

#endif
//---------------------------------------------------------------------------
//eof
