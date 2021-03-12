// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TEdtSwitch.h"
#include "..\Utils\TVerificaMalhaEdicao.h"
#include "..\..\Apl\VTApl.h"
#include "..\Busca\VTBusca.h"
#include "..\Constante\Cursor.h"
#include "..\Edita\VTEdita.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Rede\VTArea.h"
#include "..\Rede\VTChave.h"
#include "..\..\..\DLL_Inc\Funcao.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
VTEdt* NewObjEdtSwitch(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
{
	return (new TEdtSwitch(apl, ResetFocus, SetFocus, OnDone));
}

// ---------------------------------------------------------------------------
__fastcall TEdtSwitch::TEdtSwitch(VTApl *apl, TAction *ResetFocus, TAction *SetFocus,
	TAction *OnDone) : TEdt(apl, TCursor(curManobra), ResetFocus, SetFocus, OnDone)
{
	// inicia ponteiro p/ Chave
	pchave = NULL;
	this->apl = apl;
}

// ---------------------------------------------------------------------------
__fastcall TEdtSwitch::~TEdtSwitch(void)
{
	// nada a fazer
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/16233879
void __fastcall TEdtSwitch::EvtMouseDown(void)
{
	TVerificaMalhaEdicao *utilMalha; // = new TVerificaMalhaEdicao(apl);
	bool malhaIntAntes, malhaIntDepois, malhaExterna;

	try
	{ // proteção: verifica estado
		if (pchave == NULL)
			return;

		utilMalha = new TVerificaMalhaEdicao(apl);
		// verifica se possui malha interna previa
		malhaIntAntes = utilMalha->ExisteAlertaMalha(pchave->rede);
		// muda estado da Chave
		if (edita)
			edita->Manobra(pchave);
		// verifica se possui malha interna previa
		malhaIntDepois = utilMalha->ExisteAlertaMalha(pchave->rede);
		// verifica se possui malha externa
		malhaExterna = utilMalha->VerificaSeFechaMalhaExterna(pchave);
		// confirma acao, em caso negativo desfaz
		if ((!malhaIntAntes && malhaIntDepois) || (malhaExterna))
		{
			if (Confirma("Esta operação fechou malha.", "Deseja confirmar ?") != IDYES)
			{
				edita->ActionUndo->Execute();
			}
		}
		// verifica se foi definida Action externa
		if (extAction.OnDone)
		{ // executa TAction indicada
			extAction.OnDone->OnExecute(pchave);
		}
		/*
		 else
		 {//atualiza gráfico
		 graf->Redraw();
		 }
		 */
		// destro utilitario
		if (utilMalha)
		{
			delete utilMalha;
			utilMalha = NULL;
		}
	}
	catch (...)
	{ // destroi utilitario
		if (utilMalha)
		{
			delete utilMalha;
			utilMalha = NULL;
		}

	}
}

// ---------------------------------------------------------------------------
void __fastcall TEdtSwitch::EvtMouseMove(void)
{
	// variáveis locais
	TList *lisCHV;

	// procura Chave na posicao (x,y)
	area->x1 = area->x2 = evt.x;
	area->y1 = area->y2 = evt.y;
	lisCHV = busca->Chave(area);
	if (lisCHV->Count == 0)
	{ // não encontrou nenhuma Chave: muda formato do mouse
		pchave = NULL;
		graf->Cursor(TCursor(curManobra));
		return;
	}
	// seleciona primeira Chave da lista
	pchave = (VTChave*)lisCHV->First();
	// atualiza formato do cursor
	graf->Cursor((pchave->Aberta) ? TCursor(curFechaChv) : TCursor(curAbreChv));
}

// ---------------------------------------------------------------------------
// eof
