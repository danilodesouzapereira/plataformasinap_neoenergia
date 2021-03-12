//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include ".\TLigacaoCH3.h"
#include "..\..\Calculo\Comum13\VTGeralC.h"
#include "..\..\Calculo\Rede3\TCapSerie.h"
#include "..\..\Calculo\Rede3\TFase.h"
#include "..\..\Calculo\Rede3\TLigacaoC.h"
#include "..\..\Calculo\Rede3\TReg12.h"
#include "..\..\Calculo\Rede3\TReg32.h"
#include "..\..\Calculo\Rede3\TTrafo12.h"
#include "..\..\Calculo\Rede3\TTrafo33.h"
#include "..\..\Calculo\Rede3\TTrafoEquiv.h"
#include "..\..\Calculo\Rede3\TTrafoZZ.h"
#include "..\..\Calculo\Rede3\TTrecho.h"
#include "..\..\Matriz\VTMatriz.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TLigacaoCH3::TLigacaoCH3(TLigacaoC *ligacao,
                                    int       num_harmonicas)
   {
   // Variáveis locais
   int         nelem1, nelem2, num_fases, offset;
   TCapSerie   *caps1;
   TTrecho     *trec1;
   TTrafo12    *trafo12;
   TTrafo32    *trafo32;
   TTrafo33    *trafo33;
   TTrafoEquiv *trafoequiv;
   TTrafoZZ    *trafozz;

   // Salva ligação
   this->ligacao = ligacao;

   // Determina número de fases, offset na ligação pai (TLigacaoC) e
   // tamanhos das alocações
   nelem1 = MAX_FASES_G3;  // Corrente, DH, DHT
   switch(ligacao->tipo)
      {
      case CapSerie3:
         caps1     = (TCapSerie *) ligacao->plig;
         num_fases = caps1->fases->NumFases();
         if(ligacao->sentido == 0) offset = 0;
         else                      offset = num_fases;
         nelem2 = MAX_FASES_G3;  // Perda por fase
         break;

      case Reg12:
      case Trafo12:
         trafo12 = (TTrafo12 *) ligacao->plig;
         if(ligacao->sentido == 0)
            {
            num_fases = trafo12->fases_pri->NumFases();
            offset    = 0;
            }
         else
            {
            num_fases = trafo12->fases_sec->NumFases();
            offset    = trafo12->fases_pri->NumFases();
            }
         nelem2 = 1;  // Perda total do trafo
         break;

      case Reg32:
      case Trafo32:
         trafo32 = (TTrafo32 *) ligacao->plig;
         if(ligacao->sentido == 0)
            {
            num_fases = trafo32->fases_pri->NumFases();
            offset    = 0;
            }
         else
            {
            num_fases = trafo32->fases_sec->NumFases();
            offset    = trafo32->fases_pri->NumFases();
            }
         nelem2 = MAX_FASES_G3;  // Perda por fase
         break;

      case Trafo33:
         trafo33 = (TTrafo33 *) ligacao->plig;
         if(ligacao->sentido == 0)
            {
            num_fases = trafo33->fases_pri->NumFases();
            offset    = 0;
            }
         else if(ligacao->sentido == 1)
            {
            num_fases = trafo33->fases_sec->NumFases();
            offset    = trafo33->fases_pri->NumFases();
            }
         else
            {
            num_fases = trafo33->fases_ter->NumFases();
            offset    = trafo33->fases_pri->NumFases() + trafo33->fases_sec->NumFases();
            }
         nelem2 = 1;  // Perda total do trafo
         break;

      case TrafoEquiv3:
         trafoequiv = (TTrafoEquiv *) ligacao->plig;
         if(ligacao->sentido == 0)
            {
            num_fases = trafoequiv->fases_pri->NumFases();
            offset    = 0;
            }
         else
            {
            num_fases = trafoequiv->fases_sec->NumFases();
            offset    = trafoequiv->fases_pri->NumFases();
            }
         nelem2 = MAX_FASES_G3;  // Perda por fase
         break;

      case TrafoZZ3:
         trafozz   = (TTrafoZZ *) ligacao->plig;
         num_fases = trafozz->fases->NumFases();
         nelem2    = MAX_FASES_G3;  // Perda por fase
         break;

      case Trecho3:
         trec1     = (TTrecho *) ligacao->plig;
         num_fases = trec1->fases->NumFases();
         if(ligacao->sentido == 0) offset = 0;
         else                      offset = num_fases;
         nelem2 = MAX_FASES_G3;  // Perda por fase
         break;

      default:  // Erro
         return;
      }

   // Salva número de fases e offset
   this->num_fases = num_fases;
   this->offset    = offset;

   // Aloca memória para vetores
   dht_corrente   = new double [nelem1];
   vef_corrente_A = new double [nelem1];

   // Aloca memória para matrizes de duas dimensões
   dh_corrente = NewObjMatrizD(num_harmonicas, nelem1);
   corrente_A  = NewObjMatrizC(num_harmonicas, nelem1);
   perda_kva   = NewObjMatrizC(num_harmonicas, nelem2);
   }

//---------------------------------------------------------------------------
__fastcall TLigacaoCH3::~TLigacaoCH3(void)
   {
   // Libera memória
   delete[] dht_corrente;
   delete[] vef_corrente_A;

   delete   dh_corrente;
   delete   corrente_A;
   delete   perda_kva;
   }

//---------------------------------------------------------------------------

