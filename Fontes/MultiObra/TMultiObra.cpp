//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TMultiObraBO.h"
#include "TMultiObra.h"
#include "VTNodeDetalhe.h"
#include "TAvaliaAcoes.h"
//---------------------------------------------------------------------------
#include <PlataformaSinap\Fontes\Obra\VTObra.h>
#include <PlataformaSinap\Fontes\Obra\VTAcao.h>
#include <PlataformaSinap\Fontes\Obra\ItemObra\VTItemObra.h>
#include <PlataformaSinap\Fontes\Radial\VTArvore.h>
//---------------------------------------------------------------------------
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Radial.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTMultiObra* __fastcall NewObjMultiObra(VTApl *apl)
{
	try
	{
		return (new TMultiObra(apl));
	}catch (Exception &e)
	{
		return(NULL);
	}
}

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/15938942
__fastcall TMultiObra::TMultiObra(VTApl *apl)
{
    this->apl = apl;
	//cria listas
//	lisItemObra   = new TList();
	listItemObraAux = new TList();
	arvoreDetalhes = DLL_NewObjArvore(apl);
	obra_ref = NULL;
    uptodate = true;
	todos_configurados = false;
}

//---------------------------------------------------------------------------
__fastcall TMultiObra::~TMultiObra(void)
{
	//destroi os objetos que sobraram na lista, os items "bons" devem ir pro VTObra
//	if (lisItemObra)
//	{
//		LimpaTList(lisItemObra);
//		delete lisItemObra;
//		lisItemObra = NULL;
//	}
	//destrói lista sem destruir seus objetos
	if (listItemObraAux)
	{
		delete listItemObraAux;
        listItemObraAux = NULL;
	}
	if (arvoreDetalhes) {delete arvoreDetalhes; arvoreDetalhes = NULL;}
}
//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/15938942
bool __fastcall TMultiObra::AnalisaObra(VTObra *obra)
{
	bool sucesso = true;
	VTNodeDetalhe *nodeDet = NULL;

	obra_ref = obra;
	try
	{
		//reinicia a classe
//		Reinicia();
		sucesso = MontaArvore(obra);
		//verifica se essa obra ja possui items obra, se possui, ela foi carregada da base
		if (obra_ref->LisItemObra->Count != 0)
		{
			AssociaItemObraAoNodesDet();
		}
		else
		{
			//adiciona ItemObra ao node raiz
			if ((Arvore != NULL) && (Arvore->LisNodeRaiz() != NULL))
			{
				for (int nn = 0; nn < Arvore->LisNodeRaiz()->Count; nn++)
				{
					nodeDet = (VTNodeDetalhe*)Arvore->LisNodeRaiz()->Items[nn];
					nodeDet->Estado = eND_PENDENTE;
					InsereItemObra(nodeDet);
				}
			}
		}
		//apos isso, percorre todos items os "inicializando"
		InicializaItemObra();
		//acerta o estado dos nodes
		AtualizaEstadosNodes();
		//acerta o status
		this->UpToDate = true;
	}
	catch(...)
	{
        sucesso = false;
    }

	return sucesso;
}
//---------------------------------------------------------------------------
bool __fastcall TMultiObra::AtualizaEstadoNode(VTNode *nodeDetalhe)
{
	bool pendente = false;
	VTNodeDetalhe *nodeDet, *nodeAux;
	VTItemObra *itemObra;


	nodeDet = (VTNodeDetalhe*) nodeDetalhe;
	try
	{
		//primeiro atualiza os filhos
		for (int nn = 0; nn < nodeDet->LisNode()->Count; nn++)
		{
			nodeAux = (VTNodeDetalhe*)nodeDet->LisNode()->Items[nn];
			//se nao esta habilitado nem olha
			if (!nodeAux->Habilitado)
				{continue;}
			pendente |= AtualizaEstadoNode(nodeAux);
		}
		//atualiza si proprio
		//caso ele esteja habilitado
		if (nodeDet->Habilitado)
		{
			//se tem item obra é ativo
			if (nodeDet->ItemObra != NULL)
			{
				//se nao vai ser contabilizado
				if (!nodeDet->ItemObra->Contabilizar)
				{
					nodeDet->Estado = eND_NAOCONTABILIZADO;
				}
				//verifica se o item foi finalizado
				else if (nodeDet->ItemObra->Configurado)
				{
					nodeDet->Estado = eND_CONFIGURADO;
                }
				else
				{
					nodeDet->Estado = eND_PENDENTE;
					pendente = true;
                }
			}
			//se é um pai inativo
			else
			{
				//depende dos filhos
				if (pendente)
				{
					nodeDet->Estado = eND_PENDENTE;
				}
				else
				{
                    nodeDet->Estado = eND_CONFIGURADO;
                }
            }
		}
		else
		{
			nodeDet->Estado = eND_NAOCONTABILIZADO;
        }
	}
	catch(...)
	{

	}

	return pendente;
}
//---------------------------------------------------------------------------
bool __fastcall TMultiObra::AtualizaEstadosNodes(void)
{
	bool sucesso = true;
	VTNodeDetalhe *nodeAux;

	try
	{
        todos_configurados = true;
		//percorre a arvore
		for (int nn = 0; nn < Arvore->LisNodeRaiz()->Count; nn++)
		{
			nodeAux = (VTNodeDetalhe*) Arvore->LisNodeRaiz()->Items[nn];
			AtualizaEstadoNode(nodeAux);
			todos_configurados &= (nodeAux->Estado != eND_PENDENTE);
		}
//		obra_ref->Finalizada = todosProntos;

	}
	catch(...)
	{
		sucesso = false;
	}

	return sucesso;
}
//---------------------------------------------------------------------------
bool __fastcall  TMultiObra::AssociaItemObraAoNodesDet(void)
{
	bool sucesso = true;
	VTNodeDetalhe *nodeDet = NULL;

	//protecao
	if (obra_ref == NULL)
	{
		return false;
	}
	if (obra_ref->LisItemObra == NULL)
	{
        return false;
	}
	//limpa a lista auxiliar
	listItemObraAux->Clear();
	//adiciona todos itemObra preexistentes da obra na lista auxiliar
	listItemObraAux->Assign(obra_ref->LisItemObra, laCopy);

	//percorre a arvore verificando de o node possui as mesmas acoes do item
	if ((Arvore != NULL) && (Arvore->LisNodeRaiz() != NULL))
	{
		for (int nn = 0; nn < Arvore->LisNodeRaiz()->Count; nn++)
		{
			nodeDet = (VTNodeDetalhe*)Arvore->LisNodeRaiz()->Items[nn];
			AssociaItemObraAoNodesDet(nodeDet);
		}
	}
	//arruma os nodes folhas q deveriam ter items
	CriaItemObraFaltantes();
//    //acerta o estado dos nodes
//	AtualizaEstadosNodes();
	//remove os items que sobraram na lista auxiliar da obra
	RemoveEDestroiAdeB(listItemObraAux, obra_ref->LisItemObra);


	return sucesso;
}
//---------------------------------------------------------------------------
bool __fastcall  TMultiObra::AssociaItemObraAoNodesDet(VTNode *nodeDetalhe)
{
//	bool sucesso = false;
	bool possui = false;
	VTItemObra *item;
	VTNode *nodeFilho;
	VTNodeDetalhe *detAux;

	//protecao
	if (nodeDetalhe == NULL)
	{
		return false;
	}

	//verifica se algum dos itemsObra possui as mesma acoes
	for (int ni = listItemObraAux->Count-1; ni >= 0 ; ni--)
	{
		item = (VTItemObra*)listItemObraAux->Items[ni];
		//se possuir
		if (MesmasAcoes(nodeDetalhe,item))
		{   //associa ao node
			InsereItemObra(nodeDetalhe,item);
			//remove o item da lista
			listItemObraAux->Remove(item);
			possui = true;
			//habilita o node
			detAux = (VTNodeDetalhe*)nodeDetalhe;
			detAux->Habilitado = true;
			//sai do loop
            return true;
		}
	}
	//se nao encontrou
	if (!possui)
	{
		//é porque deve ser de algum filho
		for (int nn = 0; nn < nodeDetalhe->LisNode()->Count; nn++)
		{
			nodeFilho = (VTNode*) nodeDetalhe->LisNode()->Items[nn];
			//se um dos filhos possuir, expande e abiita
			possui |= AssociaItemObraAoNodesDet(nodeFilho);
		}
	}
	if (possui)
	{
		//se um descendente possuir
		//habilita o node
		detAux = (VTNodeDetalhe*)nodeDetalhe;
		detAux->Habilitado = true;
        detAux->Expandido = true;
	}


	return possui;
}
//---------------------------------------------------------------------------
bool __fastcall TMultiObra::CriaItemObraFaltantes(void)
{
    bool sucesso = true;
	VTNodeDetalhe *nodeDet = NULL;

	//protecao
	if (obra_ref == NULL)
	{
		return false;
	}
	if (obra_ref->LisItemObra == NULL)
	{
        return false;
	}
	//percorre a arvore verificando se precisa de item
	if ((Arvore != NULL) && (Arvore->LisNodeRaiz() != NULL))
	{
		for (int nn = 0; nn < Arvore->LisNodeRaiz()->Count; nn++)
		{
			nodeDet = (VTNodeDetalhe*)Arvore->LisNodeRaiz()->Items[nn];
			CriaItemObraFaltantes(nodeDet);
		}
	}
    return sucesso;
}
//---------------------------------------------------------------------------
bool __fastcall TMultiObra::CriaItemObraFaltantes(VTNode *nodeDetalhe)
{
	bool sucesso = false;
	bool possui = false;
	VTItemObra *item;
	VTNode *nodeFilho;
	VTNodeDetalhe *detAux, *detAuxFilho;

	try
	{
        //protecao
		if (nodeDetalhe == NULL)
		{
			return false;
		}
		detAux = (VTNodeDetalhe*)nodeDetalhe;
		//se nao estiver expand= (VTNodeDetalhe*)ido nem habilitado, cria
		if ((detAux->Habilitado == false)&&(detAux->Expandido == false))
		{
			InsereItemObra(detAux);
		}
		else
		{   //se algum filho estiver habilitado, todos deviam estar
			for (int nn = 0; nn < detAux->LisNode()->Count; nn++)
			{
				detAuxFilho = (VTNodeDetalhe*)detAux->LisNode()->Items[nn];
				possui |= detAuxFilho->Habilitado;
			}
			if (possui)
			{   //se tem filho habilitado, ele esta habilitado e expandido
				detAux->Habilitado = true;
                detAux->Expandido = true;
				//altera todos pra true
                for (int nn = 0; nn < detAux->LisNode()->Count; nn++)
				{
					detAuxFilho = (VTNodeDetalhe*)detAux->LisNode()->Items[nn];
					detAuxFilho->Habilitado = true;
					//e verifica
					CriaItemObraFaltantes(detAuxFilho);
				}
			}
			else
			{ //se nao possui filho habilitado e chegou aqui, deve ter item
				if(detAux->ItemObra == NULL)
				{
					InsereItemObra(detAux);
				}
			}
        }

	}
	catch(Exception &e)
	{
        AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
		sucesso = false;
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
		 sucesso = false;
	}

	return sucesso;
}
//---------------------------------------------------------------------------
void __fastcall TMultiObra::DestroiItemObra(VTNode *node)
{
	VTNodeDetalhe *nodeDet;
	VTItemObra *itemObra;

	if (node == NULL)
		{return;}
	nodeDet = (VTNodeDetalhe*)node;
	if(nodeDet->ItemObra != NULL)
	{
		//remove da lista
		if (obra_ref != NULL)
		{
			obra_ref->DeleteItemObra(nodeDet->ItemObra);
		}
		nodeDet->ItemObra = NULL;
//		nodeDet->Habilitado = false;
	}
}
//---------------------------------------------------------------------------
void __fastcall TMultiObra::DestroiItemObraJusante(VTNode *node)
{
	VTNodeDetalhe *nodeDet;
	VTItemObra *itemObra;

	if (node == NULL)
		{return;}
	for (int nn; nn < node->LisNode()->Count; nn++)
	{
		nodeDet = (VTNodeDetalhe*)node->LisNode()->Items[nn];
		//destroi e deixa o nulo o ponteiro
		DestroiItemObra(nodeDet);
		//verifica os filhos
		DestroiItemObraJusante(nodeDet);
        nodeDet->Habilitado = false;
	}
}
//---------------------------------------------------------------------------
void __fastcall TMultiObra::DestroiItemObraMontante(VTNode *node)
{
	VTNodeDetalhe *nodeDet;
	VTItemObra *itemObra;

	if (node == NULL)
		{return;}

	//verifica pai
	nodeDet = (VTNodeDetalhe*)node->Pai;
	if (nodeDet != NULL)
	{
		//destroi e deixa o nulo o ponteiro
		DestroiItemObra(nodeDet);
		//verifica os avós
		DestroiItemObraMontante(nodeDet);
		nodeDet->Habilitado = true;
	}
}
//---------------------------------------------------------------------------
bool __fastcall TMultiObra::ContraiNode(VTNode *nodeDetalhe)
{
    VTItemObra *itemObraNovo = NULL;
	VTItemObra *itemObraAux = NULL;
	VTItemObra *itemObraBase = NULL;
	VTNodeDetalhe *nodeDet, *nodeDetAux;
	bool sucesso = true;

	try
	{
		//verifica o pai do node
		nodeDet = (VTNodeDetalhe*) nodeDetalhe->Pai;
		//protecao
		if (nodeDet == NULL)
			{return false;}
		if (obra_ref == NULL)
			{return false;}
		nodeDet->Expandido = false;
		//cria um ItemObra novo
		//cria um item pra cada
		if (obra_ref != NULL)
		{
			itemObraNovo = obra_ref->CriaItemObra();
			nodeDet->ItemObra = itemObraNovo;
			//adiciona na lista
			obra_ref->AddItemObra(itemObraNovo);
//			lisItemObra->Add(itemObraNovo);
			nodeDet->Habilitado = true;
			//copia a lista de acoes do node no item
			itemObraNovo->LisAcao->Assign(nodeDet->LisAcao,laCopy);
		}
		//copia os atributos comuns dos filhos
		for(int nn = 0; nn < nodeDet->LisNode()->Count; nn++)
		{
			nodeDetAux = (VTNodeDetalhe*)nodeDet->LisNode()->Items[nn];
			itemObraAux = nodeDetAux->ItemObra;
			//se for nulo pula
			if(itemObraAux == NULL)
				{continue;}
			//copia os atributos
			if (itemObraBase == NULL)
			{
				itemObraBase = itemObraAux;
				itemObraNovo->CopiaAtributosDe(*itemObraBase);
				itemObraNovo->CorrigeValores(nodeDet->TipoEqpto);
				itemObraNovo->Nome = obra_ref->Nome + " " + nodeDet->Codigo;
				itemObraNovo->Configurado = false;
                itemObraNovo->Contabilizar = true;
			}
			else
			{   //mantem os atributos comuns
				itemObraNovo->MantemAtributosComunsDe(*itemObraAux);
			}
		}
		//destroi os itemObra dos filhos
		DestroiItemObraJusante(nodeDet);

		//atualiza o estado dos nodes
		AtualizaEstadosNodes();
	}
    catch(Exception &e)
	{
        AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
		sucesso = false;
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
		 sucesso = false;
	}
	return sucesso;
}
//---------------------------------------------------------------------------
bool __fastcall TMultiObra::ExpandeNode(VTNode *nodeDetalhe)
{
	VTItemObra *itemObraNovo = NULL;
	VTItemObra *itemObraAux = NULL;
	VTNodeDetalhe *nodeDet, *nodeDetAux;
	bool sucesso = true;

	try
	{
		//verifica se o node ja possui ItemObra
		nodeDet = (VTNodeDetalhe*) nodeDetalhe;
		//protecao
		if (obra_ref == NULL)
			{return false;}
		if (nodeDet == NULL)
			{return false;}
		nodeDet->Expandido = true;
		if (nodeDet->ItemObra != NULL)
		{   //guarda as infos do padrao ancestral
			itemObraAux = nodeDet->ItemObra;
		}
		//cria novo ItemObra pra todos os nodes Filhos copiando os atributos
		for(int nn = 0; nn < nodeDet->LisNode()->Count; nn++)
		{
			nodeDetAux = (VTNodeDetalhe*)nodeDet->LisNode()->Items[nn];
			//cria um item pra cada
			if (obra_ref != NULL)
			{
				itemObraNovo = obra_ref->CriaItemObra();
				nodeDetAux->ItemObra = itemObraNovo;
				//adiciona na lista
//				lisItemObra->Add(itemObraNovo);
				obra_ref->AddItemObra(itemObraNovo);
				nodeDetAux->Habilitado = true;
				//copia os atributos
				if (itemObraAux != NULL)
				{
					itemObraNovo->CopiaAtributosDe(*itemObraAux);
					itemObraNovo->CorrigeValores(nodeDetAux->TipoEqpto);
					itemObraNovo->Nome = obra_ref->Nome + " " + nodeDetAux->Codigo;
				}
				//copia a lista de acoes do node no item
				itemObraNovo->LisAcao->Assign(nodeDetAux->LisAcao,laCopy);
			}
		}
		//destroi os itemObra dos antecessores
		DestroiItemObraMontante(nodeDetAux);

		//atualiza o estado dos nodes
		AtualizaEstadosNodes();
	}
	catch(Exception &e)
	{
        AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
		sucesso = false;
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
		 sucesso = false;
	}

	return sucesso;
}
//---------------------------------------------------------------------------
bool __fastcall TMultiObra::FinalizaObra(void)
{
	VTItemObra *item;
	bool sucesso = true;

	try
	{
		if (obra_ref != NULL)
		{
			//percorre a lis dos itens criados
			for (int ni = obra_ref->LisItemObra->Count - 1; ni >= 0; ni--)
			{
				item = (VTItemObra*)obra_ref->LisItemObra->Items[ni];
				//salva os parametros dos items em ipeAtributos
				item->SalvaIpeAtributos();
//				//transfere
//				if(obra_ref->LisItemObra->IndexOf(item)<0)
//				{
//					obra_ref->LisItemObra->Add(item);
//				}
//				//remove
//				this->LisItemObra->Delete(ni);
			}
		}
	}
	catch(...)
	{
		sucesso = false;
	}

	return sucesso;
}
//---------------------------------------------------------------------------
bool __fastcall TMultiObra::HabilitaItemObra(VTItemObra *itemObra, bool habilita)
{
	//protecao
	if (itemObra == NULL)
		{return false;}
	itemObra->Contabilizar = habilita;
	itemObra->Configurado = !habilita;
	//atualiza os estados
	AtualizaEstadosNodes();

	return true;
}
//---------------------------------------------------------------------------
void __fastcall TMultiObra::InicializaItemObra(void)
{
	VTItemObra *item;

	for (int ni = 0; ni < obra_ref->LisItemObra->Count; ni++)
	{
		item = (VTItemObra*) obra_ref->LisItemObra->Items[ni];
        item->LeIpeAtributos();
	}
	//acerta estado dos nodes
//	AtualizaEstadosNodes();
}
//---------------------------------------------------------------------------
bool __fastcall  TMultiObra::MesmasAcoes(VTNode *nodeDet, VTItemObra *itemObra)
{
	bool todasIguais = true;
	VTAcao *acao;
	VTNodeDetalhe *detalhe;

	detalhe = (VTNodeDetalhe*) nodeDet;
	//protecao
	if (detalhe == NULL)
		{return false;}
	if (itemObra == NULL)
		{return false;}
	if (detalhe->LisAcao == NULL)
		{return false;}
	if (itemObra->LisAcao == NULL)
		{return false;}
	//verifica se tem o mesmo numero de acoes (arriscado?)
	if (detalhe->LisAcao->Count != itemObra->LisAcao->Count)
		{return false;}
	//olha se cada acao pertence a outra
	for (int na = 0; na < detalhe->LisAcao->Count; na++)
	{
		acao = (VTAcao*)detalhe->LisAcao->Items[na];
		if (itemObra->LisAcao->IndexOf(acao)< 0 )
			{return false;}
	}

	return  todasIguais;
}
//---------------------------------------------------------------------------
bool __fastcall TMultiObra::MontaArvore(VTObra *obra)
{
	bool sucesso = true;
	TList *lisAcao;
	TList *lisNodeRaiz;
	TAvaliaAcoes *avalia;

	//protecao
	if (obra == NULL)
		{return false;}
	try
	{
		lisAcao = obra->LisAcao;
		avalia = new TAvaliaAcoes(apl);
		//passa a arvore para ser preenchida
		sucesso = avalia->MontaArvore(lisAcao,arvoreDetalhes);
	}
	catch (...)
	{
		sucesso = false;
	}

	delete avalia;

    return sucesso;
}
//---------------------------------------------------------------------------
bool __fastcall TMultiObra::InsereItemObra(VTNode *node)
{
	VTItemObra *itemObraNovo;
	VTItemObra *itemObraAux;
	VTNodeDetalhe *nodeDetalhe;
	bool sucesso = false;

	//verifica se o node ja possui ItemObra
	nodeDetalhe = (VTNodeDetalhe*) node;
	if (nodeDetalhe->ItemObra != NULL)
	{
		DestroiItemObra(node);
	}
	if (obra_ref != NULL)
	{
		itemObraNovo = obra_ref->CriaItemObra();
		itemObraNovo->Nome = obra_ref->Nome + " " + node->Codigo;
		//adiciona na lista
		sucesso = InsereItemObra(node, itemObraNovo);
	}

    return sucesso;
}
//---------------------------------------------------------------------------
bool __fastcall  TMultiObra::InsereItemObra(VTNode *node, VTItemObra *itemObra)
{
    bool sucesso = false;
	VTNodeDetalhe * nodeDetalhe;

	//protecao
	if (node == NULL)
	{
		return false;
	}
	if (itemObra == NULL)
	{
        return false;
	}
	//verifica se o node ja possui ItemObra
	nodeDetalhe = (VTNodeDetalhe*) node;
	if (nodeDetalhe->ItemObra != NULL)
	{
		DestroiItemObra(node);
	}
	if (obra_ref != NULL)
	{
		//adiciona na lista
		obra_ref->AddItemObra(itemObra);
		itemObra->LisAcao->Assign(nodeDetalhe->LisAcao,laCopy);
		nodeDetalhe->Habilitado = true;
		nodeDetalhe->ItemObra = itemObra;
        itemObra->TipoAcao = nodeDetalhe->TipoAcao;
		sucesso = true;
	}

	return sucesso;
}
//---------------------------------------------------------------------------
void __fastcall TMultiObra::Reinicia(void)
{
	VTItemObra *item;

	arvoreDetalhes->Clear();
    //destroi objetos ItemAries
//	lisItemObra->Clear();
	if (obra_ref != NULL)
	{
		for (int ni = (obra_ref->LisItemObra->Count - 1); ni >= 0; ni--)
		{
			item = (VTItemObra*)obra_ref->LisItemObra->Items[ni];
            obra_ref->DeleteItemObra(item); // faço assim para passar pelo deleteItemObra
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TMultiObra::RemoveEDestroiAdeB(TList *lisItemA, TList *lisItemB)
{
	VTItemObra *item;

	//protecao
	if (lisItemA == NULL)
	{
		return;
	}
	if (lisItemB == NULL)
	{
		return;
	}
	for (int i = lisItemA->Count-1; i >=0; i--)
	{
		item = (VTItemObra*)lisItemA->Items[i];
		if (lisItemB->IndexOf(item)>=0)
		{ //encontrou em B
			lisItemB->Remove(item);
		}
		//remove de A tb
		lisItemA->Delete(i);
		//destroi o item
		delete item;
        item = NULL;
	}
}
//---------------------------------------------------------------------------
//eof

