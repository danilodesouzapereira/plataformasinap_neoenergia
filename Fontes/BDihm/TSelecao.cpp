// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TSelecao.h"
#include "..\Apl\VTApl.h"
#include "..\BD\VTBD.h"
#include "..\BD\Figura\VTBDfigura.h"
#include "..\BD\Rede\VTBDeqpto.h"
#include "..\Rede\VTCorte.h"
#include "..\Rede\VTMRede.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\..\DLL_Inc\Funcao.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
// função global p/ criar objeto da classe VTSelecao
// ---------------------------------------------------------------------------
VTSelecao* __fastcall NewObjSelecao(VTApl *apl)
{
	return (new TSelecao(apl));
}

// ---------------------------------------------------------------------------
__fastcall TSelecao::TSelecao(VTApl *apl)
{
	// salva ponteiro
	this->apl = apl;
	// cria listas
	lisEQP = new TList();
	// lisCORTE     = new TList();
	lisMREDE_SEL = new TList();
	lisMREDE_VIZ = new TList();
	lisREDE_VIZ = new TList();
	lisMREDE_PRE = new TList();
	lisMREDE_REDUZIDAS = new TList();
}

// ---------------------------------------------------------------------------
__fastcall TSelecao::~TSelecao(void)
{
	// destrói listas sem destruir seus objetos
	if (lisEQP)
	{
		delete lisEQP;
		lisEQP = NULL;
	}
	// if (lisCORTE)     {delete lisCORTE;     lisCORTE     = NULL;}
	if (lisMREDE_SEL)
	{
		delete lisMREDE_SEL;
		lisMREDE_SEL = NULL;
	}
	if (lisMREDE_VIZ)
	{
		delete lisMREDE_VIZ;
		lisMREDE_VIZ = NULL;
	}
	if (lisMREDE_PRE)
	{
		delete lisMREDE_PRE;
		lisMREDE_PRE = NULL;
	}
	if (lisREDE_VIZ)
	{
		delete lisREDE_VIZ;
		lisREDE_VIZ = NULL;
	}
	if (lisMREDE_REDUZIDAS)
	{
		delete lisMREDE_REDUZIDAS;
		lisMREDE_REDUZIDAS = NULL;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TSelecao::AlteraMRedeValida(TList *lisREDE)
{
	// variáveis locais
	VTRede *rede;

	// loop p/ todas Redes
	for (int nr = 0; nr < lisREDE->Count; nr++)
	{
		rede = (VTRede*)lisREDE->Items[nr];
		// verifica se é uma MRede que pode ser carregada
		rede->MRede->Carregada = rede->MRede->Valida;
	}
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
bool __fastcall TSelecao::CarregaEstudo(VTBD *bd, VTEstudo *estudo)
{
	// salva ponteiro p/ objetos
	this->bd = bd;
	this->estudo = estudo;
	// reinicia dados gerais
	// ReiniciaObjetosBasicos();
	// carrega estudo da base de dados
	bd->Eqpto->CarregaEstudo(estudo);
   //carrega figuras
   bd->Figura->LeFiguras(estudo, false);

	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TSelecao::CarregaRedeAdicional(VTBD *bd, VTEstudo *estudo, TList *lisREDE_PRE,
	TList *lisREDE_SEL, int aviso_num_rede)
{
	// variáveis locais
	VTMRede *mrede;

	// salva ponteiro p/ objetos
	this->bd = bd;
	this->estudo = estudo;
	// verifica se há redes selecionadas
	if (lisREDE_SEL->Count == 0)
		return (false);
	// proteção: verifica se há muitos objetos VTRede selecionados
	if ((aviso_num_rede > 0) && (lisREDE_SEL->Count > aviso_num_rede))
	{
		if (Confirma("Foram selecionadas " + IntToStr(lisREDE_SEL->Count) + " redes",
			"Confirma a seleção ?") != IDYES)
			return (false);
	}
	// altera parametro carregada de MRedes selecionadas válidas
	AlteraMRedeValida(lisREDE_SEL);
	// inicia lista de MRedes selecionadas
	IniciaLisMRedeSelecionada(lisREDE_SEL);
	// inicia listas de MRedes já carregadas que devem permancer
	IniciaLisMRedePreCarregadas(lisREDE_PRE);
	// lê lista de todas Redes da base
	LeListaRedesExistentes(bd, estudo);
	// inicia lista de MRedes vizinhas
	IniciaLisMRedeVizinha();
	// retira de lisMREDE_SEL e de lisMREDE_VIZ as MRede já carregadas
	RetiraLis1DeLis2(lisMREDE_PRE, lisMREDE_SEL);
	RetiraLis1DeLis2(lisMREDE_PRE, lisMREDE_VIZ);
	RetiraLis1DeLis2(lisMREDE_REDUZIDAS, lisMREDE_VIZ);
	// lê todos os Cortes da MRede selecionadas
	if (!bd->Eqpto->LeListaCorte(estudo, lisMREDE_SEL, NULL))
		return (false);
	// lê todos os Cortes da MRede vizinhas
	if (!bd->Eqpto->LeListaCorte(estudo, lisMREDE_VIZ, NULL))
		return (false);
	// elimina todas MRedes e Redes que não estão em lisMREDE_SEL e lisMREDE_VIZ
	DestroiMRedeNaoSelNaoViz();
	// cada MRede vizinha deve permancer apenas com um Corte
	PreparaMRedeVizinha();
	// elimina todas MRedes e Redes que não estão em lisMREDE_SEL e lisMREDE_VIZ
	DestroiMRedeNaoSelNaoViz();
	// carrega as MRedes selecionadas e as Reducoes das MRede vizinhas
	bd->Eqpto->CarregaMRedeEstudo(estudo, lisMREDE_SEL, lisMREDE_VIZ);

	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TSelecao::CarregaRedeAdicional(VTBD *bd, VTEstudo *estudo, TList *lisREDE_SEL,
	int aviso_num_rede)
{
	// salva ponteiro p/ objetos
	this->bd = bd;
	this->estudo = estudo;
	// verifica se há redes selecionadas
	if (lisREDE_SEL->Count == 0)
		return (false);
	// proteção: verifica se há muitos objetos VTRede selecionados
	if ((aviso_num_rede > 0) && (lisREDE_SEL->Count > aviso_num_rede))
	{
		if (Confirma("Foram selecionadas " + IntToStr(lisREDE_SEL->Count) + " redes",
			"Confirma a seleção ?") != IDYES)
			return (false);
	}
	// inicia lista de MRedes selecionadas
	IniciaLisMRedeSelecionada(lisREDE_SEL);
	// inicia lista de MRedes já carregadas e que devem permancer
	IniciaLisMRedePreCarregadas();
	// lê lista de todas Redes da base
	LeListaRedesExistentes(bd, estudo);
	// inicia lista de MRedes vizinhas
	IniciaLisMRedeVizinha();
	// retira de lisMREDE_SEL e de lisMREDE_VIZ as MRede já carregadas
	RetiraLis1DeLis2(lisMREDE_PRE, lisMREDE_SEL);
	RetiraLis1DeLis2(lisMREDE_PRE, lisMREDE_VIZ);
	// lê todos os Cortes da MRede selecionadas
	if (!bd->Eqpto->LeListaCorte(estudo, lisMREDE_SEL, NULL))
		return (false);
	// lê todos os Cortes da MRede vizinhas
	if (!bd->Eqpto->LeListaCorte(estudo, lisMREDE_VIZ, NULL))
		return (false);
	// elimina todas MRedes e Redes que não estão em lisMREDE_SEL e lisMREDE_VIZ
	DestroiMRedeNaoSelNaoViz();
	// cada MRede vizinha deve permancer apenas com um Corte
	PreparaMRedeVizinha();
	// elimina todas MRedes e Redes que não estão em lisMREDE_SEL e lisMREDE_VIZ
	DestroiMRedeNaoSelNaoViz();
	// carrega as MRedes selecionadas e as Reducoes das MRede vizinhas
	bd->Eqpto->CarregaMRedeEstudo(estudo, lisMREDE_SEL, lisMREDE_VIZ);

	return (true);
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
bool __fastcall TSelecao::CarregaRedeCompleta(VTBD *bd, VTEstudo *estudo, int aviso_num_rede)
{
	// variáveis locais
	TList *lisMR;
	VTMRede *mrede;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// salva ponteiro p/ objetos
	this->bd = bd;
	this->estudo = estudo;
	// reinicia dados gerais
	// ReiniciaObjetosBasicos();
	// lê todas as Redes do Estudo (e suas MRedes)
	bd->Eqpto->LeListaRede(estudo, (VTTipoRede*)NULL);
	// inicia lista lisMREDE_SEL c/ as MRedes válidas e lisMREDE_VIZ com as demais
	lisMREDE_SEL->Clear();
	lisMREDE_VIZ->Clear();
	lisMR = redes->LisMRede();
	for (int n = 0; n < lisMR->Count; n++)
	{
		mrede = (VTMRede*)lisMR->Items[n];
		if (mrede->Carregada)
			lisMREDE_SEL->Add(mrede);
		else
			lisMREDE_VIZ->Add(mrede);
	}
	// proteção: verifica se há muitos objeto VTRede selecionado
	if ((aviso_num_rede > 0) && (lisMREDE_SEL->Count > aviso_num_rede))
	{
		if (Confirma("Foram selecionadas " + IntToStr(lisMREDE_SEL->Count) + " redes",
			"Confirma a seleção ?") != IDYES)
			return (false);
	}
	// lê todos os Cortes da MRede selecionadas
	if (!bd->Eqpto->LeListaCorte(estudo, lisMREDE_SEL, NULL))
		return (false);
	// lê todos os Cortes da MRede vizinhas
	if (!bd->Eqpto->LeListaCorte(estudo, lisMREDE_VIZ, NULL))
		return (false);
	// cada MRede vizinha deve permancer apenas com um Corte
	PreparaMRedeVizinha();
	// elimina todas MRedes e Redes que não estão em lisMREDE_SEL e lisMREDE_VIZ
	DestroiMRedeNaoSelNaoViz();
	// carrega as MRedes selecionadas e as Reducoes das MRede vizinhas
	bd->Eqpto->CarregaMRedeEstudo(estudo, lisMREDE_SEL, lisMREDE_VIZ);
   //carrega figuras
    bd->Figura->LeFiguras(estudo, false);

	return (true);
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
bool __fastcall TSelecao::CarregaRedeSelecionada(VTBD *bd, VTEstudo *estudo, TList *lisREDE_SEL,
	int aviso_num_rede)
{
	VTApl *aplBD;
	VTRedes *redesBD;

	// salva ponteiro p/ objetos
	this->bd = bd;
	this->estudo = estudo;
	// verifica se há redes selecionadas
	if (lisREDE_SEL->Count == 0)
		return (false);
	// proteção: verifica se há muitos objetos VTRede selecionados
	if ((aviso_num_rede > 0) && (lisREDE_SEL->Count > aviso_num_rede))
	{
		if (Confirma("Foram selecionadas " + IntToStr(lisREDE_SEL->Count) + " redes",
			"Confirma a seleção ?") != IDYES)
			return (false);
	}
	// inicia lista de MRedes selecionadas
	IniciaLisMRedeSelecionada(lisREDE_SEL);
	// inicia lista de MRedes vizinhas
	IniciaLisMRedeVizinha();
	// lê todos os Cortes da MRede selecionadas
	if (!bd->Eqpto->LeListaCorte(estudo, lisMREDE_SEL, NULL))
		return (false);
	// lê todos os Cortes da MRede vizinhas
	if (!bd->Eqpto->LeListaCorte(estudo, lisMREDE_VIZ, NULL))
		return (false);
	// elimina todas MRedes e Redes que não estão em lisMREDE_SEL e lisMREDE_VIZ
	DestroiMRedeNaoSelNaoViz();
	// cada MRede vizinha deve permancer apenas com um Corte
	PreparaMRedeVizinha();
	// elimina todas MRedes e Redes que não estão em lisMREDE_SEL e lisMREDE_VIZ
	DestroiMRedeNaoSelNaoViz();
	//antes de carregar a rede, impede a redefinicao de ids
	aplBD = bd->AplBD;
	redesBD = (VTRedes*)aplBD->GetObject(__classid(VTRedes));
	//bloqueia redefinicao de ids
    redesBD->AllowRedefineIds = false;
	// carrega as MRedes selecionadas e as Reducoes das MRede vizinhas
	bd->Eqpto->CarregaMRedeEstudo(estudo, lisMREDE_SEL, lisMREDE_VIZ);
   //carrega figuras
	bd->Figura->LeFiguras(estudo, true, lisREDE_SEL);
	//libera a redefinicao de ids e os redefine
	redesBD->AllowRedefineIds = true;
	redesBD->RedefineIds();

	return (true);
}

// ---------------------------------------------------------------------------
void __fastcall TSelecao::DestroiMRedeNaoSelNaoViz(void)
{
	// variáveis locais
	TList *lisCORTE;
	TList *lisREDE;
	VTCorte *corte;
	VTMRede *mrede;
	VTRede *rede;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	TList *lisMREDE = redes->LisMRede();

	// loop p/ todas MRedes
	for (int n = lisMREDE->Count - 1; n >= 0; n--)
	{
		mrede = (VTMRede*)lisMREDE->Items[n];
		// verifica se MRede está na lista de MRedes já carregadas
		if (lisMREDE_PRE->IndexOf(mrede) >= 0)
			continue;
		// verifica se a MRede é uma MRede selecionada
		if (lisMREDE_SEL->IndexOf(mrede) >= 0)
			continue;
		// verifica se a MRede é uma MRede vizinha
		if (lisMREDE_VIZ->IndexOf(mrede) >= 0)
			continue;

		// IMPORTANTE: antes de destruir a MRede é preciso
		// 1) Retirar suas Redes dos seus Cortes
		// 2) Destruir as Redes

		// obtém lista de Redes da MRede
		lisREDE = mrede->LisRede();
		// retira Redes dos Cortes da MRede
		lisCORTE = mrede->LisCorte();
		for (int nc = 0; nc < lisCORTE->Count; nc++)
		{
			corte = (VTCorte*)lisCORTE->Items[nc];
			corte->RemoveRede(lisREDE);
		}
		for (int nr = lisREDE->Count - 1; nr >= 0; nr--)
		{
			rede = (VTRede*)lisREDE->Items[nr];
			redes->RemoveRede(rede);
			delete rede;
		}
		// elimina a MRede
		lisMREDE->Delete(n);
		delete mrede;
	}
}

// ---------------------------------------------------------------------------
VTCorte* __fastcall TSelecao::ExisteCorteComum(VTMRede *mrede, TList *lisMREDE)
{
	// variáveis locais
	VTCorte *corte;
	TList *lisCORTE = mrede->LisCorte();

	// loop p/ todos Cortes
	for (int nc = 0; nc < lisCORTE->Count; nc++)
	{
		corte = (VTCorte*)lisCORTE->Items[nc];
		// verifica se alguma MRede de  lisMREDE conecta o Corte
		for (int n = 0; n < lisMREDE->Count; n++)
		{
			mrede = (VTMRede*)lisMREDE->Items[n];
			// verifica se a MRede conecta o Corte
			if (mrede->ExisteCorte(corte))
				return (corte);
		}
	}
	return (NULL);
}

// ---------------------------------------------------------------------------
void __fastcall TSelecao::IniciaLisMRedePreCarregadas(TList *lisREDE_PRE)
{
	// variáveis locais
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTRede *rede;

	lisMREDE_PRE->Clear();
	lisMREDE_REDUZIDAS->Clear();
	if (lisREDE_PRE == NULL)
	{
		CopiaTList(redes->LisMRede(), lisMREDE_PRE);
		RetiraLis1DeLis2(lisMREDE_SEL, lisMREDE_PRE);
	}
	else
	{
		// loop p/ todas Redes pre-carregadas
		for (int nr = 0; nr < lisREDE_PRE->Count; nr++)
		{
			rede = (VTRede*)lisREDE_PRE->Items[nr];
			// verifica se é uma MRede que pode ser carregada
			if (!rede->MRede->Carregada)
			{
				lisMREDE_REDUZIDAS->Add(rede->MRede);
//				continue;
			}
			// verifica se a MRede já foi incluída em lisMREDE_PRE
			if (lisMREDE_PRE->IndexOf(rede->MRede) < 0)
			{ // inclui MRede da Rede
				lisMREDE_PRE->Add(rede->MRede);
			}
		}
		RetiraLis1DeLis2(lisMREDE_SEL, lisMREDE_PRE);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TSelecao::IniciaLisMRedeSelecionada(TList *lisREDE_SEL)
{
	// variáveis locais
	VTRede *rede;

	// inicia lista de MRedes selecionadas
	lisMREDE_SEL->Clear();
	// loop p/ todas Redes selecionadas
	for (int nr = 0; nr < lisREDE_SEL->Count; nr++)
	{
		rede = (VTRede*)lisREDE_SEL->Items[nr];
		// verifica se é uma MRede que pode ser carregada
		if (!rede->MRede->Carregada)
			continue;
		// verifica se a MRede já foi incluída em lisMREDE_SEL
		if (lisMREDE_SEL->IndexOf(rede->MRede) < 0)
		{ // inclui MRede da Rede
			lisMREDE_SEL->Add(rede->MRede);
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TSelecao::IniciaLisMRedeVizinha(void)
{
	// variáveis locais
	struct
	{
		TList *lisCORTE;
		TList *lisCORTE_SEL;
		TList *lisMREDE_SEL;
		TList *lisMREDE_VIZ;
	} local;
	TList *lisREDE;
	VTCorte *corte;
	VTRede *rede_viz;
	VTMRede *mrede, *mrede_viz;

	// cria listas temporárias
	local.lisCORTE = new TList();
	local.lisCORTE_SEL = new TList();
	local.lisMREDE_SEL = new TList();
	local.lisMREDE_VIZ = new TList();
	// limpa lista de MRedes vizinhas
	lisMREDE_VIZ->Clear();
	// inicia local.lisMREDE_SEL com as MRedes de lisMREDE_SEL e com as MREdes carregadas de lisMREDE_PRE
	CopiaTList(lisMREDE_SEL, local.lisMREDE_SEL);
	for (int n = 0; n < lisMREDE_PRE->Count; n++)
	{
		mrede = (VTMRede*)lisMREDE_PRE->Items[n];
		if (mrede->Carregada)
		{ // insere MRede carregada em local.lisMREDE_SEL
			if (local.lisMREDE_SEL->IndexOf(mrede) < 0)
				local.lisMREDE_SEL->Add(mrede);
		}
	}
	// repita enquanto houver MRedes selecionadas em local.lisMREDE_SEL
	while (local.lisMREDE_SEL->Count > 0)
	{ // lê lista de Cortes das MRedes selecionadas
		local.lisCORTE_SEL->Clear();
		bd->Eqpto->LeListaCorte(estudo, local.lisMREDE_SEL, local.lisCORTE_SEL);
		// filtra Cortes já analisados
		for (int nc = local.lisCORTE_SEL->Count - 1; nc >= 0; nc--)
		{
			if (local.lisCORTE->IndexOf(local.lisCORTE_SEL->Items[nc]) >= 0)
			{
				local.lisCORTE_SEL->Delete(nc);
			}
		}
		if (local.lisCORTE_SEL->Count == 0)
			break;
		// inclui Cortes de local.lisCORTE_SEL em local.lisCORTE
		CopiaTList(local.lisCORTE_SEL, local.lisCORTE);
		// limpa local.lisMREDE_SEL
		local.lisMREDE_SEL->Clear();
		// lê lista de MRedes dos Cortes selecionados
		bd->Eqpto->LeListaMRede(estudo, local.lisCORTE_SEL);
		// loop p/ todos os Cortes selecionados
		for (int nc = 0; nc < local.lisCORTE_SEL->Count; nc++)
		{
			corte = (VTCorte*)local.lisCORTE_SEL->Items[nc];
			lisREDE = corte->LisRede();
			// monta lista de MRedes vizinhas a partir das Redes dos Cortes
			local.lisMREDE_VIZ->Clear();
			for (int nr = 0; nr < lisREDE->Count; nr++)
			{
				rede_viz = (VTRede*)lisREDE->Items[nr];
				mrede_viz = rede_viz->MRede;
				if (local.lisMREDE_VIZ->IndexOf(mrede_viz) < 0)
					local.lisMREDE_VIZ->Add(mrede_viz);
			}
			// loop p/ todas as MRede vizinhas
			for (int n = 0; n < local.lisMREDE_VIZ->Count; n++)
			{
				mrede_viz = (VTMRede*)local.lisMREDE_VIZ->Items[n];
				// verifica se a MRede vizinha já está selecionada em lisMREDE_SEL ou lisMREDE_PRE
				if (lisMREDE_SEL->IndexOf(mrede_viz) >= 0)
					continue;
				if (lisMREDE_PRE->IndexOf(mrede_viz) >= 0)
					continue;
				// verifica se a MRede vizinha é redutível
				if (mrede_viz->Redutivel)
				{ // insere MRede na lista de MRede vizinhas
					if (lisMREDE_VIZ->IndexOf(mrede_viz) < 0)
					{
						lisMREDE_VIZ->Add(mrede_viz);
					}
					else
					{ // MRede vizinha deve ser carregada: transfere MRede vizinha de lisMREDE_VIZ p/ lisMREDE_SEL
						lisMREDE_VIZ->Remove(mrede_viz);
						lisMREDE_SEL->Add(mrede_viz);
						// inclui MRede vizinha em local.lisMREDE_SEL
						local.lisMREDE_SEL->Add(mrede_viz);
					}
				}
				else
				{ // MRede precisa ser carregada pois não é redutível: insere em lisMREDE_SEL
					lisMREDE_SEL->Add(mrede_viz);
				}
			}
		}
	}
	// destrói listas temporárias sem destruir seus objetos
	delete local.lisCORTE;
	delete local.lisCORTE_SEL;
	delete local.lisMREDE_SEL;
	delete local.lisMREDE_VIZ;
}

// ---------------------------------------------------------------------------
void __fastcall TSelecao::LeListaMRedeVizinha(VTMRede *mrede, TList *lisMR)
{
	// variáveis locais
	VTMRede *mrede_viz;
	VTRede *rede, *rede_viz;
	TList *lisREDE = mrede->LisRede();

	// reinicia lisMR
	lisMR->Clear();
	for (int n = 0; n < lisREDE->Count; n++)
	{
		rede = (VTRede*)lisREDE->Items[n];
		// obtém lista de Redes vizinhas
		lisREDE_VIZ->Clear();
		bd->Eqpto->LeListaRedeVizinha(estudo, rede, lisREDE_VIZ);
		// loop p/ todas Redes vizinahs
		for (int i = 0; i < lisREDE_VIZ->Count; i++)
		{
			rede_viz = (VTRede*)lisREDE_VIZ->Items[i];
			if ((mrede_viz = rede_viz->MRede) == NULL)
				continue;
			// insere MRede da Rede vizinha em lisMR
			if (lisMR->IndexOf(mrede_viz) < 0)
				lisMR->Add(mrede_viz);
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TSelecao::LeListaRedesExistentes(VTBD *bd, VTEstudo *estudo)
{
	// variáveis locais
	VTDominio *dominio;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	TList *lisDOMINIO = redes->LisDominio();

	// lê lista de Dominios
	bd->Eqpto->LeListaDominio();
	for (int n = 0; n < lisDOMINIO->Count; n++)
	{
		dominio = (VTDominio*)lisDOMINIO->Items[n];
		bd->Eqpto->LeListaRede(estudo, dominio);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TSelecao::PreparaMRedeVizinha(void)
{
	// variáveis locais
	TList *lisCORTE;
	VTCorte *corte;
	VTCorte *corte_viz;
	VTMRede *mrede_viz;
	struct
	{
		TList *lisMREDE;
	} local;

	// cria lista local e inicia com as MRedes de lisMREDE_SEL e de lisMREDE_PRE
	local.lisMREDE = new TList();
	CopiaTList(lisMREDE_SEL, local.lisMREDE);
	CopiaTList(lisMREDE_PRE, local.lisMREDE);
	// loop p/ todas as MRede vizinhas
	for (int n = 0; n < lisMREDE_VIZ->Count; n++)
	{
		mrede_viz = (VTMRede*)lisMREDE_VIZ->Items[n];
		// verifica se a MRede vizinha possui apenas um Corte
		if (mrede_viz->NumeroDeCortes() == 1)
			continue;
		// determina Corte da MRede vizinha que conecta uma MRede selecionada
		if ((corte_viz = ExisteCorteComum(mrede_viz, local.lisMREDE)) != NULL)
		{ // elimina os demais Cortes da MRede vizinha
			lisCORTE = mrede_viz->LisCorte();
			for (int nc = lisCORTE->Count - 1; nc >= 0; nc--)
			{
				corte = (VTCorte*)lisCORTE->Items[nc];
				if (corte == corte_viz)
					continue;
				// elimina Corte
				RetiraCorte(corte, lisMREDE_VIZ);
			}
		}
	}
	// destrói lista local sem destruir seus objetos
	delete local.lisMREDE;
}

/*
 // ---------------------------------------------------------------------------
 void __fastcall TSelecao::PreparaMRedeVizinha(void)
 {
 // variáveis locais
 TList *lisCORTE;
 VTCorte *corte;
 VTCorte *corte_viz;
 VTCorte *corte_pre;
 VTMRede *mrede_viz;

 // loop p/ todas as MRede vizinhas
 for (int n = lisMREDE_VIZ->Count - 1; n >= 0 ; n--)
 {
 mrede_viz = (VTMRede*)lisMREDE_VIZ->Items[n];
 // verifica se a MRede vizinha possui apenas um Corte
 if (mrede_viz->NumeroDeCortes() == 1)
 continue;
 // determina Corte da MRede vizinha que conecta uma MRede selecionada
 if ((corte_viz = ExisteCorteComum(mrede_viz, lisMREDE_SEL)) == NULL)
 continue;
 // verifica se há Cortes entre MRede vizinha e MRedes pré-carregadas
 if ((corte_pre = ExisteCorteComum(mrede_viz, lisMREDE_PRE)) != NULL)
 { // insere MRede vizinha na lista de MRedes selecionadas para carregar
 if (lisMREDE_SEL->IndexOf(mrede_viz) < 0)
 lisMREDE_SEL->Add(mrede_viz);
 //retira MRede vizinha da lista de vizinhas
 lisMREDE_VIZ->Delete(n);
 }
 else
 {
 // elimina os demais Cortes da MRede vizinha
 lisCORTE = mrede_viz->LisCorte();
 for (int nc = lisCORTE->Count - 1; nc >= 0; nc--)
 {
 corte = (VTCorte*)lisCORTE->Items[nc];
 if (corte == corte_viz)
 continue;
 // elimina Corte
 RetiraCorte(corte, lisMREDE_VIZ);
 }
 }

 }
 }
 */
// ---------------------------------------------------------------------------
void __fastcall TSelecao::RetiraCorte(VTCorte *corte, TList *lisMREDE)
{
	// variáveis locais
	TList *lisREDE;
	VTRede *rede;
	VTMRede *mrede;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// loop p/ todas MRedes em lisMREDE
	for (int n = 0; n < lisMREDE->Count; n++)
	{
		mrede = (VTMRede*)lisMREDE->Items[n];
		if (!mrede->ExisteCorte(corte))
			continue;
		// remove Corte da MRede
		mrede->RemoveCorte(corte);
		// remove Corte das Redes da MRede
		lisREDE = mrede->LisRede();
		for (int nr = 0; nr < lisREDE->Count; nr++)
		{
			rede = (VTRede*)lisREDE->Items[nr];
			rede->RemoveCorte(corte);
		}
	}
	// destrói o Corte
	(redes->LisCorte())->Remove(corte);
	delete corte;
}

// ---------------------------------------------------------------------------
// eof
