// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFaixa_Sinal.h"
#include "TFormLimites_Sinal.h"
#include "VTTema_Sinal.h"
#include "..\..\Apl\TApl.h"
#include "..\..\..\DLL_Inc\Funcao.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
__fastcall TFormLimites_Sinal::TFormLimites_Sinal(TComponent* Owner, VTApl *apl,
	TAction *ActionRemontaMapaPadrao) : TForm(Owner)
{
	// variáveis locais
	VTTema_Sinal *tema_Sinal = (VTTema_Sinal*)apl->GetObject(__classid(VTTema_Sinal));
	TList *lisFAIXA = tema_Sinal->LisFaixa();
	TFaixa_Sinal *faixa;

	// salva ponteiro p/ objetos
	this->apl = apl;
	this->ActionRemontaMapaPadrao = ActionRemontaMapaPadrao;
	// define número mínimo e máximo de faixas
	num_faixa.min = 2;
	num_faixa.max = lisFAIXA->Count;
	num_faixa.enabled = 0;
	for (int n = 0; n < lisFAIXA->Count; n++)
	{
		faixa = (TFaixa_Sinal*)lisFAIXA->Items[n];
		if (faixa->enabled)
			num_faixa.enabled++;
	}
	// habilita/desabilita comandos para inserir/retirar faixas
	HabilitaInsereRetiraFaixa();
	// insere Form em seu Parent
	// if (parent) Parent = parent;
	// inicia StringGrid
	// StringGridInicia();
	StringGridIniciadBm();
	// inicializa texto do form
	PreencheTextoForm();
}

// ---------------------------------------------------------------------------
__fastcall TFormLimites_Sinal::~TFormLimites_Sinal(void)
{
	// nada a fazer
}

// ---------------------------------------------------------------------------
void __fastcall TFormLimites_Sinal::ActionAdicionaExecute(TObject *Sender)
{
	// proteção
	if (num_faixa.enabled >= num_faixa.max)
	{
		num_faixa.enabled = num_faixa.max;
		ActionAdiciona->Enabled = false;
		if (IS_ENGLISH == 1)
		{
			Aviso("Is not possible to create new ranges.");
		}
		else
		{
			Aviso("Não é possível criar novas faixas");
		}

		return;
	}
	// incrementa número de faixas habilitadas
	num_faixa.enabled++;
	// habilita/desabilita comandos para inserir/retirar faixas
	HabilitaInsereRetiraFaixa();
	// aumenta uma linha no StringGrid
	StringGrid->RowCount++;
	// a coluna 0 já está preenchida com número da faixa, apaga aos valores de limite
	// StringGrid->Cells[1][row_count] = "";
	// StringGrid->Cells[2][row_count] = "";
}

// ---------------------------------------------------------------------------
void __fastcall TFormLimites_Sinal::ActionRetiraExecute(TObject *Sender)
{
	// proteção
	if (num_faixa.enabled <= num_faixa.min)
	{
		num_faixa.enabled = num_faixa.min;
		ActionRetira->Enabled = false;
		if (IS_ENGLISH == 1)
		{
			Aviso("Is not possible to remove more ranges.");
		}
		else
		{
			Aviso("Não é possível retirar mais faixas");
		}

		return;
	}
	// decrementa número de faixas habilitadas
	num_faixa.enabled--;
	// habilita/desabilita comandos para inserir/retirar faixas
	HabilitaInsereRetiraFaixa();
	// diminui uma linha no StringGrid
	StringGrid->RowCount--;
}

// ---------------------------------------------------------------------------
void __fastcall TFormLimites_Sinal::ActionConfirmaExecute(TObject *Sender)
{
	// variáveis locais
	bool so_validar;

	// valida dados do StringGrid
	if (!StringGridLeValores(so_validar = true))
		return;
	// lê dados do StringGrid
	if (!StringGridLeValores(so_validar = false))
		return;
	// fecha o Form
	ModalResult = mrOk;
}

// ---------------------------------------------------------------------------
void __fastcall TFormLimites_Sinal::ActionFechaExecute(TObject *Sender)
{
	ModalResult = mrCancel;
}

// ---------------------------------------------------------------------------
void __fastcall TFormLimites_Sinal::FormShow(TObject *Sender)
{
	//
}

// ---------------------------------------------------------------------------
void __fastcall TFormLimites_Sinal::HabilitaInsereRetiraFaixa(void)
{
	ActionAdiciona->Enabled = (num_faixa.enabled < num_faixa.max);
	ActionRetira->Enabled = (num_faixa.enabled > num_faixa.min);
}

// ---------------------------------------------------------------------------
void __fastcall TFormLimites_Sinal::PreencheTextoForm(void)
{
	AnsiString *texto;
	AnsiString strFCLCPENGLISH[sflcpCOUNT] =
	{"Range Limits Editor", "Close", "Accept"};
	AnsiString strFCLCPPORTUGUES[sflcpCOUNT] =
	{"Editar Limites de Intervalos", "Fechar", "Aceitar"};

	if (IS_ENGLISH == 1)
	{
		texto = &strFCLCPENGLISH[0];
	}
	else
	{
		texto = &strFCLCPPORTUGUES[0];
	}
	// titulo
	this->Caption = texto[sflcpTITLE];
	ButFechar->Caption = texto[sflcpCLOSE];
	ButConfirmar->Caption = texto[sflcpACCEPT];
}
// ---------------------------------------------------------------------------
// void __fastcall TFormLimites_Sinal::RadioGroupUnidadeClick(TObject *Sender)
// {
// //verifica unidade selecionada
// switch(RadioGroupUnidade->ItemIndex)
// {
// case 0:  StringGridIniciadBm(); break;
// //case 1:  StringGridIniciaString(); break;
// default: StringGridIniciadBm(); break;
// }
// }

// ---------------------------------------------------------------------------
bool __fastcall TFormLimites_Sinal::StringGridCellToDouble(int col, int row, double &valor)
{
	try
	{ // converte e corrige unidade (se necessário)
		valor = StrToDouble(StringGrid->Cells[col][row]);
		// if(RadioGroupUnidade->ItemIndex == 0) valor /= 1000.;  //kVA => MVA
		return (true);
	}
	catch (Exception &e)
	{
		if (IS_ENGLISH == 1)
		{
			Erro("Range " + StringGrid->Cells[0][row] + " with invalid values");
		}
		else
		{
			Erro("Faixa " + StringGrid->Cells[0][row] + " com valores inválidos");
		}

		return (false);
	}
}

/*
 //---------------------------------------------------------------------------
 void __fastcall TFormLimites_Sinal::StringGridInicia(void)
 {
 //variaveis locais
 AnsiString  txt;
 VTTema_Sinal   *tema_Sinal  = (VTTema_Sinal*)apl->GetObject(__classid(VTTema_Sinal));
 TList       *lisFAIXA = tema_Sinal->LisFaixa();
 TFaixa      *faixa;

 //define número de linhas do StringGrid
 StringGrid->RowCount = num_faixa.max + 1;
 //apaga todas as linhas
 for (int nr = 1; nr < StringGrid->RowCount; nr++) StringGrid->Rows[nr]->Clear();
 //preenche títulos das colunas com intervalos das faixas de demanda
 StringGrid->Cells[0][0] = "Faixa";
 StringGrid->Cells[1][0] = "Limite inferior (MVA)";
 StringGrid->Cells[2][0] = "Limite superior (MVA)";
 for (int nr = 1, i = 0; (nr < StringGrid->RowCount)&&(i < lisFAIXA->Count); nr++, i++)
 {
 faixa = (TFaixa*)lisFAIXA->Items[i];
 //preenche dados da faixa, mesmo que esteja desabilitada
 StringGrid->Cells[0][nr] = txt.sprintf("%02d", nr);
 StringGrid->Cells[1][nr] = DoubleToStr("%5.4f", faixa->dem_min_mva);
 StringGrid->Cells[2][nr] = DoubleToStr("%5.4f", faixa->dem_max_mva);
 }
 //limita número de linhas do StringGrid ao número de faixas habilitadas
 StringGrid->RowCount = num_faixa.enabled + 1;
 }
 */
// ---------------------------------------------------------------------------
void __fastcall TFormLimites_Sinal::StringGridIniciadBm(void)
{
	// variaveis locais
	AnsiString txt;
	VTTema_Sinal *tema_Sinal = (VTTema_Sinal*)apl->GetObject(__classid(VTTema_Sinal));
	TList *lisFAIXA = tema_Sinal->LisFaixa();
	TFaixa_Sinal *faixa;
	double valor;

	// define número de linhas do StringGrid
	StringGrid->RowCount = num_faixa.max + 1;
	// apaga todas as linhas
	for (int nr = 1; nr < StringGrid->RowCount; nr++)
		StringGrid->Rows[nr]->Clear();
	// preenche títulos das colunas com intervalos das faixas de demanda
	if (IS_ENGLISH == 1)
	{
		StringGrid->Cells[0][0] = "Range";
		StringGrid->Cells[1][0] = "Superior Limit (dBm)";
		StringGrid->Cells[2][0] = "Inferior Limit (dBm)";
	}
	else
	{
		StringGrid->Cells[0][0] = "Intevalo";
		StringGrid->Cells[1][0] = "Limite Superior (dBm)";
		StringGrid->Cells[2][0] = "Limite Inferior (dBm)";
	}

	for (int nr = 1, i = 0; (nr < StringGrid->RowCount) && (i < lisFAIXA->Count); nr++, i++)
	{
		faixa = (TFaixa_Sinal*)lisFAIXA->Items[i];
		// preenche dados da faixa, mesmo que esteja desabilitada
		StringGrid->Cells[0][nr] = txt.sprintf("%02d", nr);
		valor = faixa->sinal_min_db;
		StringGrid->Cells[1][nr] = DoubleToStr("%2.1f", valor);
		valor = faixa->sinal_max_db;
		StringGrid->Cells[2][nr] = DoubleToStr("%2.1f", valor);
	}
	// limita número de linhas do StringGrid ao número de faixas habilitadas
	StringGrid->RowCount = num_faixa.enabled + 1;
}

// ---------------------------------------------------------------------------
// void __fastcall TFormLimites_Sinal::StringGridIniciaMVA(void)
// {
// //variaveis locais
// AnsiString  txt;
// VTTema_Sinal   *tema_Sinal  = (VTTema_Sinal*)apl->GetObject(__classid(VTTema_Sinal));
// TList       *lisFAIXA = tema_Sinal->LisFaixa();
// TFaixa      *faixa;
//
// //define número de linhas do StringGrid
// StringGrid->RowCount = num_faixa.max + 1;
// //apaga todas as linhas
// for (int nr = 1; nr < StringGrid->RowCount; nr++) StringGrid->Rows[nr]->Clear();
// //preenche títulos das colunas com intervalos das faixas de demanda
// StringGrid->Cells[0][0] = "Faixa";
// StringGrid->Cells[1][0] = "Limite inferior (MVA)";
// StringGrid->Cells[2][0] = "Limite superior (MVA)";
// for (int nr = 1, i = 0; (nr < StringGrid->RowCount)&&(i < lisFAIXA->Count); nr++, i++)
// {
// faixa = (TFaixa*)lisFAIXA->Items[i];
// //preenche dados da faixa, mesmo que esteja desabilitada
// StringGrid->Cells[0][nr] = txt.sprintf("%02d", nr);
// StringGrid->Cells[1][nr] = DoubleToStr("%5.4f", faixa->dem_min_mva);
// StringGrid->Cells[2][nr] = DoubleToStr("%5.4f", faixa->dem_max_mva);
// }
// //limita número de linhas do StringGrid ao número de faixas habilitadas
// StringGrid->RowCount = num_faixa.enabled + 1;
// StringGrid->Refresh();
// }

// ---------------------------------------------------------------------------
bool __fastcall TFormLimites_Sinal::StringGridLeValores(bool so_validar)
{
	// variaveis locais
	double lim_inf, lim_sup, lim_inf_next;
	TFaixa_Sinal *faixa;
	VTTema_Sinal *tema_Sinal = (VTTema_Sinal*)apl->GetObject(__classid(VTTema_Sinal));
	TList *lisFAIXA = tema_Sinal->LisFaixa();

	// verifica se está apenas validando os valores
	if (!so_validar)
	{ // desabilita todas as faixas
		for (int nf = 0; nf < lisFAIXA->Count; nf++)
		{
			faixa = (TFaixa_Sinal*)lisFAIXA->Items[nf];
			faixa->enabled = false;
			faixa->visible = false;
		}
	}
	// lê e valida todos os valores sem atualizar os objetos TFaixa
	for (int nr = 1, nf = 0; (nr < StringGrid->RowCount) && (nf < lisFAIXA->Count); nr++, nf++)
	{
		faixa = (TFaixa_Sinal*)lisFAIXA->Items[nf];
		// lê valores de limite inferior e superior da mesma faixa
		if (!StringGridCellToDouble(1, nr, lim_inf))
			return (false);
		if (!StringGridCellToDouble(2, nr, lim_sup))
			return (false);
		// verifica se limite inferior é menor que limite superior
		if (lim_inf >= lim_sup)
		{ // valida limites
			if (IS_ENGLISH == 1)
			{
				Aviso("Superior limit " + IntToStr(nr) +
				" must be higher than inferior limit");
			}
			else
			{
				Aviso("O limite superior da faixa " + IntToStr(nr) +
				" deve ser maior que o limite inferior");
			}


			StringGrid->SetFocus();
			return false;
		}
		// verifica se há mais uma faixa depois desta
		if ((nr + 1) < StringGrid->RowCount)
		{ // lê valor de limite inferior da próxima faixa
			if (!StringGridCellToDouble(1, nr + 1, lim_inf_next))
				return (false);
			// o limite superior desta faixa deve ser igual ao limite inferior da próxima faixa
			if (!(IsDoubleZero(lim_sup - lim_inf_next)))
			{
				if (IS_ENGLISH == 1)
				{
						Aviso("The inferior limit of range " + IntToStr(nr + 1) +
					" must be equal to the superior limit of range " + IntToStr(nr));
				}
				else
				{
					Aviso("O limite inferior da faixa " + IntToStr(nr + 1) +
					" deve ser igual ao limite superior da faixa " + IntToStr(nr));
				}

				StringGrid->SetFocus();
				return false;
			}
		}
		// verifica se está apenas validando os valores
		if (!so_validar)
		{ // habilita a faixa e atualiza seus limites
			faixa->enabled = true;
			faixa->visible = true;
			faixa->sinal_min_db = lim_inf;
			faixa->sinal_max_db = lim_sup;
		}
	}
	return true;
}

// ---------------------------------------------------------------------------
// eof
