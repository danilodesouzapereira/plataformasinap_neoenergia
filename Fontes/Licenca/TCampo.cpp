//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TCampo.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//-----------------------------------------------------------------------------
VTCampo* __fastcall NewObjCampo(AnsiString nome)
   {
   return(new TCampo(nome));
   }

//---------------------------------------------------------------------------
__fastcall TCampo::TCampo(AnsiString nome)
   {
   //salva nome do campo
   PD.nome = nome;
   }

//---------------------------------------------------------------------------
__fastcall TCampo::~TCampo(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
bool __fastcall TCampo::PM_GetAsBoolean(void)
   {
   //proteção
   if (PD.valor.IsEmpty()) PD.valor = BoolToStr(false);
   return(StrToBool(PD.valor));
   }

//---------------------------------------------------------------------------
TDate __fastcall TCampo::PM_GetAsDate(void)
   {
   return(StrToDoubleToData(PD.valor));
   }

//---------------------------------------------------------------------------
double __fastcall TCampo::PM_GetAsDouble(void)
   {
   return(StrToDouble(PD.valor));
   }

//---------------------------------------------------------------------------
int __fastcall TCampo::PM_GetAsInteger(void)
   {
   return(StrToInteger(PD.valor));
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TCampo::PM_GetAsString(void)
   {
   return(PD.valor);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TCampo::PM_GetNome(void)
   {
   return(PD.nome);
   }

//---------------------------------------------------------------------------
void __fastcall TCampo::PM_SetAsBoolean(bool valor)
   {
   PD.valor = BoolToStr(valor);
   }

//---------------------------------------------------------------------------
void __fastcall TCampo::PM_SetAsDate(TDate valor)
   {
   PD.valor = DataToDoubleToStr(valor);
   }

//---------------------------------------------------------------------------
void __fastcall TCampo::PM_SetAsDouble(double valor)
   {
   PD.valor = DoubleToStr("%9.8f", valor);
   }

//---------------------------------------------------------------------------
void __fastcall TCampo::PM_SetAsInteger(int valor)
   {
   PD.valor = IntToStr(valor);
   }

//---------------------------------------------------------------------------
void __fastcall TCampo::PM_SetAsString(AnsiString valor)
   {
   PD.valor = valor;
   }

//-----------------------------------------------------------------------------
//eof



