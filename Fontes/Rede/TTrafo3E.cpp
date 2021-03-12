//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <algorithm>
#include <Math.hpp>
#include "TTrafo3E.h"
#include "VTBarra.h"
#include "VTResFlowLig.h"
#include "..\Constante\Fases.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTTrafo3E* __fastcall NewObjTrafo3E(int num_pat)
   {
   return(new TTrafo3E(num_pat));
   }
   
//---------------------------------------------------------------------------
__fastcall TTrafo3E::TTrafo3E(int num_pat)
   {
   //variávei locais
   int num_bar = 3;

   //cria objeto VTResFlowLig p/ armazenar resultados de fluxo
   AlocaResFlowLig(num_bar, num_pat);
   // LTC semprpe com tap fixo
   ltc.tipo      = ltcFIXO;
   ltc.dv_perc   = 20.;
   ltc.num_passo = 20;
   ltc.passo     = 0;
   //inicia atributos p/ visualização gráfica
   Show[lig_P] = true;
   Show[lig_Q] = true;
   }

//---------------------------------------------------------------------------
__fastcall TTrafo3E::~TTrafo3E(void)
   {
   //falta implementar
   }

//---------------------------------------------------------------------------
bool __fastcall TTrafo3E::AngulosNoSecundario(double vet_ang_grau[MAX_FASE])
   {
   //variáveis locais
   double defasagem = 0;

   if ((pri.ligacao == lgTRIANG)&&(sec.ligacao != lgTRIANG)) defasagem = (fabs(sec.defasagem));
   if ((pri.ligacao != lgTRIANG)&&(sec.ligacao == lgTRIANG)) defasagem = (-fabs(sec.defasagem));
   //inicia vetor de ângulos
   vet_ang_grau[indFASE_A] = defasagem;
   vet_ang_grau[indFASE_B] = defasagem - 120;
   vet_ang_grau[indFASE_C] = defasagem + 120;
   vet_ang_grau[indNEUTRO] = 0;
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TTrafo3E::AngulosNoTerciario(double vet_ang_grau[MAX_FASE])
   {
   //variáveis locais
   double defasagem = 0;

   if ((pri.ligacao == lgTRIANG)&&(ter.ligacao != lgTRIANG)) defasagem = (fabs(ter.defasagem));
   if ((pri.ligacao != lgTRIANG)&&(ter.ligacao == lgTRIANG)) defasagem = (-fabs(ter.defasagem));
   //inicia vetor de ângulos
   vet_ang_grau[indFASE_A] = defasagem;
   vet_ang_grau[indFASE_B] = defasagem - 120;
   vet_ang_grau[indFASE_C] = defasagem + 120;
   vet_ang_grau[indNEUTRO] = 0;
   return(true);
   }

//---------------------------------------------------------------------------
VTTrafo3E* __fastcall TTrafo3E::Clone(void)
   {
   //variáveis locais
   VTTrafo3E *trafo3E;

   //cria um novo Trafo3E
   if ((trafo3E = NewObjTrafo3E(resflow->Max_pat)) != NULL)
      {//copia dados deste Trafo3E p/ o novo Trafo3E
      trafo3E->CopiaAtributosDe(*this);
      }
   return(trafo3E);
   }

//---------------------------------------------------------------------------
void __fastcall TTrafo3E::CopiaAtributosDe(VTEqpto &ref)
   {
   //variáveis locais
   VTTrafo3E &trafo = (VTTrafo3E&)ref;

   //proteção
   if (ref.Tipo() != eqptoTRAFO3E) return;
   //copia atributos de TLigacao
   TLigacao::CopiaAtributosDe(trafo);
   //copia atributos de VTTrafo3E
   perda_ferro = trafo.perda_ferro;
   pri         = trafo.pri;
   sec         = trafo.sec;
   ter         = trafo.ter;
   zps0         = trafo.zps0;
   zps1        = trafo.zps1;
   zpt0        = trafo.zpt0;
   zpt1        = trafo.zpt1;
   zst0        = trafo.zst0;
   zst1        = trafo.zst1;
   ltc         = trafo.ltc;
   }

/*
//alteração para considerar ligaçõe Dy1 e Dy11
//---------------------------------------------------------------------------
double __fastcall TTrafo3E::DefasagemPriSec(void)
   {
   //variáveis locais
   if ((pri.ligacao == lgTRIANG)&&(sec.ligacao != lgTRIANG)) return(fabs(sec.defasagem));
   if ((pri.ligacao != lgTRIANG)&&(sec.ligacao == lgTRIANG)) return(-fabs(sec.defasagem));
   return(0.);
   }

//---------------------------------------------------------------------------
double __fastcall TTrafo3E::DefasagemPriTer(void)
   {
   //variáveis locais
   if ((pri.ligacao == lgTRIANG)&&(ter.ligacao != lgTRIANG)) return(fabs(sec.defasagem));
   if ((pri.ligacao != lgTRIANG)&&(ter.ligacao == lgTRIANG)) return(-fabs(sec.defasagem));
   return(0.);
   }
*/
//---------------------------------------------------------------------------
double __fastcall TTrafo3E::DefasagemPriSec(void)
   {
   //variáveis locais
   if ((pri.ligacao == lgTRIANG)&&(sec.ligacao != lgTRIANG)) return(sec.defasagem);
   if ((pri.ligacao != lgTRIANG)&&(sec.ligacao == lgTRIANG)) return(-sec.defasagem);
   return(0.);
   }

//---------------------------------------------------------------------------
double __fastcall TTrafo3E::DefasagemPriTer(void)
   {
   //variáveis locais
   if ((pri.ligacao == lgTRIANG)&&(ter.ligacao != lgTRIANG)) return(ter.defasagem);
   if ((pri.ligacao != lgTRIANG)&&(ter.ligacao == lgTRIANG)) return(ter.defasagem);
   return(0.);
   }

//---------------------------------------------------------------------------
bool __fastcall TTrafo3E::ExisteLigacaoDelta(void)
   {
   if (pri.ligacao == lgTRIANG) return(true);
   if (sec.ligacao == lgTRIANG) return(true);
   if (ter.ligacao == lgTRIANG) return(true);
   return(false);
   }

//---------------------------------------------------------------------------
int __fastcall TTrafo3E::Fases(VTBarra *barra)
   {
   //variáveis locais
   int ligacao;

   //verifica se é a Barra do primário, secundário ou terciário
   if      (barra == pbarra1) ligacao = pri.ligacao;
   else if (barra == pbarra2) ligacao = sec.ligacao; 
   else                       ligacao = ter.ligacao;
   //determnina fases em função da ligacao
   switch(ligacao)
      {
      case lgEST_AT:  return(faseABCNT);
      case lgTRIANG:  return(faseABC);
      case lgEST_ISO: return(faseABCN);
      }
   return(faseABCN);
   }

//---------------------------------------------------------------------------
int __fastcall TTrafo3E::NumeroBarras(void)
   {
   return(3);
   }

//---------------------------------------------------------------------------
double __fastcall TTrafo3E::PerdaNoFerroMW(void)
   {
   //variáveis locais
   double snom_max;

   //determina maior snom
   snom_max = max(max(pri.snom, sec.snom), ter.snom);
   return((perda_ferro/100.) * snom_max);  //MW
   }

//---------------------------------------------------------------------------
void __fastcall TTrafo3E::OrdenaBar123PriSecTer(void)
   {
   //variáveis locais
   double vnom[3]   = {pri.vnom, sec.vnom, ter.vnom};
   struct strBARPOS{
                   VTBarra *pbar;
                   double posbar;
                   };
   strBARPOS vet_ord[3] = {{NULL, 50}, {NULL, 50}, {NULL, 50}};
   strBARPOS vet_des[3] = {{pbarra1, esq.posbar1}, {pbarra2, esq.posbar2}, {pbarra3, esq.posbar3}};

   //verifica se as Barras já estão ordenadas
   if ((pri.vnom == pbarra1->vnom)&&
       (sec.vnom == pbarra2->vnom)&&
       (ter.vnom == pbarra3->vnom)) return;
   //monta vetor ordenado das Barras: pbar1: primário, pbar2: secundário, pbar3: terciário
   for (int n = 0; n < 3; n++)
      {
      for (int i = 0; i < 3; i++)
         {
         if (vet_des[i].pbar == NULL) continue;
         if (vet_des[i].pbar->vnom == vnom[n])
            {//salva em vet_ord[n] os dados da barra que está em vet_des[i]
            vet_ord[n].pbar   = vet_des[i].pbar;
            vet_ord[n].posbar = vet_des[i].posbar;
            //elimina de vet_des[i] a barra que já foi transferida p/ vet_ord
            vet_des[i].pbar   = NULL;
            break;
            }
         }
      }
   //redefine pbar1, pbar2, pbar3
   pbarra1 = vet_ord[0].pbar; esq.posbar1 = vet_ord[0].posbar;
   pbarra2 = vet_ord[1].pbar; esq.posbar2 = vet_ord[1].posbar;
   pbarra3 = vet_ord[2].pbar; esq.posbar3 = vet_ord[2].posbar;
   }
   
//---------------------------------------------------------------------------
int __fastcall TTrafo3E::PM_GetLigacao(void)
   {
   return((100 * pri.ligacao) + (10 * sec.ligacao) + ter.ligacao);
   }

//---------------------------------------------------------------------------
AnsiString  __fastcall TTrafo3E::PM_GetPadrao(void)
   {
   return(padrao);
   }

//---------------------------------------------------------------------------
double __fastcall TTrafo3E::PM_GetSbase(void)
   {
   //variáveis locais
   float Sbase;

   //Sbase é a maior Snom dos 3 enrolamentos
   Sbase = pri.snom;
   Sbase = max(Sbase, sec.snom);
   Sbase = max(Sbase, ter.snom);
   return(Sbase);
   }

//---------------------------------------------------------------------------
int __fastcall TTrafo3E::PM_GetTap(void)
   {
   //variáveis locais
   int    tap;
   double tap_float;
   double relacao_trafo;
   double dv_kv, dv_max_kv;

   //assume LTC com Tap fixo
   ltc.tipo = ltcFIXO;
   //determina relação de transformação para as tensões de tap
   relacao_trafo = pri.tap /sec.tap;
   //redefine tensão de tap do secundário igual à tensão nominal
   sec.tap = sec.vnom;
   ter.tap = ter.vnom;
   //recalcula tensão de tap do primário, mantendo a relação de transformação
   pri.tap = relacao_trafo * sec.tap;
   //determina o tap do trafo como valor float
   dv_kv     = pri.tap - pri.vnom;
   dv_max_kv = (ltc.dv_perc * 0.01) * pri.vnom;
   tap_float = (dv_kv * ltc.num_passo) / dv_max_kv;
   //converte tap para valor inteiro
   tap       = RoundTo(tap_float, 0);
   //proteção: não permite tap com valor maior que o número de passos
   if (abs(tap) > ltc.num_passo)
      {
      if      (tap > 0) tap = ltc.num_passo;
      else if (tap < 0) tap = -ltc.num_passo;
      }
   //salva o tap calculado
   ltc.passo = tap;

   return(tap);
   }

//---------------------------------------------------------------------------
void __fastcall TTrafo3E::PM_SetPadrao(AnsiString padrao)
   {
   this->padrao = padrao;
   }

//---------------------------------------------------------------------------
void __fastcall TTrafo3E::PM_SetTap(int tap)
   {
   //variáveis locais
   double fat_mult;

   //proteção
   if (abs(tap) > ltc.num_passo)
      {
      if      (tap > 0) tap =  ltc.num_passo;
      else if (tap < 0) tap = -ltc.num_passo;
      }
   //redefine LTC com Tap fixo
   ltc.tipo = ltcFIXO;
   pri.tap  = pri.vnom;
   sec.tap  = sec.vnom;
   ter.tap  = ter.vnom;
   //define o passo para o ajuste fixo
   ltc.passo = tap;
   //redefine tensão de tap do primário correspondente ao passo definido
   fat_mult = 1.0 + (((ltc.dv_perc * 0.01) / ltc.num_passo) * ltc.passo);
   pri.tap = pri.vnom * fat_mult;
   }

//---------------------------------------------------------------------------
void __fastcall TTrafo3E::RedefineBarraPriSecTer(VTBarra *pb[])
   {
   //variáveis locais
   struct {
		  VTBarra *pbar; int posbar;
		  } ord_ant[3] = {{pbarra1, (int)esq.posbar1}, {pbarra2, (int)esq.posbar2}, {pbarra3, (int)esq.posbar3}},
            ord_pst[3] = {{pb[0],   0},           {pb[1],   0},           {pb[2],   0}};

   //redefine posbar de acordo c/ a nova ordenação das Barras
   for (int i = 0; i < 3; i++)
      {
      for (int j = 0 ; j < 3; j++)
         {
         if (ord_pst[i].pbar == ord_ant[j].pbar)
            {
            ord_pst[i].posbar = ord_ant[j].posbar;
            break;
            }
         }
      }
   //redefine Barras
   pbarra1 = ord_pst[0].pbar; esq.posbar1 = ord_pst[0].posbar;
   pbarra2 = ord_pst[1].pbar; esq.posbar2 = ord_pst[1].posbar;
   pbarra3 = ord_pst[2].pbar; esq.posbar3 = ord_pst[2].posbar;
   //redefine Vnom
   pri.vnom = pbarra1->vnom;
   sec.vnom = pbarra2->vnom;
   ter.vnom = pbarra3->vnom;
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TTrafo3E::RedefineDeltaComoPrimario(void)
   {
   //variáveis locais
   struct {
          VTBarra *pbarra1, *pbarra2, *pbarra3;
          strPST   pri,  sec,  ter;
          strIMP   zps0, zps1, zpt0, zpt1, zst0, zst1;
          } tmp;

   //verifica se o primário está em Delta
   if (pri.ligacao == lgTRIANG) return(true);
   //verifica se o decundário está em Delta
   if (sec.ligacao == lgTRIANG)
      {//YaDYa ==> DYaYa
      tmp.pbarra1 = pbarra2;
      tmp.pbarra2 = pbarra3;
      tmp.pbarra3 = pbarra1;
      tmp.pri   = sec;
      tmp.sec   = ter;
      tmp.ter   = pri;
      tmp.zps0  = zst0;
      tmp.zst0  = zpt0;
      tmp.zpt0  = zps0;
      tmp.zps1  = zst1;
      tmp.zst1  = zpt1;
      tmp.zpt1  = zps1;
      }
   else if (ter.ligacao == lgTRIANG)
      {//YaYaD ==> DYaYa
      tmp.pbarra1 = pbarra3;
      tmp.pbarra2 = pbarra1;
      tmp.pbarra3 = pbarra2;
      tmp.pri   = ter;
      tmp.sec   = pri;
      tmp.ter   = sec;
      tmp.zps0  = zpt0;
      tmp.zst0  = zps0;
      tmp.zpt0  = zst0;
      tmp.zps1  = zpt1;
      tmp.zst1  = zps1;
      tmp.zpt1  = zst1;
      }
   else return(false);
   //salva nova configuração
   pbarra1 = tmp.pbarra1;
   pbarra2 = tmp.pbarra2;
   pbarra3 = tmp.pbarra3;
   pri     = tmp.pri;
   sec     = tmp.sec;
   ter     = tmp.ter;
   zps0    = tmp.zps0;
   zst0    = tmp.zst0;
   zpt0    = tmp.zpt0;
   zps1    = tmp.zps1;
   zst1    = tmp.zst1;
   zpt1    = tmp.zpt1;
   return(true);
   }
*/
//---------------------------------------------------------------------------
bool __fastcall TTrafo3E::RedefineDeltaComoPrimario(void)
   {
   //variáveis locais
   unsigned int ligacao;

   //determina configuração das ligações
   ligacao = 0;
   if (pri.ligacao == lgTRIANG)  ligacao  = 1;
   if (sec.ligacao == lgTRIANG)  ligacao += 2;
   if (ter.ligacao == lgTRIANG)  ligacao += 4;
   //trata as 8 possíveis configurações de ligação do Trafo
   switch (ligacao)
      {
      case 0:  //PriSecTer = YYY: nada a fazer
         return(true);
      case 1:  //PriSecTer = DYY: nada a fazer
         return(true);
      case 2:  //PriSecTer = YDY: muda Sec=>Pri, Ter=>Sec, Pri=>Ter
      case 6:  //PriSecTer = YDD: muda Ter=>Pri, Pri=>Sec, Sec=>Ter
         RotacionaPriSecTer();
         RotacionaPriSecTer();
         return(true);
      case 3:  //PriSecTer = DDY: nada a fazer
         return(true);
      case 4:  //PriSecTer = YYD: muda Ter=>Pri, Pri=>Sec, Sec=>Ter
      case 5:  //PriSecTer = DYD: muda Ter=>Pri, Pri=>Sec, Sec=>Ter
         RotacionaPriSecTer();
         return(true);
      case 7:  //PriSecTer = DDD: nada a fazer
         return(true);
      }
   return(false);
   }

//---------------------------------------------------------------------------
void __fastcall TTrafo3E::RotacionaPriSecTer(void)
   {
   //variáveis locais
   struct {
          VTBarra *pbarra1, *pbarra2, *pbarra3;
          strPST   pri,  sec,  ter;
          strIMP   zps0, zps1, zpt0, zpt1, zst0, zst1;
          } tmp;

   //rotaciona Pri=>Sec, Sec=>Ter, Ter=>Pri
   tmp.pbarra1 = pbarra3;
   tmp.pbarra2 = pbarra1;
   tmp.pbarra3 = pbarra2;
   tmp.pri   = ter;
   tmp.sec   = pri;
   tmp.ter   = sec;
   tmp.zps0  = zpt0;
   tmp.zst0  = zps0;
   tmp.zpt0  = zst0;
   tmp.zps1  = zpt1;
   tmp.zst1  = zps1;
   tmp.zpt1  = zst1;
   //salva nova configuração
   pbarra1 = tmp.pbarra1;
   pbarra2 = tmp.pbarra2;
   pbarra3 = tmp.pbarra3;
   pri     = tmp.pri;
   sec     = tmp.sec;
   ter     = tmp.ter;
   zps0    = tmp.zps0;
   zst0    = tmp.zst0;
   zpt0    = tmp.zpt0;
   zps1    = tmp.zps1;
   zst1    = tmp.zst1;
   zpt1    = tmp.zpt1;
   }

//---------------------------------------------------------------------------
int __fastcall TTrafo3E::Tipo(void)
   {
   return(eqptoTRAFO3E);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TTrafo3E::TipoAsString(void)
   {
   return("Trafo 3E");
   }

//---------------------------------------------------------------------------
//eof
