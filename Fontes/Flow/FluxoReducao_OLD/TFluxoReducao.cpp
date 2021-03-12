//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <math.h>
#include <complex>
#include "TFluxoReducao.h"
#include "..\..\Apl\VTApl.h"
#include "..\..\Complexo\Complexo.h"
#include "..\..\Constante\Fases.h"
#include "..\..\Constante\Trifasico.h"
#include "..\..\Potencia\VTDemanda.h"
#include "..\..\Patamar\VTPatamares.h"
#include "..\..\Rede\VTBarra.h"
#include "..\..\Rede\VTCarga.h"
#include "..\..\Rede\VTCorte.h"
#include "..\..\Rede\VTGerador.h"
#include "..\..\Rede\VTLigacao.h"
#include "..\..\Rede\VTResFlowLig.h"
#include "..\..\Rede\VTSuprimento.h"
#include "..\..\Rede\VTYref.h"
#include "..\..\Rede\VTMRede.h"
#include "..\..\Rede\VTReducao.h"
#include "..\..\Rede\VTRedes.h"
#include "..\..\Rede\VTResflowBar.h"
#include "..\..\Rede\VTResflowLig.h"
#include "..\..\Rede\VTResflowSup.h"
#include "..\..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//fun��o global p/ cria objeto da Classe
VTFluxoReducao* __fastcall NewObjFluxoReducao(VTApl *apl)
   {
   return(new TFluxoReducao(apl));
   }

//---------------------------------------------------------------------------
__fastcall TFluxoReducao::TFluxoReducao(VTApl *apl)
   {
   //salva ponteiro p/ objeto
   this->apl = apl;
   //cria listas de uso interno
   lisEQP = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TFluxoReducao::~TFluxoReducao(void)
   {
   //destr�i lista sem destruir objetos
   if (lisEQP) {delete lisEQP; lisEQP = NULL;}
   }

//---------------------------------------------------------------------------
bool __fastcall TFluxoReducao::Executa(VTRedes *redes, double Sbase)
   {
   //vari�veis locais
   VTMRede   *mrede;
   VTReducao *reducao;
   TList     *lisMR;
   TList     *lisREDU;

   //salva par�metros
   this->redes  = redes;
   this->Sbase  = Sbase;
   //inicia ponteiros p/ objetos
   demanda   = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
   patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   //loop p/ todas MRedes carregadas
   lisMR = redes->LisMRede();
   for (int n = 0; n < lisMR->Count; n++)
      {
      mrede = (VTMRede*)lisMR->Items[n];
      //descarta Rede carregada
      if (mrede->Carregada) continue;
      //loop p/ todas Reducoes da Rede
      lisREDU = mrede->LisReducao();
      for (int nr = 0; nr < lisREDU->Count; nr++)
         {
         reducao = (VTReducao*)lisREDU->Items[nr];
         //calcula fluxo nas Ligacoes ficticias da Reducao
         Executa(reducao);
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TFluxoReducao::Executa(VTReducao *reducao)
   {
   //vari�veis locais
   VTLigacao *pliga_fic;
   TList     *lisLIG_FIC;

   //reinicia fluxo nas Ligacoes ficticias da Reducao
   ReiniciaFluxo(reducao);
   //obt�m lista de Ligacoes do esquem�tico da Reducao
   lisLIG_FIC = reducao->LisLigacaoEsquematico();
   //calcula fluxo em cada Ligacao fict�cia
   for (int nlf = 0; nlf < lisLIG_FIC->Count; nlf++)
      {
      pliga_fic = (VTLigacao*)lisLIG_FIC->Items[nlf];
      //calcula fluxo na Ligacao ficticia aplicando Lei de Kirchof na Barra real
      //e considerando apenas os Eqptos da Reducao
      FluxoNaLigacaoFicticia(reducao, pliga_fic);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFluxoReducao::FluxoNaLigacaoFicticia(VTReducao *reducao, VTLigacao *pliga_fic)
   {
   //vari�veis locais
   double          cte;
   int             ind_bar;
   int             max_pat;
   complex<double> s_mva, sf_kva, vfn_kv;
   VTBarra         *pbar_fic, *pbar_real;
   VTBarra         *pbarra;

   //obt�m Barra ficticia da Reducao
   if ((pbar_fic = reducao->BarraFicticia(pliga_fic)) == NULL) return;
   //determina Barra real da Ligacao ficticia
   pbar_real = (pliga_fic->pbarra1 == pbar_fic) ? pliga_fic->pbarra2 : pliga_fic->pbarra1;
   //copia resultados do fluxo da Barra real p/ a pbarra fict�cia
   pbar_fic->zona = pbar_real->zona;
   pbar_fic->resflow->CopiaAtributosDe(*(pbar_real->resflow));
   //loop p/ todos patamares
   max_pat = patamares->NumPatamar();
   for (int np = 0; np < max_pat; np++)
      {//zera somat�ria do fluxo na Barra real
      s_mva = CZero();
      //soma fluxo dos Eqbar da Reducao na Barra real
      SomaFluxoEqbarReducao(reducao->LisEqbar(), pbar_real, np, s_mva);
      //soma fluxo das Ligacoes da Reducao na Barra real
      SomaFluxoLigacao(reducao->LisLigacao(), pbar_real, np, s_mva);
      //calcula demanda por fase em KVA
      sf_kva = (1000. * s_mva) / 3.;
      for (int nb = 0; nb < NBAR_LIG-1; nb++)
         {//define valor de cte p/ alterar ou n�o o sentido do fluxo
         pbarra = pliga_fic->Barra(nb);
         (pbarra == pbar_real) ? cte = 1: cte = -1;
         //atualiza resflow
         //pliga_fic->resflow->S_mva[nb][np] = cte * s_mva;
         for (int nf = 0; nf < MAX_FASE-1; nf++)
            {//determina tens�o de fase na barra real
            vfn_kv = pbarra->resflow->Vfn_kv[np][nf];
            if (IsDoubleZero(Abs(vfn_kv)))
               {pliga_fic->resflow->If_amp[nb][np][nf] = CZero();}
            else
               {
               pliga_fic->resflow->If_amp[nb][np][nf] = conj((cte * sf_kva)/vfn_kv);
               }
            }
         }
      //redefine sentido do fluxo na Ligacao ficticia
      for (int nb = 0; nb < NBAR_LIG-1; nb++)
         {//define sentido da pot.ativa e reativa na Ligacao
         pliga_fic->resflow->SentidoP[nb][np] = SentidoFluxo(pliga_fic->resflow->P_mw[nb][np]);
         pliga_fic->resflow->SentidoQ[nb][np] = SentidoFluxo(pliga_fic->resflow->Q_mvar[nb][np]);
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFluxoReducao::ReiniciaFluxo(VTReducao *reducao)
   {
   //vari�veis locais
   VTLigacao *pliga_fic;
   TList     *lisLIG_FIC;

   //loop p/ todas Ligacoes do esquem�tico da Reducao
   lisLIG_FIC = reducao->LisLigacaoEsquematico();
   for (int n = 0; n < lisLIG_FIC->Count; n++)
      {
      pliga_fic = (VTLigacao*)lisLIG_FIC->Items[n];
      pliga_fic->resflow->Reinicia();
      }
   }
/*
//---------------------------------------------------------------------------
int __fastcall TFluxoReducao::SentidoFluxo(double fluxo)
   {
   //vari�veis locais
   int sentido;

   //assume que n�o h� fluxo
   sentido = fluxoNENHUM;
   //converte fluxo p/ pu
   fluxo /= Sbase;
   if (fabs(fluxo) > 1e-8) sentido = (fluxo > 0) ? fluxoPOS : fluxoNEG;

   return(sentido);
   }
*/

//---------------------------------------------------------------------------
int __fastcall TFluxoReducao::SentidoFluxo(double fluxo_mva)
   {
   //vari�veis locais
   int sentido;

   //assume que n�o h� fluxo
   sentido = fluxoNENHUM;
   if (! IsDoubleZero(fluxo_mva, 1e-6)) sentido = (fluxo_mva > 0) ? fluxoPOS : fluxoNEG;

   return(sentido);
   }


//---------------------------------------------------------------------------
void __fastcall TFluxoReducao::SomaFluxoEqbar(TList *lisEQP, VTBarra *pbarra, int np, complex<double> &s_mva)
   {
   //vari�veis locais
   VTEqbar      *eqbar;
   VTCarga      *pcarga;
   VTGerador    *pger;
   VTSuprimento *psup;
   VTYref       *zref;

   //loop p/ todos Eqbar em lisEQP
   for (int n = 0; n < lisEQP->Count; n++)
      {
      eqbar = (VTEqbar*)lisEQP->Items[n];
      //verifica se o Eqbar est� conectado � Barra indicada
      if (eqbar->pbarra != pbarra) continue;
      //verifica o tipo do Eqbar
      if (eqbar->Tipo() == eqptoSUPRIMENTO)
         {
         psup   = (VTSuprimento*)eqbar;
         s_mva -= psup->resflow->S_mva[np];
         }
      if (eqbar->Tipo() == eqptoGERADOR)
         {
         pger   = (VTGerador*)eqbar;
         s_mva -= pger->resflow->S_mva[np];
         }
      else if (eqbar->Tipo() == eqptoCARGA)
         {
         pcarga = (VTCarga*)eqbar;
         //verifica se a Barra possui tens�o (pode haver Barra isolada)
         if (pbarra->resflow->Vff_pu_max[np] == 0.) continue;
         s_mva += demanda->S_mva[pcarga][np];
         }
      else if (eqbar->Tipo() == eqptoYREF)
         {
         zref = (VTYref*)eqbar;
         //verifica se a Barra possui tens�o (pode haver Barra isolada)
         if (pbarra->resflow->Vfn_pu_max[np] == 0.) continue;
         s_mva += zref->DemandaMVA(np);
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFluxoReducao::SomaFluxoEqbarReducao(TList *lisEQP, VTBarra *pbarra, int np, complex<double> &s_mva)
   {
   //vari�veis locais
   double       vpu;
   VTEqbar      *eqbar;
   VTCarga      *pcarga;
   VTGerador    *pger;
   VTSuprimento *psup;
   VTYref       *yref;

   //loop p/ todos Eqbar em lisEQP
   for (int n = 0; n < lisEQP->Count; n++)
      {
      eqbar = (VTEqbar*)lisEQP->Items[n];
      //verifica se o Eqbar est� conectado � Barra indicada
      if (eqbar->pbarra != pbarra) continue;
      //verifica o tipo do Eqbar
      if (eqbar->Tipo() == eqptoSUPRIMENTO)
         {
         psup   = (VTSuprimento*)eqbar;
         s_mva -= psup->resflow->S_mva[np];
         }
      else if (eqbar->Tipo() == eqptoCARGA)
         {
         pcarga = (VTCarga*)eqbar;
         //verifica se a Barra possui tens�o (pode haver Barra isolada)
         if (IsDoubleZero((vpu = pbarra->resflow->Vff_pu_max[np]))) continue;
         s_mva += (demanda->S_mva[pcarga][np] * vpu);
         }
      else if (eqbar->Tipo() == eqptoYREF)
         {
         yref = (VTYref*)eqbar;
         //verifica se a Barra possui tens�o (pode haver Barra isolada)
         if (pbarra->resflow->Vfn_pu_max[np] == 0.) continue;
         s_mva += yref->DemandaMVA(np);
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFluxoReducao::SomaFluxoLigacao(TList *lisLIG, VTBarra *pbarra, int np, complex<double> &s_mva)
   {
   //vari�veis locais
   int       ind_bar;
   VTLigacao *pliga;

   //loop p/ todas Ligacoes em lisLIG
   for (int nl = 0; nl < lisLIG->Count; nl++)
      {
      pliga = (VTLigacao*)lisLIG->Items[nl];
      //determina �ndice da Barra na Ligacao
      if ((ind_bar = pliga->IndiceBarra(pbarra)) < 0) continue;
      s_mva += pliga->resflow->S_mva[ind_bar][np];
      }
   }

//---------------------------------------------------------------------------
//eof