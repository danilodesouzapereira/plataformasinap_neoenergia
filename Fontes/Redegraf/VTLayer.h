//---------------------------------------------------------------------------
#ifndef VTLayerH
#define VTLayerH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "..\Rede\VTEqpto.h"

//---------------------------------------------------------------------------
class VTRede;
class VTReducao;

//---------------------------------------------------------------------------
enum tipoLAYER {layerREDE=1, layerREDU_ESQ, layerREDU_EQPTO};

//---------------------------------------------------------------------------
class VTLayer : public TObject
   {
   public:  //property
      __property int        Tipo    = {read=PD.tipo,    write=PD.tipo   };
      __property VTRede*    Rede    = {read=PD.rede,    write=PD.rede   };
      __property VTReducao* Reducao = {read=PD.reducao, write=PD.reducao};

   public:
                   __fastcall VTLayer(void) {};
      virtual      __fastcall ~VTLayer(void) {};
      virtual void __fastcall InsereEqpto(VTEqpto *eqpto) = 0;
      virtual void __fastcall LisBarra(TList   *lisEXT) = 0;
      virtual void __fastcall LisEqbar(TList   *lisEXT, int tipo=eqptoINDEF) = 0;
      virtual void __fastcall LisEqpto(TList   *lisEXT, int tipo=eqptoINDEF) = 0;
		virtual void __fastcall LisLigacao(TList *lisEXT, int tipo=eqptoINDEF) = 0;
		virtual void __fastcall LisMutua(TList *lisEXT) = 0;
      virtual int  __fastcall QtdeEqpto(int tipo=eqptoINDEF) = 0;

   protected:
      struct   {
               int       tipo;
               VTRede    *rede;
               VTReducao *reducao;
               }PD;
   };

//-----------------------------------------------------------------------------
// funções globais para criar objetos da classe VTLayer
//-----------------------------------------------------------------------------
VTLayer* NewObjLayer(int tipo);

#endif
//---------------------------------------------------------------------------
//eof
