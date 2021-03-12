// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormRelObraCOPEL.h"
#include "TRelObra.h"
#include "TRelObraTO.h"
// ---------------------------------------------------------------------------
// #include "VTAtivo.h"
// #include "VTBaseAtivo.h"
#include "..\Apl\VTApl.h"
#include "..\Arranjo\VTArranjo.h"
#include "..\Diretorio\VTPath.h"
//#include "..\MultiObra\VTMultiObraBO.h"
//#include "..\MultiObra\VTMultiObra.h"
#include "..\Obra\ItemObra\VTItemObra.h"
#include "..\Obra\ItemObra\VTItemObraCOPEL.h"
#include "..\Obra\VTObra.h"
#include "..\Obra\VTObras.h"
//#include "..\Obra\VTItemObraCOPEL.h"
#include "..\Planejamento\VTAlternativa.h"
#include "..\Planejamento\VTEstudo.h"
#include "..\Planejamento\VTPlanejamento.h"
#include "..\Radial\VTArvore.h"
#include "..\Radial\VTNode.h"
#include "..\Rede\VTCapacitor.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTReator.h"
#include "..\Rede\VTTrafo.h"
#include "..\Rede\VTTrafo3E.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTTipoRede.h"
#include "..\Report\VTReport.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Radial.h"
#include "..\..\DLL_Inc\Report.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
__fastcall TFormRelObraCOPEL::TFormRelObraCOPEL(TComponent* Owner, VTEstudo *estudo,
	VTApl* apl_owner) : TForm(Owner)
{
	VTRedes *redes;

	// salva ponteiro´p/ Estudo
	this->estudo = estudo;
	// salva ponteiro para apl
	apl = apl_owner;
	// determina ano inicial e final do Estudo
	ano_ini = estudo->AnoInicial;
	ano_fim = estudo->AnoFinal;
	//cria o responsavel pela inteligencia
    relObra = new TRelObra(estudo, apl);
	// garante que todas as Alternativas tenham o objeto VTBaseAtivo
	relObra->IniciaAlternativas();

	// inicia CLBoxAlternativa
	CLBoxAlternativaInicia();
	// inicia LView
	LViewInicia();
}

// ---------------------------------------------------------------------------
__fastcall TFormRelObraCOPEL::~TFormRelObraCOPEL(void)
{
	if (relObra != NULL)
	{
		delete relObra;
        relObra = NULL;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormRelObraCOPEL::ActionCancelaExecute(TObject *Sender)
{
	// fecha o Form
	ModalResult = mrCancel;
}

// ---------------------------------------------------------------------------
void __fastcall TFormRelObraCOPEL::ActionConfirmaExecute(TObject *Sender)
{
	// fecha o Form
	ModalResult = mrOk;
}

// ---------------------------------------------------------------------------
void __fastcall TFormRelObraCOPEL::ActionImprimeExecute(TObject *Sender)
{ // DVK 2014.01.06
	// variaveis locais
	AnsiString nomeArquivo;
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));
	VTReport *report;

	// cria objeto report
	report = DLL_NewObjReport(apl);
	// esconde form, para a janela Salvar Como não ficar por baixo
	Hide();
	// define pasta inicial
	SaveDialogExporta->InitialDir = path->DirExporta();
	if (SaveDialogExporta->Execute())
	{ // recebe nome do arquivo a ser exportado e exporta listView
		nomeArquivo = SaveDialogExporta->FileName;
		if (report->ArquivoTexto(nomeArquivo, LView, "", false))
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
VTAlternativa* __fastcall TFormRelObraCOPEL::AlternativaSelecionada(void)
{
	// variáveis locais
	VTAlternativa *alternativa;

	// verifica se há uma Alternativa selecionada
	if (CLBoxAlternativa->ItemIndex < 0)
		return (NULL);
	// determina Alternativa
	alternativa = (VTAlternativa*)CLBoxAlternativa->Items->Objects[CLBoxAlternativa->ItemIndex];

	return (alternativa);
}

// ---------------------------------------------------------------------------
void __fastcall TFormRelObraCOPEL::CLBoxAlternativaClick(TObject *Sender)
{
	// inicia todos os LView
	LViewInicia();
}

// ---------------------------------------------------------------------------
void __fastcall TFormRelObraCOPEL::CLBoxAlternativaClickCheck(TObject *Sender)
{
	// reinicia LView
	LViewInicia();
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/14422294
void __fastcall TFormRelObraCOPEL::CLBoxAlternativaInicia(void)
{
	// variáveis locais
	int index;
	VTAlternativa *alternativa;
	TList *lisALTER = estudo->LisAlternativa();
	TCheckListBox *CLBox = CLBoxAlternativa;

	// reinicia CLBox
	CLBox->Clear();
	// insere Alternativas no CLBox
	for (int n = 0; n < lisALTER->Count; n++)
	{
		alternativa = (VTAlternativa*)lisALTER->Items[n];
		// descarta Alterantiva sem Ativos
//		base_ativo = (VTBaseAtivo*)alternativa->Apl->GetObject(__classid(VTBaseAtivo));
//		if (base_ativo->NumeroDeAtivos() == 0)
//			continue;
		if (alternativa == estudo->AlternativaPadrao) continue;
		// insere um novo item
		index = CLBox->Items->AddObject(alternativa->Codigo, alternativa);
		// marca Alternativa como selecionada (checked)
		CLBox->Checked[index] = alternativa->Valida;
	}
	// pré-seleciona primeira Alternativa
	if (CLBox->Items->Count > 0)
		CLBox->ItemIndex = 0;
}

// ---------------------------------------------------------------------------
void __fastcall TFormRelObraCOPEL::FormClose(TObject *Sender, TCloseAction &Action)
{
	// esconde o Form
	Action = caHide;
}
// ---------------------------------------------------------------------------
void __fastcall TFormRelObraCOPEL::LViewAdvancedCustomDrawItem(TCustomListView *Sender,
		  TListItem *Item, TCustomDrawState State, TCustomDrawStage Stage,
		  bool &DefaultDraw)
{
	VTItemObraCOPEL   *item;

	item = (VTItemObraCOPEL*) Item->Data;

	if (item->Configurado)
	{
		LView->Canvas->Brush->Color = clMoneyGreen;
	}
	else
	{
		LView->Canvas->Brush->Color = clYellow;
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormRelObraCOPEL::LViewInicia(void)
{
	// variáveis locais
	double valor;
	AnsiString txt;
	TListItem *Item;
	TList *lisOBRA;
	TList *lisITEMOBRA;
	VTAlternativa *alternativa;
	VTObra *obra;
	VTItemObraCOPEL *item;
	TRelObraTO *rel = NULL;

	// reinicia LView
	LView->Items->Clear();
	LView->Items->BeginUpdate();
	// loop p/ todas Alternativas selecionadas
	for (int n = 0; n < CLBoxAlternativa->Items->Count; n++)
	{ // verifica se o Item está selecionado
		if (!CLBoxAlternativa->Checked[n])
			continue;
		// exibe dados da Alterantiva selecionada
		alternativa = (VTAlternativa*)(CLBoxAlternativa->Items->Objects[n]);
		lisOBRA  = alternativa->Obras->LisObra();
		for (int no = 0; no < lisOBRA->Count; no++)
		{
			obra = (VTObra*)lisOBRA->Items[no];
			lisITEMOBRA = obra->LisItemObra;
			for (int ni = 0; ni < lisITEMOBRA->Count; ni++)
			{
				item = (VTItemObraCOPEL*) lisITEMOBRA->Items[ni];
				if (!item->Contabilizar)
					{continue;}
				if ((rel = relObra->PreencheRelObraTO(alternativa,obra,item)) != NULL)
				{
					// insere novo ListItem
					Item = LView->Items->Add();
					Item->Data = item;
					Item->Caption = rel->nomeAlternativa;
					Item->SubItems->Add(rel->nomeObra);
					Item->SubItems->Add(rel->data);
					Item->SubItems->Add(rel->nomeSED);
					Item->SubItems->Add(rel->nomeAlimentador);
					Item->SubItems->Add(rel->acao);
					Item->SubItems->Add(rel->eqpto);
					Item->SubItems->Add(rel->cod_modulo);
					Item->SubItems->Add(rel->descricao_modulo);
					Item->SubItems->Add(rel->complemento);
					Item->SubItems->Add(rel->custo);
				}
			}
		}
	}
	// pré-seleciona primeiro Ativo
	if (LView->Items->Count > 0)
		LView->Selected = LView->Items->Item[0];
	LView->Items->EndUpdate();
}

// ---------------------------------------------------------------------------
// eof


