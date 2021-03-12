//---------------------------------------------------------------------------
#ifndef VTCelulaH
#define VTCelulaH
      
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "..\Rede\VTEqpto.h"

//---------------------------------------------------------------------------
class VTArea;
class VTEqpto;
class VTRede;
class VTReducao;

//---------------------------------------------------------------------------
class VTCelula : public TObject
   {
   public:  //property
      __property TColor  Color           = {read=PD.color   , write=PD.color};
      __property double  ValorProprio    = {read=PD.valor   , write=PD.valor};
      __property double  ValorTotal      = {read=PD.total   , write=PD.total};
      __property int     NumParcela      = {read=PD.nparc   , write=PD.nparc};
      __property VTArea* Area            = {read=PD.area};

   public:
                   __fastcall  VTCelula(void) {};
      virtual      __fastcall ~VTCelula(void) {};
      virtual void __fastcall  InsereEqptoRede(VTRede *rede, int tipo_layer, VTEqpto *eqpto) = 0;
      virtual void __fastcall  InsereEqptoReducao(VTReducao *reducao, int tipo_layer, VTEqpto *eqpto) = 0;
      virtual void __fastcall  LisEqpto(TList *lisEXT, int tipo=eqptoINDEF) = 0;
      virtual void __fastcall  LisEqptoRede(TList *lisEXT, int tipo_layer, VTRede *rede=NULL, int tipo=eqptoINDEF) = 0;
      virtual void __fastcall  LisEqptoReducao(TList *lisEXT, int tipo_layer, VTReducao *reducao=NULL, int tipo=eqptoINDEF) = 0;
      virtual void __fastcall  LisRede(TList *lisEXT) = 0;
      virtual void __fastcall  LisReducao(TList *lisEXT) = 0;
      virtual void __fastcall  LisEqptoVisivel(TList *lisEXT, int tipo=eqptoINDEF) = 0;
      virtual int  __fastcall  QtdeEqptoRede(int tipo_layer, VTRede *rede=NULL, int tipo=eqptoINDEF) = 0;

   protected:
      struct   {
               double valor;
               double total;
               int    nparc;
               TColor color;
               VTArea *area;
               }PD;
   };

//-----------------------------------------------------------------------------
// funções globais para criar objetos da classe VTCelula
//-----------------------------------------------------------------------------
VTCelula* NewObjCelula(void);

#endif
//---------------------------------------------------------------------------
//eof
