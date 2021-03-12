//---------------------------------------------------------------------------
#pragma hdrstop
#include "TSecao.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTLigacao.h"
#include "..\..\DLL_Inc\Funcao.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
TSecao * __fastcall NewObjSecao(void)
   {
   return(new TSecao());
   }

//---------------------------------------------------------------------------
__fastcall TSecao::TSecao(void)
   {
   //cria objetos locais
   lisBAR      = new TList();
   lisLIG      = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TSecao::~TSecao(void)
   {
   //destrói lista sem destruir seus objetos
   if (lisBAR) {delete lisBAR; lisBAR = NULL;}
   if (lisLIG) {delete lisLIG; lisLIG = NULL;}
   }

//---------------------------------------------------------------------------
bool __fastcall TSecao::InsereBarra(VTBarra *barra)
   {
   //proteção
   if (barra == NULL) return(false);
   //insere Barra na lista
   if (lisBAR->IndexOf(barra) < 0) lisBAR->Add(barra);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TSecao::InsereLigacao(VTLigacao *ligacao)
   {
   //proteção
   if (ligacao == NULL) return(false);
   //insere Barra na lista
   if (lisLIG->IndexOf(ligacao) < 0) lisLIG->Add(ligacao);

   return(true);
   }

//---------------------------------------------------------------------------
TList* __fastcall TSecao::LisBarra(void)
   {
   return(lisBAR);
   }

//---------------------------------------------------------------------------
TList* __fastcall TSecao::LisLigacao(void)
   {
   return(lisLIG);
   }

//---------------------------------------------------------------------------
//eof

