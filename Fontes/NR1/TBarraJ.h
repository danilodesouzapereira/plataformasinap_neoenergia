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
   TBarra   *barra;  // Barra original
   TYnodalJ *YJ;     // Ponteiro para objeto TYNodalJ em lisYJ

public:   // Métodos
          __fastcall  TBarraJ(TBarra *barra);
          __fastcall ~TBarraJ(void);
     void __fastcall  Reinicia(void);
};

#endif
//---------------------------------------------------------------------------

