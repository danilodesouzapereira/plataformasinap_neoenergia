//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TPonto1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTPonto* __fastcall NewObjPonto1(void)
   {
   try{
      return(new TPonto1());
      }catch(Exception &e)
         {
         return(NULL);
         }
   }
/*
//---------------------------------------------------------------------------
VTPonto* __fastcall NewVetObjPonto1(int num_pto)
   {
   try{
      return(new TPonto1[num_pto]);
      }catch(Exception &e)
         {
         return(NULL);
         }
   }
*/
//---------------------------------------------------------------------------
__fastcall TPonto1::TPonto1(void)
   {
   //inicia ponteiro da classe base
   PD.valor = &valor;
   }

//---------------------------------------------------------------------------
__fastcall TPonto1::~TPonto1(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
VTPonto* __fastcall TPonto1::Clone(int num_val)
   {
   //variáveis locais
   TPonto1 *clone;

   //cria objeto TPonto1
   if ((clone = (TPonto1*)(NewObjPonto1())) != NULL)
      {//copia hora inicial e final
      clone->HM_ini = HM_ini;
      clone->HM_fim = HM_fim;
      //copia valores
      clone->valor = valor;
      }
   return(clone);
   }

//---------------------------------------------------------------------------
//eof
