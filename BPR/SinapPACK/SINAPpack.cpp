// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>

#pragma comment(lib, "Diretorio.a")
#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Produto.a")
#pragma comment(lib, "Registro.a")

// ---------------------------------------------------------------------------
USEFORM("..\..\Fontes\SinapPACK\TFormPack.cpp", FormPack);
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	// verifica se o S.O. passou algum parametro
	int argCount = 0;
	LPWSTR *args = CommandLineToArgvW(GetCommandLine(), &argCount);
	if (argCount == 1)
	{
		// try
		// {
		// Application->Initialize();
		// Application->MainFormOnTaskBar = true;
		// Application->CreateForm(__classid(TFormPack), &FormPack);
		// Application->Run();
		// }
		// catch (Exception &exception)
		// {
		// Application->ShowException(&exception);
		// }
		// catch (...)
		// {
		// try
		// {
		// throw Exception("");
		// }
		// catch (Exception &exception)
		// {
		// Application->ShowException(&exception);
		// }
		// }
		return 0;
	}
	else if (argCount == 2) // (1) nome do .exe, (2) -batch
	{ // executa o SINAPpack no modo GUI
		AnsiString param = AnsiString(args[1]);

		try
		{
			// verifica se passou o parametro -pack
			if (param.UpperCase().Pos("-PACK") != 1)
				return 99; // ERROR CODE: 99 - parametro 1 diferente de -pack
			Application->Initialize();
			Application->MainFormOnTaskBar = true;
			Application->CreateForm(__classid(TFormPack), &FormPack);
		Application->Run();
		}
		catch (Exception &exception)
		{
			Application->ShowException(&exception);
			return 98; // ERROR CODE 98: erro inesperado na execucao de processo batch
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
				return 97; // ERROR CODE 97: erro inesperado na execucao
			}
		}
		return 1;
	}
	else
	{
		return 96; // ERROR CODE 96: pior caso
    }
}
// ---------------------------------------------------------------------------
