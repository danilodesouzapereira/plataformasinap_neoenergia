//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TEqptoLiga.h"
#include "TLigaSerie.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
__fastcall TEqptoLiga::TEqptoLiga(VTEqpto* eqpto)
   {
   this->eqpto = eqpto;
   lisLiga = new TList();
   nlig = 0;       //número de barras conectadas a eqpto (barra)
   }
//---------------------------------------------------------------------------
__fastcall TEqptoLiga::~TEqptoLiga(void)
   {
   if(lisLiga) {delete lisLiga; lisLiga = NULL;}
   }
//---------------------------------------------------------------------------
bool __fastcall TEqptoLiga::AddLiga(VTLigacao* pliga, int nl)
   {
   //Verifica se a arvore já foi incluída
   if(lisLiga->IndexOf(pliga) != -1) return(false);
   lisLiga->Add(pliga);
   nlig += nl;

   return(true);
   }
//---------------------------------------------------------------------------
