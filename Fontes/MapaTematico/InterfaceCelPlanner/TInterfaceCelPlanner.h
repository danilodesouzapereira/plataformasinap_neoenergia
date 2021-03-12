// ---------------------------------------------------------------------------
#ifndef TInterfaceCelPlannerH
#define TInterfaceCelPlannerH

// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTInterfaceCelPlanner.h"
#include "..\Rede\Estrutura.h"

// ---------------------------------------------------------------------------
class VTRadial;
class VTPrimario;
class VTSecundario;
class VTTrafo;

// ---------------------------------------------------------------------------
class TInterfaceCelPlanner : public VTInterfaceCelPlanner
{
public:
	__fastcall TInterfaceCelPlanner(VTApl *apl);
	__fastcall ~TInterfaceCelPlanner(void);
	strCPAntena __fastcall AntenaDefault(void);
	AnsiString __fastcall AntenaDefaultAsString(void);
	bool __fastcall ExportaArqCelPlanner(TStrings *lines);
	bool __fastcall ExportaArqCelPlannerBarras(TStrings *lines, TList *lisBarras);


private: // métodos
	void __fastcall CriaCabecalho(int nSb);
	bool __fastcall ExportaBlocoBarras(TList *listaBarras);
	bool __fastcall ExportaBlocoCargas(TList *listaBarras);
	bool __fastcall ValidaCodigosEqptos(void);
	bool __fastcall ExportaBlocoCapacitores(TList *listaBarras);
	bool __fastcall ExportaBlocoChaves(TList *listaLigacoes);
	bool __fastcall ExportaBlocoGeradores(TList *listaBarras);
	bool __fastcall ExportaBlocoReguladores(TList *listaLigacoes);
	bool __fastcall ExportaBlocoSubestacoes(TList *listaRedes);
	bool __fastcall ExportaBlocoTrafos(TList *listaLigacoes);

	// dados externos
	VTApl *apl;

	// dados locais
	VTRadial *radial;
	TList *lisEQP;
	TList *lisBARRAS;
	TList *lisSEDS;
	TList *lisLIG;
	TList *lisRedesCarregadas; // FKM 2016.04.27
	TStrings *lines;
	TStringList *lines_eqptos;
	int maiorId;
};

#endif
// ---------------------------------------------------------------------------
// eof
