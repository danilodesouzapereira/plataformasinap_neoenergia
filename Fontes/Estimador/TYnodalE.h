//---------------------------------------------------------------------------
#ifndef TYnodalEH
#define TYnodalEH

//---------------------------------------------------------------------------
#include <Classes.hpp>

class TBarra;

//---------------------------------------------------------------------------
class TYnodalE : public TObject
{
public:  // Métodos
				    __fastcall  TYnodalE(void);
				    __fastcall ~TYnodalE(void);

public:  // Dados
   TBarra *barra;       // Barra da variável de estado
   int    fase_no;      // Fase do nó (faseA, faseB, faseC ou faseN)
   int    num_var;      // Número de variáveis de estado da barra/fase (1 ou 2)
   int    *vet_linha_E; // Vetor que fornece o endereço do módulo (posição 0) e o
                        // endereço do ângulo (posição 1) da tensão da barra/fase
                        // em 'vet_var_estado'
};

#endif
//---------------------------------------------------------------------------

