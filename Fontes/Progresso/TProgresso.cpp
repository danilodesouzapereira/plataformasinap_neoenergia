// --------------------------------------------------------------------------------------------- bof
#include <vcl.h>
#pragma hdrstop

// ---------------------------------------------------------------------------------------- includes
#include "TProgresso.h"
#include "TProg.h"
#include "TFormTexto.h"

// --------------------------------------------------------------------------------------- namespace

// ------------------------------------------------------------------------------------------- class
VTProgresso* __fastcall NewObjProgresso(VTApl *apl)
{
	return (new TProgresso());
}

// -------------------------------------------------------------------------------------------------
__fastcall TProgresso::TProgresso(void)
{
	// nada a fazer
}

// -------------------------------------------------------------------------------------------------
__fastcall TProgresso::~TProgresso(void)
{
	// NÃO destrói objeto Prog
	// if (prog)        {delete prog;        prog        = NULL;}
}

// -------------------------------------------------------------------------------------------------
void __fastcall TProgresso::Add(AnsiString texto, bool clear)
{
	if (prog)
	{
		if (prog->FormTexto)
			prog->FormTexto->Add(texto, clear);
	}
}

//-----------------------------------------------------------------------------
void __fastcall TProgresso::Add(AnsiString texto, int pos, int max, bool clear)
   {
   /*
   //verifica se deve limpar a tela
   if (clear) EnviaMensagem("@clear@");
   //envia mensagem
   EnviaMensagem(texto);
   //envia mensagem para para atualizar barra de progresso
   texto.sprintf("@step@;%d;%d;", pos, max);
   //envia mensagem
   EnviaMensagem(texto);
   */
   Add(texto, clear);
   }

//---------------------------------------------------------------------------
int  __fastcall TProgresso::PM_GetServerPort(void)
   {
   return(server_port);
   }

//---------------------------------------------------------------------------
void __fastcall TProgresso::PM_SetServerPort(int port)
   {
   /*
   //verifica se já está conectado
   if (Conectado())
      {//verifica se port é igual ao atual
      if (port == server_port) return;
      //desconecta do port atual
      Desconecta();
      }
   */
   //salva port
   server_port = port;
   }

// -------------------------------------------------------------------------------------------------
void __fastcall TProgresso::Start(int tipo_progresso)
{
	// proteção: destrói thread e Forms criados anteriormente
	Stop();
	// cria e inicia Thread
	prog = new TProg(tipo_progresso);
	prog->Start();
}

// -------------------------------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/16082112
void __fastcall TProgresso::Stop(void)
{
	// finaliza thread
	if (prog)
	{ // NÃO destrói Prog: thread criada com opção de FreeOnTerminate
//jss20191003
#ifdef _DEBUG
		int passo_atual;
		if (prog->FormTexto)
        {
			passo_atual = prog->FormTexto->animacao.step;
            ++passo_atual;
        }
#endif
		prog->Terminate();
		prog = NULL;
		// Sleep(700);
	}
}

//jss20191003
// -------------------------------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/16082112
void __fastcall TProgresso::NoAutoInc(void)
{
	if (prog)
	{
		if (prog->FormTexto)
			prog->FormTexto->NoAutoInc();
	}
}

// -------------------------------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/16082112
bool __fastcall TProgresso::MaxStep(int max)
{
    bool reservado = false;
	if (prog)
		if (prog->FormTexto)
			reservado = prog->FormTexto->MaxStep(max);
    return (reservado);
}

// -------------------------------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/16082112
void __fastcall TProgresso::Step(void)
{
	if (prog)
	{
		if (prog->FormTexto)
			prog->FormTexto->Step();
	}
}

// --------------------------------------------------------------------------------------------- eof
