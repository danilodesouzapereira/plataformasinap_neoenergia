//---------------------------------------------------------------------------
#ifndef TCelulaH
#define TCelulaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTCelula.h"

//---------------------------------------------------------------------------
class VTLayer;

//---------------------------------------------------------------------------
class TCelula : public VTCelula
   {
   public:
           __fastcall  TCelula(void);
           __fastcall ~TCelula(void);
      void __fastcall  InsereEqptoRede(VTRede *rede, int tipo_layer, VTEqpto *eqpto);
      void __fastcall  InsereEqptoReducao(VTReducao *reducao, int tipo_layer, VTEqpto *eqpto);
      void __fastcall  LisEqpto(TList *lisEXT, int tipo);
      void __fastcall  LisEqptoRede(TList *lisEXT, int tipo_layer, VTRede *rede, int tipo);
      void __fastcall  LisEqptoReducao(TList *lisEXT, int tipo_layer, VTReducao *reducao, int tipo);
      void __fastcall  LisEqptoVisivel(TList *lisEXT, int tipo);
      void __fastcall  LisRede(TList *lisEXT);
      void __fastcall  LisReducao(TList *lisEXT);
      int  __fastcall  QtdeEqptoRede(int tipo_layer, VTRede *rede, int tipo);

   private: //métodos
      VTLayer* __fastcall ExisteLayerRede(VTRede *rede, int tipo_layer);
      VTLayer* __fastcall ExisteLayerReducao(VTReducao *reducao, int tipo_layer);
      void     __fastcall LisEqptoRedeAllLayer(TList *lisEXT, int tipo_layer, int tipo);
      void     __fastcall LisEqptoReducaoAllLayer(TList *lisEXT, int tipo_layer, int tipo);
      int      __fastcall QtdeEqptoRedeAllLayer(int tipo_layer, int tipo);

   private: //dados locais
      TList   *lisLAYER_REDE;
      TList   *lisLAYER_REDU;
      VTLayer *ult_layer;
   };

//---------------------------------------------------------------------------
#endif
//eof
