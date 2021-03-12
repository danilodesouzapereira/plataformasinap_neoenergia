//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TListeqp.h"
#include "..\Rede\VTEqpto.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

/*
//---------------------------------------------------------------------------
//fun��o usada na ordena��o dos Eqptos: ID em ordem crescente
static int __fastcall ComparaID(void *obj1, void* obj2)
   {
   return((((VTEqpto*)obj1)->Id) - (((VTEqpto*)obj2)->Id));
   }
*/
//---------------------------------------------------------------------------
//fun��o usada na ordena��o dos Eqptos: ID em ordem crescente
static int __fastcall ComparaID(void *obj1, void* obj2)
   {
   int id1 = ((VTEqpto*)obj1)->Id;
   int id2 = ((VTEqpto*)obj2)->Id;
   if      (id1 == id2) return( 0);
   else if (id1 <  id2) return( 1);
   else                 return(-1);
   }

//---------------------------------------------------------------------------
//fun��o para criar objeto VTListeqp
VTListeqp *NewObjListeqp(void)
   {
   return(new TListeqp());
   }

//---------------------------------------------------------------------------
__fastcall TListeqp::TListeqp(void)
   {
   //habilita busca bin�ria
   BuscaBinariaEnabled = true;
   //inicia n�mero que foram ordenados
   ordenada = false;
   }

//---------------------------------------------------------------------------
__fastcall TListeqp::~TListeqp(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
int  __fastcall TListeqp::Add(VTEqpto *eqpto)
   {
   //marca que lista n�o est� ordenada
   ordenada = false;
   //insere Eqpto
   return(TList::Add(eqpto));
   }

//---------------------------------------------------------------------------
VTEqpto* __fastcall TListeqp::BuscaBinaria(int eqpto_id)
   {
   //vari�veis locais
   int     index, index_ini, index_fim;
   VTEqpto *eqpto;

   //prote��o: verifica se h� Eqptos
   if (Count == 0) return(NULL);
   //verifica primeiro Eqpto da lista
   eqpto = (VTEqpto*)First();
   //if (eqpto->Id >  eqpto_id) return(NULL);
   if (eqpto->Id == eqpto_id) return(eqpto);
   //verifica �ltimo Eqpto da lista
   eqpto = (VTEqpto*)Last();
   //if (eqpto->Id <  eqpto_id) return(NULL);
   if (eqpto->Id == eqpto_id) return(eqpto);
   //define index_ini e index_fim: ambos sempre apontam para Eqptos j� analisados
   index_ini = 0;
   index_fim = Count-1;
   //executa busca por dicotomia
   while (index_fim > (index_ini+1))
      {
      index = (index_ini + index_fim) / 2;
      eqpto = (VTEqpto*)Items[index];
      if (eqpto->Id == eqpto_id) return(eqpto);
      //verifica se Id do eqpto � menor do que o eqpto_id indicado
      if (eqpto->Id  < eqpto_id)
         {index_ini = index;}
      else
         {index_fim = index;}
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
VTEqpto* __fastcall TListeqp::ExisteEqpto(int eqpto_id)
   {
   //vari�veis locais
   VTEqpto *eqpto;

   //prote��o: verifica se h� Eqptos
   if (Count == 0) return(NULL);
   //verifica se deve fazer busca binaria
   if (BuscaBinariaEnabled)
      {//se necess�rio, ordena a lista
      if (! ordenada)
         {
         Ordena();
         }
      return(BuscaBinaria(eqpto_id));
      }
   //executa busca sequencial
   for (int n = 0; n < Count; n++)
      {
      eqpto = (VTEqpto*)Items[n];
      if (eqpto->Id == eqpto_id) return(eqpto);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
bool __fastcall TListeqp::Ordena(void)
   {
   //vri�veis locais
   VTEqpto *eqpto_ini, *eqpto_fim;

   //atualiza indica��o de lista ordenada
   ordenada = true;
   //prote��o: lista vazia ou com apenas um elemento
   if (Count <= 1) return(true);
   //executa ordena��o
   Sort(ComparaID);
   //verifica se a ordena��o foi correta
   eqpto_ini = (VTEqpto*)First();
   eqpto_fim = (VTEqpto*)Last();
   if (eqpto_ini->Id > eqpto_fim->Id)
      {//inverte os Eqptos na lista
      for (int i = 0, j = Count-1; i < j; i++, j--)
         {
         eqpto_ini = (VTEqpto*)Items[i];
         eqpto_fim = (VTEqpto*)Items[j];
         //inverte Eqptos na lista
         Items[i] = eqpto_fim;
         Items[j] = eqpto_ini;
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
//eof

