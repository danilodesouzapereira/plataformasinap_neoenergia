//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TStkRegulador.h"
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Rede\VTRegulador.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTStkRegulador* __fastcall NewObjStkRegulador(VTApl *apl_owner)
   {
   try{//cria objeto
      return(new TStkRegulador(apl_owner));
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TStkRegulador::TStkRegulador(VTApl *apl_owner)
	{
   //salva ponteiro para objeto
   this->apl = apl_owner;
	//obtém informação da memória
	}

//---------------------------------------------------------------------------
__fastcall TStkRegulador::~TStkRegulador(void)
   {
	//nada a fazer
	}

//---------------------------------------------------------------------------
bool __fastcall TStkRegulador::AlteraPasso(VTRegulador *regulador, int passo)
   {
   //verifica se o Regulador possui passo diferente do indicado
   if (regulador->cfg_fixo.passo == passo) return(true);
   //verifica se o passo está dentro do intervalo
   if      ((passo >= 0) && (passo    > regulador->num_passo)) return(false);
   else if ((passo <  0) && ((-passo) > regulador->num_passo)) return(false);
   //redefine o passo
   regulador->cfg_fixo.passo = passo;

   return(true);
   }

//-----------------------------------------------------------------------------
//eof



