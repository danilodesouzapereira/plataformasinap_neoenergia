//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <tchar.h>

#pragma comment(lib, "AjusteDemanda.a")
#pragma comment(lib, "AlocaCapacitor.a")
#pragma comment(lib, "AlocaRegulador.a")
#pragma comment(lib, "Arranjo.a")
#pragma comment(lib, "BDihm.a")
#pragma comment(lib, "Cfg.a")
#pragma comment(lib, "Editor.a")
#pragma comment(lib, "Estudo.a")
#pragma comment(lib, "Flow.a")
#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Help.a")
#pragma comment(lib, "ImportaRedeGIS.a")
#pragma comment(lib, "Produto.a")
#pragma comment(lib, "Reclamacao.a")
#pragma comment(lib, "Redutor.a")
#pragma comment(lib, "SinapApl.a")

//---------------------------------------------------------------------------
USEFORM("..\..\Fontes\SinapRNT\TFormLicenca.cpp", FormLicenca);
USEFORM("..\..\Fontes\SinapRNT\TFormLogin.cpp", FormLogin);
USEFORM("..\..\Fontes\SinapRNT\TFormMain.cpp", FormMain);
USEFORM("..\..\Fontes\Forms\TFormEdtTxt.cpp", FormEdtTxt);
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	try
	{
		Application->Initialize();
		Application->MainFormOnTaskBar = true;
		Application->CreateForm(__classid(TFormMain), &FormMain);
		Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	catch (...)
	{
		try
		{
			throw Exception("");
		}
		catch (Exception &exception)
		{
			Application->ShowException(&exception);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------
