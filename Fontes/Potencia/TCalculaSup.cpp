// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Math.hpp>
#include "TCalculaSup.h"
#include "..\Apl\VTApl.h"
#include "..\Curvas\VTCurva.h"
#include "..\Patamar\VTPatamar.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Rede\VTFases.h"
#include "..\Rede\VTGerador.h"
#include "..\Rede\VTSuprimento.h"
#include "..\..\DLL_Inc\Funcao.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
// função global p/ criar objeto da classe
// ---------------------------------------------------------------------------
VTCalculaSup* __fastcall NewObjCalculaSup(VTApl *apl)
{
	try
	{
		return (new TCalculaSup(apl));
	}
	catch (Exception &e)
	{ // nada a fazer
	}
	// erro na criação do objeto
	return (NULL);
}

// ---------------------------------------------------------------------------
__fastcall TCalculaSup::TCalculaSup(VTApl *apl)
{
	// salva ponteiro p/ objetos
	this->apl = apl;
	patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
}

// ---------------------------------------------------------------------------
__fastcall TCalculaSup::~TCalculaSup(void)
{
	// nada a fazer
}

// ---------------------------------------------------------------------------
bool __fastcall TCalculaSup::GetValorCurvaPQVT(VTCurva *curva, int np, strSUP *str_sup)
{
	// variáveis locais
	strHM hm;
	double valor[nvCURVA_PQVT];
	VTPatamar *patamar;

	// determina Patamar
	if ((patamar = patamares->Patamar[np]) == NULL)
		return (false);
	hm.hora = patamar->HoraCal;
	hm.minuto = patamar->MinutoCal;
	if (!curva->GetValor(hm, valor, (sizeof(valor) / sizeof(double))))
		return (false);
	str_sup->p = valor[0];
	str_sup->q = valor[1];
	str_sup->v = valor[2];
	// JCG FIX em 18/06/2010
	// str_sup->t = DegToRad(valor[3]);
	str_sup->t = valor[3];
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TCalculaSup::GetValorCurvaPQVTPorFase(VTCurva *curva, int np,
	strSUP vet_sup[MAX_FASE])
{
	// variáveis locais
	int offset;
	double valor[MAX_FASE * nvCURVA_PQVT];
	strHM hm;
	VTPatamar *patamar;

	// proteção
	if (curva == NULL)
	{
		return (false);
	}
	// determina Patamar
	if ((patamar = patamares->Patamar[np]) == NULL)
		return (false);
	hm.hora = patamar->HoraCal;
	hm.minuto = patamar->MinutoCal;
	// lê valores do ponto da Curva
	if (!curva->GetValor(hm, valor, (sizeof(valor) / sizeof(double))))
		return (false);
	// salva dados lidos da Curva
	offset = 0;
	for (int nf = indFASE_A; nf <= indNEUTRO; nf++)
	{ // preenche strSUP de cada fase
		vet_sup[nf].p = valor[offset + 0];
		vet_sup[nf].q = valor[offset + 1];
		vet_sup[nf].v = valor[offset + 2];
		vet_sup[nf].t = valor[offset + 3];
		offset += MAX_FASE;
	}
	return (true);
}

/*
 //---------------------------------------------------------------------------
 bool __fastcall TCalculaSup::GetValorGerador(VTGerador *gerador, int np, strSUP *str_sup)
 {
 return(GetValorCurvaPQVT(gerador->curva, np, str_sup));
 }
 */
// ---------------------------------------------------------------------------
bool __fastcall TCalculaSup::GetValorGerador(VTGerador *gerador, int np, strSUP *str_sup)
{
	if (!GetValorCurvaPQVT(gerador->curva, np, str_sup))
		return (false);
	// verifica se o gerador utiliza curva típica (com valores de P/Q em pu)
	if (gerador->curva->Tipica)
	{ // converte valores de P/Q de pu p/ MVA
		str_sup->p *= gerador->snom;
		str_sup->q *= gerador->snom;
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TCalculaSup::GetValorGeradorReducao(VTGerador *gerador, int np,
	strSUP vet_sup[MAX_FASE])
{
	return (GetValorCurvaPQVTPorFase(gerador->curva, np, vet_sup));
}

// ---------------------------------------------------------------------------
bool __fastcall TCalculaSup::GetValorSuprimento(VTSuprimento *suprimento, int np, strSUP *str_sup)
{
	if (!GetValorCurvaPQVT(suprimento->curva, np, str_sup))
		return (false);
	// verifica se o suprimento utiliza curva típica (com valores de P/Q em pu)
	if (suprimento->curva->Tipica)
	{ // converte valores de P/Q de pu p/ MVA
		str_sup->p *= suprimento->smax;
		str_sup->q *= suprimento->smax;
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TCalculaSup::GetValorSuprimentoReducao(VTSuprimento *suprimento, int np,
	strSUP vet_sup[MAX_FASE])
{
	return (GetValorCurvaPQVTPorFase(suprimento->curva, np, vet_sup));
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/index.cfm#/tasks/7056570
bool __fastcall TCalculaSup::SetValorCurvaPQVT(VTCurva *curva, int np, strSUP *str_sup)
{
	// variáveis locais
	double valor[nvCURVA_PQVT];
	strHM hm, hm_fim;
	VTPatamar *patamar, *patamar2;

	// inicia vetor a ser gravado
	valor[0] = str_sup->p;
	valor[1] = str_sup->q;
	valor[2] = str_sup->v;
	valor[3] = str_sup->t;
	// determina Patamar
	if ((patamar = patamares->Patamar[np]) == NULL)
		return (false);
	hm.hora = patamar->HoraCal;
	hm.minuto = patamar->MinutoCal;
    // tratamento especial para curva MMTN e LMP
	if ((patamares->Escala == patMMTN) || (patamares->Escala == patLMP))
	{ // verifica se é ultimo patamar
		if (patamares->NumPatamar() == (np + 1))
		{
			if ((patamar2 = patamares->Patamar[0]) == NULL)
				return (false);
		}
		else
		{
			if ((patamar2 = patamares->Patamar[np + 1]) == NULL)
				return (false);
		}
		// determina proximo patamar
		hm_fim.hora = patamar2->HoraCal;
		hm_fim.minuto = patamar2->MinutoCal;
		// atualiza valores do Ponto da Curva
		return (curva->SetPonto(hm, hm_fim, valor, (sizeof(valor) / sizeof(double))));
	}
	// atualiza valores do Ponto da Curva
	return (curva->SetPonto(hm, valor, (sizeof(valor) / sizeof(double))));
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/index.cfm#/tasks/7056570
bool __fastcall TCalculaSup::SetValorCurvaPQVTPorFase(VTCurva *curva, int np,
	strSUP vet_sup[MAX_FASE])
{
	// variáveis locais
	int offset;
	double valor[MAX_FASE * nvCURVA_PQVT];
	strHM hm, hm_fim;
	VTPatamar *patamar, *patamar2;

	// determina Patamar
	if ((patamar = patamares->Patamar[np]) == NULL)
		return (false);
	hm.hora = patamar->HoraCal;
	hm.minuto = patamar->MinutoCal;
	offset = 0;
	for (int nf = indFASE_A; nf <= indNEUTRO; nf++)
	{ // inicia vetor a ser gravado
		valor[offset + 0] = vet_sup[nf].p;
		valor[offset + 1] = vet_sup[nf].q;
		valor[offset + 2] = vet_sup[nf].v;
		valor[offset + 3] = vet_sup[nf].t;
		offset += MAX_FASE;
	}
	// tratamento especial para curva MMTN e LMP
	if ((patamares->Escala == patMMTN) || (patamares->Escala == patLMP))
	{ // verifica se é ultimo patamar
		if (patamares->NumPatamar() == (np + 1))
		{
			if ((patamar2 = patamares->Patamar[0]) == NULL)
				return (false);
		}
		else
		{
			if ((patamar2 = patamares->Patamar[np + 1]) == NULL)
				return (false);
		}
		// determina proximo patamar
		hm_fim.hora = patamar2->HoraCal;
		hm_fim.minuto = patamar2->MinutoCal;
		// atualiza valores do Ponto da Curva
		return (curva->SetPonto(hm, hm_fim, valor, (sizeof(valor) / sizeof(double))));
	}

	// atualiza valores do Ponto da Curva
	return (curva->SetPonto(hm, valor, (sizeof(valor) / sizeof(double))));
}

// ---------------------------------------------------------------------------
bool __fastcall TCalculaSup::SetValorGerador(VTGerador *gerador, int np, strSUP *str_sup)
{
	return (SetValorCurvaPQVT(gerador->curva, np, str_sup));
}

// ---------------------------------------------------------------------------
bool __fastcall TCalculaSup::SetValorGeradorReducao(VTGerador *gerador, int np,
	strSUP vet_sup[MAX_FASE])
{
	return (SetValorCurvaPQVTPorFase(gerador->curva, np, vet_sup));
}

// ---------------------------------------------------------------------------
bool __fastcall TCalculaSup::SetValorSuprimento(VTSuprimento *suprimento, int np, strSUP *str_sup)
{
	return (SetValorCurvaPQVT(suprimento->curva, np, str_sup));
}

// ---------------------------------------------------------------------------
bool __fastcall TCalculaSup::SetValorSuprimentoReducao(VTSuprimento *suprimento, int np,
	strSUP vet_sup[MAX_FASE])
{
	return (SetValorCurvaPQVTPorFase(suprimento->curva, np, vet_sup));
}

// ---------------------------------------------------------------------------
// eof
