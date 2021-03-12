// --------------------------------------------------------------------------------------------- bof
#ifndef TSecH
#define TSecH

// ---------------------------------------------------------------------------------------- includes
#include <Classes.hpp>

// ------------------------------------------------------------------------------------ declarations

// Class declaration
class TSec;
class VTBarra;
class VTCarga;
class VTRede;
class VTTrafo;

// --------------------------------------------------------------------------------------- namespace

// ------------------------------------------------------------------------------------------- class
class TSec : public TObject
{
public:
	__fastcall TSec(void);
	__fastcall ~TSec(void);
	VTBarra* __fastcall ExisteBarra(int barra_id);
	bool __fastcall InsereBarra(VTBarra* barra);

	// dados
	int id;
	double vnom_v;
	AnsiString codigo;
	TList *lisBARRA;
	VTRede *rede;
	VTBarra *barra_mt;
	VTBarra *barra_bt;
	VTCarga *carga_eqv;
	VTTrafo *trafo;
};

TSec* __fastcall NewObjSec(void);

#endif

// --------------------------------------------------------------------------------------------- eof
