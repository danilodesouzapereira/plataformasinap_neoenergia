//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <math.h>
#include "TFluxoTramo.h"
#include "..\..\Apl\VTApl.h"
#include "..\..\Complexo\Complexo.h"
#include "..\..\Patamar\VTPatamares.h"
#include "..\..\Rede\VTBarra.h"
#include "..\..\Rede\VTFases.h"
#include "..\..\Rede\VTNet.h"
#include "..\..\Rede\VTResFlowBar.h"
#include "..\..\Rede\VTResFlowLig.h"
#include "..\..\Rede\VTTramo.h"
#include "..\..\Rede\VTTrecho.h"
#include "..\..\Zona\VTZona.h"
#include "..\..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ cria objeto da Classe
//---------------------------------------------------------------------------
VTFluxoTramo* __fastcall NewObjFluxoTramo(VTApl *apl)
   {
   return(new TFluxoTramo(apl));
   }

//---------------------------------------------------------------------------
__fastcall TFluxoTramo::TFluxoTramo(VTApl *apl)
   {
   //salva ponteiro p/ objeto
   this->apl = apl;
   patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   //cria objetos
   lisBAR = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TFluxoTramo::~TFluxoTramo(void)
   {
   //destrói listas sem destruir seus objetos
   if (lisBAR) {delete lisBAR; lisBAR = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFluxoTramo::CalculaFluxoTrecho(VTTramo *tramo)
  {
   //variáveis locais
   int             ind_bar1_trm, ind_bar2_trm, ind_bar1, ind_bar2,  max_pat;
   complex<double> s_mva;
   VTBarra         *bar_ini, *barra;
   VTTrecho        *trecho;
   TList           *lisTRE = tramo->LisTrecho();

   //loop p/ todos patamares
   max_pat = patamares->NumPatamar();
   for (int ind_pat = 0; ind_pat < max_pat; ind_pat++)
      {//define indices das Barras do tramo
      ind_bar1_trm = 0;
      ind_bar2_trm = 1;
      //determina Barra inicial do tramo
      bar_ini = tramo->Barra(ind_bar1_trm);
      //loop p/ todos os trechos
      for (int n = 0; n < lisTRE->Count; n++)
         {
         trecho   = (VTTrecho*)lisTRE->Items[n];
         ind_bar1 = trecho->IndiceBarra(bar_ini);
         ind_bar2 = 1 - ind_bar1;
         //salva dados em da barra 1
         for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++)
            {
            trecho->resflow->If_amp[ind_bar1][ind_pat][ind_fase] = tramo->resflow->If_amp[ind_bar1_trm][ind_pat][ind_fase];
            }
         trecho->resflow->SentidoP[ind_bar1][ind_pat] = tramo->resflow->SentidoP[ind_bar1_trm][ind_pat];
         trecho->resflow->SentidoQ[ind_bar1][ind_pat] = tramo->resflow->SentidoQ[ind_bar1_trm][ind_pat];
         //salva dados em da barra 2
         for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++)
            {
            trecho->resflow->If_amp[ind_bar2][ind_pat][ind_fase] = tramo->resflow->If_amp[ind_bar2_trm][ind_pat][ind_fase];
            }
         trecho->resflow->SentidoP[ind_bar2][ind_pat] = tramo->resflow->SentidoP[ind_bar2_trm][ind_pat];
         trecho->resflow->SentidoQ[ind_bar2][ind_pat] = tramo->resflow->SentidoQ[ind_bar2_trm][ind_pat];
         //redefine Barra inicial
         bar_ini = trecho->Barra(ind_bar2);
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFluxoTramo::CalculaPerdaTrecho(VTTramo *tramo)
   {
   //variáveis locais
   double   comp_tramo_m, fatmult;
   VTTrecho *trecho;
   TList    *lisTRE = tramo->LisTrecho();

   //calcula comprimento total dos trechos do tramo
   comp_tramo_m = 0;
   for (int n = 0; n < lisTRE->Count; n++)
      {
      trecho        = (VTTrecho*)lisTRE->Items[n];
      comp_tramo_m += trecho->Comprimento_m;
      }
   //proteção
   if (IsDoubleZero(comp_tramo_m)) comp_tramo_m = 1.0;
   //loop p/ todos os trechos
   for (int n = 0; n < lisTRE->Count; n++)
      {
      trecho = (VTTrecho*)lisTRE->Items[n];
      //calcula perda no Trecho
      fatmult = trecho->Comprimento_m / comp_tramo_m;
      trecho->resflow->PerdaDiaria_mwh      = tramo->resflow->PerdaDiaria_mwh * fatmult;
      trecho->resflow->PerdaDiariaFerro_mwh = 0;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFluxoTramo::CalculaTensaoBarra(VTTramo *tramo)
   {
   //variáveis locais
   int             fase_tag;
   int             ind_bar, max_pat;
   double          comp_tramo_m, fatmult;
   complex<double> vpu_bar_ini, vpu_bar_fim, dv_pu[MAX_FASE];
   VTBarra         *tre_bar_ini, *tre_bar_fim, *tramo_bar_ini, *tramo_bar_fim, *barra;
   VTTrecho        *trecho;
   TList           *lisTRE = tramo->LisTrecho();
   TList           *lisBAR = tramo->LisBarraInterna();

   //calcula comprimento total dos trechos do tramo
   comp_tramo_m = 0;
   for (int n = 0; n < lisTRE->Count; n++)
      {
      trecho        = (VTTrecho*)lisTRE->Items[n];
      comp_tramo_m += trecho->Comprimento_m;
      }
   //proteção
   if (IsDoubleZero(comp_tramo_m)) comp_tramo_m = 1.0;
   //loop p/ todos patamares
   max_pat = patamares->NumPatamar();
   for (int ind_pat = 0; ind_pat < max_pat; ind_pat++)
      {//determina sentido do fluxo no Tramo em relação à Barra 1
      ind_bar = 0;
      if (tramo->resflow->SentidoP[ind_bar][ind_pat] == fluxoNENHUM)
         {//tensão igual em todas as barras intermediárias
         tramo_bar_ini = tramo->Barra(ind_bar);
         tramo_bar_fim = tramo->Barra(1-ind_bar);
         //loop p/ todas Barras intermediárias
         for (int nb = 0; nb < lisBAR->Count; nb++)
            {
            barra = (VTBarra*)lisBAR->Items[nb];
            //define fases da Barra intermediária
            barra->resflow->Fases = tramo_bar_ini->resflow->Fases & tramo_bar_fim->resflow->Fases;
            for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++)
               {//importante: verifica se fase existe na Barra
               fase_tag = fases->IndexToTag[ind_fase];
               if ((barra->resflow->Fases & fase_tag) == fase_tag)
                  {
                  barra->resflow->Vfn_pu[ind_pat][ind_fase] = tramo_bar_ini->resflow->Vfn_pu[ind_pat][ind_fase];
                  }
               else
                  {
                   barra->resflow->Vfn_pu[ind_pat][ind_fase] = CZero();
                  }
               }
            }
         }
      else if (tramo->resflow->SentidoP[ind_bar][ind_pat] == fluxoPOS)
         {//sentido do fluxo no Tramo é da Barra1 p/ Barra2
         ind_bar = 0;
         tramo_bar_ini = tramo->Barra(ind_bar);
         tramo_bar_fim = tramo->Barra(1-ind_bar);
         //determina queda de tensão total no tramo
         for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++)
            {//importante: verifica se a Barra fim possui a fase
            fase_tag = fases->IndexToTag[ind_fase];
            if ((tramo_bar_fim->resflow->Fases & fase_tag) == fase_tag)
               {
               dv_pu[ind_fase] = tramo_bar_ini->resflow->Vfn_pu[ind_pat][ind_fase] -
                                 tramo_bar_fim->resflow->Vfn_pu[ind_pat][ind_fase];
               }
            else
               {
               dv_pu[ind_fase] = CZero();
               }
            }
         //loop p/ todos os trechos, exceto o último(na sequencia direta)
         tre_bar_ini = tramo_bar_ini;
         for (int n = 0; n < lisTRE->Count - 1; n++)
            {
            trecho  = (VTTrecho*)lisTRE->Items[n];
            fatmult = trecho->Comprimento_m / comp_tramo_m;
            //determina Barra final do Trecho
            ind_bar     = trecho->IndiceBarra(tre_bar_ini);
            tre_bar_fim = trecho->Barra(1-ind_bar);
            //atualiza fases de bar_fim c/ fase da Barra final do Tramo
            tre_bar_fim->resflow->Fases = tramo_bar_fim->resflow->Fases;
            //calcula tensão da barra final do trecho
            for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++)
               {//verifica se a Barra final tem a fase
               fase_tag = fases->IndexToTag[ind_fase];
               if ((tramo_bar_fim->resflow->Fases & fase_tag) == fase_tag)
                  {
                  vpu_bar_ini = tre_bar_ini->resflow->Vfn_pu[ind_pat][ind_fase];
                  vpu_bar_fim = vpu_bar_ini - (dv_pu[ind_fase] * fatmult);
                  tre_bar_fim->resflow->Vfn_pu[ind_pat][ind_fase] = vpu_bar_fim;
                  }
               else
                  {//zera tensão de fase na barra final
                  tre_bar_fim->resflow->Vfn_pu[ind_pat][ind_fase] = CZero();
                  }
               }
            //redefine Barra inicial
            tre_bar_ini = tre_bar_fim;
            }
         }
      else
         {//fluxo da Barra2 p/ Barra1
         ind_bar = 1;
         tramo_bar_ini = tramo->Barra(ind_bar);
         tramo_bar_fim = tramo->Barra(1-ind_bar);
         //determina queda de tensão total no tramo
         for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++)
            {//importante: verifica se a Barra fim possui a fase
            fase_tag = fases->IndexToTag[ind_fase];
            if ((tramo_bar_fim->resflow->Fases & fase_tag) == fase_tag)
               {
               dv_pu[ind_fase] = tramo_bar_ini->resflow->Vfn_pu[ind_pat][ind_fase] -
                                 tramo_bar_fim->resflow->Vfn_pu[ind_pat][ind_fase];
               }
            else
               {
               dv_pu[ind_fase] = CZero();
               }
            }
         //loop p/ todos os trechos, exceto o último(na sequencia inversa)
         tre_bar_ini = tramo_bar_ini;
         for (int n = lisTRE->Count-1; n > 0; n--)
            {
            trecho  = (VTTrecho*)lisTRE->Items[n];
            fatmult = trecho->Comprimento_m / comp_tramo_m;
            //determina Barra final do Trecho
            ind_bar     = trecho->IndiceBarra(tre_bar_ini);
            tre_bar_fim = trecho->Barra(1-ind_bar);
            //atualiza fases de bar_fim iguais às fases da Barra final do Tramo
            tre_bar_fim->resflow->Fases = tramo_bar_fim->resflow->Fases;
            //calcula tensão da barra final do trecho
            for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++)
               {//verifica se a Barra final tem a fase
               fase_tag = fases->IndexToTag[ind_fase];
               if ((tramo_bar_fim->resflow->Fases & fase_tag) == fase_tag)
                  {
                  vpu_bar_ini = tre_bar_ini->resflow->Vfn_pu[ind_pat][ind_fase];
                  vpu_bar_fim = vpu_bar_ini - (dv_pu[ind_fase] * fatmult);
                  tre_bar_fim->resflow->Vfn_pu[ind_pat][ind_fase] = vpu_bar_fim;
                  }
               else
                  {//zera tensão de fase na barra final
                  tre_bar_fim->resflow->Vfn_pu[ind_pat][ind_fase] = CZero();
                  }
               }
            //redefine Barra inicial
            tre_bar_ini = tre_bar_fim;
            }
         }
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TFluxoTramo::Executa(VTNet *net, double Sbase)
   {
   //variáveis locais
   VTLigacao *ligacao;
   TList     *lisLIG;

   //salva parâmetros
   this->net    = net;
   this->Sbase  = Sbase;
   fases        = (VTFases*)apl->GetObject(__classid(VTFases));

   //loop p/ todos Tramos
   lisLIG = net->LisLigacaoSemChave();
   for (int n = 0; n < lisLIG->Count; n++)
      {
      ligacao = (VTLigacao*)lisLIG->Items[n];
      if (ligacao->Tipo() == eqptoTRAMO)
         {//calcula tensão nas Barras internas do Tramo
         CalculaTensaoBarra((VTTramo*)ligacao);
         //calcula fluxo nos Trechos do tramo
         CalculaFluxoTrecho((VTTramo*)ligacao);
         //calcula perdas em energia nos Trechos
         CalculaPerdaTrecho((VTTramo*)ligacao);
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
//eof