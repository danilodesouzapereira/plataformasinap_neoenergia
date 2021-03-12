// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TNode.h"
#include "TNodeDetalhe.h"
#include "..\Obra\ItemObra\VTItemObra.h"
#include "..\Obra\VTAcao.h"
#include "..\Rede\VTEqpto.h"
#include "..\..\DLL_Inc\Funcao.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
VTNode* __fastcall NewObjNodeDetalhe(void)
{
	try
	{
		return (new TNodeDetalhe());
	}
	catch (Exception &e)
	{
		return (NULL);
	}
}

// ---------------------------------------------------------------------------
__fastcall TNodeDetalhe::TNodeDetalhe(void)
{
	// cria listas
	lisAcao = new TList();
	habilitado = false;
	expandido = false;
	itemObra = NULL;
	estado = eND_INVALIDO;
	tipoEqpto = eqptoINDEF;
	tipoAcao = acaoINDEFINIDA;
}

// ---------------------------------------------------------------------------
__fastcall TNodeDetalhe::~TNodeDetalhe(void)
{
	// destrói lista sem destruir seus objetos
	if (lisAcao)
	{
		delete lisAcao;
		lisAcao = NULL;
	}
}
////---------------------------------------------------------------------------
// int __fastcall TNodeDetalhe::PM_GetEstado(void)
// {
// //verifica esta habilitado
// }

// ---------------------------------------------------------------------------
// eof
