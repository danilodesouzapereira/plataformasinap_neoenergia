// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DLL_Inc\Funcao.h>
#include "VTFigura.h"
// provisorio
#include ".\Estilos\VTEstilos.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
__fastcall VTFigura::VTFigura(void)
{
	// cria listas
	lisValorFormaAtributo = new TList();
	estudoId = FalsoId();
	visible = true;
	obra = NULL;
	alternativa = NULL;
	estilo = NULL;
//	estilos = NULL;
}

// ---------------------------------------------------------------------------
__fastcall VTFigura::~VTFigura(void)
{

    if (lisValorFormaAtributo)
	{
		LimpaLisValorFormaAtributo();
		delete lisValorFormaAtributo;
		lisValorFormaAtributo = NULL;
	}
}
// ---------------------------------------------------------------------------
strVALOR_FORMA_ATRIBUTO* __fastcall VTFigura::InsereValorFormaAtributo(int formaAtributoId, AnsiString valor)
{
	strVALOR_FORMA_ATRIBUTO *str = NULL;

	try
	{
		str = InsereValorFormaAtributo();
		str->formaAtributoId = formaAtributoId;
		str->valor = valor;
		str->figuraId = this->Id;
	}
	catch(...)
	{
		if (str)
		{
			delete str;
			str = NULL;
		}
	}

	return str;
}
// ---------------------------------------------------------------------------
strVALOR_FORMA_ATRIBUTO* __fastcall VTFigura::InsereValorFormaAtributo(void)
{
	strVALOR_FORMA_ATRIBUTO *str = NULL;

	try
	{
		str = new strVALOR_FORMA_ATRIBUTO;
		str->id = FalsoId();
		lisValorFormaAtributo->Add(str);
	}
	catch(...)
	{
		if (str)
		{
			delete str;
			str = NULL;
		}
	}

	return str;
}
// ---------------------------------------------------------------------------
void __fastcall  VTFigura::LimpaLisValorFormaAtributo(void)
{
    strAUXILIAR_FIGURA *aux;

	// destrói lista e seus objetos
	if (lisValorFormaAtributo)
	{
		for (int na = (lisValorFormaAtributo->Count - 1); na >= 0; na--)
		{
			aux = (strAUXILIAR_FIGURA*)lisValorFormaAtributo->Items[na];
			delete aux;
			lisValorFormaAtributo->Delete(na);
		}
	}
}
// ---------------------------------------------------------------------------
bool __fastcall VTFigura::PM_Visible(void)
{
	bool visivel;

	visivel = (visible && !this->Status[sttEV_EXCLUIDO]);

	return visivel;
}
// ---------------------------------------------------------------------------
// eof
