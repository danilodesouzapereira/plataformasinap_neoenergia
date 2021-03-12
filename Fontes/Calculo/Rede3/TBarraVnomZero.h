//---------------------------------------------------------------------------
#ifndef TBarraVnomZeroH
#define TBarraVnomZeroH

//---------------------------------------------------------------------------
#include "..\Calculo\Comum13\VTGeralC.h"

class TBarra;

//---------------------------------------------------------------------------
class TBarraVnomZero : public TObject
{
public:  // Dados locais
   TBarra     *barra;
   AnsiString fases_vnz; // L = linha, A = fase A, B = fase B, C = fase C, N = neutro

public:  // Métodos
              __fastcall  TBarraVnomZero(TBarra *barra,
                                         char   char1);
              __fastcall ~TBarraVnomZero(void);
         void __fastcall  AddCodigo(char char1);
};

#endif
//---------------------------------------------------------------------------

