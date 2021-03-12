// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TRebuild.h"
#include "..\Apl\VTApl.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTCorte.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTMRede.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTReducao.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Rede.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
VTRebuild* __fastcall NewObjRebuild(VTApl *apl)
{
	return (new TRebuild(apl));
}

// ---------------------------------------------------------------------------
__fastcall TRebuild::TRebuild(VTApl *apl)
{
	// salva ponteiro p/ objetos
	this->apl = apl;
	// cria objetos
	lisL = new TList();
	lisBAR = new TList();
	lisCHV = new TList();
	lisNR = new TList();
	lisRR = new TList();
	lisALL_BAR = new TList();
	lisALL_REDE = new TList();
}

// ---------------------------------------------------------------------------
__fastcall TRebuild::~TRebuild(void)
{
	// destrói lista sem destruir seus objetos
	if (lisL)
	{
		delete lisL;
		lisL = NULL;
	}
	if (lisBAR)
	{
		delete lisBAR;
		lisBAR = NULL;
	}
	if (lisCHV)
	{
		delete lisCHV;
		lisCHV = NULL;
	}
	if (lisNR)
	{
		delete lisNR;
		lisNR = NULL;
	}
	if (lisRR)
	{
		delete lisRR;
		lisRR = NULL;
	}
	if (lisALL_BAR)
	{
		delete lisALL_BAR;
		lisALL_BAR = NULL;
	}
	if (lisALL_REDE)
	{
		delete lisALL_REDE;
		lisALL_REDE = NULL;
	}
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/14422280
//https://sinapsisenergia.teamwork.com/#tasks/16223814
bool __fastcall TRebuild::Executa(VTRedes *redes, TList *lisREDE, TList *lisBARRA,
	TList *lisLIGACAO)
{
	// variáveis locais
	VTRede *rede, *rede_isolada;
	VTReducao *reducao;

	// salva ponteiro para objeto
	this->redes = redes;
	//inclui ligações da rede isolada
	rede_isolada = redes->RedeIsolada();
	if(rede_isolada->LisLigacao()->Count > 0)
	{
		CopiaTList(rede_isolada->LisLigacao(), lisLIGACAO);
    }
	// reinicia lista de Barras de Redes
	redes->ReiniciaLisBarra(lisBARRA);
	// separa redes estáticas e reconfiguráveis em lisNR e lisRR
	SeparaRedesEstaticasRedesReconfiguraveis(lisREDE);
	// reconstrói redes estáticas com suas ligações próprias
	MontaRedeEstatica(lisLIGACAO);
	// reconstrói redes reconfiguráveis ordenando-as a partir da barra inicial
	MontaRedeReconfiguravel(lisLIGACAO);
	// salva Ligacoes isoladas que permaneceram em lisLIGACAO na Rede isolada
	rede_isolada->IniciaLisLigacao(lisLIGACAO);
	// insere redes das ações
	lisALL_REDE->Clear();
	CopiaTList(lisREDE, lisALL_REDE);
	// obtém lista de todas as Redes
	redes->LisRede(lisALL_REDE);
	// reinicia lista de todas as barras, incluindo barras das ações
	lisALL_BAR->Clear();
	CopiaTList(lisBARRA, lisALL_BAR);
	// insere em lisALL_BAR as Barras da Rede isolada
	rede_isolada->LisBarra(lisALL_BAR);
	// inclui Barras de todas as Redes
	for (int n = lisALL_REDE->Count - 1; n >= 0; n--)
	{
		rede = (VTRede*)lisALL_REDE->Items[n];
		// verifica se a rede é isolada
		if (rede == rede_isolada)
		{
			lisALL_REDE->Remove(rede);
			continue;
		}
		if (rede->Carregada)
		{ // inclui Barras da Rede
			rede->LisBarra(lisALL_BAR);
		}
		else
		{ // inclui Barras da Reducao da Rede
			if (rede->MRede == NULL)
				continue;
			if ((reducao = rede->MRede->ExisteReducaoMRedeNaoCarregada()) != NULL)
			{ // inclui Barras da Rede
				reducao->LisBarra(lisALL_BAR);
			}
		}
	}
	// reinicia VTRedes com as Redes e Barras
	redes->ReiniciaLisRede(lisALL_REDE);
	redes->ReiniciaLisBarra(lisALL_BAR);

	return (true);
}

// ---------------------------------------------------------------------------
void __fastcall TRebuild::MontaRedeEstatica(TList *lisLIG)
{
	// variáveis locais
	VTLigacao *ligacao;
	VTRede *rede;

	// limpa Ligacoes e Barras das Redes estáticas
	lisL->Clear();
	for (int n = 0; n < lisNR->Count; n++)
	{
		rede = (VTRede*)lisNR->Items[n];
		rede->IniciaLisLigacao(lisL);
	}
	// remonta as Redes estáticas com Ligações próprias
	for (int n = 0; n < lisNR->Count; n++)
	{
		rede = (VTRede*)lisNR->Items[n];
		// loop p/ todas as Ligacoes em lisLIG
		for (int n = lisLIG->Count - 1; n >= 0; n--)
		{
			ligacao = (VTLigacao*)lisLIG->Items[n];
			if (ligacao->rede == rede)
			{ // insere Ligacao na Rede
				rede->InsereLigacao(ligacao);
				// retira Ligacao da lista
				lisLIG->Delete(n);
			}
		}
	}
	// reinicia lista de Barras das redes
	for (int n = 0; n < lisNR->Count; n++)
	{
		rede = (VTRede*)lisNR->Items[n];
		rede->IniciaLisBarra();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TRebuild::MontaRedeReconfiguravel(TList *lisLIG)
{
	// variáveis locais
	VTLigacao *ligacao;
	VTRede *rede;

	// limpa Ligacoes e Barras das Redes estáticas
	lisL->Clear();
	for (int n = 0; n < lisRR->Count; n++)
	{
		rede = (VTRede*)lisRR->Items[n];
		rede->IniciaLisLigacao(lisL);
	}
	// reordena cada uma das Redes, atendendo as seguintes condições:
	// 1) inicia ordenação a partir da Barra inicial
	// 2) trata somente suas próprias Ligacoes
	for (int n = 0; n < lisRR->Count; n++)
	{
		rede = (VTRede*)lisRR->Items[n];
		// ordena a Rede
		OrdenaLigacaoDaRede(rede, lisLIG);
	}
	// faz uma segunda ordenação p/ transferir Ligacoes que ainda não foram alocadas
	for (int n = 0; (n < lisRR->Count) && (lisLIG->Count > 0); n++)
	{
		rede = (VTRede*)lisRR->Items[n];
		// ordena a Rede
		OrdenaLigacaoIsolada(rede, lisLIG);
	}
	// limpa todas listas
	LimpaListas();
}

// ---------------------------------------------------------------------------
void __fastcall TRebuild::LimpaListas(void)
{
	lisL->Clear();
	lisBAR->Clear();
	lisCHV->Clear();
}

// ---------------------------------------------------------------------------
bool __fastcall TRebuild::OrdenaLigacaoDaRede(VTRede *rede, TList *lisLIG)
{
	// variáveis locais
	VTBarra *pbarra, *bar_lig;
	VTLigacao *pliga;
	VTRede *rede_viz;

	// reinicia listas lisBAR, lisL, lisCHV
	LimpaListas();
	// inicia lisBAR c/ Barra inicial da Rede
	pbarra = rede->BarraInicial();
	if (pbarra)
		lisBAR->Add(pbarra);
	// determina Ligacoes em lisLIG a partir das Barras em lisBAR
	for (int n = 0; n < lisBAR->Count; n++)
	{
		pbarra = (VTBarra*)lisBAR->Items[n];
		// loop p/ todas Ligacoes em lisLIG
		for (int nl = lisLIG->Count - 1; nl >= 0; nl--)
		{
			pliga = (VTLigacao*)lisLIG->Items[nl];
			// verifica se a Ligacao está associada à rede
			if (pliga->rede != rede)
				continue;
			// verifica se a Ligacao conecta a Barra
			if (!pliga->ConectaBarra(pbarra))
				continue;
			// transfere a Ligacao de lisLIG p/ lisL
			lisLIG->Remove(pliga);
			if (lisL->IndexOf(pliga) < 0)
				lisL->Add(pliga);
			// não inclui as Barras em lisBAR se a Ligacao for Chave aberta
			if (pliga->Tipo() == eqptoCHAVE)
			{
				if (((VTChave*)pliga)->Aberta)
					continue;
			}
			// insere Barras da Ligacao em lisBAR
			for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
			{
				if ((bar_lig = pliga->Barra(ind_bar)) == NULL)
					continue;
				if (bar_lig == pbarra)
					continue;
				if (lisBAR->IndexOf(bar_lig) < 0)
					lisBAR->Add(bar_lig);
			}
		}
	}
	// reinicia listas de Barras e Ligacoes da Rede
	rede->IniciaLisLigacao(lisL);
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TRebuild::OrdenaLigacaoIsolada(VTRede *rede, TList *lisLIG)
{
	// variáveis locais
	bool barra_isolada;
	VTBarra *pbarra, *bar_lig;
	VTLigacao *pliga;
	VTRede *rede_viz;

	// reinicia listas lisBAR, lisL, lisCHV
	LimpaListas();
	// inicia lisBAR c/ todas Barras da Rede
	// pbarra = rede->BarraInicial();
	// if (pbarra) lisBAR->Add(pbarra);
	rede->LisBarra(lisBAR, barra_isolada = false);
	// determina Ligacoes em lisLIG a partir das Barras em lisBAR
	for (int n = 0; n < lisBAR->Count; n++)
	{
		pbarra = (VTBarra*)lisBAR->Items[n];
		// loop p/ todas Ligacoes em lisLIG
		for (int nl = lisLIG->Count - 1; nl >= 0; nl--)
		{
			pliga = (VTLigacao*)lisLIG->Items[nl];
			// verifica se a Ligacao conecta a Barra
			if (!pliga->ConectaBarra(pbarra))
				continue;
			// transfere a Ligacao de lisLIG p/ lisL
			lisLIG->Remove(pliga);
			if (lisL->IndexOf(pliga) < 0)
				lisL->Add(pliga);
			// não inclui as Barras em lisBAR se a Ligacao for Chave aberta
			if (pliga->Tipo() == eqptoCHAVE)
			{
				if (((VTChave*)pliga)->Aberta)
					continue;
			}
			// insere Barras da Ligacao em lisBAR
			for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
			{
				if ((bar_lig = pliga->Barra(ind_bar)) == NULL)
					continue;
				if (bar_lig == pbarra)
					continue;
				if (lisBAR->IndexOf(bar_lig) < 0)
					lisBAR->Add(bar_lig);
			}
		}
	}
	// verifica se foi encontrada novas Ligacoes para a Rede
	if (lisL->Count > 0)
	{ // inclui Ligacoes da Rede em lisL
		rede->LisLigacao(lisL);
		// reinicia listas de Barras e Ligacoes da Rede
		rede->IniciaLisLigacao(lisL);
	}
	return (true);
}

// ---------------------------------------------------------------------------
void __fastcall TRebuild::SeparaRedesEstaticasRedesReconfiguraveis(TList *lisREDE)
{
	// variáveis locais
	VTRede *rede;

	lisNR->Clear();
	lisRR->Clear();
	for (int n = 0; n < lisREDE->Count; n++)
	{
		rede = (VTRede*)lisREDE->Items[n];
		if (!rede->Carregada)
			continue;
		// if ((rede->Carregada)&&(! rede->Estatica))
		if ((rede->Carregada) && (!rede->Estatica) && (rede->BarraInicial() != NULL))
		{ // Rede radial não estática: insere em lisRR
			lisRR->Add(rede);
		}
		else
		{ // Rede não reconfigurável
			lisNR->Add(rede);
		}
	}
}

// ---------------------------------------------------------------------------
// eof
