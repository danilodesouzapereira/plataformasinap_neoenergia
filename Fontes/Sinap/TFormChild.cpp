// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DateUtils.hpp>
#include <DLL_Inc\CalcIndCont.h>
#include "TFormChild.h"
#include "TFormViewArranjo.h"
#include "TFormViewFase.h"
#include "VTSinapChild.h"
#include "..\Alerta\VTAlerta.h"
#include "..\Apl\VTApl.h"
#include "..\Arranjo\VTArranjos.h"
#include "..\BD\VTSNPDBConn.h"
#include "..\BDihm\VTAbre.h"
#include "..\BDihm\VTSalva.h"
#include "..\Consulta\VTConsulta.h"
#include "..\Curvas\VTCurva.h"
#include "..\Curvas\VTCurvas.h"
#include "..\Diretorio\VTDir.h"
#include "..\Diretorio\VTPath.h"
#include "..\Edita\VTEdita.h"
#include "..\Esquematico\VTEsquematico.h"
#include "..\Figura\TFiguras.h"
#include "..\Figura\Estilos\VTEstilos.h"
#include "..\Flow\VTFlow.h"
#include "..\Forms\TFormEdtTxt.h"
#include "..\Geral\CfgPerdaTec\VTCfgPerdaTec.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Grafico\VTGrafico.h"
#include "..\ImportaRedeGIS\VTImportaRedeGIS.h"
#include "..\ImportaBDGD\TImportaBDGD.h"
#include "..\Licenca\VTLicenca.h"
#include "..\Licenca\VTLicencas.h"
#include "..\Manobra\VTManobra.h"
#include "..\Manobra\VTManobras.h"
#include "..\Mercado\VTMercado.h"
#include "..\MultiMonitor\TFormMultiMonitor.h"
#include "..\MultiMonitor\VTMultiPanel.h"
#include "..\Obra\VTObra.h"
#include "..\Obra\VTLixeira.h"
#include "..\Patamar\VTPatamar.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Planejamento\VTEstudo.h"
#include "..\Planejamento\VTPlanejamento.h"
#include "..\Progresso\VTProgresso.h"
#include "..\Potencia\VTDemanda.h"
#include "..\RBAC\VTRBACManager.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTTipoRede.h"
#include "..\Redegraf\VTRedegraf.h"
#include "..\Redutor\VTRedutor.h"
#include "..\Stk\VTStkEqv.h"
#include "..\Topologia\VTTopologia.h"
#include "..\..\DLL_Inc\BDihm.h"
#include "..\..\DLL_Inc\Consulta.h"
#include "..\..\DLL_Inc\Curtocircuito.h"
#include "..\..\DLL_Inc\Editor.h"
#include "..\..\DLL_Inc\Especial.h"
#include "..\..\DLL_Inc\Esquematico.h"
#include "..\..\DLL_Inc\Estudo.h"
#include "..\..\DLL_Inc\Explorer.h"
#include "..\..\DLL_Inc\Fasometro.h"
#include "..\..\DLL_Inc\Figura.h"
#include "..\..\DLL_Inc\Flow.h"
#include "..\..\DLL_Inc\FluxoH3.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Grafico.h"
#include "..\..\DLL_Inc\Help.h"
#include "..\..\DLL_Inc\Historico.h"
#include "..\..\DLL_Inc\ImportaAnafas.h"
#include "..\..\DLL_Inc\ImportaAnarede.h"
#include "..\..\DLL_Inc\ImportaBDGD.h"
#include "..\..\DLL_Inc\ImportaCurva.h"
#include "..\..\DLL_Inc\InterfaceTXT.h"
#include "..\..\DLL_Inc\InterfaceXML.h"
#include "..\..\DLL_Inc\ImportaBDGD.h"
#include "..\..\DLL_Inc\Localiza.h"
#include "..\..\DLL_Inc\MapaTematico.h"
#include "..\..\DLL_Inc\Medidor.h"
#include "..\..\DLL_Inc\Mercado.h"
#include "..\..\DLL_Inc\Monopolar.h"
#include "..\..\DLL_Inc\Planejamento.h"
#include "..\..\DLL_Inc\Progresso.h"
#include "..\..\DLL_Inc\Reclamacao.h"
#include "..\..\DLL_Inc\Redutor.h"
#include "..\..\DLL_Inc\SinapApl.h"
#include "..\..\DLL_Inc\SinapToolKit.h"
#include "..\..\DLL_Inc\Spy.h"
#include "..\..\DLL_Inc\Stk.h"
// ---------------------------------------------------------------------------
#pragma resource "*.dfm"
// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16131085
//https://sinapsisenergia.teamwork.com/#/tasks/18682517
__fastcall TFormChild::TFormChild(TComponent *Owner, VTApl *apl_owner, TAction *ActionOnClose,
	int num_seq, int stk_option) : TForm(Owner)
{
	// vari�veis locais
	VTGrafico *grafico;
	VTEdita *edita;

	// salva TAction a ser executado quando este form for fechado
	this->ActionOnClose = ActionOnClose;
	// salva n�mero sequencial em Tag
	Tag = num_seq;
	// maximiza o Form
	WindowState = wsMaximized;
	Refresh();
	// cria objetos
	apl = DLL_NewObjAplChild(this, apl_owner, ActionInsRede);
	// configura objeto Edita
	edita = (VTEdita*)apl->GetObject(__classid(VTEdita));
	edita->ActionOnChangeRedeInsere(ActionRedeChanged);
	// cria e inicia objeto VTSinapChild
	IniciaObjSinapChild();
	// cria objeto MultiPanel p/ exibir janelas em 2 monitores
	IniciaMultiPanel();
	// atualiza Caption do Form e do AcctionFlow
	AtualizaCaptionForm();
	AtualizaCaptionActionFlow();
	// esconde PanelAlerta  - passado p/ o FormMDI  p/ evitar efeito feio na tela
	// PanelAlerta->Visible = false;
	// configura Grafico
	grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	grafico->DefineParent(panelGrafico);
	grafico->DefineParentToolBar(panelToolBarGrafico);
	//grafico->Show(grafTOPO);
	grafico->Disable();
	// inicia CBoxPatamar
	CBoxPatamarInicia();
	// configura Actions de acordo c/ m�dulos carregados
	VerificaModulos();
	// inicia SinapToolKIt
	SinapToolKitInicia(stk_option);
	// ajusta ToolBar em fun��o da dimens�o da tela
	if (Screen->Width < 1200)
	{ // ajusta ToolBar
		ToolBar->Height = 2 * ToolBar->Height;
		ToolBar->Wrapable = true;
	}
	// habilita aviso ao salvar a base
	aviso_base_salva = true;
    //n�o est� ocupado
    ocupado = false;

//    //desabilita alguns m�dulos se a licen�a for para SINAPviewer
//	 DesabilitaModulos();
}
// ---------------------------------------------------------------------------
__fastcall TFormChild::~TFormChild(void)
{
	// vari�veis locais
	VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	VTSinapChild *sinap_child = (VTSinapChild*)apl->GetObject(__classid(VTSinapChild));

	// desabilita tratamento do evento OnActivate
	// OnActivate = NULL;
	// retira Action de VTEdita
	edita->ActionOnChangeRedeRetira(ActionRedeChanged);
	// desabilita o gr�fico
	grafico->Disable();
	// desabilita FormStk
	sinap_child->FormStkClose();
    sinap_child->RemoveBotaoEditor(butEditor);
	// fecha todas janelas
	DLL_CloseFormHelp(this);
	CloseForm("TFormViewArranjo");
	CloseForm("TFormViewFase");
	DLL_FormChildStop(this);
	DLL_Especial_Stop(this);
	DLL_MapaTematico_Stop(this);
	// DLL_CloseFormConfiabilidade(this);
	// DVK 2017.05.02 fecha form do continuidade
	CloseForm("TFormCalcIndCont");
	DLL_CloseFormCurto(this);
	DLL_CloseFormEditor(this);
	DLL_CloseFormEsquematico(this);
	DLL_CloseFormExplorer(this);
	DLL_CloseFormMonopolar(this);
	DLL_CloseFormPadrao(this);
	DLL_CloseFormFasometro(this);
	DLL_CloseFormLocaliza(this);
	DLL_CloseFormMedidor(this);
	DLL_CloseFormEstudo(this);
	DLL_CloseFormReclamacao(this);
	// fecha o gr�fico
	grafico->Close();
	// retira MultiPanel
	if (FormMultiMonitor)
	{
		FormMultiMonitor->RetiraMultiPanel(MultiPanel);
	}
	// destr�i objetos
	if (apl)
	{
		delete apl;
		apl = NULL;
	}
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16131085
void __fastcall TFormChild::ActionAberturaMonopolarExecute(TObject *Sender)
{
//	VTRBACManager *rbac = (VTRBACManager*)apl->GetObject(__classid(VTRBACManager));
//
//	// RBAC
//	if (!rbac->ExistePermissaoModulo(modABERTURAFASE))
//	{
//		Aviso("Sua licen�a de uso n�o tem permiss�o para " + rbac->GetDescricaoOperacao(opREDE));
//		  ActionTopoExecute(NULL);
//		return;
//	}
	// executa fluxo de pot�ncia
	// ActionFlow->Execute();
	// abaixa Button
	ButAberturaMonopolar->Down = true;
	// inicia e exibe CBoxPatamar
	CBoxPatamarInicia();
	CBoxPatamar->Visible = true;
	// seleciona patamar de maior demanda
	SelecionaPatamarMaiorDemanda();
	// exibe FormMonopolar
	DLL_NewFormMonopolar(this, apl, SelecionaParentParaJanela(), CBoxPatamar->ItemIndex);
}

// ---------------------------------------------------------------------------
void __fastcall TFormChild::ActionAbreAllExecute(TObject *Sender)
{
	// vari�veis locais
	bool sucesso = false;
	VTAbre *abre;

	// cria objeto VTAbre
	if ((abre = DLL_NewObjAbre(this, apl)) != NULL)
	{ // carrega todas as redes da base
		if (abre->CarregaRedeCompleta())
		{ // reinicia este Form c/ a rede aberta
			Reinicia();
			sucesso = true;
		}
	}
	// destr�i objeto criado
	delete abre;
	// se as redes n�o foram abertas, fecha este Form
	if (!sucesso)
		Close();
}
/*
 //---------------------------------------------------------------------------
 void __fastcall TFormChild::ActionAbreNoneExecute(TObject *Sender)
 {
 //vari�veis locais
 VTImportaTXT *importaTXT;
 VTPath       *path = (VTPath*)apl->GetObject(__classid(VTPath));

 try{//l� arquivo TXT com configura��o de tipos, arranjos e arranjos da empresa
 importaTXT = DLL_NewObjImportaTXT(apl);
 //importa arquivo TXT
 importaTXT->ImportaConfiguracao(path->ArqCfgEmpresa());
 //destr�i objeto
 delete importaTXT;
 //reincia tela
 Reinicia();
 }catch(Exception &)
 {
 Aviso("N�o foi poss�vel criar a nova base de dados");
 Close();
 }
 }
 */

// ---------------------------------------------------------------------------
void __fastcall TFormChild::ActionAbreNoneExecute(TObject *Sender)
{
	// reincia tela
	Reinicia();
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/14422280
//https://sinapsisenergia.teamwork.com/#tasks/16011612
void __fastcall TFormChild::ActionAbreSelExecute(TObject *Sender)
{
	// vari�veis locais
	TForm *Form;
	VTEstudo *estudo;
	VTPlanejamento *planejamento = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));
	VTSinapChild *sinap_child = (VTSinapChild*)apl->GetObject(__classid(VTSinapChild));

	// exibe FormAbre p/ sele��o do estudo
	Form = DLL_NewFormAbre(this, apl);
	if (Form->ShowModal() != mrOk)
	{ // usu�rio cancelou sele��o de Redes: fecha este form
		Close();
		return;
	}
	// obt�m objeto Estudo
	estudo = planejamento->ExisteEstudo();
	// verifica o tipo do Estudo aberto
	switch (estudo->Tipo)
	{
	case estudoREDE_ORIGINAL: // nada a fazer
		// reinicia este Form c/ a rede aberta
		Reinicia();
		break;
	case estudoPLANEJAMENTO:
		// bloqueia o uso do Editor
//		ActionEditor->Enabled = false;
		sinap_child->HabilitaEditor(false);
		// cria TFormEstudo
		Form = DLL_NewFormEstudo(this, apl, SelecionaParentParaJanela());
		Form->Show();
		// reinicia este Form c/ a rede aberta
		Reinicia();
		break;
	case estudoRECLAMACAO_BT:
		// bloqueia o uso do Editor
//		ActionEditor->Enabled = false;
		sinap_child->HabilitaEditor(false);
		// cria TFormReclamacao
		Form = DLL_NewFormReclamacaoBT(this, apl, SelecionaParentParaJanela());
		Form->Show();
		// reinicia este Form c/ a rede aberta
		Reinicia();
		break;
	case estudoRECLAMACAO_MT:
		// bloqueia o uso do Editor
//		ActionEditor->Enabled = false;
        sinap_child->HabilitaEditor(false);
		// cria TFormReclamacao
		Form = DLL_NewFormReclamacaoMT(this, apl, SelecionaParentParaJanela());
		Form->Show();
		// reinicia este Form c/ a rede aberta
		Reinicia();
		break;
	default: // nada a fazer
		break;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormChild::ActionGraficoAnimacaoExecute(TObject *Sender)
{
	// vari�veis locais
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	// altera habilita��o da anima��o
	ActionGraficoAnimacao->Checked = (!ActionGraficoAnimacao->Checked);
	// configura gr�fico
	grafico->AnimacaoEnabled = ActionGraficoAnimacao->Checked;
}

// ---------------------------------------------------------------------------
void __fastcall TFormChild::ActionBatchDensidadeCargaExecute(TObject *Sender)
{
	// vari�veis locais
	TForm *form;

	// cria FormBatchDC p/ importar a Rede
	if ((form = DLL_NewFormBatchDC(this, apl)) == NULL)
		return;
	// exibe form como janela modal
	form->ShowModal();
	// destr�i FormHistorico
	delete form;
	// desabilita op��o de salvar na base de dados
	ActionSalva->Enabled = false;
	// fecha este form
	Close();
}

// ---------------------------------------------------------------------------
void __fastcall TFormChild::ActionBatchImportaGisExecute(TObject *Sender)
{
	// vari�veis locais
	TForm *form;

	// cria FormHistorico p/ importar a Rede
	if ((form = DLL_NewFormBatchIG(this, apl)) == NULL)
		return;
	// exibe form como janela modal
	form->ShowModal();
	// destr�i FormHistorico
	delete form;
	// desabilita op��o de salvar na base de dados
	ActionSalva->Enabled = false;
	// fecha este form
	Close();
}

// ---------------------------------------------------------------------------
void __fastcall TFormChild::ActionBatchPerdaregExecute(TObject *Sender)
{
	// vari�veis locais
	TForm *form;

	// cria TFormBatchPR p/ importar a Rede
	if ((form = DLL_NewFormBatchPR(this, apl)) == NULL)
		return;
	// exibe form como janela modal
	form->ShowModal();
	// destr�i TFormBatchPR
	delete form;
	// desabilita op��o de salvar na base de dados
	ActionSalva->Enabled = false;
	// fecha este form
	Close();
}

// ---------------------------------------------------------------------------
void __fastcall TFormChild::ActionBatchPerdatecExecute(TObject *Sender)
{
	// vari�veis locais
	TForm *form;

	// cria TFormBatchPT p/ importar a Rede
	if ((form = DLL_NewFormBatchPT(this, apl)) == NULL)
		return;
	// exibe form como janela modal
	form->ShowModal();
	// destr�i TFormBatchPT
	delete form;
	// desabilita op��o de salvar na base de dados
	ActionSalva->Enabled = false;
	// fecha este form
	Close();
}
/*
 //---------------------------------------------------------------------------
 void __fastcall TFormChild::ActionConfiabilidadeExecute(TObject *Sender)
 {
 //vari�veis locais
 int       ind_pat = 0;
 int       ano     = 0;
 VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

 //prote��o para execu��o do m�dulo de Confiabilidade
 if (! ValidaExecucaoConfiabilidade())
 {//seleciona gr�fico de topologia
 ActionTopo->Execute();
 return;
 }
 //fecha janelas
 FechaJanelas();
 //esconde CBoxPatamar
 CBoxPatamar->Visible = false;
 //abaixa Button
 ButConfiabilidade->Down = true;
 //exibe tela de confiabilidade
 DLL_NewFormConfiabilidade(this, apl, SelecionaParentParaJanela());
 //exibe gr�fico de Curto DEPOIS de ter aberto tela de Confiabilidade (que cria objeto RedeCFB)
 grafico->Show(grafCONFIAB, ano, ind_pat);
 }
 */

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16131085
void __fastcall TFormChild::ActionContinuidadeExecute(TObject *Sender)
{
	// vari�veis locais
	TForm *Form;
//	VTRBACManager *rbac = (VTRBACManager*)apl->GetObject(__classid(VTRBACManager));

//	// RBAC
//	if (!rbac->ExistePermissaoModulo(modCONTINUIDADE))
//	{
//		Aviso("Sua licen�a de uso n�o tem permiss�o para " + rbac->GetDescricaoOperacao
//			(opCONTINUIDADE));
//		  ActionTopoExecute(NULL);
//		return;
//	}

	try
	{
		if (TrocaRedeEqvBTporCargaEqv(apl))
		{
			if ((Form = DLL_NewFormCalcIndCont(this, apl, MultiPanel->PanelAtivo)) != NULL)
			{
				Form->Show();
			}
		}
		else
		{
			ActionTopoExecute(NULL);
		}
	}
	catch (Exception &e)
	{
		Aviso("N�o foi poss�vel iniciar o m�dulo de Continuidade");
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormChild::ActionConverteBaseExecute(TObject *Sender)
{
	/*
	 //vari�veis locais
	 TForm      *form;
	 VTGrafico  *grafico  = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	 //cria TForm p/ importar a Rede
	 if ((form = DLL_SNP_NewFormConversor(this, apl)) == NULL) return;
	 //exibe form como janela modal
	 if (form->ShowModal() == mrOk)
	 {//exibe gr�fico de topologia
	 grafico->AreaRedeDefine();
	 grafico->Show(grafTOPO, 0, 0);
	 }
	 else
	 {//fecha este form
	 Close();
	 }
	 //destr�i TFormImporta
	 delete form;
	 */
}

// ---------------------------------------------------------------------------
void __fastcall TFormChild::ActionCriarEsquematicoExecute(TObject *Sender)
{
	/*
	 //vari�veis locais
	 VTEsquematico *esquematico;
	 VTGrafico     *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	 VTRedes       *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));

	 try{//
	 esquematico = DLL_NewObjEsquematico(apl);
	 esquematico->IniciaEsquematico(redes);
	 delete esquematico;
	 //atualiza o gr�fico
	 //grafico->Redraw();
	 grafico->AreaRedeAlterada();
	 } catch(Exception &e)
	 {
	 }
	 */
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16131085
void __fastcall TFormChild::ActionCurtoExecute(TObject *Sender)
{
	// vari�veis locais
	int ind_pat;
	int ano = 0;
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
//	VTRBACManager *rbac = (VTRBACManager*)apl->GetObject(__classid(VTRBACManager));

//	// RBAC
//	if (!rbac->ExistePermissaoModulo(modCURTO))
//	{
//		Aviso("Sua licen�a de uso n�o tem permiss�o para " + rbac->GetDescricaoOperacao(opCURTO));
//		ActionTopoExecute(NULL);
//		return;
//	}

	// determina o patamar selecionado
	if ((ind_pat = CBoxPatamar->ItemIndex) < 0)
		ind_pat = 0;
	// verifica se ButCurto est� ativo
	if (DLL_ExisteFormCurto(this))
	{ // apenas atualiza FormCurto
		DLL_AtualizaFormCurto(this);
		return;
	}
	// fecha janelas
	FechaJanelas();
	// exibe CBoxPatamar
	CBoxPatamar->Visible = true;
	// abaixa Button
	ButCurto->Down = true;
	// exibe tela do Curto
	DLL_NewFormCurto(this, apl, SelecionaParentParaJanela());
	// apenas atualiza FormCurto
	// DLL_AtualizaFormCurto(this, ind_pat);
	// exibe gr�fico de Curto DEPOIS de ter aberto tela de Curto (que cria RedeCC)
	grafico->Show(grafCURTO, ano, ind_pat);
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16131085
void __fastcall TFormChild::ActionEditorExecute(TObject *Sender)
{
	// vari�veis locais
	bool planejamento = false;
//	VTRBACManager *rbac = (VTRBACManager*)apl->GetObject(__classid(VTRBACManager));

//	// RBAC
//	if (!rbac->ExistePermissaoModulo(modEDITOR))
//	{
//		Aviso("Sua licen�a de uso n�o tem permiss�o para " + rbac->GetDescricaoOperacao(opBASICO));
//		return;
//	}
	// exibe tela do editor
	DLL_NewFormEditor(this, apl, SelecionaParentParaJanela(), planejamento);
}

// ---------------------------------------------------------------------------
void __fastcall TFormChild::ActionExibeAlertaExecute(TObject *Sender)
{
	// vari�veis locais
	VTAlerta *alerta = (VTAlerta*)apl->GetObject(__classid(VTAlerta));

	// prote��o
	if (alerta == NULL)
		return;
	// exibe janela com mensagens de alerta
	alerta->ShowModal();
	// atualiza n�mero de mensagens
	// ButExibeAlerta->Caption = "Alertas: " + IntToStr(alerta->NumeroDeMensagens());
	// define cor do PanelAlerta
	PanelAlerta->Visible = (alerta->NumeroDeMensagens() > 0);
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16131085
void __fastcall TFormChild::ActionExplorerExecute(TObject *Sender)
{
//	VTRBACManager *rbac = (VTRBACManager*)apl->GetObject(__classid(VTRBACManager));
//
//	// RBAC
//	if (!rbac->ExistePermissaoModulo(modEXPLORER))
//	{
//		Aviso("Sua licen�a de uso n�o tem permiss�o para " + rbac->GetDescricaoOperacao(opBASICO));
//		return;
//	}
	// exibe tela do Explore
	DLL_NewFormExplorer(this, apl, SelecionaParentParaJanela());
}

// ---------------------------------------------------------------------------
void __fastcall TFormChild::ActionExportaTxtExecute(TObject *Sender)
{
	// vari�veis locais
	TForm *form;

	// verifica limita��es da Licenca de uso
	if (!ValidaLimiteLicenca())
		return;
	// cria TFormExportaTxt p/ importar a Rede
	if ((form = DLL_NewFormExportaTXT(this, apl)) == NULL)
		return;
	// exibe TFormExportaTxt como janela modal
	form->ShowModal();
	// destr�i TFormExportaTxt
	delete form;
}

// ---------------------------------------------------------------------------
void __fastcall TFormChild::ActionExportaXmlExecute(TObject *Sender)
{
	// vari�veis locais
	TForm *form;

	// verifica limita��es da Licenca de uso
	if (!ValidaLimiteLicenca())
		return;
	// cria TFormExportaXML p/ importar a Rede
	if ((form = DLL_NewFormExportaXML(this, apl)) == NULL)
		return;
	// exibe TFormExportaXML como janela modal
	form->ShowModal();
	// destr�i TFormExportaXML
	delete form;
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16131085
void __fastcall TFormChild::ActionFasometroExecute(TObject *Sender)
{
//	VTRBACManager *rbac = (VTRBACManager*)apl->GetObject(__classid(VTRBACManager));
//
//	// RBAC
//	if (!rbac->ExistePermissaoModulo(modFASOMETRO))
//	{
//		Aviso("Sua licen�a de uso n�o tem permiss�o para " + rbac->GetDescricaoOperacao(opBASICO));
//		return;
//	}
	// exibe tela do Explore
	DLL_NewFormFasometro(this, apl, SelecionaParentParaJanela());
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16131085
//https://sinapsisenergia.teamwork.com/#/tasks/18682517
void __fastcall TFormChild::ActionFlowExecute(TObject *Sender)
{
	// vari�veis locais
	int num_pat;
//	VTRBACManager *rbac = (VTRBACManager*)apl->GetObject(__classid(VTRBACManager));

//	// RBAC
//	if (!rbac->ExistePermissaoModulo(modFLUXO))
//	{
//		Aviso("Sua licen�a de uso n�o tem permiss�o para " + rbac->GetDescricaoOperacao(opREDE));
//		return;
//	}
	// fecha janelas
	FechaJanelas();
    //sinaliza que est� ocupado
    ocupado = true;
	// executa c�lculo de fluxo
	if (ExecutaCalculoFluxo())
	{ // abaixa Button
		butFlow->Down = true;
		// inicia e exibe CBoxPatamar
		CBoxPatamarInicia();
		CBoxPatamar->Visible = true;
		// seleciona patamar de maior demanda
		SelecionaPatamarMaiorDemanda();
		// atualiza eventuais janelas abertas
		AtualizaJanelas();
	}
	else
	{
		ActionTopo->Execute();
	}
    //sinaliza que n�o est� ocupado
    ocupado = false;
}

// ---------------------------------------------------------------------------
void __fastcall TFormChild::ActionFlowCfgExecute(TObject *Sender)
{
	// vari�veis locais
	TForm *Form;
	VTFlow *flow = (VTFlow*)apl->GetObject(__classid(VTFlow));

	// cria TFormCfgFlow e exibe como janela modal
	Form = DLL_NewFormCfgFlow(this, apl);
	if (Form->ShowModal() == mrOk)
	{ // marca que o fluxo n�o est� atualizado
		flow->UpToDate = false;
		// atualiza Caption de ActionFlow
		AtualizaCaptionActionFlow();
		// verifica se est� exibindo gr�fico de fluxo
		if (butFlow->Down)
		{ // recalcula fluxo e atualiza o gr�fico
			ActionFlow->Execute();
		}
	}
	// destr�i Form
	delete Form;
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16131085
void __fastcall TFormChild::ActionGraficoExecute(TObject *Sender)
{
//	VTRBACManager *rbac = (VTRBACManager*)apl->GetObject(__classid(VTRBACManager));
//
//	// RBAC
//	if (!rbac->ExistePermissaoModulo(modGRAFICO))
//	{
//		Aviso("Sua licen�a de uso n�o tem permiss�o para " + rbac->GetDescricaoOperacao(opBASICO));
//		return;
//	}
	// exibe tela do editor
	DLL_NewFormPadrao(this, apl, SelecionaParentParaJanela());
}

// ---------------------------------------------------------------------------
void __fastcall TFormChild::ActionImportaAnafasExecute(TObject *Sender)
{
	// vari�veis locais
	TForm *form;
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	// cria TForm p/ importar a Rede
	if ((form = DLL_NewFormImportaAnafas(this, apl)) == NULL)
		return;
	// exibe form como janela modal
	if (form->ShowModal() == mrOk)
	{ // exibe gr�fico de topologia
		grafico->AreaRedeDefine();
		grafico->Show(grafTOPO, 0, 0);
	}
	else
	{ // fecha este form
		Close();
	}
	// destr�i TFormImporta
	delete form;
}

// ---------------------------------------------------------------------------
void __fastcall TFormChild::ActionImportaAnaredeExecute(TObject *Sender)
{
	// vari�veis locais
	TForm *form;
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	// cria TForm p/ importar a Rede
	if ((form = DLL_NewFormImportaAnarede(this, apl)) == NULL)
		return;
	// exibe form como janela modal
	if (form->ShowModal() == mrOk)
	{ // exibe gr�fico de topologia
		grafico->AreaRedeDefine();
		grafico->Show(grafTOPO, 0, 0);
	}
	else
	{ // fecha este form
		Close();
	}
	// destr�i TFormImporta
	delete form;
}

/*
 //---------------------------------------------------------------------------
 void __fastcall TFormChild::ActionImportaGisExecute(TObject *Sender)
 {
 //vari�veis locais
 TForm      *form;
 VTGrafico  *grafico  = (VTGrafico*)apl->GetObject(__classid(VTGrafico));


 //cria TForm p/ importar a Rede
 if ((form = DLL_NewFormImportaTXT(this, apl)) == NULL) return;
 //exibe form como janela modal
 if (form->ShowModal() == mrOk)
 {//exibe gr�fico de topologia
 grafico->AreaRedeDefine();
 grafico->Show(grafTOPO, 0, 0);
 }
 else
 {//fecha este form
 Close();
 }
 //destr�i TFormImporta
 delete form;
 }
 */
// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16131085
void __fastcall TFormChild::ActionImportaGisExecute(TObject *Sender)
{
	// vari�veis locais
	TForm *form;
//	VTRBACManager *rbac = (VTRBACManager*)apl->GetObject(__classid(VTRBACManager));
//
//	// RBAC
//	if (!rbac->ExistePermissaoModulo(modIMPORTAGIS))
//	{
//		Aviso("Sua licen�a de uso n�o tem permiss�o para " + rbac->GetDescricaoOperacao(opBASICO));
//		return;
//	}
	// cria TForm p/ importar a Rede
	if ((form = DLL_NewFormImportaTXT(this, apl)) == NULL)
		return;
	// exibe form como janela modal
	if (form->ShowModal() == mrOk)
	{ // exibe gr�fico de topologia
		Reinicia();
	}
	else
	{ // fecha este form
		Close();
	}
	// destr�i TFormImporta
	delete form;
}

// ---------------------------------------------------------------------------
void __fastcall TFormChild::ActionImportaBDGDExecute(TObject *Sender)
{
	// vari�veis locais
	VTImportaRedeGIS *ImportaBDGD;
//	VTRBACManager *rbac = (VTRBACManager*)apl->GetObject(__classid(VTRBACManager));
//
//	// RBAC
//	if (!rbac->ExistePermissaoModulo(modIMPORTAGIS))
//	{
//		Aviso("Sua licen�a de uso n�o tem permiss�o para " + rbac->GetDescricaoOperacao(opBASICO));
//		return;
//	}
	// cria TForm p/ importar a Rede
	if ((ImportaBDGD = DLL_NewObjImportaBDGD(this, apl)) != NULL)
	{
		ImportaBDGD->ShowModalFormImportaRedeGIS();
	}
}

/*
 //---------------------------------------------------------------------------
 void __fastcall TFormChild::ActionImportaTxtExecute(TObject *Sender)
 {
 //vari�veis locais
 TForm      *form;
 VTGrafico  *grafico  = (VTGrafico*)apl->GetObject(__classid(VTGrafico));


 //cria TForm p/ importar a Rede
 if ((form = DLL_NewFormImportaTXT(this, apl)) == NULL) return;
 //exibe form como janela modal
 if (form->ShowModal() == mrOk)
 {//exibe gr�fico de topologia
 grafico->AreaRedeDefine();
 grafico->Show(grafTOPO, 0, 0);
 }
 else
 {//fecha este form
 Close();
 }
 //destr�i TFormImporta
 delete form;
 }
 */
// ---------------------------------------------------------------------------
void __fastcall TFormChild::ActionImportaTxtExecute(TObject *Sender)
{
	// vari�veis locais
	TForm *form;

	// cria TForm p/ importar a Rede
	if ((form = DLL_NewFormImportaTXT(this, apl)) == NULL)
		return;
	// exibe form como janela modal
	if (form->ShowModal() == mrOk)
	{ // exibe gr�fico de topologia
		Reinicia();
	}
	else
	{ // fecha este form
		Close();
	}
	// destr�i TFormImporta
	delete form;
}

// ---------------------------------------------------------------------------
void __fastcall TFormChild::ActionImportaXmlExecute(TObject *Sender)
{
	// vari�veis locais
	TForm *form;
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	// cria TForm p/ importar a Rede
	if ((form = DLL_NewFormImportaXML(this, apl)) == NULL)
		return;
	// exibe form como janela modal
	if (form->ShowModal() == mrOk)
	{ // exibe gr�fico de topologia
		grafico->AreaRedeDefine();
		grafico->Show(grafTOPO, 0, 0);
	}
	else
	{ // fecha este form
		Close();
	}
	// destr�i TFormImporta
	delete form;
}

// ---------------------------------------------------------------------------
void __fastcall TFormChild::ActionInsRedeExecute(TObject *Sender)
{
	// vari�veis locais
	bool starting = false;

	Reinicia(starting);
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16131085
void __fastcall TFormChild::ActionLocalizaExecute(TObject *Sender)
{
//	VTRBACManager *rbac = (VTRBACManager*)apl->GetObject(__classid(VTRBACManager));
//
//	// RBAC
//	if (!rbac->ExistePermissaoModulo(modLOCALIZAR))
//	{
//		Aviso("Sua licen�a de uso n�o tem permiss�o para " + rbac->GetDescricaoOperacao(opBASICO));
//		return;
//	}
	// exibe tela do Localizador
	DLL_NewFormLocaliza(this, apl, SelecionaParentParaJanela());
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/16259599
//https://sinapsisenergia.teamwork.com/#/tasks/16131085
void __fastcall TFormChild::ActionManobraDoExecute(TObject *Sender)
{
	// vari�veis locais
	VTManobra *manobra;
	// VTGrafico  *grafico  = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	VTManobras *manobras = (VTManobras*)apl->GetObject(__classid(VTManobras));
//	VTRBACManager *rbac = (VTRBACManager*)apl->GetObject(__classid(VTRBACManager));
	AnsiString obj;

//	// RBAC
//	if (!rbac->ExistePermissaoModulo(modMANOBRAS))
//	{
//		Aviso("Sua licen�a de uso n�o tem permiss�o para " + rbac->GetDescricaoOperacao(opREDE));
//		return;
//	}
	// verifica Manobra selecionada em PopupMenuManobra
	for (int n = 0; n < PopupMenuManobra->Items->Count; n++)
	{
		if (PopupMenuManobra->Items->Items[n]->Checked)
		{ // determina Manobra
//			manobra = (VTManobra*)PopupMenuManobra->Items->Items[n]->Tag;
			obj = PopupMenuManobra->Items->Items[n]->Hint;

			try
			{//IMPORTANTE: o objeto foi guardado no Hint como string, pois em 64 bits,
			//o endere�o do ponteiro n�o cabe num inteiro
				#ifdef _WIN64
				__int64 obj_int = StrToInt64(obj);
				manobra = (VTManobra*)obj_int;
				#endif

				#ifdef __WIN32__
				int obj_int = StrToInt(obj);
				manobra = (VTManobra*)obj_int;
				#endif
			}
			catch (...)
			{
				manobra = NULL;
			}

			// executa Manobra
			if (manobra != NULL)
			{
				manobras->ExecutaManobra(manobra);
				// OBS: o objeto Manobras utiliza o objeto Edita que altera a Rede e sinaliza
				// a altera��o p/ este Form atrav�s de ActionRedeChanged
				// if (butTopo->Down)
				// {//atualiza o gr�fico
				// grafico->Redraw();
				// }
			}
			return;
		}
	}
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/16259599
//https://sinapsisenergia.teamwork.com/#/tasks/16131085
void __fastcall TFormChild::ActionManobraUndoExecute(TObject *Sender)
{
	// vari�veis locais
	VTManobra *manobra;
	// VTGrafico  *grafico  = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	VTManobras *manobras = (VTManobras*)apl->GetObject(__classid(VTManobras));
//	VTRBACManager *rbac = (VTRBACManager*)apl->GetObject(__classid(VTRBACManager));
	AnsiString obj;

//	// RBAC
//	if (!rbac->ExistePermissaoModulo(modMANOBRAS))
//	{
//		Aviso("Sua licen�a de uso n�o tem permiss�o para " + rbac->GetDescricaoOperacao(opREDE));
//		return;
//	}
	// verifica Manobra selecionada em PopupMenuManobra
	for (int n = 0; n < PopupMenuManobra->Items->Count; n++)
	{
		if (PopupMenuManobra->Items->Items[n]->Checked)
		{ // determina Manobra
//			manobra = (VTManobra*)PopupMenuManobra->Items->Items[n]->Tag;
			obj = PopupMenuManobra->Items->Items[n]->Hint;

			try
			{//IMPORTANTE: o objeto foi guardado no Hint como string, pois em 64 bits,
			//o endere�o do ponteiro n�o cabe num inteiro
				#ifdef _WIN64
				__int64 obj_int = StrToInt64(obj);
				manobra = (VTManobra*)obj_int;
				#endif

				#ifdef __WIN32__
				int obj_int = StrToInt(obj);
				manobra = (VTManobra*)obj_int;
				#endif
			}
			catch (...)
			{
				manobra = NULL;
			}

			// executa Manobra
			if (manobra != NULL)
			{
				manobras->ExecutaManobraComplementar(manobra);
				// OBS: o objeto Manobras utiliza o objeto Edita que altera a Rede e sinaliza
				// a altera��o p/ este Form atrav�s de ActionRedeChanged
				// if (butTopo->Down)
				// {//atualiza o gr�fico
				// grafico->Redraw();
				// }
			}
			return;
		}
	}
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16131085
void __fastcall TFormChild::ActionMedidorExecute(TObject *Sender)
{
//	VTRBACManager *rbac = (VTRBACManager*)apl->GetObject(__classid(VTRBACManager));
//
//	// RBAC
//	if (!rbac->ExistePermissaoModulo(modMEDIDOR))
//	{
//		Aviso("Sua licen�a de uso n�o tem permiss�o para " + rbac->GetDescricaoOperacao(opBASICO));
//		return;
//	}
	// exibe tela do Medidor
	DLL_NewFormMedidor(this, apl, SelecionaParentParaJanela());
}

// ---------------------------------------------------------------------------
void __fastcall TFormChild::ActionMercadoChangedExecute(TObject *Sender)
{
	// vari�veis locais
	VTFlow *flow = (VTFlow*)apl->GetObject(__classid(VTFlow));

	// atualiza caption ActionFlow
	AtualizaCaptionActionFlow();
	// atualiza indica��o de que o flow n�o est� atualizado
	flow->UpToDate = false;
	// verifica se est� com a tela de fluxo ativa
	if (butFlow->Down)
	{
		ActionFlow->Execute();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormChild::ActionModoGraficoExecute(TObject *Sender)
{
	// vari�veis locais
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	VTRedegraf *redegraf = (VTRedegraf*)apl->GetObject(__classid(VTRedegraf));

	// verifica o modo de visualiza��o selecionado
	if (MenuItemGraficoGeo->Checked)
	{ // fecha eventual FormEsquematico
		DLL_CloseFormEsquematico(this);
		// tenta redefinir modo de visualiza��o: geo-referenciado
		redegraf->ModoGrafico = modoGEOREFERENCIADO;
		if (redegraf->ModoGrafico != modoGEOREFERENCIADO)
		{ // torna a selecionar o modo esquem�tico
			MenuItemGraficoEsq->Checked = true;
			Aviso("A rede n�o possui coordenadas geo-referenciadas");
			return;
		}
	}
	else if (MenuItemGraficoEsq->Checked)
	{ // fecha eventual FormEsquematico
		DLL_CloseFormEsquematico(this);
		// modo de visualiza��o: esquem�tico geral
		redegraf->ModoGrafico = modoESQUEMATICO;
	}
	else if (MenuItemGraficoFun->Checked)
	{ // modo de visualiza��o: esquem�tico funcional
		redegraf->ModoGrafico = modoESQ_FUNCIONAL;
		// exibe FormEsquematico
		DLL_NewFormEsquematico(this, apl, SelecionaParentParaJanela());
	}
	else
	{
		return;
	}
	// atualiza o gr�fico
	grafico->ModoGraficoAlterado();
}

// ---------------------------------------------------------------------------
void __fastcall TFormChild::ActionRedeChangedExecute(TObject *Sender)
{
	// vari�veis locais
	VTFlow *flow = (VTFlow*)apl->GetObject(__classid(VTFlow));

	// marca que o fluxo n�o est� atualizado
	flow->UpToDate = false;
	// houve altera��o na Rede
	if (butFlow->Down)
	{ // recalcula fluxo de pot�ncia
		ActionFlowExecute(Sender);
	}
	// houve altera��o na Rede
	else if (ButCurto->Down)
	{ // recalcula curto-circuito
		ActionCurtoExecute(Sender);
	}
	else
	{ // gr�fico j� foi atualizado por VTEdita
		// grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
		// grafico->AreaRedeAlterada();
		// grafico->Redraw();
		// atualiza eventuais janelas abertas
		AtualizaJanelas();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormChild::ActionSalvaExecute(TObject *Sender)
{
	// Verifica se o m�dulo de perdas est� aberto
	if (!FechaFormPerda())
	{
		return;
	}
	// salva Rede ou Estudo na base de dados
	Salva();
}

/*
 //---------------------------------------------------------------------------
 void __fastcall TFormChild::ActionSalvaComoExecute(TObject *Sender)
 {
 //vari�veis locais
 bool           base_nova = true;
 AnsiString     subdir;
 TList          *lisREDE;
 VTEqpto        *eqpto;
 VTDir          *dir          = (VTDir*)apl->GetObject(__classid(VTDir));
 VTPath         *path         = (VTPath*)apl->GetObject(__classid(VTPath));
 VTRedes        *redes        = (VTRedes*)apl->GetObject(__classid(VTRedes));
 VTPlanejamento *planejamento = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));
 VTEstudo        *estudo       = planejamento->ExisteEstudo();

 //verifica limita��es da Licenca de uso
 if (! ValidaLimiteLicenca()) return;
 //verifica se est� fazendo Planejamento
 if (estudo->Tipo != estudoREDE_ORIGINAL)
 {//solicita novo nome para o Estudo
 if (! DefineNomeEstudo(estudo)) return;
 //salva o novo Estudo
 SalvaEstudoComo(estudo);
 return;
 }
 //cria uma nova base de dados vazia
 subdir = ExtractFileName(path->DirRedeBase());
 if (! dir->CriaRedeSinap(subdir)) return;
 //atualiza objeto Path p/ apontar p/ nova base de dados (VTDir j� aponta a nova base)
 path->DefineDirRede(subdir);
 //marca que todas as Redes s�o novas (informa��o importante p/ o Redutor)
 lisREDE = redes->LisRede();
 for (int n = 0; n < lisREDE->Count; n++)
 {
 eqpto = (VTEqpto*)lisREDE->Items[n];
 eqpto->Status[sttNOVO] = true;
 }
 //salva rede na base de dados
 SalvaRedeBase(base_nova);
 //atualiza Caption do Form
 AtualizaCaptionForm();
 }
 */
/*
 //---------------------------------------------------------------------------
 void __fastcall TFormChild::ActionSalvaComoExecute(TObject *Sender)
 {
 //vari�veis locais
 bool             edita_nome_estudo;
 VTPlanejamento *planejamento = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));
 VTEstudo        *estudo       = planejamento->ExisteEstudo();

 //verifica se a Licenca permite gravar base de dados
 if (! ActionSalvaComo->Enabled) return;
 //verifica limita��es da Licenca de uso
 if (! ValidaLimiteLicenca()) return;
 //verifica se � a Rede original ou um Estudo de planejamento
 if (estudo->Tipo == estudoREDE_ORIGINAL)
 {//salva rede original
 SalvaRedeComo();
 }
 //verifica se o usu�rio quer duplicar a base com o estudo ou apenas o estudo dentro da mesma base
 else
 {
 switch(Confirma("Para criar uma nova base com o estudo, selecione 'Sim/Yes'",
 "Para criar um novo estudo na mesma base, selecione 'N�o/No'", MB_YESNOCANCEL))
 {
 case IDYES: //salva a rede em uma nova base e salva o estudo
 //garante que a rede original est� selecionada (e n�o uma alternativa)
 if (estudo->AlternativaAtiva != estudo->AlternativaPadrao)
 {
 estudo->AlternativaAtiva = estudo->AlternativaPadrao;
 DLL_AtualizaFormEstudo(this);
 }
 aviso_base_salva = false;
 if (SalvaRedeComo())
 {//habilita aviso de base salva
 aviso_base_salva = true;
 SalvaEstudoComo(estudo, edita_nome_estudo=false);
 }
 //habilita aviso de base salva
 aviso_base_salva = true;
 break;
 case IDNO:  //salva um novo estudo na mesma base
 SalvaEstudoComo(estudo, edita_nome_estudo=true);
 break;
 case IDCANCEL:
 break;
 }
 }
 }
 */
// ---------------------------------------------------------------------------
void __fastcall TFormChild::ActionSalvaComoExecute(TObject *Sender)
{
	// vari�veis locais
	bool edita_nome_estudo;
	VTPlanejamento *planejamento = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));
	VTEstudo *estudo = planejamento->ExisteEstudo();

	// Verifica se o m�dulo de perdas est� aberto
	if (!FechaFormPerda())
	{
		return;
	}
	// verifica se a Licenca permite gravar base de dados
	if (!ActionSalvaComo->Enabled)
		return;
	// verifica limita��es da Licenca de uso
	if (!ValidaLimiteLicenca())
		return;
	// verifica se � a Rede original ou um Estudo de planejamento
	if (estudo->Tipo == estudoREDE_ORIGINAL)
	{ // salva rede original
		SalvaRedeComo();
	}
	// verifica se o usu�rio quer duplicar a base com o estudo ou apenas o estudo dentro da mesma base
	else
	{
		switch (Confirma("Para criar uma nova base com o estudo, selecione 'Sim/Yes'",
			"Para criar um novo estudo na mesma base, selecione 'N�o/No'", MB_YESNOCANCEL))
		{
		case IDYES: // salva a rede em uma nova base e salva o estudo
			// atualiza FormEstudo: inicio de grava��o do estudo na base
			DLL_AtualizaFormEstudo(this, true);
			aviso_base_salva = false;
			if (SalvaRedeComo())
			{ // habilita aviso de base salva
				aviso_base_salva = true;
				SalvaEstudoComo(estudo, edita_nome_estudo = false);
			}
			// atualiza FormEstudo: fim de grava��o do estudo na base
			DLL_AtualizaFormEstudo(this, false);
			// habilita aviso de base salva
			aviso_base_salva = true;
			break;
		case IDNO: // salva um novo estudo na mesma base
			SalvaEstudoComo(estudo, edita_nome_estudo = true);
			break;
		case IDCANCEL:
			break;
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormChild::ActionSalvarBaseConvertidaExecute(TObject *Sender)
{
	// vari�veis locais
	bool edita_nome_estudo, permite_nome_igual;
	VTPlanejamento *planejamento = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));
	VTEstudo *estudo = planejamento->ExisteEstudo();

	// verifica se a Licenca permite gravar base de dados
	if (!ActionSalvaComo->Enabled)
		return;
	// verifica limita��es da Licenca de uso
	if (!ValidaLimiteLicenca())
		return;
	// desabilita aviso
	aviso_base_salva = false;
	// verifica se � a Rede original ou um Estudo de planejamento
	if (estudo->Tipo == estudoREDE_ORIGINAL)
	{ // salva rede original
		SalvaRedeComo(permite_nome_igual = false);
	}
	// salva o estudo dentro da mesma base
	else
	{
		SalvaEstudoComo(estudo, edita_nome_estudo = false);
	}
	// reabilita aviso
	aviso_base_salva = true;
}

// ---------------------------------------------------------------------------
void __fastcall TFormChild::ActionSelRedeExecute(TObject *Sender)
{
	// vari�veis locais
	TForm *Form;
	VTEstudo *estudo;
	VTPlanejamento *planejamento = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));

	// obt�m objeto Estudo
	estudo = planejamento->ExisteEstudo();
	// verifica o tipo do Estudo aberto
	if (estudo->Tipo != estudoREDE_ORIGINAL)
	{
		Aviso("N�o � poss�vel inserir redes no Planejamento");
		return;
	}
	// exibe FormAbre p/ sele��o do estudo
	Form = DLL_NewFormAbre(this, apl);
	if (Form->ShowModal() != mrOk)
	{ // usu�rio cancelou sele��o de Redes: fecha este form
		// Close();
		return;
	}
	// reinicia este Form c/ a rede aberta
	Reinicia(true);
}

// ---------------------------------------------------------------------------
void __fastcall TFormChild::ActionStkExecute(TObject *Sender)
{
	// vari�veis locais
	VTSinapChild *sinap_child = (VTSinapChild*)apl->GetObject(__classid(VTSinapChild));

	// exibe/esconde STK
	if (ButStk->Down)
		sinap_child->FormStkShow();
	else
		sinap_child->FormStkHide();
}

/*
 //---------------------------------------------------------------------------
 void __fastcall TFormChild::ActionTesteRedutorExecute(TObject *Sender)
 {
 //vari�veis locais
 VTRedutor   *redutor;
 VTTopologia *topologia = (VTTopologia*)apl->GetObject(__classid(VTTopologia));

 //cria objeto VTRedutor
 if ((redutor =  DLL_NewObjRedutor(apl)) == NULL) return;
 //desabilita mensagens do redutor
 redutor->AvisoEnabled = false;
 //monta redes reduzidas
 redutor->ExecutaReducaoMNet();
 //destr�i objeto VTRedutor
 delete redutor;
 //reconfigura Redes
 topologia->Reconfigura();
 //trata altera��o de rede
 ActionRedeChangedExecute(NULL);
 }
 */
// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16131085
void __fastcall TFormChild::ActionTopoExecute(TObject *Sender)
{
	// vari�veis locais
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
//	VTRBACManager *rbac = (VTRBACManager*)apl->GetObject(__classid(VTRBACManager));

//	// RBAC
//	if (!rbac->ExistePermissaoModulo(modTOPOLOGIA))
//	{
//		Aviso("Sua licen�a de uso n�o tem permiss�o para " + rbac->GetDescricaoOperacao(opBASICO));
//		return;
//	}
	// fecha janelas
	FechaJanelas();
	// esconde CBoxPatamar
	CBoxPatamar->Visible = false;
	// abaixa Button e acerta seu Caption
	butTopo->Down = true;
	butTopo->Caption = ActionTopo->Caption;
	// exibe gr�fico de Topologia
	grafico->Show(grafTOPO, 0, 0);
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16131085
void __fastcall TFormChild::ActionTopoArranjoExecute(TObject *Sender)
{
	// vari�veis locais
	TFormViewArranjo *FormView;
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
//	VTRBACManager *rbac = (VTRBACManager*)apl->GetObject(__classid(VTRBACManager));

//	// RBAC
//	if (!rbac->ExistePermissaoModulo(modTOPOLOGIA))
//	{
//		Aviso("Sua licen�a de uso n�o tem permiss�o para " + rbac->GetDescricaoOperacao(opBASICO));
//		return;
//	}
	// fecha janelas
	FechaJanelas();
	// esconde CBoxPatamar
	CBoxPatamar->Visible = false;
	// abaixa Button e acerta seu Caption
	butTopo->Down = true;
	butTopo->Caption = ActionTopo->Caption + "[A]";
	// exibe FormViewArranjo p/ usu�rio configurar cores
	FormView = new TFormViewArranjo(this, apl, panelGrafico);
	FormView->ExibeArranjos();
	FormView->Show();
	// exibe gr�fico de Topologia
	grafico->Show(grafARRANJO, 0, 0);
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16131085
void __fastcall TFormChild::ActionTopoBrancoPretoExecute(TObject *Sender)
{
	// vari�veis locais
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
//	VTRBACManager *rbac = (VTRBACManager*)apl->GetObject(__classid(VTRBACManager));
//
//	// RBAC
//	if (!rbac->ExistePermissaoModulo(modTOPOLOGIA))
//	{
//		Aviso("Sua licen�a de uso n�o tem permiss�o para " + rbac->GetDescricaoOperacao(opBASICO));
//		return;
//	}
	// fecha janelas
	FechaJanelas();
	// esconde CBoxPatamar
	CBoxPatamar->Visible = false;
	// abaixa Button e acerta seu Caption
	butTopo->Down = true;
	butTopo->Caption = ActionTopo->Caption + "[BP]"; ;
	// exibe gr�fico de Topologia
	grafico->Show(grafTOPO_BP, 0, 0);
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16131085
void __fastcall TFormChild::ActionTopoFasesExecute(TObject *Sender)
{
	// vari�veis locais
	TFormViewFase *FormView;
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
//	VTRBACManager *rbac = (VTRBACManager*)apl->GetObject(__classid(VTRBACManager));
//
//	// RBAC
//	if (!rbac->ExistePermissaoModulo(modTOPOLOGIA))
//	{
//		Aviso("Sua licen�a de uso n�o tem permiss�o para " + rbac->GetDescricaoOperacao(opBASICO));
//		return;
//	}
	// fecha janelas
	FechaJanelas();
	// esconde CBoxPatamar
	CBoxPatamar->Visible = false;
	// abaixa Button e acerta seu Caption
	butTopo->Down = true;
	butTopo->Caption = ActionTopo->Caption + "[F]";
	// exibe FormViewFase p/ usu�rio configurar cores
	FormView = new TFormViewFase(this, apl, panelGrafico);
	FormView->ExibeFases();
	FormView->Show();
	// exibe gr�fico de Topologia
	grafico->Show(grafFASE, 0, 0);
}

// ---------------------------------------------------------------------------
VTApl* __fastcall TFormChild::AplChild(void)
{
	return (apl);
}

// ---------------------------------------------------------------------------
void __fastcall TFormChild::AtualizaCaptionActionFlow(void)
{
	// vari�veis locais
	VTMercado *mercado = (VTMercado*)apl->GetObject(__classid(VTMercado));

	ActionFlow->Caption = "Fluxo Pot�ncia " + IntToStr(mercado->AnoAtual);
}

// ---------------------------------------------------------------------------
void __fastcall TFormChild::AtualizaCaptionForm(void)
{
	// vari�veis locais
	AnsiString txt;
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// define parte b�sica do Caption
	txt.sprintf("Rede %d: Base ", Tag);
	switch (geral->BaseDado)
	{
	case SNPDBTYPE_ORACLE: // nada a fazer
		txt = txt + "Oracle";
		break;
	case SNPDBTYPE_ACCESS:
		txt = txt + "Access " + AnsiQuotedStr(ExtractFileName(path->ArqRede), '\'');
		break;
	default:
		txt = txt + "Indefinida";
	}
	// inclui data da cria��o da rede
	txt = txt + " de " + DateToStr(redes->DataReferencia);
	// atualiza Caption
	Caption = txt;
}

// ---------------------------------------------------------------------------
void __fastcall TFormChild::AtualizaDataReferencia(void)
{
	// vari�veis locais
	VTCurvas *curvas = (VTCurvas*)apl->GetObject(__classid(VTCurvas));
	VTDemanda *demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// define m�s de refer�ncia para o objeto Curvas
	curvas->DataReferencia = redes->DataReferencia;
	demanda->DataReferencia = redes->DataReferencia;
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/14569628
//https://sinapsisenergia.teamwork.com/#tasks/16235317
void __fastcall TFormChild::AtualizaJanelas(void)
{
	// vari�veis locais
	int ano = 0;
	int ind_pat;
	VTPatamar *patamar;
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	VTAlerta *alerta = (VTAlerta*)apl->GetObject(__classid(VTAlerta));

	// determina o patamar selecionado
	if ((ind_pat = CBoxPatamar->ItemIndex) < 0)
		ind_pat = 0;
	else
	{
		patamar = (VTPatamar*)CBoxPatamar->Items->Objects[ind_pat];
		ind_pat = patamar->Index;
	}
	// atualiza eventuais janelas abertas
	DLL_AtualizaFormCurto(this);
	DLL_AtualizaFormExplorer(this);
	DLL_AtualizaFormLocaliza(this);
	DLL_AtualizaFormMedidor(this);
	DLL_AtualizaFormFasometro(this);
	DLL_Especial_Atualiza(this);
	DLL_MapaTematico_Atualiza(this);
	DLL_AtualizaTelas(this);
	// atualiza gr�fico de Flow
	// if (butFlow->Down) grafico->Show(grafFLOW, ano, ind_pat);
	if ((butFlow->Down) || (ButAberturaMonopolar->Down))
		grafico->Show(grafFLOW, ano, ind_pat);
	//reexibe os alertas
	if(alerta)
	{
        alerta->ReexibeAlertas();
    }
	// DVK 2016.12.14 atualiza gr�fico
//	grafico->Refresh();
	grafico->Redraw();
}

// ---------------------------------------------------------------------------
void __fastcall TFormChild::CBoxPatamarChange(TObject *Sender)
{
	// atualiza janelas de resultados de flow
	AtualizaJanelas();
}

// ---------------------------------------------------------------------
void __fastcall TFormChild::CBoxPatamarInicia(void)
{
	// vari�veis locais
	VTPatamar *patamar;
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
	TList *lisPAT = patamares->LisPatamar();

	// reinicia CBoxPatamar c/ Patamares
	CBoxPatamar->Clear();
	for (int np = 0; np < lisPAT->Count; np++)
	{
		patamar = (VTPatamar*)lisPAT->Items[np];
		if (patamar->Enabled)
		{
			CBoxPatamar->Items->AddObject(patamar->Nome, patamar);
		}
	}
	// pr�-seleciona primeiro patamar
	if (CBoxPatamar->Items->Count > 0)
		CBoxPatamar->ItemIndex = 0;
}

// ---------------------------------------------------------------------
void __fastcall TFormChild::CloseForm(AnsiString class_name)
{
	// vari�veis locais
	AnsiString form_name;
	TForm *Form;

	// verifica se existe um Form da classe aberto
	for (int n = Screen->FormCount - 1; n >= 0; n--)
	{ // verifica se o Form n � um TFormEditor
		form_name = Screen->Forms[n]->ClassName();
		if (class_name.AnsiCompareIC(form_name) == 0)
		{ // verifica se o Form foi criado por este Form
			if (Screen->Forms[n]->Owner == this)
			{
				delete Screen->Forms[n];
				return;
			}
		}
	}
}

// ---------------------------------------------------------------------
void __fastcall TFormChild::ConfiguraArranjos(VTApl *apl_ext)
{
	// vari�veis locais
	VTFlow *flow = (VTFlow*)apl->GetObject(__classid(VTFlow));
	VTSinapChild *sinap_child = (VTSinapChild*)apl->GetObject(__classid(VTSinapChild));

	// executa atualiza��o necess�ria p/ acomodar mudan�a de patamares
	sinap_child->DefineArranjos(apl_ext);
	// verifica se est� exibindo tela de fluxo ou curto
	if (butFlow->Down)
	{ // recalcula fluxo
		flow->UpToDate = false;
		ActionFlow->Execute();
	}
	else if (ButCurto->Down)
	{ // recalcula curto
		DLL_AtualizaFormCurto(this);
	}
}

// ---------------------------------------------------------------------
void __fastcall TFormChild::ConfiguraCurvasTipicas(VTApl *apl_ext)
{
	// vari�veis locais
	int dia_medicao;
	VTCurvas *curvas = (VTCurvas*)apl->GetObject(__classid(VTCurvas));
	VTFlow *flow = (VTFlow*)apl->GetObject(__classid(VTFlow));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTSinapChild *sinap_child = (VTSinapChild*)apl->GetObject(__classid(VTSinapChild));

	// salva dia da medi��o atualmente selecionado
	dia_medicao = curvas->DiaMedicao;
	// l� as curvas t�picas da base de dados
	sinap_child->LeBaseCurvaTipica();
	// redefine dia da medi��o selecionado
	curvas->DiaMedicao = dia_medicao;
	// reconfigura todas as cargas, associando-as com as curvas t�picas selecionadas
	redes->RedefineCurvaTipicaCarga(dia_medicao);
	// marca que o fluxo n�o est� atualizado
	flow->UpToDate = false;
	// se necess�rio, executa fluxo
	if (butFlow->Down)
	{ // recalcula fluxo e atualiza o gr�fico
		ActionFlow->OnExecute(NULL);
	}
}

// ---------------------------------------------------------------------
void __fastcall TFormChild::ConfiguraGeral(VTApl *apl_ext)
{
	// vari�veis locais
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
	VTGeral *geral_ext = (VTGeral*)apl_ext->GetObject(__classid(VTGeral));
	VTFlow *flow = (VTFlow*)apl->GetObject(__classid(VTFlow));

	// verifica a necessidade de atualizar mudar o fluxo
	if (geral->PadroesRedesDiferentes(*geral_ext))
	{ // marca que o fluxo n�o est� atualizado
		flow->UpToDate = false;
	}
	// atualiza objeto geral pr�prio a partir do objeto geral recebido
	geral->CopiaAtributosDe(*geral_ext);
	// verifica se est� exibindo tela de fluxo
	if (butFlow->Down)
	{ // recalcula fluxo
		ActionFlow->Execute();
	}
}

// ---------------------------------------------------------------------
void __fastcall TFormChild::ConfiguraGrafico(VTApl *apl_ext)
{ // DVK 2015.10.01
	// vari�veis locais
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	VTSinapChild *sinap_child = (VTSinapChild*)apl->GetObject(__classid(VTSinapChild));

	// executa atualiza��o necess�ria p/ mudan�a da cor das chaves
	sinap_child->DefineCorTipoChave(apl_ext);
	// atualiza gr�fico
	if (grafico)
	{
		grafico->Redraw();
	}
}

// ---------------------------------------------------------------------
void __fastcall TFormChild::ConfiguraPatamares(VTApl *apl_ext)
{
	// vari�veis locais
	VTSinapChild *sinap_child = (VTSinapChild*)apl->GetObject(__classid(VTSinapChild));

	// executa atualiza��o necess�ria p/ acomodar mudan�a de patamares
	sinap_child->DefinePatamares(apl_ext);
	// reinicia CBoxPatamar
	CBoxPatamarInicia();
	// verifica se est� exibindo tela de fluxo
	if (butFlow->Down)
	{ // recalcula fluxo
		ActionFlow->Execute();
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TFormChild::DefineNomeEstudo(VTEstudo *estudo)
{
	// vari�veis locais
	bool sucesso = false;
	TFormEdtTxt *FormEdtTxt;

	try
	{ // cria TFormEdtTxt p/ definir nome do Estudo
		FormEdtTxt = new TFormEdtTxt(this, "Estudo: salvar como...");
		FormEdtTxt->Label->Caption = "Novo Estudo:";
		FormEdtTxt->Texto = estudo->Codigo + "_1";
		if (FormEdtTxt->ShowModal() == mrOk)
		{
			estudo->Codigo = FormEdtTxt->Texto;
			sucesso = true;
		}
		// detr�i Form
		delete FormEdtTxt;
	}
	catch (Exception &e)
	{
	}
	return (sucesso);
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16131085
void __fastcall TFormChild::DesabilitaModulos(void)
{
	// vari�veis locais
//	 VTRBACManager *rbac = (VTRBACManager*)apl->GetObject(__classid(VTRBACManager));
	 VTSinapChild *sinap_child = (VTSinapChild*)apl->GetObject(__classid(VTSinapChild));

//	 //verifica se � SINAPviewer
//	 if(! rbac->SinapViewer())
//		  return;

	 //esconde STK
    sinap_child->FormStkHide();
    //desabilita m�dulos
    ActionFlow->Enabled = false;
    ActionCurto->Enabled = false;
    ActionAberturaMonopolar->Enabled = false;
    ActionContinuidade->Enabled = false;
    ActionManobra->Enabled = false;
    ActionMedidor->Enabled = false;
    ActionFasometro->Enabled = false;
    ActionStk->Enabled = false;
}

// ---------------------------------------------------------------------------
bool __fastcall TFormChild::ExecutaCalculoFluxo(void)
{
	// vari�veis locais
	bool sucesso;
	VTFlow *flow = (VTFlow*)apl->GetObject(__classid(VTFlow));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	try
	{ // verifica se deve executar fluxo por rede
		if (flow->PorRedeEnabled)
		{ // executa c�lculo de fluxo individual das redes
			sucesso = flow->CalculaPorRede(redes);
		}
		else
		{ // executa c�lculo de fluxo �nico das redes
			sucesso = flow->CalculaRedeUnica(redes);
		}
	}
	catch (Exception &e)
	{
		sucesso = false;
	}
	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TFormChild::ExisteAlteracaoNaRede(void)
{
	// vari�veis locais
	VTEqpto *eqpto;
	VTArranjos *arranjos = (VTArranjos*)apl->GetObject(__classid(VTArranjos));
	VTCurvas *curvas = (VTCurvas*)apl->GetObject(__classid(VTCurvas));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));
	VTFiguras *figuras = (VTFiguras*)apl->GetObject(__classid(VTFiguras));
	TList *lisEQP = edita->Obra->Lixeira->LisEqpto();

	// verifica se existe algum Eqpto novo ou alterado na rede
	if (redes->ExisteAlteracao())
		return (true);
	// verifica se h� algum Eqpto pr�-existente na base que foi retirado
	for (int n = 0; n < lisEQP->Count; n++)
	{
		eqpto = (VTEqpto*)lisEQP->Items[n];
		if (eqpto->Id > 0)
			return (true);
	}
	// verifica se h� suportes, cabos ou arranjos novos ou alterados
	if (arranjos->ExisteAlteracao())
		return (true);
	// verifica se h� curvas t�picas novas ou alterdas
	if (curvas->ExisteAlteracao())
		return (true);
	// verifica se h� figuras novas ou alteradas
	if(figuras != NULL)
	{
		if(figuras->ExisteAlteracao())
		return (true);
	}
	return (false);
}

// ---------------------------------------------------------------------------
bool __fastcall TFormChild::ExisteRedeEqvBT(void)
{
	// vari�veis locais
	VTRede *rede;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	TList *lisREDE = redes->LisRede();

	for (int n = 0; n < lisREDE->Count; n++)
	{
		rede = (VTRede*)lisREDE->Items[n];
		// if (rede->Carregada) continue;  //DVK 2016.11.28 comentada, pois qualquer
		// rede secund�ria ser� trocada por carga
		if (rede->TipoRede->Segmento == redeSEC)
			return (true);
	}
	return (false);
}

// ---------------------------------------------------------------------------
bool __fastcall TFormChild::FechaFormPerda(void)
{
	// vari�veis locais
	AnsiString form_name, perdatec, perdareg;
	int resp;
	TForm *Form;

	perdatec = "TFormPerdatec";
	perdareg = "TFormPerdareg";

	// verifica se existe um Form da classe aberto
	for (int n = Screen->FormCount - 1; n >= 0; n--)
	{ // verifica se o Form n � um TFormPerdatec/TFormPerdareg
		Form = Screen->Forms[n];
		form_name = Screen->Forms[n]->ClassName();
		if ((perdatec.AnsiCompareIC(form_name) == 0) || (perdareg.AnsiCompareIC(form_name) == 0))
		{
			resp = Confirma("Deseja fechar o m�dulo de Perdas?", "", MB_YESNOCANCEL);

			switch (resp)
			{
			case IDYES:
				// CloseForm(form_name)
				delete Form;
				return true;
			case IDNO:
			case IDCANCEL:
			default:
				return false;
			}
		}
	}
	return true;
}

// ---------------------------------------------------------------------------
void __fastcall TFormChild::FechaJanelas(void)
{
	// destr�i eventual janela aberta
	CloseForm("TFormViewArranjo");
	CloseForm("TFormViewFase");
	// CloseForm("TFormCalcIndCont");
	DLL_CloseFormCurto(this);
	// DLL_CloseFormFluxoH3(this);
	DLL_CloseFormMonopolar(this);
	// DLL_CloseFormConfiabilidade(this);

}

// ---------------------------------------------------------------------------
void __fastcall TFormChild::FormActivate(TObject *Sender)
{
	// exibe Panel2 no segundo Monitor
	if (FormMultiMonitor)
		FormMultiMonitor->ExibePanel2(MultiPanel);
}

// ---------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18682517
void __fastcall TFormChild::FormClose(TObject *Sender, TCloseAction &Action)
{
    //verifica se est� ocupado
    if(ocupado)
    {
        return;
    }
	// desabilita tratamento do evento OnActivate
	// OnActivate = NULL;
	// esconde FrameMultiMonitor, se ele existir
	// if (FormMultiMonitor) FormMultiMonitor->EscondePanel2(MultiPanel);
	// executa Action externa
	ActionOnClose->OnExecute(NULL);
	// destr�i o form
	Action = caFree;
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18682517
void __fastcall TFormChild::FormCloseQuery(TObject *Sender, bool &CanClose)
{
	// vari�veis locais
	bool confirma = true;

    //verifica se est� ocupado
    if(ocupado)
    {
        CanClose = false;
        return;
    }
    // Verifica se o m�dulo de perdas est� aberto
	if (!FechaFormPerda())
	{
        CanClose = false;
		return;
	}
	// salva Rede ou Estudo na base de dados
	CanClose = (Salva(confirma) != IDCANCEL);
}

// ---------------------------------------------------------------------------
void __fastcall TFormChild::IniciaMultiPanel(void)
{
	// obt�m objeto FormMultiMonitor
	FormMultiMonitor = (TFormMultiMonitor*)apl->GetObject("TFormMultiMonitor");
	// se houver FormMultiMonitor, cria FrameMultiMonitor em FormMultiMonitor
	if (FormMultiMonitor)
	{ // cria objeto MultiPanel
		if ((MultiPanel = FormMultiMonitor->InsereMultiPanel(panelGrafico)) != NULL)
		{ // insere MultiPanel em Apl
			apl->Add(MultiPanel);
			// exibe Panel2 no segundo monitor
			FormMultiMonitor->ExibePanel2(MultiPanel);
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormChild::IniciaObjSinapChild(void)
{
	// vari�veis locais
	VTSinapChild *sinap_child;

	// cria objeto SinapChild
	if ((sinap_child = NewObjSinapChild(apl)) != NULL)
	{ // inicia SinapChild
		sinap_child->ActionCurto = ActionCurto;
		sinap_child->ActionEditor = ActionEditor;
		sinap_child->ActionFlow = ActionFlow;
		sinap_child->ActionTopo = ActionTopo;
		sinap_child->ActionRedeChanged = ActionRedeChanged;
		sinap_child->ActionSalva = ActionSalva;
		sinap_child->ActionSalvaComo = ActionSalvaComo;
		sinap_child->ActionSalvarBaseConvertida = ActionSalvarBaseConvertida;
		sinap_child->ActionMercadoChanged = ActionMercadoChanged;
		sinap_child->PanelToolBarGrafico = panelToolBarGrafico;
		sinap_child->ButFlow = butFlow;
        sinap_child->InsereBotaoEditor(butEditor);
		// insere SinapChild em Apl
		apl->Add(sinap_child);
	}
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/14569628
void __fastcall TFormChild::InsereMsgAlerta(AnsiString txt, VTRede *rede)
{
	// vari�veis locais
	VTAlerta *alerta = (VTAlerta*)apl->GetObject(__classid(VTAlerta));

	// prote��o
	if (alerta == NULL)
		return;
	// insere mensagem em Alerta
	if (rede == NULL)
		{
		alerta->Add(txt);
		}
	else
		{
		alerta->AddMsgMalha(rede, txt);
        }
	// atualiza n�mero de mensagens
	ButExibeAlerta->Caption = "Alertas: " + IntToStr(alerta->NumeroDeMensagens());
	// atualiza StatusBar: �ltima mensagem
	LabelAlerta->Caption = txt;
	// define cor do PanelAlerta
	PanelAlerta->Visible = true;
}

// ---------------------------------------------------------------------------
void __fastcall TFormChild::ModuloEspecial_Inicia(void)
{
	// incia m�dulos especiais
	DLL_Especial_Start(this, apl);
	DLL_MapaTematico_Start(this, apl);
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/16259599
void __fastcall TFormChild::PopupMenuManobraPopup(TObject *Sender)
{
	// vari�veis locais
	TMenuItem *MenuItem, *MenuSubItem;
	VTManobra *manobra;
	VTManobras *manobras = (VTManobras*)apl->GetObject(__classid(VTManobras));
	TList *lisMANOBRA = manobras->LisManobra();

	// inicia PopupMenuManobra
	PopupMenuManobra->Items->Clear();
	// loop p/ todas Manobras
	for (int n = 0; n < lisMANOBRA->Count; n++)
	{
		manobra = (VTManobra*)lisMANOBRA->Items[n];
		// cria MenuItem p/ Manobra
		MenuItem = new TMenuItem(PopupMenuManobra);
		// insere MenuItem em PopupMenuManobra
		PopupMenuManobra->Items->Add(MenuItem);
		MenuItem->Action = NULL;
		MenuItem->Caption = manobra->Codigo;
		MenuItem->GroupIndex = 0;
		MenuItem->RadioItem = true;
		MenuItem->AutoCheck = true;
		MenuItem->Checked = false;
		// associa MenuItem c/ a Manobra atrav�s do atributo Tag
//		MenuItem->Tag = (int)manobra;

        try
			{//IMPORTANTE: o objeto foi guardado no Hint como string, pois em 64 bits,
			//o endere�o do ponteiro n�o cabe num inteiro
				#ifdef _WIN64
				__int64 obj = __int64(manobra);
				MenuItem->Hint = IntToStr(obj);
				#endif

				#ifdef __WIN32__
				int obj = int(manobra);
				MenuItem->Hint = IntToStr(obj);
				#endif
			}
			catch(...)
			{
				MenuItem->Hint = "";
			}

		//
		// cria MenuItem p/ executar Manobra
		MenuSubItem = new TMenuItem(MenuItem);
		MenuItem->Add(MenuSubItem);
		MenuSubItem->Action = ActionManobraDo;
		MenuSubItem->Caption = "Executar manobra";
		// MenuSubItem->GroupIndex = 0;
		// MenuSubItem->RadioItem  = true;
		MenuSubItem->AutoCheck = true;
		MenuSubItem->Checked = false;
		// associa MenuItem c/ a Manobra atrav�s do atributo Tag
		MenuSubItem->Tag = (int)manobra;

		try
			{//IMPORTANTE: o objeto foi guardado no Hint como string, pois em 64 bits,
			//o endere�o do ponteiro n�o cabe num inteiro
				#ifdef _WIN64
				__int64 obj = __int64(manobra);
				MenuSubItem->Hint = IntToStr(obj);
				#endif

				#ifdef __WIN32__
				int obj = int(manobra);
				MenuSubItem->Hint = IntToStr(obj);
				#endif
			}
			catch(...)
			{
				MenuSubItem->Hint = "";
			}

		//
		// cria MenuItem p/ executar Manobra complementar
		MenuSubItem = new TMenuItem(MenuItem);
		MenuItem->Add(MenuSubItem);
		MenuSubItem->Action = ActionManobraUndo;
		MenuSubItem->Caption = "Executar manobra complementar";
		// MenuSubItem->GroupIndex = 0;
		// MenuSubItem->RadioItem  = true;
		MenuSubItem->AutoCheck = true;
		MenuSubItem->Checked = false;
		// associa MenuItem c/ a Manobra atrav�s do atributo Tag
//		MenuSubItem->Tag = (int)manobra;

		try
			{//IMPORTANTE: o objeto foi guardado no Hint como string, pois em 64 bits,
			//o endere�o do ponteiro n�o cabe num inteiro
				#ifdef _WIN64
				__int64 obj = __int64(manobra);
				MenuSubItem->Hint = IntToStr(obj);
				#endif

				#ifdef __WIN32__
				int obj = int(manobra);
				MenuSubItem->Hint = IntToStr(obj);
				#endif
			}
			catch(...)
			{
				MenuSubItem->Hint = "";
			}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormChild::PopupModoGraficoPopup(TObject *Sender)
{
	// vari�veis locais
	VTRedegraf *redegraf = (VTRedegraf*)apl->GetObject(__classid(VTRedegraf));

	// determina modo gr�fico atual
	if (redegraf->ModoGrafico == modoGEOREFERENCIADO)
		MenuItemGraficoGeo->Checked = true;
	else if (redegraf->ModoGrafico == modoESQUEMATICO)
		MenuItemGraficoEsq->Checked = true;
	else
		MenuItemGraficoFun->Checked = true;
}

// ---------------------------------------------------------------------------
void __fastcall TFormChild::Reinicia(bool starting)
{
	// vari�veis locais
	AnsiString txt;
	VTDir *dir = (VTDir*)apl->GetObject(__classid(VTDir));
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
	VTEstilos *estilos = (VTEstilos*)apl->GetObject(__classid(VTEstilos));
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));
	VTFlow *flow = (VTFlow*)apl->GetObject(__classid(VTFlow));
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	VTMercado *mercado = (VTMercado*)apl->GetObject(__classid(VTMercado));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTSinapChild *sinap_child = (VTSinapChild*)apl->GetObject(__classid(VTSinapChild));

	// atualiza Caption do Form e do ActionFlow
	AtualizaCaptionForm();
	AtualizaCaptionActionFlow();
	// atualiza Dir com o arquivo da rede aberta
	dir->ArqRede = path->ArqRede;
	// verifica se foi definida Action externa para ajustar rede carregada
	if (sinap_child->ActionRedeCarregada != NULL)
	{ // executa Action externa
		sinap_child->ActionRedeCarregada->Execute();
	}
	// reinicia configura��o de VTGeral que depende de dados lidos da base
	// geral->CfgPerdaTec->IniciaConfiguracaoGis();
	geral->LeGeral(false);
	//l� tamb�m as infos basicas dos estilos
	if (estilos != NULL)
	{
        estilos->LeEstilosBase();
	}
	// reconfigura Redes
	// topologia->Reconfigura();
	// atualiza objetos Curvas e Demanda com a data de refer�ncia da rede
	AtualizaDataReferencia();
	// associa Redes e Cargas c/ Mercado
	mercado->IniciaRedes(redes);
	if (mercado->AnoInicial < YearOf(Now()))
	{
		Aviso(txt.sprintf("O Mercado est� configurado para o per�odo de %d a %d",
			mercado->AnoInicial, mercado->AnoFinal));
	}
	// marca que o fluxo n�o est� atualizado
	flow->UpToDate = false;
	// bloqueia atualiza��o do gr�fico
	grafico->Disable();
	if (starting)
	{ // habilita o gr�fico, ainda sem definir o tipo de gr�fico
		grafico->AreaRedeDefine();
	}
	else
	{ // substitui��o de uma Reducao por sua Rede real
		grafico->AreaRedeAltera();
	}
	// verifica se deve recalcular flow
	if (butFlow->Down)
	{ // recalcula fluxo de pot�ncia
		ActionFlowExecute(NULL);
	}
	else
	{ // exibe gr�fico de topologia
		ActionTopoExecute(NULL);
	}
}

// ---------------------------------------------------------------------------
int __fastcall TFormChild::Salva(bool confirma)
{
	// vari�veis locais
	VTPlanejamento *planejamento = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));
	VTEstudo *estudo = planejamento->ExisteEstudo();

	// verifica se a Licenca permite gravar base de dados
	if (!ActionSalva->Enabled)
		return (IDNO);
	// verifica se � a Rede original ou um Estudo de planejamento
	if (estudo->Tipo == estudoREDE_ORIGINAL)
	{ // salva rede original
		return (SalvaRede(confirma));
	}
	else
	{ // salva Estudo
		return (SalvaEstudo(confirma));
	}
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/17577418
void __fastcall TFormChild::SalvaAnoEstudo(VTEstudo *estudo, bool confirma)
{
    // vari�veis locais
    VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
    int ano_ini, ano_fim;

    //prote��o
    if(estudo == NULL)
        return;

    try
    {//determina valores do ano
        ano_ini = estudo->AnoInicial;
        ano_fim = estudo->AnoFinal;
        if (geral)
        { // salva par�metros do arquivo
            geral->Infoset->AddInfo("AnoIni",ano_ini);
            geral->Infoset->AddInfo("AnoFim",ano_fim);
            geral->Infoset->SaveToFile("PeriodoAnalise");
            if((! estudo->Alterado) && confirma)
            {
                Aviso("Per�odo de an�lise salvo com sucesso");
            }
        }
    }
    catch(...)
    {
        //nada
    }
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/17577418
int __fastcall TFormChild::SalvaEstudo(bool confirma)
{
	// vari�veis locais
	int resp = IDNO;
	VTPlanejamento *planejamento = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));
	VTEstudo *estudo = planejamento->ExisteEstudo();

    //verifica se o usuario mandou salvar (confirma=false)
    if(! confirma)
    {//salva ano do estudo no infoset (mostra aviso)
        SalvaAnoEstudo(estudo, true);
    }

	// verifica se h� necessidade de gravar o Estudo na base de dados
	if (estudo->Novo || estudo->Alterado || ExisteAlteracaoNaRede())
	{ // verifica limita��es da Licenca de uso
		if (!ValidaLimiteLicenca())
			return (IDNO);
		// verifica se deve pedir confirma��o do usu�rio
		if (confirma)
			resp = Confirma("Deseja salvar o planejamento na base de dados ?", "", MB_YESNOCANCEL);
		else
			resp = IDYES;
	}
	switch (resp)
	{
	case IDYES: // muda apresenta��o p/ topologia da Rede
		// ActionTopo->Execute();
        //salva ano e n�o mostra aviso
        SalvaAnoEstudo(estudo, false);
		// atualiza base de dados
		if (SalvaEstudo(estudo))
		{ // atualiza FormEstudo
			// DLL_AtualizaFormEstudo(this);
			return (IDYES);
		}
		else
		{ // atualiza FormEstudo
			// DLL_AtualizaFormEstudo(this);
			return (IDCANCEL);
		}
	case IDNO: // n�o salva a rede
		return (IDNO);
	case IDCANCEL: // n�o salva a rede
	default:
		return (IDCANCEL);
	}
}

/*
 //---------------------------------------------------------------------------
 bool __fastcall TFormChild::SalvaEstudo(VTEstudo *estudo)
 {
 //vari�veis locais
 bool    sucesso;
 VTSalva *salva;

 try{//garante que a rede original est� selecionada (e n�o uma alternativa)
 if (estudo->AlternativaAtiva != estudo->AlternativaPadrao)
 {
 estudo->AlternativaAtiva = estudo->AlternativaPadrao;
 //DVK 2014.05.08 - evita janela de erro no Win8
 DLL_CloseFormEditor(NULL);
 DLL_AtualizaFormEstudo(this);
 }
 //cria objeto VTSalva
 if ((salva = DLL_NewObjSalva(this, apl)) == NULL) return(false);
 //salva Estudo
 if ((sucesso = salva->SalvaEstudo(estudo)) == true)
 {
 Aviso("A base de dados foi atualizada corretamente");
 }
 else
 {
 Erro("A base de dados n�o foi atualizada.");
 }
 //destr�i objeto
 delete salva;
 }catch(Exception &e)
 {
 sucesso = false;
 }
 return(sucesso);
 }
 */
// ---------------------------------------------------------------------------
bool __fastcall TFormChild::SalvaEstudo(VTEstudo *estudo)
{
	// vari�veis locais
	bool sucesso;
	VTSalva *salva;

	try
	{ // cria objeto VTSalva
		if ((salva = DLL_NewObjSalva(this, apl)) == NULL)
			return (false);
		// atualiza FormEstudo: inicio de grava��o do estudo na base
		DLL_AtualizaFormEstudo(this, true);
		// salva Estudo
		if ((sucesso = salva->SalvaEstudo(estudo)) == true)
		{
			Aviso("A base de dados foi atualizada corretamente");
		}
		else
		{
			Erro("A base de dados n�o foi atualizada.");
		}
		// atualiza FormEstudo: fim de grava��o do estudo na base
		DLL_AtualizaFormEstudo(this, false);
		// destr�i objeto
		delete salva;
	}
	catch (Exception &e)
	{ // atualiza FormEstudo: fim de grava��o do estudo na base
		DLL_AtualizaFormEstudo(this, false);
		sucesso = false;
	}
	return (sucesso);
}

/*
 //---------------------------------------------------------------------------
 bool __fastcall TFormChild::SalvaEstudoComo(VTEstudo *estudo, bool edita_nome)
 {
 //vari�veis locais
 bool    sucesso;
 VTSalva *salva;

 try{//verifica se deve editar o nome do estudo
 if (edita_nome)
 {//solicita novo nome para o Estudo
 if (! DefineNomeEstudo(estudo)) return(false);
 }
 //garante que a rede original est� selecionada (e n�o uma alternativa)
 if (estudo->AlternativaAtiva != estudo->AlternativaPadrao)
 {
 estudo->AlternativaAtiva = estudo->AlternativaPadrao;
 DLL_AtualizaFormEstudo(this);
 }
 //cria objeto VTSalva
 if ((salva = DLL_NewObjSalva(this, apl)) == NULL) return(false);
 //salva Estudo
 if ((sucesso = salva->SalvaEstudoComo(estudo)) == true)
 {
 Aviso("A base de dados foi salva corretamente");
 }
 else
 {
 Erro("A base de dados n�o foi salva.");
 }
 //destr�i objeto
 delete salva;
 }catch(Exception &e)
 {
 sucesso = false;
 }
 return(sucesso);
 }
 */
// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/17577418
bool __fastcall TFormChild::SalvaEstudoComo(VTEstudo *estudo, bool edita_nome)
{
	// vari�veis locais
	bool sucesso;
	VTSalva *salva;

    //salva ano do estudo no infoset
    SalvaAnoEstudo(estudo, false);

	try
	{ // verifica se deve editar o nome do estudo
		if (edita_nome)
		{ // solicita novo nome para o Estudo
			if (!DefineNomeEstudo(estudo))
				return (false);
		}
		// cria objeto VTSalva
		if ((salva = DLL_NewObjSalva(this, apl)) == NULL)
			return (false);
		// atualiza FormEstudo: inicio de grava��o do estudo na base
		DLL_AtualizaFormEstudo(this, true);
		// salva Estudo
		if ((sucesso = salva->SalvaEstudoComo(estudo)) == true)
		{
			if (aviso_base_salva)
				Aviso("A base de dados foi salva corretamente");
		}
		else
		{
			if (aviso_base_salva)
				Erro("A base de dados n�o foi salva.");
		}
		// atualiza FormEstudo: fim de grava��o do estudo na base
		DLL_AtualizaFormEstudo(this, false);
		// destr�i objeto
		delete salva;
	}
	catch (Exception &e)
	{ // atualiza FormEstudo: fim de grava��o do estudo na base
		DLL_AtualizaFormEstudo(this, false);
		sucesso = false;
	}
	return (sucesso);
}

// ---------------------------------------------------------------------------
int __fastcall TFormChild::SalvaRede(bool confirma)
{
	// vari�veis locais
	bool base_nova = false;
	int resp = IDYES;
	VTPath *path;
	AnsiString filename, filepath;
	AnsiString nomeRede = "";

	path = (VTPath*)Apl->GetObject(__classid(VTPath));
	filepath = path->ArqRede;
	nomeRede = "(Rede " + IntToStr(this->Tag) + ") ";
	filename = ExtractFileName(filepath);

	// verifica se h� necessidade de gravar a Rede
	if (!ExisteAlteracaoNaRede())
		return (IDNO);
	// verifica limita��es da Licenca de uso
	if (!ValidaLimiteLicenca())
		return (IDNO);
	// verifica se deve pedir confirma��o do usu�rio
	if (confirma)
		resp = Confirma("Deseja atualizar a " + nomeRede, "Salvando na base de dados: " + filename +
		" ?", MB_YESNOCANCEL);
	// verifica se o usu�rio deseja atualizar a base de dados
	switch (resp)
	{
	case IDYES: // atualiza base de dados
		resp = (SalvaRedeBase(base_nova)) ? IDYES : IDCANCEL;
		// return(resp);
		break;
	case IDNO: // n�o salva a rede
		// return(IDNO);
		break;
	case IDCANCEL: // n�o fecha o form
	default:
		// return(IDCANCEL);
		break;
	}
	return (resp);
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18599859
bool __fastcall TFormChild::SalvaRedeBase(bool base_nova)
{
	// vari�veis locais
	bool sucesso = false;
    bool apaga = true;
	VTRedutor *redutor;
	VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));
	VTTopologia *topologia = (VTTopologia*)apl->GetObject(__classid(VTTopologia));

	// muda apresenta��o p/ topologia da Rede
	ActionTopo->Execute();
    // verifica se h� Eqptos isolados
	switch (ValidaEqptoIsolado())
    {
        case IDYES:// salva blocos ou barras isolados
            apaga = false;
            edita->RetiraEqptosIsolados(apaga);
            break;
        case IDNO: // apaga blocos ou barras isolados
            apaga = true;
            edita->RetiraEqptosIsolados(apaga);
            break;
        case IDCANCEL:
        default:
            return (false);
    }

	// cria objeto VTRedutor
	if ((redutor = DLL_NewObjRedutor(apl)) == NULL)
		return (false);
	// desabilita mensagens do redutor
	redutor->AvisoEnabled = false;
	// monta redes reduzidas
	if (redutor->ExecutaReducaoMNet())
	{ // atualiza base de dados
		if (base_nova)
			sucesso = SalvaRedeBaseNova();
		else
			sucesso = SalvaRedeBaseExistente();
	}
	else
	{ // avisa usu�rio e pede confirma��o p/ atualizar base de dados
		if (Confirma("N�o foi poss�vel criar as redes equivalentes",
			"A base de dados deve ser atualizada ?") == IDYES)
		{ // atualiza base de dados
			if (base_nova)
				sucesso = SalvaRedeBaseNova();
			else
				sucesso = SalvaRedeBaseExistente();
		}
	}
	// destr�i objeto VTRedutor
	delete redutor;
	// reconfigura Redes
	// topologia->Reconfigura();
	topologia->ClassificaRedes();
	// trata altera��o de rede
	ActionRedeChangedExecute(NULL);

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TFormChild::SalvaRedeBaseExistente(void)
{
	// vari�veis locais
	bool sucesso;
	VTSalva *salva;
	VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));

	try
	{ // cria objeto VTSalva
		if ((salva = DLL_NewObjSalva(this, apl)) == NULL)
			return (false);
		// salva Rede existente na base de dados
		sucesso = salva->SalvaRede();
		if (sucesso)
		{ // cancela Undo/Redo
			edita->Clear();
			if (aviso_base_salva)
				Aviso("A base de dados foi atualizada corretamente");
		}
		else
		{
			Erro("A base de dados n�o foi atualizada.");
		}
		// destr�i objeto
		delete salva;
	}
	catch (Exception &e)
	{
		sucesso = false;
	}
	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TFormChild::SalvaRedeBaseNova(void)
{
	// vari�veis locais
	bool sucesso;
	VTSalva *salva;
	VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));

	try
	{ // cria objeto VTSalva
		if ((salva = DLL_NewObjSalva(this, apl)) == NULL)
			return (false);
		// salva Rede existente na base de dados
		sucesso = salva->SalvaRedeComo();
		if (sucesso)
		{ // cancela Undo/Redo
			edita->Clear();
			if (aviso_base_salva)
				Aviso("A base de dados foi atualizada corretamente");
		}
		else
		{
			Erro("A base de dados n�o foi atualizada.");
		}
		// destr�i objeto
		delete salva;
	}
	catch (Exception &e)
	{
		sucesso = false;
	}
	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TFormChild::SalvaRedeComo(bool permite_nome_igual)
{
	// vari�veis locais
	bool sucesso;
	bool base_nova = true;
	AnsiString arq_rede;
	TList *lisREDE;
	VTEqpto *eqpto;
	VTDir *dir = (VTDir*)apl->GetObject(__classid(VTDir));
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	// VTPlanejamento *planejamento = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));
	// VTEstudo        *estudo       = planejamento->ExisteEstudo();

	// cria uma nova base de dados vazia
	arq_rede = path->ArqRede;
	if (!dir->CriaCopiaRedeSinap(arq_rede, permite_nome_igual))
		return (false);
	// atualiza objeto Path p/ apontar p/ nova base de dados (VTDir j� aponta a nova base)
	path->ArqRede = arq_rede;
	// marca que todas as Redes s�o novas (informa��o importante p/ o Redutor)
	lisREDE = redes->LisRede();
	for (int n = 0; n < lisREDE->Count; n++)
	{
		eqpto = (VTEqpto*)lisREDE->Items[n];
		eqpto->Status[sttNOVO] = true;
	}
	// salva rede na base de dados
	sucesso = SalvaRedeBase(base_nova);
	// atualiza Caption do Form
	AtualizaCaptionForm();

	return (sucesso);
}

// ---------------------------------------------------------------------------
TWinControl* __fastcall TFormChild::SelecionaParentParaJanela(void)
{
	// verifica se existe FrameMultiMonitor
	if (MultiPanel)
		return (MultiPanel->PanelAtivo);
	// micro c/ apenas 1 monitor
	return (panelGrafico);
}

// ---------------------------------------------------------------------------
void __fastcall TFormChild::SelecionaPatamarMaiorDemanda(void)
{
	// vari�veis locais
	VTPatamar *patamar;
	VTConsulta *consulta;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	try
	{ // determina Patamar com maior fluxo de pot�ncia
		consulta = DLL_NewObjConsulta(apl);
		if ((patamar = consulta->PatamarFluxoMaximo(redes)) != NULL)
		{
			CBoxPatamar->ItemIndex = CBoxPatamar->Items->IndexOfObject(patamar);
		}
		delete consulta;
	}
	catch (Exception &e)
	{ // nada a fazer
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormChild::SinapToolKitInicia(int stk_option)
{
//	// vari�veis locais
//	VTLicencas *licencas = (VTLicencas*)apl->GetObject(__classid(VTLicencas));
//
//	// verifica se � uma Licenca tipo demo
//	if (licencas->LicencaTipoDemo())
//		return;
	// inicia SinapToolKit
	DLL_FormChildStart(this, apl, stk_option);
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/16580785
bool __fastcall TFormChild::TrocaRedeEqvBTporCargaEqv(VTApl* apl)
	// hk201612: acr�scimo de fun��o JCG
{
	// vari�veis locais
	VTStkEqv *stk_eqv;
	VTProgresso *progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
	int buttons = 1;

	try
	{ // verifica se h� redes BT equivalentes
		if (ExisteRedeEqvBT())
		{
			// Aviso("As redes BT (equivalentes ou n�o) ser�o substitu�das \n"
			// "por cargas equivalentes");
			if (Confirma("As redes de BT ser�o equivalentadas.", "Deseja continuar?",
				buttons) != idYes)
			{
				return false;
			}
			/* if (Confirma("As redes BT (equivalentes ou n�o) ser�o substitu�das por cargas equivalentes",
			 "Confirma esta opera��o ?") == idNo) {
			 Close();
			 return;
			 } */
			if (progresso == NULL)
			{ // cria progresso e insere no apl //DVK 2016.11.28
				progresso = DLL_NewObjProgresso(this->Owner, Parent);
				apl->Add(progresso);
			}
			// inicia progresso
			if (progresso)
				progresso->Start(progDEFAULT);
			// cria objeto VTStkEqv
			if ((stk_eqv = DLL_NewObjStkEqv(apl)) != NULL)
			{//op��o de A4 na BT falsa (ou seja, as cargas A4 ser�o mantidas)
				stk_eqv->Executa(false);
				// destr�i objeto VTStkEqv
				delete stk_eqv;
			}
			// finaliza progresso
			if (progresso)
				progresso->Stop();
		}
	}
	catch (Exception &e)
	{
		if (progresso)
			progresso->Stop();
		Aviso("N�o foi poss�vel substituir as redes BT equivalentes por cargas");
		return false;
	}

	return true;
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18599859
int __fastcall TFormChild::ValidaEqptoIsolado(void)
{
    // vari�veis locais
    int resp = IDYES;
    VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
    VTRede *rede = redes->RedeIsolada();
    TList *lisBARRA = rede->LisBarra();
    VTBarra *barra;
    int count = 0;

    // verifica se h� Barras isoladas sem eqbar
    for (int n = 0; n < lisBARRA->Count; n++)
    {
        barra = (VTBarra*)lisBARRA->Items[n];
        if((barra->LisEqbar()->Count > 0) && (barra->LisLigacao()->Count == 0))
        {
            count++;
            break;
        }
    }
    // verifica se h� Liga��es isoladas
    if((count > 0) || (rede->LisLigacao()->Count > 0))
    {// pede confirma��o do usu�rio
        resp = Confirma("Existem equipamentos isolados na rede em estudo",
            "Deseja salvar na base de dados os equipamentos isolados ?");
    }
    return (resp);
}
/*
 //---------------------------------------------------------------------------
 bool __fastcall TFormChild::ValidaExecucaoConfiabilidade(void)
 {
 //vari�veis locais
 VTRedes *redes    = (VTRedes*)apl->GetObject(__classid(VTRedes));
 TList   *lisBARRA = redes->LisBarra();

 //verifica n�mero de Barras
 if (lisBARRA->Count > DLL_Confiabilidade_LimiteBarras())
 {
 Aviso("O m�dulo de Confiabilidade n�o pode ser executado.\n"
 "A rede selecionada excede o limite de 15.000 barras.");
 return(false);
 }
 //verifica se h� redes
 return(true);
 }
 */
// ---------------------------------------------------------------------------
bool __fastcall TFormChild::ValidaLimiteLicenca(void)
{
	// vari�veis locais
	VTLicencas *licencas = (VTLicencas*)apl->GetObject(__classid(VTLicencas));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// verifica se � uma Licenca demo
	if (licencas->LicencaTipoDemo())
	{ // verifica n�mero de Barras e Ligacoes de cada Rede
		if (redes->ViolaLimiteLicencaDemo())
		{
			Aviso("O n�mero de barras excede o limite permitido pela Licen�a de Uso.\n"
				"A rede n�o ser� gravada na base de dados.");
			return (false);
		}
	}
	return (true);
}

// ---------------------------------------------------------------------------
void __fastcall TFormChild::VerificaModulos(void)
{
	// verifica se usu�rio master
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	VTLicencas *licencas = (VTLicencas*)apl->GetObject(__classid(VTLicencas));
	VTLicenca *licenca = licencas->ExisteLicenca();
	VTSinapChild *sinap_child = (VTSinapChild*)apl->GetObject(__classid(VTSinapChild));

	// inicia FormStk
	ButStk->Down = true;
	ActionStk->Execute();
	// insere Comandos em FormStk
	sinap_child->FormStkAdd("SINAP: M�dulos B�sicos", ActionListBase, false);
	// inicia m�dulos especiais
	ModuloEspecial_Inicia();
	// habilita/desabilita m�dulos
	// ActionConfiabilidade->Enabled = DLL_Confiabilidade_Enabled();
	// exibe/esconde m�dulos
	// ActionConfiabilidade->Visible  = (licenca->Tipo != tipoELPA);
	// verifica restri��es impostas pela Licenca
//	if (licenca == NULL)
//		return;
//	if (licenca->Tipo == tipoDIAGNOSTICO)
//	{ // bloqueia ToolBar
//		for (int n = 1; n < ToolBar->ButtonCount; n++)
//		{
//			ToolBar->Buttons[n]->Enabled = false;
//		}
//		// bloqueia Actions que permite atualiza base de dados
//		ActionSalva->Enabled = false;
//	}
//	// habilita/desabilita acesso ao GoogleMaps
//	grafico->MapaEnabled = licenca->Google;
}

// ---------------------------------------------------------------------------
// eof
