//---------------------------------------------------------------------------
#ifndef VTReduzH
#define VTReduzH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTMNet;

//---------------------------------------------------------------------------
class VTReduz : public TObject
   {
   public:  //property
      __property int  ModeloCarga = {read=PD.modelo_carga,  write=PD.modelo_carga};

   public:
                   __fastcall  VTReduz(void) {};
                   __fastcall ~VTReduz(void) {};
      virtual bool __fastcall  ReiniciaReducaoCarga(VTMNet *mnet) = 0;
      virtual bool __fastcall  ReiniciaReducaoSuprimento(VTMNet *mnet) = 0;
      virtual bool __fastcall  ReiniciaReducaoSuprimento1pu(VTMNet *mnet) = 0;

   protected:  //dados acessados via property
      struct   {
               int modelo_carga;
               }PD;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTReduz
//---------------------------------------------------------------------------
VTReduz* __fastcall NewObjReduz(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof



