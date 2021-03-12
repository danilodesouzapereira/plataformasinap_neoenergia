//---------------------------------------------------------------------------
#ifndef TItemObraCOPELH
#define TItemObraCOPELH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTItemObraCOPEL.h"

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
class TItemObraCOPEL : public VTItemObraCOPEL
{
	public:
				__fastcall  TItemObraCOPEL(VTApl *apl);
				__fastcall ~TItemObraCOPEL(void);
		VTItemObra*     __fastcall Clone(bool preserveId, bool isDeep);
		void 	__fastcall 	CopiaAtributosDe(VTItemObra &ref);
		void 	__fastcall  CorrigeValores(int tipoEqpto);
		void 	__fastcall  IniciaExtras(void);
		void    __fastcall  LeIpeAtributo(VTIpeAtributo *atributo);
		void    __fastcall  MantemAtributosComunsDe(VTItemObra &ref);
		void    __fastcall  SalvaIpeAtributosProprios(void);
};

#endif
//---------------------------------------------------------------------------
//eof
