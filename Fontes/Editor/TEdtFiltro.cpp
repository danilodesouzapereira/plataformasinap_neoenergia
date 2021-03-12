//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#pragma package(smart_init)
#include "TEdtFiltro.h"
#include "TFormEdtFiltro.h"
#include "..\Constante\Cursor.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Rede\VTFiltro.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Rede.h"
//---------------------------------------------------------------------------
VTEdt* NewObjEdtFiltro(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
   {
   return(new TEdtFiltro(apl, ResetFocus, SetFocus, OnDone));
   }

//---------------------------------------------------------------------------
__fastcall TEdtFiltro::TEdtFiltro(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
                      :TEdtEqbar(apl, TCursor(curFiltro), ResetFocus, SetFocus, OnDone)
   {
   }

//---------------------------------------------------------------------------
__fastcall TEdtFiltro::~TEdtFiltro (void)
   {
   }

//---------------------------------------------------------------------------
void __fastcall TEdtFiltro::CriaEqbar(void)
   {
   //cria objeto VTFiltro
   eqbar     = DLL_NewObjFiltro();
   eqbar->Id = FalsoId();
   }

//---------------------------------------------------------------------------
bool __fastcall TEdtFiltro::ConfirmaEqbar(void)
   {
   //variáveis locais
   bool           resp;
   TFormEdtFiltro *FormEdtFiltro;

   //Cria e abre o formEdtFiltro
   FormEdtFiltro = new TFormEdtFiltro(NULL, apl, (VTFiltro*)eqbar);
   FormEdtFiltro->OpcaoRetira(false);
   //exibe FormEdtFiltro Modal e verifica se o usuário confirmou o Eqbar
   resp = (FormEdtFiltro->ShowModal() == mrOk);
   //destrói form
   delete FormEdtFiltro;
   return(resp);
   }

//---------------------------------------------------------------------------
//eof

