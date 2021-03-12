//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TResumoCarga.h"
//#include <PlataformaSinap\DLL_Inc\Rede.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTResumoCarga* __fastcall NewObjResumoCarga(void)
   {
   return(new TResumoCarga());
   }
   
//---------------------------------------------------------------------------
__fastcall TResumoCarga::TResumoCarga(void)
   {
   //inicia daos
   lisCAR = new TList();
   Node = NULL;
   Rede = NULL;
   }

//---------------------------------------------------------------------------
__fastcall TResumoCarga::~TResumoCarga(void)
   {
   //destrói lista sem destruir seus objetos
   if (lisCAR)  {delete lisCAR; lisCAR  = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TResumoCarga::InsereCarga(VTCarga *carga)
   {
   //proteção
   if (carga == NULL) return;
   //insere Carga na lista
   if (lisCAR->IndexOf(carga) < 0) lisCAR->Add(carga);
   }

//---------------------------------------------------------------------------
TList* __fastcall TResumoCarga::LisCarga(void)
   {
   return(lisCAR);
   }

//---------------------------------------------------------------------------
//eof

