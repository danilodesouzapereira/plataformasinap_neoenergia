// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <algorithm>
#include <Math.hpp>
#include "Complexo.h"
#include "TFormEditaCurva3F.h"
//#include "TFormEditaCurva.h"
#include "TFormEdtSup3.h"
#include "..\Apl\VTApl.h"
#include "..\Constante\Const.h"
#include "..\Constante\Fases.h"
#include "..\Curvas\VTCurva.h"
// #include "..\Legenda\VTLegenda.h"
// #include "..\Legenda\VTVetval.h"
#include "..\Patamar\VTPatamar.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTSuprimento.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Legenda.h"
#include "..\..\DLL_Inc\Patamar.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
using namespace std;
TFormEditaCurva3F *FormEditaCurva3F;

// ---------------------------------------------------------------------------
__fastcall TFormEditaCurva3F::TFormEditaCurva3F(TForm* form_owner, VTApl *apl_owner,
	TWinControl *parent) : TForm(Owner)
{
	// salva ponteiro p/ objetos
	this->apl = apl_owner;
	// salva ponteiro pro form pai (pra sincronizar combobox do tipoSup)
	// isso impossibilita o reuso desse form em outros lugares =(
	formEdtSup3 = (TFormEdtSup3*) form_owner;
	// insere em seu Parent
	if (parent)
	{
		Parent = parent;
		Align = alClient;
	}
	// cria objeto Patamares e define sua escala
	patamares = DLL_NewObjPatamares(apl);
	SGrid->ColWidths[0] = 75;
}

// ---------------------------------------------------------------------------
__fastcall TFormEditaCurva3F::~TFormEditaCurva3F(void)
{
	// libera memóra alocada
	coluna.Length = 0;
	// destrói objetos criados
	if (patamares)
	{
		delete patamares;
		patamares = NULL;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditaCurva3F::CBoxNumPtoInicia(void)
{
	// verifica o número de pontos da Curva definido pela Curva
	switch (curva->Escala)
	{
	case pat24HORA:
		CBoxNumPto->ItemIndex = 0;
		break;
	case pat12HORA:
		CBoxNumPto->ItemIndex = 1;
		break;
	case pat08HORA:
		CBoxNumPto->ItemIndex = 2;
		break;
	case pat06HORA:
		CBoxNumPto->ItemIndex = 3;
		break;
	case pat04HORA:
		CBoxNumPto->ItemIndex = 4;
		break;
	case pat03HORA:
		CBoxNumPto->ItemIndex = 5;
		break;
	case pat02HORA:
		CBoxNumPto->ItemIndex = 6;
		break;
	case pat01HORA:
		CBoxNumPto->ItemIndex = 7;
		break;
	case pat15MIN:
		CBoxNumPto->ItemIndex = 8;
		break;
	case pat10MIN:
		CBoxNumPto->ItemIndex = 9;
		break;
	case pat05MIN:
		CBoxNumPto->ItemIndex = 10;
		break;
	case patEDITAVEL:
		CBoxNumPto->ItemIndex = 11;
		break;
	default: // curva c/ escala indefinida
		curva->Escala = pat01HORA;
		// chamada recursiva c/ Escala definida
		CBoxNumPtoInicia();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditaCurva3F::CBoxNumPtoChange(TObject *Sender)
{
	// redefine número de Patamares
	switch (CBoxNumPto->ItemIndex)
	{
	case 0:
		patamares->Escala = pat24HORA;
		break;
	case 1:
		patamares->Escala = pat12HORA;
		break;
	case 2:
		patamares->Escala = pat08HORA;
		break;
	case 3:
		patamares->Escala = pat06HORA;
		break;
	case 4:
		patamares->Escala = pat04HORA;
		break;
	case 5:
		patamares->Escala = pat03HORA;
		break;
	case 6:
		patamares->Escala = pat02HORA;
		break;
	case 7:
		patamares->Escala = pat01HORA;
		break;
	case 8:
		patamares->Escala = pat15MIN;
		break;
	case 9:
		patamares->Escala = pat10MIN;
		break;
	case 10:
		patamares->Escala = pat05MIN;
		break;
	default:
		patamares->Escala = patEDITAVEL;
		break;
	}
	// reconfigura a Curva (muda número de pontos, salvando seus valores originais)
	curva->Reconfigura(patamares->Escala);
	// reapresenta pontos da Curva
	SGridInicia();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditaCurva3F::CBoxTipoSupChange(TObject *Sender)
{
	formEdtSup3->cboxTipoSup->ItemIndex = CBoxTipoSup->ItemIndex;
	SGridInicia();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditaCurva3F::DefineCurva(VTCurva *curva)
{
	// salva ponteiro p/ objeto
	this->curva = curva;
	// atualiza CBoxNumPto p/ exibir número de pontos da Curva
	CBoxNumPtoInicia();
	// redefine número de Patamares
	switch (curva->Escala)
	{
	case pat24HORA:
		patamares->Escala = pat24HORA;
		break;
	case pat12HORA:
		patamares->Escala = pat12HORA;
		break;
	case pat08HORA:
		patamares->Escala = pat08HORA;
		break;
	case pat06HORA:
		patamares->Escala = pat06HORA;
		break;
	case pat04HORA:
		patamares->Escala = pat04HORA;
		break;
	case pat03HORA:
		patamares->Escala = pat03HORA;
		break;
	case pat02HORA:
		patamares->Escala = pat02HORA;
		break;
	case pat01HORA:
		patamares->Escala = pat01HORA;
		break;
	case pat15MIN:
		patamares->Escala = pat15MIN;
		break;
	case pat10MIN:
		patamares->Escala = pat10MIN;
		break;
	case pat05MIN:
		patamares->Escala = pat05MIN;
		break;
	default:
		patamares->Escala = pat01HORA;
		break;
	}
	// reapresenta pontos da Curva
	SGridInicia();
	// desabilita edição p/ Curva típica
	Enabled = !curva->Tipica;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditaCurva3F::ExibeDados(void)
{
	// variáveis locais
	int nc, nr, np, numPatamar, index, casoPat, casoGran, ind_val;
	strHM hm_ini, hm_fim;
	double valor[nvCURVA_RRSUP];

	// qual opcao de patamares foi selecionada?
	casoPat = CBoxNumPto->ItemIndex;
	// qual opcao de tipo de suprimento foi selecionada?
	casoGran = CBoxTipoSup->ItemIndex;
	// protege caso não esteja nada selecionado
	if (casoPat < 0)
	{
		casoPat = 0;
		CBoxNumPto->ItemIndex = 0;
	}
	if (casoGran < 0)
	{
		casoGran = 0;
		CBoxTipoSup->ItemIndex = 0;
	}
	switch (casoPat)
	{
	case 0:
		numPatamar = 1;
		break; // 24 horas (1 ponto)
	case 1:
		numPatamar = 2;
		break; // 12 horas (2 pontos)
	case 2:
		numPatamar = 3;
		break; // 8 horas (3 pontos)
	case 3:
		numPatamar = 4;
		break; // 6 horas (4 pontos)
	case 4:
		numPatamar = 6;
		break; // 4 horas (6 pontos)
	case 5:
		numPatamar = 8;
		break; // 3 horas (8 pontos)
	case 6:
		numPatamar = 12;
		break; // 2 horas (12 pontos)
	case 7:
		numPatamar = 24;
		break; // 1 hora (24 pontos)
	case 8:
		numPatamar = 96;
		break; // 15 minutos (96 pontos)
	case 9:
		numPatamar = 144;
		break; // 10 minutos (144 pontos)
	case 10:
		numPatamar = 288;
		break; // 5 minutos (288 pontos)
	}
	switch (casoGran)
	{
	case (supPV - 1): // P/V
		SGrid->Cells[1][0] = "P_a";
		SGrid->Cells[2][0] = "V_a";
		SGrid->Cells[3][0] = "P_b";
		SGrid->Cells[4][0] = "V_b";
		SGrid->Cells[5][0] = "P_c";
		SGrid->Cells[6][0] = "V_c";
		for (np = 0; np < numPatamar; np++)
		{
			psup->curva->GetPonto(np, hm_ini, hm_fim, valor, nvCURVA_RRSUP);
			nc = SGrid->FixedCols;
			for (int ind_fase = indFASE_A; ind_fase <= indFASE_C; ind_fase++)
			{
				ind_val = 0; // valor de P
				index = (ind_fase * nvCURVA_PQVT) + ind_val;
				SGrid->Cells[nc][np + SGrid->FixedRows] = DoubleToStr("%5.4f", valor[index]);
				ind_val = 2; // valor de V
				nc = nc + 1;
				index = (ind_fase * nvCURVA_PQVT) + ind_val;
				SGrid->Cells[nc][np + SGrid->FixedRows] = DoubleToStr("%5.4f", valor[index]);
				nc = nc + 1;
			}
		}
		break;
	case (supPQ - 1): // P/Q
		SGrid->Cells[1][0] = "P_a";
		SGrid->Cells[2][0] = "Q_a";
		SGrid->Cells[3][0] = "P_b";
		SGrid->Cells[4][0] = "Q_b";
		SGrid->Cells[5][0] = "P_c";
		SGrid->Cells[6][0] = "Q_c";
		for (np = 0; np < numPatamar; np++)
		{
			psup->curva->GetPonto(np, hm_ini, hm_fim, valor, nvCURVA_RRSUP);
			nc = SGrid->FixedCols;
			for (int ind_fase = indFASE_A; ind_fase <= indFASE_C; ind_fase++)
			{
				ind_val = 0; // valor de P
				index = (ind_fase * nvCURVA_PQVT) + ind_val;
				SGrid->Cells[nc][np + SGrid->FixedRows] = DoubleToStr("%5.4f", valor[index]);
				ind_val = 1; // valor de Q
				nc = nc + 1;
				index = (ind_fase * nvCURVA_PQVT) + ind_val;
				SGrid->Cells[nc][np + SGrid->FixedRows] = DoubleToStr("%5.4f", valor[index]);
				nc = nc + 1;
			}
		}
		break;
	case (supVF - 1): // V/Teta
		SGrid->Cells[1][0] = "V_a";
		SGrid->Cells[2][0] = "Teta_a";
		SGrid->Cells[3][0] = "V_b";
		SGrid->Cells[4][0] = "Teta_b";
		SGrid->Cells[5][0] = "V_c";
		SGrid->Cells[6][0] = "Teta_c";
		for (np = 0; np < numPatamar; np++)
		{
			psup->curva->GetPonto(np, hm_ini, hm_fim, valor, nvCURVA_RRSUP);
			nc = SGrid->FixedCols;
			for (int ind_fase = indFASE_A; ind_fase <= indFASE_C; ind_fase++)
			{
				ind_val = 2; // valor de V
				index = (ind_fase * nvCURVA_PQVT) + ind_val;
				SGrid->Cells[nc][np + SGrid->FixedRows] = DoubleToStr("%6.5f", valor[index]);
				ind_val = 3; // valor de Teta
				nc = nc + 1;
				index = (ind_fase * nvCURVA_PQVT) + ind_val;
				SGrid->Cells[nc][np + SGrid->FixedRows] =
					DoubleToStr("%4.3f", RadToDeg(valor[index]));
				nc = nc + 1;
			}
		}
		break;
	default:
		break;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditaCurva3F::InsereColuna(int ind_curva, AnsiString caption, double valor_min,
	double valor_max)
{
	// variáveis locais
	int index;

	// salva índice da coluna
	index = coluna.Length;
	// salva dados da nova coluna
	coluna.Length = coluna.Length + 1;
	coluna[index].caption = caption;
	coluna[index].ind_curva = ind_curva;
	coluna[index].valor_min = valor_min;
	coluna[index].valor_max = valor_max;
}

// ---------------------------------------------------------------------------
/*
 bool __fastcall TFormEditaCurva3F::IsNumber(AnsiString string)
 {
 double aux;
 bool isNumber;
 try{
 aux= string.ToDouble();
 isNumber = true;
 }
 catch(Exception &e)
 {
 Erro("Preencha com valores numéricos");
 isNumber = false;
 }
 return isNumber;
 }
 */
// ---------------------------------------------------------------------------
bool __fastcall TFormEditaCurva3F::LePontosCurva(void)
{
	// verifica se Curva c/ patamares de período variável
	if (CBoxNumPto->ItemIndex == 11)
		return (LeDadosPatamarVariavel());
	else
		return (LeDadosPatamarFixo());
}

// ---------------------------------------------------------------------------
bool __fastcall TFormEditaCurva3F::LeDadosPatamarFixo(void)
{
	// variáveis locais
	int index, tipoSup, nc;
	strHM hm_ini, hm_fim;
	double aux;
	double valor[nvCURVA_RRSUP];
	VTPatamar *patamar;
	bool erroDeseq;
	AnsiString msgErro;

	erroDeseq = false;
	msgErro = "Tensões com grande desequilíbrio nos patamares:";
	// verifica o tipo de suprimento
	tipoSup = CBoxTipoSup->ItemIndex;
	if (tipoSup < 0)
	{
		tipoSup = (supPV - 1);
		CBoxTipoSup->ItemIndex = (supPV - 1);
	}
	try
	{ // atualiza pontos da Curva
		switch (tipoSup)
		{
		case (supPV - 1): // P/V
			for (int nr = 1, np = 0; (nr < SGrid->RowCount) && (np < patamares->NumPatamar());
			nr++, np++)
			{
				patamar = patamares->Patamar[np];
				hm_ini.hora = patamar->HoraIni;
				hm_ini.minuto = patamar->MinutoIni;
				hm_fim.hora = patamar->HoraFim;
				hm_fim.minuto = patamar->MinutoFim;
				nc = SGrid->FixedCols;
				for (int ind_fase = indFASE_A; ind_fase <= indFASE_C; ind_fase++)
				{
					index = (ind_fase * nvCURVA_PQVT) + indP; // valor de P
					if (StrToDouble(SGrid->Cells[nc][nr], aux))
					{
						valor[index] = StrToDouble(SGrid->Cells[nc][nr]);
						if (!ValidaValor(np, valor[index], coluna[nc - SGrid->FixedCols]))
							// Verificar
						{
							Erro("Valor fora do limite para no patamar " + IntToStr(hm_ini.hora) +
								":" + IntToStr(hm_ini.minuto) + " a " + IntToStr(hm_fim.hora) +
								":" + IntToStr(hm_fim.minuto));
							return (false);
						}
					}
					else
					{
						return (false);
					}
					nc++;
					index = (ind_fase * nvCURVA_PQVT) + indQ; // valor de Q
					valor[index] = 0;
					index = (ind_fase * nvCURVA_PQVT) + indV; // valor de V
					if (StrToDouble(SGrid->Cells[nc][nr], aux))
					{
						valor[index] = StrToDouble(SGrid->Cells[nc][nr]);
						if (!ValidaValor(np, valor[index], coluna[nc - SGrid->FixedCols]))
							// Verificar
						{
							Erro("Valor fora do limite para no patamar " + IntToStr(hm_ini.hora) +
								":" + IntToStr(hm_ini.minuto) + " a " + IntToStr(hm_fim.hora) +
								":" + IntToStr(hm_fim.minuto));
							return (false);
						}
					}
					else
					{
						return (false);
					}
					nc++;
					index = (ind_fase * nvCURVA_PQVT) + indT; // valor de Teta
					if (ind_fase == indFASE_A)
					{
						valor[index] = 0;
					}
					else if (ind_fase == indFASE_B)
					{
						valor[index] = (-120.0 / 180.0) * M_PI;
					}
					else if (ind_fase == indFASE_C)
					{
						valor[index] = (120.0 / 180.0) * M_PI;
					}
				}
				// seta valores pro neutro
				valor[indPn] = 0; // P
				valor[indQn] = 0; // Q
				valor[indVn] = 0; // V
				valor[indTn] = 0; // Teta
				// atualiza Curva
				if (!curva->SetPonto(hm_ini, hm_fim, valor, curva->NumeroValores))
				{
					Erro("Dados incorretos para o patamar " + IntToStr(hm_ini.hora) + " a " +
						IntToStr(hm_fim.hora));
					return (false);
				}
			}
			break;
		case (supPQ - 1): // P/Q
			for (int nr = 1, np = 0; (nr < SGrid->RowCount) && (np < patamares->NumPatamar());
			nr++, np++)
			{
				patamar = patamares->Patamar[np];
				hm_ini.hora = patamar->HoraIni;
				hm_ini.minuto = patamar->MinutoIni;
				hm_fim.hora = patamar->HoraFim;
				hm_fim.minuto = patamar->MinutoFim;
				nc = SGrid->FixedCols;
				for (int ind_fase = indFASE_A; ind_fase <= indFASE_C; ind_fase++)
				{ // Fazer check se V possui valores diferentes de 0?
					index = (ind_fase * nvCURVA_PQVT) + indP; // valor de P
					if (StrToDouble(SGrid->Cells[nc][nr], aux))
					{
						valor[index] = StrToDouble(SGrid->Cells[nc][nr]);
						if (!ValidaValor(np, valor[index], coluna[nc - SGrid->FixedCols]))
							// Verificar
						{
							Erro("Valor fora do limite para no patamar " + IntToStr(hm_ini.hora) +
								":" + IntToStr(hm_ini.minuto) + " a " + IntToStr(hm_fim.hora) +
								":" + IntToStr(hm_fim.minuto));
							return (false);
						}
					}
					else
					{
						return (false);
					}
					nc++;
					index = (ind_fase * nvCURVA_PQVT) + indQ; // valor de Q
					if (StrToDouble(SGrid->Cells[nc][nr], aux))
					{
						valor[index] = StrToDouble(SGrid->Cells[nc][nr]);
						if (!ValidaValor(np, valor[index], coluna[nc - SGrid->FixedCols]))
							// Verificar
						{
							Erro("Valor fora do limite para no patamar " + IntToStr(hm_ini.hora) +
								":" + IntToStr(hm_ini.minuto) + " a " + IntToStr(hm_fim.hora) +
								":" + IntToStr(hm_fim.minuto));
							return (false);
						}
					}
					else
					{
						return (false);
					}
					nc++;
					index = (ind_fase * nvCURVA_PQVT) + indV; // valor de V
					valor[index] = 1.0;
					index = (ind_fase * nvCURVA_PQVT) + indT; // valor de Teta
					if (ind_fase == indFASE_A)
					{
						valor[index] = 0;
					}
					else if (ind_fase == indFASE_B)
					{
						valor[index] = (-120.0 / 180.0) * M_PI;
					}
					else if (ind_fase == indFASE_C)
					{
						valor[index] = (120.0 / 180.0) * M_PI;
					}
				}
				// seta valores pro neutro
				valor[indPn] = 0; // P
				valor[indQn] = 0; // Q
				valor[indVn] = 0; // V
				valor[indTn] = 0; // Teta
				// atualiza Curva
				if (!curva->SetPonto(hm_ini, hm_fim, valor, curva->NumeroValores))
				{
					Erro("Dados incorretos para o patamar " + IntToStr(hm_ini.hora) + " a " +
						IntToStr(hm_fim.hora));
					return (false);
				}
			}
			break;
		case (supVF - 1): // V/Teta
			for (int nr = 1, np = 0; (nr < SGrid->RowCount) && (np < patamares->NumPatamar());
			nr++, np++)
			{
				patamar = patamares->Patamar[np];
				hm_ini.hora = patamar->HoraIni;
				hm_ini.minuto = patamar->MinutoIni;
				hm_fim.hora = patamar->HoraFim;
				hm_fim.minuto = patamar->MinutoFim;
				nc = SGrid->FixedCols;
				for (int ind_fase = indFASE_A; ind_fase <= indFASE_C; ind_fase++)
				{ // Fazer check se V possui valores diferentes de 0?
					index = (ind_fase * nvCURVA_PQVT) + indP; // valor de P
					valor[index] = 0;
					index = (ind_fase * nvCURVA_PQVT) + indQ; // valor de Q
					valor[index] = 0;
					index = (ind_fase * nvCURVA_PQVT) + indV; // valor de V
					if (StrToDouble(SGrid->Cells[nc][nr], aux))
					{
						valor[index] = StrToDouble(SGrid->Cells[nc][nr]);
						if (!ValidaValor(np, valor[index], coluna[nc - SGrid->FixedCols]))
							// Verificar
						{
							Erro("Valor fora do limite para no patamar " + IntToStr(hm_ini.hora) +
								":" + IntToStr(hm_ini.minuto) + " a " + IntToStr(hm_fim.hora) +
								":" + IntToStr(hm_fim.minuto));
							return (false);
						}
					}
					else
					{
						return (false);
					}
					nc++;
					index = (ind_fase * nvCURVA_PQVT) + indT; // valor de Teta
					if (StrToDouble(SGrid->Cells[nc][nr], aux))
					{
						valor[index] = StrToDouble(SGrid->Cells[nc][nr]);
						valor[index] = DegToRad(valor[index]);
						if (!ValidaValor(np, valor[index], coluna[nc - SGrid->FixedCols]))
							// Verificar
						{
							Erro("Valor fora do limite para no patamar " + IntToStr(hm_ini.hora) +
								":" + IntToStr(hm_ini.minuto) + " a " + IntToStr(hm_fim.hora) +
								":" + IntToStr(hm_fim.minuto));
							return (false);
						}
					}
					else
					{
						return (false);
					}

					nc++;
				}
				// seta valores pro neutro
				valor[indPn] = 0; // P
				valor[indQn] = 0; // Q
				valor[indVn] = 0; // V
				valor[indTn] = 0; // Teta
				// verifica quão desequilibrada são as tensões (impossibilita o calculo do fluxo)
				if (!ValidaTensao(valor[indVa], valor[indTa], valor[indVb], valor[indTb],
					valor[indVc], valor[indTc]))
				{
					erroDeseq = true;
					msgErro = msgErro + "\nde " + IntToStr(hm_ini.hora) + " a " +
						IntToStr(hm_fim.hora);
				}
				// atualiza Curva
				if (!curva->SetPonto(hm_ini, hm_fim, valor, curva->NumeroValores))
				{
					Erro("Dados incorretos para o patamar " + IntToStr(hm_ini.hora) + " a " +
						IntToStr(hm_fim.hora));
					return (false);
				}
			}
			break;
		default:
			break;
		}
	}
	catch (Exception &e)
	{
		Erro("Valor Inválido: dados da curva");
		return (false);
	}
	// verifica se a Curva cobre o período de 24 horas
	if (!curva->ValidaPeriodo24Horas())
	{
		Erro("A curva deve cobrir o período de 24 horas");
		return (false);
	}
	if (erroDeseq)
	{
		msgErro = msgErro + "\nDeseja confirmar?";
		switch (Confirma(msgErro, "", MB_YESNOCANCEL))
		{
		case ID_YES:
			return (true);
		case ID_NO:
			return (false);
		case ID_CANCEL:
			return (false);
		default:
			return (true);
		}
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TFormEditaCurva3F::LeDadosPatamarVariavel(void)
{
	// variáveis locais
	int index, tipoSup, nc;
	strHM hm_ini, hm_fim;
	double aux;
	double valor[nvCURVA_RRSUP];
	bool erroDeseq;
	AnsiString msgErro;

	erroDeseq = false;
	msgErro = "Tensões com grande desequilíbrio para os patamares:";
	tipoSup = CBoxTipoSup->ItemIndex;
	if (tipoSup < 0)
	{
		tipoSup = (supPV - 1);
		CBoxTipoSup->ItemIndex = (supPV - 1);
	}

	// reinicia Curva
	curva->Reinicia();
	try
	{ // atualiza pontos da Curva
		for (int nr = 1, np = 0; (nr < SGrid->RowCount) && (np < patamares->NumPatamar());
		nr++, np++)
		{ // lê hora inicial/final do patamar
			if (!SGridExistePeriodo(nr))
				break;
			hm_ini.hora = StrToInteger(SGrid->Cells[0][nr]);
			hm_ini.minuto = 0;
			hm_fim.hora = StrToInteger(SGrid->Cells[1][nr]);
			hm_fim.minuto = 0;
			// reinicia vetor de valores
			for (index = 0; index < nvCURVA_RRSUP; index++)
			{
				valor[index] = 0.;
			}
			// lê valores do SGrid
			nc = 2;
			switch (tipoSup)
			{
			case (supPV - 1): // P/V
				for (int ind_fase = indFASE_A; ind_fase <= indFASE_C; ind_fase++)
				{ // Validar valores!!!
					index = (ind_fase * nvCURVA_PQVT) + indP; // valor de P
					if (StrToDouble(SGrid->Cells[nc][nr], aux))
					{
						valor[index] = StrToDouble(SGrid->Cells[nc][nr]);
						if (!ValidaValor(np, valor[index], coluna[nc - 2]))
							// menos 2 por causa das 2 colunas de hora inicio e fim
						{
							Erro("Valor fora do limite para no patamar " + IntToStr(hm_ini.hora) +
								":" + IntToStr(hm_ini.minuto) + " a " + IntToStr(hm_fim.hora) +
								":" + IntToStr(hm_fim.minuto));
							return (false);
						}
					}
					else
					{
						return (false);
					}
					nc++;
					index = (ind_fase * nvCURVA_PQVT) + indQ; // valor de Q
					valor[index] = 0;
					index = (ind_fase * nvCURVA_PQVT) + indV; // valor de V
					if (StrToDouble(SGrid->Cells[nc][nr], aux))
					{
						valor[index] = StrToDouble(SGrid->Cells[nc][nr]);
						if (!ValidaValor(np, valor[index], coluna[nc - 2]))
							////menos 2 por causa das 2 colunas de hora inicio e fim:
						{
							Erro("Valor fora do limite para no patamar " + IntToStr(hm_ini.hora) +
								":" + IntToStr(hm_ini.minuto) + " a " + IntToStr(hm_fim.hora) +
								":" + IntToStr(hm_fim.minuto));
						}
					}
					else
					{
						return (false);
					}
					nc++;
					index = (ind_fase * nvCURVA_PQVT) + indT; // valor de Teta
					if (ind_fase == indFASE_A)
					{
						valor[index] = 0;
					}
					else if (ind_fase == indFASE_B)
					{
						valor[index] = (-120.0 / 180.0) * M_PI;
					}
					else if (ind_fase == indFASE_C)
					{
						valor[index] = (120.0 / 180.0) * M_PI;
					}
				}
				break;
			case (supPQ - 1): // P/Q
				for (int ind_fase = indFASE_A; ind_fase <= indFASE_C; ind_fase++)
				{ // Validar valores!!!
					index = (ind_fase * nvCURVA_PQVT) + indP; // valor de P
					if (StrToDouble(SGrid->Cells[nc][nr], aux))
					{
						valor[index] = StrToDouble(SGrid->Cells[nc][nr]);
						if (!ValidaValor(np, valor[index], coluna[nc - SGrid->FixedCols]))
							// Verificar
						{
							Erro("Valor fora do limite para no patamar " + IntToStr(hm_ini.hora) +
								":" + IntToStr(hm_ini.minuto) + " a " + IntToStr(hm_fim.hora) +
								":" + IntToStr(hm_fim.minuto));
							return (false);
						}
					}
					else
					{
						return (false);
					}
					nc++;
					index = (ind_fase * nvCURVA_PQVT) + indQ; // valor de Q
					if (StrToDouble(SGrid->Cells[nc][nr], aux))
					{
						valor[index] = StrToDouble(SGrid->Cells[nc][nr]);
						if (!ValidaValor(np, valor[index], coluna[nc - 2]))
							// menos 2 por causa das 2 colunas de hora inicio e fim:
						{
							Erro("Valor fora do limite para no patamar " + IntToStr(hm_ini.hora) +
								":" + IntToStr(hm_ini.minuto) + " a " + IntToStr(hm_fim.hora) +
								":" + IntToStr(hm_fim.minuto));
							return (false);
						}
					}
					else
					{
						return (false);
					}
					nc++;
					index = (ind_fase * nvCURVA_PQVT) + indV; // valor de V
					valor[index] = 1.0;
					index = (ind_fase * nvCURVA_PQVT) + indT; // valor de Teta
					if (ind_fase == indFASE_A)
					{
						valor[index] = 0;
					}
					else if (ind_fase == indFASE_B)
					{
						valor[index] = (-120.0 / 180.0) * M_PI;
					}
					else if (ind_fase == indFASE_C)
					{
						valor[index] = (120.0 / 180.0) * M_PI;
					}
				}
				break;
			case (supVF - 1): // V/F
				for (int ind_fase = indFASE_A; ind_fase <= indFASE_C; ind_fase++)
				{ // Validar valores!!!
					index = (ind_fase * nvCURVA_PQVT) + indP; // valor de P
					valor[index] = 0;
					index = (ind_fase * nvCURVA_PQVT) + indQ; // valor de Q
					valor[index] = 0;
					index = (ind_fase * nvCURVA_PQVT) + indV; // valor de V
					if (StrToDouble(SGrid->Cells[nc][nr], aux))
					{
						valor[index] = StrToDouble(SGrid->Cells[nc][nr]);
						if (!ValidaValor(np, valor[index], coluna[nc - 2]))
							// menos 2 por causa das 2 colunas de hora inicio e fim:
						{
							Erro("Valor fora do limite para no patamar " + IntToStr(hm_ini.hora) +
								":" + IntToStr(hm_ini.minuto) + " a " + IntToStr(hm_fim.hora) +
								":" + IntToStr(hm_fim.minuto));
							return (false);
						}
					}
					else
					{
						return (false);
					}
					nc++;
					index = (ind_fase * nvCURVA_PQVT) + indT; // valor de Teta
					if (StrToDouble(SGrid->Cells[nc][nr], aux))
					{
						valor[index] = StrToDouble(SGrid->Cells[nc][nr]);
						valor[index] = DegToRad(valor[index]);
						if (!ValidaValor(np, valor[index], coluna[nc - 2]))
							// menos 2 por causa das 2 colunas de hora inicio e fim:
						{
							Erro("Valor fora do limite para no patamar " + IntToStr(hm_ini.hora) +
								":" + IntToStr(hm_ini.minuto) + " a " + IntToStr(hm_fim.hora) +
								":" + IntToStr(hm_fim.minuto));
							return (false);
						}
					}
					else
					{
						return (false);
					}
					nc++;
				}
				break;
			}
			// seta valores pro neutro
			valor[indPn] = 0; // P
			valor[indQn] = 0; // Q
			valor[indVn] = 0; // V
			valor[indTn] = 0; // Teta
			// verifica quão desequilibrada são as tensões (impossibilita o calculo do fluxo)
			if (!ValidaTensao(valor[indVa], valor[indTa], valor[indVb], valor[indTb], valor[indVc],
				valor[indTc]))
			{
				erroDeseq = true;
				msgErro = msgErro + "\nde " + IntToStr(hm_ini.hora) + " a " + IntToStr(hm_fim.hora);
			}
			// insere patamar na Curva
			if (!curva->SetPonto(hm_ini, hm_fim, valor, curva->NumeroValores))
			{
				Erro("Dados incorretos para o patamar " + IntToStr(hm_ini.hora) + " a " +
					IntToStr(hm_fim.hora));
				return (false);
			}
		}
	}
	catch (Exception &e)
	{
		Erro("Valor Inválido: dados da curva");
		return (false);
	}
	// verifica se a Curva cobre o período de 24 horas
	if (!curva->ValidaPeriodo24Horas())
	{
		Erro("A curva deve cobrir o período de 24 horas");
		return (false);
	}
	if (erroDeseq)
	{
		msgErro = msgErro + "\nDeseja confirmar?";
		switch (Confirma(msgErro, "", MB_YESNOCANCEL))
		{
		case ID_YES:
			return (true);
		case ID_NO:
			return (false);
		case ID_CANCEL:
			return (false);
		default:
			return (true);
		}
	}
	return (true);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditaCurva3F::SetSup(VTSuprimento *sup)
{
	psup = sup;
}

// ---------------------------------------------------------------------------
bool __fastcall TFormEditaCurva3F::SGridExistePeriodo(int nr)
{
	// verifica se a linha está preenchida
	for (int nc = 0; nc < 2; nc++)
	{
		if (SGrid->Cells[nc][nr].IsEmpty())
			return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditaCurva3F::SGridInicia(void)
{
	if (curva->Escala == patEDITAVEL)
		SGridIniciaPatamarVariavel();
	else
		SGridIniciaPatamarFixo();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditaCurva3F::SGridIniciaPatamarFixo(void)
{
	// variáveis locais
	int index, aux;
	strHM hm_ini, hm_fim;
	double valor[nvCURVA_RRSUP];
	VTPatamar *patamar;
	// limpa SGrid
	for (int nr = 1; nr < SGrid->RowCount; nr++)
		SGrid->Rows[nr]->Clear();
	// reinicia dimensão do SGrid
	SGrid->RowCount = patamares->NumPatamar() + 1;
	SGrid->ColCount = 1 + coluna.Length; // ARRUMAR!
	SGrid->FixedCols = 1;
	// preenche títulos das colunas
	SGrid->Cells[0][0] = "Patamar";
	// preenche dados dos pontos da Curva
	for (int nr = 1, np = 0; (nr < SGrid->RowCount) && (np < patamares->NumPatamar()); nr++, np++)
	{ // preenche hora/minuto do Patamar
		patamar = (VTPatamar*)patamares->Patamar[np];
		SGrid->Cells[0][nr] = patamar->Nome;
	}
	ExibeDados();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditaCurva3F::SGridIniciaPatamarVariavel(void)
{
	// variáveis locais
	int index, tipoSup, nc;
	AnsiString txt;
	strHM hm_ini, hm_fim;
	double valor[nvCURVA_RRSUP];

	tipoSup = CBoxTipoSup->ItemIndex;
	// verifica se há alguma opcao selecionada, caso não seleciona P/V
	if (tipoSup < 0)
	{
		tipoSup = (supPV - 1);
		CBoxTipoSup->ItemIndex = (supPV - 1);
	}
	// proteção
	if (curva->Escala != patEDITAVEL)
		return;
	// limpa SGrid
	for (int nr = 1; nr < SGrid->RowCount; nr++)
		SGrid->Rows[nr]->Clear();
	// reinicia dimensão do SGrid
	SGrid->RowCount = 24 + 1;
	SGrid->ColCount = 2 + coluna.Length;
	SGrid->FixedCols = 0;
	// preenche títulos das colunas
	SGrid->Cells[0][0] = "Hora início";
	SGrid->Cells[1][0] = "Hora fim";
	switch (tipoSup)
	{
	case (supPV - 1):
		SGrid->Cells[2][0] = "P_a";
		SGrid->Cells[3][0] = "V_a";
		SGrid->Cells[4][0] = "P_b";
		SGrid->Cells[5][0] = "V_b";
		SGrid->Cells[6][0] = "P_c";
		SGrid->Cells[7][0] = "V_c";
		// preenche dados dos pontos da Curva
		for (int nr = 1, np = 0; (nr < SGrid->RowCount) && (np < patamares->NumPatamar());
		nr++, np++)
		{ // obtém ponto da Curva p/ o Patamar
			if (!curva->GetPonto(np, hm_ini, hm_fim, valor, curva->NumeroValores))
				break;
			curva->GetPonto(np, hm_ini, hm_fim, valor, curva->NumeroValores);
			// preenche SGrid
			SGrid->Cells[0][nr] = txt.sprintf("%02d", hm_ini.hora);
			SGrid->Cells[1][nr] = txt.sprintf("%02d", hm_fim.hora);
			nc = 2; // coluna dos valores
			for (int ind_fase = indFASE_A; ind_fase <= indFASE_C; ind_fase++)
			{
				index = (ind_fase * nvCURVA_PQVT) + indP; // valor de P
				SGrid->Cells[nc][nr] = DoubleToStr("%7.4f", valor[index]);
				nc++;
				index = (ind_fase * nvCURVA_PQVT) + indV; // valor de V
				SGrid->Cells[nc][nr] = DoubleToStr("%7.4f", valor[index]);
				nc++;
			}
		}
		break;
	case (supPQ - 1):
		SGrid->Cells[2][0] = "P_a";
		SGrid->Cells[3][0] = "Q_a";
		SGrid->Cells[4][0] = "P_b";
		SGrid->Cells[5][0] = "Q_b";
		SGrid->Cells[6][0] = "P_c";
		SGrid->Cells[7][0] = "Q_c";
		// preenche dados dos pontos da Curva
		for (int nr = 1, np = 0; (nr < SGrid->RowCount) && (np < patamares->NumPatamar());
		nr++, np++)
		{ // obtém ponto da Curva p/ o Patamar
			if (!curva->GetPonto(np, hm_ini, hm_fim, valor, curva->NumeroValores))
				break;
			curva->GetPonto(np, hm_ini, hm_fim, valor, curva->NumeroValores);
			// preenche SGrid
			SGrid->Cells[0][nr] = txt.sprintf("%02d", hm_ini.hora);
			SGrid->Cells[1][nr] = txt.sprintf("%02d", hm_fim.hora);
			nc = 2; // coluna dos valores
			for (int ind_fase = indFASE_A; ind_fase <= indFASE_C; ind_fase++)
			{
				index = (ind_fase * nvCURVA_PQVT) + indP; // valor de P
				SGrid->Cells[nc][nr] = DoubleToStr("%7.4f", valor[index]);
				nc++;
				index = (ind_fase * nvCURVA_PQVT) + indQ; // valor de Q
				SGrid->Cells[nc][nr] = DoubleToStr("%7.4f", valor[index]);
				nc++;
			}
		}
		break;
	case (supVF - 1):
		SGrid->Cells[2][0] = "V_a";
		SGrid->Cells[3][0] = "Teta_a";
		SGrid->Cells[4][0] = "V_b";
		SGrid->Cells[5][0] = "Teta_b";
		SGrid->Cells[6][0] = "V_c";
		SGrid->Cells[7][0] = "Teta_c";
		// preenche dados dos pontos da Curva
		for (int nr = 1, np = 0; (nr < SGrid->RowCount) && (np < patamares->NumPatamar());
		nr++, np++)
		{ // obtém ponto da Curva p/ o Patamar
			if (!curva->GetPonto(np, hm_ini, hm_fim, valor, curva->NumeroValores))
				break;
			curva->GetPonto(np, hm_ini, hm_fim, valor, curva->NumeroValores);
			// preenche SGrid
			SGrid->Cells[0][nr] = txt.sprintf("%02d", hm_ini.hora);
			SGrid->Cells[1][nr] = txt.sprintf("%02d", hm_fim.hora);
			nc = 2; // coluna dos valores
			for (int ind_fase = indFASE_A; ind_fase <= indFASE_C; ind_fase++)
			{
				index = (ind_fase * nvCURVA_PQVT) + indV; // valor de V
				SGrid->Cells[nc][nr] = DoubleToStr("%6.5f", valor[index]);
				nc++;
				index = (ind_fase * nvCURVA_PQVT) + indT; // valor de Teta
				SGrid->Cells[nc][nr] = DoubleToStr("%4.3f", RadToDeg(valor[index]));
				nc++;
			}
		}
		break;
	}

}

// ---------------------------------------------------------------------------
bool __fastcall TFormEditaCurva3F::ValidaHora(int np, int hora)
{
	// valida hora
	if ((hora < 0) || (hora > 24))
	{
		Erro("Hora inválida no patamar " + IntToStr(np));
		return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TFormEditaCurva3F::ValidaTensao(double modA, double argA, double modB, double argB,
	double modC, double argC)
{
	complex<double>neutro;

	neutro = polar(modA, argA) + polar(modB, argB) + polar(modC, argC);
	if (abs(neutro) > 0.2)
	{
		return false;
	}
	else
	{
		return true;
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TFormEditaCurva3F::ValidaValor(int np, double valor, strCOLUNA &coluna)
{
	// valida valor mínimo
	if ((valor < coluna.valor_min) || (valor > coluna.valor_max))
	{
		Erro(coluna.caption + ": valor inválido no patamar " + IntToStr(np));
		return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
// eof
