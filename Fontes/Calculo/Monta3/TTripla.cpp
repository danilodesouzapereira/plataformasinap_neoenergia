
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include ".\TTripla.h"
#include ".\TNo3.h"
#include "..\Comum13\VTGeralC.h"
#include "..\Rede3\TBarra.h"
#include "..\Rede3\TFase.h"
#include "..\Rede3\TTrafo12.h"
#include "..\Rede3\TTrafo32.h"
#include "..\Rede3\TTrafo33.h"
#include "..\Rede3\VTLigacaoG3.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TTripla::TTripla(VTLigacaoG3 *trafo,
                            int         fase_comum,
                            TBarra      *bar1,
                            TBarra      *bar2,
                            TBarra      *bar3)
{
// Variáveis locais
int      ind_fase;
TNo3     *no;
TTrafo12 *trafo12;
TTrafo32 *trafo32;
TTrafo33 *trafo33;

// Salva dados fornecidos
this->trafo      = trafo;
this->fase_comum = fase_comum;

// Inicialização
grupo = NULL;

// Aloca memória
lisNO = new TList;

// Inclui barras do trafo
switch(trafo->TipoLigacao())
   {
   case Trafo12:
      trafo12  = (TTrafo12 *) trafo;
      bar1     = trafo12->bar1;
      bar2     = trafo12->bar2;
      ind_fase = bar1->fases->Indice(fase_comum);
      if(ind_fase >= 0)
         {
         no = new TNo3(bar1, ind_fase);
         lisNO->Add(no);
         }
      ind_fase = bar2->fases->Indice(fase_comum);
      if(ind_fase >= 0)
         {
         no = new TNo3(bar2, ind_fase);
         lisNO->Add(no);
         }
      break;

   case Trafo32:
      trafo32  = (TTrafo32 *) trafo;
      bar1     = trafo32->bar1;
      bar2     = trafo32->bar2;
      ind_fase = bar1->fases->Indice(faseN);
      if(ind_fase >= 0)
         {
         no = new TNo3(bar1, ind_fase);
         lisNO->Add(no);
         }
      ind_fase = bar2->fases->Indice(faseN);
      if(ind_fase >= 0)
         {
         no = new TNo3(bar2, ind_fase);
         lisNO->Add(no);
         }
      break;

   case Trafo33:
      if((bar1 == NULL) || (bar2 == NULL)) break;  // Proteção
      ind_fase = bar1->fases->Indice(faseN);
      if(ind_fase >= 0)
         {
         no = new TNo3(bar1, ind_fase);
         lisNO->Add(no);
         }
      ind_fase = bar2->fases->Indice(faseN);
      if(ind_fase >= 0)
         {
         no = new TNo3(bar2, ind_fase);
         lisNO->Add(no);
         }
      if(bar3 != NULL)  // Tripla verdadeira (3 barras)
         {
         ind_fase = bar3->fases->Indice(faseN);
         if(ind_fase >= 0)
            {
            no = new TNo3(bar3, ind_fase);
            lisNO->Add(no);
            }
         }
      break;

   default:  // Erro - nada a fazer
      break;
   } // switch()
}

//---------------------------------------------------------------------------
__fastcall TTripla::~TTripla(void)
{
// Variáveis locais
TNo3 *no;

for(int n=0; n < lisNO->Count; n++)
   {
   no = (TNo3 *) lisNO->Items[n];
   delete no;
   }
delete lisNO;
}

//---------------------------------------------------------------------------
bool __fastcall TTripla::NoComum(TTripla *tr1)
{
// Variáveis locais
TNo3 *no, *no1;

for(int n=0; n < lisNO->Count; n++)
   {
   no = (TNo3 *) lisNO->Items[n];
   for(int j=0; j < tr1->lisNO->Count; j++)
      {
      no1 = (TNo3 *) tr1->lisNO->Items[j];
      if((no->barra    == no1->barra) &&
         (no->ind_fase == no1->ind_fase)) return(true);
      }
   }

return(false);
}

//---------------------------------------------------------------------------

