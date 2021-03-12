//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TNode.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe
VTNode* __fastcall NewObjNode(VTBarra *barra, VTLigacao *ligacao)
   {
   try{
      return(new TNode(barra, ligacao));
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TNode::TNode(VTBarra *barra, VTLigacao *ligacao)
   {
   //salva ponteiro p/ objetos
   PD.barra   = barra;
   PD.ligacao = ligacao;
   }

//---------------------------------------------------------------------------
__fastcall TNode::~TNode(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
VTBarra* __fastcall TNode::PM_GetBarra(void)
   {
   return(PD.barra);
   }

//---------------------------------------------------------------------------
VTLigacao* __fastcall TNode::PM_GetLigacao(void)
   {
   return(PD.ligacao);
   }

//---------------------------------------------------------------------------
VTNode*__fastcall TNode::PM_GetNodeIrmao(void)
   {
   return(PD.irmao);
   }

//---------------------------------------------------------------------------
VTNode* __fastcall TNode::PM_GetNodePai(void)
   {
   return(PD.pai);
   }

//---------------------------------------------------------------------------
void __fastcall TNode::PM_SetNodeIrmao(VTNode *node)
   {
   PD.irmao = node;
   }

//---------------------------------------------------------------------------
void __fastcall TNode::PM_SetNodePai(VTNode *node)
   {
   PD.pai = node;
   }

//---------------------------------------------------------------------------
//eof
