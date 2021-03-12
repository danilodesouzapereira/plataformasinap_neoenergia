//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TAcaoInsere.h"
#include "..\Arranjo\VTArranjo.h"
#include "..\Curvas\VTCurva.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTTrecho.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTAcao* __fastcall NewObjAcaoInsere(VTEqpto *eqpto)
   {
   try{
      return(new TAcaoInsere(eqpto));
      }catch(Exception &e)
         {
         }
   return(NULL);
   }

//---------------------------------------------------------------------------
__fastcall TAcaoInsere::TAcaoInsere(VTEqpto *eqpto)
   {
   //vari�veis locais
   VTCurva  *curva;
   VTTrecho *trecho;

   //define ID inv�lido
   Id = -1;
   //salva ponteiro p/ Eqpto
   this->eqpto = eqpto;
   //IMPORTANTE: atualiza status do eqpto novo antes de criar seu clone
   //eqpto->DefineStatus(sttNOVO, true);
   eqpto->Status[sttNOVO] = true;
   //verifica se o Eqpto � um Trecho c/ Arranjo pr�prio
   if (eqpto->Tipo() == eqptoTRECHO)
      {
      trecho = (VTTrecho*)eqpto;
      if (! trecho->ArranjoTipico)
         {//marca que o Arranjo pr�prio tamb�m � novo
         //trecho->arranjo->DefineStatus(sttNOVO, true);
         trecho->arranjo->Status[sttNOVO] = true;
         }
      }
   //verifica se o Eqpto tem uma Curva pr�pria associada
   if ((curva = eqpto->ExisteCurva()) != NULL)
      {
      if (! curva->Tipica)
         {//marca que a Curva pr�pria tamb�m � nova
         //curva->DefineStatus(sttNOVO, true);
         curva->Status[sttNOVO] = true;
         }
      }
   //cria uma c�pia do Eqpto original
   copia.original = eqpto->Clone();
   //inicia demais dados
   Pai    = NULL;
   Fixa   = false;
   Moving = false;
   //acerta o ponteiro do item obra
   ItemObra = NULL;
   AcaoClone = NULL;
   }

//---------------------------------------------------------------------------
__fastcall TAcaoInsere::~TAcaoInsere(void)
   {
   //destr�i c�pia do eqpto
   if (copia.original) {delete copia.original; copia.original = NULL;}
   }

//---------------------------------------------------------------------------
VTEqpto* __fastcall TAcaoInsere::Eqpto(void)
   {
   return(eqpto);
   }

//---------------------------------------------------------------------------
VTEqpto* __fastcall TAcaoInsere::CopiaDoEqptoOriginal(void)
   {
   return(copia.original);
   }

//---------------------------------------------------------------------------
VTEqpto* __fastcall TAcaoInsere::CopiaDoEqptoAlterado(void)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
int __fastcall TAcaoInsere::Tipo(void)
   {
   return(acaoINSERE);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TAcaoInsere::TipoAsString(void)
   {
   return("Insere");
   }

//---------------------------------------------------------------------------
//eof

