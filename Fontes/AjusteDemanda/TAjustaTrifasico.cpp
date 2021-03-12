//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Math.hpp>
#include "TAjustaTrifasico.h"
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
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCapacitor.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTFases.h"
#include "..\Rede\VTGerador.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTResFlowLig.h"
#include "..\Zona\VTZona.h"
#include "..\..\DLL_Inc\Curvas.h"
#include "..\..\DLL_Inc\Flow.h"
#include "..\..\DLL_Inc\Funcao.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função p/ criar objeto da classe
//---------------------------------------------------------------------------
VTAjusta* __fastcall NewObjAjustaTrifasico(VTApl *apl_owner)
   {
   try{
      return(new TAjustaTrifasico(apl_owner));
      }catch(Exception &e)
         {
         return(NULL);
         }
   }
   
//---------------------------------------------------------------------------
__fastcall TAjustaTrifasico::TAjustaTrifasico(VTApl *apl_owner)
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
__fastcall TAjustaTrifasico::~TAjustaTrifasico(void)
   {
   //destrói objetos locais
   if (apl) {delete apl; apl = NULL;}
   //destroi string list
   if(linhas) {delete linhas; linhas = NULL;}
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18386466
double __fastcall TAjustaTrifasico::AjustePos(VTMedicao *medicao,
                                              TList *lisCargaJusante,
                                              VTElo *elo)
   {
   //variáveis locais
   bool        pre_fluxo = false;
   double      fat_mult_p, fat_mult_q;
   double      fatcor_min, fatcor_max;
   double      erro_max = 0;
   int         pat_calculado = 0;
   VTGrupos    *grupos = NULL;

   //proteção
   if((! medicao) || (! elo))            return(0.);
   if (medicao->lisCAR_AJUS->Count == 0) return(0.);
   //loop p/ todos patamares
   for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
      {//verifica se patamar está habilitado
	  if(! pat[ind_pat])  continue;
	  //zera variaveis
	  ZeraValores();
      //calcula demanda  da curva de medição no patamar   //DVK 2014.02.27
      p_med_mw = CalculaDemandaMedicao(medicao, patamares->Patamar[ind_pat]);
      //calcula demanda na entrada do bloco (elo), calculada pelo fluxo
      p_ent_flow_mw = CalculaDemandaEntrada(medicao->ind_bar_ref, elo, ind_pat);
      //calcula demanda das Cargas fixas no patamar
      p_car_fixa_mw = CalculaDemandaCarga(medicao->lisCAR_FIXA, ind_pat);
      //calcula demanda das Cargas a jusante no patamar
      p_car_jusante_mw = CalculaDemandaCarga(lisCargaJusante, ind_pat);
      //calcula demanda das Cargas ajustáveis no patamar
      p_car_ajus_mw = CalculaDemandaCarga(medicao->lisCAR_AJUS, ind_pat);
      //calcula demanda dos geradores PQ ou PV no patamar
      p_ger_mw = CalculaDemandaGerador(medicao->lisGER_SUP, ind_pat);
      //calcula fator de ajuste P
      FatorAjusteP(&fat_mult_p, pre_fluxo);
      if(fat_mult_p < 0)
         {//não atende as condições para ajustar a demanda
         return(-1);
         }
      //calcula demanda reativa da medição no patamar
      q_med_mvar = CalculaDemandaReativaMedicao(medicao->curva_pq, patamares->Patamar[ind_pat]);
      //calcula demanda reativa na entrada do bloco (elo), calculada pelo fluxo
      q_ent_flow_mvar = CalculaDemandaReativaEntrada(medicao->ind_bar_ref, elo, ind_pat);
      //calcula demanda das Cargas fixas no patamar
      q_car_fixa_mvar = CalculaDemandaReativaCarga(medicao->lisCAR_FIXA, ind_pat);
      //calcula demanda das Cargas a jusante no patamar
      q_car_jusante_mvar = CalculaDemandaReativaCarga(lisCargaJusante, ind_pat);
      //calcula demanda reativa das Cargas ajustáveis no patamar
      q_car_ajus_mvar = CalculaDemandaReativaCarga(medicao->lisCAR_AJUS, ind_pat);
      //calcula demanda reativa dos geradores
      q_ger_mvar = CalculaDemandaReativaGerador(medicao->lisGER_SUP, ind_pat);
      //calcula fator de ajuste Q
      FatorAjusteQ(&fat_mult_q, pre_fluxo);
	  //calcula as demandas de cara grupo da medicao
	  grupos = gerGrupos->ExisteGrupos(medicao);
	  CalculaDemandasGruposAjuste(grupos, ind_pat);
	  //calcula os fatores de ajuste de cada grupo
	  CalculaFatoresGruposAjuste(grupos,pre_fluxo);
      //corrige demanda das Cargas ajustáveis
	  //CorrigeDemandaCarga(medicao, ind_pat, fat_mult_p, fat_mult_q);
	  CorrigeDemandaCarga(medicao,ind_pat);
//	  //corrige demanda das Cargas ajustáveis
//	  CorrigeDemandaCarga(medicao, ind_pat, fat_mult_p, fat_mult_q);
      //salva maior fator de correção
	  if (pat_calculado == 0)
         {
         fatcor_min = Min(fat_mult_p, fat_mult_q);
         fatcor_max = Max(fat_mult_p, fat_mult_q);
         }
      else
         {
		 fatcor_min = Min(fat_mult_p, fatcor_min);
         fatcor_min = Min(fat_mult_q, fatcor_min);
		 fatcor_max = Max(fat_mult_p, fatcor_max);
         fatcor_max = Max(fat_mult_q, fatcor_max);
		 }
	  pat_calculado++;
//	  erro_max = MaiorErro(erro_max,fat_mult_p, fat_mult_q);
	  }
   //determina maior erro
   return(Max(fabs(fatcor_max - 1.), fabs(1. - fatcor_min)));
//    return erro_max;
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18386466
double __fastcall TAjustaTrifasico::AjustePre(VTMedicao *medicao,
                                              TList *lisCargaJusante,
                                              TList *lisCapJusante)
   {
   //variáveis locais
   bool        pre_fluxo  = true;
   double      fatcor_max = 0.;
   double      fat_mult_p, fat_mult_q;
   VTGrupos    *grupos = NULL;

   //proteção
   if ((! medicao) || (! lisCargaJusante)) return(0.);
   if (medicao->lisCAR_AJUS->Count == 0)   return(0.);
   //loop p/ todos patamares
   for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
      {//verifica se patamar está habilitado
	  if(! pat[ind_pat])  continue;
	  //zera variaveis
      ZeraValores();
      //calcula demanda  da curva de medição no patamar   //DVK 2014.02.27
      p_med_mw = CalculaDemandaMedicao(medicao, patamares->Patamar[ind_pat]);
      //calcula demanda das Cargas ajustáveis no patamar
      p_car_ajus_mw = CalculaDemandaCarga(medicao->lisCAR_AJUS, ind_pat);
      //calcula demanda das Cargas a jusante no patamar
      p_car_jusante_mw = CalculaDemandaCarga(lisCargaJusante, ind_pat);
      //calcula demanda das Cargas fixas no patamar
      p_car_fixa_mw = CalculaDemandaCarga(medicao->lisCAR_FIXA, ind_pat);
      //calcula demanda dos geradores PQ ou PV no patamar
      p_ger_mw = CalculaDemandaGerador(medicao->lisGER_SUP, ind_pat);
      //calcula fator de ajuste P
      FatorAjusteP(&fat_mult_p, pre_fluxo);
      if(fat_mult_p < 0)
         {//não atende as condições para ajustar a demanda
         return(-1);
         }
      //calcula demanda reativa da medição
      q_med_mvar = CalculaDemandaReativaMedicao(medicao->curva_pq, patamares->Patamar[ind_pat]);
      //calcula demanda das Cargas ajustáveis no patamar
      q_car_ajus_mvar = CalculaDemandaReativaCarga(medicao->lisCAR_AJUS, ind_pat);
      //calcula demanda das Cargas a jusante no patamar
      q_car_jusante_mvar = CalculaDemandaReativaCarga(lisCargaJusante, ind_pat);
      //calcula demanda das Cargas fixas no patamar
      q_car_fixa_mvar = CalculaDemandaReativaCarga(medicao->lisCAR_FIXA, ind_pat);
      //calcula demanda reativa dos geradores
      q_ger_mvar = CalculaDemandaReativaGerador(medicao->lisGER_SUP, ind_pat);
      //calcula demanda dos capacitores e a jusante
      q_cap_mvar = CalculaDemandaCapacitor(medicao->lisCAPACITOR, ind_pat);
      q_cap_mvar += CalculaDemandaCapacitor(lisCapJusante, ind_pat);
      //calcula fator de ajuste Q
      FatorAjusteQ(&fat_mult_q, pre_fluxo);
	  //calcula as demandas de cara grupo da medicao
	  grupos = gerGrupos->ExisteGrupos(medicao);
	  CalculaDemandasGruposAjuste(grupos, ind_pat);
	  //calcula os fatores de ajuste de cada grupo
	  CalculaFatoresGruposAjuste(grupos,pre_fluxo);
      //corrige demanda das Cargas ajustáveis
	  //CorrigeDemandaCarga(medicao, ind_pat, fat_mult_p, fat_mult_q);
	  CorrigeDemandaCarga(medicao,ind_pat);
      //salva maior fator de correção
	  fatcor_max  = Max(fatcor_max, fat_mult_p);
      fatcor_max = Max(fatcor_max, fat_mult_q);
	  }
   return(fabs(fatcor_max - 1.));
   }

//---------------------------------------------------------------------------
double __fastcall TAjustaTrifasico::CalculaDemandaCapacitor(TList *lisCAP,
                                                               int ind_pat)
   {
   //variáveis locais
   double      q_mvar = 0;
   VTCapacitor *capacitor;

   //loop p/ todos capacitores
   for(int ncap = 0; ncap < lisCAP->Count; ncap++)
      {
      capacitor = (VTCapacitor*)lisCAP->Items[ncap];
      q_mvar   +=  demanda->Q_mvar[capacitor][ind_pat];
      }
   return(q_mvar);
   }

//---------------------------------------------------------------------------
double __fastcall TAjustaTrifasico::CalculaDemandaCarga(TList *lisCAR, int ind_pat)
   {
   //variáveis locais
   double    p_mw = 0;
   VTCarga   *carga;
   VTDemanda *demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));

   //loop p/ todas Cargas
   for (int nc = 0; nc < lisCAR->Count; nc++)
      {
      carga = (VTCarga*)lisCAR->Items[nc];
      //soma potência ativa total
      p_mw += demanda->P_mw[carga][ind_pat];
      }
   return(p_mw);
   }

//---------------------------------------------------------------------------
double __fastcall TAjustaTrifasico::CalculaDemandaEntrada(int ind_bar_ref, VTElo *elo, int ind_pat)
   {
   //variáveis locais
   //int        ind_bar;
   double     p_mw = 0;
   VTLigacao  *ligacao;

   //proteção
   if(! elo)         return 0.;
   if(! elo->Chave)  return 0.;
   ligacao = elo->Chave;
   //determina potência ativa ligacao
   p_mw = ligacao->resflow->P_mw[ind_bar_ref][ind_pat];
   return(p_mw);
   }

//---------------------------------------------------------------------------
double __fastcall TAjustaTrifasico::CalculaDemandaGerador(TList *lisGER, int ind_pat)
   {
   //variáveis locais
   double     p_mw = 0;
   VTGerador  *gerador;
   strSUP     vet_sup;

   //loop p/ todas Cargas
   for (int nc = 0; nc < lisGER->Count; nc++)
      {
      gerador = (VTGerador*)lisGER->Items[nc];
      if(! fornecimento->Get_PQVT_trifasico(gerador, ind_pat, &vet_sup)) continue;
      //soma potência ativa total
      p_mw += vet_sup.p;
      }
   return(p_mw);
   }

//---------------------------------------------------------------------------
double __fastcall TAjustaTrifasico::CalculaDemandaMedicao(VTMedicao *medicao,
                                                   VTPatamar *patamar)
   {
   //variáveis locais
   double       p_mw = 0;
   double       pq_mva[MAX_FASE+MAX_FASE];
   strHM        hm_ini;//, hm_fim;

   //determina horario do patamar
   hm_ini.hora   = patamar->HoraCal;
   hm_ini.minuto = patamar->MinutoCal;
   //obtém valor da curva
   if (! medicao->curva_pq->GetValor(hm_ini, pq_mva, medicao->curva_pq->NumeroValores)) return(p_mw);
   //acumula demanda em MW
    for (int ind_fase = indFASE_A; ind_fase <= indFASE_C; ind_fase++)
      {
      p_mw += pq_mva[ind_fase+ind_fase];
      }
   return(p_mw); //kwh => mwh
   }

//---------------------------------------------------------------------------
double __fastcall TAjustaTrifasico::CalculaDemandaReativaCarga(TList *lisCAR,
                                                               int ind_pat)
   {
   //variáveis locais
   double    q_mvar = 0;
   VTCarga   *carga;

   //loop p/ todas Cargas
   for (int nc = 0; nc < lisCAR->Count; nc++)
      {
      carga = (VTCarga*)lisCAR->Items[nc];
      //soma potência ativa total
      q_mvar += demanda->Q_mvar[carga][ind_pat];
      }
   return(q_mvar);
   }

//---------------------------------------------------------------------------
double __fastcall TAjustaTrifasico::CalculaDemandaReativaEntrada(int ind_bar_ref,
                                                                 VTElo *elo,
                                                                 int ind_pat)
   {
   //variáveis locais
   double     q_mvar = 0;
   VTLigacao  *ligacao;

   //proteção
   if(! elo)         return 0.;
   if(! elo->Chave)  return 0.;
   ligacao = elo->Chave;
   //determina potência reativa ligacao
   q_mvar = ligacao->resflow->Q_mvar[ind_bar_ref][ind_pat];
   return(q_mvar);
   }

//---------------------------------------------------------------------------
double __fastcall TAjustaTrifasico::CalculaDemandaReativaGerador(TList *lisGER,
                                                                 int ind_pat)
   {
   //variáveis locais
   double     q_mvar = 0;
   VTGerador  *gerador;
   strSUP     vet_sup;

   //loop p/ todas Cargas
   for (int nc = 0; nc < lisGER->Count; nc++)
      {
      gerador = (VTGerador*)lisGER->Items[nc];
      if(! fornecimento->Get_PQVT_trifasico(gerador, ind_pat, &vet_sup)) continue;
      //soma potência reativa total
      q_mvar += vet_sup.q;
      }
   return(q_mvar);
   }

//---------------------------------------------------------------------------
double __fastcall TAjustaTrifasico::CalculaDemandaReativaMedicao(VTCurva *curva,
                                                            VTPatamar *patamar)
   {
   //variáveis locais
   double       q_mvar = 0;
   double       pq_mva[MAX_FASE+MAX_FASE];
   strHM        hm_ini;//, hm_fim;

   //determina horario do patamar
   hm_ini.hora   = patamar->HoraCal;
   hm_ini.minuto = patamar->MinutoCal;
   //obtém valor da curva
   if (! curva->GetValor(hm_ini, pq_mva, curva->NumeroValores)) return(0.);
   //totaliza pot.reativa das fases
    for (int ind_fase = indFASE_A; ind_fase <= indFASE_C; ind_fase++)
      {
      q_mvar += pq_mva[ind_fase+ind_fase+1];
      }
   return(q_mvar);
   }
//---------------------------------------------------------------------------
void   __fastcall TAjustaTrifasico::CalculaDemandasGruposAjuste(VTGrupos *grupos, int ind_pat)
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
		//como esse é o trifásico, preenche com a info trifasica no indice 0
		grupo->p_car_ajus_mw[0] = CalculaDemandaCarga(grupo->lisCargasAjus,ind_pat);
		grupo->q_car_ajus_mvar[0] = CalculaDemandaReativaCarga(grupo->lisCargasAjus,ind_pat);
		grupo->s_car_ajus_mva[0] = complex<double>(grupo->p_car_ajus_mw[0], grupo->q_car_ajus_mvar[0]);
	}
	if (lisGRUPOS)
	{
		delete lisGRUPOS;
		lisGRUPOS = NULL;
	}
}
//---------------------------------------------------------------------------
void   __fastcall TAjustaTrifasico::CalculaFatoresGruposAjuste(VTGrupos *grupos, bool pre_fluxo)
{
	VTGrupo *grupo;
	TList *lisGRUPO = new TList;
	double p_distribuivel_mw = 0;
	double q_distribuivel_mvar = 0;
	int indexTrifasico = 0;

	if (grupos != NULL)
	{
		grupos->PreencheLisGrupo(lisGRUPO);
		//calcula o valor distribuivel
		if (pre_fluxo)
		{	//pre_fluxo
			//Pmed + Pger = (fatcor * Pcar_ajus) + Pcar_fix + Pcar_jusante
			//*fat_P = (p_med_mw + p_ger_mw - p_car_fixa_mw - p_car_jusante_mw) /
			//	   (p_car_ajus_mw);
			p_distribuivel_mw = (p_med_mw + p_ger_mw - p_car_fixa_mw - p_car_jusante_mw) - (p_car_ajus_mw);
			//*fat_Q = (q_med_mvar + q_ger_mvar + q_cap_mvar - q_car_fixa_mvar - q_car_jusante_mvar) /
			//   (q_car_ajus_mvar);
			q_distribuivel_mvar = (q_med_mvar + q_ger_mvar + q_cap_mvar - q_car_fixa_mvar - q_car_jusante_mvar) - (q_car_ajus_mvar);
		}
		else
		{	//pos_fluxo
			//Pcar_ajus' = fatcor * Pcar_ajus
			//Pcar_ajus' = (Pmed - Pflow) + Pcar_ajus
			//fatcor * Pcar_ajus = (Pmed - Pflow) + Pcar_ajus
			p_distribuivel_mw = (p_med_mw - p_ent_flow_mw);
			//*fat_Q = ((q_med_mvar - q_ent_flow_mvar) + q_car_ajus_mvar) /
			//   (q_car_ajus_mvar);
			q_distribuivel_mvar = (q_med_mvar - q_ent_flow_mvar);
		}
		//atualiza as proporcoes
		grupos->AtualizaProporcao(indexTrifasico);
		//percorre todos grupos calculando os fatores
		for (int ng = 0; ng < lisGRUPO->Count; ng++)
		{
			grupo = (VTGrupo*)lisGRUPO->Items[ng];
			//calcula pra a potencia ativa
			if (!IsDoubleZero(grupo->p_car_ajus_mw[indexTrifasico]))
			{	//calcula fator de correção
				grupo->fatcorP[indexTrifasico] =
					 ((p_distribuivel_mw * grupo->proporcaoP[indexTrifasico])
					 + grupo->p_car_ajus_mw[indexTrifasico])
					 / grupo->p_car_ajus_mw[indexTrifasico];
			}
			else
			{
				grupo->fatcorP[indexTrifasico] = 0.;
			}
			//calcula pra a potencia reativa
			if (!IsDoubleZero(grupo->q_car_ajus_mvar[indexTrifasico]))
			{	//calcula fator de correção
				grupo->fatcorQ[indexTrifasico] =
					((q_distribuivel_mvar * grupo->proporcaoQ[indexTrifasico])
					+ grupo->q_car_ajus_mvar[indexTrifasico])
					/ grupo->q_car_ajus_mvar[indexTrifasico];
			}
			else
			{
				grupo->fatcorQ[indexTrifasico] = 0.;
			}
		}
	}

   delete lisGRUPO;
}
//---------------------------------------------------------------------------
/*
//---------------------------------------------------------------------------
bool __fastcall TAjustaTrifasico::CorrigeDemandaCarga(VTMedicao *medicao, int ind_pat,
                                          double fat_mult_p, double fat_mult_q)
   {
   //variáveis locais
   bool    sucesso = true;
   double  ang_grau;
   VTBarra* barra_ref;
   VTCarga* carga;

   //proteção
   if(! medicao->chave_med) return(false);
   //loop p/ todas Cargas ajustáveis
   for (int nc = 0; (sucesso)&&(nc < medicao->lisCAR_AJUS->Count); nc++)
      {
      carga = (VTCarga*)medicao->lisCAR_AJUS->Items[nc];
      //proteção: carga conectada a um bloco isolado
      if (carga->pbarra->zona == NULL) continue;
      barra_ref = medicao->chave_med->pbarra1;
      //gurda fatores de correção
      for(int ind_fase = indFASE_A; ind_fase < MAX_FASE; ind_fase++)
         {
         fatcorP[ind_fase] = fat_mult_p/3.;
         fatcorQ[ind_fase] = fat_mult_q/3.;
         }
      //calcula defasagem entre zonas da Barra da Carga e da Barra de medição
      ang_grau = fabs(carga->pbarra->zona->T_grau[indFASE_A] - barra_ref->zona->T_grau[indFASE_A]);
      //verifica se carga ligada após um trafo DY
      if (IsDoubleZero(ang_grau))
         {//corrige o valor de cada fase
         sucesso = CorrigeDemandaCargaEqvTrafoYY(carga, ind_pat);
         }
      else if (IsDoubleZero(ang_grau - 30.))
         {//corrige o valor de cada fase
         sucesso = CorrigeDemandaCargaEqvTrafoDY(carga, ind_pat);
         }
      }
   return(sucesso);
   }
*/
//---------------------------------------------------------------------------
bool __fastcall TAjustaTrifasico::CorrigeDemandaCarga(VTMedicao *medicao, int ind_pat, double fat_mult_p, double fat_mult_q)
   {
   //variáveis locais
   double          p_mw, q_mvar;
   complex<double> sf_mva[MAX_FASE];
   VTCarga         *carga;
   TList           *lisCARGA = medicao->lisCAR_AJUS;
   VTFases         *fases    = (VTFases*)apl->GetObject(__classid(VTFases));
   int             fase_tag;

   //loop p/ todas Cargas
   for (int nc = 0; nc < lisCARGA->Count; nc++)
      {
      carga = (VTCarga*)lisCARGA->Items[nc];
      //verifica se é uma Carga equivalente com demanda definida por fase
      if (carga->DefinidaPorFase)
         {//corrige o valor de cada fase
         if(! demanda->Get_S3f_mva(carga, ind_pat, sf_mva)) continue;
         for(int nf=0; nf < MAX_FASE; nf++)
            {//verifica se a carga possui a fase
            fase_tag = fases->IndexToTag[nf];
            if((carga->Fases & fase_tag) != fase_tag)
               {
               sf_mva[nf] = CZero();
               continue;
               }
            if(IsDoubleZero(fat_mult_p)) continue;
            p_mw       = sf_mva[nf].real() * Abs(fat_mult_p);
            q_mvar     = sf_mva[nf].imag() * fat_mult_q;
            sf_mva[nf] = complex<double>(p_mw, q_mvar);
            }
         // Atualiza demanda por fase
         if(! demanda->Set_S3f_mva(carga, ind_pat, sf_mva)) return(false);
         }
      else
         {//determina demanda total
         p_mw   = demanda->P_mw[carga][ind_pat];
         q_mvar = demanda->Q_mvar[carga][ind_pat];
         //atualiza demanda total
         demanda->P_mw[carga][ind_pat]   = p_mw   * fat_mult_p;
         demanda->Q_mvar[carga][ind_pat] = q_mvar * fat_mult_q;
         }
      }
   return(true);
   }
//---------------------------------------------------------------------------
bool __fastcall TAjustaTrifasico::CorrigeDemandaCarga(VTMedicao *medicao, int ind_pat)
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
		//verifica se é uma Carga equivalente com demanda definida por fase
		if (carga->DefinidaPorFase)
		{//corrige o valor de cada fase
			if(! demanda->Get_S3f_mva(carga, ind_pat, sf_mva))
				{continue;}
			//nao entendi pq somente verifica o P...
			if(IsDoubleZero(grupo->fatcorP[indexTrifasico]))
					{continue;}
			for(int nf=0; nf < MAX_FASE; nf++)
			{//verifica se a carga possui a fase
				fase_tag = fases->IndexToTag[nf];
				if((carga->Fases & fase_tag) != fase_tag)
				{
					sf_mva[nf] = CZero();
					continue;
				}
				p_mw       = sf_mva[nf].real() * Abs(grupo->fatcorP[indexTrifasico]);
				q_mvar     = sf_mva[nf].imag() * grupo->fatcorQ[indexTrifasico];
				sf_mva[nf] = complex<double>(p_mw, q_mvar);
			}
			// Atualiza demanda por fase
			if(! demanda->Set_S3f_mva(carga, ind_pat, sf_mva))
				{return(false);}
		}
		else
		{	//determina demanda total
			p_mw   = demanda->P_mw[carga][ind_pat];
			q_mvar = demanda->Q_mvar[carga][ind_pat];
			//atualiza demanda total
			demanda->P_mw[carga][ind_pat]   = p_mw   * Abs(grupo->fatcorP[indexTrifasico]);
			demanda->Q_mvar[carga][ind_pat] = q_mvar * grupo->fatcorQ[indexTrifasico];
		}
	}
	return(true);
}
//---------------------------------------------------------------------------
double __fastcall TAjustaTrifasico::ExecutaAjuste(TList *lisNode, bool pos_fluxo)
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
double __fastcall TAjustaTrifasico::ExecutaAjustePosFluxo(TList *lisNode)
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
//   CalculaFluxo(false);
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
      str_iteracao = "\nIteração " + IntToStr(num_iteracao);
      linhas->Add(str_iteracao);
      //reinicia erro máximo
      erro_max = 0.;
      //executa cálculo do fluxo de potência
      if(! CalculaFluxo(false)) break;
      //restaura cargas de redes BT que não convergiram o último fluxo
      RestauraCargasRedesSemFluxo();
      //recalcula erro de cada medição
	  RecalculaMaiorErro();
      //loop em cada node
      for(int n = 0; n < lisNode->Count; n++)
         {//determina medição associada ao node
         node    = (VTNode*)lisNode->Items[n];
         medicao = (VTMedicao*)node->LisObjectNode()->First();
         //verifica se Medicao deve ser tratada
         if (! medicao->enabled)     continue;
         //redefine listas de cargas ajustaveis e fixas, considerando lista de
         //cargas de redes que nao rodaram fluxo
         RedefineLisCargaMed(medicao);
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
   //executa último cálculo do fluxo de potência
   CalculaFluxo(false);
   //restaura cargas de redes BT que não convergiram o último fluxo
   RestauraCargasRedesSemFluxo();
   //recalcula erro de cada medição
   RecalculaMaiorErro();
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
double __fastcall TAjustaTrifasico::ExecutaAjustePreFluxo(TList *lisNode)
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
      node = (VTNode*)lisNode->Items[n];
      medicao = (VTMedicao*)node->LisObjectNode()->First();
      //determina todas as cargas e capacitores a jusante da medição
      lisCargaJusante->Clear();
      lisCapJusante->Clear();
      if(! EqptoJusante(node, lisCargaJusante, eqptoCARGA)) continue;
      if(! EqptoJusante(node, lisCapJusante, eqptoCAPACITOR)) continue;
      //reinicia convergiu com valor inicial do erro   //DVK 2016.05.31
//      convergiu = (medicao->erro < PD.ajuste_maximo);
      convergiu = (medicao->erro < PD.erro_maximo);
      for (int num_iteracao = 1; (! convergiu)&&(num_iteracao <= num_max_iter); num_iteracao++)
         {//ajusta cargas da medição
         medicao->erro = AjustePre(medicao, lisCargaJusante, lisCapJusante);
         convergiu = (medicao->erro < PD.ajuste_maximo);
         }
      //salva maior fator de correção de todas as medições
      erro_max = Max(erro_max, medicao->erro);
      }
   //destroi listas
   if(lisCargaJusante) delete lisCargaJusante;
   if(lisCapJusante)   delete lisCapJusante;
   return(erro_max);
   }

//---------------------------------------------------------------------------
void __fastcall TAjustaTrifasico::FatorAjusteP(double *fat_P, bool pre_fluxo)
   {
   if (IsDoubleZero(p_car_ajus_mw))
      {//verifica se demanda das cargas ajustáveis é nula
      *fat_P = 0.;
      return;
      }
   //calcula fator de correção
   if(pre_fluxo)
      {//pre_fluxo
      //Pmed + Pger = (fatcor * Pcar_ajus) + Pcar_fix + Pcar_jusante
      *fat_P = (p_med_mw + p_ger_mw - p_car_fixa_mw - p_car_jusante_mw) /
               (p_car_ajus_mw);
      }
   else
      {//pos_fluxo
      //Pcar_ajus' = fatcor * Pcar_ajus
      //Pcar_ajus' = (Pmed - Pflow) + Pcar_ajus
      //fatcor * Pcar_ajus = (Pmed - Pflow) + Pcar_ajus
      *fat_P = ((p_med_mw - p_ent_flow_mw) + p_car_ajus_mw) /
               (p_car_ajus_mw);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TAjustaTrifasico::FatorAjusteQ(double *fat_Q, bool pre_fluxo)
   {
   //baseada da dedução em TAjustaTrifasico::FatorAjusteP
   if (IsDoubleZero(q_car_ajus_mvar))
      {//verifica se demanda reativa das cargas ajustáveis é nula
      *fat_Q = 0.;
      return;
      }
   //calcula fator de correção
   if(pre_fluxo)
      {
      *fat_Q = (q_med_mvar + q_ger_mvar + q_cap_mvar - q_car_fixa_mvar - q_car_jusante_mvar) /
               (q_car_ajus_mvar);
      }
   else
      {//pós-fluxo
      *fat_Q = ((q_med_mvar - q_ent_flow_mvar) + q_car_ajus_mvar) /
               (q_car_ajus_mvar);
	  }
   }
//---------------------------------------------------------------------------
double __fastcall TAjustaTrifasico::MaiorErro(double fator_max,double fatorP, double fatorQ)
{
	double erro_max = 0;
	double erro;

	erro_max = (fator_max - 1)*(fator_max - 1);
	//determina maior
	erro = (fatorP - 1)*(fatorP - 1);
	if(erro > erro_max)
		{fator_max = fatorP;}
	//verifica Q
	erro = (fatorQ - 1)*(fatorQ - 1);
	if(erro > erro_max)
		{fator_max = fatorQ;}
	return fator_max;
}
//// ---------------------------------------------------------------------------
//void __fastcall TAjustaTrifasico::NormalizaPesos(VTMedicao *medicao, int ind_pat)
//{
//	VTCarga *carga;
//	double somaNotas = 0;
//	double mediaNotas = 0;
//	int    n_cargas = 0;
//	double p_total_mw = 0;
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
void __fastcall TAjustaTrifasico::RecalculaMaiorErro(void)
   {
   //variáveis locais
   VTBlocos    *blocos    = (VTBlocos*)apl->GetObject(__classid(VTBlocos));
   VTElo       *elo;
   VTMedicao   *medicao;
   //VTPatamar   *patamar;
   VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   double      valor;
   double      maior = 0.;
   TList       *lisMEDICAO = medicoes->LisMedicao();

   for(int nm = 0; nm < lisMEDICAO->Count; nm++)
      {
      medicao = (VTMedicao*)lisMEDICAO->Items[nm];
      elo = blocos->ExisteElo(medicao->chave_med);
      if(elo == NULL) continue;
      //loop p/ todos patamares
      for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
         {//verifica se patamar está habilitado
		 if(! pat[ind_pat])  continue;
		 //zera variaveis
         ZeraValores();
         //calcula demanda da curva de medição no patamar
         p_med_mw = CalculaDemandaMedicao(medicao, patamares->Patamar[ind_pat]);
         //calcula demanda na entrada do bloco (elo), calculada pelo fluxo
         p_ent_flow_mw = CalculaDemandaEntrada(medicao->ind_bar_ref, elo, ind_pat);
         //calcula erro
         if(IsDoubleZero(p_med_mw)) valor = 0.;
         valor = fabs(1. - (p_ent_flow_mw / p_med_mw));
         //determina maior erro
         maior = Max(valor, maior);
         }
      //atualiza erro da medicao
      medicao->erro = maior;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TAjustaTrifasico::SalvaArquivoEvolucao(void)
   {
   //variáveis locais
   VTPath     *path = (VTPath*)apl->GetObject(__classid(VTPath));
   AnsiString arq_txt;

   //salva na pasta Tmp
   arq_txt = path->DirTmp() + "\\EvolucaoAjuste.txt";
   linhas->SaveToFile(arq_txt);
   }

//---------------------------------------------------------------------------
void __fastcall TAjustaTrifasico::ZeraValores(void)
   {
   //zera variaveis
   p_med_mw           = 0.;
   q_med_mvar         = 0.;
   p_car_ajus_mw      = 0.;
   q_car_ajus_mvar    = 0.;
   p_car_fixa_mw      = 0.;
   q_car_fixa_mvar    = 0.;
   p_ent_flow_mw      = 0.;
   q_ent_flow_mvar    = 0.;
   p_ger_mw           = 0.;
   q_ger_mvar         = 0.;
   p_car_jusante_mw   = 0.;
   q_car_jusante_mvar = 0.;
   q_cap_mvar         = 0.;
   }

//---------------------------------------------------------------------------
//eof

