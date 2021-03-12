//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TPonto4.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTPonto* __fastcall NewObjPonto4(void)
   {
   try{
      return(new TPonto4());
      }catch(Exception &e)
         {
         return(NULL);
         }
   }
/*
//---------------------------------------------------------------------------
VTPonto* __fastcall NewVetObjPonto4(int num_pto)
   {
   try{
      return(new TPonto4[num_pto]);
      }catch(Exception &e)
         {
         return(NULL);
         }
   }
*/
//---------------------------------------------------------------------------
__fastcall TPonto4::TPonto4(void)
   {
   //inicia ponteiro da classe base
   PD.valor = valor;
   }

//---------------------------------------------------------------------------
__fastcall TPonto4::~TPonto4(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
VTPonto* __fastcall TPonto4::Clone(int num_val)
   {
   //variáveis locais
   TPonto4 *clone;

   //cria objeto TPonto4
   if ((clone = (TPonto4*)(NewObjPonto4())) != NULL)
      {//copia hora inicial e final
      clone->HM_ini = HM_ini;
      clone->HM_fim = HM_fim;
      //copia valores
      for (int n = 0; n < 4; n++) {clone->valor[n] = valor[n];}
      }
   return(clone);
   }

//---------------------------------------------------------------------------
//eof
