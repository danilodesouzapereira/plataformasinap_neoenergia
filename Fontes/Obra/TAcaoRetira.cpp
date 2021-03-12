//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TAcaoRetira.h"
#include "..\Rede\VTEqpto.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTAcao* __fastcall NewObjAcaoRetira(VTEqpto *eqpto)
   {
   try{
      return(new TAcaoRetira(eqpto));
      }catch(Exception &e)
         {
         }
   return(NULL);
   }

//---------------------------------------------------------------------------
__fastcall TAcaoRetira::TAcaoRetira(VTEqpto *eqpto)
   {
   //define ID inválido
   Id = -1;
   //salva ponteiro p/ Eqpto
   this->eqpto = eqpto;
   //cria uma cópia do Eqpto original
   copia.original = eqpto->Clone();
   //IMPORTANTE: atualiza status do eqpto retirado
   //eqpto->DefineStatus(sttEXCLUIDO, true);
   eqpto->Status[sttEXCLUIDO] = true;
   //inicia demais dados
   Pai    = NULL;
   Fixa   = false;
   Moving = false;
   //acerta o ponteiro do item obra
   ItemObra = NULL;
   AcaoClone = NULL;
   }

//---------------------------------------------------------------------------
__fastcall TAcaoRetira::~TAcaoRetira(void)
   {
   //destrói cópia do eqpto
   if (copia.original) {delete copia.original; copia.original = NULL;}
   }

//---------------------------------------------------------------------------
VTEqpto* __fastcall TAcaoRetira::Eqpto(void)
   {
   return(eqpto);
   }

//---------------------------------------------------------------------------
VTEqpto* __fastcall TAcaoRetira::CopiaDoEqptoOriginal(void)
   {
   return(copia.original);
   }

//---------------------------------------------------------------------------
VTEqpto* __fastcall TAcaoRetira::CopiaDoEqptoAlterado(void)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
int __fastcall TAcaoRetira::Tipo(void)
   {
   return(acaoRETIRA);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TAcaoRetira::TipoAsString(void)
   {
   return("Retira");
   }

//---------------------------------------------------------------------------
//eof

