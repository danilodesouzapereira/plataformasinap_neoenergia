//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#pragma package(smart_init)
#include "TEdtCaps.h"
#include "TFormEdtCaps.h"
#include "..\Constante\Cursor.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Rede\VTCapserie.h"
#include "..\..\DLL_Inc\Rede.h"

//---------------------------------------------------------------------------
VTEdt* NewObjEdtCaps(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
   {
   return(new TEdtCaps(apl, ResetFocus, SetFocus, OnDone));
   }

//---------------------------------------------------------------------------
__fastcall TEdtCaps::TEdtCaps(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
                    :TEdtLiga(apl, TCursor(curCapserie), ResetFocus, SetFocus, OnDone)
   {
   //inicia ponteiro p/ Ligacao
   pliga = NULL;
   //mais nada a fazer: o contrutor de VTEdt faz todas iniciações
   }

//---------------------------------------------------------------------------
__fastcall TEdtCaps::~TEdtCaps(void)
   {
   //atualiza ponteiro p/ Ligacao
   pliga = NULL;
   //nada a fazer: o destrutor de VTEdt faz todas finalizações
   }

//---------------------------------------------------------------------------
void __fastcall TEdtCaps::CriaLigacao(void)
   {
   //cria objeto VTCapserie
   pliga = DLL_NewObjCapserie(patamares->NumPatamar());
   }

//---------------------------------------------------------------------------
bool __fastcall TEdtCaps::ConfirmaLigacao(void)
   {
   //variáveis locais
   bool        resp;
   TFormEdtCaps *FormEdtCaps;

   //cria Form p/ edição de Trecho
   FormEdtCaps = new TFormEdtCaps(NULL, apl, (VTCapserie*)pliga);
   FormEdtCaps->OpcaoRetira(false);
   //exibe o form
   resp = (FormEdtCaps->ShowModal() == mrOk);
   delete FormEdtCaps;
   return(resp);
   }

//---------------------------------------------------------------------------
//eof

