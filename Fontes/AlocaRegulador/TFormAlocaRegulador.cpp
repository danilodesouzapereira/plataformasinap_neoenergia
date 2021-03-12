// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Math.hpp>
#include <math.h>
#include <algorithm>
#include "TFormAlocaRegulador.h"
#include "VTAlocaRT.h"
#include "TEstoque.h"
#include "TLine.h"
#include "..\Apl\VTApl.h"
#include "..\Complexo\Complexo.h"
#include "..\Curvas\VTCurva.h"
#include "..\Diretorio\VTPath.h"
#include "..\Edita\VTEdita.h"
#include "..\Editor\TFormEdtReguladorMT.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Obra\VTObra.h"
#include "..\Obra\VTObras.h"                    //DVK 2013.07.23
#include "..\Patamar\VTPatamar.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Planejamento\VTAlternativa.h"      //DVK 2013.07.23
#include "..\Planejamento\VTEstudo.h"           //DVK 2013.07.23
#include "..\Planejamento\VTPlanejamento.h"     //DVK 2013.07.23
#include "..\Radial\VTPrimario.h"
#include "..\Radial\VTRadial.h"
#include "..\Radial\VTTronco.h"
#include "..\Rede\VTBarra.h"
// #include "..\Rede\VTChave.h"                    //DVK 2013.07.23
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTRegulador.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTResflowBar.h"
#include "..\Rede\VTResflowLig.h"
#include "..\Rede\VTTipos.h"                   //DVK 2013.07.23
#include "..\Rede\VTTipoChave.h"               //DVK 2013.07.23
#include "..\Rede\VTTrafo.h"
#include "..\Report\VTReport.h"
#include "..\Sinap\VTSinapChild.h"
#include "..\..\DLL_Inc\Edita.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Help.h"
#include "..\..\DLL_Inc\Medidor.h"
#include "..\..\DLL_Inc\Radial.h"
#include "..\..\DLL_Inc\Rede.h"
#include "..\..\DLL_Inc\Report.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TFormEdt"
#pragma resource "*.dfm"
using namespace std;

// ---------------------------------------------------------------------------
__fastcall TFormAlocaRegulador::TFormAlocaRegulador(TComponent *Owner, VTApl *apl_owner,
	TWinControl *parent) : TFormEdt(Owner, parent)
{
	// variáveis locais
	VTEdita *edita = (VTEdita*)apl_owner->GetObject(__classid(VTEdita));
	VTRedes *redes = (VTRedes*)apl_owner->GetObject(__classid(VTRedes));
	TList *lisREDE;

	// salva ponteiro p/ Apl
	this->apl = apl_owner;
	if (!VerificaPlanejamento())
		Close();
	// salva Obra originalmente associado a Edita
	obra_original = edita->Obra;
	// cria objeto Obra e associa com objeto Edita
	obra = DLL_NewObjObra(apl);
	obra->Obras = NULL;
	edita->Obra = obra;
	//copia redes da obra original para obra local
	lisREDE = obra_original->LisRede();
	if(lisREDE->Count == 0)
		lisREDE = redes->LisRede();
	obra->IniciaLisRede(lisREDE);
	// cria objeto
	alocaRT = NewObjAlocaRT(apl);
	// cria listas
	lisTabReg = new TList();
	lisEQP = new TList();
	lisREG_ALOCADO = new TList();
	lisREG_ORIG_TODOS = new TList();
	lisREG_ORIG_ATIVO = new TList();
	// cria equipamentos
	CriaEqptosFicticios();
	CriaModelosReg(); // DVK 2014.04.04
	// determina Reguladores originais da rede
	IniciaListaReguladoresOriginais();
	// preenche ListViewReg
	ListViewRegInicia();
	// preenche a lista de reguladores no grid
	IniciaGrid();
	PageControl->ActivePage = TabConfiguracao;
	TabLisAlocada->TabVisible = false;
	// inicia parametros  //DVK 2014.04.04
	IniciaParametros();
	// insere Form em seu Parent
	if (parent)
	{
		Parent = parent;
	}
	// posiciona o Form
	FormIniciaPosicao();
}

// ---------------------------------------------------------------------------
__fastcall TFormAlocaRegulador::~TFormAlocaRegulador(void)
{
	// variáveis locais
	VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

	// apaga moldura
	Moldura();
	// salva últimos dados   //DVK 2014.04.04
	SalvaDados();
	// salva parâmetros do Form
	if (geral)
		geral->Infoset->SaveToFile(this);
	// restaura Obra original ao objeto Edita
	edita->Obra = obra_original;
	// destrói objetos
	if (alocaRT)
	{
		delete alocaRT;
		alocaRT = NULL;
	}
	// destroi a lista e os obejtos
	if (lisTabReg)
	{
		LimpaTList(lisTabReg);
		delete lisTabReg;
		lisTabReg = NULL;
	}
	// destroi listas sem destruir seus objetos criados
	if (lisEQP)
	{
		delete lisEQP;
		lisEQP = NULL;
	}
	if (lisREG_ALOCADO)
	{
		delete lisREG_ALOCADO;
		lisREG_ALOCADO = NULL;
	}
	if (lisREG_ORIG_TODOS)
	{
		delete lisREG_ORIG_TODOS;
		lisREG_ORIG_TODOS = NULL;
	}
	if (lisREG_ORIG_ATIVO)
	{
		delete lisREG_ORIG_ATIVO;
		lisREG_ORIG_ATIVO = NULL;
	}
	EliminaEqptosFicticios();
	// redesenha rede
	// graf->Redraw();
}

// ---------------------------------------------------------------------------
void __fastcall TFormAlocaRegulador::ActionAddExecute(TObject *Sender)
{
	// cria mais um regulador
	CriaUmRegulador();
	// apresenta na tela
	IniciaGrid();
}

// ---------------------------------------------------------------------------
void __fastcall TFormAlocaRegulador::ActionAlocaRTExecute(TObject *Sender)
{
	// variáveis locais
	VTRegulador *reg;
	VTGrafico *graf = (VTGrafico*) apl->GetObject(__classid(VTGrafico));
	VTSinapChild *sinap_child = (VTSinapChild*) apl->GetObject(__classid(VTSinapChild));

	// valida parametros
	if (!LeParametros())
	{
		// Aviso("Parâmetros inválidos !");
		return;
	}
	// IMPORTANTE: apaga Moldura para que não fique com eventual Eqpto que será removido da Rede
	Moldura();
	// fecha janela de Medidor que pode estar associada a Eqpto Eqpto que será removido da Rede
	DLL_CloseFormMedidor(this);
	// exibe gráfico de topologia p/ evitar cálculo de fluxo durante a alocação
	sinap_child->ActionTopo->Execute();
	// desabilita os reguladores existentes
	ConfiguraReguladoresOriginais();
	// remove os reguladores alocados anteriormente
	alocaRT->RetiraBancosAlocadosDaRede();
	// executa alocação de reguladores
	if (alocaRT->Executa(lisTabReg))
	{ // obtém lista de Reguladores alocados
		lisREG_ALOCADO->Clear();
		if (alocaRT->ListaReguladorAlocado(lisREG_ALOCADO) == 0)
		{
			Aviso("Essa rede não precisa alocar mais reguladores.");
		}
	}
	// executa cálculo de fluxo
	sinap_child->ActionFlow->Execute();
	// mostra a lista de reguladores alocados
	ExibeListaAlocada(lisREG_ALOCADO);
	// altera tipo de regulador: auto ou fixo //DVK 2014.04.04
	// AlteraRegAlocado();
	// atualiza grafico
	if (graf)
		graf->AreaRedeAltera();
}

// ---------------------------------------------------------------------------
void __fastcall TFormAlocaRegulador::ActionCancelarExecute(TObject *Sender)
{
	// não incorpora os capacitores à rede
	CancelaAlocacao();
	// fecha o Form
	Close();
}

// ---------------------------------------------------------------------------
void __fastcall TFormAlocaRegulador::ActionConfirmarExecute(TObject *Sender)
{
	// incorpora os capacitores à rede
	ConfirmaAlocacao();
	// fecha o form
	Close();
}

// ---------------------------------------------------------------------------
void __fastcall TFormAlocaRegulador::ActionDeleteExecute(TObject *Sender)
{
	// variáveis locais
	TEstoque *estoque;
	int linha = gridRT->Row;

	// proteção:
	if (linha < 1)
		return;
	// if(lisTabReg->Count == 0) return;
	if (lisTabReg->Count < 2)
		return;
	if (linha > lisTabReg->Count)
		return;
	// remove regulador indicado no estoque //DVK 2015.07.21
	estoque = (TEstoque*)lisTabReg->Items[linha - 1];
	lisTabReg->Delete(linha - 1);
	delete estoque;
	// apresenta na tela
	IniciaGrid();
}

// ---------------------------------------------------------------------------
void __fastcall TFormAlocaRegulador::ActionEditarExecute(TObject *Sender)
{
	// objetos externos
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	// var local
	TFormEdtReguladorMT *form;
	TEstoque *est;
	VTRegulador *reg;
	int index = DeterminaReguladorSelecionado();

	// proteção
	if (index == 0)
		return;

	//atualiza estoque antes de abrir para edição
	if(!AlteraRegEstoque())
		return;
	// DVK 2015.07.21
	est = (TEstoque*)lisTabReg->Items[index - 1];
	reg = est->Regulador;
	// insere rede na lista
	redes->InsereRede(rede);

	try
	{ // cria form p/ edição
		form = new TFormEdtReguladorMT(this, apl, reg);
		form->OpcaoRetira(false);
		// desablita seleção de modelo, de rede e de barra de entrada
		form->CBoxRede->Enabled = false;
		//desabilita tipo de operação e configuração
		form->ComboBoxTipoAjuste->Enabled = false;
		form->CBoxConfiguracao->Enabled = false;
		// esconde de PanelFluxo
		form->PanelFluxo->Visible = false;
		// esconde opções de ajuste
		form->GBoxTipoAjuste->Visible = true;//false;
		form->gboxTapFixo->Visible = false;
		//esconde limites de tensão direto e inverso
		form->PanelLimitesDir->Visible = false;
		form->PanelLimitesInv->Visible = false;
		//esconde niveis de tensao direto e inverso
		form->PanelNVdir->Visible = false;
		form->PanelNVinv->Visible = false;
		//redimensiona insensibilidade
		form->gboxBMdir->Height = 45;
		form->gboxBMinv->Height = 45;
		// apresenta form modal
		form->ShowModal();
		IniciaGrid();
	}
	catch (Exception &e)
	{
		if (form)
		{
			delete form;
			form = NULL;
		}
		redes->LisRede()->Remove(rede);
	}
	// remove rede da lista
	redes->LisRede()->Remove(rede);
	if (form)
	{
		delete form;
		form = NULL;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormAlocaRegulador::ActionExportaExecute(TObject *Sender)
{
	// verifica qual tabela exportar
	if (PageControl->ActivePage == TabReguOriginal)
		ExportaExistentes();
	else if (PageControl->ActivePage == TabLisAlocada)
		ExportaAlocados();
	else
		Aviso("Seleciona a aba com reguladores existentes ou alocados para exportar a lista");
}

// ---------------------------------------------------------------------------
void __fastcall TFormAlocaRegulador::ActionHelpExecute(TObject *Sender)
{
	// variaveis locais
	TForm *formHelp;

	formHelp = DLL_NewFormHelp(this, apl, "Alocação_de_Regulador");
	if (formHelp)
		formHelp->Show();
}

// ---------------------------------------------------------------------------
void __fastcall TFormAlocaRegulador::ActionMedidorExecute(TObject *Sender)
{
	// variáveis locais
	VTRegulador *regulador;

	// verifica se há um Regulador selecionado em ListBoxReg
	if (ListBoxReg->ItemIndex < 0)
	{
		Aviso("Selecione o regulador para exibir seus dados");
		return;
	}
	regulador = (VTRegulador*)(ListBoxReg->Items->Objects[ListBoxReg->ItemIndex]);
	// cria um novo TFormMedidor
	DLL_SelEqpto(this, apl, Parent, regulador);
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/18261839
void __fastcall TFormAlocaRegulador::ActionRemoveRTExecute(TObject *Sender)
{
	// variaveis locais
	TList *lisReg = new TList();

	for (int i = ListBoxReg->Items->Count - 1; i >= 0; i--)
	{ // DVK 2014.02.11
		if (!ListBoxReg->Selected[i])
			continue;
		lisReg->Add(ListBoxReg->Items->Objects[i]);
		ListBoxReg->Items->Delete(i);
	}
	// remove Reguladores alocados
	// alocaRT->RetiraBancosAlocadosDaRede();
	alocaRT->RetiraBancosAlocadosDaRede(lisReg, true); // DVK 2014.02.11
	// remove da lista local
	lisREG_ALOCADO->Clear();
	alocaRT->ListaReguladorAlocado(lisREG_ALOCADO);
	// limpa ListBox e Chart
	// ListBoxReg->Items->Clear();
	for (int n = chart->SeriesCount() - 1; n >= 0; n--)
		chart->Series[n]->Clear();
	Moldura();
	// destroi lista
	if (lisReg)
		delete lisReg;
}

// ---------------------------------------------------------------------------
void __fastcall TFormAlocaRegulador::ActionSelAllExecute(TObject *Sender)
{
	for (int i = 0; i < ListBoxReg->Items->Count; i++)
	{
		ListBoxReg->Selected[i] = true;
	}
	// simula evento de click para atualizar Chart
	ListBoxRegClick(NULL);
}

// ---------------------------------------------------------------------------
void __fastcall TFormAlocaRegulador::ActionSelNoneExecute(TObject *Sender)
{
	for (int i = 0; i < ListBoxReg->Items->Count; i++)
	{
		ListBoxReg->Selected[i] = false;
	}
	// simula evento de click para atualizar Chart
	ListBoxRegClick(NULL);
}

// ---------------------------------------------------------------------------
void __fastcall TFormAlocaRegulador::ActionViewTroncoExecute(TObject *Sender)
{
	// variáveis locais
	TList *lisPri;
	VTPrimario *pri;
	VTRedes *redes = (VTRedes*) apl->GetObject(__classid(VTRedes));
	VTGrafico *graf = (VTGrafico*) apl->GetObject(__classid(VTGrafico));

	// cria objetos necessários
	VTTronco* tronco = DLL_NewObjTronco(apl);
	VTRadial* radial = DLL_NewObjRadial(apl);

	// executa a montagem da rede em radiais
	if (radial->Inicia(redes))
	{
//		lisPri->Clear();
		lisPri = radial->LisPrimario();
		for (int n = 0; n < lisPri->Count; n++)
		{
			pri = (VTPrimario*) lisPri->Items[n];
			tronco->DefinePrimario(pri, 18);
			graf->Moldura(tronco->LisLigacaoTronco());
		}
	}
	if (radial)
	{
		delete radial;
		radial = NULL;
	}
	if (tronco)
	{
		delete tronco;
		tronco = NULL;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormAlocaRegulador::AjustaCurvas(TChart *Chart)
{
	TChartSeries *serie;
	double valor_min, valor_max;
	bool first = true;
	// redefine os eixos
	// eixo x
	Chart->Axes->Bottom->Automatic = true;

	for (int n = 0; n < Chart->SeriesCount(); n++)
	{
		serie = Chart->Series[n];
		if (!serie->Active)
			continue;
		if (first)
		{
			first = false;
			valor_min = serie->YValues->MinValue;
			valor_max = serie->YValues->MaxValue;
		}
		else
		{
			valor_min = std::min(valor_min, serie->YValues->MinValue);
			valor_max = std::max(valor_max, serie->YValues->MaxValue);
		}
	}
	// eixo y
	Chart->Axes->Left->Automatic = false;
	Chart->Axes->Left->Minimum = 0.;
	Chart->Axes->Left->Maximum = 1.05 * valor_max;
	Chart->Axes->Left->Minimum = 0.95 * valor_min;
}

/*
 //---------------------------------------------------------------------------
 void __fastcall TFormAlocaRegulador::AlteraRegAlocado(void)
 { //DVK 2015.07.21
 //variaveis locais
 VTRegulador *reg;

 for(int nr = 0; nr < lisREG_ALOCADO->Count; nr++)
 {
 reg = (VTRegulador*)lisREG_ALOCADO->Items[nr];
 reg->ajuste_auto = (CBoxAjuste->ItemIndex == 2);
 }
 }
 */
// ---------------------------------------------------------------------------
bool __fastcall TFormAlocaRegulador::AlteraRegEstoque(void)
{ // DVK 2015.07.21
	// variaveis locais
	TEstoque *est;
	VTRegulador *reg;
	double snom;
	AnsiString str_snom;

	try
	{
		for (int nr = 0; nr < lisTabReg->Count; nr++)
		{ // verifica o tipo de ligação que foi selecionado no CBoxLigacao
			est = (TEstoque*)lisTabReg->Items[nr];
			reg = est->Regulador;
			// define tipo de ligação e fases
			switch (CBoxLigacao->ItemIndex)
			{
			case 0:
				reg->ligacao = lgEST_ISO;
				reg->fases = faseABC;
				break;
			case 1:
				reg->ligacao = lgDELTA_FECHADO;
				reg->fases = faseABC;
				break;
			case 2:
				reg->ligacao = lgDELTA_ABERTO;
				reg->fases = faseA;
				break;
			default:
				Aviso("Tipo de ligação indefinida");
				return (false);

			}
			// verifica o tipo de ajuste
			switch (CBoxAjuste->ItemIndex)
			{
			case 0:
			case 1:
				reg->modelo = reguFIXO;
				break;
			case 2:
				reg->modelo = reguAUTO_FIXA;
				break;
			case 3:
				reg->modelo = reguAUTO_UNI;
				break;
			default:
				Aviso("Tipo de ajuste indefinido");
				return (false);
			}
			// verifica os valores de potencia nominal e quantidade em estoque
			snom = (double)reg->snom;
			str_snom = gridRT->Cells[1][nr + 1];
			// valida
			if ((!StrToDouble(str_snom, snom)) || (snom <= 0))
			{
				Aviso("Potência inválida: " + gridRT->Cells[1][nr + 1]);
				return (false);
			}
			if (StrToInt(gridRT->Cells[2][nr + 1]) < 0)
			{
				Aviso("Quantidade em estoque inválida: " + gridRT->Cells[2][nr + 1]);
				return (false);
			}
			reg->snom = snom;
			est->Unidades = StrToInt(gridRT->Cells[2][nr + 1]);
		}
	}
	catch (Exception &e)
	{
		Aviso("Erro na leitura dos parâmetros");
		return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
void __fastcall TFormAlocaRegulador::CancelaAlocacao(void)
{
	// var local
	VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));
	VTGrafico *graf = (VTGrafico*) apl->GetObject(__classid(VTGrafico));
	VTSinapChild *sinap_child = (VTSinapChild*) apl->GetObject(__classid(VTSinapChild));

	// IMPORTANTE: apaga Moldura para que não fique com eventual Eqpto que será removido da Rede
	Moldura();
	// exibe gráfico de topologia p/ evitar cálculo de fluxo durante a alocação
	sinap_child->ActionTopo->Execute();
	// remove Reguladores alocados
	alocaRT->RetiraBancosAlocadosDaRede();
	// reabilita Reguladores originais da rede que estavam ativos
	HabilitaReguladoresOriginais();
	// restaura Obra original ao objeto Edita
	edita->Obra = obra_original; // DVK 2013.07.23
	// atualiza grafico
	if (graf)
		graf->AreaRedeAltera();
	// reinicia AlocaRT para que a lista de Reguladores alocados fique vazia
	alocaRT->Reinicia();
}

// ---------------------------------------------------------------------------
void __fastcall TFormAlocaRegulador::CBoxAjusteChange(TObject *Sender)
{
	// exibe esconde PanelTapFixo
	PanelTapFixo->Visible = (CBoxAjuste->ItemIndex == 0);
}

// ---------------------------------------------------------------------------
void __fastcall TFormAlocaRegulador::CheckBoxDistanciaClick(TObject *Sender)
{ // DVK 2015.07.24
	// mostra somente se estiver checado
	RichEditDistancia->Visible = (CheckBoxDistancia->Checked);
}

// ---------------------------------------------------------------------------
void __fastcall TFormAlocaRegulador::ConfirmaAlocacao(void)
{
	// variáveis locais
	VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));
	VTGrafico *graf = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	VTSinapChild *sinap_child = (VTSinapChild*) apl->GetObject(__classid(VTSinapChild));

	// IMPORTANTE: apaga Moldura para que não fique com eventual Eqpto que será removido da Rede
	Moldura(); // DVK 2013.07.23
	// exibe gráfico de topologia p/ evitar cálculo de fluxo durante a alocação
	sinap_child->ActionTopo->Execute();
	// reabilita Reguladores originais da rede que estavam ativos
	HabilitaReguladoresOriginais();
	// remove Reguladores alocados
	alocaRT->RetiraBancosAlocadosDaRede();
	// restaura Obra original ao objeto Edita
	edita->Obra = obra_original;
    // insere os Bancos alocados na Rede
	alocaRT->InsereBancosAlocadosNaRede();
	if (lisREG_ALOCADO->Count > 0) // DVK 2014.04.03
	{ // desabilita os reguladores existentes
		// DesabilitaReguladoresOriginais();
		if (checkDeleteLisReg->Checked)
			RemoveReguladoresOriginais(); // DVK 2013.07.23
	}
	// atualiza gráfico
	graf->AreaRedeDefine();
	// reinicia AlocaRT para que a lista de Reguladores alocados fique vazia
	alocaRT->Reinicia();
}

// ---------------------------------------------------------------------------
void __fastcall TFormAlocaRegulador::CriaEqptosFicticios(void)
{
	// var locais
	VTPatamares *pat = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
	// cria as barras fictícias da ligação
	barra1 = DLL_NewObjBarra(pat->NumPatamar());
	barra2 = DLL_NewObjBarra(pat->NumPatamar());
	barra1->Codigo = "Entrada";
	barra2->Codigo = "Saída";
	// cria rede
	rede = DLL_NewObjRede(pat->NumPatamar());
	rede->Codigo = "Estoque";
	rede->Carregada = true;
}

// ---------------------------------------------------------------------------
void __fastcall TFormAlocaRegulador::CriaModelosReg(void)
{ // DVK 2014.04.04
	// variaveis locais
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
	double snom_mva = 0.;
	int num_reg, unid;

	if ((geral) && (geral->Infoset->LoadFromFile("TFormAlocaRegulador_Dados")))
	{ // recupera do infoset
		geral->Infoset->GetInfo("QtdTiposReg", num_reg);
		for (int nr = 0; nr < num_reg; nr++)
		{
			geral->Infoset->GetInfo("reg->snom[" + IntToStr(nr) + "]", snom_mva);
			geral->Infoset->GetInfo("Unidades " + IntToStr(nr), unid);
			CriaUmRegulador(snom_mva, unid);
		}
	}
	else
	{ // reguladores default
		CriaUmRegulador(0.1, 20);
		CriaUmRegulador(0.2, 20);
		CriaUmRegulador(0.5, 20);
		CriaUmRegulador(1.0, 20);
		CriaUmRegulador(2.0, 20);
		CriaUmRegulador(2.5, 20);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormAlocaRegulador::CriaUmRegulador(double snom, int unid)
{
	// var locais
	TEstoque *estoque;
	VTPatamares *pat = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
	VTRegulador *reg;
	int var_tensao = 10; // DVK 2014.04.03

	try
	{
		reg = DLL_NewObjRegulador(pat->NumPatamar()); // cria reg p/ teste
		// define os ponteiros necessários
		reg->pbarra1 = barra1;
		reg->pbarra2 = barra2;
		reg->rede = rede;

		reg->Codigo = "RT_" + IntToStr(lisTabReg->Count + 1);
		reg->snom = snom; // MVA
		reg->modelo = reguAUTO_UNI;
		reg->var_tensao = var_tensao; // %
		reg->num_passo = 16; // total 32 degraus(+-)
		// configurações
		reg->cfg_auto.bar_ref = barra2;
		reg->cfg_auto.bar_ref_inv = barra1;
		// DVK 2013.08.01 ligacao default
		reg->ligacao = lgEST_ISO;
		reg->cfg_auto.v_pu = alocaRT->V_ALVO;
		reg->cfg_auto.vinv_pu = alocaRT->V_ALVO;
		reg->cfg_auto.dv_pu = 0.001;
		// cria objeto estoque  //DVK 2015.07.21
		estoque = new TEstoque();
		estoque->Unidades = unid;
		estoque->Regulador = reg;
		//valores de banda morta default
		reg->cfg_auto.larg_bandamorta_inv_pu = 0;
		reg->cfg_auto.larg_bandamorta_pu = 0;
		reg->rel_tap = 115;
	}
	catch (Exception &e)
	{ // o regulador é destruido em TEstoque //DVK 2015.07.21
		if (estoque)
		{
			delete estoque;
			estoque = NULL;
		}
		return;
	}
	// guarda estoque na lista //DVK 2015.07.21
	// lisTabReg->Add(reg);
	lisTabReg->Add(estoque);
}

// ---------------------------------------------------------------------------
void __fastcall TFormAlocaRegulador::ConfiguraReguladoresOriginais(void)
{
	// variáveis locais
	VTRegulador *regulador;

	// habilita/desabilita bypass dos reguladores conforme opção do usuário
	for (int n = 0; n < lisREG_ORIG_ATIVO->Count; n++)
	{
		regulador = (VTRegulador*)lisREG_ORIG_ATIVO->Items[n];
		if (checkDeleteLisReg->Checked)
			regulador->by_pass = true;
		else
			regulador->by_pass = false;
	}
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TFormAlocaRegulador::DeterminaModeloReg(int tipo)
{
	switch (tipo)
	{
	case reguFIXO:
		return ("Tap fixo");
	case reguAUTO_UNI:
		return ("Tap automático unidirecional");
	case reguAUTO_BI:
		return ("Tap automático bidirecional");
	case reguAUTO_FIXA:
		return ("Tap automático barra fixa");
	case reguENERQCT:
		return ("Regulador BT");
	default:
		return ("Indefinido");
	}
}

// ---------------------------------------------------------------------------
int __fastcall TFormAlocaRegulador::DeterminaReguladorSelecionado(void)
{
	// proteção:
	if (lisTabReg->Count == 0)
		return 0;
	if (gridRT->Row <= 0)
		return 0;
	if (gridRT->Row > lisTabReg->Count)
		return 0;

	return (gridRT->Row);
}

// ---------------------------------------------------------------------------
void __fastcall TFormAlocaRegulador::EliminaEqptosFicticios(void)
{
	// elimina objetos criados
	if (barra1)
	{
		DLL_DeleteEqpto(barra1);
		barra1 = NULL;
	}
	if (barra2)
	{
		DLL_DeleteEqpto(barra2);
		barra2 = NULL;
	}
	if (rede)
	{
		DLL_DeleteEqpto(rede);
		rede = NULL;
	}
}

/*
 //---------------------------------------------------------------------------
 void __fastcall TFormAlocaRegulador::ExibeGrafico(VTEqpto *eqpto)
 {
 VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
 VTRegulador *reg   = (VTRegulador*) eqpto;
 VTPatamar   *patamar;
 strHM       patHM;
 double      v1, v2;

 //limpa gráfico
 for(int n = chart->SeriesCount()-1; n >= 0; n--)   chart->Series[n]->Clear();

 //exibe os valores de tensão
 for(int ind_pat = 0 ; ind_pat < patamares->NumPatamar(); ind_pat++)
 {
 patamar      = patamares->Patamar[ind_pat];
 patHM.hora   = patamar->HoraIni;
 patHM.minuto = patamar->MinutoIni;
 //obtém tensão do eqpto no patamar
 v1 = TensaoMinima(reg->pbarra1, ind_pat); //reg->pbarra1->resflow->Vfn_pu_min[ind_pat];
 v2 = TensaoRegulada(reg, v1);
 //plote x,y
 InsereXY(ind_pat,   v1, chart, 0);
 InsereXY(ind_pat,   v2, chart, 1);
 }
 AjustaCurvas(chart);
 }
 */
// ---------------------------------------------------------------------------
void __fastcall TFormAlocaRegulador::ExibeGrafico(VTEqpto *eqpto)
{
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
	VTRegulador *reg = (VTRegulador*) eqpto;
	VTPatamar *patamar;
	strHM patHM;
	double v1, v2;

	// limpa gráfico
	for (int n = chart->SeriesCount() - 1; n >= 0; n--)
		chart->Series[n]->Clear();
	// exibe os valores de tensão
	for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
	{
		patamar = patamares->Patamar[ind_pat];
		patHM.hora = patamar->HoraIni;
		patHM.minuto = patamar->MinutoIni;
		// obtém tensão do eqpto no patamar
		v1 = reg->pbarra1->resflow->Vfn_pu_min[ind_pat];
		v2 = reg->pbarra2->resflow->Vfn_pu_min[ind_pat];
		// plote x,y
		InsereXY(ind_pat, v1, chart, 0);
		InsereXY(ind_pat, v2, chart, 1);
	}
	AjustaCurvas(chart);
}

// ---------------------------------------------------------------------------
void __fastcall TFormAlocaRegulador::ExibeListaAlocada(TList *lisREG)
{
	// variáveis locais
	AnsiString txt;
	VTRegulador *reg;

	// atualiza  PageControl
	TabLisAlocada->TabVisible = true;
	PageControl->ActivePage = TabLisAlocada;
	// atualiza ListBoxReg
	ListBoxReg->Items->Clear();
	for (int n = 0; n < lisREG->Count; n++)
	{
		reg = (VTRegulador*) lisREG->Items[n];
		// DVK 2014.04.03 unidade kVA
		ListBoxReg->Items->AddObject(txt.sprintf("Regulador-%02d [%.f kVA]", n + 1,
			reg->snom * 1000), reg);
	}
	// pré-seleciona primeito item
	if (ListBoxReg->Items->Count > 0)
	{
		ListBoxReg->Selected[0] = true;
		// simula seleção pelo usuário
		ListBoxRegClick(ListBoxReg);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormAlocaRegulador::ExportaAlocados(void)
{
	// variaveis locais
	TLine line;
	TStrings *lines;
	VTRegulador *reg;
	VTPath *path = (VTPath*) apl->GetObject(__classid(VTPath));

	// proteção
	if (!path)
		return;
	if (ListBoxReg->Items->Count == 0)
	{
		Aviso("Nenhum regulador foi alocado");
		return;
	}
	try
	{ // define pasta padrao para SaveDialog
		SaveDialog->InitialDir = path->DirExporta();
		// define nome padrao do arquivo
		SaveDialog->FileName = "ReguladoresAlocados.csv";
		// define extensão
		SaveDialog->DefaultExt = ".csv";
		// cria lines
		lines = new TStringList();
		if (!lines)
			return;
		lines->Clear();
		// esconde form, para a janela Salvar Como não ficar por baixo
		Hide();
		if (SaveDialog->Execute())
		{ // primeira linha
			line.Clear();
			line.Separador = ';';
			line.InsereCampo("Lista de reguladores alocados e suas coordenadas (x,y) em [m]");
			// insere linha
			lines->Add(line.Texto());
			for (int nc = 0; nc < ListBoxReg->Items->Count; nc++)
			{ // lista reguladores alocados
				reg = (VTRegulador*)ListBoxReg->Items->Objects[nc];
				// reinicia line
				line.Clear();
				line.Separador = ';';
				// linha: codigo do regulador; coord_utm x; coord_utm y;
				line.InsereCampo(reg->Codigo);
				line.InsereCampo(reg->pbarra1->utm.x / 100); // cm=>m
				line.InsereCampo(reg->pbarra1->utm.y / 100); // cm=>m
				// insere linha
				lines->Add(line.Texto());
			}
			// salva
			lines->SaveToFile(SaveDialog->FileName);
		}
		// destroi lines
		delete lines;
		Aviso("A lista de reguladores alocados foi exportada corretamente");
		// reapresenta form
		Show();
	}
	catch (Exception &e)
	{
		Aviso("Erro ao exportar lista de reguladores alocados");
		if (lines)
			delete lines;
		// reapresenta form
		Show();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormAlocaRegulador::ExportaExistentes(void)
{
	// variaveis locais
	AnsiString nome_arq;
	VTReport *report;
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));

	// cria objeto report
	report = DLL_NewObjReport(apl);
	// esconde form, para a janela Salvar Como não ficar por baixo
	Hide();
	// define pasta inicial
	SaveDialog->InitialDir = path->DirExporta();
	if (SaveDialog->Execute())
	{ // recebe nome do arquivo a ser exportado e exporta listView
		nome_arq = SaveDialog->FileName;
		if (report->ArquivoTexto(nome_arq, ListViewReg, "", false))
			Aviso("A tabela foi exportada corretamente.");
		else
			Erro("Erro ao exportar tabela.");
	}
	// destroi report
	if (report)
	{
		DLL_DeleteObjReport(report);
		report = NULL;
	}
	// reapresenta form
	Show();
}

// ---------------------------------------------------------------------------
void __fastcall TFormAlocaRegulador::FormClose(TObject *Sender, TCloseAction &Action)
{
	// destrói o Form
	Action = caFree;
}

// ---------------------------------------------------------------------------
void __fastcall TFormAlocaRegulador::FormCloseQuery(TObject *Sender, bool &CanClose)
{
	// assume que o form pode ser fechado
	CanClose = true;
	// verifica se há capacitores alocados
	lisEQP->Clear();
	if (alocaRT->ListaReguladorAlocado(lisEQP) == 0)
		return;
	// pede confirmação do usuário
	switch (Confirma("Foi feita alocação de reguladores.",
		"Deseja incorporar os novos reguladores à rede ?", MB_YESNOCANCEL))
	{
	case IDYES:
		ConfirmaAlocacao();
		CanClose = true;
		break;
	case IDNO:
		CancelaAlocacao();
		CanClose = true;
		break;
	case IDCANCEL:
	default:
		CanClose = false;
		break;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormAlocaRegulador::FormIniciaPosicao(void)
{
	// variáveis locais
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

	if (geral)
	{ // lê parâmetros do Form
		if (geral->Infoset->LoadFromFile(this))
			return;
	}
	// posiciona o Form sem alinhamento
	Left = 0;
	Top = 0;
}

// ---------------------------------------------------------------------------
void __fastcall TFormAlocaRegulador::FormShow(TObject *Sender)
{
	// reposiciona o Form
	// Top  = 0;
	// Left = 0;
}

// ---------------------------------------------------------------------------
void __fastcall TFormAlocaRegulador::HabilitaReguladoresOriginais(void)
{
	// variáveis locais
	VTRegulador *regulador;

	// desabilita bypass dos Reguladores originalmente ativos
	for (int n = 0; n < lisREG_ORIG_ATIVO->Count; n++)
	{
		regulador = (VTRegulador*)lisREG_ORIG_ATIVO->Items[n];
		regulador->by_pass = false; // DVK 2013.07.23 true
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormAlocaRegulador::IniciaGrid(void)
{
	// variavel local
	TEstoque *est;
	VTRegulador *reg;

	// titulo
	// AnsiString tit[] = {"Índice", "Snom(MVA)", "TIPO"};
	AnsiString tit[] =
	{"Índice", "Snom(MVA)", "Unidades"};
	int col = sizeof(tit) / sizeof(tit[0]);

	// limpa grid
	for (int n = 0; n < gridRT->RowCount; n++)
		gridRT->Rows[n]->Clear();

	// define número de linhas e colunas
	gridRT->ColCount = col;
	gridRT->RowCount = lisTabReg->Count + 1;
	if (gridRT->RowCount > 1)
		gridRT->FixedRows = 1;
	if (gridRT->ColCount > 1)
		gridRT->FixedCols = 1;

	// preenche o título
	for (int n = 0; n < col; n++)
		gridRT->Cells[n][0] = tit[n];

	// preenhe lista
	for (int n = 0; n < lisTabReg->Count; n++)
	{ // DVK 2015.07.21
		est = (TEstoque*)lisTabReg->Items[n];
		reg = est->Regulador;
		gridRT->Cells[0][n + 1] = IntToStr(n + 1);
		gridRT->Cells[1][n + 1] = DoubleToStr("%2.3f", reg->snom);
		// DVK 2015.07.21 quantidade no estoque
		gridRT->Cells[2][n + 1] = IntToStr(est->Unidades);
		// gridRT->Cells[2][n+1] = reg->ajuste_auto ? "AUTO" : "FIXO";
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormAlocaRegulador::IniciaListaReguladoresOriginais(void)
{
	// variáveis locais
	VTRegulador *regulador;
	VTRedes *redes = (VTRedes*) apl->GetObject(__classid(VTRedes));

	// reinicia lisREG_ORIG_TODOS com todos reguladores da rede
	lisREG_ORIG_TODOS->Clear();
	redes->LisLigacao(lisREG_ORIG_TODOS, eqptoREGULADOR);
	// reinicia lisREG_ORIG_ATIVO com os reguladores ativos (by_pass false)
	lisREG_ORIG_ATIVO->Clear();
	for (int n = lisREG_ORIG_TODOS->Count - 1; n >= 0; n--)
	{
		regulador = (VTRegulador*)lisREG_ORIG_TODOS->Items[n];
		//verifica se a rede está visível (rede da obra do planejamento)
		if (!regulador->rede->Visible())
		{
			lisREG_ORIG_TODOS->Remove(regulador);
			continue;
		}
		if (!regulador->by_pass)
		{
			lisREG_ORIG_ATIVO->Add(regulador);
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormAlocaRegulador::IniciaParametros(void)
{ // DVK 2014.04.04
	// variaveis locais
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
	AnsiString str_aux;

	// preenche parametros default
	RichEditVim_pu->Text = DoubleToStr("%4.3f", alocaRT->V_MIN);
	RichEditValvo_pu->Text = DoubleToStr("%4.3f", alocaRT->V_ALVO);
	RichEditDistancia->Text = DoubleToStr("%4.3f", alocaRT->DIST_M);
	RichEditMaxReg->Text = IntToStr(alocaRT->MAX_REG);
	RichEditTapFixo->Text = IntToStr(alocaRT->TAP_FIXO);
	checkTronco->Checked = alocaRT->OP_TRONCO;
	if (geral)
	{ // recupera do infoset
		if (geral->Infoset->LoadFromFile("TFormAlocaRegulador_Dados"))
		{
			geral->Infoset->GetInfo("Vmin_pu", str_aux);
			RichEditVim_pu->Text = str_aux;
			geral->Infoset->GetInfo("Valvo_pu", str_aux);
			RichEditValvo_pu->Text = str_aux;
			geral->Infoset->GetInfo("MaxReg", str_aux);
			RichEditMaxReg->Text = str_aux;
			geral->Infoset->GetInfo("Dist", str_aux);
			RichEditDistancia->Text = str_aux;
			geral->Infoset->GetInfo("TapFixo", str_aux);
			RichEditTapFixo->Text = str_aux;
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormAlocaRegulador::InsereReguladores(TList *lisReg)
{
	// var local
	VTRegulador *reg;

	// insere regulador novamente na sua rede
	for (int nreg = 0; nreg < lisReg->Count; nreg++)
	{
		reg = (VTRegulador*) lisReg->Items[nreg];
		if (reg->rede)
			reg->rede->InsereLigacao(reg);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormAlocaRegulador::InsereXY(double X, double Y, TChart* chart, int index)
{
	if (!chart)
		return;
	TChartSeries *series = chart->Series[index];
	if (series)
		series->AddXY(X, Y);
	if (series)
		series->AddXY(X + 1, Y);
}

// ---------------------------------------------------------------------------
bool __fastcall TFormAlocaRegulador::LeParametros(void)
{
	// var local
	double v_min, v_alvo, dist_m;
	int max_reg, tipo_ajuste, tap_fixo;
	try
	{
		v_min = StrToDouble(RichEditVim_pu->Text);
		v_alvo = StrToDouble(RichEditValvo_pu->Text);
		max_reg = StrToInteger(RichEditMaxReg->Text);
		dist_m = StrToDouble(RichEditDistancia->Text);
		tipo_ajuste = CBoxAjuste->ItemIndex;
		tap_fixo = StrToInteger(RichEditTapFixo->Text);
		// copia
		alocaRT->V_MIN = v_min;
		alocaRT->V_ALVO = v_alvo;
		alocaRT->OP_TRONCO = checkTronco->Checked;
		alocaRT->MAX_REG = max_reg;
		alocaRT->OP_DIST = CheckBoxDistancia->Checked;
		alocaRT->DIST_M = dist_m;
		alocaRT->TIPO_AJUSTE = tipo_ajuste;
		alocaRT->TAP_FIXO = tap_fixo;
		// verifica se o tipo de ajuste foi selecionado  //DVK 2015.07.21
		if (CBoxAjuste->ItemIndex < 0)
		{
			Aviso("Selecione um tipo de ajuste para os reguladores");
			return false;
		}
		// verifica se o tipo de ligação foi selecionado //DVK 2015.07.16
		if (CBoxLigacao->ItemIndex < 0)
		{
			Aviso("Selecione um tipo de ligação para as unidades monofásicas");
			return false;
		}
		// altera o tipo de ligação e de ajuste dos reguladores em estoque
		if (!AlteraRegEstoque())
			return (false);
	}
	catch (Exception &e)
	{
		Aviso("Parâmetro inválido: \n" + e.Message);
		return false;
	}
	return true;
}

// ---------------------------------------------------------------------------
void __fastcall TFormAlocaRegulador::ListBoxRegClick(TObject *Sender)
{
	// var local
	VTEqpto *eqpto;
	VTGrafico *graf = (VTGrafico*) apl->GetObject(__classid(VTGrafico));

	lisEQP->Clear();
	for (int i = 0; i < ListBoxReg->Items->Count; i++)
	{
		if (ListBoxReg->Selected[i])
		{
			eqpto = (VTEqpto*) ListBoxReg->Items->Objects[i];
			lisEQP->Add(eqpto);
		}
	}
	// desenha a moldura
	if (lisEQP->Count > 0)
		graf->Moldura(lisEQP);
	// exibe o gráfico do item selecionado
	if (lisEQP->Count > 0)
		ExibeGrafico((VTEqpto*) lisEQP->First());
}

// ---------------------------------------------------------------------------
void __fastcall TFormAlocaRegulador::ListBoxRegDblClick(TObject *Sender)
{
	// variáveis locais
	VTEqpto *eqpto = NULL;
	VTGrafico *grafico = (VTGrafico*) apl->GetObject(__classid(VTGrafico));

	for (int i = 0; i < ListBoxReg->Items->Count; i++)
	{
		if (ListBoxReg->Selected[i])
		{
			eqpto = (VTEqpto*) ListBoxReg->Items->Objects[i];
			grafico->Moldura(eqpto);
			grafico->SelecionaZoom(eqpto);
			break;
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormAlocaRegulador::ListViewRegClick(TObject *Sender)
{ // DVK 2014.02.06
	if (ListViewReg->Selected == NULL)
		return;
	// moldura no equipamento
	Moldura((VTEqpto*) ListViewReg->Selected->Data);
}

// ---------------------------------------------------------------------------
void __fastcall TFormAlocaRegulador::ListViewRegColumnClick(TObject *Sender, TListColumn *Column)
{ // DVK 2014.02.06
	// verifica se selecionou a mesma coluna
	if (LViewSort.Column == Column)
	{ // inverte ordem
		LViewSort.OrdemCrescente = !LViewSort.OrdemCrescente;
	}
	else
	{ // salva coluna usada p/ ordenação
		LViewSort.Column = Column;
		LViewSort.OrdemCrescente = true;
	}
	// força reordenação
	((TListView*)Sender)->AlphaSort();
}

// ---------------------------------------------------------------------------
void __fastcall TFormAlocaRegulador::ListViewRegCompare(TObject *Sender, TListItem *Item1,
	TListItem *Item2, int Data, int &Compare)
{ // DVK 2014.02.06
	// variáveis locais
	int index;
	double valor1, valor2;

	// proteção
	if (LViewSort.Column == NULL)
		return;
	// determina index p/ substrings
	index = LViewSort.Column->Index - 1;
	// verifica se coluna de Caption
	if (LViewSort.Column->Index == 0)
	{ // coluna de Caption
		Compare = CompareText(Item1->Caption, Item2->Caption);
	}
	else if (LViewSort.Column->Alignment == taLeftJustify)
	{ // coluna com texto
		Compare = CompareText(Item1->SubItems->Strings[index], Item2->SubItems->Strings[index]);
	}
	else
	{ // coluna com valor numérico
		valor1 = StrToDouble(Item1->SubItems->Strings[index]);
		valor2 = StrToDouble(Item2->SubItems->Strings[index]);
		if (valor1 < valor2)
			Compare = -1;
		else if (valor1 > valor2)
			Compare = 1;
		else
			Compare = 0;
	}
	// verifica se ordem crescente/decrescente
	if (!LViewSort.OrdemCrescente)
		Compare = -Compare;
}

// ---------------------------------------------------------------------------
void __fastcall TFormAlocaRegulador::ListViewRegDblClick(TObject *Sender)
{ // DVK 2014.02.06
	if (ListViewReg->Selected == NULL)
		return;
	// zoom no equipamento
	Zoom((VTEqpto*) ListViewReg->Selected->Data);
}

// ---------------------------------------------------------------------------
void __fastcall TFormAlocaRegulador::ListViewRegInicia(void)
{
	// variáveis locais
	TListItem *item;
	VTRegulador *regulador;
	AnsiString modelo;

	// reinicia  ListViewReg
	ListViewReg->Clear();
	// obtem a lista de ligações por rede
	for (int n = 0; n < lisREG_ORIG_TODOS->Count; n++)
	{
		regulador = (VTRegulador*)lisREG_ORIG_TODOS->Items[n];
		// insere ListItem
		item = ListViewReg->Items->Add();
		item->Data = regulador;
		item->Caption = regulador->rede->Codigo;
		item->SubItems->Add(regulador->Codigo);
		item->SubItems->Add(DoubleToStr("%f", regulador->snom));
		item->SubItems->Add(DoubleToStr("%f", regulador->var_tensao));
		item->SubItems->Add(IntToStr(regulador->num_passo));
		modelo = DeterminaModeloReg(regulador->modelo);
		item->SubItems->Add(modelo);
		item->SubItems->Add(regulador->by_pass ? "Inativo" : "Ativo");
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormAlocaRegulador::ListViewRegSelectItem(TObject *Sender, TListItem *Item,
	bool Selected)
{ // DVK 2014.02.06
	// proteção
	if (!Selected)
		return;
	Moldura((VTEqpto*) Item->Data);
}

// ---------------------------------------------------------------------------
void __fastcall TFormAlocaRegulador::Moldura(VTEqpto *eqpto)
{
	// variáveis locais
	VTGrafico *grafico = (VTGrafico*) apl->GetObject(__classid(VTGrafico));

	grafico->Moldura(eqpto);
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/18261839
void __fastcall TFormAlocaRegulador::RemoveLigacoes(TList *lisLig)
{
	// var local
	VTLigacao *ligacao;

	// remove ligação da rede
	for (int nlig = 0; nlig < lisLig->Count; nlig++)
	{
		ligacao = (VTLigacao*) lisLig->Items[nlig];
		if (ligacao->rede)
			ligacao->rede->RemoveLigacao(ligacao);
	}
}

/*
 //---------------------------------------------------------------------------
 void __fastcall TFormAlocaRegulador::RemoveReguladoresOriginais(void)
 {   //DVK 2013.07.23
 //var local
 VTChave     *chave;
 VTEdita     *edita  = (VTEdita*)apl->GetObject(__classid(VTEdita));
 VTPatamares *pat    = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
 VTTipos     *tipos  = (VTTipos*)apl->GetObject(__classid(VTTipos));
 VTRegulador *reg;

 //insere chave no lugar dos regs
 //   for(int nr = 0; nr < lisREG_ORIG_ATIVO->Count; nr++)
 for(int nr = 0; nr < lisREG_ORIG_TODOS->Count; nr++)
 {
 reg = (VTRegulador*) lisREG_ORIG_TODOS->Items[nr];
 chave          =  DLL_NewObjChave(pat->NumPatamar());
 chave->Id      = FalsoId();
 chave->Estado  = chvFECHADA;
 chave->pbarra1 = reg->pbarra1;
 chave->pbarra2 = reg->pbarra2;
 chave->rede    = reg->rede;
 chave->Status[sttNOVO] = true;
 chave->TipoChave = tipos->ExisteTipoChave(chaveSEC);
 edita->InsereLigacao(chave, reg->rede);
 }
 //retira todos os Reguladores originais (ativos ou não)
 edita->RetiraLisEqpto(lisREG_ORIG_TODOS);
 lisREG_ORIG_TODOS->Clear();
 lisREG_ORIG_ATIVO->Clear();
 }
 */   // DVK 2015.09.16 reescrita, sem trocar o regulador original por uma chave
// ---------------------------------------------------------------------------
void __fastcall TFormAlocaRegulador::RemoveReguladoresOriginais(void)
{
	// var local
	VTBarra *barra;
	VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTLigacao *ligacao, *liga_clone;
	VTRegulador *reg, *reg_liga;
	VTTrafo *trafo_liga;
	TList *lisLIG, *lisLIG_ALT;
	int ind_barra;

	// cria listas
	lisLIG_ALT = new TList();
	lisLIG = new TList();
	// loop em todos reguladores originais
	for (int nr = 0; nr < lisREG_ORIG_TODOS->Count; nr++)
	{
		reg = (VTRegulador*) lisREG_ORIG_TODOS->Items[nr];
		lisLIG->Clear();
		redes->LisLigacao(lisLIG, reg->pbarra1);
		for (int nl = 0; nl < lisLIG->Count; nl++)
		{ // lista todas ligações em uma das barras do regulador
			ligacao = (VTLigacao*)lisLIG->Items[nl];
			if (ligacao == reg)
				continue;
			// clona ligação e associa a Obj
			liga_clone = ligacao->Clone();
			ligacao->Obj = liga_clone;
			// determina as novas barras da liga_clone
			ind_barra = ligacao->IndiceBarra(reg->pbarra1);
			barra = ligacao->Barra(1 - ind_barra);
			liga_clone->DefineObjBarra(barra, reg->pbarra2);

			// verifica se a Ligacao é um Regulador
			if (liga_clone->Tipo() == eqptoREGULADOR)
			{
				reg_liga = (VTRegulador*)liga_clone;
				// verifica se a Barra original também é de referência para ajuste automático
				if (reg_liga->cfg_auto.bar_ref == reg->pbarra1)
				{ // redefine Barra de referência
					reg_liga->cfg_auto.bar_ref = barra;
				}
				// verifica se a Barra original também é de referência para o sentido do fluxo
				if (reg_liga->sentido.bar_ref == reg->pbarra1)
				{ // redefine Barra de referência
					reg_liga->sentido.bar_ref = barra;
				}
			}
			// verifica se a Ligacao é um Trafo
			else if (liga_clone->Tipo() == eqptoTRAFO)
			{ // verifica se a Barra desligada era de referência
				trafo_liga = (VTTrafo*)liga_clone;
				if (trafo_liga->ltc.ajuste_auto.bar_ref == reg->pbarra1)
				{ // redefine Barra de referência
					trafo_liga->ltc.ajuste_auto.bar_ref = barra;
				}
			}
			// adiciona na lista de ligações alteradas
			lisLIG_ALT->Add(ligacao);
		}
	}
	// altera ligações e retira todos os Reguladores originais (ativos ou não)
	edita->InsereAlteraRetiraLisEqpto(NULL, lisLIG_ALT, lisREG_ORIG_TODOS);
	lisREG_ORIG_TODOS->Clear();
	lisREG_ORIG_ATIVO->Clear();
	// destroi listas
	delete lisLIG_ALT;
	delete lisLIG;
}

// ---------------------------------------------------------------------------
void __fastcall TFormAlocaRegulador::SalvaDados(void)
{ // DVK 2014.04.04
	// variaveis locais
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

	// salva parâmetros
	if (geral)
	{ // restrições
		geral->Infoset->AddInfo("Vmin_pu", RichEditVim_pu->Text);
		geral->Infoset->AddInfo("Valvo_pu", RichEditValvo_pu->Text);
		geral->Infoset->AddInfo("MaxReg", RichEditMaxReg->Text);
		geral->Infoset->AddInfo("QtdTiposReg", lisTabReg->Count);
		geral->Infoset->AddInfo("Dist", RichEditDistancia->Text);
		geral->Infoset->AddInfo("TapFixo", RichEditTapFixo->Text);

		for (int n = 0; n < lisTabReg->Count; n++)
		{ // lista de reguladores
			geral->Infoset->AddInfo("reg->snom[" + AnsiString(n) + "]", gridRT->Cells[1][n + 1]);
			geral->Infoset->AddInfo("Unidades " + AnsiString(n), gridRT->Cells[2][n + 1]);
		}
		geral->Infoset->SaveToFile("TFormAlocaRegulador_Dados");
	}
}

// ---------------------------------------------------------------------------
double __fastcall TFormAlocaRegulador::SnomToSpassante(double Snom, double dv_perc)
{ // DVK 2014.04.03
	// variáveis locais
	double Spas = Snom;

	if (dv_perc > 0)
	{ // potência passante
		Spas = Snom / (dv_perc * 0.01);
	}
	return (Spas);
}

// ---------------------------------------------------------------------------
double __fastcall TFormAlocaRegulador::SpassanteToSnom(double Spas, double dv_perc)
{ // DVK 2014.04.03
	// variáveis locais
	double Snom = Spas;

	if (dv_perc > 0)
	{ // potência passante
		Snom = Spas * (dv_perc * 0.01);
	}
	return (Snom);
}

// ---------------------------------------------------------------------------
double __fastcall TFormAlocaRegulador::TensaoMinima(VTBarra *barra, int ind_pat)
{
	double vmin;
	double v1 = Abs(barra->resflow->Van_pu[ind_pat]);
	double v2 = Abs(barra->resflow->Vbn_pu[ind_pat]);
	double v3 = Abs(barra->resflow->Vcn_pu[ind_pat]);

	vmin = min(v1, v2);
	vmin = min(vmin, v3);

	return (vmin);
}

/*
 //---------------------------------------------------------------------------
 double __fastcall TFormAlocaRegulador::TensaoRegulada(VTRegulador *reg, double v_pu)
 {
 //var local
 double dv_pu = reg->var_tensao/reg->num_passo/100.; //ganho/tap
 int    tap   = reg->cfg_fixo.passo;
 double ganho;
 double erro, erro_min = 1.;

 //FIXO: nada a fazer => mantem tap

 //AUTOMATICO: tensão abaixo da referencia: aumenta tap
 if(reg->ajuste_auto)
 {//ajusta tap
 ganho = 1.;
 for(int index = 0; index < reg->num_passo; index++)
 {
 //calcula a nova tensão com elevação do tap
 ganho += dv_pu;
 //erro
 erro = fabs(ganho * v_pu - alocaRT->V_ALVO);
 //salva menor erro encontrado
 if(erro < erro_min)
 {
 erro_min = erro;
 tap      = index + 1;
 }
 }
 }
 ganho = 1. + tap*dv_pu;
 return(ganho * v_pu);
 }
 */
// ---------------------------------------------------------------------------
bool __fastcall TFormAlocaRegulador::VerificaPlanejamento(void)
{
	VTPlanejamento *planejamento = (VTPlanejamento*) apl->GetObject(__classid(VTPlanejamento));
	VTEstudo *estudo;

	// proteção: rede original //DVK 2013.07.23
	estudo = planejamento->ExisteEstudo();
	if ((estudo->Tipo > 0) && (estudo->AlternativaAtiva == estudo->AlternativaPadrao))
	{
		Aviso("Não é possível alocar reguladores na rede original");
		return (false);
	}
	else if ((estudo->Tipo > 0) && (estudo->AlternativaAtiva->Obras->LisObra()->Count == 0))
	{
		Aviso("Crie primeiro uma obra para esta alternativa");
		return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
void __fastcall TFormAlocaRegulador::Zoom(VTEqpto *eqpto)
{ // DVK 2014.02.06
	// var local
	VTGrafico *graf = (VTGrafico*) apl->GetObject(__classid(VTGrafico));
	if (graf && eqpto)
		graf->SelecionaZoom(eqpto);
}

// ---------------------------------------------------------------------------
// eof

