//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TMalha.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTLigacao.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTMalha* __fastcall NewObjMalha(void)
   {
   try{
      return(new TMalha());
      }catch(Exception &e)
         {
         return(NULL);
         }
   }
   
//-----------------------------------------------------------------------------
__fastcall TMalha::TMalha(void)
   {
   try{//cria lista p/ Ligacoes
      lisBAR = new TList();
      lisLIG = new TList();
      tmp.lisBAR = new TList();
      tmp.lisLIG = new TList();
      tmp.lisLM  = new TList();
      }catch(Exception &e)
         {
         }
   }

//-----------------------------------------------------------------------------
__fastcall TMalha::~TMalha(void)
   {
   //destrói lista sem destruir seus objetos
   if (lisBAR) {delete lisBAR; lisBAR = NULL;};
   if (lisLIG) {delete lisLIG; lisLIG = NULL;};
   if (tmp.lisBAR) {delete tmp.lisBAR; tmp.lisBAR = NULL;};
   if (tmp.lisLIG) {delete tmp.lisLIG; tmp.lisLIG = NULL;};
   if (tmp.lisLM)  {delete tmp.lisLM;  tmp.lisLM  = NULL;};
   }
   
//---------------------------------------------------------------------------
bool __fastcall TMalha::ExisteBarra(VTBarra *barra)
   {
   //verifica se a Barra existe em lisBAR
   return(lisBAR->IndexOf(barra) >= 0);
   }

//---------------------------------------------------------------------------
bool __fastcall TMalha::ExisteBarra(VTLigacao *ligacao)
   {
   //variáveis locais
   VTBarra *barra;

   //verifica se todas as Barras da Ligacao estão em lisBAR
   for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
      {
      if ((barra = ligacao->Barra(ind_bar)) == NULL) continue;
      if (! ExisteBarra(barra)) return(false);
      }
   return(true);
   }
//---------------------------------------------------------------------------
bool __fastcall TMalha::ExisteLigacao(TList *lisLIG_EXT)
   {
   //variáveis locais
   VTLigacao *ligacao;

   //verifica se alguma Ligacao de lisLIG_EXT existe em lisLIG
   for (int n = 0; n < lisLIG_EXT->Count; n++)
      {
      ligacao = (VTLigacao*)lisLIG_EXT->Items[n];
      if (lisLIG->IndexOf(ligacao) >= 0) return(true);
      }
   return(false);
   }

//---------------------------------------------------------------------------
void __fastcall TMalha::InsereLigacao(VTLigacao *ligacao)
   {
   //variáveis locais
   VTBarra   *barra;

   //verifica se a Ligacao ainda não existe em lisLIG
   if (lisLIG->IndexOf(ligacao) < 0)
      {//insere Ligacao em lisLIG
      lisLIG->Add(ligacao);
      //insere Barras da Ligacao em lisBAR
      for (int nb = 0; nb < NBAR_LIG; nb++)
         {
         if ((barra = (VTBarra*)ligacao->Barra(nb)) != NULL)
            {
            if (lisBAR->IndexOf(barra) < 0) lisBAR->Add(barra);
            }
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMalha::InsereLigacao(TList *lisLIG_EXT)
   {
   //variáveis locais
   VTLigacao *ligacao;

   //verifica se alguma Ligacao de lisLIG_EXT existe em lisLIG
   for (int n = 0; n < lisLIG_EXT->Count; n++)
      {
      ligacao = (VTLigacao*)lisLIG_EXT->Items[n];
      InsereLigacao(ligacao);
      }
   }

//---------------------------------------------------------------------------
TList* __fastcall TMalha::LisBarra(void)
   {
   return(lisBAR);
   }

//---------------------------------------------------------------------------
void __fastcall TMalha::LisBarra(TList *lisEXT)
   {
   //variáveis locais
   VTBarra *barra;

   for (int n = 0; n < lisBAR->Count; n++)
      {
      barra = (VTBarra*)lisBAR->Items[n];
      if (lisEXT->IndexOf(barra) < 0) lisEXT->Add(barra);
      }
   }

//---------------------------------------------------------------------------
TList* __fastcall TMalha::LisLigacao(void)
   {
   return(lisLIG);
   }

//---------------------------------------------------------------------------
void __fastcall TMalha::LisLigacao(TList *lisEXT)
   {
   //variáveis locais
   VTLigacao *ligacao;

   for (int n = 0; n < lisLIG->Count; n++)
      {
      ligacao = (VTLigacao*)lisLIG->Items[n];
      if (lisEXT->IndexOf(ligacao) < 0) lisEXT->Add(ligacao);
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TMalha::Monta(VTBarra *bar_ref, VTLigacao *ligacao, TList *lisLM)
   {
   //variáveis locais
   VTBarra   *barra, *bar2;
   VTLigacao *ligacao_pai, *liga;

   //reinicia listas de Barras e Ligacoes (lisBAR e lisLIG)
   lisBAR->Clear();
   lisLIG->Clear();
   //copia Ligacoes de lisLM p/ tmp.lisLM
   tmp.lisLM->Clear();
   CopiaTList(lisLM, tmp.lisLM);
   //cancela liga_pai da Ligacao
   ligacao->ligaPai = NULL;
   //ordena Barras da Ligacao
   ligacao->OrdenaBarras(barra);
   //transfere Ligacao de tmp.lisLM p/ tmp.lisLIG
   tmp.lisLM->Remove(ligacao);
   tmp.lisLIG->Add(ligacao);
   //insere Barras da Ligacao em lisBAR
   for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
      {
      if ((barra = ligacao->Barra(ind_bar)) == NULL) continue;
      lisBAR->Add(barra);
      }
   //loop p/ todas Ligacoes em tmp.lisLIG
   for (int nl = 0; nl < tmp.lisLIG->Count; nl++)
      {
      ligacao_pai = (VTLigacao*)tmp.lisLIG->Items[nl];
      //procura Ligacoes que se conectam c/ a Barra final da Ligacao pai
      for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
         {//verifica se é a Barra é nula
         if ((barra = ligacao_pai->Barra(ind_bar)) == NULL) continue;
         //IMPORTANTE: verifica se é Barra de referência
         if (barra == bar_ref) continue;
         //loop p/ todas Ligacoes em tmp.lisLM
         for (int nlig = tmp.lisLM->Count-1; nlig >= 0; nlig--)
            {
            ligacao = (VTLigacao*)tmp.lisLM->Items[nlig];
            //verifica se a Ligacao conecta a Barra sendo analisada
            if (! ligacao->ConectaBarra(barra)) continue;
            //ordena Barras da Ligacao
            ligacao->OrdenaBarras(barra);
            //define a Ligacao pai da Ligacao
            ligacao->ligaPai = ligacao_pai;
            //transfere Ligacao de tmp.lisLM p/ tmp.lisLIG
            tmp.lisLM->Delete(nlig);
            if (tmp.lisLIG->IndexOf(ligacao) < 0) tmp.lisLIG->Add(ligacao);
            //insere em tmp.lisBAR as Barras da Ligacao diferentes da Barra em análise
            for (int nb = 0; nb < NBAR_LIG; nb++)
               {
               if ((bar2 = ligacao->Barra(nb)) == NULL) continue;
               if (bar2 == barra) continue;
               //verifica se a Barra bar2 já existe em tmp.lisBAR
               if (tmp.lisBAR->IndexOf(bar2) < 0)
                  {//insere Barra bar2 em lisBAR
                  tmp.lisBAR->Add(bar2);
                  }
               //verifica se bar2 é a Barra de referência
               if (bar2 == bar_ref)
                  {//malha c/ a Barra de referência: insere Ligacao e seus pais na malha
                  do {
                     InsereLigacao(ligacao);
                     ligacao = ligacao->ligaPai;
                     }while (ligacao != NULL);
                  return(true);
                  }
               }
            }
         }
      }
   return(false);
   }

/*
//---------------------------------------------------------------------------
void __fastcall TMalha::Monta(VTLigacao *ligacao, TList *lisLM)
   {
   //variáveis locais
   int       ind_bar;
   VTBarra   *bar_ref, *bar1, *bar2;
   VTLigacao *ligacao_pai, *liga;

   //reinicia listas de Barras e Ligacoes (lisBAR e lisLIG)
   lisBAR->Clear();
   lisLIG->Clear();
   //reinicia tmp.lisBAR e tmp.lisLIG
   tmp.lisBAR->Clear();
   tmp.lisLIG->Clear();
   //copia Ligacoes de lisLM p/ tmp.lisLM
   tmp.lisLM->Clear();
   CopiaTList(lisLM, tmp.lisLM);
   //cancela liga_pai da Ligacao
   ligacao->ligaPai = NULL;
   //define Barra de referência para formação da malha
   bar_ref = ligacao->Barra(0);
   //transfer Ligacao de tmp.lisLM p/ tmp.lisLIG
   tmp.lisLM->Remove(ligacao);
   tmp.lisLIG->Add(ligacao);
   //insere Barras da Ligacao em tmp.lisBAR
   for (ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
      {
      if ((bar2 = ligacao->Barra(ind_bar)) == NULL) continue;
      if (tmp.lisBAR->IndexOf(bar2) < 0) tmp.lisBAR->Add(bar2);
      }
   //loop p/ todas Ligacoes em tmp.lisLIG
   for (int n = 0; n < tmp.lisLIG->Count; n++)
      {
      ligacao_pai = (VTLigacao*)tmp.lisLIG->Items[n];
      //procura Ligacoes que se conectam c/ a Barra final da Ligacao pai
      for (int nb = 1; nb < NBAR_LIG; nb++)
         {//verifica se é a Barra é nula
         if ((bar1 = ligacao_pai->Barra(nb)) == NULL) continue;
         //IMPORTANTE: verifica se é Barra de referência
         if (bar1 == bar_ref) continue;
         //loop p/ todas Ligacoes em tmp.lisLM
         for (int nlig = tmp.lisLM->Count-1; nlig >= 0; nlig--)
            {
            ligacao = (VTLigacao*)tmp.lisLM->Items[nlig];
            //verifica se a Ligacao conecta a Barra bar1
            if ((ind_bar = ligacao->IndiceBarra(bar1)) < 0) continue;
            //transfere Ligacao de tmp.lisLM p/ tmp.lisLIG
            tmp.lisLM->Delete(nlig);
            tmp.lisLIG->Add(ligacao);
            //se necessário, ordena as Barras da Ligacao
int PERIGO_DE_BUG_Monta=1;
            if (ind_bar != 0) ligacao->OrdenaBarras(bar1);
            //define a Ligacao pai da Ligacao
            ligacao->ligaPai = ligacao_pai;
            //insere em tmp.lisBAR as Barras da Ligacao diferentes da Barra em análise
            for (ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
               {
               if ((bar2 = ligacao->Barra(ind_bar)) == NULL) continue;
               if (bar2 == bar1) continue;
               //verifica se a Barra bar2 já existe em tmp.lisBAR
               if (tmp.lisBAR->IndexOf(bar2) < 0)
                  {//insere Barra bar2 em lisBAR
                  tmp.lisBAR->Add(bar2);
                  }
               else
                  {//existência de malha: insere Ligacao e seus pais na malha
                  liga = ligacao;
                  do {
                     InsereLigacao(liga);
                     liga = liga->ligaPai;
                     }while (liga != NULL);
                  }
               }
            }
         }
      }
   }
*/
//---------------------------------------------------------------------------
int __fastcall TMalha::NumeroLigacao(void)
   {
   return(lisLIG->Count);
   }

//---------------------------------------------------------------------------
//eof