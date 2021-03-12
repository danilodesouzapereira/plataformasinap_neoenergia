// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#pragma package(smart_init)
#include "TEdtRegulador.h"
#include "TFormEdtRegulador.h"
#include "TFormEdtReguladorMT.h"
#include "TFormEdtReguladorBT.h"
#include "..\Constante\Cursor.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTRegulador.h"
#include "..\..\DLL_Inc\Rede.h"
#include "..\..\DLL_Inc\Funcao.h"
// ---------------------------------------------------------------------------
VTEdt* NewObjEdtRegulador(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
{
	return (new TEdtRegulador(apl, ResetFocus, SetFocus, OnDone));
}

// ---------------------------------------------------------------------------
__fastcall TEdtRegulador::TEdtRegulador(VTApl *apl, TAction *ResetFocus, TAction *SetFocus,
	TAction *OnDone) : TEdtLiga(apl, TCursor(curRegulador), ResetFocus, SetFocus, OnDone)
{
	// inicia ponteiro p/ Ligacao
	pliga = NULL;
	// mais nada a fazer: o contrutor de VTEdt faz todas iniciações
}

// ---------------------------------------------------------------------------
__fastcall TEdtRegulador::~TEdtRegulador(void)
{
	// atualiza ponteiro p/ Ligacao
	pliga = NULL;
	// nada a fazer: o destrutor de VTEdt faz todas finalizações
}

// ---------------------------------------------------------------------------
void __fastcall TEdtRegulador::CriaLigacao(void)
{
	// cria objeto VTCapserie
	pliga = DLL_NewObjRegulador(patamares->NumPatamar());
}

/*
 // ---------------------------------------------------------------------------
 bool __fastcall TEdtRegulador::ConfirmaLigacao(void)
 {
 // variáveis locais
 bool resp;
 VTRegulador *reg;
 TFormEdtReguladorBT *FormEdtReguladorBT;
 TFormEdtReguladorMT *FormEdtReguladorMT;

 // verifica o tipo de regulador
 reg = (VTRegulador*)pliga;
 if (reg->modelo == reguENERQCT)
 { // cria Form p/ edição de regulador BT
 FormEdtReguladorBT = new TFormEdtReguladorBT(NULL, apl, reg);
 FormEdtReguladorBT->OpcaoRetira(false);
 // exibe o form
 resp = (FormEdtReguladorBT->ShowModal() == mrOk);
 delete FormEdtReguladorBT;
 }
 else
 { // cria Form p/ edição de regulador MT
 FormEdtReguladorMT = new TFormEdtReguladorMT(NULL, apl, reg);
 FormEdtReguladorMT->OpcaoRetira(false);
 // exibe o form
 resp = (FormEdtReguladorMT->ShowModal() == mrOk);
 delete FormEdtReguladorMT;
 }

 return (resp);
 }
 */
// ---------------------------------------------------------------------------
bool __fastcall TEdtRegulador::ConfirmaLigacao(void)
{
	// variáveis locais
	bool resp;
	VTRegulador *reg;
	//TFormEdtRegulador *FormEdtRegulador;
    TFormEdtReguladorBT *FormEdtReguladorBT;
	TFormEdtReguladorMT *FormEdtReguladorMT;

	// verifica o tipo de regulador
	reg = (VTRegulador*)pliga;

	// verifica o tipo de regulador pela barra
	if(IsReguladorBT())
	{ // cria Form p/ edição de regulador BT
		FormEdtReguladorBT = new TFormEdtReguladorBT(NULL, apl, reg);
		FormEdtReguladorBT->OpcaoRetira(false);
		// exibe o form
		//Hide();
		resp = (FormEdtReguladorBT->ShowModal()== mrOk);
		delete FormEdtReguladorBT;
	}
	else
	{ // cria Form p/ edição de regulador MT
		FormEdtReguladorMT = new TFormEdtReguladorMT(NULL, apl, reg);
		FormEdtReguladorMT->OpcaoRetira(false);
		// exibe o form
		//Hide();
		resp = (FormEdtReguladorMT->ShowModal()== mrOk);
		delete FormEdtReguladorMT;
	}
//	// cria Form p/ edição de regulador
//	FormEdtRegulador = new TFormEdtRegulador(NULL, apl, reg);
//	// exibe o form
//	resp = (FormEdtRegulador->ShowModal() == mrOk);
//	delete FormEdtRegulador;

	return (resp);
}
// ---------------------------------------------------------------------------
bool __fastcall TEdtRegulador::IsReguladorBT(void)
{
	VTBarra *barra;
	bool isBT = false;
	VTRegulador *regulador;

	// verifica o tipo de regulador
	regulador = (VTRegulador*)pliga;
	//verifica a tensão da barra do regulador
	//pega uma das barras
	if 		(regulador->pbarra1 != NULL)
		{barra = regulador->pbarra1;}
	else if (regulador->pbarra2 != NULL)
		{barra = regulador->pbarra2;}
	else if (regulador->pbarra3 != NULL)
		{barra = regulador->pbarra3;}

	if (barra != NULL)
	{
		if (barra->vnom > 1.0)
			{isBT = false;}
		else if (IsDoubleZero(barra->vnom - 1.0))
			{isBT = true;}
		else
			{isBT = true;}
	}

	return isBT;
}
// ---------------------------------------------------------------------------
// eof
