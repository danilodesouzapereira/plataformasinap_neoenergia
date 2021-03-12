// ---------------------------------------------------------------------------
#pragma hdrstop

#include<map>

#include "TNivelTensao.h"
#include "TFaixaTensao.h"
#include "TRelatorioTensao.h"
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Geral\VTGeral.h>
#include <Fontes\Geral\VTPadraoRede.h>
#include <Fontes\Rede\VTBarra.h>
#include <Fontes\Rede\VTRedes.h>
#include <Fontes\Rede\VTResFlowBar.h>
#include <Fontes\Rede\VTTipoRede.h>
#include <DLL_Inc\Funcao.h>

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// -----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/16082236
__fastcall TNivelTensao::TNivelTensao(VTApl *apl, int nivel)
{
	// salva ponteiro p/ objeto
	this->apl = apl;
	// inicia variavel
	this->nivel = nivel;
	// cria lista
	lisFAIXA = new TList();
	// cria objetos TFaixaTensao
	IniciaLisFaixa();
}

// -----------------------------------------------------------------------------
__fastcall TNivelTensao::~TNivelTensao(void)
{
	// destroi lista e seus objetos
	if (lisFAIXA != NULL)
	{
		LimpaTList(lisFAIXA);
		delete lisFAIXA;
		lisFAIXA = NULL;
	}
	// limpa map
	tensao_barras.clear();
}

// -----------------------------------------------------------------------------
TNivelTensao* __fastcall TNivelTensao::Clone(void)
{
	// variaveis locais
	TNivelTensao *clone;
	TFaixaTensao *faixaClone, *faixa;
	TList *lisFaixaClone;

	// cria um nivel novo
	if ((clone = new TNivelTensao(apl, nivel)) != NULL)
	{
		lisFaixaClone = clone->LisFaixa();
		for (int n = 0; n < lisFaixaClone->Count; n++)
		{ // copia dados da faixa
			faixaClone = (TFaixaTensao*)lisFaixaClone->Items[n];
			faixa = (TFaixaTensao*)lisFAIXA->Items[n];
			faixaClone->valor_sup = faixa->valor_sup;
			faixaClone->valor_inf = faixa->valor_inf;
			faixaClone->cor = faixa->cor;
		}
	}
    return clone;
}

// -----------------------------------------------------------------------------
double __fastcall TNivelTensao::DeterminaTensao(VTBarra *barra)
{
	// variaveis locais
	double v_pu = 0.;

	// verifica opções do relatorio_tensao
	if ((relatorio_tensao->tensao_minima) && (relatorio_tensao->tensao_fase))
	{
		v_pu = barra->resflow->Vfn_pu_min[relatorio_tensao->ind_pat];
	}

	if ((relatorio_tensao->tensao_minima) && (!relatorio_tensao->tensao_fase))
	{
		v_pu = barra->resflow->Vff_pu_min[relatorio_tensao->ind_pat];
	}

	if ((!relatorio_tensao->tensao_minima) && (relatorio_tensao->tensao_fase))
	{
		v_pu = barra->resflow->Vfn_pu_max[relatorio_tensao->ind_pat];
	}

	if ((!relatorio_tensao->tensao_minima) && (!relatorio_tensao->tensao_fase))
	{
		v_pu = barra->resflow->Vff_pu_max[relatorio_tensao->ind_pat];
	}
	return v_pu;
}

// -----------------------------------------------------------------------------
void __fastcall TNivelTensao::Executa(TRelatorioTensao *relatorio_tensao)
{
	// variaveis locais
	TFaixaTensao *faixa;
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
	int ind = 1;

	// salva ponteiro
	this->relatorio_tensao = relatorio_tensao;
	// proteção
	if (this->relatorio_tensao == NULL)
		return;
	if (this->relatorio_tensao->ind_pat < 0)
		return;
	if ((nivel < 0) || (nivel > 2))
		return;
	// separa barras que correspondam ao nivel
	if (nivel == nvAT)
		ind = 2;
	if (nivel == nvMT)
		ind = 1;
	if (nivel == nvBT)
		ind = 0;
	PreencheMap(geral->LIM_V.LIM_KV[ind][0], geral->LIM_V.LIM_KV[ind][1]);
	// determina barras que correspondam a cada faixa
	for (int n = 0; n < lisFAIXA->Count; n++)
	{
		faixa = (TFaixaTensao*)lisFAIXA->Items[n];
		PreencheFaixa(faixa);
	}
}

// -----------------------------------------------------------------------------
void __fastcall TNivelTensao::IniciaLisFaixa(void)
{
	// variaveis locais
	TFaixaTensao *faixa;
	VTPadraoRede *padraoRede;
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
	int tipo_rede_id = redePRI;

	try
	{ // verifica nivel
		switch (nivel)
		{
		case nvAT:
			tipo_rede_id = redeSUB;
			break;
		case nvMT:
			tipo_rede_id = redePRI;
			break;
		case nvBT:
			tipo_rede_id = redeSEC;
			break;
		default:
			break;
		}
		// determina padrão rede (com valores default)
		padraoRede = geral->ExistePadraoRede(tipo_rede_id);
		if (padraoRede == NULL)
			return;
		// cria uma faixa para cada limite de tensão
		// abaixo de V1
		faixa = new TFaixaTensao(apl);
		lisFAIXA->Add(faixa);
		faixa->valor_inf = -1.;
		faixa->valor_sup = padraoRede->V1;
		faixa->cor = clLime;
		// V1 ate V2
		faixa = new TFaixaTensao(apl);
		lisFAIXA->Add(faixa);
		faixa->valor_inf = padraoRede->V1;
		faixa->valor_sup = padraoRede->V2;
		faixa->cor = clGreen;
		// V2 ate V3
		faixa = new TFaixaTensao(apl);
		lisFAIXA->Add(faixa);
		faixa->valor_inf = padraoRede->V2;
		faixa->valor_sup = padraoRede->V3;
		faixa->cor = clYellow;
		// V3 ate V4
		faixa = new TFaixaTensao(apl);
		lisFAIXA->Add(faixa);
		faixa->valor_inf = padraoRede->V3;
		faixa->valor_sup = padraoRede->V4;
		faixa->cor = clWebCoral;
		// acima de V4
		faixa = new TFaixaTensao(apl);
		lisFAIXA->Add(faixa);
		faixa->valor_inf = padraoRede->V4;
		faixa->valor_sup = -1.;
		faixa->cor = clRed;
	}
	catch (...)
	{
		LimpaTList(lisFAIXA);
	}
}

// -----------------------------------------------------------------------------
TList* __fastcall TNivelTensao::LisFaixa(void)
{
	return lisFAIXA;
}

// -----------------------------------------------------------------------------
void __fastcall TNivelTensao::PreencheFaixa(TFaixaTensao *faixa)
{
	// variaveis locais
	std::map<VTBarra*, double>::iterator it = tensao_barras.begin();

	// limpa lista de barras
	faixa->LisBarra()->Clear();
	while (it != tensao_barras.end())
	{ // verifica se um dos limites é infinito
		if (faixa->valor_inf < 0)
		{
			if (it->second <= faixa->valor_sup)
			{
				faixa->InsereBarra(it->first);
			}
		}
		else if (faixa->valor_sup < 0)
		{
			if (it->second > faixa->valor_inf)
			{
				faixa->InsereBarra(it->first);
			}
		}
		// não é infinito
		else if ((it->second > faixa->valor_inf) && (it->second <= faixa->valor_sup))
		{
			faixa->InsereBarra(it->first);
		}
		it++;
	}
}

// -----------------------------------------------------------------------------
void __fastcall TNivelTensao::PreencheMap(double v_inf, double v_sup)
{
	// variaveis locais
	VTBarra *barra;
	TList *lisBARRA;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	double v_pu = 0.;

	// limpa map
	tensao_barras.clear();

	// percorre barras
	lisBARRA = redes->LisBarra();
	for (int n = 0; n < lisBARRA->Count; n++)
	{
		barra = (VTBarra*)lisBARRA->Items[n];
		// nivel MT não inclui valor inferior ou superior no intervalo
		if (nivel == nvMT)
		{ // verifica intervalo do nivel de tensão (sem incluir valores)
			if ((barra->vnom > v_inf) && (barra->vnom < v_sup))
			{ // determina valor da tensão
				v_pu = DeterminaTensao(barra);
				tensao_barras[barra] = v_pu;
			}
		}
		else
		{ // verifica intervalo do nivel de tensão (incluindo valores)
			if ((barra->vnom >= v_inf) && (barra->vnom <= v_sup))
			{ // determina valor da tensão
				v_pu = DeterminaTensao(barra);
				tensao_barras[barra] = v_pu;
			}
		}
	}
}

// -----------------------------------------------------------------------------
