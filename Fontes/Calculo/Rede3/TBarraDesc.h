//---------------------------------------------------------------------------
#ifndef TBarraDescH
#define TBarraDescH

//---------------------------------------------------------------------------
#include "..\Calculo\Comum13\VTGeralC.h"

class TBarra;

//---------------------------------------------------------------------------
class TBarraDesc : public TObject
{
public:  // Dados locais
   TBarra *barra;
   int    num_fases;   // Número de fases em 'vet_fases'
   int    *vet_fases;  // Vetor que contém as fases desconexas da barra 'barra'

public:  // Métodos
              __fastcall  TBarraDesc(TBarra *barra);
              __fastcall ~TBarraDesc(void);
         void __fastcall  AddFase(int fase_int);
};

#endif
//---------------------------------------------------------------------------

