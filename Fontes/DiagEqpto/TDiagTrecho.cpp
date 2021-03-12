// ---------------------------------------------------------------------------

#pragma hdrstop
#include <vcl.h>
#include "..\Apl\VTApl.h"
#include "..\Geral\VTInfoset.h"
#include "..\Geral\VTGeral.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTTrecho.h"
#include "..\Rede\VTTipos.h"
#include "..\Rede\VTTipoRede.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "TDiagTrecho.h"
#include "TFormDiagTrecho.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
// ---------------------------------------------------------------------------
// define do nome do arquivo
#define FILE_CFG_TRECHO "CfgDiagEqptoTrecho"

// ---------------------------------------------------------------------------
// VTDiagEqptoElem* __fastcall NewObjDiagEqptoElem(VTApl *apl)
// {
// return (new TDiagEqptoTrecho(apl));
// }
// ---------------------------------------------------------------------------
__fastcall TDiagTrecho::TDiagTrecho(VTApl *apl)
{
	this->apl = apl;
	// define se tem informações adicionais
	infoExtra = false;
	// cria a lista de configuracoes
	listCfg = new TList;
	// inicializa as opcoes do diagnostico
	dgTrecho.comp = true;
	dgTrecho.iAdm = true;
	dgTrecho.zSeq = true;
	dgTrecho.tensoes = true;
	// cria o log_trecho
	/* TODO : mudar para o executa? */
	log_trecho = new TStringList;
	// Mostra que ainda não foi executado
	executado = false;
}

// ---------------------------------------------------------------------------
__fastcall TDiagTrecho::~TDiagTrecho(void)
{
	// destroi lista e conteudo
	if (listCfg)
	{
		LimpaTList(listCfg);
		delete listCfg;
		listCfg = NULL;
	}
	// destroi log
	if (log_trecho)
	{
		delete log_trecho;
		log_trecho = NULL;
	}
	// destroi form
	if (formDiagTrecho)
	{
		delete formDiagTrecho;
		formDiagTrecho = NULL;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TDiagTrecho::AddLinhaLog(VTTrecho *trecho, int opDIAG_TRECHO)
{
	// variaveis locais
	VTTipoRede *tiporede = NULL;

	// incrementa o contador de erros
	// infoTrechos.n_ocorrencias++;
	temErro = true;
	// vetor de strings seguindo o enum col_LOG_TRECHO
	AnsiString linhaPreenchida = "";
	AnsiString linha[colTch_COUNT];
	AnsiString Separador = ";";
	// inicializa com os valores vindos do trecho
	if (trecho->rede)
	{
		tiporede = trecho->rede->TipoRede;
		if (tiporede)
		{
			linha[colTch_SEGMENTO] = trecho->rede->TipoRede->Codigo + Separador;
		}
		else
		{
			linha[colTch_SEGMENTO] = "Nenhum" + Separador;
		}
		linha[colTch_REDE] = trecho->rede->Codigo + Separador;
	}
	else
	{
		linha[colTch_SEGMENTO] = "Nenhum";
		linha[colTch_REDE] = "Nenhum";
	}
	// linha[colLog_EQPTO]     = trecho->TipoEqpto_AsString(trecho->Tipo()) + Separador;
	linha[colTch_EQPTO] = "Trecho" + Separador;
	linha[colTch_CODIGO] = trecho->Codigo + Separador;
	linha[colTch_CODBARRA1] = trecho->pbarra1->Codigo + Separador;
	linha[colTch_CODBARRA2] = trecho->pbarra2->Codigo + Separador;

	if (trecho)
	{
		switch (opDIAG_TRECHO)
		{
		case opTchCOMP:
			linha[colTch_TIPOERRO] = str_TIPOERRO_AVISO + Separador;
			linha[colTch_DESCRICAO] = "Valor de comprimento fora dos limites configurados.";
			break;

		case opTchIADM:
			linha[colTch_TIPOERRO] = str_TIPOERRO_AVISO + Separador;
			linha[colTch_DESCRICAO] = "Valor de corrente admissível fora dos limites configurados.";
			break;

		case opTchZSEQ:
			linha[colTch_TIPOERRO] = str_TIPOERRO_AVISO + Separador;
			linha[colTch_DESCRICAO] =
				"Valores de Impedância de sequência fora dos limites configurados.";
			break;

		case opTchTENSOES:
			linha[colTch_TIPOERRO] = str_TIPOERRO_ERRO + Separador;
			linha[colTch_DESCRICAO] = "Tensões nominais das barras são diferentes.";
			break;

		default: ;
		}
		// concatena os campos em uma linha
		for (int i = 0; i < colTch_COUNT; i++)
		{
			linhaPreenchida += linha[i];
		}
		log_trecho->AddObject(linhaPreenchida, trecho);
	}
}

// ---------------------------------------------------------------------------
DIAG_TRECHO* __fastcall TDiagTrecho::CriaCfgTrechoDefault(int redeSEGMENTO, AnsiString nomeTipo)
{
	DIAG_TRECHO *cfgDiagTrecho;

	cfgDiagTrecho = new DIAG_TRECHO;
	cfgDiagTrecho->tipo_rede = redeSEGMENTO;
	cfgDiagTrecho->nome_tipo = nomeTipo;
	// poe valores default
	cfgDiagTrecho->comp_min_m = 0;
	cfgDiagTrecho->comp_max_m = 500;
	cfgDiagTrecho->iadm_min_a = 0;
	cfgDiagTrecho->iadm_max_a = 200;
	cfgDiagTrecho->seq0_r_max_ohmkm = 25;
	cfgDiagTrecho->seq0_r_min_ohmkm = 0;
	cfgDiagTrecho->seq0_x_max_ohmkm = 1;
	cfgDiagTrecho->seq0_x_min_ohmkm = 0;
	cfgDiagTrecho->seq0_c_max_nFkm = 1;
	cfgDiagTrecho->seq0_c_min_nFkm = 0;
	cfgDiagTrecho->seq1_r_max_ohmkm = 25;
	cfgDiagTrecho->seq1_r_min_ohmkm = 0;
	cfgDiagTrecho->seq1_x_max_ohmkm = 1;
	cfgDiagTrecho->seq1_x_min_ohmkm = 0;
	cfgDiagTrecho->seq1_c_max_nFkm = 1;
	cfgDiagTrecho->seq1_c_min_nFkm = 0;

	return cfgDiagTrecho;
}

// ---------------------------------------------------------------------------
// void  __fastcall  TDiagEqptoTrecho::VerificaDiagCompri(void)
// {
//
// }
//
////---------------------------------------------------------------------------
// void  __fastcall  TDiagEqptoTrecho::VerificaDiagIAdm(void)
// {
//
// }
//
////---------------------------------------------------------------------------
// void  __fastcall  TDiagEqptoTrecho::VerificaDiagZSeq(void)
// {
//
// }

// ---------------------------------------------------------------------------
bool __fastcall TDiagTrecho::Executa(void)
{ // variaveis locais
	VTRedes *redes = (VTRedes*) apl->GetObject(__classid(VTRedes));
	VTRede *rede;
	VTLigacao *ligacao;
	VTTrecho *trecho;

	// inicialza infos
	infoTrechos.n_total = 0;
	infoTrechos.n_verif = 0;
	infoTrechos.n_ocorrencias = 0;

	if (log_trecho)
		IniciaLog(log_trecho);
	// if(log_trecho){log_trecho->Clear();}
	try
	{ // percorre todas as redes
		for (int nr = 0; nr < redes->LisRede()->Count; nr++)
		{
			rede = (VTRede*) redes->LisRede()->Items[nr];
			// percorre todas ligacao
			for (int nt = 0; nt < rede->LisLigacao()->Count; nt++)
			{
				ligacao = (VTLigacao*) rede->LisLigacao()->Items[nt];
				// se a ligacao for um trecho
				if (ligacao->Tipo() == eqptoTRECHO)
				{ // incrementa o numero de trechos
					infoTrechos.n_total++;
					// um cast na ligacao
					trecho = (VTTrecho*) ligacao;
					temErro = false;
					// verifica o trecho
					VerificaTrecho(trecho);
					if (temErro)
						infoTrechos.n_ocorrencias++;
				}
			}
		}
		// altera o boolean mostrando que foi executado
		executado = true;
		return true;
	}
	catch (Exception &e)
	{
		return false;
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TDiagTrecho::ExisteCfgTipoRede(int redeSEGMENTO)
{ // variaveis locais
	DIAG_TRECHO *cfgDiagTrecho;

	for (int nt = 0; nt < listCfg->Count; nt++)
	{
		cfgDiagTrecho = (DIAG_TRECHO*) listCfg->Items[nt];
		if (cfgDiagTrecho->tipo_rede == redeSEGMENTO)
		{
			return true;
		}
	}
	return false;
}

// ---------------------------------------------------------------------------
DIAG_TRECHO* __fastcall TDiagTrecho::Get_DiagTrecho(int redeSEGMENTO)
{ // variaveis locais
	DIAG_TRECHO *cfgDiagTrecho;

	for (int i = 0; i < listCfg->Count; i++)
	{
		cfgDiagTrecho = (DIAG_TRECHO*) listCfg->Items[i];
		if (redeSEGMENTO == cfgDiagTrecho->tipo_rede)
		{
			return cfgDiagTrecho;
		}
	}
	return NULL;
}

// ---------------------------------------------------------------------------
void __fastcall TDiagTrecho::Inicia(void)
{
	// primeiramente carrega as configs do arquivo
	LeArquivo();
	// verifica se falta configs pros tipos de rede existentes
	VerificaTiposRede();
	//
}

// ---------------------------------------------------------------------------
void __fastcall TDiagTrecho::IniciaLog(TStringList *log)
{ // variaveis locais
	AnsiString Separador = ";";
	AnsiString linha[colTch_COUNT];
	AnsiString linhaPreenchida = "";
	if (log)
	{
		// limpa o log
		log->Clear();

		linha[colTch_SEGMENTO] = "Segmento" + Separador;
		linha[colTch_REDE] = "Rede" + Separador;
		linha[colTch_EQPTO] = "Eqpto" + Separador;
		linha[colTch_CODIGO] = "Código" + Separador;
		linha[colTch_TIPOERRO] = "Tipo Erro" + Separador;
		linha[colTch_CODBARRA1] = "Barra 1" + Separador;
		linha[colTch_CODBARRA2] = "Barra 2" + Separador;
		linha[colTch_DESCRICAO] = "Descrição";
		// concatena os campos em uma linha
		for (int i = 0; i < colTch_COUNT; i++)
		{
			linhaPreenchida += linha[i];
		}
		log->Add(linhaPreenchida);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TDiagTrecho::IniciaCheckList(TCheckListBox *checkListBox)
{
	checkListBox->Clear();
	// checkListBox->Items->AddObject(op_TCH_TEXT[opTchCOMP],&dgTrecho.comp);
	// checkListBox->Checked[opTchCOMP] = (bool*) checkListBox->Items->Objects[opTchCOMP];
	//
	// checkListBox->Items->AddObject(op_TCH_TEXT[opTchIADM],&dgTrecho.iAdm);
	// checkListBox->Checked[opTchIADM] = (bool*) checkListBox->Items->Objects[opTchIADM];
	//
	// checkListBox->Items->AddObject(op_TCH_TEXT[opTchZSEQ],&dgTrecho.zSeq);
	// checkListBox->Checked[opTchZSEQ] = (bool*) checkListBox->Items->Objects[opTchZSEQ];
	//
	// checkListBox->Items->AddObject(op_TCH_TEXT[opTchTENSOES],&dgTrecho.tensoes);
	// checkListBox->Checked[opTchTENSOES] = (bool*) checkListBox->Items->Objects[opTchTENSOES];
	checkListBox->Items->Add(op_TCH_TEXT[opTchCOMP]);
	checkListBox->Checked[opTchCOMP] = this->OpcaoDiagCompri;
	checkListBox->Items->Add(op_TCH_TEXT[opTchIADM]);
	checkListBox->Checked[opTchIADM] = this->OpcaoDiagIadm;
	checkListBox->Items->Add(op_TCH_TEXT[opTchZSEQ]);
	checkListBox->Checked[opTchZSEQ] = this->OpcaoDiagZseq;
	checkListBox->Items->Add(op_TCH_TEXT[opTchTENSOES]);
	checkListBox->Checked[opTchTENSOES] = this->OpcaoDiagTensoes;
}

// ---------------------------------------------------------------------------
bool __fastcall TDiagTrecho::LeArquivo(void)
{ // variaveis locais
	VTInfoset *infoset;
	DIAG_TRECHO *cfgDiagTrecho;
	VTTipos *tipos = (VTTipos*)apl->GetObject(__classid(VTTipos));
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
	int tipoRede;
	bool temPadrao = false;

	try
	{
		// cria novo infoset para ler o arquivo de cfg
		infoset = geral->Infoset;
		infoset->Clear();
		// verifica se é possivel ler o arquivo
		if (infoset->LoadFromFile(FILE_CFG_TRECHO))
		{ // varre todo o arquivo lido
			for (int i = 0; i < infoset->Count; i++)
			{
				infoset->SelectLine(i);
				// verifica qual é o tipo rede  (redeSEGMENTO)
				infoset->GetInfo("tipo_rede", tipoRede);
				///verifica se o tipo a ser carregado do arquivo existe na base
				// ou se é o tipo "default"
				if ((tipos->ExisteTipoRede(tipoRede)) || (tipoRede == 0))
				{ // caso exista, cria-se um novo objeto
					cfgDiagTrecho = new DIAG_TRECHO;
					infoset->GetInfo("tipo_rede", cfgDiagTrecho->tipo_rede);
					infoset->GetInfo("nome_tipo", cfgDiagTrecho->nome_tipo);
					infoset->GetInfo("comp_min_m", cfgDiagTrecho->comp_min_m);
					infoset->GetInfo("comp_max_m", cfgDiagTrecho->comp_max_m);
					infoset->GetInfo("iadm_min_a", cfgDiagTrecho->iadm_min_a);
					infoset->GetInfo("iadm_max_a", cfgDiagTrecho->iadm_max_a);
					// infoset->AddInfo("pu", diagTrecho->pu);
					infoset->GetInfo("seq0_r_max_ohmkm", cfgDiagTrecho->seq0_r_max_ohmkm);
					infoset->GetInfo("seq0_r_min_ohmkm", cfgDiagTrecho->seq0_r_min_ohmkm);
					infoset->GetInfo("seq0_x_max_ohmkm", cfgDiagTrecho->seq0_x_max_ohmkm);
					infoset->GetInfo("seq0_x_min_ohmkm", cfgDiagTrecho->seq0_x_min_ohmkm);
					infoset->GetInfo("seq0_c_max_nFkm", cfgDiagTrecho->seq0_c_max_nFkm);
					infoset->GetInfo("seq0_c_min_nFkm", cfgDiagTrecho->seq0_c_min_nFkm);
					infoset->GetInfo("seq1_r_max_ohmkm", cfgDiagTrecho->seq1_r_max_ohmkm);
					infoset->GetInfo("seq1_r_min_ohmkm", cfgDiagTrecho->seq1_r_min_ohmkm);
					infoset->GetInfo("seq1_x_max_ohmkm", cfgDiagTrecho->seq1_x_max_ohmkm);
					infoset->GetInfo("seq1_x_min_ohmkm", cfgDiagTrecho->seq1_x_min_ohmkm);
					infoset->GetInfo("seq1_c_max_nFkm", cfgDiagTrecho->seq1_c_max_nFkm);
					infoset->GetInfo("seq1_c_min_nFkm", cfgDiagTrecho->seq1_c_min_nFkm);
					listCfg->Add(cfgDiagTrecho);
					// caso o tipo rede seja 0 (== PADRAO)
					if (tipoRede == 0)
					{
						temPadrao = true;
					}
				}
			}
		} // caso não exista o arquivo
		else
		{ // cria configs para todos os
			VerificaTiposRede();
			// cria o tipo "default"
			cfgDiagTrecho = CriaCfgTrechoDefault(0, "*Padrão*");
			// define como padrao
			// diagTrechoPADRAO = diagTrecho;
			// adiciona à lista de configs
			listCfg->Add(cfgDiagTrecho);
			temPadrao = true;
			// e cria o arquivo
			SalvaArquivo();
		}
		if (!temPadrao)
		{ // cria o tipo "default"
			cfgDiagTrecho = CriaCfgTrechoDefault(0, "*Padrão*");
			// define como padrao
			// diagTrechoPADRAO = diagTrecho;
			// adiciona à lista de configs
			listCfg->Add(cfgDiagTrecho);
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
void __fastcall TDiagTrecho::LeCheckList(TCheckListBox *checkListBox)
{
	this->OpcaoDiagCompri = checkListBox->Checked[opTchCOMP];
	this->OpcaoDiagIadm = checkListBox->Checked[opTchIADM];
	this->OpcaoDiagTensoes = checkListBox->Checked[opTchTENSOES];
	this->OpcaoDiagZseq = checkListBox->Checked[opTchZSEQ];
}

// ---------------------------------------------------------------------------
void __fastcall TDiagTrecho::PreencheListView(TListView *listView)
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
	ExtraiStrings(log_trecho->Strings[0], ";", campos);
	// adiciona as colunas
	listColumn = listView->Columns->Add();
	listColumn->Width = 50;
	listColumn->Caption = campos->Strings[colTch_SEGMENTO];
	// listColumn->Width = ColumnHeaderWidth;
	listColumn = listView->Columns->Add();
	listColumn->Width = 50;
	listColumn->Caption = campos->Strings[colTch_REDE];
	// listColumn->Width = ColumnHeaderWidth;
	listColumn = listView->Columns->Add();
	listColumn->Width = 50;
	listColumn->Caption = campos->Strings[colTch_EQPTO];
	// listColumn->Width = ColumnHeaderWidth;
	listColumn = listView->Columns->Add();
	listColumn->Width = 50;
	listColumn->Caption = campos->Strings[colTch_CODIGO];
	// listColumn->Caption = "Código";
	// listColumn->Width = ColumnHeaderWidth;
	listColumn = listView->Columns->Add();
	listColumn->Width = 50;
	listColumn->Caption = campos->Strings[colTch_TIPOERRO];
	// listColumn->Caption = "Tipo Erro";
	// listColumn->Width = ColumnHeaderWidth;
	listColumn = listView->Columns->Add();
	listColumn->Width = 80;
	listColumn->Caption = campos->Strings[colTch_CODBARRA1];
	// listColumn->Caption = "Barra 1";
	// listColumn->Width = ColumnHeaderWidth;
	listColumn = listView->Columns->Add();
	listColumn->Width = 80;
	listColumn->Caption = campos->Strings[colTch_CODBARRA2];
	// listColumn->Caption = "Barra 2";
	// listColumn->Width = ColumnHeaderWidth;
	listColumn = listView->Columns->Add();
	listColumn->Width = 200;
	listColumn->Caption = campos->Strings[colTch_DESCRICAO];
	// listColumn->Caption = "Descrição";
	// listColumn->Width = ColumnHeaderWidth;

	// //coloca valor fixo de width
	// for(int nc =0; nc <listView->Columns->Count; nc++)
	// {
	// listView->Columns->Items[index]->Width = 50;
	// }

	for (int nl = 1; nl < log_trecho->Count; nl++)
	{
		campos->Clear();
		// //Extrai campos da linha
		// ExtraiStrings(log_trecho->Strings[nl], ";", campos);
		// listItem = listView->Items->Add();
		// listItem->Data = log_trecho->Objects[nl];
		// listItem->Caption = campos->Strings[0] ;
		// for(int nc = 1; nc < campos->Count; nc++)
		// {
		// listItem->SubItems->Add(campos->Strings[nc]);
		// }
		listItem = new TListItem(listView->Items);
		// Extrai campos da linha
		ExtraiStrings(log_trecho->Strings[nl], ";", campos);
		listItem->Data = log_trecho->Objects[nl];
		listItem->Caption = campos->Strings[0];
		for (int nc = 1; nc < campos->Count; nc++)
		{
			listItem->SubItems->Add(campos->Strings[nc]);
		}
		listView->Items->AddItem(listItem);
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
long __fastcall TDiagTrecho::PM_GetItensVerificados(void)
{
	if (executado)
	{
		return infoTrechos.n_verif;
	}
	return -1;
}

// ---------------------------------------------------------------------------
TStringList* __fastcall TDiagTrecho::PM_GetLog(void)
{
	if (executado)
	{
		return log_trecho;
	}
	return NULL;
}

// ---------------------------------------------------------------------------
long __fastcall TDiagTrecho::PM_GetOcorrencias(void)
{
	if (executado)
	{
		return infoTrechos.n_ocorrencias;
	}
	else
	{
		return -1;
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TDiagTrecho::SalvaArquivo(void)
{ /* TODO : Decidir se o usuario perde valores guardados pra redes não presentes na base */
	// variaveis locais
	VTInfoset *infoset;
	DIAG_TRECHO *cfgDiagTrecho;
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

	// infoset = NewObjInfoset(apl);
	// infoset = (VTInfoset*)apl->GetObject(__classid(VTInfoset));
	try
	{
		infoset = geral->Infoset;
		infoset->Clear();
		for (int i = 0; i < listCfg->Count; i++)
		{
			cfgDiagTrecho = (DIAG_TRECHO*) listCfg->Items[i];
			infoset->InsertNewLine();
			infoset->AddInfo("tipo_rede", cfgDiagTrecho->tipo_rede);
			infoset->AddInfo("nome_tipo", cfgDiagTrecho->nome_tipo);
			infoset->AddInfo("comp_min_m", cfgDiagTrecho->comp_min_m);
			infoset->AddInfo("comp_max_m", cfgDiagTrecho->comp_max_m);
			infoset->AddInfo("iadm_min_a", cfgDiagTrecho->iadm_min_a);
			infoset->AddInfo("iadm_max_a", cfgDiagTrecho->iadm_max_a);
			// infoset->AddInfo("pu", diagTrecho->pu);
			infoset->AddInfo("seq0_r_max_ohmkm", cfgDiagTrecho->seq0_r_max_ohmkm);
			infoset->AddInfo("seq0_r_min_ohmkm", cfgDiagTrecho->seq0_r_min_ohmkm);
			infoset->AddInfo("seq0_x_max_ohmkm", cfgDiagTrecho->seq0_x_max_ohmkm);
			infoset->AddInfo("seq0_x_min_ohmkm", cfgDiagTrecho->seq0_x_min_ohmkm);
			infoset->AddInfo("seq0_c_max_nFkm", cfgDiagTrecho->seq0_c_max_nFkm);
			infoset->AddInfo("seq0_c_min_nFkm", cfgDiagTrecho->seq0_c_min_nFkm);
			infoset->AddInfo("seq1_r_max_ohmkm", cfgDiagTrecho->seq1_r_max_ohmkm);
			infoset->AddInfo("seq1_r_min_ohmkm", cfgDiagTrecho->seq1_r_min_ohmkm);
			infoset->AddInfo("seq1_x_max_ohmkm", cfgDiagTrecho->seq1_x_max_ohmkm);
			infoset->AddInfo("seq1_x_min_ohmkm", cfgDiagTrecho->seq1_x_min_ohmkm);
			infoset->AddInfo("seq1_c_max_nFkm", cfgDiagTrecho->seq1_c_max_nFkm);
			infoset->AddInfo("seq1_c_min_nFkm", cfgDiagTrecho->seq1_c_min_nFkm);
		}
		infoset->SaveToFile(FILE_CFG_TRECHO);
		return true;
	}
	catch (Exception &e)
	{
		return false;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TDiagTrecho::ShowFormCfg(void)
{
	if (!formDiagTrecho)
	{
		formDiagTrecho = new TFormDiagTrecho(NULL, apl);
		formDiagTrecho->Inicia(this);
	}
	formDiagTrecho->Show();
}

// ---------------------------------------------------------------------------
int __fastcall TDiagTrecho::ShowFormCfgModal(void)
{
	if (!formDiagTrecho)
	{
		formDiagTrecho = new TFormDiagTrecho(NULL, apl);
		formDiagTrecho->Inicia(this);
	}

	return formDiagTrecho->ShowModal();
}

// ---------------------------------------------------------------------------
int __fastcall TDiagTrecho::Tipo(void)
{
	return (dgeTRECHO);
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TDiagTrecho::TipoAsString(void)
{
	return "Trechos";
}

// ---------------------------------------------------------------------------
void __fastcall TDiagTrecho::VerificaTiposRede(void)
{
	VTTipos *tipos = (VTTipos*)apl->GetObject(__classid(VTTipos));
	TList *lisTIPOREDE = tipos->LisTipoRede();
	VTTipoRede *tipo_rede;
	DIAG_TRECHO *cfgDiagTrecho;

	// verifica os tipos de rede cadastrados na base
	// em busca de dos tipos de rede que não possuem cfg no arquivo lido
	for (int i = 0; i < lisTIPOREDE->Count; i++)
	{
		tipo_rede = (VTTipoRede*) lisTIPOREDE->Items[i];
		if (!ExisteCfgTipoRede(tipo_rede->Segmento))
		{ // quando encontra um tipo sem correspondente, cria um "default"
			cfgDiagTrecho = CriaCfgTrechoDefault(tipo_rede->Segmento, tipo_rede->Codigo);
			// adiciona à lista de configs
			listCfg->Add(cfgDiagTrecho);
		}
		else
		{
			// faz nada
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TDiagTrecho::VerificaTrecho(VTTrecho *trecho)
{
	VTRede *rede;
	int tipo_rede;

	DIAG_TRECHO *cfgDiagTrecho;

	// verifica que tipo de rede
	rede = trecho->rede;
	// caso o trecho não tenha rede associada, a verificação sera com o Padrao
	if (!rede)
	{
		tipo_rede = 0;
	}
	// caso exista, associa ao tipo de rede da rede associada ao trecho
	else
	{
		tipo_rede = rede->TipoRede->Segmento;
	}

	if ((cfgDiagTrecho = Get_DiagTrecho(tipo_rede)) != NULL)
	{
		// verifica se o comprimento do trecho esta dentro dos limites
		if (dgTrecho.comp)
		{
			VerificaOpComp(trecho, cfgDiagTrecho);
		}
		// verifica se a corrente admissivel do trecho esta dentro dos limites
		if (dgTrecho.iAdm)
		{
			VerificaOpIAdm(trecho, cfgDiagTrecho);
		}
		// verifica se a impedancia de sequencia do trecho esta dentro dos limites
		if (dgTrecho.zSeq)
		{
			VerificaOpZSeq(trecho, cfgDiagTrecho);
		}
		// incrementa o contador de trechos verificados
		infoTrechos.n_verif++;
	}
	// verifica se as tensoes das barras do trecho são iguais
	if (dgTrecho.tensoes)
	{
		VerificaOpTensoes(trecho);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TDiagTrecho::VerificaOpComp(VTTrecho *trecho, DIAG_TRECHO *cfgDiagTrecho)
{
	if (VerificaLimites(trecho->Comprimento_m, cfgDiagTrecho->comp_min_m,
		cfgDiagTrecho->comp_max_m))
	{
		AddLinhaLog(trecho, opTchCOMP);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TDiagTrecho::VerificaOpIAdm(VTTrecho *trecho, DIAG_TRECHO *cfgDiagTrecho)
{
	if (!trecho->ArranjoTipico)
	{
		if (VerificaLimites(trecho->Iadm_amp[3], cfgDiagTrecho->iadm_min_a,
			cfgDiagTrecho->iadm_max_a))
		{
			AddLinhaLog(trecho, opTchIADM);
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TDiagTrecho::VerificaOpTensoes(VTTrecho *trecho)
{ // Variaveis locais
	VTBarra *barra1, *barra2;

	barra1 = trecho->pbarra1;
	barra2 = trecho->pbarra2;
	if (barra1->vnom != barra2->vnom)
	{
		AddLinhaLog(trecho, opTchTENSOES);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TDiagTrecho::VerificaOpZSeq(VTTrecho *trecho, DIAG_TRECHO *cfgDiagTrecho)
{
	bool z_invalido = false;
	double z0r_ohmkm, z0x_ohmkm, z0c_nFkm;
	double z1r_ohmkm, z1x_ohmkm, z1c_nFkm;

	// proteção
	if (!trecho || !cfgDiagTrecho)
		return;
	// verifica se é arranjo tipico
	if (!trecho->ArranjoTipico)
	{
		trecho->Z0_ohm(z0r_ohmkm, z0x_ohmkm, z0c_nFkm);
		trecho->Z1_ohm(z1r_ohmkm, z1x_ohmkm, z1c_nFkm);
		if (VerificaLimites(z0r_ohmkm, cfgDiagTrecho->seq0_r_min_ohmkm,
			cfgDiagTrecho->seq0_r_max_ohmkm))
		{
			z_invalido = true;
		}
		if (VerificaLimites(z0x_ohmkm, cfgDiagTrecho->seq0_x_min_ohmkm,
			cfgDiagTrecho->seq0_x_max_ohmkm))
		{
			z_invalido = true;
		}
		if (VerificaLimites(z0c_nFkm, cfgDiagTrecho->seq0_c_min_nFkm,
			cfgDiagTrecho->seq0_c_max_nFkm))
		{
			z_invalido = true;
		}
		if (VerificaLimites(z1r_ohmkm, cfgDiagTrecho->seq1_r_min_ohmkm,
			cfgDiagTrecho->seq1_r_max_ohmkm))
		{
			z_invalido = true;
		}
		if (VerificaLimites(z1x_ohmkm, cfgDiagTrecho->seq1_x_min_ohmkm,
			cfgDiagTrecho->seq1_x_max_ohmkm))
		{
			z_invalido = true;
		}
		if (VerificaLimites(z1c_nFkm, cfgDiagTrecho->seq1_c_min_nFkm,
			cfgDiagTrecho->seq1_c_max_nFkm))
		{
			z_invalido = true;
		}
		// caso apresente algum erro, adiciona ao log
		if (z_invalido)
			AddLinhaLog(trecho, opTchZSEQ);
	}
	else
	{ /* não precisa verificar */ }
}

// ---------------------------------------------------------------------------
