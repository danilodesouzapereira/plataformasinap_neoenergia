//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TEdtMedicao.h"
#include "TFormEdtMedidor.h"
#include "..\Constante\Cursor.h"
#include "..\Grafico\VTGrafico.h"
//#include "..\Patamar\VTPatamares.h"
#include "..\Rede\VTMedidor.h"
#include "..\Rede\VTBarra.h"
#include "..\..\DLL_Inc\Rede.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTEdt* NewObjEdtMedicao(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
   {
   return(new TEdtMedicao(apl, ResetFocus, SetFocus, OnDone));
   }

//---------------------------------------------------------------------------
__fastcall TEdtMedicao::TEdtMedicao(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
                   :TEdtEqbar(apl, TCursor(curMedidor), ResetFocus, SetFocus, OnDone)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
__fastcall TEdtMedicao::~TEdtMedicao (void)
   {
   }

//---------------------------------------------------------------------------
void __fastcall TEdtMedicao::CriaEqbar(void)
   {
   //cria objeto Medicao
   eqbar = DLL_NewObjMedidor();
   }

//---------------------------------------------------------------------------
bool __fastcall TEdtMedicao::ConfirmaEqbar(void)
   {
   //variáveis locais
   bool            resp;
   TFormEdtMedidor *FormEdtMedidor;
   TList           *lisGER;

   //cria e abre TFormEdtMedicao como janela modal
   FormEdtMedidor = new TFormEdtMedidor(NULL, apl, (VTMedidor*)eqbar);
   FormEdtMedidor->OpcaoRetira(false);
   //exibe Form Modal e verifica se o usuário confirmou o Eqbar
   resp = (FormEdtMedidor->ShowModal() == mrOk);
   //destrói form
   delete FormEdtMedidor;
   return(resp);
   }

//---------------------------------------------------------------------------
//eof


