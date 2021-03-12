//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TAcaoAltera.h"
#include "..\Arranjo\VTArranjo.h"
#include "..\Curvas\VTCurva.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTTrecho.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTAcao* __fastcall NewObjAcaoAltera(VTEqpto *original, VTEqpto *alterado, VTEqpto *copia_original)
   {
   try{
      return(new TAcaoAltera(original, alterado, copia_original));
      }catch(Exception &e)
         {
         }
   return(NULL);
   }

//---------------------------------------------------------------------------
__fastcall TAcaoAltera::TAcaoAltera(VTEqpto *original, VTEqpto *alterado, VTEqpto *copia_original)
   {
   //variáveis locais
   VTCurva  *curva;
   VTTrecho *trecho;

   //define ID inválido
   Id = -1;
   //salva ponteiro p/ Eqpto
   eqpto  = original;
   //salva ponteiro p/ cópia do eqpto já alterado
   copia.alterado = alterado;
   //verifica se recebeu uma cópia do Eqpto original
   if (copia_original == NULL)
      {//cria um cópia do Eqpto original p/ operação de Redo
      copia_original = eqpto->Clone();
      }
   //salva cópia do eqpto original
   copia.original = copia_original;
   //IMPORTANTE: atualiza status do eqpto alterado
   //copia.alterado->DefineStatus(sttALTERADO, true);
   copia.alterado->Status[sttALTERADO] = true;
   //verifica se o Eqpto é um Trecho c/ Arranjo próprio
   if (copia.alterado->Tipo() == eqptoTRECHO)
      {
      trecho = (VTTrecho*)copia.alterado;
      if (! trecho->ArranjoTipico)
         {//marca que o Arranjo próprio também foi alterado
         //trecho->arranjo->DefineStatus(sttALTERADO, true);
         trecho->arranjo->Status[sttALTERADO] = true;
         }
      }
   //verifica se o Eqpto tem uma Curva própria associada
   if ((curva = copia.alterado->ExisteCurva()) != NULL)
      {
      if (! curva->Tipica)
         {//marca que a Curva própria também foi alterada
         //curva->DefineStatus(sttALTERADO, true);
         curva->Status[sttALTERADO] = true;
         }
      }
   //inicia demais dados
   Pai    = NULL;
   Fixa   = false;
   Moving = false;
   //acerta o ponteiro do item obra
   ItemObra = NULL;
   AcaoClone = NULL;
   }

//---------------------------------------------------------------------------
__fastcall TAcaoAltera::~TAcaoAltera(void)
   {
   //destrói cópias do Eqpto
   if (copia.original) {delete copia.original; copia.original = NULL;}
   if (copia.alterado) {delete copia.alterado; copia.alterado = NULL;}
   }

//---------------------------------------------------------------------------
VTEqpto* __fastcall TAcaoAltera::Eqpto(void)
   {
   return(eqpto);
   }

//---------------------------------------------------------------------------
VTEqpto* __fastcall TAcaoAltera::CopiaDoEqptoOriginal(void)
   {
   return(copia.original);
   }

//---------------------------------------------------------------------------
VTEqpto* __fastcall TAcaoAltera::CopiaDoEqptoAlterado(void)
   {
   return(copia.alterado);
   }

//---------------------------------------------------------------------------
int __fastcall TAcaoAltera::Tipo(void)
   {
   return(acaoALTERA);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TAcaoAltera::TipoAsString(void)
   {
   return("Altera");
   }

//---------------------------------------------------------------------------
//eof

