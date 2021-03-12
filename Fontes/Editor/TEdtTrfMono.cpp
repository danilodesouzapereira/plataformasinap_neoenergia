//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TEdtTrfMono.h"
#include "TFormEdtTrfMono.h"
#include "..\Constante\Cursor.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Rede\VTTrafo.h"
#include "..\..\DLL_Inc\Rede.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTEdt* NewObjEdtTrfMono (VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
   {
   return(new TEdtTrfMono(apl, ResetFocus, SetFocus, OnDone));
   }

//---------------------------------------------------------------------------
__fastcall TEdtTrfMono::TEdtTrfMono(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
							  :TEdtLiga(apl, TCursor(curTrafo), ResetFocus, SetFocus, OnDone)
   {
   //inicia ponteiro p/ Ligacao
   pliga = NULL;
   //mais nada a fazer: o contrutor de VTEdt faz todas iniciações
   }

//---------------------------------------------------------------------------
__fastcall TEdtTrfMono::~TEdtTrfMono(void)
   {
   //atualiza ponteiro p/ Ligacao
   pliga = NULL;
   //nada a fazer: o destrutor de VTEdt faz todas finalizações
   }
//---------------------------------------------------------------------------
void __fastcall TEdtTrfMono::CriaLigacao(void)
   {
   //cria objeto TTrafo
	pliga = DLL_NewObjTrafoMono(patamares->NumPatamar());
   }
   
//---------------------------------------------------------------------------
bool __fastcall TEdtTrfMono::ConfirmaLigacao(void)
	{
	//variáveis locais
   bool             criar_copia = false;
	bool             resp;
	TFormEdtTrfMono *FormEdtTrfMono;

	//cria e inicia form de edição do Trafo
	FormEdtTrfMono = new TFormEdtTrfMono(NULL, apl, (VTTrafo*)pliga, criar_copia);
   FormEdtTrfMono->OpcaoRetira(false);
   //exibe form
   resp = (FormEdtTrfMono->ShowModal() == mrOk);
	delete FormEdtTrfMono;
   return(resp);
   }

//---------------------------------------------------------------------------
//eof
