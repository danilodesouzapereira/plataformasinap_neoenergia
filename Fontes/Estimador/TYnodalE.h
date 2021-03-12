//---------------------------------------------------------------------------
#ifndef TYnodalEH
#define TYnodalEH

//---------------------------------------------------------------------------
#include <Classes.hpp>

class TBarra;

//---------------------------------------------------------------------------
class TYnodalE : public TObject
{
public:  // M�todos
				    __fastcall  TYnodalE(void);
				    __fastcall ~TYnodalE(void);

public:  // Dados
   TBarra *barra;       // Barra da vari�vel de estado
   int    fase_no;      // Fase do n� (faseA, faseB, faseC ou faseN)
   int    num_var;      // N�mero de vari�veis de estado da barra/fase (1 ou 2)
   int    *vet_linha_E; // Vetor que fornece o endere�o do m�dulo (posi��o 0) e o
                        // endere�o do �ngulo (posi��o 1) da tens�o da barra/fase
                        // em 'vet_var_estado'
};

#endif
//---------------------------------------------------------------------------

