// ---------------------------------------------------------------------------
#ifndef VTNixPrH
#define VTNixPrH

// ---------------------------------------------------------------------------
using namespace std;
#include <Classes.hpp>
#include <complex>

// ---------------------------------------------------------------------------
class VTChave;

// ---------------------------------------------------------------------------
class VTNixPr
{
public:
			__fastcall VTNixPr(void){};
	virtual __fastcall ~VTNixPr(void){};
	virtual AnsiString __fastcall LinhaArqCsv(void) = 0;

protected: // métodos
	virtual complex<double>	__fastcall CorrenteMaxima(VTChave *chave) = 0;
	virtual int 			__fastcall SupoeTipo(VTChave *chave) = 0;
	virtual AnsiString 		__fastcall TraduzCodigo(VTChave *chave, int index) = 0;


protected : // dados locais
	struct
	{
		AnsiString pr;
		AnsiString codigo;
		int no_inicio;
		int no_fim;
		double i_mod; // obsoleto, definir com valor zero
		double i_ang; // obsoleto, definir com valor zero
		AnsiString elo;
		int cons; // obsoleto, definir com valor zero
		int tipo;
	}strPR;
};
//---------------------------------------------------------------------------
//função global p/ criar objeto da classe
VTNixPr* __fastcall NewObjNixPrDefault(VTChave *chave, int index, bool pr_com_corrente);
VTNixPr* __fastcall NewObjNixPrNeoEnergia(VTChave *chave, int index, bool pr_com_corrente);
VTNixPr* __fastcall NewObjNixPrEDPSP(VTChave *chave, int index, bool pr_com_corrente);
// ---------------------------------------------------------------------------
#endif
// eof
