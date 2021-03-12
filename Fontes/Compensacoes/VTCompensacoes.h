//---------------------------------------------------------------------------
#ifndef VTCompensacoesH
#define VTCompensacoesH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTSubestacao;
class VTCarga;
class TConjuntoAneel;

//---------------------------------------------------------------------------
struct strParamComp
	{
	double TUSD_B1;
	double TUSD_B2;
	double TUSD_B3;
	double TUSD_BT;
	double TUSD_Final;
	double NaoUrbanoDICAnual;
	double NaoUrbanoFICAnual;
	double UrbanoDICAnual;
	double UrbanoFICAnual;

	double kei;
	double energia;
	};

//---------------------------------------------------------------------------
struct strValores : public TObject
	{
	double Anual, Trimestral, Mensal;
	};

//---------------------------------------------------------------------------
struct strLimite : public TObject
	{
	public:
		__fastcall strLimite(void);
		__fastcall ~strLimite(void);

	public:
		strValores *DIC, *FIC;
		double     DMIC;
	};

//---------------------------------------------------------------------------
struct strLimites : public TObject
	{
	public:
		__fastcall strLimites(void);
		__fastcall ~strLimites(void);

	public:
		strLimite *BT, *MT;
	};

//---------------------------------------------------------------------------
struct strFaixa : public TObject
	{
	public:
		__fastcall strFaixa(void);
		__fastcall ~strFaixa(void);

	public:
		AnsiString Referencia;
		strLimites *Urbano, *NaoUrbano;
	};

//---------------------------------------------------------------------------
enum eTipoPeriodo {etpMensal = 0, etpTrimestral, etpAnual};

enum eTipoLocal {etlUrbano = 0, etlNaoUrbano};

enum eTipoNivelTensao {etntBT = 0, etntMT, etntAT};

//---------------------------------------------------------------------------
class VTCompensacoes : public TObject
	{
	public:
										__fastcall VTCompensacoes(void) {};
										__fastcall ~VTCompensacoes(void) {};
//		virtual double          __fastcall Calcula(VTSubestacao* SE, VTCarga* Carga, double DIC, double FIC, double DMIC = 0., double DICRI = 0., eTipoPeriodo Periodo = etpAnual) = 0;
		virtual double 			__fastcall Calcula(VTSubestacao* SE, VTCarga* Carga, double DIC, double FIC, double DMIC = 0. , eTipoPeriodo Periodo = etpAnual, int index = 0) = 0;
		virtual double          __fastcall Calcula(VTSubestacao* SE, VTCarga* Carga, double *DICsMensais, double *FICsMensais) = 0;
		virtual bool            __fastcall Inicializa(AnsiString dirEmpresa) = 0;
		virtual TConjuntoAneel* __fastcall RetornaConjuntoAneel(VTSubestacao* SE) = NULL;
		virtual strFaixa*       __fastcall RetornaFaixaDEC(TConjuntoAneel* Conjunto) = NULL;
		virtual strFaixa*       __fastcall RetornaFaixaFEC(TConjuntoAneel* Conjunto) = NULL;

		virtual void   __fastcall Parametros(VTSubestacao* SE, VTCarga* Carga, strParamComp &param) = 0;

	public:
		double CompensacoesMensais[12];
		double CompensacoesTrimestrais[4];
		double CompensacaoAnual;
		double CompensacaoTotal;
	};

//---------------------------------------------------------------------------
VTCompensacoes* __fastcall NewObjCompensacoes(TComponent* Owner, VTApl* AplOwner, AnsiString Empresa = "");

//---------------------------------------------------------------------------
#endif

//---------------------------------------------------------------------------
//eof
