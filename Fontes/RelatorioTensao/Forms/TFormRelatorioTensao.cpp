// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "TFormRelatorioTensao.h"
#include "TFormCfgRelTensao.h"
#include "..\TFaixaTensao.h"
#include "..\TNivelTensao.h"
#include "..\TRelatorioTensao.h"
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Geral\VTGeral.h>
#include <Fontes\Geral\VTInfoset.h>
#include <Fontes\Grafico\VTGrafico.h>
#include <Fontes\Diretorio\VTPath.h>
#include <Fontes\Rede\VTBarra.h>
#include <Fontes\Report\VTReport.h>
#include <DLL_Inc\Funcao.h>
#include <DLL_Inc\Report.h>

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TFormEdt"
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
__fastcall TFormRelatorioTensao::TFormRelatorioTensao(TComponent *Owner, VTApl *apl_owner,
	TWinControl *parent) : TFormEdt(Owner, parent)
{
	// salva ponteiro
	apl = apl_owner;
	// insere Form em seu Parent
	if (parent)
	{
		Parent = parent;
	}
	// cria objeto
	relatorio_tensao = new TRelatorioTensao(apl);
	// posiciona o Form
	FormIniciaPosicao();
	// exibe este form
	Show();
}

// ---------------------------------------------------------------------------
__fastcall TFormRelatorioTensao::~TFormRelatorioTensao(void)
{
	// variaveis locais
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

	// destroi objeto
	DestroiTObject(relatorio_tensao);
	// salva parâmetros do Form
	if (geral)
		geral->Infoset->SaveToFile(this);
}

// ---------------------------------------------------------------------------
void __fastcall TFormRelatorioTensao::ActionConfigurarExecute(TObject *Sender)
{
	// variaveis locais
	TFormCfgRelTensao *form;

	// esconde este form
	Hide();
	try
	{ // cria formCfgTensao
		form = new TFormCfgRelTensao(this, apl, relatorio_tensao);
		if (form->ShowModal() == mrOk)
		{ // atualiza
			relatorio_tensao->Executa();
		}
		delete form;
	}
	catch (...)
	{
		AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(funcao);
		if (form)
			delete form;
	}
	// reapresenta este form
	Show();
}

// ---------------------------------------------------------------------------
void __fastcall TFormRelatorioTensao::ActionExportarExecute(TObject *Sender)
{
	// variaveis locais
	VTReport *report;
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));
	AnsiString arq_csv, titulo;

	// esconde este form
	Hide();
	try
	{ // solicita o local para salvar o arquivo
		if (SaveDialog->InitialDir.IsEmpty())
		{
			SaveDialog->InitialDir = path->DirExporta();
		}
		SaveDialog->FileName = "RelatorioTensao.csv";
		SaveDialog->Filter = "Arquivo CSV (*.csv)|*.csv|";
		// cria report
		report = DLL_NewObjReport(apl);
		if (SaveDialog->Execute())
		{
			arq_csv = SaveDialog->FileName;
			titulo = "Relatório de Tensão na Faixa " + CBoxFaixa->Text;
			// abre tela de progresso
			relatorio_tensao->ProgressoStart();
			if (report->ArquivoTexto(arq_csv, LViewBarra, titulo, false))
			{
				Aviso("Arquivo exportado com sucesso !");
			}
			else
			{
				Aviso("Erro ao exportar arquivo !");
			}
		}
		delete report;
		// fecha tela de progresso
		relatorio_tensao->ProgressoStop();
	}
	catch (...)
	{
		AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(funcao);
		if (report)
			delete report;
	}
	// reapresenta este form
	Show();
}

// ---------------------------------------------------------------------------
void __fastcall TFormRelatorioTensao::ActionFecharExecute(TObject *Sender)
{
	// fecha form
	Close();
}

// ---------------------------------------------------------------------------
void __fastcall TFormRelatorioTensao::CBoxFaixaInicia(void)
{
	// variaveis locais
	TList *lisFAIXA;
	TNivelTensao *nivel = NULL;
	TFaixaTensao *faixa;

	try
	{ // limpa cbox
		CBoxFaixa->Clear();
		// insere vazio
		CBoxFaixa->AddItem("", NULL);
		// verifica nivel de tensão selecionado
		switch (RadioGroupNivel->ItemIndex)
		{
		case 0: // AT
			nivel = relatorio_tensao->nivel_at;
			break;
		case 1: // MT
			nivel = relatorio_tensao->nivel_mt;
			break;
		case 2: // BT
			nivel = relatorio_tensao->nivel_bt;
			break;
		default:
			Aviso("Selecione um nível de tensão");
			break;
		}
		// proteção
		if (nivel == NULL)
			return;
		// insere faixas do nivel
		lisFAIXA = nivel->LisFaixa();
		for (int n = 0; n < lisFAIXA->Count; n++)
		{
			faixa = (TFaixaTensao*)lisFAIXA->Items[n];
			CBoxFaixa->AddItem(faixa->Texto(), faixa);
		}
		// seleciona primeira faixa
		if (CBoxFaixa->ItemIndex < 0)
		{
			CBoxFaixa->ItemIndex = 0;
		}
		CBoxFaixaChange(NULL);
	}
	catch (...)
	{
		AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(funcao);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormRelatorioTensao::CBoxFaixaChange(TObject *Sender)
{
	// variaveis locais
	TFaixaTensao *faixa;

	try
	{ // abre tela de progresso
		relatorio_tensao->ProgressoStart();
		// verifica o item selecionado
		faixa = (TFaixaTensao*)CBoxFaixa->Items->Objects[CBoxFaixa->ItemIndex];
		relatorio_tensao->AtualizaGrafico(faixa);
		// preenche LView com as barras da faixa
		LViewBarraInicia(faixa);
		// fecha tela de progresso
		relatorio_tensao->ProgressoStop();
	}
	catch (...)
	{ // fecha tela de progresso
		relatorio_tensao->ProgressoStop();
		AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(funcao);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormRelatorioTensao::FormClose(TObject *Sender, TCloseAction &Action)
{
	// destroi o form
	Action = caFree;
}

// ---------------------------------------------------------------------------
void __fastcall TFormRelatorioTensao::FormIniciaPosicao(void)
{
	// variáveis locais
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

	try
	{
		if (geral)
		{ // lê parâmetros do Form
			if (geral->Infoset->LoadFromFile(this))
				return;
		}
		// posiciona o Form sem alinhamento
		Left = 0;
		Top = 0;
	}
	catch (...)
	{
		AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(funcao);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormRelatorioTensao::FormShow(TObject *Sender)
{
	try
	{ // verifica se o fluxo foi executado
		if (!relatorio_tensao->FluxoOk())
		{
			Close();
			return;
		}
		relatorio_tensao->Executa();
		// atualiza CBoxFaixa
		CBoxFaixaInicia();
	}
	catch (...)
	{
		AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(funcao);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormRelatorioTensao::LViewBarraInicia(TFaixaTensao *faixa)
{
	// variaveis locais
	VTBarra *barra;
	TList *lisBARRA;
	TListItem *item;
	AnsiString coord;

	try
	{ // limpa lview
		LViewBarra->Clear();
		// proteção
		if (faixa == NULL)
			return;
		// preenche
		lisBARRA = faixa->LisBarra();
		for (int n = 0; n < lisBARRA->Count; n++)
		{
			barra = (VTBarra*)lisBARRA->Items[n];
			item = LViewBarra->Items->Add();
			item->Data = barra;
			// codigo
			item->Caption = barra->Codigo;
			// coordenada x
			coord = DoubleToStr("%3.2f", barra->utm.x / 100);
			item->SubItems->Add(coord);
			// coordenada y
			coord = DoubleToStr("%3.2f", barra->utm.y / 100);
			item->SubItems->Add(coord);
		}
	}
	catch (...)
	{
		AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(funcao);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormRelatorioTensao::RadioGroupNivelClick(TObject *Sender)
{
	try
	{ // atualiza faixas
		CBoxFaixaInicia();
	}
	catch (...)
	{
		AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(funcao);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormRelatorioTensao::LViewBarraSelectItem(TObject *Sender, TListItem *Item,
	bool Selected)
{
	// variaveis locais
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	try
	{ // desenha moldura
		grafico->Moldura((VTBarra*)Item->Data);
	}
	catch (...)
	{
		AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(funcao);
	}
}

// ---------------------------------------------------------------------------
