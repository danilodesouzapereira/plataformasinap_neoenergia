// ---------------------------------------------------------------------------
#ifndef TLisDefeitoRedeH
#define TLisDefeitoRedeH
// ---------------------------------------------------------------------------
#include <Classes.hpp>
// ---------------------------------------------------------------------------
class VTRede;

// ---------------------------------------------------------------------------
class TLisDefeitoRede : public TObject
{
public:
	__fastcall TLisDefeitoRede(VTRede* rede);
	__fastcall ~TLisDefeitoRede(void);

	// métodos

		protected : // métodos acessados via property

		private : // dados externos

		private : // dados locais

		public : VTRede *rede;

	TList *lisDefeitos; // lista de defeitos da rede
};

// ---------------------------------------------------------------------------
#endif
