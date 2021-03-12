//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TEsqRadial.h"
#include "VTSecao.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCorte.h"
#include "..\Rede\VTEqbar.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTMNet.h"
#include "..\Rede\VTMRede.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTReducao.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTEsqRadial
//---------------------------------------------------------------------------
VTEsqRadial* __fastcall NewObjEsqRadial(VTApl *apl)
   {
   return(new TEsqRadial(apl));
   }

//---------------------------------------------------------------------------
__fastcall TEsqRadial::TEsqRadial(VTApl *apl)
   {
   //salva ponteiro p/ objeto
   this->apl = apl;
   //cria listas
   lisSECAO    = new TList();
   lisBAR      = new TList();
   lisBAR_FIXA = new TList();
   lisLIG      = new TList();
   lisREDU     = new TList();
   tmp.lisEQP  = new TList();
   tmp.lisLIG  = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TEsqRadial::~TEsqRadial(void)
   {
   //destrói listas e seus objetos
   if (lisSECAO) {LimpaTList(lisSECAO); delete lisSECAO; lisSECAO = NULL;}
   //destrói listas sem destruir seus objetos
   if (lisBAR)      {delete lisBAR;      lisBAR      = NULL;}
   if (lisBAR_FIXA) {delete lisBAR_FIXA; lisBAR_FIXA = NULL;}
   if (lisLIG)      {delete lisLIG;      lisLIG      = NULL;}
   if (lisREDU)     {delete lisREDU;     lisREDU     = NULL;}
   if (tmp.lisEQP)  {delete tmp.lisEQP;  tmp.lisEQP  = NULL;}
   if (tmp.lisLIG)  {delete tmp.lisLIG;  tmp.lisLIG  = NULL;}
   }

//---------------------------------------------------------------------------
bool __fastcall TEsqRadial::Executa(VTRede *rede, bool ordena)
   {
   //variáveis locais
   VTBarra   *barra, *bar_lig;
   VTCorte   *corte;
   VTLigacao *ligacao, *ligacao_pai;

   //salva ponteiro p/ Rede
   this->rede = rede;
   //limpa listas lisBAR e lisLIG
   lisBAR->Clear();
   lisLIG->Clear();
   //reinicia lista lisSECAO
   LimpaTList(lisSECAO);
   //proteção: verifica se a Rede é radial
//   if (! rede->Radial) return(false);
   //inicia diagnóstico
   diagnostico.radial = true;
   //inicia lisBAR_FIXA c/ a Barra inicial da Rede e Barras dos Cortes c/ Reducao carregada
//   IniciaLisBarFixa();
   //verifica se a Rede possui geração
   if ((barra = ExisteBarraGeracao(rede)) != NULL)
      {//inicia lisBAR c/ a Barra de geração
      lisBAR->Add(barra);
      }
   //determina um Corte de suprimento para a Rede
   else if ((corte = ExisteCorteDeSuprimento(rede)) != NULL)
      {//inicia lisBAR c/ as Barras do Corte de suprimento
      corte->LisBarra(lisBAR);
      }
   else
      {//inicia lisBAR c/ uma Barra qualquer
      lisBAR->Add((rede->LisBarra())->First());
      }
   //inicia lista tmp.lisLIG c/ todas as Ligacoes da Rede
   tmp.lisLIG->Clear();
   rede->LisLigacao(tmp.lisLIG);
   //inicia lisLIG as Ligacoes que conectam as Barras em lisBAR
   ligacao_pai = NULL;
   for (int nb = lisBAR->Count-1; nb >= 0; nb--) //IMPORTANTE a ordem inversa
       {
       barra = (VTBarra*)lisBAR->Items[nb];
       //insere em lisLIG as Ligacoes que se conectam a Barra da Ligacao pai
       InsereLigacao(barra, ligacao_pai);
       }
   //loop p/ todas as Ligacoes em lisLIG
   for (int nl = 0; nl < lisLIG->Count; nl++)
      {
      ligacao_pai = (VTLigacao*)lisLIG->Items[nl];
      for (int ind_bar = 1; ind_bar < NBAR_LIG; ind_bar++)
         {
         if ((barra = ligacao_pai->Barra(ind_bar)) == NULL) continue;
         //insere em lisLIG as Ligacoes que se conectam a Barra da Ligacao pai
         InsereLigacao(barra, ligacao_pai);
         }
      }
   //monta lista de Secoes da Rede somente se ela for radial
   if (diagnostico.radial)
      {//copia as Ligacoes ordenadas de lisLIG p/ tmp.lisLIG
      //tmp.lisLIG->Clear();
      //CopiaTList(lisLIG, tmp.lisLIG);
      //monta uma primeira vez as Secoes e destrói as Secoes criadas
      //OBS: isto é feito p/ identificar novas Barras fixas
      //MontaSecaoRadial(false);
      //LimpaTList(lisSECAO);
      //reincia lisLIG c/ as Ligacoes ordenadas salvas em tmp.lisLIG
      //lisLIG->Clear();
      //CopiaTList(tmp.lisLIG, lisLIG);
      //monta Secoes sem alterar coordenadas das Ligacoes
      MontaSecaoRadial(ordena);
      }
   return(diagnostico.radial);
   }

//---------------------------------------------------------------------------
VTBarra* __fastcall TEsqRadial::ExisteBarraGeracao(VTRede *rede)
   {
   //variáveis locais
   VTEqbar *eqbar;

   //obtém lista de Geradores da Rede
   tmp.lisEQP->Clear();
   rede->LisEqbar(tmp.lisEQP, eqptoGERADOR);
   //se não há Gerador, tenta Suprimento
   if (tmp.lisEQP->Count == 0) rede->LisEqbar(tmp.lisEQP, eqptoSUPRIMENTO);
   //verifica se encontrou Gerador/Suprimento
   if (tmp.lisEQP->Count > 0)
      {
      eqbar = (VTEqbar*)(tmp.lisEQP->First());
      return(eqbar->pbarra);
      }
   return(NULL);
   }
/*
//---------------------------------------------------------------------------
VTCorte* __fastcall TEsqRadial::ExisteCorteDeSuprimento(VTRede *rede)
   {
   //variáveis locais
   VTCorte   *corte;
   VTReducao *reducao;

   //obtém lista de MRedes vizinhas
   tmp.lisEQP->Clear();
   rede->MRede->MNet->LisReducaoVizinha(tmp.lisEQP);
   //verifica se alguma Reducao vizinha possui geração
   for (int nr = 0; nr < tmp.lisEQP->Count; nr++)
      {
      reducao = (VTReducao*)tmp.lisEQP->Items[nr];
      if (reducao->ExisteGeracao())
         {//retorna Corte da Reducao
         return(reducao->Corte);
         }
      }
   return(NULL);
   }
*/
//---------------------------------------------------------------------------
VTCorte* __fastcall TEsqRadial::ExisteCorteDeSuprimento(VTRede *rede)
   {
   //variáveis locais
   VTCorte   *corte;
   VTReducao *reducao;
   TList     *lisCORTE;

   //verifica se a MRede da Redepossui uma MRede pai
   if (rede->MRede->Pai)
      {//determina Corte comum entre a MRede e MRede pai
      corte = rede->MRede->ExisteCorte(rede->MRede->Pai);
      //verifica se a Rede conecta o Corte
      lisCORTE = rede->LisCorte();
      if (lisCORTE->IndexOf(corte) >= 0) return(corte);
      }
   //obtém lista de MRedes vizinhas
   tmp.lisEQP->Clear();
   rede->MRede->MNet->LisReducaoVizinha(tmp.lisEQP);
   //verifica se alguma Reducao vizinha possui geração
   for (int nr = 0; nr < tmp.lisEQP->Count; nr++)
      {
      reducao = (VTReducao*)tmp.lisEQP->Items[nr];
      if (reducao->ExisteGeracao())
         {//retorna Corte da Reducao
         return(reducao->Corte);
         }
      }
   //retorna primeiro Corte
   lisCORTE = rede->LisCorte();
   corte = (lisCORTE->Count > 0) ? (VTCorte*)lisCORTE->First() : NULL;
   return(corte);
   }

//---------------------------------------------------------------------------
VTLigacao* __fastcall TEsqRadial::ExisteLigacaoFilhaUnica(VTLigacao *pai, TList *lisLIG)
   {
   //variáveis locais
   VTLigacao *filha = NULL;
   VTLigacao *ligacao;

   //procura Ligacao cuja Ligacao pai coincide c/ o pai indicado
   for (int nl = 0; nl < lisLIG->Count; nl++)
      {
      ligacao = (VTLigacao*)lisLIG->Items[nl];
      if (ligacao->ligaPai != pai) continue;
      //verifica se já encontrou uma Ligacao filha
      if (filha != NULL) return(NULL);
      //salva primeira Ligacao filha encontrada
      filha = ligacao;
      }
   return(filha);
   }

//---------------------------------------------------------------------------
VTSecao* __fastcall TEsqRadial::ExisteSecao(VTBarra *barra)
   {
   //variáveis locais
   VTSecao *secao;

   //proteção
   if (barra == NULL) return(NULL);
   //loop p/ todas Secao
   for (int n = 0; n < lisSECAO->Count; n++)
      {
      secao = (VTSecao*)lisSECAO->Items[n];
      if (secao->ExisteBarra(barra)) return(secao);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
VTSecao* __fastcall TEsqRadial::ExisteSecao(VTLigacao *ligacao)
   {
   //variáveis locais
   VTSecao *secao;

   //proteção
   if (ligacao == NULL) return(NULL);
   //loop p/ todas Secao
   for (int n = 0; n < lisSECAO->Count; n++)
      {
      secao = (VTSecao*)lisSECAO->Items[n];
      if (secao->ExisteLigacao(ligacao)) return(secao);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
void __fastcall TEsqRadial::IniciaLisBarFixa(void)
   {
   //variáveis locais
   VTBarra   *barra;
   VTCorte   *corte;
   VTReducao *redu_viz;

   //reinicia lisBAR_FIXA
   lisBAR_FIXA->Clear();
   //insere em lisBAR_FIXA Barra inicial da Rede
   if ((barra = rede->BarraInicial()) != NULL) lisBAR_FIXA->Add(barra);
   //insere em lisBAR_FIXA as Barras dos Cortes c/ Reducao vizinha carregada
   lisREDU->Clear();
   rede->MRede->LisReducaoVizinha(lisREDU);
   for (int nr = 0; nr < lisREDU->Count; nr++)
      {
      redu_viz = (VTReducao*)lisREDU->Items[nr];
/*
//Código bloqueado p/ considerar fixas todas Barras de Corte
      //verifica se Reducao carregada
//      if (! redu_viz->Carregada()) continue;
*/
      //insere Barras do Corte em lisBAR_FIXA
      corte = redu_viz->Corte;
      corte->LisBarra(lisBAR_FIXA);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TEsqRadial::InsereLigacao(VTBarra *barra, VTLigacao *ligacao_pai)
   {
   //variáveis locais
   int       ind_bar;
   VTBarra   *bar_lig;
   VTLigacao *ligacao;

   //procura Ligacoes em tmp.lisLIG que conectam a Barra
   for (int nl = tmp.lisLIG->Count - 1; nl >= 0; nl--)
      {
      ligacao = (VTLigacao*)tmp.lisLIG->Items[nl];
      //verifica se a Ligacao conecta a Barra
      if ((ind_bar = ligacao->IndiceBarra(barra)) < 0) continue;
      //se necessário, ordena as Barras da Ligacao
int PERIGO_DE_BUG_InsereLigacao=1;
      if (ind_bar != 0) ligacao->OrdenaBarras(barra);
      //transfere a Ligacao de tmp.lisLIG p/ lisLIG
      tmp.lisLIG->Delete(nl);
      lisLIG->Add(ligacao);
      //define a Ligacao pai da Ligacao
      ligacao->ligaPai = ligacao_pai;
      //insere em lisBAR as Barras da Ligacao diferentes da Barra em análise
      for (ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
         {
         if ((bar_lig = ligacao->Barra(ind_bar)) == NULL) continue;
         if (bar_lig == barra) continue;
         //verifica se a Barra da Ligacao já existe em lisBAR (malha)
         if (lisBAR->IndexOf(bar_lig) < 0)
            {//insere a outra Barra em lisBAR
            lisBAR->Add(bar_lig);
            }
         else
            {//existência de malha
            diagnostico.radial = false;
            }
         }
      }
   }

//---------------------------------------------------------------------------
TList* __fastcall TEsqRadial::LisBarra(void)
   {
   return(lisBAR);
   }

//---------------------------------------------------------------------------
TList* __fastcall TEsqRadial::LisLigacao(void)
   {
   return(lisLIG);
   }

//---------------------------------------------------------------------------
TList* __fastcall TEsqRadial::LisSecao(void)
   {
   return(lisSECAO);
   }

//---------------------------------------------------------------------------
void __fastcall TEsqRadial::MontaSecaoRadial(bool ordena)
   {
   //variáveis locais
   bool       bar_fim_fixa;
   VTLigacao *ligacao;
   VTSecao   *secao, *secao_pai;

   //monta Secoes a partir das Ligacoes ordenadas em lisLIG
   while (lisLIG->Count > 0)
      {
      ligacao = (VTLigacao*)lisLIG->Last();
      lisLIG->Remove(ligacao);
      lisSECAO->Add(secao = NewObjSecao(secaoRADIAL));
      //monta uma Secao
      MontaUmaSecao(secao, ligacao, lisLIG);
      }
   //if (! ordena) return;
   /*
   //OBS: as Secoes pai já está identificadas
   //identifica Secao pai de cada Secao
   for (int n = 0; n < lisSECAO->Count; n++)
      {
      secao     = (VTSecao*)lisSECAO->Items[n];
      ligacao   = secao->LigacaoInicial();
      secao_pai = ExisteSecao(ligacao->ligaPai);
      secao->SecaoPai = (secao == secao_pai) ? NULL : secao_pai;
      }
   */
   if (! ordena) return;
   //monta o esquemático de cada Secao
   for (int n = 0; n < lisSECAO->Count; n++)
      {
      secao = (VTSecao*)lisSECAO->Items[n];
      //define se a Barras final da Secao é fixa
      bar_fim_fixa = (lisBAR_FIXA->IndexOf(secao->BarraFinal()) >= 0);
      //força bar_fim_fixa p/ a primeira Secao de Rede c/ mais de uma Secao
      if ((n == 0)&&(lisSECAO->Count > 1)&&(lisBAR_FIXA->Count == 0)) bar_fim_fixa = true;
      //define se a Barra final da Secao é fixa
      //secao->Ordena(bar_fim_fixa);
      secao->Ordena(false);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TEsqRadial::MontaUmaSecao(VTSecao *secao, VTLigacao *ligacao, TList *lisLIG)
   {
   //variáveis locais
   VTBarra   *barra;
   VTLigacao *pai;

   //insere Ligacao filha na Secao
   secao->InsereLigacao(ligacao);
   //verifica se a Barra inicial da Ligacao é uma das Barras fixas
   if (lisBAR_FIXA->IndexOf(ligacao->Barra(0)) < 0)
      {//determina as demais Ligacoes pai que compõem a Secao
      while ((pai = ligacao->ligaPai) != NULL)
         {//retira Ligacao pai de lisLIG
         if (lisLIG->Remove(pai) < 0)
            {//Ligacao não está em lisLIG porque pertence a outra Secao (Secao pai)
            secao->SecaoPai = ExisteSecao(pai);
            //verifica se a Barra final da Secao sendo montada é uma Barra fixa
            if (lisBAR_FIXA->IndexOf(secao->BarraFinal()) >= 0)
               {//inclui Barra inicial da Secao na lista de Barras fixas
               barra = secao->BarraInicial();
               if (lisBAR_FIXA->IndexOf(barra) < 0) lisBAR_FIXA->Add(barra);
               }
            break;
            }
         //insere Ligacao pai na Secao
         secao->InsereLigacao(pai);
         //redefine Ligacao p/ buscar próxima Ligacao pai
         ligacao = pai;
         //verifica se a Barra inicial da Ligacao é uma das Barras fixas
         if (lisBAR_FIXA->IndexOf(ligacao->Barra(0)) >= 0) break;
         }
      }
   //define sentido da Secao
   secao->DefineSentido();
   }

//---------------------------------------------------------------------------
VTSecao* __fastcall TEsqRadial::SecaoFinal(void)
   {
   //proteção
   if (lisSECAO->Count == 0) return(NULL);
   return((VTSecao*)lisSECAO->Last());
   }

//---------------------------------------------------------------------------
VTSecao* __fastcall TEsqRadial::SecaoInicial(void)
   {
   //proteção
   if (lisSECAO->Count == 0) return(NULL);
   return((VTSecao*)lisSECAO->First());
   }

//---------------------------------------------------------------------------
//eof


