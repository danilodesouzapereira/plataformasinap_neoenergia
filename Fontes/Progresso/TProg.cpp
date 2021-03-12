// --------------------------------------------------------------------------------------------- bof
#include <vcl.h>
#pragma hdrstop

// ---------------------------------------------------------------------------------------- includes
#include "TProg.h"
#include "VTProgresso.h"
#include "TFormDefault.h"
#include "TFormTexto.h"

// --------------------------------------------------------------------------------------- namespace

// ------------------------------------------------------------------------------------------- class

// -------------------------------------------------------------------------------------------------
__fastcall TProg::TProg(int tipo_progresso) : TThread(true)
{
	// configura opções
	FreeOnTerminate = true;
	Priority = tpHighest;
	// cria objeto Event para bloquear processamento
	event = new TEvent(NULL, false /* ManualReset */ , false /* InitialState */ , "",
		false /* UseCOMWait */);
	// cria e exibe tela de progresso
	switch (tipo_progresso)
	{
	case progTEXTO:
		if (FormTexto == NULL)
			FormTexto = new TFormTexto(Application);
		if (FormTexto != NULL)
			FormTexto->Show();
		break;
	default:
		if (FormDefault == NULL)
			FormDefault = new TFormDefault(Application);
		if (FormDefault != NULL)
			FormDefault->Show();
		break;
	}
}

// -------------------------------------------------------------------------------------------------
__fastcall TProg::~TProg(void)
{
	// destrói objetos
	if (event)
	{
		delete event;
		event = NULL;
	}
	// destrói os Forms
	if (FormTexto)
	{
		FormTexto->Hide();
		delete FormTexto;
		FormTexto = NULL;
	}
	if (FormDefault)
	{
		FormDefault->Hide();
		delete FormDefault;
		FormDefault = NULL;
	}
}

// -------------------------------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/16082112
void __fastcall TProg::Execute(void)
{
	// atualiza tela de progresso
	while (!Terminated)
	{
//jss20191003
		if (FormTexto)
		{
			if (FormTexto->animacao.auto_inc)
				FormTexto->Step();
			FormTexto->Atualiza();
		}
		if (FormDefault)
			FormDefault->Atualiza();
		// bloqueia processamento por 1 seg
		event->WaitFor(1000);
		// reseta event
		event->ResetEvent();
		// Application->ProcessMessages();
	}
}

// -------------------------------------------------------------------------------------------------
void __fastcall TProg::Terminate(void)
{
	// destrói os Forms
	if (FormTexto)
	{
		FormTexto->Hide();
		delete FormTexto;
		FormTexto = NULL;
	}
	if (FormDefault)
	{
		FormDefault->Hide();
		delete FormDefault;
		FormDefault = NULL;
	}
	// finaliza a Thread
	TThread::Terminate();
}

// --------------------------------------------------------------------------------------------- eof
