//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TTree.h"
#include "VTNode.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTLigacao.h"
#include <DLL_Inc\Funcao.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe
VTTree* __fastcall NewObjTree(void)
   {
   try{
      return(new TTree());
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TTree::TTree(void)
   {
   try{//cria listas
      lisBAR    = new TList();
      lisLIG    = new TList();
      lisNODE   = new TList();
      lisFILTRO = new TList();
      }catch(Exception &e)
         {
         }
   }

//---------------------------------------------------------------------------
__fastcall TTree::~TTree(void)
   {
   //destrói lista SEM destruir seus objetos
   if (lisBAR)    {delete lisBAR;    lisBAR     = NULL;}
   if (lisLIG)    {delete lisLIG;    lisLIG     = NULL;}
   if (lisFILTRO) {delete lisFILTRO; lisFILTRO  = NULL;}
   //destrói lista e seus objetos
   if (lisNODE) {LimpaTList(lisNODE); delete lisNODE; lisNODE = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TTree::Add(VTBarra *barra)
   {
   Add(barra, NULL, NULL);
   }

//---------------------------------------------------------------------------
void __fastcall TTree::Add(VTBarra *barra, VTBarra *barra_pai, VTLigacao *ligacao)
   {
   //variáveis locais
   VTNode *node, *node_pai, *node_irmao;

   //proteção
   if (barra == NULL) return;
   //salva Ligacao  em lisLIG
   if (lisLIG->IndexOf(ligacao)< 0) lisLIG->Add(ligacao);
   //determina Node da Barra pai (Barra pai pode ser nula)
   node_pai = ExisteNode(barra_pai);
   //verifica se a Barra já existe em lisBAR
   if (lisBAR->IndexOf(barra) < 0)
      {//insere Barra em lisBAR
      lisBAR->Add(barra);
      }
   else
      {//determina Node já existente da Barra
      node_irmao = ExisteNode(barra);
      //determina último Node irmão (lista ligada)
      while (node_irmao->Irmao != NULL) node_irmao = node_irmao->Irmao;
      }
   //cria Node p/ a Barra
   if ((node = NewObjNode(barra, ligacao)) == NULL) return;
   //associa este Node c/ seu Node pai
   node->Pai = node_pai;
   //associa Node irmao c/ este Node (lista ligada de Nodes de mesma barra)
   if (node_irmao) node_irmao->Irmao = node;
   }

//---------------------------------------------------------------------------
VTNode* __fastcall TTree::ExisteNode(VTBarra *barra)
   {
   //variáveis locais
   VTNode *node;

   //proteção
   if (barra == NULL) return(NULL);
   //procura Node da Barra indicada
   for (int n = 0; n < lisNODE->Count; n++)
      {
      node = (VTNode*)lisNODE->Items[n];
      if (node->Barra == barra) return(node);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
void __fastcall TTree::Filtra(VTBarra *barra)
   {
   //variáveis locais
   VTNode *node;

   //reinicia lisFILTRO
   lisFILTRO->Clear();
   //insere em lisFILTRO:
   //a) os Nodes da Barra indicada
   //b) os Nodes pai e irmão dos Nodes da Barra indicada
   if ((node = ExisteNode(barra)) != NULL) lisFILTRO->Add(node);
   for (int n = 0; n < lisFILTRO->Count; n++)
      {
      node = (VTNode*)lisFILTRO->Items[n];
      //insere Node irmão em lisFILTRO
      if (node->Irmao) lisFILTRO->Add(node->Irmao);
      //insere Node pai em lisFILTRO
      if (node->Pai) lisFILTRO->Add(node->Pai);
      }
   //reinicia lisBAR e lisLIG c/ as Barras e Ligacoes dos Nodes em lisFILTRO
   lisBAR->Clear();
   lisLIG->Clear();
   for (int n = 0; n < lisFILTRO->Count; n++)
      {
      node = (VTNode*)lisFILTRO->Items[n];
      if (lisBAR->IndexOf(node->Barra) < 0) lisBAR->Add(node->Barra);
      if (node->Ligacao == NULL) continue;
      if (lisLIG->IndexOf(node->Ligacao) < 0) lisLIG->Add(node->Ligacao);
      }
   }

//---------------------------------------------------------------------------
TList* __fastcall TTree::LisBarra(void)
   {
   return(lisBAR);
   }

//---------------------------------------------------------------------------
TList* __fastcall TTree::LisLigacao(void)
   {
   return(lisLIG);
   }

//---------------------------------------------------------------------------
//eof
