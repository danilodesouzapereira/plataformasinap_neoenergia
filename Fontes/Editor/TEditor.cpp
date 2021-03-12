// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TEditor.h"
#include "VTEdt.h"
#include "..\Apl\VTApl.h"
#include "..\Constante\Cursor.h"
#include "..\Grafico\VTGrafico.h"
#include "..\..\DLL_Inc\Funcao.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
// funções globais p/ criar objetos da classe TEditor
// ---------------------------------------------------------------------------
VTEditor* NewObjEditor(VTApl *apl)
{
	return (new TEditor(apl));
}

// ---------------------------------------------------------------------------
__fastcall TEditor::TEditor(VTApl *apl)
{
	// salva ponteiro p/ objetos
	this->apl = apl;
	// assume que o Editor é do tipo completo
	Completo = true;
}

// ---------------------------------------------------------------------------
__fastcall TEditor::~TEditor(void)
{
	// destrói objetos
	Disable();
}

// ---------------------------------------------------------------------------
void __fastcall TEditor::Disable(void)
{
	// destrói editor
	if (edt_copia != edt)
	{
		// destrói objeto
		if (edt_copia)
		{
			delete edt_copia;
		}
	}
	edt_copia = NULL;
	// destrói objeto edt
	if (edt)
	{
		delete edt;
		edt = NULL;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TEditor::EditaAltera(TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
{
	// destrói Editor atual
	Disable();
	// cria novo Editor
	if ((edt = NewObjEdtAlt(apl, ResetFocus, SetFocus, OnDone)) == NULL)
		return;
}

// ---------------------------------------------------------------------------
void __fastcall TEditor::EditaBateria(TAction *ResetFocus, TAction *SetFocus)
{
	// destrói Editor atual
	Disable();
	// cria novo Editor
	if ((edt = NewObjEdtBateria(apl, ResetFocus, SetFocus)) == NULL)
		return;
}

// ---------------------------------------------------------------------------
void __fastcall TEditor::EditaBarra(TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
{
	// destrói Editor atual
	Disable();
	// cria novo Editor
	if ((edt = NewObjEdtBar(apl, ResetFocus, SetFocus, OnDone)) == NULL)
		return;
}

// ---------------------------------------------------------------------------
void __fastcall TEditor::EditaCapacitor(TAction *ResetFocus, TAction *SetFocus)
{
	// destrói Editor atual
	Disable();
	// cria novo Editor
	if ((edt = NewObjEdtCap(apl, ResetFocus, SetFocus)) == NULL)
		return;
}

// ---------------------------------------------------------------------------
void __fastcall TEditor::EditaCapserie(TAction *ResetFocus, TAction *SetFocus)
{
	// destrói Editor atual
	Disable();
	// cria novo Editor
	if ((edt = NewObjEdtCaps(apl, ResetFocus, SetFocus)) == NULL)
		return;
}

// ---------------------------------------------------------------------------
void __fastcall TEditor::EditaCarga(TAction *ResetFocus, TAction *SetFocus)
{
	// destrói Editor atual
	Disable();
	// cria novo Editor
	if ((edt = NewObjEdtCarga(apl, ResetFocus, SetFocus)) == NULL)
		return;
}

// ---------------------------------------------------------------------------
void __fastcall TEditor::EditaCargaNL(TAction *ResetFocus, TAction *SetFocus)
{
	// destrói Editor atual
	Disable();
	// cria novo Editor
	if ((edt = NewObjEdtCargaNL(apl, ResetFocus, SetFocus)) == NULL)
		return;
}

// ---------------------------------------------------------------------------
void __fastcall TEditor::EditaCargaIP(TAction *ResetFocus, TAction *SetFocus)
{
	// destrói Editor atual
	Disable();
	// cria novo Editor
	if ((edt = NewObjEdtCargaIP(apl, ResetFocus, SetFocus)) == NULL)
		return;
}

// ---------------------------------------------------------------------------
void __fastcall TEditor::EditaChave(TAction *ResetFocus, TAction *SetFocus)
{
	// destrói Editor atual
	Disable();
	// cria novo Editor
	if ((edt = NewObjEdtChv(apl, ResetFocus, SetFocus)) == NULL)
		return;
}

// ---------------------------------------------------------------------------
VTEdt* __fastcall TEditor::EditaColaEqptos(TAction *ResetFocus, TAction *SetFocus)
{
	// destrói Editor atual SOMENTE SE não for edt_copia
	if (edt_copia != edt)
	{
		Disable();
	}
	// cria novo Editor
	return (edt = NewObjEdtCola(apl, ResetFocus, SetFocus));
}

/*
 //---------------------------------------------------------------------------
 void __fastcall TEditor::EditaCopiaEqptos(TAction *ResetFocus, TAction *SetFocus)
 {
 //cria novo Editor
 if ((edt_copia = NewObjEdtCopia(apl, ResetFocus, SetFocus)) == NULL) return;
 //destrói Editor atual
 Disable();
 //salva edt_copia
 edt_copia = edt;
 }
 */
// ---------------------------------------------------------------------------
VTEdt* __fastcall TEditor::EditaCopiaEqptos(TAction *ResetFocus, TAction *SetFocus)
{
	// variáveis locais
	VTEdt *edt_novo;
	TList *lisEQP;

	// cria novo Editor
	if ((edt_novo = NewObjEdtCopia(apl, ResetFocus, SetFocus)) == NULL)
		return (NULL);
	// aproveita lista de Eqptos selecionados do Edt anterior
	if (edt != NULL)
	{
		lisEQP = edt->LisEqptoSel();
		edt_novo->EvtSelEqpto(lisEQP);
		lisEQP->Clear();
	}
	// destrói Editor anterior
	Disable();
	// salva Edt
	edt = edt_novo;
	edt_copia = edt_novo;
	return (edt_copia);
}

// ---------------------------------------------------------------------------
void __fastcall TEditor::EditaEsquematicoAlign(TAction *ResetFocus, TAction *SetFocus)
{
	// destrói Editor atual
	Disable();
	// cria novo Editor
	if ((edt = NewObjEdtAlign(apl, ResetFocus, SetFocus)) == NULL)
		return;
}

// ---------------------------------------------------------------------------
void __fastcall TEditor::EditaFigComentario(TAction *ResetFocus, TAction *SetFocus)
   {
   // destrói Editor atual
   Disable();
   // cria novo Editor
   edt = NewObjEdtFigComentario(apl, ResetFocus, SetFocus);
   }

// ---------------------------------------------------------------------------
void __fastcall TEditor::EditaFigLinkComentarioEqpto(TAction *ResetFocus, TAction *SetFocus)
   {
   // destrói Editor atual
   Disable();
   // cria novo Editor
   edt = NewObjEdtFigLink(apl, ResetFocus, SetFocus);
   }

// ---------------------------------------------------------------------------
void __fastcall TEditor::EditaFigRetangulo(TAction *ResetFocus, TAction *SetFocus)
   {
   // destrói Editor atual
   Disable();
   // cria novo Editor
   edt = NewObjEdtFigRetangulo(apl, ResetFocus, SetFocus);
   }

// ---------------------------------------------------------------------------
void __fastcall TEditor::EditaFiltro(TAction *ResetFocus, TAction *SetFocus)
{
	// destrói Editor atual
	Disable();
	// cria novo Editor
	if ((edt = NewObjEdtFiltro(apl, ResetFocus, SetFocus)) == NULL)
		return;
}

// ---------------------------------------------------------------------------
void __fastcall TEditor::EditaGerador(TAction *ResetFocus, TAction *SetFocus)
{
	// destrói Editor atual
	Disable();
	// cria novo Editor
	if ((edt = NewObjEdtGer(apl, ResetFocus, SetFocus)) == NULL)
		return;
}

// ---------------------------------------------------------------------------
void __fastcall TEditor::EditaGiro(TAction *ResetFocus, TAction *SetFocus)
{
	// destrói Editor atual
	Disable();
	// cria novo Editor
	if ((edt = NewObjEdtGiro(apl, ResetFocus, SetFocus)) == NULL)
		return;
}

// ---------------------------------------------------------------------------
void __fastcall TEditor::EditaManobra(TAction *ResetFocus, TAction *SetFocus, TWinControl *parent)
{
	// destrói Editor atual
	Disable();
	// cria novo EDITOR
	if ((edt = NewObjEdtManobra(apl, ResetFocus, SetFocus, NULL, parent)) == NULL)
		return;
}

// ---------------------------------------------------------------------------
void __fastcall TEditor::EditaMedicao(TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
{
	// destrói Editor atual
	Disable();
	// cria novo EDITOR
	if ((edt = NewObjEdtMedicao(apl, ResetFocus, SetFocus, OnDone)) == NULL)
		return;
}

// ---------------------------------------------------------------------------
void __fastcall TEditor::EditaMotor(TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
{
	// destrói Editor atual
	Disable();
	// cria novo EDITOR
	if ((edt = NewObjEdtMotor(apl, ResetFocus, SetFocus, OnDone)) == NULL)
		return;
}

// ---------------------------------------------------------------------------
void __fastcall TEditor::EditaMutua(TAction *ResetFocus, TAction *SetFocus)
{
	// destrói Editor atual
	Disable();
	// cria novo EDITOR
	if ((edt = NewObjEdtMutua(apl, ResetFocus, SetFocus)) == NULL)
		return;
}

// ---------------------------------------------------------------------------
void __fastcall TEditor::EditaPolyline(TAction *ResetFocus, TAction *SetFocus, TWinControl *parent)
{
	// destrói Editor atual
	Disable();
	// cria novo Editor
	if ((edt = NewObjEdtPolyline(apl, ResetFocus, SetFocus, NULL, parent)) == NULL)
		return;
}

// ---------------------------------------------------------------------------
void __fastcall TEditor::EditaReator(TAction *ResetFocus, TAction *SetFocus)
{
	// destrói Editor atual
	Disable();
	// cria novo EDITOR
	if ((edt = NewObjEdtReator(apl, ResetFocus, SetFocus)) == NULL)
		return;
}

// ---------------------------------------------------------------------------
void __fastcall TEditor::EditaRede(void)
{
	/*
	 //variáveis locais
	 TFormEdtRede *form;

	 try{//cria TFormEdtRede p/ alterar, inserir ou retirar Redes
	 form = new TFormEdtRede(NULL, apl);
	 //exibe TFormEdtRede como janela modal
	 form->ShowModal();
	 //destrói form
	 delete form;
	 }catch(Exception &e)
	 {//nada a fazer
	 }
	 */
}

// ---------------------------------------------------------------------------
void __fastcall TEditor::EditaRegulador(TAction *ResetFocus, TAction *SetFocus)
{
	// destrói Editor atual
	Disable();
	// cria novo EDITOR
	if ((edt = NewObjEdtRegulador(apl, ResetFocus, SetFocus)) == NULL)
		return;
}

// ---------------------------------------------------------------------------
void __fastcall TEditor::EditaSelEqpto(TList *lisEQP)
{
	// passa lista de Eqptos p/ Edt
	if (edt)
		edt->EvtSelEqpto(lisEQP);
}

// ---------------------------------------------------------------------------
void __fastcall TEditor::EditaSincronismo(TAction *ResetFocus, TAction *SetFocus,
	TWinControl *parent)
{
	// destrói Editor atual
	Disable();
	// cria novo EDITOR
	if ((edt = NewObjEdtSinc(apl, ResetFocus, SetFocus, NULL, parent)) == NULL)
		return;
}

// ---------------------------------------------------------------------------
void __fastcall TEditor::EditaSuprimento(TAction *ResetFocus, TAction *SetFocus)
{
	// destrói Editor atual
	Disable();
	// cria novo Editor
	if ((edt = NewObjEdtSup(apl, ResetFocus, SetFocus)) == NULL)
		return;
}

// ---------------------------------------------------------------------------
void __fastcall TEditor::EditaSuprimentoPorFase(TAction *ResetFocus, TAction *SetFocus)
{
	// destrói Editor atual
	Disable();
	// cria novo Editor
	if ((edt = NewObjEdtSup3(apl, ResetFocus, SetFocus)) == NULL)
		return;
}

// ---------------------------------------------------------------------------
void __fastcall TEditor::EditaSwitch(TAction *ResetFocus, TAction *SetFocus)
{
	// destrói Editor atual
	Disable();
	// cria novo EDITOR
	if ((edt = NewObjEdtSwitch(apl, ResetFocus, SetFocus)) == NULL)
		return;
}

// ---------------------------------------------------------------------------
void __fastcall TEditor::EditaTrafo(TAction *ResetFocus, TAction *SetFocus)
{
	// destrói Editor atual
	Disable();
	// cria novo Editor
	if ((edt = NewObjEdtTrf(apl, ResetFocus, SetFocus)) == NULL)
		return;
}

// ---------------------------------------------------------------------------
void __fastcall TEditor::EditaTrafo3E(TAction *ResetFocus, TAction *SetFocus)
{
	// destrói Editor atual
	Disable();
	// cria novo Editor
	if ((edt = NewObjEdtTrf3E(apl, ResetFocus, SetFocus)) == NULL)
		return;
}

// ---------------------------------------------------------------------------
void __fastcall TEditor::EditaTrafoMono(TAction *ResetFocus, TAction *SetFocus)
{
	// destrói Editor atual
	Disable();
	// cria novo Editor
	if ((edt = NewObjEdtTrfMono(apl, ResetFocus, SetFocus)) == NULL)
		return;
}

// ---------------------------------------------------------------------------
void __fastcall TEditor::EditaTrafoZZ(TAction *ResetFocus, TAction *SetFocus)
{
	// destrói Editor atual
	Disable();
	// cria novo EDITOR
	if ((edt = NewObjEdtTrfZZ(apl, ResetFocus, SetFocus)) == NULL)
		return;
}

/*
 //---------------------------------------------------------------------------
 void __fastcall TEditor::EditaTransferencia(TAction *ResetFocus, TAction *SetFocus)
 {
 //destrói Editor atual
 Disable();
 //cria novo Editor
 if ((edt = NewObjEdtTransf(apl, NULL)) == NULL) return;
 //inicia Editor
 edt->DefObjReconf(reconf);
 }
 */
// ---------------------------------------------------------------------------
void __fastcall TEditor::EditaTrecho(TAction *ResetFocus, TAction *SetFocus)
{
	// destrói Editor atual
	Disable();
	// cria novo Editor
	if ((edt = NewObjEdtTre(apl, ResetFocus, SetFocus)) == NULL)
		return;
}

// ---------------------------------------------------------------------------
void __fastcall TEditor::EditaZoom(TAction *ResetFocus, TAction *SetFocus)
{
	// destrói Editor atual
	Disable();
	// cria novo Editor
	if ((edt = NewObjEdtZoom(apl, ResetFocus, SetFocus)) == NULL)
		return;
}

// ---------------------------------------------------------------------------
void __fastcall TEditor::EvtDelete(void)
{
	// verifica se há um Edt ativo
	if (edt)
		edt->EvtDelete();
}

// ---------------------------------------------------------------------------
void __fastcall TEditor::EvtMove(bool down, bool left, bool right, bool up)
{
	// verifica se há um Edt ativo
	if (edt)
		edt->EvtMove(down, left, right, up);
}

// ---------------------------------------------------------------------------
TList* __fastcall TEditor::LisEqptoSelecionado(void)
{
	// variáveis locais
	TList *lisEQP;

	// verifica se há um Edt ativo
	if (edt == NULL)
		return (NULL);
	// retira Marcas de Eqptos selecionados pois eles ficam associados a barras temporárias
	edt->MarcaCancela();
	// obtém lista de Eqptos selecionados
	lisEQP = edt->LisEqptoSel();
	return (lisEQP->Count > 0) ? lisEQP : NULL;
}

// ---------------------------------------------------------------------------
bool __fastcall TEditor::PM_GetCompleto(void)
{
	return (tipo.completo);
}

// ---------------------------------------------------------------------------
void __fastcall TEditor::PM_SetCompleto(bool completo)
{
	tipo.completo = completo;
}

// ---------------------------------------------------------------------------
bool __fastcall TEditor::RedeAlterada(void)
{
	return (true);
}

// ---------------------------------------------------------------------------
void __fastcall TEditor::Redo(void)
{
	// falta implementar)
}

// ---------------------------------------------------------------------------
void __fastcall TEditor::Undo(void)
{
	// falta implementar)
}

// ---------------------------------------------------------------------------
// eof
