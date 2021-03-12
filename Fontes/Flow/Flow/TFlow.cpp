// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <math.hpp>
#include "TFlow.h"
#include "TFlowNet.h"
#include "VTMThread.h"
#include "..\Diag\VTDiag.h"
#include "..\FluxoReducao\VTFluxoReducao.h"
#include "..\Redutor\VTReduz.h"
#include "..\..\Apl\VTApl.h"
#include "..\..\Cronometro\VTCronometro.h"
#include "..\..\Diretorio\VTPath.h"
#include "..\..\Licenca\VTLicenca.h"
#include "..\..\Licenca\VTLicencas.h"
#include "..\..\Log\VTLog.h"
#include "..\..\Ordena\VTOrdena.h"
#include "..\..\Radial\VTArvore.h"
#include "..\..\Radial\VTNode.h"
#include "..\..\Patamar\VTPatamares.h"
#include "..\..\Progresso\VTProgresso.h"
#include "..\..\Rede\VTCarga.h"
#include "..\..\Rede\VTMRede.h"
#include "..\..\Rede\VTMNet.h"
#include "..\..\Rede\VTNet.h"
#include "..\..\Rede\VTRede.h"
#include "..\..\Rede\VTRedes.h"
#include "..\..\Zona\VTZonas.h"
#include "..\..\..\DLL_Inc\Funcao.h"
#include "..\..\..\DLL_Inc\Log.h"
#include "..\..\..\DLL_Inc\Ordena.h"
#include "..\..\..\DLL_Inc\Radial.h"
#include "..\..\..\DLL_Inc\Progresso.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
VTFlow* __fastcall NewObjFlow(VTApl *apl_owner)
{
	return (new TFlow(apl_owner));
}

// ---------------------------------------------------------------------------
__fastcall TFlow::TFlow(VTApl *apl_owner)
{
	// cria objeto apl próprio
	apl = NewObjApl(NULL, apl_owner);
	// cria demais e insere em Apl local
	apl->Add(diag = NewObjDiag(apl));
	apl->Add(arvore = DLL_NewObjArvore(apl));
	apl->Add(reduz = NewObjReduz(apl));
	apl->Add(ordena = DLL_NewObjOrdena(apl));
	apl->Add(plog = DLL_NewObjLog(apl));
	apl->Add(plogthr = DLL_NewObjLog(apl));
	apl->Add(progresso = DLL_NewObjProgresso(NULL, NULL));
	lisEQP = new TList();
	lisNET = new TList();
	lisMNET_SEL = new TList();
	lisMNET_CAL = new TList();
	// pré-seleciona algoritmo de fluxo
	// PD.algoritmo_fluxo = fluxoGAUSS1;
	// PD.algoritmo_fluxo     = fluxoAUTO;

	TipoFluxoDefault();

	PD.algoritmo_fluxo_alt = fluxoDISABLED;
	// pré-seleciona modelo de carga: próprio de cada carga
	RedeComCarga = true;
	ModeloCarga = cargaAUTO;
	// inicia opções
	UpToDate = false;
	PorRedeEnabled = true;
	MultiThreadEnabled = true;
	PerdaFerroEnabled = true;
	TramoEnabled = true;
	ReguladorBypass = false;
	ProgressoEnabled = true;
	AvisoEnabled = false;
	CronometroEnabled = false;
	RelatorioBarDesc = false; // Não gera relatório de barras desconexas
	TipoRelatorio = relNENHUM;
	// define valor de base
	Sbase = 100.; // MVA
}

// ---------------------------------------------------------------------------
__fastcall TFlow::~TFlow(void)
{
	// destrói Apl e os objetos criados
	if (apl)
	{
		delete apl;
		apl = NULL;
	}
	// destrói lista sem destruir seus objetos
	if (lisEQP)
	{
		delete lisEQP;
		lisEQP = NULL;
	}
	if (lisMNET_SEL)
	{
		delete lisMNET_SEL;
		lisMNET_SEL = NULL;
	}
	if (lisMNET_CAL)
	{
		delete lisMNET_CAL;
		lisMNET_CAL = NULL;
	}
	// destrói lista e seus objetos
	if (lisNET)
	{
		LimpaTList(lisNET);
		delete lisNET;
		lisNET = NULL;
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TFlow::AlgoritmoEquilibrado(void)
{
	if (PD.algoritmo_fluxo == fluxoEQUILIBRADO)
		return (true);
	if (PD.algoritmo_fluxo == fluxoGAUSS1)
		return (true);
	if (PD.algoritmo_fluxo == fluxoNR1)
		return (true);
	if (PD.algoritmo_fluxo == fluxoRADIAL1)
		return (true);
	return (false);
}

// ---------------------------------------------------------------------------
bool __fastcall TFlow::CalculaPorRede(VTRedes *redes, bool dupla_precisao)
{
	// variáveis locais
	bool fluxo_ok = false;
	AnsiString strArqLogThr;
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
	VTCronometro *cronometro = (VTCronometro*)apl->GetObject(__classid(VTCronometro));
	VTZonas *zonas = (VTZonas*)apl->GetObject(__classid(VTZonas));
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));

	try
	{ // verifica se o fluxo está calculado
		if (UpToDate)
			return (true);
		/*
		 //verifica se o número de barras é pequeno
		 if ((redes->LisBarra())->Count < 1000)
		 //if (((redes->LisBarra())->Count < 200)||((redes->LisMNetFluxo())->Count == 1))
		 {//executa cálculo de rede única
		 return(CalculaRedeUnica(redes));
		 }
		 */
		// inicia Progresso
		if (ProgressoEnabled)
			progresso->Start(progDEFAULT);
		// salva ponteiro p/ objeto VTRedes
		this->redes = redes;
		// inicia cronometro
		if (CronometroEnabled)
			cronometro->Reinicia();
		if (CronometroEnabled)
			cronometro->Start("TFlow", "Fluxo por rede");
		// abre Log
		if (AvisoEnabled)
			plog->Open();
		// reinicia diagnóstido de fluxo correto p/ todos patamares
		patamares->ReiniciaDiagFluxo(true);
		// reinicia dados de fluxo da rede
		diag->ReiniciaResFluxo();
		// reinicia diagnóstico do flow p/ todas Redes
		diag->ReiniciaDiagFluxo(false);
		// monta Zonas da Rede
		zonas->ChaveAbertaEnabled = false;
		zonas->AvisoEnabled = AvisoEnabled;
		if (zonas->Executa(redes))
		{ // verifica se o uso de threads está habilitado
			if (MultiThreadEnabled)
			{ // executa cálculo por MNets usando Threads
				if (AvisoEnabled && (path != NULL))
					plogthr->Open(strArqLogThr = path->DirTmp() + "\\LogThr.txt");
				fluxo_ok = ExecutaCalculoPorRedeComThread(dupla_precisao);
				if (AvisoEnabled)
					plogthr->Close();
			}
			else
			{ // executa cálculo por MNets usando Threads
				fluxo_ok = ExecutaCalculoPorRedeSemThread(dupla_precisao);
			}
			if (fluxo_ok)
			{ // atualiza diagnóstico
				// diag->ReiniciaDiagFluxo(true);
				// calcula fluxo nas Ligacoes ficticias das Reducoes
				ExecutaCalculoReducaoFicticia();
				// finaliza cronômetro e exibe tempos
				if (CronometroEnabled)
					cronometro->Stop("TFlow");
				if (CronometroEnabled)
					cronometro->Show();
				// marca que o fluxo está calculado
				UpToDate = true;
				//atualiza o redutivel
                VerificaAlteracaoFluxoRedeNaoRedutivel();
			}
			else
			{ // erro no cálculo do fluxo
				diag->ReiniciaResFluxo();
				diag->ReiniciaDiagFluxo(false);
			}
		}
		// finaliza progresso
		if (ProgressoEnabled)
			progresso->Stop();
	}
	catch (Exception &e)
	{
		if (ProgressoEnabled)
			progresso->Stop();
		fluxo_ok = false;
	}
	return (fluxo_ok);
}

// ---------------------------------------------------------------------------
bool __fastcall TFlow::CalculaRedeUnica(VTRedes *redes)
{
	// variáveis locais
	bool show_prog;
	bool fluxo_ok = false;
	TList *lisRS;
	VTCronometro *cronometro = (VTCronometro*)apl->GetObject(__classid(VTCronometro));
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
	VTZonas *zonas = (VTZonas*)apl->GetObject(__classid(VTZonas));

	try
	{ // verifica se o fluxo está calculado
		if (UpToDate)
			return (true);
		// exibe progresso
		show_prog = ((redes->LisBarra())->Count > 1000);
		if (ProgressoEnabled && show_prog)
			progresso->Start(progDEFAULT);
		// salva ponteiro p/ objeto VTRedes
		this->redes = redes;
		// inicia cronometro
		if (CronometroEnabled)
			cronometro->Reinicia();
		if (CronometroEnabled)
			cronometro->Start("TFlow", "Fluxo único");
		// abre Log
		if (AvisoEnabled)
			plog->Open();
		// reinicia diagnóstido de fluxo correto p/ todos patamares
		patamares->ReiniciaDiagFluxo(true);
		// reinicia dados de fluxo da rede
		diag->ReiniciaResFluxo();
		// reinicia diagnóstico do flow p/ todas Redes
		diag->ReiniciaDiagFluxo(false);
		// monta Zonas da Rede
		zonas->ChaveAbertaEnabled = false;
		zonas->AvisoEnabled = AvisoEnabled;
		if (zonas->Executa())
		{ // executa cálculo único p/ todas as Redes
			fluxo_ok = ExecutaCalculoRedeUnica();
			if (fluxo_ok)
			{ // atualiza diagnóstico
				// diag->ReiniciaDiagFluxo(true);
				// calcula fluxo nas Ligacoes ficticias das Reducoes
				ExecutaCalculoReducaoFicticia();
				// reabilita mensagem de aviso p/ próxima execução
				// AvisoEnabled = true;
				if (CronometroEnabled)
					cronometro->Stop("TFlow");
				if (CronometroEnabled)
					cronometro->Show();
				// marca que o fluxo está calculado
				UpToDate = true;
			}
			else
			{ // erro no cálculo do fluxo
				diag->ReiniciaDiagFluxo(false);
			}
		}
		// finaliza progresso
		if (ProgressoEnabled && show_prog)
			progresso->Stop();
	}
	catch (Exception &e)
	{
		if (ProgressoEnabled && show_prog)
			progresso->Stop();
		fluxo_ok = false;
	}
	return (fluxo_ok);
}

// ---------------------------------------------------------------------------
bool __fastcall TFlow::CalculaUmaRede(VTRedes *redes, VTRede *rede)
{
	int falta_implementar = 1;

	return (true);
}

/*
 //---------------------------------------------------------------------------
 void __fastcall TFlow::DebugRedesMNet(VTMNet  *mnet)
 {
 //variáveis locais
 AnsiString codigo;
 TList   *lisMREDE, *lisREDE;
 VTMRede *mrede;
 VTRede  *rede;

 lisMREDE = mnet->LisMRede();
 for (int j = 0; j < lisMREDE->Count; j++)
 {
 mrede = (VTMRede*)lisMREDE->Items[j];
 lisREDE = mrede->LisRede();
 for (int k = 0; k < lisREDE->Count; k++)
 {
 rede = (VTRede*)lisREDE->Items[k];
 codigo = rede->Codigo;
 }
 }
 }
 */

// ---------------------------------------------------------------------------
bool __fastcall TFlow::ExecutaCalculoMNet(VTMNet *mnet)
{
	// variáveis locais
	bool sucesso;
	VTFlowNet *flow_net;

	// ocria objeto VTFlowNet para cálculo de fluxo da MNet
	if ((flow_net = NewObjFlowNet(apl)) == NULL)
		return (false);
	// calcula Fluxo de Potência p/ VTNet
	sucesso = flow_net->Executa(mnet);
	// destrói objeto VTFlowNet
	delete flow_net;
	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TFlow::ExecutaCalculoPorRedeComThread(bool dupla_precisao)
{
	// variáveis locais
	bool excluir;
	int num_iteracao;
	TList *lisMNET_ALL;
	VTMNet *mnet;
	VTMThread *mthread;

	// obtém lista de MNet
	lisMNET_ALL = redes->LisMNetFluxo();
	// reinicia Arvore de MNet
	if (!MontaArvoreMNet(lisMNET_ALL))
		return (false);
	// cria objeto VTMThread
	mthread = NewObjMThread(apl);
	// cria um objeto VTNet para cada VTMNet
	IniciaLisNet(lisNET, lisMNET_ALL);
	// reinicia Suprimentos das Reducoes c/ 1 pu de tensão
	for (int n = 0; n < lisMNET_ALL->Count; n++)
	{
		mnet = (VTMNet*)lisMNET_ALL->Items[n];
		mnet->Enabled = true;
		reduz->ReiniciaReducaoSuprimento1pu(mnet);
	}
	// ==============================================
	// calcula fluxo, percorrendo a árvore de MNet de baixo para cima, sem calcular as MNet com geração
	// ==============================================
	plogthr->Add("Árvore de MNet de baixo para cima");
	// inicia lisMNET_SEL com as MNets que não possuem MNets filhas
	lisMNET_SEL->Clear();
	SelecionaMNetCargaSemMNetFilha(lisMNET_ALL, lisMNET_SEL);
	do
	{ // loop p/ todas MNet prontas p/ o cálculo em lisMNET_SEL
		if (lisMNET_SEL->Count > 0)
		{ // retira primeira MNet de lisMNET_SEL
			mnet = (VTMNet*)lisMNET_SEL->First();
			lisMNET_SEL->Delete(0);
			// inclui no log a MNet cuja thread é iniciada
			// LogNetIniciada(mnet);
			// executa cálculo de fluxo para a MNet (bloqueia em WaitFor caso não haja processador disponível)
			mthread->ExecutaFluxoPorThread(mnet);
		}
		// obtém lista de MNets calculadas (bloqueia em WaitFor caso não haja processador disponível)
		lisMNET_CAL->Clear();
		if (mthread->LisMNetCalculada(lisMNET_CAL) > 0)
		{ // inclui no log as MNets calculadas
			LogNetCalculada(lisMNET_CAL);
			// trata MNet que não foram calculadas corretamente
			TrataMNetComErroNoFluxo(lisMNET_CAL, excluir = false);
			// substitui MNet por sua MNet pai em lisMNET_CAL
			SubsituiMNetFilhaPorMNetPai(lisMNET_CAL);
			// inclui em lisMNET_SEL as MNet pai cujas MNet filhas já foram calculadas
			SelecionaMNetCargaComTodasMNetFilhasCalculadas(lisMNET_CAL, lisMNET_SEL);
		}
		// ============================
		// SOLUÇÃO TEMPORÁRIA para não bloquear a barrra de progresso
		Application->ProcessMessages();
	}
	while ((lisMNET_SEL->Count > 0) || (mthread->NumeroThreadAtiva() > 0));
	// ==============================================
	// calcula fluxo, percorrendo a árvore de MNet de cima, a partir das MNet com geração
	// ==============================================
	plogthr->Add("\nÁrvore de MNet de cima para baixo");
	// define número de iterações em função precisão desejada
	num_iteracao = (dupla_precisao) ? 2 : 1;
	for (int iteracao = 1; iteracao <= num_iteracao; iteracao++)
	{ // inicia lisMNET_SEL com as MNets de geração
		lisMNET_SEL->Clear();
		SelecionaMNetGeracao(lisMNET_ALL, lisMNET_SEL);
		do
		{ // loop p/ todas MNet prontas p/ o cálculo em lisMNET_SEL
			if (lisMNET_SEL->Count > 0)
			{ // retira primeira MNet de lisMNET_SEL
				mnet = (VTMNet*)lisMNET_SEL->First();
				lisMNET_SEL->Delete(0);
				// inclui no log a MNet cuja thread é iniciada
				// LogNetIniciada(mnet);
				// executa cálculo de fluxo para a MNet (bloqueia em WaitFor caso não haja processador disponível)
				mthread->ExecutaFluxoPorThread(mnet);
			}
			// obtém lista de MNets calculadas (bloqueia em WaitFor caso não haja processador disponível)
			lisMNET_CAL->Clear();
			if (mthread->LisMNetCalculada(lisMNET_CAL) > 0)
			{ // inclui no log as MNets calculadas
				LogNetCalculada(lisMNET_CAL);
				// trata MNet que não foram calculadas corretamente
				TrataMNetComErroNoFluxo(lisMNET_CAL, excluir = true);
				// inclui em lisMNET_SEL as MNet filhas
				SelecionaMNetFilhas(lisMNET_CAL, lisMNET_SEL);
			}
			// ============================
			// SOLUÇÃO TEMPORÁRIA para não bloquear a barrra de progresso
			// Application->ProcessMessages();
		}
		while ((lisMNET_SEL->Count > 0) || (mthread->NumeroThreadAtiva() > 0));
	}
	// destroi objeto VTMThread
	delete mthread;
	// destroi objetos VTNet criados
	LimpaTList(lisNET);
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TFlow::ExecutaCalculoPorRedeSemThread(bool dupla_precisao)
{
	// variáveis locais
	bool excluir;
	int num_iteracao;
	VTMNet *mnet;
	TList *lisMNET;

	// obtém lista de MNet
	lisMNET = redes->LisMNetFluxo();
	// reinicia Arvore de MNet
	if (!MontaArvoreMNet(lisMNET))
		return (false);
	// cria um objeto VTNet para cada VTMNet
	IniciaLisNet(lisNET, lisMNET);
	// reinicia Suprimentos das Reducoes c/ 1 pu de tensão
	for (int n = 0; n < lisMNET->Count; n++)
	{
		mnet = (VTMNet*)lisMNET->Items[n];
		mnet->Enabled = true;
		reduz->ReiniciaReducaoSuprimento1pu(mnet);
	}
	// calcula fluxo apenas das MNet de Carga: ordem inversa das MNet em lisMNET
	for (int n = lisMNET->Count - 1; n >= 0; n--)
	{
		mnet = (VTMNet*)lisMNET->Items[n];
		// interrompe ao encontrar a primeira MNet de geração
		if (mnet->Geracao)
			break;
		// calcula Fluxo de Potência p/ VTMNet
		if (!ExecutaCalculoMNet(mnet)) // return(false);
		{ // desabilita MNet do proccesso de cálculo de fluxo
			mnet->Enabled = false;
			// trata MNet que não foram calculadas corretamente
			TrataMNetComErroNoFluxo(mnet);
		}
	}
	// define número de iterações em função precisão desejada
	num_iteracao = (dupla_precisao) ? 2 : 1;
	for (int iteracao = 1; iteracao <= num_iteracao; iteracao++)
	{ // calcula fluxo das MNet na ordem direta das MNet em lisMNET
		for (int n = 0; n < lisMNET->Count; n++)
		{
			mnet = (VTMNet*)lisMNET->Items[n];
			// verifica se MNet está habilitada
			if (!mnet->Enabled)
				continue;
			// calcula Fluxo de Potência p/ VTMNet
			if (!ExecutaCalculoMNet(mnet)) // return(false);
			{ // desabilita MNet do proccesso iterativo de cálculo de fluxo
				mnet->Enabled = false;
				// trata MNet que não foram calculadas corretamente
				TrataMNetComErroNoFluxo(mnet);
			}
		}
	}
	// destrói objetos VTNet criados
	LimpaTList(lisNET);
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TFlow::ExecutaCalculoRedeUnica(void)
{
	// variáveis locais
	AnsiString txt;
	bool sucesso;
	int num_pat_ok, num_pat_erro;
	TList *lisMR;
	TList *lisREDE;
	VTNet *net;
	VTMRede *mrede;
	VTRede *rede;
	VTFlowNet *flow_net;
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

	// cria objeto VTNet p/ as Redes carregadas
	ordena->ChaveNA_Enabled = false;
	ordena->ChaveNF_Enabled = (PD.algoritmo_fluxo == fluxoHARMO3);
	ordena->TramoEnabled = TramoEnabled;
	if ((net = ordena->Executa(redes)) == NULL)
		return (false);
	if ((flow_net = NewObjFlowNet(apl)) == NULL)
		return (false);
	// calcula Fluxo de Potência p/ VTNet
	sucesso = flow_net->Executa(net);
	// atualiza informação de fluxo correto ou não das Redes
	lisMR = redes->LisMRede();
	for (int i = 0; i < lisMR->Count; i++)
	{
		mrede = (VTMRede*)lisMR->Items[i];
		lisREDE = mrede->LisRede();
		for (int j = 0; j < lisREDE->Count; j++)
		{
			rede = (VTRede*)lisREDE->Items[j];
			rede->FluxoOK = sucesso;
		}
	}
	// gera ensagem de alerta
	if (!sucesso)
	{ // determina número de patamares calculados e número de patamares com erro
		num_pat_ok = net->StatusFluxoNumPatOK;
		num_pat_erro = patamares->NumPatamar() - num_pat_ok;
		// define texto para Alerta
		if (num_pat_ok == 0)
			txt = "em todos patamares";
		else if (num_pat_erro == 1)
			txt = "em 1 patamar";
		else
			txt.sprintf("em %d patamares", num_pat_erro);
		// gera mensagem de alerta
		Alerta("Fluxo de potência não calculado p/ todas redes " + txt);
	}
	// destrói objetos
	delete flow_net;
	delete net;
	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TFlow::ExecutaCalculoReducaoFicticia(void)
{
	// variáveis locais
	VTFluxoReducao *fluxo;

	try
	{ // cria obejto VTFluxoReducao
		fluxo = NewObjFluxoReducao(apl);
		// executa cálculo de fluxo na Ligacoes fictícias das Reducoes
		fluxo->Executa(redes, Sbase);
		// destrói objeto
		delete fluxo;
	}
	catch (Exception &e)
	{
		return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
void __fastcall TFlow::GeraMsgAlerta(VTMNet *mnet)
{
	// variáveis locais
	AnsiString txt;
	int num_pat_ok, num_pat_erro;
	TList *lisREDE;
	TList *lisMREDE;
	VTMRede *mrede;
	VTRede *rede;
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

	// proteção
	if (mnet == NULL)
		return;
	// determina número de patamares calculados e número de patamares com erro
	num_pat_ok = mnet->Net->StatusFluxoNumPatOK;
	num_pat_erro = patamares->NumPatamar() - num_pat_ok;
	// loop p/ todas MRede da MNet
	lisMREDE = mnet->LisMRede();
	for (int i = 0; i < lisMREDE->Count; i++)
	{
		mrede = (VTMRede*)lisMREDE->Items[i];
		lisREDE = mrede->LisRede();
		for (int j = 0; j < lisREDE->Count; j++)
		{
			rede = (VTRede*)lisREDE->Items[j];
			if (!rede->Carregada)
				continue;
			// define texto para Alerta
			if (num_pat_ok == 0)
				txt = " em todos patamares";
			else if (num_pat_erro == 1)
				txt = " em 1 patamar";
			else
				txt.sprintf(" em %d patamares", num_pat_erro);
			// gera mensagem de alerta
			Alerta("Fluxo de potência não calculado p/ a rede " + AnsiQuotedStr(rede->Codigo,
				'\'') + txt);
		}
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TFlow::IniciaLisNet(TList *lisNET, TList *lisMNET)
{
	// variáveis locais
	VTMNet *mnet;

	// destrói eventuais Nets criadas anteriormente
	LimpaTList(lisNET);
	// configura opções para montar Net
	ordena->ChaveNA_Enabled = false;
	ordena->ChaveNF_Enabled = (PD.algoritmo_fluxo == fluxoHARMO3);
	ordena->TramoEnabled = TramoEnabled;
	ordena->ReguladorBypass = ReguladorBypass;
	// cria uma Net para cada MNet
	for (int n = 0; n < lisMNET->Count; n++)
	{
		mnet = (VTMNet*)lisMNET->Items[n];
		// cria Net
		mnet->Net = ordena->Executa(mnet);
		if (mnet->Net != NULL)
		{ // define seu código da Net e salva em lisNET
			mnet->Net->Codigo = mnet->Codigo;
			lisNET->Add(mnet->Net);
		}
	}
	return (true);
}

// ---------------------------------------------------------------------------
void __fastcall TFlow::LogNetCalculada(TList *lisMNET)
{
	// variáveis locais
	TList *lisMREDE, *lisREDE;
	VTMNet *mnet;
	VTMRede *mrede;
	VTRede *rede;

	for (int i = 0; i < lisMNET->Count; i++)
	{
		mnet = (VTMNet*)lisMNET->Items[i];
		plogthr->Add(AnsiString("MNET " + mnet->Codigo + ": cálculo do fluxo " + ((mnet->Enabled) ?
			"correto" : "ERRO")));
		lisMREDE = mnet->LisMRede();
		for (int j = 0; j < lisMREDE->Count; j++)
		{
			mrede = (VTMRede*)lisMREDE->Items[j];
			plogthr->Add("\tMREDE " + IntToStr(j + 1));
			lisREDE = mrede->LisRede();
			for (int k = 0; k < lisREDE->Count; k++)
			{
				rede = (VTRede*)lisREDE->Items[k];
				plogthr->Add("\t\tRede " + rede->Codigo);
			}
		}
	}
}

/*
 //---------------------------------------------------------------------------
 void __fastcall TFlow::LogNetIniciada(VTMNet  *mnet)
 {
 plogthr->Add(AnsiString("MNET " + mnet->Codigo + " - Thread iniciada"));
 }
 */
// ---------------------------------------------------------------------------
bool __fastcall TFlow::MontaArvoreMNet(TList *lisMNET)
{
	// variáveis locais
	VTNode *node, *node_pai;
	VTMNet *mnet;

	// as MNet estão ordenadas em lisMNET:
	// 1) as MNet de geraçao estão no início da lista
	// 2) cada MNet possui ponteiro para MNet pai
	// reinicia Arvore
	arvore->Clear();
	// loop p/ todas MNet el lisMNET
	for (int n = 0; n < lisMNET->Count; n++)
	{
		mnet = (VTMNet*)lisMNET->Items[n];
		// verifica se MNet é de geração
		if (mnet->Geracao)
		{ // inicia um novo ramo na Arvore
			node = arvore->InsereNode(NULL);
			// insere MNet no Node
			node->InsereObject(mnet);
			// associa MNet com Node
			mnet->Obj = node;
		}
		// verifica se a MNet possui MNet pai
		else if (mnet->Pai != NULL)
		{ // determina Node da MNet pai
			node_pai = (VTNode*)(mnet->Pai->Obj);
			// insere um novo Node no Node pai
			node = arvore->InsereNode(node_pai);
			// insere MNet no Node
			node->InsereObject(mnet);
			// associa MNet com Node
			mnet->Obj = node;
		}
		else
		{ // MNet isolada: MNet não é de geração e não possui MNet pai
			// nada a fazer
		}
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TFlow::PM_GetAvisoEnabled(void)
{
	return (PD.aviso_enabled);
}

// ---------------------------------------------------------------------------
bool __fastcall TFlow::PM_GetCronometroEnabled(void)
{
	return (PD.crono_enabled);
}

// ---------------------------------------------------------------------------
int __fastcall TFlow::PM_GetModeloCarga(void)
{
	return (PD.modelo_carga);
}

// ---------------------------------------------------------------------------
bool __fastcall TFlow::PM_GetMultiThreadEnabled(void)
{
	return (PD.threads_enabled);
}

// ---------------------------------------------------------------------------
bool __fastcall TFlow::PM_GetPerdaFerroEnabled(void)
{
	return (PD.perda_ferro_enabled);
}

// ---------------------------------------------------------------------------
bool __fastcall TFlow::PM_GetPorRedeEnabled(void)
{
	return (PD.por_rede_enabled);
}

// ---------------------------------------------------------------------------
bool __fastcall TFlow::PM_GetProgressoEnabled(void)
{
	return (PD.progresso_enabled);
}

// ---------------------------------------------------------------------------
bool __fastcall TFlow::PM_GetRedeComCarga(void)
{
	return (PD.rede_com_carga);
}

// ---------------------------------------------------------------------------
bool __fastcall TFlow::PM_GetReguladorBypass(void)
{
	return (PD.bypass_enabled);
}

// ---------------------------------------------------------------------------
bool __fastcall TFlow::PM_GetRelatorioBarDesc(void)
{
	return (PD.relatorio_bar_desc_enabled);
}

// ---------------------------------------------------------------------------
int __fastcall TFlow::PM_GetTipoFluxo(void)
{
	return (PD.algoritmo_fluxo);
}

// ---------------------------------------------------------------------------
int __fastcall TFlow::PM_GetTipoFluxoAlt(void)
{
	return (PD.algoritmo_fluxo_alt);
}

// ---------------------------------------------------------------------------
int __fastcall TFlow::PM_GetTipoRelatorio(void)
{
	return (PD.tipo_relatorio);
}

// ---------------------------------------------------------------------------
bool __fastcall TFlow::PM_GetTramoEnabled(void)
{
	return (PD.tramo_enabled);
}

// ---------------------------------------------------------------------------
bool __fastcall TFlow::PM_GetUpToDate(void)
{
	return (PD.uptodate);
}

// ---------------------------------------------------------------------------
void __fastcall TFlow::PM_SetAvisoEnabled(bool enabled)
{
	PD.aviso_enabled = enabled;
}

// ---------------------------------------------------------------------------
void __fastcall TFlow::PM_SetCronometroEnabled(bool enabled)
{
	PD.crono_enabled = enabled;
}

// ---------------------------------------------------------------------------
void __fastcall TFlow::PM_SetModeloCarga(int modelo_carga)
{
	switch (modelo_carga)
	{
	case cargaAUTO:
		PD.modelo_carga = cargaAUTO;
		break;
	case cargaICTE:
		PD.modelo_carga = cargaICTE;
		break;
	case cargaSCTE:
		PD.modelo_carga = cargaSCTE;
		break;
	case cargaZCTE:
		PD.modelo_carga = cargaZCTE;
		break;
	case cargaANEEL:
		PD.modelo_carga = cargaANEEL;
		break;
	default:
		PD.modelo_carga = cargaAUTO;
		break;
	}
	// atualiza modelo de carga de VTReduz
	reduz->ModeloCarga = ModeloCarga;
}

// ---------------------------------------------------------------------------
void __fastcall TFlow::PM_SetMultiThreadEnabled(bool enabled)
{
	// variáveis locais
	SYSTEM_INFO sysInfo;

	// habilita uso de threads somente se houver mais de um processador
	GetSystemInfo(&sysInfo);
	PD.threads_enabled = (enabled && (sysInfo.dwNumberOfProcessors > 1));
}

// ---------------------------------------------------------------------------
void __fastcall TFlow::PM_SetPerdaFerroEnabled(bool enabled)
{
	PD.perda_ferro_enabled = enabled;
}

// ---------------------------------------------------------------------------
void __fastcall TFlow::PM_SetPorRedeEnabled(bool enabled)
{
	PD.por_rede_enabled = enabled;
}

// ---------------------------------------------------------------------------
void __fastcall TFlow::PM_SetProgressoEnabled(bool enabled)
{
	PD.progresso_enabled = enabled;
}

// ---------------------------------------------------------------------------
void __fastcall TFlow::PM_SetRedeComCarga(bool rede_com_carga)
{
	PD.rede_com_carga = rede_com_carga;
}

// ---------------------------------------------------------------------------
void __fastcall TFlow::PM_SetReguladorBypass(bool enabled)
{
	PD.bypass_enabled = enabled;
}

// ---------------------------------------------------------------------------
void __fastcall TFlow::PM_SetRelatorioBarDesc(bool enabled)
{
	PD.relatorio_bar_desc_enabled = enabled;
}

// ---------------------------------------------------------------------------
void __fastcall TFlow::PM_SetTipoFluxo(int tipo_fluxo)
{
	PD.algoritmo_fluxo = tipo_fluxo;
}

// ---------------------------------------------------------------------------
void __fastcall TFlow::PM_SetTipoFluxoAlt(int tipo_fluxo)
{
	PD.algoritmo_fluxo_alt = tipo_fluxo;
}

// ---------------------------------------------------------------------------
void __fastcall TFlow::PM_SetTipoRelatorio(int tipo_rel)
{
	PD.tipo_relatorio = tipo_rel;
}

// ---------------------------------------------------------------------------
void __fastcall TFlow::PM_SetTramoEnabled(bool enabled)
{
	PD.tramo_enabled = enabled;
}

// ---------------------------------------------------------------------------
void __fastcall TFlow::PM_SetUpToDate(bool uptodate)
{
	PD.uptodate = uptodate;
}

// ---------------------------------------------------------------------------
int __fastcall TFlow::SelecionaMNetCargaComTodasMNetFilhasCalculadas(TList *lisMNET_ORIG,
	TList *lisMNET_DEST)
{
	// variáveis locais
	bool filhas_calculadas;
	int count = 0;
	VTMNet *mnet, *mnet_filha;

	// loop para todas MNet em lisMNET_ORIG
	for (int n = 0; n < lisMNET_ORIG->Count; n++)
	{
		mnet = (VTMNet*)lisMNET_ORIG->Items[n];
		if (mnet->Geracao)
			continue;
		// monta lista com MNet filhas
		lisEQP->Clear();
		SelecionaMNetFilhas(mnet, lisEQP);
		// verifica se todas MNets filhas foram calculadas
		filhas_calculadas = true;
		for (int i = 0; i < lisEQP->Count; i++)
		{
			mnet_filha = (VTMNet*)lisEQP->Items[i];
			if (mnet_filha->Net->StatusFluxo != fluxoCALCULADO)
			{ // MNet filha ainda não calculada
				filhas_calculadas = false;
				break;
			}
		}
		if (filhas_calculadas)
		{ // inclui MNet em lisMNET_DEST
			if (lisMNET_DEST->IndexOf(mnet) < 0)
			{
				lisMNET_DEST->Add(mnet);
				count++;
			}
		}
	}
	return (count);
}

// ---------------------------------------------------------------------------
int __fastcall TFlow::SelecionaMNetCargaSemMNetFilha(TList *lisMNET_ORIG, TList *lisMNET_DEST)
{
	// variáveis locais
	int count = 0;
	TList *lisNODE;
	VTMNet *mnet;
	VTNode *node;

	// loop para todas MNet em lisMNET_ORIG
	for (int n = lisMNET_ORIG->Count - 1; n >= 0; n--)
	{
		mnet = (VTMNet*)lisMNET_ORIG->Items[n];
		if (mnet->Geracao)
			break;
		// obtém VTNode associada à MNet
		if ((node = (VTNode*)mnet->Obj) == NULL)
			continue;
		// obtém lista de Nodes filhos
		lisNODE = node->LisNode();
		// verifica se a lista de Nodes filhos está vazia
		if (lisNODE->Count == 0)
		{ // inclui MNet em lisMNET_DEST
			if (lisMNET_DEST->IndexOf(mnet) < 0)
			{
				lisMNET_DEST->Add(mnet);
				count++;
			}
		}
	}
	return (count);
}

// ---------------------------------------------------------------------------
int __fastcall TFlow::SelecionaMNetGeracao(TList *lisMNET_ORIG, TList *lisMNET_DEST)
{
	// variáveis locais
	int count = 0;
	VTMNet *mnet;

	// loop para todas MNet em lisMNET_ORIG
	for (int n = 0; n < lisMNET_ORIG->Count; n++)
	{
		mnet = (VTMNet*)lisMNET_ORIG->Items[n];
		if (!mnet->Geracao)
			break;
		// inclui MNet em lisMNET_DEST
		if (lisMNET_DEST->IndexOf(mnet) < 0)
		{
			lisMNET_DEST->Add(mnet);
			count++;
		}
	}
	return (count);
}

// ---------------------------------------------------------------------------
int __fastcall TFlow::SelecionaMNetFilhas(TList *lisMNET_ORIG, TList *lisMNET_DEST)
{
	// variáveis locais
	int count = 0;
	VTMNet *mnet;

	// loop para todas MNet em lisMNET_ORIG
	for (int n = 0; n < lisMNET_ORIG->Count; n++)
	{
		mnet = (VTMNet*)lisMNET_ORIG->Items[n];
		// descarta MNet não calculada
		if (!mnet->Enabled)
			continue;
		// obtém MNets filhas da MNet
		count += SelecionaMNetFilhas(mnet, lisMNET_DEST);
	}
	return (count);
}

// ---------------------------------------------------------------------------
int __fastcall TFlow::SelecionaMNetFilhas(VTMNet *mnet, TList *lisMNET_DEST)
{
	// variáveis locais
	int count = 0;
	TList *lisNODE, *lisOBJ;
	VTMNet *mnet_filha;
	VTNode *node, *node_filho;

	// DebugRedesMNet(mnet);
	// obtém VTNode associada à MNet
	if ((node = (VTNode*)mnet->Obj) != NULL)
	{ // obtém lista de Nodes filhos
		lisNODE = node->LisNode();
		// loop p/ todos Nodes em lisNODE
		for (int n = 0; n < lisNODE->Count; n++)
		{
			node_filho = (VTNode*)lisNODE->Items[n];
			// obtém lista de objetos do Node (que tem penas uma VTMNet)
			lisOBJ = node_filho->LisObjectNode();
			if (lisOBJ->Count == 1)
			{
				mnet_filha = (VTMNet*)lisOBJ->First();
				// descarta MNet não calculada
				if (!mnet_filha->Enabled)
					continue;
				// insere VTMNet filha em lisMNET_DEST
				if (lisMNET_DEST->IndexOf(mnet_filha) < 0)
				{
					lisMNET_DEST->Add(mnet_filha);
					count++;
					// DebugRedesMNet(mnet_filha);
				}
			}
		}
	}
	return (count);
}

// ---------------------------------------------------------------------------
int __fastcall TFlow::SubsituiMNetFilhaPorMNetPai(TList *lisMNET)
{
	// variáveis locais
	VTMNet *mnet;

	// loop para todas MNet em lisMNET_
	for (int n = lisMNET->Count - 1; n >= 0; n--)
	{
		mnet = (VTMNet*)lisMNET->Items[n];
		if (mnet->Pai != NULL)
		{ // substitui MNet da lista pela MNet pai
			lisMNET->Items[n] = mnet->Pai;
		}
		else
		{ // retira MNet da lista
			lisMNET->Delete(n);
		}
	}
	return (lisMNET->Count);
}

// ---------------------------------------------------------------------------
void __fastcall TFlow::TipoFluxoDefault(void)
{
	// variaveis locais
	VTLicencas *licencas = (VTLicencas*)apl->GetObject(__classid(VTLicencas));
	VTLicenca *licenca;

	if (licencas != NULL)
	{
		licenca = licencas->ExisteLicenca();
		if (licenca != NULL)
		{
			if (licenca->Empresa == COPEL)
			{
				PD.algoritmo_fluxo = fluxoEQUILIBRADO;
				return;
			}
		}
	}

	PD.algoritmo_fluxo = fluxoAUTO;
}

// ---------------------------------------------------------------------------
void __fastcall TFlow::TrataMNetComErroNoFluxo(TList *lisMNET, bool excluir)
{
	// variáveis locais
	VTMNet *mnet;

	// loop para todas MNet em lisMNET_
	for (int n = lisMNET->Count - 1; n >= 0; n--)
	{
		mnet = (VTMNet*)lisMNET->Items[n];
		if (!mnet->Enabled)
		{ // retira MNet da lista
			if (excluir)
				lisMNET->Delete(n);
			// considera que todas as MNets filhas não foram calculadas
			TrataMNetComErroNoFluxo(mnet);
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFlow::TrataMNetComErroNoFluxo(VTMNet *mnet)
{
	// variáveis locais
	VTNode *node;

	// gera msg de alerta
	GeraMsgAlerta(mnet);
	// considera que todas as MNets filhas não foram calculadas
	node = (VTNode*)mnet->Obj;
	lisEQP->Clear();
	// JCG 2014.11.17 - as redes da MNet já estão atualizadas
	// node->LisObjectNode(lisEQP);
	node->LisObjectJusante(lisEQP);
	for (int i = 0; i < lisEQP->Count; i++)
	{
		mnet = (VTMNet*)lisEQP->Items[i];
		mnet->Enabled = false;
		// reinicia resultados de fluxo p/ toda Net
		diag->ReiniciaResFluxoBarra(mnet->Net->LisBarra());
		diag->ReiniciaResFluxoLigacao(mnet->Net->LisLigacao());
		diag->ReiniciaResFluxoLigacaoNo(mnet->Net->LisNo());
	}
}
//---------------------------------------------------------------------------
void   __fastcall TFlow::VerificaAlteracaoFluxoRedeNaoRedutivel(void)
{
	VTRede *rede;
	bool bolRedutivel;
	TList *lisRedes;

	lisRedes = redes->LisRede();

	for (int nr = 0; nr < lisRedes->Count; nr++)
	{
		rede = (VTRede*) lisRedes->Items[nr];
		if (rede->Carregada)
		{
			if (rede->MRede)
			{
				if (rede->MRede->Id == rede->Id) //teste
				{   //se nao for redutivel e o fluxo rodou, provavel q agora seja redutivel
					if (!rede->MRede->Redutivel && rede->FluxoOK)
					{
						rede->Status[sttALTERADO] = true;
                    }
				}
			}
			else
			{
				//faz nada
			}
		}
	}
}
// ---------------------------------------------------------------------------
// eof
