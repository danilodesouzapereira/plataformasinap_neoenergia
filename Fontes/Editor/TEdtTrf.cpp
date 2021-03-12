//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TEdtTrf.h"
#include "TFormEdtTrf.h"
#include "..\Constante\Cursor.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Rede\VTTrafo.h"
#include "..\..\DLL_Inc\Rede.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTEdt* NewObjEdtTrf (VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
   {
   return(new TEdtTrf(apl, ResetFocus, SetFocus, OnDone));
   }

//---------------------------------------------------------------------------
__fastcall TEdtTrf::TEdtTrf(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
                   :TEdtLiga(apl, TCursor(curTrafo), ResetFocus, SetFocus, OnDone)
   {
   //inicia ponteiro p/ Ligacao
   pliga = NULL;
   //mais nada a fazer: o contrutor de VTEdt faz todas iniciações
   }

//---------------------------------------------------------------------------
__fastcall TEdtTrf::~TEdtTrf(void)
   {
   //atualiza ponteiro p/ Ligacao
   pliga = NULL;
   //nada a fazer: o destrutor de VTEdt faz todas finalizações
   }
//---------------------------------------------------------------------------
void __fastcall TEdtTrf::CriaLigacao(void)
   {
   //cria objeto TTrafo
   pliga = DLL_NewObjTrafo(patamares->NumPatamar());
   }
   
//---------------------------------------------------------------------------
bool __fastcall TEdtTrf::ConfirmaLigacao(void)
   {
   //variáveis locais
   bool        resp;
   bool        criar_copia = false;
   TFormEdtTrf *FormEdtTrf;

   //cria e inicia form de edição do Trafo
   FormEdtTrf = new TFormEdtTrf(NULL, apl, (VTTrafo*)pliga, criar_copia);
   FormEdtTrf->OpcaoRetira(false);
   //exibe form
   resp = (FormEdtTrf->ShowModal() == mrOk);
   delete FormEdtTrf;
   return(resp);
   }

//---------------------------------------------------------------------------
//eof
