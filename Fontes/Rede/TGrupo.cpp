//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TGrupo.h"
#include "..\Rede\VTMutua.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTTrecho.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe
VTGrupo* __fastcall NewObjGrupo(void)
   {
   try{
      return(new TGrupo());
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TGrupo::TGrupo(void)
   {
   //cria lista
   lisMUT = new TList();
   lisLIG = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TGrupo::~TGrupo(void)
   {
   //verifica se o Grupo possui Mutuas associadas a um Arranjo
   if (Arranjo != NULL)
      {//destrói Mutuas
      LimpaTList(lisMUT);
      }
   //destrói lista sem destruir seus objetos
   if (lisMUT) {delete lisMUT; lisMUT = NULL;}
   if (lisLIG) {delete lisLIG; lisLIG = NULL;}
   }

//---------------------------------------------------------------------------
bool __fastcall TGrupo::ExisteLigacao(VTLigacao *ligacao)
   {
   return(lisLIG->IndexOf(ligacao) >= 0);
   }

//---------------------------------------------------------------------------
bool __fastcall TGrupo::ExisteMutua(VTMutua *mutua)
   {
   return(lisMUT->IndexOf(mutua) >= 0);
   }

//---------------------------------------------------------------------------
int __fastcall TGrupo::IndexOf(VTMutua *mutua)
   {
   return(lisMUT->IndexOf(mutua));
   }

//---------------------------------------------------------------------------
void __fastcall TGrupo::InsereLigacao(VTLigacao *ligacao)
   {
   if (lisLIG->IndexOf(ligacao) < 0) lisLIG->Add(ligacao);
   }

//---------------------------------------------------------------------------
void __fastcall TGrupo::InsereMutua(VTMutua *mutua)
   {
   //proteção
   if (mutua == NULL) return;
   //insere Mutua somente se ele ainda não existir em lisMUT
   if (lisMUT->IndexOf(mutua) < 0)
      {//insere Mutua
      lisMUT->Add(mutua);
      //insere Trechos da Mutua
      InsereLigacao(mutua->ptre1);
      InsereLigacao(mutua->ptre2);
      }
   }

//---------------------------------------------------------------------------
TList* __fastcall TGrupo::LisLigacao(void)
   {
   return(lisLIG);
   }

//---------------------------------------------------------------------------
TList* __fastcall TGrupo::LisMutua(void)
   {
   return(lisMUT);
   }

//---------------------------------------------------------------------------
VTArranjo* __fastcall TGrupo::PM_GetArranjo(void)
   {
   return(PD.arranjo);
   }

//---------------------------------------------------------------------------
void __fastcall TGrupo::PM_SetArranjo(VTArranjo *arranjo)
   {
   PD.arranjo = arranjo;
   }

//---------------------------------------------------------------------------
void __fastcall TGrupo::RemoveMutua(VTMutua *mutua)
   {
   //remove Mutua
   lisMUT->Remove(mutua);
   //remove Trechos da Mutua
   lisLIG->Remove(mutua->ptre1);
   lisLIG->Remove(mutua->ptre2);
   }

//---------------------------------------------------------------------------
void __fastcall TGrupo::Reinicia(void)
   {
   lisMUT->Clear();
   lisLIG->Clear();
   }
   
//---------------------------------------------------------------------------
//eof
