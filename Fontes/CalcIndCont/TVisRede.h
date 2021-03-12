// ---------------------------------------------------------------------------
#ifndef TVisRedeH
#define TVisRedeH
// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTVisRede.h"

// ---------------------------------------------------------------------------
class TVisRede : public VTVisRede
{
public:
	__fastcall TVisRede(VTChave* pchave);
	__fastcall ~TVisRede(void);
	void __fastcall IniciaDadosDefeito(void);
	double __fastcall FolgaIA(int indPat);
	double __fastcall IA_Crit(int indPat);
	VTEqpto* __fastcall LigaCrit(int indPat);

	// métodos

		protected : // métodos acessados via property

		private : // dados externos

		private : // dados locais

		public :
};

// ---------------------------------------------------------------------------
#endif
