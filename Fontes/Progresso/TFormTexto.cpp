// --------------------------------------------------------------------------------------------- bof
#include <vcl.h>
#pragma hdrstop
#pragma package(smart_init)
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------------------- includes
#include <algorithm>
#include "TFormTexto.h"

// --------------------------------------------------------------------------------------- namespace

// ------------------------------------------------------------------------------------------- class

// -------------------------------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/16082112
__fastcall TFormTexto::TFormTexto(TComponent* Owner) : TForm(Owner)
{
	// cria objetos
	// cs = new TCriticalSection();
	// inicia configuração para desenho
//jss20191003
	animacao.auto_inc    = true;
	animacao.max_step    = 0;
	animacao.step        = 0;
	animacao.x           = 0;
	animacao.dim         = (RichEdit->Width + 99) / 100;
	animacao.dim         = 5;
	animacao.y1          = RichEdit->Height;
	animacao.y2          = ClientHeight;
	animacao.color_barra = clGreen;
	animacao.color_fundo = clWhite;
	#ifdef _DEBUG
	this->Position = poDesigned;
	this->Top = 0;
	this->Left = 0;
	#endif
}

// -------------------------------------------------------------------------------------------------
__fastcall TFormTexto::~TFormTexto(void)
{
	// destrói objetos
	// if (cs) {delete cs; cs = NULL;}
}

// -------------------------------------------------------------------------------------------------
void __fastcall TFormTexto::Add(AnsiString texto, bool clear)
{
	// proteção
	// if (cs == NULL) return;
	// bloqueia acesso
	// cs->Acquire();
	// verifica se deve reiniciar o RichEdit
	if (clear)
		RichEdit->Lines->Clear();

	// insere linha de texto
	RichEdit->Lines->Add(texto);

	// correcao no scroll e travamento(ProcessMessage), se o codigo abaixo causar alguma
	// instabilidade substituir tudo por: RichEdit->Refresh();
	Application->ProcessMessages();
	SendMessage(RichEdit->Handle, WM_VSCROLL, SB_BOTTOM, 0);
	RichEdit->Refresh();

}

// -------------------------------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/16082112
void __fastcall TFormTexto::Atualiza(void)
{
	// variáveis locais
	TColor color;

	// proteção
	// if (cs == NULL) return;
	// bloqueia acesso
	// cs->Acquire();
	// desenha uma linha vertical
	// DrawLinha(animacao.x, animacao.y1, animacao.x, animacao.y2, animacao.dim, animacao.color_barra);

//jss20191003
//	DrawRetangulo(animacao.x, animacao.y1, animacao.x + animacao.dim, animacao.y2,
//		animacao.color_barra);
	DrawRetangulo(animacao.x,
				  animacao.y1,
				  animacao.x,
				  animacao.y2,
				  animacao.color_barra);


	// incrementa x
/*	if ((animacao.x += animacao.dim + 1) >= ClientWidth)
	{ // limpa Image
		Clear(animacao.color_fundo);
		// reincia valor de x
		animacao.x = 0;
	}
*/
	// libera acesso
	// cs->Release();
	// NÃO atualiza a tela
	// Refresh();
	// Application->ProcessMessages();
}

//jss20191003
// -------------------------------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/16082112
void __fastcall TFormTexto::NoAutoInc(void)
{
	animacao.auto_inc = false;
}

// -------------------------------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/16082112
bool __fastcall TFormTexto::MaxStep(int max)
{
    if (animacao.max_step != 0)
        return false;
    animacao.auto_inc = false;
    animacao.max_step = max;
    animacao.step     = 0;
    animacao.x        = 0;
    animacao.dim      = (RichEdit->Width + max - 1) / max;
    return true;
}

// -------------------------------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/16082112
void __fastcall TFormTexto::Step(void)
{
	// incrementa x
	++animacao.step;
	animacao.x = animacao.step * animacao.dim;
	if (animacao.auto_inc)
	{
		if ((animacao.x + 1) >= ClientWidth)
		{
			// limpa Image
			Clear(animacao.color_fundo);
			// reincia valor de x
			animacao.step = 0;
			animacao.x    = 0;
		}
	}
	else
	{
		if ((animacao.x + 1) >= ClientWidth || animacao.step > animacao.max_step)
		{
			animacao.step = animacao.max_step;
			animacao.x    = ClientWidth - 2;
		}
	}
    Atualiza();
}

// -------------------------------------------------------------------------------------------------
void __fastcall TFormTexto::Clear(TColor color)
{
	// variáveis locais
	TCanvas *canvas = Canvas;

	canvas->Lock();
	TRect r(0, animacao.y1, ClientWidth, animacao.y2);
	canvas->Brush->Style = bsSolid;
	canvas->Brush->Color = color;
	canvas->FillRect(r);
	canvas->Unlock();
}

// -------------------------------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/16082112
void __fastcall TFormTexto::DrawRetangulo(int x1, int y1, int x2, int y2, TColor color)
{
	// variáveis locais
	TCanvas *canvas = Canvas;

	canvas->Lock();
	canvas->Pen->Width = 1;
	canvas->Pen->Color = color;
	canvas->Pen->Style = psSolid;
	canvas->Brush->Style = bsSolid;
	canvas->Brush->Color = color;
	// posiciona no primeiro ponto e traça linha até o segundo
//jss20191003
//	canvas->Rectangle(x1, y1, x2, y2);
	canvas->Rectangle(0, y1, x2, y2);
	canvas->Refresh();
	canvas->Unlock();
}

// -------------------------------------------------------------------------------------------------
void __fastcall TFormTexto::FormClose(TObject *Sender, TCloseAction &Action)
{
	Action = caHide;
}

// --------------------------------------------------------------------------------------------- eof
