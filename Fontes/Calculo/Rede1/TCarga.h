//---------------------------------------------------------------------------
#ifndef TCargaH
#define TCargaH

//---------------------------------------------------------------------------
#include <Classes.hpp>

class TBarra;

//---------------------------------------------------------------------------
class TCarga : public TObject
{
public:  // Dados locais
   TBarra *barra;        // Barra da carga

public:  // Métodos
              __fastcall  TCarga(TBarra *barra);
              __fastcall ~TCarga(void);
};

#endif
//---------------------------------------------------------------------------

