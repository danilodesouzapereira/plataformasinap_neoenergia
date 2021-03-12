// ---------------------------------------------------------------------------
#pragma hdrstop
#include "TMostraGrafIndic.h"
#include "VTCalcIndCont.h"
#include "VTResCont.h"
#include "VTDefeito.h"

#include <PlataformaSinap\Fontes\Apl\VTApl.h>
#include <PlataformaSinap\Fontes\Rede\VTEqpto.h>
#include <PlataformaSinap\Fontes\Rede\VTChave.h>
#include <PlataformaSinap\Fontes\Bloco\VTBloco.h>
#include <PlataformaSinap\Fontes\Grafico\VTGrafico.h>

#include <PlataformaSinap\DLL_Inc\Funcao.h>
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
VTMostraGrafIndic* __fastcall NewObjMostraGrafIndic(VTApl* apl_owner)
{
	return (new TMostraGrafIndic(apl_owner));
}

// ---------------------------------------------------------------------------
__fastcall TMostraGrafIndic::TMostraGrafIndic(VTApl *apl_owner)
{
	this->apl = apl_owner;
	resCont = NULL;
}

// ---------------------------------------------------------------------------
__fastcall TMostraGrafIndic::~TMostraGrafIndic(void)
{

}

// ---------------------------------------------------------------------------
void __fastcall TMostraGrafIndic::MostraContribTransf(TList *lisDefeitos)
{
	// Mostra a contribuicao percentual das transferencias entre circuitos
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	VTDefeito *pdef, *pdefMax;
	TList *lisAux, *lisAux00;
	double valor0, valor1, valor2;
	TColor cor, cor0, cor1, cor2, corNULL;
	bool flag;
	double dcMax;

	cor0 = clRed;
	cor1 = clYellow;
	cor2 = clWhite;
	corNULL = clGray;

	lisAux00 = new TList();
	for (int ndef = 0; ndef < lisDefeitos->Count; ndef++)
	{
		pdef = (VTDefeito*)lisDefeitos->Items[ndef];
		// calcula a contribuicao da transferencia

		// if(pdef->ChvIsolaMont != NULL)
		// {
		// if(pdef->ChvIsolaMont->Codigo.AnsiCompare("045BF005018029") == 0)
		// int kkk = 0;
		// }
		// else
		// int jjjj = 0;
		pdef->ContribTransf();
		if (!IsDoubleZero(pdef->ganhoTransf))
			continue;
		// if(pdef->ganhoTransf > 0.0) continue;
		pdef->Bloco->Color = corNULL;
		lisAux00->Add(pdef);
	}

	lisAux = new TList();
	flag = true;
	while (flag)
	{
		flag = false;
		dcMax = -1;
		for (int ndef = 0; ndef < lisDefeitos->Count; ndef++)
		{
			pdef = (VTDefeito*)lisDefeitos->Items[ndef];
			if (lisAux00->IndexOf(pdef) != -1)
				continue;
			if (lisAux->IndexOf(pdef) != -1)
				continue;

			if (pdef->ganhoTransf > dcMax)
			{
				pdefMax = pdef;
				dcMax = pdef->ganhoTransf;
				flag = true;
			}
		}
		if (flag)
			lisAux->Add(pdefMax);
	}

	int iconta, icontaLim;
	iconta = 0;
	icontaLim = 0;
	cor = cor2;
	valor0 = 0.0;
	valor1 = 0.0;
	valor2 = 0.0;
	for (int ndef = 0; ndef < lisAux->Count; ndef++)
	{
		pdef = (VTDefeito*)lisAux->Items[ndef];
		if (ndef == 0)
			valor0 = pdef->ganhoTransf;
		pdef->Bloco->Color = cor;
		iconta++;
		if (iconta > lisAux->Count / 3)
		{
			icontaLim++;
			iconta = 0;
			if (icontaLim == 1)
			{
				cor = cor1;
				valor1 = pdef->ganhoTransf;
			}
			else
			{
				cor = cor0;
				valor2 = pdef->ganhoTransf;
			}
		}
	}

	grafico->Show(grafCONFIAB);
	delete lisAux;
	delete lisAux00;

	AnsiString aux;
	aux.sprintf("Contribuições de transferência(Clih)\n");
	aux.cat_sprintf("Vermelho\t: %6.f até %6.f\n", 0.0, valor2);
	aux.cat_sprintf("Amarelo\t: %6.f até %6.f\n", valor2, valor1);
	aux.cat_sprintf("Branco\t: %6.f até %6.f\n", valor1, valor0);
	Aviso(aux);
}

// ---------------------------------------------------------------------------
void __fastcall TMostraGrafIndic::MostraContribBloco(int indic, AnsiString titulo, double fatorEsc)
{
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	TList *lisAux, *lisAux00;
	strResBloco *resBloco, *resBlocoMax;
	TColor cor, cor0, cor1, cor2, corNULL;
	double valor0, valor1, valor2;
	VTBloco *bloco;
	bool flag;
	double dcMax;

	if (resCont == NULL)
		return;

	cor0 = clRed;
	cor1 = clYellow;
	cor2 = clGreen;
	corNULL = clGray;

	lisAux = new TList();
	lisAux00 = new TList();

	for (int n = 0; n < resCont->LisResBloco->Count; n++)
	{
		resBloco = (strResBloco*)resCont->LisResBloco->Items[n];
		if (!IsDoubleZero(resCont->Indic(resBloco, indic)))
			continue;
		lisAux00->Add(resBloco);
	}

	flag = true;
	while (flag)
	{
		flag = false;
		dcMax = -1;
		for (int n = 0; n < resCont->LisResBloco->Count; n++)
		{
			resBloco = (strResBloco*)resCont->LisResBloco->Items[n];
			if (lisAux00->IndexOf(resBloco) != -1)
				continue;
			if (lisAux->IndexOf(resBloco) != -1)
				continue;

			if (resCont->Indic(resBloco, indic) > dcMax)
			{
				resBlocoMax = resBloco;
				dcMax = resCont->Indic(resBloco, indic);
				flag = true;
			}
		}
		if (flag)
			lisAux->Add(resBlocoMax);
	}

	// Blocos com taxa zero
	for (int n = 0; n < lisAux00->Count; n++)
	{
		resBloco = (strResBloco*)lisAux00->Items[n];
		resBloco->bloco->Color = corNULL;
	}

	int iconta, icontaLim;
	iconta = 0;
	icontaLim = 0;
	cor = cor0;
	valor0 = 0.0;
	valor1 = 0.0;
	valor2 = 0.0;
	for (int n = 0; n < lisAux->Count; n++)
	{
		resBloco = (strResBloco*)lisAux->Items[n];
		if (n == 0)
			valor2 = resCont->Indic(resBloco, indic);
		resBloco->bloco->Color = cor;
		iconta++;
		if (iconta > lisAux->Count / 3)
		{
			icontaLim++;
			iconta = 0;
			if (icontaLim == 1)
			{
				cor = cor1;
				valor1 = resCont->Indic(resBloco, indic);
			}
			else
			{
				cor = cor2;
				valor0 = resCont->Indic(resBloco, indic);
			}
		}
	}

	grafico->Show(grafCONFIAB);
	delete lisAux;
	delete lisAux00;

	if (resCont->ResGeral->ncons > 0)
	{
		// valor0 /= resCont->ResGeral->ncons;
		// valor1 /= resCont->ResGeral->ncons;
		// valor2 /= resCont->ResGeral->ncons;

		valor0 *= fatorEsc;
		valor1 *= fatorEsc;
		valor2 *= fatorEsc;
	}

	AnsiString aux;
	aux.sprintf("%s\n", titulo.c_str());
	aux.cat_sprintf("Verde\t: %6.1f até %6.1f\n", 0.0, valor0);
	aux.cat_sprintf("Amarelo\t: %6.1f até %6.1f\n", valor0, valor1);
	aux.cat_sprintf("Vermelho\t: %6.1f até %6.1f\n", valor1, valor2);
	Aviso(aux);
}
// ---------------------------------------------------------------------------
