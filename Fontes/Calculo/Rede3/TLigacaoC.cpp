//---------------------------------------------------------------------------
#pragma hdrstop
#include ".\TLigacaoC.h"
#include ".\TCapSerie.h"
#include ".\TReg12.h"
#include ".\TReg32.h"
#include ".\TTrafo12.h"
#include ".\TTrafo32.h"
#include ".\TTrafo33.h"
#include ".\TTrafoEquiv.h"
#include ".\TTrafoZZ.h"
#include ".\TTrecho.h"
#include "..\Comum13\VTGeralC.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TLigacaoC::TLigacaoC(void      *plig,
                                int        tipo,
                                int        sentido,
                                AnsiString cod_bar2,
                                AnsiString cod_lig)
   {
   // Salva dados
   this->plig     = plig;
   this->tipo     = tipo;
   this->sentido  = sentido;
   this->cod_bar2 = cod_bar2.SubString(0, 15);  // Garante tamanho máximo
   this->cod_lig  = cod_lig.SubString(0, 11);   // Garante tamanho máximo
   }

//---------------------------------------------------------------------------
__fastcall TLigacaoC::~TLigacaoC(void)
   {
   }

//---------------------------------------------------------------------------
VTLigacaoG3 * __fastcall TLigacaoC::Dados(TBarra **bar1,
                                          TBarra **bar2,
                                          TBarra **bar3,
                                          int    *sentido)
{
// Variáveis locais
TCapSerie   *caps1;
TTrafo12    *trafo12;
TTrafo32    *trafo32;
TTrafo33    *trafo33;
TTrafoEquiv *trafoequiv;
TTrafoZZ    *trafozz;
TTrecho     *trec1;
VTLigacaoG3 *ligG;

switch(tipo)
   {
   case CapSerie3:
      caps1 = (TCapSerie   *) plig;
      ligG  = (VTLigacaoG3 *) caps1;
      *bar1 = caps1->bar1;
      *bar2 = caps1->bar2;
      *bar3 = NULL;
      break;

   case Reg12:
   case Trafo12:
      trafo12 = (TTrafo12    *) plig;
      ligG    = (VTLigacaoG3 *) trafo12;
      *bar1   = trafo12->bar1;
      *bar2   = trafo12->bar2;
      *bar3   = NULL;
      break;

   case Reg32:
   case Trafo32:
      trafo32 = (TTrafo32    *) plig;
      ligG    = (VTLigacaoG3 *) trafo32;
      *bar1   = trafo32->bar1;
      *bar2   = trafo32->bar2;
      *bar3   = NULL;
      break;

   case Trafo33:
      trafo33 = (TTrafo33    *) plig;
      ligG    = (VTLigacaoG3 *) trafo33;
      *bar1   = trafo33->bar1;
      *bar2   = trafo33->bar2;
      *bar3   = trafo33->bar3;
      break;

   case TrafoEquiv3:
      trafoequiv = (TTrafoEquiv *) plig;
      ligG       = (VTLigacaoG3 *) trafoequiv;
      *bar1      = trafoequiv->bar1;
      *bar2      = trafoequiv->bar2;
      *bar3      = NULL;
      break;

   case TrafoZZ3:
      trafozz = (TTrafoZZ    *) plig;
      ligG    = (VTLigacaoG3 *) trafozz;
      *bar1   = trafozz->barra;
      *bar2   = NULL;
      *bar3   = NULL;
      break;

   case Trecho3:
      trec1 = (TTrecho     *) plig;
      ligG  = (VTLigacaoG3 *) trec1;
      *bar1 = trec1->bar1;
      *bar2 = trec1->bar2;
      *bar3 = NULL;
      break;

   default:  // Erro
      ligG  = NULL;
      *bar1 = NULL;
      *bar2 = NULL;
      *bar3 = NULL;
      break;
   }

// Atribui sentido
if(ligG != NULL) *sentido = this->sentido;

return(ligG);
}

//---------------------------------------------------------------------------

