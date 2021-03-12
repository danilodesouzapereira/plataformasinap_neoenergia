// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormTesteBateria.h"
#include "..\Apl\VTApl.h"
#include "..\Curvas\VTCurva.h"
#include "..\Editor\VTEdt.h"
#include "..\Busca\VTBusca.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTBateria.h"
#include "..\Rede\VTEqpto.h"
#include "..\..\DLL_Inc\Editor.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Rede.h"
#include "..\..\DLL_Inc\Curvas.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
__fastcall TFormTesteBateria::TFormTesteBateria(TComponent* Owner, TWinControl *parent, VTApl *apl)
	: TForm(Owner)
{
	try
	{ // insere o Form no WinControl definido como parent
		DefParent(parent);
		// salva ponteiro apl
		this->apl = apl;
		// salva posição inicial da janela flutuante
		janela.top = 0;
		janela.left = 0;
		janela.width = Width;
		janela.height = Height;
		ActionEdt->Execute();
	}
	catch (Exception &e)
	{ // nada a fazer
	}
}

// ---------------------------------------------------------------------------
__fastcall TFormTesteBateria::~TFormTesteBateria(void)
{
	// nada a fazer
}

// ---------------------------------------------------------------------------
void __fastcall TFormTesteBateria::ActionAlignDownExecute(TObject *Sender)
{
	// verifica se a janela estava alinhada em cima
	if (Align == alTop)
		Align = alNone;
	// alinha janela em baixo
	Align = alBottom;
}

// ---------------------------------------------------------------------------
void __fastcall TFormTesteBateria::ActionAlignLeftExecute(TObject *Sender)
{
	// verifica se a janela estava alinhada à direita
	if (Align == alRight)
		Align = alNone;
	// alinha janela à esquerda
	Align = alLeft;
}

// ---------------------------------------------------------------------------
void __fastcall TFormTesteBateria::ActionAlignNoneExecute(TObject *Sender)
{
	// cancela alinhamento da janela (flutuante)
	Align = alNone;
	Top = janela.top;
	Left = janela.left;
	Width = janela.width;
	Height = janela.height;
}

// ---------------------------------------------------------------------------
void __fastcall TFormTesteBateria::ActionAlignRightExecute(TObject *Sender)
{
	// verifica se a janela estava alinhada à esquerda
	if (Align == alLeft)
		Align = alNone;
	// alinha janela à direita
	Align = alRight;
}

// ---------------------------------------------------------------------------
void __fastcall TFormTesteBateria::ActionAlignUpExecute(TObject *Sender)
{
	// verifica se a janela estava alinhada em baixo
	if (Align == alBottom)
		Align = alNone;
	// alinha janela em cima
	Align = alTop;
}

// ---------------------------------------------------------------------------
void __fastcall TFormTesteBateria::ActionEdtExecute(TObject *Sender)
{
	try
	{ // se necessário, cria um objeto TEdtExplorer
		if (edt != NULL)
			delete edt;
		edt = DLL_NewObjEdtBusca(apl, ActionMouseOff, ActionMouseOn, ActionSelEqpto);
		// atualiza indicação de mouse ativo e força botão Down
		AtualizaIndicacaoEditorOn(butEdt);
	}
	catch (Exception &e)
	{ // nada a fazer
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormTesteBateria::ActionMouseOffExecute(TObject *Sender)
{
	// liga indicação de mouse ativo e força botão Down
	AtualizaIndicacaoEditorOff();
}

// ---------------------------------------------------------------------------
void __fastcall TFormTesteBateria::ActionMouseOnExecute(TObject *Sender)
{
	// liga indicação de mouse ativo e força botão Down
	AtualizaIndicacaoEditorOn(button_down);
	// executa método Ativo
	Ativo();
}

// ---------------------------------------------------------------------------
void __fastcall TFormTesteBateria::Ativo(void)
{
	// método a ser redefinido nas classes derivadas
}

// ---------------------------------------------------------------------------
void __fastcall TFormTesteBateria::Atualiza(void)
{
	// método a ser redefinido nas classes derivadas
}

// ---------------------------------------------------------------------------
void __fastcall TFormTesteBateria::AtualizaIndicacaoEditorOff(void)
{
	// indica mouse inativo
	ImageMouseOff->Visible = true;
	ImageMouseOn->Visible = false;
	// verifica se há um Button "abaixado"
	if (button_down)
	{ // "levanta" Button que está abaixado
		button_down->Down = false;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormTesteBateria::AtualizaIndicacaoEditorOn(TToolButton *button)
{
	// salva ponteiro do Button
	button_down = button;
	if (button)
	{ // "abaixa" o Button
		button->Down = true;
		// indica mouse ativo
		ImageMouseOn->Visible = true;
		ImageMouseOff->Visible = false;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormTesteBateria::Centraliza(void)
{
	Left = (Parent->Width - Width) / 2;
	Top = (Parent->Height - Height) / 2;
}

// ---------------------------------------------------------------------------
void __fastcall TFormTesteBateria::DefParent(TWinControl *parent)
{
	if (parent)
		Parent = parent;
}

// ---------------------------------------------------------------------------
void __fastcall TFormTesteBateria::ActionSelEqptoExecute(TObject *Sender)
{
	// variáveis locais
	VTEqpto *eqpto = NULL;
	TList *lisTMP = (TList*)Sender;
	VTBarra *barra;
	VTBateria *bateria;

	// proteção
	if (Sender == NULL)
		return;
	// loop p/ todos Eqptos recebidos em lisTMP (Sender)
	for (int n = 0; n < lisTMP->Count; n++)
	{
		if ((eqpto = (VTEqpto*)lisTMP->Items[n]) == NULL)
			continue;
		if (eqpto->Tipo() == eqptoBARRA)
		{
			barra = (VTBarra*)eqpto;
			bateria = DLL_NewObjBateria(24);
			bateria->Id = FalsoId();
			bateria->Codigo = "12";
			bateria->Status[sttNOVO] = true;
			bateria->curvaCargaDescarga = InsereCurvaBateria();
			barra->InsereEqbar(bateria);
			Aviso("yey inseri a bateria!");
			return;
		}
	}
}

// ---------------------------------------------------------------------------
VTCurva* __fastcall TFormTesteBateria::InsereCurvaBateria(void)
{
	VTCurva *curva;
	// cria uma nova Curva
	curva = DLL_NewObjCurva();
	curva->Id = 69;
	curva->Codigo = "Curva Bateria";
	curva->Tipo = curvaPQ;
	curva->Unidade = unidKVA;
	curva->Escala = patEDITAVEL;
	curva->NumeroValores = 2;
	curva->Tipica = NULL;
	curva->DiaMedicao = SEMANA_COMPLETA;
	curva->Eqpto = NULL;
	curva->Classe = NULL;
	curva->LimiteInferior = 0;
	curva->LimiteSuperior = 0;

	return InsereValoresCurvaBateria(curva);;

}

// ---------------------------------------------------------------------------
VTCurva* __fastcall TFormTesteBateria::InsereValoresCurvaBateria(VTCurva *curva)
{
	// variáveis locais
	strHM hm_ini, hm_fim;
	double valor[2];

	hm_ini.hora = 0;
	hm_ini.minuto = 0;
	hm_fim.hora = 12;
	hm_fim.minuto = 0;
	valor[0] = 5;
	valor[1] = 1;

	curva->SetPonto(hm_ini, hm_fim, valor, 2);

	hm_ini.hora = 12;
	hm_ini.minuto = 0;
	hm_fim.hora = 0;
	hm_fim.minuto = 0;
	valor[0] = 4;
	valor[1] = 2;

	curva->SetPonto(hm_ini, hm_fim, valor, 2);

    return curva;
}

// eof
