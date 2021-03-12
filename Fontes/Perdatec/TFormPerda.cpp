// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DateUtils.hpp>
#include "TFormPerda.h"
#include "Relatorio\TRelBalAgregado.h"
#include "Relatorio\TRelEqptoAgregado.h"
#include "Relatorio\TRelPerdaAgregado.h"
#include "Relatorio\TRelRedeBalanco.h"
#include "Relatorio\TRelRedeEqpto.h"
#include "Relatorio\TRelRedePerda.h"
#include "VTResumo.h"
#include "VTResumos.h"
#include "Agregada\VTAgregada.h"
#include "Agregada\TSegmento.h"
#include "..\Apl\VTApl.h"
#include "..\Diretorio\VTPath.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Radial\VTArvore.h"
#include "..\Radial\VTNode.h"
#include "..\Rede\VTTipoRede.h"
#include "..\Report\VTReport.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Help.h"
#include "..\..\DLL_Inc\Report.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
__fastcall TFormPerda::TFormPerda(TComponent *Owner, VTApl *apl_owner, TWinControl *parent)
	: TForm(Owner)
{
	// define Parent
	if (parent)
		Parent = parent;
	// salva ponteiro
	this->apl = apl_owner;
	// this->owner = Owner;  //FKM -2015.09.21 Teste
	// cria lista
	lisTMP = new TList();
	lisRES = new TList();
	// inicia controle de inicia��o do TView
	tview.last_level = -1;
	// inicia controle da ordena��o dos LView
	LViewSort.lview = NULL;
	LViewSort.column = NULL;
	LViewSort.ascendente = true;
	// inicia fat_mult
	fat_mult = 1.;
	//cria objeto relRedePerda, relRedeEqpto, relRedeBalanco, relBalAgregado, relEqptoAgregado, relPerdaAgregado
	relRedePerda = new TRelRedePerda(apl);
	relRedeEqpto = new TRelRedeEqpto(apl);
	relRedeBalanco = new TRelRedeBalanco(apl);
	relBalAgregado = new TRelBalAgregado(apl);
	relEqptoAgregado = new TRelEqptoAgregado(apl);
	relPerdaAgregado = new TRelPerdaAgregado(apl);
	// cria objeto Resumo para fazer agrega��o
	resumo_agregado = NewObjResumo();
	resumo_agregado->tipo_resumo = resumoAGREGADO;
	// cria objeto Agregada para montar Balan�o agregado por segmento
	agregada = NewObjAgregada(apl);
	// seleciona relat�rio em CBoxRelatorio
	CBoxRelatorio->ItemIndex = -1;
	// simula sele��o de relat�rio p/ esconder todos os relat�rios
	CBoxRelatorioChange(NULL);
}

// ---------------------------------------------------------------------------
__fastcall TFormPerda::~TFormPerda(void)
{
	// vari�veis locais
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

	// salva par�metros do Form
	if (geral)
		geral->Infoset->SaveToFile(this);
	// destr�i objetos
	DestroiTObject(relRedePerda);
	DestroiTObject(relRedeEqpto);
	DestroiTObject(relRedeBalanco);
	DestroiTObject(relBalAgregado);
	DestroiTObject(relEqptoAgregado);
	DestroiTObject(relPerdaAgregado);
	// destr�i objetos
	if (resumo_agregado)
	{
		delete resumo_agregado;
		resumo_agregado = NULL;
	}
	if (agregada)
	{
		delete agregada;
		agregada = NULL;
	}
	// destr�i lista sem destruir seus objetos
	if (lisTMP)
	{
		delete lisTMP;
		lisTMP = NULL;
	}
	if (lisRES)
	{
		delete lisRES;
		lisRES = NULL;
	}
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/16127109
void __fastcall TFormPerda::ActionAlteraUnidadeExecute(TObject *Sender)
{
	// vari�veis locais
	AnsiString unidade;
	double fat_mult;
	int num_dias = DaysInMonth(date);

	if (MenuItem_kwh->Checked)
	{
		unidade = MenuItem_kwh->Caption;
		fat_mult = 1000.;
	}
	else if (MenuItem_kwh_30dias->Checked)
	{
		unidade = MenuItem_kwh_30dias->Caption;
		fat_mult = 30 * 1000.;
	}
	else if (MenuItem_kwh_diasmes->Checked)
	{
		unidade = MenuItem_kwh_diasmes->Caption;
		fat_mult = num_dias * 1000.;
	}
	else if (MenuItem_mwh->Checked)
	{
		unidade = MenuItem_mwh->Caption;
		fat_mult = 1.;
	}
	else if (MenuItem_mwh_30dias->Checked)
	{
		unidade = MenuItem_mwh_30dias->Caption;
		fat_mult = 30.;
	}
	else if (MenuItem_mwh_diasmes->Checked)
	{
		unidade = MenuItem_mwh_diasmes->Caption;
		fat_mult = num_dias;
	}
	else
	{
		Aviso("Defina a unidade para as perdas");
		return;
	}
	// atualiza FormPerda
	DefineUnidade(unidade, fat_mult);
	// atualiza em relatorios
	relRedeBalanco->DefineFatMult(fat_mult);
	relRedeEqpto->DefineFatMult(fat_mult);
	relRedePerda->DefineFatMult(fat_mult);
	relBalAgregado->DefineFatMult(fat_mult);
	relEqptoAgregado->DefineFatMult(fat_mult);
	relPerdaAgregado->DefineFatMult(fat_mult);
}

// ---------------------------------------------------------------------------
void __fastcall TFormPerda::ActionFechaExecute(TObject *Sender)
{
	Close();
}

// ---------------------------------------------------------------------------
void __fastcall TFormPerda::ActionHelpExecute(TObject *Sender)
{
	// variaveis locais
	TForm *FormHelp;

	FormHelp = DLL_NewFormHelp(this, apl, "Perda_T�cnica");
	if (FormHelp)
		FormHelp->Show();
}

// ---------------------------------------------------------------------------
void __fastcall TFormPerda::ActionLocalizaExecute(TObject *Sender)
{
	// vari�veis locais
	TTreeNode *Node;

	// verifica se EditLocaliza foi preenchido
	if (RichEditLocaliza->Text.IsEmpty())
	{
		Aviso("Defina a rede a ser localizada");
		return;
	}
	// procura TReeNode com o texto indicado
	for (int n = 0; n < TView->Items->Count; n++)
	{
		Node = TView->Items->Item[n];
		if (Node->Text.CompareIC(RichEditLocaliza->Text) == 0)
		{ // seleciona o TreeNode
			TView->Selected = Node;
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormPerda::ActionSalvaRelatorioExecute(TObject *Sender)
{
	ExportaRelatorio();
}

// ---------------------------------------------------------------------------
void __fastcall TFormPerda::AtualizaCaptions(void)
{
	// vari�veis locais
	AnsiString dias_mes;
	int num_dias = DaysInMonth(date);

	dias_mes.sprintf("/m�s(%d dias)", num_dias);
	MenuItem_kwh_diasmes->Caption = dias_mes.sprintf("kWh/m�s(%d dias)", num_dias);
	MenuItem_mwh_diasmes->Caption = dias_mes.sprintf("MWh/m�s(%d dias)", num_dias);
}

// ---------------------------------------------------------------------------
void __fastcall TFormPerda::CalculaAgregadaTotal(void)
{
	agregada->Inicia();
}

// ---------------------------------------------------------------------------
void __fastcall TFormPerda::CBoxRelatorioChange(TObject *Sender)
{
	// vari�veis locais
	VTResumos *resumos = (VTResumos*)apl->GetObject(__classid(VTResumos));

	// esconde todos GroupBox de relat�rio
	GBoxAgregadoBalanco->Visible = false;
	GBoxAgregadoEqpto->Visible = false;
	GBoxAgregadoPerda->Visible = false;
	GBoxRedeBalanco->Visible = false;
	GBoxRedeEqpto->Visible = false;
	GBoxRedePerda->Visible = false;
	GBoxPerdaSegmento->Visible = false;
	// reinicia lisRES com lista de resumos do tipo de rede selecionado
	lisRES->Clear();
	// exibe GroupBox do relat�rio selecionado
	switch (CBoxRelatorio->ItemIndex)
	{
	case 0: // Equipamentos por SED
		resumos->LisResumo(redeETD, lisRES);
		MontaRelatorioEqpto(lisRES);
		TViewInicia(levelETD);
		break;
	case 1: // Equipamentos por SDMT
		resumos->LisResumo(redePRI, lisRES);
		MontaRelatorioEqpto(lisRES);
		TViewInicia(levelPRI);
		break;
	case 2: // Equipamentos por SDBT
		resumos->LisResumo(redeSEC, lisRES);
		MontaRelatorioEqpto(lisRES);
		// TViewInicia(levelPRI);
		TViewInicia(levelSEC); // FKM 2015.09.21
		break;
	case 3: // Equipamentos por Rede
		resumos->LisResumo(lisRES);
		MontaRelatorioEqpto(lisRES);
		// TViewInicia(levelPRI);
		TViewInicia(levelSEC); // FKM 2015.09.21
		break;
	case 4: // Balan�o de Energia por SED
		resumos->LisResumo(redeETD, lisRES);
		MontaRelatorioBalanco(lisRES);
		TViewInicia(levelETD);
		break;
	case 5: // Balan�o de Energia por SDMT
		resumos->LisResumo(redePRI, lisRES);
		MontaRelatorioBalanco(lisRES);
		TViewInicia(levelPRI);
		break;
	case 6: // Balan�o de Energia por SDBT
		resumos->LisResumo(redeSEC, lisRES);
		MontaRelatorioBalanco(lisRES);
		// TViewInicia(levelPRI);
		TViewInicia(levelSEC); // FKM 2015.09.21
		break;
	case 7: // Balan�o de Energia por Rede
		resumos->LisResumo(lisRES);
		MontaRelatorioBalanco(lisRES);
		// TViewInicia(levelPRI);
		TViewInicia(levelSEC); // FKM 2015.09.21
		break;
	case 8: // Perdas por SED
		resumos->LisResumo(redeETD, lisRES);
		MontaRelatorioPerda(lisRES);
		TViewInicia(levelETD);
		break;
	case 9: // Perdas por SDMT
		resumos->LisResumo(redePRI, lisRES);
		MontaRelatorioPerda(lisRES);
		TViewInicia(levelPRI);
		break;
	case 10: // Perdas por SDBT
		resumos->LisResumo(redeSEC, lisRES);
		MontaRelatorioPerda(lisRES);
		// TViewInicia(levelPRI);
		TViewInicia(levelSEC); // FKM 2015.09.21
		break;
	case 11: // Perdas por Rede
		resumos->LisResumo(lisRES);
		MontaRelatorioPerda(lisRES);
		// TViewInicia(levelPRI);
		TViewInicia(levelSEC); // FKM 2015.09.21
		break;
	case 12: // Equipamentos Agregados por SED
		resumos->LisResumo(redeETD, lisRES);
		MontaRelatorioEqptoAgregado(lisRES);
		TViewInicia(levelETD);
		break;
	case 13: // Equipamentos Agregados por SDMT
		resumos->LisResumo(redePRI, lisRES);
		MontaRelatorioEqptoAgregado(lisRES);
		TViewInicia(levelPRI);
		break;
	case 14: // Equipamentos Agregados por Rede
		resumos->LisResumo(lisRES);
		MontaRelatorioEqptoAgregado(lisRES);
		// TViewInicia(levelPRI);
		TViewInicia(levelSEC); // FKM 2015.09.21
		break;
	case 15: // Balan�o Agregado de Energia por SED
		resumos->LisResumo(redeETD, lisRES);
		MontaRelatorioBalancoAgregado(lisRES);
		TViewInicia(levelETD);
		break;
	case 16: // Balan�o Agregado de Energia por SDMT
		resumos->LisResumo(redePRI, lisRES);
		MontaRelatorioBalancoAgregado(lisRES);
		TViewInicia(levelPRI);
		break;
	case 17: // Balan�o Agregado de Energia por Rede
		resumos->LisResumo(lisRES);
		MontaRelatorioBalancoAgregado(lisRES);
		// TViewInicia(levelPRI);
		TViewInicia(levelSEC); // FKM 2015.09.21
		break;
	case 18: // Perdas Agregadas por SED
		resumos->LisResumo(redeETD, lisRES);
		MontaRelatorioPerdaAgregado(lisRES);
		TViewInicia(levelETD);
		break;
	case 19: // Perdas Agregadas por SDMT
		resumos->LisResumo(redePRI, lisRES);
		MontaRelatorioPerdaAgregado(lisRES);
		TViewInicia(levelPRI);
		break;
	case 20: // Perdas Agregadas por Rede
		resumos->LisResumo(lisRES);
		MontaRelatorioPerdaAgregado(lisRES);
		// TViewInicia(levelPRI);
		TViewInicia(levelSEC); // FKM 2015.09.21
		break;
	case 21: // Perdas Agregadas por Segmento
		MontaRelatorioPerdaSegmento();
		TViewInicia(levelETD);
		break;
	default: // nada a fazer
		break;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormPerda::CheckBoxBalancoTotalClick(TObject *Sender)
{
	// exibe esconde GBoxBalancoTotal
	GBoxBalancoTotal->Visible = CheckBoxBalancoTotal->Checked;
}

// ---------------------------------------------------------------------------
void __fastcall TFormPerda::DefineUnidade(AnsiString unidade, double fat_mult)
{
	// vari�veis locais
	int index;
	AnsiString caption;
	TGroupBox *vet_gbox[] =
	{GBoxResumo, GBoxBalancoTotal, GBoxPerdaSegmento, GBoxAgregadoBalanco, GBoxAgregadoPerda,
		// GBoxAgregadoEqpto,
		GBoxRedeBalanco, GBoxRedePerda// GBoxRedeEqpto
	};
	TListView *vet_lview[] =
	{LViewResumo, LViewTotalBalanco, LViewPerdaSegmento, LViewRedeEqpto, LViewAgregadoEqpto,
		LViewRedePerda, LViewAgregadoPerda, LViewRedeBalanco, LViewAgregadoBalanco
	};

	// salva fat_mult aplicado a todos os valores dos LView
	this->fat_mult = fat_mult;
	// reinicia todos os LView
	for (int n = 0; n < ((sizeof vet_lview) / sizeof(void*)); n++)
	{
		vet_lview[n]->Clear();
	}
	// atualiza Caption dos GroupBox
	for (int n = 0; n < ((sizeof vet_gbox) / sizeof(void*)); n++)
	{ // verifica se o Caption possui o caractere '('
		caption = vet_gbox[n]->Caption;
		if ((index = caption.AnsiPos("(")) > 0)
		{
			caption = caption.SubString(1, index - 1);
		}
		vet_gbox[n]->Caption = caption + " (" + unidade + ")";
	}
	// monta relat�rio balan�o geral
	MontaRelatorioResumo();
	// monta relat�rio do balan�o da empresa
	MontaRelatorioBalancoEmpresa();
	// simula sele��o do relat�rio p/ atualizar o LView selecionado
	if (CBoxRelatorio->ItemIndex < 0)
		CBoxRelatorio->ItemIndex = CBoxRelatorio->Items->Count - 1;
	CBoxRelatorioChange(NULL);
}

// ---------------------------------------------------------------------------
void __fastcall TFormPerda::FormClose(TObject *Sender, TCloseAction &Action)
{
	// destr�i o Form
	// Action = caFree;
	// esconde o Form
	Action = caHide;
}

// ---------------------------------------------------------------------------
void __fastcall TFormPerda::ExportaRelatorio(void)
{
	// vari�veis locais
	AnsiString arq_ext;
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));

	// define diret�rio
	if (SaveDialog->InitialDir.IsEmpty())
		SaveDialog->InitialDir = path->DirExporta();
	// define nome do arquivo
	SaveDialog->FileName = "RelPerda";
	// simula sele��o de tipo de arquivo p/ definir extens�o default
	SaveDialog->FilterIndex = 1;
	SaveDialogTypeChange(NULL);
	// seleciona arquivo
	if (SaveDialog->Execute())
	{ // determina tipo do arquivo pelo FilterIndex
		// if (SaveDialog->FilterIndex == 1) ExportaRelatorioTexto(SaveDialog->FileName);
		// else                              ExportaRelatorioExcel(SaveDialog->FileName);
		ExportaRelatorioTexto(SaveDialog->FileName);
	}
}

/*
 //---------------------------------------------------------------------------
 void __fastcall TFormPerda::ExportaRelatorioExcel(AnsiString arq)
 {
 //vari�veis locais
 VTReport *report;

 try{//cria objeto VTReport
 report = DLL_NewObjReport(apl);
 //total
 report->PlanilhaExcel(arq, "Balanco Total",      LViewTotalBalanco);
 //agregados
 report->PlanilhaExcel(arq, "Eqptos Agregados",   LViewAgregadoEqpto);
 report->PlanilhaExcel(arq, "Balancos Agregados", LViewAgregadoBalanco);
 report->PlanilhaExcel(arq, "Perdas Agregadas",   LViewAgregadoPerda);
 //dados por rede
 if (LViewRedeEqpto->Items->Count > 0)   report->PlanilhaExcel(arq, "Eqptos por Rede",  LViewRedeEqpto);
 if (LViewRedeBalanco->Items->Count > 0) report->PlanilhaExcel(arq, "Balanco por Rede", LViewRedeBalanco);
 if (LViewRedePerda->Items->Count > 0)   report->PlanilhaExcel(arq, "Perdas por Rede",  LViewRedePerda);
 //destr�i objeto
 delete report;
 }catch(Exception &e)
 {
 Erro("N�o foi poss�vel gravar o arquivo " + ExtractFileName(arq));
 }
 }
 */
// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/7975908
//https://sinapsisenergia.teamwork.com/#tasks/16127109
void __fastcall TFormPerda::ExportaRelatorioTexto(AnsiString arq)
{
	// vari�veis locais
	bool append;
	VTReport *report;
	TGroupBox *GBox;
	TListView *vet_lview[] =
	{LViewResumo, LViewTotalBalanco, LViewPerdaSegmento, LViewRedeEqpto, LViewAgregadoEqpto,
		LViewRedePerda, LViewAgregadoPerda, LViewRedeBalanco, LViewAgregadoBalanco};
	try
	{ // verifica se o arquivo j� existe
		if (!FileExists(arq))
		{
			append = false;
		}
		else
		{ // verifica se o usu�rio deseja incluir o relat�rio no arquivo (append)
			if (Confirma("O arquivo de relat�rio j� existe.",
				"Deseja acrescentar o relat�rio no mesmo arquivo ?") != IDYES)
				return;
			append = true;
		}
		// monta o relat�rio de perdas por segmento (caso ele ainda n�o exista)
		if (LViewPerdaSegmento->Items->Count == 0)
		{
			MontaRelatorioPerdaSegmento();
		}
		// cria objeto VTReport
		report = DLL_NewObjReport(apl);
		// exporta os ListView que est�o vis�veis
		for (int n = 0; n < sizeof(vet_lview) / sizeof(void*); n++)
		{
			if (vet_lview[n]->Items->Count == 0)
				continue;
			GBox = (TGroupBox*)(vet_lview[n]->Parent);
			// os 3 primeiros ListView sempre s�o exportados, os demais s� se estiverem vis�veis
			if ((n <= 2) || (GBox->Visible))
			{ // verifica se � um relatorio que foi truncado
				if (vet_lview[n] == LViewRedeEqpto)
				{ // exporta relat�rio completo
					relRedeEqpto->Executa(arq, GBox->Caption);
					append = true;
				}
				else if (vet_lview[n] == LViewAgregadoEqpto)
				{ // exporta relat�rio completo
					relEqptoAgregado->Executa(arq, GBox->Caption);
					append = true;
				}
				else if (vet_lview[n] == LViewRedePerda)
				{ // exporta relat�rio completo
					relRedePerda->Executa(arq, GBox->Caption);
					append = true;
				}
				else if (vet_lview[n] == LViewAgregadoPerda)
				{ // exporta relat�rio completo
					relPerdaAgregado->Executa(arq, GBox->Caption);
					append = true;
				}
				else if (vet_lview[n] == LViewRedeBalanco)
				{ // exporta relat�rio completo
					relRedeBalanco->Executa(arq, GBox->Caption);
					append = true;
				}
				else if (vet_lview[n] == LViewAgregadoBalanco)
				{ // exporta relat�rio completo
					relBalAgregado->Executa(arq, GBox->Caption);
					append = true;
				}
				else
				{
					report->ArquivoTexto(arq, vet_lview[n], GBox->Caption, append);
					append = true;
				}
			}
		}
		// destr�i objeto
		delete report;
		//aviso
        Aviso("Relat�rio salvo em arquivo com sucesso");
	}
	catch (Exception &e)
	{
		Erro("N�o foi poss�vel gravar o arquivo " + ExtractFileName(arq));
	}
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TFormPerda::FiltraSeparador(AnsiString valor, char separador)
{
	// vari�veis locais
	AnsiString txt;
	int len = valor.Length();

	// descarta ponto (separador de milhar)
	for (int n = 1; n <= len; n++)
	{
		if (valor[n] != separador)
			txt += valor[n];
	}
	return (txt);
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TFormPerda::FormataEnergia(double energia_mwh)
{
	// vari�veis locais
	AnsiString txt;
	struct
	{
		char DecimalSeparator;
		char ThousandSeparator;
	} original;

	// salva separadores originais
	original.DecimalSeparator = FormatSettings.DecimalSeparator;
	original.ThousandSeparator = FormatSettings.ThousandSeparator;
	// redefine separador decimal
	FormatSettings.ThousandSeparator = '.';
	FormatSettings.DecimalSeparator = ',';
	txt = AnsiString::FormatFloat("#,##0.000", energia_mwh * fat_mult);
	// restaura separadores originais
	FormatSettings.DecimalSeparator = original.DecimalSeparator;
	FormatSettings.ThousandSeparator = original.ThousandSeparator;

	return (txt);
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TFormPerda::FormataPorcentagem(double dividendo, double divisor)
{
	// vari�veis locais
	double valor;
	AnsiString txt;
	struct
	{
		char DecimalSeparator;
		char ThousandSeparator;
	} original;

	// calcula valor
	if (IsDoubleZero(divisor))
		valor = 0.;
	else
		valor = dividendo / divisor;
	// salva separadores originais
	original.DecimalSeparator = FormatSettings.DecimalSeparator;
	original.ThousandSeparator = FormatSettings.ThousandSeparator;
	// redefine separador decimal
	FormatSettings.ThousandSeparator = '.';
	FormatSettings.DecimalSeparator = ',';
	txt = AnsiString::FormatFloat("#,##0.000", valor * 100.);
	// restaura separadores originais
	FormatSettings.DecimalSeparator = original.DecimalSeparator;
	FormatSettings.ThousandSeparator = original.ThousandSeparator;

	return (txt);
}

// ---------------------------------------------------------------------------
void __fastcall TFormPerda::FormIniciaPosicao(void)
{
	// vari�veis locais
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

	if (geral)
	{ // l� par�metros do Form
		if (geral->Infoset->LoadFromFile(this))
			return;
	}
	// posiciona o Form canto superior esquerdo
	Top = 0;
	Left = 0;
}

// ---------------------------------------------------------------------------
void __fastcall TFormPerda::FormShow(TObject *Sender)
{
	// vari�veis locais
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

	if (geral)
	{ // l� par�metros do Form
		if (geral->Infoset->LoadFromFile(this))
			return;
	}
	// posiciona o Form canto superior esquerdo
	Top = 0;
	Left = 0;
}

// ---------------------------------------------------------------------------
void __fastcall TFormPerda::Inicia(TDateTime date)
{
	// salva data de refer�ncia
	this->date = date;
	// atualiza Captions
	AtualizaCaptions();
	// calcula valores agregados de todos Resumos
	CalculaAgregadaTotal();
	Show();
	// inicia TView (ap�s montar a Arvore com Resumos das Redes)
	TViewInicia();
	// simula altera��o da unidade
	ActionAlteraUnidade->Execute();
}

// ---------------------------------------------------------------------------
void __fastcall TFormPerda::LViewAddItemBalanco(TListView *LView, VTResumo *resumo_rede,
	VTResumo *resumo_jus)
{
	// vari�veis locais
	double injetada, consumida, entregue, fornecida;
	TListItem *Item;
	VTResumo *resumo = resumo_rede;
	VTResumo *resumo_total = agregada->ResumoTotal();

	// IMPORTANTE: resumo_jus � a somat�ria do Resumo da Rede e das suas Redes a jusante

	// calcula energia injetada na rede
	// injetada = resumo->balanco.EnergiaEntrada + resumo->balanco.EnergiaRecebidaExt + resumo->balanco.EnergiaRecebidaInt;
	injetada = resumo->balanco.EnergiaEntrada;
	// + resumo->balanco.EnergiaRecebidaExt + resumo->balanco.EnergiaRecebidaInt;
	// insere novo ListItem
	Item = LView->Items->Add();
	Item->Caption = resumo->rede.TipoRede;
	Item->SubItems->Add(resumo->rede.Codigo);
	// "Inj.Externa(1)" = > energia dos suprimentos
	Item->SubItems->Add(FormataEnergia(resumo->balanco.EnergiaSuprimento));
	// "Inj.Montante(2)" =  energia recebida
	Item->SubItems->Add(FormataEnergia(resumo->balanco.EnergiaRecebidaExt +
		resumo->balanco.EnergiaRecebidaInt));
	// "Inj.GD(3)" => energia gerador
	Item->SubItems->Add(FormataEnergia(resumo->balanco.EnergiaGerador));
	// "Injetada"
	Item->SubItems->Add(FormataEnergia(injetada));
	Item->SubItems->Add(FormataPorcentagem(injetada, resumo_total->balanco.EnergiaEntrada));
	// verifica se foi definido Resumo de redes a jusante
	if (resumo_jus != NULL)
		resumo = resumo_jus;
	// perda
	Item->SubItems->Add(FormataEnergia(resumo->balanco.EnergiaPerdaTotal));
	// IPTS
	Item->SubItems->Add(FormataPorcentagem(resumo->balanco.EnergiaPerdaTotal, injetada));
	// PPT
	Item->SubItems->Add(FormataPorcentagem(resumo->balanco.EnergiaPerdaTotal,
		resumo_total->balanco.EnergiaEntrada));
	// "For.Consumidor(4)" => energia carga
	consumida = resumo->balanco.EnergiaCarga;
	Item->SubItems->Add(FormataEnergia(consumida));
	// "For.Jusante(5)" => energia entregue (cuidado: s� existe energia entregue para Resumo de rede)
	if (resumo == resumo_rede)
		entregue = resumo->balanco.EnergiaEntregueExt + resumo->balanco.EnergiaEntregueInt;
	else
		entregue = 0.;
	Item->SubItems->Add(FormataEnergia(entregue));
	// "For.Externa(6)" => n�o existe no Sinap
	Item->SubItems->Add(FormataEnergia(0.));
	// fornecida = consumida + entregue + externa
	fornecida = consumida + entregue;
	Item->SubItems->Add(FormataEnergia(fornecida));
	// fornecid) / (injetada total)
	Item->SubItems->Add(FormataPorcentagem(fornecida, resumo_total->balanco.EnergiaEntrada));
}

// ---------------------------------------------------------------------------
void __fastcall TFormPerda::LViewAddItemEqpto(TListView *LView, VTResumo *resumo_rede,
	VTResumo *resumo_jus)
{
	// vari�veis locais
	TListItem *Item;
	VTResumo *resumo = resumo_rede;

	// IMPORTANTE: resumo_jus � a somat�ria do Resumo da Rede e das suas Redes a jusante

	// insere novo ListItem
	Item = LView->Items->Add();
	// insere tipo e c�digo do Resumo da rede
	Item->Caption = resumo->rede.TipoRede;
	Item->SubItems->Add(resumo->rede.Codigo);
	// verifica se foi definido um resumo agrregado
	if (resumo_jus != NULL)
		resumo = resumo_jus;
	// insere dados dos eqptos
	Item->SubItems->Add(IntToStr(resumo->rede.QtdeRede));
	Item->SubItems->Add(IntToStr(resumo->rede.QtdeTrafo));
	Item->SubItems->Add(DoubleToStr("%4.3f", resumo->rede.QtdeCabo / 1000., ','));
	Item->SubItems->Add(IntToStr(resumo->rede.QtdeRegulador));
	Item->SubItems->Add(IntToStr(resumo->rede.QtdeCapacitor));
	// Item->SubItems->Add(IntToStr(resumo->rede.QtdeReator));
	Item->SubItems->Add(IntToStr(resumo->rede.QtdeConsA4));
	Item->SubItems->Add(IntToStr(resumo->rede.QtdeConsRes));
	Item->SubItems->Add(IntToStr(resumo->rede.QtdeConsCom));
	Item->SubItems->Add(IntToStr(resumo->rede.QtdeConsInd));
	Item->SubItems->Add(IntToStr(resumo->rede.QtdeConsRur));
	Item->SubItems->Add(IntToStr(resumo->rede.QtdeConsOut));
	Item->SubItems->Add(IntToStr(resumo->rede.QtdeConsNulo));
	// enegia de Mercado (energia original das cargas)
	Item->SubItems->Add(FormataEnergia(resumo->rede.EnergiaMercado));
}

// ---------------------------------------------------------------------------
void __fastcall TFormPerda::LViewAddItemPerda(TListView *LView, VTResumo *resumo_rede,
	VTResumo *resumo_jus)
{
	// vari�veis locais
	double injetada;
	TListItem *Item;
	VTResumo *resumo = resumo_rede;
	VTResumo *resumo_total = agregada->ResumoTotal();

	// IMPORTANTE: resumo_jus � a somat�ria do Resumo da Rede e das suas Redes a jusante

	// insere novo ListItem
	Item = LView->Items->Add();
	// Resumo de uma rede
	Item->Caption = resumo->rede.TipoRede;
	Item->SubItems->Add(resumo->rede.Codigo);
	// energia de injetada
	// injetada = resumo->balanco.EnergiaEntrada + resumo->balanco.EnergiaRecebidaExt + resumo->balanco.EnergiaRecebidaInt;
	injetada = resumo->balanco.EnergiaEntrada;
	// + resumo->balanco.EnergiaRecebidaExt + resumo->balanco.EnergiaRecebidaInt;
	Item->SubItems->Add(FormataEnergia(injetada));
	// verifica se foi definido Resumo a jusante
	if (resumo_jus != NULL)
	{ // insere perda pr�pria da rede
		Item->SubItems->Add(FormataEnergia(resumo_rede->balanco.EnergiaPerdaTotal));
		// redefine resumo como resumo_jus
		resumo = resumo_jus;
	}
	// perdas calculadas pelo fluxo
	Item->SubItems->Add(FormataEnergia(resumo->perda.PerdaCabo));
	Item->SubItems->Add(FormataEnergia(resumo->perda.PerdaRegulador +
		resumo->perda.PerdaCapacitor));
	Item->SubItems->Add(FormataEnergia(resumo->perda.PerdaTrafoCobre));
	Item->SubItems->Add(FormataEnergia(resumo->perda.PerdaTrafoFerro));
	Item->SubItems->Add(FormataEnergia(resumo->perda.PerdaRamal));
	Item->SubItems->Add(FormataEnergia(resumo->perda.PerdaMedidor));
	Item->SubItems->Add(FormataEnergia(resumo->perda.PerdaOutros));
	Item->SubItems->Add(FormataEnergia(resumo->balanco.EnergiaPerdaTotal));
	// (perda da rede)/(perda total)
	Item->SubItems->Add(FormataPorcentagem(resumo->balanco.EnergiaPerdaTotal,
		resumo_total->balanco.EnergiaPerdaTotal));
	// IPTS: (perda da rede) / (energia injetada na rede)
	Item->SubItems->Add(FormataPorcentagem(resumo->balanco.EnergiaPerdaTotal, injetada));
	// PPT: (perda total) / (energia injetada total)
	Item->SubItems->Add(FormataPorcentagem(resumo->balanco.EnergiaPerdaTotal,
		resumo_total->balanco.EnergiaEntrada));
}

// ---------------------------------------------------------------------------
void __fastcall TFormPerda::LViewAddItemSegmento(TListView *LView, TSegmento *segmento)
{
	// vari�veis locais
	double injetada, consumida, entregue, fornecida;
	TListItem *Item;
	TSegmento *segmento_filho;
	VTResumo *resumo = segmento->Resumo;
	VTResumo *total = agregada->ResumoTotal();
	TList *lisSEGMENTO = segmento->LisSegmento();

	// calcula energia injetada
	// injetada = resumo->balanco.EnergiaEntrada + resumo->balanco.EnergiaRecebidaExt;
	injetada = resumo->balanco.EnergiaEntrada; // + resumo->balanco.EnergiaRecebidaExt;
	// insere novo ListItem
	Item = LView->Items->Add();
	// Tipo da rede
	Item->Caption = segmento->TipoRedeAsString();
	// level
	Item->SubItems->Add(segmento->Level);
	// "Inj.Exerna(1)" => energia dos suprimentos
	Item->SubItems->Add(FormataEnergia(resumo->balanco.EnergiaSuprimento));
	// "Inj.Montante(2)" => energia recebida
	Item->SubItems->Add(FormataEnergia(resumo->balanco.EnergiaRecebidaExt +
		resumo->balanco.EnergiaRecebidaInt));
	// "Inj.GD(3)" => energia gerador
	Item->SubItems->Add(FormataEnergia(resumo->balanco.EnergiaGerador));
	// "Injetada(1+2+3)" => energia dos suprimentos + recebida + gerador
	Item->SubItems->Add(FormataEnergia(injetada));
	// "%(EI)"
	Item->SubItems->Add(FormataPorcentagem(injetada, total->balanco.EnergiaEntrada));
	// perda
	Item->SubItems->Add(FormataEnergia(resumo->balanco.EnergiaPerdaTotal));
	// IPTS
	Item->SubItems->Add(FormataPorcentagem(resumo->balanco.EnergiaPerdaTotal, injetada));
	// PPT
	Item->SubItems->Add(FormataPorcentagem(resumo->balanco.EnergiaPerdaTotal,
		total->balanco.EnergiaEntrada));
	// "For.Consumidor(4)" => consumida
	consumida = resumo->balanco.EnergiaCarga;
	Item->SubItems->Add(FormataEnergia(consumida));
	// "For.Jusante(5)" => entregue
	entregue = resumo->balanco.EnergiaEntregueExt + resumo->balanco.EnergiaEntregueInt;
	Item->SubItems->Add(FormataEnergia(entregue));
	// "For.Externa(6)" => externa (n�o existe no Sinap)
	Item->SubItems->Add(FormataEnergia(0.));
	// "Fornecida(4+5+6) = consumida + entregue + externa
	fornecida = consumida + entregue;
	Item->SubItems->Add(FormataEnergia(fornecida));
	// fornecida / injetada total
	Item->SubItems->Add(FormataPorcentagem(fornecida, total->balanco.EnergiaEntrada));
	// chamada recursiva para exibir os segmentos filhos
	for (int n = 0; n < lisSEGMENTO->Count; n++)
	{
		segmento_filho = (TSegmento*)lisSEGMENTO->Items[n];
		// insere ListItem
		LViewAddItemSegmento(LView, segmento_filho);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormPerda::LViewColumnClick(TObject *Sender, TListColumn *Column)
{
	// verifica se selecionou a mesma coluna
	if ((LViewSort.lview == Sender) && (LViewSort.column == Column))
	{ // inverte ordem
		LViewSort.ascendente = !LViewSort.ascendente;
	}
	else
	{ // redefine �ndice da coluna usada p/ ordena��o
		LViewSort.lview = (TListView*)Sender;
		LViewSort.column = Column;
		LViewSort.ascendente = true;
	}

	// for�a reordena��o
	LViewSort.lview->AlphaSort();
}

// ---------------------------------------------------------------------------
void __fastcall TFormPerda::LViewCompare(TObject *Sender, TListItem *Item1, TListItem *Item2,
	int Data, int &Compare)
{
	// vari�veis locais
	int index = LViewSort.column->Index - 1;
	double valor1, valor2;

	// verifica se Item1 refere-se a uma alinha de total
	if (Item1->Caption.CompareIC("total") == 0)
	{ // mant�m Item1 no final do ListView
		Compare = 1;
		return;
	}
	// verifica se Item2 refere-se a uma linha de total
	if (Item2->Caption.CompareIC("total") == 0)
	{ // mant�m Item2 no final do ListView
		Compare = -1;
		return;
	}
	// verifica Coluna sendo ordenada
	if (LViewSort.column->Index == 0)
	{ // compara Caption
		Compare = CompareText(Item1->Caption, Item2->Caption);
	}
	// verifica Coluna possui string (Tag � definido na edi��o do ListView)
	else if (LViewSort.column->Tag == 0)
	{ // compara��o de colunas com string
		Compare = CompareText(Item1->SubItems->Strings[index], Item2->SubItems->Strings[index]);
	}
	else
	{ // compara��o de colunas com valor num�rico
		valor1 = StrToDouble(FiltraSeparador(Item1->SubItems->Strings[index]));
		valor2 = StrToDouble(FiltraSeparador(Item2->SubItems->Strings[index]));
		if (valor1 < valor2)
			Compare = -1;
		else if (valor1 > valor2)
			Compare = 1;
		else
			Compare = 0;
	}
	// verifica se ordem ascendente/descendente
	if (!LViewSort.ascendente)
		Compare = -Compare;
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/16127109
void __fastcall TFormPerda::LViewRedeBalancoInicia(TList *lisRESUMO)
{
	// vari�veis locais
	VTResumo *resumo;
	TListView *LView = LViewRedeBalanco;

    //insere lista de resumos no relRede
	relRedeBalanco->InsereLisResumo(lisRESUMO, agregada);
	// reinicia LView
	LView->Visible = false;
	LView->Items->Clear();
	// preenche LView
	for (int n = 0; n < lisRESUMO->Count; n++)
	{
		resumo = (VTResumo*)lisRESUMO->Items[n];
		LViewAddItemBalanco(LView, resumo);
		// prote��o contra n�mero muito grande de Resumos
		if (n >= 2000)
		{
			Aviso("Relat�rio truncado nos primeiros 2000 itens. Salve o relat�rio para obter todas as informa��es.");
			break;
		}
	}
	LView->Visible = true;
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/16127109
void __fastcall TFormPerda::LViewRedeEqptoInicia(TList *lisRESUMO)
{
	// vari�veis locais
	VTResumo *parcial;
	TListView *LView = LViewRedeEqpto;

    //insere lista de resumos no relRede
	relRedeEqpto->InsereLisResumo(lisRESUMO, agregada);
	// reinicia LView
	LView->Visible = false;
	LView->Items->Clear();
	// preenche LView
	for (int n = 0; n < lisRESUMO->Count; n++)
	{
		parcial = (VTResumo*)lisRESUMO->Items[n];
		LViewAddItemEqpto(LView, parcial);
		// prote��o contra n�mero muito grande de Resumos
		if (n >= 2000)
		{
			Aviso("Relat�rio truncado nos primeiros 2000 itens. Salve o relat�rio para obter todas as informa��es.");
			break;
		}
	}
	LView->Visible = true;
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/7975908
void __fastcall TFormPerda::LViewRedePerdaInicia(TList *lisRESUMO)
{
	// vari�veis locais
	VTResumo *resumo;
	TListView *LView = LViewRedePerda;

	//insere lista de resumos no relRede
	relRedePerda->InsereLisResumo(lisRESUMO, agregada);
	// reinicia LView
	LView->Visible = false;
	LView->Items->Clear();
	// preenche LView
	for (int n = 0; n < lisRESUMO->Count; n++)
	{
		resumo = (VTResumo*)lisRESUMO->Items[n];
		LViewAddItemPerda(LView, resumo);
		// prote��o contra n�mero muito grande de Resumos
		if (n >= 2000)
		{
			Aviso("Relat�rio truncado nos primeiros 2000 itens. Salve o relat�rio para obter todas as informa��es.");
			break;
		}
	}
	LView->Visible = true;
}

// ---------------------------------------------------------------------------
void __fastcall TFormPerda::MontaRelatorioBalanco(TList *lisRESUMO)
{
	// inicia LViewRedeBalanco
	LViewRedeBalancoInicia(lisRESUMO);
	// exibe GBoxRedeBalanco
	GBoxRedeBalanco->Visible = true;
	GBoxRedeBalanco->Align = alClient;
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/16127109
void __fastcall TFormPerda::MontaRelatorioBalancoAgregado(TList *lisRESUMO)
{
	// vari�veis locais
	VTResumo *resumo_rede;
	VTNode *node;
	TListView *LView = LViewAgregadoBalanco;
	VTArvore *arvore = (VTArvore*)apl->GetObject(__classid(VTArvore));
	VTResumos *resumos = (VTResumos*)apl->GetObject(__classid(VTResumos));

    //insere lista de resumos no relRede
	relBalAgregado->InsereLisResumo(lisRESUMO, agregada);
	// reinicia LView
	LView->Visible = false;
	LView->Items->Clear();
	// loop p/ todos Resumos
	for (int n = 0; n < lisRESUMO->Count; n++)
	{
		resumo_rede = (VTResumo*)lisRESUMO->Items[n];
		// verifica se a Rede pertence a um grupo de Redes
		if ((node = arvore->ExisteNode(resumo_rede)) == NULL)
			continue;
		if (node->LisObjectNode()->Count > 1)
			continue;
		// monta lista com o Resumo e seus Resumos a jusante
		lisTMP->Clear();
		lisTMP->Add(resumo_rede);
		resumos->LisResumoJusante((VTNode*)(resumo_rede->object), lisTMP);
		// monta Agregada com o Resumo e resumos a jusante
		resumo_agregado->ZeraValores();
		resumo_agregado->Agrega(lisTMP);
		// zera energia recebida e entregue do Resumo agreagado
		resumo_agregado->balanco.EnergiaEntrada = 0.;
		resumo_agregado->balanco.EnergiaEntregueExt = 0.;
		resumo_agregado->balanco.EnergiaEntregueInt = 0.;
		// insere ListItem
		LViewAddItemBalanco(LView, resumo_rede, resumo_agregado);
		if (n >= 2000)
		{
			Aviso("Relat�rio truncado nos primeiros 2000 itens. Salve o relat�rio para obter todas as informa��es.");
			break;
		}
	}
	// exibe GBoxAgregadoBalanco e LView
	GBoxAgregadoBalanco->Visible = true;
	GBoxAgregadoBalanco->Align = alClient;
	LView->Visible = true;
}

// ---------------------------------------------------------------------------
void __fastcall TFormPerda::MontaRelatorioEqpto(TList *lisRESUMO)
{
	// inicia ListViewRedeEqpto
	LViewRedeEqptoInicia(lisRESUMO);
	// exibe GBoxRedeEqpto
	GBoxRedeEqpto->Visible = true;
	GBoxRedeEqpto->Align = alClient;
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/16127109
void __fastcall TFormPerda::MontaRelatorioEqptoAgregado(TList *lisRESUMO)
{
	// vari�veis locais
	VTResumo *resumo;
	TListView *LView = LViewAgregadoEqpto;
	VTResumos *resumos = (VTResumos*)apl->GetObject(__classid(VTResumos));

	//insere lista de resumos no relRede
	relEqptoAgregado->InsereLisResumo(lisRESUMO, agregada);
	// reinicia LView
	LView->Visible = false;
	LView->Items->Clear();
	// loop p/ todos Resumos
	for (int n = 0; n < lisRESUMO->Count; n++)
	{
		resumo = (VTResumo*)lisRESUMO->Items[n];
		// monta lista com Resumo e seus Resumos a jusante
		lisTMP->Clear();
		lisTMP->Add(resumo);
		resumos->LisResumoJusante((VTNode*)(resumo->object), lisTMP);
		// monta Agregada com o Resumo e resumos a jusante
		resumo_agregado->ZeraValores();
		resumo_agregado->Agrega(lisTMP);
		// insere ListItem
		LViewAddItemEqpto(LView, resumo, resumo_agregado);
		// prote��o contra n�mero muito grande de Resumos
		if (n >= 2000)
		{
			Aviso("Relat�rio truncado nos primeiros 2000 itens. Salve o relat�rio para obter todas as informa��es.");
			break;
		}
	}
	// exibe GBoxRedeEqpto e LView
	GBoxAgregadoEqpto->Visible = true;
	GBoxAgregadoEqpto->Align = alClient;
	LView->Visible = true;
}

// ---------------------------------------------------------------------------
void __fastcall TFormPerda::MontaRelatorioPerda(TList *lisRESUMO)
{
	// inicia ListViewRedeEqpto
	LViewRedePerdaInicia(lisRESUMO);
	// exibe GBoxRedePerda
	GBoxRedePerda->Visible = true;
	GBoxRedePerda->Align = alClient;
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/16127109
void __fastcall TFormPerda::MontaRelatorioPerdaAgregado(TList *lisRESUMO)
{
	// vari�veis locais
	VTResumo *resumo;
	TListView *LView = LViewAgregadoPerda;
	VTResumos *resumos = (VTResumos*)apl->GetObject(__classid(VTResumos));

    //insere lista de resumos no relPerdaAgregado
	relPerdaAgregado->InsereLisResumo(lisRESUMO, agregada);
	// reinicia LView
	LView->Visible = false;
	LView->Items->Clear();
	// loop p/ todos Resumos
	for (int n = 0; n < lisRESUMO->Count; n++)
	{
		resumo = (VTResumo*)lisRESUMO->Items[n];
		// monta lista com Resumo e seus Resumos a jusante
		lisTMP->Clear();
		lisTMP->Add(resumo);
		resumos->LisResumoJusante((VTNode*)(resumo->object), lisTMP);
		// monta Agregada com o Resumo e resumos a jusante
		resumo_agregado->ZeraValores();
		resumo_agregado->Agrega(lisTMP);
		// insere ListItem
		LViewAddItemPerda(LView, resumo, resumo_agregado);
		if (n >= 2000)
		{
			Aviso("Relat�rio truncado nos primeiros 2000 itens. Salve o relat�rio para obter todas as informa��es.");
			break;
		}
	}
	// exibe GBoxAgregadoPerda e LView
	GBoxAgregadoPerda->Visible = true;
	GBoxAgregadoPerda->Align = alClient;
	LView->Visible = true;
}
/*
 //---------------------------------------------------------------------------
 void __fastcall TFormPerda::MontaRelatorioPerdaSegmento(void)
 {
 //vari�veis locais
 double    injetada, perda;
 TListItem *Item;
 TListView *LView        = LViewPerdaSegmento;
 VTResumo  *resumo_total = agregada->ResumoTotal();
 VTResumo  *resumo_etd;
 VTResumo  *resumo_pri;
 VTResumo  *resumo_sec;

 //reinicia LView
 LView->Visible = false;
 LView->Items->Clear();
 //prote��o
 if (resumo_total == NULL) return;
 //preenche se existir resumo_etd //DVK 2014.02.27
 if ((resumo_etd = MontaResumoAgregado(redeETD)) != NULL)
 {//energia injetada
 injetada = resumo_etd->balanco.EnergiaSuprimento + resumo_etd->balanco.EnergiaRecebidaExt;
 //========================
 //segmento: subesta��o
 Item          = LView->Items->Add();
 Item->Caption = "SED: trafo";
 Item->SubItems->Add(FormataEnergia(injetada));
 //perda pr�pria
 perda = resumo_etd->perda.PerdaTrafoCobre + resumo_etd->perda.PerdaTrafoFerro;
 Item->SubItems->Add(FormataEnergia(perda));
 //IPTS
 Item->SubItems->Add(FormataPorcentagem(perda, injetada));
 //PPT
 Item->SubItems->Add(FormataPorcentagem(perda, resumo_total->balanco.EnergiaEntrada));
 }
 //preenche se existir resumo_pri //DVK 2014.02.27
 if ((resumo_pri = MontaResumoAgregado(redePRI)) != NULL)
 {
 //========================
 //segmento: rede prim�ria com perdas outras
 Item          = LView->Items->Add();
 Item->Caption = "SDMT(perdas adicionais)";
 //energia injetada
 injetada = resumo_pri->balanco.EnergiaRecebidaExt;
 Item->SubItems->Add(FormataEnergia(injetada));
 //perdas outras
 perda = resumo_pri->perda.PerdaOutros;
 Item->SubItems->Add(FormataEnergia(perda));
 //IPTS
 Item->SubItems->Add(FormataPorcentagem(perda, injetada));
 //PPT
 Item->SubItems->Add(FormataPorcentagem(perda, resumo_total->balanco.EnergiaEntrada));
 //========================
 //segmento: rede prim�ria / cabo
 Item          = LView->Items->Add();
 Item->Caption = "SDMT: cabo";
 injetada      = resumo_pri->balanco.EnergiaRecebidaExt - resumo_pri->perda.PerdaOutros;
 Item->SubItems->Add(FormataEnergia(injetada));
 //perda no cabo
 perda = resumo_pri->perda.PerdaCabo;
 Item->SubItems->Add(FormataEnergia(perda));
 //IPTS
 Item->SubItems->Add(FormataPorcentagem(perda, injetada));
 //PPT
 Item->SubItems->Add(FormataPorcentagem(perda, resumo_total->balanco.EnergiaEntrada));
 //========================
 //segmento: rede prim�ria / medidor
 Item          = LView->Items->Add();
 Item->Caption = "SDMT: medidor";
 //energia injetada (corresponde � energia das Cargas)
 injetada = resumo_pri->balanco.EnergiaCarga;
 Item->SubItems->Add(FormataEnergia(injetada));
 //perda no medidor
 perda = resumo_pri->perda.PerdaMedidor;
 Item->SubItems->Add(FormataEnergia(perda));
 //IPTS
 Item->SubItems->Add(FormataPorcentagem(perda, injetada));
 //PPT
 Item->SubItems->Add(FormataPorcentagem(perda, resumo_total->balanco.EnergiaEntrada));
 }
 //========================
 //preenche se existir resumo_sec //DVK 2014.02.27
 if ((resumo_sec = MontaResumoAgregado(redeSEC)) != NULL)
 {
 //========================
 //segmento: rede secund�ria com perdas outras
 Item          = LView->Items->Add();
 Item->Caption = "SDBT(perdas adicionais)";
 //energia injetada
 injetada = resumo_sec->balanco.EnergiaRecebidaExt;
 Item->SubItems->Add(FormataEnergia(injetada));
 //perdas outras
 perda = resumo_pri->perda.PerdaOutros;
 Item->SubItems->Add(FormataEnergia(perda));
 //IPTS
 Item->SubItems->Add(FormataPorcentagem(perda, injetada));
 //PPT
 Item->SubItems->Add(FormataPorcentagem(perda, resumo_total->balanco.EnergiaEntrada));
 //========================
 //segmento: rede secund�ria / trafo
 Item          = LView->Items->Add();
 Item->Caption = "SDBT: trafo";
 //energia injetada
 injetada = resumo_sec->balanco.EnergiaRecebidaExt - resumo_pri->perda.PerdaOutros;
 Item->SubItems->Add(FormataEnergia(injetada));
 //perda no trafo
 perda = resumo_sec->perda.PerdaTrafoCobre + resumo_sec->perda.PerdaTrafoFerro;
 Item->SubItems->Add(FormataEnergia(perda));
 //IPTS
 Item->SubItems->Add(FormataPorcentagem(perda, injetada));
 //PPT
 Item->SubItems->Add(FormataPorcentagem(perda, resumo_total->balanco.EnergiaEntrada));
 //========================
 //segmento: rede secund�ria / cabo
 Item          = LView->Items->Add();
 Item->Caption = "SDBT: cabo";
 //energia injetada
 injetada = (resumo_sec->balanco.EnergiaRecebidaExt  - resumo_pri->perda.PerdaOutros) -
 (resumo_sec->perda.PerdaTrafoCobre + resumo_sec->perda.PerdaTrafoFerro);
 Item->SubItems->Add(FormataEnergia(injetada));
 //perda no cabo
 perda = resumo_sec->perda.PerdaCabo;
 Item->SubItems->Add(FormataEnergia(perda));
 //IPTS
 Item->SubItems->Add(FormataPorcentagem(perda, injetada));
 //PPT
 Item->SubItems->Add(FormataPorcentagem(perda, resumo_total->balanco.EnergiaEntrada));
 //========================
 //segmento: rede secund�ria / ramal
 Item          = LView->Items->Add();
 Item->Caption = "SDBT: ramal";
 //energia injetada
 injetada = (resumo_sec->balanco.EnergiaRecebidaExt  - resumo_pri->perda.PerdaOutros) -
 (resumo_sec->perda.PerdaTrafoCobre + resumo_sec->perda.PerdaTrafoFerro + resumo_sec->perda.PerdaCabo);
 Item->SubItems->Add(FormataEnergia(injetada));
 //perda no ramal
 perda = resumo_sec->perda.PerdaRamal;
 Item->SubItems->Add(FormataEnergia(perda));
 //IPTS
 Item->SubItems->Add(FormataPorcentagem(perda, injetada));
 //PPT
 Item->SubItems->Add(FormataPorcentagem(perda, resumo_total->balanco.EnergiaEntrada));
 //========================
 //segmento: rede secund�ria / medidor
 Item          = LView->Items->Add();
 Item->Caption = "SDBT: medidor";
 //energia injetada
 injetada = (resumo_sec->balanco.EnergiaRecebidaExt  - resumo_pri->perda.PerdaOutros) -
 (resumo_sec->perda.PerdaTrafoCobre + resumo_sec->perda.PerdaTrafoFerro) -
 (resumo_sec->perda.PerdaCabo      + resumo_sec->perda.PerdaRamal);
 Item->SubItems->Add(FormataEnergia(injetada));
 //perda no medidor
 perda = resumo_sec->perda.PerdaMedidor;
 Item->SubItems->Add(FormataEnergia(perda));
 //IPTS
 Item->SubItems->Add(FormataPorcentagem(perda, injetada));
 //PPT
 Item->SubItems->Add(FormataPorcentagem(perda, resumo_total->balanco.EnergiaEntrada));
 }
 //========================
 //total: perdas e PPT
 Item          = LView->Items->Add();
 Item->Caption = "Total";
 //energia injetada
 Item->SubItems->Add("-");
 //perda
 perda = resumo_total->balanco.EnergiaPerdaTotal;
 Item->SubItems->Add(FormataEnergia(perda));
 //IPTS
 Item->SubItems->Add("-");
 //PPT
 Item->SubItems->Add(FormataPorcentagem(perda, resumo_total->balanco.EnergiaEntrada));
 //========================
 //segmento: perdas adcionais
 Item          = LView->Items->Add();
 Item          = LView->Items->Add();
 Item->Caption = "Perdas Adicionais";
 //energia injetada
 Item->SubItems->Add("-");
 //perda outras
 perda = resumo_total->perda.PerdaOutros;
 Item->SubItems->Add(FormataEnergia(perda));
 //IPTS
 Item->SubItems->Add("-");
 //PPT
 Item->SubItems->Add(FormataPorcentagem(perda, resumo_total->balanco.EnergiaEntrada));
 //========================
 GBoxPerdaSegmento->Visible = true;
 GBoxPerdaSegmento->Align   = alClient;
 LView->Visible             = true;
 }
 */

// ---------------------------------------------------------------------------
void __fastcall TFormPerda::MontaRelatorioPerdaSegmento(void)
{
	// vari�veis locais
	VTResumos *resumos = (VTResumos*)apl->GetObject(__classid(VTResumos));

	MontaRelatorioPerdaSegmento(resumos->LisResumo());
}

// ---------------------------------------------------------------------------
void __fastcall TFormPerda::MontaRelatorioPerdaSegmento(TList *lisRESUMO)
{
	// vari�veis locais
	double injetada, perda;
	struct
	{
		double perda_total;
		double perda_adicional;
	} somatoria;
	TListItem *Item;
	TListView *LView = LViewPerdaSegmento;
	VTResumo *resumo_total = agregada->ResumoTotal();
	VTResumo *resumo_etd;
	VTResumo *resumo_pri;
	VTResumo *resumo_sec;

	// reinicia LView
	LView->Visible = false;
	LView->Items->Clear();
	// prote��o
	if (lisRESUMO->Count == 0)
		return;
	// prote��o
	if (resumo_total == NULL)
		return;
	// inicia valores que ser�o totalizados
	somatoria.perda_total = 0;
	somatoria.perda_adicional = 0;
	// preenche se existir resumo_etd //DVK 2014.02.27
	if ((resumo_etd = MontaResumoAgregado(lisRESUMO, redeETD)) != NULL)
	{ // energia injetada
		// injetada = resumo_etd->balanco.EnergiaSuprimento + resumo_etd->balanco.EnergiaRecebidaExt;
		injetada = resumo_etd->balanco.EnergiaEntrada;
		// ========================
		// segmento: subesta��o
		Item = LView->Items->Add();
		Item->Caption = "SED: trafo";
		Item->SubItems->Add(FormataEnergia(injetada));
		// perda pr�pria
		perda = resumo_etd->perda.PerdaTrafoCobre + resumo_etd->perda.PerdaTrafoFerro;
		Item->SubItems->Add(FormataEnergia(perda));
		// IPTS
		Item->SubItems->Add(FormataPorcentagem(perda, injetada));
		// PPT
		Item->SubItems->Add(FormataPorcentagem(perda, resumo_total->balanco.EnergiaEntrada));
		// totaliza valores para os 3 segmentos
		somatoria.perda_total = resumo_etd->balanco.EnergiaPerdaTotal;
		somatoria.perda_adicional = resumo_etd->perda.PerdaOutros;
	}
	// preenche se existir resumo_pri //DVK 2014.02.27
	if ((resumo_pri = MontaResumoAgregado(lisRESUMO, redePRI)) != NULL)
	{
		// ========================
		// segmento: rede prim�ria com perdas outras
		Item = LView->Items->Add();
		Item->Caption = "SDMT(perdas adicionais)";
		// energia injetada
		// injetada = resumo_pri->balanco.EnergiaRecebidaExt;
		injetada = resumo_pri->balanco.EnergiaEntrada;
		Item->SubItems->Add(FormataEnergia(injetada));
		// perdas outras
		perda = resumo_pri->perda.PerdaOutros;
		Item->SubItems->Add(FormataEnergia(perda));
		// IPTS
		Item->SubItems->Add(FormataPorcentagem(perda, injetada));
		// PPT
		Item->SubItems->Add(FormataPorcentagem(perda, resumo_total->balanco.EnergiaEntrada));
		// ========================
		// segmento: rede prim�ria / cabo
		Item = LView->Items->Add();
		Item->Caption = "SDMT: cabo";
		// injetada      = resumo_pri->balanco.EnergiaRecebidaExt - resumo_pri->perda.PerdaOutros;
		injetada = resumo_pri->balanco.EnergiaEntrada - resumo_pri->perda.PerdaOutros;
		Item->SubItems->Add(FormataEnergia(injetada));
		// perda no cabo
		perda = resumo_pri->perda.PerdaCabo;
		Item->SubItems->Add(FormataEnergia(perda));
		// IPTS
		Item->SubItems->Add(FormataPorcentagem(perda, injetada));
		// PPT
		Item->SubItems->Add(FormataPorcentagem(perda, resumo_total->balanco.EnergiaEntrada));
		// ========================
		// segmento: rede prim�ria / medidor
		Item = LView->Items->Add();
		Item->Caption = "SDMT: medidor";
		// energia injetada (corresponde � energia das Cargas)
		injetada = resumo_pri->balanco.EnergiaCarga;
		Item->SubItems->Add(FormataEnergia(injetada));
		// perda no medidor
		perda = resumo_pri->perda.PerdaMedidor;
		Item->SubItems->Add(FormataEnergia(perda));
		// IPTS
		Item->SubItems->Add(FormataPorcentagem(perda, injetada));
		// PPT
		Item->SubItems->Add(FormataPorcentagem(perda, resumo_total->balanco.EnergiaEntrada));
		// totaliza valores para os 3 segmentos
		somatoria.perda_total += resumo_pri->balanco.EnergiaPerdaTotal;
		somatoria.perda_adicional += resumo_pri->perda.PerdaOutros;
	}
	// ========================
	// preenche se existir resumo_sec //DVK 2014.02.27
	if ((resumo_sec = MontaResumoAgregado(lisRESUMO, redeSEC)) != NULL)
	{
		// ========================
		// segmento: rede secund�ria com perdas outras
		Item = LView->Items->Add();
		Item->Caption = "SDBT(perdas adicionais)";
		// energia injetada
		// injetada = resumo_sec->balanco.EnergiaRecebidaExt;
		injetada = resumo_sec->balanco.EnergiaEntrada;
		Item->SubItems->Add(FormataEnergia(injetada));
		// perdas outras
		perda = resumo_sec->perda.PerdaOutros;
		Item->SubItems->Add(FormataEnergia(perda));
		// IPTS
		Item->SubItems->Add(FormataPorcentagem(perda, injetada));
		// PPT
		Item->SubItems->Add(FormataPorcentagem(perda, resumo_total->balanco.EnergiaEntrada));
		// ========================
		// segmento: rede secund�ria / trafo
		Item = LView->Items->Add();
		Item->Caption = "SDBT: trafo";
		// energia injetada
		// injetada = resumo_sec->balanco.EnergiaRecebidaExt - resumo_sec->perda.PerdaOutros;
		injetada = resumo_sec->balanco.EnergiaEntrada - resumo_sec->perda.PerdaOutros;
		Item->SubItems->Add(FormataEnergia(injetada));
		// perda no trafo
		perda = resumo_sec->perda.PerdaTrafoCobre + resumo_sec->perda.PerdaTrafoFerro;
		Item->SubItems->Add(FormataEnergia(perda));
		// IPTS
		Item->SubItems->Add(FormataPorcentagem(perda, injetada));
		// PPT
		Item->SubItems->Add(FormataPorcentagem(perda, resumo_total->balanco.EnergiaEntrada));
		// ========================
		// segmento: rede secund�ria / cabo
		Item = LView->Items->Add();
		Item->Caption = "SDBT: cabo";
		// energia injetada
		// injetada = (resumo_sec->balanco.EnergiaRecebidaExt  - resumo_sec->perda.PerdaOutros) -
		// (resumo_sec->perda.PerdaTrafoCobre + resumo_sec->perda.PerdaTrafoFerro);
		injetada = (resumo_sec->balanco.EnergiaEntrada - resumo_sec->perda.PerdaOutros) -
			(resumo_sec->perda.PerdaTrafoCobre + resumo_sec->perda.PerdaTrafoFerro);
		Item->SubItems->Add(FormataEnergia(injetada));
		// perda no cabo
		perda = resumo_sec->perda.PerdaCabo;
		Item->SubItems->Add(FormataEnergia(perda));
		// IPTS
		Item->SubItems->Add(FormataPorcentagem(perda, injetada));
		// PPT
		Item->SubItems->Add(FormataPorcentagem(perda, resumo_total->balanco.EnergiaEntrada));
		// ========================
		// segmento: rede secund�ria / ramal
		Item = LView->Items->Add();
		Item->Caption = "SDBT: ramal";
		// energia injetada
		// injetada = (resumo_sec->balanco.EnergiaRecebidaExt  - resumo_sec->perda.PerdaOutros) -
		// (resumo_sec->perda.PerdaTrafoCobre + resumo_sec->perda.PerdaTrafoFerro + resumo_sec->perda.PerdaCabo);
		injetada = (resumo_sec->balanco.EnergiaEntrada - resumo_sec->perda.PerdaOutros) -
			(resumo_sec->perda.PerdaTrafoCobre + resumo_sec->perda.PerdaTrafoFerro +
			resumo_sec->perda.PerdaCabo);
		Item->SubItems->Add(FormataEnergia(injetada));
		// perda no ramal
		perda = resumo_sec->perda.PerdaRamal;
		Item->SubItems->Add(FormataEnergia(perda));
		// IPTS
		Item->SubItems->Add(FormataPorcentagem(perda, injetada));
		// PPT
		Item->SubItems->Add(FormataPorcentagem(perda, resumo_total->balanco.EnergiaEntrada));
		// ========================
		// segmento: rede secund�ria / medidor
		Item = LView->Items->Add();
		Item->Caption = "SDBT: medidor";
		// energia injetada
		// injetada = (resumo_sec->balanco.EnergiaRecebidaExt  - resumo_sec->perda.PerdaOutros) -
		// (resumo_sec->perda.PerdaTrafoCobre + resumo_sec->perda.PerdaTrafoFerro) -
		// (resumo_sec->perda.PerdaCabo      + resumo_sec->perda.PerdaRamal);
		injetada = (resumo_sec->balanco.EnergiaEntrada - resumo_sec->perda.PerdaOutros) -
			(resumo_sec->perda.PerdaTrafoCobre + resumo_sec->perda.PerdaTrafoFerro) -
			(resumo_sec->perda.PerdaCabo + resumo_sec->perda.PerdaRamal);
		Item->SubItems->Add(FormataEnergia(injetada));
		// perda no medidor
		perda = resumo_sec->perda.PerdaMedidor;
		Item->SubItems->Add(FormataEnergia(perda));
		// IPTS
		Item->SubItems->Add(FormataPorcentagem(perda, injetada));
		// PPT
		Item->SubItems->Add(FormataPorcentagem(perda, resumo_total->balanco.EnergiaEntrada));
		// totaliza valores para os 3 segmentos
		somatoria.perda_total += resumo_sec->balanco.EnergiaPerdaTotal;
		somatoria.perda_adicional += resumo_sec->perda.PerdaOutros;
	}
	// ========================
	// total: perdas e PPT
	Item = LView->Items->Add();
	Item->Caption = "Total";
	// energia injetada
	Item->SubItems->Add("-");
	Item->SubItems->Add(FormataEnergia(somatoria.perda_total));
	// IPTS
	Item->SubItems->Add("-");
	// PPT
	Item->SubItems->Add(FormataPorcentagem(somatoria.perda_total,
		resumo_total->balanco.EnergiaEntrada));
	// ========================
	// segmento: perdas adcionais
	Item = LView->Items->Add();
	Item = LView->Items->Add();
	Item->Caption = "Perdas Adicionais";
	// energia injetada
	Item->SubItems->Add("-");
	Item->SubItems->Add(FormataEnergia(somatoria.perda_adicional));
	// IPTS
	Item->SubItems->Add("-");
	// PPT
	Item->SubItems->Add(FormataPorcentagem(somatoria.perda_adicional,
		resumo_total->balanco.EnergiaEntrada));
	// ========================
	GBoxPerdaSegmento->Visible = true;
	GBoxPerdaSegmento->Align = alClient;
	LView->Visible = true;
}

// ---------------------------------------------------------------------------
void __fastcall TFormPerda::MontaRelatorioResumo(void)
{
	// vari�veis locais
	TListItem *Item;
	TListView *LView = LViewResumo;
	VTResumo *total = agregada->ResumoTotal();

	// monta LViewResumo
	LView->Items->Clear();
	// energia de entrada
	Item = LView->Items->Add();
	Item->Caption = "Injetada (EI*)";
	Item->SubItems->Add(FormataEnergia(total->balanco.EnergiaEntrada));
	Item->SubItems->Add(FormataPorcentagem(total->balanco.EnergiaEntrada,
		total->balanco.EnergiaEntrada));
	// energia consumida
	Item = LView->Items->Add();
	Item->Caption = "Fornecida (EF*)";
	Item->SubItems->Add(FormataEnergia(total->balanco.EnergiaCarga));
	Item->SubItems->Add(FormataPorcentagem(total->balanco.EnergiaCarga,
		total->balanco.EnergiaEntrada));
	// perdas
	Item = LView->Items->Add();
	Item->Caption = "Perda (PT*)";
	Item->SubItems->Add(FormataEnergia(total->balanco.EnergiaPerdaTotal));
	Item->SubItems->Add(FormataPorcentagem(total->balanco.EnergiaPerdaTotal,
		total->balanco.EnergiaEntrada));
}

// ---------------------------------------------------------------------------
void __fastcall TFormPerda::MontaRelatorioBalancoEmpresa(void)
{
	// vari�veis locais
	TSegmento *segmento;
	TList *lisSEGMENTO = agregada->LisSegmentoRaiz();
	TListView *LView = LViewTotalBalanco;

	// reinicia LView
	LView->Visible = false;
	LView->Items->Clear();
	// prote��o
	if (!agregada->Valida)
		return;
	// loop p/ os Segmentos raiz da �rvores de Segmentos da Agregada
	for (int n = 0; n < lisSEGMENTO->Count; n++)
	{
		segmento = (TSegmento*)lisSEGMENTO->Items[n];
		// insere ListItem
		LViewAddItemSegmento(LView, segmento);
	}
	// exibe LView
	LView->Visible = true;
}

/*
 //---------------------------------------------------------------------------
 VTResumo* __fastcall TFormPerda::MontaResumoAgregado(int tiporede_id)
 {
 //vari�veis locais
 VTResumos  *resumos = (VTResumos*)apl->GetObject(__classid(VTResumos));

 //zera valore de resumo_agregado
 resumo_agregado->ZeraValores();
 //reinicia lisTMP com os Resumos do tiporede indicado
 lisTMP->Clear();
 resumos->LisResumo(tiporede_id, lisTMP);
 if (lisTMP->Count == 0) return(NULL);
 //monta Agregada com o Resumo e resumos a jusante
 resumo_agregado->ZeraValores();
 resumo_agregado->Agrega(lisTMP);
 //zera energia recebida e entregue do Resumo agregado
 resumo_agregado->balanco.EnergiaEntrada     = 0.;
 resumo_agregado->balanco.EnergiaEntregueExt = 0.;
 resumo_agregado->balanco.EnergiaEntregueInt = 0.;

 return(resumo_agregado);
 }
 */
// ---------------------------------------------------------------------------
VTResumo* __fastcall TFormPerda::MontaResumoAgregado(int tiporede_id)
{
	// vari�veis locais
	VTResumos *resumos = (VTResumos*)apl->GetObject(__classid(VTResumos));

	return (MontaResumoAgregado(resumos->LisResumo(), tiporede_id));
}

// ---------------------------------------------------------------------------
VTResumo* __fastcall TFormPerda::MontaResumoAgregado(TList *lisRESUMO, int tiporede_id)
{
	// vari�veis locais
	VTResumos *resumos = (VTResumos*)apl->GetObject(__classid(VTResumos));

	// zera valores de resumo_agregado
	resumo_agregado->ZeraValores();
	// reinicia lisTMP com os Resumos do tiporede indicado
	lisTMP->Clear();
	resumos->SelecionaResumo(lisRESUMO, lisTMP, tiporede_id);
	if (lisTMP->Count == 0)
		return (NULL);
	// monta Agregada com o Resumo das redes do mesmo tipo
	resumo_agregado->ZeraValores();
	resumo_agregado->Agrega(lisTMP);
	// trata casos de resumo agregado pai-filho do mesmo tipo
	resumos->TrataResumoPaiFilho(resumo_agregado, lisTMP);
	// zera energia recebida e entregue do Resumo agregado
	// resumo_agregado->balanco.EnergiaEntrada     = 0.;
	// resumo_agregado->balanco.EnergiaEntregueExt = 0.;
	// resumo_agregado->balanco.EnergiaEntregueInt = 0.;

	return (resumo_agregado);
}

// ---------------------------------------------------------------------------
void __fastcall TFormPerda::RichEditLocalizaKeyPress(TObject *Sender, char &Key)
{
	// verifica se a tecla � CR
	if (Key == VK_RETURN)
	{ // executa ActionLoaliza
		ActionLocaliza->Execute();
		// redefine Key p/ que nenhuma a��o seja executada
		Key = VK_SHIFT;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormPerda::SaveDialogTypeChange(TObject *Sender)
{
	// define extens�o default em fun��o do tipo de arquivo
	if (SaveDialog->FilterIndex == 1)
		SaveDialog->DefaultExt = ".txt";
	else
		SaveDialog->DefaultExt = ".xls";
}

// ---------------------------------------------------------------------------
void __fastcall TFormPerda::TViewChange(TObject *Sender, TTreeNode *TreeNode)
{
	// vari�veis locais

	// prote��o
	if ((TreeNode == NULL) || (TreeNode->Data == NULL))
		return;
	// inicia lisRES com os Resumos das redes correspondentes ao relat�rio selecionadao
	lisRES->Clear();
	switch (CBoxRelatorio->ItemIndex)
	{
	case 0: // Equipamentos por SED
		TViewSelectedResumo(lisRES, redeETD);
		MontaRelatorioEqpto(lisRES);
		break;
	case 1: // Equipamentos por SDMT
		TViewSelectedResumo(lisRES, redePRI);
		MontaRelatorioEqpto(lisRES);
		break;
	case 2: // Equipamentos por SDBT
		TViewSelectedResumo(lisRES, redeSEC);
		MontaRelatorioEqpto(lisRES);
		break;
	case 3: // Equipamentos por Rede
		TViewSelectedResumo(lisRES);
		MontaRelatorioEqpto(lisRES);
		break;
	case 4: // Balan�o de Energia por SED
		TViewSelectedResumo(lisRES, redeETD);
		MontaRelatorioBalanco(lisRES);
		break;
	case 5: // Balan�o de Energia por SDMT
		TViewSelectedResumo(lisRES, redePRI);
		MontaRelatorioBalanco(lisRES);
		break;
	case 6: // Balan�o de Energia por SDBT
		TViewSelectedResumo(lisRES, redeSEC);
		MontaRelatorioBalanco(lisRES);
		break;
	case 7: // Balan�o de Energia por Rede
		TViewSelectedResumo(lisRES);
		MontaRelatorioBalanco(lisRES);
		break;
	case 8: // Perdas por SED
		TViewSelectedResumo(lisRES, redeETD);
		MontaRelatorioPerda(lisRES);
		break;
	case 9: // Perdas por SDMT
		TViewSelectedResumo(lisRES, redePRI);
		MontaRelatorioPerda(lisRES);
		break;
	case 10: // Perdas por SDBT
		TViewSelectedResumo(lisRES, redeSEC);
		MontaRelatorioPerda(lisRES);
		break;
	case 11: // Perdas por Rede
		TViewSelectedResumo(lisRES);
		MontaRelatorioPerda(lisRES);
		break;
	case 12: // Equipamentos Agregados por SED
		TViewSelectedResumo(lisRES, redeETD);
		MontaRelatorioEqptoAgregado(lisRES);
		break;
	case 13: // Equipamentos Agregados por SDMT
		TViewSelectedResumo(lisRES, redePRI);
		MontaRelatorioEqptoAgregado(lisRES);
		break;
	case 14: // Equipamentos Agregados por Rede
		TViewSelectedResumo(lisRES);
		MontaRelatorioEqptoAgregado(lisRES);
		break;
	case 15: // Balan�o Agregado de Energia por SED
		TViewSelectedResumo(lisRES, redeETD);
		MontaRelatorioBalancoAgregado(lisRES);
		break;
	case 16: // Balan�o Agregado de Energia por SDMT
		TViewSelectedResumo(lisRES, redePRI);
		MontaRelatorioBalancoAgregado(lisRES);
		break;
	case 17: // Balan�o Agregado de Energia por Rede
		TViewSelectedResumo(lisRES);
		MontaRelatorioBalancoAgregado(lisRES);
		break;
	case 18: // Perdas Agregadas por SED
		TViewSelectedResumo(lisRES, redeETD);
		MontaRelatorioPerdaAgregado(lisRES);
		break;
	case 19: // Perdas Agregadas por SDMT
		TViewSelectedResumo(lisRES, redePRI);
		MontaRelatorioPerdaAgregado(lisRES);
		break;
	case 20: // Perdas Agregadas por Rede
		TViewSelectedResumo(lisRES);
		MontaRelatorioPerdaAgregado(lisRES);
		break;
	case 21: // Perdas Agregadas por Segmento
		TViewSelectedResumo(lisRES);
		MontaRelatorioPerdaSegmento(lisRES);
		break;
	default: // nada a fazer
		break;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormPerda::TViewInicia(int level)
{
	// vari�veis locais
	TTreeNode *treenode;
	VTNode *node;
	VTArvore *arvore = (VTArvore*)apl->GetObject(__classid(VTArvore));
	TList *lisNODE = arvore->LisNodeRaiz();

	// FKM 2015.09.21 - Verifica se � do hist�rico ou do perdatec
	if (Owner->ClassName().CompareIC("TFormHistorico") == 0)
	{
		level = level + 1;
	}
	// FKM 2015.09.21
	// verifica o TView est� iniciado com o mesmo level indicado
	if (tview.last_level == level)
		return;
	// desabilita tratamento do evento OnChange
	TView->OnChange = NULL;
	// reinicia TView
	TView->Items->Clear();
	// loop p/ todos os Nodes em lisNODE
	for (int n = 0; n < lisNODE->Count; n++)
	{
		node = (VTNode*)lisNODE->Items[n];
		// insere o Node em um TreeNode
		treenode = TViewInsereNode(TView, level, node);
		// ordena redes do tipo
		if (treenode != NULL)
			treenode->AlphaSort(true);
	}
	if (TView->Items->Count > 0)
	{
		TView->Selected = TView->Items->Item[0];
		if (TView->Selected)
			TView->Selected->Expand(false);
	}
	// habilita tratamento do evento OnChange
	TView->OnChange = TViewChange;
}

// ---------------------------------------------------------------------------
TTreeNode* __fastcall TFormPerda::TViewInsereNode(TTreeView *TView, int level, VTNode *node,
	TTreeNode *treenode_pai)
{
	// vari�veis locais
	TTreeNode *treenode, *treenode_grp;
	TList *lisNODE;
	TList *lisRESUMO;
	VTResumo *resumo;

	// verifica se foi definido o TreeNode pai
	if (treenode_pai == NULL)
	{ // insere um TreeNode raiz
		treenode = TView->Items->AddObject(NULL, node->Codigo, node);
	}
	else
	{
		treenode = TView->Items->AddChildObject(treenode_pai, node->Codigo, node);
	}
	// verifica se o Node atende � limita��o de Level
	if (node->Level >= level)
		return (treenode);
	// verifica n�mero de Redes do Node
	lisRESUMO = node->LisObjectNode();
	if ((lisRESUMO->Count == 0) || (lisRESUMO->Count == 1))
	{ // insere TreeNodes p/ os Nodes filhos
		lisNODE = node->LisNode();
		for (int n = 0; n < lisNODE->Count; n++)
		{
			node = (VTNode*)lisNODE->Items[n];
			TViewInsereNode(TView, level, node, treenode);
		}
		return (treenode);
	}
	else
	{ // insere TreeNode c/ o t�tulo "Redes do grupo", associado ao Node
		treenode_grp = TView->Items->AddChildObject(treenode, "<Redes do grupo>", node);
		// insere TreeNode p/ cada rede do grupo
		for (int n = 0; n < lisRESUMO->Count; n++)
		{
			resumo = (VTResumo*)lisRESUMO->Items[n];
			// insere TreeNode p/ o Resumo e n�o p/ Node
			TView->Items->AddChildObject(treenode_grp, resumo->rede.Codigo, resumo);
		}
		// determina conjunto de Nodes filhos
		lisNODE = node->LisNode();
		if (lisNODE->Count > 0)
		{ // insere TreeNode c/ o t�tulo "Redes supridas pelo grupo"
			// treenode_subgrp = TView->Items->AddChildObject(treenode_grp, "Redes supridas pelo grupo", node);
			// insere TreeNodes p/ os Nodes filhos
			for (int n = 0; n < lisNODE->Count; n++)
			{
				node = (VTNode*)lisNODE->Items[n];
				TViewInsereNode(TView, level, node, treenode);
			}
		}
		return (treenode);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormPerda::TViewSelectedResumo(TList *lisEXT, int tiporede_id)
{
	// vari�veis locais
	AnsiString class_name;
	TObject *object;
	TTreeNode *treenode;
	VTNode *node;
	VTResumo *resumo;
	VTResumos *resumos = (VTResumos*)apl->GetObject(__classid(VTResumos));

	// insere em lisEXT os Resumos das redes do tipo indicado
	for (int n = 0; n < TView->SelectionCount; n++)
	{
		treenode = TView->Selections[n];
		object = (TObject*)treenode->Data;
		class_name = object->ClassName();
		if (class_name.AnsiCompareIC("TNode") == 0)
		{ // determina lista de Resumos do Node e dos Nodes a jusante
			node = (VTNode*)(treenode->Data);
			resumos->LisResumoNode(node, tiporede_id, lisRES);
			resumos->LisResumoJusante(node, tiporede_id, lisRES);
		}
		else
		{ // determina Resumo associado ao TreeNode
			resumo = (VTResumo*)(treenode->Data);
			// verifica se o Resumo � de uma subestacao
			// if ((tiporede_id < 0)||(resumo->rede.TipoRede == tiporede_id))
			// DVK 2017.05.02 corre��o resumo->rede.TipoRedeId
			if ((tiporede_id < 0) || (resumo->rede.TipoRedeId == tiporede_id))
			{
				if (lisEXT->IndexOf(resumo) < 0)
					lisEXT->Add(resumo);
			}
		}
	}
}

// ---------------------------------------------------------------------------
// eof
