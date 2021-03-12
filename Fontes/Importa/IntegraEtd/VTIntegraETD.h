//-----------------------------------------------------------------------------
#ifndef VTIntegraEtdH
#define VTIntegraEtdH

//-----------------------------------------------------------------------------
#include <Classes.hpp>

//-----------------------------------------------------------------------------
class VTApl;

//-----------------------------------------------------------------------------
class VTIntegraEtd  : public TObject
   {
   public:
                   __fastcall  VTIntegraEtd(void) {};
      virtual      __fastcall ~VTIntegraEtd(void) {};
      virtual bool __fastcall  InsereETDcompleta(void) = 0;
      virtual bool __fastcall  InsereETDsimplificada(void) = 0;
      virtual bool __fastcall  InsereSoSuprimentos(void) = 0;
   };

//-----------------------------------------------------------------------------
// função para criar objeto VTIntegraEtd
//-----------------------------------------------------------------------------
VTIntegraEtd* NewObjIntegraEtd(VTApl *apl);

#endif
//---------------------------------------------------------------------------
//eof
