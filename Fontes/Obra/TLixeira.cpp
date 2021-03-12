//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TLixeira.h"
#include "..\Rede\VTEqpto.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTLixeira
//---------------------------------------------------------------------------
VTLixeira* __fastcall NewObjLixeira(void)
   {
   try{
      return(new TLixeira());
      }catch(Exception &e)
         {
         return(NULL);
         }
   }
   
//---------------------------------------------------------------------------
__fastcall TLixeira::TLixeira(void)
   {
   //cria objetos
   lisLIXO = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TLixeira::~TLixeira(void)
   {
   //destrói lista e seus objetos
   if (lisLIXO) {LimpaTList(lisLIXO); delete lisLIXO; lisLIXO = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TLixeira::Clear(void)
   {
   //destrói objetos lisLIXO
   LimpaTList(lisLIXO);
   }

//---------------------------------------------------------------------------
void __fastcall TLixeira::ClearEqptoNovo(void)
   {
   //variáveis locais
   VTEqpto *eqpto;

   //elimina todos os Eqptos novos (que ainda não estão na base de dados)
   for (int n = lisLIXO->Count - 1; n >= 0; n--)
      {
      eqpto = (VTEqpto*)lisLIXO->Items[n];
      //verifica se é um Eqpto novo (ainda não gravado na base de dados)
      if (eqpto->StatusNovo() || (eqpto->Id < 0))
         {//retira Eqpto da lista e destrói
         lisLIXO->Delete(n);
         delete eqpto;
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TLixeira::InsereEqpto(VTEqpto *eqpto)
   {
   //proteção
   if (eqpto == NULL) return;
   //salva Eqpto em lisLIXO
   if (lisLIXO->IndexOf(eqpto) < 0) lisLIXO->Add(eqpto);
   //atualiza Status
   eqpto->Status[sttEXCLUIDO] = true;
   }

//---------------------------------------------------------------------------
TList* __fastcall TLixeira::LisEqpto(void)
   {
   return(lisLIXO);
   }

//---------------------------------------------------------------------------
void __fastcall TLixeira::LisEqpto(TList *lisEQP, int tipo)
   {
   //variáveis locais
   VTEqpto *eqpto;

   //procura tipo de Eqpto indicado
   for (int n = 0; n < lisLIXO->Count; n++)
      {
      eqpto = (VTEqpto*)lisLIXO->Items[n];
      if (eqpto->Tipo() == tipo) lisEQP->Add(eqpto);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TLixeira::RemoveEqpto(VTEqpto *eqpto)
   {
   //proteção
   if (eqpto == NULL) return;
   //atualiza Status
   eqpto->Status[sttEXCLUIDO] = false;
   //retira Eqpto da lista lisLIXO sem destrui-lo
   lisLIXO->Remove(eqpto);
   }

//---------------------------------------------------------------------------
//eof
