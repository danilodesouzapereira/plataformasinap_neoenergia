//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <algorithm>
#include <Filectrl.hpp>
#include <math.h>
#include "TDiag.h"
#include "..\..\Apl\VTApl.h"
#include "..\..\Arranjo\VTArranjo.h"
#include "..\..\Complexo\Complexo.h"
#include "..\..\Consulta\VTConsulta.h"
#include "..\..\Geral\VTGeral.h"
#include "..\..\Geral\VTPadraoRede.h"
#include "..\..\Patamar\VTPatamar.h"
#include "..\..\Patamar\VTPatamares.h"
#include "..\..\Rede\VTBarra.h"
#include "..\..\Rede\VTCapserie.h"
#include "..\..\Rede\VTChave.h"
#include "..\..\Rede\VTFases.h"
#include "..\..\Rede\VTGerador.h"
#include "..\..\Rede\VTNo.h"
#include "..\..\Rede\VTRegulador.h"
#include "..\..\Rede\VTResflowBar.h"
#include "..\..\Rede\VTResflowLig.h"
#include "..\..\Rede\VTResflowSup.h"
#include "..\..\Rede\VTSuprimento.h"
#include "..\..\Rede\VTTrafo.h"
#include "..\..\Rede\VTTrafo3E.h"
#include "..\..\Rede\VTTrecho.h"
#include "..\..\Rede\VTTramo.h"
#include "..\..\Rede\VTNet.h"
#include "..\..\Rede\VTRede.h"
#include "..\..\Rede\VTRedes.h"
#include "..\..\..\DLL_Inc\Consulta.h"
#include "..\..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ criar objeto VTDiag
VTDiag* NewObjDiag(VTApl *apl)
   {
   return(new TDiag(apl));
   }

//---------------------------------------------------------------------------
__fastcall TDiag::TDiag(VTApl *apl)
   {
   //salva ponteiro p/ objeto
   this->apl = apl;
   geral     = (VTGeral*)apl->GetObject(__classid(VTGeral));
   patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   //cria objetos
   lisEQP = new TList();
   consulta = DLL_NewObjConsulta(apl);
   //monta map
   capacidade_max.insert(pair<double, double>(5.00, 1.60));
   capacidade_max.insert(pair<double, double>(6.25, 1.35));
   capacidade_max.insert(pair<double, double>(7.50, 1.20));
   capacidade_max.insert(pair<double, double>(8.75, 1.10));
   capacidade_max.insert(pair<double, double>(10.0, 1.00));
   }

//---------------------------------------------------------------------------
__fastcall TDiag::~TDiag(void)
   {
   //destrói objetos
	if (consulta)
	{
		delete consulta;
		consulta = NULL;
	}
   //destrói lista sem destruir objetos
	if (lisEQP)
	{
		delete lisEQP;
		lisEQP = NULL;
   }
}

////---------------------------------------------------------------------------
//void __fastcall TDiag::DiagBarra(TList *lisBAR)
//   {
//   //variáveis locais
//   int     max_pat;
//   double  vpu;
//   VTBarra *pbar;
//
//   //determina número de patamares
//   max_pat = patamares->NumPatamar();
//   //loop p/ todas Barras
//   for (int n = 0; n < lisBAR->Count; n++)
//      {
//	  pbar = (VTBarra*)lisBAR->Items[n];
//      for (int npat = 0; npat < max_pat; npat++)
//         {//verifica se fluxo foi calculado p/ o Patamar
//         if (! patamares->Patamar[npat]->FluxoCorreto)
//            {//barra isolada
//            pbar->resflow->Diagnostico[npat] = diagISOLADO;
//			continue;
//			}
//		 //verifica se Barra isolada
//		 if (IsDoubleZero(pbar->resflow->Vfn_pu_max[npat]))
//            {//barra isolada
//			pbar->resflow->Diagnostico[npat] = diagISOLADO;
//			continue;
//			}
//		 //assume diagnóstico Bom
//		 pbar->resflow->Diagnostico[npat] = diagBOM;
//		 //proteção: verifica se a Vnom da Barra foi definida
//		 if (pbar->vnom > 0)
//			{//determina tensão pu da Barra no patamar
//			vpu = pbar->resflow->Vfn_pu_min[npat];
//			if      (vpu < geral->FLOW.V[0]) pbar->resflow->Diagnostico[npat] = diagRUIM;
//			else if (vpu < geral->FLOW.V[1]) pbar->resflow->Diagnostico[npat] = diagREGULAR;
//			else if (vpu < geral->FLOW.V[2]) pbar->resflow->Diagnostico[npat] = diagBOM;
//			else if (vpu < geral->FLOW.V[3]) pbar->resflow->Diagnostico[npat] = diagREGULAR;
//			else                             pbar->resflow->Diagnostico[npat] = diagRUIM;
//
//			}
//		 }
//	  }
//   }
/*
//---------------------------------------------------------------------------
void __fastcall TDiag::DiagBarra(TList *lisBAR)
   {
   //variáveis locais
   int     max_pat;
   double  vpu;
   VTBarra *pbar;
   int	tipoSegmento = 0;
   VTPadraoRede *padraoRede;

   //determina número de patamares
   max_pat = patamares->NumPatamar();
   //loop p/ todas Barras
   for (int n = 0; n < lisBAR->Count; n++)
      {
	  pbar = (VTBarra*)lisBAR->Items[n];
      for (int npat = 0; npat < max_pat; npat++)
         {//verifica se fluxo foi calculado p/ o Patamar
         if (! patamares->Patamar[npat]->FluxoCorreto)
            {//barra isolada
            pbar->resflow->Diagnostico[npat] = diagISOLADO;
			continue;
			}
		 //verifica se Barra isolada
		 if (IsDoubleZero(pbar->resflow->Vfn_pu_max[npat]))
            {//barra isolada
			pbar->resflow->Diagnostico[npat] = diagISOLADO;
			continue;
			}
		 //assume diagnóstico Bom
		 pbar->resflow->Diagnostico[npat] = diagBOM;
		 //proteção: verifica se a Vnom da Barra foi definida
		 if (pbar->vnom > 0)
			{//determina tensão pu da Barra no patamar
			vpu = pbar->resflow->Vfn_pu_min[npat];
			tipoSegmento = geral->TipoSegmentoNivelTensao(pbar);
			padraoRede = geral->ExistePadraoRede(tipoSegmento);
			if(padraoRede != NULL)
			{
 if (vpu < padraoRede->V1)
 pbar->resflow->Diagnostico[npat] = diagRUIM;
 else if (vpu < padraoRede->V2)
 pbar->resflow->Diagnostico[npat] = diagREGULAR;
 else if (vpu < padraoRede->V3)
 pbar->resflow->Diagnostico[npat] = diagBOM;
 else if (IsDoubleZero(padraoRede->V3 - vpu))
 {
 pbar->resflow->Diagnostico[npat] = diagBOM;
 } // prodist 8 pag 42
 else if (vpu < padraoRede->V4)
 pbar->resflow->Diagnostico[npat] = diagREGULAR;
 else
 pbar->resflow->Diagnostico[npat] = diagRUIM;
			}
			else
				{
//            	if      (vpu < geral->FLOW.V[0]) pbar->resflow->Diagnostico[npat] = diagRUIM;
//				else if (vpu < geral->FLOW.V[1]) pbar->resflow->Diagnostico[npat] = diagREGULAR;
//				else if (vpu < geral->FLOW.V[2]) pbar->resflow->Diagnostico[npat] = diagBOM;
//				else if (vpu < geral->FLOW.V[3]) pbar->resflow->Diagnostico[npat] = diagREGULAR;
//				else                             pbar->resflow->Diagnostico[npat] = diagRUIM;
				}
			}
		 }
	  }
   }
 */
//---------------------------------------------------------------------------
void __fastcall TDiag::DiagBarra(TList *lisBAR)
{
	// variáveis locais
	int max_pat, diag_bar_min, diag_bar_max;
	double vpu;
	VTBarra *pbar;
	int tipoSegmento = 0;
	VTPadraoRede *padraoRede;

	// determina número de patamares
	max_pat = patamares->NumPatamar();
	// loop p/ todas Barras
	for (int n = 0; n < lisBAR->Count; n++)
	{
		pbar = (VTBarra*)lisBAR->Items[n];
		for (int npat = 0; npat < max_pat; npat++)
		{ // verifica se fluxo foi calculado p/ o Patamar
			if (!patamares->Patamar[npat]->FluxoCorreto)
			{ // barra isolada
				pbar->resflow->Diagnostico[npat] = diagISOLADO;
				continue;
			}
			// verifica se Barra isolada
			if (IsDoubleZero(pbar->resflow->Vfn_pu_max[npat]))
			{ // barra isolada
				pbar->resflow->Diagnostico[npat] = diagISOLADO;
				continue;
			}
			// assume diagnóstico Bom
			pbar->resflow->Diagnostico[npat] = diag_bar_min = diag_bar_max = diagBOM;
			// proteção: verifica se a Vnom da Barra foi definida
			if (pbar->vnom > 0.)
			{
				tipoSegmento = geral->TipoSegmentoNivelTensao(pbar);
				padraoRede = geral->ExistePadraoRede(tipoSegmento);
				if (padraoRede == NULL)
					continue;
				// determina menor tensão pu da Barra no patamar
				vpu = pbar->resflow->Vfn_pu_min[npat];
				// determina diagnóstico da tensão mínima
				if (vpu < padraoRede->V1)
					diag_bar_min = diagRUIM;
				else if (vpu < padraoRede->V2)
					diag_bar_min = diagREGULAR;
				else if (vpu < padraoRede->V3)
					diag_bar_min = diagBOM;
				else if (IsDoubleZero(padraoRede->V3 - vpu))
				{
					diag_bar_min = diagBOM;
				} // prodist 8 pag 42
				else if (vpu < padraoRede->V4)
					diag_bar_min = diagREGULAR;
				else
					diag_bar_min = diagRUIM;

				// compara com diag. da maior tensão pu da Barra no patamar
				vpu = pbar->resflow->Vfn_pu_max[npat];
				// determina diagnóstico da tensão máxima
				if (vpu < padraoRede->V1)
					diag_bar_max = diagRUIM;
				else if (vpu < padraoRede->V2)
					diag_bar_max = diagREGULAR;
				else if (vpu < padraoRede->V3)
					diag_bar_max = diagBOM;
				else if (IsDoubleZero(padraoRede->V3 - vpu))
				{
					diag_bar_max = diagBOM;
				} // prodist 8 pag 42
				else if (vpu < padraoRede->V4)
					diag_bar_max = diagREGULAR;
				else
					diag_bar_max = diagRUIM;
			}
			// considera pior diagnóstico
			if (diag_bar_max > diag_bar_min)
				pbar->resflow->Diagnostico[npat] = diag_bar_max;
			else
				pbar->resflow->Diagnostico[npat] = diag_bar_min;
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TDiag::DiagCapserie(TList *lisLIG)
   {
   //variáveis locais
   int        max_pat;
   double     p, q, s, s1_2, s2_1;
   VTLigacao  *ligacao;
   VTCapserie *capserie;

   //determina número de patamares
   max_pat = patamares->NumPatamar();
   //loop p/ todas Ligações
   for (int n = 0; n < lisLIG->Count; n++)
      {//verifica se a Ligação é um Capserie
      ligacao = (VTLigacao*)lisLIG->Items[n];
		if (ligacao->Tipo() != eqptoCAPSERIE)
			continue;
      capserie = (VTCapserie*)ligacao;
      //verifica se Ligacao isolada
		if (LigacaoIsolada(capserie))
			continue;
      //loop p/ todos os patamares
      for (int npat = 0; npat < max_pat; npat++)
         {//verifica se fluxo foi calculado no Patamar
         if (! patamares->Patamar[npat]->FluxoCorreto)
            {//Ligacao isolada
            capserie->resflow->Diagnostico[npat] = diagISOLADO;
            continue;
            }
         //determina fluxo do Capserie
         s1_2 = Abs(capserie->resflow->S_mva[0][npat]);
         s2_1 = Abs(capserie->resflow->S_mva[0][npat]);
         s    = max(s1_2, s2_1);
         //assume diagnóstico Bom
         capserie->resflow->Diagnostico[npat] = diagBOM;
         //proteção: verifica se Snom do Capserie foi definida
         if (capserie->snom > 0.)
            {//determina carregamento do Capserie
            s = s / capserie->snom;
            capserie->resflow->Cargto_pu[npat] = s;
				if (s < geral->FLOW.I[0])
					capserie->resflow->Diagnostico[npat] = diagBOM;
				else if (s < geral->FLOW.I[1])
					capserie->resflow->Diagnostico[npat] = diagREGULAR;
				else
					capserie->resflow->Diagnostico[npat] = diagRUIM;
            }
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TDiag::DiagChave(TList *lisLIG)
   {
   //variáveis locais
   int       max_pat;
   double    ipu;
   VTChave   *pchv;
   VTLigacao *pliga;

   //determina número de patamares
   max_pat = patamares->NumPatamar();
   //loop p/ todas Ligações
   for (int n = 0; n < lisLIG->Count; n++)
      {//verifica se a Ligação é uma Chave
      pliga = (VTLigacao*)lisLIG->Items[n];
		if (pliga->Tipo() != eqptoCHAVE)
			continue;
      pchv = (VTChave*)pliga;
      //verifica se Ligacao isolada
		if (LigacaoIsolada(pchv))
			continue;
      //loop p/ todos os patamares
      for (int npat = 0; npat < max_pat; npat++)
         {//verifica se fluxo foi calculado no Patamar
         if (! patamares->Patamar[npat]->FluxoCorreto)
            {//Ligacao isolada
            pchv->resflow->Diagnostico[npat] = diagISOLADO;
            continue;
            }
         //verifica se Chave aberta
         if (pchv->Aberta)
            {//assume diagnóstico bom
            pchv->resflow->Diagnostico[npat] = diagBOM;
            pchv->resflow->Cargto_pu[npat] = 0;
            continue;
            }
         //assume diagnóstico bom
         pchv->resflow->Diagnostico[npat] = diagBOM;
         if (pchv->Inom_amp > 0.)
            {//calcula carregamento da Chave
            ipu = pchv->resflow->If_amp_max[0][npat] / pchv->Inom_amp;
            pchv->resflow->Cargto_pu[npat] = ipu;
				if (ipu < geral->FLOW.I[0])
					pchv->resflow->Diagnostico[npat] = diagBOM;
				else if (ipu < geral->FLOW.I[1])
					pchv->resflow->Diagnostico[npat] = diagREGULAR;
				else
					pchv->resflow->Diagnostico[npat] = diagRUIM;
            }
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TDiag::DiagGerador(TList *lisEQB)
   {
   //variáveis locais
   int       max_pat;
   double    s;
   VTEqbar   *eqbar;
   VTGerador *pger;

   //determina número de patamares
   max_pat = patamares->NumPatamar();
   //loop p/ todos VTEqbar
   for (int n = 0; n < lisEQB->Count; n++)
      {
      eqbar = (VTEqbar*)lisEQB->Items[n];
		if (eqbar->Tipo() != eqptoGERADOR)
			continue;
      pger = (VTGerador*)eqbar;
      //loop p/ todos os patamares
      for (int npat = 0; npat < max_pat; npat++)
         {//verifica se fluxo foi calculado no Patamar
         if (! patamares->Patamar[npat]->FluxoCorreto)
            {//Gerador isolada
            pger->resflow->Diagnostico[npat] = diagISOLADO;
            continue;
            }
         //verifica tensão do Gerador
         if (IsDoubleZero(pger->pbarra->resflow->Vfn_pu_max[npat]))
            {//Gerador isolado
            pger->resflow->Cargto_pu[npat]   = 0.;
            pger->resflow->Diagnostico[npat] = diagISOLADO;
            continue;
            }
         //assume diagnóstico Bom
         pger->resflow->Diagnostico[npat] = diagBOM;
         //determina potência do Gerador
         s = Abs(pger->resflow->S_mva[npat]);
         //proteção verifica se Snom do Gerador está definida
         if (pger->snom > 0.)
            {//calcula carregamento do Gerador
            s = s / pger->snom;
            pger->resflow->Cargto_pu[npat] = s;
				if (s < geral->FLOW.I[0])
					pger->resflow->Diagnostico[npat] = diagBOM;
				else if (s < geral->FLOW.I[1])
					pger->resflow->Diagnostico[npat] = diagREGULAR;
				else
					pger->resflow->Diagnostico[npat] = diagRUIM;
            }
         }
      }
   }
/*  //implementação comentada temporariamente
//---------------------------------------------------------------------------
void __fastcall TDiag::DiagRegulador(TList *lisLIG)
   {
   //variáveis locais
   int         max_pat;
   int         ind_bar = 0;
   double      snom_mva, sf_mva, cargto_pu;
   VTLigacao   *pliga;
   VTRegulador *regulador;

   //determina número de patamares
   max_pat = patamares->NumPatamar();
   //loop p/ todas Ligações
   for (int n = 0; n < lisLIG->Count; n++)
	{ // verifica se a Ligação é um Trecho
		pliga = (VTLigacao*)lisLIG->Items[n];
		if (pliga->Tipo() != eqptoREGULADOR)
			continue;
		regulador = (VTRegulador*)pliga;
		// verifica se Ligacao isolada
		if (LigacaoIsolada(pliga))
			continue;
		// loop p/ todos os patamares
		for (int npat = 0; npat < max_pat; npat++)
		{ // verifica se fluxo foi calculado no Patamar
			if (!patamares->Patamar[npat]->FluxoCorreto)
			{ // Regulador isolado
				regulador->resflow->Diagnostico[npat] = diagISOLADO;
				continue;
			}
			// determina fluxo.máxima por fase, MVA
			sf_mva = regulador->resflow->Sf_mva_max[ind_bar][npat];
			// snom_mva = regulador->snom;
			snom_mva = regulador->Spas_mva;
			// calcula carregamento em pu
			cargto_pu = 0;
			if (!IsDoubleZero(snom_mva))
				cargto_pu = sf_mva / snom_mva;
		 //atualiza carregamento e diagnóstico do Regulador
			regulador->resflow->Cargto_pu[npat] = cargto_pu;
			if (cargto_pu < geral->FLOW.S[0])
				regulador->resflow->Diagnostico[npat] = diagBOM;
			else if (cargto_pu < geral->FLOW.S[1])
				regulador->resflow->Diagnostico[npat] = diagREGULAR;
			else
				regulador->resflow->Diagnostico[npat] = diagRUIM;
		}
	}
   }
*/
/*
//---------------------------------------------------------------------------
void __fastcall TDiag::DiagRegulador(TList *lisLIG)
   {
   //variaveis locais
   int         max_pat;
   int         ind_bar = 0;
   double      snom_mva, sf_mva, cargto_pu;
   VTLigacao   *pliga;
   VTRegulador *regulador;

   //determina numero de patamares
   max_pat = patamares->NumPatamar();
   //loop p/ todas Ligacoes
   for (int n = 0; n < lisLIG->Count; n++)
	  {//verifica se a Ligacao eh um Trecho
	  pliga = (VTLigacao*)lisLIG->Items[n];
	  if (pliga->Tipo() != eqptoREGULADOR) continue;
	  regulador = (VTRegulador*)pliga;
	  //verifica se Ligacao isolada
	  if (LigacaoIsolada(pliga)) continue;
	  //loop p/ todos os patamares
	  for (int npat = 0; npat < max_pat; npat++)
		 {//verifica se fluxo foi calculado no Patamar
		 if (! patamares->Patamar[npat]->FluxoCorreto)
			{//Regulador isolado
			regulador->resflow->Diagnostico[npat] = diagISOLADO;
			continue;
			}
		 //determina fluxo.maxima por fase, MVA
		 sf_mva   = regulador->resflow->Sf_mva_max[ind_bar][npat];
		 //snom_mva = regulador->snom;
		 snom_mva = regulador->Spas_mva;
		 //altera valor de potência nominal em função da faixa de tensão regulada
		 snom_mva = CapacidadeNominal(snom_mva, regulador->var_tensao);
		 //calcula carregamento em pu
		 cargto_pu = 0;
		 if (! IsDoubleZero(snom_mva)) cargto_pu = sf_mva/snom_mva;
		 //atualiza carregamento e diagnostico do Regulador
		 regulador->resflow->Cargto_pu[npat] = cargto_pu;
		 if      (cargto_pu < geral->FLOW.S[0]) regulador->resflow->Diagnostico[npat] = diagBOM;
		 else if (cargto_pu < geral->FLOW.S[1]) regulador->resflow->Diagnostico[npat] = diagREGULAR;
		 else                                   regulador->resflow->Diagnostico[npat] = diagRUIM;
		 }
	  }
   }
*/
//---------------------------------------------------------------------------
void __fastcall TDiag::DiagRegulador(TList *lisLIG)
   {
   //variaveis locais
   int         max_pat;
   int         ind_bar = 0;
   double      cap_max_pu, imax_a, if_a, cargto_pu;
   VTLigacao   *pliga;
   VTRegulador *regulador;

   //determina numero de patamares
   max_pat = patamares->NumPatamar();
   //loop p/ todas Ligacoes
   for (int n = 0; n < lisLIG->Count; n++)
	  {//verifica se a Ligacao eh um Trecho
	  pliga = (VTLigacao*)lisLIG->Items[n];
	  if (pliga->Tipo() != eqptoREGULADOR) continue;
	  regulador = (VTRegulador*)pliga;
	  //verifica se Ligacao isolada
	  if (LigacaoIsolada(pliga)) continue;
	  //loop p/ todos os patamares
	  for (int npat = 0; npat < max_pat; npat++)
		 {//verifica se fluxo foi calculado no Patamar
		 if (! patamares->Patamar[npat]->FluxoCorreto)
			{//Regulador isolado
			regulador->resflow->Diagnostico[npat] = diagISOLADO;
			continue;
			}

		 //determina fluxo.maxima por fase, em A
		 if_a   = regulador->resflow->If_amp_max[ind_bar][npat];
		 //determina capacidade maxima em função da faixa de tensão regulada
         //capacidade_max.begin();
		 //cap_max_pu = (capacidade_max.find(regulador->var_tensao)->second);
		 //determina corrente maxima, em A
		 imax_a = regulador->InomA();// * cap_max_pu;
		 //calcula carregamento em pu
		 cargto_pu = 0;
		 if (! IsDoubleZero(imax_a)) cargto_pu = if_a/imax_a;
		 //atualiza carregamento e diagnostico do Regulador
		 regulador->resflow->Cargto_pu[npat] = cargto_pu;
		 if      (cargto_pu < geral->FLOW.S[0])
			{regulador->resflow->Diagnostico[npat] = diagBOM;}
		 else if (cargto_pu < geral->FLOW.S[1])
			{regulador->resflow->Diagnostico[npat] = diagREGULAR;}
		 else
			{regulador->resflow->Diagnostico[npat] = diagRUIM;}
		 }
	  }
   }

//---------------------------------------------------------------------------
void __fastcall TDiag::DiagSuprimento(TList *lisEQB)
   {
   //variáveis locais
   int          max_pat;
   double       p, q, s;
   VTEqbar      *eqbar;
   VTSuprimento *psup;

   //determina número de patamares
   max_pat = patamares->NumPatamar();
   //loop p/ todos VTEqbar
   for (int n = 0; n < lisEQB->Count; n++)
      {
      eqbar = (VTEqbar*)lisEQB->Items[n];
		if (eqbar->Tipo() != eqptoSUPRIMENTO)
			continue;
      psup = (VTSuprimento*)eqbar;
      //loop p/ todos os patamares
      for (int npat = 0; npat < max_pat; npat++)
         {//verifica se fluxo foi calculado no Patamar
         if (! patamares->Patamar[npat]->FluxoCorreto)
            {//Suprimento isolada
            psup->resflow->Diagnostico[npat] = diagISOLADO;
            continue;
            }
         //verifica tensão da Barra do Suprimento
         if (IsDoubleZero(psup->pbarra->resflow->Vfn_pu_max[npat]))
            {//Suprimento isolado
            psup->resflow->Cargto_pu[npat]   = 0.;
            psup->resflow->Diagnostico[npat] = diagISOLADO;
            continue;
            }
         //assume diagnóstico Bom
         psup->resflow->Diagnostico[npat] = diagBOM;
         //determina potência do Suprimento
         s = Abs(psup->resflow->S_mva[npat]);
         //proteção verifica se Smax do Suprimento está definida
         if (psup->smax > 0.)
            {//determina carregamento do Suprimento
            s = s / psup->smax;
            psup->resflow->Cargto_pu[npat] = s;
				if (s < geral->FLOW.I[0])
					psup->resflow->Diagnostico[npat] = diagBOM;
				else if (s < geral->FLOW.I[1])
					psup->resflow->Diagnostico[npat] = diagREGULAR;
				else
					psup->resflow->Diagnostico[npat] = diagRUIM;
            }
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TDiag::DiagTrafo(TList *lisLIG)
   {
   //variáveis locais
   int       max_pat;
   double    p, q, s, s1_2, s2_1;
   VTLigacao *pliga;
   VTTrafo   *ptrf;

   //determina número de patamares
   max_pat = patamares->NumPatamar();
   //loop p/ todas Ligações
   for (int n = 0; n < lisLIG->Count; n++)
      {//verifica se a Ligação é um Trafo
      pliga = (VTLigacao*)lisLIG->Items[n];
		if (pliga->Tipo() != eqptoTRAFO)
			continue;
      ptrf = (VTTrafo*)pliga;
      //verifica se Ligacao isolada
		if (LigacaoIsolada(ptrf))
			continue;
      //loop p/ todos os patamares
     for (int npat = 0; npat < max_pat; npat++)
         {//verifica se fluxo foi calculado no Patamar
         if (! patamares->Patamar[npat]->FluxoCorreto)
            {//Trafo isolado
            ptrf->resflow->Diagnostico[npat] = diagISOLADO;
            continue;
            }
         //determina fluxo do Trafo
			s1_2 = Abs(ptrf->resflow->S_mva[0][npat]);
         s2_1 = Abs(ptrf->resflow->S_mva[0][npat]);
         s    = max(s1_2, s2_1);
         //assume diagnostico diagBOM
         ptrf->resflow->Diagnostico[npat] = diagBOM;
         //proteção: verifica se Snom do Trafo foi definida
         if (ptrf->snom > 0.)
            {//determina carregamento do Trafo
            s = s / ptrf->snom;
            ptrf->resflow->Cargto_pu[npat] = s;
				if (s < geral->FLOW.S[0])
					ptrf->resflow->Diagnostico[npat] = diagBOM;
				else if (s < geral->FLOW.S[1])
					ptrf->resflow->Diagnostico[npat] = diagREGULAR;
				else
					ptrf->resflow->Diagnostico[npat] = diagRUIM;
            }
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TDiag::DiagTrafo3E(TList *lisLIG)
   {
   //variáveis locais
   int       max_pat;
   double    p, q, s, cargto, snom;
   VTLigacao *pliga;
   VTTrafo3E *ptrf3E;

   //determina número de patamares
   max_pat = patamares->NumPatamar();
   //loop p/ todas Ligações
   for (int n = 0; n < lisLIG->Count; n++)
      {//verifica se a Ligação é um Trafo3E
      pliga = (VTLigacao*)lisLIG->Items[n];
		if (pliga->Tipo() != eqptoTRAFO3E)
			continue;
      ptrf3E = (VTTrafo3E*)pliga;
      //verifica se Ligacao isolada
		if (LigacaoIsolada(ptrf3E))
			continue;
      //loop p/ todos os patamares
      for (int npat = 0; npat < max_pat; npat++)
         {//verifica se fluxo foi calculado no Patamar
         if (! patamares->Patamar[npat]->FluxoCorreto)
            {//Trafo3E isolado
            ptrf3E->resflow->Diagnostico[npat] = diagISOLADO;
            continue;
            }
         //assume diagnóstico Bom
         ptrf3E->resflow->Cargto_pu[npat]   = 0.;
         ptrf3E->resflow->Diagnostico[npat] = diagBOM;
         //loop p/ primário, secundário e terciário
         for (int nb = 0; nb < NBAR_LIG; nb++)
            {//determina fluxos no Trafo3E
            s = Abs(ptrf3E->resflow->S_mva[nb][npat]);
            //determina Snom
				if (nb == 0)
					snom = ptrf3E->pri.snom;
				else if (nb == 1)
					snom = ptrf3E->sec.snom;
				else
					snom = ptrf3E->ter.snom;
            //proteção: verifica se a Snom foi definida
            if (snom > 0)
               {//calcula cargto
               cargto = s/snom;
               //determina carregamento máximo
               cargto = max(cargto, ptrf3E->resflow->Cargto_pu[npat]);
					ptrf3E->resflow->Cargto_pu[npat] = cargto;
					if (cargto < geral->FLOW.S[0])
						ptrf3E->resflow->Diagnostico[npat] = diagBOM;
					else if (cargto < geral->FLOW.S[1])
						ptrf3E->resflow->Diagnostico[npat] = diagREGULAR;
					else
						ptrf3E->resflow->Diagnostico[npat] = diagRUIM;
               }
            }
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TDiag::DiagTramo(TList *lisLIG)
   {
   //variáveis locais
   VTLigacao *pliga;
   VTTramo   *tramo;

   //loop p/ todas Ligações
   for (int n = 0; n < lisLIG->Count; n++)
      {//verifica se a Ligação é um Tramo
      pliga = (VTLigacao*)lisLIG->Items[n];
		if (pliga->Tipo() != eqptoTRAMO)
			continue;
      tramo = (VTTramo*)pliga;
      //calcula diagnóstico p/ todas Barras internas do Tramo
      DiagBarra(tramo->LisBarraInterna());
      //calcula diagnóstico p/ todos Trechos do Tramo
      DiagTrecho(tramo->LisTrecho());
      }
   }

//---------------------------------------------------------------------------
void __fastcall TDiag::DiagTrecho(TList *lisLIG)
   {
   //variáveis locais
   int       max_pat;
   double    iadm, ipu, ipu_max;
   VTLigacao *pliga;
   VTTrecho  *ptre;

   //determina número de patamares
   max_pat = patamares->NumPatamar();
   //loop p/ todas Ligações
   for (int n = 0; n < lisLIG->Count; n++)
      {//verifica se a Ligação é um Trecho
      pliga = (VTLigacao*)lisLIG->Items[n];
		if (pliga->Tipo() != eqptoTRECHO)
			continue;
      ptre = (VTTrecho*)pliga;
      //verifica se Ligacao isolada
		if (LigacaoIsolada(ptre))
			continue;
      //loop p/ todos os patamares
      for (int npat = 0; npat < max_pat; npat++)
         {//verifica se fluxo foi calculado no Patamar
         if (! patamares->Patamar[npat]->FluxoCorreto)
            {//Trecho isolado
            ptre->resflow->Diagnostico[npat] = diagISOLADO;
            continue;
            }
         //assume diagnóstico Bom
         ptre->resflow->Diagnostico[npat] = diagBOM;
         //determina corrente máxima do Trecho em pu
         ipu_max = 0.;
         for (int nf = 0; nf < MAX_FASE; nf++)
            {
				if ((iadm = ptre->arranjo->Ifase_amp[nf]) <= 0)
					continue;
            ipu = Abs(ptre->resflow->If_amp[0][npat][nf])/iadm;
				if (ipu > ipu_max)
					ipu_max = ipu;
            }
         //atualiza carregamento e diagnóstico do Trecho
         ptre->resflow->Cargto_pu[npat] = ipu_max;
			if (ipu_max < geral->FLOW.I[0])
				ptre->resflow->Diagnostico[npat] = diagBOM;
			else if (ipu_max < geral->FLOW.I[1])
				ptre->resflow->Diagnostico[npat] = diagREGULAR;
			else
				ptre->resflow->Diagnostico[npat] = diagRUIM;
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TDiag::Executa(VTNet *net)
   {
   //variáveis locais
   TList *lisNO;
   VTNo  *no;

   //IMPORTANTE: determina diagnóstico das Barras antes dos demais Eqptos
   DiagBarra(net->LisBarra());
   //determina diagnóstico dos Capserie
   DiagCapserie(net->LisLigacao());
   //determina diagnóstico dos Trafos
   DiagTrafo(net->LisLigacao());
   DiagTrafo3E(net->LisLigacao());
   //determina diagnóstico dos Tramos
   DiagTramo(net->LisLigacao());
   //determina diagnóstico dos Trechos
   DiagTrecho(net->LisLigacao());
   //determina diagnóstico das Chaves
   DiagChave(net->LisLigacao());
   //determina diagnóstico dos Reguladores
   DiagRegulador(net->LisLigacao());
   //determina diagnóstico de Geradores e Suprimentos
   DiagGerador(net->LisEqbar());
   DiagSuprimento(net->LisEqbar());
   //determina diagnóstico p/ Ligacoes internas aos Nos
   lisNO = net->LisNo();
   for (int n = 0; n < lisNO->Count; n++)
      {
      no = (VTNo*)lisNO->Items[n];
		if (no->lisLIG->Count == 0)
			continue;
      //determina diagnóstico das Chaves e dos Reguladores
      DiagChave(no->lisLIG);
      DiagRegulador(no->lisLIG);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TDiag::Executa(VTRede *rede)
   {
   //IMPORTANTE: determina diagnóstico das Barras antes dos demais Eqptos
   DiagBarra(rede->LisBarra());
   //determina diagnóstico dos Capserie
   DiagCapserie(rede->LisLigacao());
   //determina diagnóstico dos Trafos
   DiagTrafo(rede->LisLigacao());
   DiagTrafo3E(rede->LisLigacao());
   //determina diagnóstico dos Trechos
   DiagTrecho(rede->LisLigacao());
   //determina diagnóstico das Chaves
   DiagChave(rede->LisLigacao());
   //determina diagnóstico de Geradores e Suprimentos
   lisEQP->Clear();
   rede->LisEqbar(lisEQP);
   DiagGerador(lisEQP);
   DiagSuprimento(lisEQP);
   }

//---------------------------------------------------------------------------
bool __fastcall TDiag::LigacaoIsolada(VTLigacao *ligacao)
   {
   //variáveis locais
   int       max_pat;
   VTBarra   *barra;
   VTPatamar *patamar;

   //determina número de patamares
   max_pat = patamares->NumPatamar();
   //determina Patamar habilitado
   if ((patamar = patamares->PrimeiroPatamarHabilitado()) != NULL)
      {//verifica se as Barras da Ligacao estão sem tensão
      for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
         {
			if ((barra = ligacao->Barra(ind_bar)) == NULL)
				continue;
         //verifica se a Barra tem tensão
			if (barra->resflow->Diagnostico[patamar->Index] != diagISOLADO)
				return (false);
         }
      }
   //define diagISOLADO e carregamento nulo p/ todos patamares
   for (int np = 0; np < max_pat; np++)
      {
      ligacao->resflow->Diagnostico[np]  = diagISOLADO;
      ligacao->resflow->Cargto_pu[np] = 0.;
      }
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TDiag::ReiniciaDiagFluxo(bool diag_flow)
   {
   //variáveis locais
   int         max_pat;
   VTRede      *rede;
   VTRedes     *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList       *lisREDE = redes->LisRede();

   //determina número de patamares
   max_pat = patamares->NumPatamar();
   //reinicia diagnóstico do flow p/ todas Redes
   for (int nr = 0; nr < lisREDE->Count; nr++)
      {
      rede = (VTRede*)lisREDE->Items[nr];
      rede->FluxoOK = diag_flow;
      for(int npat = 0; npat < max_pat; npat++)
         {
         rede->diag.flow[npat] = diag_flow;
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TDiag::ReiniciaResFluxo(void)
   {
   //variáveis locais
   VTRede  *rede;
   TList   *lisBAR;
   VTRedes *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList   *lisREDE = redes->LisRede();

   //reinicia dados de flow das Barras
   ReiniciaResFluxoBarra(redes->LisBarra());
   //reinicia dados de flow das Barras das Reducoes
   ReiniciaResFluxoBarra(consulta->LisEqptoReducaoCarregada(redes, eqptoBARRA));
   //reinicia dados de flow de Eqbar
   ReiniciaResFluxoEqbar(consulta->LisEqptoRedeReducao(redes, eqptoEQBAR));
   //reinicia resultados de flow das Barras e Ligacoes de Rede isolada
   rede = redes->RedeIsolada();
   ReiniciaResFluxoBarra(rede->LisBarra());
   ReiniciaResFluxoLigacao(rede->LisLigacao());
   //loop p/ todas Redes
   for (int nr = 0; nr < lisREDE->Count; nr++)
      {
      rede = (VTRede*)lisREDE->Items[nr];
      //reinicia resultado p/ Rede
      rede->FluxoOK = false;
      //reinicia resultado para Ligacoes da Rede
      ReiniciaResFluxoLigacao(rede->LisLigacao());
      }
   //reinicia resultados de flow das Ligacoes das Reducoes
   ReiniciaResFluxoLigacao(consulta->LisEqptoReducaoCarregada(redes, eqptoLIGACAO));
   }

//---------------------------------------------------------------------------
void __fastcall TDiag::ReiniciaResFluxoBarra(TList *lisBAR)
   {
   //variáveis locais
   VTBarra *pbar;

   //loop p/ todas Barras
   for (int nb = 0; nb < lisBAR->Count; nb++)
      {
      pbar = (VTBarra*)lisBAR->Items[nb];
      pbar->resflow->Reinicia();
      }
   }

//---------------------------------------------------------------------------
void __fastcall TDiag::ReiniciaResFluxoEqbar(TList *lisEQB)
   {
   //variáveis locais
   VTEqbar      *eqbar;
   VTGerador    *pger;
   VTSuprimento *psup;

   //reinicia dados de flow dos Eqbar
   for (int n = 0; n < lisEQB->Count; n++)
      {
      eqbar = (VTEqbar*)lisEQB->Items[n];
      switch(eqbar->Tipo())
         {
         case eqptoGERADOR:
            pger = (VTGerador*)eqbar;
            pger->resflow->Reinicia();
            break;
         case eqptoSUPRIMENTO:
            psup = (VTSuprimento*)eqbar;
            psup->resflow->Reinicia();
            break;
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TDiag::ReiniciaResFluxoLigacao(TList *lisLIG)
   {
   //variáveis locais
   VTLigacao   *pliga;

   //reinicia dados de flow das Ligações
   for (int nlig = 0; nlig < lisLIG->Count; nlig++)
      {
      pliga = (VTLigacao*)lisLIG->Items[nlig];
      pliga->resflow->Reinicia();
      //verifica se Ligacao é um Tramo
      if (pliga->Tipo() == eqptoTRAMO)
         {//chamada recursiva
         ReiniciaResFluxoLigacao(((VTTramo*)pliga)->LisTrecho());
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TDiag::ReiniciaResFluxoLigacaoNo(TList *lisNO)
   {
   //variáveis locais
   VTNo *no;

   //reinicia dados de flow das Ligações
   for (int n = 0; n < lisNO->Count; n++)
      {
      no = (VTNo*)lisNO->Items[n];
      ReiniciaResFluxoLigacao(no->lisLIG);
      }
   }
//---------------------------------------------------------------------------
//eof
