
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include ".\TGrupoTripla.h"
#include ".\TNo3.h"
#include ".\TTripla.h"
#include "..\Rede3\TBarra.h"
#include "..\Rede3\TFase.h"
#include "..\Comum13\VTGeralC.h"
#include "..\..\Complexo\Complexo.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TGrupoTripla::TGrupoTripla(TTripla *tripla)
{
// Inicializa��o
tipo_carga     = true;   // Grupo de n�s de barras de carga
aterr_atSolido = false;  // Grupo n�o tem aterramento atSolido

// Define se o grupo � de triplas de neutro ou de fase, em fun��o
// da fase comum da primeira tripla
if(tripla->fase_comum == faseN) neutro = true;
else                            neutro = false;

// Cria e inicializa lista de triplas
lisTRIPLA = new TList;
AddTripla(tripla);
}

//---------------------------------------------------------------------------
__fastcall TGrupoTripla::~TGrupoTripla(void)
{
// As triplas s�o destru�das em TMonta3::AtribuiLinhasY(),
// quando da destrui��o de 'lisTRIPLA_GLOBAL'
delete lisTRIPLA;
}

//---------------------------------------------------------------------------
void __fastcall TGrupoTripla::AddTripla(TTripla *tripla)
{
// Vari�veis locais
int    fase_int, ind_fase;
TBarra *barra;
TNo3   *no;

// Adiciona tripla � lista
lisTRIPLA->Add(tripla);

// Define grupo da tripla
tripla->grupo = this;

// Desliga 'tipo_carga' do grupo se houver pelo menos um n� de gera��o
// na tripla que est� sendo adicionada
if(! tipo_carga) return;  // J� existe n� de gera��o neste grupo
for(int nn=0; nn < tripla->lisNO->Count; nn++)
   {
   no       = (TNo3 *) tripla->lisNO->Items[nn];
   barra    = no->barra;
   ind_fase = no->ind_fase;
   fase_int = barra->fases->Fase(ind_fase);
   if(barra->IsNoGeracao(fase_int))
      {
      tipo_carga = false;
      return;
      }
   }
}

//---------------------------------------------------------------------------
void __fastcall TGrupoTripla::ImpoeAterramentoMaisSignificativo(void)
{
// Vari�veis locais
int             tipo_ater_mais;
complex<double> yater_mais;
complex<double> czero(0., 0.);
TBarra          *bar1;
TNo3            *no;
TTripla         *tripla;

// IMPORTANTE: � IMPRESCIND�VEL QUE OS TIPOS DE ATERRAMENTO ESTEJAM DEFINIDOS
//             EM ORDEM CRESCENTE DE SIGNIFIC�NCIA (cf. 'enum_TIPO_ATERR' em
//             'VTGeralC.h')

// Prote��o: descarta grupo de triplas de fase
if(! neutro) return;

// Loop de triplas para identificar o aterramento mais significativo
tipo_ater_mais = atIsolado;  // Inicializa com o tipo menos significativo
yater_mais     = czero;      // Imped�ncia infinita
for(int nt=0; nt < lisTRIPLA->Count; nt++)
   {
   tripla = (TTripla *) lisTRIPLA->Items[nt];
   for(int nn=0; nn < tripla->lisNO->Count; nn++)
      {
      no   = (TNo3 *) tripla->lisNO->Items[nn];
      bar1 = no->barra;
      if(bar1->tipo_ater >= tipo_ater_mais) // "=" para permitir achar a menor imped�ncia
         {
         tipo_ater_mais = bar1->tipo_ater;
         if(Abs(bar1->yater_pu) > Abs(yater_mais)) yater_mais = bar1->yater_pu;
         }
      }
   }

// Verifica se o aterramento definido para o grupo � atSolido
if(tipo_ater_mais == atSolido)
   {
   aterr_atSolido = true;   // Liga flag
   yater_mais     = czero;  // Para manter coer�ncia
   }

// Atualiza aterramento das barras em todas as triplas do grupo
// IMPORTANTE: o aterramento dos trafos n�o � atualizado aqui - isso � feito
//             mais adiante, atrav�s da fun��o
//             TMonta3::RedefineAterramentoTrafos()
for(int nt=0; nt < lisTRIPLA->Count; nt++)
   {
   tripla = (TTripla *) lisTRIPLA->Items[nt];
   for(int nn=0; nn < tripla->lisNO->Count; nn++)
      {
      no              = (TNo3 *) tripla->lisNO->Items[nn];
      bar1            = no->barra;
      bar1->tipo_ater = tipo_ater_mais;
      bar1->yater_pu  = yater_mais;
      } // for(nn)
   } // for(nt)
}

//---------------------------------------------------------------------------

