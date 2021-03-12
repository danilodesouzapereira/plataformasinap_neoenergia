// ---------------------------------------------------------------------------
#ifndef TMostraGrafIndicH
#define TMostraGrafIndicH
// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include <stdio.h>
#include "VTMostraGrafIndic.h"

// ---------------------------------------------------------------------------
class TMostraGrafIndic : public VTMostraGrafIndic
{
public:
	__fastcall TMostraGrafIndic(VTApl *apl_owner);
	__fastcall ~TMostraGrafIndic(void);
	void __fastcall MostraContribBloco(int indic, AnsiString titulo, double fatorEsc);
	void __fastcall MostraContribTransf(TList *lisDefeitos);

private: // métodos
		protected : // métodos acessados via property

	VTResCont* __fastcall PM_GetResCont(void)
	{
		return (resCont);
	};

	void __fastcall PM_SetResCont(VTResCont *resCont)
	{
		this->resCont = resCont;
	};

private: // dados locais

	VTApl *apl;
	VTResCont *resCont;

public:
};
// ---------------------------------------------------------------------------
#endif
// eof
