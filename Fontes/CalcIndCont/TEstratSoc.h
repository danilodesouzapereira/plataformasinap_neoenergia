// ---------------------------------------------------------------------------
#ifndef TEstratSocH
#define TEstratSocH
// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTEstratSoc.h"

// ---------------------------------------------------------------------------
class TEstratSoc : public VTEstratSoc
{
public:
	__fastcall TEstratSoc(int npat);
	__fastcall ~TEstratSoc(void);
	bool __fastcall AdicionaVisRedeSoc(VTVisRede *visRede, TList *lisBlocosSoc, TList *lisBlRadJus);
	bool __fastcall AdicionaVisRedeSoc1(VTVisRede *visRede, VTBlocoRad *blRadRapida,
		VTBlocoRad *blRadLenta);
	bool __fastcall BlocoSoc(VTBloco *bloco);
	void __fastcall Inicia(void);
	void __fastcall ListaVisRede(TList *lisEXT);
	void __fastcall ListaRedeTransf(TList *lisEXT);
	void __fastcall ListaChvIsola(TList *lisEXT);

protected: // métodos acessados via property
	int __fastcall PM_GetNBlocosSoc(void);

	VTDefeito* __fastcall PM_GetDefeito(void)
	{
		return (pdef);
	};

	void __fastcall PM_SetDefeito(VTDefeito* pdef)
	{
		this->pdef = pdef;
	};

private: // dados externos

	VTDefeito *pdef;

	// dados locais

		public :

};

// ---------------------------------------------------------------------------
#endif
