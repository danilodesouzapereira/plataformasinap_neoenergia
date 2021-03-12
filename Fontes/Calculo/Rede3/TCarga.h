//---------------------------------------------------------------------------
#ifndef TCargaH
#define TCargaH

//---------------------------------------------------------------------------
#include <Classes.hpp>

class TBarra;

//---------------------------------------------------------------------------
class TCarga : public TObject
{
public:  // Defini��o de dados locais

public:  // Dados locais
   int    id;            // ID da carga (Sinap)
   TBarra *barra;        // Barra da carga

public:  // M�todos
			  __fastcall  TCarga(int    id,
								 TBarra *barra);
              __fastcall ~TCarga(void);
};

#endif
//---------------------------------------------------------------------------

