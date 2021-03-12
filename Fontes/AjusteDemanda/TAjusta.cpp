// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Math.hpp>
#include "TBackup.h"
#include "VTAjusteFatores.h"
#include "VTAjusteFator.h"
#include "..\Apl\VTApl.h"
#include "..\AjusteDemanda\Grupo\VTGerenciaGrupos.h"
#include "..\AjusteDemanda\Grupo\VTGrupos.h"
#include "..\AjusteDemanda\Grupo\VTGrupo.h"
#include "..\Bloco\VTBloco.h"
#include "..\Bloco\VTBlocos.h"
#include "..\Bloco\VTElo.h"
#include "..\Classes\VTClasse.h"
#include "..\Complexo\Complexo.h"
#include "..\Cronometro\VTCronometro.h"
#include "..\Curvas\VTCurva.h"
#include "..\Edita\VTEdita.h"
#include "..\Editor\VTEditor.h"
#include "..\Flow\VTFlow.h"
#include "..\Grafico\VTGrafico.h"
#include "..\ImportaMedicao\VTImportaMedicao.h"
#include "..\ImportaMedicao\VTMedicoes.h"
#include "..\ImportaMedicao\VTMedicao.h"
#include "..\Licenca\VTLicenca.h"
#include "..\Licenca\VTLicencas.h"
#include "..\Obra\VTObra.h"
#include "..\Patamar\VTPatamar.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Perdatec\VTPerdatec.h"
#include "..\Potencia\VTDemanda.h"
#include "..\Progresso\VTProgresso.h"
#include "..\Radial\VTArvore.h"
#include "..\Radial\VTNode.h"
#include "..\Radial\VTPrimario.h"
#include "..\Radial\VTRadial.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCanal.h"
#include "..\Rede\TCanal.h"
#include "..\Rede\VTCapacitor.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTFases.h"
#include "..\Rede\VTGerador.h"
#include "..\Rede\VTMedidor.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTResFlowLig.h"
#include "..\Rede\VTResFlowSup.h"
#include "..\Rede\VTTipoChave.h"
#include "..\Rede\VTTipoRede.h"
#include "..\Rede\VTTrecho.h"
#include "..\Sinap\VTSinapChild.h"
#include "..\Zona\VTZona.h"
#include "..\Zona\VTZonas.h"
#include "..\..\DLL_Inc\Bloco.h"
#include "..\..\DLL_Inc\Cronometro.h"
#include "..\..\DLL_Inc\Curvas.h"
#include "..\..\DLL_Inc\Edita.h"
#include "..\..\DLL_Inc\Editor.h"
#include "..\..\DLL_Inc\Flow.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\ImportaMedicao.h"
#include "..\..\DLL_Inc\Progresso.h"
#include "..\..\DLL_Inc\Radial.h"
//#include "..\..\..\ModulosOpcionais\Fontes\AjusteBatch\VTBatchAjusteTask.h"
#include <ModulosOpcionais\Fontes\AjusteBatch\VTBatchAjusteTask.h>
#include "TAjusta.h"
#include "VTBackup.h"
#include "TErroAjuste.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall TAjusta::TAjusta(VTApl *apl)
{
	// salva ponteiro p/ objetos externos
//	this->apl = NewObjApl(NULL, apl);    //classes derivadas criam
	this->apl =	apl;
	// inicia dados
	PD.incluir_consA4 = true;
	PD.incluir_perda = true;
	PD.limite_ajuste = false;
	PD.usa_medicao = true;
	PD.tipo_fluxo = fluxoAUTO;
	PD.fatcor_unico = 0.;
	PD.erro_maximo = 0.02;
	PD.tempo = 0.;
	// verifica se a licen�a � da Copel
	if (LicencaCopel())
	{
		PD.tipo_fluxo = fluxoEQUILIBRADO;
	}
	// ajusta fatores
	// PD.fatores.tipo = adtfCOUNT;
	// PD.fatores.multP = 0.;
	// PD.fatores.multQ = 0.;
	// PD.fatores.fp	= 0.;
	// define se mostra/esconde progresso
	PD.prog_enabled = true;
	// define ajuste m�ximo com valor nulo p/ desabilitar a verifica��o de medi��o incompat�vel
	PD.ajuste_maximo = 0.50;
	PD.batch_mode = false;
	// cria objetos locais
	lisEQP = new TList();
	lisCARGA_ESP = new TList();
	lisREDE_VAL = new TList();
	lisFATORES = new TList();
	lisMEDICAO_FIC = new TList();
	// cria objs. no Apl
	apl->Add(DLL_NewObjBlocos());
	apl->Add(DLL_NewObjEditor(apl));
	apl->Add(DLL_NewObjFlow(apl));
	apl->Add(DLL_NewObjRadial(apl));
	apl->Add(DLL_NewObjMedicoes(apl));
	apl->Add(DLL_NewObjImportaMedicao(NULL, apl));
	apl->Add(new TBackup(apl));
	apl->Add(DLL_NewObjCronometro(apl));
	apl->Add(DLL_NewObjArvore(apl));
	// cria o gerenciador de fatores
	fatores = NewObjAjusteFatores(apl);
	// inicia patamares habilitados (default=todos)
	IniciaPat();

	gerGrupos = NULL;
}

// ---------------------------------------------------------------------------
__fastcall TAjusta::~TAjusta(void)
{
	// vari�veis locais
	VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));
	VTBackup *backup = (VTBackup*)apl->GetObject(__classid(TBackup));

	// se tiver barra de progresso, restaura obra original do edita
	if (PD.prog_enabled)
	{
		edita->UndoCompleto();
		edita->Obra = obra_original;
	}
	// destroi obj
	if (obra)
	{
		delete obra;
		obra = NULL;
	}
	// destr�i lista sem destruir seus objetos
	if (lisEQP)
	{
		delete lisEQP;
		lisEQP = NULL;
	}
	if (lisCARGA_ESP)
	{
		delete lisCARGA_ESP;
		lisCARGA_ESP = NULL;
	}
	if (lisREDE_VAL)
	{
		delete lisREDE_VAL;
		lisREDE_VAL = NULL;
	}
	if (lisFATORES)
	{
		delete lisFATORES;
		lisFATORES = NULL;
	}
	if (lisMEDICAO_FIC)
	{
		delete lisMEDICAO_FIC;
		lisMEDICAO_FIC = NULL;
	}
//	if (apl)
//	{
//		delete apl;
//		apl = NULL;
//	}
	if (gerGrupos)
	{
		delete gerGrupos;
		gerGrupos = NULL;
}
}

// ---------------------------------------------------------------------------
void __fastcall TAjusta::AjustaMedicaoFatores(VTAjusteFator *fator)
{
	// variaveis locais
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
	VTPatamar *patamar;
	VTCurva *curva;
	const int numValores = 6;
	double valoresPQ[numValores]; // teste: alterar pra enum
	// int numPatmares;
	strHM hm;
	VTMedicao *medicao;

	// protecao
	if (fator == NULL)
	{
		return;
	}
	medicao = fator->Medicao;
	if (fator == NULL)
	{
		return;
	}
	if ((curva = medicao->curva_pq) == NULL)
	{
		return;
	}
	if (patamares == NULL)
	{
		return;
	}
	// guarda o numero de patamares
	// numPatmares = patamares->NumPatamar();
	// verifica se a escala da curva � a mesma
	if (patamares->Escala != curva->Escala)
		curva->Reconfigura(patamares->Escala);
	// percorre todos itens da curva
	for (int np = 0; np < patamares->LisPatamar()->Count; np++)
	{
		patamar = (VTPatamar*) patamares->LisPatamar()->Items[np];
		// zera valores
		valoresPQ[indP] = 0.;
		valoresPQ[indQ] = 0.;
		hm.hora = patamar->HoraIni;
		hm.minuto = patamar->MinutoIni;
		curva->GetValor(hm, valoresPQ, numValores);
		MultiplicaValoresPQ(valoresPQ, fator);
		curva->SetPonto(hm, valoresPQ, numValores);
	}
	// atualiza o erro
	medicao->erro = medicao->MaiorErro();
}

// ---------------------------------------------------------------------------
// void __fastcall TAjusta::AjustaMedicaoFatores(VTMedicao *medicao)
// {
// // variaveis locais
// VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
// VTPatamar 	*patamar;
// VTCurva		*curva;
// const 		numValores = 6;
// double		valoresPQ[numValores]; //teste: alterar pra enum
// int numPatmares;
// strHM hm;
//
// //protecao
// if (medicao == NULL)
// {return;}
// if ( (curva = medicao->curva_pq) == NULL)
// {return;}
// if (patamares == NULL)
// {return;}
// //guarda o numero de patamares
// numPatmares = patamares->NumPatamar();
// // verifica se a escala da curva � a mesma
// if (patamares->Escala != curva->Escala)
// curva->Reconfigura(patamares->Escala);
// //percorre todos itens da curva
// for (int np = 0; np < patamares->LisPatamar()->Count; np++)
// {
// patamar = (VTPatamar*) patamares->LisPatamar()->Items[np];
// //zera valores
// valoresPQ[indP]= 0.;
// valoresPQ[indQ]= 0.;
// hm.hora = patamar->HoraIni;
// hm.minuto = patamar->MinutoIni;
// //teste
// curva->GetValor(hm,valoresPQ,numValores);
// MultiplicaValoresPQ(valoresPQ, fator);
// curva->SetPonto(hm,valoresPQ,numValores);
// }
// //atualiza o erro
// medicao->erro = medicao->MaiorErro();
// }
////---------------------------------------------------------------------------
bool __fastcall TAjusta::AjustePosFluxo(void)
{
	// vari�veis locais
	bool pos_fluxo = true;
	TList *lisNode, *lisACalcular, *lisCalculado;
	VTArvore *arvore = (VTArvore*)apl->GetObject(__classid(VTArvore));
	VTBatchAjusteTask *batch = (VTBatchAjusteTask*)apl->GetObject(__classid(VTBatchAjusteTask));

	// prote��o
	if (!arvore)
		return (false);
	// cria listas
	lisNode = new TList();
	lisACalcular = new TList();
	lisCalculado = new TList();
	// lista blocos ordenados
	arvore->LisAllNode(lisNode);
	// retira da lista os nodes raiz
	RetiraNodeRaiz(lisNode);
	// loop at� analisar todos os Nodes
	while (lisCalculado->Count < lisNode->Count)
	{ // preenche lista com nodes a serem calculados
		ListaNodesACalcular(lisACalcular, lisCalculado, lisNode);
		// verifica se h� Medicao habilitada para ajuste
		if (ExisteMedicaoHabilitada(lisACalcular))
		{ // executa ajuste para os nodes da lista a calcular
			ExecutaAjuste(lisACalcular, pos_fluxo);
		}
		else
		{
			if(batch != NULL)
			{
				batch->LogAdd("N�o existem medi��es habilitadas.");
			}
		}
		// copia nodes calculados para a lista de calculados
		CopiaTList(lisACalcular, lisCalculado);
		// limpa lista a calcular para a proxima itera��o
		lisACalcular->Clear();
	}
	// destr�i listas
	delete lisNode;
	delete lisACalcular;
	delete lisCalculado;
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TAjusta::AjustePreFluxo(void)
{
	// vari�veis locais
	// bool         ultima_iteracao = false;
	bool pos_fluxo = false;
	TList *lisNode, *lisACalcular, *lisCalculado;
	VTArvore *arvore = (VTArvore*)apl->GetObject(__classid(VTArvore));
	VTBatchAjusteTask *batch = (VTBatchAjusteTask*)apl->GetObject(__classid(VTBatchAjusteTask));

	// prote��o
	if (!arvore)
		return (false);
	// cria listas
	lisNode = new TList();
	lisACalcular = new TList();
	lisCalculado = new TList();
	// lista blocos ordenados
	arvore->LisAllNode(lisNode);
	// retira da lista os nodes raiz
	RetiraNodeRaiz(lisNode);
	// loop at� analisar todos os Nodes
	while (lisCalculado->Count < lisNode->Count)
	{ // preenche lista com nodes a serem calculados
		ListaNodesACalcular(lisACalcular, lisCalculado, lisNode);
		// verifica se h� Medicao habilitada para ajuste
		if (ExisteMedicaoHabilitada(lisACalcular))
		{ // executa ajuste para os nodes da lista a calcular
			ExecutaAjuste(lisACalcular, pos_fluxo);
		}
		else
		{
			if(batch != NULL)
			{
				batch->LogAdd("N�o existem medi��es habilitadas.");
			}
		}
		// copia nodes calculados para a lista de calculados
		CopiaTList(lisACalcular, lisCalculado);
		// limpa lista a calcular para a proxima itera��o
		lisACalcular->Clear();
	}
	// destr�i listas
	delete lisNode;
	delete lisACalcular;
	delete lisCalculado;
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TAjusta::AssociaBlocoMed(VTBloco *bloco, VTMedicao *medicao)
{
	// var locais
	VTProgresso *progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));

	// prote��o
	if ((!bloco) || (!medicao))
		return (false);
	// mostra/esconde progresso
	if (!PD.prog_enabled)
		progresso = NULL;
	// inicia lista de cargas do bloco
	if (progresso)
		progresso->Add(medicao->chave_med->Codigo, true);
	if (progresso)
		progresso->Add("Localizando cargas ajust�veis");
	IniciaLisEqbar(bloco, eqptoCARGA, medicao->lisCAR_AJUS);
	// remove cargas IP da lista de ajustavel e insere na lista de cargas fixas
	TrataIP(medicao->lisCAR_AJUS, medicao->lisCAR_FIXA);
	// verifica se n�o deve incluir consumidores A4
	if (progresso)
		progresso->Add("Localizando cargas n�o ajust�veis");
//	if (!IncluirConsA4)
//		SeparaCargaNaoAjustavel(medicao);
	//alterado - o condicional fica dentro da funcao
		SeparaCargaNaoAjustavel(medicao);
	// inicia lista de Capacitores
	if (progresso)
		progresso->Add("Localizando capacitores");
	IniciaLisEqbar(bloco, eqptoCAPACITOR, medicao->lisCAPACITOR);
	// inicia lista de Geradores PV
	if (progresso)
		progresso->Add("Localizando geradores");
	IniciaLisEqbar(bloco, eqptoGERADOR, medicao->lisGER_SUP);
	// determina patamar de m�ximo carregamento
	if (progresso)
		progresso->Add("Determinando patamar de demanda m�xima");
	medicao->ind_pat_dem_max = CalculaPatamarDemandaMax(medicao->lisCAR_AJUS);
	// define todas Cargas com Curva pr�pria
	if (progresso)
		progresso->Add("Alterando cargas para curva pr�pria");
	RedefineCargaComCurvaPropria(medicao->lisCAR_AJUS);
	// converte medi��o em curva PQ
	// if(progresso) progresso->Add("Convertendo curvas de medi��o");
	// ConverteCurva(medicao);
	return (true);
}

// ---------------------------------------------------------------------------
// void __fastcall TAjusta::AtualizaValorErroFicticio(VTMedicao *medicaoFic)
// {
//
// }

// ---------------------------------------------------------------------------
double __fastcall TAjusta::CalculaDemandaTotal(TList *lisCAR, VTPatamar *patamar)
{
	// vari�veis locais
	complex<double>s_tot_mva, s_car_mva;
	VTCarga *carga;
	VTDemanda *demanda = (VTDemanda*) apl->GetObject(__classid(VTDemanda));

	try
	{ // zera valor da somat�ria das demandas
		// s_tot_mva = CZero();
		s_tot_mva = complex<double>(0., 0.);
		// calcula somat�ria das demandas das cargas prim�rias e equivalentes
		for (int nc = 0; nc < lisCAR->Count; nc++)
		{
			carga = (VTCarga*)lisCAR->Items[nc];
			// obt�m demanda total da carga
			s_car_mva = demanda->S_mva[carga][patamar->Index];
			// totaliza demanda da Carga
			s_tot_mva += s_car_mva;
		}
		if (IsComplexZero(s_tot_mva))
			return (0.);
		return (Abs(s_tot_mva));
	}
	catch (Exception &e)
	{
		return (0.);
	}
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18416854
bool __fastcall TAjusta::CalcularFluxo(AnsiString msg_usuario)
{
	// vari�veis locais
	VTFlow *flow;
	bool precisa_executar = false;
    int count_redes = 0;
    VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

    //em modo batch, o flow est� no apl pai
	if (!BatchMode)
	{
		flow = (VTFlow*)apl->Owner->GetObject(__classid(VTFlow));
	}
	else
	{
		flow = (VTFlow*)apl->GetObject(__classid(VTFlow));
	}
    //verifica se o fluxo ja foi executado
	if (!flow->UpToDate)
	{
		precisa_executar = true;
	}
	if (!precisa_executar)
		return (true);
	// pede confirma��o do usu�rio somente se houver mais de 100 redes e houver solicita��o para isso
    count_redes = redes->LisRede()->Count;
	if ((!msg_usuario.IsEmpty()) && (count_redes > 100))
	{
		switch (Confirma("O m�dulo precisa executar o fluxo de pot�ncia.", "Deseja continuar ?",
			MB_YESNOCANCEL))
		{
		case IDYES:
			// executa c�lculo de fluxo de pot�ncia
			return (CalculaFluxo(true));
		case IDNO:
		case IDCANCEL:
		default:
			return (false);
		}
	}
	else
	{ // executa c�lculo de fluxo de pot�ncia
		return (CalculaFluxo(true));
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TAjusta::CalculaFluxo(bool prog_enabled)
{
	// vari�veis locais
	VTFlow *flow = (VTFlow*)apl->GetObject(__classid(VTFlow));
	// VTProgresso *progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// executa c�lculo de fluxo de pot�ncia
	flow->AvisoEnabled = false;
	flow->TramoEnabled = true;
	flow->TipoRelatorio = relNENHUM;
	flow->TipoFluxo = PD.tipo_fluxo;
	flow->UpToDate = false;
	flow->PerdaFerroEnabled = PD.incluir_perda;
	// exibe progresso do Flow se n�o estiver exibindo progresso pr�prio
	if (prog_enabled)
		flow->ProgressoEnabled = PD.prog_enabled;
	else
		flow->ProgressoEnabled = false;
	// calcula fluxo
	return (flow->CalculaPorRede(redes));
}
/*
 //---------------------------------------------------------------------------
 bool __fastcall TAjusta::CargaAlimentadaPorTrafoDY(VTCarga *carga)
 {
 //vari�veis locais
 VTZona *zona;

 //verifica se existe Zona da Carga
 if ((zona = carga->pbarra->zona) == NULL) return(false);
 //verifica se zona trif�sica
 if (! zona->Trifasica) return(false);
 //verifica �ngulo da Zona
 //if (IsDoubleZero(fabs(carga->pbarra->zona->T_grau[indFASE_A]))) return(false);
 if (IsDoubleZero(fabs(zona->T_grau[indFASE_A]))) return(false);
 return(true);
 }
 */

// ---------------------------------------------------------------------------
int __fastcall TAjusta::CalculaPatamarDemandaMax(TList *lisCAR)
{
	// var local
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
	int num_pat = patamares->NumPatamar();
	int ind_pat_max = 0;
	double dem_pat, dem_max = 0.;

	// loop p/ todos Patamares
	for (int ind_pat = 0; ind_pat < num_pat; ind_pat++)
	{
		// verifica se patamar est� habilitado
		if (!pat[ind_pat])
			continue;
		// calcula soma das demanda das Cargas
		dem_pat = CalculaDemandaTotal(lisCAR, patamares->Patamar[ind_pat]);
		if (dem_pat > dem_max)
		{
			ind_pat_max = ind_pat;
			dem_max = dem_pat;
		}
	}
	return (ind_pat_max);
}

// ---------------------------------------------------------------------------
bool __fastcall TAjusta::ContinuaMesmoComPrimariosNaoAjustaveis(TList *lisREDE_VAL)
{ // DVK 2016.12.15 reescrito
	// vari�veis locais
	VTMedicoes *medicoes = (VTMedicoes*)apl->GetObject(__classid(VTMedicoes));
	VTRede *rede;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	TList *lisREDE = redes->LisRede();
	AnsiString txt, aviso;
	int count = 1;
	VTBatchAjusteTask *batch = (VTBatchAjusteTask*)apl->GetObject(__classid(VTBatchAjusteTask));

	// reinicia lista
	lisREDE_VAL->Clear();
	medicoes->PreencheLisRedeVal(lisREDE_VAL);
	// verifica se todas as redes s�o inv�lidas
	if (lisREDE_VAL->Count == 0)
	{
		if (PD.prog_enabled)
			Aviso("N�o h� redes v�lidas para o ajuste");
		if(batch != NULL)
			{batch->LogDetalheAdd("ERRO: N�o h� redes v�lidas para o ajuste.");}
		return (false);
	}
	for (int nr = 0; nr < lisREDE->Count; nr++)
	{
		rede = (VTRede*)lisREDE->Items[nr];
        //verifica se h� cluster de rede (reticulado)
        if(rede->Cluster != NULL)
        {
            if (PD.prog_enabled)
                Aviso("Redes inv�lidas para o ajuste");
            if(batch != NULL)
                {batch->LogDetalheAdd("ERRO: Redes inv�lidas para o ajuste.");}
            return (false);
        }

		if (lisREDE_VAL->IndexOf(rede) >= 0)
			continue;
		if (rede->TipoRede->Segmento != redePRI)
			continue;
		// n�o insere redes equivalentadas
		if (!rede->Carregada)
		{
			aviso += txt.sprintf("%02d. %s: rede n�o carregada\n", count++, rede->Codigo.c_str());
		}
		else if (medicoes->FluxoZerado(rede))
		{ // n�o insere rede com fluxo zerado
			aviso += txt.sprintf("%02d. %s: rede com fluxo zerado\n", count++,
			rede->Codigo.c_str());
		}
		else
		{ // n�o insere rede com redes supridas equivalentadas
			aviso += txt.sprintf("%02d. %s: alimenta redes n�o carregadas\n", count++,
				rede->Codigo.c_str());
		}
	}
	if (aviso.IsEmpty())
		return (true);
	// exibe informa��o ao usu�rio e pede para ele confirmar a continua��o
	txt = "H� redes que n�o podem ser ajustadas:\n" + aviso;
	if(batch != NULL)
		{batch->LogDetalheAdd("ERRO: "+ txt);}
	if (PD.prog_enabled)
		return (Confirma(txt, "Deseja continuar ?") == IDYES);

	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TAjusta::CriaCurvaPropria(VTCarga* carga)
{
	// vari�veis locais
	double valor[nvCURVA_PQ];
	// double       p,q;
	strHM patHM;
	VTCurva *curva;
	VTPatamar *patamar;
	VTDemanda *demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

	try
	{ // prote��o
		if (!patamares)
			return (false);
		if (!demanda)
			return (false);
		// verifica se � um consumidor A4
		if (carga->classe->Tag == consA4)
		{ // verifica se a corre��o de consumidor prim�rio est� habilitada
			if (!IncluirConsA4)
				return (true);
		}
		// cria curva pr�pria, com demanda por fase
		curva = DLL_NewObjCurva();
		curva->Tipo = curvaPQ;
		curva->Unidade = unidMVA;
		curva->NumeroValores = nvCURVA_PQ;
		curva->Tipica = false;
		curva->Eqpto = carga;
		curva->Classe = NULL;
		// cria pontos da Curva, c/ valores nulos
		curva->Reconfigura(patamares->Escala);
		// define valores dos pontos da Curva pr�pria
		for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
		{
			patamar = patamares->Patamar[ind_pat];
			patHM.hora = patamar->HoraIni;
			patHM.minuto = patamar->MinutoIni;
			// obt�m demanda da Craga no patamar
			valor[0] = demanda->P_mw[carga][ind_pat];
			valor[1] = demanda->Q_mvar[carga][ind_pat];
			curva->SetPonto(patHM, valor, nvCURVA_PQ);
		}
		// redefine Curva da Carga
		carga->DefineCurva(curva);
	}
	catch (Exception &e)
	{
		return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
VTMedidor* __fastcall TAjusta::CriaMedidorAPartirFluxo(VTChave *chave)
{
	// variaveis locais
	VTMedicoes *medicoes = (VTMedicoes*)apl->GetObject(__classid(VTMedicoes));
	VTMedidor *medidor;
	VTCanal *canalAtiva[MAX_FASE];
	VTCanal *canalReativa[MAX_FASE];
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
	VTPatamar *patamar;
	VTChave *chaveAUX;
	int tipo_canal;
	int tipoFase;
	// VTFases *fases = (VTFases*)apl_owner->GetObject(__classid(VTFases));

	// verifica se foi calculado o fluxo para a rede da chave
	if (chave->rede->FluxoOK != true)
	{
		return NULL;
	}
	// verifica se existe Medidor p/ chave
	if ((medidor = medicoes->ExisteMedidor(chave)) == NULL)
	{ // cria um novo objeto VTMedidor
		medidor = medicoes->InsereMedidor(chave);
		if (medidor == NULL)
			return medidor;
		medicoes->LisMedidor()->Add(medidor);
	}
	// verifica se existe canal da grandeza trif�sica P
	tipo_canal = canalP;
	for (int fase = indFASE_A; fase <= indFASE_C; fase++)
	{
		switch (fase)
		{
		case indFASE_A:
			tipoFase = faseA;
			break;
		case indFASE_B:
			tipoFase = faseB;
			break;
		case indFASE_C:
			tipoFase = faseC;
			break;
		default: ;
		}
		if ((canalAtiva[fase] = medidor->ExisteCanal(chave, tipo_canal, tipoFase)) == NULL)
		{ // cria canal abc
			canalAtiva[fase] = medidor->InsereCanal(chave, tipo_canal, tipoFase);
			// insere demais dados do canal novo
			canalAtiva[fase]->Codigo = canalAtiva[fase]->TipoCanalAsString;
			canalAtiva[fase]->Enabled = true;
			canalAtiva[fase]->Curva = DLL_NewObjCurva();
			canalAtiva[fase]->Curva->Tipica = false;
			canalAtiva[fase]->Curva->Escala = pat01HORA;
			canalAtiva[fase]->Curva->NumeroValores = nvCURVA_CANAL;
			canalAtiva[fase]->Curva->Reconfigura(pat01HORA);
			// verifica se Canal est� associada a uma Chave
			if (canalAtiva[fase]->Eqpto->Tipo() == eqptoCHAVE)
			{ // verifica se a Chave � um Disjuntor
				chaveAUX = (VTChave*)canalAtiva[fase]->Eqpto;
				if (chaveAUX->TipoChave->Tipo == chaveDISJUNTOR)
					canalAtiva[fase]->Tolerancia_perc = TOLERANCIA_MEDICAO_DJ;
				else
					canalAtiva[fase]->Tolerancia_perc = TOLERANCIA_MEDICAO_CHV;
			}
			switch (canalAtiva[fase]->TipoCanal)
			{
			case canalP:
				canalAtiva[fase]->Curva->Unidade = unidKW;
				break;
			default:
				break;
			}
		}
		if (!canalAtiva[fase]->Enabled)
		{
			canalAtiva[fase]->Enabled = true;
		}
		// verifica se a escala da curva � a mesma
		if (patamares->Escala != canalAtiva[fase]->Curva->Escala)
		{
			canalAtiva[fase]->Curva->Reconfigura(patamares->Escala);
		}
	}

	// verifica se existe canal da grandeza trif�sica Q
	tipo_canal = canalQ;
	for (int fase = indFASE_A; fase <= indFASE_C; fase++)
	{
		switch (fase)
		{
		case indFASE_A:
			tipoFase = faseA;
			break;
		case indFASE_B:
			tipoFase = faseB;
			break;
		case indFASE_C:
			tipoFase = faseC;
			break;
		default: ;
		}
		if ((canalReativa[fase] = medidor->ExisteCanal(chave, tipo_canal, tipoFase)) == NULL)
		{ // cria canal abc
			canalReativa[fase] = medidor->InsereCanal(chave, tipo_canal, tipoFase);
			// insere demais dados do canal novo
			canalReativa[fase]->Codigo = canalReativa[fase]->TipoCanalAsString;
			canalReativa[fase]->Enabled = true;
			canalReativa[fase]->Curva = DLL_NewObjCurva();
			canalReativa[fase]->Curva->Tipica = false;
			canalReativa[fase]->Curva->Escala = pat01HORA;
			canalReativa[fase]->Curva->NumeroValores = nvCURVA_CANAL;
			canalReativa[fase]->Curva->Reconfigura(pat01HORA);
			// verifica se Canal est� associada a uma Chave
			if (canalReativa[fase]->Eqpto->Tipo() == eqptoCHAVE)
			{ // verifica se a Chave � um Disjuntor
				chaveAUX = (VTChave*)canalReativa[fase]->Eqpto;
				if (chaveAUX->TipoChave->Tipo == chaveDISJUNTOR)
					canalReativa[fase]->Tolerancia_perc = TOLERANCIA_MEDICAO_DJ;
				else
					canalReativa[fase]->Tolerancia_perc = TOLERANCIA_MEDICAO_CHV;
			}
			switch (canalReativa[fase]->TipoCanal)
			{
			case canalQ:
				canalReativa[fase]->Curva->Unidade = unidKVAR;
				break;
			default:
				break;
			}
		}
		if (!canalReativa[fase]->Enabled)
		{
			canalReativa[fase]->Enabled = true;
		}
		// verifica se a escala da curva � a mesma
		if (patamares->Escala != canalReativa[fase]->Curva->Escala)
		{
			canalReativa[fase]->Curva->Reconfigura(patamares->Escala);
		}
	}
	// transfere os valores do fluxo para o medidor
	for (int fase = indFASE_A; fase <= indFASE_C; fase++)
	{
		for (int np = 0; np < patamares->NumPatamar(); np++)
		{
			patamar = (VTPatamar*) patamares->LisPatamar()->Items[np];
			if (chave->resflow->Pf_mw[0][np][fase] > 0)
			{
				canalAtiva[fase]->ValorMedido[patamar] = chave->resflow->Pf_mw[0][np][fase];
				canalReativa[fase]->ValorMedido[patamar] = chave->resflow->Qf_mvar[0][np][fase];
			}
			else
			{
				canalAtiva[fase]->ValorMedido[patamar] = chave->resflow->Pf_mw[1][np][fase];
				canalReativa[fase]->ValorMedido[patamar] = chave->resflow->Qf_mvar[1][np][fase];
			}

		}
	}
	return medidor;
}

// ---------------------------------------------------------------------------
void __fastcall TAjusta::CriaMedidoresAPartirFluxo(TList *lisMedidores)
{
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTRede *redeMT;
	TList *lisLIGACOES;
	TList *lisRedesCarregadas;
	VTLigacao *ligacao;
	VTMedidor *medidor;

	// protecao
	if (lisMedidores == NULL)
	{
		return;
	}
	lisLIGACOES = new TList;
	lisRedesCarregadas = new TList;
	redes->LisRedeCarregada(lisRedesCarregadas);
	// percorre a lsita de carregadas
	for (int nc = 0; nc < lisRedesCarregadas->Count; nc++)
	{
		redeMT = (VTRede*) lisRedesCarregadas->Items[nc];
		// se for nao for mt pula
		if (redeMT->TipoRede->Segmento != redePRI)
		{
			continue;
		}
		// se for mt pega verifica a 1a chave
		// (como no TMedicoes::InsereDisjuntoresFicticios)
		// caso a rede n�o possua barra inicial...
		if (redeMT->BarraInicial() == NULL)
		{
			continue;
		}
		// cria uma lista de ligacoes
		lisLIGACOES->Clear();
		// obtem a lista de ligacoes na barra inicial dessa rede
		redes->LisLigacao(lisLIGACOES, redeMT->BarraInicial());
		// varre a lista procurando chaves dessa rede
		for (int nl = 0; nl < lisLIGACOES->Count; nl++)
		{
			ligacao = (VTLigacao*)lisLIGACOES->Items[nl];
			// verifica se a liga��o � uma chave FECHADA e se pertence a mesma rede
			if ((ligacao->rede == redeMT) && (ligacao->Tipo() == eqptoCHAVE))
			{
				if (((VTChave*)ligacao)->Fechada)
				{
					medidor = CriaMedidorAPartirFluxo((VTChave*)ligacao);
					if (medidor != NULL)
					{
						lisMedidores->Add(medidor);
					}
				}
			}
		}
	}
	DestroiTObject(lisRedesCarregadas);
	DestroiTObject(lisLIGACOES);
}

// ---------------------------------------------------------------------------
void __fastcall TAjusta::CriaMedicoesFicticias(void)
{
	VTMedicoes *medicoes = (VTMedicoes*)apl->GetObject(__classid(VTMedicoes));
	TList *lisMedidoresFicticios = new TList;
	VTMedicao *medicao;
	VTMedidor *medidor;
	VTRede *redeMedidor;
	VTAjusteFator* fator;
	/* TODO : somente cria medidores necessarios */
	// primeiramente cria todos medidores com "medicoes" igual ao fluxo
	CriaMedidoresAPartirFluxo(lisMedidoresFicticios);
	/* TODO : if calculo uptodate, nao calcular novamente */
	// percorre a lista de medidores para criar as medicoes
	for (int nm = 0; nm < lisMedidoresFicticios->Count; nm++)
	{
		medidor = (VTMedidor*)lisMedidoresFicticios->Items[nm];
		if (medidor != NULL)
		{
			redeMedidor = IdentificaRedeMedidor(medidor);
			if (redeMedidor != NULL)
			{ // verifica o fator que possui essa rede
				fator = fatores->ExisteFator(redeMedidor);
				// caso exista
				if ((fator != NULL) && (fator->Ativo))
				{
					medicao = medicoes->CriaMedicaoMedidor(medidor);
					if (medicao != NULL)
					{ // guarda a medicao no fator
						fator->Medicao = medicao;
						// adiciona
						medicoes->LisMedicao()->Add(medicao);
						// adiciona � lista de medicoes ficticias
						lisFATORES->Add(fator);
						lisMEDICAO_FIC->Add(medicao);
					}
				}
				else
				{ // nao existe fator ativo para aquele medidor
					medidor->Clear();
				}
			}
			fator = NULL;
			redeMedidor = NULL;
		}
	}
	// percorre a lista de medicoes e as ajusta com os fatores
	for (int nf = 0; nf < lisFATORES->Count; nf++)
	{
		fator = (VTAjusteFator*)lisFATORES->Items[nf];
		if (fator != NULL)
		{
			AjustaMedicaoFatores(fator);
		}
	}
	// destroi as listas
	DestroiTObject(lisMedidoresFicticios);
}

// ---------------------------------------------------------------------------
void __fastcall TAjusta::DefineDiagnosticoTempo(TList *lisNODE)
{
	// variaveis locais
	VTMedicao *medicao;
	VTNode *node;

	// prote��o
	if (!lisNODE)
		return;
	for (int n = 0; n < lisNODE->Count; n++)
	{ // determina medi��o associada ao node
		node = (VTNode*)lisNODE->Items[n];
		medicao = (VTMedicao*)node->LisObjectNode()->First();
		medicao->diagnostico = diagTEMPO;
	}
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TAjusta::DiagMedicao(TList *lisMEDICAO)
{
	// variaveis locais
	VTMedicao *medicao;
	AnsiString diag = "";

	for (int n = 0; n < lisMEDICAO->Count; n++)
	{ // insere linhas com cada medi��o e seu diagnostico (diferente de ok)
		medicao = (VTMedicao*)lisMEDICAO->Items[n];
		if (!medicao->enabled)
			continue;
		if (medicao->diagnostico == diagMED_OK)
			continue;
		diag += medicao->chave_med->Codigo + ": " + medicao->DiagnosticoAsStr() + " ";
	}
	return diag;
}

// ---------------------------------------------------------------------------
bool __fastcall TAjusta::EqptoJusante(VTNode *node, TList *lisExt, int tipo)
{
	// variaveis locais
	TList *lisNodeFilho;
	VTBloco *bloco;
	VTNode *node_jus;

	// prote��o
	if ((!node) || (!lisExt))
		return (false);
	lisNodeFilho = node->LisNode();
	for (int n = 0; n < lisNodeFilho->Count; n++)
	{ // insere eqptos dos blocos a jusante do node
		node_jus = (VTNode*)lisNodeFilho->Items[n];
		bloco = (VTBloco*)node_jus->Object;
		if (!bloco)
			continue;
		bloco->LisEqbar(lisExt, tipo);
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TAjusta::Executa(bool med_neg_enabled)
{
	// vari�veis locais
	bool convergiu = false;
	TErroAjuste erro_ajuste;
	// VTFlow       *flow_apl   = (VTFlow*)apl->GetObject(__classid(VTFlow));
	VTZonas *zonas = (VTZonas*)apl->GetObject(__classid(VTZonas));
	VTProgresso *progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
	VTMedicoes *medicoes = (VTMedicoes*)apl->GetObject(__classid(VTMedicoes));
	VTCronometro *cronometro = (VTCronometro*)apl->GetObject(__classid(VTCronometro));
	VTBatchAjusteTask *batch = (VTBatchAjusteTask*)apl->GetObject(__classid(VTBatchAjusteTask));
	bool sucesso = true;

	// mostra/esconde progresso
	if (!PD.prog_enabled)
		progresso = NULL;
	try
	{ // limpa diagn�stico
		Diagnostico = "";
		// limpa lisCARGA_ESP
		lisCARGA_ESP->Clear();
		// prote��o
		if (!medicoes)
		{
			if(batch != NULL)
				{batch->LogDetalheAdd("FALHA: Objeto Medicoes inv�lido.");}
			return (false);
		}
		// inicia Zonas
		if(batch != NULL)
				{batch->LogDetalheAdd("Iniciando TZonas");}
		if (!zonas->Executa())
		{
			if(batch != NULL)
				{batch->LogDetalheAdd("FALHA: Executa do TZonas falhou.");}
			return (false);
		}
		// cria obj cronometro e guarda no apl
		if (cronometro)
		{
			cronometro->Reset("Ajuste de demanda");
			cronometro->Start("Ajuste de demanda");
		}
		// cria medicoes falsas caso necessario
		if (PD.usa_medicao == false)
		{
			CriaMedicoesFicticias();
			medicoes->PreencheLisMedidor(true);
		}
		// guarda somente medi��es v�lidas
		lisMEDICAO = medicoes->LisMedicaoValida(med_neg_enabled);
		// verifica se foi definido o uso de limite de erro inicial e final
		if (!LimitaAjuste)
		{ // assume um valor de 2% para o erro final
			ErroMaximo = 0.02;
			AjusteMaximo = 0.10;
		}
		// exibe progresso
		if (progresso)
		{
			progresso->Start(progTEXTO);
		}
#ifdef _DEBUG
		// if (progresso) progresso->Stop();
#endif
		if (progresso)
		{
			progresso->Add("Ajuste de Demanda");
		}
		if (progresso)
		{
			progresso->Add("Reconfigurando curvas das cargas");
		}
		if(batch != NULL)
				{batch->LogDetalheAdd("Reconfigurando curvas das cargas");}
        //cria objeto gerenciador dos grupos de ajuste
		if (gerGrupos != NULL)
			{delete gerGrupos;}
		gerGrupos = NewObjGerenciaGrupos();
		// TESTE - provisorio, retirar
		VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
		TList *lisCARGA = new TList;
		redes->LisEqbar(lisCARGA, eqptoCARGA);
		//gerGrupos->DEBUG_CategoriaAleatoria(lisCARGA);
        //marca todas cargas da categoria NADA APARENTE como n�o ajustavel
		gerGrupos->SetCargasAjustavelCategoria(false, lisCARGA, catAD_NADAAPARENTE);
		delete lisCARGA;
		// fim codigo provisorio
		
		// prepara dados p/ ajuste de demanda
		if (!PreparaDados(medicoes, med_neg_enabled))
		{ // DVK 2015.09.29 complementa mensagem do diagn�stico
			Diagnostico += "\nErro na prepara��o inicial dos dados para ajuste";
			// esconde progresso
			if (progresso)
				progresso->Stop();
			cronometro->Stop("Ajuste de demanda");
//			if(batch != NULL)
//			{
//				batch->LogAdd("FALHA: Erro na prepara��o inicial dos dados para ajuste.");
//			}
			return (false);
		}
		//classifica as cargas das medicoes v�lidas entre as categorias de ajuste
		gerGrupos->CriaGrupos(lisMEDICAO);
		//seta os pesos teste
		gerGrupos->SetPesoGrupo(2.0, catAD_FRAUDULENTO);
		gerGrupos->SetPesoGrupo(2.0, catAD_IRREGULAR);
		gerGrupos->SetPesoGrupo(1.0, catAD_NAO_INSPEC);
        gerGrupos->Inicia();
        //loga os medidores
		if(batch != NULL)
		{
			batch->LogDetalheAdd("Logando erros iniciais.");
			LogaMedidores();
		}
		// executa ajuste preliminar, sem executar o fluxo
		if (progresso)
		{
			progresso->Add("Executando ajuste preliminar", true);
		}
		if(batch != NULL)
				{batch->LogDetalheAdd("Executando ajuste preliminar");}
		convergiu = AjustePreFluxo();
        if (!convergiu)
		{
			if(batch != NULL)
			{
				batch->LogAdd("FALHA: Ajuste pr� fluxo n�o convergiu.");
			}
		}
		// executa ajuste com fluxo de pot�ncia
		if (progresso)
		{
			progresso->Add("Executando ajuste final");
		}
		if(batch != NULL)
				{batch->LogDetalheAdd("Executando ajuste final");}
		if (convergiu)
		{
			convergiu = AjustePosFluxo();
			if (!convergiu)
			{
				if(batch != NULL)
					{batch->LogAdd("FALHA: Ajuste p�s fluxo n�o convergiu.");}
			}
		}
		// executa �ltimo c�lculo do fluxo de pot�ncia
		if(batch != NULL)
			{batch->LogDetalheAdd("Calculando fluxo de potencia");}
		sucesso = CalculaFluxo(false);
		// restaura cargas de redes BT que n�o convergiram o �ltimo fluxo
		if(batch != NULL)
			{batch->LogDetalheAdd("Restaurando redes BT que n�o convergiram");}
		RestauraCargasRedesSemFluxo();
		// recalcula energia das Cargas
		if(batch != NULL)
			{batch->LogDetalheAdd("Atualizando so valores de energia das cargas");}
		RecalculaEnergiaCargas();
		// atualiza diagnostico, baseado em cada diagnostico das medi��es
		Diagnostico += DiagMedicao(medicoes->LisMedicao());
		if(batch != NULL)
			{batch->LogDetalheAdd("Diagn�stico: " + Diagnostico);}
		// atualiza indica��o de que o fluxo foi calculado ou n�o
		// if (flow_apl) flow_apl->UpToDate = flow->UpToDate;
		// esconde progresso
		if (progresso)
		{
			progresso->Stop();
		}
		// para cronometro e n�o destroi (apl vai destruir)
		if(cronometro)
			cronometro->Stop("Ajuste de demanda");
        if(batch != NULL)
			{batch->LogDetalheAdd("Fim do ajuste");}
		//loga diagnostico
		LogaDiagnostico();
	}
	catch (Exception &e)
	{
		convergiu = false;
		// esconde progresso
		if (progresso)
		{
			progresso->Stop();
		}
		if(batch != NULL)
		{
			batch->LogDetalheAdd("FALHA: Exception em TAjusta::Executa "+ e.Message);
		}
	}
	catch (...)
	{
        if(batch != NULL)
		{
			batch->LogDetalheAdd("FALHA: Exception em TAjusta::Executa ");
		}
    }
	//destroi objeto de gerenciamento de grupos
	if (gerGrupos)
	{
		delete gerGrupos;
		gerGrupos = NULL;
	}


	// retorna indica��o de sucesso ou erro
	return (convergiu);
}

// ---------------------------------------------------------------------------
VTMedicao* __fastcall TAjusta::ExisteMedicao(VTElo *elo)
{
	// variaveis locais
	VTMedicao *medicao;

	for (int nm = 0; nm < lisMEDICAO->Count; nm++)
	{ // localiza medi��o associada � chave do elo
		medicao = (VTMedicao*)lisMEDICAO->Items[nm];
		if (medicao->chave_med == elo->Chave)
			return (medicao);
	}
	return (NULL);
}

// ---------------------------------------------------------------------------
bool __fastcall TAjusta::ExisteMedicaoHabilitada(TList *lisNODE)
{
	// vari�veis locais
	VTMedicao *medicao;
	VTNode *node;

	// loop para todos Nodes
	for (int n = 0; n < lisNODE->Count; n++)
	{ // determina medi��o associada ao node
		node = (VTNode*)lisNODE->Items[n];
		medicao = (VTMedicao*)node->LisObjectNode()->First();
		if (medicao == NULL)
			continue;
		if (medicao->enabled)
			return (true);
	}
	return (false);
}

// ---------------------------------------------------------------------------
bool __fastcall TAjusta::Finaliza(void)
{
	// vari�veis locais
	TBackup *backup = (TBackup*)apl->GetObject(__classid(TBackup));
	VTMedicoes *medicoes = (VTMedicoes*)apl->GetObject(__classid(VTMedicoes));
	bool sucesso = false;

	try
	{
		// retira chaves ficticias
		medicoes->RemoveChavesFic();
		// restaura cargas de redes que n�o foram ajustadas
		RestauraCargasRedesNaoAjustadas();
		// limpa backup
		backup->Clear();
		sucesso = true;
	}
	catch (...)
	{
		sucesso = false;
	}

	return sucesso;
}

// ---------------------------------------------------------------------------
bool __fastcall TAjusta::FundeBlocosSemMedicao(void)
{
	// vari�veis locais
	bool sucesso = true;
	VTArvore *arvore = (VTArvore*)apl->GetObject(__classid(VTArvore));
	VTBlocos *blocos = (VTBlocos*)apl->GetObject(__classid(VTBlocos));
	VTBloco *bloco_jus, *bloco_mon;
	VTElo *elo;
	VTMedicao *medicao;
	VTNode *node;
	TList *lisNODE, *lisBLOCO_FUND;

	// prote��o
	if ((!arvore) || (!blocos))
		return (false);
	// cria listas
	lisNODE = new TList();
	lisBLOCO_FUND = new TList();
	// inicia lisNODE
	arvore->LisAllNode(lisNODE);
	for (int n = lisNODE->Count - 1; n >= 0; n--)
	{ // come�a das folhas pra raiz
		node = (VTNode*)lisNODE->Items[n];
		bloco_jus = (VTBloco*)node->Object;
		if (!node->Pai)
			continue;
		bloco_mon = (VTBloco*)node->Pai->Object;
		elo = blocos->ExisteElo(bloco_jus, bloco_mon);
		if (!elo)
		{
			sucesso = false;
			break;
		}
		medicao = ExisteMedicao(elo);
		// verifica se ha medi��o
		if (medicao != NULL)
			continue;
		// inicia lisBLOCO_FUND com o bloco do node
		lisBLOCO_FUND->Clear();
		lisBLOCO_FUND->Add(bloco_jus);
		// funde blocos sem medi��o no bloco pai do node
		blocos->FundeBlocos(bloco_mon, lisBLOCO_FUND);
	}
	// destroi listas
	delete lisNODE;
	delete lisBLOCO_FUND;
	return (sucesso);
}

// ---------------------------------------------------------------------------
int __fastcall TAjusta::IndBarraMontante(VTBloco *bloco_jus, VTBloco *bloco_mon)
{
	// vari�veis locais
	VTElo *elo;
	VTChave *chave;
	VTBarra *barra;
	VTBlocos *blocos = (VTBlocos*)apl->GetObject(__classid(VTBlocos));

	// localiza elo
	elo = blocos->ExisteElo(bloco_jus, bloco_mon);
	chave = elo->Chave;
	// testa barra1
	barra = chave->pbarra1;
	if (!bloco_mon->ExisteBarra(barra) && (bloco_jus->ExisteBarra(barra)))
	{ // a barra montante � a barra2
		barra = chave->pbarra2;
	}
	return (chave->IndiceBarra(barra));
}

// ---------------------------------------------------------------------------
VTRede* __fastcall TAjusta::IdentificaRedeMedidor(VTMedidor *medidor)
{
	VTEqpto *eqpto;
	TList *lisEQPTOSmed;
	VTLigacao *ligacao;
	lisEQPTOSmed = new TList;
	VTRede *rede = NULL;

	medidor->LisEqptoDosCanais(lisEQPTOSmed);
	// percorre a lista procurando alguma ligacao
	for (int nl = 0; nl < lisEQPTOSmed->Count; nl++)
	{
		eqpto = (VTEqpto*) lisEQPTOSmed->Items[nl];
		if (eqpto->TipoLigacao())
		{
			ligacao = (VTLigacao*) eqpto;
			if (ligacao->rede != NULL)
			{
				rede = ligacao->rede;
				break;
			}
		}
		else
		{
			continue;
		}
	}
	DestroiTObject(lisEQPTOSmed);

	return rede;
}

// ---------------------------------------------------------------------------
bool __fastcall TAjusta::IniciaBlocos(void)
{
	// vari�veis locais
	bool sucesso = true;
	VTArvore *arvore = (VTArvore*)apl->GetObject(__classid(VTArvore));
	VTBlocos *blocos = (VTBlocos*)apl->GetObject(__classid(VTBlocos));
	VTBloco *bloco_jus, *bloco_mon;
	VTElo *elo; // ,       *elo_pai;
	VTMedicao *medicao; // ,   *medicao_pai;
	VTNode *node;
	TList *lisNODE;

	// prote��o
	if ((!arvore) || (!blocos))
		return (false);
	lisNODE = new TList();
	arvore->LisAllNode(lisNODE);
	for (int n = lisNODE->Count - 1; n >= 0; n--)
	{ // come�a das folhas pra raiz
		node = (VTNode*)lisNODE->Items[n];
		bloco_jus = (VTBloco*)node->Object;
		if (!node->Pai)
			continue;
		bloco_mon = (VTBloco*)node->Pai->Object;
		elo = blocos->ExisteElo(bloco_jus, bloco_mon);
		if (!elo)
		{
			sucesso = false;
			break;
		}
		medicao = ExisteMedicao(elo);
		// insere medi��o na lista de objetos do node
		if (!medicao)
		{
			sucesso = false;
			break;
		}
		node->InsereObject(medicao);
		// associa medi��o ao bloco a jusante
		if (!AssociaBlocoMed(bloco_jus, medicao))
		{
			sucesso = false;
			break;
		}
		// determina barra de refer�ncia da medi��o (montante)
		medicao->ind_bar_ref = IndBarraMontante(bloco_jus, bloco_mon);
		sucesso = true;
	}
	// destroi lista
	delete lisNODE;
	return (sucesso);
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/15314553
// https://sinapsisenergia.teamwork.com/#/tasks/17882048
//https://sinapsisenergia.teamwork.com/#/tasks/18381986
bool __fastcall TAjusta::IniciaDados(TComponent *Owner)
{
	VTEdita *edita; // = (VTEdita*)apl->Owner->GetObject(__classid(VTEdita));
	VTRedes *redes; // = (VTRedes*)apl->Owner->GetObject(__classid(VTRedes));
	VTSinapChild *sinap_child; // = (VTSinapChild*)apl->Owner->GetObject(__classid(VTSinapChild));
	// VTImportaMedicao *importa_medicao;
	AnsiString txt;
	VTMedicoes *medicoes = (VTMedicoes*)apl->GetObject(__classid(VTMedicoes));
	VTProgresso *prog = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
	VTRadial *radial = (VTRadial*)apl->GetObject(__classid(VTRadial));
	TBackup *backup = (TBackup*)apl->GetObject(__classid(TBackup));
	VTBatchAjusteTask *batch = (VTBatchAjusteTask*)apl->GetObject(__classid(VTBatchAjusteTask));

	try
	{ // inicia vari�vel de controle de ajuste executado
		PD.ajuste_ja_executado = false;
		if (!BatchMode)
		{
			edita = (VTEdita*)apl->Owner->GetObject(__classid(VTEdita));
			redes = (VTRedes*)apl->Owner->GetObject(__classid(VTRedes));
			sinap_child = (VTSinapChild*)apl->Owner->GetObject(__classid(VTSinapChild));
			// cria progresso
			if (PD.prog_enabled && (prog == NULL))
			{
				apl->Add(prog = DLL_NewObjProgresso(Owner, NULL));
			}
			// mostra progresso
			if (prog != NULL)
				prog->Start(progDEFAULT);
			// exibe gr�fico de topologia

			sinap_child->ActionTopo->Execute();
			// limpa backup
			backup->Clear();
		}
		else
		{
			PD.prog_enabled = false;
			edita = (VTEdita*)apl->GetObject(__classid(VTEdita));
			redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
		}
		// inicia objeto Radial
		radial->Inicia(redes);
		// salva Obra originalmente associado a Edita
		obra_original = edita->Obra;
		// se tiver barra de progresso, cria objeto Obra e associa com objeto Edita
		if (PD.prog_enabled && (obra == NULL))
		{
			obra = DLL_NewObjObra(apl);
			edita->Obra = obra;
		}
		if(batch != NULL)
		{
			batch->LogDetalheAdd("Limpa medi��es anteriores");
		}
		// limpa medi��es existentes
		medicoes->Clear();
		if(batch != NULL)
		{
			batch->LogDetalheAdd("Insere disjuntores fict�cios (se necess�rio)");
		}
		// insere disjuntores ficticios (se necess�rio)
		medicoes->InsereDisjuntoresFicticios(true);
		// verifica se h� Prim�rios que n�o podem ser ajustados e pede p/ usu�rio
        if(batch != NULL)
		{
			batch->LogDetalheAdd("Descarta alimentadores n�o carregados (equivalentados)");
		}
		// confirmar a execu��o
		if (!ContinuaMesmoComPrimariosNaoAjustaveis(lisREDE_VAL))
		{ // para progresso
			if (prog != NULL)
				prog->Stop();
			return false;
		}
        if(batch != NULL)
		{
			batch->LogDetalheAdd("Guarda c�pia das cargas originais");
		}
		// cria c�pia das Cargas
		if(! SalvaCopiaDasCargasOriginais())
		{// para progresso
			if (prog != NULL)
				prog->Stop();
			return false;
        }
		// para progresso
		if (prog != NULL)
			prog->Stop();
		// se tiver barra de progresso, mostra confirma��o ao executar fluxo
		txt = (PD.prog_enabled) ? "Executar fluxo?" : "";
        if(batch != NULL)
		{
			batch->LogDetalheAdd("Roda fluxo");
		}
		// se necess�rio, calcula o fluxo de pot�ncia
		if (!CalcularFluxo(txt))
		{
			if (!BatchMode)
			{// cancela tudo
				if (PD.prog_enabled)
					Aviso("Erro no c�lculo do fluxo. O ajuste n�o pode ser executado.");
				else
					Alerta("Erro no c�lculo do fluxo.");
			}
			return false;
		}
		// verifica novamente se h� Prim�rios que n�o podem ser ajustados e pede p/ usu�rio
        if(batch != NULL)
		{
			batch->LogDetalheAdd("Descarta alimentadores que n�o rodaram fluxo");
		}
		// confirmar a execu��o
		if (!ContinuaMesmoComPrimariosNaoAjustaveis(lisREDE_VAL))
		{// desconsidera o retorno falso quando o n�mero de redes validas for zero
            if(lisREDE_VAL->Count == 0)
		{
                if(batch != NULL)
                    batch->LogDetalheAdd("O n�mero de medi��es para fazer o ajuste � 0");
            }
            else
            {// para progresso
                if (prog != NULL)
                    prog->Stop();
                return false;
            }
		}
		if(batch != NULL)
		{
			batch->LogDetalheAdd("Atualiza medidores com resultado do fluxo");
		}
		// inicia medidores (curva estimada com valores do fluxo)
		medicoes->AtualizaMedidores();
		
	}
	catch (Exception &e)
	{ // para progresso
		if (prog != NULL)
			prog->Stop();
		return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
void __fastcall TAjusta::IniciaLisEqbar(VTBloco *bloco, int tipo_eqbar, TList *lisEQB)
{
	// monta lisEQB c/ os eqbar do bloco
	lisEQB->Clear();
	bloco->LisEqbar(lisEQB, tipo_eqbar);
}

// ---------------------------------------------------------------------------
void __fastcall TAjusta::IniciaPat(void)
{
	// habilita todos os patamares
	for (int n = 0; n < pat01HORA; n++)
	{
		pat[n] = true;
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TAjusta::LicencaCopel(void)
{
	// variaveis locais
	VTLicencas *licencas = (VTLicencas*)apl->GetObject(__classid(VTLicencas));
	VTLicenca *licenca;

	// prote��o
	if (licencas == NULL)
		return false;
	if ((licenca = licencas->ExisteLicenca()) == NULL)
		return false;
	// verifica se � da Copel
	return (licenca->Empresa == COPEL);
}

// ---------------------------------------------------------------------------
TList* __fastcall TAjusta::LisRedeVal(void)
{
	return (lisREDE_VAL);
}

// ---------------------------------------------------------------------------
void __fastcall TAjusta::ListaNodesACalcular(TList *lisACalcular, TList *lisCalculado,
	TList *lisNode)
{
	// variaveis locais
	VTNode *node;
	TList *lisNodeFilho;

	// prote��o
	if ((!lisACalcular) || (!lisNode))
		return;
	for (int n = lisNode->Count - 1; n >= 0; n--)
	{ // loop em todos os nodes ao contrario
		node = (VTNode*)lisNode->Items[n];
		if (lisCalculado->IndexOf(node) >= 0)
			continue;
		lisNodeFilho = node->LisNode();
		// verifica se � node folha ou pai calculado
		if ((lisNodeFilho->Count <= 0) || (NodePaiComFilhosCalculados(node, lisCalculado)))
		{ // insere na lista de nodes a serem calculados
			lisACalcular->Add(node);
		}
	}
}
// ---------------------------------------------------------------------------
void       __fastcall  TAjusta::LogaDiagnostico(void)
{   //adaptado do TFormLogAjustaDemanda
	//var locais
	VTBatchAjusteTask *batch = (VTBatchAjusteTask*)apl->GetObject(__classid(VTBatchAjusteTask));
	VTMedicoes *medicoes = (VTMedicoes*)apl->GetObject(__classid(VTMedicoes));
	AnsiString diagnostico;
	AnsiString erro_perc;
	VTMedicao  *medicao;
	AnsiString msgLog;
	TList *lisMedicao;

	if (batch == NULL)
	{
		//faz nada
        return;
	}
	if (medicoes == NULL)
	{
		batch->LogDetalheAdd("Objeto 'Medicoes' ausente.");
		return;
	}
	//
	lisMedicao = medicoes->LisMedicao();
	batch->LogDetalheAdd("Log do diagn�stico das medi��es:");
	batch->LogDetalheAdd("   N. medi��es: " + IntToStr(lisMedicao->Count));
	for(int nm = 0; nm < lisMedicao->Count; nm++)
	{//loop em todas medicao
		medicao = (VTMedicao*)lisMedicao->Items[nm];
		//rede
		msgLog = "   "+ medicao->chave_med->rede->Codigo +";";
		//chave
		msgLog += medicao->chave_med->Codigo +";";
		//diagn�stico
		switch(medicao->diagnostico)
		{
			case diagMED_OK:
				erro_perc   = DoubleToStr("%2.1f%%", medicao->erro * 100., ',');
				diagnostico = "OK";
				break;
			case diagMED_ERRO_INICIAL:
				//				erro_perc   = ""; //DVK 2015.09.24
				erro_perc   = DoubleToStr("%2.1f%%", medicao->erro * 100., ',');
				//				diagnostico = "Diferen�a inicial maior do que o limite definido";
				diagnostico.sprintf("Diferen�a inicial maior do que o limite definido (%2.1f%%)",
					  this->AjusteMaximo * 100.);
				break;
			case diagMED_ERRO_FINAL:
				erro_perc   = DoubleToStr("%2.1f%%", medicao->erro * 100., ',');
				//verifica se o limite do ajuste foi definido
				if(! this->LimitaAjuste)
				{//altera mensagem de diagn�stico
				//					diagnostico.sprintf("A precis�o m�xima obtida com o ajuste foi de (%2.1f%%)",
				//										  ajusta->ErroMaximo);
					diagnostico = "OK";
					break;
				}
				diagnostico.sprintf("O erro final � maior que o erro m�ximo definido (%2.1f%%)",this->ErroMaximo * 100.);
				break;
			case diagMED_ERRO_FLUXO:
				erro_perc   = "";
				diagnostico = "Fluxo n�o calculado para a rede";
				break;
			case diagTEMPO:
				erro_perc   = DoubleToStr("%2.1f%%", medicao->erro * 100., ',');
				diagnostico = "Tempo excedido para c�lculo";
				break;
			default:
				erro_perc   = "";
				diagnostico = "";
				break;
		}
		//erro percentual
		//		item->SubItems->Add(erro_perc);
		//diagn�stico
		msgLog += diagnostico +";";

		batch->LogDetalheAdd(msgLog);
	}
}
// ---------------------------------------------------------------------------
void       __fastcall TAjusta::LogaMedidores(void)
{   //adaptado do form Demanda
	// variaveis locais
	VTBatchAjusteTask *batch = (VTBatchAjusteTask*)apl->GetObject(__classid(VTBatchAjusteTask));
	VTMedidor *medidor;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	TList *lisMEDIDOR;

	if (batch == NULL)
	{
		//faz nada
		return;
	}
	// cria lista
	lisMEDIDOR = new TList();
	redes->LisEqbar(lisMEDIDOR, eqptoMEDIDOR);
	for (int nm = 0; nm < lisMEDIDOR->Count; nm++)
	{
		medidor = (VTMedidor*)lisMEDIDOR->Items[nm];
		// reinicia tipo de grandeza
        LogaMedidor(medidor);
	}
	// destroi lista
	delete lisMEDIDOR;
}
// ---------------------------------------------------------------------------
void __fastcall TAjusta::LogaMedidor(VTMedidor *medidor)
{	//adaptado do form Demanda
	VTBatchAjusteTask *batch = (VTBatchAjusteTask*)apl->GetObject(__classid(VTBatchAjusteTask));
	VTCanal *canal, *canalAux;
	VTLigacao *ligacao = NULL;
	TList  *lisCANAL;
	bool grandezaI, grandezaP, grandezaQ;
	AnsiString msgLog = "";

	//faz nada se nao for pra lgoar
	if ((batch == NULL) ||(medidor == NULL))
		{return;}
	try
	{	//pega a lista de canais
		lisCANAL = medidor->LisCanal();
		//verifica se existe ligacao associada ao medidor
		for (int nc = 0; nc < lisCANAL->Count; nc++)
		{ // verifica se ha algum canal associado a ligacao
			canal = (VTCanal*)lisCANAL->Items[nc];
			if (canal->Eqpto->TipoLigacao())
			{
				ligacao = (VTLigacao*)canal->Eqpto;
				break;
			}
		}
		//faz nada se nao tiver ligacao
		if (ligacao == NULL)
			{return;}
		// reinicia tipo de grandeza
		grandezaI = grandezaP = grandezaQ = false;
		for (int nc = 0; nc < lisCANAL->Count; nc++)
		{ // insere medidores
			canal = (VTCanal*)lisCANAL->Items[nc];
			// insere dados de canais com grandezas n�o repetidas por medidor
			if ((grandezaI) && (canal->TipoCanal == canalI))
				continue;
			if ((grandezaP) && (canal->TipoCanal == canalP))
				continue;
			if ((grandezaQ) && (canal->TipoCanal == canalQ))
				continue;
			if (canal->TipoCanal == canalI)
				grandezaI = true;
			if (canal->TipoCanal == canalP)
				grandezaP = true;
			if (canal->TipoCanal == canalQ)
				grandezaQ = true;

			//insere uma linha
			// medidor
			msgLog  = "codigo: " + medidor->Codigo + ";";
			// grandeza
			msgLog += "tipo canal: " + canal->TipoCanalAsString + ";";
			// fase A
			canalAux = medidor->ExisteCanal(ligacao, canal->TipoCanal, faseA);
			msgLog += "Erro fase A: "+ MaiorErro(canalAux) +";";
			// fase B
			canalAux = medidor->ExisteCanal(ligacao, canal->TipoCanal, faseB);
			msgLog += "Erro fase B: "+ MaiorErro(canalAux) +";";
			// fase C
			canalAux = medidor->ExisteCanal(ligacao, canal->TipoCanal, faseC);
			msgLog += "Erro fase C: "+ MaiorErro(canalAux) +";";
			// fase ABC
			canalAux = medidor->ExisteCanal(ligacao, canal->TipoCanal, faseABC);
			msgLog += "Erro fase ABC: "+ MaiorErro(canalAux) +";";

			//loga
			batch->LogDetalheAdd(msgLog);
		}
	}
	catch (Exception &e)
	{
		batch->LogDetalheAdd("ERRO em LogaMedidor : " +e.Message);
	}
	catch (...)
	{
		batch->LogDetalheAdd("ERRO inesperado em LogaMedidor.");
	}
}
// ---------------------------------------------------------------------------
AnsiString __fastcall TAjusta::MaiorErro(VTCanal *canal)
{
	// variaveis locais
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
	VTPatamar *patamar;
	TList *lisPAT = patamares->LisPatamar();
	double med, est;
	double erro = 0.;
	double maior = 0.;
	AnsiString str_valor;

	// prote��o
	if (!canal)
		return ("0.0%");
	for (int np = 0; np < lisPAT->Count; np++)
	{ // verifica a maior diferen�a entre todos os patamares
		patamar = (VTPatamar*)lisPAT->Items[np];
		//verifica se patamar est� habilitado
		if(! this->Patamares[np])  continue;
		med = canal->ValorMedido[patamar];
		est = canal->ValorEstimado[patamar];
		if (IsDoubleZero(med))
			continue;
		if (IsDoubleZero(est))
			continue;
		erro = (fabs(med - est) / med);
		// considera modulo do erro, pois ele pode ser negativo
		erro = fabs(erro);
		maior = Max(maior, erro);
	}
	// retorna valor
	maior = fabs(maior) * 100.;
    str_valor = DoubleToStr("%2.1f", fabs(maior), ',') + "%";
	return (str_valor);
}
// ---------------------------------------------------------------------------
void __fastcall TAjusta::MultiplicaValoresPQ(double valores[], VTAjusteFator *fator)
{
	double kfp;
	if (fator == NULL)
	{
		return;
	}
	// age de acordo com os fatores
	switch (fator->TipoFatorCorrec)
	{
	case adtfFATOR_UNICO:
		// fase a
		valores[indP] = valores[indP] * fator->FatorCorrecP;
		valores[indQ] = valores[indQ] * fator->FatorCorrecQ;
		// fase b
		valores[indP + 2] = valores[indP + 2] * fator->FatorCorrecP;
		valores[indQ + 2] = valores[indQ + 2] * fator->FatorCorrecQ;
		// fase c
		valores[indP + 4] = valores[indP + 4] * fator->FatorCorrecP;
		valores[indQ + 4] = valores[indQ + 4] * fator->FatorCorrecQ;
		break;

	case adtfFATOR_PQ:
		// fase a
		valores[indP] = valores[indP] * fator->FatorCorrecP;
		valores[indQ] = valores[indQ] * fator->FatorCorrecQ;
		// fase b
		valores[indP + 2] = valores[indP + 2] * fator->FatorCorrecP;
		valores[indQ + 2] = valores[indQ + 2] * fator->FatorCorrecQ;
		// fase c
		valores[indP + 4] = valores[indP + 4] * fator->FatorCorrecP;
		valores[indQ + 4] = valores[indQ + 4] * fator->FatorCorrecQ;
		break;

	case adtfFATOR_PFP:
		kfp = tan(acos(fator->FatorCorrecFP));
		// fase a
		valores[indP] = valores[indP] * fator->FatorCorrecP;
		valores[indQ] = valores[indP] * kfp;
		// fase b
		valores[indP + 2] = valores[indP + 2] * fator->FatorCorrecP;
		valores[indQ + 2] = valores[indP + 2] * kfp;
		// fase c
		valores[indP + 4] = valores[indP + 4] * fator->FatorCorrecP;
		valores[indQ + 4] = valores[indP + 4] * kfp;
		break;
	default: ;
	}

}
////---------------------------------------------------------------------------
// void __fastcall TAjusta::MultiplicaValoresPQ(double valores[])
// {
// double kfp;
//
// //age de acordo com os fatores
// switch (PD.fatores.tipo)
// {
// case adtfFATOR_UNICO:
// //fase a
// valores[indP] = valores[indP]*PD.fatores.multP;
// valores[indQ] = valores[indQ]*PD.fatores.multQ;
// //fase b
// valores[indP+2] = valores[indP+2]*PD.fatores.multP;
// valores[indQ+2] = valores[indQ+2]*PD.fatores.multQ;
// //fase c
// valores[indP+4] = valores[indP+4]*PD.fatores.multP;
// valores[indQ+4] = valores[indQ+4]*PD.fatores.multQ;
// break;
//
// case adtfFATOR_PQ:
// //fase a
// valores[indP] = valores[indP]*PD.fatores.multP;
// valores[indQ] = valores[indQ]*PD.fatores.multQ;
// //fase b
// valores[indP+2] = valores[indP+2]*PD.fatores.multP;
// valores[indQ+2] = valores[indQ+2]*PD.fatores.multQ;
// //fase c
// valores[indP+4] = valores[indP+4]*PD.fatores.multP;
// valores[indQ+4] = valores[indQ+4]*PD.fatores.multQ;
// break;
//
// case adtfFATOR_PFP:
// kfp = tan(acos(PD.fatores.fp));
// //fase a
// valores[indP] = valores[indP]*PD.fatores.multP;
// valores[indQ] = valores[indP]*kfp;
// //fase b
// valores[indP+2] = valores[indP+2]*PD.fatores.multP;
// valores[indQ+2] = valores[indP+2]*kfp;
// //fase c
// valores[indP+4] = valores[indP+4]*PD.fatores.multP;
// valores[indQ+4] = valores[indP+4]*kfp;
// break;
// default:
// ;
// }
// }

// ---------------------------------------------------------------------------
bool __fastcall TAjusta::NodePaiComFilhosCalculados(VTNode *node, TList *lisCalculado)
{
	// variaveis locais
	bool sucesso = true;
	TList *lisNodeFilho;
	VTNode *node_filho;

	// prote��o
	if ((!node) || (!lisCalculado))
		return (false);
	lisNodeFilho = node->LisNode();
	for (int n = 0; n < lisNodeFilho->Count; n++)
	{ // verifica se ha algum node filho n�o calculado
		node_filho = (VTNode*)lisNodeFilho->Items[n];
		if (lisCalculado->IndexOf(node_filho) < 0)
			sucesso = false;
	}
	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TAjusta::PM_GetPat(int ind_pat)
{
	// retorna se o patamar est� habilitado
	return pat[ind_pat];
}

// ---------------------------------------------------------------------------
void __fastcall TAjusta::PM_SetPat(int ind_pat, bool enabled)
{
	// determina se o patamar est� habilitado
	pat[ind_pat] = enabled;
}

// ---------------------------------------------------------------------------
bool __fastcall TAjusta::PreparaDados(VTMedicoes *medicoes, bool med_neg_enabled)
{
	// vari�veis locais
	VTArvore *arvore = (VTArvore*)apl->GetObject(__classid(VTArvore));
	VTBlocos *blocos = (VTBlocos*)apl->GetObject(__classid(VTBlocos));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTBatchAjusteTask *batch = (VTBatchAjusteTask*)apl->GetObject(__classid(VTBatchAjusteTask));

	// inicia blocos
	if (!blocos->Executa(redes))
	{
		if(batch != NULL)
			{batch->LogDetalheAdd("FALHA: Falha no TBlocos::Executa.");}
		return (false);
	}
	// elimina Elos cujas Chaves est�o abertas
	if (!blocos->EliminaElosComChaveAberta())
	{
		if(batch != NULL)
			{batch->LogDetalheAdd("FALHA: Falha no TBlocos::EliminaElosComChaveAberta.");}
		return (false);
	}
	// elimina Blocos isolados ou conectados apenas por Chaves abertas
	if (!blocos->EliminaBlocosNaoEnergizados())
	{
		if(batch != NULL)
			{batch->LogDetalheAdd("FALHA: Falha no TBlocos::EliminaBlocosNaoEnergizados.");}
		return (false);
	}
	// elimina Elos que conectam um mesmo Bloco
	if (!blocos->EliminaElosComBlocosIguais())
	{
		if(batch != NULL)
			{batch->LogDetalheAdd("FALHA: Falha no TBlocos::EliminaElosComBlocosIguais.");}
		return (false);
	}
	// int n = blocos->LisBloco()->Count;
	// inicia arvore
	arvore->Clear();
	if (!arvore->Inicia(blocos))
	{
		if(batch != NULL)
			{batch->LogDetalheAdd("FALHA: Falha no TArvore::Inicia(blocos).");}
		return (false);
	}
	// funde os blocos que n�o tiverem medi��o associada
	if (!FundeBlocosSemMedicao())
	{
		if(batch != NULL)
			{batch->LogDetalheAdd("FALHA: Falha no TAjusta::FundeBlocosSemMedicao");}
		return (false);
	}
	// reinicia arvore com blocos fundidos
	arvore->Clear();
	if (!arvore->Inicia(blocos))
	{
		if(batch != NULL)
			{batch->LogDetalheAdd("FALHA: Falha no TArvore::Inicia(blocos fundidos)");}
		return (false);
	}
	// prepara os dados de cada bloco
	if (!ValidaBlocos())
	{
		if(batch != NULL)
			{batch->LogDetalheAdd("FALHA: Falha no TAjusta::ValidaBlocos");}
		return (false);
	}
	if (!IniciaBlocos())
	{
		if(batch != NULL)
			{batch->LogDetalheAdd("FALHA: Falha no TAjusta::IniciaBlocos");}
		return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
void __fastcall TAjusta::RecalculaEnergiaCargas(void)
{
	// vari�veis locais
	VTCarga *carga;
	VTMedicao *medicao;
	VTDemanda *demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
	VTMedicoes *medicoes = (VTMedicoes*)apl->GetObject(__classid(VTMedicoes));
	TList *lisMEDICAO = medicoes->LisMedicao();

	try
	{ // recalcula energia das Cargas que foram ajustadas
		for (int n = 0; n < lisMEDICAO->Count; n++)
		{ // verifica se a Medicao est� habilitada
			medicao = (VTMedicao*)lisMEDICAO->Items[n];
			if (!medicao->enabled)
				continue;
			for (int nc = 0; nc < medicao->lisCAR_AJUS->Count; nc++)
			{
				carga = (VTCarga*)medicao->lisCAR_AJUS->Items[nc];
				carga->Energia_mwhmes = demanda->Emes_mwh[carga];
			}
		}
	}
	catch (Exception &e)
	{ // nada
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TAjusta::RedefineCargaComCurvaPropria(TList *lisCAR)
{
	// vari�veis locais
	VTCarga *carga;
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

	// prote��o
	if (lisCAR->Count == 0)
		return (true);
	// loop p/ todas Cargas
	for (int nc = 0; nc < lisCAR->Count; nc++)
	{
		carga = (VTCarga*)lisCAR->Items[nc];
		if (carga->IP)
			continue;
		// seta indica��o de altera��o da Carga
		carga->Status[sttALTERADO] = true;
		// verifica se a Carga definida com Curva t�pica
		// if (carga->curva->Tipica)
		// { // cria Curva pr�pria p/ a Rede
		// CriaCurvaPropria(carga);
		// }
		if (carga->curva->Escala != patamares->Escala)
		{ // redefine n�mero de pontos da Curva
			carga->curva->Reconfigura(patamares->Escala);
		}
		// redefine curva c/ demandas por fase
		RedefineCargaComDemandaPorFase(carga);
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TAjusta::RedefineCargaComDemandaPorFase(VTCarga *carga)
{
	// vari�veis locais
	int num_fase;
	double valor[nvCURVA_RRCAR];
	double p_mw, q_mvar;
	strHM patHM;
	VTCurva *curva;
	VTPatamar *patamar;
	VTFases *fases = (VTFases*)apl->GetObject(__classid(VTFases));
	VTDemanda *demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

	// verifica se a Curva de demanda da Carga j� est� definida por fase
	if (carga->DefinidaPorFase)
	{
		return (true);
	}
	// verifica se � um consumidor A4
	if (carga->classe->Tag == consA4)
	{ // verifica se a corre��o de consumidor prim�rio est� habilitada
		if (!IncluirConsA4)
			return (true);
	}
	try
	{ // cria curva com demanda por fase
		curva = DLL_NewObjCurva();
		curva->Tipo = curvaPQ;
		curva->Unidade = unidMVA;
		curva->NumeroValores = nvCURVA_RRCAR;
		curva->Tipica = false;
		curva->Eqpto = carga;
		curva->Classe = NULL;
		// determina n�mero de fases da Carga
		if ((num_fase = fases->NumeroDeFases(carga->Fases)) == 0)
		{ // prote��o contra fases inv�lidas
			num_fase = 3;
			carga->Fases = faseABC;
		}
		// prote��o
		if ((num_fase < 0) || (num_fase > 3))
		{
			num_fase = 3;
			carga->Fases = faseABC;
		}
		// cria pontos da Curva, c/ valores nulos
		curva->Reconfigura(patamares->Escala);
		// define valores dos pontos da Curva pr�pria
		for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
		{
			patamar = patamares->Patamar[ind_pat];
			patHM.hora = patamar->HoraIni;
			patHM.minuto = patamar->MinutoIni;
			// obt�m demanda equilibrada da Carga no patamar
			p_mw = (demanda->P_mw[carga][ind_pat]) / num_fase;
			q_mvar = (demanda->Q_mvar[carga][ind_pat]) / num_fase;
			// reinicia valor
			for (int n = 0; n < nvCURVA_RRCAR; n++)
				valor[n] = 0.;
			// define valores da Curva no patamar
			if ((carga->Fases & faseA) == faseA)
			{
				valor[0] = p_mw;
				valor[1] = q_mvar;
			}
			if ((carga->Fases & faseB) == faseB)
			{
				valor[2] = p_mw;
				valor[3] = q_mvar;
			}
			if ((carga->Fases & faseC) == faseC)
			{
				valor[4] = p_mw;
				valor[5] = q_mvar;
			}
			curva->SetPonto(patHM, valor, nvCURVA_RRCAR);
		}
		// redefine Curva da Carga
		carga->DefineCurva(curva);
	}
	catch (Exception &e)
	{
		return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
void __fastcall TAjusta::RedefineLisCargaMed(VTMedicao *medicao)
{
	// vari�veis locais
	VTCarga *carga;

	for (int nc = 0; nc < lisCARGA_ESP->Count; nc++)
	{
		carga = (VTCarga*)lisCARGA_ESP->Items[nc];
		if (carga->IP)
			continue;
		if (medicao->lisCAR_AJUS->IndexOf(carga) < 0)
			continue;
		// retira carga da lista ajust�vel
		medicao->lisCAR_AJUS->Remove(carga);
		// insere carga (com curva original) na lista fixa
		if (medicao->lisCAR_FIXA->IndexOf(carga) < 0)
			medicao->lisCAR_FIXA->Add(carga);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TAjusta::RemoveMedicoesFicticias(void)
{
	VTMedicoes *medicoes = (VTMedicoes*)apl->GetObject(__classid(VTMedicoes));
	VTMedicao *medicao;

	// remove todas medicoes ficticias
	for (int nm = (lisMEDICAO_FIC->Count - 1); nm >= 0; nm--)
	{
		medicao = (VTMedicao*)lisMEDICAO_FIC->Items[nm];
		if (medicoes->LisMedicao()->IndexOf(medicao) >= 0)
		{
			medicoes->LisMedicao()->Remove(medicao);
		}
		// adiciona � lista de medicoes ficticias
		lisMEDICAO_FIC->Remove(medicao);
		// destroi medicao
		DestroiTObject(medicao);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TAjusta::RestauraCargasOriginais(void)
{
	// vari�veis locais
	TBackup *backup = (TBackup*)apl->GetObject(__classid(TBackup));
	VTFlow *flow = (VTFlow*)apl->GetObject(__classid(VTFlow));
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	if (backup)
		backup->RestauraCargaTodosPrimarios();
	// atualiza grafico e flow
	grafico->AreaRedeDefine();
	flow->UpToDate = false;
	// cria nova c�pia das Cargas
	SalvaCopiaDasCargasOriginais();
}

// ---------------------------------------------------------------------------
void __fastcall TAjusta::RestauraCargasRedesNaoAjustadas(void)
{
	// vari�veis locais
	TBackup *backup = (TBackup*)apl->GetObject(__classid(TBackup));
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	VTMedicoes *medicoes = (VTMedicoes*)apl->GetObject(__classid(VTMedicoes));
	TList *lisMED_ERRO = new TList();

	// verifica se alguma Medicao apresentou erro no ajuste
	medicoes->LisMedicaoComErroNoAjuste(lisMED_ERRO);
	// restaura todas Cargas de cada medi��o com erro
	if (backup)
		backup->RestauraCargaMedicao(lisMED_ERRO);
	// atualiza grafico
	grafico->AreaRedeDefine();
	// destroi lista
	delete lisMED_ERRO;
}

// ---------------------------------------------------------------------------
void __fastcall TAjusta::RestauraCargasRedesSemFluxo(void)
{
	// vari�veis locais
	TBackup *backup = (TBackup*)apl->GetObject(__classid(TBackup));
	// VTCarga     *carga;
	VTRede *rede;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	TList *lisREDE = redes->LisRede();
	TList *lisCARGA;

	// prote��o
	if (backup == NULL)
		return;
	lisCARGA = new TList();
	// limpa lista
	lisCARGA_ESP->Clear();
	for (int nr = 0; nr < lisREDE->Count; nr++)
	{ // lista todas as redes que n�o rodaram fluxo, devido ao ajuste
		rede = (VTRede*)lisREDE->Items[nr];
		if (rede->FluxoOK)
			continue;
		// descarta redes que n�o sejam BT
		if (rede->TipoRede->Segmento != redeSEC)
			continue;
		// lista cargas
		lisCARGA->Clear();
		rede->LisEqbar(lisCARGA, eqptoCARGA);
		// copia para lista de cargas especiais
		CopiaTList(lisCARGA, lisCARGA_ESP);
	}
	if (lisCARGA_ESP->Count > 0)
	{ // restaura lista de cargas (substituindo por curva original nessa lista)
		backup->RestauraCurvaCargaAtual(lisCARGA_ESP);
		// redefine cargas com curva pr�pria
		RedefineCargaComCurvaPropria(lisCARGA_ESP);
	}
	// destroi lista
	delete lisCARGA;
}

// ---------------------------------------------------------------------------
void __fastcall TAjusta::RetiraNodeRaiz(TList *lisNode)
{
	// vari�veis locais
	TList *lisNodeRaiz;
	VTArvore *arvore = (VTArvore*)apl->GetObject(__classid(VTArvore));
	VTNode *node; // , *raiz;

	// prote��o
	if (!lisNode)
		return;
	lisNodeRaiz = arvore->LisNodeRaiz();
	// raiz = (VTNode*)lisNodeRaiz->First();
	for (int n = lisNode->Count - 1; n >= 0; n--)
	{
		node = (VTNode*)lisNode->Items[n];
		if (lisNodeRaiz->IndexOf(node) >= 0)
			lisNode->Delete(n);
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TAjusta::SalvaCopiaDasCargasOriginais(void)
{
	// vari�veis locais
	TBackup *backup = (TBackup*)apl->GetObject(__classid(TBackup));
	VTPrimario *primario;
	VTRadial *radial = (VTRadial*)apl->GetObject(__classid(VTRadial));
	VTRede *rede;
	VTBatchAjusteTask *batch = (VTBatchAjusteTask*)apl->GetObject(__classid(VTBatchAjusteTask));
	bool sucessoTodas = true;

	try
	{ // loop para todos Prim�rios v�lidos
		for (int n = 0; n < lisREDE_VAL->Count; n++)
		{
			rede = (VTRede*)lisREDE_VAL->Items[n];
			primario = radial->ExistePrimario(rede);
			if (primario == NULL)
				continue;
			if(! backup->SalvaCargaUmPrimario(primario))
			{
				//return false;
				sucessoTodas = false;
				if(batch != NULL)
				{
					if (primario->Rede != NULL)
						{batch->LogDetalheAdd("FALHA: N�o foi poss�vel fazer backup das cargas do primario: " +primario->Rede->Codigo);}
					else
						{batch->LogDetalheAdd("FALHA: N�o foi poss�vel fazer backup das cargas do primario indefinido");}
				}
			}

		}
	}
	catch (Exception &e)
	{
		return false;
	}
	return sucessoTodas;
}

// ---------------------------------------------------------------------------
void __fastcall TAjusta::SeparaCargaNaoAjustavel(VTMedicao *medicao)
{
	// vari�veis locais
	VTCarga *carga;
	bool remover = false;

	for (int n = medicao->lisCAR_AJUS->Count - 1; n >= 0; n--)
	{
		carga = (VTCarga*)medicao->lisCAR_AJUS->Items[n];
		remover = false;
		// DVK 2016.10.05 verifica se � do grupo A
		//remove se a opcao  e N�O incluir A4 E a carga for A4
		remover = !IncluirConsA4 && ((carga->classe->Tag == consA4) || (carga->GrupoA));
		//marca a carga como n�o ajustavel
		if (remover)
			{carga->Ajustavel = false;}
		//verifica se a carga N�O � ajustavel
//		if ((carga->classe->Tag == consA4) || (carga->GrupoA) || (carga))
		if (!carga->Ajustavel)
		{ // remove Carga da lista Cargas ajust�veis
			medicao->lisCAR_AJUS->Delete(n);
			// insere Carga na lista de Cargas fixas
			if (medicao->lisCAR_FIXA->IndexOf(carga) < 0)
				medicao->lisCAR_FIXA->Add(carga);
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TAjusta::TrataIP(TList *lisCARGA_AJUS, TList *lisCARGA_FIXA)
{
	// vari�veis locais
	VTCarga *carga;

	for (int n = lisCARGA_AJUS->Count - 1; n >= 0; n--)
	{
		carga = (VTCarga*)lisCARGA_AJUS->Items[n];
		if (!carga->IP)
			continue;
		// remove cargas IP da lista ajustavel
		lisCARGA_AJUS->Remove(carga);
		// insere cargas IP da lista fixa
		if (lisCARGA_FIXA->IndexOf(carga) < 0)
			lisCARGA_FIXA->Add(carga);
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TAjusta::ValidaBlocos(void)
{
	// vari�veis locais
	bool sucesso = true;
	VTArvore *arvore = (VTArvore*)apl->GetObject(__classid(VTArvore));
	VTBlocos *blocos = (VTBlocos*)apl->GetObject(__classid(VTBlocos));
	VTBloco *bloco_jus, *bloco_mon, *bloco_avo;
	VTElo *elo, *elo_pai;
	VTNode *node;
	TList *lisNODE;
	VTBatchAjusteTask *batch = (VTBatchAjusteTask*)apl->GetObject(__classid(VTBatchAjusteTask));

	// prote��o
	if ((!arvore) || (!blocos))
	{
		if(batch != NULL)
		{
			batch->LogDetalheAdd("TBlocos ou TArvores inv�lido");
		}
		return (false);
	}
	// cria lista
	lisNODE = new TList();
	arvore->LisAllNode(lisNODE);
	for (int n = lisNODE->Count - 1; n >= 0; n--)
	{ // come�a das folhas pra raiz
		node = (VTNode*)lisNODE->Items[n];
		bloco_jus = (VTBloco*)node->Object;
		if (!node->Pai)
			continue;
		bloco_mon = (VTBloco*)node->Pai->Object;
		elo = blocos->ExisteElo(bloco_jus, bloco_mon);
		if (!elo)
		{
            if(batch != NULL)
			{
				batch->LogDetalheAdd("Elo inv�lido");
			}
			sucesso = false;
			break;
		}
		// valida medi��o, em rela��o ao bloco a montante com medi��o
		if (node->Pai->Pai)
		{
			bloco_avo = (VTBloco*)node->Pai->Pai->Object;
			elo_pai = blocos->ExisteElo(bloco_avo, bloco_mon);
			if (!ValidaMedicao(elo_pai, elo))
			{
                if(batch != NULL)
				{
					batch->LogDetalheAdd("Elo v�lido mas medi��o inv�lida");
				}
				sucesso = false;
				break;
			}
			sucesso = true;
		}
	}
	// destroi lista
	delete lisNODE;
	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TAjusta::ValidaDiagFluxo(VTMedicao *medicao)
{
	// verifica se o fluxo foi executado para o Primario
	if (medicao->chave_med->rede->FluxoOK)
	{ // atualiza diagn�stico, somente se o diagn�stico anterior for erro no fluxo
		if (medicao->diagnostico == diagMED_ERRO_FLUXO)
			medicao->diagnostico = diagMED_OK;
	}
	else
	{ // atualiza diagn�stico
		medicao->diagnostico = diagMED_ERRO_FLUXO;
		return (false);
	}
	// verifica se h� cargas ajust�veis
	if (medicao->lisCAR_AJUS->Count == 0)
	{
		medicao->diagnostico = diagMED_ERRO_FLUXO;
		return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TAjusta::ValidaMedicao(VTElo *elo_pai, VTElo *elo)
{
	// variaveis locais
	VTCurva *curva_pai, *curva;
	VTMedicao *medicao_pai, *medicao;
	VTPatamar *patamar;
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
	double val_med[6], val_med_pai[6];
	strHM patHM;
	VTBatchAjusteTask *batch = (VTBatchAjusteTask*)apl->GetObject(__classid(VTBatchAjusteTask));

	// prote��o
	if ((!elo_pai) || (!elo))
		return (false);
	medicao = ExisteMedicao(elo);
	medicao_pai = ExisteMedicao(elo_pai);
	if ((!medicao) || (!medicao_pai))
		return (false);
	curva = medicao->curva_pq;
	curva_pai = medicao_pai->curva_pq;
	// se nao tem curva pai, nao precisa validar
	if (!curva_pai)
		return (true);
	// valida ponto a ponto
	for (int np = 0; np < patamares->NumPatamar(); np++)
	{ // determina o Patamar
		patamar = patamares->Patamar[np];
		// verifica se patamar est� habilitado
		if (!pat[np])
			continue;
		// determina valor no patamar
		patHM.hora = patamar->HoraCal;
		patHM.minuto = patamar->MinutoCal;
		// medicao
		if (!curva->GetValor(patHM, val_med, curva->NumeroValores))
		{ // zera valores
			for (int n = 0; n < sizeof(val_med) / sizeof(double); n++)
				val_med[n] = 0;
		}
		// medicao pai
		if (!curva_pai->GetValor(patHM, val_med_pai, curva_pai->NumeroValores))
		{ // zera valores
			for (int n = 0; n < sizeof(val_med_pai) / sizeof(double); n++)
				val_med_pai[n] = 0;
		}
		// compara valores  {Pa,Qa, Pb,Qb, Pc,Qc}
		for (int ind = 0; ind < curva_pai->NumeroValores; ind += 2)
		{ // a medi��o de P nao pode ser maior ou igual � medi��o pai
			if (IsDoubleZero(val_med[ind]) && IsDoubleZero(val_med_pai[ind]))
				continue;
			if (val_med[ind] >= val_med_pai[ind])
			{
				Diagnostico = "A medi��o em " + medicao->chave_med->Codigo +
					" � maior ou igual a " + medicao_pai->chave_med->Codigo + " no per�odo " +
					patamar->Periodo;
				if (batch != NULL)
				{
                    batch->LogAdd("ERRO: Possui medi��o incoerente com a medi��o � montante.");
					batch->LogDetalheAdd(Diagnostico);
				}
				return (false);
			}
		}
	}
	return (true);
}

// ---------------------------------------------------------------------------
void __fastcall TAjusta::ZeraCarga(VTCarga *carga)
{
	// vari�veis locais
	int num_pat;
	double p_mw, q_mvar;
	complex<double>sf_mva[MAX_FASE];
	VTDemanda *demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));

	// prote��o
	if (carga->curva == NULL)
		return;
	// determina quantidade de patamares da curva da carga
	num_pat = carga->curva->NumeroPontos();
	// loop p/ todos patamares
	for (int ind_pat = 0; ind_pat < num_pat; ind_pat++)
	{ // corrige o valor de cada fase
		for (int nf = 0; nf < MAX_FASE; nf++)
		{
			p_mw = 0.;
			q_mvar = 0.;
			sf_mva[nf] = complex<double>(p_mw, q_mvar);
		}
		// Atualiza demanda por fase
		if (!demanda->Set_S3f_mva(carga, ind_pat, sf_mva))
			return;
	}
}

// ---------------------------------------------------------------------------
// eof
