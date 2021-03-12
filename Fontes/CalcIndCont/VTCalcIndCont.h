// ---------------------------------------------------------------------------
#ifndef VTCalcIndContH
#define VTCalcIndContH
// ---------------------------------------------------------------------------
#include <Classes.hpp>
// #include "VTParConfRec.h"
#include "..\BDadoParam\Dados\VTParamConf.h"
#include "..\BDadoParam\Dados\VTVerifCjto.h"
#include "..\BDadoParam\Dados\VTVerifSe.h"
// ---------------------------------------------------------------------------
class VTApl;
class VTBarra;
class VTBlocos;
class VTBloco;
class VTCarga;
class VTChave;
class VTEqpto;
class VTRadial;
class VTRede;
class VTBlocoRad;
class VTBarraRad;
class VTVisRede;
class VTResCont;
class VTDefeito;
class VTRede;
class strResBloco;
class strResRede;
class VTBuscaProt;
class VTParamConf;
class strCenario;

// ---------------------------------------------------------------------------
struct strDadoInsercao : TObject
{
	VTEqpto *eqpto;
	double taxa_falha;
};

// ---------------------------------------------------------------------------
struct strDadoSubstituicao : TObject
{
	VTEqpto *eqpto_inserido;
	VTEqpto *eqpto_retirado;
	double variacao_taxa_falha_perc;
};

// ---------------------------------------------------------------------------
struct strParamBT : TObject
{
	AnsiString codCargaBT;
	VTCarga *carga;
	int noc;
	double cli, clih, tma; // taxa de falha e tempo de reparo
	double dic, fic;
};
//hk201808: Cosim
/*struct strChaveTelecomDisponibilidade : TObject
	{
	UnicodeString codigoChaveAuto;
	double disponibilidade;
	};

struct strCenario : TObject
	{
	UnicodeString codigoCenario;
	int ID;
	TList *lisChavesAuto;
	};
*/
// ---------------------------------------------------------------------------
class VTCalcIndCont : public TObject
{
public: // property
   __property VTApl* Apl = {read = PM_GetApl};
	__property VTParamConf* ParamConf = {read = PM_GetParamConf};
	__property TList* LisChavesAtivas ={read = PM_GetLisChavesAtivas};
	__property TList* LisChavesAuto ={read = PM_GetLisChavesAuto};
	__property TList* LisChavesIF ={read = PM_GetLisChavesIF}; // hk201710IF
	__property TList* LisEQIF ={read = PM_GetLisEQIF}; // hk201710IF
	__property TList* LisChavesNovas ={read = PM_GetLisChavesNovas};
	__property TList* LisDefeitos ={read = PM_GetLisDefeitos};
	__property TList* LisBlocoRad ={read = PM_GetLisBlocoRad};
	__property TList* LisVisRede ={read = PM_GetLisVisRede};
	__property TList* LisTipoEqProt ={read = PM_GetLisTipoEqProt};
	__property VTBuscaProt* BuscaProt ={read = PM_GetBuscaProt};
	__property VTDefeito* Defeito[VTBloco * bloco] ={read = PM_GetDefeito};
	__property VTResCont* ResCont ={read = PM_GetResCont};
	__property VTBlocos* Blocos ={read = PM_GetBlocos};
	__property VTRadial* Radial ={read = PM_GetRadial};
	__property int IProfReliga ={read = PM_GetIProfReliga, write = PM_SetIProfReliga};
	__property bool FlagForcaAnual ={read = PM_GetFlagForcaAnual, write = PM_SetFlagForcaAnual};

public:
	__fastcall VTCalcIndCont(void) {};
	__fastcall ~VTCalcIndCont(void) {};
	virtual bool __fastcall LeArquivosMdb(AnsiString pathParam) = 0;
	virtual bool __fastcall Executa(void) = 0;
	virtual bool __fastcall Executa(TList *lisInsercao, TList *lisSubstituicao) = 0;
	virtual bool __fastcall ExecutaDicFicPuMensal(void) = 0;
	virtual bool __fastcall ExecutaCompensacoes(VTRede *rede = NULL) = 0;
	virtual bool __fastcall ExecutaCompensacoesMensal(VTRede *rede = NULL) = 0;
	virtual bool __fastcall ExecutaRede(VTRede *rede, bool flagReiniciaRes = true) = 0;
	// hk201701: Adaptação redes subtransmissao: flagIniciaRes se inicia resultados associados a rede
	virtual bool __fastcall ExecutaEstrategia(VTDefeito *pdef) = 0;
	virtual bool __fastcall ExecutaAjusteParametrosLisRede(TList *lisRede, double decMeta,
		double fecMeta, double &decAjusta, double &fecAjusta, double erroMax = 0.05, int iteraMax =
		20) = 0;
	virtual bool __fastcall ExecutaAjusteParametrosLisRedeBlocoCarga(TList *lisRede, double decMeta,
		double fecMeta, double &decAjusta, double &fecAjusta, double erroMax = 0.05, int iteraMax =
		20) = 0;
	virtual void __fastcall ExecutaAjusteLisParamVerif(void) = 0;
	virtual void __fastcall IniciaVerifDefault(void) = 0;
	virtual bool __fastcall ReiniciaTipoEqProt(void) = 0;
	// reinicia paramConf->LisTipoEqProt = f(buscaProt->LisTipoEqProt)
	virtual bool __fastcall ReiniciaLisParamEqptoExterno(TList *lisParamEqptoExt, bool flagInf =true) = 0;
	virtual bool __fastcall ReiniciaLisParamRedeExterno(TList *lisParamRedeExt) = 0;
	virtual void __fastcall RetornaParametrosHistorico(bool flagGeral, bool flagCircuito,
		bool flagEqpto, bool flagDistHora) = 0;

	virtual VTDefeito* __fastcall IdentificaDefeito(VTEqpto *eqpto) = 0;
	virtual VTChave* __fastcall ChaveBloco(VTBloco *bloco) = 0;
	// hk201702: função que retorna a chave de suprimento do bloco
	virtual bool __fastcall AvaliaFolgaVis(void) = 0;
	virtual void __fastcall IniciaProblema(VTRede *rede) = 0;
	virtual void __fastcall IniciaParamDefeito(void) = 0;
	virtual void __fastcall IniciaListasResultados(VTRede *rede) = 0;
	virtual void __fastcall InsereChaveNova(VTChave *pchave) = 0;
	virtual void __fastcall RemoveChaveNova(VTChave *pchave) = 0;
	virtual void __fastcall ReiniciaChavesNovas(void) = 0;
   // gh201809: flagParamConf deve atualizar a lista do paramConf
	virtual void __fastcall InsereChaveAuto(VTChave *pchave, bool flagParamConf, bool flagRA) = 0;
	// hk201710: flagRA= true, mudança de tipo de chave para religadora
	virtual void __fastcall RemoveChaveAuto(VTChave *pchave, bool flagParamConf) = 0;
	virtual void __fastcall ReiniciaChavesAuto(void) = 0;
	// Retorna chaves dos equipamentos de protecao ate o suprimento
	virtual void __fastcall ImprimeParametrosDefeito(AnsiString arqSaida) = 0;
	virtual void __fastcall InsereChaveIF(VTChave *pchave) = 0; // hk201710
	virtual void __fastcall InsereChavesIF(AnsiString arq) = 0;
	virtual void __fastcall RemoveChaveIF(VTChave *pchave) = 0; // hk201710

protected: // métodos acessados via property
	virtual VTApl* __fastcall PM_GetApl(void) = 0;
	virtual VTParamConf* __fastcall PM_GetParamConf(void) = 0;

	//hk2018: reformula: SAI virtual void __fastcall PreparaIndicacaoFalta(void) = 0; // hk201710IF
	// __property bool        FlagInicia          = {read=PM_GetFlagInicia};
	//hk201808: reformula: sai: __property AnsiString PathParam ={read = PM_GetPathParam};
	//__property TList* LisParamBloco ={read = PM_GetLisParamBloco};
	//__property TList* LisVerifCjto ={read = PM_GetLisVerifCjto};
	//hk201808: reformula: sai: __property TList* LisParamRede ={read = PM_GetLisParamRede};
	//hk201808: reformula: sai: __property TList* LisParamEqpto ={read = PM_GetLisParamEqpto};
	//hk201808: reformula: sai: __property bool FlagRecurso ={read = PM_GetFlagRecurso, write = PM_SetFlagRecurso};
	//hk201808: reformula: sai: __property bool FlagRetorno ={read = PM_GetFlagRetorno, write = PM_SetFlagRetorno};
	//hk201808: reformula: sai: __property double TLocaliza ={read = PM_GetTLocaliza, write = PM_SetTLocaliza};
	//hk201808: reformula: sai: __property double TManManual ={read = PM_GetTManManual, write = PM_SetTManManual};
	//hk201808: reformula: sai: __property double TManAuto ={read = PM_GetTManAuto, write = PM_SetTManAuto};
	//hk201808: reformula: sai: __property double TReparo ={read = PM_GetTReparo, write = PM_SetTReparo};
	//hk201808: reformula: sai: __property double Lambda ={read = PM_GetLambda, write = PM_SetLambda};
	//hk201808: reformula: sai: __property double PuLim ={read = PM_GetPuLim, write = PM_SetPuLim};
	//hk201808: reformula: sai: __property double PercDefTrans ={read = PM_GetPercDefTrans, write = PM_SetPercDefTrans};
	//gh201806: acréscimo de fator de redução de tempo de preparo, devido à instalação de IF em fusível
	//hk201808: reformula: sai: __property double RedTprepIF ={read = PM_GetRedTprepIF, write = PM_SetRedTprepIF};
	//hk201808: reformula: sai: __property double DistFalhasPat[int np] ={read = PM_GetDistFalhasPat, write = PM_SetDistFalhasPat};
	//hk201808: reformula: sai: __property double* DistFalhas ={read = PM_GetDistFalhas};
	//hk201808: reformula: sai: __property strResBloco* ResBloco[VTBloco * bloco] ={read = PM_GetResBloco};
	//hk201808: reformula: sai: __property strResRede* ResRede[VTRede * rede] ={read = PM_GetResRede};
	//hk201808: reformula: sai: __property strResRede* ResGeral ={read = PM_GetResGeral};
	//virtual void __fastcall RedistribuiFalhasHora(void) = 0; //hk201808: reformula
	//virtual bool __fastcall GravaArqMdb(AnsiString arqParam) = 0;
	//virtual bool __fastcall LeArqMdb(AnsiString arqParam) = 0; //hk201808: reformulação: sai de publico
	//virtual bool __fastcall GravaArqParam(AnsiString arqParam) = 0;//hk201808: reformulação: sai de publico
	//virtual void __fastcall GravaDirConf(void) = 0;//hk201808: reformulação: sai de publico
	//virtual bool __fastcall LeArqParam(AnsiString arqParam) = 0;//hk201808: reformulação: sai de publico
	//virtual bool __fastcall LeArquivos(AnsiString pathParam) = 0;
   //virtual void __fastcall ListaOrdenadaBlocos(TList *lisExt, VTRede *rede) = 0; //hk201808: sai: funcao de buscaProt
	//virtual strDadoParamRede* __fastcall ParametroConfCodRede(AnsiString codRede) = 0;
	// virtual strDadoParamRede*     __fastcall ParametroConf(VTRede *rede) = 0;
	// virtual strDadoParamBloco*    __fastcall ParametroConfBloco(VTBloco *bloco) = 0;
	// virtual strDadoParamCarga*    __fastcall ParametroConfCarga(VTCarga *carga) = 0;
	//virtual strDadoParamEqpto* __fastcall ParametroDadoEqpto(AnsiString codChave,
	//	AnsiString codRede) = 0;
	//virtual strDadoParamEqpto* __fastcall ParametroDadoEqpto(VTCarga *carga) = 0;
	//virtual strDadoParamEqpto* __fastcall ParametroDadoEqpto(VTChave *chave) = 0;
   //hk201808: reformula: sai: virtual VTVisRede* __fastcall IdentificaVisRede(VTChave *pchv) = 0;
	//hk201808: reformula: sai: virtual void __fastcall LisChavesProt(VTRede *rede, TList *lisChaveExt) = 0;
	//hk201808: reformula: sai: virtual void __fastcall LisChavesVis(VTRede *rede, TList *lisChaveExt) = 0; // hk2004
	//hk201808: reformula: sai: virtual VTBarraRad* __fastcall LisBarProt(VTBloco *bloco) = 0; // hk2004
	//hk201808: reformula: sai: virtual bool __fastcall LisBlJus(VTChave *pchv, TList *lisEXTBloco) = 0;
	//hk201808: reformula: sai: virtual bool __fastcall LisBlProt(VTChave *pchv, TList *lisEXTBloco, TList *lisEXTChave) = 0;
	//hk201808: reformula: sai: virtual bool __fastcall LisZonaProtJus(VTChave *pchv, TList *lisBlocoExt) = 0;
	//hk201808: reformula: sai: virtual void __fastcall LisChavesOrd(TList *lisChavesOrd) = 0; //hk201808: funcao de buscaProt: tirar de calcIndCont
	//hk201808: reformula: sai: virtual void __fastcall ExecutaIndicadoresDefeitoTelecom(strCenario *cenario) = 0; //hk201808: Cosim
	//hk201808: reformula: sai: bool __fastcall         PM_GetFlagRecurso(void);
	//hk201808: reformula: sai: void __fastcall 			PM_SetFlagRecurso(bool flagRecurso);
	//hk201808: reformula: sai: bool __fastcall 			PM_GetFlagRetorno(void);
	//hk201808: reformula: sai: void __fastcall 			PM_SetFlagRetorno(bool flagRetorno);
	//hk201808: reformula: sai: double __fastcall 		PM_GetTLocaliza(void);
	//hk201808: reformula: sai: void __fastcall 			PM_SetTLocaliza(double tlocaliza);
	//hk201808: reformula: sai: double __fastcall 		PM_GetTManManual(void);
	//hk201808: reformula: sai: void __fastcall 			PM_SetTManManual(double tman_manual);
	//hk201808: reformula: sai: double __fastcall 		PM_GetTManAuto(void);
	//hk201808: reformula: sai: void __fastcall 			PM_SetTManAuto(double tman_auto);
	//hk201808: reformula: sai: double __fastcall 		PM_GetTReparo(void);
	//hk201808: reformula: sai: void __fastcall 			PM_SetTReparo(double trep);
	//hk201808: reformula: sai: double __fastcall 		PM_GetLambda(void);
	//hk201808: reformula: sai: void __fastcall 			PM_SetLambda(double lambda);
	//hk201808: reformula: sai: double __fastcall 		PM_GetPuLim(void);
	//hk201808: reformula: sai: void __fastcall 			PM_SetPuLim(double pu_lim);
	//hk201808: reformula: sai: double __fastcall 		PM_GetPercDefTrans(void);
	//hk201808: reformula: sai: void __fastcall 			PM_SetPercDefTrans(double percDefTrans);
	//gh201806: acréscimo de fator de redução de tempo de preparo, devido à instalação de IF em fusível
	//hk201808: reformula: sai: double __fastcall 		PM_GetRedTprepIF(void);
	//hk201808: reformula: sai: void __fastcall 			PM_SetRedTprepIF(double red_tprep_IF);

	//hk201808: reformula: sai: double __fastcall 		PM_GetDistFalhasPat(int np);
	//hk201808: reformula: sai: void __fastcall 			PM_SetDistFalhasPat(int np, double dfalha);

	//hk201808: reformula: sai: double* __fastcall 		PM_GetDistFalhas(void) {return (paramConf->DistFalhasPat);};


	//hk201808: reformula: sai: virtual bool __fastcall PM_GetFlagRecurso(void) = 0;
	//hk201808: reformula: sai: virtual void __fastcall PM_SetFlagRecurso(bool flagRecurso) = 0;
	//hk201808: reformula: sai: virtual bool __fastcall PM_GetFlagRetorno(void) = 0;
	//hk201808: reformula: sai: virtual void __fastcall PM_SetFlagRetorno(bool flagRetorno) = 0;
	//hk201808: reformula: sai: virtual double __fastcall PM_GetTLocaliza(void) = 0;
	//hk201808: reformula: sai: virtual void __fastcall PM_SetTLocaliza(double tlocaliza) = 0;
	//hk201808: reformula: sai: virtual double __fastcall PM_GetTManManual(void) = 0;
	//hk201808: reformula: sai: virtual void __fastcall PM_SetTManManual(double tman_manual) = 0;
	//hk201808: reformula: sai: virtual double __fastcall PM_GetTManAuto(void) = 0;
	//hk201808: reformula: sai: virtual void __fastcall PM_SetTManAuto(double tman_auto) = 0;
	//hk201808: reformula: sai: virtual double __fastcall PM_GetTReparo(void) = 0;
	//hk201808: reformula: sai: virtual void __fastcall PM_SetTReparo(double trep) = 0;
	//hk201808: reformula: sai: virtual double __fastcall PM_GetLambda(void) = 0;
	//hk201808: reformula: sai: virtual void __fastcall PM_SetLambda(double lambda) = 0;
	//hk201808: reformula: sai: virtual double __fastcall PM_GetPuLim(void) = 0;
	//hk201808: reformula: sai: virtual void __fastcall PM_SetPuLim(double pu_lim) = 0;
	//hk201808: reformula: sai: virtual double __fastcall PM_GetPercDefTrans(void) = 0;
	//hk201808: reformula: sai: virtual void __fastcall PM_SetPercDefTrans(double percDefTrans) = 0;
	//gh201806: acréscimo de fator de redução de tempo de preparo, devido à instalação de IF em fusível
	//hk201808: reformula: sai: virtual double __fastcall PM_GetRedTprepIF(void) = 0;
	//hk201808: reformula: sai: virtual void __fastcall PM_SetRedTprepIF(double red_tprep_IF) = 0;
	//hk201808: reformula: sai: virtual double __fastcall PM_GetDistFalhasPat(int np) = 0;
	//hk201808: reformula: sai: virtual void __fastcall PM_SetDistFalhasPat(int np, double dfalha) = 0;
	//hk201808: reformula: sai: virtual double* __fastcall PM_GetDistFalhas(void) = 0;
	virtual TList* __fastcall PM_GetLisChavesAtivas(void) = 0;
	virtual TList* __fastcall PM_GetLisChavesAuto(void) = 0;
	virtual TList* __fastcall PM_GetLisChavesIF(void) = 0; // hk201710IF
	virtual TList* __fastcall PM_GetLisEQIF(void) = 0; // hk201710IF
	virtual TList* __fastcall PM_GetLisChavesNovas(void) = 0;
	//hk201808: reformula: sai: virtual TList* __fastcall PM_GetLisParamBloco(void) = 0;
	//hk201808: reformula: sai: virtual TList* __fastcall PM_GetLisVerifCjto(void) = 0;
	//hk201808: reformula: sai: virtual TList* __fastcall PM_GetLisParamRede(void) = 0;
	//hk201808: reformula: sai: virtual TList* __fastcall PM_GetLisParamEqpto(void) = 0;
	virtual TList* __fastcall PM_GetLisDefeitos(void) = 0;
	virtual TList* __fastcall PM_GetLisBlocoRad(void) = 0;
	virtual TList* __fastcall PM_GetLisVisRede(void) = 0;
	virtual TList* __fastcall PM_GetLisTipoEqProt(void) = 0;
	virtual VTBuscaProt* __fastcall PM_GetBuscaProt(void) = 0;
	virtual VTDefeito* __fastcall PM_GetDefeito(VTBloco *bloco) = 0;
	virtual VTResCont* __fastcall PM_GetResCont(void) = 0;
	//hk201808: reformula: sai: virtual strResBloco* __fastcall PM_GetResBloco(VTBloco *bloco) = 0;
	//hk201808: reformula: sai: virtual strResRede* __fastcall PM_GetResRede(VTRede *rede) = 0;
	//hk201808: reformula: sai: virtual strResRede* __fastcall PM_GetResGeral(void) = 0;
	virtual VTBlocos* __fastcall PM_GetBlocos(void) = 0;
	virtual VTRadial* __fastcall PM_GetRadial(void) = 0;
	//hk201808: reformula: sai: virtual AnsiString __fastcall PM_GetPathParam(void) = 0;
	virtual int __fastcall PM_GetIProfReliga(void) = 0;
	virtual void __fastcall PM_SetIProfReliga(int iProfReliga) = 0;
	virtual bool __fastcall PM_GetFlagForcaAnual(void) = 0; // hk201703: ajustes planTurmas
	virtual void __fastcall PM_SetFlagForcaAnual(bool flagForcaAnual) = 0;
	// hk201703: ajustes planTurmas

public:
};

// ---------------------------------------------------------------------------
// função global p/ criar objeto
// ---------------------------------------------------------------------------
VTCalcIndCont* __fastcall NewObjCalcIndCont(VTApl* apl_owner, TList *lisChavesFunde,
	bool flagVis);
VTCalcIndCont* __fastcall NewObjCalcIndContCosim(VTApl* apl_owner, TList *lisChavesFunde,
	bool flagVis);//hk201808: Cosim
// lisChavesFunde: chaves na rede final com eliminacao de chaves e fusao de blocos; flagVis: se roda o fluxo e avalia folga de transferencia por chaves NA

// ---------------------------------------------------------------------------
#endif
// eof
