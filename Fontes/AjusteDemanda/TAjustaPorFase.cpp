//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Math.hpp>
#include "TAjustaPorFase.h"
#include "..\Apl\VTApl.h"
#include "..\AjusteDemanda\Grupo\VTGrupo.h"
#include "..\AjusteDemanda\Grupo\VTGrupos.h"
#include "..\AjusteDemanda\Grupo\VTGerenciaGrupos.h"
#include "..\Bloco\VTBloco.h"
#include "..\Bloco\VTBlocos.h"
#include "..\Bloco\VTElo.h"
#include "..\Classes\VTClasse.h"
#include "..\Complexo\Complexo.h"
#include "..\Cronometro\VTCronometro.h"
#include "..\Curvas\VTCurva.h"
#include "..\Diretorio\VTPath.h"
#include "..\Flow\VTFlow.h"
#include "..\ImportaMedicao\VTMedicoes.h"
#include "..\ImportaMedicao\VTMedicao.h"
#include "..\Patamar\VTPatamar.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Potencia\VTDemanda.h"
#include "..\Potencia\VTFornecimento.h"
#include "..\Progresso\VTProgresso.h"
#include "..\Radial\VTNode.h"
#include "..\Radial\VTPrimario.h"
#include "..\Rede\VTCapacitor.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTFases.h"
#include "..\Rede\VTGerador.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTResFlowLig.h"
#include "..\..\DLL_Inc\Curvas.h"
#include "..\..\DLL_Inc\Flow.h"
#include "..\..\DLL_Inc\Funcao.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função p/ criar objeto da classe
//---------------------------------------------------------------------------
VTAjusta* __fastcall NewObjAjustaPorFase(VTApl *apl_owner)
   {
   try{
      return(new TAjustaPorFase(apl_owner));
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TAjustaPorFase::TAjustaPorFase(VTApl *apl_owner)
                          :TAjusta(apl_owner)
   {
   //cria objeto Apl local
   apl = NewObjApl(NULL, apl_owner);
   //verifica se existe objeto Flow
   if ((flow = (VTFlow*)apl->GetObject(__classid(VTFlow))) == NULL)
      {//cria objeto Flow local
      apl->Add(flow = DLL_NewObjFlow(apl));
      }
   ZeraValores();
   //cria string list com evolução dos erros por medição e por iteração
   linhas = new TStringList();
   }

//---------------------------------------------------------------------------
__fastcall TAjustaPorFase::~TAjustaPorFase(void)
   {
   //destrói objetos locais
   if (apl) {delete apl; apl = NULL;}
   //destroi string list
   if(linhas) {delete linhas; linhas = NULL;}
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18386466
double __fastcall TAjustaPorFase::AjustePos(VTMedicao *medicao,
                                            TList *lisCargaJusante, VTElo *elo)
   {
   //variáveis locais
   bool        pre_fluxo = false;
   double      fat_mult_p[MAX_FASE], fat_mult_q[MAX_FASE];
   double      fatcor_min, fatcor_max;
   int pat_calculado = 0;
   VTGrupos *grupos = NULL;

   //proteção
   if((! medicao) || (! elo))            return(0.);
   if (medicao->lisCAR_AJUS->Count == 0) return(0.);
   //loop p/ todos patamares
   for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
      {//verifica se patamar está habilitado
	  if(! pat[ind_pat])  continue;
	  //zera variaveis
      ZeraValores();
      //calcula demanda da curva de medição no patamar
	  CalculaDemandaMedicaoFase(medicao->curva_pq, patamares->Patamar[ind_pat], p_med_mw);
      //calcula demanda na entrada do bloco (elo), calculada pelo fluxo
      CalculaDemandaEntradaFase(medicao->ind_bar_ref, elo, ind_pat, p_ent_flow_mw);
      //calcula demanda das Cargas ajustáveis no patamar
      CalculaDemandaCargaFase(medicao->lisCAR_AJUS, ind_pat, p_car_ajus_mw);
      //calcula demanda das Cargas a jusante no patamar
      CalculaDemandaCargaFase(lisCargaJusante, ind_pat, p_car_jusante_mw);
      //calcula demanda dos geradores PQ ou PV no patamar
      CalculaDemandaGeradorFase(medicao->lisGER_SUP, ind_pat, p_ger_mw);
	  //calcula fator de ajuste P (~ erro da medicao)
      FatorAjusteP(fat_mult_p, pre_fluxo);
      if(FatorNegativo(fat_mult_p))
         {//não atende as condições para ajustar a demanda
         return(-1);
         }
	  //calcula demanda reativa da medição
      CalculaDemandaReativaMedicaoFase(medicao->curva_pq, patamares->Patamar[ind_pat], q_med_mvar);
      //calcula demanda reativa na entrada do bloco (elo), calculada pelo fluxo
      CalculaDemandaReativaEntradaFase(medicao->ind_bar_ref, elo, ind_pat, q_ent_flow_mvar);
      //calcula demanda reativa das Cargas ajustáveis
      CalculaDemandaReativaCargaFase(medicao->lisCAR_AJUS, ind_pat, q_car_ajus_mvar);
      //calcula demanda das Cargas a jusante no patamar
      CalculaDemandaReativaCargaFase(lisCargaJusante, ind_pat, q_car_jusante_mvar);
      //calcula demanda reativa dos geradores
      CalculaDemandaReativaGeradorFase(medicao->lisGER_SUP, ind_pat, q_ger_mvar);
	  //calcula fator de ajuste Q (~ erro da medicao)
	  FatorAjusteQ(fat_mult_q, pre_fluxo);
	  //calcula as demandas de cara grupo da medicao
	  grupos = gerGrupos->ExisteGrupos(medicao);
	  CalculaDemandasGruposAjuste(grupos, ind_pat);
	  //calcula os fatores de ajuste de cada grupo
	  CalculaFatoresGruposAjuste(grupos,pre_fluxo);
	  //corrige demanda das Cargas ajustáveis
	  //CorrigeDemandaCarga(medicao->lisCAR_AJUS, ind_pat, fat_mult_p, fat_mult_q);
	  CorrigeDemandaCarga(medicao,ind_pat);
	  //salva maior fator de correção
	  if (pat_calculado == 0)
         {
         fatcor_min = Min(MenorFator(fat_mult_p), MenorFator(fat_mult_q));
         fatcor_max = Max(MaiorFator(fat_mult_p), MaiorFator(fat_mult_q));
         }
      else
		 {
		 fatcor_min = Min(MenorFator(fat_mult_p), fatcor_min);
         fatcor_min = Min(MenorFator(fat_mult_q), fatcor_min);
		 fatcor_max = Max(MaiorFator(fat_mult_p), fatcor_max);
         fatcor_max = Max(MaiorFator(fat_mult_q), fatcor_max);
		 }
	  pat_calculado++;
	  }

   //determina maior erro
   return(Max(fabs(fatcor_max - 1.), fabs(1. - fatcor_min)));
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18386466
double __fastcall TAjustaPorFase::AjustePre(VTMedicao *medicao, TList *lisCargaJusante,
                                              TList *lisCapJusante)
   {
   //variáveis locais
   bool        pre_fluxo = true;
   double      fat_mult_p[MAX_FASE], fat_mult_q[MAX_FASE];
   double      fatcor_max = 0.;
   VTGrupos *grupos = NULL;

   //proteção
   if ((! medicao) || (! lisCargaJusante)) return(0.);
   if (medicao->lisCAR_AJUS->Count == 0)   return(0.);
   //loop p/ todos patamares
   for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
      {//verifica se patamar está habilitado
	  if(! pat[ind_pat])  continue;
	  //zera variaveis
      ZeraValores();
      //calcula demanda da curva de medição no patamar   //DVK 2014.02.27
      CalculaDemandaMedicaoFase(medicao->curva_pq, patamares->Patamar[ind_pat], p_med_mw);
      //calcula demanda das Cargas ajustáveis no patamar
      CalculaDemandaCargaFase(medicao->lisCAR_AJUS, ind_pat, p_car_ajus_mw);
      //calcula demanda das Cargas a jusante no patamar
      CalculaDemandaCargaFase(lisCargaJusante, ind_pat, p_car_jusante_mw);
      //calcula demanda das Cargas fixas no patamar
	  CalculaDemandaCargaFase(medicao->lisCAR_FIXA, ind_pat, p_car_fixa_mw);
      //calcula fator de ajuste P
      FatorAjusteP(fat_mult_p, pre_fluxo);
      if(FatorNegativo(fat_mult_p))
         {//não atende as condições para ajustar a demanda
         return(-1);
         }
      //calcula demanda reativa da medição
      CalculaDemandaReativaMedicaoFase(medicao->curva_pq, patamares->Patamar[ind_pat], q_med_mvar);
      //calcula demanda das Cargas ajustáveis no patamar
      CalculaDemandaReativaCargaFase(medicao->lisCAR_AJUS, ind_pat, q_car_ajus_mvar);
      //calcula demanda das Cargas a jusante no patamar
      CalculaDemandaReativaCargaFase(lisCargaJusante, ind_pat, q_car_jusante_mvar);
      //calcula demanda das Cargas fixas no patamar
      CalculaDemandaReativaCargaFase(medicao->lisCAR_FIXA, ind_pat, q_car_fixa_mvar);
      //calcula demanda dos capacitores e a jusante
      CalculaDemandaCapacitorFase(medicao->lisCAPACITOR, ind_pat, q_cap_mvar);
      CalculaDemandaCapacitorFase(lisCapJusante, ind_pat, q_cap_mvar);
      //calcula fator de ajuste Q
	  FatorAjusteQ(fat_mult_q, pre_fluxo);
       //calcula as demandas de cara grupo da medicao
	  grupos = gerGrupos->ExisteGrupos(medicao);
	  CalculaDemandasGruposAjuste(grupos, ind_pat);
	  //calcula os fatores de ajuste de cada grupo
	  CalculaFatoresGruposAjuste(grupos,pre_fluxo);
	  //corrige demanda das Cargas ajustáveis
	  //CorrigeDemandaCarga(medicao->lisCAR_AJUS, ind_pat, fat_mult_p, fat_mult_q);
	  CorrigeDemandaCarga(medicao,ind_pat);
	  //salva maior fator de correção
	  fatcor_max = Max(fatcor_max, MaiorFator(fat_mult_p));
      fatcor_max = Max(fatcor_max, MaiorFator(fat_mult_q));
      }
   return(fabs(fatcor_max - 1.));
   }

//---------------------------------------------------------------------------
void __fastcall TAjustaPorFase::CalculaDemandaCapacitorFase(TList *lisCAP,
                                       int ind_pat, double q_mvar[MAX_FASE])
   {
   //variáveis locais
   VTCapacitor     *capacitor;
   complex<double> s_mva[MAX_FASE];

   //loop p/ todos capacitores
   for(int ncap = 0; ncap < lisCAP->Count; ncap++)
      {
      capacitor = (VTCapacitor*)lisCAP->Items[ncap];
      //determina potencia por fase
      if(! demanda->Get_S3f_mva(capacitor,ind_pat,s_mva)) continue;
      for (int ind_fase = indFASE_A; ind_fase < MAX_FASE; ind_fase++)
         {
         q_mvar[ind_fase] += s_mva[ind_fase].imag();
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TAjustaPorFase::CalculaDemandaCargaFase(TList *lisCAR, int ind_pat,
                                                         double p_mw[MAX_FASE])
   {
   //variáveis locais
   VTCarga         *carga;
   complex<double> s_mva[MAX_FASE];

   //loop p/ todas Cargas
   for (int nc = 0; nc < lisCAR->Count; nc++)
      {
      carga = (VTCarga*)lisCAR->Items[nc];
      //determina potencia por fase
      if(! demanda->Get_S3f_mva(carga,ind_pat,s_mva)) continue;
      for (int ind_fase = indFASE_A; ind_fase < MAX_FASE; ind_fase++)
         {
         p_mw[ind_fase] += s_mva[ind_fase].real();
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TAjustaPorFase::CalculaDemandaEntradaFase(int ind_bar_ref, VTElo *elo, int ind_pat,
                                             double p_ent_flow_mw[MAX_FASE])
   {
   //variáveis locais
   VTLigacao *ligacao;

   //proteção
   if(! elo->Chave)  return;
   ligacao = elo->Chave;
   //determina potência ativa ligacao
   p_ent_flow_mw[indFASE_A] = ligacao->resflow->Pa_mw[ind_bar_ref][ind_pat];
   p_ent_flow_mw[indFASE_B] = ligacao->resflow->Pb_mw[ind_bar_ref][ind_pat];
   p_ent_flow_mw[indFASE_C] = ligacao->resflow->Pc_mw[ind_bar_ref][ind_pat];
   p_ent_flow_mw[indNEUTRO] = 0.;
   }

//---------------------------------------------------------------------------
void __fastcall TAjustaPorFase::CalculaDemandaGeradorFase(TList *lisGER,
                                 int ind_pat, double p_mw[MAX_FASE])
   {
   //variáveis locais
   VTGerador  *gerador;
   strSUP     vet_sup[MAX_FASE];

   //loop p/ todos geradores
   for (int nc = 0; nc < lisGER->Count; nc++)
      {
      gerador = (VTGerador*)lisGER->Items[nc];
      //determina potencia por fase
      if(! fornecimento->Get_PQVT_porfase(gerador,ind_pat,vet_sup)) continue;
      for (int ind_fase = indFASE_A; ind_fase < MAX_FASE; ind_fase++)
         {
         p_mw[ind_fase] += vet_sup[ind_fase].p;
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TAjustaPorFase::CalculaDemandaMedicaoFase(VTCurva *curva,
                                 VTPatamar *patamar, double p_med_mw[MAX_FASE])
   {
   //variáveis locais
   double  pq_mva[MAX_FASE+MAX_FASE];
   strHM   hm_ini;//, hm_fim;

   //determina horario do patamar
   hm_ini.hora   = patamar->HoraCal;
   hm_ini.minuto = patamar->MinutoCal;
   //obtém valor da curva
   if (! curva->GetValor(hm_ini, pq_mva, curva->NumeroValores)) return;
   //determina pot. ativa das fases
    for (int ind_fase = indFASE_A; ind_fase <= indFASE_C; ind_fase++)
      {
      p_med_mw[ind_fase] = pq_mva[ind_fase+ind_fase];
      }
   }

//---------------------------------------------------------------------------
void __fastcall TAjustaPorFase::CalculaDemandaReativaCargaFase(TList *lisCAR,
                                 int ind_pat, double q_mvar[MAX_FASE])
   {
   //variáveis locais
   VTCarga         *carga;
   complex<double> s_mva[MAX_FASE];

   //loop p/ todas Cargas
   for (int nc = 0; nc < lisCAR->Count; nc++)
      {
      carga = (VTCarga*)lisCAR->Items[nc];
      //determina potencia por fase
      if(! demanda->Get_S3f_mva(carga,ind_pat,s_mva)) continue;
      for (int ind_fase = indFASE_A; ind_fase < MAX_FASE; ind_fase++)
         {
         q_mvar[ind_fase] += s_mva[ind_fase].imag();
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TAjustaPorFase::CalculaDemandaReativaEntradaFase(int ind_bar_ref,
                                 VTElo *elo, int ind_pat,
                                 double q_ent_flow_mvar[MAX_FASE])
   {
   //variáveis locais
   VTLigacao  *ligacao;

   //proteção
   if(! elo)          return;
   if(! elo->Chave)  return;
   ligacao = elo->Chave;
   //determina potência reativa ligacao
   q_ent_flow_mvar[indFASE_A] = ligacao->resflow->Qa_mvar[ind_bar_ref][ind_pat];
   q_ent_flow_mvar[indFASE_B] = ligacao->resflow->Qb_mvar[ind_bar_ref][ind_pat];
   q_ent_flow_mvar[indFASE_C] = ligacao->resflow->Qc_mvar[ind_bar_ref][ind_pat];
   q_ent_flow_mvar[indNEUTRO] = 0.;
   }

//---------------------------------------------------------------------------
void __fastcall TAjustaPorFase::CalculaDemandaReativaGeradorFase(TList *lisGER,
                              int ind_pat, double q_mvar[MAX_FASE])
   {
   //variáveis locais
   VTGerador   *gerador;
   strSUP      vet_sup[MAX_FASE];

   //loop p/ todos os geradores
   for (int nc = 0; nc < lisGER->Count; nc++)
      {
      gerador = (VTGerador*)lisGER->Items[nc];
      //determina potencia por fase
      if(! fornecimento->Get_PQVT_porfase(gerador,ind_pat,vet_sup)) continue;
      for (int ind_fase = indFASE_A; ind_fase < MAX_FASE; ind_fase++)
         {
         q_mvar[ind_fase] += vet_sup[ind_fase].q;
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TAjustaPorFase::CalculaDemandaReativaMedicaoFase(VTCurva *curva,
                              VTPatamar *patamar, double q_med_mvar[MAX_FASE])
   {
   //variáveis locais
   double       pq_mva[MAX_FASE+MAX_FASE];
   strHM        hm_ini;//, hm_fim;

   //define horario do patamar
   hm_ini.hora   = patamar->HoraCal;
   hm_ini.minuto = patamar->MinutoCal;
   //obtém valor da curva
   if (! curva->GetValor(hm_ini, pq_mva, curva->NumeroValores)) return;
   //determina pot.reativa das fases
   for (int ind_fase = indFASE_A; ind_fase <= indFASE_C; ind_fase++)
      {
      q_med_mvar[ind_fase] = pq_mva[ind_fase+ind_fase+1];
	  }
   }
//---------------------------------------------------------------------------
void   __fastcall TAjustaPorFase::CalculaDemandasGruposAjuste(VTGrupos *grupos, int ind_pat)
{
    VTGrupo *grupo = NULL;
	TList *lisGRUPOS = NULL;

	if (grupos == NULL)
		{return;}
	//preeche a lista com os grupo do grupos
	lisGRUPOS = new TList;
	grupos->PreencheLisGrupo(lisGRUPOS);
	//percorre todos grupos atualizando a info de demanda
	for (int i = 0; i < lisGRUPOS->Count; i++)
	{
		grupo = (VTGrupo*)lisGRUPOS->Items[i];
		grupo->p_car_ajus_mw[0]= grupo->p_car_ajus_mw[1] = grupo->p_car_ajus_mw[2] = 0;
		grupo->q_car_ajus_mvar[0] = grupo->q_car_ajus_mvar[1] = grupo->q_car_ajus_mvar[2] = 0;
		//como esse náo é o trifásico, preenche com a info de cada fase
		CalculaDemandaCargaFase(grupo->lisCargasAjus,ind_pat, grupo->p_car_ajus_mw);
		CalculaDemandaReativaCargaFase(grupo->lisCargasAjus,ind_pat, grupo->q_car_ajus_mvar);
		for (int n = indFASE_A; n < MAX_FASE; n++)
		{
            grupo->s_car_ajus_mva[n] = complex<double>(grupo->p_car_ajus_mw[n], grupo->q_car_ajus_mvar[n]);
		}
	}
	if (lisGRUPOS)
	{
		delete lisGRUPOS;
        lisGRUPOS = NULL;
	}
}
//---------------------------------------------------------------------------
void   __fastcall TAjustaPorFase::CalculaFatoresGruposAjuste(VTGrupos *grupos, bool pre_fluxo)
{
	VTGrupo *grupo;
	TList *lisGRUPO = new TList;
	double p_distribuivel_mw[MAX_FASE];
	double q_distribuivel_mvar[MAX_FASE];


	for(int ind_fase = indFASE_A; ind_fase < MAX_FASE; ind_fase++)
	{
		p_distribuivel_mw[ind_fase] = 0.;
		q_distribuivel_mvar[ind_fase] = 0;
	}
	if (grupos != NULL)
	{
		grupos->PreencheLisGrupo(lisGRUPO);
		for(int ind_fase = indFASE_A; ind_fase < MAX_FASE; ind_fase++)
		{
			//calcula o valor distribuivel
			if (pre_fluxo)
			{	//pre_fluxo
				//Pmed + Pger = (fatcor * Pcar_ajus) + Pcar_fix + Pcar_jusante
				//*fat_P = (p_med_mw + p_ger_mw - p_car_fixa_mw - p_car_jusante_mw) /
				//	   (p_car_ajus_mw);
				p_distribuivel_mw[ind_fase] = (p_med_mw[ind_fase] + p_ger_mw[ind_fase]
								 - p_car_fixa_mw[ind_fase] - p_car_jusante_mw[ind_fase])
								 - (p_car_ajus_mw[ind_fase]);
				//*fat_Q = (q_med_mvar + q_ger_mvar + q_cap_mvar - q_car_fixa_mvar - q_car_jusante_mvar) /
				//   (q_car_ajus_mvar);
				q_distribuivel_mvar[ind_fase] = (q_med_mvar[ind_fase] + q_ger_mvar[ind_fase]
								+ q_cap_mvar[ind_fase] - q_car_fixa_mvar[ind_fase]
								- q_car_jusante_mvar[ind_fase]) - (q_car_ajus_mvar[ind_fase]);
			}
			else
			{	//pos_fluxo
				//Pcar_ajus' = fatcor * Pcar_ajus
				//Pcar_ajus' = (Pmed - Pflow) + Pcar_ajus
				//fatcor * Pcar_ajus = (Pmed - Pflow) + Pcar_ajus
				p_distribuivel_mw[ind_fase] = (p_med_mw[ind_fase] - p_ent_flow_mw[ind_fase]);
				//*fat_Q = ((q_med_mvar - q_ent_flow_mvar) + q_car_ajus_mvar) /
				//   (q_car_ajus_mvar);
				q_distribuivel_mvar[ind_fase] = (q_med_mvar[ind_fase] - q_ent_flow_mvar[ind_fase]);
			}
			//atualiza as proporcoes
			grupos->AtualizaProporcao(ind_fase);
			//percorre todos grupos calculando os fatores
			for (int ng = 0; ng < lisGRUPO->Count; ng++)
			{
				grupo = (VTGrupo*)lisGRUPO->Items[ng];
				//calcula pra a potencia ativa
				if (!IsDoubleZero(grupo->p_car_ajus_mw[ind_fase]))
				{	//calcula fator de correção
					grupo->fatcorP[ind_fase] =
						 ((p_distribuivel_mw[ind_fase] * grupo->proporcaoP[ind_fase])
						 + grupo->p_car_ajus_mw[ind_fase])
						 / grupo->p_car_ajus_mw[ind_fase];
				}
				else
				{
					grupo->fatcorP[ind_fase] = 0.;
				}
				//calcula pra a potencia reativa
				if (!IsDoubleZero(grupo->q_car_ajus_mvar[ind_fase]))
				{	//calcula fator de correção
					grupo->fatcorQ[ind_fase] =
						((q_distribuivel_mvar[ind_fase] * grupo->proporcaoQ[ind_fase])
						+ grupo->q_car_ajus_mvar[ind_fase])
						/ grupo->q_car_ajus_mvar[ind_fase];
				}
				else
				{
					grupo->fatcorQ[ind_fase] = 0.;
				}
			}
		}
	}

   delete lisGRUPO;
}
//---------------------------------------------------------------------------
bool __fastcall TAjustaPorFase::CorrigeDemandaCarga(TList *lisCARGA, int ind_pat,
                        double fat_mult_p[MAX_FASE], double fat_mult_q[MAX_FASE])
   {
   //variáveis locais
   double          p_mw, q_mvar;
   complex<double> sf_mva[MAX_FASE];
   VTCarga         *carga;
   VTFases         *fases    = (VTFases*)apl->GetObject(__classid(VTFases));
   int             fase_tag;

   //loop p/ todas Cargas
   for (int nc = 0; nc < lisCARGA->Count; nc++)
      {
      carga = (VTCarga*)lisCARGA->Items[nc];
      //corrige o valor de cada fase
      if(! demanda->Get_S3f_mva(carga, ind_pat, sf_mva)) continue;
      for(int nf = indFASE_A; nf < MAX_FASE; nf++)
		 {//verifica se a carga possui a fase
         fase_tag = fases->IndexToTag[nf];
         if((carga->Fases & fase_tag) != fase_tag)
            {
            sf_mva[nf] = CZero();
            continue;
            }
         if(IsDoubleZero(fat_mult_p[nf])) continue;
		 p_mw       = sf_mva[nf].real() *  Abs(fat_mult_p[nf]);
         q_mvar     = sf_mva[nf].imag() * fat_mult_q[nf];
         sf_mva[nf] = complex<double>(p_mw, q_mvar);
         }
      // Atualiza demanda por fase
      if(! demanda->Set_S3f_mva(carga, ind_pat, sf_mva)) return(false);
      }
   return(true);
   }
//---------------------------------------------------------------------------
bool __fastcall TAjustaPorFase::CorrigeDemandaCarga(VTMedicao *medicao, int ind_pat)
{
	//variáveis locais
	double          p_mw, q_mvar;
	complex<double> sf_mva[MAX_FASE];
	VTCarga         *carga;
	TList           *lisCARGA = medicao->lisCAR_AJUS;
	VTFases         *fases    = (VTFases*)apl->GetObject(__classid(VTFases));
	int             fase_tag;
	VTGrupos        *grupos = NULL;
	VTGrupo         *grupo = NULL;
	int             indexTrifasico = 0;

	//recupera o grupos da medicao
	grupos = gerGrupos->ExisteGrupos(medicao);
	if (grupos == NULL)
	{
		return false;
	}
	//loop p/ todas Cargas
	for (int nc = 0; nc < lisCARGA->Count; nc++)
	{
		carga = (VTCarga*)lisCARGA->Items[nc];
        //verifica em qual grupo está
		grupo = grupos->ExisteGrupo(carga->Categoria); //TODO : testar se demora mto
		if (grupo == NULL)
			{continue;}
		//corrige o valor de cada fase
		if(! demanda->Get_S3f_mva(carga, ind_pat, sf_mva))
			{continue;}
		for(int nf=0; nf < MAX_FASE; nf++)
		{//verifica se a carga possui a fase
			fase_tag = fases->IndexToTag[nf];
			if((carga->Fases & fase_tag) != fase_tag)
			{
				sf_mva[nf] = CZero();
				continue;
			}
			//nao entendi pq somente verifica o P...
			if(IsDoubleZero(grupo->fatcorP[nf]))
				{continue;}
			p_mw       = sf_mva[nf].real() * Abs(grupo->fatcorP[nf]);
			q_mvar     = sf_mva[nf].imag() * grupo->fatcorQ[nf];
			sf_mva[nf] = complex<double>(p_mw, q_mvar);
		}
		// Atualiza demanda por fase
		if(! demanda->Set_S3f_mva(carga, ind_pat, sf_mva))
			{return(false);}
	}
	return(true);
}
//---------------------------------------------------------------------------
double __fastcall TAjustaPorFase::ExecutaAjuste(TList *lisNode, bool pos_fluxo)
   {
   //recebe objetos do apl
   demanda         = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
   fornecimento    = (VTFornecimento*)apl->GetObject(__classid(VTFornecimento));
   medicoes        = (VTMedicoes*)apl->GetObject(__classid(VTMedicoes));
   patamares       = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   //limpa string list
   linhas->Clear();
   //inicia
   if(pos_fluxo) return(ExecutaAjustePosFluxo(lisNode));
   else          return(ExecutaAjustePreFluxo(lisNode));
   }

//---------------------------------------------------------------------------
double __fastcall TAjustaPorFase::ExecutaAjustePosFluxo(TList *lisNode)
   {
   //variáveis locais
   AnsiString   str_iteracao, str_rede_erro;
   double       erro_max     = 0.;
   bool         convergiu    = false;
   int          num_max_iter = 5;
   VTBlocos     *blocos      = (VTBlocos*)apl->GetObject(__classid(VTBlocos));
   VTCronometro *cronometro  = (VTCronometro*)apl->GetObject(__classid(VTCronometro));
   VTProgresso  *progresso   = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
   VTElo        *elo;
   VTMedicao    *medicao;
   VTNode       *node;
   TList        *lisCargaJusante;

   //proteção
   if(! lisNode) return(0.);
   if(! blocos)  return(0.);
   //mostra/esconde progresso
   if(! PD.prog_enabled) progresso = NULL;
   //cria listas
   lisCargaJusante = new TList();
   //executa cálculo do fluxo de potência
   CalculaFluxo(false);
   //restaura cargas de redes BT que não convergiram o fluxo
//   RestauraCargasRedesSemFluxo();
   for (int num_iteracao = 1; (! convergiu)&&(num_iteracao <= num_max_iter); num_iteracao++)
      {//verifica cronometro
      cronometro->Stop("Ajuste de demanda");
      if(PD.tempo > 0.)
         {
         if(cronometro->TempoMSeg("Ajuste de demanda") > PD.tempo)
            {//interrompe processamento
            DefineDiagnosticoTempo(lisNode);
            return(0.);
            }
         }
      //continua cronometro
      cronometro->Start("Ajuste de demanda");
      //atualiza progresso
      str_iteracao = "Iteração " + IntToStr(num_iteracao);
      if(progresso) progresso->Add(str_iteracao);
      linhas->Add("\n" + str_iteracao);
      //reinicia erro máximo
      erro_max = 0.;
      //loop em cada node
      for(int n = 0; n < lisNode->Count; n++)
         {//determina medição associada ao node
         node    = (VTNode*)lisNode->Items[n];
         medicao = (VTMedicao*)node->LisObjectNode()->First();
         //verifica se Medicao deve ser tratada
         if (! medicao->enabled)       continue;
         //redefine listas de cargas ajustaveis e fixas, considerando lista de
         //cargas de redes que nao rodaram fluxo
//         RedefineLisCargaMed(medicao);
         //valida diagnóstico do fluxo
         if(! ValidaDiagFluxo(medicao)) continue;
         //valida erro da medicao
         if((num_iteracao > 1) && (medicao->erro < PD.erro_maximo)) continue;
         //determina todas as cargas a jusante da medição
         lisCargaJusante->Clear();
         if(! EqptoJusante(node, lisCargaJusante, eqptoCARGA))      continue;
         //determina elo da medicao
         elo = blocos->ExisteElo(medicao->chave_med);
         //ajusta cargas da medição
         medicao->erro = AjustePos(medicao, lisCargaJusante, elo);
         //executa cálculo do fluxo de potência
         if(! CalculaFluxo(false)) break;
         //restaura cargas de redes BT que não convergiram o último fluxo
//         RestauraCargasRedesSemFluxo();
         //recalcula erro
//         RecalculaMaiorErro(medicao, elo);
         //atualiza linhas
         str_rede_erro = medicao->chave_med->Codigo + DoubleToStr("\t %4.3f", medicao->erro);
         linhas->Add(str_rede_erro);
         //atualiza diagnóstico: inconsistência entre fluxo e medição
         if(medicao->erro < 0)
            {
            medicao->diagnostico = diagMED_ERRO_FLUXO;
            continue;
            }
         //salva maior fator de correção de todas as medições
         erro_max = Max(erro_max, medicao->erro);
         }
      convergiu = (erro_max <= PD.erro_maximo);
      }
   //atualiza diagnóstico para cada Medicao
   for(int n = 0; n < lisNode->Count; n++)
      {//determina medição associada ao node
      node    = (VTNode*)lisNode->Items[n];
      medicao = (VTMedicao*)node->LisObjectNode()->First();
      //verifica se Medicao deve ser tratada
      if (! medicao->enabled)                         continue;
      //verifica se houve erro no fluxo
      if (medicao->diagnostico == diagMED_ERRO_FLUXO) continue;
      //verifica se houve limitação no ajuste
      if(! PD.limite_ajuste)
         {//a medição com erro (se houver erro) passa a ser OK
         medicao->diagnostico = diagMED_OK;
         continue;
         }
      if (medicao->erro <= PD.erro_maximo)
         {//erro dentro da tolerância
         medicao->diagnostico = diagMED_OK;
         }
      else
         {//erro fora da tolerância
         medicao->diagnostico = diagMED_ERRO_FINAL;
         }
      }
   //salva arquivo com dados do string list
   SalvaArquivoEvolucao();
   //destroi listas
   if(lisCargaJusante) delete lisCargaJusante;
   return(erro_max);
   }

//---------------------------------------------------------------------------
double __fastcall TAjustaPorFase::ExecutaAjustePreFluxo(TList *lisNode)
   {
   //variáveis locais
   double      erro_max = 0.;
   bool        convergiu = false;
   int         num_max_iter = 10;
   VTBlocos    *blocos    = (VTBlocos*)apl->GetObject(__classid(VTBlocos));
   VTMedicao   *medicao;
   VTNode      *node;
   TList       *lisCargaJusante, *lisCapJusante;

   //proteção
   if(! lisNode) return(0.);
   if(! blocos)  return(0.);
   //cria listas
   lisCargaJusante = new TList();
   lisCapJusante = new TList();
   //loop em cada node
   for(int n = 0; n < lisNode->Count; n++)
      {//determina medição associada ao node
      node    = (VTNode*)lisNode->Items[n];
      medicao = (VTMedicao*)node->LisObjectNode()->First();
      //verifica se Medicao deve ser tratada
      if (! medicao->enabled)     continue;
      //não faz pré-ajuste se tiver geração no bloco com a medição
      if(medicao->lisGER_SUP->Count > 0) continue;
      //determina todas as cargas e capacitores a jusante da medição
      lisCargaJusante->Clear();
      lisCapJusante->Clear();
      if(! EqptoJusante(node, lisCargaJusante, eqptoCARGA)) continue;
      if(! EqptoJusante(node, lisCapJusante, eqptoCAPACITOR)) continue;
      //reinicia convergiu com valor inicial do erro //DVK 2016.05.31
      convergiu = (medicao->erro < PD.ajuste_maximo);
      for (int num_iteracao = 1; (! convergiu)&&(num_iteracao <= num_max_iter); num_iteracao++)
         {//ajusta cargas da medição
         medicao->erro = AjustePre(medicao, lisCargaJusante, lisCapJusante);
         convergiu = (medicao->erro < PD.erro_maximo);
         }
      //salva maior fator de correção de todas as medições
      erro_max = Max(erro_max, medicao->erro);
      }
   //destroi listas
   delete lisCargaJusante;
   delete lisCapJusante;

   return(erro_max);
   }

//---------------------------------------------------------------------------
void __fastcall TAjustaPorFase::FatorAjusteP(double fat_P[MAX_FASE], bool pre_fluxo)
   {
   //baseada da dedução em TAjustaTrifasico::FatorAjusteP
   for(int ind_fase = indFASE_A; ind_fase < MAX_FASE; ind_fase++)
      {
      if (IsDoubleZero(p_car_ajus_mw[ind_fase]))
         {//verifica se demanda das cargas ajustáveis é nula
         fat_P[ind_fase] = 0.;
         return;
         }
      //calcula fator de correção
      if(pre_fluxo)
         {
         fat_P[ind_fase] = (p_med_mw[ind_fase] + p_ger_mw[ind_fase]
                           - p_car_fixa_mw[ind_fase] - p_car_jusante_mw[ind_fase]) /
                           (p_car_ajus_mw[ind_fase]);
         }
      else
         {//pós-fluxo
         fat_P[ind_fase] =  ((p_med_mw[ind_fase] - p_ent_flow_mw[ind_fase])
                            + p_car_ajus_mw[ind_fase]) /
                            (p_car_ajus_mw[ind_fase]);
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TAjustaPorFase::FatorAjusteQ(double fat_Q[MAX_FASE], bool pre_fluxo)
   {
   //baseada da dedução em TAjustaTrifasico::FatorAjusteP
   for(int ind_fase = indFASE_A; ind_fase < MAX_FASE; ind_fase++)
      {
      if (IsDoubleZero(q_car_ajus_mvar[ind_fase]))
         {//verifica se demanda reativa das cargas ajustáveis é nula
         fat_Q[ind_fase] = 0.;
         return;
         }
      //calcula fator de correção
      if(pre_fluxo)
         {
         fat_Q[ind_fase] = (q_med_mvar[ind_fase] + q_ger_mvar[ind_fase] + q_cap_mvar[ind_fase]
                           - q_car_fixa_mvar[ind_fase] - q_car_jusante_mvar[ind_fase]) /
                           (q_car_ajus_mvar[ind_fase]);
         }
      else
         {//pós-fluxo
         fat_Q[ind_fase] = ((q_med_mvar[ind_fase] - q_ent_flow_mvar[ind_fase])
                           + q_car_ajus_mvar[ind_fase]) /
                           (q_car_ajus_mvar[ind_fase]);
         }
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TAjustaPorFase::FatorNegativo(double fat_mult_p[MAX_FASE])
   {
   for(int ind_fase = indFASE_A; ind_fase <= indFASE_C; ind_fase++)
      {//verifica se algum fator de correção é negativo
      if (fat_mult_p[ind_fase] < 0) return(true);
      }
   return(false);
   }
//---------------------------------------------------------------------------
double __fastcall TAjustaPorFase::MaiorErro(double fator_max,double fatorP[MAX_FASE] ,double fatorQ[MAX_FASE])
{
	double erro_max = 0;
	double erro;

	erro_max = (fator_max - 1)*(fator_max - 1);
	for(int ind_fase = indFASE_A; ind_fase <= indFASE_C; ind_fase++)
	{//determina maior
		erro = (fatorP[ind_fase] - 1)*(fatorP[ind_fase] - 1);
		if(erro > erro_max)
			{fator_max = fatorP[ind_fase];}
		//verifica Q
		erro = (fatorQ[ind_fase] - 1)*(fatorQ[ind_fase] - 1);
		if(erro > erro_max)
			{fator_max = fatorQ[ind_fase];}
	}

	return fator_max;
}
//---------------------------------------------------------------------------
double __fastcall TAjustaPorFase::MaiorFator(double fator[MAX_FASE])
   {
   //variaveis locais
   double maior;

   for(int ind_fase = indFASE_A; ind_fase <= indFASE_C; ind_fase++)
	  {//determina maior
	  if (ind_fase == indFASE_A) maior = fator[ind_fase];
	  else                       maior = Max(maior, fator[ind_fase]);
	  }
   return(maior);
   }

//---------------------------------------------------------------------------
double __fastcall TAjustaPorFase::MenorFator(double fator[MAX_FASE])
   {
   //variaveis locais
   double menor;

   for(int ind_fase = indFASE_A; ind_fase <= indFASE_C; ind_fase++)
      {//determina menor valor
      if (ind_fase == indFASE_A) menor = fator[ind_fase];
      else                       menor = Min(menor, fator[ind_fase]);
      }
   return(menor);
   }
//// ---------------------------------------------------------------------------
//void __fastcall TAjustaPorFase::NormalizaPesos(VTMedicao *medicao)
//{
//	VTCarga *carga;
//	double somaNotas = 0;
//	double mediaNotas = 0;
//	int    n_cargas = 0;
//	srand (time(NULL));
//
//	//percorre todas as cargas da medicao
//	for (int nc = 0; nc < medicao->lisCAR_AJUS->Count; nc++)
//	{
//		carga = (VTCarga*)medicao->lisCAR_AJUS->Items[nc];
//		//soma as notas
//		//teste
//		carga->NotaInspecao = Random(100);
//		somaNotas += carga->NotaInspecao;
//
//	}
//	//n cargas
//	n_cargas = medicao->lisCAR_AJUS->Count;
//
//	if (n_cargas == 0)
//	{
//		mediaNotas = 0;
//	}
//	else
//	{
//		mediaNotas = somaNotas / (double)n_cargas;
//	}
//	//normaliza
//	for (int nc = 0; nc < medicao->lisCAR_AJUS->Count; nc++)
//	{
//		carga = (VTCarga*)medicao->lisCAR_AJUS->Items[nc];
//
//		if (IsDoubleZero(mediaNotas))
//		{
//            carga->PesoAjuste = 1;
//		}
//		else
//		{
//			carga->PesoAjuste = carga->NotaInspecao / mediaNotas;
//		}
//	}
//
//}
//---------------------------------------------------------------------------
void __fastcall TAjustaPorFase::RecalculaMaiorErro(VTMedicao *medicao, VTElo *elo)
   { //função desnecessária, já que nesse método não há redes BT
   /*
   //variáveis locais
   VTPatamar   *patamar;
   VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   double      valor[MAX_FASE];
   double      maior = 0.;

   //proteção
   if(medicao == NULL) return;
   if(elo     == NULL) return;
   //loop p/ todos patamares
   for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
      {//zera variaveis
      ZeraValores();
      //calcula demanda da curva de medição no patamar
      CalculaDemandaMedicaoFase(medicao->curva_pq, patamares->Patamar[ind_pat], p_med_mw);
      //calcula demanda na entrada do bloco (elo), calculada pelo fluxo
      CalculaDemandaEntradaFase(medicao->ind_bar_ref, elo, ind_pat, p_ent_flow_mw);
      for(int ind_fase = indFASE_A; ind_fase < MAX_FASE; ind_fase++)
         {//calcula erro por fase
         if(IsDoubleZero(p_med_mw[ind_fase])) {valor[ind_fase] = 0.; continue;}
         valor[ind_fase] = p_ent_flow_mw[ind_fase] / p_med_mw[ind_fase];
         }
      //determina maior erro
	  maior = Max(MaiorFator(valor), maior);
      }
   //atualiza erro da medicao
   medicao->erro = fabs(1. - maior);
   */
   }

//---------------------------------------------------------------------------
void __fastcall TAjustaPorFase::SalvaArquivoEvolucao(void)
   {
   //variáveis locais
   VTPath     *path = (VTPath*)apl->GetObject(__classid(VTPath));
   AnsiString arq_txt;

   //salva na pasta Tmp
   arq_txt = path->DirTmp() + "\\EvolucaoAjuste.txt";
   linhas->SaveToFile(arq_txt);
   }

//---------------------------------------------------------------------------
void __fastcall TAjustaPorFase::ZeraValores(void)
   {
   //zera vetores
   for(int ind_fase = indFASE_A; ind_fase < MAX_FASE; ind_fase++)
      {
      p_med_mw[ind_fase]           = 0.;
      q_med_mvar[ind_fase]         = 0.;
      p_car_ajus_mw[ind_fase]      = 0.;
      q_car_ajus_mvar[ind_fase]    = 0.;
      p_car_fixa_mw[ind_fase]      = 0.;
      q_car_fixa_mvar[ind_fase]    = 0.;
      p_ent_flow_mw[ind_fase]      = 0.;
      q_ent_flow_mvar[ind_fase]    = 0.;
      p_car_jusante_mw[ind_fase]   = 0.;
      q_car_jusante_mvar[ind_fase] = 0.;
      p_ger_mw[ind_fase]           = 0.;
      q_ger_mvar[ind_fase]         = 0.;
      q_cap_mvar[ind_fase]         = 0.;
      }
   }

//---------------------------------------------------------------------------
//eof

