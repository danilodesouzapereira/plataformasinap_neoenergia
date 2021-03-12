#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\Apl\VTApl.h>
#include <Fontes\GestaoLicenca\Forms\TFormAlocaLicenca.h>
#include <Fontes\GestaoLicenca\Forms\TFormDadosLicenca.h>
#include <Fontes\GestaoLicenca\Forms\TFormCFGProxy.h>
#include <Fontes\GestaoLicenca\BO\VTLicencaBO.h>
#include <DLL_Inc\GestaoLicenca.h>
#include <DLL_Inc\Funcao.h>
#pragma argsused

#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Licenca.a")
#pragma comment(lib, "Micro.a")
#pragma comment(lib, "Progresso.a")
#pragma comment(lib, "Registro.a")
#pragma comment(lib, "Servidor.a")

// -----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
	return 1;
}

// -----------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormAlocaLicenca(TComponent *Owner)
{
	// variáveis locais
	TForm *form;

	// destrói todos os TFormAlocaLicenca abertos
	while ((form = ExisteForm("TFormAlocaLicenca", Owner)) != NULL)
		delete form;
}

// -----------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormDadosLicenca(TComponent *Owner)
{
	// variáveis locais
	TForm *form;

	// destrói todos os TFormDadosLicenca abertos
	while ((form = ExisteForm("TFormDadosLicenca", Owner)) != NULL)
		delete form;
}
// -----------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormCFGProxy(TComponent *Owner)
{
	// variáveis locais
	TForm *form;

	// destrói todos os TFormDadosLicenca abertos
	while ((form = ExisteForm("TFormCFGProxy", Owner)) != NULL)
		delete form;
}

// -----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormAlocaLicenca(TComponent *Owner, VTApl *apl_owner)
{
	try
	{ // cria form TFormAlocaLicenca
		return (new TFormAlocaLicenca(Owner, apl_owner));
	}
	catch (System::Sysutils::Exception &e)
	{
		return (NULL);
	}
}

// -----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormDadosLicenca(TComponent *Owner, VTApl *apl_owner)
{
	try
	{ // cria form TFormDadosLicenca
		return (new TFormDadosLicenca(Owner, apl_owner));
	}
	catch (System::Sysutils::Exception &e)
	{
		return (NULL);
	}
}
// -----------------------------------------------------------------------------
EXPORT TFormCFGProxy* __fastcall DLL_NewFormCFGProxy(TComponent *Owner)
{
	try
	{ // cria form TFormDadosLicenca
		return (new TFormCFGProxy(Owner));
	}
	catch (System::Sysutils::Exception &e)
	{
		return (NULL);
	}
}
//-----------------------------------------------------------------------------
EXPORT VTLicencaBO*  __fastcall DLL_NewObjLicencaBO(VTApl *apl_owner)
   {
   return(NewObjLicencaBO(apl_owner));
   }

// ---------------------------------------------------------------------------
// eof
