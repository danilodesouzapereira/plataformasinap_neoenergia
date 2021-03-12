//---------------------------------------------------------------------------
#ifndef TBarraJH
#define TBarraJH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <complex>
#include <vcl.h>

using namespace std;

class TBarra;
class TYnodalJ;

//---------------------------------------------------------------------------
class TBarraJ : public TObject
{
public:  // Dados locais
   TBarra   *barra;        // Barra original
   int      fase_int_ref;  // Fase de referência (somente para barras PV)
   TYnodalJ **vet_YJ;      // Vetor de ponteiros para objetos TYNodalJ em lisYJ
   double   *vet_vnom_inv; // Vetor contendo o inverso da tensão nominal em cada fase [1/V]

public:   // Métodos
          __fastcall  TBarraJ(TBarra *barra);
          __fastcall ~TBarraJ(void);
     void __fastcall  Reinicia(void);
};

#endif
//---------------------------------------------------------------------------

