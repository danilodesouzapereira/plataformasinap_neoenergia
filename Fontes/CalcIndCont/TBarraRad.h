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

	// métodos

		protected : // métodos acessados via property

		private : // dados externos

		private : // dados locais

		public :
};

// ---------------------------------------------------------------------------
#endif
