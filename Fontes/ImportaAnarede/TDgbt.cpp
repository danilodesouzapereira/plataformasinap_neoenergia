//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TDgbt.h"
#include "TDbar.h"

//---------------------------------------------------------------------------
__fastcall TDgbt::TDgbt(void)
   {
   //cria lista de Barras
   //lisDBAR = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TDgbt::~TDgbt(void)
   {
   //destr�i lista sem destruir seus objetos
   //if (lisDBAR) {delete lisDBAR; lisDBAR = NULL;}
   }
/*
//-----------------------------------------------------------------------------
bool __fastcall TDgbt::ContemDbar(TDbar *dbar)
   {
   return(lisDBAR->IndexOf(dbar) >= 0);
   }

//-----------------------------------------------------------------------------
void __fastcall TDgbt::InsereDbar(TDbar *dbar)
   {
   //prote��o
   if (dbar == NULL) return;
   //verifica se o Dbar ainda n�o existe em lisDBAR
   if (lisDBAR->IndexOf(dbar) < 0)
      {//insere Dbar na lista
      lisDBAR->Add(dbar);
      //associa Dbar c/ este Dgbt
      dbar->dgbt = this;
      }
   }
*/
//-----------------------------------------------------------------------------
// eof
