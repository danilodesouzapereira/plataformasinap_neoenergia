//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <math.h>
#include "TFluxoNo.h"
#include "..\..\Apl\VTApl.h"
#include "..\..\Complexo\Complexo.h"
#include "..\..\Ordena\VTOrdena.h"
#include "..\..\Potencia\VTDemanda.h"
#include "..\..\Patamar\VTPatamares.h"
#include "..\..\Rede\VTBarra.h"
#include "..\..\Rede\VTCapacitor.h"
#include "..\..\Rede\VTCarga.h"
#include "..\..\Rede\VTLigacao.h"
#include "..\..\Rede\VTFases.h"
#include "..\..\Rede\VTGerador.h"
#include "..\..\Rede\VTNet.h"
#include "..\..\Rede\VTNo.h"
#include "..\..\Rede\VTReator.h"
#include "..\..\Rede\VTResFlowBar.h"
#include "..\..\Rede\VTResFlowLig.h"
#include "..\..\Rede\VTResFlowSup.h"
#include "..\..\Rede\VTSuprimento.h"
#include "..\..\Rede\VTTrecho.h"
#include "..\..\Rede\VTYref.h"
#include "..\..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//fun��o global p/ cria objeto da Classe
VTFluxoNo* __fastcall NewObjFluxoNo(VTApl *apl)
   {
   return(new TFluxoNo(apl));
   }

//---------------------------------------------------------------------------
__fastcall TFluxoNo::TFluxoNo(VTApl *apl)
   {
   //salva ponteiro p/ objeto
   this->apl = apl;
   demanda   = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
   patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   //inicia dados
   ModeloCarga = cargaAUTO;
   //cria objetos
   lisBAR = new TList();
   lisLIG = new TList();
   lisPAR = new TList();
   lisEQB = new TList();
   lisTMP = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TFluxoNo::~TFluxoNo(void)
   {
   //destr�i listas sem destruir seus objetos
   if (lisBAR) {delete lisBAR; lisBAR = NULL;}
   if (lisLIG) {delete lisLIG; lisLIG = NULL;}
   if (lisPAR) {delete lisPAR; lisPAR = NULL;}
   if (lisEQB) {delete lisEQB; lisEQB = NULL;}
   if (lisTMP) {delete lisTMP; lisTMP = NULL;}
   }

//---------------------------------------------------------------------------
bool __fastcall TFluxoNo::Executa(VTNet *net, double Sbase)
   {
   //vari�veis locais
   VTNo   *pno;
   TList *lisNO;

   //salva par�metros
   this->net    = net;
   this->Sbase  = Sbase;
   //loop p/ todos Nos
   lisNO = net->LisNo();
   for (int n = 0; n < lisNO->Count; n++)
      {
      pno = (VTNo*)lisNO->Items[n];
      //verifica se o N� possui apenas uma Barra
      if (pno->lisBAR->Count == 1) continue;
      //calcula fluxo nas Liga��es internas do N�
      Executa(pno);
      }
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TFluxoNo::Executa(VTNo *pno)
   {
   //vari�veis locais
   bool    repita;
   VTBarra *pbarra;
   VTLigacao *pligacao;

   //OBS: as tens�es das Barras j� foram atualizadas em VTData
   //define mesma tens�o p/ todas Barras do No
   //TensoesBarras(pno);
   //inicia listas lisBAR e lisLIG c/ Barras e Ligacoes do No que precisam ser analisadas
   lisBAR->Clear(); CopiaTList(pno->lisBAR, lisBAR);
   lisLIG->Clear(); CopiaTList(pno->lisLIG, lisLIG);
   //reinicia dados de fluxo das Ligacoes do No
   ReiniciaDadosFlow(lisLIG);
   do {//verifica se h� uma Barra em lisBAR � qual se conecta uma �nica Ligacao de lisLIG
      while (ExisteBarraComApenasUmaLigacao(pno, &pbarra, &pligacao))
         {//retira Barra de lisBAR
         lisBAR->Remove(pbarra);
         //retira Ligacao de lisLIG
         lisLIG->Remove(pligacao);
         //monta lisTMP com:
         // a) todas Ligacoes do N� que conectam a Barra
         // b) todas Ligacoes da rede que conectam a Barra
         // c) todos os Eqbar da Barra
         lisTMP->Clear();
         SelecionaLigacoesBarra(pbarra, pno->lisLIG, lisTMP);
         SelecionaLigacoesBarra(pbarra, net->LisLigacaoSemChave(), lisTMP);
         SelecionaEqbarBarra(pbarra, pno->lisCAP,  lisTMP);
         SelecionaEqbarBarra(pbarra, pno->lisCAR,  lisTMP);
         SelecionaEqbarBarra(pbarra, pno->lisGER,  lisTMP);
         SelecionaEqbarBarra(pbarra, pno->lisSUP,  lisTMP);
         SelecionaEqbarBarra(pbarra, pno->lisZT,   lisTMP);
         //calcula Fluxo na Ligacao: somat�ria de fluxos das Liga��es que conectam a Barra
         ExecutaFluxoLigacao(pbarra, pligacao, lisTMP);
         }
      //divide fluxo entre Ligacoes paralelas ligadas em uma mesma barra
      if ((repita = ExisteBarraComLigacoesParalelas(&pbarra, lisPAR)) == true)
         {//retira Barra de lisBAR
         lisBAR->Remove(pbarra);
         //retira Ligacoes de lisLIG
         RetiraLis1DeLis2(lisPAR, lisLIG);
         //monta lisTMP com:
         // a) todas Ligacoes do N� que conectam a Barra
         // b) todas Ligacoes da rede que conectam a Barra
         // c) todos os Eqbar da Barra
         lisTMP->Clear();
         SelecionaLigacoesBarra(pbarra, pno->lisLIG, lisTMP);
         SelecionaLigacoesBarra(pbarra, net->LisLigacaoSemChave(), lisTMP);
         SelecionaEqbarBarra(pbarra, pno->lisCAP,  lisTMP);
         SelecionaEqbarBarra(pbarra, pno->lisCAR,  lisTMP);
         SelecionaEqbarBarra(pbarra, pno->lisGER,  lisTMP);
         SelecionaEqbarBarra(pbarra, pno->lisSUP,  lisTMP);
         SelecionaEqbarBarra(pbarra, pno->lisZT,   lisTMP);
         //calcula Fluxo nas Ligacoes: somat�ria de fluxos das Liga��es que conectam a Barra
         ExecutaFluxoLigacoesParalelas(pbarra, lisPAR, lisTMP);
         }
      }while(repita);
   }

//---------------------------------------------------------------------------
void __fastcall TFluxoNo::ExecutaFluxoLigacao(VTBarra *pbarra, VTLigacao *pligacao, TList *lisEQP)
   {
   //vari�veis locais
   int             ind_b1, ind_b2;
   complex<double> s_mva;
   complex<double> i_amp[MAX_FASE];
   VTEqpto         *eqpto;
   int             max_pat = patamares->NumPatamar();

   //determina �ndices das barras 1 e 2 da Ligacao
   ind_b1 = (pligacao->pbarra1 == pbarra) ? 1 : 0;
   ind_b2 = 1 - ind_b1;
   //retira Ligacao da lista lisEQP
   lisEQP->Remove(pligacao);
   //loop p/ todos patamares
   for (int npat = 0; npat < max_pat; npat++)
      {//zera vetores de correntes por fase
      for (int nf = 0; nf < MAX_FASE; nf++) {i_amp[nf] = CZero();}
      //calcula somatorio dos fluxos dos Eqptos conectados � Barra
      for (int n = 0; n < lisEQP->Count; n++)
         {
         eqpto = (VTEqpto*)lisEQP->Items[n];
         if (eqpto->TipoLigacao())
            {
            SomaCorrente(npat, i_amp, pbarra, (VTLigacao*)eqpto);
            }
         else switch(eqpto->Tipo())
            {
            case eqptoSUPRIMENTO: SomaCorrente(npat, i_amp, pbarra, (VTSuprimento*)eqpto); break;
            case eqptoGERADOR:    SomaCorrente(npat, i_amp, pbarra, (VTGerador*)eqpto);    break;
            case eqptoCARGA:      SomaCorrente(npat, i_amp, pbarra, (VTCarga*)eqpto);      break;
            case eqptoCAPACITOR:  SomaCorrente(npat, i_amp, pbarra, (VTCapacitor*)eqpto);  break;
            case eqptoREATOR:     SomaCorrente(npat, i_amp, pbarra, (VTReator*)eqpto);     break;
            case eqptoYREF:       SomaCorrente(npat, i_amp, pbarra, (VTYref*)eqpto);       break;
            default:              break;
            }
         }
      //atualiza corrente de cada fase da Ligacao no patamar
      for (int nf = 0; nf < MAX_FASE; nf++)
         {//filtra valores muito pequenos
         if (IsDoubleZero(Abs(i_amp[nf]), 1.e-3)) {i_amp[nf] = CZero();}
         //atualiza correntes da Ligacao
         pligacao->resflow->If_amp[ind_b1][npat][nf] =  i_amp[nf];
         pligacao->resflow->If_amp[ind_b2][npat][nf] = -i_amp[nf];
         }
      //determina sentido do fluxo de pot.ativa e reativa
      s_mva = pligacao->resflow->S_mva[ind_b1][npat];
      pligacao->resflow->SentidoP[ind_b1][npat] = SentidoFluxo(s_mva.real());
      pligacao->resflow->SentidoQ[ind_b1][npat] = SentidoFluxo(s_mva.imag());
      pligacao->resflow->SentidoP[ind_b2][npat] = SentidoFluxo(-s_mva.real());
      pligacao->resflow->SentidoQ[ind_b2][npat] = SentidoFluxo(-s_mva.imag());
      //define perdas nulas
      pligacao->resflow->PerdaDiaria_mwh      = 0 ;
      pligacao->resflow->PerdaDiariaFerro_mwh = 0;
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TFluxoNo::ExecutaFluxoLigacoesParalelas(VTBarra *pbarra, TList *lisPAR, TList *lisEQP)
   {
   //vari�veis locais
   int             ind_b1, ind_b2;
   complex<double> s_mva;
   complex<double> i_amp[MAX_FASE];
   VTLigacao       *pligacao;
   VTEqpto         *eqpto;
   int             max_pat = patamares->NumPatamar();

   //retira Chaves da lista lisEQP
   RetiraLis1DeLis2(lisPAR, lisEQP);
   //loop p/ todos patamares
   for (int npat = 0; npat < max_pat; npat++)
      {//zera vetores de correntes por fase
      for (int nf = 0; nf < MAX_FASE; nf++) {i_amp[nf] = CZero();}
      //calcula somatorio dos fluxos dos Eqptos conectados � Barra
      for (int n = 0; n < lisEQP->Count; n++)
         {
         eqpto = (VTEqpto*)lisEQP->Items[n];
         switch(eqpto->Tipo())
            {
            case eqptoCHAVE:      SomaCorrente(npat, i_amp, pbarra, (VTLigacao*)eqpto);    break;
            case eqptoTRAMO:      SomaCorrente(npat, i_amp, pbarra, (VTLigacao*)eqpto);    break;
            case eqptoTRECHO:     SomaCorrente(npat, i_amp, pbarra, (VTLigacao*)eqpto);    break;
            case eqptoTRAFO:      SomaCorrente(npat, i_amp, pbarra, (VTLigacao*)eqpto);    break;
            case eqptoTRAFO3E:    SomaCorrente(npat, i_amp, pbarra, (VTLigacao*)eqpto);    break;
            case eqptoSUPRIMENTO: SomaCorrente(npat, i_amp, pbarra, (VTSuprimento*)eqpto); break;
            case eqptoGERADOR:    SomaCorrente(npat, i_amp, pbarra, (VTGerador*)eqpto);    break;
            case eqptoCARGA:      SomaCorrente(npat, i_amp, pbarra, (VTCarga*)eqpto);      break;
            case eqptoCAPACITOR:  SomaCorrente(npat, i_amp, pbarra, (VTCapacitor*)eqpto);  break;
            case eqptoREATOR:     SomaCorrente(npat, i_amp, pbarra, (VTReator*)eqpto);     break;
            case eqptoYREF:       SomaCorrente(npat, i_amp, pbarra, (VTYref*)eqpto);       break;
            default:              break;
            }
         }
      //atualiza pot�ncia de cada fase da Ligacao no patamar
      for (int nc = 0; nc < lisPAR->Count; nc++)
         {
         pligacao = (VTLigacao*)lisPAR->Items[nc];
         //determina �ndices das barras 1 e 2 da Chave
         ind_b1 = (pligacao->pbarra1 == pbarra) ? 1 : 0;
         ind_b2 = 1 - ind_b1;
         for (int nf = 0; nf < MAX_FASE; nf++)
            {//filtra valores muito pequenos
            if (IsDoubleZero(Abs(i_amp[nf]), 1.e-3)) {i_amp[nf] = CZero();}
            //atualiza correntes da Ligacao
            pligacao->resflow->If_amp[ind_b1][npat][nf] =  i_amp[nf];
            pligacao->resflow->If_amp[ind_b2][npat][nf] = -i_amp[nf];
            }
         //determina sentido do fluxo de pot.ativa e reativa
         s_mva = pligacao->resflow->S_mva[ind_b1][npat];
         pligacao->resflow->SentidoP[ind_b1][npat] = SentidoFluxo(s_mva.real());
         pligacao->resflow->SentidoQ[ind_b1][npat] = SentidoFluxo(s_mva.imag());
         pligacao->resflow->SentidoP[ind_b2][npat] = SentidoFluxo(-s_mva.real());
         pligacao->resflow->SentidoQ[ind_b2][npat] = SentidoFluxo(-s_mva.imag());
         //define perdas nulas
         pligacao->resflow->PerdaDiaria_mwh      = 0 ;
         pligacao->resflow->PerdaDiariaFerro_mwh = 0;
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFluxoNo::ExisteBarraComLigacoesParalelas(VTBarra **pbarra, TList *lisDEST)
   {
   //vari�veis locais
   VTBarra *pbar;
   VTLigacao *plig;

   //loop p/ todas Barras em lisBAR
   for (int nb = 0; nb < lisBAR->Count; nb++)
      {
      pbar = (VTBarra*)lisBAR->Items[nb];
      //determina Chaves em lisLIG que conectam a Barra
      lisDEST->Clear();
      for (int nc = 0; nc < lisLIG->Count; nc++)
         {
         plig = (VTLigacao*)lisLIG->Items[nc];
         if ((plig->pbarra1 == pbar)||(plig->pbarra2 == pbar)) lisDEST->Add(plig);
         }
      if (lisDEST->Count >= 2)
         {
         *pbarra = pbar;
         return(true);
         }
      }
   return(false);
   }

//---------------------------------------------------------------------------
bool __fastcall TFluxoNo::ExisteBarraComApenasUmaLigacao(VTNo *pno, VTBarra **pbarra, VTLigacao **pligacao)
   {
   //vari�veis locais
   bool    sucesso = false;
   VTBarra *pbar;
   VTLigacao *plig;

   //inicia ponteiros recebidos
   *pbarra   = NULL;
   *pligacao = NULL;
   //loop p/ todas Barras em lisBAR
   for (int nb = 0; nb < lisBAR->Count; nb++)
      {
      pbar = (VTBarra*)lisBAR->Items[nb];
      //determina Chaves em lisLIG que conectam a Barra
      lisTMP->Clear();
      for (int nc = 0; nc < lisLIG->Count; nc++)
         {
         plig = (VTLigacao*)lisLIG->Items[nc];
         if ((plig->pbarra1 == pbar)||(plig->pbarra2 == pbar)) lisTMP->Add(plig);
         if (lisTMP->Count > 1) break;
         }
      //verifica se existe somente uma Chave que conecta a Barra
      if (lisTMP->Count == 1)
         {//encontrou uma Barra � qual se conecta uma �nica Chave
         sucesso = true;
         //verifica se j� havia encontrado uma outra Barra
         if (*pbarra == NULL)
            {//primeira Barra encontrada
            *pbarra = pbar;
            *pligacao = (VTLigacao*)lisTMP->First();
            }
         else
            {//j� havia sido encontrada uma Barra: seleciona a que tem menor n�mero de Eqbar
            if (pno->NumEqbar(pbar) < pno->NumEqbar(*pbarra))
               {//salva Barra e Chave
               *pbarra = pbar;
               *pligacao = (VTLigacao*)lisTMP->First();
               }
            }
         }
      }
   //n�o encontrou nenhuma Barra � qual se conecta uma �nica Chave
   return(sucesso);
   }

//---------------------------------------------------------------------------
void __fastcall TFluxoNo::ReiniciaDadosFlow(TList *lisLIG)
   {
   //vari�veis locais
   VTLigacao *pligacao;

   for (int nc = 0; nc < lisLIG->Count; nc++)
      {
      pligacao = (VTLigacao*)lisLIG->Items[nc];
      pligacao->resflow->Reinicia();
      }
   }
   
//---------------------------------------------------------------------------
void __fastcall TFluxoNo::SelecionaEqbarBarra(VTBarra *pbarra, TList *lisORIG, TList *lisDEST)
   {
   //vari�veis locais
   VTEqbar *eqbar;

   //determina Eqbar em lisORIG que conectam a Barra
   for (int nl = 0; nl < lisORIG->Count; nl++)
      {
      eqbar = (VTEqbar*)lisORIG->Items[nl];
      if (eqbar->pbarra == pbarra)
         {//Eqbar conecta a Barra: insere em lisDEST
         lisDEST->Add(eqbar);
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFluxoNo::SelecionaLigacoesBarra(VTBarra *pbarra, TList *lisORIG, TList *lisDEST)
   {
   //vari�veis locais
   VTLigacao *pliga;

   //determina Liga��es em lisORIG que conectam a Barra
   for (int nl = 0; nl < lisORIG->Count; nl++)
      {
      pliga = (VTLigacao*)lisORIG->Items[nl];
      //determina Barra da Liga��o � igual � Barra
      if (pliga->IndiceBarra(pbarra) >= 0)
         {//Liga��o conecta a Barra: insere em lisDEST
         lisDEST->Add(pliga);
         }
      }
   }
/*
//---------------------------------------------------------------------------
int __fastcall TFluxoNo::SentidoFluxo(double fluxo)
   {
   //vari�veis locais
   int sentido;

   //assume que n�o h� fluxo
   sentido = fluxoNENHUM;
   //converte fluxo p/ pu
   fluxo /= Sbase;
   if (fabs(fluxo) > 1e-6) sentido = (fluxo > 0) ? fluxoPOS : fluxoNEG;

   return(sentido);
   }
*/
//---------------------------------------------------------------------------
int __fastcall TFluxoNo::SentidoFluxo(double fluxo)
   {
   //vari�veis locais
   int sentido;

   //assume que n�o h� fluxo
   sentido = fluxoNENHUM;
   if (fabs(fluxo) > 1e-8) sentido = (fluxo > 0) ? fluxoPOS : fluxoNEG;

   return(sentido);
   }

//---------------------------------------------------------------------------
void __fastcall TFluxoNo::SomaCorrente(int np, complex<double>i_amp[MAX_FASE], VTBarra *pbarra, VTCapacitor *pcap)
   {
   //var�veis locais
   double          fat_zcte, vfn_pu_mod;
   complex<double> vfn_kv, sf_mva;

   //c�digo tempor�rio: divide as demandas nas 3 fases mas n�o no neutro
   sf_mva  = - demanda->S_mva[pcap][np] / 3.;
   for (int nf = 0; nf < MAX_FASE-1; nf++)
      {//determina tens�o de fase da Barra em kV
      vfn_kv     = pbarra->resflow->Vfn_kv[np][nf];
      //determina m�dulo da tens�o de fase em pu
      vfn_pu_mod = Abs(pbarra->resflow->Vfn_pu[np][nf]);
      //prote��o
      if (IsDoubleZero(vfn_pu_mod)) continue;
      //calcula corrente injetada na fase (assume modelo Zcte)
      fat_zcte   = vfn_pu_mod *vfn_pu_mod;
      i_amp[nf] += (conj((sf_mva * fat_zcte) / vfn_kv) * 1000.); //kA = >A;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFluxoNo::SomaCorrente(int np, complex<double>i_amp[MAX_FASE], VTBarra *pbarra, VTCarga *pcar)
   {
   //vari�veis locais
   double          vfn_pu_mod, fat_icte, fat_scte, fat_zcte, fmult;
   complex<double> sf_mva[MAX_FASE], vfn_kv;
   VTDemanda       *demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));

   //obt�m demandas da carga por fase
   if(! demanda->Get_S3f_mva(pcar, np, sf_mva)) return; // N�o conseguiu distribuir
   //determina parcelas de Icte, Scte e Zcte
   switch(ModeloCarga)
      {
      case cargaICTE: {fat_icte = 1.0; fat_scte = 0.0; fat_zcte = 0.0; break;}
      case cargaSCTE: {fat_icte = 0.0; fat_scte = 1.0; fat_zcte = 0.0; break;}
      case cargaZCTE: {fat_icte = 0.0; fat_scte = 0.0; fat_zcte = 1.0; break;}
      case cargaAUTO:
      default:
         fat_icte = pcar->Icte_pu;
         fat_scte = pcar->Scte_pu;
         fat_zcte = pcar->Zcte_pu;
         //prote��o
         if (! IsDoubleZero(fat_icte + fat_scte + fat_zcte - 1.0))
            {
            fat_icte = 1.0; fat_scte = 0.0; fat_zcte = 0.0;
            }
         break;
      }
   // Loop de fases
   for (int nf = 0; nf < MAX_FASE; nf++)
      {//determina tens�o de fase da Barra em kV
      vfn_kv     = pbarra->resflow->Vfn_kv[np][nf];
      //determina m�dulo da tens�o de fase em pu
      vfn_pu_mod = Abs(pbarra->resflow->Vfn_pu[np][nf]);
      //prote��o
      if (IsDoubleZero(vfn_pu_mod)) continue;
      //trata parcela Icte
      if (fat_icte > 0.)
         {
         fmult      = fat_icte * vfn_pu_mod;
         i_amp[nf] += (conj((sf_mva[nf] * fmult) / vfn_kv) * 1000.); //kA = >A;
         }
      //trata parcela Scte
      if (fat_scte > 0.)
         {
         fmult      = fat_scte;
         i_amp[nf] += (conj((sf_mva[nf] * fmult) / vfn_kv) * 1000.); //kA = >A;
         }
      //trata parcela Zcte
      if (fat_zcte > 0.)
         {
         fmult      = fat_zcte * vfn_pu_mod * vfn_pu_mod;
         i_amp[nf] += (conj((sf_mva[nf] * fmult) / vfn_kv) * 1000.); //kA = >A;
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFluxoNo::SomaCorrente(int np, complex<double>i_amp[MAX_FASE], VTBarra *pbarra, VTGerador *pger)
   {
   //loop p/ todas as fases
   for (int nf = 0; nf < MAX_FASE; nf++)
      {
      i_amp[nf] -= pger->resflow->If_amp[np][nf];
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFluxoNo::SomaCorrente(int np, complex<double>i_amp[MAX_FASE], VTBarra *pbarra, VTLigacao *pliga)
   {
   //vari�veis locais
   int ind_bar;

   //determina �ndice da Barra que conecta a Ligacao
   if ((ind_bar = pliga->IndiceBarra(pbarra)) < 0) return;
   //loop p/ todas as fases
   for (int nf = 0; nf < MAX_FASE; nf++)
      {
      i_amp[nf] += pliga->resflow->If_amp[ind_bar][np][nf];
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFluxoNo::SomaCorrente(int np, complex<double>i_amp[MAX_FASE], VTBarra *pbarra, VTReator *reator)
   {
   //var�veis locais
   double          fat_zcte, vfn_pu_mod;
   complex<double> vfn_kv, sf_mva;

   //c�digo tempor�rio: divide as demandas nas 3 fases mas n�o no neutro
   sf_mva  = demanda->S_mva[reator][np] / 3.;
   for (int nf = 0; nf < MAX_FASE-1; nf++)
      {//determina tens�o de fase da Barra em kV
      vfn_kv     = pbarra->resflow->Vfn_kv[np][nf];
      //determina m�dulo da tens�o de fase em pu
      vfn_pu_mod = Abs(pbarra->resflow->Vfn_pu[np][nf]);
      //prote��o
      if (IsDoubleZero(vfn_pu_mod)) continue;
      //calcula corrente injetada na fase (assume modelo Zcte)
      fat_zcte   = vfn_pu_mod *vfn_pu_mod;
      i_amp[nf] += (conj((sf_mva * fat_zcte) / vfn_kv) * 1000.); //kA = >A;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFluxoNo::SomaCorrente(int np, complex<double>i_amp[MAX_FASE], VTBarra *pbarra, VTSuprimento *psup)
   {
   //loop p/ todas as fases
   for (int nf = 0; nf < MAX_FASE; nf++)
      {
      i_amp[nf] -= psup->resflow->If_amp[np][nf];
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFluxoNo::SomaCorrente(int np, complex<double>i_amp[MAX_FASE], VTBarra *pbarra, VTYref *yref)
   {
   //vari�veis locais
   double          Sbase = 100.;
   complex<double> vfn_kv;
   complex<double> yf_s;

   //calcula admit�ncia por fase de Yref
   yf_s = yref->Y1_s(Sbase, pbarra->vnom) / 3.;
   //loop p/ todas as fases
   for (int nf = 0; nf < MAX_FASE-1; nf++)
      {
      vfn_kv = pbarra->resflow->Vfn_kv[np][nf];
      i_amp[nf] += ((vfn_kv * yf_s) * 1000.); //kA = >A;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFluxoNo::TensoesBarras(VTNo *pno)
   {
   //vari�veis locais
   VTBarra     *pbar;
   VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   int         max_pat = patamares->NumPatamar();

   //loop p/ todas Barras do No
   for (int nb = 0; nb < pno->lisBAR->Count; nb++)
      {
      pbar = (VTBarra*)pno->lisBAR->Items[nb];
      if (pbar == pno->pai) continue;
      //define fases da Barra
      pbar->resflow->Fases = pno->pai->resflow->Fases;
      //copia tens�es da Barra pai p/ a Barra selecionada
      for (int np = 0; np < max_pat; np++)
         {
         pbar->resflow->Van_pu[np] = pno->pai->resflow->Van_pu[np];
         pbar->resflow->Vbn_pu[np] = pno->pai->resflow->Vbn_pu[np];
         pbar->resflow->Vcn_pu[np] = pno->pai->resflow->Vcn_pu[np];
         pbar->resflow->Vnt_pu[np] = pno->pai->resflow->Vnt_pu[np];
         }
      }
   }

//---------------------------------------------------------------------------
//eof