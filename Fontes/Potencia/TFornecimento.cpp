//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.hpp>
#pragma hdrstop
#include "TFornecimento.h"
#include "..\Rede\VTGerador.h"
#include "..\Rede\VTNo.h"
#include "..\Rede\VTSuprimento.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe
//---------------------------------------------------------------------------
VTFornecimento* __fastcall NewObjFornecimento(VTApl *apl)
   {
   try{
      return(new TFornecimento(apl));
      }catch(Exception &e)
         {//nada a fazer
         }
   //erro na criação do objeto
   return(NULL);
   }

//---------------------------------------------------------------------------
__fastcall TFornecimento::TFornecimento(VTApl *apl)
                         :TForBasico(apl)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
__fastcall TFornecimento::~TFornecimento(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
bool __fastcall TFornecimento::GetPQVTNo(VTNo *no, int np, strSUP vet_sup[MAX_FASE])
   {
   //variáveis locais
   int          nelem;
   VTGerador    *gerador;
   VTSuprimento *suprimento;
   int          tiposup[] = {supVF, supPV, supPQ};

   //insere primeiro Suprimento ou Gerador do tipo supVF ou supPV
   nelem = sizeof(tiposup) / sizeof(int);
   for (int nt = 0; nt < nelem; nt++)
      {//insere primeiro Suprimento do tiposup[nt] que encontrar
      for (int n = 0; n < no->lisSUP->Count; n++)
         {
         suprimento = (VTSuprimento*)no->lisSUP->Items[n];
         if (suprimento->tiposup == tiposup[nt])
            {
            if(suprimento->DefinidoPorFase) GetPQVTSuprimentoDeseq(suprimento, np, vet_sup);
            else                            GetPQVTSuprimentoEquil(suprimento, np, vet_sup);
            //return(true);
            }
         }
      //insere primeiro Gerador do tiposup[nt] que encontrar
      for (int n = 0; n < no->lisGER->Count; n++)
         {
         gerador = (VTGerador*)no->lisGER->Items[n];
         if (gerador->tiposup == tiposup[nt])
            {
            if(gerador->DefinidoPorFase) GetPQVTGeradorDeseq(gerador, np, vet_sup);
            else                         GetPQVTGeradorEquil(gerador, np, vet_sup);
            //return(true);
            }
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
//eof
