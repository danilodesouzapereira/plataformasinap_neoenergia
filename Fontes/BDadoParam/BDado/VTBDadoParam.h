//---------------------------------------------------------------------------
#ifndef VTBDadoParamH
#define VTBDadoParamH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Data.Win.ADODB.hpp>
#include <Data.DB.hpp>

//---------------------------------------------------------------------------
class VTRede;
class VTVerifCjto;
class VTVerifSe;

//---------------------------------------------------------------------------
//#include "..\Dados\VTAlim.h"

//---------------------------------------------------------------------------
//definição de estruturas para leitura do banco
struct sdi_ParamGerais
			{
			//bool   flagAutoSoc;   //true: aceita socorros com operações telecom (isolamto e socorr); false: caso contrário
			bool   flagRecurso;   //sim se utiliza recursos de socorro a jusante
			bool   flagRetorno;   //sim se considera influencia de manobras de retorno nos indicadores
			double tlocaliza;     //duração de preparação e localizacao de defeito
			double tman_manual;   //duração de manobras manuais
			double tman_auto;     //duração de manobra com telecomando
			//hkdef double trepDefault;   //tempo de reparo geral
			//hkdef double lambdaDefault; //taxa de falha geral
			double pu_lim;        //limite de sobrecarga na transferencia
			double percDefTrans;  //percentual de defeitos transitórios
			int    iProfReliga;   //sensibilidade de religamento: =0, somente o bloco imediatamente a jusante: =1, até o próximo religador
			double tlimIC;        //duração máxima do desligamento para não acrescentar os indicadores de contin.
			double     lambda;  //taxa de falhas
			double     trep;    //tempo de reparo
			double red_tprep_IF; //gh201806: % reduçao média do tempo de preparo para instalação de IFs em fusíveis
         double vindisp;//hk201808: indisp: indisponibilidade padrão
			};

struct sdi_Fld_ParamGerais
			{
			//TField* flagAutoSoc;
			TField* flagRecurso;
			TField* flagRetorno;
			TField* tlocaliza;
			TField* tman_manual;
			TField* tman_auto;
			//hkdef TField* trepDefault;
			//hkdef TField* lambdaDefault;
			TField* pu_lim;
			TField* percDefTrans;
			TField* iProfReliga;
			TField* tlimIC;
			TField* lambda;
			TField* trep;
			TField* red_tprep_IF; //gh201806: % reduçao média do tempo de preparo para instalação de IFs em fusíveis
			TField* vindisp; //hk201808: indisp: indisponibilidade padrão
			};

struct sdi_ParamRede
			{
			AnsiString codRede; //codigo da rede
			double     lambda;  //taxa de falhas
			double     trep;    //tempo de reparo
			};

struct sdi_Fld_ParamRede
			{
			TField* codRede;
			TField* lambda;
			TField* trep;
			};

struct sdi_ParamEqpto
			{
			AnsiString codEqpto;     //Equipamento
			AnsiString codRede;      //Código Rede
			AnsiString tipoRede;     //Rede (MT, BT, AT)
			double     nFalhasEquip; //Falhas por ano (do equipamento localizado)
			double     durTotal;     //Duracao [min.] por ano das falhas no equipamento localizado
			double     ndiasObs;     //Número de dias entre dataIni e dataFim
         double     ptpre;//hk201705: acréscimo de percentuais do tat (prep, desl+loc, exe)
         double     ptdlo;//hk201705: acréscimo de percentuais do tat (prep, desl+loc, exe)
         double     ptexe;//hk201705: acréscimo de percentuais do tat (prep, desl+loc, exe)
			//parâmetros - Noc e Tm
			double     nf;
			double     nfr;
			double     td;
			double     cli; //hkmar
			double     clih;//hkmar
			double     nfMes[12];
			double     nfrMes[12];
			double     tdMes[12];
			double     climes[12]; //hkmar
			double     clihmes[12];//hkmar
			double     nfHora[24];
			double     nfrHora[24];
			};

struct sdi_Fld_ParamEqpto
			{
			TField* codEqpto;
			TField* codRede;
			TField* tipoRede;
			TField* ndiasObs;
			TField* nf;
			TField* nfr;
			TField* td;
			TField* cli; //hkmar
			TField* clih;//hkmar
         TField* ptpre;//hk201705: acréscimo de percentuais do tat (prep, desl+loc, exe)
         TField* ptdlo;//hk201705: acréscimo de percentuais do tat (prep, desl+loc, exe)
         TField* ptexe;//hk201705: acréscimo de percentuais do tat (prep, desl+loc, exe)
			TField* nfMes1;
			TField* nfMes2;
			TField* nfMes3;
			TField* nfMes4;
			TField* nfMes5;
			TField* nfMes6;
			TField* nfMes7;
			TField* nfMes8;
			TField* nfMes9;
			TField* nfMes10;
			TField* nfMes11;
			TField* nfMes12;
			TField* nfrMes1;
			TField* nfrMes2;
			TField* nfrMes3;
			TField* nfrMes4;
			TField* nfrMes5;
			TField* nfrMes6;
			TField* nfrMes7;
			TField* nfrMes8;
			TField* nfrMes9;
			TField* nfrMes10;
			TField* nfrMes11;
			TField* nfrMes12;
			TField* tdMes1;
			TField* tdMes2;
			TField* tdMes3;
			TField* tdMes4;
			TField* tdMes5;
			TField* tdMes6;
			TField* tdMes7;
			TField* tdMes8;
			TField* tdMes9;
			TField* tdMes10;
			TField* tdMes11;
			TField* tdMes12;
			//hkmar
			TField *cliMes1, *cliMes2, *cliMes3, *cliMes4, *cliMes5;
			TField *cliMes6, *cliMes7, *cliMes8, *cliMes9, *cliMes10, *cliMes11, *cliMes12;
			//hkmar
			TField *clihMes1, *clihMes2, *clihMes3, *clihMes4, *clihMes5;
			TField *clihMes6, *clihMes7, *clihMes8, *clihMes9, *clihMes10, *clihMes11, *clihMes12;

			TField* nfHora0;
			TField* nfHora1;
			TField* nfHora2;
			TField* nfHora3;
			TField* nfHora4;
			TField* nfHora5;
			TField* nfHora6;
			TField* nfHora7;
			TField* nfHora8;
			TField* nfHora9;
			TField* nfHora10;
			TField* nfHora11;
			TField* nfHora12;
			TField* nfHora13;
			TField* nfHora14;
			TField* nfHora15;
			TField* nfHora16;
			TField* nfHora17;
			TField* nfHora18;
			TField* nfHora19;
			TField* nfHora20;
			TField* nfHora21;
			TField* nfHora22;
			TField* nfHora23;
			TField* nfrHora0;
			TField* nfrHora1;
			TField* nfrHora2;
			TField* nfrHora3;
			TField* nfrHora4;
			TField* nfrHora5;
			TField* nfrHora6;
			TField* nfrHora7;
			TField* nfrHora8;
			TField* nfrHora9;
			TField* nfrHora10;
			TField* nfrHora11;
			TField* nfrHora12;
			TField* nfrHora13;
			TField* nfrHora14;
			TField* nfrHora15;
			TField* nfrHora16;
			TField* nfrHora17;
			TField* nfrHora18;
			TField* nfrHora19;
			TField* nfrHora20;
			TField* nfrHora21;
			TField* nfrHora22;
			TField* nfrHora23;
			};

struct sdi_ChaveTelecom
			{
			AnsiString codChave; //código da chave
			};

struct sdi_Fld_ChaveTelecom
			{
			TField* codChave;
			};

struct sdi_ChaveIF
			{
			AnsiString codChave; //código da chave
			};

struct sdi_Fld_ChaveIF
			{
			TField* codChave;
			};

struct sdi_ParamBloco
			{
			AnsiString codChave; //código da chave
         AnsiString codBarraChave; //código da barra de referência para definição do bloco hk201802a
			AnsiString codRede;  //código da rede MT associada a carga //hk201610: ajustes de paramBloco em etapas carregando partes da rede - retirada de eliminação de paramBloco não localizado na rede
			double     lambda;   //taxa de falhas
			double     trep;     //tempo de reparo
         double     ptpre, ptdlo, ptexe; //hk201710IF: percentuais do tempo total de atendimento: trep
			};

struct sdi_Fld_ParamBloco
			{
			TField* codChave;
         TField* codBarraChave; //código da barra de referência para definição do bloco hk201802a
			TField* codRede;  //hk201610: ajustes de paramBloco em etapas carregando partes da rede - retirada de eliminação de paramBloco não localizado na rede
			TField* lambda;
			TField* trep;
         //hk201710IF
			TField* ptpre;
			TField* ptdlo;
			TField* ptexe;
			};

struct sdi_ParamCarga
			{
			AnsiString codCarga; //código da carga
			AnsiString codRede;  //hk201610: código da rede MT associada a carga //hk201610: ajustes de paramBloco em etapas carregando partes da rede - retirada de eliminação de paramBloco não localizado na rede
			double     lambda;   //taxa de falhas
			double     trep;     //tempo de reparo
         double     ptpre, ptdlo, ptexe; //hk201710IF: percentuais do tempo total de atendimento: trep
			};

struct sdi_Fld_ParamCarga
			{
			TField* codCarga;
			TField* codRede; //hk201610: ajustes de paramBloco em etapas carregando partes da rede - retirada de eliminação de paramBloco não localizado na rede
			TField* lambda;
			TField* trep;
         //hk201710IF
			TField* ptpre;
			TField* ptdlo;
			TField* ptexe;
			};

struct sdi_VerifCjto
			{
			AnsiString codCjto;  //Nome do conjunto
			double     decVerif; //Indicador dec verificado
			double     fecVerif; //Indicador fec verificado
			};

struct sdi_Fld_VerifCjto
			{
			TField* codCjto;
			TField* decVerif;
			TField* fecVerif;
			};

struct sdi_VerifSe
			{
			AnsiString  codSe;    //Código da subestação
			double      decVerif; //Indicador dec verificado
			double      fecVerif; //Indicador fec verificado
			VTVerifCjto *cjto;   //Ponteiro para o conjunto ao qual pertence
			};

struct sdi_Fld_VerifSe
			{
			TField* codSe;
			TField* decVerif;
			TField* fecVerif;
			};

struct sdi_VerifCircuito
			{
			AnsiString codCircuito; //Código do circuito
			double     decVerif;    //Indicador dec verificado
			double     fecVerif;    //Indicador fec verificado
			};

struct sdi_Fld_VerifCircuito
			{
			TField* codCircuito;
			TField* decVerif;
			TField* fecVerif;
			};

struct sdi_VerifCjtoSe
			{
			AnsiString codCjto;  //Nome do conjunto
			AnsiString codSe;    //Nome da subestação
			};

struct sdi_Fld_VerifCjtoSe
			{
			TField* codCjto;
			TField* codSe;
			};

struct sdi_TipoEqProt
			{
			AnsiString codTipoEqProt;
			};

struct sdi_Fld_TipoEqProt
			{
			TField* codTipoEqProt;
			};

//---------------------------------------------------------------------------
class VTApl;
class VTParamConf;

//---------------------------------------------------------------------------
class VTBDadoParam : public TObject
	{
	public:
									__fastcall  VTBDadoParam(void) {};
		virtual              __fastcall ~VTBDadoParam(void) {};
		virtual bool         __fastcall Aberto(void) = 0;
		virtual void         __fastcall DefineCaminho(AnsiString caminho) = 0;
		virtual void         __fastcall DefineDirBase(AnsiString dir) = 0;
		virtual AnsiString   __fastcall DirBase(void) = 0;
		virtual bool         __fastcall GravaDado(VTParamConf *paramConf) = 0;
		virtual bool         __fastcall ImportaDadoParamEqpto(TList *lisParamEqpto) = 0;
		virtual bool         __fastcall ImportaDadoTipoEqProt(TList *lisTipoEqProt) = 0;
		virtual bool         __fastcall LeDado(VTParamConf *paramConf, TStringList *lisCodRede = NULL) = 0;
		virtual VTParamConf* __fastcall ParamConf(void) = 0;
      // Função para importação dos dados de IFs existentes na rede, a partir de txt
		virtual bool __fastcall ImportaDadoChaveIF(TStringList *lisGrupoSensores, AnsiString arq) = 0;
	   virtual bool __fastcall GravaDadoIndisponibilidade(TList *lisEqptoIndisp) = 0; //hk201808: indisp
      virtual bool __fastcall GravaDadoIndisponibilidadePadrao(double vindisp) = 0;//hk201808: indisp

    virtual bool            __fastcall GravaDadoParamBloco(void) = 0;
    virtual bool            __fastcall GravaDadoParamBloco(sdi_ParamBloco &sdi) = 0;
    virtual bool            __fastcall GravaDadoParamCarga(void) = 0;
    virtual bool            __fastcall GravaDadoParamCarga(sdi_ParamCarga &sdi) = 0;
    virtual bool            __fastcall GravaDadoParamEqpto(TList *lisParamEqpto) = 0;
    virtual bool            __fastcall GravaDadoParamEqpto(sdi_ParamEqpto &sdi, bool flagCamposIF) = 0;
    virtual bool            __fastcall GravaDadoParamGerais(void) = 0;
    virtual bool            __fastcall GravaDadoParamGerais(sdi_ParamGerais &sdi, bool flagTprepIF) = 0;
    virtual bool            __fastcall GravaDadoParamRede(void) = 0;
    virtual bool            __fastcall GravaDadoParamRede(sdi_ParamRede &sdi) = 0;
    virtual bool            __fastcall GravaDadoVerifCjto(void) = 0;
    virtual bool            __fastcall GravaDadoVerifCjto(sdi_VerifCjto &sdi) = 0;
    virtual bool            __fastcall GravaDadoVerifCjtoSe(void) = 0;
    virtual bool            __fastcall GravaDadoVerifCjtoSe(sdi_VerifCjtoSe &sdi) = 0;
    virtual bool            __fastcall GravaDadoVerifSe(void) = 0;
    virtual bool            __fastcall GravaDadoVerifSe(sdi_VerifSe &sdi) = 0;

    virtual bool            __fastcall LeDadoParamGerais(void) = 0;
    virtual bool            __fastcall LeDadoVerifCjto(void) = 0;
    virtual bool            __fastcall LeDadoVerifSe() = 0;

    virtual void            __fastcall SetParamConf(VTParamConf *paramConf) = 0;

	protected:  //métodos acessados via property
	};

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe
//---------------------------------------------------------------------------
VTBDadoParam* __fastcall NewObjBDadoParam(VTApl *apl_owner);
//---------------------------------------------------------------------------
#endif
