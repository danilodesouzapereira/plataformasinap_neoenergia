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

public:  // M�todos
              __fastcall  TCarga(TBarra *barra);
              __fastcall ~TCarga(void);
};

#endif
//---------------------------------------------------------------------------

