//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#include <complex>
#pragma hdrstop
#include "TCurtoReducao.h"
#include "..\..\Apl\VTApl.h"
#include "..\..\Complexo\Complexo.h"
#include "..\..\Constante\Trifasico.h"
#include "..\..\Rede\VTBarra.h"
#include "..\..\Rede\VTCorte.h"
#include "..\..\Rede\VTReducao.h"
#include "..\..\Rede\VTMRede.h"
#include "..\..\Rede\VTRede.h"
#include "..\..\Rede\VTRedes.h"
#include "..\..\RedeCC\ResCurto.h"
#include "..\..\RedeCC\VTBarCC.h"
#include "..\..\RedeCC\VTLigCC.h"
#include "..\..\RedeCC\VTRedeCC.h"
#include "..\..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ cria objeto da Classe
VTCurtoReducao* __fastcall NewObjCurtoReducao(VTApl *apl)
   {
   return(new TCurtoReducao(apl));
   }

//---------------------------------------------------------------------------
__fastcall TCurtoReducao::TCurtoReducao(VTApl *apl)
   {
   //salva ponteiro p/ objeto
   this->apl = apl;
   //cria listas de uso interno
   lisMR     = new TList();
   lisEQP    = new TList();
   lisLIG    = new TList();
   lisLIG_CC = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TCurtoReducao::~TCurtoReducao(void)
   {
   //destrói lista sem destruir objetos
   if (lisMR)     {delete lisMR;     lisMR     = NULL;}
   if (lisEQP)    {delete lisEQP;    lisEQP    = NULL;}
   if (lisLIG)    {delete lisLIG;    lisLIG    = NULL;}
   if (lisLIG_CC) {delete lisLIG_CC; lisLIG_CC = NULL;}
   }
/*
//---------------------------------------------------------------------------
void __fastcall TCurtoReducao::CalculaCorrenteNaLigacaoFicticia(VTLigacao *liga_fic, TList *lisLIG)
   {
   //variáveis locais
   complex<double> i_fas, i_seq;
   VTLigCC *ligCC_FIC, *ligCC;

   //obtém ligCC da Ligacao fictícia
   if ((ligCC_FIC = ExisteLigCC_Ficticia(liga_fic)) == NULL) return;
   //obtém lista de LigCC das Ligacoes em lisLIG
   MontaLisLigCC(lisLIG, lisLIG_CC);
   //loop p/ todos defeitos
   for (int ind_def = 0; ind_def < NUM_MAX_DEF; ind_def++)
      {//calcula correntes de fase
      for (int ind_fas = 0; ind_fas < NUM_MAX_FAS; ind_fas++)
         {//soma correntes de fase das LigCC em lisLIG_CC
         i_fas = czero;
         for (int nl = 0; nl < lisLIG_CC->Count; nl++)
            {
            ligCC = (VTLigCC*)lisLIG_CC->Items[nl];
            i_fas = i_fas + ligCC->Ifas_amp[ind_def][ind_fas];
            }
         //salva corrente de fase da LigCC fictícia
         ligCC_FIC->Ifas_amp[ind_def][ind_fas] = i_fas;
         }
      for (int ind_seq = 0; ind_seq < NUM_MAX_SEQ; ind_seq++)
         {//soma correntes de sequencia das LigCC em lisLIG_CC
         i_seq = czero;
         for (int nl = 0; nl < lisLIG_CC->Count; nl++)
            {
            ligCC = (VTLigCC*)lisLIG_CC->Items[nl];
            i_seq = i_seq + ligCC->Iseq_amp[ind_def][ind_seq];
            }
         //salva corrente de sequencia da LigCC fictícia
         ligCC_FIC->Iseq_amp[ind_def][ind_seq] = i_seq;
         }
      }
   }
*/
//---------------------------------------------------------------------------
void __fastcall TCurtoReducao::CalculaCorrenteNaLigacaoFicticia(VTLigacao *liga_fic, VTBarra *barra, TList *lisLIG)
   {
   //variáveis locais
   int             ind_bar;
   double          p_mw;
   complex<double> s_mva[NUM_MAX_DEF];
   STR_FASE        i_fas[NUM_MAX_DEF][NBAR_LIG];
   STR_SEQ         i_seq[NUM_MAX_DEF][NBAR_LIG];
   VTLigCC         *ligCC_FIC, *ligCC;
   VTBarCC         *barCC;

   //obtém LigCC da Ligacao fictícia
   if ((ligCC_FIC = (VTLigCC*)liga_fic->Obj) == NULL) return;
   //obtém BarCC da Barra
   if ((barCC = (VTBarCC*)barra->Obj) == NULL) return;
   //obtém lista de LigCC das Ligacoes em lisLIG
   MontaLisLigCC(lisLIG, lisLIG_CC);
   //calcula correntes de fase por defeito
   for (int ind_def = 0; ind_def < NUM_MAX_DEF; ind_def++)
      {//inicia correntes de fase e de sequencia do defeito
      i_fas[ind_def][0].fasA = CZero();
      i_fas[ind_def][0].fasB = CZero();
      i_fas[ind_def][0].fasC = CZero();
      i_seq[ind_def][0].seq0 = CZero();
      i_seq[ind_def][0].seq1 = CZero();
      i_seq[ind_def][0].seq2 = CZero();
      //inicia potência por defeito
      s_mva[ind_def]      = CZero();
      for (int nl = 0; nl < lisLIG_CC->Count; nl++)
         {
         ligCC = (VTLigCC*)lisLIG_CC->Items[nl];
         //determina índice da Barra na Ligacao
         ind_bar = ligCC->Ligacao->IndiceBarra(barra);
         //soma correntes de fase da LigCC
         i_fas[ind_def][0].fasA += ligCC->Ifas_amp[ind_def][ind_bar][0];
         i_fas[ind_def][0].fasB += ligCC->Ifas_amp[ind_def][ind_bar][1];
         i_fas[ind_def][0].fasC += ligCC->Ifas_amp[ind_def][ind_bar][2];
         //soma correntes de sequencia da LigCC
         i_seq[ind_def][0].seq0 += ligCC->Iseq_amp[ind_def][ind_bar][0];
         i_seq[ind_def][0].seq1 += ligCC->Iseq_amp[ind_def][ind_bar][1];
         i_seq[ind_def][0].seq2 += ligCC->Iseq_amp[ind_def][ind_bar][2];
         //soma pot.curto do defeito da LigCC
         s_mva[ind_def] += ligCC->S_mva[ind_def][ind_bar];
         }
      //define correntes em relação à segunda Barra
      i_fas[ind_def][1].fasA = -i_fas[ind_def][0].fasA;
      i_fas[ind_def][1].fasB = -i_fas[ind_def][0].fasB;
      i_fas[ind_def][1].fasC = -i_fas[ind_def][0].fasC;
      i_seq[ind_def][1].seq0 = -i_seq[ind_def][0].seq0;
      i_seq[ind_def][1].seq1 = -i_seq[ind_def][0].seq1;
      i_seq[ind_def][1].seq2 = -i_seq[ind_def][0].seq2;
      }
   //atualiza correntes da LigCC_FIC
   redeCC->SalvaIccLigCC(ligCC_FIC, barCC, i_fas, i_seq);
   //redefine pot.curto de defeito da Chave
   ind_bar = liga_fic->IndiceBarra(barra);
   for (int ind_def = 0; ind_def < NUM_MAX_DEF; ind_def++)
      {
      ligCC_FIC->S_mva[ind_def][ind_bar]   = -s_mva[ind_def];
      ligCC_FIC->S_mva[ind_def][1-ind_bar] =  s_mva[ind_def];
      p_mw  = ligCC_FIC->S_mva[ind_def][0].real();
      if (Abs(ligCC_FIC->S_mva[ind_def][0]) == 0.)
         {
         ligCC_FIC->Sentido[ind_def][0] = fluxoNENHUM;
         ligCC_FIC->Sentido[ind_def][1] = fluxoNENHUM;
         }
      else if (p_mw > 0.)
         {
         ligCC_FIC->Sentido[ind_def][0] = fluxoPOS;
         ligCC_FIC->Sentido[ind_def][1] = fluxoNEG;
         }
      else if (p_mw < 0.)
         {
         ligCC_FIC->Sentido[ind_def][0] = fluxoNEG;
         ligCC_FIC->Sentido[ind_def][1] = fluxoPOS;
         }
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TCurtoReducao::Executa(void)
   {
   //variáveis locais
   VTMRede   *mrede;
   VTReducao *reducao;
   TList     *lisREDU;
   TList     *lisMR;

   //obtém ponteiros p/ Redes e RedeCC
   redes  = (VTRedes*)apl->GetObject(__classid(VTRedes));
   redeCC = (VTRedeCC*)apl->GetObject(__classid(VTRedeCC));
   //loop p/ todas Redes carregadas
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
         //calcula correntes nas Ligacoes ficticias da Reducao
         Executa(reducao);
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TCurtoReducao::Executa(VTReducao *reducao)
   {
   //variáveis locais
   VTBarra   *barra, *bar_fic;
   VTLigacao *liga_fic;
   VTLigCC   *ligCC;
   TList     *lisLIG_FIC;

   //calcula correntes na Ligacoes fictícias aplicando lei de kirchoff
   //na Barra não fictícia considerando todas LigCC exceto as da Reducao
   //e as correntes injetadas na Barra do Corte

   //obtém lista de Ligacoes do esquemático da Reducao
   lisLIG_FIC = reducao->LisLigacaoEsquematico();
   //verifica se a Redução possui geração
   if ((lisLIG_FIC->Count == 1)&&(! reducao->ExisteGeracao())) return;
   //obtém Barra ficticia da Reducao
   //bar_fic = reducao->BarraFicticia();
   //loop p/ Ligacao fictícia da Reducao
   for (int nlf = 0; nlf < lisLIG_FIC->Count; nlf++)
      {
      liga_fic = (VTLigacao*)lisLIG_FIC->Items[nlf];
      //obtém Barra ficticia da Ligacao
      if ((bar_fic = reducao->BarraFicticia(liga_fic)) == NULL) continue;
      //determina Barra real da Ligacao ficticia
      barra = (liga_fic->pbarra1 == bar_fic) ? liga_fic->pbarra2 : liga_fic->pbarra1;
      //monta lista lisLIG c/ todas Ligacoes (das Redes e Reducoes) que conectam a Barra real
      MontaLisLigacao(barra, reducao->Corte, lisLIG);
      //retira de lisLIG as Ligacoes da própria Reducao
      RetiraLis1DeLis2(reducao->LisLigacao(), lisLIG);
      //calcula corrente de curto Ligacao ficticia
      CalculaCorrenteNaLigacaoFicticia(liga_fic, barra, lisLIG);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TCurtoReducao::MontaLisLigacao(VTBarra *pbarra, VTCorte *corte, TList *lisLIG)
   {
   //variáveis locais
   VTLigacao *pliga;
   VTMRede   *mrede;
   VTReducao *reducao;

   //reinicia lisLIG
   lisLIG->Clear();
   //loop p/ todas MRede que conectam o Corte
   lisMR->Clear();
   corte->LisMRede(lisMR);
   for (int n = 0; n < lisMR->Count; n++)
      {
      mrede = (VTMRede*)lisMR->Items[n];
      //verifica se a MRede está carregada
      if (mrede->Carregada)
         {//obtém lista de Ligacoes da MRede
         lisEQP->Clear();
         mrede->LisLigacao(lisEQP);
         }
      else
         {//obtém lista de Ligacoes da Reducao da Rede
         if ((reducao = mrede->ExisteReducao(corte)) == NULL) continue;
         lisEQP->Clear();
         reducao->LisLigacao(lisEQP);
         }
      //loop p/ todas Ligacoes da Rede ou Reducao
      for (int i = 0; i < lisEQP->Count; i++)
         {
         pliga = (VTLigacao*)lisEQP->Items[i];
         //verifica se a Ligacao conecta a Barra
         if (pliga->ConectaBarra(pbarra))
            {//insere Ligacao em lisLIG
            if (lisLIG->IndexOf(pliga) < 0) lisLIG->Add(pliga);
            }
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TCurtoReducao::MontaLisLigCC(TList *lisLIG, TList *lisLIG_CC)
   {
   //variáveis locais
   VTLigacao *ligacao;
   VTLigCC   *ligCC;

   //limpa lisLIG_CC
   lisLIG_CC->Clear();
   //determina LigCC de cada Ligacao
   for (int nl = 0; nl < lisLIG->Count; nl++)
      {
      ligacao = (VTLigacao*)lisLIG->Items[nl];
      ligCC   = (VTLigCC*)ligacao->Obj;
      if (ligCC != NULL) lisLIG_CC->Add(ligCC);
      }
   }

//---------------------------------------------------------------------------
//eof