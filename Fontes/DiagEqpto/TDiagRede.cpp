// ---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
#include <limits>
#include <math.h>
#include "..\Apl\VTApl.h"
#include "..\..\Arranjo\VTArranjo.h"
#include "..\..\Arranjo\VTArranjos.h"
#include "..\Geral\VTInfoset.h"
#include "..\Geral\VTGeral.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Potencia\VTDemanda.h"
#include "..\Radial\VTPrimario.h"
#include "..\Radial\VTRadial.h"
#include "..\Radial\VTSecundario.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTEqbar.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTFases.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTRegulador.h"
#include "..\Rede\VTTrafo.h"
#include "..\Rede\VTTrafo3E.h"
#include "..\Rede\VTTrecho.h"
#include "..\Rede\VTTipos.h"
#include "..\Rede\VTTipoRede.h"
#include "..\Zona\VTZona.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Potencia.h"
#include "..\..\DLL_Inc\Radial.h"
#include "TDiagRede.h"
// #include "TFormDiagEqptoRede.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
// ---------------------------------------------------------------------------
// define do nome do arquivo
// #define FILE_CFG_REDE "CfgDiagEqptoREDE"

// ---------------------------------------------------------------------------
__fastcall TDiagRede::TDiagRede(VTApl *apl)
{
	this->apl = apl;
	// Mostra que ainda não foi executado
	executado = false;
	// define se tem informações adicionais
	infoExtra = false;
	// cria as listas
	listCfg = new TList;
	// cria objetos
	lisBAR = new TList();
	lisEQP = new TList();
	lisLIG = new TList();
	lisTRF = new TList();
	// inicializa as opcoes do diagnostico
	dgRede.fases = true;
	dgRede.demTrafo = true;
	dgRede.regRever = true;
	dgRede.vnomTrafo = true;
	// cria o log_barra
	log_rede = new TStringList;
}

// ---------------------------------------------------------------------------
__fastcall TDiagRede::~TDiagRede(void)
{
	// destroi lista e conteudo
	if (listCfg)
	{
		LimpaTList(listCfg);
		delete listCfg;
		listCfg = NULL;
	}
	// destrói listas sem destruir seus objetos
	if (lisBAR)
	{
		delete lisBAR;
		lisBAR = NULL;
	}
	if (lisEQP)
	{
		delete lisEQP;
		lisEQP = NULL;
	}
	if (lisLIG)
	{
		delete lisLIG;
		lisLIG = NULL;
	}
	if (lisTRF)
	{
		delete lisTRF;
		lisTRF = NULL;
	}
	// destroi log
	if (log_rede)
	{
		delete log_rede;
		log_rede = NULL;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TDiagRede::AddLinhaLog(VTEqpto *eqpto, int opDIAG_REDE, AnsiString descr)
{ // incrementa o contador de erros
	// infoRedes.n_ocorrencias++;
	temErro = true;
	// vetor de strings seguindo o enum col_LOG_TRECHO
	AnsiString linhaPreenchida = "";
	AnsiString linha[colRede_COUNT];
	AnsiString Separador = ";";
	VTLigacao *ligacao;
	VTEqbar *eqbar;
	VTRede *rede;
	VTBarra *barra;

	// debug
	int tipo = eqpto->Tipo();

	if (eqpto)
	{
		switch (eqpto->Tipo())
		{
		case eqptoEQBAR:
			eqbar = (VTEqbar*)eqpto;
			linha[colRede_SEGMENTO] = "-" + Separador;
			linha[colRede_REDE] = "-" + Separador;
			linha[colRede_CODIGO] = eqpto->Codigo + Separador;
			linha[colRede_EQPTO] = eqpto->TipoAsString() + Separador;
			linha[colRede_UTMX] = IntToStr(eqbar->pbarra->utm.x / 100) + Separador;
			linha[colRede_UTMY] = IntToStr(eqbar->pbarra->utm.y / 100) + Separador;
			break;

		case eqptoCHAVE:
			ligacao = (VTLigacao*)eqpto;
			if (ligacao->rede)
			{
				if (ligacao->rede->TipoRede)
				{
					linha[colRede_SEGMENTO] = ligacao->rede->TipoRede->Codigo + Separador;
				}
				else
				{
					linha[colRede_SEGMENTO] = "-" + Separador;
				}
				linha[colRede_REDE] = ligacao->rede->Codigo + Separador;
			}
			else
			{
				linha[colRede_REDE] = "-" + Separador;
			}
			linha[colRede_CODIGO] = ligacao->Codigo + Separador;
			linha[colRede_EQPTO] = eqpto->TipoAsString() + Separador;
			// linha[colRede_UTMX] = IntToStr(ligacao->pbarra1->utm.x / 100) + Separador;
			// linha[colRede_UTMY] = IntToStr(ligacao->pbarra1->utm.y / 100) + Separador;
			linha[colRede_UTMX] = MediaUTM_X(ligacao) + Separador;
			linha[colRede_UTMY] = MediaUTM_Y(ligacao) + Separador;
			break;

		case eqptoTRAFO:
			ligacao = (VTLigacao*)eqpto;
			if (ligacao->rede)
			{
				if (ligacao->rede->TipoRede)
				{
					linha[colRede_SEGMENTO] = ligacao->rede->TipoRede->Codigo + Separador;
				}
				else
				{
					linha[colRede_SEGMENTO] = "-" + Separador;
				}
				linha[colRede_REDE] = ligacao->rede->Codigo + Separador;
			}
			else
			{
				linha[colRede_REDE] = "-" + Separador;
			}
			linha[colRede_CODIGO] = ligacao->Codigo + Separador;
			linha[colRede_EQPTO] = eqpto->TipoAsString() + Separador;
			// linha[colRede_UTMX] = IntToStr(ligacao->pbarra1->utm.x / 100) + Separador;
			// linha[colRede_UTMY] = IntToStr(ligacao->pbarra1->utm.y / 100) + Separador;
			linha[colRede_UTMX] = MediaUTM_X(ligacao) + Separador;
			linha[colRede_UTMY] = MediaUTM_Y(ligacao) + Separador;
			break;

		case eqptoTRECHO:
			ligacao = (VTLigacao*)eqpto;
			if (ligacao->rede)
			{
				if (ligacao->rede->TipoRede)
				{
					linha[colRede_SEGMENTO] = ligacao->rede->TipoRede->Codigo + Separador;
				}
				else
				{
					linha[colRede_SEGMENTO] = "-" + Separador;
				}
				linha[colRede_REDE] = ligacao->rede->Codigo + Separador;
			}
			else
			{
				linha[colRede_REDE] = "-" + Separador;
			}
			linha[colRede_CODIGO] = ligacao->Codigo + Separador;
			linha[colRede_EQPTO] = eqpto->TipoAsString() + Separador;
			// linha[colRede_UTMX] = IntToStr(ligacao->pbarra1->utm.x / 100) + Separador;
			// linha[colRede_UTMY] = IntToStr(ligacao->pbarra1->utm.y / 100) + Separador;
			linha[colRede_UTMX] = MediaUTM_X(ligacao) + Separador;
			linha[colRede_UTMY] = MediaUTM_Y(ligacao) + Separador;
			break;

		case eqptoREGULADOR:
			ligacao = (VTLigacao*)eqpto;
			if (ligacao->rede)
			{
				if (ligacao->rede->TipoRede)
				{
					linha[colRede_SEGMENTO] = ligacao->rede->TipoRede->Codigo + Separador;
				}
				else
				{
					linha[colRede_SEGMENTO] = "-" + Separador;
				}
				linha[colRede_REDE] = ligacao->rede->Codigo + Separador;
			}
			else
			{
				linha[colRede_REDE] = "-" + Separador;
			}
			linha[colRede_CODIGO] = ligacao->Codigo + Separador;
			linha[colRede_EQPTO] = eqpto->TipoAsString() + Separador;
			// linha[colRede_UTMX] = IntToStr(ligacao->pbarra1->utm.x / 100) + Separador;
			// linha[colRede_UTMY] = IntToStr(ligacao->pbarra1->utm.y / 100) + Separador;
			linha[colRede_UTMX] = MediaUTM_X(ligacao) + Separador;
			linha[colRede_UTMY] = MediaUTM_Y(ligacao) + Separador;
			break;

		case eqptoREDE:
			rede = (VTRede*)eqpto;
			barra = rede->BarraInicial();
			if (rede)
			{
				if (rede->TipoRede)
				{
					linha[colRede_SEGMENTO] = rede->TipoRede->Codigo + Separador;
				}
				else
				{
					linha[colRede_SEGMENTO] = "-" + Separador;
				}
				linha[colRede_REDE] = rede->Codigo + Separador;
			}
			else
			{
				linha[colRede_REDE] = "-" + Separador;
			}
			linha[colRede_CODIGO] = rede->Codigo + Separador;
			linha[colRede_EQPTO] = eqpto->TipoAsString() + Separador;
			if (barra != NULL)
			{
				linha[colRede_UTMX] = IntToStr(barra->utm.x / 100) + Separador;
				linha[colRede_UTMY] = IntToStr(barra->utm.y / 100) + Separador;
				eqpto = barra;
			}
			else
			{
				linha[colRede_UTMX] = " " + Separador;
				linha[colRede_UTMY] = " " + Separador;
			}
			break;

		default:
			linha[colRede_SEGMENTO] = " " + Separador;
			linha[colRede_REDE] = " " + Separador;
			linha[colRede_CODIGO] = " " + Separador;
			linha[colRede_EQPTO] = " " + Separador;
			linha[colRede_UTMX] = " " + Separador;
			linha[colRede_UTMY] = " " + Separador;
		}
		switch (opDIAG_REDE)
		{
		case opRedeFASES:
			linha[colRede_TIPOERRO] = str_TIPOERRO_ERRO + Separador;
			linha[colRede_DESCRICAO] = descr;
			break;

		case opRedeDEMTRAFO:
			linha[colRede_TIPOERRO] = str_TIPOERRO_AVISO + Separador;
			linha[colRede_DESCRICAO] = descr;
			break;

		case opRedeREGREVER:
			linha[colRede_TIPOERRO] = str_TIPOERRO_ERRO + Separador;
			linha[colRede_DESCRICAO] = descr;
			break;

		case opRedeVNOMTRAFO:
			linha[colRede_TIPOERRO] = str_TIPOERRO_AVISO + Separador;
			linha[colRede_DESCRICAO] = descr;
			break;

		default: ;
		}
		// concatena os campos em uma linha
		for (int i = 0; i < colRede_COUNT; i++)
		{
			linhaPreenchida += linha[i];
		}
		log_rede->AddObject(linhaPreenchida, eqpto);
	}
}

// ---------------------------------------------------------------------------
// DIAG_TRECHO* __fastcall  TDiagRede::CriaCfgRedeDefault(int redeSEGMENTO, AnsiString nomeTipo)
// {
// DIAG_REDE *diagRede;
//
// diagRede = new DIAG_TRECHO;
// diagTrecho->tipo_rede = redeSEGMENTO;
// diagTrecho->nome_tipo = nomeTipo;
// //poe valores default
// diagTrecho->comp_min_m = 0;
// diagTrecho->comp_max_m = 500;
// diagTrecho->iadm_min_a = 0;
// diagTrecho->iadm_max_a = 200;
// diagTrecho->seq0_r_max_ohmkm =  25;
// diagTrecho->seq0_r_min_ohmkm = 0;
// diagTrecho->seq0_x_max_ohmkm = 1;
// diagTrecho->seq0_x_min_ohmkm = 0;
// diagTrecho->seq0_c_max_nFkm =  1;
// diagTrecho->seq0_c_min_nFkm = 0;
// diagTrecho->seq1_r_max_ohmkm = 25;
// diagTrecho->seq1_r_min_ohmkm = 0;
// diagTrecho->seq1_x_max_ohmkm = 1;
// diagTrecho->seq1_x_min_ohmkm = 0;
// diagTrecho->seq1_c_max_nFkm =  1;
// diagTrecho->seq1_c_min_nFkm = 0;
//
// return diagRede;
// }

// ---------------------------------------------------------------------------
int __fastcall TDiagRede::DeterminaFasesLigacaoPai(VTLigacao *filha)
{
	// variáveis locais
	// int        fases_pai;
	VTBarra *barra_comum;
	VTLigacao *pai;

	while ((pai = filha->ligaPai) != NULL)
	{
		if ((pai->Tipo() != eqptoCHAVE) && (pai->Tipo() != eqptoREGULADOR))
		{ // determina Barra comum
			if ((barra_comum = filha->BarraComum(pai)) == NULL)
				return (faseINV);
			{ // determina fases da Ligacao pai na Barra comum
				return (pai->Fases(barra_comum));
			}
		}
		// redefine Ligacao filha
		filha = pai;
	}
	return (faseINV);
}

// ---------------------------------------------------------------------------
bool __fastcall TDiagRede::Executa(void)
{ // variaveis locais
	// VTRedes     *redes =  (VTRedes*) apl->GetObject(__classid(VTRedes));
	// VTBarra     *barra;
	// TList       *lisBarra;

	// inicialza infos
	infoRedes.n_total = 0;
	infoRedes.n_verif = 0;
	infoRedes.n_ocorrencias = 0;
	if (log_rede)
		IniciaLog(log_rede);
	try
	{
		VerificaRede();
		// altera o boolean mostrando que foi executado
		executado = true;
		// deleta a lista de barras isoladas
		return true;
	}
	catch (Exception &e)
	{
		return false;
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TDiagRede::ExisteTrafo(VTSecundario * secundario)
{ // variaveis locais
	bool existe = false;
	TList *lisTRAFOS;

	// protecoes
	if (secundario == NULL)
		return false;
	if (secundario->Rede == NULL)
		return false;
	// cria lista
	lisTRAFOS = new TList;
	// recupera lista de trafos da rede
	secundario->Rede->LisLigacao(lisTRAFOS, eqptoTRAFO);
	// caso a rede tenha trafos
	if (lisTRAFOS->Count > 0)
	{
		existe = true;
	}
	// destroi lista
	if (lisTRAFOS)
	{
		delete lisTRAFOS;
		lisTRAFOS = NULL;
	}

	// retorno
	return existe;
}

// ---------------------------------------------------------------------------
VTLigacao* __fastcall TDiagRede::ExistePaiValido(VTLigacao *filha)
{ // variáveis locais
	VTLigacao *pai = filha->ligaPai;

	// determina Ligacao pai que seja valida
	while (pai != NULL)
	{ // não considera Ligacao pai que é Chave
		if (pai->Tipo() == eqptoTRECHO)
			return (pai);
		if (pai->Tipo() == eqptoTRAFO)
			return (pai);
		if (pai->Tipo() == eqptoREGULADOR)
			return (pai);
		// deermina próxima Ligacao pai
		pai = pai->ligaPai;
	}

	return (NULL);
}

// ---------------------------------------------------------------------------
// VTTrecho* __fastcall TDiagRede::ExisteTrechoPai(VTLigacao *filha)
// {
// //variáveis locais
// VTLigacao *pai = filha->ligaPai;
//
// /* TODO : OLHAR OBJ DA BARRA A PROCURA DO PAI */
// //determina Ligacao pai que seja um Trecho
// while (pai != NULL)
// {//não considera Ligacao pai que é Chave
// if (pai->Tipo() == eqptoTRECHO) return((VTTrecho*)pai);
// //interrompe a busca caso a Ligação pai seja Trafo ou Regulador
// if (pai->Tipo() == eqptoTRAFO)     return(NULL);
// if (pai->Tipo() == eqptoREGULADOR) return(NULL);
// //deermina próxima Ligacao pai
// pai = pai->ligaPai;
// }
//
// return(NULL);
// }

// ---------------------------------------------------------------------------
int __fastcall TDiagRede::FasesDaLigacao(VTLigacao *ligacao, VTBarra *barra)
{
	// variáveis locais
	int fases_tag;
	VTTrafo *trafo;

	// determina fases da Ligacao
	fases_tag = ligacao->Fases(barra);
	// verifica se a Ligacao é um Trafo
	if (ligacao->Tipo() == eqptoTRAFO)
	{ // obtém lista dos Trafos que conectam a Barra
		LisTrafoConectaBarra(lisLIG, barra, lisTRF);
		// determina fases do conjunto de Trafos
		fases_tag = 0;
		for (int n = 0; n < lisTRF->Count; n++)
		{
			trafo = (VTTrafo*)lisTRF->Items[n];
			fases_tag |= trafo->Fases(barra);
		}
	}
	return (fases_tag);
}

// ---------------------------------------------------------------------------
int __fastcall TDiagRede::FasesDisponiveis(VTLigacao *ligacao, VTBarra *barraSecTerc)
{ // variaveis locais
	int fasesDisp;
	int tipoLig;
	VTBarra *barra;
	//
	VTRegulador *regulador;
	VTTrafo *trafo;
	VTTrecho *trecho;

	if (ligacao == NULL)
		return (faseINV);
	// verifica qual o tipo de ligacao
	tipoLig = ligacao->Tipo();
	// verifica as fases disponiveis de acordo com o tipo de ligacao
	switch (tipoLig)
	{
	case eqptoCHAVE:
		fasesDisp = FasesDisponiveis(ExistePaiValido(ligacao));
		break;
	case eqptoREGULADOR:
		regulador = (VTRegulador*) ligacao;
		// olha a barra controlada
		/* TODO : (presume não existir regulador reversivel) */
		// barra = regulador->cfg_auto.bar_ref;
		// guarda as fases do secundario
		// fasesDisp = ligacao->Fases(barra);
		// default: trifásico
		fasesDisp = faseABCN;
		// verifica tipo de ligação
		if (regulador->ligacao == lgMONOFASICO)
			fasesDisp = regulador->fases;
		break;
	case eqptoTRAFO:
		trafo = (VTTrafo*) ligacao;
		// ordena prim sec
		trafo->OrdenaBar12PriSec();
		// pega a barra do secundario
		barra = trafo->BarraSecundario();
		// guarda as fases do secundario
		fasesDisp = ligacao->Fases(barra);
		break;
	case eqptoTRAFO3E:
		if (barraSecTerc == NULL)
		{
			fasesDisp = faseINV;
			break;
		}
		else
		{ // guarda as fases do secundario
			fasesDisp = ligacao->Fases(barraSecTerc);
		}
		break;
	case eqptoTRECHO:
		trecho = (VTTrecho*) ligacao;
		fasesDisp = trecho->arranjo->Fases;
		break;
	default:
		fasesDisp = faseINV; ;
	}
	return fasesDisp;
}

// ---------------------------------------------------------------------------
int __fastcall TDiagRede::FasesNecessarias(VTLigacao *ligacao)
{ // variaveis locais
	int fasesNec;
	int tipoLig;
	VTBarra *barra;
	VTLigacao *ligAux;
	TList *lisLigFilhas;
	VTRegulador *regulador;
	VTTrafo *trafo;
	VTTrafo3E *trafo3e;
	VTTrecho *trecho;

	if (ligacao == NULL)
		return (faseINV);
	// verifica qual o tipo de ligacao
	tipoLig = ligacao->Tipo();
	// verifica as fases disponiveis de acordo com o tipo de ligacao
	switch (tipoLig)
	{
	case eqptoCHAVE:
		// cria lista de filhas
		lisLigFilhas = new TList;
		// junta todas filhas
		ProcuraFilhas(ligacao, ligacao->rede->LisLigacao(), lisLigFilhas);
		// zera as fases necessarias
		fasesNec = 0;
		// percorre a lista e "soma" as fases necessarias
		for (int nl = 0; nl < lisLigFilhas->Count; nl++)
		{
			ligAux = (VTLigacao*) lisLigFilhas->Items[nl];
			fasesNec |= FasesNecessarias(ligAux);
		}
		// destroi lista de filhas
		delete lisLigFilhas;
		break;
	case eqptoREGULADOR:
		regulador = (VTRegulador*) ligacao;
		// olha a barra controlada
		/* TODO : (presume não existir regulador reversivel) */
		// barra = regulador->cfg_auto.bar_ref;
		// guarda as fases do secundario
		// fasesNec = ligacao->Fases(barra);
		// default: trifásico
		fasesNec = faseABC;
		// verifica tipo de ligação
		if (regulador->ligacao == lgMONOFASICO)
			fasesNec = regulador->fases;
		break;
	case eqptoTRAFO:
		trafo = (VTTrafo*) ligacao;
		// ordena prim sec
		trafo->OrdenaBar12PriSec();
		// pega a barra do secundario
		barra = trafo->BarraPrimario();
		// guarda as fases do secundario
		fasesNec = ligacao->Fases(barra);
		// retira neutro e terra
		fasesNec = fasesNec & faseABC;
		break;
	case eqptoTRAFO3E:
		trafo3e = (VTTrafo3E*) ligacao;
		// ordena prim sec
		trafo3e->OrdenaBar123PriSecTer();
		fasesNec = ligacao->Fases(trafo3e->pbarra1);
		break;
	case eqptoTRECHO:
		trecho = (VTTrecho*) ligacao;
		fasesNec = trecho->arranjo->Fases;
		break;
	default:
		fasesNec = faseINV; ;
	}
	return fasesNec;

}

// ---------------------------------------------------------------------------
void __fastcall TDiagRede::Inicia(void)
{
	// nada a fazer
}

// ---------------------------------------------------------------------------
void __fastcall TDiagRede::IniciaCheckList(TCheckListBox *checkListBox)
{
	checkListBox->Clear();

	checkListBox->Items->Add(op_REDE_TEXT[opRedeFASES]);
	checkListBox->Checked[opRedeFASES] = this->OpcaoDiagFases;

	checkListBox->Items->Add(op_REDE_TEXT[opRedeDEMTRAFO]);
	checkListBox->Checked[opRedeDEMTRAFO] = this->OpcaoDiagDemTrafo;

	checkListBox->Items->Add(op_REDE_TEXT[opRedeVNOMTRAFO]);
	checkListBox->Checked[opRedeVNOMTRAFO] = this->OpcaoDiagVnomTrafo;

	checkListBox->Items->Add(op_REDE_TEXT[opRedeREGREVER]);
	checkListBox->Checked[opRedeREGREVER] = this->OpcaoDiagRegRever;
}

// ---------------------------------------------------------------------------
void __fastcall TDiagRede::IniciaLog(TStringList *log)
{ // variaveis locais
	AnsiString Separador = ";";
	AnsiString linha[colRede_COUNT];
	AnsiString linhaPreenchida = "";
	if (log)
	{
		// limpa o log
		log->Clear();
		// cria os cabeçalhos
		linha[colRede_SEGMENTO] = "Segmento" + Separador;
		linha[colRede_REDE] = "Rede" + Separador;
		linha[colRede_EQPTO] = "Eqpto" + Separador;
		linha[colRede_CODIGO] = "Código" + Separador;
		linha[colRede_TIPOERRO] = "Tipo Erro" + Separador;
		linha[colRede_UTMX] = "UTM X (m)" + Separador;
		linha[colRede_UTMY] = "UTM Y (m)" + Separador;
		linha[colRede_DESCRICAO] = "Descrição";
		// concatena os campos em uma linha
		for (int i = 0; i < colRede_COUNT; i++)
		{
			linhaPreenchida += linha[i];
		} // adiciona os cabeçalhos ao log
		log->Add(linhaPreenchida);
	}
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TDiagRede::MediaUTM_X(VTLigacao *ligacao)
{
	AnsiString valor;
	int media;

	media = (ligacao->pbarra1->utm.x + ligacao->pbarra2->utm.x) / 200;
	valor = IntToStr(media);

	return valor;
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TDiagRede::MediaUTM_Y(VTLigacao *ligacao)
{
	AnsiString valor;
	int media;

	media = (ligacao->pbarra1->utm.y + ligacao->pbarra2->utm.y) / 200;
	valor = IntToStr(media);

	return valor;
}

// ---------------------------------------------------------------------------
long __fastcall TDiagRede::PM_GetItensVerificados(void)
{
	if (executado)
	{
		return infoRedes.n_verif;
	}

	return -1;
}

// ---------------------------------------------------------------------------
TStringList* __fastcall TDiagRede::PM_GetLog(void)
{
	if (executado)
	{
		return log_rede;
	}

	return NULL;
}

// ---------------------------------------------------------------------------
long __fastcall TDiagRede::PM_GetOcorrencias(void)
{
	if (executado)
	{
		return infoRedes.n_ocorrencias;
	}

	return -1;
}

// ---------------------------------------------------------------------------
void __fastcall TDiagRede::PreencheListView(TListView *listView)
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
	ExtraiStrings(log_rede->Strings[0], ";", campos);

	// adiciona as colunas
	listColumn = listView->Columns->Add();
	listColumn->Width = 62;
	listColumn->Caption = campos->Strings[colRede_SEGMENTO];

	listColumn = listView->Columns->Add();
	listColumn->Width = 50;
	listColumn->Caption = campos->Strings[colRede_REDE];

	listColumn = listView->Columns->Add();
	listColumn->Width = 50;
	listColumn->Caption = campos->Strings[colRede_EQPTO];

	listColumn = listView->Columns->Add();
	listColumn->Width = 50;
	listColumn->Caption = campos->Strings[colRede_CODIGO];

	listColumn = listView->Columns->Add();
	listColumn->Width = 60;
	listColumn->Caption = campos->Strings[colRede_TIPOERRO];

	listColumn = listView->Columns->Add();
	listColumn->Width = 70;
	listColumn->Caption = campos->Strings[colRede_UTMX];

	listColumn = listView->Columns->Add();
	listColumn->Width = 70;
	listColumn->Caption = campos->Strings[colRede_UTMY];

	listColumn = listView->Columns->Add();
	listColumn->Width = 150;
	listColumn->Caption = campos->Strings[colRede_DESCRICAO];

	for (int nl = 1; nl < log_rede->Count; nl++)
	{
		campos->Clear();
		// Extrai campos da linha
		ExtraiStrings(log_rede->Strings[nl], ";", campos);
		listItem = listView->Items->Add();
		listItem->Data = log_rede->Objects[nl];
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
void __fastcall TDiagRede::ProcuraFilhas(VTLigacao *pai, TList *lisLIG, TList *lisEXT)
{ // variaveis locais
	VTLigacao *ligacao;

	// percorre todas ligacoes À procura de ligacoes com pai = a chave
	for (int nl = 0; nl < lisLIG->Count; nl++)
	{
		ligacao = (VTLigacao*)lisLIG->Items[nl];
		if (ligacao->ligaPai == pai)
		{
			lisEXT->Add(ligacao);
		}
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TDiagRede::LeArquivo(void)
{ // variaveis locais
	// VTInfoset   *infoset;
	// DIAG_TRECHO *diagTrecho;
	// VTTipos     *tipos       = (VTTipos*)apl->GetObject(__classid(VTTipos));
	// VTGeral     *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
	// int         tipoRede;
	// bool        temPadrao = false;
	//
	// try {
	// //cria novo infoset para ler o arquivo de cfg
	// infoset = geral->Infoset;
	// infoset->Clear();
	// //verifica se é possivel ler o arquivo
	// if(infoset->LoadFromFile(FILE_CFG_TRECHO))
	// {  //varre todo o arquivo lido
	// for (int i = 0; i < infoset->Count; i++)
	// {
	// infoset->SelectLine(i);
	// //verifica qual é o tipo rede  (redeSEGMENTO)
	// infoset->GetInfo("tipo_rede", tipoRede);
	// ///verifica se o tipo a ser carregado do arquivo existe na base
	// //ou se é o tipo "default"
	// if ((tipos->ExisteTipoRede(tipoRede)) || (tipoRede == 0))
	// {  //caso exista, cria-se um novo objeto
	// diagTrecho = new DIAG_TRECHO;
	// infoset->GetInfo("tipo_rede", diagTrecho->tipo_rede);
	// infoset->GetInfo("nome_tipo", diagTrecho->nome_tipo);
	// infoset->GetInfo("comp_min_m", diagTrecho->comp_min_m);
	// infoset->GetInfo("comp_max_m", diagTrecho->comp_max_m);
	// infoset->GetInfo("iadm_min_a", diagTrecho->iadm_min_a);
	// infoset->GetInfo("iadm_max_a", diagTrecho->iadm_max_a);
	// //infoset->AddInfo("pu", diagTrecho->pu);
	// infoset->GetInfo("seq0_r_max_ohmkm", diagTrecho->seq0_r_max_ohmkm);
	// infoset->GetInfo("seq0_r_min_ohmkm", diagTrecho->seq0_r_min_ohmkm);
	// infoset->GetInfo("seq0_x_max_ohmkm", diagTrecho->seq0_x_max_ohmkm);
	// infoset->GetInfo("seq0_x_min_ohmkm", diagTrecho->seq0_x_min_ohmkm);
	// infoset->GetInfo("seq0_c_max_nFkm", diagTrecho->seq0_c_max_nFkm);
	// infoset->GetInfo("seq0_c_min_nFkm", diagTrecho->seq0_c_min_nFkm);
	// infoset->GetInfo("seq1_r_max_ohmkm", diagTrecho->seq1_r_max_ohmkm);
	// infoset->GetInfo("seq1_r_min_ohmkm", diagTrecho->seq1_r_min_ohmkm);
	// infoset->GetInfo("seq1_x_max_ohmkm", diagTrecho->seq1_x_max_ohmkm);
	// infoset->GetInfo("seq1_x_min_ohmkm", diagTrecho->seq1_x_min_ohmkm);
	// infoset->GetInfo("seq1_c_max_nFkm", diagTrecho->seq1_c_max_nFkm);
	// infoset->GetInfo("seq1_c_min_nFkm", diagTrecho->seq1_c_min_nFkm);
	// LisCfgDiagTrecho->Add(diagTrecho);
	// //caso o tipo rede seja 0 (== PADRAO)
	// if(tipoRede == 0){temPadrao = true;}
	// }
	// }
	// }//caso não exista o arquivo
	// else
	// {  //cria configs para todos os
	// VerificaTiposRede();
	// //cria o tipo "default"
	// diagTrecho = CriaCfgTrechoDefault(0, "*Padrão*");
	// //define como padrao
	// //diagTrechoPADRAO = diagTrecho;
	// //adiciona à lista de configs
	// LisCfgDiagTrecho->Add(diagTrecho);
	// temPadrao = true;
	// //e cria o arquivo
	// SalvaArquivo();
	// }
	// if (!temPadrao)
	// {  //cria o tipo "default"
	// diagTrecho = CriaCfgTrechoDefault(0, "*Padrão*");
	// //define como padrao
	// //diagTrechoPADRAO = diagTrecho;
	// //adiciona à lista de configs
	// LisCfgDiagTrecho->Add(diagTrecho);
	// temPadrao = true;
	// }
	// return true;
	// } catch (Exception &e) {return false;}
	return false;
}

// ---------------------------------------------------------------------------
void __fastcall TDiagRede::LeCheckList(TCheckListBox *checkListBox)
{
	this->OpcaoDiagFases = checkListBox->Checked[opRedeFASES];
	this->OpcaoDiagDemTrafo = checkListBox->Checked[opRedeDEMTRAFO];
	this->OpcaoDiagRegRever = checkListBox->Checked[opRedeREGREVER];
	this->OpcaoDiagVnomTrafo = checkListBox->Checked[opRedeVNOMTRAFO];
}

// ---------------------------------------------------------------------------
void __fastcall TDiagRede::LisTrafoConectaBarra(TList *lisLIG, VTBarra *barra, TList *lisEXT)
{
	// variáveis locais
	VTLigacao *ligacao;

	// reinicia lisEXT
	lisEXT->Clear();
	for (int n = 0; n < lisLIG->Count; n++)
	{
		ligacao = (VTLigacao*)lisLIG->Items[n];
		if (ligacao->Tipo() == eqptoTRAFO)
		{
			if (ligacao->ConectaBarra(barra))
				lisEXT->Add(ligacao);
		}
	}
}

// ----------------------------------------------------------------------------
bool __fastcall TDiagRede::SalvaArquivo(void)
{ /* TODO : Decidir se o usuario perde valores guardados pra redes não presentes na base */
	// //variaveis locais
	// VTInfoset   *infoset;
	// DIAG_TRECHO *diagTrecho;
	// VTGeral     *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
	//
	// //infoset = NewObjInfoset(apl);
	// //infoset = (VTInfoset*)apl->GetObject(__classid(VTInfoset));
	// try{
	// infoset = geral->Infoset;
	// infoset->Clear();
	// for (int i = 0; i < LisCfgDiagTrecho->Count; i++)
	// {
	// diagTrecho = (DIAG_TRECHO*) LisCfgDiagTrecho->Items[i];
	// infoset->InsertNewLine();
	// infoset->AddInfo("tipo_rede", diagTrecho->tipo_rede);
	// infoset->AddInfo("nome_tipo", diagTrecho->nome_tipo);
	// infoset->AddInfo("comp_min_m", diagTrecho->comp_min_m);
	// infoset->AddInfo("comp_max_m", diagTrecho->comp_max_m);
	// infoset->AddInfo("iadm_min_a", diagTrecho->iadm_min_a);
	// infoset->AddInfo("iadm_max_a", diagTrecho->iadm_max_a);
	// //infoset->AddInfo("pu", diagTrecho->pu);
	// infoset->AddInfo("seq0_r_max_ohmkm", diagTrecho->seq0_r_max_ohmkm);
	// infoset->AddInfo("seq0_r_min_ohmkm", diagTrecho->seq0_r_min_ohmkm);
	// infoset->AddInfo("seq0_x_max_ohmkm", diagTrecho->seq0_x_max_ohmkm);
	// infoset->AddInfo("seq0_x_min_ohmkm", diagTrecho->seq0_x_min_ohmkm);
	// infoset->AddInfo("seq0_c_max_nFkm", diagTrecho->seq0_c_max_nFkm);
	// infoset->AddInfo("seq0_c_min_nFkm", diagTrecho->seq0_c_min_nFkm);
	// infoset->AddInfo("seq1_r_max_ohmkm", diagTrecho->seq1_r_max_ohmkm);
	// infoset->AddInfo("seq1_r_min_ohmkm", diagTrecho->seq1_r_min_ohmkm);
	// infoset->AddInfo("seq1_x_max_ohmkm", diagTrecho->seq1_x_max_ohmkm);
	// infoset->AddInfo("seq1_x_min_ohmkm", diagTrecho->seq1_x_min_ohmkm);
	// infoset->AddInfo("seq1_c_max_nFkm", diagTrecho->seq1_c_max_nFkm);
	// infoset->AddInfo("seq1_c_min_nFkm", diagTrecho->seq1_c_min_nFkm);
	// }
	// infoset->SaveToFile(FILE_CFG_TRECHO);
	// }
	// catch (Exception &e){return false;}
	return false;
}

// ---------------------------------------------------------------------------
void __fastcall TDiagRede::ShowFormCfg(void)
{
	// nada a fazer
}

// ---------------------------------------------------------------------------
int __fastcall TDiagRede::ShowFormCfgModal(void)
{
	// nada a fazer
	return -1;
}

// ---------------------------------------------------------------------------
void __fastcall TDiagRede::SomaScomplexoPorPatamares(VTEqpto *eqpto, TList *lisValores)
{ // variaveis locais
	// VTPatamares       *patamares =  (VTPatamares*) apl->GetObject(__classid(VTPatamares));
	// VTDemanda         *demanda = DLL_NewObjDemanda(apl);
	VTDemanda *demanda = (VTDemanda*) apl->GetObject(__classid(VTDemanda));
	complex<double> *s_aux;
	bool demandaInterno = false;
	// proteção se não existir demanda
	if (!demanda)
	{
		demandaInterno = true;
		demanda = DLL_NewObjDemanda(apl);
	}
	for (int np = 0; np < lisValores->Count; np++)
	{
		s_aux = (complex<double> *) lisValores->Items[np];
		*s_aux += demanda->S_mva[eqpto][np];
	}
	// deleta o demanda caso tenha sido criado aqui
	if (demanda && demandaInterno)
	{
		delete demanda;
		demanda = NULL;
	}
}

// ---------------------------------------------------------------------------
int __fastcall TDiagRede::Tipo(void)
{
	return (dgeREDE);
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TDiagRede::TipoAsString(void)
{
	return "Rede";
}

// ---------------------------------------------------------------------------
void __fastcall TDiagRede::VerificaDemanda(VTSecundario * secundario)
{ // variaveis locais
	VTPatamares *patamares = (VTPatamares*) apl->GetObject(__classid(VTPatamares));
	VTLigacao *ligacao;
	VTTrafo *trafo;
	VTBarra *barra;
	VTEqbar *eqbar;
	TList *lisEQBAR = NULL;
	TList *valoresS = new TList;
	complex<double> *s_aux;
	double maiorS, P, Q, modS;
	bool temErro = false;

	// inicializa a lista com os valores de S complexo para cada patamar
	for (int np = 0; np < patamares->NumPatamar(); np++)
	{
		s_aux = new complex<double>;
		// s_aux = 0;
		valoresS->Add(s_aux);
	}
	// obtém lista ordenada de barras e ligações do secundario
	secundario->Ordena(lisBAR, lisLIG);
	if (!ExisteTrafo(secundario))
	{
		AddLinhaLog(secundario->Rede, opRedeDEMTRAFO, "Secundário sem trafo MT/BT.");
		temErro = true;
	}
	else
	{ // proteção
		if (lisLIG->Count > 0)
		{
			// olha a primeira ligação
			ligacao = (VTLigacao*) lisLIG->First();
			// espero que a primeira ligacao seja um trafo /* TODO : E OS BANCOS DE TRAFOS MONO? */
			if (ligacao->Tipo() == eqptoTRAFO)
			{ // guarda o trafo
				trafo = (VTTrafo*) ligacao;
				// loop para olhar todas as barras
				for (int nl = 0; nl < lisBAR->Count; nl++)
				{
					barra = (VTBarra*)lisBAR->Items[nl];
					// desconsidera barra do primário do trafo
					if (trafo->BarraPrimario() == barra)
						continue;
					lisEQBAR = barra->LisEqbar();
					for (int ne = 0; ne < lisEQBAR->Count; ne++)
					{
						eqbar = (VTEqbar*) lisEQBAR->Items[ne];
						if (eqbar->Tipo() == eqptoCAPACITOR)
						{
							SomaScomplexoPorPatamares(eqbar, valoresS);
						}
						if (eqbar->Tipo() == eqptoCARGA)
						{
							SomaScomplexoPorPatamares(eqbar, valoresS);
						}
						if (eqbar->Tipo() == eqptoREATOR)
						{
							SomaScomplexoPorPatamares(eqbar, valoresS);
						}
					}
				}
				// verifica o maior módulo de S
				for (int nv = 0; nv < valoresS->Count; nv++)
				{
					s_aux = (complex<double> *) valoresS->Items[nv];
					P = s_aux->real();
					Q = s_aux->imag();
					// calcula o modulo de S
					modS = sqrt(pow(P, 2) + pow(Q, 2));
					// inicializa o maiorS
					if (nv == 0)
						maiorS = modS;
					// guarda o maior S
					if (modS > maiorS)
						maiorS = modS;
				}
				if (maiorS > trafo->snom)
				{
					AddLinhaLog(trafo, opRedeDEMTRAFO,
						"Potência nominal do trafo inferior à soma das cargas da rede BT.");
					temErro = true;
				}
			}
		}
		// incrementa o contador de ocorrencias
		if (temErro)
			infoRedes.n_ocorrencias++;
	}
	// deleta a lista temporaria de valores e os valores
	if (valoresS)
	{
		for (int nv = (valoresS->Count - 1); nv >= 0; nv--)
		{
			s_aux = (complex<double> *) valoresS->Items[nv];
			delete s_aux;
		}
		delete valoresS;
		valoresS = NULL;
	}

}

// ---------------------------------------------------------------------------
void __fastcall TDiagRede::VerificaFases(VTPrimario * primario)
{
	// obtém lista ordenada de barras e ligações do primário
	primario->Ordena(lisBAR, lisLIG);
	// verifica fases de Cargas e Trechos
	VerificaFases();
}

// ---------------------------------------------------------------------------
void __fastcall TDiagRede::VerificaFases(VTSecundario * secundario)
{
	// obtém lista ordenada de barras e ligações do primário
	secundario->Ordena(lisBAR, lisLIG);
	// verifica as fases da 1a ligação do secundario
	VerificaFasesEntradaSecundario(secundario);
	// verifica fases de Trechos e trafos
	VerificaFases();
}

// ---------------------------------------------------------------------------
void __fastcall TDiagRede::VerificaFases(void)
{ // variáveis locais
	VTFases *fases = (VTFases*) apl->GetObject(__classid(VTFases));
	int fases_filha, fases_pai;
	VTLigacao *filha, *pai;

	// loop para olhar todas as ligacoes
	for (int nl = 0; nl < lisLIG->Count; nl++)
	{
		filha = (VTLigacao*)lisLIG->Items[nl];
		// se a ligacao for uma chave, nao verifica
		if (filha->Tipo() == eqptoCHAVE)
			continue;
		// determina Ligacao pai
		if ((pai = ExistePaiValido(filha)) == NULL)
			continue;
		// salva as fases disponiveis do pai
		fases_pai = FasesDisponiveis(pai);
		// salva as fases necessarias do pai
		fases_filha = FasesNecessarias(filha);
		// verifica se a fase da filha "cabe" na fases do pai
		if ((fases_pai & fases_filha) != fases_filha)
		{
			AddLinhaLog(filha, opRedeFASES,
				"Fases incoerentes com as fases do equipamento à montante.");
			// incrementa o contador de ocorrencias
			infoRedes.n_ocorrencias++;
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TDiagRede::VerificaFasesEntradaSecundario(VTSecundario *secundario)
{ // variáveis locais
	VTFases *fases = (VTFases*) apl->GetObject(__classid(VTFases));
	int fases_filha, fases_pai;
	VTBarra *barra;
	VTLigacao *ligacao, *pai, *filha;
	VTTrecho *trecho;
	VTBarra *bar_filha, *bar_pai; // *bar_carga;
	TList *lisFilhas;

	// determina Barra inicial
	if ((barra = secundario->BarraInicial) == NULL)
		return;
	// verifica se a Barra inicial tem informação de sua Ligacao pai
	if (barra->Obj == NULL)
		return;
	// determina Ligacao pai da Barra inicial do Secundario
	ligacao = (VTLigacao*)barra->Obj;
	// determina o pai
	// if (ligacao->Tipo() != eqptoTRECHO)
	pai = ExistePaiValido(ligacao);
	// protecao
	if (pai == NULL)
		return;
	// protecao
	if (lisLIG->Count <= 0)
	{
		return;
	}
	// pega a 1a ligacao do secundario
	filha = (VTLigacao*)lisLIG->First();
	// salva as fases disponiveis do pai
	fases_pai = FasesDisponiveis(pai);
	// salva as fases necessarias do pai
	fases_filha = FasesNecessarias(filha);
	// verifica se a fase da filha "cabe" na fases do pai
	if ((fases_pai & fases_filha) != fases_filha)
	{
		AddLinhaLog(filha, opRedeFASES, "Início da rede BT com fases incoerentes.");
		// incrementa o contador de ocorrencias
		infoRedes.n_ocorrencias++;
	}
}

// ---------------------------------------------------------------------------
// void __fastcall  TDiagRede::VerificaFasesTrafo(VTTrecho *trecho_pai, VTTrafo *trafo)
// {
// //variáveis locais
// struct{
// int     fases_pri, fases_sec;
// }antes, depois;
// int     fases_pai, fases_filha, fases_nt;
// VTBarra *bar_ref_trafo, *bar_ref_trecho;
//
// //atualiza log
// if (plog) plog->DefineValor("Tabela", "");
// if (plog) plog->DefineValor("Eqpto",  trafo->TipoAsString());
// if (plog) plog->DefineValor("Código", trafo->Codigo);
// if (plog) plog->DefineValor("Rede",   trafo->rede->Codigo);
// if (plog) plog->DefineValor("ID",     trafo->Extern_id);
// //salva fases originais do trafo
// antes.fases_pri = trafo->Fases(trafo->BarraPrimario());
// antes.fases_sec = trafo->Fases(trafo->BarraSecundario());
// //CUIDADO: pode haver outras Ligações entre o Trafo e o Trecho pai
// bar_ref_trafo  = trafo->BarraComum(trafo->ligaPai);
// bar_ref_trecho = BarraComum(trecho_pai, trafo);
// //proteção
// if ((bar_ref_trafo == NULL)||(bar_ref_trecho == NULL)) return;
// //determina fases do trecho pai, excluindo neutro e terra
// fases_pai = trecho_pai->arranjo->Fases & faseABC;
// //determina fases do Trafo (enrolamento conectado pela Barra)
// fases_filha = trafo->Fases(bar_ref_trafo);
// //salva informação de neutro e terra do trafo
// fases_nt = fases_filha & (faseN | faseT);
// //elimina neutro e terra das fases do trafo
// fases_filha &= faseABC;
// //verifica se as fases do Trafo são compatíveis com as fases da Ligacao pai
////   if ((fases_filha & fases_pai) != fases_filha)
// if ((fases_filha & fases_pai) != fases_pai)
////      {//trafo com fases incorretas
////      if (fases->NumeroDeFases(fases_filha) <= fases->NumeroDeFases(fases_pai))
////         {//trafo com apenas 1 fase: assume uma das fases pai
////         if      ((fases_filha == faseB) && (fases_pai & faseC)) fases_filha = faseC;
////         else if ((fases_filha == faseC) && (fases_pai & faseB)) fases_filha = faseB;
////         else if ((fases_pai & faseA) == faseA)  fases_filha = faseA;
////         else if ((fases_pai & faseB) == faseB)  fases_filha = faseB;
////         else if ((fases_pai & faseC) == faseC)  fases_filha = faseC;
////         }
// {//trafo com fases incorretas
// //DVK 2016.05.23 validação depende do número de fases
// if (fases->NumeroDeFases(fases_filha) == fases->NumeroDeFases(fases_pai))
// {//assume mesmas fases do pai
// fases_filha = fases_pai;
// }
// if ((fases->NumeroDeFases(fases_filha) == 2) && (fases->NumeroDeFases(fases_pai) == 1))
// {//assume mesmas fases do pai
// fases_filha = fases_pai;
// }
// if ((fases->NumeroDeFases(fases_filha) == 1) && (fases->NumeroDeFases(fases_pai) == 2))
// {//trafo com apenas 1 fase: assume uma das fases pai
// if      ((fases_filha == faseB) && (fases_pai & faseC)) fases_filha = faseC;
// else if ((fases_filha == faseC) && (fases_pai & faseB)) fases_filha = faseB;
// else if ((fases_pai & faseA) == faseA)  fases_filha = faseA;
// else if ((fases_pai & faseB) == faseB)  fases_filha = faseB;
// else if ((fases_pai & faseC) == faseC)  fases_filha = faseC;
// }
// //atualiza fases do Trafos
// if (bar_ref_trafo == trafo->BarraPrimario()) trafo->pri.fase = fases_filha | fases_nt;
// else                                         trafo->sec.fase = fases_filha | fases_nt;
// //atualiza Status
// trafo->Status[sttERROGIS] = true;
// //salva fases depois da alteração
// depois.fases_pri = trafo->Fases(trafo->BarraPrimario());
// depois.fases_sec = trafo->Fases(trafo->BarraSecundario());
// //DVK 2014.10.14 altera mensagem no log
// if((antes.fases_pri == depois.fases_pri) && (antes.fases_sec == depois.fases_sec))
// {
// if (plog) plog->Add("Fases do trafo " + fases->AsStrABCN[antes.fases_pri]
// + "/" + fases->AsStrABCN[antes.fases_sec] +
// " incompatíveis com fases da rede " + fases->AsStrABCN[fases_pai]);
// }
// else
// {
// if (plog) plog->Add("Fases alteradas de " +
// fases->AsStrABCN[antes.fases_pri] + "/" +
// fases->AsStrABCN[antes.fases_sec] + " para " +
// fases->AsStrABCN[depois.fases_pri] + "/" +
// fases->AsStrABCN[depois.fases_sec]);
// }
// }
// }
// ---------------------------------------------------------------------------
void __fastcall TDiagRede::VerificaRede(void)
{ // variáveis locais
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	radial = (VTRadial*)apl->GetObject(__classid(VTRadial));

	if (radial == NULL)
	{ // cria objeto e insere em Apl
		apl->Add(radial = DLL_NewObjRadial(apl));
	}
	// inicia objeto Radial
	radial->Inicia(redes);
	if (dgRede.fases)
	{
		VerificaOpFases();
	}
	if (dgRede.demTrafo)
	{
		VerificaOpDemTrafos();
	}
	if (dgRede.regRever)
	{
		VerificaOpRegRever();
	}
	if (dgRede.vnomTrafo)
	{
		VerificaOpVnomTrafos();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TDiagRede::VerificaOpFases(void)
{ // variáveis locais
	TList *lisSEC, *lisPRI;
	VTPrimario *primario;
	VTSecundario *secundario;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	// VTRadial     *radial = (VTRadial*)apl->GetObject(__classid(VTRadial));

	if (radial == NULL)
	{
		radial = (VTRadial*)apl->GetObject(__classid(VTRadial));
		if (radial == NULL)
		{
			// cria objeto e insere em Apl
			apl->Add(radial = DLL_NewObjRadial(apl));
		}
		// inicia objeto Radial
		radial->Inicia(redes);
	}
	// //inicia objeto Radial
	// radial->Inicia(redes);
	// obtem lista de redes primárias
	lisPRI = radial->LisPrimario();
	// obtém lista de redes secundárias
	lisSEC = radial->LisSecundario();
	// loop p/todos primarios
	for (int npri = 0; npri < lisPRI->Count; npri++)
	{
		primario = (VTPrimario*) lisPRI->Items[npri];
		// if (progresso) progresso->Add(primario->Rede->Codigo);
		VerificaFases(primario);
		// incrementa o n de verificacoes
		infoRedes.n_verif++;
	}
	// loop p/ todos Secundario
	for (int n = 0; n < lisSEC->Count; n++)
	{
		secundario = (VTSecundario*)lisSEC->Items[n];
		// if (progresso) progresso->Add(secundario->Rede->Codigo);
		VerificaFases(secundario);
		// incrementa o n de verificacoes
		infoRedes.n_verif++;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TDiagRede::VerificaOpDemTrafos(void)
{ // variáveis locais
	TList *lisSEC, *lisPRI;
	// VTPrimario   *primario;
	VTSecundario *secundario;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	// VTRadial     *radial = (VTRadial*)apl->GetObject(__classid(VTRadial));

	if (radial == NULL)
	{
		radial = (VTRadial*)apl->GetObject(__classid(VTRadial));
		if (radial == NULL)
		{
			// cria objeto e insere em Apl
			apl->Add(radial = DLL_NewObjRadial(apl));
		}
		// inicia objeto Radial
		radial->Inicia(redes);
	}
	// obtem lista de redes primárias
	lisPRI = radial->LisPrimario();
	// obtém lista de redes secundárias
	lisSEC = radial->LisSecundario();
	// loop p/ todos Secundario
	for (int n = 0; n < lisSEC->Count; n++)
	{
		secundario = (VTSecundario*)lisSEC->Items[n];
		// if (progresso) progresso->Add(secundario->Rede->Codigo);
		// if (n==335)
		// {
		// int a = 0;
		// }
		VerificaDemanda(secundario);
		// incrementa o n de verificacoes
		infoRedes.n_verif++;
	}

}

// ---------------------------------------------------------------------------
void __fastcall TDiagRede::VerificaOpRegRever(void)
{ // método eliminado, pois o regulador sempre vai operar no modo reversível, mas com comportamento diferente,
	// dependendo do modelo escolhido

	// variáveis locais
	// VTRedes     *redes  = (VTRedes*)apl->GetObject(__classid(VTRedes));
	// VTRede      *rede;
	// VTRegulador *regulador;
	// TList       *lisREDE = redes->LisRede();
	// TList       *mlisEQP  = new TList();
	//
	// verifica se há redes com geração e reguladores de tensão sem inversão no fluxo
	// isso pode fazer a rede não rodar fluxo
	// for(int nr = 0; (nr < lisREDE->Count) ; nr++)
	// {//verifica se a mensagem de alerta já foi gerada
	// rede = (VTRede*)lisREDE->Items[nr];
	// limpa lista
	// mlisEQP->Clear();
	// verifica se a rede tem geração
	// rede->LisEqpto(mlisEQP, eqptoGERADOR);
	// if(mlisEQP->Count == 0)  continue;
	// limpa lista
	// mlisEQP->Clear();
	// verifica se a rede tem reguladores
	// rede->LisEqpto(mlisEQP, eqptoREGULADOR);
	// for(int n = 0; n < mlisEQP->Count; n++)
	// {
	// regulador = (VTRegulador*)mlisEQP->Items[n];
	// verifica se o regulador tem inversão do fluxo
	// if(! regulador->sentido.reversivel)
	// {//emite alerta
	// AddLinhaLog(rede,opRedeREGREVER,"Rede com geração: existe, pelo menos,"
	// " um regulador de tensão sem inversão no fluxo");
	// break;
	// }
	// }
	// }
	// destroi lista
	// delete mlisEQP;
}
// ---------------------------------------------------------------------------
// void  __fastcall  TDiagRede::VerificaOpVnomTrafos(void)
// {//variáveis locais
// VTRedes  *redes  = (VTRedes*)apl->GetObject(__classid(VTRedes));
// VTFases  *fases  = (VTFases*)apl->GetObject(__classid(VTFases));
// VTBarra  *barra1, *barra2;
// VTTrafo  *trafo;
// double   vf1, vl1, vf2, vl2;
//
// //limpa lista de trafos
// lisTRF->Clear();
// //preenche a lsita com todos os trafos
// redes->LisLigacao(lisTRF,eqptoTRAFO);
// //percorre tods trafos
// for (int nt = 0; nt < lisTRF->Count; nt++ )
// {
// trafo = (VTTrafo*) lisTRF->Items[nt];
// trafo->OrdenaBar12PriSec();
// barra1 = trafo->BarraPrimario();
// barra2 = trafo->BarraSecundario();
// //define valores de V fase V linha
// if(barra1->zona)
// {
// vl1 = barra1->zona->Vff_kv;
// //verifica a tensao da fase disponivel no primario
// if((trafo->pri.fase & faseA) == faseA)vf1 = barra1->zona->Vfn_kv[indFASE_A];
// else if((trafo->pri.fase & faseB) == faseB)vf1 = barra1->zona->Vfn_kv[indFASE_B];
// else if((trafo->pri.fase & faseC) == faseC)vf1 = barra1->zona->Vfn_kv[indFASE_C];
// //caso nao tenha nenhuma fase disponivel..
// else {vf1 = (barra1->vnom)/M_SQRT_3;}
// }
// else
// {
// vl1 = barra1->vnom;
// vf1 = (barra1->vnom)/M_SQRT_3;
// }
// if(barra2->zona)
// {
// vl2 = barra2->zona->Vff_kv;
// //verifica a tensao da fase disponivel no primario
// if((trafo->sec.fase & faseA) == faseA)vf2 = barra2->zona->Vfn_kv[indFASE_A];
// else if((trafo->sec.fase & faseB) == faseB)vf2 = barra2->zona->Vfn_kv[indFASE_B];
// else if((trafo->sec.fase & faseC) == faseC)vf2 = barra2->zona->Vfn_kv[indFASE_C];
// //caso nao tenha nenhuma fase disponivel..
// else {vf2 = (barra2->vnom)/M_SQRT_3;}
// }
// else
// {
// vl2 = barra2->vnom;
// vf2 = (barra2->vnom)/M_SQRT_3;
// }
// //se for trafo mono
// if(trafo->Monofasico)
// {
// //verifica o numero de fases do pri
// if(fases->NumeroDeFases(trafo->pri.fase) > 1)
// {
// if( (vl1 < 0.9*trafo->pri.vnom) || (vl1 > 1.1*trafo->pri.vnom) )
// {
// AddLinhaLog(trafo,opRedeVNOMTRAFO,"Tensão nominal do primário muito diferente da tensão na barra.");
// }
// }
// else
// {
// if( (vf1 < 0.9*trafo->pri.vnom) || (vf1 > 1.1*trafo->pri.vnom) )
// {
// AddLinhaLog(trafo,opRedeVNOMTRAFO,"Tensão nominal do primário muito diferente da tensão na barra.");
// }
// }
// //verifica o numero de fases do sec
// if(fases->NumeroDeFases(trafo->sec.fase) > 1)
// {
// if( (vl2 < 0.9*trafo->sec.vnom) || (vl2 > 1.1*trafo->sec.vnom) )
// {
// AddLinhaLog(trafo,opRedeVNOMTRAFO,"Tensão nominal do secundário muito diferente da tensão na barra.");
// }
// }
// else
// {
// if( (vf2 < 0.9*trafo->sec.vnom) || (vf2 > 1.1*trafo->sec.vnom) )
// {
// AddLinhaLog(trafo,opRedeVNOMTRAFO,"Tensão nominal do secundário muito diferente da tensão na barra.");
// }
// }
// }
// //se for trafo trifasico
// else
// {
// if( (vl1 < 0.9*trafo->pri.vnom) || (vl1 > 1.1*trafo->pri.vnom) )
// {
// AddLinhaLog(trafo,opRedeVNOMTRAFO,"Tensão nominal do primário muito diferente da tensão na barra.");
// }
// if( (vl2 < 0.9*trafo->sec.vnom) || (vl2 > 1.1*trafo->sec.vnom) )
// {
// AddLinhaLog(trafo,opRedeVNOMTRAFO,"Tensão nominal do secundário muito diferente da tensão na barra.");
// }
// }
// }
// }

// ---------------------------------------------------------------------------
void __fastcall TDiagRede::VerificaOpVnomTrafos(void)
{ // variáveis locais
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTFases *fases = (VTFases*)apl->GetObject(__classid(VTFases));
	VTBarra *barra1, *barra2;
	VTTrafo *trafo;
	double vf1, vl1, vf2, vl2;

	// limpa lista de trafos
	lisTRF->Clear();
	// preenche a lsita com todos os trafos
	redes->LisLigacao(lisTRF, eqptoTRAFO);
	// percorre todos trafos
	for (int nt = 0; nt < lisTRF->Count; nt++)
	{
		trafo = (VTTrafo*) lisTRF->Items[nt];
		trafo->OrdenaBar12PriSec();
		barra1 = trafo->BarraPrimario();
		barra2 = trafo->BarraSecundario();
		// define valores de V fase V linha
		if (barra1->zona != NULL)
		{
			if (barra1->zona->Tipo != zonaINDEFINIDA)
			{
				vl1 = barra1->zona->Vff_kv;
				if (trafo->Monofasico)
				{ // verifica a tensao da fase disponivel no primario
					if ((trafo->pri.fase & faseA) == faseA)
						vf1 = barra1->zona->Vfn_kv[indFASE_A];
					else if ((trafo->pri.fase & faseB) == faseB)
						vf1 = barra1->zona->Vfn_kv[indFASE_B];
					else if ((trafo->pri.fase & faseC) == faseC)
						vf1 = barra1->zona->Vfn_kv[indFASE_C];
					// caso nao tenha nenhuma fase disponivel..
					else
					{
						vf1 = 0.;
					}
				}
				else // caso seja trifasico
				{
					vf1 = (barra1->vnom) / M_SQRT_3;
				}
			}
			else // caso possua zona indefinida
			{ // assume trifasico
				vl1 = barra1->vnom;
				vf1 = (barra1->vnom) / M_SQRT_3;
			}
		}
		else // caso não possua zona
		{ // assume trifasico
			vl1 = barra1->vnom;
			vf1 = (barra1->vnom) / M_SQRT_3;
		}
		if (barra2->zona != NULL)
		{
			if (barra2->zona->Tipo != zonaINDEFINIDA)
			{
				vl2 = barra2->zona->Vff_kv;
				if (trafo->Monofasico)
				{ // verifica a tensao da fase disponivel no primario
					if ((trafo->sec.fase & faseA) == faseA)
						vf2 = barra2->zona->Vfn_kv[indFASE_A];
					else if ((trafo->sec.fase & faseB) == faseB)
						vf2 = barra2->zona->Vfn_kv[indFASE_B];
					else if ((trafo->sec.fase & faseC) == faseC)
						vf2 = barra2->zona->Vfn_kv[indFASE_C];
					// caso nao tenha nenhuma fase disponivel..
					else
					{
						vf2 = 0.;
					}
				}
				else // caso seja trifasico
				{
					vf2 = (barra2->vnom) / M_SQRT_3;
				}
			}
			else // caso possua zona indefinida
			{ // assume trifasico
				vl2 = barra2->vnom;
				vf2 = (barra2->vnom) / M_SQRT_3;
			}
		}
		else // caso não possua zona
		{ // assume trifasico
			vl2 = barra2->vnom;
			vf2 = (barra2->vnom) / M_SQRT_3;
		}
		// proteção
		if (IsDoubleZero(vl1) || IsDoubleZero(vl2) || IsDoubleZero(vf1) || IsDoubleZero(vf2))
		{
			AddLinhaLog(trafo, opRedeVNOMTRAFO, "Tensões Nulas");
			continue;
		}
		// se for trafo mono
		if (trafo->Monofasico)
		{
			// verifica o numero de fases do pri
			if (fases->NumeroDeFases(trafo->pri.fase) > 1)
			{
				if ((vl1 < 0.9 * trafo->pri.vnom) || (vl1 > 1.1 * trafo->pri.vnom))
				{
					AddLinhaLog(trafo, opRedeVNOMTRAFO,
						"Tensão nominal do primário muito diferente da tensão na barra.");
				}
			}
			else
			{
				if ((vf1 < 0.9 * trafo->pri.vnom) || (vf1 > 1.1 * trafo->pri.vnom))
				{
					AddLinhaLog(trafo, opRedeVNOMTRAFO,
						"Tensão nominal do primário muito diferente da tensão na barra.");
				}
			}
			// verifica o numero de fases do sec
			if (fases->NumeroDeFases(trafo->sec.fase) > 1)
			{
				if ((vl2 < 0.9 * trafo->sec.vnom) || (vl2 > 1.1 * trafo->sec.vnom))
				{
					AddLinhaLog(trafo, opRedeVNOMTRAFO,
						"Tensão nominal do secundário muito diferente da tensão na barra.");
				}
			}
			else
			{
				if ((vf2 < 0.9 * trafo->sec.vnom) || (vf2 > 1.1 * trafo->sec.vnom))
				{
					AddLinhaLog(trafo, opRedeVNOMTRAFO,
						"Tensão nominal do secundário muito diferente da tensão na barra.");
				}
			}
		}
		// se for trafo trifasico
		else
		{
			if ((vl1 < 0.9 * trafo->pri.vnom) || (vl1 > 1.1 * trafo->pri.vnom))
			{
				AddLinhaLog(trafo, opRedeVNOMTRAFO,
					"Tensão nominal do primário muito diferente da tensão na barra.");
			}
			if ((vl2 < 0.9 * trafo->sec.vnom) || (vl2 > 1.1 * trafo->sec.vnom))
			{
				AddLinhaLog(trafo, opRedeVNOMTRAFO,
					"Tensão nominal do secundário muito diferente da tensão na barra.");
			}
		}
	}
}

// ---------------------------------------------------------------------------
