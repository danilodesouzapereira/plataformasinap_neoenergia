// ---------------------------------------------------------------------------
#ifndef TLisBlocoChaveH
#define TLisBlocoChaveH
// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTLisBlocoChave.h"

// ---------------------------------------------------------------------------
class TLisBlocoChave : public VTLisBlocoChave
{
public:
	__fastcall TLisBlocoChave(VTChave *pchv);
	__fastcall ~TLisBlocoChave(void);

	// m�todos

		protected : // m�todos acessados via property

		private : // dados externos

		private : // dados locais
		// AnsiString codigo;
		// TList *lisRedeViz;  //lista com todas as redes vizinhas

		public :
};

// ---------------------------------------------------------------------------
#endif
