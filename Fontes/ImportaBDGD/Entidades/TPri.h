// --------------------------------------------------------------------------------------------- bof
#ifndef TPriH
#define TPriH

// ---------------------------------------------------------------------------------------- includes
#include <Classes.hpp>

// ------------------------------------------------------------------------------------ declarations

// Class declaration
class TSec;
class VTBarra;
class VTChave;
class VTRede;


// --------------------------------------------------------------------------------------- namespace

// ------------------------------------------------------------------------------------------- class
class TPri : public TObject
{
public:
	__fastcall TPri(void);
	__fastcall ~TPri(void);
	VTBarra* __fastcall ExisteBarra(int barra_id);
	TSec* __fastcall ExisteSec(int sec_id);
	bool __fastcall InsereBarra(VTBarra *barra);
	bool __fastcall InsereSec(TSec *sec);

	// dados
	int id;
	AnsiString codigo;
	AnsiString nome;
	VTBarra *barra_ini;
	double vnom_kv;
	VTRede *rede;
	VTChave *chave_dj;
	TList *lisSEC;
	TList *lisBAR;
};

TPri* __fastcall NewObjPri(void);

#endif

// --------------------------------------------------------------------------------------------- eof
