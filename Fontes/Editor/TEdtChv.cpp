//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TEdtChv.h"
#include "TFormEdtChv.h"
#include "TFormEdtChvMS.h"
#include "..\Apl\VTApl.h"
#include "..\Constante\Cursor.h"
#include "..\Busca\VTBusca.h"
#include "..\Editor\VTEditor.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Rede\VTArea.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTEqbar.h"
#include "..\..\DLL_Inc\Rede.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTEdt* NewObjEdtChv(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
   {
   return(new TEdtChv(apl, ResetFocus, SetFocus, OnDone));
   }
//---------------------------------------------------------------------------
__fastcall TEdtChv::TEdtChv(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
                   :TEdtLiga(apl, TCursor(curChave), ResetFocus, SetFocus, OnDone)
   {
   //inicia ponteiro p/ Ligacao
   pliga = NULL;
   //mais nada a fazer: o contrutor de VTEdt faz todas inicia��es
   }

//---------------------------------------------------------------------------
__fastcall TEdtChv::~TEdtChv(void)
   {
   //nada a fazer: o destrutor de VTEdtLiga faz todas finaliza��es
   }

//---------------------------------------------------------------------------
void __fastcall TEdtChv::CriaLigacao(void)
   {
   //cria objeto Tchave
   pliga = DLL_NewObjChave(patamares->NumPatamar());
   }

//---------------------------------------------------------------------------
bool __fastcall TEdtChv::ConfirmaLigacao(void)
   {
   //vari�veis locais
   bool     resp;
   TForm    *Form;
   VTChave  *chave  = (VTChave*)pliga;
   VTEditor *editor = (VTEditor*)apl->GetObject(__classid(VTEditor));

   //cria form de edi��o conforme o Editor
   if (editor->Completo)Form = new TFormEdtChv(NULL, apl, chave, false);
   else                 Form = new TFormEdtChvMS(NULL, apl, chave, false);
   //prote��o
   if (Form == NULL) return(false);
   //exibe o form
	resp = (Form->ShowModal() == mrOk);
	delete Form;

   return(resp);
   }

//---------------------------------------------------------------------------
//eof

