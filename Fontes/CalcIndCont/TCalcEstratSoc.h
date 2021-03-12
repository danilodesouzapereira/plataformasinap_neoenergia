// ---------------------------------------------------------------------------
#ifndef TCalcEstratSocH
#define TCalcEstratSocH
// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTCalcEstratSoc.h"
// ---------------------------------------------------------------------------
class VTChave;
class VTEstratSoc;
class VTVisRede;

// ---------------------------------------------------------------------------
class TCalcEstratSoc : public VTCalcEstratSoc
{
public:
	__fastcall TCalcEstratSoc(void);
	__fastcall ~TCalcEstratSoc(void);
	bool __fastcall CalculaEstrategiaSoc(VTDefeito *pdef, int iPat, double pu_lim,
		TList *lisChavesAuto, TList *lisChavesAtivas);
	bool __fastcall IniciaEstrategiaSoc(VTDefeito *pdef, int iPat);

private:
	bool __fastcall ChaveAutomatica(VTChave *pchv);
	bool __fastcall ChaveAtiva(VTChave *pchv);
	void __fastcall OrdenaVisRedeCap(VTDefeito *pdef, int iPat, bool flagAuto);
	void __fastcall OrdenaVisRedeDeltaDEC(VTDefeito *pdef);
	void __fastcall VerificaVis(VTEstratSoc *estratSoc, VTVisRede *visRede);

protected: // métodos acessados via property

	TList* __fastcall PM_GetLisChavesAuto(void)
	{
		return (lisChavesAuto);
	};

	void __fastcall PM_SetLisChavesAuto(TList *lisChavesAuto)
	{
		this->lisChavesAuto = lisChavesAuto;
	};

	double __fastcall PM_GetPuLim(void)
	{
		return (pu_lim);
	};

	void __fastcall PM_SetPuLim(double pu_lim)
	{
		this->pu_lim = pu_lim;
	};

	TList* __fastcall PM_GetLisRelSaida(void)
	{
		return (lisRelSaida);
	};

	void __fastcall PM_SetLisRelSaida(TList *lisRelSaida)
	{
		this->lisRelSaida = lisRelSaida;
	};

private: // dados externos
	// n14 VTBuscaProt *buscaProt;

		private : // dados locais

	TList *lisChavesAuto; // lista de chaves automátivas (lista externa)
	TList *lisChavesAtivas; // lista de chaves ativadas hkaloca
	double pu_lim; // limite de transferencia
	TList *lisRelSaida; // lista de TStringList com estratégias de manobra testadas

public:

};

// ---------------------------------------------------------------------------
#endif
