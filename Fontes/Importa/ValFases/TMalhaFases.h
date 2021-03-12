//---------------------------------------------------------------------------

#ifndef TMalhaFasesH
#define TMalhaFasesH
//---------------------------------------------------------------------------
#include "..\Constante\Fases.h"
//arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------
class VTBarra;
class VTLigacao;
class VTMalha;
//---------------------------------------------------------------------------
class TMalhaFases : public TObject
{
	public:
				__fastcall  TMalhaFases(void);
				__fastcall ~TMalhaFases(void);
		void 	__fastcall  Executa(VTBarra *barraInicial, VTMalha *malha);
		bool	__fastcall  ExisteCaminho(VTBarra *barra, int indFase);

	private: //métodos
		void 	__fastcall  Executa(int indFase);
		int     __fastcall  FaseIndToFaseTag(int indFase);
		bool	__fastcall  LigacaoPermiteFase(VTLigacao *lig, int indFase, VTBarra *ref);

	public:
		VTBarra *barraInicio;
	private:
		VTMalha *malha;

		TList *lisLIGBARRAINI[MAX_FASE];
		TList *lisBARRA[MAX_FASE];
};
//-----------------------------------------------------------------------------
// eof
#endif
