//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TTipos.h"
#include "VTTipoChave.h"
#include "VTTipoRede.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTTipos
//---------------------------------------------------------------------------
VTTipos* __fastcall NewObjTipos(void)
   {
   return(new TTipos());
   }

//---------------------------------------------------------------------------
__fastcall TTipos::TTipos(void)
   {
   //cria listas
   lisTIPOCHAVE   = new TList();
   lisTIPOREDE    = new TList();
   //cria listas p/ objetos duplicados
   dup.lisTIPOCHAVE   = new TList();
   dup.lisTIPOREDE    = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TTipos::~TTipos(void)
   {
   //destrói listas e seus objetos
   if (lisTIPOCHAVE) {LimpaTList(lisTIPOCHAVE); delete lisTIPOCHAVE; lisTIPOCHAVE = NULL;}
   if (lisTIPOREDE)  {LimpaTList(lisTIPOREDE);  delete lisTIPOREDE;  lisTIPOREDE  = NULL;}
   //destrói listas e seus objetos
   if (dup.lisTIPOCHAVE) {LimpaTList(dup.lisTIPOCHAVE); delete dup.lisTIPOCHAVE; dup.lisTIPOCHAVE = NULL;}
   if (dup.lisTIPOREDE)  {LimpaTList(dup.lisTIPOREDE);  delete dup.lisTIPOREDE;  dup.lisTIPOREDE  = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TTipos::Clear(void)
   {
   //limpa listas
   LimpaTList(lisTIPOCHAVE);
   LimpaTList(lisTIPOREDE);
   //limpa listas
   LimpaTList(dup.lisTIPOCHAVE);
   LimpaTList(dup.lisTIPOREDE);
   }

//---------------------------------------------------------------------------
VTTipoChave* __fastcall TTipos::ExisteTipoChave(int tipochave_id)
   {
   //variáveis locais
   VTTipoChave *tipochave;

   //loop p/ todos objetos VTTipoChave
   for (int n = 0; n < lisTIPOCHAVE->Count; n++)
      {
      tipochave = (VTTipoChave*)lisTIPOCHAVE->Items[n];
      if (tipochave->Id == tipochave_id)    return(tipochave);
      }
   //procura TipoChave na lista de TipoChaves duplicado: dup.lisTIPOCHAVE
   for (int n = 0; n < dup.lisTIPOCHAVE->Count; n++)
      {
      tipochave = (VTTipoChave*)dup.lisTIPOCHAVE->Items[n];
      if (tipochave->Id == tipochave_id)
         {//existe TipoChave duplicado: determina TipoChave original pelo Codigo
         if ((tipochave = ExisteTipoChave(tipochave->Codigo, tipochave->Tipo)) != NULL) return(tipochave);
         }
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
VTTipoChave* __fastcall TTipos::ExisteTipoChave(AnsiString codigo, int tipo)
   {
   //variáveis locais
   VTTipoChave *tipochave;

   //loop p/ todos objetos VTTipoChave
   for (int n = 0; n < lisTIPOCHAVE->Count; n++)
      {
      tipochave = (VTTipoChave*)lisTIPOCHAVE->Items[n];
      //verifica se deve fazer busca por tipo
      if (tipo >= 0)
         {
         if (tipochave->Tipo == tipo) return(tipochave);
         }
      //verifica se deve fazer busca por código
      if (! codigo.IsEmpty())
         {
         if (tipochave->Codigo.AnsiCompareIC(codigo) == 0) return(tipochave);
         }
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
VTTipoRede* __fastcall TTipos::ExisteTipoRede(int tiporede_id)
   {
   //variáveis locais
   VTTipoRede *tiporede;

   //loop p/ todos objetos VTTipoRede
   for (int n = 0; n < lisTIPOREDE->Count; n++)
      {
      tiporede = (VTTipoRede*)lisTIPOREDE->Items[n];
      if (tiporede->Id == tiporede_id) return(tiporede);
      }
   //procura TipoRede na lista de TipoRedes duplicado: dup.lisTIPOREDE
   for (int n = 0; n < dup.lisTIPOREDE->Count; n++)
      {
      tiporede = (VTTipoRede*)dup.lisTIPOREDE->Items[n];
      if (tiporede->Id == tiporede_id)
         {//existe TipoRede duplicado: determina TipoRede original pelo Codigo
         if ((tiporede = ExisteTipoRede(tiporede->Codigo, tiporede->Segmento)) != NULL) return(tiporede);
         }
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
VTTipoRede* __fastcall TTipos::ExisteTipoRede(AnsiString codigo, int segmento)
   {
   //variáveis locais
   VTTipoRede *tiporede;

   //loop p/ todos objetos VTTipoRede
   for (int n = 0; n < lisTIPOREDE->Count; n++)
      {
      tiporede = (VTTipoRede*)lisTIPOREDE->Items[n];
      //verifica se deve fazer busca por segmento
      if (segmento >= 0)
         {
         if (tiporede->Segmento == segmento) return(tiporede);
         }
      //verifica se deve fazer busca por código
      if (! codigo.IsEmpty())
         {
         if (tiporede->Codigo.AnsiCompareIC(codigo) == 0) return(tiporede);
         }
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
void __fastcall TTipos::InsereTipoChave(VTTipoChave *tipochave)
   {
   //proteção
   if (tipochave == NULL) return;
   //verifica se o TipoChave já existe
   if (lisTIPOCHAVE->IndexOf(tipochave) < 0)
      {//verifica se existe TipoChave com mesmo código
      if (ExisteTipoChave(tipochave->Codigo, tipochave->Tipo))
         {//insere TipoChave em dup.lisTIPOCHAVE
         dup.lisTIPOCHAVE->Add(tipochave);
         }
      else
         {//insere objeto VTTipoChave na lista lisTIPOCHAVE
         lisTIPOCHAVE->Add(tipochave);
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TTipos::InsereTipoRede(VTTipoRede *tiporede)
   {
   //proteção
   if (tiporede == NULL) return;
   //verifica se o objeto VTTipoRede já existe em lisTIPOREDE
   if (lisTIPOREDE->IndexOf(tiporede) < 0)
      {//verifica se existe TipoRede com mesmo código
      if (ExisteTipoRede(tiporede->Codigo, tiporede->Segmento))
         {//insere objeto VTTipoRede na lista dup.lisTIPOREDE
         dup.lisTIPOREDE->Add(tiporede);
         }
      else
         {//insere objeto VTTipoRede na lista lisTIPOREDE
         lisTIPOREDE->Add(tiporede);
         }
      }
   }

//---------------------------------------------------------------------------
TList* __fastcall TTipos::LisTipoChave(void)
   {
   return(lisTIPOCHAVE);
   }

//---------------------------------------------------------------------------
TList* __fastcall TTipos::LisTipoRede(void)
   {
   return(lisTIPOREDE);
   }

//---------------------------------------------------------------------------
//eof

