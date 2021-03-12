//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TEsqMalha.h"
#include "VTMalha.h"
#include "VTMalhas.h"
#include "VTSecao.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCorte.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTMRede.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTReducao.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTEsqMalha
//---------------------------------------------------------------------------
VTEsqMalha* __fastcall NewObjEsqMalha(VTApl *apl)
   {
   return(new TEsqMalha(apl));
   }

//---------------------------------------------------------------------------
__fastcall TEsqMalha::TEsqMalha(VTApl *apl)
   {
   //salva ponteiro p/ objeto
   this->apl = apl;
   //cria objetos
   malhas = NewObjMalhas();
   //cria listas
   lisSECAO    = new TList();
   lisBAR      = new TList();
   lisBAR_FIXA = new TList();
   lisBAR_INI  = new TList();
   lisLIG      = new TList();
   lisREDU     = new TList();
   tmp.lisLIG  = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TEsqMalha::~TEsqMalha(void)
   {
   //destrói objetos
   if (malhas) {delete malhas; malhas = NULL;}
   //destrói listas e seus objetos
   if (lisSECAO) {LimpaTList(lisSECAO); delete lisSECAO; lisSECAO = NULL;}
   //destrói listas sem destruir seus objetos
   if (lisBAR)      {delete lisBAR;      lisBAR      = NULL;}
   if (lisBAR_FIXA) {delete lisBAR_FIXA; lisBAR_FIXA = NULL;}
   if (lisBAR_INI)  {delete lisBAR_INI;  lisBAR_INI  = NULL;}
   if (lisLIG)      {delete lisLIG;      lisLIG      = NULL;}
   if (lisREDU)     {delete lisREDU;     lisREDU     = NULL;}
   if (tmp.lisLIG)  {delete tmp.lisLIG;  tmp.lisLIG  = NULL;}
   }

//---------------------------------------------------------------------------
bool __fastcall TEsqMalha::Executa(VTRede *rede)
   {
   //variáveis locais
   VTBarra   *barra, *bar_lig;
   VTLigacao *ligacao, *ligacao_pai;

   //salva ponteiro p/ Rede
   this->rede = rede;
   //limpa listas
   lisBAR->Clear();
   lisLIG->Clear();
   LimpaTList(lisSECAO);
   //inicia lisBAR_FIXA c/ Barras de Geracao da Rede e Barras dos Cortes c/ Reducao carregada
   IniciaLisBarFixa();
   //determina malhas da Rede
   if (malhas->Executa(rede))
      {//rede com malhas: inicia lisBAR_INI c/ Barras de Geracao e Barras das Malhas
      IniciaLisBarInicial(rede, malhas->LisBarraMalha());
      //inicia tmp.lisLIG c/ as Ligacoes que não estão em malhas
      rede->LisLigacao(tmp.lisLIG);
      RetiraLis1DeLis2(malhas->LisLigacaoMalha(), tmp.lisLIG);
      }
   else
      {//rede sem malhas: inicia lisBAR_INI somente c/ Barras de Geracao
      IniciaLisBarInicial(rede, malhas->LisBarraMalha());
      //inicia lista tmp.lisLIG c/ todas as Ligacoes da Rede
      rede->LisLigacao(tmp.lisLIG);
      }
   //insere em lisLIG as Ligacoes que conectam as Barras em lisBAR_INI
   for (int nb = 0; nb < lisBAR_INI->Count; nb++)
      {
      barra = (VTBarra*)lisBAR_INI->Items[nb];
      if (lisBAR->IndexOf(barra) >= 0) continue;
      ligacao_pai = NULL;
      InsereLigacao(barra, ligacao_pai);
      //loop p/ todas as Ligacoes em lisLIG
      for (int nl = 0; (nl < lisLIG->Count)&&(tmp.lisLIG->Count > 0); nl++)
         {
         ligacao_pai = (VTLigacao*)lisLIG->Items[nl];
         for (int ind_bar = 1; ind_bar < NBAR_LIG; ind_bar++)
            {
            if ((barra = ligacao_pai->Barra(ind_bar)) == NULL) continue;
            //insere em lisLIG as Ligacoes que se conectam a Barra da Ligacao pai
            InsereLigacao(barra, ligacao_pai);
            }
         }
      }
   //monta lista de Secoes da Rede
   MontaSecaoMalha();
   //monta uma primeira vez as Secoes e destrói as Secoes criadas
   //OBS: isto é feito p/ identificar novas Barras fixas
   MontaSecaoRadial(false);
   LimpaTList(lisSECAO);
   //torna a montar as Secoes
   MontaSecaoRadial(true);
   return(true);
   }

//---------------------------------------------------------------------------
VTLigacao* __fastcall TEsqMalha::ExisteLigacaoFilhaUnica(VTLigacao *pai, TList *lisLIG)
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
VTSecao* __fastcall TEsqMalha::ExisteSecao(VTBarra *barra)
   {
   //variáveis locais
   VTSecao *secao;

   //loop p/ todas Secao
   for (int n = 0; n < lisSECAO->Count; n++)
      {
      secao = (VTSecao*)lisSECAO->Items[n];
      if (secao->ExisteBarra(barra)) return(secao);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
VTSecao* __fastcall TEsqMalha::ExisteSecao(VTLigacao *ligacao)
   {
   //variáveis locais
   VTSecao *secao;

   //loop p/ todas Secao
   for (int n = 0; n < lisSECAO->Count; n++)
      {
      secao = (VTSecao*)lisSECAO->Items[n];
      if (secao->ExisteLigacao(ligacao)) return(secao);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
void __fastcall TEsqMalha::IniciaLisBarFixa(void)
   {
   //variáveis locais
   VTBarra   *barra;
   VTCorte   *corte;
   VTReducao *redu_viz;
   TList     *lisB;

   //reinicia lisBAR_FIXA
   lisBAR_FIXA->Clear();
   //insere em lisBAR_FIXA Barra inicial da Rede (se houver)
   if ((barra = rede->BarraInicial()) != NULL) lisBAR_FIXA->Add(barra);
   //procura Barras da Rede c/ Geracao
   lisB = rede->LisBarra();
   for (int nb = 0; nb < lisB->Count; nb++)
      {
      barra =(VTBarra*)lisB->Items[nb];
      if (barra->ExisteGeracao())
         {//insere Barra em ordena.lisBAR
         lisBAR_FIXA->Add(barra);
         }
      }
   //insere em lisBAR_FIXA as Barras dos Cortes em que há Reducao vizinha carregada
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
void __fastcall TEsqMalha::IniciaLisBarInicial(VTRede *rede, TList *lisBM)
   {
   //variáveis locais
   VTBarra   *barra;
   VTCorte   *corte;
   VTReducao *redu_viz;
   TList     *lisB;

   //reinicia lisBAR_INI
   lisBAR_INI->Clear();
   //insere em lisBAR_INI Barra inicial da Rede (se houver)
   if ((barra = rede->BarraInicial()) != NULL)
      {
      lisBAR_INI->Add(barra);
      }
   //procura Barras da Rede c/ Geracao
   lisB = rede->LisBarra();
   for (int nb = 0; nb < lisB->Count; nb++)
      {
      barra =(VTBarra*)lisB->Items[nb];
      if (barra->ExisteGeracao())
         {//insere Barra em lisBAR_INI
         if (lisBAR_INI->IndexOf(barra) < 0)lisBAR_INI->Add(barra);
         }
      }
   //procura Barras dos Cortes em que há Reducao vizinha c/ geração
   lisREDU->Clear();
   rede->MRede->LisReducaoVizinha(lisREDU);
   for (int nr = 0; nr < lisREDU->Count; nr++)
      {
      redu_viz = (VTReducao*)lisREDU->Items[nr];
      //verifica se a Reducao vizinha possui geração
      if (! redu_viz->ExisteGeracao()) continue;
      //insere Barras do Corte em lisBAR_INI
      corte = redu_viz->Corte;
      corte->LisBarra(lisBAR_INI);
      }
   //verifica se não foi encontrada nenhuma Barra inicial
   if (lisBAR_INI->Count == 0)
      {//assume a primeira da Rede (OBS: isto não deveria acontecer)
      lisB = rede->LisBarra();
      if (lisB->Count > 0) lisBAR_INI->Add(lisB->First());
      }
   //inclui Barras das Malhas
   for (int nb = 0; nb < lisBM->Count; nb++)
      {
      barra =(VTBarra*)lisBM->Items[nb];
      if (lisBAR_INI->IndexOf(barra) < 0) lisBAR_INI->Add(barra);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TEsqMalha::InsereLigacao(VTBarra *barra, VTLigacao *ligacao_pai)
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
//if(ligacao->Tipo() == eqptoTRAFO)
//{int a=0;}
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
TList* __fastcall TEsqMalha::LisBarra(void)
   {
   return(lisBAR);
   }

//---------------------------------------------------------------------------
TList* __fastcall TEsqMalha::LisLigacao(void)
   {
   return(lisLIG);
   }

//---------------------------------------------------------------------------
TList* __fastcall TEsqMalha::LisSecao(void)
   {
   return(lisSECAO);
   }

//---------------------------------------------------------------------------
void __fastcall TEsqMalha::MontaSecaoMalha(void)
   {
   //variáveis locais
   VTLigacao *ligacao;
   VTSecao   *secao;
   VTMalha   *malha;
   TList     *lisLM;
   TList     *lisMALHA;

   //loop p/ todas Malhas
   lisMALHA = malhas->LisMalha();
   for (int n = 0; n < lisMALHA->Count; n++)
      {
      malha = (VTMalha*)lisMALHA->Items[n];
      //cria uma Secao p/ Malha e insere em lisSECAO
      if ((secao = NewObjSecao(secaoMALHA)) == NULL) continue;
      lisSECAO->Add(secao);
      //insere Ligacoes da Malha na Secao
      lisLM = malha->LisLigacao();
      for (int nl = 0; nl < lisLM->Count; nl++)
         {
         ligacao = (VTLigacao*)lisLM->Items[nl];
         secao->InsereLigacao(ligacao);
         }
      //monta o esquemático da Secao
      secao->Ordena(false);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TEsqMalha::MontaSecaoRadial(bool ordena)
   {
   //variáveis locais
   bool       bar_fim_fixa;
   VTLigacao *ligacao;
   VTSecao   *secao;

   //inicia tmp.lisLIG c/ as Ligacoes ordenadas que estão em lisLIG
   tmp.lisLIG->Clear();
   CopiaTList(lisLIG, tmp.lisLIG);
   //monta Secao a partir das Ligacoes em tmp.lisLIG
   while (tmp.lisLIG->Count > 0)
      {//retira última Ligacao da lista
      ligacao = (VTLigacao*)tmp.lisLIG->Last();
      tmp.lisLIG->Remove(ligacao);
      //cria uma nova Secao e insere em lisSECAO
      if ((secao = NewObjSecao(secaoRADIAL)) == NULL) continue;
      lisSECAO->Add(secao);
      //monta uma Secao
      MontaUmaSecao(secao, ligacao, tmp.lisLIG);
      }
   if (! ordena) return;
   //identifica Secao pai de cada Secao
   for (int n = 0; n < lisSECAO->Count; n++)
      {
      secao   = (VTSecao*)lisSECAO->Items[n];
      ligacao = secao->LigacaoInicial();
      secao->SecaoPai = ExisteSecao(ligacao);
      }
   //monta o esquemático de cada Secao
   for (int n = 0; n < lisSECAO->Count; n++)
      {
      secao = (VTSecao*)lisSECAO->Items[n];
      //define se a Barras final da Secao é fixa
      bar_fim_fixa = (lisBAR_FIXA->IndexOf(secao->BarraFinal()) >= 0);
      //força bar_fim_fixa p/ a primeira Secao de Rede c/ mais de uma Secao
      if ((n == 0)&&(lisSECAO->Count > 1)) bar_fim_fixa = true;
      //define se a Barra final da Secao é fixa
      secao->Ordena(bar_fim_fixa);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TEsqMalha::MontaUmaSecao(VTSecao *secao, VTLigacao *ligacao, TList *lisLIG)
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
               if (lisBAR_FIXA->IndexOf(barra) < 0)
                  {lisBAR_FIXA->Add(barra);}
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
//eof


