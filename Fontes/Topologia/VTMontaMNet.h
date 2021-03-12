//---------------------------------------------------------------------------
#ifndef VTMontaMNetH
#define VTMontaMNetH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTRedes;

//---------------------------------------------------------------------------
class VTMontaMNet : public TObject
   {
   public:
                   __fastcall  VTMontaMNet(void) {};
                   __fastcall ~VTMontaMNet(void) {};
      virtual void __fastcall  Executa(VTRedes *redes) = 0;
      //função usada p/ criar e ordenar uma lista de MNets
      virtual void __fastcall  ExecutaLisMNetOrdenada(VTRedes *redes, TList *lisEXT_ORD, TList *lisEXT_ISO) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTMontaMNet
//---------------------------------------------------------------------------
VTMontaMNet* __fastcall NewObjMontaMNet(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof



 