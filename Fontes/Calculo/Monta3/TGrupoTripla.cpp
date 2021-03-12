
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
// Inicialização
tipo_carga     = true;   // Grupo de nós de barras de carga
aterr_atSolido = false;  // Grupo não tem aterramento atSolido

// Define se o grupo é de triplas de neutro ou de fase, em função
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
// As triplas são destruídas em TMonta3::AtribuiLinhasY(),
// quando da destruição de 'lisTRIPLA_GLOBAL'
delete lisTRIPLA;
}

//---------------------------------------------------------------------------
void __fastcall TGrupoTripla::AddTripla(TTripla *tripla)
{
// Variáveis locais
int    fase_int, ind_fase;
TBarra *barra;
TNo3   *no;

// Adiciona tripla à lista
lisTRIPLA->Add(tripla);

// Define grupo da tripla
tripla->grupo = this;

// Desliga 'tipo_carga' do grupo se houver pelo menos um nó de geração
// na tripla que está sendo adicionada
if(! tipo_carga) return;  // Já existe nó de geração neste grupo
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
// Variáveis locais
int             tipo_ater_mais;
complex<double> yater_mais;
complex<double> czero(0., 0.);
TBarra          *bar1;
TNo3            *no;
TTripla         *tripla;

// IMPORTANTE: É IMPRESCINDÍVEL QUE OS TIPOS DE ATERRAMENTO ESTEJAM DEFINIDOS
//             EM ORDEM CRESCENTE DE SIGNIFICÂNCIA (cf. 'enum_TIPO_ATERR' em
//             'VTGeralC.h')

// Proteção: descarta grupo de triplas de fase
if(! neutro) return;

// Loop de triplas para identificar o aterramento mais significativo
tipo_ater_mais = atIsolado;  // Inicializa com o tipo menos significativo
yater_mais     = czero;      // Impedância infinita
for(int nt=0; nt < lisTRIPLA->Count; nt++)
   {
   tripla = (TTripla *) lisTRIPLA->Items[nt];
   for(int nn=0; nn < tripla->lisNO->Count; nn++)
      {
      no   = (TNo3 *) tripla->lisNO->Items[nn];
      bar1 = no->barra;
      if(bar1->tipo_ater >= tipo_ater_mais) // "=" para permitir achar a menor impedância
         {
         tipo_ater_mais = bar1->tipo_ater;
         if(Abs(bar1->yater_pu) > Abs(yater_mais)) yater_mais = bar1->yater_pu;
         }
      }
   }

// Verifica se o aterramento definido para o grupo é atSolido
if(tipo_ater_mais == atSolido)
   {
   aterr_atSolido = true;   // Liga flag
   yater_mais     = czero;  // Para manter coerência
   }

// Atualiza aterramento das barras em todas as triplas do grupo
// IMPORTANTE: o aterramento dos trafos não é atualizado aqui - isso é feito
//             mais adiante, através da função
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

