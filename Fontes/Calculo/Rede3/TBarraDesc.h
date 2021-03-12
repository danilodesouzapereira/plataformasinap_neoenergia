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
   int    num_fases;   // N�mero de fases em 'vet_fases'
   int    *vet_fases;  // Vetor que cont�m as fases desconexas da barra 'barra'

public:  // M�todos
              __fastcall  TBarraDesc(TBarra *barra);
              __fastcall ~TBarraDesc(void);
         void __fastcall  AddFase(int fase_int);
};

#endif
//---------------------------------------------------------------------------

