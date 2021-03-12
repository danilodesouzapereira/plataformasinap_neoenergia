// ---------------------------------------------------------------------------
#pragma hdrstop
#include <Math.hpp>
#include "TTensaoMinima.h"
#include "..\Apl\VTApl.h"
#include "..\Basico\VTCfgCor.h"
#include "..\Tensao\TTemaTensao.h"
#include "..\..\Apl\VTApl.h"
#include "..\..\Complexo\Complexo.h"
#include "..\..\Constante\Fases.h"
#include "..\..\Diretorio\VTPath.h"
#include "..\..\Distancia\VTCaminho.h"
#include "..\..\Editor\VTEdt.h"
#include "..\..\Flow\VTFlow.h"
#include "..\..\Geral\VTGeral.h"
#include "..\..\Geral\VTInfoset.h"
#include "..\..\Grafico\VTGrafico.h"
#include "..\..\Mercado\VTMercado.h"
#include "..\..\Patamar\VTPatamar.h"
#include "..\..\Patamar\VTPatamares.h"
#include "..\..\Radial\VTArvore.h"
#include "..\..\Radial\VTNode.h"
#include "..\..\Radial\VTPrimario.h"
#include "..\..\Radial\VTSecundario.h"
#include "..\..\Radial\VTRadial.h"
#include "..\..\Radial\VTTronco.h"
#include "..\..\Rede\VTBarra.h"
#include "..\..\Rede\VTEqbar.h"
#include "..\..\Rede\VTLigacao.h"
#include "..\..\Rede\VTNo.h"
#include "..\..\Rede\VTRede.h"
#include "..\..\Rede\VTRedes.h"
#include "..\..\Rede\VTResFlowBar.h"
#include "..\..\Rede\VTTipoRede.h"
#include "..\..\Rede\VTTrecho.h"
#include "..\..\Report\VTReport.h"
#include "..\..\..\DLL_Inc\Editor.h"
#include "..\..\..\DLL_Inc\Funcao.h"
#include "..\..\..\DLL_Inc\Help.h"
#include "..\..\..\DLL_Inc\Distancia.h"
#include "..\..\..\DLL_Inc\Radial.h"
#include "..\..\..\DLL_Inc\Report.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall TResBarra::TResBarra(void)
{
	// nada
}

// ---------------------------------------------------------------------------
__fastcall TResBarra::~TResBarra(void)
{
	// nada
}

// ---------------------------------------------------------------------------
__fastcall TTensaoMinima::TTensaoMinima(VTApl *apl_owner)
{
	// salva ponteiro
	this->apl = apl_owner;
	// cria objetos
	apl->Add(caminho = DLL_NewObjCaminho(apl));
	apl->Add(radial = DLL_NewObjRadial(apl));
	apl->Add(tronco = DLL_NewObjTronco(apl));
	lisLIG = new TList();
	lisRES_BAR = new TList();
	// anula ponteiros e valores na estrutura com opções
	opcao.rede = NULL;
	opcao.barra_ini = NULL;
	opcao.barra_fim = NULL;
	opcao.tronco = true;
	opcao.pat = 0;
	// opcao.ano = 0;
	// preenche estrutura geral
	PreencheGeral();
	PreencheGeralPat();
}

// ---------------------------------------------------------------------------
__fastcall TTensaoMinima::~TTensaoMinima(void)
{
	// destrói lista sem destruir seus objetos
	if (lisLIG)
	{
		delete lisLIG;
		lisLIG = NULL;
	}
	// destroi tudo
	if (lisLIG)
	{
		LimpaTList(lisRES_BAR);
		delete lisRES_BAR;
		lisRES_BAR = NULL;
	}
	// if (lisBAR_SEL) {delete lisBAR_SEL; lisBAR_SEL = NULL;}
}

// ---------------------------------------------------------------------------
VTBarra* __fastcall TTensaoMinima::BarraMenorTensaoFF(VTRede *rede, int ind_pat)
{
	// variáveis locais
	VTBarra *barra, *barra_min;
	TList *lisBAR = rede->LisBarra();
	double vbar_pu;
	double vmin_pu = 10.; // chute alto
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

	// inicia ponteiro
	barra_min = NULL;
	for (int n = 0; n < lisBAR->Count; n++)
	{ // procura menor tensão no patamar
		barra = (VTBarra*)lisBAR->Items[n];
		vbar_pu = barra->resflow->Vff_pu_min[ind_pat];
		// despreza zero
		if (IsDoubleZero(vbar_pu))
			continue;
		if (vmin_pu > vbar_pu)
		{
			barra_min = barra;
			vmin_pu = vbar_pu;
		}
	}
	return barra_min;
}

// ---------------------------------------------------------------------------
VTBarra* __fastcall TTensaoMinima::BarraMenorTensaoFN(VTRede *rede, int ind_pat)
{
	// variáveis locais
	VTBarra *barra, *barra_min;
	TList *lisBAR = rede->LisBarra();
	double vbar_pu;
	double vmin_pu = 10.; // chute alto
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

	// inicia ponteiro
	barra_min = NULL;
	for (int n = 0; n < lisBAR->Count; n++)
	{ // procura menor tensão no patamar
		barra = (VTBarra*)lisBAR->Items[n];
		vbar_pu = barra->resflow->Vfn_pu_min[ind_pat];
		// despreza zero
		if (IsDoubleZero(vbar_pu))
			continue;
		if (vmin_pu > vbar_pu)
		{
			barra_min = barra;
			vmin_pu = vbar_pu;
		}
	}
	return barra_min;
}

// ---------------------------------------------------------------------------
VTBarra* __fastcall TTensaoMinima::BarraMenorTensaoPatFF(VTRede *rede, int &pat_min)
{
	// variáveis locais
	VTBarra *barra, *barra_min;
	TList *lisBAR = rede->LisBarra();
	double vbar_pu;
	double vmin_pu = 10.; // chute alto
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

	// inicia ponteiro
	barra_min = NULL;
	pat_min = 0;
	for (int n = 0; n < lisBAR->Count; n++)
	{ // procura menor tensão e guarda patamar
		barra = (VTBarra*)lisBAR->Items[n];
		for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
		{
			vbar_pu = barra->resflow->Vff_pu_min[ind_pat];
			// despreza zero
			if (IsDoubleZero(vbar_pu))
				continue;
			if (vmin_pu > vbar_pu)
			{
				barra_min = barra;
				vmin_pu = vbar_pu;
				pat_min = ind_pat;
			}
		}
	}
	return barra_min;
}

// ---------------------------------------------------------------------------
VTBarra* __fastcall TTensaoMinima::BarraMenorTensaoPatFN(VTRede *rede, int &pat_min)
{
	// variáveis locais
	VTBarra *barra, *barra_min;
	TList *lisBAR = rede->LisBarra();
	double vbar_pu;
	double vmin_pu = 10.; // chute alto
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

	// inicia ponteiro
	barra_min = NULL;
	pat_min = 0;
	for (int n = 0; n < lisBAR->Count; n++)
	{ // procura menor tensão e guarda patamar
		barra = (VTBarra*)lisBAR->Items[n];
		for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
		{
			vbar_pu = barra->resflow->Vfn_pu_min[ind_pat];
			// despreza zero
			if (IsDoubleZero(vbar_pu))
				continue;
			if (vmin_pu > vbar_pu)
			{
				barra_min = barra;
				vmin_pu = vbar_pu;
				pat_min = ind_pat;
			}
		}
	}
	return barra_min;
}

// ---------------------------------------------------------------------------
void __fastcall TTensaoMinima::Calcula2Barras(void)
{
	// variaveis locais
	VTBarra *barra_ini, *barra_fim;
	VTLigacao *ligacao;
	VTRede *rede;
	int ind_pat;

	// guarda valores
	ind_pat = opcao.pat;
	try
	{ // guarda barra inicial e final
		barra_ini = opcao.barra_ini;
		barra_fim = opcao.barra_fim;
		// determina caminho elétrico entre barra inicial e barra final
		lisLIG->Clear();
		caminho->Executa(barra_fim, barra_ini, lisLIG);
		// cria resBarra conforme barras da ligação, começando pela barra inicial
		CriaResBarra(barra_ini, lisLIG);
	}
	catch (Exception &e)
	{
		// nada
	}
}

// ---------------------------------------------------------------------------
double __fastcall TTensaoMinima::CalculaDistancia_m(VTLigacao *ligacao)
{
	// variáveis locais
	int x1, y1, x2, y2;
	double dist_m;

	// verifica se a Ligacao é um trecho
	if (ligacao->Tipo() == eqptoTRECHO)
	{
		dist_m = ((VTTrecho*)ligacao)->Comprimento_m;
	}
	// verifica se as Barras tem coord.utm
	else if ((ligacao->pbarra1->CoordenadasUtm_cm(x1, y1)) && (ligacao->pbarra2->CoordenadasUtm_cm
		(x2, y2)))
	{
		dist_m = 0.01 * Distancia_cm(x1, y1, x2, y2);
	}
	else
	{
		dist_m = 2;
	}
	return (dist_m);
}

// ---------------------------------------------------------------------------
void __fastcall TTensaoMinima::CalculaTronco(void)
{
	// variaveis locais
	VTRede *rede;
	VTPrimario *primario;
	VTSecundario *secundario;
	int ind_pat;

	// guarda valores
	rede = opcao.rede;
	ind_pat = opcao.pat;
	// inicia radial
	if (!radial->Inicia(rede->Redes))
		return;
	// age conforme o segmento
	switch (rede->TipoRede->Segmento)
	{
	case redePRI:
		// seleciona primário da rede
		if ((primario = radial->ExistePrimario(rede)) == NULL)
			return;
		// inicia objeto tronco
		tronco->DefinePrimario(primario, ind_pat);
		// obtém lista de ligações que são do tronco
		lisLIG->Clear();
		CopiaTList(tronco->LisLigacaoTronco(), lisLIG);
		// cria resBarra conforme barras da ligação, começando pela barra inicial
		CriaResBarra(primario->BarraInicial, lisLIG);
		break;
	case redeSEC:
		// seleciona Secundário da rede
		if ((secundario = radial->ExisteSecundario(rede)) == NULL)
			return;
		// inicia objeto tronco
		tronco->DefineSecundario(secundario, ind_pat);
		// obtém lista de ligações que são do tronco
		lisLIG->Clear();
		CopiaTList(tronco->LisLigacaoTronco(), lisLIG);
		// cria resBarra conforme barras da ligação, começando pela barra inicial
		CriaResBarra(secundario->BarraInicial, lisLIG);
		break;
	default:
		return;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TTensaoMinima::CalculaPiorTensao(void)
{
	// variaveis locais
	VTBarra *barra_ini, *pior;
	VTLigacao *ligacao;
	VTRede *rede;
	int ind_pat;

	// guarda valores
	rede = opcao.rede;
	ind_pat = opcao.pat;
	try
	{ // guarda barra inicial
		barra_ini = rede->BarraInicial();
		// busca pior barra
		pior = BarraMenorTensaoFN(rede, ind_pat);
		// determina caminho elétrico entre barra inicial e pior barra
		lisLIG->Clear();
		caminho->Executa(pior, barra_ini, lisLIG);
		// cria resBarra conforme barras da ligação, começando pela barra inicial
		CriaResBarra(barra_ini, lisLIG);
	}
	catch (Exception &e)
	{
		// nada
	}
}

// ---------------------------------------------------------------------------
void __fastcall TTensaoMinima::CriaResBarra(VTBarra *bar_ini, TList *lisLIG)
{
	// variaveis locais
	TResBarra *res_bar;
	VTBarra *barra;
	VTLigacao *ligacao;
	int ind_pat = opcao.pat;
	double dist_total_m = 0.;

	// proteção
	if (bar_ini == NULL)
		return;
	try
	{ // primeira barra
		res_bar = new TResBarra();
		res_bar->barra = bar_ini;
		res_bar->van_pu_min = Abs(bar_ini->resflow->Van_pu[ind_pat]);
		res_bar->vbn_pu_min = Abs(bar_ini->resflow->Vbn_pu[ind_pat]);
		res_bar->vcn_pu_min = Abs(bar_ini->resflow->Vcn_pu[ind_pat]);
		res_bar->vfn_pu_min = bar_ini->resflow->Vfn_pu_min[ind_pat];
		res_bar->dist_m = 0.;
		res_bar->cod = bar_ini->Codigo;
		res_bar->pat_min = ind_pat;
		lisRES_BAR->Add(res_bar);
		// demais barras da lista: inicia distancia total em m
		dist_total_m = 0.;
		barra = bar_ini;
		for (int nl = 0; nl < lisLIG->Count; nl++)
		{
			ligacao = (VTLigacao*)lisLIG->Items[nl];
			// determina próxima Barra
			barra = (ligacao->pbarra1 == barra) ? ligacao->pbarra2 : ligacao->pbarra1;
			// determina distância total da barra à barra inicial
			dist_total_m += CalculaDistancia_m(ligacao);
			// insere res_barra
			res_bar = new TResBarra();
			res_bar->barra = barra;
			res_bar->van_pu_min = Abs(barra->resflow->Van_pu[ind_pat]);
			res_bar->vbn_pu_min = Abs(barra->resflow->Vbn_pu[ind_pat]);
			res_bar->vcn_pu_min = Abs(barra->resflow->Vcn_pu[ind_pat]);
			res_bar->vfn_pu_min = barra->resflow->Vfn_pu_min[ind_pat];
			res_bar->dist_m = dist_total_m;
			res_bar->cod = barra->Codigo;
			res_bar->pat_min = ind_pat;
			lisRES_BAR->Add(res_bar);
		}
		// guarda barra inicial e final (ultima do loop)
		dist.barra_ini = bar_ini;
		dist.barra_fim = barra;
	}
	catch (Exception &e)
	{
		// nada
	}
}

// ---------------------------------------------------------------------------
void __fastcall TTensaoMinima::Executa(void)
{
	// limpa lisRES_BAR
	LimpaTList(lisRES_BAR);
	// verifica opções selecionadas
	if (opcao.rede == NULL)
	{ // verifica se há barras selecionadas
		if ((opcao.barra_ini == NULL) && (opcao.barra_fim == NULL))
		{
			Aviso("Selecione uma rede.");
			return;
		}
	}
	if (opcao.pat < 0)
	{
		Aviso("Selecione um patamar válido.");
		return;
	}
	// verifica se há barras selecionadas
	if ((opcao.barra_ini != NULL) && (opcao.barra_fim != NULL))
	{
		Calcula2Barras();
        return;
	}
	else if (opcao.tronco)
	{
		CalculaTronco();
	}
	else
	{
		CalculaPiorTensao();
	}
	// preenche estrutura rede
	PreencheRede();
	PreencheGeralPat();
}

// ---------------------------------------------------------------------------
TList* __fastcall TTensaoMinima::LisLig(void)
{
	return (lisLIG);
}

// ---------------------------------------------------------------------------
TList* __fastcall TTensaoMinima::LisResBarra(void)
{
	return (lisRES_BAR);
}

// ---------------------------------------------------------------------------
void __fastcall TTensaoMinima::PreencheGeral(void)
{
	// considera fase-fase
	PreencheGeralFF();
	// considera fase-neutro
	PreencheGeralFN();
}

// ---------------------------------------------------------------------------
void __fastcall TTensaoMinima::PreencheGeralFF(void)
{
	// variáveis locais
	VTBarra *barra, *pior;
	VTRede *rede;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
	TList *lisREDE = redes->LisRede();
	int menor_pat, ind_pat;

	// inicia ponteiros
	barra = pior = NULL;
	menor_pat = 0;
	// localiza barra com menor tensão
	for (int n = 0; n < lisREDE->Count; n++)
	{
		rede = (VTRede*)lisREDE->Items[n];
		barra = BarraMenorTensaoPatFF(rede, ind_pat);
		if (barra == NULL)
			continue;
		if (pior == NULL)
		{
			pior = barra;
			menor_pat = ind_pat;
			resgeral.pat_min = patamares->Patamar[menor_pat]->Periodo;
			continue;
		}
		// verifica se a barra é a pior
		if (barra->resflow->Vff_pu_min[ind_pat] < pior->resflow->Vff_pu_min[menor_pat])
		{
			pior = barra;
			menor_pat = ind_pat;
			resgeral.pat_min = patamares->Patamar[menor_pat]->Periodo;
		}
	}
	// proteção
	if (pior == NULL)
		return;
	// salva valores
	resgeral.barra_ff = pior;
	resgeral.vab_pu_min = Abs(pior->resflow->Vab_pu[menor_pat]);
	resgeral.vbc_pu_min = Abs(pior->resflow->Vbc_pu[menor_pat]);
	resgeral.vca_pu_min = Abs(pior->resflow->Vca_pu[menor_pat]);
}

// ---------------------------------------------------------------------------
void __fastcall TTensaoMinima::PreencheGeralFN(void)
{
	// variáveis locais
	VTBarra *barra, *pior;
	VTRede *rede;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
	TList *lisREDE = redes->LisRede();
	int menor_pat, ind_pat;

	// inicia ponteiros
	barra = pior = NULL;
	menor_pat = 0;
	// localiza barra com menor tensão
	for (int n = 0; n < lisREDE->Count; n++)
	{
		rede = (VTRede*)lisREDE->Items[n];
		barra = BarraMenorTensaoPatFN(rede, ind_pat);
		if (barra == NULL)
			continue;
		if (pior == NULL)
		{
			pior = barra;
			menor_pat = ind_pat;
			resgeral.pat_min = patamares->Patamar[menor_pat]->Periodo;
			continue;
		}
		// verifica se a barra é a pior
		if (barra->resflow->Vfn_pu_min[ind_pat] < pior->resflow->Vfn_pu_min[menor_pat])
		{
			pior = barra;
			menor_pat = ind_pat;
			resgeral.pat_min = patamares->Patamar[menor_pat]->Periodo;
		}
	}
	// proteção
	if (pior == NULL)
		return;
	// salva valores
	resgeral.barra_fn = pior;
	resgeral.van_pu_min = Abs(pior->resflow->Van_pu[menor_pat]);
	resgeral.vbn_pu_min = Abs(pior->resflow->Vbn_pu[menor_pat]);
	resgeral.vcn_pu_min = Abs(pior->resflow->Vcn_pu[menor_pat]);
}

// ---------------------------------------------------------------------------
void __fastcall TTensaoMinima::PreencheGeralPat(void)
{
	// considera fase-fase
	PreencheGeralPatFF();
	// considera fase-neutro
	PreencheGeralPatFN();
}

// ---------------------------------------------------------------------------
void __fastcall TTensaoMinima::PreencheGeralPatFF(void)
{
	// variáveis locais
	VTBarra *barra, *pior;
	VTRede *rede;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
	TList *lisREDE = redes->LisRede();
	int menor_pat = opcao.pat;

	// inicia ponteiros
	barra = pior = NULL;
	// localiza barra com menor tensão
	for (int n = 0; n < lisREDE->Count; n++)
	{
		rede = (VTRede*)lisREDE->Items[n];
		barra = BarraMenorTensaoFF(rede, menor_pat);
		if (barra == NULL)
			continue;
		if (pior == NULL)
		{
			pior = barra;
			resgeralpat.pat_min = patamares->Patamar[menor_pat]->Periodo;
			continue;
		}
		// verifica se a barra é a pior
		if (barra->resflow->Vff_pu_min[menor_pat] < pior->resflow->Vff_pu_min[menor_pat])
		{
			pior = barra;
			resgeralpat.pat_min = patamares->Patamar[menor_pat]->Periodo;
		}
	}
	// proteção
	if (pior == NULL)
		return;
	// salva valores
	resgeralpat.barra_ff = pior;
	resgeralpat.vab_pu_min = Abs(pior->resflow->Vab_pu[menor_pat]);
	resgeralpat.vbc_pu_min = Abs(pior->resflow->Vbc_pu[menor_pat]);
	resgeralpat.vca_pu_min = Abs(pior->resflow->Vca_pu[menor_pat]);
}

// ---------------------------------------------------------------------------
void __fastcall TTensaoMinima::PreencheGeralPatFN(void)
{
	// variáveis locais
	VTBarra *barra, *pior;
	VTRede *rede;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
	TList *lisREDE = redes->LisRede();
	int menor_pat = opcao.pat;

	// inicia ponteiros
	barra = pior = NULL;
	// localiza barra com menor tensão
	for (int n = 0; n < lisREDE->Count; n++)
	{
		rede = (VTRede*)lisREDE->Items[n];
		barra = BarraMenorTensaoFN(rede, menor_pat);
		if (barra == NULL)
			continue;
		if (pior == NULL)
		{
			pior = barra;
			resgeralpat.pat_min = patamares->Patamar[menor_pat]->Periodo;
			continue;
		}
		// verifica se a barra é a pior
		if (barra->resflow->Vfn_pu_min[menor_pat] < pior->resflow->Vfn_pu_min[menor_pat])
		{
			pior = barra;
			resgeralpat.pat_min = patamares->Patamar[menor_pat]->Periodo;
		}
	}
	// proteção
	if (pior == NULL)
		return;
	// salva valores
	resgeralpat.barra_fn = pior;
	resgeralpat.van_pu_min = Abs(pior->resflow->Van_pu[menor_pat]);
	resgeralpat.vbn_pu_min = Abs(pior->resflow->Vbn_pu[menor_pat]);
	resgeralpat.vcn_pu_min = Abs(pior->resflow->Vcn_pu[menor_pat]);
}

// ---------------------------------------------------------------------------
void __fastcall TTensaoMinima::PreencheRede(void)
{
	// considera fase-fase
	PreencheRedeFF();
	// considera fase-neutro
	PreencheRedeFN();
}

// ---------------------------------------------------------------------------
void __fastcall TTensaoMinima::PreencheRedeFF(void)
{
	// variáveis locais
	VTBarra *pior;
	VTRede *rede = opcao.rede;
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
	int menor_pat, ind_pat;

	// inicia ponteiros
	pior = NULL;
	menor_pat = 0;
	// localiza barra com menor tensão
	pior = BarraMenorTensaoPatFF(rede, menor_pat);
	resrede.pat_min = patamares->Patamar[menor_pat]->Periodo;

	// proteção
	if (pior == NULL)
		return;
	// salva valores
	resrede.barra_ff = pior;
	resrede.vab_pu_min = Abs(pior->resflow->Vab_pu[menor_pat]);
	resrede.vbc_pu_min = Abs(pior->resflow->Vbc_pu[menor_pat]);
	resrede.vca_pu_min = Abs(pior->resflow->Vca_pu[menor_pat]);
}

// ---------------------------------------------------------------------------
void __fastcall TTensaoMinima::PreencheRedeFN(void)
{
	// variáveis locais
	VTBarra *pior;
	VTRede *rede = opcao.rede;
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
	int menor_pat, ind_pat;

	// inicia ponteiros
	pior = NULL;
	menor_pat = 0;
	// localiza barra com menor tensão
	pior = BarraMenorTensaoPatFN(rede, menor_pat);
	resrede.pat_min = patamares->Patamar[menor_pat]->Periodo;

	// proteção
	if (pior == NULL)
		return;
	// salva valores
	resrede.barra_fn = pior;
	resrede.van_pu_min = Abs(pior->resflow->Van_pu[menor_pat]);
	resrede.vbn_pu_min = Abs(pior->resflow->Vbn_pu[menor_pat]);
	resrede.vcn_pu_min = Abs(pior->resflow->Vcn_pu[menor_pat]);
}

// ---------------------------------------------------------------------------
