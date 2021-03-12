//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TPonto2.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTPonto* __fastcall NewObjPonto2(void)
   {
   try{
      return(new TPonto2());
      }catch(Exception &e)
         {
         return(NULL);
         }
   }
/*
//---------------------------------------------------------------------------
VTPonto* __fastcall NewVetObjPonto2(int num_pto)
   {
   try{
      return(new TPonto2[num_pto]);
      }catch(Exception &e)
         {
         return(NULL);
         }
   }
*/
//---------------------------------------------------------------------------
__fastcall TPonto2::TPonto2(void)
   {
   //inicia ponteiro da classe base
   PD.valor = valor;
   }

//---------------------------------------------------------------------------
__fastcall TPonto2::~TPonto2(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
VTPonto* __fastcall TPonto2::Clone(int num_val)
   {
   //variáveis locais
   TPonto2 *clone;

   //cria objeto TPonto2
   if ((clone = (TPonto2*)(NewObjPonto2())) != NULL)
      {//copia hora inicial e final
      clone->HM_ini = HM_ini;
      clone->HM_fim = HM_fim;
      //copia valores
      for (int n = 0; n < 2; n++) {clone->valor[n] = valor[n];}
      }
   return(clone);
   }

//---------------------------------------------------------------------------
//eof
