// ---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
#include <limits>
#include "..\Apl\VTApl.h"
#include "..\Arranjo\VTArranjo.h"
#include "..\Geral\VTInfoset.h"
#include "..\Geral\VTGeral.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTFases.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTTrafo.h"
#include "..\Rede\VTTrecho.h"
#include "..\Rede\VTTipos.h"
#include "..\Rede\VTTipoRede.h"
#include "..\Zona\VTZona.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "TDiagTrafo.h"
#include "TFormCatalogoTrf.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
// ---------------------------------------------------------------------------
// define do nome do arquivo
#define FILE_CFG_TRAFO "CfgDiagEqptoTrafo"

// ---------------------------------------------------------------------------
// VTDiagEqptoElem* __fastcall NewObjDiagEqptoElem(VTApl *apl)
// {
// return (new TDiagEqptoTrafo(apl));
// }
// ---------------------------------------------------------------------------
__fastcall TDiagTrafo::TDiagTrafo(VTApl *apl)
{
	this->apl = apl;
	// define se tem informações adicionais
	infoExtra = false;
	// cria a lista de configuracoes
	listCfg = new TList;
	// inicializa as opcoes do diagnostico
	dgTrafo.existeNoCatalogo = true;
	dgTrafo.vnom = true;
	dgTrafo.vtap = true;
	dgTrafo.snom = true;
	dgTrafo.banco = true;
	// dgTrafo.fases = true;
	dgTrafo.perdafe = true;
	dgTrafo.z = true;
	dgTrafo.ligIncoerente = true;
	// dgTrafo.isolado = true;
	// cria o log_trafo
	/* TODO : mudar para o executa? */
	log_trafo = new TStringList;
	// Mostra que ainda não foi executado
	executado = false;
}

// ---------------------------------------------------------------------------
__fastcall TDiagTrafo::~TDiagTrafo(void)
{
	// destroi lista e conteudo
	if (listCfg)
	{
		LimpaTList(listCfg);
		delete listCfg;
		listCfg = NULL;
	}
	// destroi log
	if (log_trafo)
	{
		delete log_trafo;
		log_trafo = NULL;
	}
	// destroi form
	if (formCatalogoTrf)
	{
		delete formCatalogoTrf;
		formCatalogoTrf = NULL;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TDiagTrafo::AddLinhaLog(VTTrafo *trafo, int opDIAG_TRAFO, DIAG_TRAFO *cfgDiagTrafo,
	AnsiString desc, int enrolamento)
{
	// variaveis locais
	VTFases *fases = (VTFases*) apl->GetObject(__classid(VTFases));
	VTTipoRede *tiporede = NULL;

	// incrementa o contador de erros
	// infoTrafos.n_ocorrencias++;
	temErro = true;
	// vetor de strings seguindo o enum col_LOG_TRECHO
	AnsiString linhaPreenchida = "";
	AnsiString linha[colTrf_COUNT];
	AnsiString Separador = ";";
	// inicializa com os valores vindos do trecho
	if (trafo->rede)
	{
		tiporede = trafo->rede->TipoRede;
		if (tiporede)
		{
			linha[colTrf_SEGMENTO] = trafo->rede->TipoRede->Codigo + Separador;
		}
		else
		{
			linha[colTrf_SEGMENTO] = "Nenhum" + Separador;
		}
		linha[colTrf_REDE] = trafo->rede->Codigo + Separador;
	}
    else
	{
		linha[colTrf_SEGMENTO] = "Nenhum";
		linha[colTrf_REDE] = "Nenhum";
	}
	// linha[colLog_EQPTO]     = trecho->TipoEqpto_AsString(trecho->Tipo()) + Separador;
	linha[colTrf_EQPTO] = "Trafo" + Separador;
	linha[colTrf_CODIGO] = trafo->Codigo + Separador;
	linha[colTrf_CODBARRA1] = trafo->pbarra1->Codigo + Separador;
	linha[colTrf_CODBARRA2] = trafo->pbarra2->Codigo + Separador;

	if (trafo)
	{
		switch (opDIAG_TRAFO)
		{ // caso verificação de existencia no catalogo
		case opTrfCATALOGO:
			linha[colTrf_TIPOERRO] = str_TIPOERRO_AVISO + Separador;
			linha[colTrf_DESCRICAO] = "Transformadores não encontrado no catálogo." + Separador;
			linha[colTrf_PADRAO] = " - " + Separador;
			linha[colTrf_VAL_PADRAO] = " - " + Separador;
			linha[colTrf_VAL_TRAFO] = DoubleToStr("%4.3f", trafo->snom) + " [MVA]";

			break;
			// caso verificação de coerencia de Vnom com catalogo
		case opTrfVNOM:
			linha[colTrf_TIPOERRO] = str_TIPOERRO_AVISO + Separador;
			linha[colTrf_DESCRICAO] = desc + Separador;
			// caso tenha encontrado padrao, mostra o nome do item do catalogo
			if (cfgDiagTrafo)
			{
				linha[colTrf_PADRAO] = cfgDiagTrafo->codigo + Separador;
			}
			else
			{
				linha[colTrf_PADRAO] = " - " + Separador;
			}
			// switch/case pros enrolamentos
			switch (enrolamento)
			{
			case 1: // enrolamento 1, primário
				// caso tenha encontrado padrao, mostra o nome do item do catalogo
				if (cfgDiagTrafo)
					linha[colTrf_VAL_PADRAO] = DoubleToStr("%4.3f", cfgDiagTrafo->vnomPri_kv) +
						" [kV]" + Separador;
				else
				{
					linha[colTrf_VAL_PADRAO] = " - " + Separador;
				}
				linha[colTrf_VAL_TRAFO] = DoubleToStr("%4.3f", trafo->pri.vnom) + " [kV]";
				break;

			case 2: // enrolamento 2, secundário
				// caso tenha encontrado padrao, mostra o nome do item do catalogo
				if (cfgDiagTrafo)
					linha[colTrf_VAL_PADRAO] = DoubleToStr("%4.3f", cfgDiagTrafo->vnomSec_kv) +
						" [kV]" + Separador;
				else
				{
					linha[colTrf_VAL_PADRAO] = " - " + Separador;
				}
				linha[colTrf_VAL_TRAFO] = DoubleToStr("%4.3f", trafo->sec.vnom) + " [kV]";
				break;

			default:
				linha[colTrf_VAL_PADRAO] = " - " + Separador;
				linha[colTrf_VAL_TRAFO] = " - "; ;
			}
			break;
			// caso verificação de coerencia de Vtap
		case opTrfVTAP:
			linha[colTrf_TIPOERRO] = str_TIPOERRO_AVISO + Separador;
			linha[colTrf_DESCRICAO] = desc + Separador;
			linha[colTrf_PADRAO] = " - " + Separador;
			linha[colTrf_VAL_PADRAO] = " - " + Separador;
			// switch/case pros enrolamentos
			switch (enrolamento)
			{
			case 1: // enrolamento 1, primário
				linha[colTrf_VAL_TRAFO] = DoubleToStr("%4.3f", trafo->pri.tap) + " [kV]";
				break;

			case 2: // enrolamento 2, secundário
				linha[colTrf_VAL_TRAFO] = DoubleToStr("%4.3f", trafo->sec.tap) + " [kV]";
				break;

			default:
				linha[colTrf_VAL_TRAFO] = " - "; ;
			}
			break;
			// caso verificação de coerencia de Vnom com catalogo
		case opTrfSNOM:
			linha[colTrf_TIPOERRO] = str_TIPOERRO_AVISO + Separador;
			linha[colTrf_DESCRICAO] = desc + Separador;
			linha[colTrf_VAL_TRAFO] = DoubleToStr("%4.3f", trafo->snom) + " [MVA]";
			// caso tenha encontrado padrao, mostra o nome do item do catalogo
			if (cfgDiagTrafo)
			{
				linha[colTrf_PADRAO] = cfgDiagTrafo->codigo + Separador;
			}
			else
			{
				linha[colTrf_PADRAO] = " - " + Separador;
			}
			if (cfgDiagTrafo)
				linha[colTrf_VAL_PADRAO] = DoubleToStr("%4.3f", cfgDiagTrafo->snom_mva) + " [MVA]" +
					Separador;
			else
			{
				linha[colTrf_VAL_PADRAO] = " - " + Separador;
			}
			break;

			// caso verificação de existencia no catalogo
		case opTrfPERDAFE:
			linha[colTrf_TIPOERRO] = str_TIPOERRO_AVISO + Separador;
			linha[colTrf_DESCRICAO] = desc + Separador;
			// caso tenha encontrado padrao, mostra o nome do item do catalogo
			if (cfgDiagTrafo)
				linha[colTrf_PADRAO] = cfgDiagTrafo->codigo + Separador;
			else
			{
				linha[colTrf_PADRAO] = " - " + Separador;
			}
			// caso tenha encontrado padrao, mostra o valor do item do catalogo
			if (cfgDiagTrafo)
				linha[colTrf_VAL_PADRAO] = DoubleToStr("%4.3f", cfgDiagTrafo->perdaFe) + " [%]" +
					Separador;
			else
			{
				linha[colTrf_VAL_PADRAO] = " - " + Separador;
			}
			// caso tenha trafo, mostra o valor do item do catalogo
			linha[colTrf_VAL_TRAFO] = DoubleToStr("%4.3f", trafo->perda_ferro) + " [%]";
			break;

			// caso verificação de coerencia de Fases com catalogo
			// case opTrfFASES:
			// linha[colTrf_TIPOERRO]  = str_TIPOERRO_AVISO + Separador;
			// linha[colTrf_DESCRICAO] = desc + Separador;
			// linha[colTrf_PADRAO] =" - " + Separador;
			// //switch/case pros enrolamentos
			// switch (enrolamento)
			// {
			// case 1: //enrolamento 1, primário
			// linha[colTrf_VAL_PADRAO] =" - " + Separador;
			// linha[colTrf_VAL_TRAFO]  = fases->AsStrABCN[trafo->pri.fase];
			// break;
			//
			// case 2: //enrolamento 2, secundário
			// //caso tenha encontrado padrao, mostra o nome do item do catalogo
			// linha[colTrf_VAL_PADRAO] = " - " + Separador;
			// linha[colTrf_VAL_TRAFO] = fases->AsStrABCN[trafo->sec.fase];
			// break;
			//
			// default:
			// linha[colTrf_VAL_PADRAO]= " - " + Separador;
			// linha[colTrf_VAL_TRAFO] = " - " ;
			// }
			// break;

			// caso verificação de coerencia de Fases com catalogo
		case opTrfLIGINCOERENTE:
			linha[colTrf_TIPOERRO] = str_TIPOERRO_ERRO + Separador;
			linha[colTrf_DESCRICAO] = desc + Separador;
			linha[colTrf_PADRAO] = " - " + Separador;
			// switch/case pros enrolamentos
			switch (enrolamento)
			{
			case 1: // enrolamento 1, primário
				linha[colTrf_VAL_PADRAO] = " - " + Separador;
				linha[colTrf_VAL_TRAFO] = fases->AsStrABCN[trafo->pri.fase];
				break;

			case 2: // enrolamento 2, secundário
				// caso tenha encontrado padrao, mostra o nome do item do catalogo
				linha[colTrf_VAL_PADRAO] = " - " + Separador;
				linha[colTrf_VAL_TRAFO] = fases->AsStrABCN[trafo->sec.fase];
				break;

			default:
				linha[colTrf_VAL_PADRAO] = " - " + Separador;
				linha[colTrf_VAL_TRAFO] = " - ";
			}
			break;

			// caso verificação de coerencia de Fases com catalogo
		case opTrfBANCOMONO:
			linha[colTrf_TIPOERRO] = str_TIPOERRO_ERRO + Separador;
			linha[colTrf_DESCRICAO] = desc + Separador;
			linha[colTrf_PADRAO] = " - " + Separador;
			linha[colTrf_VAL_PADRAO] = " - " + Separador;
			linha[colTrf_VAL_TRAFO] = " - ";
			break;

		case opTrfIMPEDANCIA:
			linha[colTrf_TIPOERRO] = str_TIPOERRO_AVISO + Separador;
			linha[colTrf_DESCRICAO] = desc + Separador;
			linha[colTrf_PADRAO] = " - " + Separador;
			linha[colTrf_VAL_PADRAO] = " - " + Separador;
			linha[colTrf_VAL_TRAFO] = " - ";
			break;

			// case opTrfISO:
			// linha[colTrf_TIPOERRO]  = str_TIPOERRO_AVISO + Separador;
			// linha[colTrf_DESCRICAO] = "Trafo isolado." + Separador;
			// linha[colTrf_PADRAO] = " - " + Separador;
			// linha[colTrf_VAL_PADRAO] = " - " + Separador;
			// linha[colTrf_VAL_TRAFO] = " - ";
			// break;

		default:
			linha[colTrf_TIPOERRO] = " - " + Separador;
			linha[colTrf_DESCRICAO] = " - " + Separador;
			linha[colTrf_PADRAO] = " - " + Separador;
			linha[colTrf_VAL_PADRAO] = " - " + Separador;
			linha[colTrf_VAL_TRAFO] = " - "; ;
		}
		// concatena os campos em uma linha
		for (int i = 0; i < colTrf_COUNT; i++)
		{
			linhaPreenchida += linha[i];
		}
		log_trafo->AddObject(linhaPreenchida, trafo);
	}
}

// ---------------------------------------------------------------------------
int __fastcall TDiagTrafo::ContaNeutros(TList *lisTrafos, int enrolamento)
{ // variaveis locais
	VTTrafo *trafo;
	int numNeutro = 0;

	for (int nt = 0; nt < lisTrafos->Count; nt++)
	{
		trafo = (VTTrafo*) lisTrafos->Items[nt];
		if ((enrolamento == 0) || (enrolamento == 1))
		{
			if ((trafo->pri.fase & faseN) == faseN)
			{
				numNeutro++;
			}
		}
		if ((enrolamento == 0) || (enrolamento == 2))
		{
			if ((trafo->sec.fase & faseN) == faseN)
			{
				numNeutro++;
			}
		}
	}
	return numNeutro;
}

// ---------------------------------------------------------------------------
DIAG_TRAFO* __fastcall TDiagTrafo::CriaCfgTrafoDefault(int id, AnsiString codigo)
{
	DIAG_TRAFO *cfgDiagTrafo;

	cfgDiagTrafo = new DIAG_TRAFO;
	cfgDiagTrafo->id = id;
	cfgDiagTrafo->codigo = codigo;
	cfgDiagTrafo->snom_mva = 0.75;
	cfgDiagTrafo->vnomPri_kv = 13.8;
	cfgDiagTrafo->vnomSec_kv = 0.24;
	cfgDiagTrafo->perdaFe = 0.5;
	// cfgDiagTrafo->ligacaoPri = lgTRIANG;
	// cfgDiagTrafo->ligacaoPri = lgEST_AT;

	return cfgDiagTrafo;
}

// ---------------------------------------------------------------------------
bool __fastcall TDiagTrafo::DeltaAbertoValido(VTTrafo *trafo1, VTTrafo *trafo2, int enrolamento)
{ // variaveis locais
	// VTZona   *zona;
	// VTBarra  *barra;
	VTFases *fases = (VTFases*) apl->GetObject(__classid(VTFases));
	int faseTrafo1, faseTrafo2;
	VTTrafo *trafoComNeutro = NULL;
	VTTrafo *trafoSemNeutro = NULL;
	bool valido = true;
	TList *listTemp = new TList();

	// adiciona os trafos à lista temporaria
	listTemp->Add(trafo1);
	listTemp->Add(trafo2);
	// barras do primário diferentes
	if (trafo1->BarraPrimario() != trafo2->BarraPrimario())
		return false;
	// barras do secundário diferentes
	if (trafo1->BarraSecundario() != trafo2->BarraSecundario())
		return false;
	// verifica enrolamento 1 (primário)
	if ((enrolamento == 0) || (enrolamento == 1))
	{ // verifica só a barra do primário
		faseTrafo1 = trafo1->pri.fase & faseABC;
		faseTrafo2 = trafo2->pri.fase & faseABC;
		// caso eles estejam ligados À mesma fase, tem algo errado
		if ((faseTrafo1 & faseTrafo2) != 0x00)
		{
			AddLinhaLog(trafo1, opTrfBANCOMONO, NULL,
				"Delta Aberto com primários ligados à mesma fase.");
			valido = false;
		}
		// verifica se os primários tem aterramento
		if (((trafo1->pri.fase & faseT) != faseT) || ((trafo2->pri.fase & faseT) != faseT))
		{
			AddLinhaLog(trafo1, opTrfBANCOMONO, NULL, "Delta Aberto com primários não aterrados.");
			valido = false;
		}
		// verifica se ambos primários tem neutro
		if (ContaNeutros(listTemp, 1) != 2)
		{
			AddLinhaLog(trafo1, opTrfBANCOMONO, NULL, "Delta Aberto com primários faltando neutro");
			valido = false;
		}
	} // verifica enrolamento 2 (secundário)
	if ((enrolamento == 0) || (enrolamento == 2))
	{ // verifica só a barra do secundário
		// verifica se pelo menos 1 dos trafos possui neutro
		if ((trafo1->sec.fase & faseN) == faseN)
		{
			trafoComNeutro = trafo1;
			trafoSemNeutro = trafo2;
		}
		if ((trafo2->sec.fase & faseN) == faseN)
		{
			trafoComNeutro = trafo2;
			trafoSemNeutro = trafo1;
		}
		// nenhum trafo possuiu neutro
		if (!trafoComNeutro)
		{
			AddLinhaLog(trafo1, opTrfBANCOMONO, NULL, "Delta Aberto sem neutro no secundário.");
			valido = false;
		}
		// caso os 2 trafos possuam neutro no secundário...
		if (((trafo1->sec.fase & faseN) == faseN) && ((trafo2->sec.fase & faseN) == faseN))
		{
			AddLinhaLog(trafo1, opTrfBANCOMONO, NULL,
			"Delta Aberto com dois neutros no secundário");
			valido = false;
		}
		// caso exista trafo com neutro
		if (trafoComNeutro && trafoSemNeutro)
		{
			// verifica se o trafo com neutro possui o terra
			if ((trafoComNeutro->sec.fase & faseT) != faseT)
			{
				AddLinhaLog(trafo1, opTrfBANCOMONO, NULL,
					"Delta Aberto sem neutro aterrado no secundário.");
				valido = false;
			}
		}
		// guarda as fases dos secundarios dos trafos sem NT
		faseTrafo1 = trafo1->sec.fase & faseABC;
		faseTrafo2 = trafo2->sec.fase & faseABC;
		// verifica se o trafo  possui 2 fases, ex: ABN
		if (fases->NumeroDeFases(faseTrafo1) != 2)
		{
			AddLinhaLog(trafo1, opTrfBANCOMONO, NULL,
				"Delta Aberto com fases incoerentes no secundário");
			valido = false;
		}
		// verifica se o outro trafo tem 2 fases ex: BC
		if (fases->NumeroDeFases(faseTrafo2) != 2)
		{
			AddLinhaLog(trafo1, opTrfBANCOMONO, NULL,
				"Delta Aberto com fases incoerentes no secundário");
			valido = false;
		}
		// verifica se forma um triangulo e se tem 1 fase em comum
		if (((faseTrafo1 | faseTrafo2) != faseABC) && ((faseTrafo1 & faseTrafo2) != 0x00))
		{
			AddLinhaLog(trafo1, opTrfBANCOMONO, NULL,
				"Delta Aberto que não forma um delta no secundário.");
			valido = false;
		}
	}
	// deleta a lista temporaria
	if (listTemp)
	{
		delete listTemp;
		listTemp = NULL;
	}
	return valido;
}

// ---------------------------------------------------------------------------
bool __fastcall TDiagTrafo::DeltaFechadoValido(VTTrafo *trafo1, VTTrafo *trafo2, VTTrafo *trafo3,
	int enrolamento)
{ // variaveis locais
	// VTZona   *zona;
	// VTBarra  *barra;
	VTFases *fases = (VTFases*) apl->GetObject(__classid(VTFases));
	int faseTrafo1, faseTrafo2, faseTrafo3;
	VTTrafo *trafoComNeutro = NULL;
	VTTrafo *trafoSemNeutro1 = NULL;
	VTTrafo *trafoSemNeutro2 = NULL;
	TList *listTemp = new TList();
	bool valido = true;

	// preenche a lista temporaria com os trafos;
	listTemp->Add(trafo1);
	listTemp->Add(trafo2);
	listTemp->Add(trafo3);
	// barras do primário diferentes
	if (trafo1->BarraPrimario() != trafo2->BarraPrimario())
		return false;
	if (trafo1->BarraPrimario() != trafo3->BarraPrimario())
		return false;
	// barras do secundário diferentes
	if (trafo1->BarraSecundario() != trafo2->BarraSecundario())
		return false;
	if (trafo1->BarraSecundario() != trafo3->BarraSecundario())
		return false;

	// verifica enrolamento 1 (primário)
	if ((enrolamento == 0) || (enrolamento == 1))
	{ // verifica só a barra do primário
		faseTrafo1 = trafo1->pri.fase & faseABC;
		faseTrafo2 = trafo2->pri.fase & faseABC;
		faseTrafo3 = trafo3->pri.fase & faseABC;
		// caso eles estejam ligados À mesma fase, tem algo errado
		if (((faseTrafo1 & faseTrafo2) != 0x00) || ((faseTrafo1 & faseTrafo3) != 0x00) ||
			((faseTrafo2 & faseTrafo3) != 0x00))
		{
			AddLinhaLog(trafo1, opTrfBANCOMONO, NULL,
				"Delta Fechado com primários ligados à mesma fase.");
			valido = false;
		}
		// verifica se os primários tem aterramento
		if (((trafo1->pri.fase & faseT) == faseT) || ((trafo2->pri.fase & faseT) == faseT) ||
			((trafo3->pri.fase & faseT) == faseT))
		{
			AddLinhaLog(trafo1, opTrfBANCOMONO, NULL, "Delta Fechado com primários aterrados.");
			valido = false;
		}
	} // verifica enrolamento 2 (secundário)
	if ((enrolamento == 0) || (enrolamento == 2))
	{ // verifica só a barra do secundário
		// verifica se pelo menos 1 dos trafos possui neutro
		if ((trafo1->sec.fase & faseN) == faseN)
		{
			trafoComNeutro = trafo1;
			trafoSemNeutro1 = trafo2;
			trafoSemNeutro2 = trafo3;
		}
		if ((trafo2->sec.fase & faseN) == faseN)
		{
			trafoComNeutro = trafo2;
			trafoSemNeutro1 = trafo1;
			trafoSemNeutro2 = trafo3;
		}
		if ((trafo3->sec.fase & faseN) == faseN)
		{
			trafoComNeutro = trafo3;
			trafoSemNeutro1 = trafo1;
			trafoSemNeutro2 = trafo2;
		}
		// nenhum trafo possuiu neutro
		if (!trafoComNeutro)
		{
			AddLinhaLog(trafo1, opTrfBANCOMONO, NULL, "Delta Fechado sem neutro no secundário.");
			valido = false;
		}
		// caso os 2+trafos possuam neutro no secundário...
		if (ContaNeutros(listTemp, 2) != 1)
		{
			AddLinhaLog(trafo1, opTrfBANCOMONO, NULL,
				"Delta Fechado com dois ou mais neutros no secundário");
			valido = false;
		}
		// caso exista trafo com neutro
		if (trafoComNeutro && trafoSemNeutro1 && trafoSemNeutro2)
		{ // verifica se o trafo com neutro possui o terra
			if ((trafoComNeutro->sec.fase & faseT) != faseT)
			{
				AddLinhaLog(trafo1, opTrfBANCOMONO, NULL,
					"Delta Fechado sem neutro aterrado no secundário.");
				valido = false;
			}
		}
		// guarda as fases dos secundarios dos trafos sem NT
		faseTrafo1 = trafo1->sec.fase & faseABC;
		faseTrafo2 = trafo2->sec.fase & faseABC;
		faseTrafo3 = trafo3->sec.fase & faseABC;
		// verifica se o trafo com neutro possui 2 fases, ex: ABN
		if (fases->NumeroDeFases(faseTrafo1) != 2)
		{
			AddLinhaLog(trafo1, opTrfBANCOMONO, NULL,
				"Delta Fechado com fases incoerentes no secundário");
			valido = false;
		}
		// verifica se o outro trafo tem 2 fases ex: BC
		if (fases->NumeroDeFases(faseTrafo2) != 2)
		{
			AddLinhaLog(trafo1, opTrfBANCOMONO, NULL,
				"Delta Fechado com fases incoerentes no secundário");
			valido = false;
		}
		// verifica se o outro trafo tem 2 fases ex: CA
		if (fases->NumeroDeFases(faseTrafo3) != 2)
		{
			AddLinhaLog(trafo1, opTrfBANCOMONO, NULL,
				"Delta Fechado com fases incoerentes no secundário");
			valido = false;
		}
		// verifica se forma um triangulo e se tem 1 fase em comum
		if (((faseTrafo1 | faseTrafo2) != faseABC) && ((faseTrafo1 & faseTrafo2) != 0x00))
		{
			AddLinhaLog(trafo1, opTrfBANCOMONO, NULL,
				"Delta Fechado que não forma um delta no secundário.");
			valido = false;
		}
	}
	// deleta a lista temporaria
	if (listTemp)
	{
		delete listTemp;
		listTemp = NULL;
	}
	// retorna o se é valido ou não
	return valido;
}

// ---------------------------------------------------------------------------
// bool     __fastcall  TDiagTrafo::Executa(void)
// {  //variaveis locais
// VTRedes     *redes =  (VTRedes*) apl->GetObject(__classid(VTRedes));
// //VTRede      *rede;
// //VTLigacao   *ligacao;
// VTTrafo    *trafo;
// TList      *LisTrafo = new TList();
//
// if(log_trafo){log_trafo->Clear();}
// try
// {
// //pega a lista de todos os Trafos (excluindo-se os 3E e zig zag)
// redes->LisLigacao(LisTrafo,eqptoTRAFO);
// //percorre todas os trafos
// for(int nt = 0; nt < LisTrafo->Count; nt++)
// {
// trafo = (VTTrafo*) LisTrafo->Items[nt];
// if(trafo){VerificaTrafo(trafo);}
// }
// //altera o boolean mostrando que foi executado
// executado = true;
// //deleta a lista
// if(LisTrafo){delete LisTrafo; LisTrafo = NULL;}
// return true;
// }
// catch(Exception &e)
// {return false;}
// }

// ---------------------------------------------------------------------------
bool __fastcall TDiagTrafo::Executa(void)
{ // variaveis locais
	VTRedes *redes = (VTRedes*) apl->GetObject(__classid(VTRedes));
	// VTRede      *rede;
	// VTLigacao   *ligacao;
	VTTrafo *trafo, *trafo2, *trafo3;
	TList *LisTrafo = new TList();
	TList *LisTrafoDeltaAberto = new TList();
	TList *LisTrafoDeltaFechado = new TList();

	// inicialza infos
	infoTrafos.n_total = 0;
	infoTrafos.n_verif = 0;
	infoTrafos.n_ocorrencias = 0;
	if (log_trafo)
		IniciaLog(log_trafo);
	// if(log_trafo){log_trafo->Clear();}
	try
	{
		// pega a lista de todos os Trafos (excluindo-se os 3E e zig zag)
		redes->LisLigacao(LisTrafo, eqptoTRAFO);
		// conta quantos trafos
		infoTrafos.n_total = LisTrafo->Count;
		// separa a lista de trafos em , delta aberto, delta fechado, outros
		SeparaBancosMono(LisTrafo, LisTrafoDeltaAberto, LisTrafoDeltaFechado);
		// percorre os outros trafos
		for (int nt = 0; nt < LisTrafo->Count; nt++)
		{

			trafo = (VTTrafo*) LisTrafo->Items[nt];
			temErro = false;
			if (trafo)
			{
				VerificaTrafo(trafo);
			}
			if (temErro)
				infoTrafos.n_ocorrencias++;
			if (nt == 470)
			{
				int a = 0;
			}
		}
		// percorre os delta aberto
		for (int nt = 0; nt < LisTrafoDeltaAberto->Count; nt++)
		{
			trafo = (VTTrafo*) LisTrafoDeltaAberto->Items[nt];
			temErro = false;
			if (trafo)
			{
				VerificaTrafo(trafo);
			}
			// a cada 2 começa um banco novo
			if ((nt % 2) == 0)
			{
				trafo2 = NULL;
				if ((nt + 1) < LisTrafoDeltaAberto->Count)
					trafo2 = (VTTrafo*) LisTrafoDeltaAberto->Items[nt + 1];
				if ((trafo2) && (dgTrafo.banco))
					VerificaOpBancoMono(trafo, trafo2);
			}
			if (temErro)
				infoTrafos.n_ocorrencias++;
		}
		// percorre os delta fechados
		for (int nt = 0; nt < LisTrafoDeltaFechado->Count; nt++)
		{
			trafo = (VTTrafo*) LisTrafoDeltaFechado->Items[nt];
			temErro = false;
			if (trafo)
			{
				VerificaTrafo(trafo);
			}
			// a cada 3 começa um banco novo
			if ((nt % 3) == 0)
			{
				trafo2 = NULL;
				if ((nt + 1) < LisTrafoDeltaFechado->Count)
					trafo2 = (VTTrafo*) LisTrafoDeltaFechado->Items[nt + 1];
				trafo3 = NULL;
				if ((nt + 2) < LisTrafoDeltaFechado->Count)
					trafo3 = (VTTrafo*) LisTrafoDeltaFechado->Items[nt + 2];
				if ((trafo2) && (trafo3) && (dgTrafo.banco))
					VerificaOpBancoMono(trafo, trafo2, trafo3);
			}
			if (temErro)
				infoTrafos.n_ocorrencias++;
		}
		// altera o boolean mostrando que foi executado
		executado = true;
		// deleta a lista
		if (LisTrafo)
		{
			delete LisTrafo;
			LisTrafo = NULL;
		}
		if (LisTrafoDeltaAberto)
		{
			delete LisTrafoDeltaAberto;
			LisTrafoDeltaAberto = NULL;
		}
		if (LisTrafoDeltaFechado)
		{
			delete LisTrafoDeltaFechado;
			LisTrafoDeltaFechado = NULL;
		}
		return true;
	}
	catch (Exception &e)
	{
		return false;
	}
}

// ---------------------------------------------------------------------------
// bool     __fastcall  TDiagTrafo::ExisteCfgTipoRede(int redeSEGMENTO)
// {  //variaveis locais
// DIAG_TRAFO *cfgDiagTrafo;
//
// for(int nt = 0; nt < listCfg->Count; nt++)
// {
// cfgDiagTrafo = (DIAG_TRAFO*) listCfg->Items[nt];
// if (cfgDiagTrafo->tipo_rede == redeSEGMENTO) {return true;}
// }
// return false;
// }

// ---------------------------------------------------------------------------
DIAG_TRAFO* __fastcall TDiagTrafo::Get_DiagTrafo(int id)
{ // variaveis locais
	DIAG_TRAFO *cfgDiagTrafo;

	for (int i = 0; i < listCfg->Count; i++)
	{
		cfgDiagTrafo = (DIAG_TRAFO*) listCfg->Items[i];
		if (id == cfgDiagTrafo->id)
		{
			return cfgDiagTrafo;
		}
	}
	return NULL;
}

// ---------------------------------------------------------------------------
void __fastcall TDiagTrafo::Inicia(void)
{
	// primeiramente carrega as configs do arquivo
	LeArquivo();
	// verifica se falta configs pros tipos de rede existentes
	// VerificaTiposRede();
	//
}

// ---------------------------------------------------------------------------
void __fastcall TDiagTrafo::IniciaLog(TStringList *log)
{ // variaveis locais
	AnsiString Separador = ";";
	AnsiString linha[colTrf_COUNT];
	AnsiString linhaPreenchida = "";
	if (log)
	{
		// limpa o log
		log->Clear();

		linha[colTrf_SEGMENTO] = "Segmento" + Separador;
		linha[colTrf_REDE] = "Rede" + Separador;
		linha[colTrf_EQPTO] = "Equipamento" + Separador;
		linha[colTrf_CODIGO] = "Código" + Separador;
		linha[colTrf_TIPOERRO] = "Tipo Erro" + Separador;
		linha[colTrf_CODBARRA1] = "Barra 1" + Separador;
		linha[colTrf_CODBARRA2] = "Barra 2" + Separador;
		linha[colTrf_PADRAO] = "Trafo Padrão" + Separador;
		linha[colTrf_DESCRICAO] = "Descrição" + Separador;
		linha[colTrf_VAL_PADRAO] = "Valor Padrão" + Separador;
		linha[colTrf_VAL_TRAFO] = "Valor Trafo";

		// concatena os campos em uma linha
		for (int i = 0; i < colTrf_COUNT; i++)
		{
			linhaPreenchida += linha[i];
		}
		log->Add(linhaPreenchida);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TDiagTrafo::IniciaCheckList(TCheckListBox *checkListBox)
{
	checkListBox->Clear();
	checkListBox->Items->Add(op_TRF_TEXT[opTrfCATALOGO]);
	checkListBox->Checked[opTrfCATALOGO] = this->OpcaoDiagExisteNoCatalogo;

	checkListBox->Items->Add(op_TRF_TEXT[opTrfVNOM]);
	checkListBox->Checked[opTrfVNOM] = this->OpcaoDiagVnom;

	checkListBox->Items->Add(op_TRF_TEXT[opTrfVTAP]);
	checkListBox->Checked[opTrfVTAP] = this->OpcaoDiagVtap;

	checkListBox->Items->Add(op_TRF_TEXT[opTrfSNOM]);
	checkListBox->Checked[opTrfSNOM] = this->OpcaoDiagSnom;

	checkListBox->Items->Add(op_TRF_TEXT[opTrfBANCOMONO]);
	checkListBox->Checked[opTrfBANCOMONO] = this->OpcaoDiagBancoMono;

	// checkListBox->Items->Add(op_TRF_TEXT[opTrfFASES]);
	// checkListBox->Checked[opTrfFASES] = this->OpcaoDiagFases;

	checkListBox->Items->Add(op_TRF_TEXT[opTrfPERDAFE]);
	checkListBox->Checked[opTrfPERDAFE] = this->OpcaoDiagPerdaFe;

	checkListBox->Items->Add(op_TRF_TEXT[opTrfIMPEDANCIA]);
	checkListBox->Checked[opTrfIMPEDANCIA] = this->OpcaoDiagImpedancia;

	checkListBox->Items->Add(op_TRF_TEXT[opTrfLIGINCOERENTE]);
	checkListBox->Checked[opTrfLIGINCOERENTE] = this->OpcaoDiagLigIncoerente;

	// checkListBox->Items->Add(op_TRF_TEXT[opTrfISO]);
	// checkListBox->Checked[opTrfISO] = this->OpcaoDiagIsolado;
}

// ---------------------------------------------------------------------------
bool __fastcall TDiagTrafo::LeArquivo(void)
{ // variaveis locais
	VTInfoset *infoset;
	DIAG_TRAFO *cfgDiagTrafo;
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
	bool temPadrao = false;

	try
	{
		// cria novo infoset para ler o arquivo de cfg
		infoset = geral->Infoset;
		infoset->Clear();
		// verifica se é possivel ler o arquivo
		if (infoset->LoadFromFile(FILE_CFG_TRAFO))
		{ // varre todo o arquivo lido
			for (int i = 0; i < infoset->Count; i++)
			{
				infoset->SelectLine(i);
				cfgDiagTrafo = new DIAG_TRAFO;
				infoset->GetInfo("id", cfgDiagTrafo->id);
				infoset->GetInfo("codigo", cfgDiagTrafo->codigo);
				infoset->GetInfo("snom_mva", cfgDiagTrafo->snom_mva);
				infoset->GetInfo("vnomPri_kv", cfgDiagTrafo->vnomPri_kv);
				infoset->GetInfo("vnomSec_kv", cfgDiagTrafo->vnomSec_kv);
				// infoset->GetInfo("ligacaoPri", cfgDiagTrafo->ligacaoPri);
				// infoset->GetInfo("ligacaoSec", cfgDiagTrafo->ligacaoSec);
				infoset->GetInfo("perdaFe", cfgDiagTrafo->perdaFe);
				listCfg->Add(cfgDiagTrafo);
				// caso o tipo rede seja 0 (== PADRAO)
				if (cfgDiagTrafo->id == 0)
				{
					temPadrao = true;
				}
			}
		} // caso não exista o arquivo
		else
		{ // cria o tipo "default"
			cfgDiagTrafo = CriaCfgTrafoDefault(0, "*Padrão*");
			// adiciona à lista de configs
			listCfg->Add(cfgDiagTrafo);
			temPadrao = true;
			// e cria o arquivo
			SalvaArquivo();
		}
		if (!temPadrao)
		{ // cria o tipo "default"
			cfgDiagTrafo = CriaCfgTrafoDefault(0, "*Padrão*");
			// define como padrao
			// diagTrafoPADRAO = diagTrafo;
			// adiciona à lista de configs
			listCfg->Add(cfgDiagTrafo);
			temPadrao = true;
		}
		return true;
	}
	catch (Exception &e)
	{
		return false;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TDiagTrafo::LeCheckList(TCheckListBox *checkListBox)
{
	this->OpcaoDiagExisteNoCatalogo = checkListBox->Checked[opTrfCATALOGO];
	this->OpcaoDiagVnom = checkListBox->Checked[opTrfVNOM];
	this->OpcaoDiagVtap = checkListBox->Checked[opTrfVTAP];
	this->OpcaoDiagSnom = checkListBox->Checked[opTrfSNOM];
	this->OpcaoDiagBancoMono = checkListBox->Checked[opTrfBANCOMONO];
	// this->OpcaoDiagFases             = checkListBox->Checked[opTrfFASES];
	this->OpcaoDiagPerdaFe = checkListBox->Checked[opTrfPERDAFE];
	this->OpcaoDiagImpedancia = checkListBox->Checked[opTrfIMPEDANCIA];
	this->OpcaoDiagLigIncoerente = checkListBox->Checked[opTrfLIGINCOERENTE];
	// this->OpcaoDiagIsolado           = checkListBox->Checked[opTrfISO];
}

// ---------------------------------------------------------------------------
long __fastcall TDiagTrafo::PM_GetItensVerificados(void)
{
	if (executado)
	{
		return infoTrafos.n_verif;
	}
	return -1;
}

// ---------------------------------------------------------------------------
TStringList* __fastcall TDiagTrafo::PM_GetLog(void)
{
	if (executado)
	{
		return log_trafo;
	}
	else
	{
		return NULL;
	}
}

// ---------------------------------------------------------------------------
long __fastcall TDiagTrafo::PM_GetOcorrencias(void)
{
	if (executado)
	{
		return infoTrafos.n_ocorrencias;
	}
	else
	{
		return -1;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TDiagTrafo::PreencheListView(TListView *listView)
{ // variaveis locais
	TListItem *listItem;
	TListColumn *listColumn;
	TStringList *campos = new TStringList();

	// tentar melhorar performance do listview
	listView->Items->BeginUpdate();
	// limpa o listView
	listView->Clear();
	listView->Columns->Clear();

	campos->Clear();
	// Extrai campos da linha
	ExtraiStrings(log_trafo->Strings[0], ";", campos);

	// adiciona as colunas
	listColumn = listView->Columns->Add();
	listColumn->Width = 70;
	// listColumn = listView->Column[colTrf_SEGMENTO];
	listColumn->Caption = campos->Strings[colTrf_SEGMENTO];
	// listColumn->Caption = "Segmento";
	// listColumn->Width = ColumnHeaderWidth;
	listColumn = listView->Columns->Add();
	listColumn->Width = 60;
	// listColumn = listView->Column[colTrf_REDE];
	listColumn->Caption = campos->Strings[colTrf_REDE];
	// listColumn->Caption = "Rede";
	// listColumn->Width = ColumnHeaderWidth;
	listColumn = listView->Columns->Add();
	listColumn->Width = 80;
	// listColumn = listView->Column[colTrf_EQPTO];
	listColumn->Caption = campos->Strings[colTrf_EQPTO];
	// listColumn->Caption = "Equipamento";
	// listColumn->Width = ColumnHeaderWidth;
	listColumn = listView->Columns->Add();
	listColumn->Width = 50;
	// listColumn = listView->Column[colTrf_CODIGO];
	listColumn->Caption = campos->Strings[colTrf_CODIGO];
	// listColumn->Caption = "Codigo";
	// listColumn->Width = ColumnHeaderWidth;
	listColumn = listView->Columns->Add();
	listColumn->Width = 90;
	// listColumn = listView->Column[colTrf_TIPOERRO];
	listColumn->Caption = campos->Strings[colTrf_TIPOERRO];
	// listColumn->Caption = "Tipo Erro";
	// listColumn->Width = ColumnHeaderWidth;
	listColumn = listView->Columns->Add();
	listColumn->Width = 50;
	// listColumn = listView->Column[colTrf_CODBARRA1];
	listColumn->Caption = campos->Strings[colTrf_CODBARRA1];
	// listColumn->Caption = "Barra 1";
	// listColumn->Width = ColumnHeaderWidth;
	listColumn = listView->Columns->Add();
	listColumn->Width = 50;
	// listColumn = listView->Column[colTrf_CODBARRA2];
	listColumn->Caption = campos->Strings[colTrf_CODBARRA2];
	// listColumn->Caption = "Barra 2";
	// listColumn->Width = ColumnHeaderWidth;
	listColumn = listView->Columns->Add();
	listColumn->Width = 122;
	// listColumn = listView->Column[colTrf_PADRAO];
	// listColumn->Caption = "Trafo Padrão";
	listColumn->Caption = campos->Strings[colTrf_PADRAO];
	// listColumn->Width = ColumnHeaderWidth;
	listColumn = listView->Columns->Add();
	listColumn->Width = 300;
	// listColumn = listView->Column[colTrf_DESCRICAO];
	listColumn->Caption = campos->Strings[colTrf_DESCRICAO];
	// listColumn->Caption = "Descrição";
	// listColumn->Width = ColumnHeaderWidth;
	listColumn = listView->Columns->Add();
	listColumn->Width = 80;
	// listColumn = listView->Column[colTrf_VAL_PADRAO];
	listColumn->Caption = campos->Strings[colTrf_VAL_PADRAO];
	// listColumn->Caption = "Valor Padrão";
	// listColumn->Width = ColumnHeaderWidth;
	listColumn = listView->Columns->Add();
	listColumn->Width = 80;
	// listColumn = listView->Column[colTrf_VAL_TRAFO];
	listColumn->Caption = campos->Strings[colTrf_VAL_TRAFO];
	// listColumn->Caption = "Valor Trafo";
	// listColumn->Width = ColumnHeaderWidth;

	// //coloca valor fixo de width
	// for(int nc =0; nc <listView->Columns->Count; nc++)
	// {
	// listView->Columns->Items[index]->Width = 50;
	// }

	for (int nl = 1; nl < log_trafo->Count; nl++)
	{
		campos->Clear();
		// Extrai campos da linha
		ExtraiStrings(log_trafo->Strings[nl], ";", campos);
		listItem = listView->Items->Add();
		listItem->Data = log_trafo->Objects[nl];
		listItem->Caption = campos->Strings[0];
		for (int nc = 1; nc < campos->Count; nc++)
		{
			listItem->SubItems->Add(campos->Strings[nc]);
		}
	}
	// destroi o TStrings
	if (campos)
	{
		delete campos;
		campos = NULL;
	}
	// tentar melhorar performance do listview
	listView->Items->EndUpdate();
}

// ---------------------------------------------------------------------------
bool __fastcall TDiagTrafo::SalvaArquivo(void)
{ /* TODO : Decidir se o usuario perde valores guardados pra redes não presentes na base */
	// variaveis locais
	VTInfoset *infoset;
	DIAG_TRAFO *cfgDiagTrafo;
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

	// infoset = NewObjInfoset(apl);
	// infoset = (VTInfoset*)apl->GetObject(__classid(VTInfoset));
	try
	{
		infoset = geral->Infoset;
		infoset->Clear();
		for (int i = 0; i < listCfg->Count; i++)
		{
			cfgDiagTrafo = (DIAG_TRAFO*) listCfg->Items[i];
			infoset->InsertNewLine();
			infoset->AddInfo("id", i);
			/* TODO : Pensar melhor nessa id, id muda de acordo com a posicao na lista... */
			infoset->AddInfo("codigo", cfgDiagTrafo->codigo);
			infoset->AddInfo("snom_mva", cfgDiagTrafo->snom_mva);
			infoset->AddInfo("vnomPri_kv", cfgDiagTrafo->vnomPri_kv);
			infoset->AddInfo("vnomSec_kv", cfgDiagTrafo->vnomSec_kv);
			// infoset->AddInfo("ligacaoPri", cfgDiagTrafo->ligacaoPri);
			// infoset->AddInfo("ligacaoSec", cfgDiagTrafo->ligacaoSec);
			infoset->AddInfo("perdaFe", cfgDiagTrafo->perdaFe);
		}
		infoset->SaveToFile(FILE_CFG_TRAFO);
		return true;
	}
	catch (Exception &e)
	{
		return false;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TDiagTrafo::SeparaBancosMono(TList *lisTrafos, TList *lisBancoDeltaAberto,
	TList *lisBancoDeltaFechado)
{ // variaveis locais
	VTRedes *redes = (VTRedes*) apl->GetObject(__classid(VTRedes));
	VTBarra *barraPri, *barraSec;
	VTLigacao *ligacao;
	VTTrafo *trafo, *outroTrafo;
	TList *listTemp = new TList();
	TList *listTrafosParalelo = new TList();
	bool temTrafoParalelo = false;

	// proteção (caso alguma das listas não existam
	if ((!lisTrafos) || (!lisBancoDeltaAberto) || (!lisBancoDeltaFechado))
		return;

	lisBancoDeltaAberto->Clear();
	lisBancoDeltaFechado->Clear();

	for (int nt = (lisTrafos->Count - 1); nt >= 0; nt--)
	{
		// assume que não tem trafo paralelo
		temTrafoParalelo = false;
		// limpa as listas auxiliares
		listTemp->Clear();
		listTrafosParalelo->Clear();
		// verifica o trafo da lista de trafos
		trafo = (VTTrafo*) lisTrafos->Items[nt];
		// trafo = (VTTrafo*) lisTrafos->First();
		// se for um trafo mono faz a verificação
		if (trafo->Monofasico)
		{
			// olha a barra do primário
			barraPri = trafo->BarraPrimario();
			// olha a barra do secundário
			barraSec = trafo->BarraSecundario();
			// pega todas ligações conectadas à barra do primário
			redes->LisLigacao(listTemp, barraPri);
			// percorre a lista de ligacoes vizinhas
			for (int nv = 0; nv < listTemp->Count; nv++)
			{
				ligacao = (VTLigacao*)listTemp->Items[nv];
				// verfica se é trafo
				if (ligacao->Tipo() == eqptoTRAFO)
				{
					outroTrafo = (VTTrafo*)ligacao;
					// verifica se não é o mesmo trafo que o analizado
					if ((trafo != outroTrafo) && (outroTrafo->Monofasico))
					{ // verifica se ambas as barras são iguais
						if ((outroTrafo->BarraPrimario() == barraPri) &&
							(outroTrafo->BarraSecundario() == barraSec))
						{ // sendo iguais adiciona à lista de trafos paralelo
							listTrafosParalelo->Add(outroTrafo);
							// possui trafo paralelo
							temTrafoParalelo = true;
						}
					}
				}
			}
			// caso tenha encontrado trafo em paralelo, tb adiciona ele a lista
			if (temTrafoParalelo)
			{ // adiciona o trafo original à lista de trafos em paralelo
				listTrafosParalelo->Add(trafo);
			}
			for (int np = 0; np < listTrafosParalelo->Count; np++)
			{ // se forem 2 trafos em paralelo, é provavel q seja um delta aberto
				if (listTrafosParalelo->Count == 2)
				{
					lisBancoDeltaAberto->Add(listTrafosParalelo->Items[np]);
					// remove o trafo em paralelo da lista original de trafos
					lisTrafos->Remove(listTrafosParalelo->Items[np]);
					// decrementa o contador do 'for' externo se retirou os trafos diferentes do analisado
					if (trafo != listTrafosParalelo->Items[np])
						nt--;
				}
				// se forem 3 trafos em paralelo, é provavel q seja um delta fechado
				else if (listTrafosParalelo->Count == 3)
				{
					lisBancoDeltaFechado->Add(listTrafosParalelo->Items[np]);
					// remove o trafo em paralelo da lista original de trafos
					lisTrafos->Remove(listTrafosParalelo->Items[np]);
					// decrementa o contador do 'for' externo se retirou os trafos diferentes do analisado
					if (trafo != listTrafosParalelo->Items[np])
						nt--;
				}
				// se for algo diferente disso...
				else
				{
					// pensar melhor
				}
			}
		}
	}
	// deleta a lista temporária
	if (listTemp)
	{
		delete listTemp;
		listTemp = NULL;
	}
	// deleta a lista com os trafos em paralelo
	if (listTrafosParalelo)
	{
		delete listTrafosParalelo;
		listTrafosParalelo = NULL;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TDiagTrafo::ShowFormCfg(void)
{
	if (formCatalogoTrf)
		formCatalogoTrf->Show();
	else
	{
		formCatalogoTrf = new TFormCatalogoTrf(NULL, apl);
		formCatalogoTrf->Inicia(this);
		formCatalogoTrf->Show();
	}
}

// ---------------------------------------------------------------------------
int __fastcall TDiagTrafo::ShowFormCfgModal(void)
{
	// if(formDiagEqptoTrafo)
	// {
	// return formDiagEqptoTrafo->ShowModal();
	// }
	// else
	// {
	// formDiagEqptoTrafo = new TFormDiagEqptoTrafo(NULL, apl);
	// formDiagEqptoTrafo->Inicia(this);
	// return formDiagEqptoTrafo->ShowModal();
	// }
	return -1; /* TODO : pensar melhor */
}

// ---------------------------------------------------------------------------
int __fastcall TDiagTrafo::Tipo(void)
{
	return (dgeTRAFO);
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TDiagTrafo::TipoAsString(void)
{
	return "Transformadores";
}

// ---------------------------------------------------------------------------
void __fastcall TDiagTrafo::VerificaOpBancoMono(VTTrafo *trafo1, VTTrafo *trafo2, VTTrafo *trafo3)
{
	// proteção (caso não exista um dos trafos)
	if ((!trafo1) || (!trafo2))
		return;

	// caso seja parte de um delta aberto
	if (trafo3 == NULL)
	{
		DeltaAbertoValido(trafo1, trafo2);
	}
	// caso seja parte de um delta fechado
	if (trafo3 != NULL)
	{
		DeltaFechadoValido(trafo1, trafo2, trafo3);
	}
}

// ---------------------------------------------------------------------------
// Provavel problema de desempenho!
DIAG_TRAFO* __fastcall TDiagTrafo::VerificaOpExisteNoCatalogo(VTTrafo *trafo)
{ // variaveis locais
	DIAG_TRAFO *cfgDiagTrafo;
	bool encontrou = false;

	for (int ni = 0; ni < listCfg->Count; ni++)
	{
		cfgDiagTrafo = (DIAG_TRAFO*)listCfg->Items[ni];
		// compara os valores de Snom
		if (IsDoubleZero((cfgDiagTrafo->snom_mva) - (trafo->snom)))
		{ // correspondencia exata
			encontrou = true;
			return cfgDiagTrafo;
		}
	}
	// se não encontrou
	if (!encontrou)
	{
		if (dgTrafo.existeNoCatalogo)
			AddLinhaLog(trafo, opTrfCATALOGO);
	}
	return NULL;
}

// ---------------------------------------------------------------------------
// void  __fastcall  TDiagTrafo::VerificaOpFases(VTTrafo *trafo)
// {  //variaveis locais
// VTZona *zona;
// VTBarra *barra;
//
// //Verifica o primário
// barra = trafo->BarraPrimario();
// zona = barra->zona;
// if(zona)
// {
// if((zona->Fases  & trafo->pri.fase) != trafo->pri.fase)
// {
// AddLinhaLog(trafo, opTrfFASES, NULL, "Fases do primário incompatíveis com a rede", 1);
// }
// //Verifica o secundário (é necessário mesmo? )
// barra = trafo->BarraSecundario();
// zona = barra->zona;
// if((zona->Fases  & trafo->sec.fase) != trafo->sec.fase)
// {
// AddLinhaLog(trafo, opTrfFASES, NULL, "Fases do secundário incompatíveis com a rede", 2);
// }
// }
// else{
// //int a = 0;
// }
// }

// ---------------------------------------------------------------------------
void __fastcall TDiagTrafo::VerificaOpLigIncoerente(VTTrafo *trafo)
{ // variaveis locais
	VTFases *fases = (VTFases*)apl->GetObject(__classid(VTFases));
	int num_Fases = 0;
	bool problema = false;
	VTBarra *barraPrim;
	VTRedes *redes = (VTRedes*) apl->GetObject(__classid(VTRedes));
	VTLigacao *ligacao;
	TList *lisLig;
	VTTrecho *trecho;
	int fasesDisponiveis = 0;
	int fasesNecessarias = 0;

	// se o trafo for monofásico, verificar se falta neutro/terra no primario
	if (trafo->Monofasico)
	{
		if (fases->NumeroDeFases(trafo->pri.fase) == 0)
		{
			AddLinhaLog(trafo, opTrfLIGINCOERENTE, NULL,
			"Fases ausentes na ligação do primário", 1);
		}
		if (fases->NumeroDeFases(trafo->sec.fase) == 0)
		{
			AddLinhaLog(trafo, opTrfLIGINCOERENTE, NULL,
				"Fases ausentes na ligação do secundário", 2);
		}
		// verifica se é bi ou mono
		if (fases->NumeroDeFases(trafo->pri.fase) == 1)
		{ // se não tiver neutro nem terra... tem problema
			if (((trafo->pri.fase & faseN) != faseN) && ((trafo->pri.fase & faseT) != faseT))
			{
				problema = true;
			}
			if (problema)
			{
				AddLinhaLog(trafo, opTrfLIGINCOERENTE, NULL,
					"Ausência de neutro ou terra no primário", 1);
			}
		}
		// verifica se a rede possui o neutro
		barraPrim = trafo->BarraPrimario();
		lisLig = new TList;
		redes->LisLigacao(lisLig, barraPrim);
		for (int nl = 0; nl < lisLig->Count; nl++)
		{
			ligacao = (VTLigacao*)lisLig->Items[nl];
			if (ligacao->Tipo() == eqptoTRECHO)
			{
				trecho = (VTTrecho*) ligacao;
				// cria as fases disponiveis
				fasesDisponiveis = fasesDisponiveis | trecho->arranjo->Fases;
			}
		}
		// depois de varrer todos trechos, verifica se possui neutro
		if ((fasesDisponiveis & faseN) != faseN)
		{
			fasesNecessarias = trafo->Fases(barraPrim);
			// verifica se o neutro é aterrado
			if ((fasesNecessarias & faseT) != faseT)
			{
				// trafo->pri.fase = trafo->pri.fase + faseT;
				AddLinhaLog(trafo, opTrfLIGINCOERENTE, NULL,
					"Ausência de aterramento no primário.", 1);
			}
		}
		delete lisLig;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TDiagTrafo::VerificaOpImpedancia(VTTrafo *trafo, DIAG_TRAFO *cfgDiagTrafo)
{
	if (IsDoubleZero(trafo->z0.r) && IsDoubleZero(trafo->z0.x))
	{
		AddLinhaLog(trafo, opTrfIMPEDANCIA, cfgDiagTrafo, "Valor de Z0 nulo.");
	}
	if (IsDoubleZero(trafo->z1.r) && IsDoubleZero(trafo->z1.x))
	{
		AddLinhaLog(trafo, opTrfIMPEDANCIA, cfgDiagTrafo, "Valor de Z1 nulo.");
	}
}
// ---------------------------------------------------------------------------
// void  __fastcall  TDiagTrafo::VerificaOpIsolado(VTTrafo *trafo)
// {  //variaveis locais
// VTBarra *barra1, *barra2;
//
// barra1 = trafo->BarraPrimario();
// barra2 = trafo->BarraSecundario();
// if((barra1->zona == NULL) && (barra2->zona == NULL))
// {
// AddLinhaLog(trafo, opTrfISO);
// }
//
// }

// ---------------------------------------------------------------------------
void __fastcall TDiagTrafo::VerificaOpPerdaFe(VTTrafo *trafo, DIAG_TRAFO *cfgDiagTrafo)
{ // variaveis locais
	double a, b;

	// caso não tenha sido encontrado uma correspondência, verifica se != 0
	if (!cfgDiagTrafo)
	{
		if (IsDoubleZero(trafo->perda_ferro))
		{
			AddLinhaLog(trafo, opTrfPERDAFE, NULL, "Perda no ferro nula.");
		}
	}
	else
	{
		// verifica os valores do vnom do primário
		a = trafo->perda_ferro;
		b = cfgDiagTrafo->perdaFe;
		if (!IsDoubleZero(a - b, 1e-3))
		{
			AddLinhaLog(trafo, opTrfPERDAFE, cfgDiagTrafo,
				"Perda no ferro com valor incompatível ao valor do catálogo.");
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TDiagTrafo::VerificaOpSnom(VTTrafo *trafo, DIAG_TRAFO *cfgDiagTrafo)
{ // variaveis locais
	double a, b;

	// caso não tenha sido encontrado uma correspondência, verifica se != 0
	if (!cfgDiagTrafo)
	{
		if (IsDoubleZero(trafo->snom))
		{
			AddLinhaLog(trafo, opTrfSNOM, NULL, "Potência nominal nula");
		}
		if (trafo->snom > 100)
		{
			AddLinhaLog(trafo, opTrfSNOM, NULL, "Potência nominal acima de 100 MVA");
		}
	}
	else
	{
		// verifica os valores do vnom do primário
		a = trafo->snom;
		b = cfgDiagTrafo->snom_mva;
		if (!IsDoubleZero(a - b, 1e-3))
		{
			AddLinhaLog(trafo, opTrfSNOM, cfgDiagTrafo,
			"Valor incompatível com o valor do catálogo");
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TDiagTrafo::VerificaOpVnom(VTTrafo *trafo, DIAG_TRAFO *cfgDiagTrafo)
{ // variaveis locais
	double a, b;

	// verifica os valores do vnom do primário
	a = trafo->pri.vnom;
	b = cfgDiagTrafo->vnomPri_kv;
	if (!IsDoubleZero(a - b, 1e-3))
	{
		AddLinhaLog(trafo, opTrfVNOM, cfgDiagTrafo,
			"Tensão nominal do primário não coerente com o catálogo", 1);
	}
	// verifica os valores do vnom do secundário
	a = trafo->sec.vnom;
	b = cfgDiagTrafo->vnomSec_kv;
	if (!IsDoubleZero(a - b, 1e-3))
	{
		AddLinhaLog(trafo, opTrfVNOM, cfgDiagTrafo,
			"Tensão nominal do secundário não coerente com o catálogo", 2);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TDiagTrafo::VerificaOpVtap(VTTrafo *trafo)
{ // variaveis
	double minTap, maxTap;

	// verifica se o valor do tap está dentro de um limite, de acordo com sua potência
	if (trafo->snom < 0.3) // MVA  (valor maximo q vi no PRODIST anexo 1 perdas em trafos de dist)
	{
		// verifica o primário
		minTap = 0.9 * trafo->pri.vnom;
		maxTap = 1.1 * trafo->pri.vnom;
		if ((trafo->pri.tap > maxTap) || (trafo->pri.tap < minTap))
		{
			AddLinhaLog(trafo, opTrfVTAP, NULL, "Tap primário fora do intervalo de +/-10%", 1);
		}
		// verifica o secundário
		minTap = 0.9 * trafo->sec.vnom;
		maxTap = 1.1 * trafo->sec.vnom;
		if ((trafo->sec.tap > maxTap) || (trafo->sec.tap < minTap))
		{
			AddLinhaLog(trafo, opTrfVTAP, NULL, "Tap secundário fora do intervalo de +/-10%", 2);
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TDiagTrafo::VerificaTrafo(VTTrafo *trafo)
{ // variaveis locais
	DIAG_TRAFO *cfgDiagTrafo;

	// verifica se existe no catalogo
	cfgDiagTrafo = VerificaOpExisteNoCatalogo(trafo);
	// caso encontre no catalogo, pode-se fazer outras verificações
	if (cfgDiagTrafo)
	{ // compara vnoms com o catalogo
		if (dgTrafo.vnom)
			VerificaOpVnom(trafo, cfgDiagTrafo);
	}
	// compara perdaFe com o catalogo
	if (dgTrafo.perdafe)
		VerificaOpPerdaFe(trafo, cfgDiagTrafo);
	// compara Snom com o catalogo
	if (dgTrafo.perdafe)
		VerificaOpSnom(trafo, cfgDiagTrafo);
	// compara vtap
	if (dgTrafo.vtap)
		VerificaOpVtap(trafo);
	// compara fases
	// if(dgTrafo.fases)VerificaOpFases(trafo);
	// Verifica Impedâncias de sequência
	if (dgTrafo.z)
		VerificaOpImpedancia(trafo, cfgDiagTrafo);
	// Verifica a ligacao
	if (dgTrafo.ligIncoerente)
		VerificaOpLigIncoerente(trafo);
	// Verifica se está isolado
	// if(dgTrafo.isolado)VerificaOpIsolado(trafo);
	// incrementa o numero de trafos verificados
	infoTrafos.n_verif++;
}

// ---------------------------------------------------------------------------
