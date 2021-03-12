// ---------------------------------------------------------------------------
#ifndef RelatorioJusante
#define RelatorioJusante

// ---------------------------------------------------------------------------
#include <Classes.hpp>
using namespace std;
#include <complex>
// ---------------------------------------------------------------------------
#define eqptoCONSUMIDOR 70
// ---------------------------------------------------------------------------
class VTApl;
class VTBloco;
class VTChave;
class VTEqpto;
class VTPatamar;
// ---------------------------------------------------------------------------
struct strpi_CARGA
{
	struct
	{
		int qtde;
		double P_mw;
		double Q_mvar;
		complex<double> S_mva;
	}at;
	struct
	{
		int qtde;
		double P_mw;
		double Q_mvar;
		complex<double> S_mva;
	}mt;
	struct
	{
		int qtde;
		double P_mw;
		double Q_mvar;
		complex<double> S_mva;
	}bt;
	struct
	{
		int qtde;
		double P_mw;
		double Q_mvar;
		complex<double> S_mva;
	}total;
};
struct strpi_CONSUMIDOR
{
	struct
	{
		int residencial;
		int comercial;
		int industrial;
		int rural;
		int outros;
		int total;
		int bt;
		int mt;
		int at;
	}qtde;
	double energia_mwhmes;
};

struct strpi_RESUMO
{
	AnsiString  codigo_eqptoRef;
	int qtd_trafos;
	int qtd_chaves;
	int qtd_cargas;
	int qtd_consumidores;
	int qtd_capacitores_shunt;
	int qtd_capacitores_serie;
	int qtd_reguladores;
	int qtd_trechos;
	double comp_trecho_km;
};

struct strpi_TRAFO
{
	double snom_mva;
	int    qtde;

	struct
	{
		int qtde;
		double snom_mva;
	}trifasico;
	struct
	{
		int qtde;
		double snom_mva;
	}monofasico;
};
// ---------------------------------------------------------------------------
class VTRelatorioJusante : public TObject
{

public:
				 __fastcall VTRelatorioJusante(void){};
	virtual 	 __fastcall ~VTRelatorioJusante(void){};
	virtual void __fastcall AgregaValores(TStrings *key_value, VTEqpto *eqptoRef,
										int eqptoTIPO, VTPatamar *patamar = NULL, int tipo_relatorio = 0)=0;
	//nova interface
	virtual VTChave* __fastcall ChavePai(VTBloco *bloco) =0;
	virtual void __fastcall DestacaEquipamentosJusante(void)=0;
	virtual void __fastcall DestacaEquipamentosMontante(void)=0;
	virtual void __fastcall ListaBlocosJusante(VTBloco *bloco, TList *lisEXT)=0;
	virtual void __fastcall ListaEqptosJusante(TList *lisEXT, VTEqpto *eqptoRef,
																int eqptoTIPO)=0;
	virtual void __fastcall ListaEqptosMontante(TList *lisEXT, VTEqpto *eqptoRef,
																int eqptoTIPO)=0;
	virtual void __fastcall PreparaExecucao(void)=0;
	virtual void __fastcall SelecionaPatamar(VTPatamar *patamar)=0;

public:  //property
	__property TColor CorDestaque = {read=colorDestaque, write=colorDestaque};

protected:
	TColor	colorDestaque;

};

VTRelatorioJusante* NewObjRelatorioJusante(VTApl *apl);

#endif
// ---------------------------------------------------------------------------
// eof
