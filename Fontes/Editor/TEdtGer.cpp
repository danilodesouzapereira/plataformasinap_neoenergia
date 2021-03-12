//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TEdtGer.h"
#include "TFormEdtGer.h"
#include "..\Constante\Cursor.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Rede\VTGerador.h"
#include "..\Rede\VTBarra.h"
#include "..\..\DLL_Inc\Rede.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTEdt* NewObjEdtGer(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
   {
   return(new TEdtGer(apl, ResetFocus, SetFocus, OnDone));
   }

//---------------------------------------------------------------------------
__fastcall TEdtGer::TEdtGer(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
                   :TEdtEqbar(apl, TCursor(curGerador), ResetFocus, SetFocus, OnDone)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
__fastcall TEdtGer::~TEdtGer (void)
   {
   }

//---------------------------------------------------------------------------
void __fastcall TEdtGer::CriaEqbar(void)
   {
   //cria objeto VTGerador
   eqbar = DLL_NewObjGerador(patamares->NumPatamar());
   }

//---------------------------------------------------------------------------
bool __fastcall TEdtGer::ConfirmaEqbar(void)
   {
   //variáveis locais
   bool         resp;
   TFormEdtGer *FormEdtGer;
   TList       *lisGER;

   //cria e abre TFormEdtGer como janela modal
   FormEdtGer = new TFormEdtGer(NULL, apl, (VTGerador*)eqbar);
   FormEdtGer->OpcaoRetira(false);
   //exibe Form Modal e verifica se o usuário confirmou o Eqbar
   resp = (FormEdtGer->ShowModal() == mrOk);
   //destrói form
   delete FormEdtGer;
   return(resp);
   }

//---------------------------------------------------------------------------
//eof


