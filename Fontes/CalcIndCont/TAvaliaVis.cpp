// ---------------------------------------------------------------------------
#pragma hdrstop
#include "TAvaliaVis.h"
#include "VTBuscaProt.h"
#include "VTVisRede.h"
#include "VTLigaCrit.h"

#include <PlataformaSinap\Fontes\Apl\VTApl.h>
#include <PlataformaSinap\Fontes\Arranjo\VTArranjo.h>
#include <PlataformaSinap\Fontes\Complexo\Complexo.h>
#include <PlataformaSinap\Fontes\Flow\VTFlow.h>
#include <PlataformaSinap\Fontes\Geral\VTGeral.h>
#include <PlataformaSinap\Fontes\Patamar\VTPatamares.h>
#include <PlataformaSinap\Fontes\Patamar\VTPatamar.h>
#include <PlataformaSinap\Fontes\Topologia\VTTopologia.h>
#include <PlataformaSinap\Fontes\Rede\VTBarra.h>
#include <PlataformaSinap\Fontes\Rede\VTCarga.h>
#include <PlataformaSinap\Fontes\Rede\VTEqbar.h>
#include <PlataformaSinap\Fontes\Rede\VTCapserie.h>
#include <PlataformaSinap\Fontes\Rede\VTGerador.h>
#include <PlataformaSinap\Fontes\Rede\VTRedes.h>
#include <PlataformaSinap\Fontes\Rede\VTRede.h>
#include <PlataformaSinap\Fontes\Rede\VTLigacao.h>
#include <PlataformaSinap\Fontes\Rede\VTMRede.h>
#include <PlataformaSinap\Fontes\Rede\VTEqpto.h>
#include <PlataformaSinap\Fontes\Rede\VTRegulador.h>
#include <PlataformaSinap\Fontes\Rede\VTTrafo.h>
#include <PlataformaSinap\Fontes\Rede\VTTrafo3E.h>
#include <PlataformaSinap\Fontes\Rede\VTTramo.h>
#include <PlataformaSinap\Fontes\Rede\VTChave.h>
#include <PlataformaSinap\Fontes\Rede\VTTrecho.h>
#include <PlataformaSinap\Fontes\Rede\VTResflowBar.h>
#include <PlataformaSinap\Fontes\Rede\VTResflowLig.h>
#include <PlataformaSinap\Fontes\Rede\VTResflowSup.h>
#include <PlataformaSinap\Fontes\Rede\VTSuprimento.h>
#include <PlataformaSinap\Fontes\Diretorio\VTPath.h>
#include <PlataformaSinap\DLL_Inc\Funcao.h>
// ---------------------------------------------------------------------------

#pragma package(smart_init)

// ---------------------------------------------------------------------------
// COMENTÁRIOS
// hk201608-Mudanca consideração de limite de chaves : Folga(double &dfolga, double &capNom, double &ifluxo, VTEqpto *pliga, int num_pat)
// hk201609-Sai função bool __fastcall Executa(TList *lisChvVis, bool flagFluxo = false);
// ---------------------------------------------------------------------------
VTAvaliaVis* __fastcall NewObjAvaliaVis(VTApl* apl)
{
	return (new TAvaliaVis(apl));
}

// ---------------------------------------------------------------------------
__fastcall TAvaliaVis::TAvaliaVis(VTApl *apl)
{
	this->apl = apl;
	// seta iPat para cálculo do fluxo em todos os patamares
	iPat = -1;
	// Seta avaliacao de chaves para true
	flagAvaliaChave = true;
}

// ---------------------------------------------------------------------------
__fastcall TAvaliaVis::~TAvaliaVis(void)
{
//      //gh201809: retorna para as configurações de fluxo originais
//		VTFlow *flow = (VTFlow*)apl->GetObject(__classid(VTFlow));
//		//retorna as configurações de fluxo para os valores default
//		flow->TipoFluxo = fluxoAUTO;
//		flow->ProgressoEnabled = true;

}

// ---------------------------------------------------------------------------
bool __fastcall TAvaliaVis::ExecutaFluxo(void)
{
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTFlow *flow = (VTFlow*)apl->GetObject(__classid(VTFlow));
	// VTTopologia *topologia =(VTTopologia*)apl->GetObject(__classid(VTTopologia));
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
	VTPatamar *patamar, *patamarFluxo;
	int numPat = patamares->NumPatamar();
	int tipoFlowOrig = flow->TipoFluxo;

	// Seta patamar
	if (!(iPat < 0))
	{
		// Habilita todos os patamares
		for (int n = 0; n < numPat; n++)
			patamares->Patamar[n]->Enabled = true;
		patamarFluxo = patamares->Patamar[iPat];
		patamarFluxo->Enabled = true;
		if (patamarFluxo != NULL)
		{
			for (int n = 0; n < numPat; n++)
			{
				patamar = patamares->Patamar[n];
				if (patamar == patamarFluxo)
					continue;
				patamar->Enabled = false;
			}
		}
		// Retorna com todos os patamares habilitados
		for (int n = 0; n < numPat; n++)
			patamares->Patamar[n]->Enabled = true;
		// Executa fluxo de potencia
		flow->TipoFluxo = fluxoNR1; // fluxoEQUILIBRADO;//;  fluxoRADIAL1;   gh201809: alterado para NR1
		flow->ProgressoEnabled = false;
		flow->UpToDate = false;
		flow->CalculaPorRede(redes, false);
	}
	else
	{
		// Executa fluxo de potencia
		flow->TipoFluxo = fluxoNR1; // fluxoEQUILIBRADO;//;  fluxoRADIAL1; gh201809: alterado para NR1
		flow->ProgressoEnabled = false;
		flow->UpToDate = false;
		flow->CalculaPorRede(redes, false);

		// calcula fluxo de potência
		// flow->HabilitaAviso(false);
		// flow->HabilitaOtimizacao(true);
		// flow->TipoRelatorio = relNENHUM;
		// flow->TipoFluxo     = fluxoEQUILIBRADO;
		// flow->UpToDate       = false;
		// flow->CalculaPorRede(redes, 0);
	}
	flow->TipoFluxo = tipoFlowOrig;
	flow->UpToDate = false;
    flow->ProgressoEnabled = true;

	// ImprimeResFluxo();
	return (true);
}

// ---------------------------------------------------------------------------
void __fastcall TAvaliaVis::ImprimeResFluxo(void)
{
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTLigacao *pliga;
	TList *lisLiga;
	FILE *fout;
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));
	AnsiString arq;

	arq = path->DirTmp() + "\\resFluxo.txt";
	fout = fopen(arq.c_str(), "wt");
	lisLiga = new TList();
	redes->LisLigacao(lisLiga);
	for (int n = 0; n < lisLiga->Count; n++)
	{
		pliga = (VTLigacao*)lisLiga->Items[n];
		fprintf(fout, "%s\t%6.2f\n", pliga->Codigo.c_str(), pliga->resflow->If_amp_max[0][18]);
	}
	delete lisLiga;
	fclose(fout);
}

// ---------------------------------------------------------------------------
bool __fastcall TAvaliaVis::ExecutaVisRede(TList *lisVisRede, bool flagFluxo)
{
	VTVisRede *visRede;
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
	VTEqpto *pliga;
	double folga, capNom, ifluxo;
	bool flagAux;
	strDelta *delta;

	if (flagFluxo)
	{
		if (!ExecutaFluxo())
			return (false);
	}

	// Busca ligacoes críticas
	for (int nv = 0; nv < lisVisRede->Count; nv++)
	{
		visRede = (VTVisRede*)lisVisRede->Items[nv];

		LimpaTList(visRede->ligaCrit1->lisDelta);
		LimpaTList(visRede->ligaCrit2->lisDelta);
		for (int npat = 0; npat < patamares->NumPatamar(); npat++)
		{
			if (!patamares->Patamar[npat]->Enabled)
				continue;
			visRede->ligaCrit1->lisDelta->Add(delta = new strDelta);
			delta->ipat = npat;
			flagAux = true;
			for (int nl = 0; nl < visRede->lisLiga1->Count; nl++)
			{
				pliga = (VTEqpto*)visRede->lisLiga1->Items[nl];
				Folga(folga, capNom, ifluxo, pliga, npat); // folga em corrente
				if ((flagAux) || (folga < delta->dMax))
				{
					delta->pliga = pliga;
					delta->dMax = folga;
					delta->ia_nom = capNom;
					delta->ifluxo = ifluxo;
					flagAux = false;
				}
			}

			visRede->ligaCrit2->lisDelta->Add(delta = new strDelta);
			delta->ipat = npat;
			flagAux = true;
			for (int nl = 0; nl < visRede->lisLiga2->Count; nl++)
			{
				pliga = (VTEqpto*)visRede->lisLiga2->Items[nl];
				Folga(folga, capNom, ifluxo, pliga, npat);
				if ((flagAux) || (folga < delta->dMax))
				{
					delta->pliga = pliga;
					delta->dMax = folga;
					delta->ia_nom = capNom;
					delta->ifluxo = ifluxo;
					flagAux = false;
				}
			}
		}
	}
	return (true);
}

// ---------------------------------------------------------------------------
void __fastcall TAvaliaVis::Folga(double &dfolga, double &capNom, double &ifluxo, VTEqpto *pliga,
	int num_pat)
{
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
	VTTrafo *ptrf;
	VTTrafo3E *ptrf3E;
	VTTramo *tramo;
	VTTrecho *ptre;
	VTChave *pchv;
	VTCapserie *capserie;
	VTGerador *pger;
	VTRegulador *regulador;
	VTSuprimento *psup;
	double iadm, daux, iflAux;
	bool flagAux;

	switch (pliga->Tipo())
	{
	case eqptoTRECHO:
		ptre = (VTTrecho*)pliga;
		flagAux = true;
		// Verifica folga entre os cabos das fases
		for (int nf = 0; nf < MAX_FASE - 1; nf++)
		{
			if ((iadm = ptre->arranjo->Ifase_amp[nf]) <= 0)
				continue;
			daux = iadm - (iflAux = Abs(ptre->resflow->If_amp[0][num_pat][nf]));
			if ((flagAux) || (daux < dfolga))
			{
				dfolga = daux;
				capNom = iadm;
				ifluxo = iflAux;
				flagAux = false;
			}
		}
		break;
	case eqptoCHAVE:
		if (flagAvaliaChave == true)
		{ // hkv201608-Mudanca consideração de limite de chaves : Folga(double &dfolga, double &capNom, double &ifluxo, VTEqpto *pliga, int num_pat)
			pchv = (VTChave*)pliga;
			flagAux = true;
			if ((iadm = pchv->Inom_amp) > 0)
			{
				for (int nf = 0; nf < MAX_FASE - 1; nf++)
				{
					daux = iadm - (iflAux = Abs(pchv->resflow->If_amp[0][num_pat][nf]));
					if ((flagAux) || (daux < dfolga))
					{
						dfolga = daux;
						capNom = iadm;
						ifluxo = iflAux;
						flagAux = false;
					}
				}
			}
		}
		else
		{
			// hkv201608-Mudanca consideração de limite de chaves
			pchv = (VTChave*)pliga;
			// dfolga = (geral->FLOW.I[1] - pchv->resflow->Cargto_pu[num_pat]) * pchv->Inom_amp;
			dfolga = 99999.0; // chaves não limitantes      PENDENCIA Chaves limitantes
			capNom = pchv->Inom_amp;
		}
		break;
	default:
		dfolga = 99999.0;
		/* case eqptoTRAFO:
		 ptrf = (VTTrafo*)pliga;
		 dfolga = geral->FLOW.I[1] - ptrf->resflow->Cargto_pu[num_pat];
		 break;
		 case eqptoTRAFO3E:
		 ptrf3E = (VTTrafo3E*)pliga;
		 dfolga = geral->FLOW.I[1] - ptrf3E->resflow->Cargto_pu[num_pat];
		 break;
		 case eqptoGERADOR:
		 pger = (VTGerador*)pliga;

		 dfolga = geral->FLOW.I[1] - pger->resflow->Cargto_pu[num_pat];
		 break;
		 case eqptoCAPSERIE:
		 capserie = (VTCapserie*)pliga;
		 dfolga = geral->FLOW.I[1] - pger->resflow->Cargto_pu[num_pat];
		 break;
		 case eqptoREGULADOR:
		 regulador = (VTRegulador*)pliga;
		 dfolga = geral->FLOW.I[1] - regulador->resflow->Cargto_pu[num_pat];
		 break;
		 case eqptoSUPRIMENTO:
		 psup = (VTSuprimento*)pliga;
		 dfolga = geral->FLOW.I[1] - psup->resflow->Cargto_pu[num_pat];
		 break;
		 */
	}
}
/* hk201609-Sai
 //---------------------------------------------------------------------------
 bool __fastcall TAvaliaVis::Executa(TList *lisChvVis, bool flagFluxo)
 {
 VTBuscaProt  *buscaProt = (VTBuscaProt*)apl->GetObject(__classid(VTBuscaProt));
 VTVisRede    *visRede;
 TList       *lisVisRede;
 bool        resposta;

 lisVisRede = new TList();
 for(int nv = 0; nv < buscaProt->LisVisRede->Count; nv++)
 {
 visRede = (VTVisRede*)buscaProt->LisVisRede->Items[nv];
 if(lisChvVis->IndexOf(visRede->pchave) == -1) continue;
 lisVisRede->Add(visRede);
 }
 resposta = ExecutaVisRede(lisVisRede, flagFluxo);
 delete lisVisRede;

 return(resposta);
 }
 */
// ---------------------------------------------------------------------------
