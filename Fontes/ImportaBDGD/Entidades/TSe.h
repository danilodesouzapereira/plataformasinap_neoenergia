// ---------------------------------------------------------------------------
#ifndef TSeH
#define TSeH

// arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>

// ---------------------------------------------------------------------------
class VTBarra;
class TPri;
class VTRede;
class VTTrafo;
class VTTrafo3E;
class VTSuprimento;

// ---------------------------------------------------------------------------
class TSe : public TObject
{
public:
	__fastcall TSe(void);
	__fastcall ~TSe(void);
	AnsiString __fastcall CodigoBarraAT(void);
	VTBarra* __fastcall ExisteBarra(int barra_id);
	VTBarra* __fastcall ExisteBarra(double vnom_kv);
	TPri* __fastcall ExistePri(VTRede *rede);
	TPri* __fastcall ExistePri(int rede_extid);
	TPri* __fastcall ExistePri(AnsiString codigo);
	VTSuprimento* __fastcall ExisteSuprimento(int id_circuito);
	VTTrafo* __fastcall ExisteTrafo(int id);
	VTTrafo* __fastcall ExisteTrafo(double vsec_kv);
	VTTrafo3E* __fastcall ExisteTrafo3E(int id);
	VTTrafo3E* __fastcall ExisteTrafo3E(double vbaixa_kv);
	bool __fastcall InsereBarra(VTBarra *barra);
	bool __fastcall InserePri(TPri *pri);
	bool __fastcall InsereTrafo(VTTrafo *trafo);
	bool __fastcall InsereTrafo3E(VTTrafo3E *trafo3E);

	// dados
	int id;
	AnsiString codigo;
	AnsiString nome;
	AnsiString reg;
	AnsiString loc_tec;
	double x_m;
	double y_m;
	VTRede *rede;
	VTSuprimento *suprimento;
	TList *lisBARRA;
	TList *lisPRI;
	TList *lisTRF;
	TList *lisTRF3E;
	//Teste
	TList *lisSUPRIMENTO;
};

// ---------------------------------------------------------------------------
TSe* __fastcall NewObjSe(void);

#endif
// ---------------------------------------------------------------------------
// eof
