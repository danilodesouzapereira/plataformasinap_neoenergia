//---------------------------------------------------------------------------
#ifndef TConfiabH
#define TConfiabH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTConfiab.h"

//---------------------------------------------------------------------------
class TCalcIndices;
class VTDataEqpto;
class TGeraCorte;
class VTArvores;
class VTGeraPath;
class VTEqpto;
class VTRede;
class VTTipoChave;
class VTCabo;

//---------------------------------------------------------------------------
class TConfiab : public VTConfiab
	{
	public:
					 __fastcall TConfiab(VTApl *apl_owner);
					 __fastcall ~TConfiab(void);

		  void    __fastcall AddLigaExc(VTLigacao* pliga, bool flag_12);
		  bool    __fastcall AgregaRes(RES_CONF &res_conf, VTEqpto* eqpto);
		  bool    __fastcall AgregaRes(RES_CONF &res_conf, TList* lisCargas);
		  bool    __fastcall AgregaRes(RES_CONF &res_conf, VTBloco* bloco);
VTDadoConfiab*  __fastcall DadoConfiab(void);
VTDadoConfiab*  __fastcall DadoConfiabNovo(void);
		  void    __fastcall DefDadoConfiab(VTDadoConfiab* dataExt);
		  void    __fastcall DefParEqpto(PAR_CONF pconf, VTEqpto* eqpto);
		  void    __fastcall DefProgresso(VTProgresso  *prog);
		  bool    __fastcall ExecutaAjParamRede(double prec);
		  bool    __fastcall ExecutaCalcIndices(void);
		  bool    __fastcall ExecutaCortesSC(void);
		  bool    __fastcall ExecutaSC(void);
		TList*    __fastcall GeraRedeAgregada(TList *lisFora, bool flag = true);
		  void    __fastcall ImprimeParametrosRede(FILE *fout);
		  void    __fastcall Inicia(void);
		  void    __fastcall IniciaConfigGerais(void);
		  void    __fastcall IniciaConfigRede(void);
		  bool    __fastcall IniciaParamConf(VTDadoConfiab *dataConfiab);
		  void    __fastcall InsereLigaExcl(TList* lis);
		TList*    __fastcall LigaExcl(void);
		TList*    __fastcall ListaArvores(void);
		TList*    __fastcall ListaCortes(void);
		void      __fastcall ListaCortes(VTEqpto* eqRaiz, TList* lis1, TList* lis2, TList* lis3);
		TList*    __fastcall ListaLiga(void);
		TList*    __fastcall ListaGrupoCargas(void);
		TList*    __fastcall ListaSup(void);

		 bool     __fastcall GravaArqParam(void);
		 bool     __fastcall LeArquivos(AnsiString pathParam);


	private: //métodos
	  VTEqpto* __fastcall BuscaEqpto(AnsiString codEqpto);
		VTRede* __fastcall BuscaRede(AnsiString cod);
 VTTipoChave* __fastcall BuscaTipoChave(AnsiString cod);
		VTCabo* __fastcall BuscaTipoTrecho(AnsiString cod);
		  void  __fastcall DefineSuprimentos(void);
		  void  __fastcall DefineCargas(void);
	VTArvores* __fastcall GeraArvore(VTEqpto* eqpto);
		  void  __fastcall GeraCopiaArvores(TList* lisArvs);
		  bool  __fastcall LeArqParam(AnsiString arqParam);
  AnsiString  __fastcall LePathParam(void);
  AnsiString  __fastcall LeDado(FILE* fp, bool &flagEOF);
		  void  __fastcall ImprimeDataEqpto(FILE *fout, VTDataEqpto *dataEqpto);
		  void  __fastcall LeDataEqpto(FILE *fmed, VTDataEqpto *dataEqpto, AnsiString tipoLeit);

	protected: //métodos acessados via property
	 VTBlocos*  __fastcall PM_GetBlocos(void);
	 AnsiString __fastcall PM_GetPathParam(void) {return(pathParam);};


	private: //dados externos
		VTProgresso *prog;
	private: //dados locais

		VTApl  *apl;
		TCalcIndices *calcind;
		TList      *lisArv;
		TList      *lisLiga;
		TList      *lisEqpto;
		VTGeraPath *gerapath;
		TList      *lisGrupoCargas, *lisSUP;
		VTDadoConfiab *dataConfiab;

		AnsiString pathParam;
	};

#endif
//---------------------------------------------------------------------------
//eof

