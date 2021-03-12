// ---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include<tchar.h>
#include <DLL_Inc\BatchProcess.h>
#include "..\..\Fontes\Sinap\TFormMDI.h"
#include <iostream>
//#include <iostream.h>
#include <conio.h>

#pragma comment(lib, "AjusteDemanda.a")
#pragma comment(lib, "AlocaCapacitor.a")
#pragma comment(lib, "AlocaRegulador.a")
#pragma comment(lib, "Arranjo.a")
#pragma comment(lib, "BD.a")
#pragma comment(lib, "BDihm.a")
#pragma comment(lib, "BDlicenca.a")
#pragma comment(lib, "Balanceamento.a")
#pragma comment(lib, "BatchProcess.a")
#pragma comment(lib, "Bloco.a")
#pragma comment(lib, "Busca.a")
#pragma comment(lib, "CalcIndCont.a")
#pragma comment(lib, "Cfg.a")
#pragma comment(lib, "Classes.a")
#pragma comment(lib, "Consulta.a")
#pragma comment(lib, "Cronometro.a")
#pragma comment(lib, "Curto.a")
#pragma comment(lib, "CurtoCircuito.a")
#pragma comment(lib, "Curvas.a")
#pragma comment(lib, "CustoNodal.a")
#pragma comment(lib, "Debug.a")
#pragma comment(lib, "DensidadeCarga.a")
#pragma comment(lib, "Diagnostico.a")
#pragma comment(lib, "Diretorio.a")
#pragma comment(lib, "Distancia.a")
#pragma comment(lib, "Edita.a")
#pragma comment(lib, "Editor.a")
#pragma comment(lib, "Equivalente.a")
#pragma comment(lib, "Eqv1.a")
#pragma comment(lib, "Eqv3.a")
#pragma comment(lib, "Especial.a")
#pragma comment(lib, "Esquematico.a")
#pragma comment(lib, "Estudo.a")
#pragma comment(lib, "Explorer.a")
#pragma comment(lib, "Fasometro.a")
#pragma comment(lib, "Flow.a")
#pragma comment(lib, "FluxoCC.a")
#pragma comment(lib, "FluxoG1.a")
#pragma comment(lib, "FluxoG3.a")
#pragma comment(lib, "FluxoH3.a")
#pragma comment(lib, "FluxoNR1.a")
#pragma comment(lib, "FluxoNR3.a")
#pragma comment(lib, "FluxoRadial1.a")
#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Gauss1.a")
#pragma comment(lib, "Gauss3.a")
#pragma comment(lib, "Geral.a")
#pragma comment(lib, "GestaoLicenca.a")
#pragma comment(lib, "Grafico.a")
#pragma comment(lib, "Help.a")
#pragma comment(lib, "Historico.a")
#pragma comment(lib, "IhmCustoNodal.a")
#pragma comment(lib, "ImportaAnafas.a")
#pragma comment(lib, "ImportaAnarede.a")
#pragma comment(lib, "ImportaCurva.a")
#pragma comment(lib, "ImportaMedicao.a")
#pragma comment(lib, "ImportaBDGD.a")
#pragma comment(lib, "ImportaRedeGIS.a")
#pragma comment(lib, "InterfaceTXT.a")
#pragma comment(lib, "InterfaceWS.a")
#pragma comment(lib, "InterfaceXML.a")
#pragma comment(lib, "Legenda.a")
#pragma comment(lib, "Licenca.a")
#pragma comment(lib, "Localiza.a")
#pragma comment(lib, "Log.a")
#pragma comment(lib, "Login.a")
#pragma comment(lib, "Manobra.a")
#pragma comment(lib, "MapaTematico.a")
#pragma comment(lib, "Medidor.a")
#pragma comment(lib, "Mercado.a")
#pragma comment(lib, "Micro.a")
#pragma comment(lib, "Monopolar.a")
#pragma comment(lib, "MontaZ.a")
#pragma comment(lib, "NR1.a")
#pragma comment(lib, "NR3.a")
#pragma comment(lib, "Ordena.a")
#pragma comment(lib, "Patamar.a")
#pragma comment(lib, "Perdatec.a")
#pragma comment(lib, "Planejamento.a")
#pragma comment(lib, "Potencia.a")
#pragma comment(lib, "Produto.a")
#pragma comment(lib, "Progresso.a")
#pragma comment(lib, "Protecao.a")
#pragma comment(lib, "Protesw.a")
#pragma comment(lib, "Radial.a")
#pragma comment(lib, "Reclamacao.a")
#pragma comment(lib, "Rede.a")
#pragma comment(lib, "RedeCC.a")
#pragma comment(lib, "RedeFH.a")
#pragma comment(lib, "Redegraf.a")
#pragma comment(lib, "Redutor.a")
#pragma comment(lib, "Relatorio.a")
#pragma comment(lib, "Report.a")
#pragma comment(lib, "ResCC.a")
#pragma comment(lib, "Servidor.a")
#pragma comment(lib, "SinapApl.a")
#pragma comment(lib, "SinapToolKit.a")
#pragma comment(lib, "SisLin.a")
#pragma comment(lib, "Spy.a")
#pragma comment(lib, "Stk.a")
#pragma comment(lib, "Topologia.a")
#pragma comment(lib, "Wizard.a")
#pragma comment(lib, "Zona.a")


// ---------------------------------------------------------------------
using namespace std;

// ---------------------------------------------------------------------
USEFORM("..\..\Fontes\Sinap\TFormStk.cpp", FormStk);
USEFORM("..\..\Fontes\Sinap\TFormMDI.cpp", FormMDI);
USEFORM("..\..\Fontes\Sinap\TFormChild.cpp", FormChild);
USEFORM("..\..\Fontes\Sinap\TFormViewArranjo.cpp", FormViewArranjo);
USEFORM("..\..\Fontes\Sinap\TFormViewFase.cpp", FormViewFase);
USEFORM("..\..\Fontes\Forms\TFormEdtTxt.cpp", FormEdtTxt);
USEFORM("..\..\Fontes\MultiMonitor\TFormMultiMonitor.cpp", FormMultiMonitor);
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	int retorno = -1;
	// verifica se o S.O. passou algum parametro
	int argCount = 0;
	LPWSTR *args = CommandLineToArgvW(GetCommandLine(), &argCount);
	// executa o SINAPgrid no modo GUI
	if (argCount == 1)
	{

		try
		{
			Application->Initialize();
			Application->MainFormOnTaskBar = true;
			Application->CreateForm(__classid(TFormMDI), &FormMDI);
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
		return 0; //execucao em modo GUI OK

	}
	else if (argCount == 2) // (1) nome do .exe, (2) -batch
	{
		AnsiString pid = "";
		AnsiString param = AnsiString(args[1]);

		try
		{
			//
			// verifica os parametros
			//

			// verifica se passou o parametro -batch
			if (param.UpperCase().Pos("-BATCH") == 1)
			{
				//return 1; // ERROR CODE: 1 - parametro 1 diferente de -batch
                // verifica se o parametro estah no formato correto: -batch=<arquivo .ini>
				if ((param.Pos("=") == 0) || (param.UpperCase().Pos(".INI") == 0))
					return 2;
				// ERROR CODE: 2 - o parametro nao estah no formato correto: -batch=<arquivo .ini>

				// verifica se o arquivo .ini existe
				AnsiString iniFile = param.SubString((param.Pos("=") + 1), param.Length());
				if (!FileExists(iniFile, true))
					return 3; // ERROR CODE: 3 - o arquivo .ini nao existe

				// tudo OK, ativar e executar modo batch
				return DLL_BatchProcess_Execute(iniFile, pid);
			}
			else if (param.UpperCase().Pos("-CABAL") == 1)
			{
					 //return 1; // ERROR CODE: 1 - parametro 1 diferente de -batch
					 // verifica se o parametro estah no formato correto: -batch=<arquivo .ini>
				if ((param.Pos("=") == 0) || (param.UpperCase().Pos(".JSON") == 0))
					return 2;
				// ERROR CODE: 2 - o parametro nao estah no formato correto: -batch=<arquivo .ini>

				// verifica se o arquivo .ini existe
				AnsiString iniFile = param.SubString((param.Pos("=") + 1), param.Length());
				if (!FileExists(iniFile, true))
					return 3; // ERROR CODE: 3 - o arquivo .ini nao existe

				// tudo OK, ativar e executar modo batch
				return DLL_Cabal_Execute(iniFile);
			}
			else if (param.UpperCase().Pos("-DMS") == 1) // Inicialização do DMS
			{
				try
				{
					Application->Initialize();
					Application->MainFormOnTaskBar = true;
					Application->CreateForm(__classid(TFormMDI), &FormMDI);
					FormMDI->InicializacaoDMS(10);
					Application->Run();
				}
				catch (Exception &exception){Application->ShowException(&exception);}
				catch (...)
				{
					try{throw Exception("");}catch (Exception &exception)
					{Application->ShowException(&exception);}
				}
			}
			else
			{
				return 1; // ERROR CODE: 1 - parametro 1 diferente de -batch
            }


		}
		catch (Exception &exception)
		{
			return 98; // ERROR CODE 98: erro inesperado na execucao de processo batch
		}

	}
	else if(argCount == 3)
	{
		AnsiString param = AnsiString(args[1]);
		try
		{
            // verifica se passou o parametro -batch
			if (param.UpperCase().Pos("-BATCH") != 1)
				return 1; // ERROR CODE: 1 - parametro 1 diferente de -batch

			if ((param.Pos("=") == 0) || (param.UpperCase().Pos(".INI") == 0))
					return 2;
				// verifica se o arquivo .ini existe
				AnsiString iniFile = param.SubString((param.Pos("=") + 1), param.Length());
				if (!FileExists(iniFile, true))
				return 3; // ERROR CODE: 3 - o arquivo .ini nao existe

				//Pega o PID passado como parametro inicial
					param = AnsiString(args[2]);
				if ((param.UpperCase().Pos("-PID") != 1))
					return 2;

				AnsiString pid = param.SubString((param.Pos("=") + 1), param.Length());

				//Ativa MODO BATCH
				return DLL_BatchProcess_Execute(iniFile, pid);
		}
		catch (Exception &exception)
		{
			return 98; // ERROR CODE 98: erro inesperado na execucao de processo batch
		}
    }
	else if (argCount > 3) // (1) nome do .exe, (2) -batch
	{
		AnsiString param = AnsiString(args[1]);
		try
		{
			// verifica se passou o parametro -CALC
			if (param.UpperCase().Pos("-CALC") != 1)
			return 1;
				// O processo a ser realizado é o FluxoBatch
				// verifica se o parametro estah no formato correto: -CALC=<arquivo .ini>
			param = AnsiString(args[2]);
			if ((param.UpperCase().Pos("TIPO=1") == 0))
				return 2;
			// ERROR CODE: 2 - o parametro nao estah no formato correto: tipo=
			else
			{	// tudo OK, ativar e executar modo batch
				TStringList *lisArg = new TStringList;
				//preenche o stringlist com os argumentos
				for (int na = 0; na < argCount; na++)
						{lisArg->Add(args[na]);}
				retorno = DLL_BatchCalc_Execute("FluxoBatch", lisArg);
				delete lisArg;
				return retorno;
			}
			}
		catch (Exception &exception)
		{
			return 98; // ERROR CODE 98: erro inesperado na execucao de processo batch

		}
	}
	return 99; // ERROR CODE 99: erro inesperado na execucao do SINAPgrid
}

// ---------------------------------------------------------------------
// eof
