//---------------------------------------------------------------------------
#ifndef VTMontaH
#define VTMontaH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTMNet;
class VTNet;
class VTRede;
class VTRedes;

//---------------------------------------------------------------------------
class VTMonta : public TObject
   {
  public:
      __property bool IsoladoEnabled = {read=PD.isolado_enabled, write=PD.isolado_enabled};

   public:
                     __fastcall VTMonta(void) {};
                     __fastcall ~VTMonta(void) {};
      virtual VTNet* __fastcall Executa(TList *lisBAR, TList *lisEQB,
                                        TList *lisLIG, TList *lisMUTUA) = 0;
      virtual VTNet* __fastcall Executa(VTMNet *mnet) = 0;
      virtual VTNet* __fastcall Executa(VTRede *rede) = 0;
      virtual VTNet* __fastcall Executa(VTRedes *redes) = 0;

   protected:  //dados acessados via property
      struct   {
               bool isolado_enabled;
               }PD;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTMonta
//---------------------------------------------------------------------------
VTMonta* __fastcall NewObjMonta(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof



 