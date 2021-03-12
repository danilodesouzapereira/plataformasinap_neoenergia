//---------------------------------------------------------------------------
#pragma hdrstop
#include ".\TMedidor.h"
#include "..\Comum13\Constantes_Calculo.h"
#include "..\Comum13\VTGeralC.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TMedidor::TMedidor(void)
   {
   // Inicialização
   snp_canal      = NULL;
   codigo         = "";
   ligado         = true;
   enabled        = true;
   eqpto_id       = ID_INVALIDO;
   eqpto          = NULL;
   tipo_eqpto     = -1;
   tipo_medidor   = medREAL;
   tipo_medicao   = -1;
   id_calc        = -1;
   bar_ref        = NULL;
   fase_int       = -1;
   toler_pu       =  0.;
   sigma1_pu      =  0.;
   sigma2_pu      =  0.;
   val_fe_pu      =  0.;
   val_med_pu     =  0.;
   val_est_pu     =  0.;
   val_est_nom_pu =  0.;
   desvio_ndp     =  0.;
   }

//---------------------------------------------------------------------------
__fastcall TMedidor::~TMedidor(void)
   {
   }

//---------------------------------------------------------------------------
void __fastcall TMedidor::CalculaDesvioPadrao(double semi_intervalo_normal)
{
// Calcula desvio padrão do medidor
// Critério para considerar medição nula/não-nula:
//    ESTIM1: 0.3 kVA = 0.0003 MVA = 0.0003 / 100     = 3e-6 pu (pot. trifásica)
//    ESTIM3: 0.1 kVA = 0.0001 MVA = 0.0001 / (100/3) = 3e-6 pu (pot. por fase)
if(fabs(val_med_pu) > 3.e-6)  // Considera medição não-nula
   {
   sigma1_pu = toler_pu * val_fe_pu / semi_intervalo_normal;
   }
else  // Considera medição nula
   {
   val_med_pu = 0.;
   val_fe_pu  = 0.;
   sigma1_pu  = toler_pu;
   }
}

//---------------------------------------------------------------------------
void __fastcall TMedidor::CalculaFundoDeEscala1(double Vnom_linha_kV,
                                                double Sbase_3F_MVA,
                                                double semi_intervalo_normal,
                                                double valor_fe)
{
// Esta função atende ao Estimador1

// Variáveis locais
double ibase_A;

// Calcula valor de fundo de escala em pu
if(TipoMedicaoCorrente())
   {
   ibase_A   = 1000. * Sbase_3F_MVA / (sqrt(3.) * Vnom_linha_kV);
   val_fe_pu = valor_fe / ibase_A;
   }
else if(TipoMedicaoPotencia())
   {
   val_fe_pu = valor_fe / Sbase_3F_MVA;
   }
else  // Medidor de módulo de tensão (fundo de escala = tensão de base)
   {
   val_fe_pu = 1.;
   }
}

//---------------------------------------------------------------------------
void __fastcall TMedidor::CalculaFundoDeEscala3(double Vnom_fase_kV,
                                                double Sbase_fase_MVA,
                                                double semi_intervalo_normal,
                                                double valor_fe)
{
// Esta função atende ao Estimador3

// Variáveis locais
double ibase_A;

// Calcula valor de fundo de escala em pu
if(TipoMedicaoCorrente())
   {
   ibase_A   = 1000. * Sbase_fase_MVA / Vnom_fase_kV;
   val_fe_pu = valor_fe / ibase_A;
   }
else if(TipoMedicaoPotencia())
   {
   val_fe_pu = valor_fe / Sbase_fase_MVA;
   }
else  // Medidor de módulo de tensão (fundo de escala = tensão de base)
   {
   val_fe_pu = 1.;
   }
}

//---------------------------------------------------------------------------
bool __fastcall TMedidor::TipoMedicaoCorrente(void)
{
if(tipo_medicao == medMOD_I) return(true);
else                         return(false);
}

//---------------------------------------------------------------------------
bool __fastcall TMedidor::TipoMedicaoPotencia(void)
{
if((tipo_medicao == medP) ||
   (tipo_medicao == medQ) ||
   (tipo_medicao == medS)) return(true);
else                       return(false);
}

//---------------------------------------------------------------------------
bool __fastcall TMedidor::TipoMedicaoTensao(void)
{
if(tipo_medicao == medMOD_V) return(true);
else                         return(false);
}

//---------------------------------------------------------------------------

