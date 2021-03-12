// ---------------------------------------------------------------------------
#ifndef TCalcIndContH
#define TCalcIndContH
// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include <stdio.h>
#include "VTCalcIndCont.h"
#include "VTBuscaProt.h"    //hk201710IF
// ---------------------------------------------------------------------------
class VTEstratSoc;
class TLisDefeitoRede;
class VTTrafo;
class VTParamConf;
class VTPatamares;
class VTGeraLisParamEqpto;
class strDadoParamEqpto;
class VTLog;
class VTBuscaProt;
// ---------------------------------------------------------------------------
class TCalcIndCont : public VTCalcIndCont
{
public:
	__fastcall TCalcIndCont(VTApl *apl_owner, TList *lisChavesFunde, bool flagVis);
	__fastcall ~TCalcIndCont(void);
	// Funcoes de cálculo de indicadores
	bool __fastcall Executa(void);
	bool __fastcall Executa(TList *lisInsercao, TList *lisSubstituicao);
	bool __fastcall ExecutaDicFicPuMensal(void);
	bool __fastcall ExecutaCompensacoes(VTRede *rede);
	bool __fastcall ExecutaCompensacoesMensal(VTRede *rede);
	bool __fastcall ExecutaRede(VTRede *rede, bool flagReiniciaRes);
	bool __fastcall ExecutaEstrategia(VTDefeito *pdef);   // hk201701: Adaptação redes subtransmissao: flagIniciaRes se inicia resultados associados a rede
	bool __fastcall ExecutaAjusteParametrosLisRede(TList *lisRede, double decMeta, double fecMeta,
		double &decAjusta, double &fecAjusta, double erroMax = 0.05, int iteraMax = 20);
	void __fastcall ExecutaAjusteLisParamVerif(void);
	void __fastcall RetornaParametrosHistorico(bool flagGeral, bool flagCircuito, bool flagEqpto,
		bool flagDistHora); // reinicia paramConf->LisTipoEqProt = f(buscaProt->LisTipoEqProt)
	VTDefeito* __fastcall IdentificaDefeito(VTEqpto *eqpto);

	// Funções de inicialização
	bool __fastcall AvaliaFolgaVis(void); // hkedp
	void __fastcall IniciaProblema(VTRede *rede); // deleta defeitos de rede e reinicia parametros
	void __fastcall IniciaParamDefeito(void); // reinicia parametros por defeito em
	void __fastcall IniciaListasResultados(VTRede *rede); // funcao de valores historicos ou fornecidos pelo usuário
	// Alteracoes em listas de chaves novas e automatizadas
	void __fastcall InsereChaveNova(VTChave *pchave);
	void __fastcall RemoveChaveNova(VTChave *pchave);
	void __fastcall ReiniciaChavesNovas(void); // lisChavesNovas->Clear()
	// gh201809: flagParamConf
	void __fastcall InsereChaveAuto(VTChave *pchave, bool flagParamConf, bool flagRA = false); //hk201710: flagRA= true, mudança de tipo de chave para religadora
	void __fastcall RemoveChaveAuto(VTChave *pchave, bool flagParamConf);
	void __fastcall ReiniciaChavesAuto(void); // Funcao que zera lista de chaves automáticas
	void __fastcall RemoveChaveIF(VTChave *pchave);//hk201710
	//gh201806: função para associar o cadastro de IFs importado de arquivo txt às chaves da rede
	void __fastcall InsereChavesIF(AnsiString arq);
	//gh201806 : função para inserir chaves com IFs a partir de lista de chaves
	void __fastcall InsereLisChavesIF(TList *lisChavesIF);
	// Funcoes de consultas de topologia
	VTChave* __fastcall ChaveBloco(VTBloco *bloco);
	// Retorna chaves dos equipamentos de protecao ate o suprimento
	// Impressao de resultados e parametros
	void __fastcall ImprimeParametrosDefeito(AnsiString arqSaida);
   //hk2018: reformula: SAI: void __fastcall PreparaIndicacaoFalta(void);   //hk201710IF
	bool __fastcall LeArquivosMdb(AnsiString pathParam);


//------------------------------------------------------------------------------
	// Funcao de entrada de parametros por lista
	bool __fastcall ReiniciaTipoEqProt(void); //hk201808: reformula: sai: atualiza paramConf->LisTipoEqProt
	void __fastcall IniciaVerifDefault(void); //hk201808: reformula: sai?
	bool __fastcall ReiniciaLisParamEqptoExterno(TList *lisParamEqptoExt, bool flagInf); //hkreformula: sai
	bool __fastcall ReiniciaLisParamRedeExterno(TList *lisParamRedeExt); //hkreformula: sai
	void __fastcall InsereChaveIF(VTChave *pchave);//hkreformula: sai//hkreformula//hk201710
	//------------------------------------------------------------------------------
	//hk201808: reformula: sai: VTVisRede* __fastcall IdentificaVisRede(VTChave *pchv); //hk201808: reformula: sai
	//hk201808: reformula: sai: void __fastcall LisChavesProt(VTRede *rede, TList *lisChaveExt); //hk201808: funcao de buscaProt: tirar de calcIndCont// hk2004 Retorna lista de chaves NA --> suprimento
	//hk201808: reformula: sai: void __fastcall LisChavesVis(VTRede *rede, TList *lisChaveExt);  //hk201808: funcao de buscaProt: tirar de calcIndCont
	//hk201808: reformula: sai: bool __fastcall LisBlJus(VTChave *pchv, TList *lisEXTBloco); //hk201808: funcao de buscaProt: tirar de calcIndCont
	//hk201808: reformula: sai: VTBarraRad* __fastcall LisBarProt(VTBloco *bloco); //hk201808: funcao de buscaProt: tirar de calcIndCont
	//hk201808: reformula: sai: bool __fastcall LisBlProt(VTChave *pchv, TList *lisEXTBloco, TList *lisEXTChave); //hk201808: funcao de buscaProt: tirar de calcIndCont// hk201702: função que retorna a chave de suprimento do bloco
	//hk201808: reformula: sai: bool __fastcall LisZonaProtJus(VTChave *pchv, TList *lisBlocoExt); //hk201808: funcao de buscaProt: tirar de calcIndCont
	//hk201808: reformula: sai: void __fastcall LisChavesOrd(TList *lisChavesOrd); //hk201808: funcao de buscaProt: tirar de calcIndCont
	//hk201808: reformula: sai: void __fastcall ExecutaIndicadoresDefeitoTelecom(strCenario *cenario) {return;}; //hk201808: Cosim
	//bool __fastcall GravaArqMdb(AnsiString arqParam);//hkreformula: sai// Leitura e gravacao de parametros
	//strDadoParamRede* __fastcall ParametroConfCodRede(AnsiString codRede); //hk201808: reformula: sai: funcao de paramConf: sai
	//strDadoParamEqpto* __fastcall ParametroDadoEqpto(AnsiString codChave, AnsiString codRede);//hk201808: reformula: sai
	//strDadoParamEqpto* __fastcall ParametroDadoEqpto(VTCarga *carga);  //hk201808: reformula: sai
	//strDadoParamEqpto* __fastcall ParametroDadoEqpto(VTChave *chave);  //hk201808: reformula: sai
	//void __fastcall RedistribuiFalhasHora(void); //hkreformula: sai
	//bool __fastcall LeArqMdb(AnsiString arqParam);//hk201808: reformula: sai
	//bool __fastcall GravaArqParam(AnsiString arqParam);//hk201808: reformula: sai
	//void __fastcall GravaDirConf(void);//hk201808: reformula: vai para paramConf
	//bool __fastcall LeArqParam(AnsiString arqParam); //hk201808: reformula: sai
	//bool __fastcall LeArquivos(AnsiString pathParam);//hk201808: reformula: sai
   //void __fastcall TCalcIndCont::PreparaIndicacaoFalta(void)   //hk201710IF  //hk201808: reformulação: sai para buscaProt
//   void __fastcall ListaOrdenadaBlocos(TList *lisExt, VTRede *rede); //hk201808: sai: funcao de buscaProt
	// Funcoes de consulta de parametros
	// strDadoParamRede*   __fastcall ParametroConf(VTRede *rede);
	// strDadoParamBloco*  __fastcall ParametroConfBloco(VTBloco *bloco);
	// strDadoParamCarga*  __fastcall ParametroConfCarga(VTCarga *carga);

private: // métodos
   bool __fastcall PreparaParamConfRede(void);//hkreformula: funcao complementar preparaParamConfRede
	void __fastcall IniciaTipoProt(void);
	bool __fastcall ExecutaCalculoMensal(int iMes);
   void __fastcall ExecutaAtualizaResRede(TLisDefeitoRede *pDefeitoRedeAux = NULL);   //hk201804: redes pai-filha
	void __fastcall ExecutaAtualizaResBloco(TLisDefeitoRede *pDefeitoRede);
	bool __fastcall ExecutaCalculoCompensacoes(VTRede *rede);
	bool __fastcall ExecutaCalculoCompensacoesMensal(VTRede *rede);
	bool __fastcall ExecutaAjusteParametrosLisRedeBlocoCarga(TList *lisRede, double decMeta,
		double fecMeta, double &decAjusta, double &fecAjusta, double erroMax = 0.001,
		int iteraMax = 20);
	void __fastcall GeraLisBlocosDefeito(VTRede *rede = NULL);
	bool __fastcall CalculaEstrategiaSoc(VTDefeito *pdef, int iPat);
   double __fastcall CalculaDMIC(double dic, double fic);
   bool   __fastcall CalculaEstrategiasDefeito(VTDefeito *pdef); //hkv201607 bool para caso não haja chave de protecao a montante do defeito
	void __fastcall CalculaIndicadoresDefeito(VTDefeito *pdef);
   void __fastcall CalculaIndicadoresDefeitoCenario(VTDefeito *pdef); //hk201808: indisp: temp
   bool __fastcall ChaveTelecom(VTChave *pchv); //hk201808: indisp: temp
	void __fastcall CalculaIndicadoresRedeMT(TLisDefeitoRede *pDefeitoRede);
	void __fastcall CalculaIndicadoresRedeBT1(VTRede *rede, int iMes = -1);
	void __fastcall RecalcIndicPiscadas(VTDefeito *pdef);
	void __fastcall RecalcIndicPiscadasBloco(VTBloco *blCalc, double ddesl);
	void __fastcall RecalcIndic(double &dcons, double &tcons, TList *lisBlCalc, double ddesl,
		double dtempo, int np, bool flagINDIC = true);
	void __fastcall RecalcIndicBloco(double &dcons, double &tcons, VTBloco *blCalc, double ddesl,
		double dtempo, int np, bool flagINDIC = true);
	VTDefeito* __fastcall BuscaDefeitoRede(VTBloco *bloco, TLisDefeitoRede *lisDefRede);
	VTCarga* __fastcall BuscaCargaTrafo(VTTrafo *trafo, TList *lisCarga);
	double __fastcall ComprimentoBloco(VTBloco *bloco);
	double __fastcall FolgaIA(VTVisRede *visRede, int indPat);
	bool   __fastcall ChaveAutomatica(VTChave *pchv);

	void __fastcall IniciaObjetos(VTApl *aplOwner);
	void __fastcall IniciaLisDefeitos(VTRede *rede = NULL);
	void __fastcall IniciaParamDefeitoMes(int iMes);
	void __fastcall IniciaChavesTelecomandadasRede(void);
	// hkv201608: Funcao que Insere novas chaves telecomandadas religadoras com informações da rede de chaves telecomandadas
	void __fastcall ReinciaLisDefeitos(VTRede *rede = NULL);
	void __fastcall PreparaLisChavesAtivas(void);
	void __fastcall InsereLisTipoChaveProtecaoBuscaProt(TList *lisTipoEqProtExterno);
	void __fastcall RemoveChaveAutoBuscaProt(VTChave *pchave); // hkv201608
	void __fastcall InsereLisChavesTelecom(TList *lisChavesTelecomExterno);

	AnsiString __fastcall LeDado(FILE* fp, bool &flagEOF);

	void __fastcall VerificaVis(VTEstratSoc *estratSoc, VTVisRede *visRede);
	VTEqpto* __fastcall ExisteEqptoTrafo(AnsiString codigo, TList *lisEQPTO); // hkMaio
	int __fastcall NCons(VTCarga *carga);
	int __fastcall NConsLisBloco(TList *lisBlocos);
	bool __fastcall VarZonaProt(VTChave *pchave, int &nconsProt, double &cargaProt,
		double &distProt);
	bool __fastcall LisZonaProt(VTChave *pchv, TList *lisBlocoExt);

	void __fastcall ImprimeImpactos(TLisDefeitoRede *pDefeitoRede);
	void __fastcall ImprimeManobrasDefeito(VTDefeito *pdef, AnsiString arqSaida);
	void __fastcall ImprimeManobrasCjtoDefeitos(TLisDefeitoRede *pDefeitoRede, AnsiString arqSaida,
		bool flagAppend = false);

	double __fastcall ComprimentoRede(VTRede *rede);     //hkreformula: sai
	void __fastcall ParamDefault(void); //hk201808: reformula: sai
//----------------------------------------------------------------------------
	//void __fastcall CalculaIndicadoresRedeBT(VTRede *rede, int iMes = -1);//hk201808: reformula
	//void __fastcall OrdenaVisRedeCap(VTDefeito *pdef, int iPat, bool flagAuto = false);
	//void __fastcall OrdenaVisRedeDeltaDEC(VTDefeito *pdef);
	// void   __fastcall CalculaCargaBlocos(VTRede *rede); //sai 201607

        //void   __fastcall PreparaIndicacaoFalta(void);   //hk201710IF
//----------------------------------------------------------------------------
	//bool __fastcall FundeBlocos(TList *lisChavesFunde);  //hk201808: reformula: sai
	//void __fastcall IniciaLisVerifCjto(void);            //hk201808: reformula: sai
	//void __fastcall IniciaLisParamRede(void);            //hk201808: reformula: sai
	//void __fastcall IniciaDistFalhas(void);              //hk201808: reformula: sai
	//AnsiString __fastcall LePathParam(void);             //hk201808: reformula: sai
	//void __fastcall BuscaBlocosAgreg(VTBloco *bloco, TList *lisExt); //hk201808: reformula sai
	//void __fastcall IniciaCalcIndContSE(void); //hk201808: reformula: SAI

protected: // métodos acessados via property

	// Objetos locais
	VTApl* __fastcall PM_GetApl(void) {return (apl);};
	VTBlocos* __fastcall PM_GetBlocos(void);
	VTRadial* __fastcall PM_GetRadial(void){return (radial);};


	// Parametros gerais
	VTParamConf* __fastcall PM_GetParamConf(void) {return (paramConf);};

	TList* __fastcall 		PM_GetLisChavesAtivas(void);
	TList* __fastcall 		PM_GetLisChavesAuto(void);
   TList* __fastcall 		PM_GetLisChavesIF(void) {return(buscaProt->LisChavesIF);};//hk201710IF
   TList* __fastcall 		PM_GetLisEQIF(void) {return(buscaProt->LisEQIF);};//hk201710IF
   TList* __fastcall 		PM_GetLisChavesNovas(void) {return(lisChavesNovas);};

	// Lista de objetos defeito por bloco de carga
	TList* __fastcall 		PM_GetLisDefeitos(void){return (lisDefeitos);};
	VTDefeito* __fastcall 	PM_GetDefeito(VTBloco *bloco);

	// Listas e objetos em buscaProt
	VTBuscaProt* __fastcall PM_GetBuscaProt(void) {return (buscaProt);};
	TList* __fastcall 		PM_GetLisBlocoRad(void);
	TList* __fastcall 		PM_GetLisVisRede(void);
	TList* __fastcall 		PM_GetLisTipoEqProt(void);
	VTResCont* __fastcall 	PM_GetResCont(void) {return (resCont);};
	int __fastcall 			PM_GetIProfReliga(void);
	void __fastcall 			PM_SetIProfReliga(int iProfReliga);
	bool __fastcall 			PM_GetFlagForcaAnual(void) {return (flagForcaAnual);}; // hk201703: ajustes planTurmas
	void __fastcall 			PM_SetFlagForcaAnual(bool flagForcaAnual) {this->flagForcaAnual = flagForcaAnual;}; // hk201703: ajustes planTurmas
	// Lista de parametros por equipamento, rede, bloco
	//hk201808: reformula: sai: AnsiString __fastcall PM_GetPathParam(void) {return (paramConf->PathParam);};
	//hk201808: reformula: sai: TList* __fastcall 		PM_GetLisParamEqpto(void) {return (paramConf->LisParamEqpto);};
	//hk201808: reformula: sai: TList* __fastcall 		PM_GetLisParamRede(void) {return (paramConf->LisParamRede);};
	//hk201808: reformula: sai: TList* __fastcall 		PM_GetLisParamBloco(void) {return (paramConf->LisParamBloco);};
	//hk201808: reformula: sai: TList* __fastcall 		PM_GetLisVerifCjto(void) {return (paramConf->LisVerifCjto);};
	//hk201808: reformula: sai: TList* __fastcall 		PM_GetLisVerifSe(void) {return (paramConf->LisVerifSe);};
	//hk201808: reformula: sai: TList* __fastcall 		PM_GetLisVerifCircuito(void){return (paramConf->LisVerifCircuito);};
	//hk201808: reformula: sai: strResBloco* __fastcall PM_GetResBloco(VTBloco *bloco);
	//hk201808: reformula: sai: strResRede* __fastcall 	PM_GetResRede(VTRede *rede);
	//hk201808: reformula: sai: strResRede* __fastcall 	PM_GetResGeral(void);
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

private: // dados locais

	VTApl *apl;
	VTBuscaProt *buscaProt;
	VTRadial *radial;
	VTGeraLisParamEqpto *geraLisParamEqpto; // objeto para a preparacao de parametros
	VTLog *log;

	// valores de taxas
	bool flagParBloco; // = false--> parametros médios do circuito
	// = true:    teste de flagflagParCircBloco
	bool flagParCircBloco;
	// =true: parametro se existe bloco com falha observada--> parametro do bloco, caso contrario média do circuito
	// =false: parametro se existe bloco com falha observada--> parametro do bloco, caso contrario taxa = 0.0
	int iDataEqpto;
	// uso das informações para a obtencao de taxas e tempos de reparo por defeito em bloco
	// Níveis de informação: geral, circuito, equipamento
	// 0: uso de todos os níveis existentes (geral, circuito, equipamento)
	// 1: uso de médias por circuito e equipamento
	// 2: uso de médias por bloco
	// 3: uso de médias por circuito
	// 4: uso de parâmetro geral
	// listas com valores ajustados
	bool flagVisAvalia; // se já foi rodado o fluxo para avaliação de folgas
	bool flagMensal; // flag se é realizado cálculo mensal de indicadores
	bool flagForcaAnual; // flag força ao cálculo de continuidade anual

	// Objetos com resultados da simulação
	VTResCont *resCont;
	TList *lisDefeitos; // lista total de defeitos
	TList *lisDefeitoRede; // lista de objeto que guarda lista de defeitos por rede

	// Listas intermediárias
	TList *lisCargaBlocos;
	TList *lisChavesClone;
	// Lista de chaves clone com informações de status(p.ex tipo da chave) originais
	//hk201808: reformula: TList *lisChavesAtivas;
	// Lista de chaves consideradas = lisChavesTotal - (lisChavesNovas - lisChavesAuto);
	TList *lisChavesNovas; // Lista de chaves novas - a serem implementadas
	// Se não pertencem a lisChavesAuto - não considerada

	TList *lisChavesFunde; // lista de chaves para fusao de blocos

	//AnsiString pathParam;  //hk201808: reformula: vai para paramConf

	VTParamConf *paramConf;
	VTPatamares *patamares;
	int numPat; // número de patamares

   double disponibilidadeChave;//hk201808: indisp: temp

public:
};
// ---------------------------------------------------------------------------
#endif
// eof
