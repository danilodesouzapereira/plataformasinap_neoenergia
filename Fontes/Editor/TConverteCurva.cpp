//---------------------------------------------------------------------------

#pragma hdrstop

#include "TConverteCurva.h"
#include "..\Apl\VTApl.h"
#include "..\Constante\Const.h"
#include "..\Curvas\VTCurva.h"
#include "..\Patamar\VTPatamar.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTFases.h"
#include "..\..\DLL_Inc\Funcao.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)


// ---------------------------------------------------------------------------
__fastcall TConverteCurva::TConverteCurva(VTApl *apl)
{
	//salva apl
	this->apl = apl;
	//anula ponteiros
	eqpto = NULL;
	curva_tri = NULL;
    curva_fase = NULL;
}

// ---------------------------------------------------------------------------
__fastcall TConverteCurva::~TConverteCurva(void)
{
	//destroi clones da curva
	DestroiTObject(curva_fase);
	DestroiTObject(curva_tri);
}

// ---------------------------------------------------------------------------
void __fastcall TConverteCurva::DefineCurva(VTEqpto *eqpto, VTCurva *curva)
{
	// salva ponteiro p/ objeto
	this->eqpto = eqpto;
	//destroi clones se já existirem
	if(curva_fase != NULL)
		delete curva_fase;
	if(curva_tri != NULL)
		delete curva_tri;
	//cria clones da curva
	curva_fase = curva->Clone();
	curva_tri = curva->Clone();
}

// ---------------------------------------------------------------------------
int __fastcall TConverteCurva::DeterminaFases(void)
{
	//variaveis locais
	int fase_tag;
	VTCarga *carga;

	//determina fases do eqpto
	switch(eqpto->Tipo())
	{
		case eqptoCARGA:
			carga = (VTCarga*)eqpto;
			fase_tag = carga->Fases;
			break;
		case eqptoSUPRIMENTO:
		case eqptoGERADOR:
			fase_tag = faseABC;
			break;
		default: fase_tag = faseABC;
	}
	return fase_tag;
}

// ---------------------------------------------------------------------------
VTCurva* __fastcall TConverteCurva::FaseToTrifasica(void)
{
	//variaveis locais
	double valor[nvCURVA_RRCAR];
	double novo_valor[nvCURVA_RRCAR];
	strHM hm_ini, hm_fim;
	int num_fases, fase_tag;
	VTFases  *fases = (VTFases*)apl->GetObject(__classid(VTFases));
	VTPatamar *patamar;
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

	//verifica se já foi feita conversão
	if(curva_tri->NumeroValores != nvCURVA_RRCAR)
	{
		return curva_tri;
	}
	//altera número de valores conforme tipo
	curva_tri->NumeroValores = NumValores(curva_tri->Tipo);
	curva_tri->Reinicia();
	// reinicia vetor de valores
	for (int index = 0; index < nvCURVA_RRCAR; index++)
	{
		novo_valor[index] = 0.;
	}
	//determina fases do eqpto
	fase_tag = DeterminaFases();
	//não faz nada se for monofasico
//	num_fases = fases->NumeroDeFases(fase_tag);
//	if(num_fases == 1)
//		return curva_tri;
	for (int np = 0; np < patamares->NumPatamar(); np++)
	{ // determina Patamar
		patamar = patamares->Patamar[np];
		// determina valores da Curva no patamar
		hm_ini.hora = patamar->HoraIni;
		hm_ini.minuto = patamar->MinutoIni;
		hm_fim.hora = patamar->HoraFim;
		hm_fim.minuto = patamar->MinutoFim;
		if (!curva_fase->GetValor(hm_ini, valor, curva_fase->NumeroValores))
			continue;
		//calcula P e Q trifasico
		novo_valor[indP] = valor[0] + valor[2] + valor[4];
		novo_valor[indQ] = valor[1] + valor[3] + valor[5];
		//altera curva
		if (!curva_tri->SetPonto(hm_ini, hm_fim, novo_valor, curva_tri->NumeroValores))
			continue;
	}
	return curva_tri;
}

// ---------------------------------------------------------------------------
int __fastcall TConverteCurva::NumValores(int tipo)
{
	switch(tipo)
	{//determina numero de valores conforme tipo da curva
		case curvaPQVT:    return nvCURVA_PQVT;
		case curvaPQ:      return nvCURVA_PQ;
		case curvaPQ_MDP:  return nvCURVA_PQ_MDP;
		case curvaPFP:     return nvCURVA_PFP;
		case curvaPFP_MDP: return nvCURVA_PFP_MDP;
		case curvaLD:      return nvCURVA_LD;
		default:           return nvCURVA_PFP_MDP;
	}
}

// ---------------------------------------------------------------------------
VTCurva* __fastcall TConverteCurva::TrifasicaToFase(void)
{
	//variaveis locais
	double valor[nvCURVA_RRCAR];
	double novo_valor[nvCURVA_RRCAR];
	double p, q;
	strHM hm_ini, hm_fim;
	int num_fases, fase_tag;
	VTFases  *fases = (VTFases*)apl->GetObject(__classid(VTFases));
    VTPatamar *patamar;
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

    //verifica se já foi feita conversão
	if(curva_fase->NumeroValores == nvCURVA_RRCAR)
	{
		return curva_fase;
	}
	//altera número de valores
	curva_fase->NumeroValores = nvCURVA_RRCAR;
	curva_fase->Reinicia();
    // reinicia vetor de valores
	for (int index = 0; index < nvCURVA_RRCAR; index++)
	{
		novo_valor[index] = 0.;
	}
	//determina fases do eqpto
	fase_tag = DeterminaFases();
    //não faz nada se for monofasico
	num_fases = fases->NumeroDeFases(fase_tag);
//	if(num_fases == 1)
//		return curva_fase;
    for (int np = 0; np < patamares->NumPatamar(); np++)
	{ // determina Patamar
		patamar = patamares->Patamar[np];
		// determina valores da Curva no patamar
		hm_ini.hora = patamar->HoraIni;
		hm_ini.minuto = patamar->MinutoIni;
		hm_fim.hora = patamar->HoraFim;
		hm_fim.minuto = patamar->MinutoFim;
		if (!curva_tri->GetPonto(np, hm_ini, hm_fim, valor, curva_tri->NumeroValores))
			continue;
		//determina valor do patamar, considerando tipo de curva
		p = ValorPtri(valor);
		q = ValorQtri(valor);

		//calcula P e Q por fase
		for (int index = 0; index <= 4; index+=2)
		{
			if((index == 0) && (fase_tag & faseA))
			{
				novo_valor[index] = p / num_fases;
				novo_valor[index+1] = q / num_fases;
				continue;
			}
			else if((index == 2) && (fase_tag & faseB))
			{
				novo_valor[index] = p / num_fases;
				novo_valor[index+1] = q / num_fases;
				continue;
			}
			else if((index == 4) && (fase_tag & faseC))
			{
				novo_valor[index] = p / num_fases;
				novo_valor[index+1] = q / num_fases;
                continue;
			}
		}
		//altera curva
		if (!curva_fase->SetPonto(hm_ini, hm_fim, novo_valor, curva_fase->NumeroValores))
			continue;
	}
	return curva_fase;
}

// ---------------------------------------------------------------------------
double __fastcall TConverteCurva::Valor(int ind_fase, double valor_tri, int fase_tag)
{
	// variáveis locais
	double valor = 0.;
	int num_fases, fase;
	VTFases  *fases = (VTFases*)apl->GetObject(__classid(VTFases));

	//verifica quantidade de fases
	num_fases = fases->NumeroDeFases(fase_tag);
	//verifica fase solicitada
	fase = fases->IndexToTag[ind_fase];
	if(fase_tag & fase)
	{
		valor = valor_tri / num_fases;
	}
	return valor;
}

// ---------------------------------------------------------------------------
double __fastcall TConverteCurva::ValorPtri(double valor[nvCURVA_RRCAR])
{
	// variáveis locais
	double p = 0.;

	switch(curva_tri->Tipo)
	{//sempre vai ser o primeiro item do valor
		case curvaPFP_MDP:
        case curvaPQ:
		case curvaPQVT:
		case curvaPQ_MDP:
		case curvaPFP:
			p = valor[0];
			break;
		default: break;
	}
	return p;
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/17074295
double __fastcall TConverteCurva::ValorQtri(double valor[nvCURVA_RRCAR])
{
	// variáveis locais
	double q = 0.;
	double s, p, fp;

    switch(curva_tri->Tipo)
	{//vai ser segundo item do valor
		case curvaPQVT:
		case curvaPQ:
		case curvaPQ_MDP:
// jss20191121
//			p = valor[1];
			q = valor[1];
			break;
		case curvaPFP:
		case curvaPFP_MDP:
			//calcula s e q
			p = valor[0];
			fp = valor[1];
			//proteção
			if(IsDoubleZero(fp))
				break;
			s = p / fp;
			q = sqrt((s*s) - (p*p));
            break;
		default: break;
	}
	return q;
}

// ---------------------------------------------------------------------------

