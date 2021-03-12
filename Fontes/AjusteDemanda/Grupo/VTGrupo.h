// ---------------------------------------------------------------------------
#ifndef VTGrupoH
#define VTGrupoH
// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include <PlataformaSinap\Fontes\Constante\Fases.h>
#include <complex>

// ---------------------------------------------------------------------------
class VTGrupo : public TObject
{
public:
	__fastcall VTGrupo(void){};
	__fastcall ~VTGrupo(void){};

	virtual void __fastcall Inicia(void) = 0;

public:
	int categoria;
	TList *lisCargas;
	TList *lisCargasAjus;
	double peso;
	double proporcaoP[MAX_FASE];
	double proporcaoQ[MAX_FASE];
	double p_car_ajus_mw[MAX_FASE], q_car_ajus_mvar[MAX_FASE];
	double fatcorP[MAX_FASE], fatcorQ[MAX_FASE];

	std::complex<double>s_car_ajus_mva[MAX_FASE]; // somantória da demanda das cargas ajustáveis
};

// ---------------------------------------------------------------------------
VTGrupo* __fastcall NewObjGrupo(void);
// ---------------------------------------------------------------------------
#endif
// eof
