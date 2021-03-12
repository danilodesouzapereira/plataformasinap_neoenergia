//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#pragma package(smart_init)
#include "TEdtCap.h"
#include "TFormEdtCap.h"
#include "..\Constante\Cursor.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Rede\VTCapacitor.h"
#include "..\Rede\VTBarra.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Rede.h"

//---------------------------------------------------------------------------
VTEdt* NewObjEdtCap(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
   {
   return(new TEdtCap(apl, ResetFocus, SetFocus, OnDone));
   }

//---------------------------------------------------------------------------
__fastcall TEdtCap::TEdtCap(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
                   :TEdtEqbar(apl, TCursor(curCapacitor), ResetFocus, SetFocus, OnDone)
   {
   }

//---------------------------------------------------------------------------
__fastcall TEdtCap::~TEdtCap (void)
   {
   }

//---------------------------------------------------------------------------
void __fastcall TEdtCap::CriaEqbar(void)
   {
   //cria objeto TCapacitor
   eqbar = DLL_NewObjCapacitor(patamares->NumPatamar());
   eqbar->Id = FalsoId();
   }

//---------------------------------------------------------------------------
bool __fastcall TEdtCap::ConfirmaEqbar(void)
   {
   //variáveis locais
   bool        resp;
   TFormEdtCap *FormEdtCap;

   //Cria e abre o FormEdtCap
   FormEdtCap = new TFormEdtCap(NULL, apl, (VTCapacitor*)eqbar);
   FormEdtCap->OpcaoRetira(false);
   //exibe FormEdtCap Modal e verifica se o usuário confirmou o Eqbar
   resp = (FormEdtCap->ShowModal() == mrOk);
   //destrói form
   delete FormEdtCap;
   return(resp);
   }

//---------------------------------------------------------------------------
//eof

