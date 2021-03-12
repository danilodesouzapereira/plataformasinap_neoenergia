//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TMontaMalha.h"
#include "VTMalha.h"
#include "VTMalhas.h"
#include "..\Apl\VTApl.h"
#include "..\Ordena\VTOrdena.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTEqbar.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTMNet.h"
#include "..\Rede\VTNet.h"
#include "..\Rede\VTRedes.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Ordena.h"
#include "..\..\DLL_Inc\Rede.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TMontaMalha::TMontaMalha(VTApl *apl)
   {
	//salva ponteiro p/ objeto apl
   this->apl = apl;
	//cria listas
	lisEQP = new TList();
	lisBAR = new TList();
	lisLIG = new TList();
	lisSEL = new TList();
	lisBGS = new TList();
	lisMAL = new TList;
	//cria objeto Ordena
	ordena = DLL_NewObjOrdena(apl);
	ordena->ChaveNF_Enabled = true;
	ordena->ChaveNA_Enabled = false;
	ordena->TramoEnabled    = false;
	ordena->ReguladorBypass = false;
	}

//---------------------------------------------------------------------------
__fastcall TMontaMalha::~TMontaMalha(void)
	{
	//destrói objetos
	if (ordena) {delete ordena; ordena = NULL;}
	//destrói lista sem destruir seus objetos
	if (lisEQP) {delete lisEQP; lisEQP = NULL;}
	if (lisBAR) {delete lisBAR; lisBAR = NULL;}
	if (lisLIG) {delete lisLIG; lisLIG = NULL;}
	if (lisSEL) {delete lisSEL; lisSEL = NULL;}
	if (lisBGS) {delete lisBGS; lisBGS = NULL;}
	if (lisMAL) {delete lisMAL; lisMAL = NULL;}
	}

//---------------------------------------------------------------------------
bool __fastcall TMontaMalha::Executa(VTMalhas *malhas, VTRedes *redes)
	{
	//variáveis locais
	VTMNet    *mnet;
	VTNet     *net;
	TList     *lisMNET = redes->LisMNetFluxo();

	//salva ponteiro p/ objeto
	this->malhas = malhas;
	//loop para todas MNet
	for (int i = 0; i < lisMNET->Count; i++)
		{
		mnet = (VTMNet*)lisMNET->Items[i];
		//cria objeto Net
		net = ordena->Executa(mnet);
		//identifica Malhas na Net
		Executa(net);
		//destrói objeto Net
		DLL_DeleteEqpto(net);
		}
	return(true);
	}

//---------------------------------------------------------------------------
bool __fastcall TMontaMalha::Executa(VTNet *net)
	{
	//variáveis locais
	VTBarra *barra;
	VTEqbar *eqbar;

	//seleciona uma Barra qualquer da Net
	if (net->LisBarra()->Count == 0) return(false);
	barra = (VTBarra*)(net->LisBarra()->First());
	//identifica Malhas internas/externs
	IdentificaMalhaExternaInterna(barra, net->LisLigacao());
	//verifica número de geradores/suprimentos DEPOIS de ter identiifcado malhas internas/externs
	if (net->NumGerSup() > 1)
		{//inicia lisBGS com Barras com Gerador/Suprimeento
		IniciaLisBarraGeradorSuprimento(net);
		//identifica Malhas de geração
		IdentificaMalhaGeracao(net->LisLigacao());
		}

	return(true);
	}

//---------------------------------------------------------------------------
bool __fastcall TMontaMalha::Executa(VTRede *rede)
	{
	//variáveis locais
	VTBarra *barra;

	//seleciona uma Barra para iniciar a ordenação da Rede
	if ((barra = rede->BarraInicial()) == NULL)
		{//seleciona uma Barra qualquer da Rede
		if (rede->LisBarra()->Count == 0) return(false);
		barra = (VTBarra*)(rede->LisBarra()->First());
		}
	//identifica Malhas da Rede
	IdentificaMalhaExternaInterna(barra, rede->LisLigacao());

	return(true);
	}

//---------------------------------------------------------------------------
VTMalha* __fastcall TMontaMalha::ExisteMalhaComLigacaoComum(TList *lisMALHA, VTMalha *malha_ref)
	{
	//variáveis locais
	VTMalha *malha;

	//loop p/ todas Malhas internas
	for (int n = 0; n < lisMALHA->Count; n++)
		{
		malha = (VTMalha*)lisMALHA->Items[n];
		if (malha == malha_ref) continue;
		//verifica se as Malhas tem Ligacao comum
		if (malha->ExisteLigacaoComum(malha_ref)) return(malha);
		}
	return(NULL);
	}

//---------------------------------------------------------------------------
VTMalha* __fastcall TMontaMalha::ExisteMalhaGeracaoComLigacaoComum(VTMalha *malha_ref)
	{
	//variáveis locais
	VTMalha *malha;
	TList   *lisMALHA;

	//loop p/ todas Malhas de geração
	lisMALHA = malhas->LisMalhaGeracao();
	for (int n = 0; n < lisMALHA->Count; n++)
		{
		malha = (VTMalha*)lisMALHA->Items[n];
		if (malha == malha_ref) continue;
		//verifica se as Malhas tem Ligacao comum
		if (malha->ExisteLigacaoComum(malha_ref)) return(malha);
		}
	return(NULL);
	}

//---------------------------------------------------------------------------
void __fastcall TMontaMalha::FundeMalhas(VTMalha *malha_ref)
	{
	//variáveis locais
	VTMalha *malha;

	//inicia lisMAL com todas Malhas interna/externas
	lisMAL->Clear();
	CopiaTList(malhas->LisMalhaExterna(), lisMAL);
	CopiaTList(malhas->LisMalhaInterna(), lisMAL);
	//verifica se há Malhas com Ligações comuns
	while ((malha = ExisteMalhaComLigacaoComum(lisMAL, malha_ref)) != NULL)
		{//incorpora malha em malha_ref
		malha_ref->IncorporaMalha(malha);
		//retira malhas de lisMAL
		lisMAL->Remove(malha);
		//retira malha de Malhas
		malhas->RemoveMalha(malha);
		//destrói malha removida
		delete malha;
		}
	}

//---------------------------------------------------------------------------
void __fastcall TMontaMalha::FundeMalhasGeracao(VTMalha *malha_ref)
	{
	//variáveis locais
	VTMalha *malha;

	//verifica se há Malhas de geração com Ligações comuns
	while ((malha = ExisteMalhaGeracaoComLigacaoComum(malha_ref)) != NULL)
		{//incorpora malha em malha_ref
		malha_ref->IncorporaMalha(malha);
		//retira malha de Malhas
		malhas->RemoveMalha(malha);
		//destrói malha removida
		delete malha;
		}
	//inicia lisMAL com todas Malhas interna/externas
	lisMAL->Clear();
	CopiaTList(malhas->LisMalhaExterna(), lisMAL);
	CopiaTList(malhas->LisMalhaInterna(), lisMAL);
	//verifica se há Malhas (que não são de geração) com Ligações comuns
	while ((malha = ExisteMalhaComLigacaoComum(lisMAL, malha_ref)) != NULL)
		{//incorpora malha em malha_ref
		malha_ref->IncorporaMalha(malha);
		//retira malhas de lisMAL
		lisMAL->Remove(malha);
		//NÃO retira malha interna/externa de Malhas
		//malhas->RemoveMalha(malha);
		//NÃO destrói interna/externa incluída na malha de geração
		//delete malha;
		}
	}

//---------------------------------------------------------------------------
void __fastcall TMontaMalha::IdentificaMalhaExternaInterna(VTBarra *barra, TList *lisLIG_EXT)
	{
	//variáveis locais
	VTBarra   *bar_filha;
	VTChave   *chave;
	VTLigacao *ligacao_filha, *ligacao_pai;

	//proteção
	if (barra == NULL) return;
	//inicia lisEQP com todas Ligacoes de lisLIG_EXT
	lisEQP->Clear();
	CopiaTList(lisLIG_EXT, lisEQP);
	//define a Barra selecionada não possui uma Ligacao pai que a conectou
	ligacao_pai = NULL;
	barra->Obj  = ligacao_pai;
	//inicia lisBAR com a Barra selecionada
	lisBAR->Clear();
	lisBAR->Add(barra);
	//monta lisLIG a partir das Barras em lisBAR
	lisLIG->Clear();
	for (int nb = 0; nb < lisBAR->Count; nb++)
		{
		barra = (VTBarra*)lisBAR->Items[nb];
		//determina Ligacao pai
		ligacao_pai = (VTLigacao*)(barra->Obj);
		//determina Ligacoes em lisEQP que se conectam à Barra
		for (int nl = lisEQP->Count-1; nl >= 0; nl--)
			{
			ligacao_filha = (VTLigacao*)lisEQP->Items[nl];
			//verifica se Ligacao filha é uma Chave aberta
			if (ligacao_filha->Tipo() == eqptoCHAVE)
            {
				chave = (VTChave*)ligacao_filha;
				if (chave->Aberta)
					{//retira Ligacao de lisEQP
					lisEQP->Delete(nl);
					continue;
					}
				}
			//verifica se a Ligacao conecta a Barra
			if (ligacao_filha->ConectaBarra(barra))
				{//transfere Ligacao filha de lisEQP p/ lisLIG
				lisEQP->Delete(nl);
				lisLIG->Add(ligacao_filha);
            //associa Ligacao filha c/ Ligacao Pai
            ligacao_filha->ligaPai = ligacao_pai;
				//insere Barras da Ligacao filha em lisBAR
				for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
               {
					if ((bar_filha = ligacao_filha->Barra(ind_bar)) == NULL) continue;
               //verifica se Barra filha é igual a Barra sendo analisada
               if (bar_filha == barra) continue;
					//verifica se Barra filha já existe em lisBAR
					if (lisBAR->IndexOf(bar_filha) < 0)
						{//define Ligacao a partir da qual a Barra filha foi localizada
                  bar_filha->Obj = ligacao_filha;
                  //insere Barra filha em lisBAR
						lisBAR->Add(bar_filha);
						}
					else
						{//existência de malha
						InsereMalha(ligacao_filha, (VTLigacao*)(bar_filha->Obj));
						}
					}
				}
			}
		}
	//limpa listas locais
	lisEQP->Clear();
	lisBAR->Clear();
	lisLIG->Clear();
	}

//---------------------------------------------------------------------------
void __fastcall TMontaMalha::IdentificaMalhaGeracao(TList *lisLIG_EXT)
	{
	//variáveis locais
	VTBarra   *barra;
	VTBarra   *bar_filha;
	VTChave   *chave;
	VTLigacao *ligacao_filha, *ligacao_pai;

	//seleciona primeira Barra de lisBGS
	barra = (VTBarra*)lisBGS->First();
	//inicia lisEQP com todas Ligacoes de lisLIG_EXT
	lisEQP->Clear();
	CopiaTList(lisLIG_EXT, lisEQP);
	//define a Barra selecionada não possui uma Ligacao pai que a conectou
	ligacao_pai = NULL;
	barra->Obj  = ligacao_pai;
	//inicia lisBAR com a Barra selecionada
	lisBAR->Clear();
	lisBAR->Add(barra);
	//monta lisLIG a partir das Barras em lisBAR
	lisLIG->Clear();
	for (int nb = 0; nb < lisBAR->Count; nb++)
		{
		barra = (VTBarra*)lisBAR->Items[nb];
		//determina Ligacao pai
		ligacao_pai = (VTLigacao*)(barra->Obj);
		//determina Ligacoes em lisEQP que se conectam à Barra
		for (int nl = lisEQP->Count-1; nl >= 0; nl--)
			{
			ligacao_filha = (VTLigacao*)lisEQP->Items[nl];
			//verifica se Ligacao filha é uma Chave aberta
			if (ligacao_filha->Tipo() == eqptoCHAVE)
            {
				chave = (VTChave*)ligacao_filha;
				if (chave->Aberta)
					{//retira Ligacao de lisEQP
					lisEQP->Delete(nl);
					continue;
					}
				}
			//verifica se a Ligacao conecta a Barra
			if (ligacao_filha->ConectaBarra(barra))
				{//transfere Ligacao filha de lisEQP p/ lisLIG
				lisEQP->Delete(nl);
				lisLIG->Add(ligacao_filha);
            //associa Ligacao filha c/ Ligacao Pai
            ligacao_filha->ligaPai = ligacao_pai;
				//insere Barras da Ligacao filha em lisBAR
				for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
               {
					if ((bar_filha = ligacao_filha->Barra(ind_bar)) == NULL) continue;
					//verifica se Barra filha é igual a Barra sendo analisada
					if (bar_filha == barra) continue;
					//verifica se Barra filha é uma Barra de geração
					if (lisBGS->IndexOf(bar_filha) >= 0)
						{//existência de malha de geração
						InsereMalhaGeracao(ligacao_filha);
						}
					//verifica se Barra filha já existe em lisBAR
					if (lisBAR->IndexOf(bar_filha) < 0)
						{//define Ligacao a partir da qual a Barra filha foi localizada
						bar_filha->Obj = ligacao_filha;
						//insere Barra filha em lisBAR
						lisBAR->Add(bar_filha);
						}
					}
				}
			}
		}
	//limpa listas locais
	lisEQP->Clear();
	lisBAR->Clear();
	lisLIG->Clear();
	}

//---------------------------------------------------------------------------
int __fastcall TMontaMalha::IniciaLisBarraGeradorSuprimento(VTNet *net)
	{
	//variáveis locais
	VTBarra *barra;
	VTEqbar *eqbar;

	//inicia lista com Barras com geração
	lisBGS->Clear();
	lisEQP->Clear();
	net->LisEqbar(lisEQP, eqptoSUPRIMENTO);
	net->LisEqbar(lisEQP, eqptoGERADOR);
	for (int n = 0; n < lisEQP->Count; n++)
		{
		eqbar = (VTEqbar*)lisEQP->Items[n];
		barra = eqbar->pbarra;
		if (lisBGS->IndexOf(barra)) lisBGS->Add(barra);
		}
	return(lisBGS->Count);
	}

//---------------------------------------------------------------------------
TList* __fastcall TMontaMalha::IniciaLisLigacaoEmMalha(VTLigacao *ligacao1, VTLigacao *ligacao2)
	{
	//variáveis locais
	int index;

	//reinicia lista de Barras e Ligacoes
	lisSEL->Clear();
	//insere primeira Ligacao e todas suas Ligacoes pais
	while(ligacao1 != NULL)
		{
		lisSEL->Add(ligacao1);
		ligacao1= ligacao1->ligaPai;
		}
	//loop p/ segunda Ligacao e todas suas Ligacoes pais
	while(ligacao2 != NULL)
		{//verifica se a Ligacao já existe em lisSEL
		if ((index = lisSEL->IndexOf(ligacao2)) < 0)
			{//insere Ligacao em lisSEL
			lisSEL->Add(ligacao2);
			}
		else
			{//remove a Ligacao de lisSEL
			lisSEL->Delete(index);
			}
		ligacao2= ligacao2->ligaPai;
		}
	return(lisSEL);
	}

//---------------------------------------------------------------------------
void __fastcall TMontaMalha::InsereMalha(VTLigacao *ligacao1, VTLigacao *ligacao2)
	{
	//variáveis locais
	VTMalha *malha;
	TList   *lisLIG_MALHA;

	try{//determina conjunto de Ligações em malha
		lisLIG_MALHA = IniciaLisLigacaoEmMalha(ligacao1, ligacao2);
		if (lisLIG_MALHA->Count == 0) return;
		//cria uma nova Malha
		if ((malha = NewObjMalha()) == NULL) return;
		//insere Ligacoes na Malha a partir da primeira Ligacao
		malha->InsereLigacao(lisLIG_MALHA);
		//verifica se a Malha incorpora outras Malhas
		FundeMalhas(malha);
		//insere Malha em lisMALHA
		malhas->InsereMalha(malha);
		}catch(Exception &e)
			{//nada a fazer
			}
	}

//---------------------------------------------------------------------------
void __fastcall TMontaMalha::InsereMalhaGeracao(VTLigacao *ligacao)
	{
	//variáveis locais
	VTMalha *malha;

	try{//determina conjunto de Ligações em malha
		lisSEL->Clear();
		//insere primeira Ligacao e todas suas Ligacoes pais
		while(ligacao != NULL)
			{
			lisSEL->Add(ligacao);
			ligacao = ligacao->ligaPai;
			}
		//cria uma nova Malha
		if ((malha = NewObjMalha()) == NULL) return;
		//insere Ligacoes na Malha a partir da primeira Ligacao
		malha->InsereLigacao(lisSEL);
		//verifica se a Malha incorpora outras Malhas
		FundeMalhasGeracao(malha);
		//insere Malha em lisMALHA
		malhas->InsereMalhaGeracao(malha);
		}catch(Exception &e)
			{//nada a fazer
			}
	}

//---------------------------------------------------------------------------
//eof

