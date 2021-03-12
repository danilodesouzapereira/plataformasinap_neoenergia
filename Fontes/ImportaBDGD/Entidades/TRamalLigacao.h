// --------------------------------------------------------------------------------------------- bof
#ifndef TRamalLigacaoH
#define TRamalLigacaoH

// ---------------------------------------------------------------------------------------- includes
#include <Classes.hpp>

// --------------------------------------------------------------------------------------- CLASS
class TBarraBDGD;
class TAreaLocalizacao;
class TFasesConexao;
class TArranjoBDGD;


// -------------------------------------------------------------------------------------------
class TRamalLigacao : public TObject
{
public: //Property
		__property int          	  Id            = {read=PD.id_ramal_ligacao, write=PD.id_ramal_ligacao };
		__property int        	      Id_Rede       = {read=PD.id_rede,   	     write=PD.id_rede          };
		__property TBarraBDGD*   	      Barra_1       = {read=PD.barra1,   	     write=PD.barra1           };
		__property TBarraBDGD*            Barra_2       = {read=PD.barra2,           write=PD.barra2           };
		__property TAreaLocalizacao*  Area_Loc      = {read=PD.area_loc,         write=PD.area_loc         };
		__property TFasesConexao*     Fases_Con     = {read=PD.fases_con,        write=PD.fases_con        };
		__property TArranjoBDGD*          Arranjo       = {read=PD.arranjo,          write=PD.arranjo          };
		__property AnsiString   	  Codigo        = {read=PD.codigo,           write=PD.codigo           };
		__property double   		  Comprimento_m = {read=PD.comprimento_m,    write=PD.comprimento_m    };
		__property AnsiString   	  Descricao     = {read=PD.descricao,        write=PD.descricao        };
public:
	__fastcall TRamalLigacao(void);
	__fastcall ~TRamalLigacao(void);

	protected:
	// dados
	struct
	{
		int id_ramal_ligacao;
		int id_rede;
		TBarraBDGD* barra1;
		TBarraBDGD* barra2;
		TAreaLocalizacao* area_loc;
		TFasesConexao* fases_con;
		TArranjoBDGD* arranjo;
		AnsiString codigo;
		double comprimento_m;
		AnsiString descricao;
	}PD;
};

// função global
TRamalLigacao* __fastcall NewObjRamalLigacao(void);

#endif

// --------------------------------------------------------------------------------------------- eof
