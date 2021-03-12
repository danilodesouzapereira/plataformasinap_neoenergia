//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TMalhas.h"
#include "VTMalha.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTCorte.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTMRede.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTReducao.h"
#include "..\..\DLL_Inc\Funcao.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//-----------------------------------------------------------------------------
VTMalhas* __fastcall NewObjMalhas(void)
   {
   return(new TMalhas());
   }

//---------------------------------------------------------------------------
__fastcall TMalhas::TMalhas(void)
   {
   //cria listas
   lisMALHA  = new TList();
   lisBM     = new TList();
   lisLM     = new TList();
   lisPAI    = new TList();
   lisLIG    = new TList();
   lisREDU   = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TMalhas::~TMalhas(void)
   {
   //destrói lista e seus objetos
   if (lisMALHA)  {LimpaTList(lisMALHA); delete lisMALHA; lisMALHA = NULL;}
   //destrói listas sem destruir seus objetos
   if (lisBM)   {delete lisBM;   lisBM   = NULL;}
   if (lisLM)   {delete lisLM;   lisLM   = NULL;}
   if (lisPAI)  {delete lisPAI;  lisPAI  = NULL;}
   if (lisLIG)  {delete lisLIG;  lisLIG  = NULL;}
   if (lisREDU) {delete lisREDU; lisREDU = NULL;}
   }

//---------------------------------------------------------------------------
//TMalhas::AnalisaPaiFilho()  -  private
//---------------------------------------------------------------------------
void __fastcall TMalhas::AnalisaPaiFilho(void)
   {
   //variáveis locais
   VTBarra   *pbar_orig, *pbar_dest;
   VTLigacao *ligacao;

   //inicia lista de Ligacoes c/ todas Ligacoes da Rede execeto as Chaves abertas
   MontaListLigacoes();
   //loop para todas as barras em lisPAI
   for (int nbar = 0; nbar < lisPAI->Count; nbar++)
      {
      pbar_orig = (VTBarra*)lisPAI->Items[nbar];
      //determina as barras filhas desta barra
      for (int nlig = lisLIG->Count-1; nlig >= 0; nlig--)
         {
         ligacao = (VTLigacao*)lisLIG->Items[nlig];
         //verifica se a Ligacao conecta a barra pai
         if (ligacao->ConectaBarra(pbar_orig))
            {//retira Ligação da lista
            lisLIG->Delete(nlig);
            //loop p/ todas Barras da Ligacao
            for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
               {
               pbar_dest = ligacao->Barra(ind_bar);
               if ((pbar_dest == NULL)||(pbar_dest == pbar_orig)) continue;
               //verifica se a barra filha já existe em lisPAI
               if (ExisteBarra(lisPAI, pbar_dest))
                  {//barra já existe em lisPAI: malha.
                  //insere as barras em lisBM
                  InsereBarraMalha(pbar_dest);
                  InsereBarraMalha(pbar_orig);
                  //InsereTrechoMalha(ptrecho);
                  }
               else
                  {//barra ainda não existe em lisPAI: insere em lisPAI
                  lisPAI->Add(pbar_dest);
                  }
               }
            }
         }
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TMalhas::Executa(VTRede *rede)
   {
   //salva ponteiro para objeto
   this->rede = rede;
   //limpa listas de Barras e Ligacoes em malha
   lisBM->Clear();
   lisLM->Clear();
   //limpa lista de Malhas
   LimpaTList(lisMALHA);
   //verifica se há malha (e identifica 2 primeira Barras da malha)
   if (! ExisteMalha()) return(false);
   //existe malha: identifica demais Barras da malha
   IdentificaBarrasMalha();
   //identifica Ligacoes da malha
   IdentificaLigacoesMalha();
   //pode haver mais de uma Malha: separa Ligacoes e Barras de
   //cada uma das malhas
   SeparaMalhas();
   return(lisMALHA->Count > 0);
   }


//---------------------------------------------------------------------------
bool __fastcall TMalhas::ExisteBarra(TList *lisBAR, VTBarra *pbarra)
   {
   //variáveis locais
   VTBarra *pbar;

   for (int nbar = 0; nbar < lisBAR->Count; nbar++)
      {
      pbar = (VTBarra*)lisBAR->Items[nbar];
      if (pbar == pbarra) return(true);
      }
   return(false);
   }

//---------------------------------------------------------------------------
bool __fastcall TMalhas::ExisteLigacao(TList *lis, VTLigacao *ligacao)
   {
   //variáveis locais
   VTLigacao *pl;

   for (int nlig = 0; nlig < lis->Count; nlig++)
      {
      pl = (VTLigacao*)lis->Items[nlig];
      if (pl == ligacao) return(true);
      }
   return(false);
   }

//---------------------------------------------------------------------------
bool __fastcall TMalhas::ExisteMalha(void)
   {
   //variáveis locais
   int num_liga;

   //inicia lisPAI c/ as Barras iniciais p/ análise
   MontaListBarraPai();
   //para determinar se existe malha, analisa a relação pai/filho entre todas as barras
   AnalisaPaiFilho();
   //se houver malhas, as barras são inseridas na lista lisBM
   return(lisBM->Count > 0);
   }


//---------------------------------------------------------------------------
VTMalha* __fastcall TMalhas::ExisteMalha(VTLigacao *ligacao)
   {
   //variáveis locais
   VTMalha *malha;

   //procura Malha que tenha as duas Barras da Ligacao
   for (int n = 0; n < lisMALHA->Count; n++)
      {
      malha = (VTMalha*)lisMALHA->Items[n];
      if (malha->ExisteBarra(ligacao)) return(malha);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
//TMalhas::IdentificaBarrasMalha()  -  private
//
// Identifica todas as Barras pertencentes a Malha. Para isto,
// faz a análise da relação Pai/Filho a partir de cada Barra previamente
// identificada na Malha. O método ExisteMalha() inicia a lista lisBM com
// as 2 primeiras barras identificadas na malha
//---------------------------------------------------------------------------
void __fastcall TMalhas::IdentificaBarrasMalha(void)
   {
   //variáveis locais
   VTBarra  *pbarra;

   //loop para todas as barras de lisBM (barras que estão na malha)
   for (int nbar = 0; nbar < lisBM->Count; nbar++)
      {
      pbarra = (VTBarra*)lisBM->Items[nbar];
      //insere a barra em lisPAI
      lisPAI->Clear();
      lisPAI->Add(pbarra);
      //para determinar novas barras e trechos presentes na malha,
      //analisa a relação pai/filho entre todas as barras, a partir da barra
      //inserida em lisPAI
      AnalisaPaiFilho();
      }
   }

//---------------------------------------------------------------------------
//TMalhas::IdentificaLigacoesMalha()  -  private
//
// Identifica todos as Ligacoes pertencentes às malhas
// O método IdentificaBarrasMalha() já preencheu lisBM c/ todas as
// Barras em malhas
//---------------------------------------------------------------------------
void __fastcall TMalhas::IdentificaLigacoesMalha(void)
   {
   //variáveis locais
   VTBarra   *pbarra, *pbar;
   VTLigacao *ligacao;

   //monta lista de Ligacoes
   MontaListLigacoes();
   //loop para todas as barras de lisBM (barras que estão na malha)
   for (int nbar = 0; nbar < lisBM->Count; nbar++)
      {
      pbarra = (VTBarra*)lisBM->Items[nbar];
      //determina Ligacoes que se conectam a esta Barra
      for (int nlig = 0; nlig < lisLIG->Count; nlig++)
         {
         ligacao = (VTLigacao*)lisLIG->Items[nlig];
         //verifica se a Ligaçao conecta a Barra
         if (! ligacao->ConectaBarra(pbarra)) continue;
         for (int ind_bar = 0; ind_bar <NBAR_LIG; ind_bar++)
            {
            pbar = ligacao->Barra(ind_bar);
            if ((pbar == NULL)||(pbar == pbarra)) continue;
            //verifica se pbar está na lista de Barras em malha
            if (lisBM->IndexOf(pbar) >= 0)
               {//as duas Barras da Ligação estão em malha: inclui Ligação na lista liLM
               InsereLigacaoMalha(ligacao);
               }
            }
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMalhas::InsereBarraMalha(VTBarra *pbarra)
   {
   //verifica se a barra já existe em lisBM
   if (ExisteBarra(lisBM, pbarra)) return;
   //insere barra em lisBM
   lisBM->Add(pbarra);
   }

//---------------------------------------------------------------------------
void __fastcall TMalhas::InsereLigacaoMalha(VTLigacao *ligacao)
   {
   //verifica se o trecho já existe em lisTM
   if (ExisteLigacao(lisLM, ligacao)) return;
   //insere trecho em lisTM
   lisLM->Add(ligacao);
   }

//-----------------------------------------------------------------------------
TList* __fastcall TMalhas::LisBarraMalha(void)
   {
   //variáveis locais
   VTMalha *malha;

   //reinicia lisBM
   lisBM->Clear();
   //insere em lisBM as Barras de todas as Malhas
   for (int n = 0; n < lisMALHA->Count; n++)
      {
      malha = (VTMalha*)lisMALHA->Items[n];
      malha->LisBarra(lisBM);
      }
   return(lisBM);
   }

//-----------------------------------------------------------------------------
TList* __fastcall TMalhas::LisLigacaoMalha(void)
   {
   //variáveis locais
   VTMalha *malha;

   //reinicia lisLM
   lisLM->Clear();
   //insere em lisLM as Ligacoes de todas as Malhas
   for (int n = 0; n < lisMALHA->Count; n++)
      {
      malha = (VTMalha*)lisMALHA->Items[n];
      malha->LisLigacao(lisLM);
      }
   return(lisLM);
   }
   
//-----------------------------------------------------------------------------
TList* __fastcall TMalhas::LisMalha(void)
   {
   return(lisMALHA);
   }

//---------------------------------------------------------------------------
void __fastcall TMalhas::MontaListBarraPai(void)
   {
   //variáveis locais
   VTBarra   *barra;
   VTCorte   *corte;
   VTReducao *redu_viz;
   TList     *lisBAR = rede->LisBarra();

   //reinicia lisPAI
   lisPAI->Clear();
   //obtém lista de Reducoes vizinhas da Rede
   lisREDU->Clear();
   rede->MRede->LisReducaoVizinha(lisREDU);
   for (int nr = 0; nr < lisREDU->Count; nr++)
      {
      redu_viz = (VTReducao*)lisREDU->Items[nr];
      //verifica se a Reducao vizinha possui geração
      if (! redu_viz->ExisteGeracao()) continue;
      //insere Barras do Corte em lisPAI
      corte = redu_viz->Corte;
      corte->LisBarra(lisPAI);
      }
   //verifica se não foi encontrado nenhuma Barra inicial relativa aos Corte
   if (lisPAI->Count > 0) return;
   //procura Barra da Rede c/ Geracao
   for (int nb = 0; nb < lisBAR->Count; nb++)
      {
      barra =(VTBarra*)lisBAR->Items[nb];
      if (barra->ExisteGeracao())
         {//insere Barra em ordena.lisBAR
         lisPAI->Add(barra);
         return;
         }
      }
   //não foi encontrada nenhuma Barra inicial: assume a primeira da Rede
   if (lisBAR->Count > 0) lisPAI->Add(lisBAR->First());
   }

//---------------------------------------------------------------------------
void __fastcall TMalhas::MontaListLigacoes(void)
   {
   //variáveis locais
   VTChave   *chave;
   VTLigacao *ligacao;

   //copia lista de Ligacoes da Rede em lisLIGI
   lisLIG->Clear();
   rede->LisLigacao(lisLIG);
   //loop p/ todas Ligacoes em lisLIG
   for (int nl = lisLIG->Count-1; nl >= 0; nl--)
      {
      ligacao = (VTLigacao*)lisLIG->Items[nl];
      //cancela ponteiro p/ Ligacao pai
      ligacao->ligaPai = NULL;
      //retira Chave aberta da lista
      if (ligacao->Tipo() == eqptoCHAVE)
         {//verifica se Chave aberta
         chave = (VTChave*)ligacao;
         if (chave->Aberta)
            {//retira Chave de lisLIG
            lisLIG->Delete(nl);
            }
         }
      }
   }

//---------------------------------------------------------------------------
//TMalhas::SeparaMalhas()  -  private
//
//Os métodos ExisteMalha() e IdentificaMalha() separam todas as Ligações e
//Barras que estão em malha nas listas lisLM e lisBM.
//Como uma Rede pode ter várias malhas distintas, este método cria as
//várias Malhas considerando apenas as Ligações e Barras já identificadas nas malhas
//---------------------------------------------------------------------------
void __fastcall TMalhas::SeparaMalhas(void)
   {
   //variáveis locais
   VTBarra   *barra;
   VTLigacao *ligacao;
   VTMalha   *malha, *malha_i, *malha_j;

   //loop p/ todas Barras em lisBM
   for (int nb = 0; nb < lisBM->Count; nb++)
      {
      barra = (VTBarra*)lisBM->Items[nb];
      //loop p/ todas as Ligações em lisLM
      for (int nl = 0; nl < lisLM->Count; nl++)
         {
         ligacao = (VTLigacao*)lisLM->Items[nl];
         //verifica se a Ligacao conecta a Barra
         if (! ligacao->ConectaBarra(barra)) continue;
         //monta Malha a partir da Barra/Ligacao
         if ((malha = NewObjMalha()) == NULL) continue;
         //monta a Malha
         malha->Monta(barra, ligacao, lisLM);
         //verifica se a Malha possui mais de 1 Ligacao
         if (malha->NumeroLigacao() > 1)
            {//insere Malha em lisMALHA
            lisMALHA->Add(malha);
            }
         else
            {//destrói Malha
            delete malha;
            }
         }
      }
   //agrega Malhas c/ Ligacoes comuns
   for (int i = lisMALHA->Count-1; i >= 1; i--)
      {//determina Malha de índice i
      malha_i = (VTMalha*)lisMALHA->Items[i];
      for (int j = i-1; j >= 0; j--)
         {//determina Malha de índice j
         malha_j = (VTMalha*)lisMALHA->Items[j];
         //verifica se existem Ligacoes da Malha i na Malha j
         if (! malha_j->ExisteLigacao(malha_i->LisLigacao())) continue;
         //funde a Malha i na Malha j
         malha_j->InsereLigacao(malha_i->LisLigacao());
         //retira Malha i de lisMALHA e destrói objeto
         lisMALHA->Delete(i);
         delete malha_i;
         break;
         }
      }
   }

//---------------------------------------------------------------------------
//eof