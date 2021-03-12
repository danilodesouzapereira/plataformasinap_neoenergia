// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "TFormCfgRelTensao.h"
#include "TFormAlteraFaixa.h"
#include "..\TFaixaTensao.h"
#include "..\TNivelTensao.h"
#include "..\TRelatorioTensao.h"
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Patamar\VTPatamar.h>
#include <Fontes\Patamar\VTPatamares.h>
#include <DLL_Inc\Funcao.h>

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
__fastcall TFormCfgRelTensao::TFormCfgRelTensao(TComponent* Owner, VTApl *apl,
	TRelatorioTensao *relatorio_tensao) : TForm(Owner)
{
	// salva ponteiro
	this->apl = apl;
	this->relatorio_tensao = relatorio_tensao;
	// inicia valores
	nivel_ativo = NULL;
	Inicia();
}

// ---------------------------------------------------------------------------
__fastcall TFormCfgRelTensao::~TFormCfgRelTensao(void)
{
	// nada
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgRelTensao::ActionAlteraCorExecute(TObject *Sender)
{
	// variaveis locais
	TPanel *panel;

	try
	{ // determina panel selecionado
		panel = (TPanel*)Sender;

		if (ColorDialog->Execute())
		{ // atualiza cor
			panel->Color = ColorDialog->Color;
		}
	}
	catch (...)
	{
		AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(funcao);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgRelTensao::ActionAlteraFaixasExecute(TObject *Sender)
{
	// variaveis locais
	TFormAlteraFaixa *form;

	// esconde este form
	Hide();
	try
	{ // atualiza eventual troca de cor
		AtualizaFaixas();
		// cria formAlteraFaixa
		form = new TFormAlteraFaixa(this, apl, relatorio_tensao);
		if (form->ShowModal() == mrOk)
		{ // atualiza
			TabControlInicia();
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
void __fastcall TFormCfgRelTensao::ActionFechaExecute(TObject *Sender)
{
	try
	{ // salva radio groups
		relatorio_tensao->tensao_fase = (RGroupTensao->ItemIndex == 0);
		relatorio_tensao->tensao_minima = (RGroupLimite->ItemIndex == 0);
		relatorio_tensao->ind_pat = CBoxPatamar->ItemIndex;
		AtualizaFaixas();
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
void __fastcall TFormCfgRelTensao::AtualizaFaixas(void)
{
	// variaveis locais
	TList *lisFAIXA;
	TFaixaTensao *faixa;

	try
	{ // proteção
		if (nivel_ativo == NULL)
			return;
		lisFAIXA = nivel_ativo->LisFaixa();
		for (int n = 0; n < lisFAIXA->Count; n++)
		{
			faixa = (TFaixaTensao*)lisFAIXA->Items[n];
			switch (n)
			{
			case 0:
				faixa->cor = PanCorV1->Color;
				break;
			case 1:
				faixa->cor = PanCorV2->Color;
				break;
			case 2:
				faixa->cor = PanCorV3->Color;
				break;
			case 3:
				faixa->cor = PanCorV4->Color;
				break;
			case 4:
				faixa->cor = PanCorV5->Color;
				break;
			default:
				break;
			}
		}
	}
	catch (...)
	{
		AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(funcao);
	}
}

// ---------------------------------------------------------------------
void __fastcall TFormCfgRelTensao::CBoxPatamarInicia(void)
{
	// variáveis locais
	VTPatamar *patamar;
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
	TList *lisPAT = patamares->LisPatamar();

	try
	{ // reinicia CBoxPatamar c/ Patamares
		CBoxPatamar->Clear();
		for (int np = 0; np < lisPAT->Count; np++)
		{
			patamar = (VTPatamar*)lisPAT->Items[np];
			CBoxPatamar->Items->AddObject(patamar->Nome, patamar);
		}
		// seleciona patamar definido no relatorio_tensao
		CBoxPatamar->ItemIndex = relatorio_tensao->ind_pat;
	}
	catch (...)
	{
		AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(funcao);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgRelTensao::FormShow(TObject *Sender)
{
	// nada
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgRelTensao::Inicia(void)
{
	try
	{ // inicia radio groups
		RGroupTensao->ItemIndex = (relatorio_tensao->tensao_fase ? 0 : 1);
		RGroupLimite->ItemIndex = (relatorio_tensao->tensao_minima ? 0 : 1);
		// inicia patamares
		CBoxPatamarInicia();
		// inicia TabControl
		TabControlInicia();
	}
	catch (...)
	{
		AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(funcao);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgRelTensao::TabControlInicia(void)
{
	try
	{ // adota nível MT
		tabControlNivelV->TabIndex = 1;
		tabControlNivelVChange(NULL);
	}
	catch (...)
	{
		AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(funcao);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgRelTensao::tabControlNivelVChange(TObject *Sender)
{
	// variaveis locais
	TFaixaTensao *faixa;
	TNivelTensao *nivel = NULL;
	TList *lisFAIXA;

	try
	{ // determina nível de tensão selecionado
		switch (tabControlNivelV->TabIndex)
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
		if (nivel != NULL)
		{ // determina faixas
			lisFAIXA = nivel->LisFaixa();
			// preenche tabControlNivelV
			for (int n = 0; n < lisFAIXA->Count; n++)
			{
				faixa = (TFaixaTensao*)lisFAIXA->Items[n];
				switch (n)
				{
				case 0:
					lblV1->Caption = faixa->Texto();
					PanCorV1->Color = faixa->cor;
					break;
				case 1:
					lblV2->Caption = faixa->Texto();
					PanCorV2->Color = faixa->cor;
					break;
				case 2:
					lblV3->Caption = faixa->Texto();
					PanCorV3->Color = faixa->cor;
					break;
				case 3:
					lblV4->Caption = faixa->Texto();
					PanCorV4->Color = faixa->cor;
					break;
				case 4:
					lblV5->Caption = faixa->Texto();
					PanCorV5->Color = faixa->cor;
					break;
				default:
					break;
				}
			}
		}
		// atualiza nivel_ativo
		nivel_ativo = nivel;
	}
	catch (...)
	{
		AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(funcao);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgRelTensao::tabControlNivelVChanging(TObject *Sender, bool &AllowChange)
{
	try
	{ // proteção
		if (nivel_ativo == NULL)
		{
			AllowChange = true;
			return;
		}
		// atualiza faixas
		AtualizaFaixas();

		AllowChange = true;
	}
	catch (...)
	{
		AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(funcao);
	}
}

// ---------------------------------------------------------------------------
