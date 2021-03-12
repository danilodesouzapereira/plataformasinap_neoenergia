// ---------------------------------------------------------------------------
#ifndef TGeraLisParamEqptoH
#define TGeraLisParamEqptoH
// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include <stdio.h>
#include "VTGeraLisParamEqpto.h"
// ---------------------------------------------------------------------------
class VTRede;
class VTCarga;
class VTChave;
class VTEqpto;
class VTBuscaProt;
class VTBloco;
class strDadoParamEqpto;
class strDadoParamBloco;
class strDadoParamRede;

// ---------------------------------------------------------------------------
class TGeraLisParamEqpto : public VTGeraLisParamEqpto
{
public:
	__fastcall TGeraLisParamEqpto(VTApl *apl_owner);
	__fastcall ~TGeraLisParamEqpto(void);
	bool __fastcall GeraLisParamEqptoBdado(TList *lisOc, TList *lisEqptoOc, TList *lisParamEqpto,
		int ndiasObsExt);
	bool __fastcall GeraLisBarraOcBdado(TList *lisEqptoOc, TList *lisBarraOc);
	// hk201702: integração simTurmas
	void __fastcall PreparaLisParamEqptoRede(TList *lisParamEqptoExterno, double *distFalhas);
	void __fastcall ReiniciaLisParamBlocoRede(TList       *lisParamEqptoEntrada,
                                              TList       *lisTipoEqProt,
                                              TList       *lisParamBlocoExterno,
                                              TList       *lisParamCargaExterno,
                                              VTBlocos    *blocos);
	void __fastcall PreparaParamConfRede(VTParamConf *paramConf, VTBlocos *blocos);
	void __fastcall PreparaLisParamBlocoRede(TList *lisParamBlocoExterno, bool flagEliminaReg =
		false); // hk201607 Virou função externa, acrescimo de flag : PreparaLisParamBlocoRede(TList *lisParamBlocoExterno, bool flagEliminaReg = false)
	void __fastcall PreparaLisParamCargaRede(TList *lisParamCargaExterno, bool flagEliminaReg =
		false); // hk201607 Virou função externa, acrescimo de flag : PreparaLisParamCargaRede(TList *lisParamCargaExterno, bool flagEliminaReg = false)

	// hk201611: mudanca dos parametros void __fastcall CalculaDadosParamRede(VTParamConf *paramConf);
	void __fastcall CalculaDadosParamRede(TList *lisParamEqpto, TList *lisParamRede,
		strDadoParamGerais *paramGeral);
	void __fastcall ReiniciaDadosParamEqpto(TList *lisParamEqpto);
	void __fastcall ReiniciaDadosParamRede(TList *lisParamRede);
	void __fastcall ReiniciaDadosParamGeral(strDadoParamGerais *paramGeral);
	void __fastcall ReiniciaDistribHora(strDadoParamGerais *paramGeral, double *distFalhasPat);
	void __fastcall RedistribuiFalhasCirc(VTParamConf *paramConf);
	int __fastcall RedistribuiFalhasCircMes(VTParamConf *paramConf);
	// retorna número de meses com falhas
	void __fastcall ImprimeLisParamEqpto(TList *lisParamEqpto, AnsiString nomeArq);
   void __fastcall PreparaLisChavesIF(TList *lisChavesIFExterno);//hk201808: reformulação

private: // métodos
	void __fastcall CalculaConsBlocoEJusante(TList *lisParamBlocoExterno);
	int __fastcall DeterminaQtdClientes(TList *lisCARGA);
	strDadoParamRede* __fastcall ParametroConf(VTRede *rede, TList *lisParamRede);
	strDadoParamRede* __fastcall ParametroConf(AnsiString codRede, TList *lisParamRede);
	void __fastcall PreparaLisTipoEqProt(TList *lisTipoEqProtExterno);
	void __fastcall PreparaLisChavesTelecom(TList *lisChavesTelecomExterno);
	void __fastcall PreparaLisParamRede(TList *lisParamRede);
	void __fastcall RedistribuiFalhasCirc(TList *lisParamEqpto, TList *lisParamBloco, TList *lisParamCarga,
						 double percDefTrans, int iProfReliga);
	// void __fastcall PreparaLisParamEqptoRede(TList *lisParamEqptoExterno, double *distFalhas);
	void __fastcall RedistribuiFalhasHora(double *distFalhasPat);
	int __fastcall NCons(VTCarga *carga);
	VTCarga* __fastcall BuscaCarga(AnsiString codCarga, TList *lisCarga);
	VTChave* __fastcall BuscaChave(AnsiString codChave, TList *lisChave);
	VTEqpto* __fastcall BuscaEqpto(AnsiString EqptoCodBase, AnsiString CodigoCircuito);
	VTRede* __fastcall BuscaRede(AnsiString CodigoCircuito);
	int __fastcall DefineNDiasObs(void);
	bool __fastcall GeraDistribFalhasPatamar(void);
	bool __fastcall GeraTemposBarraOc(void);
	AnsiString __fastcall GetFaseFromBloco(VTBloco *bloco);
	VTRede* __fastcall BuscaRedeMT_CargaBT(VTCarga *cargaBT, TList *lisAux);
	strDadoParamEqpto* __fastcall BuscaParamEqpto(VTEqpto *eqptoOcorr, TList *lisParamEqptoExterno);
	void __fastcall ImprimeLisOc(void);
	strDadoParamBloco* __fastcall ParametroConfBloco(VTBloco *bloco, TList *lisParamBloco);
	void __fastcall ImprimeLisParamSim(TList *lisParamEqpto, AnsiString nomeArq);

	void __fastcall ParamEqptoArredonda(TList *lisParamEqptoExterno, double iprec);
	bool __fastcall GeraParamEqptoBdado(void);
	double __fastcall ComprimentoBloco(VTBloco *bloco);
	double __fastcall ComprimentoBlocos(TList *lisBlocos);
	double __fastcall ComprimentoRede(VTRede *rede);
   void __fastcall PreparaEqptoIndisp(TList *lisEqptoIndisp);//hk201808: indisp

	// void __fastcall GeraNFalhasBarraOc(void);
protected: // métodos acessados via property

	TList* __fastcall PM_GetLisBarraOc(void){return (lisBarraOc);}; // hk201703: adaptacoes planTurmas

private: // objetos externos

	VTApl *apl;

	// dados locais
	VTBuscaProt *buscaProt;
	TList *lisParamEqpto, *lisParamRede, *lisOc, *lisBarraOc;
	int ndiasObs;
	// listas auxiliares
	TList *lisEqpto;
	TList *lisEqptoOc;
	TList *lisCircuitos;

};
// ---------------------------------------------------------------------------
#endif
// eof
