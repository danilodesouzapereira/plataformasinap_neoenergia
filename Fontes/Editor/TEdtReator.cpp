//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#pragma package(smart_init)
#include "TEdtReator.h"
#include "TFormEdtReator.h"
#include "..\Constante\Cursor.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Rede\VTReator.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Rede.h"
//---------------------------------------------------------------------------
VTEdt* NewObjEdtReator(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
   {
   return(new TEdtReator(apl, ResetFocus, SetFocus, OnDone));
   }

//---------------------------------------------------------------------------
__fastcall TEdtReator::TEdtReator(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
                      :TEdtEqbar(apl, TCursor(curReator), ResetFocus, SetFocus, OnDone)
   {
   }

//---------------------------------------------------------------------------
__fastcall TEdtReator::~TEdtReator (void)
   {
   }

//---------------------------------------------------------------------------
void __fastcall TEdtReator::CriaEqbar(void)
   {
   //cria objeto VTReator
   eqbar = DLL_NewObjReator(patamares->NumPatamar());
   eqbar->Id = FalsoId();
   }

//---------------------------------------------------------------------------
bool __fastcall TEdtReator::ConfirmaEqbar(void)
   {
   //variáveis locais
   bool          resp;
   TFormEdtReator *FormEdtReator;

   //Cria e abre o formEdtReator
   FormEdtReator = new TFormEdtReator(NULL, apl, (VTReator*)eqbar);
   FormEdtReator->OpcaoRetira(false);
   //exibe Form Modal e verifica se o usuário confirmou o Eqbar
   resp = (FormEdtReator->ShowModal() == mrOk);
   //destrói form
   delete FormEdtReator;
   return(resp);
   }

//---------------------------------------------------------------------------
//eof

