// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TAplEditor.h"
#include "TFormEditor.h"
#include "TFormSelEqpto.h"
#include "TFormEdtFaseTrecho.h"
#include "TFormEdtManobra.h"
#include "TFormEdtRede.h"
#include "TFormEdtSinc.h"
#include "TFormImportaBarras.h"
#include "Recondutorar\TFormRecondutorar.h"
#include "VTEditor.h"
#include "Carro\TFormCarro.h"
#include "EditaRede\TEditaSED.h"
#include "EditaRede\TInsereFusivel.h"
#include "EditaRede\TRemoveBT.h"
#include "EditaRede\TRemoveTrecho.h"
#include "Grupo\TFormGrupo.h"
#include "Tensao\TFormTensao.h"
#include "..\AutoTag\VTAutoTag.h"
#include "..\Diretorio\VTPath.h"
#include "..\Edita\VTEdita.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Licenca\VTLicenca.h"
#include "..\Licenca\VTLicencas.h"
#include "..\Obra\VTObra.h"
#include "..\Progresso\VTProgresso.h"
#include "..\RBAC\VTRBACManager.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Redegraf\VTRedegraf.h"
#include "..\..\DLL_Inc\AutoTag.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Help.h"
#include "..\..\DLL_Inc\Importa.h"
#include "..\..\DLL_Inc\Progresso.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
__fastcall TFormEditor::TFormEditor(TComponent* Owner, VTApl *apl_owner, TWinControl *parent,
	bool planejamento) : TForm(Owner)
{
	// vari�veis locais
	VTEdita *edita = (VTEdita*)apl_owner->GetObject(__classid(VTEdita));
	VTAutoTag *autoTag = NULL;

	// cria objeto apl pr�prio
	apl = new TAplEditor(this, apl_owner);
	// salva ponteiro p/ objeto VTEditor
	editor = (VTEditor*)apl->GetObject(__classid(VTEditor));
	// redefine Parent
	if (parent)
		Parent = parent;
	// cria e exibe ou n�o FormGrupo
	apl->Add(FormGrupo = new TFormGrupo(this, apl, GBoxGrupo));
	// GBoxGrupo->Visible = (FormGrupo->NumeroGrupo() > 0);
	// if (GBoxGrupo->Visible) FormGrupo->Show();
	FormGrupo->Show();
	// ATEN��O: truque importante
	// associa Button butUndo c/ ActionUndo do objeto TEdita p/ habilitar/desabilitar bot�o MAS
	// o tratamento do evento OnClick continua sendo feito localmente
	butUndo->Action = edita->ActionUndo;
	butUndo->ImageIndex = 18;
	butUndo->OnClick = ActionUndoExecute; // tratamento local
	// associa Button butRedo c/ ActionRedo do objeto TEdita p/ habilitar/desabilitar bot�o MAS
	// o tratamento do evento OnClick continua sendo feito localmente
	butRedo->Action = edita->ActionRedo;
	butRedo->ImageIndex = 19;
	butRedo->OnClick = ActionRedoExecute; // tratamento local
	// cria EdtAltera
	ActionEdtAltera->Execute();
	// atualiza Caption
	Caption = edita->Nome;
	// habilita/desabilita comandos de usu�rio privilegiado
	ConfiguraOpcaoUsuario();
	// habilita/desabilita comandos p/ planejamento
	if (planejamento)
	{
		ConfiguraOpcaoPlanejamento();
	}
	//alinhamento default
	Top = 0;
	Left = 0;
	action_align = ActionAlignNone;
}

// ---------------------------------------------------------------------------
__fastcall TFormEditor::~TFormEditor(void)
{
	// vari�veis locais
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

	// destr�i eventual FormFilho que esteja aberto
	if (FormFilho)
	{
		delete FormFilho;
		FormFilho = NULL;
	}
	// N�O destr�i FormGrupo porque ele foi inserido no Apl
	// if (FormGrupo)   {delete FormGrupo;   FormGrupo = NULL;}
	// destr�i
	// salva par�metros do Form
	if (geral)
		geral->Infoset->SaveToFile(this);
	// destr�i objetos
	editor->Disable();
	if (apl)
	{
		delete apl;
		apl = NULL;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditor::ActionAlignBottomExecute(TObject *Sender)
{
	// verifica se a janela estava alinhada em cima
	if (Align == alTop)
		Align = alNone;
	// alinha janela em baixo
	Align = alBottom;
	// altera estilo da borda e redimensiona o form
	BorderStyle = bsNone;
	ClientHeight = 2 * ToolBarEqpto->ButtonHeight;
	// FKM -2015.10.06
	PanelToolBars->AutoSize = false;
	PanelToolBars->Height = ToolBarEqpto->ButtonHeight;
	ToolBarAvancado->Align = alLeft;
	ToolBarEsq->Align = alLeft;
	ToolBarEqpto->Align = alLeft;
	GBoxGrupo->Align = alBottom;
	//alinhamento determinado
	action_align = ActionAlignBottom;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditor::ActionAlignLeftExecute(TObject *Sender)
{
	// verifica se a janela estava alinhada � direita
	if (Align == alRight)
		Align = alNone;
	// alinha janela � esquerda
	Align = alLeft;
	// altera estilo da borda e redimensiona o form
	BorderStyle = bsNone;
	ClientWidth = 2 * ToolBarEqpto->ButtonWidth;
	// FKM -2015.10.06
	PanelToolBars->AutoSize = true;
	ToolBarAvancado->Align = alTop;
	ToolBarEqpto->Align = alTop;
	ToolBarEsq->Align = alTop;
	GBoxGrupo->Align = alClient;
	//alinhamento determinado
	action_align = ActionAlignLeft;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditor::ActionAlignNoneExecute(TObject *Sender)
{
	// cancela alinhamento da janela (flutuante)
	Align = alNone;
	Top = janela.top;
	Left = janela.left;
	Width = janela.width;
	Height = janela.height;
	// altera estilo da borda
	BorderStyle = bsSizeToolWin;
	// FKM -2015.10.06
	PanelToolBars->AutoSize = true;
	ToolBarEqpto->Align = alTop;
	ToolBarEsq->Align = alTop;
	ToolBarAvancado->Align = alTop;
	GBoxGrupo->Align = alClient;
    //alinhamento determinado
	action_align = ActionAlignNone;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditor::ActionAlignRightExecute(TObject *Sender)
{
	// verifica se a janela estava alinhada � esquerda
	if (Align == alLeft)
		Align = alNone;
	// alinha janela � direita
	Align = alRight;
	// altera estilo da borda e redimensiona o form
	BorderStyle = bsNone;
	ClientWidth = 2 * ToolBarEqpto->ButtonWidth;
	// FKM -2015.10.06
	PanelToolBars->AutoSize = true;
	ToolBarAvancado->Align = alTop;
	ToolBarEqpto->Align = alTop;
	ToolBarEsq->Align = alTop;
	GBoxGrupo->Align = alClient;
	//alinhamento determinado
	action_align = ActionAlignRight;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditor::ActionAlignTopExecute(TObject *Sender)
{
	// verifica se a janela estava alinhada em baixo
	if (Align == alBottom)
		Align = alNone;
	// alinha janela em cima
	Align = alTop;
	// altera estilo da borda e redimensiona o form
	BorderStyle = bsNone;
	ClientHeight = 2 * ToolBarEqpto->ButtonHeight;
	// FKM -2015.10.06
	PanelToolBars->AutoSize = false;
	PanelToolBars->Height = ToolBarEqpto->ButtonHeight;
	ToolBarAvancado->Align = alLeft;
	ToolBarEsq->Align = alLeft;
	ToolBarEqpto->Align = alLeft;
	GBoxGrupo->Align = alBottom;
	//alinhamento determinado
	action_align = ActionAlignTop;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditor::ActionAlteraFasesExecute(TObject *Sender)
{
	// vari�veis locais
	TFormEdtFaseTrecho *form;

	// desabilita Edt atual
	editor->Disable();
	// liga indica��o de mouse ativo e for�a bot�o Down
	AtualizaIndicacaoEditorOn(butAltera);
	try
	{ // cria Form como janela normal
		form = new TFormEdtFaseTrecho(this, apl);
		form->Show();
		// n�o destr�i o Form (ele se destr�i sozinho)
		// delete form;
	}
	catch (Exception &e)
	{ // nada a fazer
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditor::ActionAreaRedeMaiorExecute(TObject *Sender)
{
	// vari�veis locais
	double fator = 0.1;
	TList *lisEQP;
	VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));
	// VTGrafico  *graf     = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTRedegraf *redegraf = (VTRedegraf*)apl->GetObject(__classid(VTRedegraf));

	// verifica gr�fico em exibi��o
	if (redegraf->ModoGrafico == modoESQUEMATICO)
	{ // altera dimens�es do diagrama esquem�tico da rede
		if ((lisEQP = editor->LisEqptoSelecionado()) == NULL)
		{
			edita->AlteraDiagEsqArea(redes, fator);
		}
		else
		{
			edita->AlteraDiagEsqArea(redes, lisEQP, fator);
		}
	}
	else
	{ // altera dimens�es do diagrama georeferenciado da rede
		edita->AlteraDiagGeoArea(redes, fator);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditor::ActionAreaRedeMenorExecute(TObject *Sender)
{
	// vari�veis locais
	double fator = -0.1;
	TList *lisEQP;
	VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));
	// VTGrafico  *graf     = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTRedegraf *redegraf = (VTRedegraf*)apl->GetObject(__classid(VTRedegraf));

	// verifica gr�fico em exibi��o
	if (redegraf->ModoGrafico == modoESQUEMATICO)
	{ // altera dimens�es do diagrama esquem�tico da rede
		if ((lisEQP = editor->LisEqptoSelecionado()) == NULL)
		{
			edita->AlteraDiagEsqArea(redes, fator);
		}
		else
		{
			edita->AlteraDiagEsqArea(redes, lisEQP, fator);
		}
	}
	else
	{ // altera dimens�es do diagrama georeferenciado da rede
		edita->AlteraDiagGeoArea(redes, fator);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditor::ActionClearExecute(TObject *Sender)
{
	// vari�veis locais
	VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// pede confirma��o do usu�rio
	if (Confirma("Todas as redes e seus equipamentos ser�o eliminados.",
		"ATEN��O: Esta opera��o n�o poder� ser desfeita.") == IDYES)
	{ // reinicia Edita
		edita->Clear();
		redes->Clear();
		grafico->AreaRedeAltera();
		// habilita/desabilita comandos do Editor
		HabilitaEditor();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditor::ActionDeleteExecute(TObject *Sender)
{
	editor->EvtDelete();
	// liga indica��o de mouse ativo e for�a bot�o Down
	AtualizaIndicacaoEditorOn(NULL);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditor::ActionEdtAlteraExecute(TObject *Sender)
{
	// cria editor de altera��o
	editor->EditaAltera(ActionMouseOff, ActionMouseOn);
	// liga indica��o de mouse ativo e for�a bot�o Down
	AtualizaIndicacaoEditorOn(butAltera);
	// habilita/desabilita comandos do Editor
	HabilitaEditor();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditor::ActionEdtBarraExecute(TObject *Sender)
{
	// cria editor de Suprimento
	editor->EditaBarra(ActionMouseOff, ActionMouseOn);
	// liga indica��o de mouse ativo e for�a bot�o Down
	AtualizaIndicacaoEditorOn(butBarra);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditor::ActionEdtBateriaExecute(TObject *Sender)
{
	// cria editor do Armazenamento de Energia
	editor->EditaBateria(ActionMouseOff, ActionMouseOn);
	// liga indica��o de mouse ativo e for�a bot�o Down
	AtualizaIndicacaoEditorOn(butBateria);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditor::ActionEdtCapacitorExecute(TObject *Sender)
{
	// cria editor de carga
	editor->EditaCapacitor(ActionMouseOff, ActionMouseOn);
	// liga indica��o de mouse ativo e for�a bot�o Down
	AtualizaIndicacaoEditorOn(ButCapacitor);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditor::ActionEdtCapserieExecute(TObject *Sender)
{
	// cria editor de carga
	editor->EditaCapserie(ActionMouseOff, ActionMouseOn);
	// liga indica��o de mouse ativo e for�a bot�o Down
	AtualizaIndicacaoEditorOn(ButCapserie);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditor::ActionEdtMotorExecute(TObject *Sender)
{
	// cria editor de Suprimento
	editor->EditaMotor(ActionMouseOff, ActionMouseOn);
	// liga indica��o de mouse ativo e for�a bot�o Down
	AtualizaIndicacaoEditorOn(btnMotor);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditor::ActionEdtCargaExecute(TObject *Sender)
{
	// cria editor de carga
	editor->EditaCarga(ActionMouseOff, ActionMouseOn);
	// liga indica��o de mouse ativo e for�a bot�o Down
	AtualizaIndicacaoEditorOn(butCarga);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditor::ActionEdtCargaIPExecute(TObject *Sender)
{
	// cria editor de carga
	editor->EditaCargaIP(ActionMouseOff, ActionMouseOn);
	// liga indica��o de mouse ativo e for�a bot�o Down
	AtualizaIndicacaoEditorOn(ButCargaIP);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditor::ActionEdtChaveExecute(TObject *Sender)
{
	// cria editor de Chave
	editor->EditaChave(ActionMouseOff, ActionMouseOn);
	// liga indica��o de mouse ativo e for�a bot�o Down
	AtualizaIndicacaoEditorOn(butChave);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditor::ActionEdtColaExecute(TObject *Sender)
{
	// cria editor p/ copiar Eqptos
	// INTERESSANTE: passa ActionEdtAltera p/ ser executada por EdtCola ap�s colar eqptos
	editor->EditaColaEqptos(ActionEdtAltera, ActionMouseOn);
	// liga indica��o de mouse ativo e for�a bot�o Down
	AtualizaIndicacaoEditorOn(ButEdtCola);
	// habilita/desabilita comandos do Editor
	HabilitaEditor();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditor::ActionEdtCopiaExecute(TObject *Sender)
{
	// cria editor p/ copiar Eqptos
	editor->EditaCopiaEqptos(ActionMouseOff, ActionMouseOn);
	// liga indica��o de mouse ativo e for�a bot�o Down
	AtualizaIndicacaoEditorOn(ButEdtCopia);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditor::ActionEdtFiltroExecute(TObject *Sender)
{
	// cria editor de carga
	editor->EditaFiltro(ActionMouseOff, ActionMouseOn);
	// liga indica��o de mouse ativo e for�a bot�o Down
	AtualizaIndicacaoEditorOn(ButEdtFiltro);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditor::ActionEdtGeradorExecute(TObject *Sender)
{
	// cria editor de Gerador
	editor->EditaGerador(ActionMouseOff, ActionMouseOn);
	// liga indica��o de mouse ativo e for�a bot�o Down
	AtualizaIndicacaoEditorOn(butGerador);
	// verifica se pode ser inserido um Gerador
	if (!ValidaInsercaoGeracao())
	{ // cancela Edt atual
		editor->Disable();
		AtualizaIndicacaoEditorOff();
	}
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/16534326
void __fastcall TFormEditor::ActionEdtManobraExecute(TObject *Sender)
{
	// vari�veis locais
	TForm *FormEdtManobra;
	VTRBACManager *rbac = (VTRBACManager*)apl->GetObject(__classid(VTRBACManager));

	// RBAC
	if (!rbac->ExistePermissaoModulo(modMANOBRAS))
	{
		Aviso("Sua licen�a de uso n�o tem permiss�o para " + rbac->GetDescricaoOperacao(opREDE));
		// liga indica��o de mouse ativo e for�a bot�o Down
		AtualizaIndicacaoEditorOn(butAltera);
		return;
	}
	/*
	// cria editor de Manobra de Chaves
	editor->EditaManobra(ActionMouseOff, ActionMouseOn, Parent);
	// liga indica��o de mouse ativo e for�a bot�o Down
	AtualizaIndicacaoEditorOn(butManobra);
	*/

	// desabilita Edt atual
	editor->Disable();
	// desliga indica��o de mouse ativo
	AtualizaIndicacaoEditorOff();
	// esconde este Form
//	Hide();
	// cria Form para edi��o de manobra
	try
	{ // cria FormEdtManobra p/ editar manobras
		FormEdtManobra = new TFormEdtManobra(this, apl, ActionFormFilhoClosed, Parent);
		// exibe FormEdtManobra como janela normal
		FormEdtManobra->Show();
		// n�o destr�i o Form (ele se destr�i sozinho)
		// delete FormEdtRede
		// salva ponteiro para FormEdtManobra
		SalvaFormFilho(FormEdtManobra);
	}
	catch (Exception &e)
	{ // nada a fazer
	}

}

// ---------------------------------------------------------------------------
void __fastcall TFormEditor::ActionEdtMedicaoExecute(TObject *Sender)
{
	// cria editor de Medi��o
	editor->EditaMedicao(ActionMouseOff, ActionMouseOn);
	// liga indica��o de mouse ativo e for�a bot�o Down
	AtualizaIndicacaoEditorOn(ButMedicao);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditor::ActionEdtMoveDownExecute(TObject *Sender)
{
	// vari�veis locais
	bool down = true;
	bool left = false;
	bool right = false;
	bool up = false;

	// gera evento relativos �s setas de movimenta��o p/ editor
	editor->EvtMove(down, left, right, up);
	// liga indica��o de mouse ativo e for�a bot�o Down
	AtualizaIndicacaoEditorOn();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditor::ActionEdtMoveLeftExecute(TObject *Sender)
{
	// vari�veis locais
	bool down = false;
	bool left = true;
	bool right = false;
	bool up = false;

	// gera evento relativos �s setas de movimenta��o p/ editor
	editor->EvtMove(down, left, right, up);
	// liga indica��o de mouse ativo e for�a bot�o Down
	AtualizaIndicacaoEditorOn();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditor::ActionEdtMoveRightExecute(TObject *Sender)
{
	// vari�veis locais
	bool down = false;
	bool left = false;
	bool right = true;
	bool up = false;

	// gera evento relativos �s setas de movimenta��o p/ editor
	editor->EvtMove(down, left, right, up);
	// liga indica��o de mouse ativo e for�a bot�o Down
	AtualizaIndicacaoEditorOn();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditor::ActionEdtMoveUpExecute(TObject *Sender)
{
	// vari�veis locais
	bool down = false;
	bool left = false;
	bool right = false;
	bool up = true;

	// gera evento relativos �s setas de movimenta��o p/ editor
	editor->EvtMove(down, left, right, up);
	// liga indica��o de mouse ativo e for�a bot�o Down
	AtualizaIndicacaoEditorOn();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditor::ActionEdtMutuaExecute(TObject *Sender)
{
	// cria editor de Reator
	editor->EditaMutua(ActionMouseOff, ActionMouseOn);
	// liga indica��o de mouse ativo e for�a bot�o Down
	AtualizaIndicacaoEditorOn(ButMutua);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditor::ActionEdtRedeExecute(TObject *Sender)
{
	// vari�veis locais
	TForm *FormEdtRede;

	// desabilita Edt atual
	editor->Disable();
	// liga indica��o de mouse ativo e for�a bot�o Down
	AtualizaIndicacaoEditorOn(butRede);
	// esconde este Form
	Hide();
	// cria Form para edi��o de redes
	try
	{ // cria TFormEdtRede p/ alterar, inserir ou retirar Redes
		FormEdtRede = new TFormEdtRede(this, apl, ActionFormFilhoClosed, Parent);
		// exibe TFormEdtRede como janela normal
		FormEdtRede->Show();
		// n�o destr�i o Form (ele se destr�i sozinho)
		// delete FormEdtRede
		// salva ponteiro para FormEdtRede
		SalvaFormFilho(FormEdtRede);
	}
	catch (Exception &e)
	{ // nada a fazer
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditor::ActionEdtReguladorExecute(TObject *Sender)
{
	// cria editor de Regulador
	editor->EditaRegulador(ActionMouseOff, ActionMouseOn);
	// liga indica��o de mouse ativo e for�a bot�o Down
	AtualizaIndicacaoEditorOn(ButRegulador);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditor::ActionEdtPolylineExecute(TObject *Sender)
{
	// cria editor de Trecho
	editor->EditaPolyline(ActionMouseOff, ActionMouseOn, Parent);
	// liga indica��o de mouse ativo e for�a bot�o Down
	AtualizaIndicacaoEditorOn(ButEdtPolyline);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditor::ActionEdtReatorExecute(TObject *Sender)
{
	// cria editor de Reator
	editor->EditaReator(ActionMouseOff, ActionMouseOn);
	// liga indica��o de mouse ativo e for�a bot�o Down
	AtualizaIndicacaoEditorOn(ButEdtReator);
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/16534326
void __fastcall TFormEditor::ActionEdtSincExecute(TObject *Sender)
{
	// cria editor de Sincronismo de Chaves
//	editor->EditaSincronismo(ActionMouseOff, ActionMouseOn, Parent);
	// liga indica��o de mouse ativo e for�a bot�o Down
//	AtualizaIndicacaoEditorOn(ButSinc);

	//variaveis locais
	TForm *FormEdtSinc;

	// desabilita Edt atual
	editor->Disable();
	// desliga indica��o de mouse ativo
	AtualizaIndicacaoEditorOff();
	// esconde este Form
//	Hide();
	// cria Form para edi��o de manobra
	try
	{ // cria FormEdtSinc p/ editar sincronismo
		FormEdtSinc = new TFormEdtSinc(this, apl, ActionFormFilhoClosed, Parent);
		// exibe FormEdtSinc como janela normal
		FormEdtSinc->Show();
		// n�o destr�i o Form (ele se destr�i sozinho)
		// delete FormEdtSinc
		// salva ponteiro para FormEdtSinc
		SalvaFormFilho(FormEdtSinc);
	}
	catch (Exception &e)
	{ // nada a fazer
	}

}

// ---------------------------------------------------------------------------
void __fastcall TFormEditor::ActionEdtSuprimentoExecute(TObject *Sender)
{
	// cria editor de Suprimento
	editor->EditaSuprimento(ActionMouseOff, ActionMouseOn);
	// liga indica��o de mouse ativo e for�a bot�o Down
	AtualizaIndicacaoEditorOn(butSuprimento);
	// verifica se pode ser inserido um Suprimento
	if (!ValidaInsercaoGeracao())
	{ // cancela Edt atual
		editor->Disable();
		AtualizaIndicacaoEditorOff();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditor::ActionEdtSuprimentoPorFaseExecute(TObject *Sender)
{
	// cria editor de Suprimento
	editor->EditaSuprimentoPorFase(ActionMouseOff, ActionMouseOn);
	// liga indica��o de mouse ativo e for�a bot�o Down
	AtualizaIndicacaoEditorOn(butSuprimentoPorFase);
	// verifica se pode ser inserido um Suprimento
	if (!ValidaInsercaoGeracao())
	{ // cancela Edt atual
		editor->Disable();
		AtualizaIndicacaoEditorOff();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditor::ActionEdtSwitchExecute(TObject *Sender)
{
	// cria editor de Switch (abertura/fechamento de chave)
	editor->EditaSwitch(ActionMouseOff, ActionMouseOn);
	// liga indica��o de mouse ativo e for�a bot�o Down
	AtualizaIndicacaoEditorOn(butSwitch);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditor::ActionEdtTensaoExecute(TObject *Sender)
{
	// vari�veis locais
	TFormTensao *FormTensao;

	// desabilita Edt atual
	editor->Disable();
	// liga indica��o de mouse ativo e for�a bot�o Down
	AtualizaIndicacaoEditorOn(ButEdtTensao);
	// esconde este Form
	Hide();
	try
	{ // cria FormSelEqpto como janela normal
		FormTensao = new TFormTensao(this, apl, ActionFormFilhoClosed, Parent);
		FormTensao->Show();
		// n�o destr�i o Form (ele se destr�i sozinho)
		// delete FormEdtRede
		SalvaFormFilho(FormTensao);
	}
	catch (Exception &e)
	{ // nada a fazer
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditor::ActionEdtTrafoExecute(TObject *Sender)
{
	// cria editor de Trafo
	editor->EditaTrafo(ActionMouseOff, ActionMouseOn);
	// liga indica��o de mouse ativo e for�a bot�o Down
	AtualizaIndicacaoEditorOn(butTrafo);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditor::ActionEdtTrafo3EExecute(TObject *Sender)
{
	// cria editor de Trafo3E
	editor->EditaTrafo3E(ActionMouseOff, ActionMouseOn);
	// liga indica��o de mouse ativo e for�a bot�o Down
	AtualizaIndicacaoEditorOn(ButTrafo3E);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditor::ActionEdtTrafoMonoExecute(TObject *Sender)
{
	// cria editor de Trafo monof�sico
	editor->EditaTrafoMono(ActionMouseOff, ActionMouseOn);
	// liga indica��o de mouse ativo e for�a bot�o Down
	AtualizaIndicacaoEditorOn(ButTrafoMono);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditor::ActionEdtTrafoZZExecute(TObject *Sender)
{
	// cria editor de Trecho
	editor->EditaTrafoZZ(ActionMouseOff, ActionMouseOn);
	// liga indica��o de mouse ativo e for�a bot�o Down
	AtualizaIndicacaoEditorOn(ButTrafoZZ);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditor::ActionEdtTrechoExecute(TObject *Sender)
{
	// cria editor de Trecho
	editor->EditaTrecho(ActionMouseOff, ActionMouseOn);
	// liga indica��o de mouse ativo e for�a bot�o Down
	AtualizaIndicacaoEditorOn(butTrecho);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditor::ActionEsqAlignExecute(TObject *Sender)
{
	// cria editor de esquem�tico, alinhamento de Liga��es
	editor->EditaEsquematicoAlign(ActionMouseOff, ActionMouseOn);
	// liga indica��o de mouse ativo e for�a bot�o Down
	AtualizaIndicacaoEditorOn(ButEsqAlign);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditor::ActionEsqInverteXExecute(TObject *Sender)
{
	// vari�veis locais
	VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));
	// VTGrafico *graf  = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// verifica a exist�ncia de Reducoes
	if (ExisteReducao(redes))
	{
		Aviso("Esta altera��o n�o deve ser feita quando h� redes equivalentes");
		return;
	}
	edita->AlteraDiagEsqInverteX(redes);
	// atualiza o gr�fico
	// graf->Redraw();
	// graf->AreaRedeAlterada();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditor::ActionEsqInverteYExecute(TObject *Sender)
{
	// vari�veis locais
	VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));
	// VTGrafico *graf  = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// verifica a exist�ncia de Reducoes
	if (ExisteReducao(redes))
	{
		Aviso("Esta altera��o n�o deve ser feita quando h� redes equivalentes");
		return;
	}
	edita->AlteraDiagEsqInverteY(redes);
	// atualiza o gr�fico
	// graf->Redraw();
	// graf->AreaRedeAlterada();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditor::ActionEsqReiniciaExecute(TObject *Sender)
{
	// vari�veis locais
	VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));
	// VTGrafico *graf  = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	/*
	 //verifica a exist�ncia de Reducoes
	 if (ExisteReducao(redes))
	 {
	 Aviso("Esta altera��o n�o deve ser feita quando h� redes equivalentes");
	 return;
	 }
	 */
	// verifica a exist�ncia de Reducoes
	if (Confirma("Reiniciar diagrama esquem�tico igual ao geo-referenciado",
		"Esta opera��o n�o poder� ser desfeita.") != IDYES)
		return;
	edita->AlteraDiagEsqIgualDiagGeo(redes);
	// atualiza o gr�fico
	// graf->Redraw();
	// graf->AreaRedeAlterada();
}

//---------------------------------------------------------------------------
void __fastcall TFormEditor::ActionFigComentarioExecute(TObject *Sender)
   {
   //cria editor de Suprimento
   editor->EditaFigComentario(ActionMouseOff, ActionMouseOn);
   //liga indica��o de mouse ativo e for�a bot�o Down
   AtualizaIndicacaoEditorOn(ButFigComentario);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEditor::ActionFigLinkExecute(TObject *Sender)
   {
   //cria editor de Suprimento
   editor->EditaFigLinkComentarioEqpto(ActionMouseOff, ActionMouseOn);
   //liga indica��o de mouse ativo e for�a bot�o Down
   AtualizaIndicacaoEditorOn(ButFigLink);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEditor::ActionFigRetanguloExecute(TObject *Sender)
   {
   //cria editor de Suprimento
   editor->EditaFigRetangulo(ActionMouseOff, ActionMouseOn);
   //liga indica��o de mouse ativo e for�a bot�o Down
   AtualizaIndicacaoEditorOn(ButFigRetangulo);
   }

// ---------------------------------------------------------------------------
void __fastcall TFormEditor::ActionFormFilhoClosedExecute(TObject *Sender)
{
	// verifica se Sender �iguala FormFiljo
	if (Sender == FormFilho)
	{ // FormFilho foi fechado e destru�do
		FormFilho = NULL;
	}
	// destr�i FormEdtRede
	// if (FormEdtRede) {delete FormEdtRede; FormEdtRede = NULL;}
	// liga indica��o de mouse inativo e for�a bot�o Up
	AtualizaIndicacaoEditorOff();
	// habilita/desabilita comandos do Editor
	HabilitaEditor();
	// seleciona EdtAltera
	ActionEdtAltera->Execute();
	// exibe este Form
	Show();
	// FKM - 2015.10.06 Arruma a exibi��o do form
//	ActionAlignNone->Execute();
	//considera ultimo alinhamento determinado
	action_align->Execute();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditor::ActionHelpExecute(TObject *Sender)
{
	// variaveis locais
	TForm *formHelp;

	formHelp = DLL_NewFormHelp(this, apl, "Editor");
	if (formHelp)
		formHelp->Show();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditor::ActionGiroExecute(TObject *Sender)
{
	// cria editor de Suprimento
	editor->EditaGiro(ActionMouseOff, ActionMouseOn);
	// liga indica��o de mouse ativo e for�a bot�o Down
	AtualizaIndicacaoEditorOn(ButGiro);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditor::ActionImportaBarrasExecute(TObject *Sender)
{
	// vari�veis locais
	TFormImportaBarras *form;

	// desabilita Edt atual
	editor->Disable();
	// liga indica��o de mouse ativo e for�a bot�o Down
	AtualizaIndicacaoEditorOn(butAltera);
	try
	{ // cria Form como janela normal
		form = new TFormImportaBarras(this, apl);
		form->Show();
		// n�o destr�i o Form (ele se destr�i sozinho)
		// delete form;
		SalvaFormFilho(form);
	}
	catch (Exception &e)
	{ // nada a fazer
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditor::ActionInsCarroEletricoExecute(TObject *Sender)
{
	// vari�veis locais
	TFormCarro *FormCarro;

	try
	{ // cria TFormCarro e exibe como janela modal
		FormCarro = new TFormCarro(this, apl);
		FormCarro->ShowModal();
		delete FormCarro;
	}
	catch (Exception &e)
	{ // nada a fazer
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditor::ActionInsereFusivelExecute(TObject *Sender)
{ // DVK 2016.07.29
	// vari�veis locais
	bool sucesso = false;
	AnsiString arquivo;
	TInsereFusivel *insere_fus;
	VTProgresso *prog = (VTProgresso*)apl->GetObject(__classid(VTProgresso));

	if (prog == NULL)
	{ // cria progresso
		prog = DLL_NewObjProgresso(this, this->Parent);
		if (prog)
			apl->Add(prog);
	}
	// importa arquivo
	OpenDialog->Title = "Importar Arquivo com Tipos de Fus�veis por Trafo";
	arquivo = ImportaArquivo();
	if (arquivo.IsEmpty())
		return;
	switch (Confirma("A inser��o de chaves fus�veis � feita nos trafos MT/BT", "Deseja continuar?"))
	{
	case IDYES:
		// mostra progresso
		if (prog)
			prog->Start(progDEFAULT);
		// cria objeto p/ inserir fusiveis
		insere_fus = new TInsereFusivel(apl);
		sucesso = insere_fus->Insere(arquivo);
		delete insere_fus;
		if (sucesso)
			Aviso("Fus�veis inseridos com sucesso");
		else
			Aviso("Erro ao inserir fus�veis");
		break;
	case IDNO:
		return;
	case IDCANCEL:
		return;
	}
	if (prog)
		prog->Stop();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditor::ActionMouseOffExecute(TObject *Sender)
{
	// liga indica��o de mouse inativo e for�a bot�o Up
	AtualizaIndicacaoEditorOff();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditor::ActionMouseOnExecute(TObject *Sender)
{
	// liga indica��o de mouse ativo e for�a bot�o Down
	AtualizaIndicacaoEditorOn(button_down);
}

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/14217067
void __fastcall TFormEditor::ActionRecondutorarExecute(TObject *Sender)
{
	// vari�veis locais
	TFormRecondutorar *form;

	// desabilita Edt atual
	editor->Disable();
	// liga indica��o de mouse ativo e for�a bot�o Down
	AtualizaIndicacaoEditorOn(butAltera);
	try
	{ // cria Form como janela normal
		form = new TFormRecondutorar(this, apl, ActionFormFilhoClosed, Parent);
		form->Show();
		// n�o destr�i o Form (ele se destr�i sozinho)
		// delete FormEdtRede
		SalvaFormFilho(form);
		// n�o destr�i o Form (ele se destr�i sozinho)
		// delete form;
	}
	catch (Exception &e)
	{ // nada a fazer
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditor::ActionRemoveFusivelExecute(TObject *Sender)
{ // DVK 2016.07.29
	// vari�veis locais
	bool sucesso = false;
	TInsereFusivel *insere_fus;
	VTProgresso *prog = (VTProgresso*)apl->GetObject(__classid(VTProgresso));

	if (prog == NULL)
	{ // cria progresso
		prog = DLL_NewObjProgresso(this, this->Parent);
		if (prog)
			apl->Add(prog);
	}
	switch (Confirma("A remo��o de chaves fus�veis � feita nos trafos MT/BT", "Deseja continuar ?"))
	{
	case IDYES:
		// mostra progresso
		if (prog)
			prog->Start(progDEFAULT);
		// cria objeto p/ remover fusiveis
		insere_fus = new TInsereFusivel(apl);
		sucesso = insere_fus->Remove();
		delete insere_fus;
		if (sucesso)
			Aviso("Remo��o de fus�veis feita com sucesso");
		else
			Aviso("Erro ao remover fus�veis");
		break;
	case IDNO:
		return;
	case IDCANCEL:
		return;
	}
	if (prog)
		prog->Stop();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditor::ActionSelEqptoExecute(TObject *Sender)
{
	// vari�veis locais
	TFormSelEqpto *FormSelEqpto;

	// desabilita Edt atual
	editor->Disable();
	// liga indica��o de mouse ativo e for�a bot�o Down
	AtualizaIndicacaoEditorOn(ButSelEqpto);
	// esconde este Form
	Hide();
	try
	{ // cria FormSelEqpto como janela normal
		FormSelEqpto = new TFormSelEqpto(this, apl, ActionFormFilhoClosed, Parent);
		FormSelEqpto->Show();
		// n�o destr�i o Form (ele se destr�i sozinho)
		// delete FormEdtRede
		SalvaFormFilho(FormSelEqpto);
	}
	catch (Exception &e)
	{ // nada a fazer
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditor::ActionSelNoneExecute(TObject *Sender)
{ // DVK 2014.08.01
	// vari�veis locais
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	// poderia ser "editor->EvtEscape();" direto
	// limpa lista de eqptos selecionados
	if (!editor->LisEqptoSelecionado())
		return;
	editor->LisEqptoSelecionado()->Clear();
	// liga indica��o de mouse ativo e for�a bot�o Down
	AtualizaIndicacaoEditorOn(NULL);
	grafico->Redraw();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditor::ActionRedoExecute(TObject *Sender)
{
	// vari�veis locais
	VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));
	// VTGrafico *graf  = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	// executa ActionRedo do objeto TEdita
	edita->ActionRedo->Execute();
	// atualiza o gr�fico
	// graf->Redraw();
	// habilita/desabilita comandos do Editor
	HabilitaEditor();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditor::ActionUndoExecute(TObject *Sender)
{
	// vari�veis locais
	VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));
	// VTGrafico *graf  = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	// executa ActionUndo do objeto TEdita
	edita->ActionUndo->Execute();
	// atualiza o gr�fico
	// graf->Redraw();
	// habilita/desabilita comandos do Editor
	HabilitaEditor();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditor::ActionZoomExecute(TObject *Sender)
{
	// cria editor que trata Zoom no bot�o esquerda do mouse
	editor->EditaZoom(ActionMouseOff, ActionMouseOn);
	// liga indica��o de mouse ativo e for�a bot�o Down
	AtualizaIndicacaoEditorOn(ButZoom);
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/14004105
void __fastcall TFormEditor::AtualizaIndicacaoEditorOff(void)
{
	//desativa atalhos ligados a copiar e colar
	ActionEdtCopia->ShortCut = scNone;
	ActionEdtCola->ShortCut = scNone;
	// indica mouse inativo
	ImageMouseOff->Visible = true;
	ImageMouseOn->Visible = false;
	// "levanta" Button que est� abaixado
	if (button_down)
	{ // "abaixa" o Button
		button_down->Down = false;
	}
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/14004105
void __fastcall TFormEditor::AtualizaIndicacaoEditorOn(TToolButton *button)
{
	//ativa atalhos ligados a copiar e colar
	ActionEdtCopia->ShortCut = TextToShortCut("Ctrl+C");
	ActionEdtCola->ShortCut = TextToShortCut("Ctrl+V");
	//indica mouse ativo
	ImageMouseOn->Visible = true;
	ImageMouseOff->Visible = false;
	//verifica se existia um Button dowwn
	if (button_down)
	{//"levanta" o Button
		button_down->Down = false;
	}
	//for�a bot�o Down
	if (button)
	{ //"abaixa" o Button
		button->Down = true;
		//salva ponteiro do Button
		button_down = button;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditor::ConfiguraOpcaoPlanejamento(void)
{
	// desabilita comandos n�o dispon�veis p/ planejamento
	ToolBarEsq->Visible = false;
	ActionGiro->Visible = false;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditor::ConfiguraOpcaoUsuario(void)
{
	// vari�veis locais
	bool visible = false;
	VTLicenca *licenca;
	VTLicencas *licencas = (VTLicencas*)apl->GetObject(__classid(VTLicencas));

	// obt�m Licenca
	if (licencas != NULL)
	{
		if ((licenca = licencas->ExisteLicenca()) != NULL)
		{ // verifica se usu�rio privilegiado
			visible = (licenca->Tipo == tipoMASTER);
		}
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TFormEditor::ExisteReducao(VTRedes *redes)
{
	// vari�veis locais
	VTRede *rede;
	TList *lisREDE = redes->LisRede();

	for (int nr = 0; nr < lisREDE->Count; nr++)
	{
		rede = (VTRede*)lisREDE->Items[nr];
		if (!rede->Carregada)
			return (true);
	}
	return (false);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditor::FormClose(TObject *Sender, TCloseAction &Action)
{
	// destr�i o form
	Action = caFree;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditor::FormIniciaPosicao(void)
{
	// vari�veis locais
	int height;
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

	//verifica se o alinhamento foi definido antes, diferente do None
	if(action_align != ActionAlignNone)
		return;
	// define posi��o default da janela flutuante
	janela.top = 0;
	janela.left = 0;
	janela.width = Width;
	janela.height = Height;
	if (geral)
	{ // l� par�metros do Form
		if (geral->Infoset->LoadFromFile(this))
		{
			switch (Align)
			{
			case alLeft:
				ActionAlignLeft->Execute();
				break;
			case alRight:
				ActionAlignRight->Execute();
				break;
			case alTop:
				ActionAlignTop->Execute();
				break;
			case alBottom:
				ActionAlignBottom->Execute();
				break;
			case alNone: // salva �ltima
				janela.top = Top;
				janela.left = Left;
				janela.width = Width;
				janela.height = Height;
				// acerta dimens�o em fun��o do GBoxGrupo vis�vel ou n�o
				height = ToolBarEqpto->Height + ToolBarEsq->Height;
				if (GBoxGrupo->Visible)
				{
					if (ClientHeight <= height)
					{
						ClientHeight = height + 15;
						janela.height = Height;
					}
				}
				else
				{
					if (ClientHeight > height)
					{
						ClientHeight = height;
						janela.height = Height;
					}
				}
				break;
            default: break;
			}
			return;
		}
	}
	// posiciona o Form sem alinhamento
	ActionAlignNoneExecute(NULL);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditor::FormShow(TObject *Sender)
{
	// habilita/desabilita comandos do Editor
	HabilitaEditor();
	// posiciona o Form
	FormIniciaPosicao();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditor::HabilitaEditor(void)
{
	// vari�veis locais
	bool enabled;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	TList *lisREDE = redes->LisRede();

	// verifica se h� pelo menos 1 Rede em Redes
	enabled = (lisREDE->Count > 0);
	// habilita/desabilita Buttons de ToolBarEsq
	for (int n = 0; n < ToolBarEsq->ButtonCount; n++)
	{
		ToolBarEsq->Buttons[n]->Enabled = enabled;
	}
	// habilita/desabilita Buttons de ToolBarEqpto
	for (int n = 0; n < ToolBarEqpto->ButtonCount; n++)
	{
		if (ToolBarEqpto->Buttons[n] == butRedo)
			continue;
		if (ToolBarEqpto->Buttons[n] == butUndo)
			continue;
		ToolBarEqpto->Buttons[n]->Enabled = enabled;
	}
	// habilita butAlign, butRede, ButEdtCola
	ButAlign->Enabled = true;
	butBarra->Enabled = true;
	butRede->Enabled = true;
	ButEdtCola->Enabled = true;
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TFormEditor::ImportaArquivo(void)
{
	// variaveis locais
	AnsiString filename = "";
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));

	// define pasta default
	if (!path)
		return ("");
	OpenDialog->InitialDir = path->DirImporta();
	// abre arquivo
	if (OpenDialog->Execute())
	{
		filename = OpenDialog->FileName;
		// verifica se o arquivo existe
		if (!FileExists(filename))
			return ("");
	}
	return (filename);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditor::SalvaFormFilho(TForm *form_filho)
{
	// destr�i eventual FormFilho que esteja aberto
	if (FormFilho)
	{
		delete FormFilho;
		FormFilho = NULL;
	}
	// salva o novo FormFilho
	FormFilho = form_filho;
}

// ---------------------------------------------------------------------------
bool __fastcall TFormEditor::ValidaInsercaoGeracao(void)
{
	// vari�veis locais
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	return (redes->ValidaInsercaoGeracao());
}

// ---------------------------------------------------------------------------
// eof
// fun��es comentadas, pois ainda n�o usam Edita
void __fastcall TFormEditor::ActionEditaSEDExecute(TObject *Sender)
{ /*
	 //variaveis locais
	 bool        sucesso = false;
	 AnsiString  arquivo;
	 TEditaSED   *edita_sed;
	 VTProgresso *prog    = (VTProgresso*)apl->GetObject(__classid(VTProgresso));

	 if(prog == NULL)
	 {//cria progresso
	 prog = DLL_NewObjProgresso(this, this->Parent);
	 if(prog) apl->Add(prog);
	 }
	 //importa arquivo
	 OpenDialog->Title = "Importar Arquivo com Pot�ncias de Curto dos Suprimentos";
	 arquivo = ImportaArquivo();
	 if(arquivo.IsEmpty()) return;
	 switch(Confirma("Essa a��o � irrevers�vel.", "Deseja continuar ?"))
	 {
	 case IDYES:
	 //mostra progresso
	 if(prog) prog->Start(progDEFAULT);
	 //cria objeto p/ editar a SED
	 edita_sed  = new TEditaSED(apl);
	 sucesso = edita_sed->Executa(arquivo);
	 delete edita_sed;
	 if(sucesso) Aviso("Deu certo");
	 else        Aviso("Erro");
	 break;
	 case IDNO:
	 case IDCANCEL: return;
	 }
	 if(prog) prog->Stop();
	 */
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditor::ActionEqvBTExecute(TObject *Sender)
{ /*
	 //variaveis locais
	 bool        sucesso = false;
	 AnsiString  arquivo;
	 TRemoveBT   *remove_bt;
	 VTProgresso *prog    = (VTProgresso*)apl->GetObject(__classid(VTProgresso));

	 if(prog == NULL)
	 {//cria progresso
	 prog = DLL_NewObjProgresso(this, this->Parent);
	 if(prog) apl->Add(prog);
	 }
	 switch(Confirma("Essa a��o � irrevers�vel.", "Deseja continuar?"))
	 {
	 case IDYES:
	 //mostra progresso
	 if(prog) prog->Start(progDEFAULT);
	 //cria objeto p/ remover redes BT
	 remove_bt  = new TRemoveBT(apl);
	 sucesso = remove_bt->Equivalenta();
	 delete remove_bt;
	 if(sucesso) Aviso("Deu certo");
	 else        Aviso("Erro");
	 break;
	 case IDNO:     return;
	 case IDCANCEL: return;
	 }
	 if(prog) prog->Stop();
	 */
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditor::ActionRemoveBTExecute(TObject *Sender)
{ /*
	 //variaveis locais
	 bool        sucesso = false;
	 AnsiString  arquivo;
	 TRemoveBT   *remove_bt;
	 VTProgresso *prog    = (VTProgresso*)apl->GetObject(__classid(VTProgresso));

	 if(prog == NULL)
	 {//cria progresso
	 prog = DLL_NewObjProgresso(this, this->Parent);
	 if(prog) apl->Add(prog);
	 }
	 switch(Confirma("A remo��o das redes BT deve ser feita \n"
	 "depois de equivalentar as cargas.",
	 "Deseja continuar?"))
	 {
	 case IDYES:
	 //mostra progresso
	 if(prog) prog->Start(progDEFAULT);
	 //cria objeto p/ remover redes BT
	 remove_bt  = new TRemoveBT(apl);
	 sucesso = remove_bt->Executa();
	 delete remove_bt;
	 if(sucesso) Aviso("Deu certo");
	 else        Aviso("Erro");
	 break;
	 case IDNO:     return;
	 case IDCANCEL: return;
	 }
	 if(prog) prog->Stop();
	 */
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditor::ActionRemoveTrechosExecute(TObject *Sender)
{ /*
	 //variaveis locais
	 bool            sucesso = false;
	 TRemoveTrecho   *remove;
	 VTProgresso     *prog    = (VTProgresso*)apl->GetObject(__classid(VTProgresso));

	 if(prog == NULL)
	 {//cria progresso
	 prog = DLL_NewObjProgresso(this, this->Parent);
	 if(prog) apl->Add(prog);
	 }
	 switch(Confirma("Essa a��o � irrevers�vel.", "Deseja continuar?"))
	 {
	 case IDYES:
	 //mostra progresso
	 if(prog) prog->Start(progDEFAULT);
	 //cria objeto p/ remover trechos duplicados
	 remove  = new TRemoveTrecho(apl);
	 sucesso = remove->Executa();
	 delete remove;
	 if(sucesso) Aviso("Deu certo");
	 else        Aviso("Erro");
	 break;
	 case IDNO:     return;
	 case IDCANCEL: return;
	 }
	 if(prog) prog->Stop();
	 */
}

// ---------------------------------------------------------------------------
//eof


