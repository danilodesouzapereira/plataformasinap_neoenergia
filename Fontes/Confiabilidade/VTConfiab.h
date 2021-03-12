//---------------------------------------------------------------------------
#ifndef VTConfiabH
#define VTConfiabH
//---------------------------------------------------------------------------
#include <stdio.h>

//---------------------------------------------------------------------------
struct  PAR_CONF
        {
        double tx_falha;
        double tx_interr;
		  double treparo;
		  double comp;
		  double tman;
		  double fat_isola; //para os blocos desernegisados pela protecao treparo´ = treparo * fat_isola
		  };

struct RES_CONF
       {
		 double dec, fec, end, pmed_mw;
       double cli, clih;
		 double fat_dec, fat_fec; //fatores de correcao
		 int ncons;
       };
struct RES_MC
		 {
		 double tx_ff;
		 double tx_fm;
		 double r_ff;
		 double r_fm;
		 };
//---------------------------------------------------------------------------
class TArvores;
class VTApl;
class VTBlocos;
class VTBloco;
class VTCfgGrafConfiab;
class VTDadoConfiab;
class VTEqpto;
class VTEqbar;
class VTLigacao;
class VTProgresso;
//---------------------------------------------------------------------------
class VTConfiab: public TObject
	{
	public:
				__fastcall VTConfiab(void) {};
				__fastcall ~VTConfiab(void) {};
		//property
		__property VTBlocos*         Blocos  =   {read = PM_GetBlocos};
		__property AnsiString        PathParam = {read = PM_GetPathParam};
		//__property VTCfgGrafConfiab* CfgGraf = { read = PM_GetCfgGrafConfiab };

		virtual void            __fastcall AddLigaExc(VTLigacao* pliga, bool flag_12 = true) = 0;
		virtual bool            __fastcall AgregaRes(RES_CONF &res_conf, VTEqpto* eqpto) = 0;
		virtual bool            __fastcall AgregaRes(RES_CONF &res_conf, TList* lisCargas) = 0;
		virtual bool            __fastcall AgregaRes(RES_CONF &res_conf, VTBloco* bloco) = 0;
		virtual VTDadoConfiab*  __fastcall DadoConfiab(void) = 0;
		virtual VTDadoConfiab*  __fastcall DadoConfiabNovo(void) = 0;
		virtual void            __fastcall DefDadoConfiab(VTDadoConfiab* dataExt) = 0;
		virtual void            __fastcall DefParEqpto(PAR_CONF pconf, VTEqpto* eqpto) = 0;
		virtual void            __fastcall DefProgresso(VTProgresso  *prog) = 0;
		virtual bool            __fastcall ExecutaAjParamRede(double prec) = 0;
		virtual bool            __fastcall ExecutaCalcIndices(void) = 0;
		virtual bool            __fastcall ExecutaCortesSC(void) = 0;
		virtual bool            __fastcall ExecutaSC(void) = 0;
		virtual TList*          __fastcall GeraRedeAgregada(TList *lisFora, bool flag = true) = 0;
		virtual void            __fastcall ImprimeParametrosRede(FILE *fout) = 0;
		virtual void            __fastcall Inicia(void) = 0;
		virtual void            __fastcall IniciaConfigGerais(void) = 0;
		virtual void            __fastcall IniciaConfigRede(void) = 0;
		virtual bool            __fastcall IniciaParamConf(VTDadoConfiab *dataConfiab) = 0;
		virtual void            __fastcall InsereLigaExcl(TList* lis) = 0;
		virtual TList*          __fastcall LigaExcl(void) = 0;
		virtual TList*          __fastcall ListaArvores(void) = 0;
		virtual TList*          __fastcall ListaCortes(void) = 0;
		virtual void            __fastcall ListaCortes(VTEqpto* eqRaiz, TList* lis1, TList* lis2, TList* lis3) = 0;
		virtual TList*          __fastcall ListaLiga(void) = 0;
		virtual TList*          __fastcall ListaGrupoCargas(void) = 0;
		virtual TList*          __fastcall ListaSup(void) = 0;

		virtual bool            __fastcall GravaArqParam(void) = 0;
		virtual bool            __fastcall LeArquivos(AnsiString pathParam) = 0;
		//virtual bool            __fastcall ExecutaCS(void) = 0;
		//virtual void            __fastcall DefSuprimento(VTEqpto* eqpto) = 0;

	protected: //métodos acessados via property
		virtual VTBlocos*          __fastcall PM_GetBlocos(void) = 0;
		virtual AnsiString         __fastcall PM_GetPathParam(void) = 0;
		//virtual VTCfgGrafConfiab*  __fastcall PM_GetCfgGrafConfiab(void) = 0;
	};

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTConfiab
//---------------------------------------------------------------------------
VTConfiab* __fastcall NewObjConfiab(VTApl *apl_owner);

//---------------------------------------------------------------------------
#endif
//eof


