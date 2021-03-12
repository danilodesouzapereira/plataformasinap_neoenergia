// ---------------------------------------------------------------------------
#ifndef TTema_SinalH
#define TTema_SinalH

// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTTema_Sinal.h"

// ---------------------------------------------------------------------------
class VTApl;
class VTCelula;
class VTRede;
class VTScg;

// ---------------------------------------------------------------------------
class TTema_Sinal : public VTTema_Sinal
{
public: // métodos
			__fastcall TTema_Sinal(VTApl *apl);
			__fastcall ~TTema_Sinal(void);
	void 	__fastcall ConverteLatLonGMStoSINAP(TList *lisSTR);
	void 	__fastcall Executa(TList *lisImpCelPlan);
	// void   __fastcall  Executa(TList *lisREDE, TList *lisCLASSE, int ind_pat);
	void 	__fastcall FaixaValoresCelPlanner(void);
	void 	__fastcall FaixaValoresDefault(void);
	// void   __fastcall  FaixaValoresSinal(void);
	// void   __fastcall  FaixaValoresQtdCelula(void);
	void 	__fastcall FaixaValoresUniforme(void);
	TList* 	__fastcall LisFaixa(void);
	void 	__fastcall MontaFaixaSinal(void);
	// void   __fastcall  MontaGraficoDegrade(TColor color);
	bool 	__fastcall MontaGraficoDegradeFaixa(TColor color1, TColor color2, bool linearDem = false);
	// bool   __fastcall  MontaGraficoDegradeFaixaLinear(TColor color1, TColor color2);
	void 	__fastcall MontaGraficoPadrao(void);
	// void   __fastcall  MontaGraficoSinalMax(void);
	int 	__fastcall TotalCelula(void);
	// double __fastcall  TotalDemanda_mva(void);

private: // métodos locais
	void 	__fastcall ConverteLatLonGMStoUTM(TList *lisSTR);
	void 	__fastcall ConverteLatLonGMStoLatLonSNP(TList *lisSTR);
	void 	__fastcall CalculaValorProprioCelula(void);
	// void   __fastcall CalculaValorTotalCelula(void);
	TColor 	__fastcall Degrade(TColor color, double valor);
	TColor 	__fastcall Degrade(TColor beginColor, TColor endColor, double valor);
	TColor 	__fastcall DegradeCinza(double valor);
	// void   __fastcall IniciaLisCarga(VTRede *rede, TList *lisCARGA);
	void 	__fastcall IniciaLisCelula(TList *lisImpCelPlan);
	void 	__fastcall IniciaLisFaixa(void);
	void 	__fastcall IniciaLisFaixaEnabled(void);
	// void   __fastcall MontaFaixaDemanda(void);
	// void   __fastcall NormalizaValorTotalCelula(void);
	void 	__fastcall RemoveCelulaNula(TList *lisORDENADA);

	// objetos externos
	VTApl *apl;

	// dados locais
	VTScg *scg;
	TList *lisEQP;
	TList *lisFAIXA;
	TList *lisFAIXA_ENABLED;
	bool degrade_ok;

	struct
	{
		int qtde_cel;
		double i_dbm;
	}

	total;
	bool lisCelula_iniciado;
};

// ---------------------------------------------------------------------------
#endif
// eof
