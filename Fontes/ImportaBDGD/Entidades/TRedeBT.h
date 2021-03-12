// --------------------------------------------------------------------------------------------- bof
#ifndef TRedeBTH
#define TRedeBTH

// ---------------------------------------------------------------------------------------- includes
#include <Classes.hpp>

class TRedeMT;
class TTensao;

// -------------------------------------------------------------------------------------------
class TRedeBT : public TObject
{
public: //Property
		__property int        	      Id_Rede_BT       = {read=PD.id_rede_BT,   	 write=PD.id_rede_BT       };
		__property int        	      Id_Rede_MT       = {read=PD.id_rede_MT,   	 write=PD.id_rede_MT       };
		__property TRedeMT*        	  Rede_MT          = {read=PD.rede_MT,   	     write=PD.rede_MT          };
		__property AnsiString   	  Codigo           = {read=PD.codigo,            write=PD.codigo           };
		__property TTensao*           Tensao_SecMTBT   = {read=PD.tensao_secMTBT,    write=PD.tensao_secMTBT           };
		__property TTensao*           Tensao_TerMTBT   = {read=PD.tensao_terMTBT,    write=PD.tensao_terMTBT           };

public:
	__fastcall TRedeBT(void);
	__fastcall ~TRedeBT(void);

	protected:
	// dados
	struct
	{
		int id_rede_MT;
		TRedeMT* rede_MT;
		int id_rede_BT;
		AnsiString codigo;
		TTensao* tensao_secMTBT;
		TTensao* tensao_terMTBT;
	}PD;
};

// função global
TRedeBT* __fastcall NewObjRedeBT(void);

#endif

// --------------------------------------------------------------------------------------------- eof
