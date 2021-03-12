//---------------------------------------------------------------------------
#pragma hdrstop
#include ".\TBarra.h"
#include "..\Comum13\Constantes_Calculo.h"
#include "..\Comum13\TBateriaC.h"
#include "..\Comum13\VTGeralC.h"
#include "..\..\Complexo\Complexo.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TBarra::TBarra()
   {
   complex<double> czero(0., 0.);

   flag_conexa     = true;        // Em princípio, barra conexa
   mantida         = false;       // Em princípio, barra não será mantida (Redutor1)
   sup_equivalente = false;       // Em princípio, não será calculado suprimento equivalente na barra
   tipo            = BAR_PQ;      // Assume barra PQ
   tipo_original   = BAR_PQ;
   id              = ID_INVALIDO; // Inicialização
   offset          = -1;
   linhaY          = -1;
   eqpto           = NULL;
   ang_ref_rad     = 0.;
   vnom_kv         = 0.;
   yref_reduc0_pu  = czero;
   yref_reduc1_pu  = czero;
   yref_soma0_pu   = czero;
   yref_soma1_pu   = czero;
   scalc_mva       = czero;
   tensao_pu       = complex<double>(1., 0.);
   sup             = NULL;
   lisLIG          = NULL;
   lisBATERIA      = NULL;
   ZeraCarga();
   ZeraCapacitorReator(0);
   ZeraCapacitorReator(1);
   }

//---------------------------------------------------------------------------
__fastcall TBarra::~TBarra(void)
   {
   TObject *pobj;

   // Destroi lisLIG
   if(lisLIG != NULL)
      {
      for(int nl=0; nl < lisLIG->Count; nl++)
         {
         pobj = (TObject *) lisLIG->Items[nl];
         delete pobj;
         }
      delete lisLIG;
      }

   // Destroi lisBATERIA
   delete lisBATERIA;  // NB: os objetos TBateria são destruídos em Trede1
   }

//---------------------------------------------------------------------------
void __fastcall TBarra::AddBateria(TBateriaC *bateria)
{
if(lisBATERIA == NULL) lisBATERIA = new TList;
lisBATERIA->Add(bateria);
}

//---------------------------------------------------------------------------
complex<double> __fastcall TBarra::CalculaCorrente(double          Sbase_mva,
                                                   complex<double> v1)
{
// Variáveis locais
double          vmod;
complex<double> czero(0., 0.);
complex<double> corr, s_pu;

// Proteção
vmod = Abs(v1, ZEROV);
if(vmod == 0.) return(czero);

// Calcula corrente absorvida pela carga na tensão atual
s_pu = (vet_carga_pu[Icte] * vmod) +
        vet_carga_pu[Scte]         +
       (vet_carga_pu[Zcte] * vmod * vmod);
corr = conj(s_pu / v1);

// Inclui corrente absorvida por capacitores e reatores instalados na barra
corr += v1 * complex<double>(0., best1_pu);

// Retorna
return(corr);
}

//---------------------------------------------------------------------------
bool __fastcall TBarra::CargaNula(void)
{
// Variáveis locais
TBateriaC *batc1;

// Verifica cargas regulares
for(int nmo=0; nmo < NUM_MODELO_CARGA; nmo++)
   {
   if(Abs(vet_carga_pu[nmo], TOLER_CORRENTE) > 0.) return(false);
   }

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TBarra::TipoCarga(void)
   {
   bool teste;

   teste = (tipo == BAR_PQ) || (tipo == BAR_EXT) || (tipo == BAR_PQV);
   return(teste);
   }

//---------------------------------------------------------------------------
bool __fastcall TBarra::TipoGeracao(void)
   {
   return(! TipoCarga());
   }

//---------------------------------------------------------------------------
void __fastcall TBarra::ZeraCapacitorReator(int seq)
   {
   if     (seq == 0) best0_pu = 0.;
   else if(seq == 1) best1_pu = 0.;
   }

//---------------------------------------------------------------------------
void __fastcall TBarra::ZeraCarga(void)
   {
   complex<double> czero(0., 0.);

   for(int nm=0; nm < NUM_MODELO_CARGA; nm++)
      {
      vet_carga_pu[nm] = czero;
      }
   scalc_mva = czero;
   }

//---------------------------------------------------------------------------





