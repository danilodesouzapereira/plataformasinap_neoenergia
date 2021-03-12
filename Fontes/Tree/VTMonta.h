//---------------------------------------------------------------------------
#ifndef VTMontaH
#define VTMontaH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTRede;
class VTNet;
class VTCorte;

//---------------------------------------------------------------------------
class VTMonta : public TObject
   {
   public:
                     __fastcall VTMonta(void) {};
                     __fastcall ~VTMonta(void) {};
      virtual VTNet* __fastcall Net(VTRede *rede) = 0;
      virtual VTNet* __fastcall Net(TList *lisREDE) = 0;
      virtual VTNet* __fastcall Net(TList *lisREDE, TList *lisREDU) = 0;
      virtual VTNet* __fastcall NetParaReducao(VTRede *rede, VTCorte *corte) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTMonta
//---------------------------------------------------------------------------
VTMonta* __fastcall NewObjMonta(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof



 