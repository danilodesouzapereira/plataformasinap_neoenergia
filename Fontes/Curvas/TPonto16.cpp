//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TPonto16.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTPonto* __fastcall NewObjPonto16(void)
   {
   try{
      return(new TPonto16());
      }catch(Exception &e)
         {
         return(NULL);
         }
   }
/*
//---------------------------------------------------------------------------
VTPonto* __fastcall NewVetObjPonto16(int num_pto)
   {
   try{
      return(new TPonto16[num_pto]);
      }catch(Exception &e)
         {
         return(NULL);
         }
   }
*/
//---------------------------------------------------------------------------
__fastcall TPonto16::TPonto16(void)
   {
   //inicia ponteiro da classe base
   PD.valor = valor;
   }

//---------------------------------------------------------------------------
__fastcall TPonto16::~TPonto16(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
VTPonto* __fastcall TPonto16::Clone(int num_val)
   {
   //variáveis locais
   TPonto16 *clone;

   //cria objeto TPonto16
   if ((clone = (TPonto16*)(NewObjPonto16())) != NULL)
      {//copia hora inicial e final
      clone->HM_ini = HM_ini;
      clone->HM_fim = HM_fim;
      //copia valores
      for (int n = 0; n < 16; n++) {clone->valor[n] = valor[n];}
      }
   return(clone);
   }

//---------------------------------------------------------------------------
//eof
