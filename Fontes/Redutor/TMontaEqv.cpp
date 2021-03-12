//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//#include <math.h>
//#include <math.hpp>
#include "TMontaEqv.h"
#include "..\Apl\VTApl.h"
//#include "..\Arranjo\VTArranjo.h"
#include "..\Complexo\Complexo.h"
#include "..\Curvas\VTCurva.h"
#include "..\Diretorio\VTPath.h"
#include "..\Eqv\VTEqv.h"
#include "..\Complexo\Complexo.h"
#include "..\Patamar\VTPatamar.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Potencia\VTDemanda.h"
#include "..\Potencia\VTFornecimento.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTFases.h"
#include "..\Rede\VTGerador.h"
#include "..\Rede\VTGrupo.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTMatY.h"
#include "..\Rede\VTMutua.h"
#include "..\Rede\VTNo.h"
#include "..\Rede\VTNet.h"
#include "..\Rede\VTResflowBar.h"
#include "..\Rede\VTSuprimento.h"
#include "..\Rede\VTTrafo.h"
#include "..\Rede\VTTrafo3E.h"
#include "..\Rede\VTTramo.h"
#include "..\Rede\VTYref.h"
#include "..\Zona\VTZona.h"
#include "..\..\DLL_Inc\Arranjo.h"
#include "..\..\DLL_Inc\Curvas.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Eqv1.h"
#include "..\..\DLL_Inc\Eqv3.h"
#include "..\..\DLL_Inc\Rede.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTFluxo
//---------------------------------------------------------------------------
VTMontaEqv* __fastcall NewObjMontaEqv(VTApl *apl)
   {
   return(new TMontaEqv(apl));
   }

//---------------------------------------------------------------------------
__fastcall TMontaEqv::TMontaEqv(VTApl *apl)
   {
   //salva ponteiro p/ objetos
   this->apl = apl;
   //geral      = (VTGeral*)apl->GetObject(__classid(VTGeral));
   patamares    = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   demanda      = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
   fornecimento = (VTFornecimento*)apl->GetObject(__classid(VTFornecimento));
   //cria objetos
   lisCAR    = new TList();
   lisSUP    = new TList();
   lisTRE    = new TList();
   lisTRF    = new TList();
   lisYREF   = new TList();
   lisGRUPO  = new TList();
   lisEQP    = new TList();
   lisNO     = new TList();
   lisNO_COR = new TList();
   lisNO_GER = new TList();

   // Inicializa estrutura com frações do modelo de carga
   mod_carga.Icte = 1.;  // Default
   mod_carga.Scte = 0.;
   mod_carga.Zcte = 0.;
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/16233879
__fastcall TMontaEqv::~TMontaEqv(void)
   {
   //destrói objetos
   if (eqv)  {delete eqv;  eqv  = NULL;}
   //destrói lista sem destruir seus objetos
   if (lisEQP)    {delete lisEQP;    lisEQP    = NULL;}
   if (lisNO)     {delete lisNO;     lisNO     = NULL;}
   if (lisNO_COR) {delete lisNO_COR; lisNO_COR = NULL;}
   if (lisNO_GER) {delete lisNO_GER; lisNO_GER = NULL;}
   if (lisGRUPO)  {delete lisGRUPO;  lisGRUPO  = NULL;}
   //remove ligações da lista de ligação de suas barras
   RemoveLigDaBarra(lisTRE);
   RemoveLigDaBarra(lisTRF);
   //destrói listas e seus objetos
   if (lisCAR)    {LimpaTList(lisCAR);    delete lisCAR;    lisCAR    = NULL;}
   if (lisSUP)    {LimpaTList(lisSUP);    delete lisSUP;    lisSUP    = NULL;}
   if (lisTRE)    {LimpaTList(lisTRE);    delete lisTRE;    lisTRE    = NULL;}
   if (lisTRF)    {LimpaTList(lisTRF);    delete lisTRF;    lisTRF    = NULL;}
   if (lisYREF)   {LimpaTList(lisYREF);   delete lisYREF;   lisYREF   = NULL;}
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaEqv::CriaCurva(VTCarga *pcarga)
   {
   //variáveis locais
   VTCurva   *curva;

   //cria uma nova Curva
   if ((curva = DLL_NewObjCurva()) == NULL) return(false);
   //associa Curva c/ a Carga
   pcarga->DefineCurva(curva);
   //define dados da Curva
   curva->Codigo        = pcarga->Codigo;
   curva->Id            = pcarga->Id;    //MUITO IMPORTANTE
   curva->Tipo          = curvaPQ;
   curva->Unidade       = unidMVA;
   curva->Escala        = patamares->NumPatamar();
   curva->Tipica        = false;
   curva->NumeroValores = nvCURVA_RRCAR;
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaEqv::DefineModeloCarga(double Icte,
                                             double Scte,
                                             double Zcte)
   {
   if(fabs(Icte + Scte + Zcte - 1.) < 0.0001)
      {
      mod_carga.Icte = Icte;
      mod_carga.Scte = Scte;
      mod_carga.Zcte = Zcte;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaEqv::DefineModeloEqv(bool equilibrado)
   {
   //destrói eventual objeto VEqv criado anteriormente
   if (eqv != NULL) {delete eqv;}
   //cria objeto Eqv1 ou Eqv3
   if (equilibrado)
      {eqv = DLL_NewObjEqv1(apl);}
   else
      {eqv = DLL_NewObjEqv3(apl);}
   }

//---------------------------------------------------------------------------
void __fastcall TMontaEqv::DefineNoRedutivel(TList *lisNO, bool  redutivel)
   {
   //variáveis locais
   VTNo *no;

   //loop p/ todos Nos
   for (int n = 0; n < lisNO->Count; n++)
      {
      no = (VTNo*)lisNO->Items[n];
      no->redutivel = redutivel;
      }
   }

/*
//---------------------------------------------------------------------------
bool __fastcall TMontaEqv::EqvAtualizaTensoes(int np)
   {
   // Variáveis locais
   complex<double> vfn_pu[MAX_FASE];
   VTNo            *pno;

   // Atualiza tensões de todos os Nos
   for(int n = 0; n < lisNO->Count; n++)
      {
      pno = (VTNo*)lisNO->Items[n];
      //determina tensões de fase em V
      for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++)
         {
         vfn_pu[ind_fase] = pno->pai->resflow->Vfn_pu[np][ind_fase];
         }
      //atualiza tensão da Barra
      if(! eqv->AtualizaTensao(pno->pai->Id, vfn_pu)) return(false);
      }

   return(true);
   }
*/
//---------------------------------------------------------------------------
bool __fastcall TMontaEqv::EqvAtualizaTensoes(int np)
   {
   // Variáveis locais
   complex<double> vfn_pu[MAX_FASE];
   VTNo            *pno;
   TList           *lisNO_NET = net->LisNo();

   // Atualiza tensões de todos os Nos
   for(int n = 0; n < lisNO_NET->Count; n++)
      {
      pno = (VTNo*)lisNO_NET->Items[n];
      //determina tensões de fase em pu
      for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++)
         {
         vfn_pu[ind_fase] = pno->pai->resflow->Vfn_pu[np][ind_fase];
         // Comandos abaixo incluídos em 10/12/2009 (JCG)
         if (eqv->Modelo == modeloEQV1)
            {// Desconta o ângulo da zona para tensões calculadas por Gauss1/NR1
            // vfn_pu[ind_fase] *= polar(1., -pno->pai->zona->T_rad[ind_fase]);
            // vfn_pu[ind_fase] *= polar(1., -pno->pai->zona->Defasagem_rad);


// ATENÇÃO: EM ANÁLISE (EFEITOS COLATERAIS?) !!!!!! (2016.08.31)
vfn_pu[ind_fase] *= polar(1., -pno->pai->zona->T_rad[ind_fase]);


            }
         }
      //atualiza tensão da Barra
      if(! eqv->AtualizaTensao(pno->pai->Id, vfn_pu)) return(false);
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaEqv::EqvResBarra(int np)
   {
   // Variáveis locais
   complex<double>  sf_kva[MAX_FASE];
   VTNo             *pno;
   strResBarraAdmit res_barra;

   // Loop p/ todos Nos de geração
   for(int n = 0; n < lisNO_GER->Count; n++)
      {
      pno  = (VTNo*)lisNO_GER->Items[n];

      // Obtém carga equivalente total p/ a Barra pai do No
      if (! eqv->ResBarraDeltaCarga(pno->pai->Id, sf_kva)) return(false);
      //soma as demandas por fase
      //s_kva = CZero();
      //for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++)
      //   {
      //   s_kva += sf_kva[ind_fase];
      //   }
      // Cria/atualiza Carga equivalente
      if(! EqvResCarga(pno, np, sf_kva)) return(false);
      // IMPORTANTE: cria Yref somente 1 vez (impedâncias ctes.)
      if(np > 0) continue;
      // Obtém delta de admitância equivalente na Barra pai do No
      res_barra.bar_id = pno->pai->Id;
      if (! eqv->ResBarraAdmit(&res_barra)) return(false);
      //cria Yref equivalente
      if(! EqvResYref(pno, res_barra.Ynodal0_pu, res_barra.Ynodal1_pu,
                      res_barra.mat_ynodal_S)) return(false);
      }

   // Loop p/ todos Nos de Corte
   for(int n = 0; n < lisNO_COR->Count; n++)
      {
      pno = (VTNo*)lisNO_COR->Items[n];
      // Obtém carga equivalente que deve ser acrescida na Barra pai do No
      if (! eqv->ResBarraDeltaCarga(pno->pai->Id, sf_kva)) return(false);
      //soma as demandas por fase
      //s_kva = CZero();
      //for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++)
      //   {
      //   s_kva += sf_kva[ind_fase];
      //   }
      // Cria/atualiza Carga equivalente
      if(! EqvResCarga(pno, np, sf_kva)) return(false);
      // IMPORTANTE: cria Yref somente 1 vez (impedâncias ctes.)
      if(np > 0) continue;
      // Obtém delta de admitância equivalente na Barra pai do No
      res_barra.bar_id = pno->pai->Id;
      if (! eqv->ResBarraAdmit(&res_barra)) return(false);
      //cria Yref equivalente
      if(! EqvResYref(pno, res_barra.Ynodal0_pu, res_barra.Ynodal1_pu,
                      res_barra.mat_ynodal_S)) return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaEqv::EqvResCarga(VTNo *pno, int  np, complex<double> sf_kva[])
   {
   //variáveis locais
   complex<double> sf_mva, vetor_s_mva[MAX_FASE];
   VTCarga         *pcarga;
   VTDemanda       *demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));

   //verifica se já existe uma Carga na Barra pai do No
   if ((pcarga = ExisteCarga(pno->pai)) == NULL)
      {//cria uma nova Carga
      if ((pcarga = DLL_NewObjCarga(patamares->NumPatamar())) == NULL) return(false);
      //associa Carga c/ Barra pai do No
      pcarga->DefineObjBarra(pno->pai);
      //define que é uma Carga de Reducao
      pcarga->Reducao = true;
      //inicia dados da Carga
      pcarga->Id         = FalsoId();
      pcarga->esq.posbar = 50;
      pcarga->esq.dxbar  = 4000;
      pcarga->esq.dybar  = 4000;
      //define  modelo da Carga
      pcarga->Icte_pu = mod_carga.Icte;
      pcarga->Scte_pu = mod_carga.Scte;
      pcarga->Zcte_pu = mod_carga.Zcte;
      //cria Curva PQ p/ a Carga
      if (! CriaCurva(pcarga)) {delete pcarga; return(false);}
      //insere Carga na lista lisCAR
      lisCAR->Add(pcarga);
      }
   //converte demanda de kVA p/ MVA
   //s_mva = s_kva / 1000.;
   //divide a demanda entre as 3 fases da Carga e zera demanda de neutro
   //s_mva /= 3.;
   //atualiza demandas da fase da Carga no patamar
   for (int nf = 0; nf < MAX_FASE; nf++)
      {
      sf_mva          = sf_kva[nf] * 0.001;  // kVA => MVA
      vetor_s_mva[nf] = sf_mva;
      }
   //atualiza demanda da Carga no patamar
   demanda->Set_S3f_mva(pcarga, np, vetor_s_mva);
   return(true);
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TMontaEqv::EqvResLigacao(void)
   {
   //variáveis locais
   double             defasagem;
   int                num_lig;
   VTBarra            *pbar1, *pbar2;
   VTMatY             *pij, *pji;
   VTTrafo            *ptrafo;
   strResLigacaoAdmit res_ligacao;

   //obtém número de trechos remanescentes
   num_lig = eqv->ResLigacaoCount();
   //cria novos Trechos ou Trafos p/ a rede
   for (int index = 0; index < num_lig; index++)
      {//obtém IDs das barras ligadas pelo Trecho
      res_ligacao.indice = index;
      if (! eqv->ResLigacao(&res_ligacao)) return(false);
      //determina Barras ligadas pela Ligacao
      if ((pbar1 = ExisteBarra(lisNO, res_ligacao.bar1_id)) == NULL) continue;
      if ((pbar2 = ExisteBarra(lisNO, res_ligacao.bar2_id)) == NULL) continue;
      //cria Trafo, independendente das tensões das barras
      if ((ptrafo = DLL_NewObjTrafo(patamares->NumPatamar())) == NULL) continue;
      //IMPORTANTE: define Trafo de redução
      ptrafo->Reducao = true;
      //inicia dados do Trafo
      ptrafo->Id       = FalsoId();
      ptrafo->Codigo   = res_ligacao.codigo;
      ptrafo->snom     = Sbase;
      ptrafo->pri.vnom = pbar1->vnom;
      ptrafo->pri.tap  = ptrafo->pri.vnom;
      ptrafo->sec.vnom = pbar2->vnom;
      ptrafo->sec.tap  = ptrafo->sec.vnom;

      // NOVO TRAFO EQUIVALENTE, COM FASES DISTINTAS NO PRIMÁRIO E NO SECUNDÁRIO
      // (HPS, 2016.08.05)
      // ptrafo->pri.fase = pbar1->zona->Fases;
      // ptrafo->sec.fase = pbar2->zona->Fases;
      ptrafo->pri.fase = res_ligacao.fase_int_pri;
      ptrafo->sec.fase = res_ligacao.fase_int_sec;

      //define defasagem e tipo de ligação em função da defasagem das Zonas
      //defasagem             = pbar2->zona->T_grau[indFASE_A] - pbar1->zona->T_grau[indFASE_A];
      defasagem             = pbar2->zona->Defasagem_grau - pbar1->zona->Defasagem_grau;
      ptrafo->pri.defasagem = 0;

      // ATENÇÃO: modificação feita em 2015.10.17 (HPS)
      // ptrafo->sec.defasagem = fabs(defasagem);  // ANTES
      ptrafo->sec.defasagem = defasagem;  // DEPOIS

      if(defasagem == 0)
         {//estrela/estrela: o aterramento depende de existir ou neutro aterrado
         ptrafo->pri.ligacao = (ptrafo->pri.fase & faseT) ? lgEST_AT : lgEST_ISO;
         ptrafo->sec.ligacao = (ptrafo->sec.fase & faseT) ? lgEST_AT : lgEST_ISO;
         }
      else if(defasagem > 0)
         {//triângulo/estrela aterrado
         ptrafo->pri.ligacao = lgTRIANG;
         ptrafo->sec.ligacao = (ptrafo->sec.fase & faseT) ? lgEST_AT : lgEST_ISO;
         }
      else
         {//estrela aterrado/triângulo
         ptrafo->pri.ligacao = (ptrafo->pri.fase & faseT) ? lgEST_AT : lgEST_ISO;
         ptrafo->sec.ligacao = lgTRIANG;
         }

      // Preenche dados do trafo equivalente
      pij = ptrafo->eqv.matYij_s;
      pji = ptrafo->eqv.matYji_s;
      if((pij == NULL) || (pji == NULL)) return(false);
      ptrafo->eqv.modelo     = eqv->Modelo;
      ptrafo->eqv.y0_ij_pu.g = res_ligacao.Ynodal0_ij_pu.real();
      ptrafo->eqv.y0_ij_pu.b = res_ligacao.Ynodal0_ij_pu.imag();
      ptrafo->eqv.y0_ji_pu.g = res_ligacao.Ynodal0_ji_pu.real();
      ptrafo->eqv.y0_ji_pu.b = res_ligacao.Ynodal0_ji_pu.imag();
      ptrafo->eqv.y1_ij_pu.g = res_ligacao.Ynodal1_ij_pu.real();
      ptrafo->eqv.y1_ij_pu.b = res_ligacao.Ynodal1_ij_pu.imag();
      ptrafo->eqv.y1_ji_pu.g = res_ligacao.Ynodal1_ji_pu.real();
      ptrafo->eqv.y1_ji_pu.b = res_ligacao.Ynodal1_ji_pu.imag();
      for(int i=0; i < MAX_FASE; i++)
         {
         for(int j=0; j < MAX_FASE; j++)
            {
            pij->Y[i][j] = res_ligacao.mat_ynodal_ij_S[i][j];
            pji->Y[i][j] = res_ligacao.mat_ynodal_ji_S[i][j];
            }
         }

      //JCG 2016.01.21 - proteção incluída para evitar matriz de admitâncias nulas
      //                 que irão provocar erro no fluxo
      //verifica matriz de admitância nula
      if (MatrizAdmitanciaNula(ptrafo))
         {//descarta o trafo
         delete ptrafo;
         continue;
         }

      //associa Trafo c/ Barra pai do Nos
      ptrafo->DefineObjBarra(pbar1, pbar2);
      //define posição nas Barras
      ptrafo->esq.posbar1 = 50;
      ptrafo->esq.posbar2 = 50;
      //insere Trafo em lisTRF
      lisTRF->Add(ptrafo);
      }
   return(true);
   }
*/
/*
//---------------------------------------------------------------------------
bool __fastcall TMontaEqv::EqvResLigacao(void)
   {
   //variáveis locais
   double             defasagem;
   int                num_lig;
   VTBarra            *pbar1, *pbar2;
   VTMatY             *pij, *pji;
   VTTrafo            *ptrafo;
   strResLigacaoAdmit res_ligacao;

   //obtém número de trechos remanescentes
   num_lig = eqv->ResLigacaoCount();
   //cria novos Trechos ou Trafos p/ a rede
   for (int index = 0; index < num_lig; index++)
      {//obtém IDs das barras ligadas pelo Trecho
      res_ligacao.indice = index;
      if (! eqv->ResLigacao(&res_ligacao)) return(false);
      //determina Barras ligadas pela Ligacao
      if ((pbar1 = ExisteBarra(lisNO, res_ligacao.bar1_id)) == NULL) continue;
      if ((pbar2 = ExisteBarra(lisNO, res_ligacao.bar2_id)) == NULL) continue;
      //calcula defasagem entre primário e secundário
      defasagem = pbar2->zona->Defasagem_grau - pbar1->zona->Defasagem_grau;
      //cria Trafo, independendente das tensões das barras
      if ((ptrafo = DLL_NewObjTrafo(patamares->NumPatamar())) == NULL) continue;
      //IMPORTANTE: define Trafo de redução
      ptrafo->Reducao = true;
      //inicia dados do Trafo
      ptrafo->Id            = FalsoId();
      ptrafo->Codigo        = res_ligacao.codigo;
      ptrafo->snom          = Sbase;
      //define dados do primário
      ptrafo->pri.defasagem = 0;
      ptrafo->pri.fase      = res_ligacao.fase_int_pri;
      //define dados do secundário
      ptrafo->sec.defasagem = defasagem;
      ptrafo->sec.fase      = res_ligacao.fase_int_sec;

      //inclui neutro e terra, caso a Zona tenha neutro
      // if ((pbar1->zona->Fases & faseN) == faseN) ptrafo->pri.fase |= faseN;
      // if ((pbar1->zona->Fases & faseT) == faseT) ptrafo->pri.fase |= faseT;
      if ((pbar1->resflow->Fases & faseN) == faseN) ptrafo->pri.fase |= faseN;
      if ((pbar1->resflow->Fases & faseT) == faseT) ptrafo->pri.fase |= faseT;
      if ((pbar2->resflow->Fases & faseN) == faseN) ptrafo->sec.fase |= faseN;
      if ((pbar2->resflow->Fases & faseT) == faseT) ptrafo->sec.fase |= faseT;

      //define tipo da ligação
      if(pbar1->zona->Trifasica && pbar2->zona->Trifasica)
         { // Primário e secundário trifásicos
         ptrafo->pri.vnom = pbar1->zona->Vff_kv;
         ptrafo->pri.tap  = ptrafo->pri.vnom;
         //define tipo da ligação: depende da defasagem
         if (IsDoubleZero(defasagem))
            {//estrela: o aterramento depende de existir ou neutro aterrado
            ptrafo->pri.ligacao = (ptrafo->pri.fase & faseT) ? lgEST_AT : lgEST_ISO;
            }
         else
            {//triângulo
            ptrafo->pri.ligacao = lgTRIANG;
            }
         ptrafo->sec.vnom = pbar2->zona->Vff_kv;
         ptrafo->sec.tap  = ptrafo->sec.vnom;
         //define tipo da ligação: depende  da defasagem
         if (IsDoubleZero(defasagem))
            {//estrela: o aterramento depende de existir ou neutro aterrado
            ptrafo->sec.ligacao = (ptrafo->sec.fase & faseT) ? lgEST_AT : lgEST_ISO;
            }
         else
            {//triângulo
            ptrafo->sec.ligacao = lgTRIANG;
            }
         }
      else if(pbar1->zona->Trifasica && (! pbar2->zona->Trifasica))
         { // Primário trifásico, secundário monofásico
         ptrafo->pri.vnom = pbar1->zona->Vff_kv;
         ptrafo->pri.tap  = ptrafo->pri.vnom;
         //define tipo da ligação: depende da defasagem
         if (IsDoubleZero(defasagem))
            {//estrela: o aterramento depende de existir ou neutro aterrado
            ptrafo->pri.ligacao = (ptrafo->pri.fase & faseT) ? lgEST_AT : lgEST_ISO;
            }
         else
            {//triângulo
            ptrafo->pri.ligacao = lgTRIANG;
            }
		  	switch(ptrafo->sec.fase & faseABC)
            {// 1 fase com neutro: define tensão nominal igual à tensão fase/fase da Zona
            case faseA: ptrafo->sec.vnom = pbar2->zona->Vfn_kv[indFASE_A]; break;
            case faseB: ptrafo->sec.vnom = pbar2->zona->Vfn_kv[indFASE_B]; break;
            case faseC: ptrafo->sec.vnom = pbar2->zona->Vfn_kv[indFASE_C]; break;
            // 2 fases com ou sem neutro: define tensão nominal igual à tensão fase/fase da Zona
            case faseAB:  ptrafo->sec.vnom = pbar2->zona->Vff_kv; break;
            case faseBC:  ptrafo->sec.vnom = pbar2->zona->Vff_kv; break;
            case faseCA:  ptrafo->sec.vnom = pbar2->zona->Vff_kv; break;
            // 3 fases: define tensão nominal igual à tensão fase/fase da Zona
            case faseABC: ptrafo->sec.vnom = pbar2->zona->Vff_kv; break;
            default: return(false);
            }
         //define tensão de tap
         ptrafo->sec.tap  = ptrafo->sec.vnom;
         //define tipo da ligação
         ptrafo->sec.ligacao = lgMONOFASICO;
         }

      // Preenche dados do trafo equivalente
      pij = ptrafo->eqv.matYij_s;
      pji = ptrafo->eqv.matYji_s;
      if((pij == NULL) || (pji == NULL)) return(false);
      ptrafo->eqv.modelo     = eqv->Modelo;
      ptrafo->eqv.y0_ij_pu.g = res_ligacao.Ynodal0_ij_pu.real();
      ptrafo->eqv.y0_ij_pu.b = res_ligacao.Ynodal0_ij_pu.imag();
      ptrafo->eqv.y0_ji_pu.g = res_ligacao.Ynodal0_ji_pu.real();
      ptrafo->eqv.y0_ji_pu.b = res_ligacao.Ynodal0_ji_pu.imag();
      ptrafo->eqv.y1_ij_pu.g = res_ligacao.Ynodal1_ij_pu.real();
      ptrafo->eqv.y1_ij_pu.b = res_ligacao.Ynodal1_ij_pu.imag();
      ptrafo->eqv.y1_ji_pu.g = res_ligacao.Ynodal1_ji_pu.real();
      ptrafo->eqv.y1_ji_pu.b = res_ligacao.Ynodal1_ji_pu.imag();
      for(int i=0; i < MAX_FASE; i++)
         {
         for(int j=0; j < MAX_FASE; j++)
            {
            pij->Y[i][j] = res_ligacao.mat_ynodal_ij_S[i][j];
            pji->Y[i][j] = res_ligacao.mat_ynodal_ji_S[i][j];
            }
         }

      //JCG 2016.01.21 - proteção incluída para evitar matriz de admitâncias nulas
      //                 que irão provocar erro no fluxo
      //verifica matriz de admitância nula
      if (MatrizAdmitanciaNula(ptrafo))
         {//descarta o trafo
         delete ptrafo;
         continue;
         }
      //associa Trafo c/ Barra pai do Nos
      ptrafo->DefineObjBarra(pbar1, pbar2);
      //define posição nas Barras
      ptrafo->esq.posbar1 = 50;
      ptrafo->esq.posbar2 = 50;
      //insere Trafo em lisTRF
      lisTRF->Add(ptrafo);
      }
   return(true);
   }
*/
//---------------------------------------------------------------------------
bool __fastcall TMontaEqv::EqvResLigacao(void)
   {
   //variáveis locais
   double             defasagem;
   int                num_lig;
   VTBarra            *pbar1, *pbar2;
   VTMatY             *pij, *pji;
   VTTrafo            *ptrafo;
   strResLigacaoAdmit res_ligacao;

   //obtém número de trechos remanescentes
   num_lig = eqv->ResLigacaoCount();
   //cria novos Trechos ou Trafos p/ a rede
   for (int index = 0; index < num_lig; index++)
      {//obtém IDs das barras ligadas pelo Trecho
      res_ligacao.indice = index;
      if (! eqv->ResLigacao(&res_ligacao)) return(false);
      //determina Barras ligadas pela Ligacao
      if ((pbar1 = ExisteBarra(lisNO, res_ligacao.bar1_id)) == NULL) continue;
      if ((pbar2 = ExisteBarra(lisNO, res_ligacao.bar2_id)) == NULL) continue;
      //calcula defasagem entre primário e secundário
      defasagem = pbar2->zona->Defasagem_grau - pbar1->zona->Defasagem_grau;
      //cria Trafo, independendente das tensões das barras
      if ((ptrafo = DLL_NewObjTrafo(patamares->NumPatamar())) == NULL) continue;
      //IMPORTANTE: define Trafo de redução
      ptrafo->Reducao = true;
      //inicia dados do Trafo
      ptrafo->Id            = FalsoId();
      ptrafo->Codigo        = res_ligacao.codigo;
      ptrafo->snom          = Sbase;
      //define dados do primário
      ptrafo->pri.defasagem = 0;
      ptrafo->pri.fase      = res_ligacao.fase_int_pri;
      //define dados do secundário
      ptrafo->sec.defasagem = defasagem;
      ptrafo->sec.fase      = res_ligacao.fase_int_sec;
      //inclui neutro e terra, caso a Zona tenha neutro
      // if ((pbar1->zona->Fases & faseN) == faseN) ptrafo->pri.fase |= faseN;
      // if ((pbar1->zona->Fases & faseT) == faseT) ptrafo->pri.fase |= faseT;
      if ((pbar1->resflow->Fases & faseN) == faseN) ptrafo->pri.fase |= faseN;
      if ((pbar1->resflow->Fases & faseT) == faseT) ptrafo->pri.fase |= faseT;
      if ((pbar2->resflow->Fases & faseN) == faseN) ptrafo->sec.fase |= faseN;
      if ((pbar2->resflow->Fases & faseT) == faseT) ptrafo->sec.fase |= faseT;
      //define tipo da ligação
      if(pbar1->zona->Trifasica && pbar2->zona->Trifasica)
         { // Primário e secundário trifásicos
         ptrafo->pri.vnom = pbar1->zona->Vff_kv;
         ptrafo->pri.tap  = ptrafo->pri.vnom;
         //define tipo da ligação do primário: depende da defasagem
         if (IsDoubleZero(defasagem))
            {//estrela: o aterramento depende de existir ou neutro aterrado
            ptrafo->pri.ligacao = (ptrafo->pri.fase & faseT) ? lgEST_AT : lgEST_ISO;
            }
         else
            {//triângulo
            ptrafo->pri.ligacao = lgTRIANG;
            }
         ptrafo->sec.vnom = pbar2->zona->Vff_kv;
         ptrafo->sec.tap  = ptrafo->sec.vnom;
         //define tipo da ligação do secundário: sempre estrela, o aterramento depende de existir neutro aterrado
         ptrafo->sec.ligacao = (ptrafo->sec.fase & faseT) ? lgEST_AT : lgEST_ISO;
         }
      else if(pbar1->zona->Trifasica && (! pbar2->zona->Trifasica))
         { // Primário trifásico, secundário monofásico
         ptrafo->pri.vnom = pbar1->zona->Vff_kv;
         ptrafo->pri.tap  = ptrafo->pri.vnom;
         //define tipo da ligação: depende da defasagem
         if (IsDoubleZero(defasagem))
            {//estrela: o aterramento depende de existir ou neutro aterrado
            ptrafo->pri.ligacao = (ptrafo->pri.fase & faseT) ? lgEST_AT : lgEST_ISO;
            }
         else
            {//triângulo
            ptrafo->pri.ligacao = lgTRIANG;
            }
		  	switch(ptrafo->sec.fase & faseABC)
            {// 1 fase com neutro: define tensão nominal igual à tensão fase/fase da Zona
            case faseA: ptrafo->sec.vnom = pbar2->zona->Vfn_kv[indFASE_A]; break;
            case faseB: ptrafo->sec.vnom = pbar2->zona->Vfn_kv[indFASE_B]; break;
            case faseC: ptrafo->sec.vnom = pbar2->zona->Vfn_kv[indFASE_C]; break;
            // 2 fases com ou sem neutro: define tensão nominal igual à tensão fase/fase da Zona
            case faseAB:  ptrafo->sec.vnom = pbar2->zona->Vff_kv; break;
            case faseBC:  ptrafo->sec.vnom = pbar2->zona->Vff_kv; break;
            case faseCA:  ptrafo->sec.vnom = pbar2->zona->Vff_kv; break;
            // 3 fases: define tensão nominal igual à tensão fase/fase da Zona
            case faseABC: ptrafo->sec.vnom = pbar2->zona->Vff_kv; break;
            default: return(false);
            }
         //define tensão de tap
         ptrafo->sec.tap  = ptrafo->sec.vnom;
         //define tipo da ligação
         ptrafo->sec.ligacao = lgMONOFASICO;
         }

      // Preenche dados do trafo equivalente
      pij = ptrafo->eqv.matYij_s;
      pji = ptrafo->eqv.matYji_s;
      if((pij == NULL) || (pji == NULL)) return(false);
      ptrafo->eqv.modelo     = eqv->Modelo;
      ptrafo->eqv.y0_ij_pu.g = res_ligacao.Ynodal0_ij_pu.real();
      ptrafo->eqv.y0_ij_pu.b = res_ligacao.Ynodal0_ij_pu.imag();
      ptrafo->eqv.y0_ji_pu.g = res_ligacao.Ynodal0_ji_pu.real();
      ptrafo->eqv.y0_ji_pu.b = res_ligacao.Ynodal0_ji_pu.imag();
      ptrafo->eqv.y1_ij_pu.g = res_ligacao.Ynodal1_ij_pu.real();
      ptrafo->eqv.y1_ij_pu.b = res_ligacao.Ynodal1_ij_pu.imag();
      ptrafo->eqv.y1_ji_pu.g = res_ligacao.Ynodal1_ji_pu.real();
      ptrafo->eqv.y1_ji_pu.b = res_ligacao.Ynodal1_ji_pu.imag();
      for(int i=0; i < MAX_FASE; i++)
         {
         for(int j=0; j < MAX_FASE; j++)
            {
            pij->Y[i][j] = res_ligacao.mat_ynodal_ij_S[i][j];
            pji->Y[i][j] = res_ligacao.mat_ynodal_ji_S[i][j];
            }
         }

      //JCG 2016.01.21 - proteção incluída para evitar matriz de admitâncias nulas
      //                 que irão provocar erro no fluxo
      //verifica matriz de admitância nula
      if (MatrizAdmitanciaNula(ptrafo))
         {//descarta o trafo
         delete ptrafo;
         continue;
         }
      //associa Trafo c/ Barra pai do Nos
      ptrafo->DefineObjBarra(pbar1, pbar2);
      //define posição nas Barras
      ptrafo->esq.posbar1 = 50;
      ptrafo->esq.posbar2 = 50;
      //insere Trafo em lisTRF
      lisTRF->Add(ptrafo);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaEqv::EqvResYref(VTNo            *pno,
                                      complex<double> y0_pu,
                                      complex<double> y1_pu,
												  complex<double> maty_pu[MAX_FASE][MAX_FASE])
   {
   //variáveis locais
   VTYref *yref;

   try{//cria Yref e insere em lisYREF
      yref = DLL_NewObjYref();
      lisYREF->Add(yref);
      //inicia dados de Yref
		yref->Id    = FalsoId();
		yref->Y0_pu = y0_pu;
		yref->Y1_pu = y1_pu;
      for (int i = 0; i < MAX_FASE; i++)
         {
         for (int j = 0; j < MAX_FASE; j++)
            {
				yref->Y_pu[i][j] = maty_pu[i][j];
            }
         }
      yref->esq.posbar = 50;
      yref->esq.dxbar  = -4000;
      yref->esq.dybar  = 4000;
      //associa Yref c/ Barra pai do No
      yref->DefineObjBarra(pno->pai);
      }catch(Exception &e)
         {
         return(false);
         }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaEqv::Executa(VTNet *net, TList *lisLIG, TList *lisBAR,
                                   TList *lisBAR_COR, TList *lisBAR_GER,
                                   int ano_carga, double Sbase,
                                   bool aviso_enabled)
   {
   // Variáveis locais
   bool      flag_reducao;
   int       max_pat;
   VTBarra   *barra;
   //VTPatamar *patamar;

   // Salva dados recebidos
   this->net           = net;
   this->ano           = ano_carga;
   this->Sbase         = Sbase;
   this->aviso_enabled = aviso_enabled;
   //proteção
   if (eqv == NULL) return(false);
   //inicia listas de Nos
   if(! IniciaLisNo(lisBAR,     lisNO))     return(false);
   if(! IniciaLisNo(lisBAR_COR, lisNO_COR)) return(false);
   if(! IniciaLisNo(lisBAR_GER, lisNO_GER)) return(false);
   //define Nos  redutíveis ou não
   DefineNoRedutivel(net->LisNo(), false);
   DefineNoRedutivel(lisNO,        true );
   DefineNoRedutivel(lisNO_COR,    false);
   DefineNoRedutivel(lisNO_GER,    false);
   // Redução da rede: primeira parte (topologia; independente dos patamares)
   if(aviso_enabled) eqv->TipoRelatorio = 2;  // trRELATORIO_DETALHADO
   else              eqv->TipoRelatorio = 0;  // trSEM_RELATORIO
   if(! eqv->MontaRedeOriginal(net)) return(false);
   if(! eqv->DeterminaReducao())     return(false); // Não há barras a eliminar
   if(! eqv->ReduzRede())            return(false);
   if(! EqvResLigacao())             return(false);
   eqv->DefineModeloCarga(mod_carga.Icte, mod_carga.Scte, mod_carga.Zcte);

   // Redução da rede: segunda parte (imposição de tensões e cálculo das
   // novas cargas; dependente dos patamares)
   max_pat = patamares->NumPatamar();
   for (int np = 0; np < max_pat; np++)
      {
//      patamar = (VTPatamar*)patamares->Patamar[np];
//      if (! patamar->Enabled)             continue;
      if (! EqvAtualizaTensoes(np))       return(false);
      if (! eqv->CalculaNovasCargas())    return(false);
      if (! EqvResBarra(np))              return(false);
      if (! eqv->ImprimeRedeReduzida(np)) return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
VTBarra* __fastcall TMontaEqv::ExisteBarra(TList *lisNO, int bar_id)
   {
   //variáveis locais
   VTNo *pno;

   //loop p/ todos Nos da lista
   for (int n = 0; n < lisNO->Count; n++)
      {
      pno = (VTNo*)lisNO->Items[n];
      if (pno->pai->Id == bar_id) return(pno->pai);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
VTCarga* __fastcall TMontaEqv::ExisteCarga(VTBarra *pbarra)
   {
   //variáveis locais
   VTCarga *pcarga;

   //loop p/ todas Cargas em lisCAR
   for (int n = 0; n < lisCAR->Count; n++)
      {
      pcarga = (VTCarga*)lisCAR->Items[n];
      if (pcarga->pbarra == pbarra) return(pcarga);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaEqv::IniciaLisGrupoMutua(TList *lisLIG)
   {
   //variáveis locais
   VTEqpto  *eqpto;
   VTGrupo  *grupo;

   //reinicia lista lisGRUPO
   lisGRUPO->Clear();
   //determina Grupos de Mutuas p/ Trechos em lisLIG
   for (int n = 0; n < lisLIG->Count; n++)
      {
      eqpto = (VTEqpto*)lisLIG->Items[n];
      if (eqpto->Tipo() != eqptoTRECHO) continue;
      //verifica se o Trecho está em um Grupo de Mutua
      if ((grupo = net->ExisteGrupoMutua((VTTrecho*)eqpto)) == NULL) continue;
      //insere Grupo em lisGRUPO
      if (lisGRUPO->IndexOf(grupo) < 0) lisGRUPO->Add(grupo);
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaEqv::IniciaLisNo(TList *lisBAR, TList *lisNO)
   {
   //variáveis locais
   VTBarra *barra;

   //reinicia lisNO
   lisNO->Clear();
   //loop p/ todas Barras
   for (int n = 0; n < lisBAR->Count; n++)
      {
      barra = (VTBarra*)lisBAR->Items[n];
      if (barra->No[net] == NULL) return(false);
      //inse No em lisNO
      if (lisNO->IndexOf(barra->No[net]) < 0) lisNO->Add(barra->No[net]);
      }
   return(true);
   }

//---------------------------------------------------------------------------
TList* __fastcall TMontaEqv::LisCargaEquivalente(void)
   {
   return(lisCAR);
   }

//---------------------------------------------------------------------------
TList* __fastcall TMontaEqv::LisTrafoEquivalente(void)
   {
   return(lisTRF);
   }

//---------------------------------------------------------------------------
TList* __fastcall TMontaEqv::LisTrechoEquivalente(void)
   {
   return(lisTRE);
   }

//---------------------------------------------------------------------------
TList* __fastcall TMontaEqv::LisYrefEquivalente(void)
   {
   return(lisYREF);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaEqv::MatrizAdmitanciaNula(VTTrafo *trafo)
   {
   //variáveis locais
   VTMatY *pij, *pji;

   // Preenche dados do trafo equivalente
   pij = trafo->eqv.matYij_s;
   pji = trafo->eqv.matYji_s;
   for(int i=0; i < MAX_FASE; i++)
      {
      for(int j=0; j < MAX_FASE; j++)
         {
         if (Abs(pij->Y[i][j]) > 0.) return(false);
         if (Abs(pji->Y[i][j]) > 0.) return(false);
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/16233879
void __fastcall TMontaEqv::RemoveLigDaBarra(TList *lisLIG)
   {
   //variáveis locais
   VTLigacao *ligacao;

   for(int n = 0; n < lisLIG->Count; n++)
	   {
	   ligacao = (VTLigacao*)lisLIG->Items[n];
       //garante que as barras da ligação não tenham mais a ligação em sua lista
	   if (ligacao->pbarra1 != NULL) ligacao->pbarra1->RemoveLigacao(ligacao);
	   if (ligacao->pbarra2 != NULL) ligacao->pbarra2->RemoveLigacao(ligacao);
	   if (ligacao->pbarra3 != NULL) ligacao->pbarra3->RemoveLigacao(ligacao);
	   }
   }

//---------------------------------------------------------------------------
int __fastcall TMontaEqv::SentidoFluxo(double fluxo)
   {
   //variáveis locais
   int sentido;

   //assume que não há fluxo
   sentido = fluxoNENHUM;
   //converte fluxo p/ pu
   fluxo /= Sbase;
   if (fabs(fluxo) > 1e-5) sentido = (fluxo > 0) ? fluxoPOS : fluxoNEG;

   return(sentido);
   }

//---------------------------------------------------------------------------
//eof
