
/*
**********************************************************************
*                                                                    *
*   Constantes_Calculo.h - Definição de constantes para o cálculo    *
*                                                                    *
**********************************************************************
*/

#ifndef Constantes_CalculoH
#define Constantes_CalculoH

// Constantes de uso geral
#define ZEROV                   1.e-8  // Número muito pequeno - EVITAR ALTERAR ESTE VALOR!!
#define TOLER_CORRENTE          1.e-8  // Tolerância para assumir corrente de carga igual a zero

// CurtoCS e CurtoMA
#define REATANCIA_PEQUENA       1.e-6  // Reatância para deprezar trafos (Zentrada só com trechos de rede)

// Estimador1/Estimador3
#define SIGMA_PU_MINIMO         0.01  // [pu]
#define SIGMA_PU_MAXIMO       100.    // [pu]

// FluxoH3
#define TOLER_DH                1.e-6 // Tolerância para calcular distorção harmônica
#define DH_INFINITA         9999.9999 // Distorção harmônica "infinita"

#endif

