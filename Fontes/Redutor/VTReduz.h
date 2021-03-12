//---------------------------------------------------------------------------
#ifndef VTReduzH
#define VTReduzH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTCorte;
class VTMNet;
class VTMRede;
class VTRede;
class VTReducao;
class VTRedes;

//---------------------------------------------------------------------------
class VTReduz : public TObject
   {
   public:  //property
      __property int  ModeloEqv = {read=PD.modelo_eqv, write=PD.modelo_eqv};

   public:
                         __fastcall  VTReduz(void) {};
                         __fastcall ~VTReduz(void) {};
      virtual VTReducao* __fastcall  Executa(VTMNet *mnet, VTMRede *mrede, VTCorte *corte, bool aviso_enabled) = 0;
      virtual bool       __fastcall  Executa(VTRede *rede, TList *lisBAR, bool aviso_enabled) = 0;
      virtual bool       __fastcall  Executa(VTRedes *redes, TList *lisBAR, bool aviso_enabled) = 0;

   protected:  //dados acessados via property
      struct{
            int modelo_eqv;
            }PD;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTReduz
//---------------------------------------------------------------------------
VTReduz* __fastcall NewObjReduzBarra(VTApl *apl);
VTReduz* __fastcall NewObjReduzCarga(VTApl *apl);
VTReduz* __fastcall NewObjReduzRede(VTApl *apl);
VTReduz* __fastcall NewObjReduzSuprimento(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof



 