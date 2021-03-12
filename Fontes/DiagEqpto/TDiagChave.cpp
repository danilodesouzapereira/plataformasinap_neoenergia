// ---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
#include <math.h>
#include "..\Apl\VTApl.h"
#include "..\Geral\VTInfoset.h"
#include "..\Geral\VTGeral.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTTipoChave.h"
#include "..\Rede\VTTipos.h"
#include "..\Rede\VTTipoRede.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "TDiagChave.h"
#include "TFormDiagChave.h"
// ---------------------------------------------------------------------------
#include "..\..\DLL_Inc\Cronometro.h"
#include "..\Cronometro\VTCronometro.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
// ---------------------------------------------------------------------------
// define do nome do arquivo
#define FILE_CFG_CHAVE "CfgDiagEqptoChave"

// ---------------------------------------------------------------------------
// VTDiagEqptoElem* __fastcall NewObjDiagEqptoElem(VTApl *apl)
// {
// return (new TDiagEqptoChave(apl));
// }
// ---------------------------------------------------------------------------
__fastcall TDiagChave::TDiagChave(VTApl *apl)
{
	this->apl = apl;
	// Mostra que ainda não foi executado
	executado = false;
	// define se tem informações adicionais
	infoExtra = false;
	// cria a lista de configuracoes
	listCfg = new TList;
	// inicializa as opcoes do diagnostico
	dgChave.comp = true;
	dgChave.iNom = true;
	dgChave.tipo = true;
	dgChave.rede = true;
	dgChave.tensoes = true;
	// dgChave.isolado  = true;
	// cria o log_chave
	/* TODO : mudar para o executa? */
	log_chave = new TStringList;

}

// ---------------------------------------------------------------------------
__fastcall TDiagChave::~TDiagChave(void)
{
	// destroi lista e conteudo
	// if(LisCfgDiagChave){ LimpaTList(LisCfgDiagChave); delete LisCfgDiagChave; LisCfgDiagChave = NULL;}
	if (listCfg)
	{
		LimpaTList(listCfg);
		delete listCfg;
		listCfg = NULL;
	}
	// destroi log
	if (log_chave)
	{
		delete log_chave;
		log_chave = NULL;
	}
	// destroi form
	if (formDiagChave)
	{
		delete formDiagChave;
		formDiagChave = NULL;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TDiagChave::AddLinhaLog(VTChave *chave, int opDIAG_CHAVE)
{
	// variáveis locais
	VTTipoRede *tiporede = NULL;

	// incrementa o contador de erros
	// infoChaves.n_ocorrencias++;
	temErro = true;
	// vetor de strings seguindo o enum col_LOG_CHAVE
	AnsiString linhaPreenchida = "";
	AnsiString linha[colChv_COUNT];
	AnsiString Separador = ";";
	// inicializa com os valores vindos do trecho

	if (chave->rede)
	{
		tiporede = chave->rede->TipoRede;
		if (tiporede)
		{
			linha[colChv_SEGMENTO] = chave->rede->TipoRede->Codigo + Separador;
		}
		else
		{
			linha[colChv_SEGMENTO] = "Nenhum" + Separador;
		}
		linha[colChv_REDE] = chave->rede->Codigo + Separador;
	}
	else
	{
		linha[colChv_REDE] = "Nenhum";
		linha[colChv_SEGMENTO] = "Nenhum";
	}
	linha[colChv_EQPTO] = "Chave" + Separador;
	linha[colChv_CODIGO] = chave->Codigo + Separador;
	linha[colChv_CODBARRA1] = chave->pbarra1->Codigo + Separador;
	linha[colChv_CODBARRA2] = chave->pbarra2->Codigo + Separador;

	if (chave)
	{
		switch (opDIAG_CHAVE)
		{
		case opChvREDE:
			linha[colChv_TIPOERRO] = "Aviso" + Separador;
			linha[colChv_DESCRICAO] = "Chave não associada à uma rede.";
			break;

		case opChvCOMP:
			linha[colChv_TIPOERRO] = "Aviso" + Separador;
			linha[colChv_DESCRICAO] = "Distância entre as barras de valor elevado";
			break;

		case opChvINOM:
			linha[colChv_TIPOERRO] = "Aviso" + Separador;
			linha[colChv_DESCRICAO] = "Corrente nominal fora dos limites para o tipo de chave.";
			break;

			// case opChvISO:
			// linha[colChv_TIPOERRO]  = "Aviso" + Separador;
			// linha[colChv_DESCRICAO] = "Chave isolada.";
			// break;

		case opChvTIPO:
			linha[colChv_TIPOERRO] = "Aviso" + Separador;
			linha[colChv_DESCRICAO] = "Chave com tipo indefinido.";
			break;

		case opChvTENSOES:
			linha[colChv_TIPOERRO] = str_TIPOERRO_ERRO + Separador;
			linha[colChv_DESCRICAO] = "Tensões nominais das barras são diferentes.";
			break;

		default: ;
		}
		// concatena os campos em uma linha
		for (int i = 0; i < colChv_COUNT; i++)
		{
			linhaPreenchida += linha[i];
		}
		log_chave->AddObject(linhaPreenchida, chave);
	}
}

// ---------------------------------------------------------------------------
DIAG_CHAVE* __fastcall TDiagChave::CriaCfgChaveDefault(int chaveTIPO, AnsiString nomeTipo)
{
	DIAG_CHAVE *diagChave;

	diagChave = new DIAG_CHAVE;

	diagChave->tipo_chave = chaveTIPO;
	diagChave->nome_tipo = nomeTipo;
	diagChave->inom_min_a = 0;
	diagChave->inom_max_a = 400;
	diagChave->comp_max_m = 5;

	return diagChave;
}

// ---------------------------------------------------------------------------
bool __fastcall TDiagChave::Executa(void)
{ // variaveis locais
	VTRedes *redes = (VTRedes*) apl->GetObject(__classid(VTRedes));
	// VTRede      *rede;
	VTChave *chave;
	TList *lisChaves = new TList;

	// inicialza infos
	infoChaves.n_total = 0;
	infoChaves.n_verif = 0;
	infoChaves.n_ocorrencias = 0;
	// inicializa contagem de metros de chaves
	infoChaves.m = 0;
	if (log_chave)
		IniciaLog(log_chave);
	try
	{ // pega a lista de todas as chaves das redes carregadas
		redes->LisLigacao(lisChaves, eqptoCHAVE);
		// guarda qntas chaves devem ser verificadas
		infoChaves.n_total = lisChaves->Count;
		for (int nc = 0; nc < lisChaves->Count; nc++)
		{
			chave = (VTChave*) lisChaves->Items[nc];
			temErro = false;
			VerificaChave(chave);
			if (temErro)
				infoChaves.n_ocorrencias++;
		}
		// altera o boolean mostrando que foi executado
		executado = true;
		// Aviso("existe " + DoubleToStr("%4.3f", km) + " m em chaves");
	}
	catch (Exception &e)
	{
		return false;
	}

	return true;
}

// ---------------------------------------------------------------------------
bool __fastcall TDiagChave::ExisteCfgTipoChave(int chaveTIPO)
{ // variaveis locais
	DIAG_CHAVE *diagChave;

	// for(int nt = 0; nt < LisCfgDiagChave->Count; nt++)
	// {
	// diagChave = (DIAG_CHAVE*) LisCfgDiagChave->Items[nt];
	// if (diagChave->tipo_chave == chaveTIPO) {return true;}
	// }
	for (int nt = 0; nt < listCfg->Count; nt++)
	{
		diagChave = (DIAG_CHAVE*) listCfg->Items[nt];
		if (diagChave->tipo_chave == chaveTIPO)
		{
			return true;
		}
	}
	return false;
}

// ---------------------------------------------------------------------------
DIAG_CHAVE* __fastcall TDiagChave::Get_DiagChave(int chaveTIPO)
{ // variaveis locais
	DIAG_CHAVE *diagChave;

	// for(int i = 0 ; i < LisCfgDiagChave->Count; i++)
	// {
	// diagChave = (DIAG_CHAVE*) LisCfgDiagChave->Items[i];
	// if(chaveTIPO == diagChave->tipo_chave){return diagChave;}
	// }
	for (int i = 0; i < listCfg->Count; i++)
	{
		diagChave = (DIAG_CHAVE*) listCfg->Items[i];
		if (chaveTIPO == diagChave->tipo_chave)
		{
			return diagChave;
		}
	}

	return NULL;
}

// ---------------------------------------------------------------------------
void __fastcall TDiagChave::Inicia(void)
{
	// primeiramente carrega as configs do arquivo
	LeArquivo();
	// verifica se falta configs pros tipos de rede existentes
	VerificaTiposChave();
	//
}

// ---------------------------------------------------------------------------
void __fastcall TDiagChave::IniciaLog(TStringList *log)
{ // variaveis locais
	AnsiString Separador = ";";
	AnsiString linha[colChv_COUNT];
	AnsiString linhaPreenchida = "";
	if (log)
	{
		// limpa o log
		log->Clear();

		linha[colChv_SEGMENTO] = "Segmento" + Separador;
		linha[colChv_REDE] = "Rede" + Separador;
		linha[colChv_EQPTO] = "Equipamento" + Separador;
		linha[colChv_CODIGO] = "Código" + Separador;
		linha[colChv_TIPOERRO] = "Tipo Erro" + Separador;
		linha[colChv_CODBARRA1] = "Barra 1" + Separador;
		linha[colChv_CODBARRA2] = "Barra 2" + Separador;
		linha[colChv_DESCRICAO] = "Descrição";
		// concatena os campos em uma linha
		for (int i = 0; i < colChv_COUNT; i++)
		{
			linhaPreenchida += linha[i];
		}
		log->Add(linhaPreenchida);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TDiagChave::IniciaCheckList(TCheckListBox *checkListBox)
{
	checkListBox->Clear();

	checkListBox->Items->Add(op_CHV_TEXT[opChvREDE]);
	checkListBox->Checked[opChvREDE] = this->OpcaoDiagRede;

	checkListBox->Items->Add(op_CHV_TEXT[opChvCOMP]);
	checkListBox->Checked[opChvCOMP] = this->OpcaoDiagCompri;

	checkListBox->Items->Add(op_CHV_TEXT[opChvINOM]);
	checkListBox->Checked[opChvINOM] = this->OpcaoDiagInom;

	checkListBox->Items->Add(op_CHV_TEXT[opChvTIPO]);
	checkListBox->Checked[opChvTIPO] = this->OpcaoDiagTipo;

	checkListBox->Items->Add(op_CHV_TEXT[opChvTENSOES]);
	checkListBox->Checked[opChvTENSOES] = this->OpcaoDiagTensoes;

	// checkListBox->Items->Add(op_CHV_TEXT[opChvISO]);
	// checkListBox->Checked[opChvISO] = this->OpcaoDiagIsolado;
}

// ---------------------------------------------------------------------------
bool __fastcall TDiagChave::LeArquivo(void)
{ // variaveis locais
	VTInfoset *infoset;
	DIAG_CHAVE *diagChave;
	VTTipos *tipos = (VTTipos*)apl->GetObject(__classid(VTTipos));
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
	int tipoChave;
	bool temPadrao = false;

	try
	{
		// cria novo infoset para ler o arquivo de cfg
		infoset = geral->Infoset;
		infoset->Clear();
		// verifica se é possivel ler o arquivo
		if (infoset->LoadFromFile(FILE_CFG_CHAVE))
		{ // varre todo o arquivo lido
			for (int i = 0; i < infoset->Count; i++)
			{
				infoset->SelectLine(i);
				// verifica qual é o tipo rede  (redeSEGMENTO)
				infoset->GetInfo("tipo_chave", tipoChave);
				///verifica se o tipo a ser carregado do arquivo existe na base
				// ou se é o tipo "default"
				if ((tipos->ExisteTipoRede(tipoChave)) || (tipoChave == 0))
				{ // caso exista, cria-se um novo objeto
					diagChave = new DIAG_CHAVE;
					infoset->GetInfo("tipo_chave", diagChave->tipo_chave);
					infoset->GetInfo("nome_tipo", diagChave->nome_tipo);
					infoset->GetInfo("inom_min_a", diagChave->inom_min_a);
					infoset->GetInfo("inom_max_a", diagChave->inom_max_a);
					infoset->GetInfo("comp_max_m", diagChave->comp_max_m);
					// LisCfgDiagChave->Add(diagChave);
					listCfg->Add(diagChave);
					// caso o tipo rede seja 0 (== PADRAO)
					if (tipoChave == 0)
					{
						temPadrao = true;
					}
				}
			}
		} // caso não exista o arquivo
		else
		{ // cria configs para todos os
			VerificaTiposChave();
			// cria o tipo "default"
			diagChave = CriaCfgChaveDefault(0, "*Padrão*");
			// define como padrao
			// diagTrechoPADRAO = diagTrecho;
			// adiciona à lista de configs
			// LisCfgDiagChave->Add(diagChave);
			listCfg->Add(diagChave);
			temPadrao = true;
			// e cria o arquivo
			SalvaArquivo();
		}
		if (!temPadrao)
		{ // cria o tipo "default"
			diagChave = CriaCfgChaveDefault(0, "*Padrão*");
			// define como padrao
			// diagTrechoPADRAO = diagTrecho;
			// adiciona à lista de configs
			// LisCfgDiagChave->Add(diagChave);
			listCfg->Add(diagChave);
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
void __fastcall TDiagChave::LeCheckList(TCheckListBox *checkListBox)
{
	this->OpcaoDiagCompri = checkListBox->Checked[opChvCOMP];
	this->OpcaoDiagInom = checkListBox->Checked[opChvINOM];
	this->OpcaoDiagTipo = checkListBox->Checked[opChvTIPO];
	this->OpcaoDiagRede = checkListBox->Checked[opChvREDE];
	this->OpcaoDiagTensoes = checkListBox->Checked[opChvTENSOES];
	// this->OpcaoDiagIsolado  = checkListBox->Checked[opChvISO];
}

// ---------------------------------------------------------------------------
long __fastcall TDiagChave::PM_GetItensVerificados(void)
{
	if (executado)
	{
		return infoChaves.n_verif;
	}
	return -1;
}

// ---------------------------------------------------------------------------
TStringList* __fastcall TDiagChave::PM_GetLog(void)
{
	if (executado)
	{
		return log_chave;
	}
	return NULL;
}

// ---------------------------------------------------------------------------
long __fastcall TDiagChave::PM_GetOcorrencias(void)
{
	if (executado)
	{
		return infoChaves.n_ocorrencias;
	}
	else
	{
		return -1;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TDiagChave::PreencheListView(TListView *listView)
{ // variaveis locais
	TListItem *listItem;
	// TListItems *listaListItems;
	TListColumn *listColumn;
	TStringList *campos = new TStringList();
	// VTCronometro *timer = DLL_NewObjCronometro(apl);
	AnsiString teste;

	// tentar melhorar performance do listview
	listView->Items->BeginUpdate();
	// limpa o listView
	// timer->Start("clear");
	listView->Clear();
	listView->Columns->Clear();
	campos->Clear();
	// timer->Stop("clear");
	// Extrai campos da linha
	ExtraiStrings(log_chave->Strings[0], ";", campos);
	// listaListItems = new TTListItems[log_chave->Count];
	// timer->Start("Tempo colunas");
	// adiciona as colunas
	listColumn = listView->Columns->Add();
	listColumn->Width = 70;
	// listColumn = listView->Column[colChv_SEGMENTO];
	listColumn->Caption = campos->Strings[colChv_SEGMENTO];
	// listColumn->Caption = "Segmento";
	// listColumn->Width = ColumnHeaderWidth;
	listColumn = listView->Columns->Add();
	listColumn->Width = 60;
	// listColumn = listView->Column[colChv_REDE];
	listColumn->Caption = campos->Strings[colChv_REDE];
	// listColumn->Caption = "Rede";
	// listColumn->Width = ColumnHeaderWidth;
	listColumn = listView->Columns->Add();
	listColumn->Width = 80;
	// listColumn = listView->Column[colChv_EQPTO];
	listColumn->Caption = campos->Strings[colChv_EQPTO];
	// listColumn->Caption = "Eqpto";
	// listColumn->Width = ColumnHeaderWidth;
	listColumn = listView->Columns->Add();
	listColumn->Width = 50;
	// listColumn = listView->Column[colChv_CODIGO];
	listColumn->Caption = campos->Strings[colChv_CODIGO];
	// listColumn->Caption = "Código";
	// listColumn->Width = ColumnHeaderWidth;
	listColumn = listView->Columns->Add();
	listColumn->Width = 60;
	// listColumn = listView->Column[colChv_TIPOERRO];
	listColumn->Caption = campos->Strings[colChv_TIPOERRO];
	// listColumn->Caption = "Tipo Erro";
	// listColumn->Width = ColumnHeaderWidth;
	listColumn = listView->Columns->Add();
	listColumn->Width = 50;
	// listColumn = listView->Column[colChv_CODBARRA1];
	listColumn->Caption = campos->Strings[colChv_CODBARRA1];
	// listColumn->Caption = "Barra 1";
	// listColumn->Width = ColumnHeaderWidth;
	listColumn = listView->Columns->Add();
	listColumn->Width = 50;
	// listColumn = listView->Column[colChv_CODBARRA2];
	listColumn->Caption = campos->Strings[colChv_CODBARRA2];
	// listColumn->Caption = "Barra 2";
	// listColumn->Width = ColumnHeaderWidth;
	listColumn = listView->Columns->Add();
	listColumn->Width = 250;
	// listColumn = listView->Column[colChv_DESCRICAO];
	listColumn->Caption = campos->Strings[colChv_DESCRICAO];
	// listColumn->Caption = "Descrição";
	// listColumn->Width = ColumnHeaderWidth;
	// timer->Stop("Tempo colunas");

	// //coloca valor fixo de width
	// for(int nc =0; nc <listView->Columns->Count; nc++)
	// {
	// listView->Columns->Items[index]->Width = 50;
	// }

	// timer->Start("items");
	for (int nl = 1; nl < log_chave->Count; nl++)
	{
		campos->Clear();
		// Extrai campos da linha
		// timer->Start("ExtraiS " +IntToStr(nl));
		ExtraiStrings(log_chave->Strings[nl], ";", campos);
		// timer->Stop("ExtraiS " +IntToStr(nl));
		// timer->Start("Add " +IntToStr(nl));
		listItem = listView->Items->Add();
		// timer->Stop("Add " +IntToStr(nl));
		// timer->Start("Add teste" +IntToStr(nl));
		teste = campos->Strings[0];
		// timer->Stop("Add teste" +IntToStr(nl));
		// timer->Start("Add Caption " +IntToStr(nl));
		listItem->Caption = teste;
		// timer->Stop("Add Caption " +IntToStr(nl));
		// timer->Start("Add Object " +IntToStr(nl));
		listItem->Data = log_chave->Objects[nl];
		// timer->Stop("Add Object " +IntToStr(nl));
		// listItem->Caption = IntToStr(nl);
		for (int nc = 1; nc < campos->Count; nc++)
		{
			// timer->Start("Add Sub " +IntToStr(nl) +" " +IntToStr(nc));
			listItem->SubItems->Add(campos->Strings[nc]);
			// timer->Stop("Add Sub " +IntToStr(nl) +" " +IntToStr(nc));
		}

		// listItem = new TListItem(listView->Items);
		// //Extrai campos da linha
		// ExtraiStrings(log_chave->Strings[nl], ";", campos);
		// listItem->Data = log_chave->Objects[nl];
		// listItem->Caption = campos->Strings[0] ;
		// for(int nc = 1; nc < campos->Count; nc++)
		// {
		// listItem->SubItems->Add(campos->Strings[nc]);
		// }
		// listView->Items->AddItem(listItem);
	}
	// timer->Stop("items");
	// destroi o TStrings
	if (campos)
	{
		delete campos;
		campos = NULL;
	}
	// tentar melhorar performance do listview
	listView->Items->EndUpdate();

	// timer->Show();
	// if(timer){delete timer; timer = NULL;}
}

// ---------------------------------------------------------------------------
bool __fastcall TDiagChave::SalvaArquivo(void)
{ /* TODO : Decidir se o usuario perde valores guardados pra redes não presentes na base */
	// variaveis locais
	VTInfoset *infoset;
	DIAG_CHAVE *diagChave;
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

	try
	{
		infoset = geral->Infoset;
		infoset->Clear();
		// for (int i = 0; i < LisCfgDiagChave->Count; i++)
		// {
		// diagChave = (DIAG_CHAVE*) LisCfgDiagChave->Items[i];
		// infoset->InsertNewLine();
		// infoset->AddInfo("tipo_chave", diagChave->tipo_chave);
		// infoset->AddInfo("nome_tipo",  diagChave->nome_tipo);
		// infoset->AddInfo("inom_min_a", diagChave->inom_min_a);
		// infoset->AddInfo("inom_max_a", diagChave->inom_max_a);
		// infoset->AddInfo("comp_max_m", diagChave->comp_max_m);
		// }
		for (int i = 0; i < listCfg->Count; i++)
		{
			diagChave = (DIAG_CHAVE*) listCfg->Items[i];
			infoset->InsertNewLine();
			infoset->AddInfo("tipo_chave", diagChave->tipo_chave);
			infoset->AddInfo("nome_tipo", diagChave->nome_tipo);
			infoset->AddInfo("inom_min_a", diagChave->inom_min_a);
			infoset->AddInfo("inom_max_a", diagChave->inom_max_a);
			infoset->AddInfo("comp_max_m", diagChave->comp_max_m);
		}
		infoset->SaveToFile(FILE_CFG_CHAVE);
		return true;
	}
	catch (Exception &e)
	{
		return false;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TDiagChave::ShowFormCfg(void)
{
	if (formDiagChave)
		formDiagChave->Show();
	else
	{
		formDiagChave = new TFormDiagChave(NULL, apl);
		formDiagChave->Inicia(this);
		formDiagChave->Show();
	}
}

// ---------------------------------------------------------------------------
int __fastcall TDiagChave::ShowFormCfgModal(void)
{
	if (!formDiagChave)
	{
		formDiagChave = new TFormDiagChave(NULL, apl);
		formDiagChave->Inicia(this);
	}

	return formDiagChave->ShowModal();
}

// ---------------------------------------------------------------------------
int __fastcall TDiagChave::Tipo(void)
{
	return (dgeCHAVE);
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TDiagChave::TipoAsString(void)
{
	return "Chaves";
}

// ---------------------------------------------------------------------------
void __fastcall TDiagChave::VerificaTiposChave(void)
{
	VTTipos *tipos = (VTTipos*)apl->GetObject(__classid(VTTipos));
	TList *lisTIPO = tipos->LisTipoChave();
	VTTipoChave *tipo_chave;
	DIAG_CHAVE *diagChave;

	// verifica os tipos de rede cadastrados na base
	// em busca de dos tipos de rede que não possuem cfg no arquivo lido
	for (int i = 0; i < lisTIPO->Count; i++)
	{
		tipo_chave = (VTTipoChave*) lisTIPO->Items[i];
		if (!ExisteCfgTipoChave(tipo_chave->Tipo))
		{ // quando encontra um tipo sem correspondente, cria um "default"
			diagChave = CriaCfgChaveDefault(tipo_chave->Tipo, tipo_chave->Codigo);
			// adiciona à lista de configs
			// LisCfgDiagChave->Add(diagChave);
			listCfg->Add(diagChave);
		}
		else
		{
			// faz nada
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TDiagChave::VerificaChave(VTChave *chave)
{
	VTTipoChave *tipoChave;
	int tipo_chave;
	DIAG_CHAVE *diagChave;

	// verifica que tipo de chave
	tipoChave = chave->TipoChave;
	// caso o trecho não tenha tipo associada, a verificação sera com o Padrao
	if (!tipoChave)
	{
		tipo_chave = 0;
	}
	// caso exista, associa ao tipo de rede da rede associada ao trecho
	else
	{
		tipo_chave = chave->TipoChave->Tipo;
	}
	if ((diagChave = Get_DiagChave(tipo_chave)) != NULL)
	{
		// verifica se o "comprimento" da chave esta dentro dos limites
		if (dgChave.comp)
		{
			VerificaOpComp(chave, diagChave);
		}
		// verifica se a corrente nominal não é nula
		if (dgChave.iNom)
		{
			VerificaOpINom(chave, diagChave);
		}
	}
	// verifica se o tipo é indefinido
	if (dgChave.tipo)
	{
		VerificaOpTipo(chave);
	}
	// verifica se as tensões das barras são iguais
	if (dgChave.tensoes)
	{
		VerificaOpTensoes(chave);
	}
	// verifica se não está associada à rede
	if (dgChave.rede)
	{
		VerificaOpRede(chave);
	}
	// verifica se está isolada
	// if (dgChave.isolado) {VerificaOpIsolado(chave);}
	// incrementa o contador de verificacoes
	infoChaves.n_verif++;
}

// ---------------------------------------------------------------------------
void __fastcall TDiagChave::VerificaOpComp(VTChave *chave, DIAG_CHAVE *diagChave)
{ // variaveis locais
	VTBarra *barra1, *barra2;
	double distancia, x2, y2;

	barra1 = chave->pbarra1;
	barra2 = chave->pbarra2;

	// calcula a distancia entre as barras
	x2 = pow(((long double)barra1->utm.x - barra2->utm.x), 2);
	y2 = pow(((long double)barra1->utm.y - barra2->utm.y), 2);
	distancia = sqrt(x2 + y2) / 100; // ficar em metros
	infoChaves.m = infoChaves.m + distancia;
	// se a distancia for maior que o definido, vai pro log
	if (distancia > diagChave->comp_max_m)
		AddLinhaLog(chave, opChvCOMP);
}

// ---------------------------------------------------------------------------
void __fastcall TDiagChave::VerificaOpINom(VTChave *chave, DIAG_CHAVE *diagChave)
{
	if (VerificaLimites(chave->Inom_amp, diagChave->inom_min_a, diagChave->inom_max_a))
	{
		AddLinhaLog(chave, opChvINOM);
	}
}

// ---------------------------------------------------------------------------
// void  __fastcall  TDiagChave::VerificaOpIsolado(VTChave *chave)
// {  //variaveis locais
// VTBarra *barra1, *barra2;
//
// barra1 = chave->pbarra1;
// barra2 = chave->pbarra2;
//
// if((barra1->zona == NULL) && (barra2->zona == NULL))
// {
// AddLinhaLog(chave, opChvISO);
// }
// }

// ---------------------------------------------------------------------------
void __fastcall TDiagChave::VerificaOpTipo(VTChave *chave)
{
	if (chave->Tipo() == chaveINDEFINIDA)
	{
		AddLinhaLog(chave, opChvTIPO);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TDiagChave::VerificaOpRede(VTChave *chave)
{
	if (chave->rede == NULL)
	{
		AddLinhaLog(chave, opChvREDE);
	}
	else if (chave->rede->TipoRede == NULL)
	{
		AddLinhaLog(chave, opChvREDE);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TDiagChave::VerificaOpTensoes(VTChave *chave)
{ // variaveis locais
	VTBarra *barra1, *barra2;

	barra1 = chave->pbarra1;
	barra2 = chave->pbarra2;
	if (barra1->vnom != barra2->vnom)
	{
		AddLinhaLog(chave, opChvTENSOES);
	}
}

// ---------------------------------------------------------------------------
