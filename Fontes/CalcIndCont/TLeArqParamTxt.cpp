// ---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
#include <algorithm>
#include <math.h>
#include "TLeArqParamTxt.h"
#include "VTLigaCrit.h"
#include "TVisRedeSoc.h"
#include "VTAvaliaVis.h"
#include "TLisDefeitoRede.h"
#include "VTVisRede.h"
#include "VTBlocoRad.h"
#include "VTBarraRad.h"
#include "VTEstratSoc.h"
#include "VTResCont.h"
#include "VTBuscaProt.h"
#include "VTDefeito.h"
#include "VTLisBlocoChave.h"
#include "VTLeArqParamTxt.h"
#include "..\BDadoParam\Dados\VTParamConf.h"
#include "..\BDadoParam\BDado\VTBDadoParam.h"
#include "..\BdadoParam\Modela\VTGeraLisParamEqpto.h"
#include "..\CalcIndCont\VTCalcEstratSoc.h"

#include <PlataformaSinap\Fontes\Compensacoes\VTCompensacoes.h>
#include <PlataformaSinap\Fontes\Apl\VTApl.h>
#include <PlataformaSinap\Fontes\Bloco\VTBloco.h>
#include <PlataformaSinap\Fontes\Bloco\VTBlocos.h>
#include <PlataformaSinap\Fontes\Potencia\VTDemanda.h>
#include <PlataformaSinap\Fontes\Diretorio\VTPath.h>
#include <PlataformaSinap\Fontes\Radial\VTRadial.h>
#include <PlataformaSinap\Fontes\Radial\VTSubestacao.h>
#include <PlataformaSinap\Fontes\Radial\VTPrimario.h>
#include <PlataformaSinap\Fontes\Rede\VTTipos.h>
#include <PlataformaSinap\Fontes\Rede\VTTipoChave.h>
#include <PlataformaSinap\Fontes\Rede\VTRede.h>
#include <PlataformaSinap\Fontes\Rede\VTRedes.h>
#include <PlataformaSinap\Fontes\Rede\VTBarra.h>
#include <PlataformaSinap\Fontes\Rede\VTCarga.h>
#include <PlataformaSinap\Fontes\Rede\VTChave.h>
#include <PlataformaSinap\Fontes\Rede\VTTrecho.h>
#include <PlataformaSinap\Fontes\Rede\VTTrafo.h>
#include <PlataformaSinap\Fontes\Rede\VTLigacao.h>
#include <PlataformaSinap\Fontes\Patamar\VTPatamares.h>
#include <PlataformaSinap\DLL_Inc\Compensacoes.h>

#include "..\..\DLL_Inc\BDadoParam.h"

#include <PlataformaSinap\DLL_Inc\Bloco.h>
#include <PlataformaSinap\DLL_Inc\Radial.h>
#include <PlataformaSinap\DLL_Inc\Funcao.h>
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
VTLeArqParamTxt* __fastcall NewObjLeArqParamTxt(VTApl* apl_owner)
{
	return (new TLeArqParamTxt(apl_owner));
}

// ---------------------------------------------------------------------------
__fastcall TLeArqParamTxt::TLeArqParamTxt(VTApl *apl_owner)
{
	this->apl = apl_owner;
}

// ---------------------------------------------------------------------------
__fastcall TLeArqParamTxt::~TLeArqParamTxt(void)
{
}

// ---------------------------------------------------------------------------
bool __fastcall TLeArqParamTxt::LeArqParam(VTParamConf *paramConf, TList *lisChavesClone,
	TList *lisChavesNovas, AnsiString arqParam)
{
	this->paramConf = paramConf;
	this->lisChavesClone = lisChavesClone;
	this->lisChavesNovas = lisChavesNovas;
	buscaProt = (VTBuscaProt*)apl->GetObject(__classid(VTBuscaProt));
	geraLisParamEqpto = (VTGeraLisParamEqpto*)apl->GetObject(__classid(VTGeraLisParamEqpto));

	lisChavesAtivas = buscaProt->LisChavesAtivas;

	TList *lisParamRede = paramConf->LisParamRede;
	TList *lisParamEqpto = paramConf->LisParamEqpto;
	TList *lisParamCarga = paramConf->LisParamCarga;
	TList *lisVerifCjto = paramConf->LisVerifCjto;

	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));
	AnsiString arq;
	AnsiString codCirc, codChaveAuto, aux;
	int ipat;
	double falha_patamar;
	// double  cli_mt, clih_mt, taxa_BT, tma_BT;
	double treparoCirc, lambdaCirc;
	bool flagEOF;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTRede *rede;
	TList *lisRede, *lisCargas;
	strDadoParamRede *paramRede;
	TStringList *lisStrChave;
	AnsiString codChave;
	TList *lisChaves, *lisAux, *lisAux1;
	VTChave *pchave;
	bool resposta = true;
	FILE *fmed;

	// Reinicia ponteiro obj de cargas para busca de paramEqpto
	VTCarga *carga;
	lisCargas = new TList();
	redes->LisEqbar(lisCargas, eqptoCARGA);
	for (int nc = 0; nc < lisCargas->Count; nc++)
	{
		carga = (VTCarga*)lisCargas->Items[nc];
		carga->Obj = NULL;
	}
	delete lisCargas;
	// Reinicia listas de parametros
	LimpaTList(lisParamRede); // lista de parâmetros por rede (->strParamRede)
	LimpaTList(lisParamEqpto); // lista de parâmetros por equipamento atuado (->strParamEqpto)

	if (arqParam.AnsiCompare("") == 0)
	{
		arqParam = path->DirTmp() + "\\parConfRec.txt";
		if (!FileExists(arqParam))
			return (false);
	}
	else
	{
		if (!FileExists(arqParam))
			return (false);
	}

	fmed = fopen(arqParam.c_str(), "r");

	try
	{ // se necessário, cria um objeto EdtBusca
		// ----------Leitura de parametros gerais----------
		// paramConf->ParamGerais.flagAutoSoc = (StrToInt(LeDado(fmed, flagEOF)) == 1);
		LeDado(fmed, flagEOF);
		paramConf->ParamGerais->flagRecurso = (StrToInt(LeDado(fmed, flagEOF)) == 1);
		paramConf->ParamGerais->flagRetorno = (StrToInt(LeDado(fmed, flagEOF)) == 1);
		paramConf->ParamGerais->tlocaliza = StrToDouble(LeDado(fmed, flagEOF));
		paramConf->ParamGerais->tman_manual = StrToDouble(LeDado(fmed, flagEOF));
		paramConf->ParamGerais->tman_auto = StrToDouble(LeDado(fmed, flagEOF));
		paramConf->ParamGerais->trep = StrToDouble(LeDado(fmed, flagEOF)); // hkdef
		// double lambdaAux = StrToDouble(LeDado(fmed, flagEOF));
		// paramConf->ParamGerais->lambda = (IsDoubleZero(lambdaAux) ? 0.1 : lambdaAux);   //hkdef
		paramConf->ParamGerais->lambda = StrToDouble(LeDado(fmed, flagEOF)); // hkdef
		paramConf->ParamGerais->pu_lim = StrToDouble(LeDado(fmed, flagEOF));
		paramConf->ParamGerais->percDefTrans = StrToDouble(LeDado(fmed, flagEOF));
		buscaProt->IProfReliga = StrToInt(LeDado(fmed, flagEOF)); // hkedp
		aux = LeDado(fmed, flagEOF);
		// --Carrega listas para buscas
		lisRede = redes->LisRede();
		lisCargas = new TList();
		redes->LisEqbar(lisCargas, eqptoCARGA);
		lisChaves = new TList();
		redes->LisEqpto(lisChaves, eqptoCHAVE);
		lisAux = new TList();
		// ----------Leitura de parametros por circuito----------
		// ----------:carrega lisParamRede->paramRede----------
		while (!flagEOF)
		{
			// codCirc;  TAXA_MT; TMA_MT;	cli_mt;	clih_mt;	TAXA_BT;	TMA_BT
			if ((codCirc = LeDado(fmed, flagEOF)).AnsiCompare("PARAMCIRC") == 0)
				break;
			lambdaCirc = StrToDouble(LeDado(fmed, flagEOF));
			treparoCirc = StrToDouble(LeDado(fmed, flagEOF));
			// cli_mt         = StrToDouble(LeDado(fmed, flagEOF));
			// clih_mt        = StrToDouble(LeDado(fmed, flagEOF));
			// taxa_BT        = StrToDouble(LeDado(fmed, flagEOF));
			// tma_BT         = StrToDouble(LeDado(fmed, flagEOF));

			// treparoCirc    = StrToDouble(LeDado(fmed, flagEOF));

			if (codCirc.AnsiCompare("(null)") == 0)
				continue;

			// Insere taxa em circuito
			// Busca rede->Codigo = codCirc
			paramRede = ParametroConfCodRede(codCirc);
			// if(rede == NULL) continue;//rede com o código não encontrado
			if (paramRede == NULL)
			{
				lisParamRede->Add(paramRede = new strDadoParamRede);
				paramRede->rede = NULL;
				// paramRede->flagParamVerif = false;
				// paramRede->flagHist       = false;
			}

			if (paramRede->rede == NULL)
			{
				rede = NULL;
				for (int nr = 0; nr < lisRede->Count; nr++)
				{
					rede = (VTRede*)lisRede->Items[nr];
					aux = codCirc.Trim();
					if (rede->Codigo.AnsiCompare(aux) == 0)
						break;
					rede = NULL;
				}
				paramRede->rede = rede;
				if (paramRede->rede != NULL)
					paramRede->compRede = ComprimentoRede(rede);
			}
			paramRede->codRede = codCirc;
			paramRede->lambda = lambdaCirc;
			paramRede->trep = treparoCirc;
			// hkpc paramRede->flagAjusta = false;
		}
		// ----------Leitura de distribuição de ocorrências por patamar 24h----------
		// ----------:carrega vetor distFalhasPat----------
		double *distFalhasPat = paramConf->DistFalhasPat;
		while (!flagEOF)
		{
			if ((aux = LeDado(fmed, flagEOF)).AnsiCompare("PARAMFALHAPATAMAR") == 0)
				break;
			ipat = StrToInt(aux);
			falha_patamar = StrToDouble(LeDado(fmed, flagEOF));
			if (ipat < numPat)
				distFalhasPat[ipat] = falha_patamar;
		}

		// Redistribui falhas para somar 1.0, soma distFalhasPat[0->23] = 1.0
		RedistribuiFalhasHora();

		// ----------Leitura de conjunto de chaves telecomandadas----------
		// ----------
		lisStrChave = new TStringList();
		while (!flagEOF)
		{
			if ((aux = LeDado(fmed, flagEOF)).AnsiCompare("PARAMCHAVESTELECOM") == 0)
				break;
			codChave = aux;
			lisStrChave->Add(codChave);
		}

		// Inicia lista com chaves automáticas e telecomandadas hknova
		ReiniciaChavesAuto(); // IniciaLisChavesAuto();
		// Preenche lista com chaves da rede
		lisAux->Clear();
		lisAux->Assign(lisChaves, laOr);
		for (int nc1 = lisAux->Count - 1; nc1 >= 0; nc1--)
		{
			pchave = (VTChave*)lisAux->Items[nc1];
			for (int nc = 0; nc < lisStrChave->Count; nc++)
			{
				codChave = lisStrChave->Strings[nc];
				if (pchave->Codigo.AnsiCompare(codChave) != 0)
					continue;
				lisAux->Delete(nc1);
				lisStrChave->Delete(nc);
				// ----------Insere chave em lisChavesAuto--------------
				// -e altera o tipo da chave para chaveRA, religadora---
				InsereChaveAuto(pchave);
				break;
			}
		}

		// VTTipos *tipos =  (VTTipos*)apl->GetObject(__classid(VTTipos));
		// TList   *lisTipoChave = tipos->LisTipoChave();
		// VTTipoChave  *tipoChave;
		AnsiString codTipoChave;
		bool flagProb = false;

		// ----------Leitura de tipos de equipamento de protecao----------
		// --:popula a lista de tipos buscaProt->LisTipoEqProt------------
		lisAux->Clear();
		while (!flagEOF)
		{
			if ((aux = LeDado(fmed, flagEOF)).AnsiCompare("TIPOEQUIPAMENTOPROTECAO") == 0)
				break;
			codTipoChave = aux;
			if (!InsereTipoChave(codTipoChave, lisAux))
				flagProb = true;
			// Busca tipoChave com codigo = codTipoChave
			// tipoChave = NULL;
			// for(int n = 0; n < lisTipoChave->Count; n++)
			// {
			// tipoChave = (VTTipoChave*)lisTipoChave->Items[n];
			// if(tipoChave->Codigo.AnsiCompare(codTipoChave) == 0) break;
			// tipoChave = NULL;
			// }
			// if(tipoChave != NULL)
			// {
			// if(lisAux->IndexOf(tipoChave) == -1) lisAux->Add(tipoChave);
			// }
			// else flagProb = true;

		}
		if (!flagProb)
		{
			buscaProt->LisTipoEqProt->Clear();
			buscaProt->LisTipoEqProt->Assign(lisAux, laOr);
		}

		// PENDENCIA
		// Leitura de parametros relativos a equipamentos - chaves atuadas e cargas relativas a trafos de bt

		strDadoParamEqpto *paramEqpto; // altera
		AnsiString codRede;
		double taxa, trep;
		int itipo;

		lisAux1 = new TList();
		while (!flagEOF)
		{
			if ((aux = LeDado(fmed, flagEOF)).AnsiCompare("PARAMETROSEQPTO") == 0)
				break;
			if (flagEOF)
				break;
			codChave = aux; // LeDado(fmed, flagEOF);
			itipo = StrToInt(LeDado(fmed, flagEOF));
			// eqptoCHAVE: tipo chave, eqptoCARGA: tipo carga equivalente
			codRede = LeDado(fmed, flagEOF); // BT ou CodigoCircuito
			taxa = StrToDouble(LeDado(fmed, flagEOF)); // total de falhas
			trep = StrToDouble(LeDado(fmed, flagEOF)); // tempo total de reparo para total falhas

			paramEqpto = ParametroDadoEqpto(codChave, codRede);
			if (paramEqpto == NULL)
			{
				lisParamEqpto->Add(paramEqpto = new strDadoParamEqpto);
			}
			paramEqpto->codEqpto = codChave;
			paramEqpto->codRede = codRede;
			paramEqpto->rede = BuscaRede(codRede);
			paramEqpto->eqpto = NULL;
			paramEqpto->carga = NULL;
			// hkpc paramEqpto->flagConsIsolado= false;
			// hkpc paramEqpto->flagEncontrado = false;
			switch (itipo)
			{
			case eqptoCHAVE:
				paramEqpto->tipoRede = "MT";
				paramEqpto->eqpto = BuscaChave(paramEqpto->codEqpto, lisChaves);
				// hkpc paramEqpto->flagEncontrado = (paramEqpto->eqpto != NULL);
				break;
			case eqptoCARGA:
				paramEqpto->tipoRede = "BT";
				paramEqpto->carga = BuscaCarga(paramEqpto->codEqpto, lisCargas);
				if (paramEqpto->carga != NULL) // Considera a carga concentrada
				{
					paramEqpto->nconsJus = NCons(paramEqpto->carga);
					paramEqpto->rede = BuscaRedeMT_CargaBT(paramEqpto->carga, lisAux1);
				}
				// hkpc paramEqpto->flagEncontrado = (paramEqpto->carga != NULL);
				if (paramEqpto->carga != NULL)
					paramEqpto->carga->Obj = paramEqpto;
				break;
			default:
				break;
			}

			paramEqpto->nFalhasEquip = taxa;
			paramEqpto->durTotal = trep;
			paramEqpto->ndiasObs = 365;
			paramEqpto->lambda = 0.1;
			paramEqpto->trep = 0.0;
		}

		// hkinclusao de parametros ajustados por bloco
		AnsiString codChave;
		VTChave *chave;
		strDadoParamBloco *paramBloco;
		VTBlocoRad *blocoRad;
		TList *lisParamBloco = paramConf->LisParamBloco;

		LimpaTList(lisParamBloco);
		codChave = LeDado(fmed, flagEOF); // cjto/se
		while (!flagEOF)
		{
			if (codChave.AnsiCompare("PARAMETROSBLOCO") == 0)
				break;
			if ((chave = BuscaChave(codChave, lisChaves)) == NULL)
			{
				LeDado(fmed, flagEOF); // paramBloco->lambda
				LeDado(fmed, flagEOF); // paramBloco->trep
			}
			else
			{
				lisParamBloco->Add(paramBloco = new strDadoParamBloco);
				paramBloco->codChave = codChave;
				paramBloco->eqpto = chave;
				paramBloco->bloco = buscaProt->IdentificaBloco(chave);
				paramBloco->lambda = StrToDouble(LeDado(fmed, flagEOF));
				paramBloco->trep = StrToDouble(LeDado(fmed, flagEOF));
				paramBloco->lisObrasBloco = NULL;
			}
			codChave = LeDado(fmed, flagEOF);
		}

		// hkinclusao de parametros ajustados por carga
		AnsiString codCarga;
		VTCarga *carga;
		strDadoParamCarga *paramCarga;
		LimpaTList(lisParamCarga); // lista de parâmetros por carga
		codCarga = LeDado(fmed, flagEOF); // cjto/se
		while (!flagEOF)
		{
			if (codCarga.AnsiCompare("PARAMETROSCARGABT") == 0)
				break;
			if ((carga = BuscaCarga(codCarga, lisCargas)) == NULL)
			{
				LeDado(fmed, flagEOF); // paramCarga->lambda
				LeDado(fmed, flagEOF); // paramCarga->trep
			}
			else
			{
				lisParamCarga->Add(paramCarga = new strDadoParamCarga);
				paramCarga->codCarga = codCarga;
				paramCarga->rede = BuscaRedeMT_CargaBT(carga, lisAux);
				paramCarga->carga = carga;
				paramCarga->lambda = StrToDouble(LeDado(fmed, flagEOF));
				paramCarga->trep = StrToDouble(LeDado(fmed, flagEOF));
				paramCarga->nconsJus = NCons(carga);
			}
			codCarga = LeDado(fmed, flagEOF);
		}

		// Calcula por paramRede para paramEqpto de MT
		// durTotal
		// nfalhasEquip
		// ndiasObs     em funcao de lisParamEqpto->paramEqpto(MT)

		// 201611: mudanca dos parametros geraLisParamEqpto->CalculaDadosParamRede(paramConf);//->LisParamEqpto, paramConf->LisParamRede);//, paramConf->ParamGerais);//hkmuda1
		geraLisParamEqpto->CalculaDadosParamRede(paramConf->LisParamEqpto, paramConf->LisParamRede,
			paramConf->ParamGerais);
		// ->LisParamEqpto, paramConf->LisParamRede);//, paramConf->ParamGerais);//hkmuda1
		// Reinicia taxas e tempos de reparo lisParamEqpto->paramEqpto(lambda, trep)
		// Funcao de:
		// paramEqpto->durTotal
		// paramEqpto->nFalhasEquip
		// paramEqpto->ndiasObs
		geraLisParamEqpto->ReiniciaDadosParamEqpto(paramConf->LisParamEqpto);
		// Reinicia lisParamBloco->paramBloco->(lambda, trep)
		// caso haja mudancas de topologia ou de equipamentos de protecao
		// Funcao de:
		// paramEqpto->lambda
		// paramEqpto->trep

		// Atualiza baseado em lisParamBloco, lisParamRede, paramDefault:
		// lisDefeitoRede->lisDefRede->lisDefeitos->pdef->[Taxa_falha, TReparo, ComprimentoBloco]
		// Atualiza taxas (por blocos-->por circuito-->valor geral
		// pdef->TaxaFalha         = paramBloco->lambda;
		// pdef->TReparo           = paramBloco->trep;
		// --->pdef->TaxaFalha     = paramRede->lambda;
		// --->pdef->TReparo       = paramRede->trep;
		// --->--->pdef->TaxaFalha = paramDefault->lambda;
		// --->--->pdef->TReparo   = paramDefault->trep;
		// IniciaParamDefeito();

		LimpaTList(lisVerifCjto);
		AnsiString codSe;
		strDadoVerifCjto *verifSe;
		codSe = LeDado(fmed, flagEOF); // cjto/se
		while (!flagEOF)
		{
			if (codSe.AnsiCompare("PARAMVERIFCJTO") == 0)
				break;
			lisVerifCjto->Add(verifSe = new strDadoVerifCjto);
			verifSe->codSe = codSe.Trim();
			aux = LeDado(fmed, flagEOF);
			verifSe->nomeSe = aux.Trim();
			aux = LeDado(fmed, flagEOF);
			verifSe->decVerif = StrToDouble(aux);
			aux = LeDado(fmed, flagEOF);
			verifSe->fecVerif = StrToDouble(aux);
			codSe = LeDado(fmed, flagEOF); // cjto/se
		}

		delete lisAux1;
		delete lisAux;
		delete lisStrChave;

	}
	catch (Exception &e)
	{
		Aviso("Arquivo selecionado pode não ser o correto");
		resposta = false;
	}
	delete lisChaves;
	delete lisCargas;
	fclose(fmed);

	// Executa ajuste de lisParamEqpto->paramEqpto->(lambda, trep)
	// ExecutaAjusteLisParamVerif();

	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TLeArqParamTxt::GravaArqParam(VTParamConf *paramConf, AnsiString pathParam)
{
	buscaProt = (VTBuscaProt*)apl->GetObject(__classid(VTBuscaProt));

	TList *lisParamRede = paramConf->LisParamRede;
	TList *lisParamEqpto = paramConf->LisParamEqpto;
	TList *lisParamBloco = paramConf->LisParamBloco;
	TList *lisParamCarga = paramConf->LisParamCarga;
	TList *lisVerifCjto = paramConf->LisVerifCjto;
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTRede *rede;
	TList *lisRede, *lisCargas;
	AnsiString arq, arqDirConf;
	FILE *fmed;
	AnsiString codCirc, codChaveAuto, aux;
	int ipat, iRec, iRecAuto;
	strDadoParamRede *paramRede;
	double fatAno;

	if (pathParam.AnsiCompare("") == 0)
		arq = path->DirTmp() + "\\parConfRec.txt";
	else
		arq = pathParam + "\\parConfRec.txt";

	fmed = fopen(arq.c_str(), "wt");

	// iRecAuto = (paramConf->ParamGerais.flagAutoSoc) ? 1 : 0;
	// fprintf(fmed, "%i\n", iRecAuto);
	fprintf(fmed, "%i\n", 0);
	iRec = (paramConf->ParamGerais->flagRecurso) ? 1 : 0;
	fprintf(fmed, "%i\n", iRec);
	iRec = (paramConf->ParamGerais->flagRetorno) ? 1 : 0;
	fprintf(fmed, "%i\n", iRec);
	fprintf(fmed, "%10.6f\n", paramConf->ParamGerais->tlocaliza);
	fprintf(fmed, "%10.6f\n", paramConf->ParamGerais->tman_manual);
	fprintf(fmed, "%10.6f\n", paramConf->ParamGerais->tman_auto);
	fprintf(fmed, "%10.6f\n", paramConf->ParamGerais->trep); // hkdef
	double lambdaAux = paramConf->ParamGerais->lambda;
	fprintf(fmed, "%10.6f\n", (IsDoubleZero(lambdaAux) ? 0.1 : lambdaAux)); // hkdef
	fprintf(fmed, "%10.6f\n", paramConf->ParamGerais->pu_lim);
	fprintf(fmed, "%10.6f\n", paramConf->ParamGerais->percDefTrans);
	fprintf(fmed, "%i\n", buscaProt->IProfReliga); // verificar
	fprintf(fmed, "PARAMGERAIS\n");
	// fprintf(fmed, "CHAVESAUTO\n");
	// lisCargas = new TList();

	for (int np = 0; np < lisParamRede->Count; np++)
	{
		paramRede = (strDadoParamRede*)lisParamRede->Items[np];
		if (paramRede->rede == NULL)
			continue; // exclui redes não carregadas
		// if(paramRede->rede == NULL) continue;
		// if(IsDoubleZero(paramRede->lambda)) continue;
		fprintf(fmed, "%s;", paramRede->codRede.c_str());
		fprintf(fmed, "%10.6f;", paramRede->lambda);
		fprintf(fmed, "%10.6f\n", paramRede->trep);
	}

	// delete lisCargas;
	fprintf(fmed, "PARAMCIRC\n");
	double *distFalhasPat = paramConf->DistFalhasPat;
	for (int np = 0; np < numPat; np++)
	{
		fprintf(fmed, "%i;", np);
		fprintf(fmed, "%10.6f\n", distFalhasPat[np]);
	}
	fprintf(fmed, "PARAMFALHAPATAMAR\n");

	VTChave *pchave;
	for (int n = 0; n < buscaProt->LisChvAuto->Count; n++)
	{
		pchave = (VTChave*)buscaProt->LisChvAuto->Items[n];
		fprintf(fmed, "%s\n", pchave->Codigo.c_str());
	}
	fprintf(fmed, "PARAMCHAVESTELECOM\n");

	VTTipoChave *tipoChave;
	TList *lisTipoEqProt = buscaProt->LisTipoEqProt;
	for (int nc = 0; nc < lisTipoEqProt->Count; nc++)
	{
		tipoChave = (VTTipoChave*)lisTipoEqProt->Items[nc];
		fprintf(fmed, "%s\n", tipoChave->Codigo.c_str());
	}
	fprintf(fmed, "TIPOEQUIPAMENTOPROTECAO\n");

	strDadoParamEqpto *paramEqpto;
	// codChave =  aux;//LeDado(fmed, flagEOF);
	// itipo    =  StrToInt(LeDado(fmed, flagEOF)); //eqptoCHAVE: tipo chave, eqptoCARGA: tipo carga equivalente
	// codRede  =  LeDado(fmed, flagEOF);
	// taxa     =  StrToDouble(LeDado(fmed, flagEOF));//total de falhas
	// trep     =  StrToDouble(LeDado(fmed, flagEOF));//tempo total de reparo para total falhas
	for (int n = 0; n < lisParamEqpto->Count; n++)
	{
		paramEqpto = (strDadoParamEqpto*)lisParamEqpto->Items[n];
		// Se não foi localizada a rede não imprime
		if (paramEqpto->rede == NULL)
			continue;

		if (!(paramEqpto->ndiasObs > 0))
			continue;
		// Grava parametros somente de equipamentos da média
		// if(paramEqpto->tipoRede.AnsiCompare("MT") != 0) continue;
		// Se não foi encontrado na rede não grava
		if ((paramEqpto->eqpto == NULL) && (paramEqpto->carga == NULL))
			continue;

		if (paramEqpto->carga != NULL)
			fprintf(fmed, "%s;", paramEqpto->carga->Codigo.c_str());
		else
			fprintf(fmed, "%s;", paramEqpto->eqpto->Codigo.c_str());
		// codEqpto);

		if (paramEqpto->tipoRede == "BT")
			fprintf(fmed, "%i;", eqptoCARGA);
		else
			fprintf(fmed, "%i;", eqptoCHAVE);
		fprintf(fmed, "%s;", paramEqpto->codRede.c_str());
		fatAno = paramEqpto->ndiasObs / 365;
		fprintf(fmed, "%10.6f;%10.6f\n", paramEqpto->nFalhasEquip / fatAno,
			paramEqpto->durTotal / fatAno);
	}
	fprintf(fmed, "PARAMETROSEQPTO\n");

	// hkinclusao de parametros ajustados por bloco
	strDadoParamBloco *paramBloco;
	VTBlocoRad *blocoRad;
	for (int nb = 0; nb < lisParamBloco->Count; nb++)
	{
		paramBloco = (strDadoParamBloco*)lisParamBloco->Items[nb];
		if ((blocoRad = buscaProt->IdentificaBlocoRad(paramBloco->bloco)) == NULL)
			continue;
		if (blocoRad->Chave == NULL)
			continue;
		fprintf(fmed, "%s;%10.6f;%10.6f\n", blocoRad->Chave->Codigo.c_str(), paramBloco->lambda,
			paramBloco->trep);
	}
	fprintf(fmed, "PARAMETROSBLOCO\n");

	strDadoParamCarga *paramCarga;
	for (int n = 0; n < lisParamCarga->Count; n++)
	{
		paramCarga = (strDadoParamCarga*)lisParamCarga->Items[n];
		if (paramCarga->carga == NULL)
			continue;
		fprintf(fmed, "%s;%10.6f;%10.6f\n", paramCarga->carga->Codigo.c_str(), paramCarga->lambda,
			paramCarga->trep);
	}
	fprintf(fmed, "PARAMETROSCARGABT\n");
	strDadoVerifCjto *verifSe;
	for (int n = 0; n < lisVerifCjto->Count; n++)
	{
		verifSe = (strDadoVerifCjto*)lisVerifCjto->Items[n];
		fprintf(fmed, "%s;", verifSe->codSe.c_str());
		fprintf(fmed, "%s;", verifSe->nomeSe.c_str());
		fprintf(fmed, "%10.6f;%10.6f\n", verifSe->decVerif, verifSe->fecVerif);
	}

	fprintf(fmed, "PARAMVERIFCJTO\n");
	// muda
	fclose(fmed);
	// Grava arquivo com diretório para o próximo acesso
	// GravaDirConf();

	return (true);
}

// ---------------------------------------------------------------------------
strDadoParamRede* __fastcall TLeArqParamTxt::ParametroConfCodRede(AnsiString codRede) // ssai
{
	TList *lisParamRede = paramConf->LisParamRede;
	strDadoParamRede* paramRede = NULL;

	for (int np = 0; np < lisParamRede->Count; np++)
	{
		paramRede = (strDadoParamRede*)lisParamRede->Items[np];
		if (paramRede->codRede.AnsiCompare(codRede.Trim()) == 0)
			break;
		paramRede = NULL;
	}
	return (paramRede);
}

// ---------------------------------------------------------------------------
double __fastcall TLeArqParamTxt::ComprimentoRede(VTRede *rede) // ssai
{
	TList *lisLiga;
	VTEqpto *pliga;
	double comp_km = 0.0;

	if (rede == NULL)
		return (0.0);

	lisLiga = rede->LisLigacao();

	for (int n = 0; n < lisLiga->Count; n++)
	{
		pliga = (VTEqpto*)lisLiga->Items[n];
		if (pliga->Tipo() != eqptoTRECHO)
			continue;
		comp_km += ((VTTrecho*)pliga)->Comprimento_km;
	}
	return (comp_km);
}

// ---------------------------------------------------------------------------
void __fastcall TLeArqParamTxt::RedistribuiFalhasHora(void) // ssai
{
	double *distFalhasPat = paramConf->DistFalhasPat;

	double dTot = 0.0;

	for (int np = 0; np < numPat; np++)
		dTot += distFalhasPat[np];

	if (IsDoubleZero(dTot))
	{
		for (int np = 0; np < numPat; np++)
			distFalhasPat[np] = 1.0 / numPat;
	}
	else
	{
		for (int np = 0; np < numPat; np++)
			distFalhasPat[np] = distFalhasPat[np] / dTot;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TLeArqParamTxt::ReiniciaChavesAuto(void)
{
	VTChave *pchave, *pchaveClone;
	// Reinicia chaves previamente clonadas em lisChavesClone
	// busca pchave em lisChavesClone
	for (int n = lisChavesClone->Count - 1; n >= 0; n--)
	{
		pchaveClone = (VTChave*)lisChavesClone->Items[n];
		pchave = (VTChave*)pchaveClone->Obj;

		pchave->CopiaAtributosDe(*pchaveClone);
	}
	LimpaTList(lisChavesClone);

	// hkedp
	while (buscaProt->LisChvAuto->Count > 0)
	{
		pchave = (VTChave*)buscaProt->LisChvAuto->First();
		RemoveChaveAuto(pchave);
	}

	// hkedp buscaProt->LisChvAuto->Clear();

}

// ---------------------------------------------------------------------------
void __fastcall TLeArqParamTxt::RemoveChaveAuto(VTChave *pchave)
{
	VTChave *pchaveClone;

	if (buscaProt->LisChvAuto->IndexOf(pchave) == -1)
		return;
	while (buscaProt->LisChvAuto->IndexOf(pchave) != -1)
		buscaProt->LisChvAuto->Remove(pchave);

	// Caso seja chave nova remove de lista de chaves ativas hkedp
	if (lisChavesNovas->IndexOf(pchave) != -1)
		lisChavesAtivas->Remove(pchave);

	// busca pchave em lisChavesClone
	for (int n = lisChavesClone->Count - 1; n >= 0; n--)
	{
		pchaveClone = (VTChave*)lisChavesClone->Items[n];
		if (((VTChave*)(pchaveClone->Obj)) != pchave)
			continue;
		// *pchave << *pchaveClone;
		pchave->CopiaAtributosDe(*pchaveClone);
		lisChavesClone->Delete(n);
		delete pchaveClone;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TLeArqParamTxt::InsereChaveAuto(VTChave *pchave)
{
	VTTipos *tipos = (VTTipos*)apl->GetObject(__classid(VTTipos));
	VTTipoChave *tipoReliga = NULL;
	VTTipoChave *tipoDisjuntor = NULL;
	VTChave *pchaveClone;

	if (buscaProt->LisChvAuto->IndexOf(pchave) != -1)
		return;

	// busca pchave em lisChavesClone
	// busca pchave em lisChavesClone
	for (int n = lisChavesClone->Count - 1; n >= 0; n--)
	{
		pchaveClone = (VTChave*)lisChavesClone->Items[n];
		if (((VTChave*)pchaveClone->Obj) != pchave)
			continue;
		lisChavesClone->Delete(n);
		delete pchaveClone;
	}

	// InsereChave em lisChavesClone
	lisChavesClone->Add(pchaveClone = pchave->Clone());
	pchaveClone->Obj = (TObject*)pchave;

	buscaProt->LisChvAuto->Add(pchave);

	pchave->Telecomandada = true;
	if ((pchave->TipoChave->Tipo != chaveRA) && (pchave->TipoChave->Tipo != chaveDJ))
	{
		tipoReliga = tipos->ExisteTipoChave(chaveRA);
		if (tipoReliga)
			pchave->TipoChave = tipoReliga;
	}

	// Insere chave na lista de chaves ativas hkedp
	if (lisChavesAtivas->IndexOf(pchave) == -1)
		lisChavesAtivas->Add(pchave);
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TLeArqParamTxt::LeDado(FILE* fp, bool &flagEOF)
{
	AnsiString x;
	char aux;

	flagEOF = false;

	x = "";
	aux = fgetc(fp);
	while ((aux != ';') && (aux != '\n') && (aux != EOF))
	{
		x += AnsiString(aux);
		aux = fgetc(fp);
	}
	if (aux == EOF)
		flagEOF = true;

	return (x);
}

// ---------------------------------------------------------------------------
bool __fastcall TLeArqParamTxt::InsereTipoChave(AnsiString codTipoChave, TList *lisExt)
{
	VTTipos *tipos = (VTTipos*)apl->GetObject(__classid(VTTipos));
	TList *lisTipoChave = tipos->LisTipoChave();
	VTTipoChave *tipoChave;
	bool flagProb = false;

	// Busca tipoChave com codigo = codTipoChave
	tipoChave = NULL;
	for (int n = 0; n < lisTipoChave->Count; n++)
	{
		tipoChave = (VTTipoChave*)lisTipoChave->Items[n];
		if (tipoChave->Codigo.AnsiCompare(codTipoChave) == 0)
			break;
		tipoChave = NULL;
	}

	if (tipoChave != NULL)
	{
		if (lisExt->IndexOf(tipoChave) == -1)
			lisExt->Add(tipoChave);
	}
	else
		flagProb = true;

	return (flagProb);
}

// ---------------------------------------------------------------------------
strDadoParamEqpto* __fastcall TLeArqParamTxt::ParametroDadoEqpto(AnsiString codChave,
	AnsiString codRede) // ssai
{
	TList *lisParamEqpto = paramConf->LisParamEqpto;
	strDadoParamEqpto *paramEqpto;
	// busca paramEqpto
	paramEqpto = NULL;
	for (int np = 0; np < lisParamEqpto->Count; np++)
	{
		paramEqpto = (strDadoParamEqpto*)lisParamEqpto->Items[np];
		if ((paramEqpto->codEqpto.AnsiCompare(codChave) == 0) && (paramEqpto->codRede.AnsiCompare
			(codRede) == 0))
		{
			break;
		}
		paramEqpto = NULL;
	}
	return (paramEqpto);
}

// ---------------------------------------------------------------------------
VTRede* __fastcall TLeArqParamTxt::BuscaRede(AnsiString codRede)
{
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTRede *rede;
	TList *lisRede = redes->LisRede();

	rede = NULL;
	for (int nr = 0; nr < lisRede->Count; nr++)
	{
		rede = (VTRede*)lisRede->Items[nr];
		// hknova if(!rede->Carregada) continue;
		if ((rede->Codigo.AnsiCompare(codRede) == 0) && (rede->Carregada))
			break;
		rede = NULL;
	}
	return (rede);
}

// ---------------------------------------------------------------------------
VTChave* __fastcall TLeArqParamTxt::BuscaChave(AnsiString codChave, TList *lisChave)
{
	VTChave *chave;
	chave = NULL;
	for (int n = 0; n < lisChave->Count; n++)
	{
		chave = (VTChave*)lisChave->Items[n];
		if (chave->Codigo.AnsiCompare(codChave) == 0)
			break;
		chave = NULL;
	}
	return (chave);
}

// ---------------------------------------------------------------------------
VTCarga* __fastcall TLeArqParamTxt::BuscaCarga(AnsiString codCarga, TList *lisCarga)
{
	VTCarga *carga;
	carga = NULL;
	for (int n = 0; n < lisCarga->Count; n++)
	{
		carga = (VTCarga*)lisCarga->Items[n];
		if (carga->Codigo.AnsiCompare(codCarga) == 0)
			break;
		carga = NULL;
	}
	return (carga);
}

// ---------------------------------------------------------------------------
int __fastcall TLeArqParamTxt::NCons(VTCarga *carga)
{
	int naux = 0;

	naux += carga->NumConsResidencial;
	naux += carga->NumConsComercial;
	naux += carga->NumConsIndustrial;
	naux += carga->NumConsRural;
	naux += carga->NumConsOutros; // MISTURADO COM IP!!!!
	naux += carga->NumConsA4;
	// Valor mínimo de consumidores
	if (naux == 0)
		naux = 1;

	return (naux);
}

// ---------------------------------------------------------------------------
VTRede* __fastcall TLeArqParamTxt::BuscaRedeMT_CargaBT(VTCarga *cargaBT, TList *lisAux)
{
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTLigacao *ligacao;
	lisAux->Clear();
	redes->LisLigacao(lisAux, cargaBT->pbarra);
	if (lisAux->Count == 0)
		return (NULL);

	return (((VTLigacao*)lisAux->First())->rede);
}
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
