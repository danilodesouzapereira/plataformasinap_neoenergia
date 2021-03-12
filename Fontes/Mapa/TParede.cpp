//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TParede.h"
#include "..\Rede\VTBarra.h"
#include "..\..\DLL_Inc\Funcao.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTParede* __fastcall NewObjParede(void)
   {
   try{//cria objeto TParede
      return(new TParede());
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TParede::TParede(void)
   {
   //inicia atributos
   Rede = NULL;
   //cria lista
   lisBARRA   = new TList();
   lisLIGACAO = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TParede::~TParede(void)
   {
   //destrói lista sem destruir seus objetos
   if (lisBARRA  ) {delete lisBARRA  ;  lisBARRA   = NULL;}
   if (lisLIGACAO) {delete lisLIGACAO;  lisLIGACAO = NULL;}
   }

//---------------------------------------------------------------------------
bool __fastcall TParede::ExisteBarra(VTBarra *barra)
   {
   return(lisBARRA->IndexOf(barra) >= 0);
   }

//---------------------------------------------------------------------------
bool __fastcall TParede::ExisteLigacao(VTLigacao *ligacao)
   {
   return(lisLIGACAO->IndexOf(ligacao) >= 0);
   }

//---------------------------------------------------------------------------
bool __fastcall TParede::InsereBarra(VTBarra *barra)
   {
   //protecção
   if (barra == NULL) return(false);
   //verifica se a Barra já existe em lisBARRA
   if (lisBARRA->IndexOf(barra) < 0) lisBARRA->Add(barra);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TParede::InsereLigacao(VTLigacao *ligacao)
   {
   //proteção
   if (ligacao == NULL) return(false);
   //verifica se a Ligacao já existe em lisLIGACAO
   if (lisLIGACAO->IndexOf(ligacao) < 0) lisLIGACAO->Add(ligacao);

   return(true);
   }

//---------------------------------------------------------------------------
TList* __fastcall TParede::LisBarra(void)
   {
   return(lisBARRA);
   }

//---------------------------------------------------------------------------
int __fastcall TParede::LisBarra(TList *lisEXT)
   {
   //variáveis locais
   int     count_ini = lisEXT->Count;
   VTBarra *barra;

   //determina objetos VTEqbar do tipo indicado
   for (int nb = 0; nb < lisBARRA->Count; nb++)
      {
      barra = (VTBarra*)lisBARRA->Items[nb];
      if (lisEXT->IndexOf(barra) < 0) lisEXT->Add(barra);
      }
   return (lisEXT->Count - count_ini);
   }

//---------------------------------------------------------------------------
int __fastcall TParede::LisEqbar(TList *lisEXT,   int tipo)
   {
   //variáveis locais
   int     count_ini = lisEXT->Count;
   VTBarra *barra;

   //determina objetos VTEqbar do tipo indicado
   for (int nb = 0; nb < lisBARRA->Count; nb++)
      {
      barra = (VTBarra*)lisBARRA->Items[nb];
      barra->LisEqbar(lisEXT, tipo);
      }
   return (lisEXT->Count - count_ini);
   }

//---------------------------------------------------------------------------
int __fastcall TParede::LisEqpto(TList *lisEXT, int tipo)
   {
   //variáveis locais
   int count_ini = lisEXT->Count;

   //verifica tipo de eqpto solicitado
   if      (tipo == eqptoBARRA)   LisBarra(lisEXT);
   else if (tipo == eqptoEQBAR)   LisEqbar(lisEXT);
   else if (tipo == eqptoLIGACAO) LisLigacao(lisEXT);
   else if (tipo == -1)
      {//todos os equipamentos
      LisBarra(lisEXT);
      LisEqbar(lisEXT);
      LisLigacao(lisEXT);
      }
   else
      {//um tipo de equipamento específico (Eqbar ou Ligacao)
      LisEqbar(lisEXT, tipo);
      LisLigacao(lisEXT, tipo);
      }
   return(lisEXT->Count - count_ini);
   }

//---------------------------------------------------------------------------
TList* __fastcall TParede::LisLigacao(void)
   {
   return(lisLIGACAO);
   }

//---------------------------------------------------------------------------
int __fastcall TParede::LisLigacao(TList *lisEXT, int tipo)
   {
   //variáveis locais
   int     count_ini = lisEXT->Count;
   VTEqpto *eqpto;

   //verifica o tipo de objeto VTLigacao solicitado
   if ((tipo < 0)||(tipo == eqptoLIGACAO))
      {//copia todas Ligacoes
      for (int n = 0; n < lisLIGACAO->Count; n++)
         {
         eqpto = (VTEqpto*)lisLIGACAO->Items[n];
         //insere objeto VTEqpto em lisEQP
         if (lisEXT->IndexOf(eqpto)) lisEXT->Add(eqpto);
         }
      }
   else
      {//copia somente os objetos VTLigacao do tipo solicitado
      for (int n = 0; n < lisLIGACAO->Count; n++)
         {
         eqpto = (VTEqpto*)lisLIGACAO->Items[n];
         if (eqpto->Tipo() == tipo)
            {//insere objeto VTEqpto em lisEQP
            if (lisEXT->IndexOf(eqpto)) lisEXT->Add(eqpto);
            }
         }
      }
   return(lisEXT->Count - count_ini);
   }

//---------------------------------------------------------------------------
void __fastcall TParede::ReiniciaLisEqpto(void)
   {
   //limpa listas lisBARRA e lisLIGACAO
   lisBARRA->Clear();
   lisLIGACAO->Clear();
   }

//---------------------------------------------------------------------------
//eof

