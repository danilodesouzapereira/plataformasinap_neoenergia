// ---------------------------------------------------------------------------
#ifndef TCargaBlocoH
#define TCargaBlocoH
// ---------------------------------------------------------------------------
#include <Classes.hpp>
// ---------------------------------------------------------------------------
class VTBloco;

// ---------------------------------------------------------------------------
class TCargaBloco : public TObject
{
public:
	__fastcall TCargaBloco(VTBloco *bloco, int npat);
	__fastcall ~TCargaBloco(void);

	// m�todos

		protected : // m�todos acessados via property

		private : // dados externos

		private : // dados locais

		public : VTBloco *bloco; // bloco em falha

	double *cargaMW;
	int ncons;
};

// ---------------------------------------------------------------------------
#endif
