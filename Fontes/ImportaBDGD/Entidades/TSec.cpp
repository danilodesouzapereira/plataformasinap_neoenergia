// --------------------------------------------------------------------------------------------- bof
#include <vcl.h>
#pragma hdrstop
#pragma package(smart_init)

// ---------------------------------------------------------------------------------------- includes

// Plataforma include
#include <Fontes\Rede\VTBarra.h>
#include <Fontes\Rede\VTRede.h>

// Project include
#include <TSec.h>

// --------------------------------------------------------------------------------------- namespace

// ------------------------------------------------------------------------------------------- class

// -------------------------------------------------------------------------------------------------
TSec* __fastcall NewObjSec(void)
{
	try
	{
		return (new TSec());
	}
	catch (Exception &e)
	{
	}
	// erro na criação do objeto
	return (NULL);
}

// -------------------------------------------------------------------------------------------------
__fastcall TSec::TSec(void)
{
	// cria lista
	lisBARRA = new TList();
	vnom_v = 0;
	carga_eqv = NULL;
}

// -------------------------------------------------------------------------------------------------
__fastcall TSec::~TSec(void)
{
	// destroi somente lista
	if (lisBARRA)
	{
		delete lisBARRA;
		lisBARRA = NULL;
	}
}

// ---------------------------------------------------------------------------
VTBarra* __fastcall TSec::ExisteBarra(int barra_id)
{
	// variáveis locais
	VTBarra *barra;

	// procura Barra em lisBAR
	for (int n = 0; n < lisBARRA->Count; n++)
	{
		barra = (VTBarra*)lisBARRA->Items[n];
		if (barra->Id == barra_id)
			return (barra);
	}
	return (NULL);
}

// -------------------------------------------------------------------------------------------------
bool __fastcall TSec::InsereBarra(VTBarra* barra)
{
	// proteção
	if (barra == NULL)
		return (false);
	// verifica se a barra ja existe na lista
	if(lisBARRA->IndexOf(barra) < 0)
	{
		lisBARRA->Add(barra);
		return(true);
	}
	return(false);
}

// --------------------------------------------------------------------------------------------- eof
