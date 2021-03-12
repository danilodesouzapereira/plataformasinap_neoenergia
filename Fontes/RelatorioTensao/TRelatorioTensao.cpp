// ---------------------------------------------------------------------------
#pragma hdrstop

#include "TRelatorioTensao.h"
#include "TFaixaTensao.h"
#include "TNivelTensao.h"
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Consulta\VTConsulta.h>
#include <Fontes\Flow\VTFlow.h>
#include <Fontes\Geral\VTGeral.h>
#include <Fontes\Geral\VTInfoset.h>
#include <Fontes\Grafico\VTGrafico.h>
#include <Fontes\Patamar\VTPatamar.h>
#include <Fontes\Progresso\VTProgresso.h>
#include <Fontes\Rede\VTRedes.h>
#include <DLL_Inc\Consulta.h>
#include <DLL_Inc\Funcao.h>
#include <DLL_Inc\Progresso.h>

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// -----------------------------------------------------------------------------
__fastcall TRelatorioTensao::TRelatorioTensao(VTApl *apl)
{
	// salva ponteiro p/ objeto
	this->apl = apl;
	// cria objetos
	progresso = DLL_NewObjProgresso(NULL, NULL);
	nivel_at = new TNivelTensao(apl, nvAT);
	nivel_mt = new TNivelTensao(apl, nvMT);
	nivel_bt = new TNivelTensao(apl, nvBT);
	// valores default
	tensao_fase = true;
	tensao_minima = true;
	config_alterada = true;
	ind_pat = -1;
	// le dados do infoset
	LeDados();
}

// -----------------------------------------------------------------------------
__fastcall TRelatorioTensao::~TRelatorioTensao(void)
{
	// variaveis locais
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	// salva dados no infoset
	SalvaDados();
	// destroi objetos
	DestroiTObject(progresso);
	DestroiTObject(nivel_at);
	DestroiTObject(nivel_mt);
	DestroiTObject(nivel_bt);
	// limpa grafico
	grafico->LimpaDestaque();
	AtualizaGrafico(NULL);
}

// -----------------------------------------------------------------------------
void __fastcall TRelatorioTensao::AtualizaGrafico(TFaixaTensao *faixa)
{
	// variaveis locais
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	VTEqpto *eqpto = NULL;

	if (faixa == NULL)
	{ // redesenha todas as barras sem moldura
		grafico->Moldura(eqpto);
		grafico->Redraw();
		return;
	}
	// desenha moldura nas barras da faixa
	grafico->Moldura(faixa->LisBarra());
}

// -----------------------------------------------------------------------------
void __fastcall TRelatorioTensao::CopiaFaixas(TNivelTensao *nivel_ori, TNivelTensao *nivel_des)
{
	// variaveis locais
	TFaixaTensao *faixa_ori, *faixa_des;
	TList *lisFaixaOri, *lisFaixaDes;

	lisFaixaDes = nivel_des->LisFaixa();
	lisFaixaOri = nivel_ori->LisFaixa();
	for (int n = 0; n < lisFaixaDes->Count; n++)
	{ // copia dados da faixa_ori
		faixa_des = (TFaixaTensao*)lisFaixaDes->Items[n];
		faixa_ori = (TFaixaTensao*)lisFaixaOri->Items[n];
		faixa_des->valor_sup = faixa_ori->valor_sup;
		faixa_des->valor_inf = faixa_ori->valor_inf;
		// faixa_des->cor = faixa_ori->cor;
	}
}

// -----------------------------------------------------------------------------
bool __fastcall TRelatorioTensao::Executa(void)
{
	// variaveis locais
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	// verifica se houve alteração na configuração
	if (!config_alterada)
		return true;
	// abre tela de progresso simples
	ProgressoStart();
	try
	{ // limpa destaques da rede
		grafico->LimpaDestaque();
		// preenche listas de barras de cada faixa e de cada nivel
		nivel_bt->Executa(this);
		nivel_mt->Executa(this);
		nivel_at->Executa(this);
		// pinta as barras conforme as faixas de tensão e as cores determinadas
		PintaBarras(nivel_bt);
		PintaBarras(nivel_mt);
		PintaBarras(nivel_at);
		// atualiza grafico
		grafico->Redraw();
	}
	catch (...)
	{ // fecha progresso
		ProgressoStop();
		return false;
	}
	// atualiza config_alterada
	config_alterada = false;
	// fecha progresso
	ProgressoStop();
	return true;
}

// -----------------------------------------------------------------------------
bool __fastcall TRelatorioTensao::FluxoOk(void)
{
	// variaveis locais
	VTConsulta *consulta;
	VTPatamar *patamar;
	VTFlow *flow = (VTFlow*)apl->GetObject(__classid(VTFlow));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	bool sucesso = true;

	// verifica se o objeto Flow existe
	if (flow == NULL)
	{
		Aviso("Erro ao executar fluxo de potência");
		return false;
	}
	// verifica se ja foi rodado fluxo
	if (!flow->UpToDate)
	{
		if (Confirma("O módulo exige que o fluxo de potência seja executado.",
			"Deseja continuar ?") != ID_YES)
		{
			return false;
		}
		// calcula fluxo
		sucesso = flow->CalculaPorRede(redes);
		ind_pat = -1;
	}
	if (ind_pat < 0)
	{
		try
		{ // determina Patamar com maior fluxo de potência
			consulta = DLL_NewObjConsulta(apl);
			if ((patamar = consulta->PatamarFluxoMaximo(redes)) != NULL)
			{
				ind_pat = patamar->Index;
			}
			delete consulta;
		}
		catch (...)
		{ // nada a fazer
			sucesso = false;
		}
	}
	return sucesso;
}

// ---------------------------------------------------------------------------
void __fastcall TRelatorioTensao::LeDados(void)
{
	// variaveis locais
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
	TFaixaTensao *faixa;
	TList *lisFAIXA;
	AnsiString aux;
	int int_aux;

	// le parâmetros
	if (geral)
	{ // recupera do infoset
		if (geral->Infoset->LoadFromFile("TRelatorioTensao"))
		{ // tensao de fase
			geral->Infoset->GetInfo("TensaoFase", aux);
			tensao_fase = (aux.AnsiCompareIC("1") == 0);
			// tensao minima
			geral->Infoset->GetInfo("TensaoMinima", aux);
			tensao_minima = (aux.AnsiCompareIC("1") == 0);
			// niveis de tensão
			// AT
			lisFAIXA = nivel_at->LisFaixa();
			for (int n = 0; n < lisFAIXA->Count; n++)
			{
				faixa = (TFaixaTensao*)lisFAIXA->Items[n];
				geral->Infoset->GetInfo("AT_F" + AnsiString(n) + "_Inf", faixa->valor_inf);
				geral->Infoset->GetInfo("AT_F" + AnsiString(n) + "_Sup", faixa->valor_sup);
				geral->Infoset->GetInfo("AT_F" + AnsiString(n) + "_Cor", int_aux);
				faixa->cor = (TColor)int_aux;
			}
			// MT
			lisFAIXA = nivel_mt->LisFaixa();
			for (int n = 0; n < lisFAIXA->Count; n++)
			{
				faixa = (TFaixaTensao*)lisFAIXA->Items[n];
				geral->Infoset->GetInfo("MT_F" + AnsiString(n) + "_Inf", faixa->valor_inf);
				geral->Infoset->GetInfo("MT_F" + AnsiString(n) + "_Sup", faixa->valor_sup);
				geral->Infoset->GetInfo("MT_F" + AnsiString(n) + "_Cor", int_aux);
				faixa->cor = (TColor)int_aux;
			}
			// BT
			lisFAIXA = nivel_bt->LisFaixa();
			for (int n = 0; n < lisFAIXA->Count; n++)
			{
				faixa = (TFaixaTensao*)lisFAIXA->Items[n];
				geral->Infoset->GetInfo("BT_F" + AnsiString(n) + "_Inf", faixa->valor_inf);
				geral->Infoset->GetInfo("BT_F" + AnsiString(n) + "_Sup", faixa->valor_sup);
				geral->Infoset->GetInfo("BT_F" + AnsiString(n) + "_Cor", int_aux);
				faixa->cor = (TColor)int_aux;
			}
		}
	}
}

// -----------------------------------------------------------------------------
void __fastcall TRelatorioTensao::PintaBarras(TNivelTensao *nivel)
{
	// variaveis locais
	TFaixaTensao *faixa;
	TList *lisFAIXA;
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	int dim = 2;

	// proteçao
	if (nivel == NULL)
		return;
	// determina faixas
	lisFAIXA = nivel->LisFaixa();

	for (int n = 0; n < lisFAIXA->Count; n++)
	{ // destaca as barras da faixa com a cor da faixa
		faixa = (TFaixaTensao*)lisFAIXA->Items[n];
		grafico->DestacaBarras(faixa->LisBarra(), faixa->cor, dim);
	}
}

// -----------------------------------------------------------------------------
void __fastcall TRelatorioTensao::ProgressoStart(void)
{
	// abre tela de progresso simples
	if (progresso != NULL)
	{
		progresso->Start(progDEFAULT);
	}
}

// -----------------------------------------------------------------------------
void __fastcall TRelatorioTensao::ProgressoStop(void)
{
	// fecha progresso
	if (progresso != NULL)
		progresso->Stop();
}

// ---------------------------------------------------------------------------
void __fastcall TRelatorioTensao::SalvaDados(void)
{
	// variaveis locais
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
	TFaixaTensao *faixa;
	TList *lisFAIXA;
	AnsiString aux;

	// salva parâmetros
	if (geral)
	{ // tensao de fase
		aux = (tensao_fase ? "1" : "0");
		geral->Infoset->AddInfo("TensaoFase", aux);
		// tensao minima
		aux = (tensao_minima ? "1" : "0");
		geral->Infoset->AddInfo("TensaoMinima", aux);
		// niveis de tensão
		// AT
		lisFAIXA = nivel_at->LisFaixa();
		for (int n = 0; n < lisFAIXA->Count; n++)
		{
			faixa = (TFaixaTensao*)lisFAIXA->Items[n];
			geral->Infoset->AddInfo("AT_F" + AnsiString(n) + "_Inf", faixa->valor_inf);
			geral->Infoset->AddInfo("AT_F" + AnsiString(n) + "_Sup", faixa->valor_sup);
			geral->Infoset->AddInfo("AT_F" + AnsiString(n) + "_Cor", (int)faixa->cor);
		}
		// MT
		lisFAIXA = nivel_mt->LisFaixa();
		for (int n = 0; n < lisFAIXA->Count; n++)
		{
			faixa = (TFaixaTensao*)lisFAIXA->Items[n];
			geral->Infoset->AddInfo("MT_F" + AnsiString(n) + "_Inf", faixa->valor_inf);
			geral->Infoset->AddInfo("MT_F" + AnsiString(n) + "_Sup", faixa->valor_sup);
			geral->Infoset->AddInfo("MT_F" + AnsiString(n) + "_Cor", (int)faixa->cor);
		}
		// BT
		lisFAIXA = nivel_bt->LisFaixa();
		for (int n = 0; n < lisFAIXA->Count; n++)
		{
			faixa = (TFaixaTensao*)lisFAIXA->Items[n];
			geral->Infoset->AddInfo("BT_F" + AnsiString(n) + "_Inf", faixa->valor_inf);
			geral->Infoset->AddInfo("BT_F" + AnsiString(n) + "_Sup", faixa->valor_sup);
			geral->Infoset->AddInfo("BT_F" + AnsiString(n) + "_Cor", (int)faixa->cor);
		}
		geral->Infoset->SaveToFile("TRelatorioTensao");
	}
}

// -----------------------------------------------------------------------------
