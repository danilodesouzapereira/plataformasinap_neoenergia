//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TDados.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTDados* __fastcall NewObjDados(void)
   {
   return(new TDados());
   }

//---------------------------------------------------------------------------
__fastcall TDados::TDados(void)
   {
   //cria lista
   LisStrList = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TDados::~TDados(void)
   {
   //destrói lista e seus objetos
   if (LisStrList) {LimpaTList(LisStrList); delete LisStrList; LisStrList = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TDados::Clear(void)
   {
   LimpaTList(LisStrList);
   }

//---------------------------------------------------------------------------
TStringList* __fastcall TDados::InsereStringList(void)
   {
   //variáveis locais
   TStringList *StrList;

   LisStrList->Add(StrList = new TStringList());

   return(StrList);
   }

//---------------------------------------------------------------------------
TList* __fastcall TDados::LisStringList(void)
   {
   return(LisStrList);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TDados::PM_GetCelula(int ind_reg, int ind_col)
   {
   //variáveis locais
   AnsiString   valor;
   TStringList *StrList;

   //proteção
   if (ind_reg < LisStrList->Count)
      {
      StrList = (TStringList*)LisStrList->Items[ind_reg];
      if (ind_col < StrList->Count)
         {
         valor = StrList->Strings[ind_col];
         }
      }
   return(valor);
   }

//---------------------------------------------------------------------------
void __fastcall TDados::PM_SetCelula(int ind_reg, int ind_col, AnsiString valor)
   {
   //variáveis locais
   TStringList *StrList;

   //proteção
   if (ind_reg < LisStrList->Count)
      {
      StrList = (TStringList*)LisStrList->Items[ind_reg];
      if (ind_col < StrList->Count)
         {
         StrList->Strings[ind_col] = valor;
         }
      }
   }

//---------------------------------------------------------------------------
//eof


