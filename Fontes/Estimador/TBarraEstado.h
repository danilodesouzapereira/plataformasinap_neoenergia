//---------------------------------------------------------------------------
#ifndef TBarraEstadoH
#define TBarraEstadoH

//---------------------------------------------------------------------------
#include <Classes.hpp>

class TBarra;
class TYnodalE;

//---------------------------------------------------------------------------
class TBarraEstado : public TObject
{
public:  // M�todos
				    __fastcall  TBarraEstado(TBarra *barra,
                                         bool   flag_equilibrado);
				    __fastcall ~TBarraEstado(void);

public:  // Dados
   TBarra   *barra;   // Barra da vari�vel de estado
   bool     bar_ref;  // Indica se a barra � de refer�ncia para a Estima��o
   TYnodalE **vet_YE; // Vetor que cont�m o objeto TYnodalE de cada par barra/fase
};

#endif
//---------------------------------------------------------------------------

