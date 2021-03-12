// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "TFormAlteraFaixa.h"
#include "..\TFaixaTensao.h"
#include "..\TNivelTensao.h"
#include "..\TRelatorioTensao.h"
#include <Fontes\Apl\VTApl.h>
#include <DLL_Inc\Funcao.h>

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
__fastcall TFormAlteraFaixa::TFormAlteraFaixa(TComponent* Owner, VTApl *apl,
	TRelatorioTensao *relatorio_tensao) : TForm(Owner)
{
	// salva ponteiro
	this->apl = apl;
	this->relatorio_tensao = relatorio_tensao;
	// inicia variavel nula
	nivel_ativo = NULL;
	ind_nivel = -1;
	// cria niveis temporarios
	nivel_at = new TNivelTensao(apl, nvAT);
	nivel_mt = new TNivelTensao(apl, nvMT);
	nivel_bt = new TNivelTensao(apl, nvBT);
	// copia faixas
	relatorio_tensao->CopiaFaixas(relatorio_tensao->nivel_at, nivel_at);
	relatorio_tensao->CopiaFaixas(relatorio_tensao->nivel_mt, nivel_mt);
	relatorio_tensao->CopiaFaixas(relatorio_tensao->nivel_bt, nivel_bt);
}

// ---------------------------------------------------------------------------
__fastcall TFormAlteraFaixa::~TFormAlteraFaixa(void)
{
	// destroi niveis temporarios
	DestroiTObject(nivel_at);
	DestroiTObject(nivel_mt);
	DestroiTObject(nivel_bt);
}

// ---------------------------------------------------------------------------
void __fastcall TFormAlteraFaixa::ActionCancelaExecute(TObject *Sender)
{
	// fecha este Form
	ModalResult = mrCancel;
}

// ---------------------------------------------------------------------------
void __fastcall TFormAlteraFaixa::ActionConfirmaExecute(TObject *Sender)
{
	try
	{ // verifica valores se s�o v�lidos (da �ltima altera��o, pois demais j� estariam ok)
		if (!ValidaValores())
			return;
		// copia valores para os niveis n�o tempor�rios
		relatorio_tensao->CopiaFaixas(nivel_at, relatorio_tensao->nivel_at);
		relatorio_tensao->CopiaFaixas(nivel_mt, relatorio_tensao->nivel_mt);
		relatorio_tensao->CopiaFaixas(nivel_bt, relatorio_tensao->nivel_bt);
		// atualiza config_alterada
		relatorio_tensao->config_alterada = true;
		// fecha este Form
		ModalResult = mrOk;
	}
	catch (...)
	{
		AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(funcao);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormAlteraFaixa::RGroupNivelClick(TObject *Sender)
{
	try
	{ // verifica se o nivel ativo foi alterado
		if (RGroupNivel->ItemIndex == ind_nivel)
			return;
		if (nivel_ativo != NULL)
		{
			if (!ValidaValores())
			{
				RGroupNivel->ItemIndex = ind_nivel;
				return;
			}
		}
		// verifica nivel de tens�o selecionado para preencher SGrid
		switch (RGroupNivel->ItemIndex)
		{
		case 0: // AT
			SGridInicia(nivel_at);
			nivel_ativo = nivel_at;
			ind_nivel = RGroupNivel->ItemIndex;
			break;
		case 1: // MT
			SGridInicia(nivel_mt);
			nivel_ativo = nivel_mt;
			ind_nivel = RGroupNivel->ItemIndex;
			break;
		case 2: // BT
			SGridInicia(nivel_bt);
			nivel_ativo = nivel_bt;
			ind_nivel = RGroupNivel->ItemIndex;
			break;
		default:
			Aviso("Selecione um n�vel de tens�o");
			break;
		}
	}
	catch (...)
	{
		AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(funcao);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormAlteraFaixa::SGridInicia(TNivelTensao *nivel)
{
	// variaveis locais
	TList *lisFAIXA;
	TFaixaTensao *faixa;

	try
	{ // determina lisFAIXA
		lisFAIXA = nivel->LisFaixa();
		if (lisFAIXA == NULL)
			return;
		// limpa SGrid
		for (int nr = 1; nr < SGridValor->RowCount; nr++)
		{
			SGridValor->Rows[nr]->Clear();
		}
		// define n�mero de linhas
		SGridValor->RowCount = 6;
		// preenche cabe�alho
		SGridValor->Cells[0][0] = "Faixa";
		SGridValor->Cells[1][0] = "Valor inferior (pu)";
		SGridValor->Cells[2][0] = "Valor superior (pu)";
		// preenche valores das faixas do nivel de tens�o
		for (int nr = 1, i = 0; (nr < SGridValor->RowCount) && (i < lisFAIXA->Count); nr++, i++)
		{
			faixa = (TFaixaTensao*)lisFAIXA->Items[i];
			SGridValor->Cells[0][nr] = IntToStr(nr);
			SGridValor->Cells[1][nr] = DoubleToStr("%4.3f", faixa->valor_inf);
			SGridValor->Cells[2][nr] = DoubleToStr("%4.3f", faixa->valor_sup);
		}
	}
	catch (...)
	{
		AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(funcao);
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TFormAlteraFaixa::StringGridCellToDouble(int col, int row, double &valor)
{
	try
	{ // converte e corrige unidade (se necess�rio)
		valor = StrToDouble(SGridValor->Cells[col][row]);
		return (true);
	}
	catch (...)
	{
		Erro("Faixa " + SGridValor->Cells[0][row] + " com valores inv�lidos");
		return (false);
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TFormAlteraFaixa::ValidaValores(void)
{
	// variaveis locais
	double lim_inf, lim_sup, lim_inf_next;
	TFaixaTensao *faixa;
	TList *lisFAIXA = nivel_ativo->LisFaixa();

	try
	{ // l� e valida todos os valores sem atualizar os objetos TFaixa
		for (int nr = 1, nf = 0; (nr < SGridValor->RowCount) && (nf < lisFAIXA->Count); nr++, nf++)
		{
			faixa = (TFaixaTensao*)lisFAIXA->Items[nf];
			// l� valores de limite inferior e superior da mesma faixa
			if (!StringGridCellToDouble(1, nr, lim_inf))
				return (false);
			if (!StringGridCellToDouble(2, nr, lim_sup))
				return (false);

			// verifica se h� mais uma faixa depois desta
			if ((nr + 1) < SGridValor->RowCount)
			{ // l� valor de limite inferior da pr�xima faixa
				if (!StringGridCellToDouble(1, nr + 1, lim_inf_next))
					return (false);
				// o limite superior desta faixa deve ser igual ao limite inferior da pr�xima faixa
				if (!IsDoubleZero(lim_sup - lim_inf_next))
				{
					Aviso("O limite inferior da faixa " + IntToStr(nr + 1) +
						" deve ser igual ao limite superior da faixa " + IntToStr(nr));
					SGridValor->SetFocus();
					return false;
				}
				// verifica se limite inferior � menor que limite superior
				if (lim_inf > lim_sup)
				{ // valida limites
					Aviso("O limite superior da faixa " + IntToStr(nr) +
						" deve ser maior que o limite inferior");
					SGridValor->SetFocus();
					return false;
				}
			}
			else
			{ // caso especial: �ltima faixa
				if (lim_sup > 0.)
				{
					Aviso("O limite superior da �ltima faixa deve ser -1");
					SGridValor->SetFocus();
					return false;
				}
			}
			// insere valores na faixa
			faixa->valor_inf = lim_inf;
			faixa->valor_sup = lim_sup;
		}
	}
	catch (...)
	{
		AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(funcao);
		return false;
	}
	return true;
}

// ---------------------------------------------------------------------------
