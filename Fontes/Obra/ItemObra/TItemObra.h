// ---------------------------------------------------------------------------
#ifndef TItemObraH
#define TItemObraH

// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTItemObra.h"

// ---------------------------------------------------------------------------
class VTApl;

// ---------------------------------------------------------------------------
class TItemObra : public VTItemObra
{
	public:
						__fastcall TItemObra(VTApl *apl);
		virtual 		__fastcall ~TItemObra(void);
		VTItemObra*     __fastcall Clone(bool preserveId, bool isDeep);
				void    __fastcall CopiaAtributosDe(VTItemObra &ref);
				void 	__fastcall CorrigeValores(int tipoEqpto);
				void    __fastcall IniciaExtras(void);
                void    __fastcall 	InsereAcao(VTAcao *acao);
				void    __fastcall InsereIpeAtributo(VTIpeAtributo *atributo);
				void    __fastcall LeIpeAtributos(void);
				void    __fastcall LeIpeAtributo(VTIpeAtributo *atributo);
				void    __fastcall MantemAtributosComunsDe(VTItemObra &ref);
				void    __fastcall SalvaIpeAtributos(void);
				void    __fastcall SalvaIpeAtributosProprios(void);
				void    __fastcall ReiniciaValores(int tipoEqpto);
	protected:
		VTIpeAtributo* ExisteIpeAtributoSemelhante(VTIpeAtributo *atributo);
		void __fastcall AssociaAcoesClone(VTItemObra *clone);
};

#endif
// ---------------------------------------------------------------------------
// eof
