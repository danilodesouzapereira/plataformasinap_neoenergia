//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TDrawFlow.h"
#include "TPadrao.h"
#include "..\Legenda\VTLegenda.h"
#include "..\Rede\VTEqpto.h"
#include "..\..\DLL_Inc\Legenda.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//-----------------------------------------------------------------------------
// função global
//-----------------------------------------------------------------------------
VTDraw* NewObjDrawFlow(VTApl *apl)
   {
   return(new TDrawFlow(apl));
   }

//---------------------------------------------------------------------------
__fastcall TDrawFlow::TDrawFlow(VTApl *apl)
                     :TDrawEditor(apl)
   {
   try{//cria objetos
      legenda = DLL_NewObjLegenda(apl);
      } catch (Exception &e)
         {//nada fazer
         }
   }

//---------------------------------------------------------------------------
__fastcall TDrawFlow::~TDrawFlow(void)
   {
   //destrói objetos
   if (legenda) {delete legenda; legenda = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TDrawFlow::MontaLegenda(VTEqpto *eqpto, int x, int y, TColor cor)
   {
   //apresenta a legenda
   if (legenda == NULL) return;
   if ((! legenda_enabled)&&(! eqpto->Show[eqp_FIXO])) return;
   //exibe legenda
   legenda->LisValores(eqpto, lisDADO, padrao->flow.ano_carga, padrao->flow.num_pat);
   ExibeLegenda(x, y, cor);
   //ExibeLegenda(x, y, clWhite);   //clGray;
   }

//---------------------------------------------------------------------------
//eof


