
/*
**********************************************************************
*                                                                    *
*   Constantes_Calculo.h - Defini��o de constantes para o c�lculo    *
*                                                                    *
**********************************************************************
*/

#ifndef Constantes_CalculoH
#define Constantes_CalculoH

// Constantes de uso geral
#define ZEROV                   1.e-8  // N�mero muito pequeno - EVITAR ALTERAR ESTE VALOR!!
#define TOLER_CORRENTE          1.e-8  // Toler�ncia para assumir corrente de carga igual a zero

// CurtoCS e CurtoMA
#define REATANCIA_PEQUENA       1.e-6  // Reat�ncia para deprezar trafos (Zentrada s� com trechos de rede)

// Estimador1/Estimador3
#define SIGMA_PU_MINIMO         0.01  // [pu]
#define SIGMA_PU_MAXIMO       100.    // [pu]

// FluxoH3
#define TOLER_DH                1.e-6 // Toler�ncia para calcular distor��o harm�nica
#define DH_INFINITA         9999.9999 // Distor��o harm�nica "infinita"

#endif

