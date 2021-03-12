// --------------------------------------------------------------------------------------------- bof
#include <vcl.h>
#pragma hdrstop
#pragma package(smart_init)
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------------------- includes
#include <complex>
#include <algorithm>
#include "TFormDefault.h"

// --------------------------------------------------------------------------------------- namespace

// ------------------------------------------------------------------------------------------- class

// -------------------------------------------------------------------------------------------------
__fastcall TFormDefault::TFormDefault(TComponent* Owner) : TForm(Owner)
{
	// garante que área útil seja um quandrado
	if (ClientHeight != ClientWidth)
		ClientHeight = ClientWidth;
	// cria objetos
	// cs = new TCriticalSection();
	// inicia configuração para desenho
	animacao.x = ClientRect.Left + (ClientWidth / 2);
	animacao.y = ClientRect.Top + (ClientHeight / 2);
	animacao.raio = 20;
	animacao.count_max = 16;
	animacao.count = 0;
}

// -------------------------------------------------------------------------------------------------
__fastcall TFormDefault::~TFormDefault(void)
{
	// destrói objetos
	// if (cs) {delete cs; cs = NULL;}
}

// -------------------------------------------------------------------------------------------------
void __fastcall TFormDefault::Atualiza(void)
{
	// variáveis locais
	int x, y;
	double fator;
	complex<double>coord_xy;

	// proteção
	// if (cs == NULL) return;
	// bloqueia acesso
	// cs->Acquire();
	// determina coordenadas do ponto
	fator = double(animacao.count) / double(animacao.count_max);
	coord_xy = polar(double(animacao.raio), fator * 2 * M_PI);
	x = animacao.x + coord_xy.real();
	y = animacao.y + coord_xy.imag();
	// desenha circunferência
	DrawCirculoXor(x, y, 3);
	// define próximo ponto
	if ((animacao.count++) >= (animacao.count_max - 1))
		animacao.count = 0;
	// atualiza a tela
	// Refresh();
	// Application->ProcessMessages();
	// libera acesso
	// cs->Release();
	// Beep();
}

// -------------------------------------------------------------------------------------------------
void __fastcall TFormDefault::DrawCirculoXor(int x, int y, int raio)
{
	// variáveis locais
	TPenMode orig_mode;
	TCanvas *canvas = Canvas;

	canvas->Lock();
	// salva e altera PenMode
	orig_mode = Canvas->Pen->Mode;
	// define padrão da linha
	canvas->Pen->Mode = pmXor;
	// canvas->Pen->Color = (Color == clBlack) ? clWhite : clBlack;
	canvas->Pen->Color = clWhite;
	canvas->Pen->Width = 1;
	canvas->Brush->Style = bsSolid; // bsClear;
	// escreve o texto
	canvas->Ellipse(x - raio, y - raio, x + raio, y + raio);
	// restaura PenMode
	canvas->Pen->Mode = orig_mode;
	canvas->Refresh();
	canvas->Unlock();
}

// -------------------------------------------------------------------------------------------------
void __fastcall TFormDefault::DrawCircunferencia(int x, int y, int raio, TColor color)
{
	// variáveis locais
	// TCanvas  *canvas = Image->Canvas;
	TCanvas *canvas = Canvas;

	canvas->Lock();
	// define cor da linha
	canvas->Pen->Width = 1;
	canvas->Pen->Mode = pmXor;
	canvas->Pen->Color = color;
	canvas->Brush->Style = bsSolid; // bsClear;
	canvas->Ellipse(x - raio, y - raio, x + raio, y + raio);
	canvas->Unlock();
}

// -------------------------------------------------------------------------------------------------
void __fastcall TFormDefault::FormClose(TObject *Sender, TCloseAction &Action)
{
	Action = caHide;
}

// --------------------------------------------------------------------------------------------- eof
