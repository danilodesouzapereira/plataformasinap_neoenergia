// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TInsere.h"
#include "VTSelecao.h"
#include "..\Apl\VTApl.h"
#include "..\BD\VTBD.h"
#include "..\Geral\VTGeral.h"
#include "..\Planejamento\VTEstudo.h"
#include "..\Planejamento\VTPlanejamento.h"
#include "..\Rede\VTMRede.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTReducao.h"
#include "..\..\DLL_Inc\BD.h"
#include "..\..\DLL_Inc\Funcao.h"
#//---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
VTInsere* __fastcall NewObjInsere(VTApl *apl_owner, TAction *ActionInsRede)
{
	try
	{
		return (new TInsere(apl_owner, ActionInsRede));
	}
	catch (Exception &e)
	{
		return (NULL);
	}
}

// ---------------------------------------------------------------------------
__fastcall TInsere::TInsere(VTApl *apl_owner, TAction *ActionInsRede)
{
	// salva ponteiro p/ objetos
	this->apl_owner = apl_owner;
	this->ActionInsRede = ActionInsRede;
	// cria lista
	lisSEL = new TList();
}

// ---------------------------------------------------------------------------
__fastcall TInsere::~TInsere(void)
{
	// destrói lista sem destruir seus objetos
	if (lisSEL)
	{
		delete lisSEL;
		lisSEL = NULL;
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TInsere::Executa(AnsiString cod_rede)
{
	// variáveis locais
	bool sucesso = false;
	VTMRede *mrede;

	// determina MRede da Rede indicada
	if ((mrede = ExisteMRede(cod_rede)) != NULL)
	{ // insere Redes da MRede indicada
		sucesso = InsereRede(mrede);
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TInsere::Executa(VTReducao *reducao)
{
	// variáveis locais
	bool sucesso = false;
	VTMRede *mrede_owner;

	// proteção
	if (reducao == NULL)
		return (false);
	// salva ponteiro p/ MRede da Reducao
	mrede_owner = reducao->MRede;
	// verifica se a MRede já esta carregada
	if (mrede_owner->Carregada)
		return (false);
	// verifica se a MRede pode ser carregada
	if (!mrede_owner->Valida)
		return (false);
	// insere Redes da MRede indicada
	sucesso = InsereRede(mrede_owner);

	return (sucesso);
}

// ---------------------------------------------------------------------------
VTMRede* __fastcall TInsere::ExisteMRede(AnsiString cod_rede)
{
	// variáveis locais
	VTRede *rede;
	VTRedes *redes = (VTRedes*)apl_owner->GetObject(__classid(VTRedes));

	// verifica se existe a Rede inidcada
	if ((rede = redes->ExisteRede(cod_rede)) == NULL)
		return (NULL);
	return (rede->MRede);
}

// ---------------------------------------------------------------------------
VTMRede* __fastcall TInsere::ExisteMRede(int mrede_id)
{
	// variáveis locais
	VTRedes *redes = (VTRedes*)apl_owner->GetObject(__classid(VTRedes));

	// verifica se existe a MRede inidcada
	return (redes->ExisteMRede(mrede_id));
}

// ---------------------------------------------------------------------------
bool __fastcall TInsere::IniciaConexao(VTBD *bd)
{
	// variáveis locais
	VTGeral *geral = (VTGeral*)apl_owner->GetObject(__classid(VTGeral));

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
	}
	catch (Exception &e)
	{
		return (false);
	}
	return (bd->Conectado);
}

// ---------------------------------------------------------------------------
bool __fastcall TInsere::InsereRede(VTMRede *mrede)
{
	// variáveis locais
	int aviso_num_rede = 0;
	VTBD *bd_owner;
	VTEstudo *estudo;
	VTSelecao *selecao;
	VTPlanejamento *planejamento = (VTPlanejamento*)apl_owner->GetObject(__classid(VTPlanejamento));

	try
	{ // cria objetos BD e Selecao
		bd_owner = DLL_NewObjBD(apl_owner);
		selecao = NewObjSelecao(apl_owner);
		if ((bd_owner != NULL) && (selecao != NULL))
		{ // verifica se existe o estudo zero
			if ((estudo = planejamento->ExisteEstudo(estudoID_ZERO)) == NULL)
			{ // insere estudoZERO_ID
				estudo = planejamento->InsereEstudoRedeOriginal();
			}
			// inicia lisSEL com as Redes da MRede
			lisSEL->Clear();
			mrede->LisRede(lisSEL);
			mrede->Carregada = true;
			// inicia conexão com base de dados
			if (IniciaConexao(bd_owner))
			{
				bd_owner->ProgressoEnabled = true;
				// carrega MRede selecionada e suas MREdes vizinhas como equivalentes
				selecao->CarregaRedeAdicional(bd_owner, estudo, lisSEL, aviso_num_rede);
			}
			// destrói objetos BD e Selecao
			delete bd_owner;
			delete selecao;
			// executa Action externa, indicando alteração da Rede
			if (ActionInsRede != NULL)
				ActionInsRede->Execute();
		}
	}
	catch (Exception &e)
	{
		return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
// eof
