//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TDrawCurto.h"
#include "..\Apl\VTApl.h"
#include "..\RedeCC\VTBarCC.h"
#include "..\RedeCC\VTLigCC.h"
#include "..\RedeCC\VTRedeCC.h"
#include "..\ResCC\VTResCC.h"
#include "..\..\DLL_Inc\ResCC.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//-----------------------------------------------------------------------------
// fun��o global
//-----------------------------------------------------------------------------
VTDraw* NewObjDrawCurto(VTApl *apl)
   {
   return(new TDrawCurto(apl));
   }

//---------------------------------------------------------------------------
__fastcall TDrawCurto::TDrawCurto(VTApl *apl)
                      :TDrawEditor(apl)
   {
   try{//cria objetos
      resCC = DLL_NewObjResCC(apl);
      } catch (Exception &e)
         {//nada fazer
         }
   }

//---------------------------------------------------------------------------
__fastcall TDrawCurto::~TDrawCurto(void)
   {
   //destr�i objetos
   if (resCC) {delete resCC; resCC = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TDrawCurto::MontaLegenda(VTEqpto *eqpto, int x, int y, TColor cor)
   {
   /*
   //vari�veis locais
   VTBarCC *barCC;
   VTLigCC *ligCC;

   //se ncess�rio, obt�m RedeCC
   if (redeCC == NULL) redeCC = (VTRedeCC*)apl->GetObject(__classid(VTRedeCC));
   if (redeCC == NULL) return;
   //verifica o tipo do Eqpto
   if (eqpto->TipoBarra())
      {
      if ((barCC = redeCC->ExisteBarCC(eqpto->Id)) == NULL) return;
      resCC->LisDado(barCC, -1, lisDADO);
      ExibeLegenda(x, y, cor);
      }
   else if (eqpto->Tipo() == eqptoTRECHO)
      {
      if ((ligCC = redeCC->ExisteLigCC(eqptoTRECHO, eqpto->Id)) == NULL) return;
      resCC->LisDado(ligCC, -1, lisDADO);
      ExibeLegenda(x, y, cor);
      }
   */
   }

//---------------------------------------------------------------------------
//eof


