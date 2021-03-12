// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TEdtAltCargaNL.h"
#include "TFormEdtCarNL.h"
#include "..\Constante\Cursor.h"
#include "..\Rede\VTArea.h"
#include "..\Busca\VTBusca.h"
#include "..\Grafico\VTGrafico.h"
/*
 #include "VTEdicao.h"
 #include "Marca\TMarca.h"
 #include "..\Apl\VTApl.h"

 #include "..\Curvas\VTCurva.h"
 #include "..\Constante\Cursor.h"
 #include "..\Distancia\VTCaminho.h"
 #include "..\Edita\VTEdita.h"
 #include "..\Editor\VTEditor.h"

 #include "..\Obra\VTObra.h"
 #include "..\Obra\VTObras.h"
 #include "..\Patamar\VTPatamares.h"

 #include "..\Rede\VTBarra.h"
 #include "..\Rede\VTCapacitor.h"
 #include "..\Rede\VTCapserie.h"
 #include "..\Rede\VTCarga.h"
 #include "..\Rede\VTCargaNL.h"
 #include "..\Rede\VTChave.h"
 #include "..\Rede\VTCorte.h"
 #include "..\Rede\VTEqbar.h"
 #include "..\Rede\VTGerador.h"
 #include "..\Rede\VTMedidor.h"
 #include "..\Rede\VTMotor.h"
 #include "..\Rede\VTFiltro.h"
 // #include "..\Rede\VTLigacao.h"
 #include "..\Rede\VTMutua.h"
 #include "..\Rede\VTReator.h"
 #include "..\Rede\VTRede.h"
 #include "..\Rede\VTRedes.h"
 #include "..\Rede\VTReducao.h"
 #include "..\Rede\VTRegulador.h"
 #include "..\Rede\VTResflowBar.h"
 #include "..\Rede\VTResflowLig.h"
 #include "..\Rede\VTResflowSup.h"
 #include "..\Rede\VTSuprimento.h"
 #include "..\Rede\VTTrafo.h"
 #include "..\Rede\VTTrafo3E.h"
 #include "..\Rede\VTTrafoZZ.h"
 #include "..\Rede\VTTrecho.h"
 #include "..\Rede\VTYref.h"
 #include "..\Redegraf\VTRedegraf.h"
 #include "..\..\DLL_Inc\Funcao.h"
 #include "..\..\DLL_Inc\Rede.h"
 */
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
// função global p/ criar objetos da classe TEdtAlt
// ---------------------------------------------------------------------------
VTEdt* NewObjEdtAltCargaNL(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone)
{
	return (new TEdtAltCargaNL(apl, ResetFocus, SetFocus, OnDone));
}

// ---------------------------------------------------------------------------
__fastcall TEdtAltCargaNL::TEdtAltCargaNL(VTApl *apl, TAction *ResetFocus, TAction *SetFocus,
	TAction *OnDone) : TEdtAlt(apl, ResetFocus, SetFocus, OnDone)
{

}

// ---------------------------------------------------------------------------
__fastcall TEdtAltCargaNL::~TEdtAltCargaNL(void)
{

}

// ---------------------------------------------------------------------------
bool __fastcall TEdtAltCargaNL::BuscaEqpto(void)
{
	// variáveis locais
	TList *lis;

	// varifica se há alguma Barra nas coordenadas
	area->x1 = area->x2 = evt.x;
	area->y1 = area->y2 = evt.y;
	lis = busca->Eqpto(area);
	if (lis->Count == 0)
	{ // não encontrou nenhum eqpto: muda formato do mouse
		eqpto = NULL;
		graf->Cursor(crDefault);
		return (false);
	}
	// determina primeiro Eqpto da lista
	eqpto = (VTEqpto*)lis->First();
	// muda formato do mouse
	switch (eqpto->Tipo())
	{ // barra
	case eqptoCARGANL:
		graf->Cursor(TCursor(curCarga));
		break;
	default:
		break;
	}
	return (true);
}

// ---------------------------------------------------------------------------
void __fastcall TEdtAltCargaNL::EstadoSel1_EvtMouseUp(void)
{
	// variáveis locais
	int resp = mrOk;

	// verifica se a tecla Control ou Shift está apertada (o que indica seleção de Eqpto)
	if (evt.Shift.Contains(ssCtrl) || evt.Shift.Contains(ssShift))
		return;
	// verifica se há uma Marca selecionada
	if ((marca) || (eqpto == NULL))
	{ // cancela Marcas
		MarcaCancela();
		// atualiza gráfico
		graf->Redraw(redrawEQP_SEL_COR_ORIG, this);
		// limpa listas
		lisEQS->Clear();
		estado = estSEL0;
		return;
	}
	// cancela Marcas
	// graf->Redraw(redrawMARCA, this);
	MarcaCancela();
	// edita eqpto selecionado
	switch (eqpto->Tipo())
	{ // barra

	case eqptoCARGANL:
		resp = EditaCargaNL((VTCargaNL*)eqpto);
		break;
	default:
		break;
	}
	switch (resp)
	{
	case mrNo: // Eqpto foi retirado da Rede
		lisEQS->Clear();
		// sinaliza término de operação
		Done();
		break;
	case mrOk: // Eqpto foi alterado
		// sinaliza término de operação
		Done();
	case mrCancel: // Eqpto não foi alterado
	default:
		// atualiza gráfico: reapresenta eqpto selecionado c/ sua cor original
		graf->Redraw(redrawEQP_SEL_COR_ORIG, this);
		lisEQS->Clear();
		break;
	}
	// atualiza estado
	estado = estSEL0;
}
// ---------------------------------------------------------------------------
// eof
