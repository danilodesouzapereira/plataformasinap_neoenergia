//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TPadraoRede.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTPadraoRede* __fastcall NewObjPadraoRede(void)
   {
   try{
      return(new TPadraoRede());
      }catch(Exception &e)
         {
         return(NULL);
         }
   } // NewObjPadraoRede

//---------------------------------------------------------------------------
__fastcall TPadraoRede::TPadraoRede(void)
   {
   PadraoRedeId  = 0;
   Descricao     = "";
   NumFases      = 0;
   VFF           = 0;
   VFN           = 0;
   V1            = 0;
   V2            = 0;
   V3            = 0;
   V4            = 0;
   FPI1          = 0;
   FPI2          = 0;
   } // TPadraoRede

//---------------------------------------------------------------------------
__fastcall TPadraoRede::~TPadraoRede(void)
   {
   // Nada a fazer
   } // ~TPadraoRede

//---------------------------------------------------------------------------
VTPadraoRede* __fastcall TPadraoRede::Clone(void)
   {
   //variáveis locais
   TPadraoRede *clone;

   //cria um novo objeto
   clone = new TPadraoRede();
   //copia dados deste objeto
   clone->PD = PD;
   return(clone);
   }


