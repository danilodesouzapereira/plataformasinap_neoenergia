// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TFormSelResumoTec.h"
#include "TSelResumoTec.h"
#include "VTResumoTec.h"
#include "..\Apl\VTApl.h"
#include "..\Diretorio\VTPath.h"
#include "..\Planejamento\VTAlternativa.h"
#include "..\Planejamento\VTEstudo.h"
#include "..\Planejamento\VTPlanejamento.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTTipoRede.h"
#include "..\..\DLL_Inc\Funcao.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
__fastcall TFormSelResumoTec::TFormSelResumoTec(TComponent* Owner, VTApl *apl) : TForm(Owner)
{
	//
	this->apl = apl;
	// cria objetos
	selResumoTec = new TSelResumoTec(apl);
	// preenche todos CheckListBox
	CheckListBoxAltInicia();
	CheckListBoxAnoInicia();
	CheckListBoxSedSdmtInicia();
}

// ---------------------------------------------------------------------------
__fastcall TFormSelResumoTec::~TFormSelResumoTec(void)
{
	// destroi objeto
	if (selResumoTec)
	{
		delete selResumoTec;
		selResumoTec = NULL;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormSelResumoTec::CheckListBoxAltInicia(void)
{
	// variáveis locais
	VTPlanejamento *planejamento = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));
	VTEstudo *estudo = NULL;
	TList *lisESTUDO, *lisALT;
	VTAlternativa *alternativa;

	if (planejamento != NULL)
	{
		lisESTUDO = planejamento->LisEstudo();
		if (lisESTUDO->Count == 1)
			estudo = (VTEstudo*)lisESTUDO->First();
	}
	// proteção
	if (estudo == NULL)
		return;
	// limpa lista de alternativas
	CheckListBoxAlt->Clear();
	// preenche com as alternativas do estudo
	lisALT = estudo->LisAlternativa();
	for (int n = 0; n < lisALT->Count; n++)
	{
		alternativa = (VTAlternativa*)lisALT->Items[n];
		CheckListBoxAlt->AddItem(alternativa->Codigo, alternativa);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormSelResumoTec::CheckListBoxAnoInicia(void)
{
	// variáveis locais
	VTPlanejamento *planejamento = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));
	VTEstudo *estudo = NULL;
	TList *lisESTUDO;
	AnsiString str_ano;

	if (planejamento != NULL)
	{
		lisESTUDO = planejamento->LisEstudo();
		if (lisESTUDO->Count == 1)
			estudo = (VTEstudo*)lisESTUDO->First();
	}
	// proteção
	if (estudo == NULL)
		return;
	// limpa lista de anos
	CheckListBoxAno->Clear();
	// preenche com os anos do estudo
	for (int ano = estudo->AnoInicial; ano <= estudo->AnoFinal; ano++)
	{
		str_ano = IntToStr(ano);
		CheckListBoxAno->AddItem(str_ano, NULL);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormSelResumoTec::CheckListBoxSedSdmtInicia(void)
{
	// variáveis locais
	VTAlternativa *alternativa;
	VTResumoTec *resumo, *resumo_rede;
	VTPlanejamento *planejamento = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));
	VTEstudo *estudo = NULL;
	TList *lisESTUDO, *lisALT, *lisRESUMO;
	AnsiString str_ano, sub_alim;
	TListItem *item;

	// limpa lista de subestações e alimentadores
	CheckListViewAlim->Clear();
	// verifica se ainda há estudo
	if (planejamento != NULL)
	{
		lisESTUDO = planejamento->LisEstudo();
		if (lisESTUDO->Count == 1)
			estudo = (VTEstudo*)lisESTUDO->First();
	}
	// proteção
	if (estudo == NULL)
		return;
	// loop em cada alternativa
	lisALT = estudo->LisAlternativa();
	for (int n = 0; n < lisALT->Count; n++)
	{
		alternativa = (VTAlternativa*)lisALT->Items[n];
		resumo = (VTResumoTec*)alternativa->Apl->GetObject(__classid(VTResumoTec));
		if (resumo == NULL)
			continue;
		// loop em cada resumo de alimentadores
		lisRESUMO = resumo->LisResumoTecRede();
		for (int nr = 0; nr < lisRESUMO->Count; nr++)
		{
			resumo_rede = (VTResumoTec*)lisRESUMO->Items[nr];
			// insere no set composição alimentador_subestacao
			sub_alim = resumo_rede->Subestacao + "_" + resumo_rede->Alimentador;
			if (alimentador.find(sub_alim) != alimentador.end())
				continue;
			alimentador.insert(sub_alim);
			item = CheckListViewAlim->Items->Add();
			item->Data = resumo_rede;
			item->Caption = "";
			item->SubItems->Add(resumo_rede->Subestacao);
			item->SubItems->Add(resumo_rede->Alimentador);
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormSelResumoTec::CheckListViewAlimColumnClick(TObject *Sender,
	TListColumn *Column)
{
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
void __fastcall TFormSelResumoTec::CheckListViewAlimCompare(TObject *Sender, TListItem *Item1,
	TListItem *Item2, int Data, int &Compare)
{
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
	{ // coluna de Caption (onde ficam os checks)
		return;
	}
	else
	{ // coluna com texto
		Compare = CompareText(Item1->SubItems->Strings[index], Item2->SubItems->Strings[index]);
	}
	// verifica se ordem crescente/decrescente
	if (!LViewSort.OrdemCrescente)
		Compare = -Compare;
}

// ---------------------------------------------------------------------------
void __fastcall TFormSelResumoTec::FormShow(TObject *Sender)
{
	// seleciona tab alternativas
	PageControlAT->ActivePage = TabSheetAlt;
}

// ---------------------------------------------------------------------------
void __fastcall TFormSelResumoTec::ActionAltSelAllExecute(TObject *Sender)
{
	// percorre todos itens
	for (int n = 0; n < CheckListBoxAlt->Count; n++)
	{
		CheckListBoxAlt->Checked[n] = true;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormSelResumoTec::ActionAltSelNoneExecute(TObject *Sender)
{
	// percorre todos itens
	for (int n = 0; n < CheckListBoxAlt->Count; n++)
	{
		CheckListBoxAlt->Checked[n] = false;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormSelResumoTec::ActionAnoSelAllExecute(TObject *Sender)
{
	// percorre todos itens
	for (int n = 0; n < CheckListBoxAno->Count; n++)
	{
		CheckListBoxAno->Checked[n] = true;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormSelResumoTec::ActionAnoSelNoneExecute(TObject *Sender)
{
	// percorre todos itens
	for (int n = 0; n < CheckListBoxAno->Count; n++)
	{
		CheckListBoxAno->Checked[n] = false;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormSelResumoTec::ActionImprimeExecute(TObject *Sender)
{
	// variaveis locais
	AnsiString nome_arq;
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));

	// valida seleção
	if (!ValidaSelecao())
		return;
	// esconde form, para a janela Salvar Como não ficar por baixo
	Hide();
	// define pasta inicial
	SaveDialogExporta->InitialDir = path->DirExporta();
	if (SaveDialogExporta->Execute())
	{ // recebe nome do arquivo a ser exportado
		nome_arq = SaveDialogExporta->FileName;
		// insere alternativas selecionadas
		InsereAlternativas();
		// insere anos selecionados
		InsereAnos();
		// insere alimentadores selecionados
		InsereAlimentadores();
		// executa
		if (selResumoTec->Exporta(nome_arq))
		{
			Aviso("Relatório exportado com sucesso");
		}
	}
	Show();
}

// ---------------------------------------------------------------------------
void __fastcall TFormSelResumoTec::ActionSdmtSelAllExecute(TObject *Sender)
{
	// percorre todos itens
	for (int n = 0; n < CheckListViewAlim->Items->Count; n++)
	{
		CheckListViewAlim->Items->Item[n]->Checked = true;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormSelResumoTec::ActionSdmtSelNoneExecute(TObject *Sender)
{
	// percorre todos itens
	for (int n = 0; n < CheckListViewAlim->Items->Count; n++)
	{
		CheckListViewAlim->Items->Item[n]->Checked = false;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormSelResumoTec::ActionFechaExecute(TObject *Sender)
{
	ModalResult = mrCancel;
}

// ---------------------------------------------------------------------------
void __fastcall TFormSelResumoTec::InsereAlimentadores(void)
{
	// variaveis locais
	VTResumoTec *resumo;

	for (int n = 0; n < CheckListViewAlim->Items->Count; n++)
	{
		if (!CheckListViewAlim->Items->Item[n]->Checked)
			continue;
		resumo = (VTResumoTec*)CheckListViewAlim->Items->Item[n]->Data;
		selResumoTec->InsereAlimentador(resumo->Alimentador, resumo->Subestacao);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormSelResumoTec::InsereAlternativas(void)
{
	// variaveis locais
	VTAlternativa *alternativa;

	for (int n = 0; n < CheckListBoxAlt->Count; n++)
	{
		if (!CheckListBoxAlt->Checked[n])
			continue;
		alternativa = (VTAlternativa*)CheckListBoxAlt->Items->Objects[n];
		selResumoTec->InsereAlternativa(alternativa);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormSelResumoTec::InsereAnos(void)
{
	// variaveis locais
	int ano;

	for (int n = 0; n < CheckListBoxAno->Count; n++)
	{
		if (!CheckListBoxAno->Checked[n])
			continue;
		ano = StrToInt(CheckListBoxAno->Items->Strings[n]);
		selResumoTec->InsereAno(ano);
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TFormSelResumoTec::ValidaSelecao(void)
{
	// variaveis locais
	bool selecionado = false;

	// verifica se tem alternativa selecionada
	for (int n = 0; n < CheckListBoxAlt->Count; n++)
	{
		if (CheckListBoxAlt->Checked[n])
		{
			selecionado = true;
			break;
		}
	}
	if (!selecionado)
	{
		Aviso("Selecione, pelo menos, uma alternativa");
		return false;
	}
	// verifica se tem algum ano selecionado
	selecionado = false;
	for (int n = 0; n < CheckListBoxAno->Count; n++)
	{
		if (CheckListBoxAno->Checked[n])
		{
			selecionado = true;
			break;
		}
	}
	if (!selecionado)
	{
		Aviso("Selecione, pelo menos, um ano de análise");
		return false;
	}
	// verifica se tem algum alimentador selecionado
	for (int n = 0; n < CheckListViewAlim->Items->Count; n++)
	{
		if (CheckListViewAlim->Items->Item[n]->Checked)
		{
			selecionado = true;
			break;
		}
	}
	if (!selecionado)
	{
		Aviso("Selecione, pelo menos, um alimentador");
		return false;
	}
	return true;
}

// ---------------------------------------------------------------------------
