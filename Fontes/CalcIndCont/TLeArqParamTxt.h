// ---------------------------------------------------------------------------
#ifndef TLeArqParamTxtH
#define TLeArqParamTxtH
// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include <stdio.h>
#include "VTLeArqParamTxt.h"
// ---------------------------------------------------------------------------
class VTEstratSoc;
class TLisDefeitoRede;
class VTTrafo;
class VTParamConf;
class VTPatamares;
class VTGeraLisParamEqpto;
class strDadoParamEqpto;
class strDadoParamRede;
class VTRede;
class VTChave;
class VTCarga;
class VTBuscaProt;

// ---------------------------------------------------------------------------
class TLeArqParamTxt : public VTLeArqParamTxt
{
public:
	__fastcall TLeArqParamTxt(VTApl *apl_owner);
	__fastcall ~TLeArqParamTxt(void);
	// Leitura e gravacao de parametros
	bool __fastcall LeArqParam(VTParamConf *paramConf, TList *lisChavesClone, TList *lisChavesNovas,
		AnsiString arqParam);
	bool __fastcall GravaArqParam(VTParamConf *paramConf, AnsiString arqParam);

private: // métodos
	strDadoParamRede* __fastcall ParametroConfCodRede(AnsiString codRede); // ssai
	double __fastcall ComprimentoRede(VTRede *rede);
	void __fastcall RedistribuiFalhasHora(void);
	void __fastcall ReiniciaChavesAuto(void);
	void __fastcall InsereChaveAuto(VTChave *pchave);
	AnsiString __fastcall LeDado(FILE* fp, bool &flagEOF);
	bool __fastcall InsereTipoChave(AnsiString codTipoChave, TList *lisTipoChave);
	strDadoParamEqpto* __fastcall ParametroDadoEqpto(AnsiString codChave, AnsiString codRede);
	VTRede* __fastcall BuscaRede(AnsiString codRede);
	VTChave* __fastcall BuscaChave(AnsiString codChave, TList *lisChave);
	VTCarga* __fastcall BuscaCarga(AnsiString codCarga, TList *lisCarga);
	int __fastcall NCons(VTCarga *carga);
	VTRede* __fastcall BuscaRedeMT_CargaBT(VTCarga *cargaBT, TList *lisAux);
	void __fastcall RemoveChaveAuto(VTChave *pchave);

	// dados locais

	VTApl *apl;
	VTBuscaProt *buscaProt;
	VTGeraLisParamEqpto *geraLisParamEqpto; // objeto para a preparacao de parametros

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
	// double *distFalhasPat; //distribuição de falhas por patamar
	// listas com valores ajustados
	bool flagVisAvalia; // se já foi rodado o fluxo para avaliação de folgas

	// Objetos com resultados da simulação
	TList *lisDefeitos; // lista total de defeitos
	TList *lisDefeitoRede; // lista de objeto que guarda lista de defeitos por rede

	// Listas intermediárias
	TList *lisCargaBlocos;
	TList *lisChavesClone;
	// Lista de chaves clone com informações de status(p.ex tipo da chave) originais
	TList *lisChavesAtivas;
	// Lista de chaves consideradas = lisChavesTotal - (lisChavesNovas - lisChavesAuto);
	TList *lisChavesNovas; // Lista de chaves novas - a serem implementadas
	// Se não pertencem a lisChavesAuto - não considerada
	AnsiString pathParam;

	VTParamConf *paramConf;
	VTPatamares *patamares;
	int numPat; // número de patamares

public:
};
// ---------------------------------------------------------------------------
#endif
// eof
