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

private: // m�todos
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
	bool flagParBloco; // = false--> parametros m�dios do circuito
	// = true:    teste de flagflagParCircBloco
	bool flagParCircBloco;
	// =true: parametro se existe bloco com falha observada--> parametro do bloco, caso contrario m�dia do circuito
	// =false: parametro se existe bloco com falha observada--> parametro do bloco, caso contrario taxa = 0.0
	int iDataEqpto;
	// uso das informa��es para a obtencao de taxas e tempos de reparo por defeito em bloco
	// N�veis de informa��o: geral, circuito, equipamento
	// 0: uso de todos os n�veis existentes (geral, circuito, equipamento)
	// 1: uso de m�dias por circuito e equipamento
	// 2: uso de m�dias por bloco
	// 3: uso de m�dias por circuito
	// 4: uso de par�metro geral
	// double *distFalhasPat; //distribui��o de falhas por patamar
	// listas com valores ajustados
	bool flagVisAvalia; // se j� foi rodado o fluxo para avalia��o de folgas

	// Objetos com resultados da simula��o
	TList *lisDefeitos; // lista total de defeitos
	TList *lisDefeitoRede; // lista de objeto que guarda lista de defeitos por rede

	// Listas intermedi�rias
	TList *lisCargaBlocos;
	TList *lisChavesClone;
	// Lista de chaves clone com informa��es de status(p.ex tipo da chave) originais
	TList *lisChavesAtivas;
	// Lista de chaves consideradas = lisChavesTotal - (lisChavesNovas - lisChavesAuto);
	TList *lisChavesNovas; // Lista de chaves novas - a serem implementadas
	// Se n�o pertencem a lisChavesAuto - n�o considerada
	AnsiString pathParam;

	VTParamConf *paramConf;
	VTPatamares *patamares;
	int numPat; // n�mero de patamares

public:
};
// ---------------------------------------------------------------------------
#endif
// eof
