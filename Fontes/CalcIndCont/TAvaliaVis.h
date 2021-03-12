// ---------------------------------------------------------------------------
#ifndef TAvaliaVisH
#define TAvaliaVisH
// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTAvaliaVis.h"
// ---------------------------------------------------------------------------
class VTEqpto;

// ---------------------------------------------------------------------------
class TAvaliaVis : public VTAvaliaVis
{
public:
	__fastcall TAvaliaVis(VTApl *apl);
	__fastcall ~TAvaliaVis(void);
	bool __fastcall ExecutaVisRede(TList *lisVisRede, bool flagFluxo = false);
	// hkv201609-Sai função bool __fastcall Executa(TList *lisChvVis, bool flagFluxo = false);

private: // métodos
	bool __fastcall ExecutaFluxo(void);
	void __fastcall Folga(double &dfolga, double &capNom, double &ifluxo, VTEqpto *pliga,
		int num_pat);
	void __fastcall ImprimeResFluxo(void);

protected: // métodos acessados via property

	int __fastcall PM_GetIPat(void)
	{
		return (iPat);
	};

	void __fastcall PM_SetIPat(int iPat)
	{
		this->iPat = iPat;
	};

	bool __fastcall PM_GetFlagAvaliaChave(void)
	{
		return (flagAvaliaChave);
	};

	void __fastcall PM_SetFlagAvaliaChave(bool flag)
	{
		flagAvaliaChave = flag;
	};

private: // objetos externos

	VTApl *apl;
	int iPat; // Patamar de cálculo do fluxo de potencia, se = -1: todos os patamares
	bool flagAvaliaChave;

	// dados locais
		public :
};

// ---------------------------------------------------------------------------
#endif
// eof
