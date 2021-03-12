//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TCorte.h"
#include "..\Rede\VTEqpto.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TCorte::TCorte(void)
   {
   lisEqpto = new TList();
   lisChvSoc = new TList();
   }
//---------------------------------------------------------------------------
__fastcall TCorte::~TCorte(void)
   {
   if(lisEqpto) {delete lisEqpto; lisEqpto = NULL;}
   if(lisChvSoc) {delete lisChvSoc; lisChvSoc = NULL;}
   }
//---------------------------------------------------------------------------
AnsiString __fastcall TCorte::Codigo(void)
   {
   VTEqpto* eqpto;
   AnsiString aux;

   aux = "";
   for(int n = 0; n < lisEqpto->Count; n++)
      {
      eqpto = (VTEqpto*)lisEqpto->Items[n];
      aux.cat_sprintf("%s ", eqpto->Codigo.c_str());
      }
   return(aux);
   }
//---------------------------------------------------------------------------

