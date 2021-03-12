// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DateUtils.hpp>
#include <Filectrl.hpp>
#include "TValFasesMalha.h"
#include "TMalhaFases.h"
#include "..\..\Apl\VTApl.h"
#include "..\..\Arranjo\VTArranjo.h"
#include "..\..\Arranjo\VTArranjos.h"
#include "..\..\Arranjo\VTCabo.h"
#include "..\..\Arranjo\VTPtoFixacao.h"
#include "..\..\Arranjo\VTSuporte.h"
#include "..\..\Diretorio\VTPath.h"
#include "..\..\Log\VTLog.h"
#include "..\..\MontaZ\VTMontaZ.h"
#include "..\..\Progresso\VTProgresso.h"
#include "..\..\Radial\VTArvore.h"
#include "..\..\Radial\VTMalha.h"
#include "..\..\Radial\VTMalhas.h"
#include "..\..\Radial\VTNode.h"
#include "..\..\Rede\VTBarra.h"
#include "..\..\Rede\VTCapacitor.h"
#include "..\..\Rede\VTCarga.h"
#include "..\..\Rede\VTChave.h"
#include "..\..\Rede\VTEqpto.h"
#include "..\..\Rede\VTFases.h"
#include "..\..\Rede\VTFiltro.h"
#include "..\..\Rede\VTLigacao.h"
#include "..\..\Rede\VTMotor.h"
#include "..\..\Rede\VTRede.h"
#include "..\..\Rede\VTRedes.h"
#include "..\..\Rede\VTRegulador.h"
#include "..\..\Rede\VTSuprimento.h"
#include "..\..\Rede\VTTipoRede.h"
#include "..\..\Rede\VTTrafo.h"
#include "..\..\Rede\VTTrafo3E.h"
#include "..\..\Rede\VTTrecho.h"
#include "..\..\Topologia\VTTopologia.h"
#include "..\..\..\DLL_Inc\Arranjo.h"
#include "..\..\..\DLL_Inc\Funcao.h"
#include "..\..\..\DLL_Inc\Log.h"
#include "..\..\..\DLL_Inc\Radial.h"
#include "..\..\..\DLL_Inc\Rede.h"

// ---------------------------------------------------------------------------
VTValFases* __fastcall NewObjValFasesMalha(VTApl *apl)
{
	return (new TValFasesMalha(apl));
}

// ---------------------------------------------------------------------------
__fastcall TValFasesMalha::TValFasesMalha(VTApl *apl) : TValFases(apl)
{
	arvore = DLL_NewObjArvore(apl);
	malhas = DLL_NewObjMalhas(apl);
	// cria uma lista para colocar os nodes as malhas
	lisMalhasRede = new TList;
	lisLigacoesRede = new TList;
	lisBarrasRede = new TList;
	lisBARRAORD = new TList;
	lisLIGACAOORD = new TList;
	lisMALHAFASES = new TList;
	plog = (VTLog*)apl->GetObject(__classid(VTLog));
	if (plog == NULL)
	{
		logCriado = true;
		//cria o log
		plog = DLL_NewObjLog(apl);
		ConfiguraLog();
	}
	else
		{logCriado = false;}

}

// ---------------------------------------------------------------------------
__fastcall TValFasesMalha::~TValFasesMalha(void)
{
	DestroiTObject(arvore);
	DestroiTObject(malhas);
	if (logCriado)
	{
		DestroiTObject(plog);
	}
	DestroiTObject(lisLigacoesRede);
	DestroiTObject(lisMalhasRede);
	DestroiTObject(lisBarrasRede);
	DestroiTObject(lisBARRAORD);
	DestroiTObject(lisLIGACAOORD);
	LimpaTList(lisMALHAFASES);
	DestroiTObject(lisMALHAFASES);
}

// ---------------------------------------------------------------------------
void __fastcall TValFasesMalha::AlteraFasesRegulador(VTRegulador *reg, int fasesNovas, VTBarra *ref)
{
	int fasesAntigas;
	int numFasesNovas;
	int caracteristicaNT = 0;
	int ligacaoAntiga;

	//guarda se o regulador tinha neutro
	caracteristicaNT = reg->Fases(ref) & faseNT;
	//verifica o numeor de fases exigidas
	numFasesNovas = fases->NumeroDeFases(fasesNovas);
	//guarda o tipo de ligacao
	ligacaoAntiga = reg->ligacao;
	switch (numFasesNovas)
	{
		case 3: //trifasico
			//verifica se as fases novas exige neutro
			if ((fasesNovas & faseN) == faseN)
			{
				reg->ligacao = lgEST_AT;
			}
			else
			{   //verifica a ligacao antiga, se for diferente de deltas ou estrela aterrado..
				switch (ligacaoAntiga)
				{   //sendo alguma trifasica sem neutro, faz nada (mantem a ligacao)
					case lgDELTA_FECHADO:
					case lgDELTA_ABERTO:
					case lgTRIANG:
					case lgEST_ISO:
						break;
					//sendo alguma diferente
					default:
						reg->ligacao = lgEST_ISO;
					;
				}
			}
			reg->fases = fasesNovas;
			break;
		default://bifasico e monofasico
			reg->ligacao = lgMONOFASICO;
			fasesNovas |= faseN;
			reg->fases = fasesNovas;
			break;
        ;
	}
}
// ---------------------------------------------------------------------------
void __fastcall TValFasesMalha::AnalisaMalha(VTLigacao *ligacaoPai, VTMalha *malha, TList *lisAnalisarPai)
{
	VTLigacao *ligacao_pai, *ligacao_filha, *ligAux;
	VTBarra *barra, *barraPai;
	VTNode *node, *nodePai, *nodeFilho;
	VTMalha *malhaAux;
	VTBarra *barraAux, *barraFilha;
	TList *lisLigMalha = new TList;
	TList *lisLIGANALISAR = new TList;
	TMalhaFases *malhaFases;

	lisLigMalha->Assign(malha->LisLigacao());
	//cria o node da malha
	node = DLL_NewObjNode();
	nodePai = (VTNode*)ligacaoPai->Obj;
	nodePai->InsereNode(node);
	arvore->InsereNode(nodePai, node);
	// Essa analise visa transformar a malha em 1 node e identificar os pontos
	// de conexao dessa rede com o resto da rede
	//1o passo eh ver todas ligacoes da malha que ligam à barra filha
	for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
	{
		barraAux = ligacaoPai->Barra(ind_bar);
		if (barraAux->Obj == ligacaoPai)
		{
			barraFilha = barraAux;
			barraFilha->Tag |= barraFRONTEIRAPAI; //marca a barra como sendo de fronteira
			break;
		}
	}
	//cria o analisador de malhas por fase desse malha
	lisMALHAFASES->Add(malhaFases = new TMalhaFases());
	malhaFases->Executa(barraFilha,malha);
	//guarda esse malhas no node
	node->Object = malhaFases;
	//verifica quais das ligacoes da malha ligam à barra filha
	for (int nl = (lisLigMalha->Count - 1); nl >= 0 ; nl--)
	{
		ligAux = (VTLigacao*)lisLigMalha->Items[nl];
		if (ligAux->ConectaBarra(barraFilha))
		{
            //ordena as ligacoes da conexao na malha
			ligAux->OrdenaBarras(barraFilha);
			// associa Ligacao filha c/ Ligacao Pai
			ligAux->ligaPai = ligacaoPai;
			ligAux->Obj = node;
			node->InsereObject(ligAux);
			lisLIGANALISAR->Add(ligAux);  //adiciona a lista local das que serao analisadas
			lisLigMalha->Delete(nl);      //remove da lista de lig da malha
			lisLIGACAOORD->Add(ligAux);   //adiciona à lista ordenada
		}
	}
	// loop p/ todas Ligacoes que se conectam
	for (int nlp = 0; nlp < lisLIGANALISAR->Count; nlp++)
	{
		barraPai = NULL;
		ligacao_pai = (VTLigacao*)lisLIGANALISAR->Items[nlp];
		//como ela esta sendo analisada, retira-se da lista de todas ligacoes
		lisLigacoesRede->Remove(ligacao_pai);
		// loop p/ todas Barras da Ligacao pai
		for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
		{
			if ((barra = ligacao_pai->Barra(ind_bar)) == NULL)
				continue;
			// descarta Barras já analisadas
			if (lisBARRAORD->IndexOf(barra) >= 0)
				continue;
			// insere Barra em lisEXT_BAR
			lisBARRAORD->Add(barra);
			// salva em Barra a Ligacao pai que a conectou
			barra->Obj = ligacao_pai;
			// determina Ligacoes em da rede toda que ainda nao foram analisadas
			//e que se conectam à Barra
			for (int nl = (lisLigacoesRede->Count - 1); nl >= 0; nl--)
			{ // OBS: as chaves abertas foram excluídas de lisEQP no loop anterior
				ligacao_filha = (VTLigacao*)lisLigacoesRede->Items[nl];
				if (ligacao_filha->ConectaBarra(barra))
				{	//ordena as ligacoes da conexao na malha
					ligacao_filha->OrdenaBarras(barra);
					// associa Ligacao filha c/ Ligacao Pai
					ligacao_filha->ligaPai = ligacao_pai;
					ligacao_filha->Obj = node;
					// verifica se a ligacao pertence à malha
					if (lisLigMalha->IndexOf(ligacao_filha) >= 0)
					{
						//verifica se ela ja sta na fila para analisar
						if (lisLIGANALISAR->IndexOf(ligacao_filha) < 0)
						{//e adiciona ao node
							node->InsereObject(ligacao_filha);
							lisLIGANALISAR->Add(ligacao_filha);
							lisLIGACAOORD->Add(ligacao_filha);
						}
						else
						{
                            //ja foi analisada, deve-se fazer nada
						}
					}
					else
					{ // verifica se a ligacao filha pertence à uma malha
						if (ligacao_filha->Tag == 1)
						{ // pertencendo a uma malha, verifica qual malha
							malhaAux = ExisteMalha(ligacao_filha);
							if ((malhaAux != NULL) && (malhaAux != malha))
							{
								AnalisaMalha(ligacao_pai, malhaAux, lisAnalisarPai);
								//test
								nl = lisLigacoesRede->Count - 1;
							}
							else
							{
								// ???
							}
						}
						else
						{   // transfere Ligacao filha de lisEQP p/ lisEXT_LIG
							if(lisAnalisarPai->IndexOf(ligacao_filha) < 0)
							{
								lisAnalisarPai->Add(ligacao_filha);
								ligacao_filha->ligaPai = ligacao_pai;
								barra->Tag |= barraFRONTEIRAFILHO; //marca a barra como sendo de fronteira
							}
						}
					}
				}
			}
		}
	}
	DestroiTObject(lisLIGANALISAR);
	DestroiTObject(lisLigMalha);
}

// ---------------------------------------------------------------------------
VTBarra* __fastcall TValFasesMalha::BarraComum(VTNode *nodePai, VTNode *nodeFilho)
{
	TList *lisBARPai, *lisBARFilho, *lisBARcomum;
	VTBarra *barraComum = NULL;

	lisBARPai = new TList;
	lisBARFilho = new TList;
	lisBARcomum = new TList;

	if (nodeFilho == NULL)
		{return NULL;}
	LisBarraLigacoes(nodePai->LisObjectNode(),lisBARPai);
	LisBarraLigacoes(nodeFilho->LisObjectNode(),lisBARFilho);
	//com as duas lista de barras, preenche uma lsita com as barras em comum
	lisBARcomum->Assign(lisBARPai,laAnd,lisBARFilho);

	if (lisBARcomum->Count == 1)
	{
		barraComum = (VTBarra*)lisBARcomum->First();
	}
//	else
//	{   //TESTE
//		Aviso("multiplas barras em comum..");
//    }

	DestroiTObject(lisBARPai);
	DestroiTObject(lisBARFilho);
	DestroiTObject(lisBARcomum);

	return barraComum;
}
// ---------------------------------------------------------------------------
VTBarra* __fastcall TValFasesMalha::BarraMenorIndexOrdenado(TList *lisBarras)
{
	VTBarra *barraMenor, *barraAux;
	int index = lisBARRAORD->Count;
	int indiceBarra;

	barraMenor = NULL;
	for (int nb = 0; nb < lisBarras->Count; nb++)
	{
		barraAux = (VTBarra*)lisBarras->Items[nb];
		indiceBarra = lisBARRAORD->IndexOf(barraAux);
		if (indiceBarra >= 0)
		{
			if (index > indiceBarra)
			{
				index = indiceBarra;
				barraMenor = barraAux;
			}
		}
	}

	return barraMenor;
}
// ---------------------------------------------------------------------------
VTBarra* __fastcall TValFasesMalha::BarraPai(VTLigacao *ligacao)
{
	VTLigacao *ligaPai;
	VTBarra *barraComum, *barraPai, *barraAux;
	TList *lisBarrasTrafo3E;
	TList *lisBarrasMalha;

	barraPai = NULL;
	//sendo uma ligacao unica...
	ligaPai = ligacao->ligaPai;
	if (ligaPai != NULL)
	{
		barraComum = ligacao->BarraComum(ligaPai);
		if (barraComum != NULL)
		{
			barraPai = barraComum;
		}
	}
	else //caso não tenha uma ligacao pai...
	{
		if (ligacao->Tipo() == eqptoTRAFO3E)
		{   //no caso do trafo3e é preciso ver a barra que foi ordenada 1o
			lisBarrasTrafo3E = new TList;
			for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
			{
				barraAux = ligacao->Barra(ind_bar);
				if (barraAux != NULL)
					{lisBarrasTrafo3E->Add(barraAux);}
			}
			//compara as barras do trafo com a lista ordenada
			barraPai = BarraMenorIndexOrdenado(lisBarrasTrafo3E);
			delete lisBarrasTrafo3E;
		}
		else
		{   // não sendo uma ligacao com 3 barras, basta ver a barra que nao tem como
			// pai a propria ligacao
			for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
			{
				barraAux = ligacao->Barra(ind_bar);
				if (barraAux == NULL)
					{continue;}
				if ((barraAux->Obj != ligacao))
				{
					barraPai = barraAux;
					break;
				}
			}
		}
	}

	return barraPai;
}
// ---------------------------------------------------------------------------
VTBarra* __fastcall TValFasesMalha::BarraPai(VTNode *node)
{
	VTLigacao *ligacao;
	VTBarra *barraPai;
	TList *lisBarrasTrafo3E;
	TList *lisBarrasMalha;

    barraPai = NULL;

	if (node->LisObjectNode()->Count == 1)
	{
		ligacao = (VTLigacao*) node->LisObjectNode()->First();
		barraPai = BarraPai(ligacao);
	}
	else if (node->LisObjectNode()->Count > 1)
	{
		//teste
		//sera que a barra da lista de barras da malha com menor indice na lista ordenadas vai ser a pai?
		lisBarrasMalha = new TList;
		LisBarraLigacoes(node->LisObjectNode(),lisBarrasMalha);
		barraPai = BarraMenorIndexOrdenado(lisBarrasMalha);
		delete lisBarrasMalha;
	}

	return barraPai;
}
// ---------------------------------------------------------------------------
void __fastcall TValFasesMalha::ConfiguraLog(void)
{
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));
	AnsiString logname;
	AnsiString filepath;
	FILE *myfile;
	char *file;


	if (plog)
	{
    	//cria arquivo
		filepath = path->DirImporta();
		logname = FormatDateTime("\\yyyy.mm.dd-hh.mm.ss.zzz", Now());
		logname += "_LogImportacao.csv";
		filepath += logname;
		file = filepath.c_str();
		myfile = fopen(file, "a");
		if (myfile != NULL)
		{
			fclose(myfile);
			myfile = NULL;
		}
		//com o arquivo criado
		plog->Open(filepath);
		plog->InsereCampo("Data");
		plog->InsereCampo("Rede");
		plog->InsereCampo("Eqpto");
		plog->InsereCampo("Código");
		plog->InsereCampo("ID");
		plog->InsereCampo("Descricao");
	}
}
// ---------------------------------------------------------------------------
void __fastcall TValFasesMalha::CorrigeFasesEqpto(VTEqpto *eqpto, int fasesDisponiveis, VTBarra *ref)
{
	int tipo = 0;
	VTTrafo *trafo;
	VTTrecho *trecho, *trechoPai;
	VTCarga	*carga;
	VTLigacao* ligacao;
	VTBarra *barra;
	VTRegulador *reg;
	int fasesNova = 0;

	tipo = eqpto->Tipo();
	switch (tipo)
	{
		case eqptoTRAFO:
			trafo = (VTTrafo*) eqpto;
			trecho = TValFases::ExisteTrechoPai((VTLigacao*)trafo, false);
			TValFases::CorrigeFasesTrafo(trecho, trafo);
			barra = BarraPai((VTLigacao*) trafo);
			//atualiza a tag da barra
			MarcaFasesDisponiveis(barra);
			break;
		case eqptoCARGA:
			carga = (VTCarga*) eqpto;
			fasesDisponiveis = fasesDisponiveis & faseABC;
			if (fases->NumeroDeFases(fasesDisponiveis) == 1)
			{
				fasesNova =  fasesDisponiveis | faseN;
			}
			TValFases::CorrigeFasesCarga(carga,fasesNova);
			break;
		case eqptoREGULADOR: //se o regulador precisar de mais fases, arruma os trechos...
			reg = (VTRegulador*)eqpto;
			ligacao = TValFases::ExistePaiValido(reg);
			if ((ligacao != NULL) && (ligacao->Tipo() == eqptoTRECHO))
			{
				trechoPai = (VTTrecho*) ligacao;
				fasesNova = trechoPai->Fases(ref);
				fasesNova |= GetFaseFaltante(fasesDisponiveis,reg->Fases(ref));
				TValFases::RedefineArranjo(trechoPai,fasesNova);
				barra = BarraPai((VTLigacao*) eqpto);
				//atualiza a tag da barra
				MarcaFasesDisponiveis(barra);
			}
			break;
		case eqptoTRECHO:
			//ajusta o arranjo do trecho pai  baseado nas fases do trecho filho
			trecho = (VTTrecho*) eqpto;
			ligacao = TValFases::ExistePaiValido(trecho);
			if ((ligacao != NULL) && (ligacao->Tipo() == eqptoTRECHO))
			{
				trechoPai = (VTTrecho*) ligacao;
				fasesNova = trechoPai->Fases(ref);
				fasesNova |= GetFaseFaltante(fasesDisponiveis,trecho->Fases(ref));
				TValFases::RedefineArranjo(trechoPai,fasesNova);
				barra = BarraPai((VTLigacao*) eqpto);
				//atualiza a tag da barra
				MarcaFasesDisponiveis(barra);
			}
			else if ((ligacao != NULL) && (ligacao->Tipo() == eqptoREGULADOR))
			{
				reg = (VTRegulador*)ligacao;
				fasesNova = trecho->Fases(ref);
				fasesNova |= GetFaseFaltante(fasesDisponiveis,reg->Fases(ref));
				AlteraFasesRegulador(reg,fasesNova,ref);
				barra = BarraPai((VTLigacao*) eqpto);
				//atualiza a tag da barra
				MarcaFasesDisponiveis(barra);

            }
			break;
		case eqptoCHAVE:
		default:
		;
	}
}
// ---------------------------------------------------------------------------
bool __fastcall TValFasesMalha::Executa(void)
{
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTCarga *carga;
	TList *lisBARRAsIniciais, *lisSUPRI, *lisCARGA;
	VTSuprimento *sup;
	VTLigacao *ligacao;
	bool sucesso = true;
//	PD.corrige_fases = true;

	try
	{
		// lista as malhas
		malhas->Inicia();
		PreparaOrdenacao();
		// sabendo as malhas que essa rede possui pode-se ordenar à partir da barra
		lisBARRAsIniciais = new TList;
		lisSUPRI = new TList;
		//verifica adiciona as barras dos suprimentos
		redes->LisEqbar(lisSUPRI, eqptoSUPRIMENTO);
		for (int ns = 0; ns < lisSUPRI->Count; ns++)
		{
			sup = (VTSuprimento*)lisSUPRI->Items[ns];
			if (lisBARRAsIniciais->IndexOf(sup->pbarra) < 0)
			{
				sup->pbarra->Tag |= sup->Fases;
				lisBARRAsIniciais->Add(sup->pbarra);
			}
		}
		//ordena as ligacoes em uma arvore
		OrdenaLigacoes(lisBARRAsIniciais);
		//agora com as ligacoes apontando para os pais e pros nodes na arvore
		//pode-se verificar as ligacoes
		for(int nl = (lisLIGACAOORD->Count - 1); nl >= 0; nl--)
		{
			ligacao = (VTLigacao*) lisLIGACAOORD->Items[nl];
			VerificaLigacao(ligacao);
		}
		//agora verifica todas cargas
		lisCARGA = new TList;
		redes->LisEqbar(lisCARGA,eqptoCARGA);
		for(int nc = (lisCARGA->Count - 1); nc >= 0; nc--)
		{
			carga = (VTCarga*) lisCARGA->Items[nc];
			VerificaCargas(carga);
		}
//		delete lisCARGA;
	}
	catch(Exception &e)
	{
        sucesso = false;
	}
	DestroiTObject(lisCARGA);
	DestroiTObject(lisBARRAsIniciais);
	//caso corrige as fases, arruma arede
    if (PD.corrige_fases)
	{	//reconfigura Redes
//		topologia->Reconfigura();
	}
	return sucesso;
}

// ---------------------------------------------------------------------------
VTMalha* __fastcall TValFasesMalha::ExisteMalha(VTLigacao *ligacao)
{
	VTMalha *malhaLigacao, *malha;

	malhaLigacao = NULL;

	for (int nm = 0; nm < lisMalhasRede->Count; nm++)
	{
		malha = (VTMalha*) lisMalhasRede->Items[nm];
		if (malha->LisLigacao()->IndexOf(ligacao) >= 0)
		{
			malhaLigacao = malha;
			break;
		}
	}

	return malhaLigacao;
}

// ---------------------------------------------------------------------------
int __fastcall TValFasesMalha::FasesDisponiveis(VTBarra* barraOrd)
{
	int fasesDisp = 0;
    VTNode *node, *nodePai;
	VTLigacao *ligacao;

	//protecao
	if (barraOrd == NULL)
		{return fasesDisp;}
	//se a barra nao tiver ligacao pai
	if (barraOrd->Obj == NULL)
	{
		fasesDisp = TValFases::FasesDisponiveis(barraOrd);
	}
	else
	{
		ligacao = (VTLigacao*)barraOrd->Obj;
		if (ligacao != NULL)
		{
        	//1o verifica se a ligacao pertence à uma malha
			node = (VTNode*)ligacao->Obj;
			fasesDisp = FasesDisponiveis(node, barraOrd);
		}
	}


	return fasesDisp;
}
// ---------------------------------------------------------------------------
int __fastcall TValFasesMalha::FasesDisponiveis(VTNode *nodeLig, VTBarra *filha)
{
	VTLigacao *ligacao;
	int fasesDisp =0;
	TList *lisLIGAUX;
	VTNode *nodePai;
	TMalhaFases *malhaFases;
	VTBarra *barraPaiMalha;
	int faseTag = 0;

	//protecao
	if ((filha == NULL) || (nodeLig == NULL))
		{return fasesDisp;}
	//1o verifica se a ligacao pertence à uma malha
	if (nodeLig->LisObjectNode()->Count == 1)
	{
		ligacao = (VTLigacao*)nodeLig->LisObjectNode()->First();
		fasesDisp = FasesDisponibilizadas(ligacao,filha);
	}
	else
	{   //como é uma malha precisamos ver quais ligacoes da malha se conectam à
		//essa barra filha, e compomos com os trechos dessa malha que chegam na barra
		if (filha != NULL)
		{
			//verifica se a barra é barra pai da malha
			if ((filha->Tag & barraFRONTEIRAPAI) == barraFRONTEIRAPAI)
			{
				lisLIGAUX = new TList;
				nodePai = nodeLig->Pai;
				//se o node pai for soh uma ligacao, tranquilo
				fasesDisp = FasesDisponiveis(nodePai,filha);
			}
			else //não sendo barra pai
			{
				malhaFases = (TMalhaFases*)nodeLig->Object;
				if (malhaFases != NULL)
				{   //verfica cada fase
                	// loop p/ todas Barras da Ligacao pai
					for (int nf = indFASE_A; nf < MAX_FASE; nf++)
					{
						//verifica se a barra pai da malha possui a fase
						barraPaiMalha = malhaFases->barraInicio;
						faseTag = fases->IndexToTag[nf];
//						if ((barraPaiMalha->Tag & faseTag) == faseTag)
						if (true)
						{//verifica se existe caminho ate a barra pai da fase nf
							if (malhaFases->ExisteCaminho(filha,nf))
							{
								fasesDisp |= faseTag;
							}
						}
					}
				}
			}
//			else
//			{
//				for (int nl = 0; nl < nodeLig->LisObjectNode()->Count; nl++)
//				{
//					ligacao = (VTLigacao*) nodeLig->LisObjectNode()->Items[nl];
//					if (ligacao->ConectaBarra(filha))
//					{   //adiciona as necessidades das ligacoes da malha conectada à barra
//						fasesDisp |= FasesDisponibilizadas(ligacao,filha);
//					}
//				}
//			}
		}
	}
	return fasesDisp;
}
// ---------------------------------------------------------------------------
int __fastcall TValFasesMalha::FasesDisponibilizadas(VTLigacao *ligacao, VTBarra *filha)
{
	// variaveis locais
	int tipoLig;
	int fasesDisp = 0;
	VTBarra *barra, *barraAux;
	VTLigacao *ligacaoAux;
	VTRegulador *regulador;
	VTNode *nodeFilho, *nodePai;

	//protecao
	if (filha == NULL)
		{return fasesDisp;}
	if (ligacao == NULL)
	{
		fasesDisp = TValFases::FasesDisponiveis(filha);
    }
	else
	{   //se a ligacao nao conectar na barra?
		if (!ligacao->ConectaBarra(filha)) 
		{
			return fasesDisp;    
		}
		// verifica qual o tipo de ligacao
		tipoLig = ligacao->Tipo();
		// verifica as fases disponiveis de acordo com o tipo de ligacao
		switch (tipoLig)
		{
		case eqptoREGULADOR:
			regulador = (VTRegulador*) ligacao;
			switch (regulador->ligacao)
			{
				case lgMONOFASICO:
					fasesDisp = regulador->fases;
					break;
				case lgEST_AT:
					fasesDisp = faseABCN;
					break;
				case lgEST_ISO:
				case lgDELTA_FECHADO:
				case lgDELTA_ABERTO:
				default:
					fasesDisp = faseABC;
					break;
			}
			break;
		case eqptoCHAVE:
			ligacaoAux = ligacao->ligaPai;
			nodePai = (VTNode*)ligacaoAux->Obj;
			nodeFilho = (VTNode*)ligacao->Obj;
			//verifica qual é a outra barra
			barraAux = ligacao->pbarra1;
			if (barraAux == filha)
			{   //troca
				barraAux = ligacao->pbarra1;
			}
			//verifica se a ligacao pai esta em outro node
			if ((nodePai == nodeFilho) && (nodeFilho != NULL))
			{   //verifica quais sao as fases que a lgiacao pai disponibiliza na barra (pai)
				fasesDisp = FasesDisponibilizadas(ligacaoAux,barraAux);
			}
			else
			{   //v qual as fases disponiveis do nodelig na barra auxiliar (pai)
				fasesDisp = FasesDisponiveis((VTNode*)ligacaoAux->Obj, barraAux);
			}
			break;
		case eqptoTRAFO:
		case eqptoTRAFO3E:
		case eqptoTRECHO:
		default:
			fasesDisp = ligacao->Fases(filha);
			break;
		}
	}

	return fasesDisp;
}
// ---------------------------------------------------------------------------
int __fastcall TValFasesMalha::FasesNecessarias(VTLigacao *ligacao)
{ // variaveis locais
	int fasesNec;
	int tipoLig;
	VTBarra *barra;
	VTLigacao *ligAux, *ligaPai;
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
	//devido a ordenação das ligacoes...
	case eqptoREGULADOR:
	case eqptoTRAFO:
	case eqptoTRAFO3E:
		fasesNec = ligacao->Fases(ligacao->pbarra1);
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
int __fastcall TValFasesMalha::FasesNecessarias(VTNode *nodeLig)
{
	VTLigacao *ligacao;
	int fasesNec=0;
	VTNode *nodePai, *nodeFilho;
	VTBarra *barraComum;

	//1o verifica se a ligacao pertence à uma malha
	if (nodeLig->LisObjectNode()->Count == 1)
	{
		ligacao = (VTLigacao*)nodeLig->LisObjectNode()->First();
		fasesNec = FasesNecessarias(ligacao);
	}
	else
	{   //como é uma malha, é preciso ver qual barra ela tem em comum com o node pai
		//assim veremos todas ligacoes que se conectam à essa barra e qual conjunto
		//de fases elas necessitam
		nodePai = nodeLig->Pai;
		//caso nao tenha node pai..
		if (nodePai == NULL)
		{   //pega a barra que foi adicionada 1o, logo mais proxima ao suprimento
			barraComum = BarraMenorIndexOrdenado(nodeLig->LisObjectNode());
		}
		else
		{   //pega a barra de conexao entre os nodes
			barraComum = BarraComum(nodePai, nodeLig);
			if (barraComum != NULL) 
			{
				for (int nl = 0; nl < nodeLig->LisObjectNode()->Count; nl++)
				{   
					ligacao = (VTLigacao*) nodeLig->LisObjectNode()->Items[nl];
					if (ligacao->ConectaBarra(barraComum)) 
					{   //adiciona as necessidades das ligacoes da malha conectada à barra
						fasesNec |= FasesNecessarias(ligacao);
					}
				}
			}
		}
	}
	return fasesNec;
}
// ---------------------------------------------------------------------------
int	__fastcall TValFasesMalha::GetFaseDisponivel(int fasesDisponiveis, int numFasesDesejada)
{
	int faseTag = 0;
	int numFasesDisp = 0;
	int faseTagAux = 0;
	int numFaseEncontrada = 0;

	numFasesDisp = fases->NumeroDeFases(fasesDisponiveis);
	//protecao
	if ((numFasesDesejada > 3) || (numFasesDesejada < 1) || (numFasesDisp < numFasesDesejada))
		{return faseINV;}
	//sendo valido...
	for (int nf = indFASE_A; nf <= indFASE_C; nf++)
	{   //adiciona ao faseTag a fase disponi
		if (numFaseEncontrada < numFasesDesejada)
		{
			faseTagAux = fases->IndexToTag[nf];
			if ((fasesDisponiveis & faseTagAux) == faseTagAux)
			{
				faseTag |= faseTagAux;
                numFaseEncontrada++;
			}
		}
	}

	return faseTag;
}
// ---------------------------------------------------------------------------
int __fastcall TValFasesMalha::GetFaseFaltante(int fasesDisponiveis, int fasesNecessaria)
{
	int faseFaltante = 0;
	int numFaseVelha, numFaseDisp;

	//1o passo, tira o terra
	fasesDisponiveis = fasesDisponiveis & faseABCN;
	fasesNecessaria = fasesNecessaria & faseABCN;
	//depois verifica o que elas tem de diferente
	faseFaltante =  fasesDisponiveis ^ fasesNecessaria;
	//agora verifica o que falta
	faseFaltante = faseFaltante & fasesNecessaria;
	//confuso né? mas eu fiz a tabela verdade para verificar...
	//exemplo faseDisp = AC (101), faseNec = B (010)
	//faseDisp ^ faseNec = ABC (111), isso & faseNec = B (010)
	//exemplo 2 faseDisp = ABC (111), faseNec = B (010)
	//faseDisp ^ faseNec = AC (101), isso & faseNec = 0 (000)

	return faseFaltante;
}
// ---------------------------------------------------------------------------
void  __fastcall TValFasesMalha::LisBarraLigacoes(TList *lisLig, TList *lisEXT_BAR)
{
	VTBarra *barra;
	VTLigacao *ligacao;

	lisEXT_BAR->Clear();
	for (int nl= 0; nl < lisLig->Count; nl++)
	{
		ligacao = (VTLigacao*) lisLig->Items[nl];
		for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
		{
			barra = ligacao->Barra(ind_bar);
			if ((barra != NULL) && (lisEXT_BAR->IndexOf(barra) < 0))
			{
				lisEXT_BAR->Add(barra);
			}
		}
	}

}
// ---------------------------------------------------------------------------
void  __fastcall TValFasesMalha::LisEqbarPassivos(TList *lisEXT)
{
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTEqbar *eqbar;

	if (lisEXT == NULL)
		{return;}
	redes->LisEqbar(lisEXT);
	//remove os eqbar ativos
	for (int ne = (lisEXT->Count - 1); ne >= 0; ne--)
	{
		eqbar = (VTEqbar*) lisEXT->Items[ne];
		if ((eqbar->Tipo() == eqptoSUPRIMENTO) || (eqbar->Tipo() == eqptoGERADOR))
		{
        	lisEXT->Delete(ne);
		}
	}
}
// ---------------------------------------------------------------------------
void __fastcall TValFasesMalha::LogaEqpto(VTEqpto *eqpto, AnsiString msg)
{
	AnsiString codigoRede = "";
	VTLigacao *ligacao;

	//marca o eqpto com erro
	eqpto->Status[sttERROGIS] = true;
	//pega a informacao de acordo
	if (eqpto->TipoLigacao())
	{
		ligacao = (VTLigacao*) eqpto;
        codigoRede = ligacao->rede->Codigo;
	}
	if (plog)
	{
		plog->DefineValor("Data", "");
		plog->DefineValor("Rede", codigoRede);
		plog->DefineValor("Eqpto", eqpto->TipoAsString());
		plog->DefineValor("Código", eqpto->Codigo);
		plog->DefineValor("ID", eqpto->Extern_id);
		plog->Add(msg);
	}
}
// ---------------------------------------------------------------------------
AnsiString __fastcall TValFasesMalha::MsgFasesAlteradas(int fasesOrig, int fasesAlt)
{
	AnsiString msg;

	msg = "Fases alteradas de " + fases->AsStrABCN[fasesOrig] + " para " +
		fases->AsStrABCN[fasesAlt];

	return msg;
}
// ---------------------------------------------------------------------------
AnsiString __fastcall TValFasesMalha::MsgFasesIncoerentes(int fasesDisponiveis, int fasesNecessarias)
{
	AnsiString msg;

	msg = "Fases incoerentes com o equipamento à montante, [" + fases->AsStrABCN[fasesDisponiveis] + "] para [" +
		fases->AsStrABCN[fasesNecessarias] +"].";

	return msg;
}
// ---------------------------------------------------------------------------
AnsiString __fastcall TValFasesMalha::MsgFasesTrafoAlteradas(int fasesOrig, int fasesAlt, int enrol)
{
	AnsiString msg;
	AnsiString enrolamento;

	switch (enrol)
	{
		case 1:
			enrolamento = "primário";
			break;
		case 2:
			enrolamento = "secundário";
			break;
		case 3:
			enrolamento = "terciário";
			break;
		default:
            enrolamento = "transformador";
			;
	}

	msg = "Fases do " + enrolamento + " alteradas de " + fases->AsStrABCN[fasesOrig] + " para " +
		fases->AsStrABCN[fasesAlt];

	return msg;
}
// ---------------------------------------------------------------------------
void __fastcall TValFasesMalha::MarcaFasesDisponiveis(VTBarra* barraOrdenada)
{
	// variaveis locais
	VTLigacao *ligacao;
	VTNode *nodePai;

	//protecao
	if (barraOrdenada == NULL)
		{return;}
	ligacao = (VTLigacao*) barraOrdenada->Obj;
	if (ligacao == NULL)
	{
		barraOrdenada->Tag = TValFases::FasesDisponiveis(barraOrdenada);
	}
	else
	{
		nodePai = (VTNode*) ligacao->Obj;
		if (nodePai != NULL)
		{
			barraOrdenada->Tag = FasesDisponiveis(nodePai,barraOrdenada);
		}
		else
		{
			barraOrdenada->Tag = TValFases::FasesDisponiveis(barraOrdenada);
		}
	}
}
// ---------------------------------------------------------------------------
VTBarra* __fastcall TValFasesMalha::ObtemBarraInicial(VTRede *rede)
{
	 VTBarra *barraIni = NULL;
	 VTEqbar *eqbar;
	 TList *lisAUX;
	 VTTrafo *trafo;
	 VTBarra *barra;
	 double vnomMax;

	 //protecao
	 if (rede == NULL)
		{return NULL;}
	 //1o verifica se rede possui barra inicial definida
	 if ((barraIni = rede->BarraInicial()) != NULL)
		{return barraIni;}
	 //2o caso não  possua, a 1a opção são as barras dos suprimentos
	 //cria a lista
	 lisAUX = new TList;
	 //obtem a lista de suprimnetos da rede
	 rede->LisEqbar(lisAUX, eqptoSUPRIMENTO);
	 if (lisAUX->Count > 0)
	 {
		eqbar = (VTEqbar*)lisAUX->First(); //esperava o q? =(
		barraIni = eqbar->pbarra;
		if (barraIni != NULL)
			{return barraIni;}
	 }
	 //3o caso, não possui suprimento
	 //verifica se a rede é bt e usa o trafo mt
	 if (rede->TipoRede->Segmento == redeSEC)
	 {
		lisAUX->Clear();
		//obtem a lista de trafos da rede
		rede->LisEqbar(lisAUX, eqptoTRAFO);
		trafo = (VTTrafo*)lisAUX->First(); //esperava o q? =(
		for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
		{
			barra = trafo->Barra(ind_bar);
			if (ind_bar == 0)
			{
				barraIni = barra;
				vnomMax = barra->vnom;
			}
			else
			{
				if (vnomMax < barra->vnom)
				{
                	barraIni = barra;
					vnomMax = barra->vnom;
				}
			}
		}

	 }


	 DestroiTObject(lisAUX);
     return barraIni;
}
// ---------------------------------------------------------------------------
bool __fastcall TValFasesMalha::OrdenaLigacoes(TList *lisBARRAINI)
{
	// variáveis locais
	VTBarra *barra, *barraPai;
	VTChave *chave;
	VTLigacao *ligacao_filha, *ligacao_pai;
	VTNode *nodePai, *nodeFilho, *node;
	VTMalha *malha;
	TList *lisLIGAnalisar;

	//inicia lisEXT_BAR
	lisBARRAORD->Clear();
	// inicia lisEXT_LIG com as Ligacoes que conectam a Barra inicial
	lisLIGACAOORD->Clear();
	//cria uma lista de ligacoes a serem analisadas
	lisLIGAnalisar = new TList;
	//limpa a arvore
	arvore->Clear();
	//cria os nós raiz
	for (int nb = 0; nb < lisBARRAINI->Count; nb++)
	{
		barra = (VTBarra*) lisBARRAINI->Items[nb];
		//como são barras iniciais, a barra não aponta para alguma ligacao pai
		barra->Obj = NULL;
		lisBARRAORD->Add(barra);
		for (int nl = lisLigacoesRede->Count - 1; nl >= 0; nl--)
		{
			ligacao_filha = (VTLigacao*)lisLigacoesRede->Items[nl];
			// verifica se é uma Chave aberta
			if (ligacao_filha->Tipo() == eqptoCHAVE)
			{
				chave = (VTChave*)ligacao_filha;
				if (chave->Aberta)
				{ // retira Ligacao de lisEQP
					lisLigacoesRede->Delete(nl);
					continue;
				}
			}
			// verifica se a Ligacao conecta a Barra
			if (ligacao_filha->ConectaBarra(barra))
			{
				//ordena as barras da ligacao
				ligacao_filha->OrdenaBarras(barra);
				// transfere Ligacao filha de lisEQP p/ lisEXT_LIG
				lisLigacoesRede->Delete(nl);
				lisLIGACAOORD->Add(ligacao_filha);
				lisLIGAnalisar->Add(ligacao_filha);
				// define Ligacao pai como nula
				ligacao_filha->ligaPai = NULL;
				// cria os nós raiz da arvore
				node = DLL_NewObjNode();
				ligacao_filha->Obj = node;
				node->InsereObject(ligacao_filha);
				arvore->InsereNode(NULL, node);
			}
		}
	}
	// loop p/ todas Ligacoes pai que estão em lisEXT_LIG
	for (int nlp = 0; nlp < lisLIGAnalisar->Count; nlp++)
	{
		barraPai = NULL;
		ligacao_pai = (VTLigacao*)lisLIGAnalisar->Items[nlp];
		nodePai = (VTNode*)ligacao_pai->Obj;
		// loop p/ todas Barras da Ligacao pai
		for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
		{
			if ((barra = ligacao_pai->Barra(ind_bar)) == NULL)
				continue;
			// descarta Barras já analisadas
			if (lisBARRAORD->IndexOf(barra) >= 0)
				{continue;}
			// insere Barra em lisEXT_BAR
			lisBARRAORD->Add(barra);
			// salva em Barra a Ligacao pai que a conectou
			barra->Obj = ligacao_pai;
			// determina Ligacoes em lisEQP que se conectam à Barra
			for (int nl = (lisLigacoesRede->Count - 1); nl >= 0; nl--)
			{ // OBS: as chaves abertas foram excluídas de lisEQP no loop anterior
				ligacao_filha = (VTLigacao*)lisLigacoesRede->Items[nl];
				// verifica se a Ligacao conecta a Barra
				if (ligacao_filha->ConectaBarra(barra))
				{
                    //verifica se esta se auto analisando
					if (ligacao_filha != ligacao_pai)
					{//ordena as barras da ligacao
						ligacao_filha->OrdenaBarras(barra);
                    }
					// verifica se a ligacao filha pertence à uma malha
					if (ligacao_filha->Tag == 1)
					{ // pertencendo a uma malha, verifica qual malha
						malha = ExisteMalha(ligacao_filha);
						if (malha != NULL)
						{
							AnalisaMalha(ligacao_pai, malha,lisLIGAnalisar);
                            //teste
							nl = lisLigacoesRede->Count - 1;
						}
						else
						{
							// ???
						}
					}
					else
					{
						//verifica se esta se auto analisando
						if (ligacao_filha != ligacao_pai)
						{
							lisLIGAnalisar->Add(ligacao_filha);
							// associa Ligacao filha c/ Ligacao Pai
							ligacao_filha->ligaPai = ligacao_pai;
						}
						//transfere Ligacao filha de lisEQP p/ lisEXT_LIG
						lisLigacoesRede->Delete(nl);
						lisLIGACAOORD->Add(ligacao_filha);

						// cria os nós filhos
						node = DLL_NewObjNode();
						ligacao_filha->Obj = node;
						node->InsereObject(ligacao_filha);
						arvore->InsereNode((VTNode*)ligacao_pai->Obj, node);
					}
				}
			}
		}
	}
	//percorre todas barras ordenadas e carimba as fases disponiveis no tag
	for (int nb = 0; nb < lisBARRAORD->Count; nb++)
	{
		barra = (VTBarra*)lisBARRAORD->Items[nb];
		MarcaFasesDisponiveis(barra);
	}
	// limpa listas locais
	lisLigacoesRede->Clear();

	DestroiTObject(lisLIGAnalisar);
	return (true);
}
// ---------------------------------------------------------------------------
bool __fastcall TValFasesMalha::PreparaOrdenacao(void)
{
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTLigacao *ligacao;
	VTMalha *malha;
	TList *lisMALHAsInternas = malhas->LisMalhaInterna();
	TList *lisMALHAsExternas = malhas->LisMalhaExterna();
	VTBarra* barra;

	// inicia lisEQP com todas Ligacoes da rede
	lisLigacoesRede->Clear();
	redes->LisLigacao(lisLigacoesRede);
	//guarda as barrras
	lisBarrasRede->Clear();
	redes->LisBarra(lisBarrasRede);
	//marca todas barras com tag 0 indicando que nao possuem fases disponiveis
	for (int nb = 0; nb < lisBarrasRede->Count; nb++)
	{
		barra = (VTBarra*) lisBarrasRede->Items[nb];
		barra->Tag = 0;
		barra->Obj = NULL;
	}
	// percorre todas ligacoes alterando a tag para 0
	for (int nl = 0; nl < lisLigacoesRede->Count; nl++)
	{
		ligacao = (VTLigacao*) lisLigacoesRede->Items[nl];
		ligacao->Tag = 0;
	}
	// marca como 1 para todas ligacoes que pertencem a uma malha
	for (int nm = 0; nm < lisMALHAsInternas->Count; nm++)
	{
		malha = (VTMalha*) lisMALHAsInternas->Items[nm];
		for (int nlm = 0; nlm < malha->LisLigacao()->Count; nlm++)
		{
			ligacao = (VTLigacao*) malha->LisLigacao()->Items[nlm];
			ligacao->Tag = 1;
		}
	}
	// marca como 1 para todas ligacoes que pertencem a uma malha
	for (int nm = 0; nm < lisMALHAsExternas->Count; nm++)
	{
		malha = (VTMalha*) lisMALHAsExternas->Items[nm];
		for (int nlm = 0; nlm < malha->LisLigacao()->Count; nlm++)
		{
			ligacao = (VTLigacao*) malha->LisLigacao()->Items[nlm];
			ligacao->Tag = 1;
		}
	}
	lisMalhasRede->Assign(lisMALHAsInternas,laCopy);
	lisMalhasRede->Assign(lisMALHAsExternas,laOr);
	return true;
}
// ---------------------------------------------------------------------------
void __fastcall TValFasesMalha::ProcuraFilhas(VTLigacao *pai, TList *lisLIG, TList *lisEXT)
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
//// ---------------------------------------------------------------------------
bool __fastcall TValFasesMalha::RotacionaCarga(VTCarga *carga, int faseNova)
{
	int caracteristicaNT = 0;
	bool sucesso = true;

	try
	{
		//guarda se a carga tinha neutro e terra
		caracteristicaNT = carga->Fases & faseNT;
		//altera fase da carga
		carga->Fases = faseNova;
		carga->Fases |= caracteristicaNT;
	}
	catch (Exception &e)
	{
		sucesso = false;
	}

	return sucesso;

}
//// ---------------------------------------------------------------------------
bool __fastcall TValFasesMalha::RotacionaEqpto(VTEqpto *eqpto, int fasesDisponiveis, VTBarra *ref)
{
	int tipo = 0;
	bool sucesso = true;
	int fasesEqpto,  numFasesEqpto, numFasesDisp;
	int fasesNovas;
	VTEqbar *eqbar;
	VTLigacao *ligacao;
	VTCarga *carga;

	fasesEqpto = numFasesEqpto =  numFasesDisp = 0;
	//verifica o tipo de eqpto
	tipo = eqpto->Tipo();
	//verifica se eh uma ligacao
	if (eqpto->TipoLigacao())
	{
		ligacao = (VTLigacao*) eqpto;
		fasesEqpto = ligacao->Fases(ref);
	}
	else if (eqpto->TipoEqbar())
	{
    	switch (tipo)
		{
			case eqptoCARGA:
				carga = (VTCarga*) eqpto;
				fasesEqpto = carga->Fases;
				break;
			default:
				fasesEqpto = faseINV;
			;
		}
	}
	//protecao
	if (fasesEqpto != faseINV)
	{
		numFasesEqpto = fases->NumeroDeFases(fasesEqpto);
		numFasesDisp  = fases->NumeroDeFases(fasesDisponiveis);
		//se o numero de fases eqpto for maior que o numero de fases disponiveis,
		//nao tem jeito
		if (numFasesEqpto > numFasesDisp)
		{
			sucesso = false;
		}
		//dá jogo, entao pega a 1a combinacao valida de fases
		fasesNovas = GetFaseDisponivel(fasesDisponiveis,numFasesEqpto);
		//agora age de acordo com o tipo de eqpto
		switch (tipo)
		{
			case eqptoTRAFO:      //pode-se rotacionar um enrolamento
				sucesso = RotacionaTrafo((VTTrafo*) eqpto, fasesNovas, ref);
				break;
			case eqptoCARGA:
				sucesso = RotacionaCarga((VTCarga*) eqpto, fasesNovas);
				break;
			//não se rotaciona esses eqptos pois pode gerar incoerencia com os
			//eqpamentos seguintes
			case eqptoREGULADOR:
			case eqptoTRECHO:    //
			case eqptoCHAVE:
			default:
				sucesso = false;
			;
		}
	}
	else
		{sucesso = false;}

	return sucesso;
}
// ---------------------------------------------------------------------------
bool __fastcall TValFasesMalha::RotacionaTrafo(VTTrafo *trafo, int faseNova,
																	VTBarra *ref)
{
	VTBarra *barraAux;
	int caracteristicaNT = 0;
	bool sucesso = true;

	try
	{
        //1o verifica se não eh trifasico
		if (trafo->Monofasico)
		{   //sendo monofasico, analisa as fases disponiveis
			if (ref == trafo->BarraPrimario())
			{
				caracteristicaNT = trafo->pri.fase & faseNT;
				trafo->pri.fase = faseNova;
				///se o trafo vai possuir 2 fases, nao poe o neutro
				if (fases->NumeroDeFases(faseNova) == 1)
				{//guarda se a carga tinha neutro e terra
					trafo->pri.fase |= caracteristicaNT;
				}
			}
			else if (ref == trafo->BarraSecundario())
			{	//guarda se a carga tinha neutro e terra
				caracteristicaNT = trafo->pri.fase & faseNT;
				trafo->sec.fase = faseNova;
				trafo->sec.fase |= caracteristicaNT;
			}
			else
				{sucesso = false;}
		}
		else //sendo trifasico rotacionar não muda algo
			{sucesso = false;}
	}
	catch (Exception &e)
	{
		sucesso = false;
	}

	return sucesso;
}
// ---------------------------------------------------------------------------
void __fastcall TValFasesMalha::VerificaCargas(VTCarga *carga)
{
	VTBarra *barra;
	int fasesNecessarias, fasesDisponiveis;

	fasesNecessarias = fasesDisponiveis = 0;
	//protecao
	if (carga == NULL)
		{return;}
	barra  = carga->pbarra;
	fasesDisponiveis = barra->Tag & faseABCNT;
	//se  fasesDisponiveis == 0 é que esta isolada entao deixa quieto
	if (fasesDisponiveis == 0)
		{return;}
	fasesNecessarias = carga->Fases;
	//compara as necesssarias com as disponiveis e corrige se for o caso
	if ((fasesDisponiveis & fasesNecessarias) != fasesNecessarias)
	{
		//Loga o problema
		LogaEqpto((VTEqpto*)carga,MsgFasesIncoerentes(fasesDisponiveis,fasesNecessarias));
		//caso a opcao de corrigir estiver habilitada
		if (PD.corrige_fases)
		{ 	//1o tenta rotacionar o eqpto
			if (!RotacionaEqpto((VTEqpto*)carga, fasesDisponiveis, barra))
			{
				//se nao conseguir rotacionar, tem que corrigir de outra forma
				CorrigeFasesEqpto((VTEqpto*)carga, fasesDisponiveis, barra);
			}
        }
	}
}
// ---------------------------------------------------------------------------
void __fastcall TValFasesMalha::VerificaLigacao(VTLigacao *ligacao)
{
	int tipo;
	VTBarra *barraPai;
	int fasesNecessarias, fasesDisponiveis;
	VTNode *node;

	fasesNecessarias = fasesDisponiveis = 0;
	barraPai = BarraPai(ligacao);
	//teste
//	if (barraPai == NULL)
//		{Aviso("Barra Pai nula..");}
//	else
//		{fasesDisponiveis = barraPai->Tag;}
	if (barraPai != NULL)
		{fasesDisponiveis = barraPai->Tag;}
	else
        {return;}
	//verifica as fases necessarias
	node = (VTNode*) ligacao->Obj;
	fasesNecessarias = FasesNecessarias(ligacao);
	//compara as necesssarias com as disponiveis e corrige se for o caso
	if ((fasesDisponiveis & fasesNecessarias) != fasesNecessarias)
	{
		//Loga o problema
		LogaEqpto((VTEqpto*)ligacao,MsgFasesIncoerentes(fasesDisponiveis,fasesNecessarias));
		//caso a opcao de corrigir estiver habilitada
		if (PD.corrige_fases)
		{	//1o tenta rotacionar o eqpto
			if (!RotacionaEqpto((VTEqpto*)ligacao, fasesDisponiveis, barraPai))
			{
				//se nao conseguir rotacionar, tem que corrigir de outra forma
				CorrigeFasesEqpto((VTEqpto*)ligacao, fasesDisponiveis, barraPai);
			}
		}
	}
}
// -----------------------------------------------------------------------------
// eof
