// ---------------------------------------------------------------------------

#ifndef TBarraRadH
#define TBarraRadH
// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTBarraRad.h"

// ---------------------------------------------------------------------------
class TBarraRad : public VTBarraRad
{
public:
	__fastcall TBarraRad(VTBarra *pbarra);
	__fastcall ~TBarraRad(void);
	void __fastcall Inicia(void);

	// m�todos

		protected : // m�todos acessados via property

		private : // dados externos

		private : // dados locais

		public :
};

// ---------------------------------------------------------------------------
#endif
