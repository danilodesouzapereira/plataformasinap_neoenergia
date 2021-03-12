//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TPontoG.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTPonto* __fastcall NewObjPontoG(int num_val)
   {
   try{
      return(new TPontoG(num_val));
      }catch(Exception &e)
         {
         return(NULL);
         }
   }
/*
//---------------------------------------------------------------------------
VTPonto* __fastcall NewVetObjPontoG(int num_pto)
   {
   try{
      return(new TPontoG[num_pto]);
      }catch(Exception &e)
         {
         return(NULL);
         }
   }
*/
//---------------------------------------------------------------------------
__fastcall TPontoG::TPontoG(int num_val)
   {
   //aloca memória p/ os valores
   valor = new float[num_val];
   //inicia ponteiro da classe base
   PD.valor = valor;
   }

//---------------------------------------------------------------------------
__fastcall TPontoG::~TPontoG(void)
   {
   //libera memória alocada
   if (valor != NULL) {delete[] valor; valor = NULL;}
   }

//---------------------------------------------------------------------------
VTPonto* __fastcall TPontoG::Clone(int num_val)
   {
   //variáveis locais
   TPontoG *clone;

   //cria objeto TPontoG
   if ((clone = (TPontoG*)(NewObjPontoG(num_val))) != NULL)
      {//copia hora inicial e final
      clone->HM_ini = HM_ini;
      clone->HM_fim = HM_fim;
      //copia valores
      for (int n = 0; n < num_val; n++) {clone->valor[n] = valor[n];}
      }
   return(clone);
   }

//---------------------------------------------------------------------------
//eof
