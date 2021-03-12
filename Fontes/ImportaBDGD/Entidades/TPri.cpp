// --------------------------------------------------------------------------------------------- bof
#include <vcl.h>
#pragma hdrstop
#pragma package(smart_init)

// ---------------------------------------------------------------------------------------- includes

// DLL include
#include <DLL_Inc\Funcao.h>
// Plataforma include
#include <Fontes\Rede\VTBarra.h>
#include <Fontes\Rede\VTRede.h>
#include <TSec.h>
#include <TPri.h>

// --------------------------------------------------------------------------------------- namespace

// ------------------------------------------------------------------------------------------- class

// -------------------------------------------------------------------------------------------------
TPri* __fastcall NewObjPri(void)
{
	try
	{
		return (new TPri());
	}
	catch (Exception &e)
	{
	}
	// erro na criação do objeto
	return (NULL);
}

// -------------------------------------------------------------------------------------------------
__fastcall TPri::TPri(void)
{
	// cria listas
	lisSEC = new TList();
	lisBAR = new TList();

}

// -------------------------------------------------------------------------------------------------
__fastcall TPri::~TPri(void)
{
	// destrói lista e seus objetos
    if (lisBAR)
	{
		delete lisBAR;
		lisBAR = NULL;
	}
	if (lisSEC)
	{
		LimpaTList(lisSEC);
		delete lisSEC;
		lisSEC = NULL;
	}
}

// -------------------------------------------------------------------------------------------------
TSec* __fastcall TPri::ExisteSec(int sec_id)
{
	// variáveis locais
	TSec *sec;
	for (int n = 0; n < lisSEC->Count; n++)
	{
		sec = (TSec*)lisSEC->Items[n];
		if (sec->id == sec_id)
			return (sec);
	}
	return (NULL);
}


// ---------------------------------------------------------------------------
VTBarra* __fastcall TPri::ExisteBarra(int barra_id)
{
	// variáveis locais
	VTBarra *barra;

	// procura Barra em lisBAR
	for (int n = 0; n < lisBAR->Count; n++)
	{
		barra = (VTBarra*)lisBAR->Items[n];
		if (barra->Id == barra_id)
			return (barra);
	}
	return (NULL);
}

// ---------------------------------------------------------------------------
bool __fastcall TPri::InsereBarra(VTBarra *barra)
{
	// proeção
	if (barra == NULL)
		return (false);
	// insere Barra em lisBAR
	lisBAR->Add(barra);
	return (true);
}

// -------------------------------------------------------------------------------------------------
bool __fastcall TPri::InsereSec(TSec *sec)
{
	// proteção
	if (sec == NULL)
		return (false);
	// insere Sec na lista
	if (lisSEC->IndexOf(sec) < 0)
		lisSEC->Add(sec);
	return (true);
}


// --------------------------------------------------------------------------------------------- eof
