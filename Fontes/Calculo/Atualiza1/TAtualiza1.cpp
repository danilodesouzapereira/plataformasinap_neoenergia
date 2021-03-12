//---------------------------------------------------------------------------
#include ".\TAtualiza1.h"
#include "..\Calculo1\VTCalculo1.h"
#include "..\Comum13\Constantes_Calculo.h"
#include "..\Comum13\Estruturas.h"
#include "..\Comum13\TBateriaC.h"
#include "..\Comum13\TMedidor.h"
#include "..\Comum13\TTempoCPU.h"
#include "..\Comum13\VTGeralC.h"
#include "..\Rede1\TBarra.h"
#include "..\Rede1\TCarga.h"
#include "..\Rede1\TCNL.h"
#include "..\Rede1\TSup.h"
#include "..\Rede1\VTRede1.h"
#include "..\VTData.h"
#include "..\..\Apl\VTApl.h"
#include "..\..\Complexo\Complexo.h"
#include "..\..\Patamar\VTPatamar.h"
#include "..\..\Patamar\VTPatamares.h"
#include "..\..\Rede\VTEqpto.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTAtualiza1 * __fastcall NewObjAtualiza1(VTRede1  *rede1,
                                         VTGeralC *geralC)
   {
   return(new TAtualiza1(rede1, geralC));
   }

//---------------------------------------------------------------------------
__fastcall TAtualiza1::TAtualiza1(VTRede1  *rede1,
                                  VTGeralC *geralC)
   {
   // Salva ponteiros para objetos externos
   this->rede1  = rede1;
   this->geralC = geralC;
   this->data   = geralC->data;

   // Define valor de constantes
   grau_rad      = M_PI / 180.;
   raiz3         = sqrt(3.);
   raiz3_inverso = 1. / raiz3;
   cum           = complex<double>(1., 0.);
   czero         = complex<double>(0., 0.);
   }

//---------------------------------------------------------------------------
__fastcall TAtualiza1::~TAtualiza1(void)
   {
   }

//---------------------------------------------------------------------------
bool __fastcall TAtualiza1::AtualizaBateria(int np)
{
// Variáveis locais
int             nmo;
double          sbinv = 1. / geralC->Sbase;
double          duracao_h, p_kw, q_kvar;
complex<double> s_pu;
TBarra          *bar1;
TBateriaC       *batc1;
VTPatamar       *patamar;
VTPatamares     *patamares;

// Retorna se o índice do patamar for inválido
if(np < 0) return(true);

// Inicialização
patamares = (VTPatamares *) geralC->apl->GetObject(__classid(VTPatamares));
patamar   = patamares->Patamar[np];
duracao_h = patamar->Duracao;

// Loop para todas as Baterias
for(int nbt=0; nbt < rede1->lisBATERIA->Count; nbt++)
   {
   batc1 = (TBateriaC *) rede1->lisBATERIA->Items[nbt];
   bar1  = batc1->barra;

   // Atualiza bateria para o patamar atual
   if(! batc1->Atualiza(np, duracao_h, data)) return(false);

   // Obtém potências absorvidas e modelo da carga, em função
   // do modo de operação da bateria
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

   // Adiciona contribuição da bateria
   s_pu                     = 0.001 * complex<double>(p_kw, q_kvar) * sbinv;
   bar1->vet_carga_pu[nmo] += s_pu;
   }

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TAtualiza1::AtualizaCapacitorReator(int seq,
                                                    int np)
   {
   // Variáveis locais
   int          count;
   double       sbinv = 1. / geralC->Sbase;
   double       soma;
   TBarra       *bar1;
   smcCAPACITOR capacitor;
   smcDEMANDA   demanda;
   smcREATOR    reator;

   // Define dimensão
   demanda.capacity = MAX_FASE_DEM;

   // Zera susceptância de todas as barras
   for(int nb=0; nb < rede1->lisBAR->Count; nb++)
      {
      bar1 = (TBarra *) rede1->lisBAR->Items[nb];
      bar1->ZeraCapacitorReator(seq);
      }

   // Retorna se o índice do patamar for inválido
   if(np < 0) return(true);

   // Loop para todos os Capacitores
   count = data->NumeroCapacitor;
   for (int n=0; n < count; n++)
      {
      if (! data->EqptoCapacitor(n, &capacitor)) return(false);
      if(seq == 0)
         {
         if(capacitor.ligacao != ligacaoESTRELA_ATERRADO) continue; // Ligação irrelevante
         }
      bar1 = (TBarra *) capacitor.barraCALC;

      // Obtém demanda do capacitor no patamar atual
      if(! data->Demanda(capacitor.eqpto, np, &demanda)) return(false);

      // Totaliza demanda de capacitor na barra
      soma = 0.;
      for(int k=0; k < demanda.count; k++)
         {
         soma += (demanda.dem[k].s_mva[Zcte]).imag();
         }

      // Atualiza estático da barra
      if(seq == 0) bar1->best0_pu += (soma * sbinv);
      else         bar1->best1_pu += (soma * sbinv);
      }

   // Loop para todos os Reatores
   count = data->NumeroReator;
   for (int n=0; n < count; n++)
      {
      if (! data->EqptoReator(n, &reator)) return(false);
      if(seq == 0)
         {
         if(reator.ligacao != ligacaoESTRELA_ATERRADO) continue; // Ligação irrelevante
         }
      bar1 = (TBarra *) reator.barraCALC;

      // Obtém demanda do reator no patamar atual
      if(! data->Demanda(reator.eqpto, np, &demanda)) return(false);

      // Totaliza demanda de reator na barra
      soma = 0.;
      for(int k=0; k < demanda.count; k++)
         {
         soma += (demanda.dem[k].s_mva[Zcte]).imag();
         }

      // Atualiza estático da barra
      if(seq == 0) bar1->best0_pu -= (soma * sbinv);
      else         bar1->best1_pu -= (soma * sbinv);
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TAtualiza1::AtualizaCarga(int np)
   {
   // Variáveis locais
   int             mc1, modelo_carga;
   double          sbinv = 1. / geralC->Sbase;
   complex<double> cvalor;
   TBarra          *bar1;
   TCarga          *cargaG1;
   TSup            *sup1;
   smcCARGA        carga;
   smcDEMANDA      demanda;

   // Define dimensão
   demanda.capacity = MAX_FASE_DEM;

   // Zera carga de todas as barras
   for(int nb=0; nb < rede1->lisBAR->Count; nb++)
      {
      bar1 = (TBarra *) rede1->lisBAR->Items[nb];
      bar1->ZeraCarga();
      }

   // Retorna se o índice do patamar for inválido
   if(np < 0) return(true);

   // Obtém modelo de carga imposto (caso geral: mcNaoImposto)
   modelo_carga = geralC->ModeloCargaImposto;

   // Loop para todas as Cargas
   for(int nc=0; nc < rede1->lisCARGA->Count; nc++)
      {
      cargaG1 = (TCarga *) rede1->lisCARGA->Items[nc];
      bar1    = cargaG1->barra;

      // Obtém demanda da carga no patamar solicitado
      if(! data->EqptoCarga(nc, &carga)) return(false);
      if(! data->Demanda(carga.eqpto, np, &demanda)) return(false);

      // Totaliza demanda de carga na barra
      for(int k=0; k < demanda.count; k++)
		 {
		 // Acumula potência para cada modelo de carga
         for(int nmo=0; nmo < NUM_MODELO_CARGA; nmo++)
			{
			cvalor = demanda.dem[k].s_mva[nmo];
			if(modelo_carga == mcNaoImposto) mc1 = nmo;
			else                             mc1 = modelo_carga;
			bar1->vet_carga_pu[mc1] += cvalor;
			}
         }
      }

   // Converte valores para pu
   for(int nb=0; nb < rede1->lisBAR->Count; nb++)
      {
      bar1 = (TBarra *) rede1->lisBAR->Items[nb];
      for(int nmo=0; nmo < NUM_MODELO_CARGA; nmo++)
         {
         bar1->vet_carga_pu[nmo] *= sbinv;
         }
      }

   // Atualiza carga de barras com suprimento do tipo PQ
   for(int ns=0; ns < rede1->lisSUP->Count; ns++)
      {
      sup1 = (TSup *) rede1->lisSUP->Items[ns];
      bar1 = sup1->barra;
      if(bar1->tipo != BAR_PQ) continue;          // Suprimento irrelevante
      bar1->vet_carga_pu[Scte] -= sup1->sesp_pu;  // Scarga = -Sger
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TAtualiza1::AtualizaCNLsFundamental(void)
   {
   // Variáveis locais
   double ib_inv, stot_pu;
   TBarra *bar1;
   TCNL   *cnl;

   // Loop para todas as CNLs
   for(int nc=0; nc < rede1->lisCNL->Count; nc++)
      {
      cnl  = (TCNL *) rede1->lisCNL->Items[nc];
      bar1 = cnl->barra;

      // Obtém corrente da CNL na frequência fundamental
      ib_inv  = 0.001 * bar1->vnom_kv * raiz3 / geralC->Sbase;   // 1/Ibase
      stot_pu = cnl->corrente_a * ib_inv;

      // Atualiza carga
      bar1->vet_carga_pu[Icte] += polar(stot_pu, cnl->phi_rad);  // Icte
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TAtualiza1::AtualizaCurto(int np)
   {
   // if(! AtualizaSuprimentos(np))        return(false);
   if(! AtualizaCarga(np))              return(false);
   if(! AtualizaBateria(np))            return(false);
   if(! AtualizaCNLsFundamental())      return(false);
   if(! AtualizaCapacitorReator(1, np)) return(false);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TAtualiza1::AtualizaEstabilidade(int np)
   {
   if(! AtualizaSuprimentos(np))        return(false);
   if(! AtualizaCarga(np))              return(false);
   if(! AtualizaBateria(np))            return(false);
   if(! AtualizaCNLsFundamental())      return(false);
   if(! AtualizaCapacitorReator(1, np)) return(false);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TAtualiza1::AtualizaEstimador1(int np)
   {
   // Variáveis locais
   clock_t inicio;

   inicio = clock();
   // Atualiza suprimentos para o patamar 'np'
   if(! AtualizaSuprimentos(np))        return(false);
   if(! AtualizaCarga(np))              return(false);
   if(! AtualizaBateria(np))            return(false);
   if(! AtualizaCNLsFundamental())      return(false);
   if(! AtualizaCapacitorReator(1, np)) return(false);

   // Comandos específicos do Estimador
   if(! AtualizaMedidoresReais(np))     return(false);

   geralC->tCPU->Acumula(ttATUALIZA, inicio);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TAtualiza1::AtualizaFluxoRadial1(int np)
   {
   if(! AtualizaSuprimentos(np))        return(false);
   if(! AtualizaCarga(np))              return(false);
   if(! AtualizaBateria(np))            return(false);
   if(! AtualizaCNLsFundamental())      return(false);
   if(! AtualizaCapacitorReator(1, np)) return(false);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TAtualiza1::AtualizaGauss1(int np)
   {
   // Variáveis locais
   clock_t inicio;

   inicio = clock();
   if(! AtualizaSuprimentos(np))        return(false);
   if(! AtualizaCarga(np))              return(false);
   if(! AtualizaBateria(np))            return(false);
   if(! AtualizaCNLsFundamental())      return(false);
   if(! AtualizaCapacitorReator(1, np)) return(false);
   geralC->tCPU->Acumula(ttATUALIZA, inicio);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TAtualiza1::AtualizaMedidoresReais(int np)
   {
   // Variáveis locais
   double     ibase, ibase_const, sbinv;
   TMedidor   *med1;
   smcMEDICAO medicao;

   // Inicialização
   sbinv       =  1. / geralC->Sbase;
   ibase_const = 1000. * geralC->Sbase * raiz3_inverso;

   // Atualiza medidores reais
   for(int nm=0; nm < geralC->num_med_reais; nm++)
      {
      med1             = (TMedidor *) rede1->lisMED->Items[nm];
      med1->val_med_pu = 0.;
      if(! med1->enabled) continue;  // Descarta medidor desabilitado
      medicao.canal = med1->snp_canal;
      if(! data->Medicao(np, medicao)) return(false);
      med1->val_med_pu = medicao.valor;

      // Acerto de unidade em função da grandeza medida
      if(med1->TipoMedicaoCorrente())
         {
         ibase             = ibase_const / med1->bar_ref->vnom_kv; // [A]
         med1->val_med_pu /= ibase;                                // [A] -> [pu]
         }
      else if(med1->TipoMedicaoPotencia())
         {
         med1->val_med_pu *= sbinv;  // [MW] -> [pu], etc.
         }
      } // for(nm)

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TAtualiza1::AtualizaNR1(int np)
   {
   // Variáveis locais
   clock_t inicio;

   inicio = clock();
   if(! AtualizaSuprimentos(np))        return(false);
   if(! AtualizaCarga(np))              return(false);
   if(! AtualizaBateria(np))            return(false);
   if(! AtualizaCNLsFundamental())      return(false);
   if(! AtualizaCapacitorReator(1, np)) return(false);
   geralC->tCPU->Acumula(ttATUALIZA, inicio);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TAtualiza1::AtualizaPartidaMotor(int np)
   {
   if(! AtualizaSuprimentos(np))        return(false);
   if(! AtualizaCarga(np))              return(false);
   if(! AtualizaBateria(np))            return(false);
   if(! AtualizaCNLsFundamental())      return(false);
   if(! AtualizaCapacitorReator(1, np)) return(false);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TAtualiza1::AtualizaSuprimentos(int np)
   {
   // ATENÇÃO: o Sinap permite vários suprimentos por barra, mas o Cálculo
   //          permite somente 1 suprimento por barra. Isto obriga a examinar
   //          todos os suprimentos do Sinap na hora de fazer a atualização
   //          do suprimento (único) do Cálculo em um determinado patamar.

   // Variáveis locais
   bool          zona_unica;
   int           count;
   double        mod1, ang1, p1, q1, sbinv;
   TBarra        *bar1;
   TSup          *sup1;
   smcGERADOR    gerador;
   smcSUPRIMENTO suprimento;
   smcGERACAO    geracao;

   // Inicialização
   zona_unica       = true;  // Gauss1
   geracao.capacity = MAX_FASE;
   sbinv            = 1. / geralC->Sbase;

   // Zera potência de suprimentos (para contabilizar corretamente vários
   // suprimentos ligados na mesma barra)
   for(int ns=0; ns < rede1->lisSUP->Count; ns++)
      {
      sup1 = (TSup *) rede1->lisSUP->Items[ns];
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
      bar1 = (TBarra *) suprimento.barraCALC;
      sup1 = rede1->LocalizaSuprimentoBarra(bar1);
      if(sup1 == NULL) return(false);

      // Atualiza suprimento em função do seu tipo
      switch(bar1->tipo)
         {
         case BAR_SW:  // Barra swing
            mod1 = geracao.ger[0].v_pu;
            ang1 = geracao.ger[0].t_rad;
            if(fabs(mod1) < ZEROV) mod1 = 1.; // Proteção
            sup1->tensao_pu = polar(mod1, ang1);
            break;

         case BAR_PV:  // Suprimento PV
            mod1 = geracao.ger[0].v_pu;
            ang1 = Arg(sup1->tensao_pu);      // Preserva valor anterior do ângulo
            if(fabs(mod1) < ZEROV) mod1 = 1.; // Proteção
            sup1->tensao_pu  = polar(mod1, ang1);
            p1               = (geracao.ger[0].p_mw +
                                geracao.ger[1].p_mw +
                                geracao.ger[2].p_mw) * sbinv;  // 3 fases
            sup1->sesp_pu   += complex<double>(p1, 0.);
            break;

         case BAR_PQ:  // Suprimento PQ
            p1             = (geracao.ger[0].p_mw +
                              geracao.ger[1].p_mw +
                              geracao.ger[2].p_mw) * sbinv;    // 3 fases
            q1             = (geracao.ger[0].q_mvar +
                              geracao.ger[1].q_mvar +
                              geracao.ger[2].q_mvar) * sbinv;  // 3 fases
            sup1->sesp_pu += complex<double>(p1, q1);
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
      bar1 = (TBarra *) gerador.barraCALC;
      sup1 = rede1->LocalizaSuprimentoBarra(bar1);
      if(sup1 == NULL) return(false);
      // Atualiza gerador em função do seu tipo
      switch(bar1->tipo)
         {
         case BAR_SW:  // Barra swing
            mod1 = geracao.ger[0].v_pu;
            ang1 = geracao.ger[0].t_rad;
            if(fabs(mod1) < ZEROV) mod1 = 1.; // Proteção
            sup1->tensao_pu = polar(mod1, ang1);
            break;

         case BAR_PV:  // Gerador PV
            mod1 = geracao.ger[0].v_pu;
            ang1 = Arg(sup1->tensao_pu);      // Preserva valor anterior do ângulo
            if(fabs(mod1) < ZEROV) mod1 = 1.; // Proteção
            sup1->tensao_pu  = polar(mod1, ang1);
            p1               = (geracao.ger[0].p_mw +
                                geracao.ger[1].p_mw +
                                geracao.ger[2].p_mw) * sbinv;  // 3 fases
            sup1->sesp_pu   += complex<double>(p1, 0.);
            break;

         case BAR_PQ:  // Gerador PQ
            p1             = (geracao.ger[0].p_mw +
                              geracao.ger[1].p_mw +
                              geracao.ger[2].p_mw) * sbinv;    // 3 fases
            q1             = (geracao.ger[0].q_mvar +
                              geracao.ger[1].q_mvar +
                              geracao.ger[2].q_mvar) * sbinv;  // 3 fases
            sup1->sesp_pu += complex<double>(p1, q1);
            break;

         default:  // Erro
            return(false);
         }
      }

   return(true);
   }

//---------------------------------------------------------------------------
//eof

