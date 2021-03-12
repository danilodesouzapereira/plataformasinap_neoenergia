// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TAbre.h"
#include "TConverteBase.h"
#include "VTSelecao.h"
#include "Esquematico\VTEsquematico.h"
#include "..\Apl\VTApl.h"
#include "..\Arranjo\VTArranjos.h"
#include "..\BD\VTBD.h"
#include "..\BD\Figura\VTBDFigura.h"
#include "..\BD\Rede\VTBDeqpto.h"
#include "..\BD\Rede\VTBDgeral.h"
#include "..\Busca\VTBusca.h"
#include "..\Classes\VTClasses.h"
#include "..\Curvas\VTCurvas.h"
#include "..\Diretorio\VTPath.h"
#include "..\Geral\VTGeral.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Manobra\VTManobras.h"
#include "..\Mercado\VTMercado.h"
#include "..\Planejamento\VTEstudo.h"
#include "..\Planejamento\VTPlanejamento.h"
#include "..\Progresso\VTProgresso.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTTipos.h"
#include "..\Sinap\VTSinapChild.h"
#include "..\Zona\VTZonas.h"
#include "..\..\DLL_Inc\BD.h"
#include "..\..\DLL_Inc\Busca.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Grafico.h"
#include "..\..\DLL_Inc\Progresso.h"
#include "..\..\DLL_Inc\Rede.h"
#include "..\..\..\ModulosOpcionais\Fontes\ConfiabilidadeBatch\VTBatchConf.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
VTAbre* __fastcall NewObjAbre(TComponent *Owner, VTApl *apl_owner)
{
	return (new TAbre(Owner, apl_owner));
}

// ---------------------------------------------------------------------------
__fastcall TAbre::TAbre(TComponent *Owner, VTApl *apl_owner)
{
	// cria objeto Apl local
	apl = NewObjApl(apl_owner->Form, apl_owner);
	// cria objetos
	// apl->Add(DLL_NewObjRedes(apl));
	apl->Add(bd = DLL_NewObjBD(apl));
	apl->Add(selecao = NewObjSelecao(apl));
	apl->Add(esquematico = NewObjEsquematico(apl));
	// apl->Add(              stk->NewObjConsulta(apl));
	apl->Add(grafico = DLL_NewObjGraficoAbre(apl));
	apl->Add(DLL_NewObjBusca(apl));
	// cria lista de Redes selecionadas
	lisREDE_SEL = new TList();
	// inicia número de redes para gerar aviso ao usuário
	AvisoNumRede = 100;
	// habilita progresso
	Progresso = true;
}

// ---------------------------------------------------------------------------
__fastcall TAbre::~TAbre(void)
{
	// destrói objeto Apl
//	if(bd)
//	{
//		bd->Desconecta();
//    }
	if (apl)
	{
		delete apl;
		apl = NULL;
	}
	// destrói listas sem destruir seus objetos
	if (lisREDE_SEL)
	{
		delete lisREDE_SEL;
		lisREDE_SEL = NULL;
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TAbre::CarregaEstudoExistente(VTEstudo *estudo)
{
	// variáveis locais
	VTPlanejamento *planejamento = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// cancela Redes selecionadas
	SelecaoClear();
	// reinicia objeto Redes, inclusive o Domínio
	redes->Clear(true);
	// reinicia Planejamento, mantendo apenas este Estudo
	planejamento->Reinicia(estudo);
	if (!selecao->CarregaEstudo(bd, estudo))
		return (false);
	// marca que o Estudo não é novo e não possui alterações
	estudo->Novo = false;
	estudo->Alterado = false;
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TAbre::CarregaRedeAdicional(TList *lisREDE_PRE)
{
	// variáveis locais
	VTEstudo *estudo_zero;
	VTPlanejamento *planejamento = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));

	// verifica se há Redes selecionadas
	if (SelecaoNumeroRede() == 0)
		return (true);
	// verifica se existe o estudo zero
	if ((estudo_zero = planejamento->ExisteEstudo(estudoID_ZERO)) == NULL)
		return (false);
	// carrega MRedes das Redes selecionadas e Reducoes das MRedes vizinhas do Estudo zero
	if (!selecao->CarregaRedeAdicional(bd, estudo_zero, lisREDE_PRE, lisREDE_SEL, AvisoNumRede))
		return (false);
	// reinicia Planejamento, mantendo apenas o Estudo indicado
	// planejamento->Reinicia(estudo_alvo);

	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TAbre::CarregaRedeCompleta(void)
{
	// variáveis locais
	VTEstudo *estudo_zero;
	VTPlanejamento *planejamento = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));

	// verifica se existe conexão com a bases
	if (!bd->Conectado)
	{ // inicia conexão
		if (!IniciaConexao())
			return (false);
	}
	// verifica se existe o estudo zero
	if ((estudo_zero = planejamento->ExisteEstudo(estudoID_ZERO)) == NULL)
	{ // insere estudoZERO_ID
		estudo_zero = planejamento->InsereEstudoRedeOriginal();
	}
	// reinicia Planejamento apenas com Estudo zero
	planejamento->Reinicia(estudo_zero);
	// carrega todas as Redes
	return (selecao->CarregaRedeCompleta(bd, estudo_zero, AvisoNumRede));
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/14422280
bool __fastcall TAbre::CarregaRedeSelecionada(VTEstudo *estudo_alvo)
{
	// variáveis locais
	int estudo_alvo_id;
	VTEstudo *estudo_zero;
	VTPlanejamento *planejamento = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));

	// verifica se há Redes selecionadas
	if (SelecaoNumeroRede() == 0)
		return (true);
	// verifica se foi definido o Estudo que vai conter as redes carregadas
	if (estudo_alvo == NULL)
	{ // verifica se existe o estudo zero
		if ((estudo_zero = planejamento->ExisteEstudo(estudoID_ZERO)) == NULL)
		{ // insere estudoZERO_ID
			estudo_zero = planejamento->InsereEstudoRedeOriginal();
		}
		// define Estudo alvo igual ao Estudo zero
		estudo_alvo = estudo_zero;
	}
	// salva ID do estudo alvo
	estudo_alvo_id = estudo_alvo->Id;
	// redefine ID do Estudo alvo com ID do estudo zero (OBS: importante na leitura das redes de um estudo novo)
	if(estudo_alvo->Id < 0)
		estudo_alvo->Id = estudoID_ZERO;
	// IMPORTANTE: mantém apenas o Estudo alvo no Planejamento
	planejamento->Reinicia(estudo_alvo);
	// carrega MRedes das Redes selecionadas e Reducoes das MRedes vizinhas do Estudo
	if (!selecao->CarregaRedeSelecionada(bd, estudo_alvo, lisREDE_SEL, AvisoNumRede))
		return (false);
	// restaura ID do Estudo alvo
	estudo_alvo->Id = estudo_alvo_id;

	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TAbre::ConverteBase(void)
{
	// variáveis locais
	TConverteBase *converte;

	try
	{ // cria objeto que converte base
		converte = new TConverteBase(apl);
		if (!converte->Executa())
		{
			Aviso("Não foi possível converter a base de dados");
		}
		delete converte;
	}
	catch (Exception &e)
	{
		if (converte != NULL)
			delete converte;
		return false;
	}
	return true;
}

// ---------------------------------------------------------------------------
bool __fastcall TAbre::CriaEstudoComRedeSelecionada(int tipo_estudo)
{
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TAbre::IniciaConexao(TList *lisREDE_PRE)
{
	// variáveis locais
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
	VTBatchConf *batch = (VTBatchConf*)apl->GetObject(__classid(VTBatchConf));

	try
	{ // cancela conexão atual
		bd->Desconecta();
		// verifica a base de dados selecioanda
		switch (geral->BaseDado)
		{
		case SNPDBTYPE_ORACLE:
			bd->ConectaServidorRedeRemoto();
			break;
		case SNPDBTYPE_ACCESS:
			bd->ConectaServidorRedeLocal();
			break;
		default:
			return (false);
		}
		// verifica se a lista de redes pré-selecionadas existe/está vazia
		if ((lisREDE_PRE == NULL) || (lisREDE_PRE->Count == 0))
		{ // reinicia objetos básicos
			IniciaObjetosBasicos();
		}
		else
		{ // reinicia sem limpar o que já foi aberto
			IniciaRedesBD();
		}
		// verifica validade da Base
		if (!bd->Eqpto->ValidaVersaoBaseParaLeitura())
		{
			if(batch == NULL)
			{
				Aviso("A base de dados não pode ser lida");
			}
			else
			{
				batch->LogAdd("A base de dados não pode ser lida");
            }
			bd->Desconecta();
		}
	}
	catch (Exception &e)
	{
		return (false);
	}
	return (bd->Conectado);
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/16259599
//https://sinapsisenergia.teamwork.com/#tasks/16571159
//https://sinapsisenergia.teamwork.com/#tasks/16647405
void __fastcall TAbre::IniciaObjetosBasicos(void)
{
	// Variáveis locais
	VTArranjos *arranjos = (VTArranjos*)apl->GetObject(__classid(VTArranjos));
	VTClasses *classes = (VTClasses*)apl->GetObject(__classid(VTClasses));
	VTCurvas *curvas = (VTCurvas*)apl->GetObject(__classid(VTCurvas));
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
	VTManobras *manobras = (VTManobras*)apl->GetObject(__classid(VTManobras));
	VTMercado *mercado = (VTMercado*)apl->GetObject(__classid(VTMercado));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTTipos *tipos = (VTTipos*)apl->GetObject(__classid(VTTipos));
	VTPlanejamento *planejamento = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));
	VTZonas *zonas = (VTZonas*)apl->GetObject(__classid(VTZonas));

	// proteção
	if (!bd->Conectado)
		return;
	// cancela redes selecionadas
	SelecaoClear();
	// reinicia Esquematico
	esquematico->Clear();
	// reinicia objetos básicos que serão atualizados a partir da nova base
	zonas->Clear(redes);
	mercado->Reinicia();
	planejamento->Reinicia();
	manobras->Reinicia();
	redes->Clear();
	tipos->Clear();
	arranjos->Clear();
	curvas->Clear();
	classes->Clear();
	// reinicia objetos a partir da nova base de dados
	bd->Geral->LeGeral(geral);
	bd->Geral->LeTipos();
	bd->Eqpto->LeListaEstudo();
	bd->Eqpto->LeListaDominio();
	bd->Figura->LeAuxiliaresFiguras();
}

// ---------------------------------------------------------------------------
void __fastcall TAbre::IniciaRedesBD(void)
{
	// Variáveis locais
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

	// proteção
	if (!bd->Conectado)
		return;
	// cancela redes selecionadas
	SelecaoClear();
	// reinicia Esquematico
	esquematico->Clear();
	// reinicia objetos a partir da base de dados
	// bd->Geral->LeGeral(geral);
	// bd->Geral->LeTipos();
	bd->Eqpto->LeListaEstudo();
	bd->Eqpto->LeListaDominio();
}

// ---------------------------------------------------------------------------
VTApl*__fastcall TAbre::PM_GetApl(void)
{
	return (apl);
}

// ---------------------------------------------------------------------------
bool __fastcall TAbre::PM_GetProgresso(void)
{
	return (progresso != NULL);
}

// ---------------------------------------------------------------------------
void __fastcall TAbre::PM_SetProgresso(bool enabled)
{
	if (enabled)
	{
		if (progresso == NULL)
		{
			apl->Add(progresso = DLL_NewObjProgresso(NULL, NULL));
		}
	}
	else
	{ // destrói objeto Progresso
		if (progresso != NULL)
		{
			apl->Remove(progresso);
			delete progresso;
			progresso = NULL;
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TAbre::SelecaoClear(void)
{
	// reinicia listas
	lisREDE_SEL->Clear();
}

// ---------------------------------------------------------------------------
bool __fastcall TAbre::SelecaoExisteRede(VTRede *rede)
{
	return (lisREDE_SEL->IndexOf(rede) >= 0);
}

// ---------------------------------------------------------------------------
TList* __fastcall TAbre::SelecaoInsereRede(VTRede *rede)
{
	// insere Rede da lista de Redes selecionadas
	if (lisREDE_SEL->IndexOf(rede) < 0)
		lisREDE_SEL->Add(rede);
	return (lisREDE_SEL);
}

// ---------------------------------------------------------------------------
TList* __fastcall TAbre::SelecaoLisRede(void)
{
	return (lisREDE_SEL);
}

// ---------------------------------------------------------------------------
int __fastcall TAbre::SelecaoNumeroRede(void)
{
	return (lisREDE_SEL->Count);
}

// ---------------------------------------------------------------------------
TList* __fastcall TAbre::SelecaoRetiraRede(VTRede *rede)
{
	// retira Rede da lista de Redes selecionadas
	if (lisREDE_SEL->IndexOf(rede) >= 0)
	{
		lisREDE_SEL->Remove(rede);
	}
	return (lisREDE_SEL);
}

// ---------------------------------------------------------------------------
bool __fastcall TAbre::ValidaBaseRede(void)
{
	// variáveis locais
	AnsiString versao;

	if (!bd->Conectado)
	{
		// Aviso("Base não conectada");
		return false;
	}
	// access: versao atual; oracle: vazio
	versao = bd->Eqpto->VersaoBase();
	if ((versao.AnsiCompareIC(BASE_REV_ATUAL) == 0) || (versao.IsEmpty()))
		return true;
	// invalida
	if (versao.AnsiCompareIC("invalida") == 0)
		return false;
	// precisa converter
	if (Confirma
		("A base de dados é de versão anterior. Deseja criar uma nova base compatível com a versão atual ?",
		"Esse processo pode demorar, dependendo do volume de dados.") != idYes)
		return false;
	// converte
	return (ConverteBase());
}

// ---------------------------------------------------------------------------
// eof
