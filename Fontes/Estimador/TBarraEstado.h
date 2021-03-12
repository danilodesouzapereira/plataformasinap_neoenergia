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
public:  // Métodos
				    __fastcall  TBarraEstado(TBarra *barra,
                                         bool   flag_equilibrado);
				    __fastcall ~TBarraEstado(void);

public:  // Dados
   TBarra   *barra;   // Barra da variável de estado
   bool     bar_ref;  // Indica se a barra é de referência para a Estimação
   TYnodalE **vet_YE; // Vetor que contém o objeto TYnodalE de cada par barra/fase
};

#endif
//---------------------------------------------------------------------------

