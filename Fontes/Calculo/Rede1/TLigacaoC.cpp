//---------------------------------------------------------------------------
#pragma hdrstop
#include ".\TLigacaoC.h"
#include ".\TCapSerie.h"
#include ".\TReg2.h"
#include ".\TTrafo2.h"
#include ".\TTrafo3.h"
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
   this->cod_lig  = cod_lig.SubString(0, 15);   // Garante tamanho máximo
   }

//---------------------------------------------------------------------------
__fastcall TLigacaoC::~TLigacaoC(void)
   {
   }

//---------------------------------------------------------------------------
VTLigacaoG1 * __fastcall TLigacaoC::Dados(TBarra **bar1,
                                          TBarra **bar2,
                                          TBarra **bar3,
                                          int    *sentido)
{
// Variáveis locais
TCapSerie   *caps1;
TTrafo2     *trafo2;
TTrafo3     *trafo3;
TTrafoEquiv *trafoequiv;
TTrafoZZ    *trafozz;
TTrecho     *trec1;
VTLigacaoG1 *ligG;

switch(tipo)
   {
   case CapSerie1:
      caps1 = (TCapSerie   *) plig;
      ligG  = (VTLigacaoG1 *) caps1;
      *bar1 = caps1->bar1;
      *bar2 = caps1->bar2;
      *bar3 = NULL;
      break;

   case Reg2:
   case Trafo2:
      trafo2 = (TTrafo2     *) plig;
      ligG   = (VTLigacaoG1 *) trafo2;
      *bar1  = trafo2->bar1;
      *bar2  = trafo2->bar2;
      *bar3  = NULL;
      break;

   case Trafo3:
      trafo3 = (TTrafo3     *) plig;
      ligG   = (VTLigacaoG1 *) trafo3;
      *bar1  = trafo3->bar1;
      *bar2  = trafo3->bar2;
      *bar3  = trafo3->bar3;
      break;

   case TrafoEquiv1:
      trafoequiv = (TTrafoEquiv *) plig;
      ligG       = (VTLigacaoG1 *) trafoequiv;
      *bar1      = trafoequiv->bar1;
      *bar2      = trafoequiv->bar2;
      *bar3      = NULL;
      break;

   case TrafoZZ1:
      trafozz = (TTrafoZZ    *) plig;
      ligG    = (VTLigacaoG1 *) trafozz;
      *bar1   = trafozz->barra;
      *bar2   = NULL;
      *bar3   = NULL;
      break;

   case Trecho1:
      trec1 = (TTrecho     *) plig;
      ligG  = (VTLigacaoG1 *) trec1;
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

