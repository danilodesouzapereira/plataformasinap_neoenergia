//---------------------------------------------------------------------------
#ifndef VTParamConfH
#define VTParamConfH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl; //hkreformula
class VTRede;
class VTEqpto;
class VTBloco;
class VTBarra;
class VTCarga;
class VTChave;
class VTTipoChave;
class VTVerifCjto;
class VTVerifSe;
class VTVerifCircuito;
//---------------------------------------------------------------------------
//definições de estruturas que armazenam parâmetros

struct strBlocoObra : TObject
			{
			double comprimento_eqpto;
			double lambda_eqpto;
			};

//---------------------------------------------------------------------------
struct strDadoParamGerais : TObject
			{
         bool   flagRecurso;   //sim se utiliza recursos de socorro a jusante
			bool   flagRetorno;   //sim se considera influencia de manobras de retorno nos indicadores
			double tlocaliza;     //duração de preparação e localizacao de defeito
			double tman_manual;   //duração de manobras manuais
			double tman_auto;     //duração de manobra com telecomando
			double pu_lim;        //limite de sobrecarga na transferencia
			double percDefTrans;  //percentual de defeitos transitórios
			int    iProfReliga;   //sensibilidade de religamento: =0, somente o bloco imediatamente a jusante: =1, até o próximo religador
			double tlimIC;        //duração máxima do desligamento para não acrescentar os indicadores de contin.
			double     lambda;  //taxa de falhas
			double     trep;    //tempo de reparo
			double     red_tprep_IF; //gh201806: % reduçao média do tempo de preparo para instalação de IFs em fusíveis
         double     taxa_descoord;//hk201711: taxa de descoordenação de proteção
			//Itens calculados nas redes abertas em funcao de lisParamEqpto
			double     ptpre, ptdlo, ptexe;//hk201705: acréscimo de percentuais do tat (prep, desl+loc, exe)
			double     compRede;   //comprimento das redes em km
			double     nFalhasEquip;//Falhas no período (do equipamento localizado);
			double     durTotal;//Duracao [min.] no período das falhas no equipamento localizado;
			double     ndiasObs; //Número de dias entre dataIni e dataFim
			double     nfhora[24];  //numero de falhas horárias em MT
			double     nFEquip[12]; //Falhas por ano (do equipamento localizado)
			double     durTot[12];  //Duracao [min.] por ano das falhas no equipamento localizado
			double     cli, clih;
			double     climes[12], clihmes[12];
			double     lamb_pu[12], tr_pu[12]; //taxa de falha e tempo de reparo mensais
			};

//---------------------------------------------------------------------------
struct strDadoParamRede : TObject
			{
			AnsiString codRede; //codigo da rede
			double     lambda;  //taxa de falhas
			double     trep;    //tempo de reparo
			double     cli, clih;
			double     climes[12], clihmes[12];
			double     lamb_pu[12], tr_pu[12]; //taxa de falha e tempo de reparo mensais
			//Parametros obtidos a partir das redes abertas
			VTRede     *rede;
			double     compRede;   //comprimento da rede em km
			//Itens calculados nas redes abertas em funcao de lisParamEqpto
			double     nFalhasEquip;//Falhas no período (do equipamento localizado);
			double     durTotal;//Duracao [min.] no período das falhas no equipamento localizado;
         double     ptpre, ptdlo, ptexe;//hk201705: acréscimo de percentuais do tat (prep, desl+loc, exe)
			double     ndiasObs; //Número de dias entre dataIni e dataFim
			double     nFEquip[12]; //Falhas por ano (do equipamento localizado)
			double     durTot[12];  //Duracao [min.] por ano das falhas no equipamento localizado
			int		  qtdOcorr;//quantidade de ocorrencias
			};

//---------------------------------------------------------------------------
struct strDadoParamEqpto : TObject
			{
			AnsiString codEqpto;     //Equipamento
			AnsiString codRede;      //Código Rede
			AnsiString tipoEquipamento; //Tipo do Equipamento

			double     ndiasObs;     //Número de dias entre dataIni e dataFim
			double     nfgeral, nf, nfr, td; //hk201612: nfgeral: total de falhas associadas ao equipamento, nf: falhas em equipamento mt ou trafo de bt, nfr: total de clientes interrompidos em falhas de ramal de baixa tensão
         double     tat; //hk201703: Correção do tempo de atendimento: para exibição em módulo de gestão de ocorrências
			double     cli, clih;
			double     nfmes[12], nfrmes[12], tdmes[12];
			double     nfhora[24], nfrhora[24];
			double     climes[12], clihmes[12];//valores de clientes interrompidos e clientes hora interrompidos
         double     ptpre, ptdlo, ptexe;//hk201705: acréscimo de percentuais do tat (prep, desl+loc, exe)

			int		  qtdOcorrEqpto;//quantidade de ocorrencias por equipamento
			int		  qtdOcorr;//quantidade de ocorrencias

			//Parametros e variáveis auxiliares obtidos a partir das redes abertas
			AnsiString tipoRede;     //Rede (MT, BT, AT)
			double     nFalhasEquip; //Falhas por ano (do equipamento localizado)
			double     durTotal;     //Duracao [min.] por ano das falhas no equipamento localizado
			//Valores mensais de numero de falhas e duracao total das falhas
			double     nFEquip[12]; //Falhas por ano (do equipamento localizado)
			double     durTot[12];  //Duracao [min.] por ano das falhas no equipamento localizado
			VTRede  *rede;
			VTEqpto *eqpto;
			VTCarga *carga;
			TObject *paramOrig;
			TObject *barraOcOrig;
			int     nconsJus; //número de consumidores a jusante do trafo de distrib. - carga BT
			double  lambda, trep; //taxa de falha e tempo de reparo

			double  lamb[12], tr[12]; //taxa de falha e tempo de reparo
			};

//---------------------------------------------------------------------------
struct strDadoVerifCjto : TObject
			{
			AnsiString nomeCjto; //Nome do conjunto
			AnsiString nomeSe;   //Nome da Se
			AnsiString codSe;    //Codigo da Se compátivel com a rede do SINAP
			double     decVerif; //Indicador dec verificado
			double     fecVerif; //Indicador fec verificado
			};

//---------------------------------------------------------------------------
struct strDadoParamBloco : TObject
{
    AnsiString   codChave;                          //código da chave de suprimento do bloco
    AnsiString   codBarra;  //hk201802a: código da barra da chave para definir univocamente o bloco em caso de reconfiguração de rede
    AnsiString   codRede;   //código da rede associada ao bloco
    double       lambda_permanente, lambda, trep; //taxa de falhas permanentes, taxa de falha total e tempo de reparo
    double       lamb_pu[12], tr_pu[12]; //pu mensal taxa de falha e tempo de reparo mensais
    double       ptpre, ptdlo, ptexe;               //hk201705: acréscimo de percentuais do tat (prep, desl+loc, exe)
    //Parametros e variáveis auxiliares obtidos a partir das redes abertas
    VTEqpto     *eqpto;       //chave de suprimento
    VTBarra     *eqptoBarra;  //hk201802a: barra da chave para definir univocamente o bloco em caso de reconfiguração de rede
    VTBloco     *bloco;       //rede
    AnsiString   fase;      //Indicador de blocos que sejam monofásicos, bifásicos ou trifásicos.
    double       compBloco;    //comprimento do bloco em km
    double       compZonaProt;   //comprimento, km, da zona de protecao
    double       duraTot;        //duracao total em minutos por ano (*365/ndiasObs)
    double       nFalhas;        //número de falhas por ano (*365/ndiasObs)
    double       fatTMA;         //hk201703: adaptações para planTurmas
    int          nconsJus;       //hk201708: número de consumidores do bloco e dos blocos a jusante do mesmo
    int          nconsBloco;     //hk201708: número de consumidores do bloco
    TList       *lisObrasBloco;	//GH2018.03.21: lista de obras que alteram a taxa de falha do bloco no módulo planejamento
};

//---------------------------------------------------------------------------
struct strDadoParamCarga : TObject
			{
			AnsiString codCarga;
			double  lambda, trep; //taxa de falha e tempo de reparo
         double  ptpre, ptdlo, ptexe;//hk201705: acréscimo de percentuais do tat (prep, desl+loc, exe)
			double  lamb_pu[12], tr_pu[12]; //pu mensal taxa de falha e tempo de reparo mensais
			//Parametros e variáveis auxiliares obtidos a partir das redes abertas
			VTRede  *rede;        //rede da carga
			AnsiString codRede;   //código da rede MT associada a carga //hk201610: ajustes de paramBloco em etapas carregando partes da rede - retirada de eliminação de paramBloco não localizado na rede
			VTCarga *carga;
			int     nconsJus;     //número de consumidores a jusante do trafo de distrib. - carga BT
			double  duraTot;        //duracao total em minutos por ano (*365/ndiasObs)
			double  nFalhas;        //número de falhas por ano (*365/ndiasObs)
			double  fatTMA;         //hk201703: adaptações para planTurmas
			};

//---------------------------------------------------------------------------
struct strDadoChaveTelecom : TObject
			{
			AnsiString codChave;
			VTChave *chave;
			};

//---------------------------------------------------------------------------
struct strDadoChaveIF : TObject
			{
			AnsiString codChave;
			VTChave *chave;
			};

//---------------------------------------------------------------------------
struct strDadoTipoEqProt : TObject
			{
			AnsiString codTipoEqProt;
			VTTipoChave *tipoChave;
			};

//---------------------------------------------------------------------------
struct strDadoEqptoIndisp : TObject  //hk201808: indisp
			{
			AnsiString cod_eqpto;
			double     vindisp; //valor de indisponibilidade do equipamento telecomandado
         VTEqpto    *eqpto;
			};

//---------------------------------------------------------------------------
class VTParamConf : public TObject
	{
	public: //property
		__property strDadoParamGerais* ParamGerais                 = {read=PM_GetParamGerais};
		__property strDadoEqptoIndisp* EqptoIndispPadrao           = {read=PM_GetEqptoIndispPadrao};
		__property strDadoParamBloco* ParamBloco[VTBloco *bloco]  = {read=PM_GetParamBloco};
		__property strDadoParamCarga* ParamCarga[VTCarga *carga]  = {read=PM_GetParamCarga};
		__property strDadoParamRede*  ParamRede[VTRede *rede]     = {read=PM_GetParamRede};
		__property strDadoParamRede*  ParamRedeCod[AnsiString codRede]     = {read=PM_GetParamCodRede};
		__property TList*              LisParamBloco     = {read=PM_GetLisParamBloco};
		__property TList*              LisParamCarga     = {read=PM_GetLisParamCarga};
		__property TList*              LisParamEqpto     = {read=PM_GetLisParamEqpto};
		__property TList*              LisParamRede      = {read=PM_GetLisParamRede};
		__property TList*              LisVerifCjto      = {read=PM_GetLisVerifCjto};
		__property TList*              LisVerifSe        = {read=PM_GetLisVerifSe};
		__property TList*              LisVerifCircuito  = {read=PM_GetLisVerifCircuito};
		__property TList*              LisChavesTelecom  = {read=PM_GetLisChavesTelecom};
		__property TList*              LisChavesIF  		 = {read=PM_GetLisChavesIF};
		__property TList*              LisTipoEqProt     = {read=PM_GetLisTipoEqProt};
		__property TList*              LisEqptoIndisp    = {read=PM_GetLisEqptoIndisp}; //hk201808: indisp
		__property double*             DistFalhasPat     = {read=PM_GetDistFalhasPat};
      __property AnsiString          PathParam         = {read=PM_GetPathParam};//hk201808: reformula

	public:
										 __fastcall VTParamConf(void) {};
		virtual                  __fastcall ~VTParamConf(void) {};
		virtual VTParamConf*     __fastcall Clone(VTParamConf* parConfOrig) = 0;
		virtual VTVerifCircuito* __fastcall CriaVerifCircuito(void) = 0;
		virtual VTVerifCjto*     __fastcall CriaVerifCjto(void) = 0;
		virtual VTVerifSe*       __fastcall CriaVerifSe(void) = 0;
		virtual VTVerifCircuito* __fastcall ExisteVerifCircuito(AnsiString codigo) = 0;
		virtual VTVerifSe*       __fastcall ExisteVerifSe(AnsiString codigo) = 0;
		virtual void             __fastcall ImprimeParametrosSimula(AnsiString arq) = 0;
		virtual void             __fastcall ImprimeParametrosEntrada(AnsiString arq) = 0;
		virtual void             __fastcall ImprimeParametrosMes(AnsiString arq) = 0;
		virtual void             __fastcall IniciaParamDefault(void) = 0;   //hk201608
      //hkreformula
      virtual void             __fastcall InsereLisTipoChaveProtecaoParamConf(TList *lisTipoEqProtExterno) = 0;
      virtual void             __fastcall IniciaLisParamRede(TList *lisRede) = 0;
      virtual void             __fastcall IniciaDistFalhas(int numPat) = 0;
      virtual void             __fastcall RedistribuiFalhasHora(int numPat) = 0; //hkreformula:
      virtual bool             __fastcall GravaArqMdb(VTApl *apl, AnsiString arqParam) = 0;  //hkreformula:
      virtual bool             __fastcall LeArquivosMdb(VTApl* apl, AnsiString pathParam) = 0;//hkreformula
      virtual void             __fastcall GravaDirConf(VTApl *apl) = 0;//hkreformula
    virtual strDadoParamEqpto* __fastcall ParametroDadoEqpto(VTChave *chave) = 0;//hkreformula
    virtual strDadoParamEqpto* __fastcall ParametroDadoEqpto(AnsiString codChave,
	                               AnsiString codRede) = 0;//hkreformula
    virtual strDadoParamEqpto* __fastcall ParametroDadoEqpto(VTCarga *carga) = 0;//hkreformula
    virtual AnsiString         __fastcall LePathParam(VTApl* apl) = 0; //hk201808: reformula vai para bdadoParam

	public: //dados locais

	protected:  //métodos acessados via property
		virtual strDadoParamGerais* __fastcall PM_GetParamGerais(void) = 0;
		virtual strDadoEqptoIndisp* __fastcall PM_GetEqptoIndispPadrao(void) = 0;//hk201808: indisp
		virtual strDadoParamBloco*  __fastcall PM_GetParamBloco(VTBloco *bloco) = 0;
		virtual strDadoParamCarga*  __fastcall PM_GetParamCarga(VTCarga *carga) = 0;
		virtual strDadoParamRede*   __fastcall PM_GetParamRede(VTRede *rede) = 0;
		virtual strDadoParamRede*   __fastcall PM_GetParamCodRede(AnsiString codRede) = 0;
		virtual TList*              __fastcall PM_GetLisParamBloco(void) = 0;
		virtual TList*              __fastcall PM_GetLisParamCarga(void) = 0;
		virtual TList*              __fastcall PM_GetLisParamEqpto(void) = 0;
		virtual TList*              __fastcall PM_GetLisParamRede(void) = 0;
		virtual TList*              __fastcall PM_GetLisVerifCjto(void) = 0;
		virtual TList*              __fastcall PM_GetLisVerifSe(void) = 0;
		virtual TList*              __fastcall PM_GetLisVerifCircuito(void) = 0;
		virtual TList*              __fastcall PM_GetLisChavesTelecom(void) = 0;
		virtual TList*              __fastcall PM_GetLisChavesIF(void) = 0;
		virtual TList*              __fastcall PM_GetLisTipoEqProt(void) = 0;
		virtual TList*              __fastcall PM_GetLisEqptoIndisp(void) = 0; //hk201808: indisp
		virtual double*             __fastcall PM_GetDistFalhasPat(void) = 0;
		virtual AnsiString          __fastcall PM_GetPathParam(void) = 0;//hk201808: reformula

	};

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe
//---------------------------------------------------------------------------
VTParamConf* __fastcall NewObjParamConf(void);
//---------------------------------------------------------------------------
#endif
