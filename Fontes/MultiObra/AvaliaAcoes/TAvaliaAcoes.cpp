// ---------------------------------------------------------------------------
#include <vcl.h>
#include <map>
#pragma hdrstop
#include "TAvaliaAcoes.h"
#include "VTAvaliaAcoesEqpto.h"
#include "TNodeDetalhe.h"
// ---------------------------------------------------------------------------
#include <PlataformaSinap\Fontes\Apl\VTApl.h>
#include <PlataformaSinap\Fontes\Obra\VTAcao.h>
#include <PlataformaSinap\Fontes\Radial\VTArvore.h>
#include <PlataformaSinap\Fontes\Rede\VTEqbar.h>
#include <PlataformaSinap\Fontes\Rede\VTEqpto.h>
#include <PlataformaSinap\Fontes\Rede\VTLigacao.h>
#include <PlataformaSinap\Fontes\Rede\VTRede.h>
#include <PlataformaSinap\Fontes\Rede\VTRedes.h>
// ---------------------------------------------------------------------------
#include "..\..\DLL_Inc\Funcao.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
// VTItemARIES* __fastcall NewObjItemAries(void)
// {
// try{
// return (new TItemARIES());
// }catch (Exception &e)
// {
// return(NULL);
// }
// }

// ---------------------------------------------------------------------------
__fastcall TAvaliaAcoes::TAvaliaAcoes(VTApl *apl)
{
    this->apl = apl;
}

// ---------------------------------------------------------------------------
__fastcall TAvaliaAcoes::~TAvaliaAcoes(void)
{

}
// ---------------------------------------------------------------------------
void __fastcall TAvaliaAcoes::AvaliaNode(TNodeDetalhe *node)
{
	TNodeDetalhe *nodeAux;
	TList *lisNode = NULL;

	//v q nivel o node tah
	switch(node->Level)
	{
		case 0:
			SeparaNivel1(node);
			break;
		case 1:
			SeparaNivel2(node);
			break;
		case 2:
			SeparaNivel3(node);
			break;
		case 3:
			SeparaNivel4(node);
			break;
		default:
            ;
	}
	//depois de separar o proprio nivel, separa os filhos
	lisNode = node->LisNode();
	for (int nd = 0; nd < lisNode->Count; nd++) 
	{
		nodeAux = (TNodeDetalhe*)lisNode->Items[nd];
		AvaliaNode(nodeAux);//chamada recursiva
	}

	return;
}
// ---------------------------------------------------------------------------
//feito da mesma forma que é feito la no TAtivoARIES
VTRede* __fastcall TAvaliaAcoes::DeterminaRede(VTAcao *acao)
{	// variaveis locais
	VTLigacao *ligacao, *ligacaoAntes, *ligacaoDepois;
	VTRede *rede = NULL;
	VTEqbar *eqbar;
	VTEqpto *eqpto;

    //protecao
	if (acao == NULL)
		{return NULL;}

	eqpto = acao->Eqpto();

	if (eqpto->TipoLigacao())
	{ // define codigo da rede se o equipamento for ligacao
		ligacao = (VTLigacao*)eqpto;
		ligacaoAntes = (VTLigacao*) acao->CopiaDoEqptoOriginal();
		ligacaoDepois = (VTLigacao*)acao->CopiaDoEqptoAlterado();
		rede = ligacao->rede;
		if (rede->TipoRede == NULL)
		{
			if ((ligacaoDepois != NULL) && (ligacaoDepois->rede->TipoRede != NULL))
			{
				rede = ligacaoDepois->rede;
			}
			else if ((ligacaoAntes != NULL) && (ligacaoAntes->rede->TipoRede != NULL))
			{
				rede = ligacaoAntes->rede;
			}
		}
	}
	else if (eqpto->Tipo() == eqptoREDE)
	{ // verifica se é uma rede
		rede = (VTRede*)eqpto;
	}
	else if (eqpto->TipoEqbar())
	{ // verifica se é eqbar
		eqbar = (VTEqbar*)eqpto;
		// localiza a rede
		rede = DeterminaRede(eqbar);
	}

	return rede;
}
// ---------------------------------------------------------------------------
//feito da mesma forma que é feito la no TAtivoARIES
VTRede* __fastcall TAvaliaAcoes::DeterminaRede(VTEqbar *eqbar)
{	// variaveis locais
	TList *lisLigacoes;
	TList *lisRedesLig;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTLigacao *ligacao;
	VTRede *rede_padrao;
	VTRede *redeEqbar = NULL;

	try
	{ // cria as listas
		lisLigacoes = new TList;
		lisRedesLig = new TList;
		// verifica quais ligacoes estao ligadas à barra do eqbar
		redes->LisLigacao(lisLigacoes, eqbar->pbarra);
		// percorre as ligacoes listando as redes
		for (int nr = 0; nr < lisLigacoes->Count; nr++)
		{
			ligacao = (VTLigacao*)lisLigacoes->Items[nr];
			// salva a 1a rede
			if (nr == 0)
			{
				rede_padrao = ligacao->rede;
			}
			// verifica se a rede é igual à padrao
			if (ligacao->rede == rede_padrao)
			{
				// nomeRede = ligacao->rede->Codigo;
				redeEqbar = ligacao->rede;
			}
			else
			{
				// nomeRede = "";
				redeEqbar = NULL;
				break;
			}
		}
	}
	catch (Exception &e)
	{
		// nomeRede = "";
		redeEqbar = NULL;
	}
	// deleta a lsita de redes
	if (lisLigacoes)
	{
		delete lisLigacoes;
		lisLigacoes = NULL;
	}
	if (lisRedesLig)
	{
		delete lisRedesLig;
		lisRedesLig = NULL;
	}

	// return nomeRede;
	return redeEqbar;

}
// ---------------------------------------------------------------------------
int __fastcall  TAvaliaAcoes::DeterminaTipoEqpto(VTAcao *acao)
{
	VTEqpto *eqpto;
	int tipo = eqptoINDEF;

	if (acao == NULL)
		{return eqptoINDEF;}
	eqpto = acao->Eqpto();
    if (eqpto == NULL)
		{return eqptoINDEF;}

	//verifica o tipo
	tipo = eqpto->Tipo();

	return tipo;
}
// ---------------------------------------------------------------------------
AnsiString __fastcall TAvaliaAcoes::DeterminaTipoSemelhanca(VTAcao *acao, int tipoEqpto)
{
	AnsiString tipoSemel = "";
	VTAvaliaAcoesEqpto *avalia = NULL;

	if (tipoEqpto <= 0) 
	{
		tipoEqpto = DeterminaTipoEqpto(acao);    
	}
	switch (tipoEqpto)
	{
		case eqptoTRECHO:
			avalia = NewObjAvaliaAcoesTrecho(apl);
			break;
		case eqptoREGULADOR:
			avalia = NewObjAvaliaAcoesRegulador(apl);
			break;
		case eqptoTRAFO:
			avalia = NewObjAvaliaAcoesTrafo(apl);
			break;
		case eqptoREDE:
			avalia = NewObjAvaliaAcoesRede(apl);
			break;
		case eqptoCHAVE:
			avalia = NewObjAvaliaAcoesChave(apl);
			break;
		case eqptoCAPACITOR:
			avalia = NewObjAvaliaAcoesCapacitor(apl);
			break;
		default:
            avalia = NULL
		;
	}

	if (avalia != NULL) 
	{
		tipoSemel = avalia->DeterminaTipoSemelhanca(acao);    
	}

	delete avalia;
	
	return  tipoSemel;
}
// ---------------------------------------------------------------------------
void __fastcall TAvaliaAcoes::InsereAcoes(VTAcao *acao, TList *lisEXT)
{
	TList *list = NULL;
	VTAcao *acaoAux = NULL;

	try
	{
	//    acao = (VTAcao*) lisAcoes->Items[na];
		if (acao->Tipo() == acaoCOMPOSTA)
		{
			list = acao->LisAcao();
			if (list != NULL)
			{
				for (int nc = 0; nc < list->Count; nc++)
				{
					acaoAux = (VTAcao*) list->Items[nc];
					InsereAcoes(acaoAux, lisEXT);
				}
			}
		}
		else
		{
			lisEXT->Add(acao);
		}
	}
	catch(Exception &e)
	{
		AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
	}
}
// ---------------------------------------------------------------------------
bool __fastcall TAvaliaAcoes::MontaArvore(TList *lisAcoes, VTArvore *arvoreEXT)
{
	TNodeDetalhe *nodeDet;
	TList *lisNodeRaiz;
	bool sucesso = true;
	
	try
	{
		//cria arvore
		arvoreEXT->Clear();
		//monta o node raiz
		lisNodeRaiz = new TList;
		SeparaNivel0(lisAcoes,lisNodeRaiz);
		//percorre todos nodes raiz
		for (int nr = 0; nr < lisNodeRaiz->Count; nr++) 
		{
			nodeDet = (TNodeDetalhe*) lisNodeRaiz->Items[nr];
			arvoreEXT->InsereNode(NULL,nodeDet);
			AvaliaNode(nodeDet);    
		}
	}
	catch(...)
	{
		if (arvoreEXT != NULL) 
		{
			arvoreEXT->Clear();		
		}
		sucesso = false;	
	}

	DestroiTObject(lisNodeRaiz);
	return  sucesso;
}
// ---------------------------------------------------------------------------
void __fastcall TAvaliaAcoes::SeparaNivel0(TList *lisAcoes, TList *lisEXT_nodeDetalhe)// nivel Obra
{
	VTAcao *acao;
	TNodeDetalhe *nodeDet;

	//protecao
	if (lisEXT_nodeDetalhe == NULL)
		{return;}
	if (lisAcoes == NULL)
		{return;}
	//limpa a lista
	lisEXT_nodeDetalhe->Clear();
	//cria um node com todas acoes
	nodeDet = new TNodeDetalhe();
	nodeDet->Codigo = "Obra: ";
	nodeDet->TipoEqpto = eqptoINDEF;
	//copia a lista de acoes no node
	//tem q percorrer a lista adicionando uma a uma por causa das compostas
	for (int na = 0; na < lisAcoes->Count; na++)
	{
		acao = (VTAcao*) lisAcoes->Items[na];
		InsereAcoes(acao,nodeDet->LisAcao);
	}
	//adiciona
	lisEXT_nodeDetalhe->Add(nodeDet);
    int a = 0;
}
// ---------------------------------------------------------------------------
void __fastcall TAvaliaAcoes::SeparaNivel1(TNodeDetalhe *node) // nivel Tipo Eqpto
{
	VTAcao *acao;
	TNodeDetalhe *nodeDet;
	std::map<int, TNodeDetalhe*> mapTipoEqpto;
	std::map<int, TNodeDetalhe*>::iterator it;
	TList *lisAcao; 
	TList *lisAcoes = NULL;

	//protecao
	if (node == NULL)
		{return;}
	lisAcoes = node->LisAcao;
	if (lisAcoes == NULL)
		{return;}
//	if (lisEXT_nodeDetalhe == NULL)
//		{return;}
	if (node == NULL)
		{return;}
	//limpa a lista
//	lisEXT_nodeDetalhe->Clear();
	//percore todas acoes verificando o equipamento
	for (int na = 0; na < lisAcoes->Count; na++)
	{
		acao = (VTAcao*)lisAcoes->Items[na];
		//reseta a lista
		lisAcao = NULL;
		if (acao->Eqpto() != NULL)
		{
			//verifica se ja existe node
			it = mapTipoEqpto.find(acao->Eqpto()->Tipo());
			//nao encontrou
			if (it == mapTipoEqpto.end())
			{
				//cria um novo
				nodeDet = new TNodeDetalhe();
				try
				{
					nodeDet->Codigo = " Eqpto: " +acao->Eqpto()->TipoAsString();
					nodeDet->TipoEqpto = acao->Eqpto()->Tipo();
				}
				catch(...)
				{
					nodeDet->Codigo = "Indefinido";
					nodeDet->TipoEqpto = eqptoINDEF;
                }
				//insere no map
				mapTipoEqpto.insert(std::pair<int, TNodeDetalhe*>(acao->Eqpto()->Tipo(), nodeDet));
				//guarda a lista
				lisAcao = nodeDet->LisAcao;
				//guarda o node na lista Externa
//				lisEXT_nodeDetalhe->Add(nodeDet);
				node->InsereNode(nodeDet);
			}
			else //encontrou
			{
				nodeDet = it->second;
				//guarda a lista
				lisAcao = nodeDet->LisAcao;
			}
			//se encontrou uma listAcao
			if (lisAcao != NULL)
			{
                lisAcao->Add(acao);
			}
		}
	}
}
// ---------------------------------------------------------------------------
void __fastcall TAvaliaAcoes::SeparaNivel2(TNodeDetalhe *node) // nivel Semelhanca
{
	VTAcao *acao;
	TNodeDetalhe *nodeDet;
	std::map<AnsiString, TNodeDetalhe*> mapSemelhanca;
	std::map<AnsiString, TNodeDetalhe*>::iterator it;
	TList *lisAcao;
	AnsiString nomeSemelhanca;
	TList *lisAcoes = NULL;

	//protecao
	if (node == NULL)
		{return;}
	lisAcoes = node->LisAcao;
	if (lisAcoes == NULL)
		{return;}
//	if (lisEXT_nodeDetalhe == NULL)
//		{return;}
	if (lisAcoes == NULL)
		{return;}
	//limpa a lista
//	lisEXT_nodeDetalhe->Clear();
	//percore todas acoes verificando a semelhanca
	for (int na = 0; na < lisAcoes->Count; na++)
	{
		acao = (VTAcao*)lisAcoes->Items[na];
		//reseta a lista
		lisAcao = NULL;
		if (acao->Eqpto() != NULL)
		{
			//determina a semelhanca
			nomeSemelhanca = DeterminaTipoSemelhanca(acao, node->TipoEqpto);
			//verifica se ja existe node
			it = mapSemelhanca.find(nomeSemelhanca);
			//nao encontrou
			if (it == mapSemelhanca.end())
			{
				//cria um novo
				nodeDet = new TNodeDetalhe();
				nodeDet->TipoEqpto = node->TipoEqpto;
				if (nomeSemelhanca.IsEmpty())
				{
					nodeDet->Codigo = "Indeterminada";
				}
				else
				{
					nodeDet->Codigo = nomeSemelhanca;
				}
				//insere no map
				mapSemelhanca.insert(std::pair<AnsiString, TNodeDetalhe*>(nomeSemelhanca, nodeDet));
				//guarda a lista
				lisAcao = nodeDet->LisAcao;
				//guarda o node na lista Externa
//				lisEXT_nodeDetalhe->Add(nodeDet);    
				node->InsereNode(nodeDet);
			}
			else //encontrou
			{
				nodeDet = it->second;
				//guarda a lista
				lisAcao = nodeDet->LisAcao;
			}
			//se encontrou uma listAcao
			if (lisAcao != NULL)
			{
                lisAcao->Add(acao);
			}
		}
	}
}
// ---------------------------------------------------------------------------
void __fastcall TAvaliaAcoes::SeparaNivel3(TNodeDetalhe *node) // nivel Rede
{
    VTAcao *acao;
	TNodeDetalhe *nodeDet;
	std::map<VTRede*, TNodeDetalhe*> mapRede;
	std::map<VTRede*, TNodeDetalhe*>::iterator it;
	TList *lisAcao;
	VTRede *rede;
	TList *lisAcoes = NULL;

	//protecao
	if (node == NULL)
		{return;}
	lisAcoes = node->LisAcao;
	if (lisAcoes == NULL)
		{return;}
//	if (lisEXT_nodeDetalhe == NULL)
//		{return;}
	if (lisAcoes == NULL)
		{return;}
	//limpa a lista
//	lisEXT_nodeDetalhe->Clear();
	//percore todas acoes verificando a rede do equipamento
	for (int na = 0; na < lisAcoes->Count; na++)
	{
		acao = (VTAcao*)lisAcoes->Items[na];
		//reseta a lista
		lisAcao = NULL;
		if (acao->Eqpto() != NULL)
		{
			//determina a rede
			rede = DeterminaRede(acao);
			//PENSAR: deve existir um item no map pra rede NULL?

			//verifica se ja existe node
			it = mapRede.find(rede);
			//nao encontrou
			if (it == mapRede.end())
			{
				//cria um novo
				nodeDet = new TNodeDetalhe();
				nodeDet->TipoEqpto = node->TipoEqpto;
				if (rede == NULL)
				{
					nodeDet->Codigo = "Rede: Indeterminada";
				}
				else
				{
					nodeDet->Codigo = "Rede: " + rede->Codigo;
				}
				//insere no map
				mapRede.insert(std::pair<VTRede*, TNodeDetalhe*>(rede, nodeDet));
				//guarda a lista
				lisAcao = nodeDet->LisAcao;
				//guarda o node na lista Externa
//				lisEXT_nodeDetalhe->Add(nodeDet);
				node->InsereNode(nodeDet);
			}
			else //encontrou
			{
				nodeDet = it->second;
				//guarda a lista
				lisAcao = nodeDet->LisAcao;
			}
			//se encontrou uma listAcao
			if (lisAcao != NULL)
			{
                lisAcao->Add(acao);
			}
		}
	}
}
// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16528714
void __fastcall TAvaliaAcoes::SeparaNivel4(TNodeDetalhe *node) // nivel Eqpto
{
    VTAcao *acao;
	TNodeDetalhe *nodeDet;
	std::map<VTEqpto*, TNodeDetalhe*> mapEqpto;
	std::map<VTEqpto*, TNodeDetalhe*>::iterator it;
	TList *lisAcao;
	VTRede *rede;
	TList *lisAcoes = NULL;

	//protecao
	if (node == NULL)
		{return;}
	lisAcoes = node->LisAcao;
	if (lisAcoes == NULL)
		{return;}
//	if (lisEXT_nodeDetalhe == NULL)
//		{return;}
	if (lisAcoes == NULL)
		{return;}
	//limpa a lista
//	lisEXT_nodeDetalhe->Clear();
	//percore todas acoes verificando a rede do equipamento
	for (int na = 0; na < lisAcoes->Count; na++)
	{
		acao = (VTAcao*)lisAcoes->Items[na];
		//reseta a lista
		lisAcao = NULL;
		if (acao->Eqpto() != NULL)
		{
			//verifica se ja existe node
			it = mapEqpto.find(acao->Eqpto());
			//nao encontrou
			if (it == mapEqpto.end())
			{
				//cria um novo
				nodeDet = new TNodeDetalhe();
				nodeDet->TipoEqpto = node->TipoEqpto;
				if (acao->Eqpto() == NULL)
				{
					nodeDet->Codigo = "Indeterminado";
				}
				else
				{
					if (acao->Eqpto()->Codigo.IsEmpty())
					{
						nodeDet->Codigo = "ID eqpto: " + IntToStr(acao->Eqpto()->Id);
					}
					else
					{
						nodeDet->Codigo = "Código: " +acao->Eqpto()->Codigo;
						nodeDet->Codigo += " Id: " + IntToStr(acao->Eqpto()->Id);
                    }

				}
				//insere no map
				mapEqpto.insert(std::pair<VTEqpto*, TNodeDetalhe*>(acao->Eqpto(), nodeDet));
				//guarda a lista
				lisAcao = nodeDet->LisAcao;
				//guarda o node na lista Externa
//				lisEXT_nodeDetalhe->Add(nodeDet);
				node->InsereNode(nodeDet);
			}
			else //encontrou
			{
				nodeDet = it->second;
				//guarda a lista
				lisAcao = nodeDet->LisAcao;
			}
			//se encontrou uma listAcao
			if (lisAcao != NULL)
			{
                lisAcao->Add(acao);
			}
		}
	}
}

// ---------------------------------------------------------------------------
// eof
