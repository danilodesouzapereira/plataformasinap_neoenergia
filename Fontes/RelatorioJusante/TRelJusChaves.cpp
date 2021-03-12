// ----------------------------------------------------------------------------
#pragma hdrstop

#include <Fontes\Apl\VTApl.h>
#include <Fontes\Bloco\VTBloco.h>
#include <Fontes\Bloco\VTBlocos.h>
#include <Fontes\Bloco\VTElo.h>
#include <Fontes\Classes\VTClasse.h>
#include <Fontes\Patamar\VTPatamar.h>
#include <Fontes\Patamar\VTPatamares.h>
#include <Fontes\Potencia\VTDemanda.h>
#include <Fontes\Radial\VTArvore.h>
#include <Fontes\Radial\VTNode.h>
#include <Fontes\Rede\VTBarra.h>
#include <Fontes\Rede\VTChave.h>
#include <Fontes\Rede\VTCarga.h>
#include <Fontes\Rede\VTRedes.h>
#include <Fontes\Rede\VTTrafo.h>

#include "..\Forms\TFormRjChaves.h"
#include "..\..\DLL_Inc\Bloco.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Potencia.h"
#include "..\..\DLL_Inc\Radial.h"

#include "TInfoRJChave.h"
#include "TRelJusChaves.h"
// ----------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
int __fastcall CompareNodesNivelDesc(void *Item1, void *Item2)
{
	TInfoRJChave *item1, *item2;
	VTNode *node1, *node2;

	item1 = (TInfoRJChave*)Item1;
	item2 = (TInfoRJChave*)Item2;

	node1 = item1->nodeBlocoJus;
	node2 = item2->nodeBlocoJus;

	return (node2->Level - node1->Level); //descending
}
// ---------------------------------------------------------------------------
VTRelJusChaves* NewObjRelJusChaves(VTApl *apl)
{
	return (new TRelJusChaves(apl));
}

// ----------------------------------------------------------------------------
__fastcall TRelJusChaves::TRelJusChaves(VTApl *apl_owner)
{
	this->apl = NewObjApl(NULL,apl_owner);
	executado = false;
}

// ----------------------------------------------------------------------------
__fastcall TRelJusChaves::~TRelJusChaves(void)
{
	if(arvoreBlocos){
		delete arvoreBlocos;
		arvoreBlocos = NULL;
	}
	if (arvoreChaves) {
		LimpaArvoreChaves();
		delete arvoreChaves;
		arvoreChaves = NULL;
	}
}
// ----------------------------------------------------------------------------
void __fastcall TRelJusChaves::AgregaValores(int n_dias, int patamar)
{
	this->indPatamar = patamar;
	this->nDias = n_dias;

	if (executado)
	{
		//reinicia "estruturas"
		ClearArvoreChaves();
		//agrega os valores nas estruturas
		AgregaValores();
	}

	//TESTE
	//TForm *form = new TFormRjChaves(NULL,arvoreChaves);
	//form->Show();
}
// ----------------------------------------------------------------------------
bool __fastcall TRelJusChaves::AgregaValores(void)
{
	//TList *lisINFOCHAVE;
	TInfoRJChave *str;
	TList *lisNODE;
	VTNode *nodeRaiz;
	bool sucesso = true;

	//lisINFOCHAVE = new TList;
	//lisNODE = new TList;
	//arvoreChaves->LisAllObject(lisINFOCHAVE);
	lisNODE = arvoreChaves->LisNodeRaiz();

	//ordena de forma descendente de acordo com o nivel do node
	//lisINFOCHAVE->Sort(CompareNodesNivelDesc);
	//percorre a arvore de chaves
//	for (int ni =0; ni < lisINFOCHAVE->Count; ni++)
//	{
//		str = (TInfoRJChave*) lisINFOCHAVE->Items[ni];
//	}
	for (int nn =0; nn < lisNODE->Count; nn++)
	{
		nodeRaiz = (VTNode*) lisNODE->Items[nn];
		str = (TInfoRJChave*) nodeRaiz->Object;
		AgregaValoresJusante(str);
	}
    return sucesso;
}
// ----------------------------------------------------------------------------
void __fastcall TRelJusChaves::AgregaValoresCarga(TList *lisCARGA, TInfoRJChave *str)
{
	VTDemanda *demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
	VTCarga *carga;
	VTBarra *barra;

	str->consumo.n_dias = this->nDias;
	//percorre a lista de cargas e agrega os valores
	for(int nc = 0; nc < lisCARGA->Count; nc++)
	{
		carga = (VTCarga*) lisCARGA->Items[nc];
		barra = carga->pbarra;

		//classifica de acordo com a tensao
		if((barra->vnom > 69.0) || (IsDoubleZero(barra->vnom - 69.0)))
		{
//			str.at.qtde++;
//			str.at.P_mw += demanda->P_mw[carga][patamar->Index];
//			str.at.Q_mvar += demanda->Q_mvar[carga][patamar->Index];
//			str.at.S_mva += demanda->S_mva[carga][patamar->Index];
		}
		else if((barra->vnom > 1.0) || (IsDoubleZero(barra->vnom - 1.0)))
		{
			if (carga->classe->Tag == consEQUIVALENTE)
			{
				str->qntdeConsuMT.resid += carga->NumConsResidencial;
				str->qntdeConsuMT.comerc += carga->NumConsComercial;
				str->qntdeConsuMT.indust += carga->NumConsIndustrial;
			}
			else
			{	//verifica a classe da carga
				if (carga->classe->Tag == consRESIDENCIAL)
					{str->qntdeConsuMT.resid++;}
				else if (carga->classe->Tag == consCOMERCIAL)
					{str->qntdeConsuMT.comerc++;}
				else if (carga->classe->Tag == consINDUSTRIAL)
					{str->qntdeConsuMT.indust++;}
			}
		}
		else
		{
			if (carga->classe->Tag == consEQUIVALENTE)
			{
				str->qntdeConsuBT.resid += carga->NumConsResidencial;
				str->qntdeConsuBT.comerc += carga->NumConsComercial;
				str->qntdeConsuBT.indust += carga->NumConsIndustrial;
			}
			else
			{	//verifica a classe da carga
				if (carga->classe->Tag == consRESIDENCIAL)
					{str->qntdeConsuMT.resid++;}
				else if (carga->classe->Tag == consCOMERCIAL)
					{str->qntdeConsuMT.comerc++;}
				else if (carga->classe->Tag == consINDUSTRIAL)
					{str->qntdeConsuMT.indust++;}
			}
		}
		//verifica a classe da carga e consolida o consumo de acordo
		if (carga->classe->Tag == consRESIDENCIAL)
		{
			str->consumo.resid_kwh =  str->consumo.n_dias*demanda->Edia_mwh[carga];
		}
		else if (carga->classe->Tag == consCOMERCIAL)
		{                                         /* TODO : setar valor do dia */
			str->consumo.comerc_kwh =  str->consumo.n_dias*demanda->Edia_mwh[carga];
		}
		else if (carga->classe->Tag == consINDUSTRIAL)
		{
			str->consumo.indust_kwh =  str->consumo.n_dias*demanda->Edia_mwh[carga];
		}
		//contabiliza a demanda acumulada
		str->demandaAtivaAcum_kw += demanda->P_mw[carga][this->indPatamar];
		str->demandaReativaAcum_kvar += demanda->Q_mvar[carga][this->indPatamar];
	}
}
// ----------------------------------------------------------------------------
void __fastcall TRelJusChaves::AgregaValoresJusante(TInfoRJChave *str)
{
	VTNode *nodeJus;
	TList *lisNodeJus;
	TInfoRJChave *strFilha;

	lisNodeJus = new TList;

	// agrega os valores proprios do bloco
	ValoresProprios(str);
	//
	nodeJus = str->nodeChaveJus;
	nodeJus->LisNode(lisNodeJus);


	//soma os filhos
	if (lisNodeJus->Count > 0)
	{	//percorre os nodes e agrega os valores
		for (int nn = 0; nn < lisNodeJus->Count; nn++)
		{
			nodeJus = (VTNode*) lisNodeJus->Items[nn];
			strFilha = (TInfoRJChave*) nodeJus->Object;
			AgregaValoresJusante(strFilha);
			str->Add(strFilha);
		}
	}
	//deleta
	DestroiTObject(lisNodeJus);
}
// ----------------------------------------------------------------------------
void __fastcall TRelJusChaves::AgregaValoresTrafo(TList *lisTRAFO, TInfoRJChave *str)
{
	VTTrafo *trafo;

	for (int nt = 0; nt < lisTRAFO->Count; nt++)
	{
		trafo = (VTTrafo*) lisTRAFO->Items[nt];
		str->potenciaNomAcum_kva += trafo->snom;
	}
}
// ----------------------------------------------------------------------------
void __fastcall TRelJusChaves::ClearArvoreChaves(void)
{
	TList *lisStrRJChave;
	TInfoRJChave *str;
	TList *lisNode;
	VTNode *node;

	lisStrRJChave = new TList;
	lisNode = new TList;
	arvoreChaves->LisAllNode(lisNode);
	//arvoreChaves->LisAllObject(lisStrRJChave);
	//lista todos objetos dos nodes
	for (int nn = 0; nn < lisNode->Count; nn++)
	{
		node = (VTNode*)lisNode->Items[nn];
		lisStrRJChave->Add(node->Object);
	}
   	//percorre a lista de objetos
	for(int ns = (lisStrRJChave->Count - 1); ns >= 0; ns--)
	{
		str = (TInfoRJChave*) lisStrRJChave->Items[ns];
		str->Clear();
	}
	delete lisStrRJChave;
}
// ----------------------------------------------------------------------------
bool __fastcall TRelJusChaves::Inicia(void)
{
	bool sucesso = false;
	//this->indPatamar = patamar;

	//inicia arvore de blocos
	IniciaArvoreBlocos();
	//inicia arvore de chaves
	IniciaArvoreChaves();
	//Ordena as chaves na arvore
	sucesso = OrdenaChaves();
	//guarda situacao da execucao
	this->executado = sucesso;

	return sucesso;
}
// ----------------------------------------------------------------------------
VTNode* __fastcall TRelJusChaves::ExisteNodeBloco(VTBloco *bloco)
{
	//VTArvore    *arvore    = (VTArvore*)apl->GetObject(__classid(VTArvore));
	TList       *listAUX;
	VTNode 		*node;

	if((arvoreBlocos == NULL) || (bloco == NULL))
		{return NULL;}
	listAUX = new TList;
	arvoreBlocos->LisAllNode(listAUX);
	for (int nn = 0; nn < listAUX->Count; nn++)
	{
		node = (VTNode*) listAUX->Items[nn];
		if(node->Object == bloco)
		{
			break;
		}
		else
		{
			node = NULL;
		}
	}
	//destroi lista
	DestroiTObject(listAUX);
	return node;
}
// ----------------------------------------------------------------------------
int __fastcall TRelJusChaves::IniciaArvoreBlocos(void)
{	//variáveis locais

   blocos    = (VTBlocos*)apl->GetObject(__classid(VTBlocos));
   VTBloco     *bloco_jus, *bloco_mon;
   VTElo       *elo,       *elo_pai;
   VTRedes     *redes     = (VTRedes*)apl->GetObject(__classid(VTRedes));
   VTNode      *node;
   TList       *lisNODE;

	if(blocos == NULL)
	{
		apl->Add(blocos = DLL_NewObjBlocos());
	}
   //inicia arvore
   if(arvoreBlocos) {delete arvoreBlocos;}
   arvoreBlocos = DLL_NewObjArvore(apl);
   //proteção
   if((! arvoreBlocos) || (! blocos))
		{return 2;}
   if(! blocos->Executa(redes))
		{return 3;}
   //elimina Elos cujas Chaves estão abertas
   if (! blocos->EliminaElosComChaveAberta())
		{return 4;}
   //elimina Blocos isolados ou conectados apenas por Chaves abertas
   if (! blocos->EliminaBlocosNaoEnergizados())
		{return 5;}
   //elimina Elos que conectam um mesmo Bloco
   if (! blocos->EliminaElosComBlocosIguais())
		{return 6;}
   if(! arvoreBlocos->Inicia(blocos))
		{return 7;}

   return(1);

}
// ----------------------------------------------------------------------------
void __fastcall TRelJusChaves::IniciaArvoreChaves(void)
{	//inicia arvore
	if(arvoreChaves) {delete arvoreChaves;}
	arvoreChaves = DLL_NewObjArvore(apl);
}
// ----------------------------------------------------------------------------
void __fastcall  TRelJusChaves::InsereNodesChaveFilhas(VTBloco *pai, VTNode *nodeChavePai)
{
	VTNode *nodePai;
	VTNode *nodeFilho;
	VTBloco *blocoFilho;
	VTElo  *elo;
	VTChave *chave;
	TList *lisNodeFilho;
	VTNode *nodeChaveFilha;
	TInfoRJChave *str;

	try
	{
		//protecao
		if (pai == NULL)
			{return;}
		nodePai = ExisteNodeBloco(pai);
		lisNodeFilho = nodePai->LisNode();
		//percorre a lista de nodes
		for (int nn = 0; nn < lisNodeFilho->Count; nn++)
		{
			nodeFilho = (VTNode*) lisNodeFilho->Items[nn];
			blocoFilho = (VTBloco*)nodeFilho->Object;
			elo = blocos->ExisteElo(pai,blocoFilho);
			chave = elo->Chave;
			if (chave != NULL)
			{	//insere node da chave
				nodeChaveFilha = arvoreChaves->InsereNode(nodeChavePai);
				nodeChaveFilha->Codigo = chave->Codigo;
				str = new TInfoRJChave;
				nodeChaveFilha->Object = str;
				str->chave = chave;
				str->blocoJus = blocoFilho;
				str->nodeBlocoJus = nodeFilho;
				str->nodeChaveJus = nodeChaveFilha;
				//insere filhas dessa chave
				InsereNodesChaveFilhas(blocoFilho, nodeChaveFilha);
			}
		}
	}
	catch(Exception &e)
	{
		//esconde o problema...
		//mentira, debug
		int a = 0;
    }
}

// ----------------------------------------------------------------------------
void __fastcall TRelJusChaves::LimpaArvoreChaves(void)
{
	TList *lisStrRJChave;
	TInfoRJChave *str;

	lisStrRJChave =new TList;
	arvoreChaves->LisAllObject(lisStrRJChave);

	for(int ns = (lisStrRJChave->Count - 1); ns >= 0; ns--)
	{
		str = (TInfoRJChave*) lisStrRJChave->Items[ns];
		lisStrRJChave->Remove(str);
		delete str;
	}

	delete lisStrRJChave;
}
// ----------------------------------------------------------------------------
void __fastcall TRelJusChaves::ValoresProprios(TInfoRJChave *str)
{
	TList *lisCARGAS;
	TList *lisTRAFOS;

	lisCARGAS = new TList;
	lisTRAFOS = new TList;
	//lista as cargas do bloco a jusante da chave
	str->blocoJus->LisEqbar(lisCARGAS, eqptoCARGA);
	//lista os trafos do bloco a jusante da chave
	str->blocoJus->LisLigacao(lisTRAFOS, eqptoTRAFO);
	//carrega as cargas proprias na lista
	str->lisCargas->Assign(lisCARGAS,laCopy);
	str->lisTrafos->Assign(lisTRAFOS,laCopy);
	AgregaValoresCarga(lisCARGAS, str);
	AgregaValoresTrafo(lisTRAFOS, str);
}
// ----------------------------------------------------------------------------
bool __fastcall TRelJusChaves::OrdenaChaves(void)
{
	TList *lisNodeRaiz;
	VTNode *nodeRaiz;
	VTBloco *blocoPai;
	bool sucesso = false;

	try
	{
		lisNodeRaiz = arvoreBlocos->LisNodeRaiz();
		for (int nr =0; nr < lisNodeRaiz->Count; nr++)
		{
			nodeRaiz = (VTNode*) lisNodeRaiz->Items[nr];
			blocoPai = (VTBloco*) nodeRaiz->Object;
			InsereNodesChaveFilhas(blocoPai ,NULL);
		}
		sucesso = true;
	}
	catch (Exception &e)
	{
		sucesso = false;
	}

	return sucesso;
}
// ----------------------------------------------------------------------------
