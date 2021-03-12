//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <math.h>
#include "TFluxoBarra.h"
#include "..\..\Apl\VTApl.h"
#include "..\..\Complexo\Complexo.h"
#include "..\..\Potencia\VTDemanda.h"
#include "..\..\Potencia\VTFornecimento.h"
#include "..\..\Patamar\VTPatamares.h"
#include "..\..\Rede\VTBarra.h"
#include "..\..\Rede\VTCapacitor.h"
#include "..\..\Rede\VTCarga.h"
#include "..\..\Rede\VTFases.h"
#include "..\..\Rede\VTGerador.h"
#include "..\..\Rede\VTNet.h"
#include "..\..\Rede\VTNo.h"
#include "..\..\Rede\VTReator.h"
#include "..\..\Rede\VTResFlowBar.h"
#include "..\..\Rede\VTResFlowSup.h"
#include "..\..\Rede\VTSuprimento.h"
#include "..\..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ cria objeto da Classe
VTFluxo* __fastcall NewObjFluxoBarra(VTApl *apl)
   {
   return(new TFluxoBarra(apl));
   }

//---------------------------------------------------------------------------
__fastcall TFluxoBarra::TFluxoBarra(VTApl *apl)
   {
   //salva ponteiro p/ objeto
   this->apl = apl;
   //inicia dados
   ModeloCarga = cargaAUTO;
   try{//cria objetos
      lisEQP = new TList();
      }catch(Exception &e)
         {
         }
   }

//---------------------------------------------------------------------------
__fastcall TFluxoBarra::~TFluxoBarra(void)
   {
   //destrói lista sem destruir seus objetos
   if (lisEQP) {delete lisEQP; lisEQP = NULL;}
   }

//---------------------------------------------------------------------------
double __fastcall TFluxoBarra::CapacitoresNo(VTDemanda *demanda,
                                             VTNo      *pno,
                                             int       np)
   {
   // Variáveis locais
   double      qtot_mvar;
   VTCapacitor *capacitor;

   // Loop de capacitores do nó
   qtot_mvar = 0.;
   for(int nc=0; nc < pno->lisCAP->Count; nc++)
      {
      capacitor  = (VTCapacitor *) pno->lisCAP->Items[nc];
      qtot_mvar += demanda->Q_mvar[capacitor][np];
      }

   return(qtot_mvar);
   }

//---------------------------------------------------------------------------
bool __fastcall TFluxoBarra::CargaNo(VTDemanda       *demanda,
                                     VTNo            *pno,
                                     int             np,
                                     complex<double> scarga_mva[MAX_FASE])
   {
   // Variáveis locais
   int             modelo_carga;
   complex<double> saux_mva[MAX_FASE];
   VTCarga         *carga;

   // Zera vetor de demandas da carga
   for(int nf=0; nf < MAX_FASE; nf++)
      {
      scarga_mva[nf] = CZero();
      }

   // Loop de cargas do nó
   for(int nc=0; nc < pno->lisCAR->Count; nc++)
      {
      carga  = (VTCarga *) pno->lisCAR->Items[nc];
      if(! demanda->Get_S3f_mva(carga, np, saux_mva)) continue;
      // Distribuição entre fases OK, atualiza demanda por fase
      modelo_carga = (ModeloCarga == cargaAUTO) ? carga->ModeloCarga : ModeloCarga;
      for(int nf=0; nf < MAX_FASE; nf++)
         {
         scarga_mva[nf] += saux_mva[nf] * FatorModeloCarga(pno->pai, modelo_carga, np, nf);
         }
      }

   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TFluxoBarra::CfgAvisoErro(bool enabled)
   {
   aviso_enabled = enabled;
   }

//---------------------------------------------------------------------------
bool __fastcall TFluxoBarra::Executa(VTNet *net, double Sbase,
                                    AnsiString arqRelFlow, AnsiString arqRelRede)
   {
   //variáveis locais
   VTNo   *pno;
   TList *lisNO;

   //salva parâmetros
   this->net    = net;
   this->Sbase  = Sbase;
   //Net deve ter um único No
   lisNO = net->LisNo();
   if (lisNO->Count != 1) return(false);
   //determina único Nó
   pno = (VTNo*)lisNO->First();
   //reinicia dados do Flow
   ReiniciaDadosFlow(pno);
   //verifica se o No possui geração VF
   if (! pno->ExisteGeracaoVF()) return(false);

   // Garante 1 suprimento ou 1 gerador
//   if((pno->lisSUP->Count + pno->lisGER->Count) != 1) return(false);

   //executa cálculo de tensão p/ o único Nó
   if (! ExecutaCalculoTensao(pno)) return(false);
   if (! ExecutaCalculoFornecimento(pno)) return(false);
   return(true);
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TFluxoBarra::ExecutaCalculoFornecimento(VTNo *pno)
   {
   //variáveis locais
   int             nf, ind_fase, max_pat;
   double          v_pu;
   complex<double> saux_mva, s_mva[MAX_FASE];
   VTEqpto        *eqpto;
   VTGerador      *gerador;
   VTSuprimento   *suprimento;
   VTDemanda      *demanda   = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
   VTPatamares    *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   VTFases        *fases     = (VTFases*)apl->GetObject(__classid(VTFases));
   VTDemFase      *dem_fase;
   VTListaDemFase *listaDF;

   //obtém lista de Geradores/Suprimentos do No
   lisEQP->Clear();
   pno->LisGerador(lisEQP);
   pno->LisSuprimento(lisEQP);
   //loop p/ todos Patamares
   max_pat = patamares->NumPatamar();
   for (int np = 0; np < max_pat; np++)
      {//zera demandas por fase p/ o patamar
      for (nf = 0; nf < MAX_FASE; nf++) s_mva[nf] = complex<double>(0., 0.);
      //obtém demandas por fase do No
      listaDF = demanda->Sfase[pno][np];
      for (int n = 0; n < listaDF->Count; n++)
         {
         dem_fase = listaDF->Items[n];
         //determina demanda nominal
         saux_mva = complex<double>(dem_fase->P_mw, dem_fase->Q_mvar);
         //verifica fases
         switch(nf = dem_fase->Fases)
            {
            case faseA:
            case faseB:
            case faseC:
            case faseN:
               ind_fase         = fases->TagToIndex[nf];
               s_mva[ind_fase] += saux_mva * FatorModeloCarga(pno->pai, dem_fase->ModeloCarga, np, ind_fase);
               break;
            case faseAB:
            case faseABN:
               s_mva[indFASE_A] += (saux_mva / 2.) * FatorModeloCarga(pno->pai, dem_fase->ModeloCarga, np, indFASE_A);
               s_mva[indFASE_B] += (saux_mva / 2.) * FatorModeloCarga(pno->pai, dem_fase->ModeloCarga, np, indFASE_B);
               break;
            case faseBC:
            case faseBCN:
               s_mva[indFASE_B] += (saux_mva / 2.) * FatorModeloCarga(pno->pai, dem_fase->ModeloCarga, np, indFASE_B);
               s_mva[indFASE_C] += (saux_mva / 2.) * FatorModeloCarga(pno->pai, dem_fase->ModeloCarga, np, indFASE_C);
               break;
            case faseCA:
            case faseCAN:
               s_mva[indFASE_C] += (saux_mva / 2.) * FatorModeloCarga(pno->pai, dem_fase->ModeloCarga, np, indFASE_C);
               s_mva[indFASE_A] += (saux_mva / 2.) * FatorModeloCarga(pno->pai, dem_fase->ModeloCarga, np, indFASE_A);
               break;
            case faseABC:
            case faseABCN:
               s_mva[indFASE_A] += (saux_mva / 3.) * FatorModeloCarga(pno->pai, dem_fase->ModeloCarga, np, indFASE_A);
               s_mva[indFASE_B] += (saux_mva / 3.) * FatorModeloCarga(pno->pai, dem_fase->ModeloCarga, np, indFASE_B);
               s_mva[indFASE_C] += (saux_mva / 3.) * FatorModeloCarga(pno->pai, dem_fase->ModeloCarga, np, indFASE_C);
               break;
            }
         }
      //divide demanda entre os Geradores/Suprimentos do No
      for (int n = 0; n < lisEQP->Count; n++)
         {
         eqpto = (VTEqpto*)lisEQP->Items[n];
         if (eqpto->Tipo() == eqptoGERADOR)
            {
            gerador = (VTGerador*)eqpto;
            for (nf = 0; nf < MAX_FASE; nf++)
               {//OBS: esta linha dá erro no Builder2007 (release)
               //gerador->resflow->Sf_mva[np][nf] = s_mva[nf]/double(lisEQP->Count);
               saux_mva = s_mva[nf]/double(lisEQP->Count);
               gerador->resflow->Sf_mva[np][nf] = saux_mva;
               }
            }
         else if (eqpto->Tipo() == eqptoSUPRIMENTO)
            {
            suprimento = (VTSuprimento*)eqpto;
            for (nf = 0; nf < MAX_FASE; nf++)
               {//OBS: esta linha dá erro no Builder2007 (release)
               //suprimento->resflow->Sf_mva[np][nf] = s_mva[nf]/double(lisEQP->Count);
               saux_mva = s_mva[nf]/double(lisEQP->Count);
               suprimento->resflow->Sf_mva[np][nf] = saux_mva;
               }
            }
         }
      }
   return(true);
   }
*/
//---------------------------------------------------------------------------
bool __fastcall TFluxoBarra::ExecutaCalculoFornecimento(VTNo *pno)
   {
   //variáveis locais
   int             nf, ind_fase, max_pat, modelo_carga;
   double          v_pu, q_mvar;
   complex<double> saux_mva, s_mva[MAX_FASE], scarga_mva[MAX_FASE];
   VTEqpto        *eqpto;
   VTGerador      *gerador;
   VTSuprimento   *suprimento;
   VTDemanda      *demanda   = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
   VTPatamares    *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   // VTFases        *fases     = (VTFases*)apl->GetObject(__classid(VTFases));

   //obtém lista de Geradores/Suprimentos do No
   lisEQP->Clear();
   pno->LisGerador(lisEQP);
   pno->LisSuprimento(lisEQP);
   //loop p/ todos Patamares
   max_pat = patamares->NumPatamar();
   for (int np = 0; np < max_pat; np++)
      {//zera demandas por fase p/ o patamar
      for (nf = 0; nf < MAX_FASE; nf++) s_mva[nf] = complex<double>(0., 0.);
      //obtém demanda reativa nominal de Reatores e Capacitores do No
      q_mvar = ReatoresNo(demanda, pno, np) - CapacitoresNo(demanda, pno, np);
      //divide a demanda reativa entre as 3 fase
      q_mvar /= 3.;
      for (nf = 0; nf < MAX_FASE-1; nf++)
         {
         s_mva[nf] += complex<double>(0., q_mvar * FatorModeloCarga(pno->pai, cargaZCTE, np, nf));
         }
      //obtém demanda por fase de todas as cargas do No (IMPORTANTE: sem incluir Capacitores/Reatores)
      if(CargaNo(demanda, pno, np, scarga_mva))
         {
         for(int nf=0; nf < MAX_FASE; nf++)
            {
            s_mva[nf] += scarga_mva[nf];
            }
         }

      //divide demanda entre os Geradores/Suprimentos do No
      for (int n = 0; n < lisEQP->Count; n++)
         {
         eqpto = (VTEqpto*)lisEQP->Items[n];
         if (eqpto->Tipo() == eqptoGERADOR)
            {
            gerador = (VTGerador*)eqpto;
            for (nf = 0; nf < MAX_FASE; nf++)
               {//OBS: esta linha dá erro no Builder2007 (release)
               //gerador->resflow->Sf_mva[np][nf] = s_mva[nf]/double(lisEQP->Count);
               saux_mva = s_mva[nf]/double(lisEQP->Count);
               gerador->resflow->Sf_mva[np][nf] = saux_mva;
               gerador->resflow->TipoSup        = gerador->tiposup;
               }
            }
         else if (eqpto->Tipo() == eqptoSUPRIMENTO)
            {
            suprimento = (VTSuprimento*)eqpto;
            for (nf = 0; nf < MAX_FASE; nf++)
               {//OBS: esta linha dá erro no Builder2007 (release)
               //suprimento->resflow->Sf_mva[np][nf] = s_mva[nf]/double(lisEQP->Count);
               saux_mva = s_mva[nf]/double(lisEQP->Count);
               suprimento->resflow->Sf_mva[np][nf] = saux_mva;
               suprimento->resflow->TipoSup        = suprimento->tiposup;
               }
            }
         }
      }
   return(true);
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TFluxoBarra::ExecutaCalculoTensao(VTNo *no)
   {
   //variáveis locais
   bool            def_por_fase;
   int             fases, max_pat;
   complex<double> v_pu_rad;
   strSUP          vet_sup[MAX_FASE];
   VTBarra         *barra;
   VTPatamares     *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   // Obtém dados do nó
   No_Dados(no, &def_por_fase, &fases);

   //loop p/ todos Patamares
   max_pat = patamares->NumPatamar();
   for (int np = 0; np < max_pat; np++)
      {//determina V,P,Q,T do Suprimento do No
      No_GetPQVTNo(no, np, vet_sup);
      if( def_por_fase)  // Fornecimento desequilibrado
         {//atualiza tensões de todas Barras do No
         for (int nb = 0; nb < no->lisBAR->Count; nb++)
            {
            barra = (VTBarra*)no->lisBAR->Items[nb];
            barra->resflow->Fases = fases;
            for(int nf = 0; nf < MAX_FASE; nf++)
               {
               v_pu_rad  = polar(vet_sup[nf].v, vet_sup[nf].t);
               barra->resflow->Vfn_pu[np][nf] = v_pu_rad;
               }
            }
         }
      else
         {// Fornecimento equilibrado
         v_pu_rad = polar(vet_sup[0].v, vet_sup[0].t);
         //atualiza tensões de todas Barras do No
         for (int nb = 0; nb < no->lisBAR->Count; nb++)
            {
            barra = (VTBarra*)no->lisBAR->Items[nb];
            barra->resflow->Fases      = fases;
            barra->resflow->Van_pu[np] = v_pu_rad;
            barra->resflow->Vbn_pu[np] = v_pu_rad * Alfa2();
            barra->resflow->Vcn_pu[np] = v_pu_rad * Alfa();
            barra->resflow->Vnt_pu[np] = complex<double>(0., 0.);
            }
         }
      } // for(np)
   return(true);
   }
*/
//---------------------------------------------------------------------------
bool __fastcall TFluxoBarra::ExecutaCalculoTensao(VTNo *no)
   {
   //variáveis locais
   bool            def_por_fase;
   int             fases, max_pat;
   complex<double> v_pu_rad;
   strSUP          vet_sup[MAX_FASE];
   VTBarra         *barra;
   VTPatamares     *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   //determina número de Patamares
   max_pat = patamares->NumPatamar();
   // Obtém dados do nó
   No_Dados(no, &def_por_fase, &fases);
   //loop p/ todas Barras
   for (int nb = 0; nb < no->lisBAR->Count; nb++)
      {//atualiza tensões de todas Barras do No
      barra = (VTBarra*)no->lisBAR->Items[nb];
      barra->resflow->Fases = fases;
      for (int np = 0; np < max_pat; np++)
         {//determina PQVT or fase do Suprimento do No patamar
         No_GetPQVTNo(no, np, vet_sup);
         for(int nf = 0; nf < MAX_FASE; nf++)
            {
            v_pu_rad  = polar(vet_sup[nf].v, vet_sup[nf].t);
            barra->resflow->Vfn_pu[np][nf] = v_pu_rad;
            }
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
double __fastcall TFluxoBarra::FatorModeloCarga(VTBarra *barra, int modelo_carga, int ind_pat, int ind_fase)
   {
   //variáveis locais
   double fator, v_pu;

   //determina módulo da tensão em pu
   v_pu = Abs(barra->resflow->Vfn_pu[ind_pat][ind_fase]);
    //verifica modelo de carga
    switch(modelo_carga)
      {
      case cargaICTE:
         fator = v_pu;
         break;
      case cargaSCTE:
         fator = 1.0;
         break;
      case cargaZCTE:
         fator = (v_pu *v_pu);
         break;
      default:
         fator = v_pu;
         break;
      }
    return(fator);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TFluxoBarra::Modelo(void)
   {
   return("Gauss");
   }

//---------------------------------------------------------------------------
bool __fastcall TFluxoBarra::PM_GetEquilibrado(void)
   {
   return(false);
   }

//---------------------------------------------------------------------------
void __fastcall TFluxoBarra::No_Dados(VTNo *pno, bool *def_por_fase, int *fases)
   {
   //variáveis locais
   VTGerador    *gerador;
   VTSuprimento *suprimento;

   // Utiliza primeiro suprimento ou gerador "Definido Por Fase" que ele encontrar no nó
   for(int n=0; n < pno->lisSUP->Count; n++)
      {
      suprimento = (VTSuprimento *) pno->lisSUP->Items[n];
      if(suprimento->DefinidoPorFase)
         {
         *def_por_fase = true;
         *fases        = suprimento->Fases;
         return;
         }
      }
   for(int n=0; n < pno->lisGER->Count; n++)
      {
      gerador = (VTGerador *) pno->lisGER->Items[n];
      if(gerador->DefinidoPorFase)
         {
         *def_por_fase = true;
         *fases        = faseABCN;  // Gerador não tem "Fases"
         return;
         }
      }

   // Default
   *def_por_fase = false;
   *fases        = faseABCN;
   }

//---------------------------------------------------------------------------
bool __fastcall TFluxoBarra::No_GetPQVTNo(VTNo *pno, int np, strSUP vet_sup[MAX_FASE])
   {
   //variáveis locais
   VTGerador      *gerador;
   VTSuprimento   *suprimento;
   VTFornecimento *fornecimento = (VTFornecimento*)apl->GetObject(__classid(VTFornecimento));

   // Utiliza primeiro suprimento ou gerador que ele encontrar no nó
   for(int n=0; n < pno->lisSUP->Count; n++)
      {
      suprimento = (VTSuprimento *) pno->lisSUP->Items[n];
      if(fornecimento->Get_PQVT_porfase(suprimento, np, vet_sup)) return(true);
      }
   for(int n=0; n < pno->lisGER->Count; n++)
      {
      gerador = (VTGerador *) pno->lisGER->Items[n];
      if(fornecimento->Get_PQVT_porfase(gerador, np, vet_sup)) return(true);
      }

   return(false);
   }

//---------------------------------------------------------------------------
double __fastcall TFluxoBarra::ReatoresNo(VTDemanda *demanda,
                                          VTNo      *pno,
                                          int       np)
   {
   // Variáveis locais
   double   qtot_mvar;
   VTReator *reator;

   // Loop de reatores do nó
   qtot_mvar = 0.;
   for(int nr=0; nr < pno->lisREA->Count; nr++)
      {
      reator     = (VTReator *) pno->lisREA->Items[nr];
      qtot_mvar += demanda->Q_mvar[reator][np];
      }

   return(qtot_mvar);
   }

//---------------------------------------------------------------------------
void __fastcall TFluxoBarra::ReiniciaDadosFlow(VTNo *pno)
   {
   pno->pai->resflow->Reinicia();
   }
   
//---------------------------------------------------------------------------
bool __fastcall TFluxoBarra::UtilizaBarraPV(void)
   {
   return(false);
   }

//---------------------------------------------------------------------------
bool __fastcall TFluxoBarra::ValidaAderencia(VTNet *net, VTLog *plog)
   {
   return(true);
   }

//---------------------------------------------------------------------------
//eof