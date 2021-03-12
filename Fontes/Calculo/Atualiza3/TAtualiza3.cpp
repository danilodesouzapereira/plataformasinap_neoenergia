//---------------------------------------------------------------------------
#include ".\TAtualiza3.h"
#include "..\Comum13\Constantes_Calculo.h"
#include "..\Comum13\TBateriaC.h"
#include "..\Comum13\TMedidor.h"
#include "..\Comum13\TTempoCPU.h"
#include "..\Comum13\VTGeralC.h"
#include "..\Rede3\TBarra.h"
#include "..\Rede3\TCarga.h"
#include "..\Rede3\TCNL.h"
#include "..\Rede3\TFase.h"
#include "..\Rede3\TSup.h"
#include "..\Rede3\VTRede3.h"
#include "..\VTData.h"
#include "..\..\Apl\VTApl.h"
#include "..\..\Complexo\Complexo.h"
#include "..\..\Matriz\VTMatriz.h"
#include "..\..\Patamar\VTPatamar.h"
#include "..\..\Patamar\VTPatamares.h"
#include "..\..\Rede\VTEqpto.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTAtualiza3 * __fastcall NewObjAtualiza3(VTRede3  *rede3,
                                         VTGeralC *geralC)
   {
   return(new TAtualiza3(rede3, geralC));
   }

//---------------------------------------------------------------------------
__fastcall TAtualiza3::TAtualiza3(VTRede3  *rede3,
                                  VTGeralC *geralC)
   {
   // Salva ponteiros para objetos externos
   this->rede3  = rede3;
   this->geralC = geralC;
   this->data   = geralC->data;

   // Define valor de constantes
   czero         = complex<double>(0., 0.);
   grau_rad      = M_PI / 180.;
   raiz3         = sqrt(3.);
   raiz3_inverso = 1. / raiz3;
   }

//---------------------------------------------------------------------------
__fastcall TAtualiza3::~TAtualiza3(void)
   {
   }

//---------------------------------------------------------------------------
bool __fastcall TAtualiza3::AtualizaBateria(int np)
{
// Vari�veis locais
int             lig, nmo;
double          duracao_h, p_kw, q_kvar;
complex<double> s_mva;
TBarra          *bar1;
TBateriaC       *batc1;
VTPatamar       *patamar;
VTPatamares     *patamares;

// Retorna se o �ndice do patamar for inv�lido
if(np < 0) return(true);

// Inicializa��o
patamares = (VTPatamares *) geralC->apl->GetObject(__classid(VTPatamares));
patamar   = patamares->Patamar[np];
duracao_h = patamar->Duracao;

// Loop para todas as Baterias
for(int nbt=0; nbt < rede3->lisBATERIA->Count; nbt++)
   {
   batc1 = (TBateriaC *) rede3->lisBATERIA->Items[nbt];
   bar1  = batc1->barra;
   lig   = batc1->ligacao;

   // Atualiza bateria para o patamar atual
   if(! batc1->Atualiza(np, duracao_h, data)) return(false);

   // Obt�m pot�ncias absorvidas e modelo da carga, em fun��o
   // do modo de opera��o da bateria
   switch(batc1->modo_operacao)
      {
      case mopCARGA:
         nmo    = batc1->modelo_carga;
         p_kw   = fabs(batc1->pext_kw);
         q_kvar = fabs(batc1->qext_kvar);
         break;

      case mopDESCARGA:
         nmo    = batc1->modelo_desc;
         p_kw   = - fabs(batc1->pext_kw);    // Gerador: carga negativa
         q_kvar = - fabs(batc1->qext_kvar);  // Gerador: carga negativa
         break;

      default:  // mopOCIOSA
         nmo    = Zcte;
         p_kw   = batc1->p_ociosa_pu * batc1->pnom_kw;
         q_kvar = batc1->q_ociosa_pu * batc1->pnom_kw;
         break;
      }

   // Adiciona contribui��o da bateria
   s_mva = 0.001 * complex<double>(p_kw, q_kvar);
   bar1->mat_carga_mva->AddC2[lig][nmo] = s_mva;
   }

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TAtualiza3::AtualizaCapacitorReator(int np)
   {
   // Vari�veis locais
   int          count, lig_carga, lig_est;
   double       valor;
   TBarra       *bar1;
   smcCAPACITOR capacitor;
   smcDEMANDA   demanda;
   smcREATOR    reator;

   // Define dimens�o
   demanda.capacity = MAX_FASE_DEM;

   // Inicializa��o - zera pot�ncias reativas de est�tico em todas as barras
   for(int nb=0; nb < rede3->lisBAR->Count; nb++)
      {
      bar1 = (TBarra *) rede3->lisBAR->Items[nb];
      bar1->ZeraCapacitorReator();
      }

   // Retorna se o �ndice do patamar for inv�lido
   if(np < 0) return(true);

   // Loop para todos os Capacitores
   count = data->NumeroCapacitor;
   for(int n = 0; n < count; n++)
      {
      // Obt�m demanda do capacitor no patamar atual
      if(! data->EqptoCapacitor(n, &capacitor))          return(false);
      if(! data->Demanda(capacitor.eqpto, np, &demanda)) return(false);
      bar1 = (TBarra *) capacitor.barraCALC;

      // Totaliza demanda de capacitor na barra
      for(int k=0; k < demanda.count; k++)
         {
         // Determina liga��o do capacitor, traduzindo c�digo
         lig_carga = rede3->ConverteFasesCarga(demanda.dem[k].fases_int, bar1);
         lig_est   = rede3->ConverteLigacaoCargaParaEstatico(lig_carga);
         if(lig_est < 0) continue;  // Erro
         valor                         = (demanda.dem[k].s_mva[Zcte]).imag();
         bar1->vet_qest_mvar[lig_est] += valor; // Capacitor: vet_qest_mvar[] > 0
         }
      }

   // Loop para todos os Reatores
   count = data->NumeroReator;
   for(int n = 0; n < count; n++)
      {
      // Obt�m demanda do reator no patamar atual
      if(! data->EqptoReator(n, &reator))             return(false);
      if(! data->Demanda(reator.eqpto, np, &demanda)) return(false);
      bar1 = (TBarra *) reator.barraCALC;

      // Totaliza demanda de reator na barra
      for(int k=0; k < demanda.count; k++)
         {
         // Determina liga��o do reator, traduzindo c�digos
         lig_carga = rede3->ConverteFasesCarga(demanda.dem[k].fases_int, bar1);
         lig_est  = rede3->ConverteLigacaoCargaParaEstatico(lig_carga);
         if(lig_est < 0) continue;  // Erro
         valor                         = (demanda.dem[k].s_mva[Zcte]).imag();
         bar1->vet_qest_mvar[lig_est] -= valor; // Reator: vet_qest_mvar[] < 0
         }
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TAtualiza3::AtualizaCarga(int np)
   {
   // Vari�veis locais
   int             lig_carga, mc1, modelo_carga;
   complex<double> cvalor, soma;
   TBarra          *bar1;
   TFase           *fases_sup;
   TSup            *sup1;
   smcCARGA        carga;
   smcDEMANDA      demanda;

   // Define dimens�o
   demanda.capacity = MAX_FASE_DEM;

   // Zera carga de todas as barras
   for(int nb=0; nb < rede3->lisBAR->Count; nb++)
      {
      bar1 = (TBarra *) rede3->lisBAR->Items[nb];
      bar1->ZeraCarga();
      }

   // Retorna se o �ndice do patamar for inv�lido
   if(np < 0) return(true);

   // Obt�m modelo de carga imposto (caso geral: mcNaoImposto)
   modelo_carga = geralC->ModeloCargaImposto;

   // Loop para todas as Cargas
   for(int nc=0; nc < rede3->lisCARGA->Count; nc++)
      {
      // Obt�m demanda da carga no patamar solicitado
      if(! data->EqptoCarga(nc, &carga))             return(false);
      if(! data->Demanda(carga.eqpto, np, &demanda)) return(false);
      bar1 = (TBarra *) carga.barraCALC;

      // Totaliza demanda de carga na barra
      for(int k=0; k < demanda.count; k++)
         {
         // Determina liga��o da carga, traduzindo c�digos
         lig_carga = rede3->ConverteFasesCarga(demanda.dem[k].fases_int, bar1);
         if(lig_carga < 0) continue;  // Erro

		 // Acumula pot�ncia para cada modelo de carga
		 for(int nmo=0; nmo < NUM_MODELO_CARGA; nmo++)
			{
			cvalor = demanda.dem[k].s_mva[nmo];
			if(modelo_carga == mcNaoImposto) mc1 = nmo;
			else                             mc1 = modelo_carga;
			bar1->mat_carga_mva->AddC2[lig_carga][mc1] = cvalor;
			}
         } // for(k)
      }  // for(nc)

   // Atualiza carga de barras com suprimento do tipo PQ. Imp�e modelo Scte.
   for(int ns=0; ns < rede3->lisSUP->Count; ns++)
      {
      sup1 = (TSup *) rede3->lisSUP->Items[ns];
      if(sup1->bar_int->tipo != BAR_PQ) continue; // Suprimento irrelevante
      fases_sup = sup1->fases;
      bar1      = sup1->bar_int;                  // Aloca carga na barra interna
      soma      = czero;
      for(int nfa=0; nfa < fases_sup->NumFases(); nfa++)
         {
         soma += sup1->vet_sesp_mva[nfa];
         }
      lig_carga = rede3->ConverteFasesCarga(fases_sup->FasesAsInt(), bar1);
      if(lig_carga < 0) continue;  // Erro
      bar1->mat_carga_mva->AddC2[lig_carga][Scte] = -soma; // Scarga -= Sger; 100% Scte
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TAtualiza3::AtualizaCNLsFundamental(bool flag_troca_sinal)
   {
   // Vari�veis locais
   int    indfreq0;
   double fiA, fiB, fiC, sA_mva, sB_mva, sC_mva;
   // double fi;
   TBarra *bar1;
   TCNL   *cnl;

   // Loop para todas as CNLs
   for(int nc=0; nc < rede3->lisCNL->Count; nc++)
      {
      cnl      = (TCNL *) rede3->lisCNL->Items[nc];
      bar1     = cnl->barra;
      indfreq0 = cnl->IndiceHarmonica(1);
      if(indfreq0 < 0) return(false);  // Erro

      // Obt�m correntes da CNL na frequ�ncia fundamental
      sA_mva = 0.001 * cnl->mat_corrente->D2[indfreq0][0] * bar1->VnomFase_kv(faseA);
      sB_mva = 0.001 * cnl->mat_corrente->D2[indfreq0][1] * bar1->VnomFase_kv(faseB);
      sC_mva = 0.001 * cnl->mat_corrente->D2[indfreq0][2] * bar1->VnomFase_kv(faseC);

      // Altera��o sugerida por Fred - 2012_08_16: [fi = 0 - ang_c1]
      // fi     = acos(fabs(cnl->cos_fi));
      // if(cnl->cos_fi < 0.) fi  = -fi;  // Carga capacitiva
      fiA = cnl->mat_ang_rad->D2[indfreq0][0]; // �ngulo de POT�NCIA (phi) na fundamental (rad)
      fiB = cnl->mat_ang_rad->D2[indfreq0][1];
      fiC = cnl->mat_ang_rad->D2[indfreq0][2];
      if(flag_troca_sinal)  // Troca sinal da pot�ncia complexa
         {
         fiA += M_PI;
         fiB += M_PI;
         fiC += M_PI;
         }

      // Atualiza carga - considera modelo Icte
      // IMPORTANTE: se a barra n�o possuir neutro, liga cada fase da CNL �
      //             refer�ncia (terra)
      if(rede3->VerificaFasesCarga(bar1, cgAN))
         {
         bar1->mat_carga_mva->AddC2[cgAN][Icte] = polar(sA_mva, fiA);
         }
      else if(rede3->VerificaFasesCarga(bar1, cgAT))
         {
         bar1->mat_carga_mva->AddC2[cgAT][Icte] = polar(sA_mva, fiA);
         }

      if(rede3->VerificaFasesCarga(bar1, cgBN))
         {
         bar1->mat_carga_mva->AddC2[cgBN][Icte] = polar(sB_mva, fiB);
         }
      else if(rede3->VerificaFasesCarga(bar1, cgBT))
         {
         bar1->mat_carga_mva->AddC2[cgBT][Icte] = polar(sB_mva, fiB);
         }

      if(rede3->VerificaFasesCarga(bar1, cgCN))
         {
         bar1->mat_carga_mva->AddC2[cgCN][Icte] = polar(sC_mva, fiC);
         }
      else if(rede3->VerificaFasesCarga(bar1, cgCT))
         {
         bar1->mat_carga_mva->AddC2[cgCT][Icte] = polar(sC_mva, fiC);
         }
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TAtualiza3::AtualizaEstimador3(int np)
   {
   // Atualiza suprimentos para o patamar 'np'
   if(! AtualizaSuprimentos(np))     return(false);
   if(! AtualizaCarga(np))           return(false);
   if(! AtualizaBateria(np))         return(false);

// PEND�NCIA: CONVERTER CNLs PARA PU. CARGAS, CAPACITORES E REATORES J�
//            EST�O SENDO CONVERTIDOS A PU EM
//            TEstimador3::AtualizaMedidoresVirtuais()   [HPS, 2015.12.28]
   if(! AtualizaCNLsFundamental())   return(false);

   if(! AtualizaCapacitorReator(np)) return(false);

   // Comandos espec�ficos do Estimador
   if(! AtualizaMedidoresReais(np))  return(false);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TAtualiza3::AtualizaFluxoH3(int np)
   {
   if(! AtualizaSuprimentos(np))     return(false);
   if(! AtualizaCarga(np))           return(false);
   if(! AtualizaBateria(np))         return(false);
   if(! AtualizaCNLsFundamental())   return(false);
   if(! AtualizaCapacitorReator(np)) return(false);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TAtualiza3::AtualizaGauss3(int np)
   {
   // Vari�veis locais
   clock_t inicio;

   inicio = clock();
   if(! AtualizaSuprimentos(np))     return(false);
   if(! AtualizaCarga(np))           return(false);
   if(! AtualizaBateria(np))         return(false);
   if(! AtualizaCNLsFundamental())   return(false);
   if(! AtualizaCapacitorReator(np)) return(false);
   geralC->tCPU->Acumula(ttATUALIZA, inicio);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TAtualiza3::AtualizaMedidoresReais(int np)
   {
   // Vari�veis locais
   double     ibase, ibase_const, Sbase_fase_inv;
   TMedidor   *med1;
   smcMEDICAO medicao;

   // Inicializa��o
   ibase_const    = 1000. * geralC->Sbase_fase;
   Sbase_fase_inv = 1. / geralC->Sbase_fase;

   // Atualiza medidores reais
   for(int nm=0; nm < geralC->num_med_reais; nm++)
      {
      med1             = (TMedidor *) rede3->lisMED->Items[nm];
      med1->val_med_pu = 0.;
      if(! med1->enabled) continue;  // Descarta medidor desabilitado
      medicao.canal = med1->snp_canal;
      if(! data->Medicao(np, medicao)) return(false);
      med1->val_med_pu = medicao.valor;

      // Acerto de unidade em fun��o da grandeza medida
      if(med1->TipoMedicaoCorrente())
         {
         ibase             = ibase_const / med1->bar_ref->VnomFase_kv(med1->fase_int); // [A]
         med1->val_med_pu /= ibase;           // [A] -> [pu]
         }
      else if(med1->TipoMedicaoPotencia())
         {
         med1->val_med_pu *= Sbase_fase_inv;  // [MW] de uma fase -> [pu], etc.
         }
      } // for(nm)

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TAtualiza3::AtualizaNR3(int np)
   {
   // Vari�veis locais
   clock_t inicio;

   inicio = clock();
   if(! AtualizaSuprimentos(np))     return(false);
   if(! AtualizaCarga(np))           return(false);
   if(! AtualizaBateria(np))         return(false);
   if(! AtualizaCNLsFundamental())   return(false);
   if(! AtualizaCapacitorReator(np)) return(false);
   geralC->tCPU->Acumula(ttATUALIZA, inicio);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TAtualiza3::AtualizaSuprimentos(int np)
   {
   // ATEN��O: o Sinap permite v�rios suprimentos por barra, mas o C�lculo
   //          permite somente 1 suprimento por barra. Isto obriga a examinar
   //          todos os suprimentos do Sinap na hora de fazer a atualiza��o
   //          do suprimento (�nico) do C�lculo em um determinado patamar.

   // Vari�veis locais
   bool            zona_unica;
   int             count, fase_int, ind, num_fases;
   double          ang1, mod1, p_mw;
   complex<double> v1;
   TBarra          *bar_ext, *bar_int;
   TFase           *fases_sup;
   TSup            *sup1;
   smcGERADOR      gerador;
   smcSUPRIMENTO   suprimento;
   smcGERACAO      geracao;

   // Inicializa��o
   zona_unica       = false;  // Gauss3
   geracao.capacity = MAX_FASE;

   // Zera pot�ncia de suprimentos (para contabilizar corretamente v�rios
   // suprimentos ligados na mesma barra)
   for(int ns=0; ns < rede3->lisSUP->Count; ns++)
      {
      sup1 = (TSup *) rede3->lisSUP->Items[ns];
      sup1->ZeraPotencia();
      }

   // Loop para todos os Suprimentos
   count = data->NumeroSuprimento;
   for(int n=0; n < count; n++)
      {
      if(! data->EqptoSuprimento(n, &suprimento)) return(false);
      if(! data->Geracao(suprimento.eqpto, np, zona_unica, &geracao)) return(false);
      if(! geracao.ativa) continue;  // Descarta suprimento inativo

      // Localiza suprimento na lista local
      bar_ext = (TBarra *) suprimento.barraCALC;
      sup1    = rede3->LocalizaSuprimentoBarraExt(bar_ext);
      if(sup1 == NULL) return(false);
      bar_int   = sup1->bar_int;
      fases_sup = sup1->fases;
      num_fases = fases_sup->NumFases();
      // Atualiza suprimento em fun��o do seu tipo
      switch(bar_int->tipo)
         {
         case BAR_SW:  // Barra swing
            for(int nfa=0; nfa < num_fases; nfa++)
               {
               fase_int = fases_sup->Fase(nfa);
               ind      = fases_sup->IndiceABCN(nfa);
               mod1     = geracao.ger[ind].v_pu;
               if((fase_int != faseN) && (fabs(mod1) < ZEROV)) mod1 = 1.; // Prote��o
               ang1                     = geracao.ger[ind].t_rad;
               sup1->vet_tensao_pu[nfa] = polar(mod1, ang1);
               }
            break;

         case BAR_PV:  // Barra PV
            for(int nfa=0; nfa < num_fases; nfa++)
               {
               fase_int = fases_sup->Fase(nfa);
               ind      = fases_sup->IndiceABCN(nfa);
               mod1     = geracao.ger[ind].v_pu;
               if((fase_int != faseN) && (fabs(mod1) < ZEROV)) mod1 = 1.; // Prote��o
               ang1 = Arg(sup1->vet_tensao_pu[nfa]); // Preserva valor anterior do �ngulo
               sup1->vet_tensao_pu[nfa]  = polar(mod1, ang1);
               sup1->vet_sesp_mva[nfa]  += complex<double>(geracao.ger[ind].p_mw, 0.);
               }
            break;

         case BAR_PQ:  // Barra PQ
            for(int nfa=0; nfa < num_fases; nfa++)
               {
               fase_int                 = fases_sup->Fase(nfa);
               ind                      = fases_sup->IndiceABCN(nfa);
               sup1->vet_sesp_mva[nfa] += complex<double>(geracao.ger[ind].p_mw,
                                                          geracao.ger[ind].q_mvar);
               // Inicializa tens�o, se isso n�o foi feito antes
               mod1 = Abs(sup1->vet_tensao_pu[nfa]);
               ang1 = geracao.ger[ind].t_rad;
               if((fase_int != faseN) && (mod1 < ZEROV)) mod1 = 1.; // Prote��o
               sup1->vet_tensao_pu[nfa] = polar(mod1, ang1);
               }
            break;

         default:  // Erro
            return(false);
         }
      }

   // Loop para todos Geradores
   count = data->NumeroGerador;
   for(int n=0; n < count; n++)
      {
      if(! data->EqptoGerador(n, &gerador)) return(false);
      if(! data->Geracao(gerador.eqpto, np, zona_unica, &geracao)) return(false);
      if(! geracao.ativa) continue;  // Descarta gerador inativo

      // Localiza gerador na lista local
      bar_ext = (TBarra *) gerador.barraCALC;
      sup1    = rede3->LocalizaSuprimentoBarraExt(bar_ext);
      if(sup1 == NULL) return(false);
      bar_int   = sup1->bar_int;
      fases_sup = sup1->fases;
      num_fases = fases_sup->NumFases();

      // Atualiza gerador em fun��o do seu tipo
      switch(bar_int->tipo)
         {
         case BAR_SW:  // Barra swing
            for(int nfa=0; nfa < num_fases; nfa++)
               {
               fase_int = fases_sup->Fase(nfa);
               ind      = fases_sup->IndiceABCN(nfa);
               mod1     = geracao.ger[ind].v_pu;
               if((fase_int != faseN) && (fabs(mod1) < ZEROV)) mod1 = 1.; // Prote��o
               ang1                     = geracao.ger[ind].t_rad;
               sup1->vet_tensao_pu[nfa] = polar(mod1, ang1);
               }
            break;

         case BAR_PV:  // Barra PV
            for(int nfa=0; nfa < num_fases; nfa++)
               {
               fase_int = fases_sup->Fase(nfa);
               ind      = fases_sup->IndiceABCN(nfa);
               mod1     = geracao.ger[ind].v_pu;
               if((fase_int != faseN) && (fabs(mod1) < ZEROV)) mod1 = 1.; // Prote��o
               ang1  = Arg(sup1->vet_tensao_pu[nfa]); // Preserva valor anterior do �ngulo
               sup1->vet_tensao_pu[nfa]  = polar(mod1, ang1);
               p_mw                      = geracao.ger[ind].p_mw;


// DEBUG GERADORES BCN, CAN, BN E CN - IN�CIO
// O Editor n�o permite definir as configura��es abaixo
// Caso GeradorBCN+PV:
// if(fase_int == faseC) p_mw = sup1->vet_sesp_mva[0].real();
// Caso GeradorCAN+PV:
// if(fase_int == faseA) sup1->vet_sesp_mva[0] = complex<double>(p_mw, 0.);
// Caso GeradorBN+PV_TrafoBN+AB:
// if(fase_int == faseB) {sup1->vet_tensao_pu[nfa] = polar(1.05, ang1); p_mw = 0.05;}
// Caso GeradorCN+PV_TrafoCN+AB:
// if(fase_int == faseC) {sup1->vet_tensao_pu[nfa] = polar(1.05, ang1); p_mw = 0.05;}
// DEBUG GERADORES BCN, CAN, BN E CN - FIM


               sup1->vet_sesp_mva[nfa]  += complex<double>(p_mw, 0.);
               }
            break;

         case BAR_PQ:  // Barra PQ
            for(int nfa=0; nfa < num_fases; nfa++)
               {
               fase_int                 = fases_sup->Fase(nfa);
               ind                      = fases_sup->IndiceABCN(nfa);
               sup1->vet_sesp_mva[nfa] += complex<double>(geracao.ger[ind].p_mw,
                                                          geracao.ger[ind].q_mvar);
               // Inicializa tens�o nas fases, se isso n�o foi feito antes
               mod1 = Abs(sup1->vet_tensao_pu[nfa]);
               ang1 = geracao.ger[ind].t_rad;
               if((fase_int != faseN) && (mod1 < ZEROV)) mod1 = 1.; // Prote��o
               sup1->vet_tensao_pu[nfa] = polar(mod1, ang1);
               }
            break;

         default:  // Erro
            return(false);
         }
      }

   return(true);
   }

//---------------------------------------------------------------------------

