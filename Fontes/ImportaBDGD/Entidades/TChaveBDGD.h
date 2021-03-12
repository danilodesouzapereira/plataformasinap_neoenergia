// --------------------------------------------------------------------------------------------- bof
#ifndef TChaveBDGDH
#define TChaveBDGDH

// ---------------------------------------------------------------------------------------- includes
#include <Classes.hpp>

// --------------------------------------------------------------------------------------- CLASS
class TBarraBDGD;
class TFasesConexao;
class TTipoUnidade;
class TCapElo;
class TCorrenteNom;


// -------------------------------------------------------------------------------------------
class TChaveBDGD : public TObject
{
public: //Property
		__property int          	  Id            = {read=PD.id_chave,       write=PD.id_chave     };
		__property int        	      Id_Rede       = {read=PD.id_rede,   	   write=PD.id_rede      };
		__property TBarraBDGD*   	  Barra_1       = {read=PD.barra1,   	   write=PD.barra1       };
		__property TBarraBDGD*        Barra_2       = {read=PD.barra2,         write=PD.barra2       };
		__property TFasesConexao*     Fases_Con     = {read=PD.fases_con,      write=PD.fases_con    };
		__property TTipoUnidade*      Tipo_Unidade  = {read=PD.tipo_unidade,   write=PD.tipo_unidade };
		__property TCapElo*           Elo_Fusivel  	= {read=PD.elo_fusivel,    write=PD.elo_fusivel  };
		__property TCorrenteNom*      Corrente_Nom  = {read=PD.corr_nom,       write=PD.corr_nom     };
		__property AnsiString   	  Codigo        = {read=PD.codigo,         write=PD.codigo       };
		__property bool   			  Telecomandada = {read=PD.telecomandada,  write=PD.telecomandada};
		__property int          	  Estado_Normal = {read=PD.estado_normal,  write=PD.estado_normal};
		__property AnsiString   	  Descricao     = {read=PD.descricao,      write=PD.descricao    };
public:
	__fastcall TChaveBDGD(void);
	__fastcall ~TChaveBDGD(void);

	protected:
	// dados
	struct
	{
		int id_chave;
		int id_rede;
		TBarraBDGD* barra1;
		TBarraBDGD* barra2;
		TFasesConexao* fases_con;
		TTipoUnidade* tipo_unidade;
		TCapElo* elo_fusivel;
		TCorrenteNom* corr_nom;
		AnsiString codigo;
		bool telecomandada;
		int estado_normal;
		AnsiString descricao;
	}PD;
};

// função global
TChaveBDGD* __fastcall NewObjChaveBDGD(void);

#endif

// --------------------------------------------------------------------------------------------- eof
