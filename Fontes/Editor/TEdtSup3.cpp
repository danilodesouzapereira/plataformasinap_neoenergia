//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TEdtSup3.h"
#include "TFormEdtSup3.h"
#include "..\Constante\Cursor.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Rede\VTSuprimento.h"
#include "..\Rede\VTBarra.h"
#include "..\..\DLL_Inc\Rede.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTEdt* NewObjEdtSup3(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
   {
   return(new TEdtSup3(apl, ResetFocus, SetFocus, OnDone));
   }

//---------------------------------------------------------------------------
__fastcall TEdtSup3::TEdtSup3(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
                   :TEdtEqbar(apl, TCursor(curSuprimento), ResetFocus, SetFocus, OnDone)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
__fastcall TEdtSup3::~TEdtSup3 (void)
   {
   }
//---------------------------------------------------------------------------
void __fastcall TEdtSup3::CriaEqbar(void)
	{
	//cria objeto VTSuprimento
	eqbar = DLL_NewObjSuprimento(patamares->NumPatamar());
   }

//---------------------------------------------------------------------------
bool __fastcall TEdtSup3::ConfirmaEqbar(void)
   {
   //variáveis locais
   bool         resp;
   TFormEdtSup3  *FormEdtSup3;

   //Cria e abre o formEdtSup3
   FormEdtSup3 = new TFormEdtSup3(NULL, apl, (VTSuprimento*)eqbar, false);
   //exibe Form Modal e verifica se o usuário confirmou o Eqbar
	resp = (FormEdtSup3->ShowModal() == mrOk);
   //destrói form
	delete FormEdtSup3;
   return(resp);
   }

//---------------------------------------------------------------------------
//eof


