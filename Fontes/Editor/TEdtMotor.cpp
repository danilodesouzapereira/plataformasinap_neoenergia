// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#pragma package(smart_init)
#include "TEdtMotor.h"
#include "TFormEdtMotor.h"
#include "TFormEdtMotorMS.h"
#include "..\Apl\VTApl.h"
#include "..\Constante\Cursor.h"
#include "..\Editor\VTEditor.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Rede\VTMotor.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Rede.h"

// ---------------------------------------------------------------------------
VTEdt* NewObjEdtMotor(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
{
	return (new TEdtMotor(apl, ResetFocus, SetFocus, OnDone));
}

// ---------------------------------------------------------------------------
__fastcall TEdtMotor::TEdtMotor(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
	: TEdtEqbar(apl, TCursor(curMotor), ResetFocus, SetFocus, OnDone)
	//
	// __fastcall TEdtMotor::TEdtMotor(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
	// :TEdtEqbar(apl, TCursor(curReator), ResetFocus, SetFocus, OnDone)
{
	//
}

// ---------------------------------------------------------------------------
__fastcall TEdtMotor::~TEdtMotor(void)
{
	//
}

// ---------------------------------------------------------------------------
void __fastcall TEdtMotor::CriaEqbar(void)
{
	// cria objeto VTMotor
	eqbar = DLL_NewObjMotor();
	eqbar->Id = FalsoId();
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/index.cfm#tasks/14887876
bool __fastcall TEdtMotor::ConfirmaEqbar(void)
{
	// variáveis locais
	bool resp = false;
	TFormEdtMotor *FormEdtMotor;
	TFormEdtMotorMS *FormEdtMotorMS;
	VTEditor *editor = (VTEditor*)apl->GetObject(__classid(VTEditor));

	// cria form de edição conforme o Editor
	if (editor->Completo)
	{ // Cria e abre o formEdtMotor
		FormEdtMotor = new TFormEdtMotor(NULL, apl, (VTMotor*)eqbar);
		FormEdtMotor->OpcaoRetira(false);
		// exibe Form Modal e verifica se o usuário confirmou o Eqbar
		resp = (FormEdtMotor->ShowModal() == mrOk);
		// destrói form
		delete FormEdtMotor;
	}

	else
	{ // Cria e abre o formEdtMotorMS
		FormEdtMotorMS = new TFormEdtMotorMS(NULL, apl, (VTMotor*)eqbar);
		FormEdtMotorMS->OpcaoRetira(false);
		// exibe Form Modal e verifica se o usuário confirmou o Eqbar
		resp = (FormEdtMotorMS->ShowModal() == mrOk);
		// destrói form
		delete FormEdtMotorMS;
	}
	return (resp);
}

// ---------------------------------------------------------------------------
// eof
