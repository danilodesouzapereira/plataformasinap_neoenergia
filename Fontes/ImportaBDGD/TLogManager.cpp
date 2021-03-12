// --------------------------------------------------------------------------------------------- bof
#pragma hdrstop
#pragma package(smart_init)

// ---------------------------------------------------------------------------------------- includes

// DLL include
#include <DLL_Inc\Funcao.h>

// Plataforma include
#include <Fontes\Apl\VTApl.h>

// Project include
#include <TLogManager.h>
#include <TLogThread.h>

// --------------------------------------------------------------------------------------- namespace
using namespace std;

// -------------------------------------------------------------------------------------------------
__fastcall TLogManager::TLogManager(VTApl *apl)
{

	this->apl = apl;
	lisLogs = new TList;
}

// -------------------------------------------------------------------------------------------------
__fastcall TLogManager::~TLogManager(void)
{
	// destroi e limpa a lista
	if (lisLogs)
	{
		LimpaTList(lisLogs);
		delete lisLogs;
		lisLogs = NULL;
	}
}

// -------------------------------------------------------------------------------------------------
TLogThread* __fastcall TLogManager::NewLogThread(AnsiString nome_classe, AnsiString dir)
{
	// variaveis locais
	TLogThread *log_thread_nova, *log_thread;
	AnsiString nomelimpo = StringReplace(nome_classe, "::", "_", TReplaceFlags() << rfReplaceAll);

	if (lisLogs)
	{
		// verifica se ja existe um log com mesmo nome
		if ((log_thread = LogThread(nomelimpo)) == NULL)
		{
			log_thread_nova = new TLogThread(apl, nome_classe, dir);
			lisLogs->Add(log_thread_nova);
			return log_thread_nova;
		}
		else
		{
			return log_thread;
		}
	}
	return (NULL);
}

// -------------------------------------------------------------------------------------------------
TLogThread* __fastcall TLogManager::LogThread(AnsiString nome_classe)
{
	// variaveis locais
	TLogThread *log_thread;
	AnsiString nomelimpo = StringReplace(nome_classe, "::", "_", TReplaceFlags() << rfReplaceAll);

	for (int nl = 0; nl < lisLogs->Count; nl++)
	{
		log_thread = (TLogThread*)lisLogs->Items[nl];
		// compara os nomes
		if (log_thread->Codigo.AnsiCompare(nomelimpo) == 0)
		{
			return log_thread;
		}
	}
	return (NULL);
}

// --------------------------------------------------------------------------------------------- eof
