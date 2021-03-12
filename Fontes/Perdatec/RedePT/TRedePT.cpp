//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TRedePT.h"
#include "..\..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTRedePT* __fastcall NewObjRedePT(void)
   {
   try{
      return(new TRedePT());
      }catch(Exception &e)
         {
         }
   return(NULL);
   }

//---------------------------------------------------------------------------
__fastcall TRedePT::TRedePT(void)
   {
   //cria listas
   lisBAR     = new TList();
   lisCAR_MED = new TList();
   lisLIG_RL  = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TRedePT::~TRedePT(void)
   {
   //destrói lista sem destruir seus objetos
   if (lisBAR    ) {delete lisBAR    ; lisBAR     = NULL;}
   if (lisCAR_MED) {delete lisCAR_MED; lisCAR_MED = NULL;}
   if (lisLIG_RL ) {delete lisLIG_RL ; lisLIG_RL  = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TRedePT::InsereBarra(VTBarra *barra)
   {
   if (lisBAR->IndexOf(barra) < 0) lisBAR->Add(barra);
   }

//---------------------------------------------------------------------------
void __fastcall TRedePT::InsereCargaMedidor(VTCarga *carga)
   {
   if (lisCAR_MED->IndexOf(carga) < 0) lisCAR_MED->Add(carga);
   }

//---------------------------------------------------------------------------
void __fastcall TRedePT::InsereLigacaoRamal(VTLigacao *ligacao)
   {
   if (lisLIG_RL->IndexOf(ligacao) < 0) lisLIG_RL->Add(ligacao);
   }

//---------------------------------------------------------------------------
TList* __fastcall TRedePT::LisBarra(void)
   {
   return(lisBAR);
   }

//---------------------------------------------------------------------------
TList* __fastcall TRedePT::LisCargaMedidor(void)
   {
   return(lisCAR_MED);
   }

//---------------------------------------------------------------------------
TList* __fastcall TRedePT::LisLigacaoRamal(void)
   {
   return(lisLIG_RL);
   }

//---------------------------------------------------------------------------
//eof



