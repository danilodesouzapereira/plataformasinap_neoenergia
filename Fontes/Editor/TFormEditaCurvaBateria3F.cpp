// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Math.hpp>
#include "Complexo.h"
#include "TFormEditaCurvaBateria3F.h"
#include "TFormEdtBateria.h"
#include "..\Apl\VTApl.h"
#include "..\Constante\Fases.h"
#include "..\Constante\Const.h"
#include "..\Curvas\VTCurva.h"
#include "..\Curvas\VTCurvas.h"
#include "..\Patamar\VTPatamar.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Rede\VTBateria.h"
#include "..\Rede\VTFases.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Patamar.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormEditaCurvaBateria3F *FormEditaCurvaBateria3F;

// ---------------------------------------------------------------------------
__fastcall TFormEditaCurvaBateria3F::TFormEditaCurvaBateria3F(TComponent* Owner, TForm* form_owner, VTApl * apl_owner,
	TWinControl * parent) : TForm(Owner)
{
	this->apl = apl_owner;
	// salva ponteiro pro form pai (pra sincronizar combobox do tipoSup)
	// isso impossibilita o reuso desse form em outros lugares =(
	formEdtBateria = (TFormEdtBateria*) form_owner;
	// insere em seu Parent
	if (parent)
	{
		Parent = parent;
		Align = alClient;
	}
	// cria objeto Patamares e define sua escala
	patamares_local = DLL_NewObjPatamares(apl);
	// valores default
	SGrid->ColWidths[0] = 75;
	colunas = 1;
	fase_tag = -1;
	por_fase = false;
}

// ---------------------------------------------------------------------------
__fastcall TFormEditaCurvaBateria3F::~TFormEditaCurvaBateria3F(void)
{
	// destrói objetos criados
	if (patamares_local)
	{
		delete patamares_local;
		patamares_local = NULL;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditaCurvaBateria3F::CBoxNumPtoInicia(void)
{
	// verifica o número de pontos da Curva definido pela Curva
	switch (curva->NumeroPontos())
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
		curva->Escala = pat24HORA;
		curva->Reconfigura(curva->NumeroPontos());
		// chamada recursiva c/ Escala definida
		CBoxNumPtoInicia();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditaCurvaBateria3F::CBoxNumPtoChange(TObject *Sender)
{
	// redefine número de Patamares
	switch (CBoxNumPto->ItemIndex)
	{
	case 0:
		patamares_local->Escala = pat24HORA;
		break;
	case 1:
		patamares_local->Escala = pat12HORA;
		break;
	case 2:
		patamares_local->Escala = pat08HORA;
		break;
	case 3:
		patamares_local->Escala = pat06HORA;
		break;
	case 4:
		patamares_local->Escala = pat04HORA;
		break;
	case 5:
		patamares_local->Escala = pat03HORA;
		break;
	case 6:
		patamares_local->Escala = pat02HORA;
		break;
	case 7:
		patamares_local->Escala = pat01HORA;
		break;
	case 8:
		patamares_local->Escala = pat15MIN;
		break;
	case 9:
		patamares_local->Escala = pat10MIN;
		break;
	case 10:
		patamares_local->Escala = pat05MIN;
		break;
	default:
		patamares_local->Escala = patEDITAVEL;
		break;
	}
	// reconfigura a Curva (muda número de pontos, salvando seus valores originais)
	curva->Reconfigura(patamares_local->Escala);
	// reapresenta pontos da Curva
	// pcarga->DefinidaPorFase = (CBoxTipoCarga->ItemIndex == 1);
	SGridInicia();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditaCurvaBateria3F::CBoxTipoCargaChange(TObject *Sender)
{
	if (CBoxTipoCarga->ItemIndex == 0)
	{//trifasico
		TrataPorFaseParaTrifasico();
	}
	else
	{//por fase
		TrataTrifasicoParaPorFase();
	}
	SGridInicia();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditaCurvaBateria3F::DefineCurva(VTCurva *curva)
{
	// salva ponteiro p/ objeto
	this->curva = curva;
	// atualiza CBoxNumPto p/ exibir número de pontos da Curva
	CBoxNumPtoInicia();
	// redefine número de Patamares
	switch (curva->NumeroPontos())
	{
	case pat24HORA:
		patamares_local->Escala = pat24HORA;
		break;
	case pat12HORA:
		patamares_local->Escala = pat12HORA;
		break;
	case pat08HORA:
		patamares_local->Escala = pat08HORA;
		break;
	case pat06HORA:
		patamares_local->Escala = pat06HORA;
		break;
	case pat04HORA:
		patamares_local->Escala = pat04HORA;
		break;
	case pat03HORA:
		patamares_local->Escala = pat03HORA;
		break;
	case pat02HORA:
		patamares_local->Escala = pat02HORA;
		break;
	case pat01HORA:
		patamares_local->Escala = pat01HORA;
		break;
	case pat15MIN:
		patamares_local->Escala = pat15MIN;
		break;
	case pat10MIN:
		patamares_local->Escala = pat10MIN;
		break;
	case pat05MIN:
		patamares_local->Escala = pat05MIN;
		break;
	default:
		patamares_local->Escala = pat24HORA;
		curva->Reconfigura(patamares_local->Escala);
		break;
	}
	// reapresenta pontos da Curva
	SGridInicia();
	// desabilita edição p/ Curva típica
	Enabled = (!curva->Tipica);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditaCurvaBateria3F::ExibeDados(void)
{
	// variáveis locais
	VTPatamar *patamar;
	int col_fixa, dim;
	strHM hm_ini, hm_fim;
	AnsiString cabecalho;
	double valor[nvCURVA_RRCAR];

	if (CBoxNumPto->ItemIndex == 11)
		col_fixa = 2;
	else
		col_fixa = 1;

	// reinicia vetor de valores
	for (int index = 0; index < nvCURVA_RRCAR; index++)
	{
		valor[index] = 0.;
	}
	try
	{
		if (!por_fase)
		{ // Valores de carga trifasicos
			dim = nvCURVA_PQ;

			for (int nr = 1, np = 0; (nr < SGrid->RowCount) && (np < patamares_local->NumPatamar()); nr++, np++)
			{
				patamar = patamares_local->Patamar[np];
				hm_ini.hora = patamar->HoraIni;
				hm_ini.minuto = patamar->MinutoIni;
				curva->GetValor(hm_ini, valor, dim);
				SGrid->Cells[col_fixa][nr] = DoubleToStr("%9.7f", valor[indP]);
				SGrid->Cells[col_fixa + 1][nr] = DoubleToStr("%9.7f", valor[indQ]);
			}
		}
		else
		{ // Valores de carga por fase
			dim = nvCURVA_RRCAR;

			for (int nr = 1, np = 0; (nr < SGrid->RowCount) && (np < patamares_local->NumPatamar()); nr++, np++)
			{
				patamar = patamares_local->Patamar[np];
				hm_ini.hora = patamar->HoraIni;
				hm_ini.minuto = patamar->MinutoIni;
				curva->GetValor(hm_ini, valor, dim);
				// verifica fases disponíveis
				for (int nc = 0; (nc <= colunas - 2); nc += 2)
				{
					cabecalho = SGrid->Cells[col_fixa + nc][0];
					if (cabecalho.AnsiCompareIC("P_A") == 0)
					{
						SGrid->Cells[col_fixa + nc][nr] = DoubleToStr("%9.7f", valor[0]);
						SGrid->Cells[col_fixa + nc + 1][nr] = DoubleToStr("%9.7f", valor[1]);
					}
					else if (cabecalho.AnsiCompareIC("P_B") == 0)
					{
						SGrid->Cells[col_fixa + nc][nr] = DoubleToStr("%9.7f", valor[2]);
						SGrid->Cells[col_fixa + nc + 1][nr] = DoubleToStr("%9.7f", valor[3]);
					}
					else if (cabecalho.AnsiCompareIC("P_C") == 0)
					{
						SGrid->Cells[col_fixa + nc][nr] = DoubleToStr("%9.7f", valor[4]);
						SGrid->Cells[col_fixa + nc + 1][nr] = DoubleToStr("%9.7f", valor[5]);
					}
				}
			}
		}
	}
	catch (Exception &e)
	{
		Erro("Dados incorretos para o patamar " + IntToStr(hm_ini.hora));
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditaCurvaBateria3F::FormShow(TObject *Sender)
{
	//nada
}

// ---------------------------------------------------------------------------
bool __fastcall TFormEditaCurvaBateria3F::LePontosCurva(void)
{
	// variáveis locais
	TFase *fase;

	// dados gerais
	fase = (TFase*)formEdtBateria->CBoxFase->Items->Objects[formEdtBateria->CBoxFase->ItemIndex];
	fase_tag = fase->tag;
	// determina se é curva por fase ou trifasica
	por_fase = (curva->NumeroValores == nvCURVA_RRCAR);

	// verifica se Curva c/ patamares de período variável
	if (CBoxNumPto->ItemIndex == 11)
		return (LeDadosPatamarVariavel());
	else
		return (LeDadosPatamarFixo());
}

// ---------------------------------------------------------------------------
bool __fastcall TFormEditaCurvaBateria3F::LeDadosPatamarFixo(void)
{
	// variáveis locais
	strHM hm_ini, hm_fim;
	VTPatamar *patamar;
	AnsiString cabecalho;
	double valor[nvCURVA_RRCAR];

	// reinicia vetor de valores
	for (int index = 0; index < nvCURVA_RRCAR; index++)
	{
		valor[index] = 0.;
	}
	try
	{ // atualiza pontos da Curva
		for (int nr = 1, np = 0; (nr < SGrid->RowCount) && (np < patamares_local->NumPatamar());
		nr++, np++)
		{
			patamar = patamares_local->Patamar[np];
			hm_ini.hora = patamar->HoraIni;
			hm_ini.minuto = patamar->MinutoIni;
			hm_fim.hora = patamar->HoraFim;
			hm_fim.minuto = patamar->MinutoFim;
			if (!por_fase)
			{
				valor[indP] = StrToDouble(SGrid->Cells[1][nr]);
				valor[indQ] = StrToDouble(SGrid->Cells[2][nr]);
				if (!curva->SetPonto(hm_ini, hm_fim, valor, curva->NumeroValores))
				{
					Erro("Dados incorretos para o patamar " + IntToStr(hm_ini.hora) + " a " +
						IntToStr(hm_fim.hora));
					return (false);
				}
			}
			else
			{ // verifica fases disponíveis
				for (int nc = 1; nc < SGrid->ColCount; nc += 2)
				{
					cabecalho = SGrid->Cells[nc][0];
					if (cabecalho.AnsiCompareIC("P_A") == 0)
					{
						valor[0] = StrToDouble(SGrid->Cells[nc][nr]);
						valor[1] = StrToDouble(SGrid->Cells[nc + 1][nr]);
					}
					else if (cabecalho.AnsiCompareIC("P_B") == 0)
					{
						valor[2] = StrToDouble(SGrid->Cells[nc][nr]);
						valor[3] = StrToDouble(SGrid->Cells[nc + 1][nr]);
					}
					else if (cabecalho.AnsiCompareIC("P_C") == 0)
					{
						valor[4] = StrToDouble(SGrid->Cells[nc][nr]);
						valor[5] = StrToDouble(SGrid->Cells[nc + 1][nr]);
					}
				}
				// atualiza Curva
				if (!curva->SetPonto(hm_ini, hm_fim, valor, curva->NumeroValores))
				{
					Erro("Dados incorretos para o patamar " + IntToStr(hm_ini.hora) + " a " +
						IntToStr(hm_fim.hora));
					return (false);
				}
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
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TFormEditaCurvaBateria3F::LeDadosPatamarVariavel(void)
{
	// variáveis locais
	strHM hm_ini, hm_fim;
	VTPatamar *patamar;
	double valor[nvCURVA_RRCAR];
	AnsiString cabecalho;

	// reinicia vetor de valores
	for (int index = 0; index < nvCURVA_RRCAR; index++)
	{
		valor[index] = 0.;
	}

	try
	{ // atualiza pontos da Curva
		for (int nr = 1, np = 0; (nr < SGrid->RowCount) && (np < patamares_local->NumPatamar());
		nr++, np++)
		{ // lê hora inicial/final do patamar
			if (!SGridExistePeriodo(nr))
				break;
			hm_ini.hora = StrToInteger(SGrid->Cells[0][nr]);
			hm_ini.minuto = 0;
			hm_fim.hora = StrToInteger(SGrid->Cells[1][nr]);
			hm_fim.minuto = 0;
			if (!por_fase)
			{
				valor[indP] = StrToDouble(SGrid->Cells[2][nr]);
				valor[indQ] = StrToDouble(SGrid->Cells[3][nr]);
				if (!curva->SetPonto(hm_ini, hm_fim, valor, curva->NumeroValores))
				{
					Erro("Dados incorretos para o patamar " + IntToStr(hm_ini.hora) + " a " +
						IntToStr(hm_fim.hora));
					return (false);
				}
			}
			else
			{ // verifica fases disponíveis
				for (int nc = 2; nc < SGrid->ColCount; nc += 2)
				{
					cabecalho = SGrid->Cells[nc][0];
					if (cabecalho.AnsiCompareIC("P_A") == 0)
					{
						valor[0] = StrToDouble(SGrid->Cells[nc][nr]);
						valor[1] = StrToDouble(SGrid->Cells[nc + 1][nr]);
					}
					else if (cabecalho.AnsiCompareIC("P_B") == 0)
					{
						valor[2] = StrToDouble(SGrid->Cells[nc][nr]);
						valor[3] = StrToDouble(SGrid->Cells[nc + 1][nr]);
					}
					else if (cabecalho.AnsiCompareIC("P_C") == 0)
					{
						valor[4] = StrToDouble(SGrid->Cells[nc][nr]);
						valor[5] = StrToDouble(SGrid->Cells[nc + 1][nr]);
					}
				}
				// atualiza Curva
				if (!curva->SetPonto(hm_ini, hm_fim, valor, curva->NumeroValores))
				{
					Erro("Dados incorretos para o patamar " + IntToStr(hm_ini.hora) + " a " +
						IntToStr(hm_fim.hora));
					return (false);
				}
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
	return (true);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditaCurvaBateria3F::SGridInicia(void)
{
	// Variáveis locais
	VTFases *fases = (VTFases*)apl->GetObject(__classid(VTFases));
	TFase *fase;
	int num_fases, num_colunas;

	// determina fases disponiveis
	fase = (TFase*)formEdtBateria->CBoxFase->Items->Objects[formEdtBateria->CBoxFase->ItemIndex];
	if (fase == NULL)
		return;
	fase_tag = fase->tag;
	num_fases = fases->NumeroDeFases(fase_tag);
	// determina se é curva por fase ou trifasica
	por_fase = (curva->NumeroValores == nvCURVA_RRCAR);

	if (CBoxTipoCarga->ItemIndex < 0)
	{
		CBoxTipoCarga->ItemIndex = (por_fase ? 1 : 0);
	}
	if (fase_tag < 0)
	{
		fase_tag = 0;
		formEdtBateria->CBoxFase->ItemIndex = 0;
	}

	if (CBoxNumPto->ItemIndex == 11)
	{ // editavel
		if (!por_fase)
		{ // trifasica
			colunas = 4; // hora ini, hora fim, P e Q
		}
		else
		{ // por fase
			colunas = 2 + (num_fases * 2); // hora ini, hora fim, P e Q por fase
		}
		SGridIniciaPatamarVariavel();
	}

	else
	{ // fixo
		if (!por_fase)
		{ // trifasica
			colunas = 3; // patamar, P e Q
		}
		else
		{ // fixo e por fase
			colunas = 1 + (num_fases * 2); // patamar, P e Q por fase
		}
		SGridIniciaPatamarFixo();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditaCurvaBateria3F::SGridIniciaPatamarFixo(void)
{
	// variáveis locais
	VTPatamar *patamar;
	VTFases *fases = (VTFases*)apl->GetObject(__classid(VTFases));
	int tag;

	// limpa SGrid
	for (int nr = 1; nr < SGrid->RowCount; nr++)
		SGrid->Rows[nr]->Clear();
	// reinicia dimensão do SGrid
	SGrid->RowCount = patamares_local->NumPatamar() + 1;
	SGrid->ColCount = colunas;
	SGrid->FixedCols = 1;
	// preenche títulos das colunas
	SGrid->Cells[0][0] = "Patamar";
	if (!por_fase)
	{
		SGrid->Cells[1][0] = "P";
		SGrid->Cells[2][0] = "Q";
	}
	else
	{ // Numero de fases para P e Q mais uma coluna para os patamares
		for (int ind_fase = indFASE_A, col_fase = 1; ind_fase <= indFASE_C; ind_fase++)
		{
			tag = fases->IndexToTag[ind_fase];
			if ((tag & fase_tag) == tag)
			{
				SGrid->Cells[col_fase][0] = "P_" + fases->AsStrABCN[tag];
				SGrid->Cells[col_fase + 1][0] = "Q_" + fases->AsStrABCN[tag];
				col_fase += 2;
			}
		}
	}
	// preenche dados dos patamares
	for (int nr = 1, np = 0; (nr < SGrid->RowCount) && (np < patamares_local->NumPatamar()); nr++, np++)
	{ // preenche hora/minuto do Patamar
		patamar = patamares_local->Patamar[np];
		SGrid->Cells[0][nr] = patamar->Nome;
	}
	// insere valores da curva
	ExibeDados();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditaCurvaBateria3F::SGridIniciaPatamarVariavel(void)
{
	// variáveis locais
	VTFases *fases = (VTFases*)apl->GetObject(__classid(VTFases));
	VTPatamar *patamar;
	int tag;
	AnsiString txt;

	// limpa SGrid
	for (int nr = 1; nr < SGrid->RowCount; nr++)
		SGrid->Rows[nr]->Clear();
	// reinicia dimensão do SGrid
	SGrid->RowCount = patamares_local->NumPatamar() + 1;
	SGrid->ColCount = colunas;
	SGrid->FixedCols = 2;
	// preenche títulos das colunas
	SGrid->Cells[0][0] = "Hora início";
	SGrid->Cells[1][0] = "Hora fim";
	if (!por_fase)
	{
		SGrid->Cells[2][0] = "P";
		SGrid->Cells[3][0] = "Q";
	}
	else
	{ // Numero de fases para P e Q mais 2 colunas para hora inicial e final
		for (int ind_fase = indFASE_A, col_fase = 2; ind_fase <= indFASE_C; ind_fase++)
		{
			tag = fases->IndexToTag[ind_fase];
			if ((tag & fase_tag) == tag)
			{
				SGrid->Cells[col_fase][0] = "P_" + fases->AsStrABCN[tag];
				SGrid->Cells[col_fase + 1][0] = "Q_" + fases->AsStrABCN[tag];
				col_fase += 2;
			}
		}
	}
	// preenche dados dos patamares
	for (int nr = 1, np = 0; (nr < SGrid->RowCount) && (np < patamares_local->NumPatamar()); nr++, np++)
	{ // preenche hora inicial e final do Patamar
		patamar = patamares_local->Patamar[np];
		SGrid->Cells[0][np + 1] = txt.sprintf("%02d", patamar->HoraIni);
		SGrid->Cells[1][np + 1] = txt.sprintf("%02d", patamar->HoraFim);
	}
	// insere valores da curva
	ExibeDados();
}

// ---------------------------------------------------------------------------
bool __fastcall TFormEditaCurvaBateria3F::SGridExistePeriodo(int nr)
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
void __fastcall TFormEditaCurvaBateria3F::TrataPorFaseParaTrifasico(void)
{
	//variaveis locais
	int max_valor = 6;
	double valor[nvCURVA_RRCAR];
	double p, q;
	strHM hm;
	VTPatamar *patamar;
	VTCurva *clone;

	//verifica se a curva já é PQ
	if(curva->NumeroValores == nvCURVA_PQ)
		return;

	//clona curva para que os valores do clone sejam calculados para a curva
	clone = curva->Clone();
	curva->NumeroValores = nvCURVA_PQ;
	curva->Reconfigura(curva->NumeroPontos());
	for(int np = 0; np < curva->NumeroPontos(); np++)
	{
		patamar = patamares_local->Patamar[np];
		hm.hora = patamar->HoraIni;
		hm.minuto = patamar->MinutoIni;
		clone->GetValor(hm, valor, nvCURVA_RRCAR);
		p = q = 0.;
		for(int n = 0; n < max_valor; n+=2)
		{//soma valores
			p += valor[n];
			q += valor[n+1];
		}
		curva->SetPonto(hm, valor, nvCURVA_PQ);
	}
	//destroi clone
	delete clone;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEditaCurvaBateria3F::TrataTrifasicoParaPorFase(void)
{
	//variaveis locais
	VTFases *fases = (VTFases*)apl->GetObject(__classid(VTFases));
	//int max_valor = 6;
	int num_fase;
	double valor[nvCURVA_RRCAR];
	double p, q;
	strHM hm;
	VTPatamar *patamar;
	VTCurva *clone;

	//verifica se a curva já é Por Fase
	if(curva->NumeroValores == nvCURVA_RRCAR)
		return;

	//determina numero de fases total
	num_fase = fases->NumeroDeFases(fase_tag);
	if(num_fase == 0)
		return;

	//clona curva para que os valores do clone sejam calculados para a curva
	clone = curva->Clone();
	curva->NumeroValores = nvCURVA_RRCAR;
	curva->Reconfigura(curva->NumeroPontos());
	for(int np = 0; np < curva->NumeroPontos(); np++)
	{
		patamar = patamares_local->Patamar[np];
		hm.hora = patamar->HoraIni;
		hm.minuto = patamar->MinutoIni;
		clone->GetValor(hm, valor, nvCURVA_PQ);
		//calcula valor em função da quantidade de fases (equilibrado)
		p = (valor[indP] / num_fase);
		q = (valor[indQ] / num_fase);
		//insere valor
		valor[0] = (fase_tag & faseA) ? p : 0.;
		valor[1] = (fase_tag & faseA) ? q : 0.;
		valor[2] = (fase_tag & faseB) ? p : 0.;
		valor[3] = (fase_tag & faseB) ? q : 0.;
		valor[4] = (fase_tag & faseC) ? p : 0.;
		valor[5] = (fase_tag & faseC) ? q : 0.;
		curva->SetPonto(hm, valor, nvCURVA_RRCAR);
	}
	//destroi clone
	delete clone;
}

//---------------------------------------------------------------------------

