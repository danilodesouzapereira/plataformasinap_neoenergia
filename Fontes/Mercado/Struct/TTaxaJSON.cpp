//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TTaxaJSON.h"
#include "TPeriodo.h"
#include "..\Classes\VTClasse.h"
#include "..\Constante\Const.h"
#include "..\Rede\VTFases.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTTaxaJSON* __fastcall NewObjTaxaJSON(void)
   {
   try{
      return(new TTaxaJSON());
      }catch(Exception &e)
         {
         }
   return(NULL);
   }

//---------------------------------------------------------------------------
__fastcall TTaxaJSON::TTaxaJSON(void)
   {
   //inicia dadoS
   try{
		PD.ano = 0;
		PD.valor = 0.0;
      }catch(Exception &e)
         {
         }
   }

//---------------------------------------------------------------------------
__fastcall TTaxaJSON::~TTaxaJSON(void)
   {
   PD.ano = 0;
   PD.valor = 0.0;
   }

//---------------------------------------------------------------------------
int __fastcall TTaxaJSON::PM_GetAno(void)
   {
   return(PD.ano);
   }

//---------------------------------------------------------------------------
double __fastcall TTaxaJSON::PM_GetTaxa_perc(void)
   {
   return(PD.valor);
   }

//---------------------------------------------------------------------------
void __fastcall TTaxaJSON::PM_SetAno(int ano)
   {
   PD.ano = ano;
   }

//---------------------------------------------------------------------------
void __fastcall TTaxaJSON::PM_SetTaxa_perc(double valor)
   {
   PD.valor = valor;
   }


//---------------------------------------------------------------------------
//eof
