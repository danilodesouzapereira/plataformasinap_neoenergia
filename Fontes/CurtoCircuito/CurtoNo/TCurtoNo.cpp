//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#pragma hdrstop
#include "TCurtoNo.h"
#include "..\..\Apl\VTApl.h"
#include "..\..\Complexo\Complexo.h"
#include "..\..\Constante\Trifasico.h"
#include "..\..\Rede\VTBarra.h"
#include "..\..\Rede\VTLigacao.h"
#include "..\..\Rede\VTNet.h"
#include "..\..\Rede\VTNo.h"
#include "..\..\Rede\VTTrecho.h"
#include "..\..\RedeCC\ResCurto.h"
#include "..\..\RedeCC\VTBarCC.h"
#include "..\..\RedeCC\VTLigCC.h"
#include "..\..\RedeCC\VTRedeCC.h"
#include "..\..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ cria objeto da Classe
VTCurtoNo* __fastcall NewObjCurtoNo(VTApl *apl)
   {
   return(new TCurtoNo(apl));
   }

//---------------------------------------------------------------------------
__fastcall TCurtoNo::TCurtoNo(VTApl *apl)
   {
   //salva ponteiro p/ objeto
   this->apl = apl;
   //cria objetos
   lisBAR_NO = new TList();
   lisLIG_NO = new TList();
   lisTMP = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TCurtoNo::~TCurtoNo(void)
   {
   //destrói listas sem destruir seus objetos
   if (lisBAR_NO) {delete lisBAR_NO; lisBAR_NO = NULL;}
   if (lisLIG_NO) {delete lisLIG_NO; lisLIG_NO = NULL;}
   if (lisTMP) {delete lisTMP; lisTMP = NULL;}
   }

//---------------------------------------------------------------------------
bool __fastcall TCurtoNo::Executa(VTNet *net)
   {
   //variáveis locais
   VTNo   *pno;
   TList *lisNO;

   //salva ponteiro p/ objeto
   this->net = net;
   //obtém demais objetos
   redeCC = (VTRedeCC*)apl->GetObject(__classid(VTRedeCC));
   //loop p/ todos Nos
   lisNO = net->LisNo();
   for (int n = 0; n < lisNO->Count; n++)
      {//calcula fluxo nas Liacoes do Nó
      pno = (VTNo*)lisNO->Items[n];
      Executa(pno);
      }
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TCurtoNo::Executa(VTNo *pno)
   {
   //variáveis locais
   VTBarra   *pbarra;
   VTLigacao *ligacao;

   //define mesma tensão p/ todas Barras do No
   //inicia listas lisBAR_NO c/ Barras do No lisLIG_NO c/ Ligacoes do No que precisam ser analisadas
   lisBAR_NO->Clear(); CopiaTList(pno->lisBAR, lisBAR_NO);
   lisLIG_NO->Clear(); CopiaTList(pno->lisLIG, lisLIG_NO);
   //verifica se há uma Barra em lisBAR_NO à qual se conecta uma única Ligacao de lisLIG_NO
   while (ExisteBarraComApenasUmaLigacao(&pbarra, &ligacao))
      {//retira Barra de lisBAR_NO
//TESTES
//if (ligacao->Codigo.AnsiCompareIC("103") == 0)
//{int a=0;}
      lisBAR_NO->Remove(pbarra);
      //descarta se for a Barra de defeito
      if (pbarra == redeCC->BarraDefeito()) {continue;}
      //determina todas Ligacoes que conectam a Barra e retira a Ligacao interna ao Nó
      lisTMP->Clear();
      SelecionaLigacoesBarra(pbarra, net->LisLigacao(), lisTMP);
      lisTMP->Remove(ligacao);
      if (lisTMP->Count == 0) continue;
      //calcula Corrente na Ligacao: somatória de correntes das Ligações que conectam a Barra
      Executa(pbarra, ligacao, lisTMP);
      //retira Ligacao de lisLIG_NO
      //JCG 2017.07.24
      //lisLIG_NO->Remove(ligacao);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TCurtoNo::Executa(VTBarra *pbarra, VTLigacao *ligacao, TList *lisLIG)
   {
   //variáveis locais
   int             ind_bar, ind_bar_lig;
   double          p_mw, sinal;
   complex<double> s_mva[NUM_MAX_DEF];
   STR_FASE        i_fas[NUM_MAX_DEF][NBAR_LIG];
   STR_SEQ         i_seq[NUM_MAX_DEF][NBAR_LIG];
   VTLigacao       *plig;
   VTLigCC         *ligCC;
   VTBarCC         *barCC;

   //determinan BarCC da Barra
   barCC = (VTBarCC*)pbarra->Obj;
   //determina índice da Barra na Ligacao
   ind_bar_lig = ligacao->IndiceBarra(pbarra);
   //calcula correntes de fase por defeito
   for (int ind_def = 0; ind_def < NUM_MAX_DEF; ind_def++)
      {//inicia potência por defeito
      s_mva[ind_def] = CZero();
      //inicia correntes de fase e de sequencia do defeito c/ as correntes da Barra
      i_fas[ind_def][ind_bar_lig].fasA = barCC->Ifas_amp[ind_def][0];   //CZero();
      i_fas[ind_def][ind_bar_lig].fasB = barCC->Ifas_amp[ind_def][1];   //CZero();
      i_fas[ind_def][ind_bar_lig].fasC = barCC->Ifas_amp[ind_def][2];   //CZero();
      i_seq[ind_def][ind_bar_lig].seq0 = barCC->Iseq_amp[ind_def][0];   //CZero();
      i_seq[ind_def][ind_bar_lig].seq1 = barCC->Iseq_amp[ind_def][1];   //CZero();
      i_seq[ind_def][ind_bar_lig].seq2 = barCC->Iseq_amp[ind_def][2];   //CZero();
      for (int nl = 0; nl < lisLIG->Count; nl++)
         {
         plig = (VTLigacao*)lisLIG->Items[nl];
         //determina LigCC da plig
         if ((ligCC = (VTLigCC*)plig->Obj) == NULL) continue;
         //determina índice da Barra na Ligacao
         ind_bar = plig->IndiceBarra(pbarra);
         //soma correntes de fase da LigCC (com sinal trocado)
         i_fas[ind_def][ind_bar_lig].fasA -= ligCC->Ifas_amp[ind_def][ind_bar][0];
         i_fas[ind_def][ind_bar_lig].fasB -= ligCC->Ifas_amp[ind_def][ind_bar][1];
         i_fas[ind_def][ind_bar_lig].fasC -= ligCC->Ifas_amp[ind_def][ind_bar][2];
         //soma correntes de sequencia da LigCC (com sinal trocado)
         i_seq[ind_def][ind_bar_lig].seq0 -= ligCC->Iseq_amp[ind_def][ind_bar][0];
         i_seq[ind_def][ind_bar_lig].seq1 -= ligCC->Iseq_amp[ind_def][ind_bar][1];
         i_seq[ind_def][ind_bar_lig].seq2 -= ligCC->Iseq_amp[ind_def][ind_bar][2];
         //soma pot.curto do defeito da LigCC  (com sinal trocado)
         s_mva[ind_def] -= ligCC->S_mva[ind_def][ind_bar];
         }
      //converte correntes de A p/ kA
      i_fas[ind_def][ind_bar_lig].fasA /= 1000.;
      i_fas[ind_def][ind_bar_lig].fasB /= 1000.;
      i_fas[ind_def][ind_bar_lig].fasC /= 1000.;
      i_seq[ind_def][ind_bar_lig].seq0 /= 1000.;
      i_seq[ind_def][ind_bar_lig].seq1 /= 1000.;
      i_seq[ind_def][ind_bar_lig].seq2 /= 1000.;
      //define correntes em relação à segunda Barra
      ind_bar = 1 - ind_bar_lig;
      i_fas[ind_def][ind_bar].fasA = -i_fas[ind_def][ind_bar_lig].fasA;
      i_fas[ind_def][ind_bar].fasB = -i_fas[ind_def][ind_bar_lig].fasB;
      i_fas[ind_def][ind_bar].fasC = -i_fas[ind_def][ind_bar_lig].fasC;
      i_seq[ind_def][ind_bar].seq0 = -i_seq[ind_def][ind_bar_lig].seq0;
      i_seq[ind_def][ind_bar].seq1 = -i_seq[ind_def][ind_bar_lig].seq1;
      i_seq[ind_def][ind_bar].seq2 = -i_seq[ind_def][ind_bar_lig].seq2;
      //JCG 2017.07.24
      //habilita os valores em relação às duas barras
      i_fas[ind_def][ind_bar].enabled      = true;
      i_fas[ind_def][ind_bar_lig].enabled  = true;
      }
   //atualiza correntes da LigCC da ChaveLigacao
   if ((ligCC = (VTLigCC*)ligacao->Obj) == NULL) return;
   redeCC->SalvaIccLigCC(ligCC, barCC, i_fas, i_seq);
   }

//---------------------------------------------------------------------------
bool __fastcall TCurtoNo::ExisteBarraComApenasUmaLigacao(VTBarra **pbarra, VTLigacao **ligacao)
   {
   //variáveis locais
   VTBarra   *pbar;
   VTLigacao *pchv;

   //loop p/ todas Barras em lisBAR_NO
   for (int nb = 0; nb < lisBAR_NO->Count; nb++)
      {
      pbar = (VTBarra*)lisBAR_NO->Items[nb];
      //determina Ligacoes em lisLIG_NO que conectam a Barra
      lisTMP->Clear();
      for (int nc = 0; nc < lisLIG_NO->Count; nc++)
         {
         pchv = (VTLigacao*)lisLIG_NO->Items[nc];
         if ((pchv->pbarra1 == pbar)||(pchv->pbarra2 == pbar)) lisTMP->Add(pchv);
         if (lisTMP->Count > 1) break;
         }
      //verifica se existe somente uma Ligacao que conecta a Barra
      if (lisTMP->Count == 1)
         {//encontrou Barra à qual se conecta uma única Ligacao: atualiza ponteiros
         *pbarra = pbar;
         *ligacao = (VTLigacao*)lisTMP->First();
         return(true);
         }
      }
   //não encontrou nenhuma Barra à qual se conecta uma única Ligacao
   return(false);
   }

//---------------------------------------------------------------------------
void __fastcall TCurtoNo::SelecionaLigacoesBarra(VTBarra *pbarra, TList *lisORIG, TList *lisDEST)
   {
   //variáveis locais
   VTLigacao *pliga;

   //determina Ligações em lisORIG que conectam ao No da Barra
   for (int nl = 0; nl < lisORIG->Count; nl++)
      {
      pliga = (VTLigacao*)lisORIG->Items[nl];
      //determina Barra da Ligação é igual à Barra
      if (pliga->IndiceBarra(pbarra) >= 0)
         {//Ligação conecta a Barra: insere em lisDEST
         lisDEST->Add(pliga);
         }
      }
   }

//---------------------------------------------------------------------------
//eof