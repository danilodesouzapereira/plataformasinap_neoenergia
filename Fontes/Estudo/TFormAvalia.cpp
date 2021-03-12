// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormAvalia.h"
#include "AnaTec\VTAnaTec.h"
#include "AnaTec\VTResumoTec.h"
#include "AnaTec\TFormSelResumoTec.h"
#include "..\Apl\VTApl.h"
#include "..\Diretorio\VTPath.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Planejamento\VTAlternativa.h"
#include "..\Planejamento\VTEstudo.h"
#include "..\Planejamento\VTPlanejamento.h"
#include "..\Report\VTReport.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Report.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
__fastcall TFormAvalia::TFormAvalia(TComponent* Owner, VTApl *apl_owner) : TForm(Owner)
{
	// salva objetos
	apl = apl_owner;
	// executa análise técnica
	if (ExecutaAnaliseTecnica())
	{ // inicia LView p/ apresentar da análise técnica
		LViewInicia();
	}
}

// ---------------------------------------------------------------------------
__fastcall TFormAvalia::~TFormAvalia(void)
{
	// variáveis locais
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

	// salva parâmetros do Form
	if (geral)
		geral->Infoset->SaveToFile(this);
}

// ---------------------------------------------------------------------------
void __fastcall TFormAvalia::ActionFechaExecute(TObject *Sender)
{
	// fecha o Form
	ModalResult = mrOk;
}

// ---------------------------------------------------------------------------
void __fastcall TFormAvalia::ActionExportaAlimExecute(TObject *Sender)
{
	//esconde este form
	Hide();
	// cria TFormSelResumoTec
	TFormSelResumoTec *FormSelResumoTec;
	FormSelResumoTec = new TFormSelResumoTec(this, apl);
	FormSelResumoTec->ShowModal();
	// destrói TFormSelResumoTec
	delete FormSelResumoTec;
	//reapresenta form
    Show();
}

// ---------------------------------------------------------------------------
void __fastcall TFormAvalia::ActionImprimeExecute(TObject *Sender)
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
	SaveDialogExporta->InitialDir = path->DirExporta();
	if (SaveDialogExporta->Execute())
	{ // recebe nome do arquivo a ser exportado e exporta listView
		nome_arq = SaveDialogExporta->FileName;
		if (report->ArquivoTexto(nome_arq, LViewAnaTec, "", false))
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
bool __fastcall TFormAvalia::ExecutaAnaliseTecnica(void)
{
	// variáveis locais
	bool sucesso;
	VTAnaTec *ana_tec;

	try
	{ // cria objeto VTAnaTec
		ana_tec = NewObjAnaTec(apl);
		// executa análise técnica das Alternativas
		sucesso = ana_tec->Executa();
		// destró objeto VTAnaTec
		delete ana_tec;
	}
	catch (Exception &e)
	{
		Erro("Não foi possível executar a análise técnica das alternativas");
		sucesso = false;
	}
	return (sucesso);
}

// ---------------------------------------------------------------------------
void __fastcall TFormAvalia::ExibirRede(bool enabled)
{
	// salva dado
	exibir_redes = enabled;
}

// ---------------------------------------------------------------------------
VTEstudo* __fastcall TFormAvalia::ExisteEstudo(void)
{
	// variáveis locais
	VTEstudo *estudo = NULL;
	VTPlanejamento *planejamento = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));
	TList *lisESTUDO = planejamento->LisEstudo();

	if (lisESTUDO->Count == 1)
		estudo = (VTEstudo*)lisESTUDO->First();

	return (estudo);
}

// ---------------------------------------------------------------------------
void __fastcall TFormAvalia::FormIniciaPosicao(void)
{
	// variáveis locais
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

	if (geral)
	{ // lê parâmetros do Form
		geral->Infoset->LoadFromFile(this);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormAvalia::LViewInicia(void)
{
	// variáveis locais
	TList *lisALTERNATIVA;
	TList *lisOBRA;
	TListItem *Item;
	VTAlternativa *alternativa;
	VTResumoTec *resumo;
	TListView *LView = LViewAnaTec;
	VTEstudo *estudo = ExisteEstudo();

	// reinicia LView
	LView->Items->Clear();
	// proteção
	if (estudo == NULL)
		return;
	// loop p/ todas Alternativas do Estudo
	lisALTERNATIVA = estudo->LisAlternativa();
	for (int n = 0; n < lisALTERNATIVA->Count; n++)
	{
		alternativa = (VTAlternativa*)lisALTERNATIVA->Items[n];
		resumo = (VTResumoTec*)alternativa->Apl->GetObject(__classid(VTResumoTec));
		for (int ano = resumo->AnoInicial; ano <= resumo->AnoFinal; ano++)
		{ // insere ListItem
			Item = LView->Items->Add();
			Item->Data = alternativa;
			Item->Checked = alternativa->Valida;
			Item->Caption = alternativa->Codigo;
			Item->SubItems->Add(IntToStr(ano));
			// barras c/ tensão crítica
			Item->SubItems->Add(DoubleToStr("%3.2f", resumo->DiagBarra[ano][diagRUIM]));
			// trafos c/ cargto crítico
			Item->SubItems->Add(DoubleToStr("%3.2f", resumo->DiagTrafo[ano][diagRUIM]));
			// trechos  / cargto crítico
			Item->SubItems->Add(DoubleToStr("%3.2f", resumo->DiagTrechoKm[ano][diagRUIM]));
			// energia recebida
			Item->SubItems->Add(DoubleToStr("%3.2f", resumo->Entrada_mwhm[ano]));
			// energia gerada
			Item->SubItems->Add(DoubleToStr("%3.2f", resumo->Gerada_mwhm[ano]));
			// energia consumida
			Item->SubItems->Add(DoubleToStr("%3.2f", resumo->Entregue_mwhm[ano]));
			// perda
			Item->SubItems->Add(DoubleToStr("%3.2f", resumo->Perda_mwhm[ano]));
			// DEC
			Item->SubItems->Add(DoubleToStr("%3.2f", resumo->DEC[ano])); // DVK 2015.08.12
			// FEC
			Item->SubItems->Add(DoubleToStr("%3.2f", resumo->FEC[ano])); // DVK 2015.08.12
			// END
			Item->SubItems->Add(DoubleToStr("%3.2f", resumo->END[ano]));
			// Compensação
			Item->SubItems->Add(DoubleToStr("%3.2f", resumo->Compensacao[ano]));
		}
	}
}
// ---------------------------------------------------------------------------
// eof
