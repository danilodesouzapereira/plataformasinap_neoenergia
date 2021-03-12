// ---------------------------------------------------------------------------
#pragma hdrstop
#include "TRecondutorar.h"
#include "..\..\..\DLL_Inc\Funcao.h"
#include "..\..\..\DLL_Inc\Radial.h"
#include "..\..\Apl\VTApl.h"
#include "..\..\Arranjo\VTArranjo.h"
#include "..\..\Edita\VTEdita.h"
#include "..\..\Flow\VTFlow.h"
#include "..\..\Patamar\VTPatamar.h"
#include "..\..\Radial\VTArvore.h"
#include "..\..\Radial\VTNode.h"
#include "..\..\Radial\VTPrimario.h"
#include "..\..\Radial\VTRadial.h"
#include "..\..\Radial\VTTronco.h"
#include "..\..\Rede\VTLigacao.h"
#include "..\..\Rede\VTRede.h"
#include "..\..\Rede\VTRedes.h"
#include "..\..\Rede\VTResFlowLig.h"
#include "..\..\Rede\VTTrecho.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/14217067
__fastcall TRecondutorar::TRecondutorar(VTApl *apl, TList *lisEQP)
{
	// guarda ponteiros
	this->apl = apl;
	this->lisEQP = lisEQP;
	// cria lista
	lisBAR = new TList();
	lisLIG = new TList();
	//lisNODE_JUS = new TList();
	// cria obj arvore
	arvore = DLL_NewObjArvore(apl);
	// cria obj radial e inicia
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	radial = DLL_NewObjRadial(apl);
	radial->Inicia(redes);
	// cria tronco
	tronco = DLL_NewObjTronco(apl);
}

// ---------------------------------------------------------------------------
__fastcall TRecondutorar::~TRecondutorar(void)
{
	// destroi lista sem destruir objetos
	DestroiTObject(lisBAR);
	DestroiTObject(lisLIG);
	//DestroiTObject(lisNODE_JUS);
	// destroi objetos
	arvore->Clear();
	DestroiTObject(arvore);
	DestroiTObject(radial);
	DestroiTObject(tronco);
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TRecondutorar::ComprimentoSel(void)
{
	// variaveis locais
	AnsiString str_comp_km;
	double comp_km = 0.;
	VTTrecho *trecho;

	for (int n = 0; n < lisEQP->Count; n++)
	{ // acumula comprimento dos trechos selecionados
		trecho = (VTTrecho*)lisEQP->Items[n];
		comp_km += trecho->Comprimento_km;
	}
	str_comp_km = DoubleToStr("%4.3f", comp_km, ',');
	return str_comp_km;
}

// ---------------------------------------------------------------------------
void __fastcall TRecondutorar::CopiaTrechos(TList *lisLIG)
{
	// variaveis locais
	VTLigacao *ligacao;

	for (int n = 0; n < lisLIG->Count; n++)
	{ // insere somente trechos na lisEQP
		ligacao = (VTLigacao*)lisLIG->Items[n];
		if (ligacao->Tipo() == eqptoTRECHO)
			lisEQP->Add(ligacao);
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TRecondutorar::Executa(VTArranjo *arranjo)
{
	// variaveis locais
	bool sucesso = false;
	VTTrecho *trecho, *clone;
	VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));
	TList *lisALT;

	// proteção
	if (lisEQP->Count == 0)
	{
		Aviso("Selecione trechos para recondutoramento");
		return false;
	}
	// cria lista de eqptos alterados
	lisALT = new TList();
	for (int n = 0; n < lisEQP->Count; n++)
	{
		trecho = (VTTrecho*)lisEQP->Items[n];
		// verifica se precisa trocar o arranjo
		if (trecho->arranjo == arranjo)
			continue;
		// clona trecho e troca arranjo do clone
		clone = trecho->Clone();
		clone->arranjo = arranjo;
		trecho->Obj = clone;
		// insere trecho na lista de eqptos alterados
		lisALT->Add(trecho);
	}
	// altera todos os trechos de uma vez
	sucesso = edita->AlteraLisEqpto(lisALT);
	// destroi lista
	delete lisALT;
	return sucesso;
}

// ---------------------------------------------------------------------------
VTEqpto* __fastcall TRecondutorar::PM_GetEqpto(void)
{
	return eqpto;
}

// ---------------------------------------------------------------------------
void __fastcall TRecondutorar::PM_SetEqpto(VTEqpto* eqpto)
{
	// variaveis locais
	VTLigacao *liga_old, *liga_new;

	// proteção
	if(eqpto == NULL)
	{
		this->eqpto = eqpto;
		return;
    }
	if (this->eqpto == NULL)
	{ // salva ponteiro
		this->eqpto = eqpto;
		return;
	}
	// verifica se sao de redes diferentes
	liga_old = (VTLigacao*)this->eqpto;
	liga_new = (VTLigacao*)eqpto;
	if (liga_old->rede != liga_new->rede)
	{ // verifica se a opção opJUSANTE ou opT_ARRANJO foi selecionada
		if ((opcao == opJUSANTE) || (opcao == opT_ARRANJO))
		{ // remonta arvore
			arvore->Clear();
			arvore->Inicia(lisLIG, liga_new->rede->BarraInicial());
		}
		// verifica se a opção com fluxo foi selecionada
		if ((opcao == opT_RADIAL) || (opcao == opT_TOTAL))
		{ // salva ponteiro
			this->eqpto = eqpto;
			// remonta tronco
			SelecionaTroncoFluxo(opcao == opT_RADIAL);
		}
	}
	// salva ponteiro
	this->eqpto = eqpto;
}

// ---------------------------------------------------------------------------
VTPatamar* __fastcall TRecondutorar::PM_GetPatamar(void)
{
	return patamar;
}

// ---------------------------------------------------------------------------
void __fastcall TRecondutorar::PM_SetPatamar(VTPatamar* patamar)
{
	// proteção
	if (this->patamar == NULL)
	{ // salva ponteiro
		this->patamar = patamar;
		return;
	}
	// verifica se sao patamares diferentes
	if (patamar != this->patamar)
	{ // verifica se a opção com fluxo foi selecionada
		if ((opcao == opT_RADIAL) || (opcao == opT_TOTAL))
		{ // salva ponteiro
			this->patamar = patamar;
			// remonta tronco
			SelecionaTroncoFluxo(opcao == opT_RADIAL);
		}
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TRecondutorar::Seleciona(void)
{
	// variaveis locais
	bool sucesso = false;
	bool limitado = false;
	VTFlow *flow = (VTFlow*)apl->GetObject(__classid(VTFlow));

	// proteção
	if (eqpto == NULL)
	{
		Aviso("Selecione uma ligação para servir de referência");
		return false;
	}
	if ((opcao == opT_RADIAL) || (opcao == opT_TOTAL))
	{
		if (patamar == NULL)
		{
			Aviso("Selecione um patamar para determinação do tronco");
			return false;
		}
		if (!flow->UpToDate)
		{
			Aviso("Execute o fluxo de potência");
			return false;
		}
	}
	// verifica qual opção foi escolhida
	switch (opcao)
	{
	case opJUSANTE:
		sucesso = SelecionaJusante();
		break;

	case opT_ARRANJO:
		sucesso = SelecionaTroncoArranjo();
		break;

	case opT_RADIAL:
		limitado = true;
		sucesso = SelecionaTroncoFluxo(limitado);
		break;

	case opT_TOTAL:
		limitado = false;
		sucesso = SelecionaTroncoFluxo(limitado);
		break;

	default:
		break;
	}
	return sucesso;
}

// ---------------------------------------------------------------------------
bool __fastcall TRecondutorar::SelecionaJusante(void)
{
	// variaveis locais
	bool sucesso = true;
	VTLigacao *ligacao = (VTLigacao*)eqpto;
	VTNode *node;

	try
	{ // copia ligações da rede em lisLIG
		lisLIG->Clear();
		CopiaTList(ligacao->rede->LisLigacao(), lisLIG);
		// verifica se a arvore está montada
		if (arvore->LisNodeRaiz()->Count == 0)
		{
			sucesso = arvore->Inicia(lisLIG, ligacao->rede->BarraInicial());
		}
		if (sucesso)
		{ // determina node que contenha a ligacao
			if ((node = arvore->ExisteNode(ligacao)) == NULL)
			{
				Aviso("Erro ao determinar ligações a jusante");
				return false;
			}
			// determina lista de ligações a jusante
			lisLIG->Clear();
			node->LisObjectJusante(lisLIG);
			// determina somente trechos desta lista
			lisLIG->Add(ligacao);
			CopiaTrechos(lisLIG);
		}
	}
	catch (Exception &e)
	{
		sucesso = false;
	}
	return sucesso;
}

// ---------------------------------------------------------------------------
bool __fastcall TRecondutorar::SelecionaTroncoArranjo(void)
{
	// variaveis locais
	bool sucesso = true;
	VTLigacao *ligacao = (VTLigacao*)eqpto;
	TList *lisNODE_RAIZ;

	try
	{ // copia ligações da rede em lisLIG
		lisLIG->Clear();
		CopiaTList(ligacao->rede->LisLigacao(), lisLIG);
		// verifica se a arvore está montada
		if (arvore->LisNodeRaiz()->Count == 0)
		{
			sucesso = arvore->Inicia(lisLIG, ligacao->rede->BarraInicial());
		}
		if (sucesso)
		{ // determina node raiz com a ligação que conecta a barra inicial
			lisNODE_RAIZ = arvore->LisNodeRaiz();
			if (lisNODE_RAIZ->Count != 1)
			{
				Aviso("Alimentador inválido para selecionar trechos com mesmo arranjo");
				return false;
			}
			// monta lisEQP somente com os trechos com mesmo arranjo que o primeiro trecho
			if (!TrechosMesmoArranjoInicial(lisNODE_RAIZ))
			{
				Aviso("Erro ao determinar trechos com mesmo arranjo");
				return false;
			}
		}
	}
	catch (Exception &e)
	{
		sucesso = false;
	}
	return sucesso;
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/16220176
bool __fastcall TRecondutorar::SelecionaTroncoFluxo(bool limitado)
{
	// variaveis locais
	bool sucesso = false;
	VTPrimario *primario;
	VTLigacao *ligacao = (VTLigacao*)eqpto;

	try
	{ // determina primario
		if ((primario = radial->ExistePrimario(ligacao->rede)) == NULL)
		{
			Aviso("Selecione uma ligação de uma rede primária");
			return false;
		}
		// protecao
		if (patamar == NULL)
		{
			Aviso("Selecione um patamar");
			return false;
		}
		// inicia tronco
		tronco->LisLigacaoTronco()->Clear();
		tronco->LisLigacaoRamal()->Clear();
		tronco->LisBarraTronco()->Clear();
		tronco->CorrenteRamalLimitada = true;
		// verifica se possui limitação de corrente
		if (limitado)
		{ // altera para limitar corrente default (1 pu)
            tronco->CorrenteRamalMax_pu = 1.;
		}
		else
		{ // altera para não limitar corrente (isto é, corrente = 0 pu)
			tronco->CorrenteRamalMax_pu = 0.;
		}
		if (!tronco->DefinePrimario(primario, patamar->Index))
		{
			Aviso("Não foi possível definir o tronco para a rede " + primario->Rede->Codigo);
			return false;
		}
		// determina trechos desse tronco
		CopiaTrechos(tronco->LisLigacaoTronco());
		sucesso = true;
	}
	catch (Exception &e)
	{
		sucesso = false;
	}

	return sucesso;
}

// ---------------------------------------------------------------------------
VTTrecho* __fastcall TRecondutorar::TrechoInicial(VTBarra *barra, VTRede *rede, VTLigacao *liga_ini)
{
	// variaveis locais
	int ind_barra;
	VTBarra *barra2;
	VTTrecho *trecho;
	VTLigacao *ligacao, *liga_rede;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// determina ligações da barra
	lisLIG->Clear();
	redes->LisLigacao(lisLIG, barra);
	// inicia variaveis
	liga_rede = trecho = NULL;
	for (int n = 0; n < lisLIG->Count; n++)
	{
		ligacao = (VTLigacao*)lisLIG->Items[n];
		// pula ligacao se for a mesma liga_ini
		if (ligacao == liga_ini)
			continue;
		// verifica se é da rede
		if (ligacao->rede == rede)
		{
			liga_rede = ligacao;
			// verifica se é trecho
			if (ligacao->Tipo() == eqptoTRECHO)
			{
				trecho = (VTTrecho*)ligacao;
				break;
			}
		}
	}
	// verifica se não encontrou
	if (trecho == NULL)
	{
		if (liga_rede != NULL)
		{ // determina outra barra
			ind_barra = liga_rede->IndiceBarra(barra);
			barra2 = liga_rede->Barra(1 - ind_barra);
			// chamada recursiva
			trecho = TrechoInicial(barra2, rede, liga_rede);
		}
	}
	return trecho;
}

// ---------------------------------------------------------------------------
bool __fastcall TRecondutorar::TrechosMesmoArranjoInicial(TList *lisNODE)
{
	// variaveis locais
	VTNode *node;
	VTLigacao *ligacao;
	VTTrecho *trecho;
	VTArranjo *arranjo = NULL;
	TList *lisObjNode;

	//proteção
	if(lisNODE->Count == 0)
        return false;
	//percorre niveis da arvore
	for (int n = 0; n < lisNODE->Count; n++)
	{
		node = (VTNode*)lisNODE->Items[n];
		//percorre ligacoes que compoem o node
		lisObjNode = node->LisObjectNode();
		for(int nl = 0; nl < lisObjNode->Count; nl++)
		{
			ligacao = (VTLigacao*)lisObjNode->Items[nl];
			if(ligacao->Tipo() == eqptoTRECHO)
			{
				trecho = (VTTrecho*)ligacao;
				//verifica se o arranjo do trecho inicial ja foi definido
				if(arranjo == NULL)
				{//guarda arranjo inicial
					arranjo = trecho->arranjo;
					lisEQP->Add(trecho);
					//verifica nodes a jusante se possui mesmo arranjo que arranjo do trecho inicial
					if(! TrechosMesmoArranjoNode(node->LisNode(), arranjo))
						break;
                }
            }
		}
		//verifica se já encontrou arranjo
		if(arranjo == NULL)
		{//chamada recursiva
			if(! TrechosMesmoArranjoInicial(node->LisNode()))
				continue;
        }
	}
	return (lisEQP->Count > 0);
}

// ---------------------------------------------------------------------------
bool __fastcall TRecondutorar::TrechosMesmoArranjoNode(TList *lisNODE, VTArranjo *arranjo)
{
	// variaveis locais
	VTNode *node;
	VTLigacao *ligacao;
	VTTrecho *trecho = NULL;

    for (int n = 0; n < lisNODE->Count; n++)
	{
		node = (VTNode*)lisNODE->Items[n];
		//percorre ligacoes que compoem o node
		lisLIG->Clear();
		node->LisObjectNode(lisLIG);
		for(int nl = 0; nl < lisLIG->Count; nl++)
		{
			ligacao = (VTLigacao*)lisLIG->Items[nl];
			if(ligacao->Tipo() == eqptoTRECHO)
			{
				trecho = (VTTrecho*)ligacao;
                //verifica se possui mesmo arranjo que arranjo do trecho inicial
				if(trecho->arranjo == arranjo)
				{
					lisEQP->Add(trecho);
					//chamada recursiva
					//verifica nodes a jusante se possui mesmo arranjo que arranjo do trecho inicial
					return(TrechosMesmoArranjoNode(node->LisNode(), arranjo));
				}
			}
		}
		//não encontrou trecho: chamada recursiva
		if(trecho == NULL)
		{
			if(TrechosMesmoArranjoNode(node->LisNode(), arranjo))
				continue;
		}
	}
	if(trecho != NULL)
	{//encontrou, mas com arranjo diferente: fim
		return false;
	}
	return true;
}

// ---------------------------------------------------------------------------
