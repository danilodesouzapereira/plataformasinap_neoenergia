//---------------------------------------------------------------------------
#ifndef VTFlowNetH
#define VTFlowNetH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTMNet;
class VTNet;

//---------------------------------------------------------------------------
class VTFlowNet : public TObject
   {
   public:
                   __fastcall  VTFlowNet(void) {};
                   __fastcall ~VTFlowNet(void) {};
      virtual bool __fastcall  Executa(VTMNet *mnet) = 0;
      virtual bool __fastcall  Executa(VTNet *net) = 0;
   };

//-----------------------------------------------------------------------------
// função global para criar objeto VTFlowNet
//-----------------------------------------------------------------------------
VTFlowNet* NewObjFlowNet(VTApl* apl);

#endif
//---------------------------------------------------------------------------
//eof
