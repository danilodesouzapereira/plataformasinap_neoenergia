// ---------------------------------------------------------------------------
#pragma hdrstop
#include "TCalcEstratSoc.h"
#include "VTBuscaProt.h"
#include "VTVisRede.h"
#include "VTVisRedeSoc.h"
#include "VTBlocoRad.h"
#include "VTEstratSoc.h"
#include "VTDefeito.h"
#include <PlataformaSinap\Fontes\Rede\VTBarra.h>
#include <PlataformaSinap\Fontes\Rede\VTChave.h>
#include <PlataformaSinap\DLL_Inc\Funcao.h>
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
VTCalcEstratSoc* __fastcall NewObjCalcEstratSoc(void)
{
	return (new TCalcEstratSoc());
}

// ---------------------------------------------------------------------------
__fastcall TCalcEstratSoc::TCalcEstratSoc(void)
{
	lisRelSaida = NULL;
}

// ---------------------------------------------------------------------------
__fastcall TCalcEstratSoc::~TCalcEstratSoc(void)
{
}

// ---------------------------------------------------------------------------
bool __fastcall TCalcEstratSoc::IniciaEstrategiaSoc(VTDefeito *pdef, int iPat)
{
	VTEstratSoc *estratSoc;

	// Cria estratégia de socorro para pdef no patamar iPat
	estratSoc = pdef->EstratSoc[iPat];

	// PEND - é necessário iniciar a estratégia de socorro, estratSoc->Inicia(); ??

	// Adiciona bloco de defeito a lista de blocos desligados no reparo
	// estratSoc->lisBlocosReparo->Add(pdef->Bloco); //hk03 lisBlocosAgrega
	estratSoc->lisBlocosReparo->Assign(pdef->LisBlocosAgrega, laOr); // hk03 lisBlocosAgrega

	// Adiciona de blocos desligados a jusante do defeito no período de reparo
	estratSoc->lisBlocosReparo->Assign(pdef->LisBlocosJusDef, laOr);

	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TCalcEstratSoc::CalculaEstrategiaSoc(VTDefeito *pdef, int iPat, double pu_lim,
	TList *lisChavesAuto, TList *lisChavesAtivas)
{
	// Estrategias de socorro com busca de estrategias intermediárias em 2 etapa
	// de isolamento/ salvamento 1a rápida (máxima carga transferida), 2a lenta(blocos
	// remanescentes, e máxima transferência de carga)

	// Buscar entre a chave ou conjunto de chaves vis para socorrer circuitos
	// loop na lista de chaves vis e teste de transferencia
	VTEstratSoc *estratSoc;
	int iconta, indexBloco;
	double icirc, folga, pu_transf, cargaMw;
	///	VTBlocos	   *blocos = (VTBlocos*)apl->GetObject(__classid(VTBlocos));
	///	VTBloco     *blocoAux;
	///	VTChave     *pchave;
	// TCargaBloco *cBloco;
	VTVisRede *visRede;
	// TList       *lisBlRadJus;//, *lisAux;
	VTBlocoRad *blRad, *blRadAux;
	VTVisRedeSoc *visRedeSoc;
	bool flagAuto;

	this->lisChavesAuto = lisChavesAuto;
	this->lisChavesAtivas = lisChavesAtivas;
	this->pu_lim = pu_lim;

	// Cria estratégia de socorro para pdef no patamar iPat
	estratSoc = pdef->EstratSoc[iPat];

	// Inicializa estratSoc
	// estratSoc->iAmp_AdmLiga    = 0.0;
	// estratSoc->iAmp_Ligacao    = 0.0;
	// estratSoc->iAmp_Transf     = 0.0;
	// estratSoc->cargaMw_Transf  = 0.0;
	// estratSoc->puTransf        = 99.0;
	// estratSoc->lisBlocosSoc->Clear();
	// estratSoc->lisBlocosReparo->Clear();
	// LimpaTList(estratSoc->lisVisRedeSoc);

	//pdef->TReparo;  //hk201808: reformula: sai

	// Verifica se existe pelo menos uma estratégia de socorro
	if (pdef->LisVisRedeDef->Count == 0)
		return (false);
	// n14 if((pdef->LisVisRedeDef->Count == 0)||(!flagRecurso)) return(false);

	// Ordena pdef->LisVisRedeDef por capacidade em iPat, buscaProt->FolgaIA(pchave, iPat)
	// OrdenaChavesCap(pdef, iPat, flagAuto = true);
	flagAuto = true;
	OrdenaVisRedeCap(pdef, iPat, flagAuto); // verificar
	// OrdenaChavesDeltaDEC(pdef);

	// Cria lista de blocosRadJus
	// lisBlRadJus = new TList();

	// Verifica estratégias
	iconta = 0;
	// Verifica em ordem de capacidade as possibilidades de transferencia
	while ((iconta < pdef->LisVisRedeDef->Count) && (estratSoc->lisBlocosSoc->Count <
		pdef->LisBlocosMan->Count))
	{
		// pchave = (VTChave*)pdef->LisChvVis->Items[iconta];
		visRede = (VTVisRede*)pdef->LisVisRedeDef->Items[iconta];
		iconta++;
		// IniNova hkMar16
		if (lisChavesAtivas != NULL)
		{
			if (lisChavesAtivas->IndexOf(visRede->pchave) == -1)
				continue;
		}
		// Verifica se chave de transf. é automática
		// if(flagAutoSoc) {if(!buscaProt->ChaveAutomatica(pchave)) continue;}//Muda2
		// Identifica visRede
		// if((visRede = buscaProt->IdentificaVisRede(pchave)) == NULL) continue;
		// Verifica se a chave tem blocos a serem socorridos
		if (visRede->lisBlocosDefVis->Count == 0)
			continue;
		// Verifica a melhor opção com chaves automáticas
		VerificaVis(estratSoc, visRede);
	}
	// Remove blocos socorridos da lista de blocos desligados durante o reparo
	for (int nbl = 0; nbl < estratSoc->lisBlocosSoc->Count; nbl++)
		estratSoc->lisBlocosReparo->Remove(estratSoc->lisBlocosSoc->Items[nbl]);

	// delete lisBlRadJus; hkrel
	return (true);
}

// ---------------------------------------------------------------------------
void __fastcall TCalcEstratSoc::OrdenaVisRedeCap(VTDefeito *pdef, int iPat, bool flagAuto)
{
	// Ordenacao de chaves por automação seguido da folga de transferencia
	// VTChave *pchave, *pchaveMax;
	VTVisRede *visRede, *visRedeMax;
	TList *lisAux;
	double capMax, capAux, capAuto;
	// Ordena pdef->LisChvVis por capacidade em iPat, buscaProt->FolgaIA(pchave, iPat)
	lisAux = new TList();

	lisAux->Assign(pdef->LisVisRedeDef, laOr);
	if (flagAuto)
	{
		// capMax = -999.0;
		if (lisAux->Count > 0)
		{
			visRede = (VTVisRede*)lisAux->Items[0];
			capMax = visRede->FolgaIA(iPat);
		}

		for (int n = 0; n < lisAux->Count; n++)
		{
			visRede = (VTVisRede*)lisAux->Items[n];
			// n14 capAux = buscaProt->FolgaIA(visRede, iPat);
			capAux = visRede->FolgaIA(iPat);
			if (capAux > capMax)
			{
				capMax = capAux;
			}
		}
		capAuto = capMax + 1;
	}

	if (capAuto < 0)
		capAuto = -capAuto;

	pdef->LisVisRedeDef->Clear();
	while (lisAux->Count > 0)
	{
		// capMax = -999.0;
		visRede = (VTVisRede*)lisAux->Items[0];
		visRedeMax = visRede;
		capMax = visRede->FolgaIA(iPat);
		if (flagAuto)
		{
			if (ChaveAutomatica(visRede->pchave))
				capMax += capAuto;
		}
		for (int n = 0; n < lisAux->Count; n++)
		{
			visRede = (VTVisRede*)lisAux->Items[n];
			capAux = visRede->FolgaIA(iPat);
			// capAux = buscaProt->FolgaIA(visRede, iPat);
			if (flagAuto)
			{
				if (ChaveAutomatica(visRede->pchave))
					capAux += capAuto;
			}
			if (capAux > capMax)
			{
				visRedeMax = visRede;
				capMax = capAux;
			}
		}
		lisAux->Remove(visRedeMax);
		pdef->LisVisRedeDef->Add(visRedeMax);
	}
	delete lisAux;
}

// ---------------------------------------------------------------------------
void __fastcall TCalcEstratSoc::OrdenaVisRedeDeltaDEC(VTDefeito *pdef)
{
	VTVisRede *visRede, *visRedeMin;
	// VTChave   *pchave, *pchaveMin;
	VTBlocoRad *blRad, *blRadJus;
	double taux;
	int index;
	TList *lisAux;
	double decAux, decMin;
	// TList   *lisChvVis;

	double tman_manual = 10;
	double tman_auto = 1;
	// Ordenação por delta dec potencial

	// lisChvVis = pdef->LisChvVisDef;

	for (int nv = 0; nv < pdef->LisVisRedeDef->Count; nv++)
	{
		// pchave = (VTChave*)LisChvVis->Items[nv];
		// if((visRede = buscaProt->IdentificaVisRede(pchave)) == NULL) continue;
		visRede = (VTVisRede*)pdef->LisVisRedeDef->Items[nv];
		visRede->deltaDEC = 0.0;
		blRadJus = (VTBlocoRad*)visRede->lisBlocoRadIsolaJus->Last();
		blRad = NULL;
		// Busca a primeira chave automática montante->jusante
		for (int n = visRede->lisBlocoRadIsolaJus->Count - 1; n >= 0; n--)
		{
			// BlocoRad a jusante do bloco de defeito
			blRad = (VTBlocoRad*)visRede->lisBlocoRadIsolaJus->Items[n];
			if (ChaveAutomatica(blRad->Chave))
			{
				index = n;
				break;
			}
			blRad = NULL;
		}
		if (blRad != NULL)
		{ // Encontrou manobra rápida
			taux = tman_auto;
			if (ChaveAutomatica(visRede->pchave))
				taux += tman_auto;
			else
				taux += tman_manual;
			visRede->deltaDEC += (blRad->NCons * taux);
			// Manobra lenta
			if (index < visRede->lisBlocoRadIsolaJus->Count - 1)
			{
				taux = tman_auto + tman_manual;
				visRede->deltaDEC += (blRadJus->NCons - blRad->NCons) * taux;
			}
		}
		else
		{
			taux = tman_manual;
			if (ChaveAutomatica(visRede->pchave))
				taux += tman_auto;
			else
				taux += tman_manual;
			visRede->deltaDEC += blRadJus->NCons * taux;
		}

	}
	// Ordena alternativas de vis por deltaDEC

	lisAux = new TList();
	lisAux->Assign(pdef->LisVisRedeDef, laOr);
	pdef->LisVisRedeDef->Clear();
	while (lisAux->Count > 0)
	{
		// pchaveMin = (VTChave*)lisAux->First();
		// visRede = buscaProt->IdentificaVisRede(pchave);
		visRedeMin = (VTVisRede*)lisAux->First();
		decMin = visRede->deltaDEC;
		for (int n = 1; n < lisAux->Count; n++)
		{
			decAux = visRede->deltaDEC;
			if (decAux < decMin)
			{
				visRedeMin = visRede;
				decMin = decAux;
			}
		}
		lisAux->Remove(visRedeMin);
		pdef->LisVisRedeDef->Add(visRedeMin);
	}
	delete lisAux;
}

// ---------------------------------------------------------------------------
bool __fastcall TCalcEstratSoc::ChaveAutomatica(VTChave *pchv)
{
	bool resposta = false; // Default: chave automática
	if (lisChavesAuto != NULL)
	{
		resposta = (lisChavesAuto->IndexOf(pchv) != -1);
	}
	return (resposta);
}

// ---------------------------------------------------------------------------
bool __fastcall TCalcEstratSoc::ChaveAtiva(VTChave *pchv)
{
	bool resposta = true; // Default: a chave é ativa
	if (lisChavesAtivas != NULL)
	{
		resposta = (lisChavesAtivas->IndexOf(pchv) != -1);
	}
	return (resposta);
}

// ---------------------------------------------------------------------------
void __fastcall TCalcEstratSoc::VerificaVis(VTEstratSoc *estratSoc, VTVisRede *visRede)
{  //hk201808: parou: parou como guardar as segundas opções de atuação
	TList *lisBlRadJus, *lisAux;
	double icirc, folga, cargaMw, pu_transf;
	VTBlocoRad *blRad, *blRadRapida, *blRadLenta;
	int index;
	TStringList *strAux;

	icirc = visRede->IA_Crit(estratSoc->npat);
	folga = visRede->FolgaIA(estratSoc->npat);
	// n14	icirc = buscaProt->IA_Crit(visRede, estratSoc->npat);
	// n14	folga = buscaProt->FolgaIA(visRede, estratSoc->npat);

	// Inicia blocosRad
	blRadRapida = NULL;
	blRadLenta = NULL;

	// Sequencia rápida
	blRad = NULL;
	index = -1;
	// Verifica as possibilidades de transferencia a com isolamento partir da chave
	// da chave imediatamente a jusante do bloco de defeito
	for (int n = visRede->lisBlocoRadIsolaJus->Count - 1; n >= 0; n--)
	{
		// BlocoRad a jusante do bloco de defeito
		blRad = (VTBlocoRad*)visRede->lisBlocoRadIsolaJus->Items[n];
		// Verifica se a chave de isolamento é automática
		// chaves automáticas são ativas
		if (!ChaveAutomatica(blRad->Chave))
		{
			blRad = NULL;
			continue;
		}

		// Verifica se blRad já foi socorrido - impede formação de malha - em estratSoc->lisBlocosSoc
		if (estratSoc->BlocoSoc(blRad->Bloco))
		{
			blRad = NULL;
			continue;
		}
		for (int nb = 0; nb < blRad->LisBlocoJus->Count; nb++)
		{
			if (estratSoc->BlocoSoc((VTBloco*)blRad->LisBlocoJus->Items[nb]))
			{
				blRad = NULL;
				break;
			}
		}
		if (blRad == NULL)
			continue;

		cargaMw = blRad->CargaBlRad[estratSoc->npat];
		if (icirc > 0.0)
			pu_transf = (icirc - folga + 1000 * cargaMw / (visRede->pchave->pbarra1->vnom * 1.73205)
				) / icirc; // (pchave->pbarra1->vnom * 1.73205)) / icirc;
		else
			pu_transf = 99.0;

		if (lisRelSaida != NULL)
		{
			lisRelSaida->Add(strAux = new TStringList());
			strAux->AddObject(IntToStr(n), visRede->LigaCrit(estratSoc->npat));
			strAux->AddObject(visRede->pchave->Codigo, visRede->pchave);
			strAux->AddObject(blRad->Chave->Codigo, blRad->Chave);
			strAux->Add(DoubleToStr("%6.2f", icirc));
			strAux->Add(DoubleToStr("%6.2f", folga));
			strAux->Add(DoubleToStr("%6.2f",
				1000 * cargaMw / (visRede->pchave->pbarra1->vnom * 1.73205)));
			strAux->Add(DoubleToStr("%6.2f", cargaMw));
			strAux->Add(DoubleToStr("%6.2f", pu_transf));
			strAux->Add(DoubleToStr("%6.2f", pu_lim));
		}

		if (pu_transf <= pu_lim)
		{
			index = n + 1;
			break;
		}
		blRad = NULL;
	}
	// blRad é NULL ->não ha chaves automáticas, nenhuma automática é viável
	blRadRapida = blRad;

	if ((blRadRapida == NULL) && (index < 0))
		index = 0;

	if ((blRadRapida == NULL) || (index >= 0))
	{
		blRad = NULL;
		// busca operacao lenta
		for (int n = visRede->lisBlocoRadIsolaJus->Count - 1; n >= index; n--)
		{
			// BlocoRad a jusante do bloco de defeito
			blRad = (VTBlocoRad*)visRede->lisBlocoRadIsolaJus->Items[n];
			// Verifica se a chave de isolamento é não automática
			if (ChaveAutomatica(blRad->Chave))
			{
				blRad = NULL;
				continue;
			}
			if (!ChaveAtiva(blRad->Chave))
			{
				blRad = NULL;
				continue;
			} // hkaloca

			// Verifica se blRad já foi socorrido - impede formação de malha - em estratSoc->lisBlocosSoc
			if (estratSoc->BlocoSoc(blRad->Bloco))
			{
				blRad = NULL;
				continue;
			}
			for (int nb = 0; nb < blRad->LisBlocoJus->Count; nb++)
			{
				if (blRadRapida != NULL)
				{ // Verifica se bloco esta contido em manobra rápida
					if (blRadRapida->LisBlocoJus->IndexOf(blRad->LisBlocoJus->Items[nb]) != -1)
						continue;
				}
				if (estratSoc->BlocoSoc((VTBloco*)blRad->LisBlocoJus->Items[nb]))
				{
					blRad = NULL;
					break;
				}
			}
			if (blRad == NULL)
				continue;

			cargaMw = blRad->CargaBlRad[estratSoc->npat];
			if (icirc > 0.0)
				pu_transf =
					(icirc - folga + 1000 * cargaMw / (visRede->pchave->pbarra1->vnom * 1.73205))
					/ icirc; // (pchave->pbarra1->vnom * 1.73205)) / icirc;
			else
				pu_transf = 99.0;

			if (lisRelSaida != NULL)
			{
				lisRelSaida->Add(strAux = new TStringList());
				strAux->AddObject(IntToStr(n), visRede->LigaCrit(estratSoc->npat));
				strAux->AddObject(visRede->pchave->Codigo, visRede->pchave);
				strAux->AddObject(blRad->Chave->Codigo, blRad->Chave);
				strAux->Add(DoubleToStr("%6.2f", icirc));
				strAux->Add(DoubleToStr("%6.2f", folga));
				strAux->Add(DoubleToStr("%6.2f",
					1000 * cargaMw / (visRede->pchave->pbarra1->vnom * 1.73205)));
				strAux->Add(DoubleToStr("%6.2f", cargaMw));
				strAux->Add(DoubleToStr("%6.2f", pu_transf));
				strAux->Add(DoubleToStr("%6.2f", pu_lim));
			}

			if (pu_transf <= pu_lim)
			{
				index = n;
				break;
			}
			blRad = NULL;
		}
		blRadLenta = blRad;
	}
	// Adiciona estratégia de socorro
	estratSoc->AdicionaVisRedeSoc1(visRede, blRadRapida, blRadLenta);
}
// ---------------------------------------------------------------------------
