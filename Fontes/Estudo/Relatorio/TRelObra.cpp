// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TRelObra.h"
#include "TRelObraTO.h"
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
#include "..\Apl\VTApl.h"
// #include "..\Arranjo\VTArranjo.h"
// #include "..\Diretorio\VTPath.h"
#include "..\MultiObra\VTMultiObraBO.h"
#include "..\MultiObra\VTMultiObra.h"
#include "..\Obra\VTObra.h"
#include "..\Obra\VTAcao.h"
#include "..\Obra\VTObras.h"
#include "..\Obra\ItemObra\VTItemObraCOPEL.h"
#include "..\Planejamento\VTAlternativa.h"
#include "..\Planejamento\VTEstudo.h"
// #include "..\Planejamento\VTPlanejamento.h"
#include "..\Radial\VTArvore.h"
#include "..\Radial\VTNode.h"
#include "..\Rede\VTCapacitor.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTReator.h"
#include "..\Rede\VTTrafo.h"
#include "..\Rede\VTTrafo3E.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTTipoRede.h"
#include "..\Report\VTReport.h"
// ---------------------------------------------------------------------------
#include <ModulosOpcionais\Fontes\BaseGRP\VTPadrao.h>
// ---------------------------------------------------------------------------
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Radial.h"
// #include "..\..\DLL_Inc\Report.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall TRelObra::TRelObra(VTEstudo *estudo, VTApl* apl_owner)
{
	VTRedes *redes;

	// salva ponteiro´p/ Estudo
	this->estudo = estudo;
	// salva ponteiro para apl
	apl = apl_owner;
	// cria a arvore de redes
	arvore_redes = DLL_NewObjArvore(apl);
	redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	arvore_redes->Inicia(redes);

	lisRelObraTO = new TList;
	campos = new TStringList();
}

// ---------------------------------------------------------------------------
__fastcall TRelObra::~TRelObra(void)
{
	if (arvore_redes)
	{
		delete arvore_redes;
		arvore_redes = NULL;
	}
	if (lisRelObraTO)
	{
		LimpaTList(lisRelObraTO);
		delete lisRelObraTO;
		lisRelObraTO = NULL;
	}
	if (campos)
	{
		delete campos;
		campos = NULL;
	}
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/15641481
bool __fastcall TRelObra::AlimentadorPai(VTRede *rede_sub, VTRede *rede_filha)
{
	// variáveis locais
	AnsiString cod_gis;

	// proteçao
	if ((rede_sub == NULL) || (rede_filha == NULL))
		return false;

	// limpa campos
	campos->Clear();
	ExtraiStrings(rede_sub->Extern_id, "_", campos);
	// somente com 2 campos ou  mais será analisado
	if (campos->Count < 2)
		return false;
	for (int n = 1; n < campos->Count; n++)
	{ // o primeiro campo é o codigo GIS da sub, então compara os demais
		cod_gis = campos->Strings[n];
		if (cod_gis.AnsiCompareIC(rede_filha->Extern_id) == 0)
			return true;
	}
	return false;
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TRelObra::DeterminaAcao(VTItemObraCOPEL *item)
{
	AnsiString acaoStr = "";

	if ((item->PadraoIns != NULL) && (item->PadraoRet == NULL))
	{
		acaoStr = "Insere";
	}
	else if ((item->PadraoIns == NULL) && (item->PadraoRet != NULL))
	{
		acaoStr = "Retira";
	}
	else if ((item->PadraoIns != NULL) && (item->PadraoRet != NULL))
	{
		acaoStr = "Altera";
	}
	else
	{
		acaoStr = "Invalida";
	}

	return acaoStr;
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TRelObra::DeterminaCodigoPadrao(VTItemObraCOPEL *item)
{
	AnsiString codigo = "";

	try
	{
		if ((item != NULL) && (item->PadraoIns != NULL))
		{
			codigo = item->PadraoIns->Codigo;
		}
	}
	catch (Exception &e)
	{
		AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
		codigo = "";
	}
	catch (...)
	{
		AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(funcao);
		codigo = "";
	}

	return codigo;
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TRelObra::DeterminaCusto(VTItemObraCOPEL *item)
{
	AnsiString custo = "";

	custo = FormataValorMonetario(item->Custo);

	return custo;
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TRelObra::DeterminaDescricaoPadrao(VTItemObraCOPEL *item)
{
	AnsiString descr = "";

	try
	{
		if ((item != NULL) && (item->PadraoIns != NULL))
		{
			descr = item->PadraoIns->Descricao;
		}
	}
	catch (Exception &e)
	{
		AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
		descr = "";
	}
	catch (...)
	{
		AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(funcao);
		descr = "";
	}

	return descr;
}

// ---------------------------------------------------------------------------
VTEqpto* __fastcall TRelObra::DeterminaEqpto(VTItemObraCOPEL *item)
{
	VTEqpto *eqpto = NULL;
	VTAcao *acao;
	VTPadrao *padrao = NULL;
	try
	{

		if (item != NULL)
		{
			// verifica padrao de insercao
			if (item->PadraoIns != NULL)
			{
				padrao = item->PadraoIns;
			}
			else
			{
				padrao = item->PadraoRet;
			}
			// se encontrou
			if (padrao != NULL)
			{
				// seleciona um eqpto dentro das acoes que possua o mesmo eqpto do padrao construtivo
				for (int na = 0; na < item->LisAcao->Count; na++)
				{
					acao = (VTAcao*)item->LisAcao->Items[na];

					if ((acao->Eqpto() != NULL) && (acao->Eqpto()->Tipo() == padrao->TipoEqpto))
					{
						eqpto = acao->Eqpto();
						break;
					}
				}
			}
		}
	}
	catch (Exception &e)
	{
		AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
		eqpto = NULL;
	}
	catch (...)
	{
		AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(funcao);
		eqpto = NULL;
	}

	return eqpto;
}

// ---------------------------------------------------------------------------
VTRede* __fastcall TRelObra::DeterminaRede(VTEqpto *eqpto)
{
	VTRede *rede = NULL;

	try
	{
		// seleciona um eqpto dentro das acoes que possua o mesmo eqpto do padrao construtivo
		if (eqpto != NULL)
		{
			rede = DeterminaRedeDoEqpto(eqpto);
		}
	}
	catch (Exception &e)
	{
		AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
		rede = NULL;
	}
	catch (...)
	{
		AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(funcao);
		rede = NULL;
	}

	return rede;
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/16051935
VTRede* __fastcall TRelObra::DeterminaRedeDaBarra(VTBarra *barra)
{
	// variáveis locais
	VTRede *rede = NULL;
	VTRedes *redes;
	TList *lisREDE;

	if (alternativa != NULL)
	{
		redes = (VTRedes*)alternativa->Apl->GetObject(__classid(VTRedes));
		lisREDE = redes->LisRede();
		for (int n = 0; n < lisREDE->Count; n++)
		{
			rede = (VTRede*)lisREDE->Items[n];
			if (rede->ExisteBarra(barra))
				return (rede);
		}
	}

	return (rede);
}

// ---------------------------------------------------------------------------
VTRede* __fastcall TRelObra::DeterminaRedeDoEqpto(VTEqpto *eqpto)
{
	// variáveis locais
	VTRede *rede = NULL;

	// verifica se Eqpto é uma Rede
	if (eqpto->Tipo() == eqptoREDE)
	{
		rede = (VTRede*)eqpto;
	}
	// verifica se Eqpto é uma Ligacao
	else if (eqpto->TipoLigacao())
	{
		rede = ((VTLigacao*)eqpto)->rede;
	}
	// verifica se Eqpto é uma Ligacao Eqbar
	else if (eqpto->TipoEqbar())
	{ // determina uma Rede que inclui a Barra do Eqbar
		rede = DeterminaRedeDaBarra(((VTEqbar*)eqpto)->pbarra);
	}
	// proteção
	// if (rede == NULL) return(NULL);
	// somente redePRI e redeETD são representadas na base de ativos
	// if (rede->TipoRede->Segmento == redePRI) return(rede);
	// if (rede->TipoRede->Segmento == redeETD) return(rede);
	// rede não representada na base de ativos
	return (rede);
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/15641481
VTRede* __fastcall TRelObra::DeterminaSubestacao(VTRede* rede)
{
	UnicodeString className;
	TList *lisOBJ;
	TObject *obj;
	VTNode *node, *node_pai;
	VTRede *sub = NULL;
	VTRede *redeAux = NULL;
	int ind;

	try
	{ // proteção
		if (arvore_redes == NULL)
			return NULL;
		node = arvore_redes->ExisteNode(rede);
		if (node == NULL)
			return NULL;
		// verifica se possui node_pai
		node_pai = node->Pai;
		if ((node_pai != NULL) && (node_pai->Object != NULL))
		{ // verifica se é grupo de redes
			ind = node_pai->Codigo.AnsiPos("Grupo");
			if (ind <= 0)
			{ // verifica se object é rede subestação
				className = node_pai->Object->ClassName();
				if (className.CompareIC("TRede") == 0)
				{
					sub = (VTRede*)node_pai->Object;
					if ((sub->Carregada) && (sub->TipoRede->Segmento == redeETD))
					{ // verifica se a sub filha não é um alimentador pai
						if (!AlimentadorPai(sub, rede))
							return sub;
					}
				}
			}
			// verifica objects do node
			lisOBJ = node_pai->LisObjectNode();
			for (int n = 0; n < lisOBJ->Count; n++)
			{
				obj = (TObject*)lisOBJ->Items[n];
				className = obj->ClassName();
				if (className.CompareIC("TRede") == 0)
				{
					sub = (VTRede*)obj;
					if ((sub->Carregada) && (sub->TipoRede->Segmento == redeETD))
					{ // verifica se a sub filha não é um alimentador pai
						if (!AlimentadorPai(sub, rede))
							return sub;
					}
				}
			}
		}
		else
		{ // verifica se o próprio node possui a subestação
			lisOBJ = node->LisObjectNode();
			for (int nr = 0; nr < lisOBJ->Count; nr++)
			{
				obj = (TObject*)lisOBJ->Items[nr];
				className = obj->ClassName();
				if (className.CompareIC("TRede") == 0)
				{
					sub = (VTRede*)obj;
					if (!sub->Carregada)
						continue;
					if ((sub->TipoRede != NULL) && (sub->TipoRede->Segmento == redeETD))
					{ // verifica se a sub filha não é um alimentador pai
						if (!AlimentadorPai(sub, rede))
							return sub;
					}
				}
			}
		}
	}
	catch (Exception &e)
	{
		AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
		sub = NULL;
	}
	catch (...)
	{
		AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(funcao);
		sub = NULL;
	}

	return NULL;
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TRelObra::FormataValorMonetario(double valor)
{
	AnsiString custoStr = "-";
	// Currency valorCurr;
	TFormatSettings format = TFormatSettings::Create(GetThreadLocale());

	// valorCurr = valor;
	try
	{
		custoStr = CurrToStrF(valor, ffCurrency, 2, format);
	}
	catch (...)
	{
		custoStr = "-";
	}

	return custoStr;
}

// ---------------------------------------------------------------------------
void __fastcall TRelObra::IniciaAlternativas(void)
{
	VTMultiObraBO *multiObraBO = (VTMultiObraBO*)apl->GetObject(__classid(VTMultiObraBO));
	VTAlternativa *alternativa;
	TList *lisALTER = estudo->LisAlternativa();
	TList *lisOBRAS;
	VTObra *obra;
	VTItemObraCOPEL *item;

	try
	{
		// percorrer todas as obras e verificando se todas tem o multiobra
		for (int n = 0; n < lisALTER->Count; n++)
		{
			alternativa = (VTAlternativa*)lisALTER->Items[n];
			lisOBRAS = alternativa->Obras->LisObra();
			for (int no = 0; no < lisOBRAS->Count; no++)
			{
				obra = (VTObra*) lisOBRAS->Items[no];
				// cria o multiobra e analisa obra caso já nao exista
				multiObraBO->AnalisaAcoesObra(obra);
			}
		}
	}
	catch (Exception &e)
	{
		AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
	}
	catch (...)
	{
		AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(funcao);
	}
}

// ---------------------------------------------------------------------------
TRelObraTO* __fastcall TRelObra::PreencheRelObraTO(VTAlternativa *alt, VTObra *obra,
	VTItemObraCOPEL *item)
{
	TRelObraTO *rel = NULL;
	std::map<VTItemObraCOPEL*, TRelObraTO*>::iterator it;
	VTEqpto *eqpto = NULL;
	AnsiString txt = "";
	VTRede *redePropria = NULL;

	if ((alt == NULL) || (obra == NULL) || (item == NULL))
	{
		return rel;
	}

	this->alternativa = alt;
	// verifica se o itemObra ja possui um relobraTO
	it = mapRelObra.find(item);
	if (it != mapRelObra.end())
	{
		rel = it->second;
	}
	else
	{
		rel = new TRelObraTO();
		mapRelObra.insert(std::pair<VTItemObraCOPEL*, TRelObraTO*>(item, rel));
		lisRelObraTO->Add(rel);
	}
	// preenche o rel
	rel->nomeAlternativa = alt->Codigo;
	rel->nomeObra = obra->Nome;
	rel->data = IntToStr(obra->AnoAtivacao);
	// determina eqpto
	eqpto = DeterminaEqpto(item);
	if (eqpto != NULL)
	{
		// descobre a rede do item
		redePropria = DeterminaRede(eqpto);
		// verifica o tipo
		if ((redePropria != NULL) && (redePropria->TipoRede != NULL) &&
			(redePropria->TipoRede->Segmento == redePRI))
		{
			rel->alimentador = redePropria;
			rel->nomeAlimentador = rel->alimentador->Codigo;
			rel->subestacao = DeterminaSubestacao(rel->alimentador);
			if (rel->subestacao != NULL)
			{
				rel->nomeSED = rel->subestacao->Codigo;
			}
		}
		else if ((redePropria != NULL) && (redePropria->TipoRede != NULL) &&
			(redePropria->TipoRede->Segmento == redeETD))
		{
			rel->subestacao = redePropria;
			rel->nomeSED = rel->subestacao->Codigo;
		}
		// determina complemento
		switch (eqpto->Tipo())
		{
		case eqptoTRECHO:
			txt.sprintf("comp(km): %4.3f", item->QuantidadeIns);
			break;

		default:
			txt.sprintf("unidades: %1.0f", item->QuantidadeIns);
			break;
		}
		rel->complemento = txt;

		rel->acao = DeterminaAcao(item);

		rel->eqpto = eqpto->TipoAsString();
	}

	rel->cod_modulo = DeterminaCodigoPadrao(item);
	rel->descricao_modulo = DeterminaDescricaoPadrao(item);

	rel->custo = DeterminaCusto(item);

	return rel;
}
// ---------------------------------------------------------------------------
// eof
