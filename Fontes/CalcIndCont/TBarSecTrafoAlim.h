// ---------------------------------------------------------------------------

#ifndef TBarSecTrafoAlimH
#define TBarSecTrafoAlimH
// ---------------------------------------------------------------------------
#include <Classes.hpp>
// ---------------------------------------------------------------------------
class VTBarra;
class VTTrafo;

// ---------------------------------------------------------------------------
class TBarSecTrafoAlim : public TObject
{
public:
	__fastcall TBarSecTrafoAlim(void);
	__fastcall ~TBarSecTrafoAlim(void);

	// métodos

		protected : // métodos acessados via property

		private : // dados externos

		private : // dados locais

		public : VTTrafo *trafo;

	VTBarra *barSecTrafo;
	TList *lisCircuitosBarSecTrafo;
	TList *lisChavesNATransf;
	double carga[24]; // carga em 24 patamares
};

// ---------------------------------------------------------------------------
#endif
