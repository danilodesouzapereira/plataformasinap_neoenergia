//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//#include <Math.hpp>
#include "TResFluxo.h"
#include "..\Apl\VTApl.h"
#include "..\Calculo\Comum13\VTGeralC.h"
#include "..\Complexo\Complexo.h"
#include "..\Potencia\VTFornecimento.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTBateria.h"
#include "..\Rede\VTGerador.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTNo.h"
#include "..\Rede\VTRegulador.h"
#include "..\Rede\VTResFlowBar.h"
#include "..\Rede\VTResFlowLig.h"
#include "..\Rede\VTResFlowSup.h"
#include "..\Rede\VTSuprimento.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe
//---------------------------------------------------------------------------
VTResFluxo* __fastcall NewObjResFluxo(VTApl *apl)
   {
	return(new TResFluxo(apl));
   }

//---------------------------------------------------------------------------
__fastcall TResFluxo::TResFluxo(VTApl *apl)
   {
	//salva ponteiro para objeto
	this->apl    = apl;
	fornecimento = (VTFornecimento*)apl->GetObject(__classid(VTFornecimento));
	//cria objetos
	lisEQP = new TList();
	}

//---------------------------------------------------------------------------
__fastcall TResFluxo::~TResFluxo(void)
   {
	//destrói lista sem destrui seus objetos
	if (lisEQP) {delete lisEQP; lisEQP = NULL;}
	}

//---------------------------------------------------------------------------
bool __fastcall TResFluxo::ResFlowBarra(VTEqpto  *eqpto,
                                        int      ind_pat,
                                        smcFLUXO *smc_fluxo)
   {
   //variáveis locais
   double  precisao = 1.e-6;  // Corrente em ampère - em estudo (HPS, 2012.05.25)
	VTBarra *barra;
   VTNo    *no;

   //determina No e sua Barra pai
   if (eqpto->Tipo()          != eqptoNO) return(false);
   if ((no    = (VTNo*)eqpto) == NULL)    return(false);
   if ((barra = no->pai)      == NULL)    return(false);
   //preenche resultados de potência da Barra
	barra->resflow->Ia_amp[ind_pat] = RoundZero(smc_fluxo->ifase_amp[0], precisao);
   barra->resflow->Ib_amp[ind_pat] = RoundZero(smc_fluxo->ifase_amp[1], precisao);
   barra->resflow->Ic_amp[ind_pat] = RoundZero(smc_fluxo->ifase_amp[2], precisao);
   barra->resflow->In_amp[ind_pat] = RoundZero(smc_fluxo->ifase_amp[3], precisao);
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TResFluxo::ResFlowBarra(VTEqpto   *eqpto,
                                        int       ind_pat,
                                        smcTENSAO *smc_tensao)
   {
   //variáveis locais
   VTBarra *barra;
   VTNo    *no;
   int     fases_tag = 0;
   double  precisao  = 1.e-8;  // Tensão em pu - em estudo (HPS, 2012.05.25)

   //determina No e sua Barra pai
   if (eqpto->Tipo()          != eqptoNO) return(false);
   if ((no    = (VTNo*)eqpto) == NULL)    return(false);
   if ((barra = no->pai)      == NULL)    return(false);
   //monta informação de fases disponíveis
   if (smc_tensao->existe_fase[indFASE_A]) fases_tag  = faseA;
   if (smc_tensao->existe_fase[indFASE_B]) fases_tag |= faseB;
   if (smc_tensao->existe_fase[indFASE_C]) fases_tag |= faseC;
   if (smc_tensao->existe_fase[indNEUTRO]) fases_tag |= faseN;
   //atualiza fases da Barra
   barra->resflow->Fases = fases_tag;
   //atualiza tensões por fase, zerando valores muito pequenos
   for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++)
      {
      //barra->resflow->Vfn_pu[ind_pat][ind_fase] = smc_tensao->v_pu[ind_fase];
      //linha alterada por bug do compilador
      //barra->resflow->Vfn_pu[ind_pat][ind_fase] = RoundZero(smc_tensao->v_pu[ind_fase], precisao);
      smc_tensao->v_pu[ind_fase] = RoundZero(smc_tensao->v_pu[ind_fase], precisao);
      barra->resflow->Vfn_pu[ind_pat][ind_fase] = smc_tensao->v_pu[ind_fase];
      }
   if (no->lisBAR->Count > 1)
      {
      for (int nb = 0; nb < no->lisBAR->Count; nb++)
         {
         barra = (VTBarra*)no->lisBAR->Items[nb];
         //atualiza fases da Barra
         barra->resflow->Fases = fases_tag;
         //atualiza tensões por fase
         for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++)
            {//zera valores muito pequenos
            //barra->resflow->Vfn_pu[ind_pat][ind_fase] = smc_tensao->v_pu[ind_fase];
            barra->resflow->Vfn_pu[ind_pat][ind_fase] = smc_tensao->v_pu[ind_fase];
            }
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TResFluxo::ResFlowBateria(smcESTADO_BATERIA *estado_bateria)
   {
   // Variáveis locais
   int       estado, ind_pat, modo_operacao;
   VTBateria *bateria;

   // Transfere resultados
   bateria       = (VTBateria *) estado_bateria->eqpto;
   ind_pat       = estado_bateria->ind_pat;
   estado        = estado_bateria->estado;
   modo_operacao = estado_bateria->modo_operacao;
   if     (estado == estCARREGADA)       bateria->Estado[ind_pat]        = esCARREGADA;
   else if(estado == estDESCARREGADA)    bateria->Estado[ind_pat]        = esDESCARREGADA;
   else                                  bateria->Estado[ind_pat]        = esINTERMEDIARIO;
   if     (modo_operacao == mopCARGA)    bateria->Modo_operacao[ind_pat] = moCARGA;
   else if(modo_operacao == mopDESCARGA) bateria->Modo_operacao[ind_pat] = moDESCARGA;
   else                                  bateria->Modo_operacao[ind_pat] = moOCIOSA;
	bateria->Earmaz_atual_kwh[ind_pat] = estado_bateria->en_armaz_kwh;
	bateria->Pext_kw[ind_pat]          = estado_bateria->pext_kw;
	bateria->Qext_kvar[ind_pat]        = estado_bateria->qext_kvar;

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TResFluxo::ResFlowGeracao(VTEqpto *eqpto, int ind_pat, smcFLUXO *smc_fluxo)
   {
   //variáveis locais
   int             numVF;
   double          p_mw, q_mvar, p_total, q_total;
   double          fator;
   double          precisao = 1.e-9; // Potência em MVA - em estudo (HPS, 2012_05_25)
   complex<double> s_mva;
   VTNo            *no;
	VTGerador       *pger;
   VTSuprimento    *psup;

   //determina No
   if (eqpto->Tipo()          != eqptoNO) return(false);
   if ((no    = (VTNo*)eqpto) == NULL)    return(false);
   //verifica se há Gerador/Suprimento no Nó
   if (! no->ExisteGeracao()) return(false);
   //monta lisEQP c/ Geradores e Suprimentos do No
	lisEQP->Clear();
   no->LisSuprimento(lisEQP);
   no->LisGerador(lisEQP);
   //descarta Gerador/Suprimento inativo no patamar
   RetiraGeradorSuprimentoInativo(lisEQP, ind_pat);
   if (lisEQP->Count == 0) return(true);
   //verifica se foi passado valor total de geração
   if (smc_fluxo->total)
      {//divide o fluxo nas 3 fases e assume zero p/ o neutro
      s_mva = RoundZero((smc_fluxo->stotal_mva / 3.), precisao);
      for (int ind_fase = 0; ind_fase < MAX_FASE-1; ind_fase++)
         {
         smc_fluxo->sfase_mva[ind_fase] = s_mva;
         }
      smc_fluxo->sfase_mva[MAX_FASE-1] = CZero();
      }
   else
      {//totaliza o fluxo das fases
      smc_fluxo->stotal_mva = CZero();
      for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++)
         {
         s_mva                           = RoundZero(smc_fluxo->sfase_mva[ind_fase], precisao);
         smc_fluxo->sfase_mva[ind_fase]  = s_mva;
         smc_fluxo->stotal_mva          += s_mva;
         }
      }
   //trata todos Geradores/Suprimento do tipo PQ, retirando-os de lisEQP
   for (int n = lisEQP->Count - 1; n >= 0; n--)
      {
      eqpto = (VTEqpto*)lisEQP->Items[n];
      if (eqpto->Tipo() == eqptoGERADOR)
         {//verifica tipo do Gerador
         pger = (VTGerador*)eqpto;
         if (pger->tiposup != supPQ) continue;
         //resultado do flow é igual a P/Q definido p/ o Gerador
			pger->resflow->S_mva[ind_pat] = fornecimento->S_mva[pger][ind_pat];
         pger->resflow->TipoSup        = supPQ;
         //IMPORTANTE: desconta contibuição do Gerador do fluxo total
         smc_fluxo->stotal_mva -= pger->resflow->S_mva[ind_pat];
         for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++)
            {
            smc_fluxo->sfase_mva[ind_fase] -= pger->resflow->Sf_mva[ind_pat][ind_fase];
            }
         }
      else if (eqpto->Tipo() == eqptoSUPRIMENTO)
         {
         psup = (VTSuprimento*)eqpto;
         if (psup->tiposup != supPQ) continue;
         //resultado do flow é igual a P/Q definido p/ o Suprimento
         psup->resflow->S_mva[ind_pat] = fornecimento->S_mva[psup][ind_pat];
         psup->resflow->TipoSup        = supPQ;
         //IMPORTANTE: desconta contibuição do Suprimento do fluxo total
         smc_fluxo->stotal_mva -= psup->resflow->S_mva[ind_pat];
         for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++)
            {
            smc_fluxo->sfase_mva[ind_fase] -= psup->resflow->Sf_mva[ind_pat][ind_fase];
            }
         }
      //remove Eqpto da lista
      lisEQP->Delete(n);
      }
   //IMPORTANTE: a lista lisEQP não possui mais Gerador/Suprimento do tipo PQ
   //verifica se há Gerador/Suprimento em lisEQP
   if (lisEQP->Count == 0) return(true);
   //verifica se existe um único Gerador/Suprimento (caso mais comum)
   if (lisEQP->Count == 1)
      {
      eqpto = (VTEqpto*)lisEQP->First();
      if (eqpto->Tipo() == eqptoGERADOR)
         {
         pger = (VTGerador*)eqpto;
         //define tipo de suprimento
         pger->resflow->TipoSup = (no->Swing()) ? supVF : supPV;
         //resultado do flow é atribuído ao Gerador
         //pger->resflow->S_mva[ind_pat] = smc_fluxo->stotal_mva;
         for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++)
            {
            s_mva = RoundZero(smc_fluxo->sfase_mva[ind_fase], precisao);
            pger->resflow->Sf_mva[ind_pat][ind_fase] = s_mva;
            }
         }
      else if (eqpto->Tipo() == eqptoSUPRIMENTO)
         {
         psup = (VTSuprimento*)eqpto;
         //define tipo de suprimento
         psup->resflow->TipoSup = (no->Swing()) ? supVF : supPV;
         //resultado do flow é atribuído ao Suprimento
         //psup->resflow->S_mva[ind_pat] = smc_fluxo->stotal_mva;
         for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++)
            {
            s_mva = RoundZero(smc_fluxo->sfase_mva[ind_fase], precisao);
            psup->resflow->Sf_mva[ind_pat][ind_fase] = s_mva;
            }
         }
      return(true);
      }
   //IMPORTANTE: existe mais de um Gerador/Suprimento PV/VF
   numVF   = no->NumSupGerVF();
   // numPV   = no->NumSupGerPV();
   p_total = smc_fluxo->stotal_mva.real();
   q_total = smc_fluxo->stotal_mva.imag();
   //trata os Geradores/Suprimentos PV antes dos Geradores/Suprimentos VF
   for (int n = 0; n < lisEQP->Count; n++)
      {
      eqpto = (VTEqpto*)lisEQP->Items[n];
      if (eqpto->Tipo() == eqptoGERADOR)
         {//verifica tipo do Gerador
         pger = (VTGerador*)eqpto;
         if (pger->tiposup != supPV) continue;
         //define tipo de suprimento
         pger->resflow->TipoSup = (no->Swing()) ? supVF : supPV;
         try{//define pot.reativa proporcional à pot.ativa
            p_mw    = fornecimento->P_mw[pger][ind_pat];
            fator   = (! IsDoubleZero(p_total)) ? fabs(p_mw/p_total) : 1./lisEQP->Count;
            q_mvar  = fator * q_total;
            }catch(Exception &e)
               {
               p_mw = q_mvar = 0.;
               }
         pger->resflow->S_mva[ind_pat] = complex<double>(p_mw, q_mvar);
         //IMPORTANTE: desconta contibuição do Gerador do fluxo total
         smc_fluxo->stotal_mva -= pger->resflow->S_mva[ind_pat];
         for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++)
            {
            smc_fluxo->sfase_mva[ind_fase] -= pger->resflow->Sf_mva[ind_pat][ind_fase];
            }
         }
      else if (eqpto->Tipo() == eqptoSUPRIMENTO)
         {//verifica tipo do Suprimento
         psup = (VTSuprimento*)eqpto;
         if (psup->tiposup != supPV) continue;
         //define tipo de suprimento
         psup->resflow->TipoSup = (no->Swing()) ? supVF : supPV;
         try{//define pot.reativa proporcional à pot.ativa
            p_mw    = fornecimento->P_mw[psup][ind_pat];
            fator   = (! IsDoubleZero(p_total)) ? fabs(p_mw/p_total) : 1./lisEQP->Count;
            q_mvar  = fator * q_total;
            }catch(Exception &e)
               {
               p_mw = q_mvar = 0.;
               }
         psup->resflow->S_mva[ind_pat] = complex<double>(p_mw, q_mvar);
         //IMPORTANTE: desconta contibuição do Suprimento do fluxo total
         smc_fluxo->stotal_mva -= psup->resflow->S_mva[ind_pat];
         for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++)
            {
            smc_fluxo->sfase_mva[ind_fase] -= psup->resflow->Sf_mva[ind_pat][ind_fase];
            }
         }
      }
   //trata os Geradores/Suprimentos VF
   for (int n = 0; n < lisEQP->Count; n++)
      {
		eqpto = (VTEqpto*)lisEQP->Items[n];
      if (eqpto->Tipo() == eqptoGERADOR)
         {//verifica tipo do Gerador
         pger = (VTGerador*)eqpto;
         if (pger->tiposup != supVF) continue;
         //define tipo de suprimento
         pger->resflow->TipoSup = supVF;
         try{//distribui fluxo total restante igualmente entre os Geradores/Suprimento VF
            //pger->resflow->S_mva[ind_pat] = smc_fluxo->stotal_mva / double(numVF);
            for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++)
               {
               pger->resflow->Sf_mva[ind_pat][ind_fase] =
                     RoundZero((smc_fluxo->sfase_mva[ind_fase] / double(numVF)), precisao);
               }
            }catch(Exception &e)
               {
               pger->resflow->S_mva[ind_pat] = CZero();
               }
         }
      else if (eqpto->Tipo() == eqptoSUPRIMENTO)
         {//verifica tipo do Suprimento
         psup = (VTSuprimento*)eqpto;
         if (psup->tiposup != supVF) continue;
         //define tipo de suprimento
         psup->resflow->TipoSup = supVF;
         try{//distribui fluxo total restante igualmente entre os Geradores/Suprimento VF
            //psup->resflow->S_mva[ind_pat] = smc_fluxo->stotal_mva / double(numVF);
            for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++)
               {
               psup->resflow->Sf_mva[ind_pat][ind_fase] =
                     RoundZero((smc_fluxo->sfase_mva[ind_fase] / double(numVF)), precisao);
               }
            }catch(Exception &e)
               {
               psup->resflow->S_mva[ind_pat] = CZero();
               }
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TResFluxo::ResFlowLigacao(VTEqpto *eqpto, int ind_bar, int ind_pat, smcFLUXO *smc_fluxo)
   {
   //variáveis locais
   double          precisao = 1.e-6; // Corrente em ampère - em estudo (HPS, 2012_05_25)
   complex<double> iamp;
   VTLigacao       *ligacao;

   //proteção: verifica tipo do Eqpto
   if (! eqpto->TipoLigacao()) return(false);
   //preenche resultados de fluxo da Ligacao
   ligacao = (VTLigacao*)eqpto;
   //salva as correntes
   for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++)
      {
      iamp = smc_fluxo->ifase_amp[ind_fase];
      ligacao->resflow->If_amp[ind_bar][ind_pat][ind_fase] = RoundZero(iamp, precisao);
      }
   //define sentido do fluxo (ATENÇÃO: barras ainda
   ligacao->resflow->SentidoP[ind_bar][ind_pat] = SentidoFluxo(ligacao->resflow->P_mw[ind_bar][ind_pat]);
   ligacao->resflow->SentidoQ[ind_bar][ind_pat] = SentidoFluxo(ligacao->resflow->Q_mvar[ind_bar][ind_pat]);
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TResFluxo::ResFlowRegulador(VTEqpto *eqpto, int ind_pat, smcTRAFO2E *smc_trafo2E)
   {
   //variáveis locais
   VTRegulador *regulador;

   //JCG 2018.04.03
   //proteção: verifica tipo do Eqpto
   if (eqpto->Tipo() != eqptoREGULADOR) return(true); // HPS, 2018.05.06 // return(false);
   //salva o passo calculado para cada fase do regulador
   regulador = (VTRegulador*)eqpto;
   for (int ind_fase = 0; ind_fase < MAX_FASE-1; ind_fase++)
      {
      regulador->passo_calc[ind_pat][ind_fase] = smc_trafo2E->ltc.passo_calc[ind_fase];
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TResFluxo::ResPerdaEnergiaGlobal(smcPERDA_ENERGIA_GLOBAL *smc_perda)
   {
   // Todos os fluxos chamam esta função após completar o cálculo de
   // todos os patamares

   // Uso:
   complex<double> energia_perdida_diaria_mvah = smc_perda->perda_energia_diaria_mvah;

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TResFluxo::ResPerdaEnergiaLigacao(smcPERDA_ENERGIA_LIGACAO *smc_perda)
   {
	//variáveis locais
   VTEqpto   *eqpto;
   VTLigacao *ligacao;

   // Verifica se o eqpto é uma Ligacao
   eqpto = smc_perda->eqpto;
   if (eqpto->TipoLigacao())
      {
      ligacao = (VTLigacao *) eqpto;
      //salva perda em energia na ligação e perda em energia no ferro (não apenas p/ Trafos)
      ligacao->resflow->PerdaDiaria_mwh      = smc_perda->perda_energia_diaria_kwh * 0.001;
      ligacao->resflow->PerdaDiariaFerro_mwh = smc_perda->perda_ferro_diaria_kwh   * 0.001;
      }
	return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TResFluxo::RetiraGeradorSuprimentoInativo(TList *lisGS, int ind_pat)
   {
   //variáveis locais
   VTEqpto      *eqpto;
   VTGerador    *gerador;
   VTSuprimento *suprimento;

   for (int n = lisGS->Count-1; n >= 0; n--)
      {
		eqpto = (VTEqpto*)lisGS->Items[n];
      if (! fornecimento->Ativo[eqpto][ind_pat])
         {//retira Eqpto da lista
         lisGS->Delete(n);
         //atualiza resultado do fluxo para o Gerador/Suprimento
         if      (eqpto->Tipo() == eqptoGERADOR)    ((VTGerador*)eqpto)->resflow->S_mva[ind_pat] = CZero();
         else if (eqpto->Tipo() == eqptoSUPRIMENTO) ((VTSuprimento*)eqpto)->resflow->S_mva[ind_pat] = CZero();
         }
      }
   }

//---------------------------------------------------------------------------
int __fastcall TResFluxo::SentidoFluxo(double s_mva)
   {
   //variáveis locais
   int sentido;

   //verifica valor em pu
	if (fabs(s_mva / Sbase_mva) < 1.e-8)
      {//fluxo nulo
      return(fluxoNENHUM);
      }
   //retorna sentido positivo ou negativo
   return((s_mva > 0) ? fluxoPOS : fluxoNEG);
   }
//---------------------------------------------------------------------------
//eof
