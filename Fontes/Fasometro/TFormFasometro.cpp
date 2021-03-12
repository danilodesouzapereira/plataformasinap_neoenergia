// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <math.h>
#include <math.hpp>
#include <algorithm>
#include "TFormFasometro.h"
#include "TAplFasometro.h"
#include "VTFasor.h"
#include "VTFasores.h"
#include "..\Edita\VTEdita.h"
#include "..\Editor\VTEdt.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Patamar\VTPatamar.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTTrecho.h"
#include "..\RedeCC\VTRedeCC.h"
#include "..\..\DLL_Inc\Editor.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Help.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TFORMEDT"
#pragma link "TFormEdt"
#pragma resource "*.dfm"
using namespace std;

// ---------------------------------------------------------------------------
__fastcall TFormFasometro::TFormFasometro(TForm *form_owner, VTApl *apl_owner, TWinControl *parent)
	: TFormEdt(form_owner, parent)
{
	// variáveis locais
	VTEdita *edita = (VTEdita*)apl_owner->GetObject(__classid(VTEdita));

	try
	{ // cria objeto TAplSpy
		apl = new TAplFasometro(this, apl_owner);
		fasores = (VTFasores*)apl->GetObject(__classid(VTFasores));
		// insere ActionRedeChanged em VTEdita
		if (edita)
			edita->ActionOnChangeRedeInsere(ActionRedeChanged);
		// inicia CBoxPatamar
		IniciaCBoxPatamar();
		// inicia propriedades do Form p/ desenhar os fasores
		FormIniciaPropriedades();
	}
	catch (Exception &e)
	{ // nada a fazer
	}
}

// ---------------------------------------------------------------------------
__fastcall TFormFasometro::~TFormFasometro(void)
{
	// variáveis locais
	VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

	// salva parâmetros do Form
	if (geral)
		geral->Infoset->SaveToFile(this);
	// apaga moldura se o eqpto não for nulo
	if (eqpto != NULL)
	{
		eqpto = NULL;
		grafico->Moldura(eqpto);
	}
	// retira ActionRedeChanged de VTEdita
	if (edita)
		edita->ActionOnChangeRedeRetira(ActionRedeChanged);
	// destrói objetos
	if (edt)
	{
		delete edt;
		edt = NULL;
	}
	if (apl)
	{
		delete apl;
		apl = NULL;
	}
	// libera memória alocada p/ bitmap de TImage
	if (graph_bmp)
	{
		delete graph_bmp;
		graph_bmp = NULL;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormFasometro::ActionAnimacaoExecute(TObject *Sender)
{
	// habilita/desabilita Timer
	Timer->Enabled = butAnimacao->Down;
}

// ---------------------------------------------------------------------------
void __fastcall TFormFasometro::ActionEdtExecute(TObject *Sender)
{
	try
	{ // se necessário, cria um objeto VTEdt
		if (edt != NULL)
			delete edt;
		// cria VTEdt p/ controle do mouse
		edt = DLL_NewObjEdtBusca(apl, ActionMouseOff, ActionMouseOn, ActionSelEqpto);
		// atualiza indicação de mouse ativo e força botão Down
		AtualizaIndicacaoEditorOn(butEdt);
		// Form está ativo: atualiza  moldura
		Ativo();
	}
	catch (Exception &e)
	{ // nada a fazer
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormFasometro::ActionHelpExecute(TObject *Sender)
{ // variaveis locais
	TForm *formHelp;

	formHelp = DLL_NewFormHelp(this, apl, "Fasômetro");
	if (formHelp)
		formHelp->Show();
}

// ---------------------------------------------------------------------------
void __fastcall TFormFasometro::ActionRedeChangedExecute(TObject *Sender)
{
	//
}

// ---------------------------------------------------------------------------
void __fastcall TFormFasometro::ActionSelEqptoExecute(TObject *Sender)
{
	// variáveis locais
	VTBarCC *barCC;
	VTLigCC *ligCC;
	VTRedeCC *redeCC = (VTRedeCC*)apl->GetObject(__classid(VTRedeCC));
	TList *lisEQP = (TList*)Sender;
	VTEqpto *teste;

	// proteção
	if (lisEQP == NULL)
		return;
	if (lisEQP->Count == 0)
		return;
	// determina Eqpto selecionado
	if ((eqpto = (VTEqpto*)lisEQP->First()) == NULL)
		return;
	// define Trecho fictício a ser exibido caso seja selecionado o Trecho de defeito
	ind_trecho_fic = (ind_trecho_fic == 0) ? 1 : 0;
	// atualiza Caption
	Caption = "Fasores - " + eqpto->TipoAsString() + " " + eqpto->Codigo;
	// reinicia Fasores
	fasores->Reinicia();
	// verifica se deve exibir dados de fluxo de potência ou de curto-circuito
	// if (redeCC == NULL) //FKM 2015.12.02
	if ((redeCC == NULL) || (redeCC->EqptoDefeito() == NULL))
	{ // fasores de fluxo
		if (eqpto->TipoBarra())
		{
			fasores->Insere((VTBarra*)eqpto);
		}
		else if (eqpto->TipoLigacao())
		{
			fasores->Insere((VTLigacao*)eqpto);
		}
	}
	else
	{ // fasores de curto
		if (eqpto->TipoBarra())
		{
			barCC = (VTBarCC*)eqpto->Obj;
			fasores->Insere(barCC);
		}
		else if (eqpto->TipoLigacao())
		{ // verifica  se é o Trecho de defeito
			if (eqpto == redeCC->TrechoDefeito())
			{ // obtém Trecho fictício
				eqpto = redeCC->TrechoFicticio(ind_trecho_fic);
				ligCC = (VTLigCC*)eqpto->Obj;
				fasores->Insere(ligCC);
			}
			else
			{
				ligCC = (VTLigCC*)eqpto->Obj;
				fasores->Insere(ligCC);
			}
		}
	}
	// reinicia CLBoxFasor
	IniciaCLBoxFasor();
	// apresenta diagrama de fasores
	DrawFasor();
	// preenche o listView com os fasores
	PreencheListView();
	// Form está ativo: atualiza  moldura
	Ativo();
}

// ---------------------------------------------------------------------------
void __fastcall TFormFasometro::Ativo(void)
{
	// variáveis locais
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	// exibe moldura no Eqpto
	if ((grafico != NULL) && (eqpto != NULL))
	{
		grafico->Moldura(eqpto);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormFasometro::Atualiza(void)
{
	// variáveis locais
	VTRedeCC *redeCC = (VTRedeCC*)apl->GetObject(__classid(VTRedeCC));

	// verifica se Fasores de dados de curto
	if (fasores->TipoCurto)
	{ // verifica gráfico sendo exibido
		if (redeCC != NULL)
		{ // atualiza Fasores de acordo c/ patamar
			fasores->DefinePatamar(CBoxPatamar->ItemIndex);
		}
		else
		{ // reinicia Fasores e CLBoxFasor
			fasores->Reinicia();
			IniciaCLBoxFasor();
		}
	}
	// verifica se Fasores de dados de fluxo
	else if (fasores->TipoFluxo)
	{ // verifica gráfico em exibição
		// if (redeCC == NULL) //FKM 2015.12.02
		if ((redeCC == NULL) || (redeCC->EqptoDefeito() == NULL))
		{ // atualiza Fasores de acordo c/ patamar
			fasores->DefinePatamar(CBoxPatamar->ItemIndex);
		}
		else
		{ // reinicia Fasores e CLBoxFasor
			fasores->Reinicia();
			IniciaCLBoxFasor();
		}
	}
	// desenha Fasor
	DrawFasor();
	// atualiza o listView
	PreencheListView();
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TFormFasometro::AvaliaPrefixo(double *valor, double prefixo) // FKM 2015.12.01
{ // variaveis locais
	double inv_log1000 = 1 / logf(1000);
	int pot_mil = logf(prefixo) * inv_log1000;
	int prefix;
	int n_DivMil = 0;
	int n_MulMil = 0;

	// proteção, caso o valor seja perto de zero
	if (IsDoubleZero(*valor))
		return "";
	// diminui o valor pra um prefixo menor
	while (*valor > 1000)
	{
		*valor = *valor / 1000;
		n_DivMil++;
	}
	// aumena o valor pra um prefixo maior
	while (*valor < 0.6)
	{
		*valor = *valor * 1000;
		n_MulMil++;
	}
	// define o melhor prefixo
	prefix = pot_mil + n_DivMil - n_MulMil;
	// retorna o prefixo em texto
	switch (prefix)
	{
	case 8: // yotta
		return "Y";
	case 7: // zetta
		return "Z";
	case 6: // exa
		return "E";
	case 5: // peta
		return "P";
	case 4: // tera
		return "T";
	case 3: // giga
		return "G";
	case 2: // mega
		return "M";
	case 1: // kilo
		return "k";
	case 0: // nada
		return "";
	case -1: // mili
		return "m";
	case -2: // micro
		return "μ";
	case -3: // nano
		return "n";
	default:
		return ""; ;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormFasometro::CBoxPatamarChange(TObject *Sender)
{
	// proteção
	if (CBoxPatamar->ItemIndex < 0)
		return;
	// recalcula os Fasores e atualiza a tela
	Atualiza();
}

// ---------------------------------------------------------------------------
void __fastcall TFormFasometro::CLBoxFasorClickCheck(TObject *Sender)
{
	// variáveis locais
	VTFasor *fasor;

	// habilita/desabilita visualização de todos os Fasores
	for (int n = 0; n < CLBoxFasor->Items->Count; n++)
	{
		if ((fasor = (VTFasor*)CLBoxFasor->Items->Objects[n]) == NULL)
			continue;
		fasor->visible = CLBoxFasor->Checked[n];
	}
	// reapresenta os Fasores
	DrawFasor();
	// atualiza o listView
	PreencheListView();
}

// ---------------------------------------------------------------------------
void __fastcall TFormFasometro::CLBoxFasorDrawItem(TWinControl *Control, int Index, TRect &Rect,
	TOwnerDrawState State)
{
	// variáveis locais
	TCanvas *Canvas = CLBoxFasor->Canvas;
	VTFasor *fasor;
	TColor color_ori;
	TBrushStyle style_ori;

	// limpa a área do item
	Canvas->FillRect(Rect);
	// determina objeto Fasor
	if ((fasor = (VTFasor*)CLBoxFasor->Items->Objects[Index]) == NULL)
		return;
	// salva cor/style original
	color_ori = Canvas->Brush->Color;
	style_ori = Canvas->Brush->Style;
	// define área do retângulo p/ a cor do Fasor
	TRect r = Rect;
	r.Top += 2;
	r.Bottom -= 2;
	r.Right = r.Left + (r.Bottom - r.Top) + 2;
	// escreve código do Fasor
	Canvas->TextOut(r.Right + 1, Rect.Top, fasor->codigo);
	// desenha retângulo c/ a cor
	Canvas->Brush->Color = fasor->color;
	Canvas->Brush->Style = bsSolid;
	Canvas->FillRect(r); // desenha o retângulo da cor
	// restaura cor/style original
	Canvas->Brush->Color = color_ori;
	Canvas->Brush->Style = style_ori;
}

// ---------------------------------------------------------------------------
void __fastcall TFormFasometro::DrawCircunferencia(int x, int y, int raio, TColor cor)
{
	// variáveis locais
	struct
	{
		int width;
		TColor color;
	} pen;
	struct
	{
		TBrushStyle style;
	} brush;
	// salva configuração de TPen e TBrush
	pen.width = ImageFasor->Canvas->Pen->Width;
	pen.color = ImageFasor->Canvas->Pen->Color;
	brush.style = ImageFasor->Canvas->Brush->Style;
	// configura TPen e TBrush
	ImageFasor->Canvas->Pen->Width = 1;
	ImageFasor->Canvas->Pen->Color = cor;
	ImageFasor->Canvas->Brush->Style = bsClear;
	// desenha a circunferência
	ImageFasor->Canvas->Ellipse(x - raio, y - raio, x + raio, y + raio);
	// restaura configuração de TPen e TBrush
	ImageFasor->Canvas->Pen->Width = pen.width;
	ImageFasor->Canvas->Pen->Color = pen.color;
	ImageFasor->Canvas->Brush->Style = brush.style;
}

// ---------------------------------------------------------------------------
void __fastcall TFormFasometro::DrawFasor()
{
	// variáveis locais
	double raio;
	int xt, yt, x0, y0, xa, ya, xb, yb, xc, yc;
	struct
	{
		int width;
		TColor color;
	} pen;
	TColor cor_fundo = clBtnFace;
	VTFasor *fasor;
	// TList   *lisFASOR = fasores->LisFasor();

	// salva atributos do Canvas
	pen.color = ImageFasor->Canvas->Pen->Color;
	pen.width = ImageFasor->Canvas->Pen->Width;
	// limpa o Canvas
	LimpaImageFasor(cor_fundo);
	// define coordenadas da origem dos eixos
	x0 = ImageFasor->Width / 2;
	y0 = ImageFasor->Height / 2;
	// assume as mesmas coordenadas p/ as 3 fases
	xa = xb = xc = x0;
	ya = yb = yc = y0;
	// desenha eixos
	ImageFasor->Canvas->Pen->Color = clWhite; // clLtGray clGray;
	ImageFasor->Canvas->MoveTo(x0, 0);
	ImageFasor->Canvas->LineTo(x0, ImageFasor->Height);
	ImageFasor->Canvas->MoveTo(0, y0);
	ImageFasor->Canvas->LineTo(ImageFasor->Width, y0);
	// loop p/ todos fasores
	// for (int n = 0; n < lisFASOR->Count; n++)
	for (int n = 0; n < CLBoxFasor->Items->Count; n++)
	{ // verifica se o Fasor está habilitado
		if (!CLBoxFasor->Checked[n])
			continue;
		// fasor = (VTFasor*)lisFASOR->Items[n];
		fasor = (VTFasor*)CLBoxFasor->Items->Objects[n];
		if (fasor->dim == 0)
			return;
		// define escala
		escala = (2. * fasor->dim) / (0.8 * min(ImageFasor->Width, ImageFasor->Height));
		// converte coordenadas do fasor p/ pixel (invertendo y)
		if (fasor->a.enabled)
		{
			xa = x0 + (fasor->a.x - fasor->orig.x) / escala;
			ya = y0 - (fasor->a.y - fasor->orig.y) / escala;
		}
		if (fasor->b.enabled)
		{
			xb = x0 + (fasor->b.x - fasor->orig.x) / escala;
			yb = y0 - (fasor->b.y - fasor->orig.y) / escala;
		}
		if (fasor->c.enabled)
		{
			xc = x0 + (fasor->c.x - fasor->orig.x) / escala;
			yc = y0 - (fasor->c.y - fasor->orig.y) / escala;
		}
		// desenha circunferência
		raio = RoundTo(fasor->raio / escala, 0) + 1;
		DrawCircunferencia(x0, y0, raio, clWhite); // clLtGray clGray
		// configura Canvas(Font, Pen e Brush)
		ImageFasor->Canvas->Pen->Color = fasor->color;
		ImageFasor->Canvas->Pen->Width = 1;
		ImageFasor->Canvas->Font->Color = fasor->color;
		ImageFasor->Canvas->Brush->Color = cor_fundo;
		ImageFasor->Canvas->Brush->Style = bsClear;
		// desenha fase A
		if (fasor->a.enabled)
		{
			ImageFasor->Canvas->MoveTo(x0, y0);
			ImageFasor->Canvas->LineTo(xa, ya);
			DrawSeta(x0, y0, xa, ya, fasor->color);
			xt = xa;
			if (xa < x0)
				xt -= ImageFasor->Canvas->TextWidth(fasor->a.codigo);
			yt = ya;
			if (ya < y0)
				yt -= ImageFasor->Canvas->TextHeight(fasor->a.codigo);
			ImageFasor->Canvas->TextOut(xt, yt, fasor->a.codigo);
		}
		// desenha fase B
		if (fasor->b.enabled)
		{
			ImageFasor->Canvas->MoveTo(x0, y0);
			ImageFasor->Canvas->LineTo(xb, yb);
			DrawSeta(x0, y0, xb, yb, fasor->color);
			xt = xb;
			if (xb < x0)
				xt -= ImageFasor->Canvas->TextWidth(fasor->b.codigo);
			yt = yb;
			if (yb < y0)
				yt -= ImageFasor->Canvas->TextHeight(fasor->b.codigo);
			ImageFasor->Canvas->TextOut(xt, yt, fasor->b.codigo);
		}
		// desenha fase C
		if (fasor->c.enabled)
		{
			ImageFasor->Canvas->MoveTo(x0, y0);
			ImageFasor->Canvas->LineTo(xc, yc);
			DrawSeta(x0, y0, xc, yc, fasor->color);
			xt = xc;
			if (xc < x0)
				xt -= ImageFasor->Canvas->TextWidth(fasor->c.codigo);
			yt = yc;
			if (yc < y0)
				yt -= ImageFasor->Canvas->TextHeight(fasor->c.codigo);
			ImageFasor->Canvas->TextOut(xt, yt, fasor->c.codigo);
		}
	}
	// restaura configuração de TPen
	ImageFasor->Canvas->Pen->Color = pen.color;
	ImageFasor->Canvas->Pen->Width = pen.width;
}

// ---------------------------------------------------------------------------
void __fastcall TFormFasometro::DrawSeta(int x1, int y1, int x2, int y2, TColor cor)
{
	// variáveis locais
	double hipot, sin_alfa, cos_alfa;
	double dx = x2 - x1;
	double dy = y2 - y1;
	double comp = 12;
	double larg = 4;
	int xa, ya, xb, yb;

	// proteção
	if ((x2 == x1) && (y2 == y1))
		return;
	// determina seno e cosseno do ângulo da reta
	hipot = sqrt((dx * dx) + (dy * dy));
	if (hipot < comp)
		return;
	cos_alfa = dx / hipot;
	sin_alfa = dy / hipot;
	// determina os pontos A e B
	xa = x2 - RoundTo((comp * cos_alfa) + (larg * sin_alfa), 0);
	ya = y2 - RoundTo((comp * sin_alfa) - (larg * cos_alfa), 0);
	xb = x2 - RoundTo((comp * cos_alfa) - (larg * sin_alfa), 0);
	yb = y2 - RoundTo((comp * sin_alfa) + (larg * cos_alfa), 0);
	// desenha as linha (xa,ya)/(x2,y2) e (x2,y2)/(xb,yb)
	ImageFasor->Canvas->MoveTo(xa, ya);
	ImageFasor->Canvas->LineTo(x2, y2);
	ImageFasor->Canvas->LineTo(xb, yb);
	/*
	 ImageFasor->Canvas->MoveTo(xa, ya);
	 ImageFasor->Canvas->LineTo(x2, y2);
	 ImageFasor->Canvas->LineTo(xb, yb);
	 ImageFasor->Canvas->LineTo(xa, ya);
	 ImageFasor->Canvas->FloodFill((xa+xb)/2, (ya+yb)/2, cor, fsSurface);
	 */
}

/*
 //---------------------------------------------------------------------------
 void __fastcall TFormFasometro::DrawSeta(int xa, int ya, int xb, int yb, int dim, int Align, double alfa, TColor cor)
 {
 //variáveis locais
 int x1, y1, x2, y2, x3, y3, x4, y4;
 double   a, b;
 //determina o quadrante
 if (xb > xa)
 {
 if (yb > ya)
 {//quadrante 4
 x1 = ; y1 = ;
 x2 = ; y2 = ;
 x3 = ; y3 = ;
 x4 = ; y4 = ;
 }
 else
 {//quadrante 1
 }
 }
 else if (yb > ya)
 {//quandrante 3
 }
 else
 {//quadrante 2
 }

 //converte coordenadas p/ coordenadas da tela
 //x = Xzoom_to_Xtela(x);
 //y = Yzoom_to_Ytela(y);
 //define x3,y3 x4,y4 em função da posicao
 switch(Align)
 {
 case alLeft:   //seta p/ esquerda
 //define x1,y1,x2,y2 em função da dimensão
 x -= 2;
 x1 = x-dim; y1 = y-dim;
 x2 = x+dim; y2 = y+dim;
 //define x3,y3,x4,y4 em função do ângulo
 x3 = x4 = x + dim * cos(alfa/2);
 y4      = y - dim * sin(alfa/2);
 y3      = y + dim * sin(alfa/2);
 break;
 case alRight:  //seta p/ direita
 break;
 case alTop:    //seta p/ cima
 break;
 case alBottom: //seta p/ baixo
 //define x1,y1,x2,y2 em função da dimensão
 y += 3;
 x1 = x-dim; y1 = y-dim;
 x2 = x+dim; y2 = y+dim;
 //define x3,y3,x4,y4 em função do ângulo
 x3 = x + dim * cos((M_PI - alfa)/2);
 x4 = x - dim * cos((M_PI - alfa)/2);
 y4 = y3 = y - dim * sin((M_PI - alfa)/2);
 break;
 }
 //desenha a pizza
 imag->Canvas->Brush->Style = bsSolid;
 imag->Canvas->Brush->Color = cor;
 imag->Canvas->Pen->Width   = 1;
 imag->Canvas->Pen->Color   = cor;
 imag->Canvas->Pie(x1, y1, x2, y2, x3, y3, x4, y4);
 }
 */

// ---------------------------------------------------------------------------
void __fastcall TFormFasometro::FormClose(TObject *Sender, TCloseAction &Action)
{
	// destrói o form
	Action = caFree;
}

// ---------------------------------------------------------------------------
void __fastcall TFormFasometro::FormIniciaPosicao(void)
{
	// variáveis locais
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

	if (geral)
	{ // lê parâmetros do Form
		if (geral->Infoset->LoadFromFile(this))
			return;
	}
	// posiciona o Form sem alinhamento
	ActionAlignNoneExecute(NULL);
}

// ---------------------------------------------------------------------------
void __fastcall TFormFasometro::FormIniciaPropriedades(void)
{
	// seta propriedade DoubleBuffered p/ evitar flicker
	DoubleBuffered = true;
	// limita tamanho do form
	Constraints->MaxHeight = Screen->Height;
	Constraints->MaxWidth = Screen->Width;
	// limita tamanho do gráfico
	ImageFasor->Constraints->MaxHeight = Screen->Height; // - 46;
	ImageFasor->Constraints->MaxWidth = Screen->Width; // - 8;
	// aloca Bitmap p/ o gráfico
	// if (ImageFasor->Picture->Graphic) delete ImageFasor->Picture->Graphic;
	graph_bmp = new Graphics::TBitmap();
	graph_bmp->Height = Screen->Height;
	graph_bmp->Width = Screen->Width;
	ImageFasor->Picture->Graphic = graph_bmp;
}

// ---------------------------------------------------------------------------
void __fastcall TFormFasometro::FormResize(TObject *Sender)
{
	// redesenha o fasor
	if (fasores)
	{
		DrawFasor();
		// atualiza o listView
		// PreencheListView();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormFasometro::FormShow(TObject *Sender)
{
	// posiciona o Form
	FormIniciaPosicao();
	// inicia objeto EdtExplorer
	ActionEdtExecute(NULL);
}

// ---------------------------------------------------------------------
void __fastcall TFormFasometro::IniciaCBoxPatamar(void)
{
	// variáveis locais
	VTPatamar *patamar;
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
	TList *lisPAT = patamares->LisPatamar();

	// reinicia CBoxPatamar c/ Patamares
	CBoxPatamar->Clear();
	for (int np = 0; np < lisPAT->Count; np++)
	{
		patamar = (VTPatamar*)lisPAT->Items[np];
		CBoxPatamar->Items->AddObject(patamar->Nome, patamar);
	}
	// pré-seleciona primeiro patamar
	if (CBoxPatamar->Items->Count > 0)
		CBoxPatamar->ItemIndex = 0;
}

// ---------------------------------------------------------------------
void __fastcall TFormFasometro::IniciaCLBoxFasor(void)
{
	// variáveis locais
	VTFasor *fasor;
	TList *lisFASOR = fasores->LisFasor();

	// reinicia CLBoxFasor
	CLBoxFasor->Clear();
	for (int np = 0; np < lisFASOR->Count; np++)
	{
		fasor = (VTFasor*)lisFASOR->Items[np];
		CLBoxFasor->Items->AddObject(fasor->codigo, fasor);
		CLBoxFasor->Checked[CLBoxFasor->Items->Count - 1] = fasor->visible;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormFasometro::LimpaImageFasor(TColor color)
{
	/*
	 //variáveis locais
	 struct   {
	 TColor color;
	 TBrushStyle = style;
	 }brush;
	 //salva configuração de Brush
	 brush.color = ImageFasor->Canvas->Brush->Color;
	 brush.style = ImageFasor->Canvas->Brush->Style;
	 */
	// obtém área de TImagefasor
	TRect r = ImageFasor->ClientRect;
	// apaga a área com a cor de fundo
	ImageFasor->Canvas->Brush->Color = color;
	ImageFasor->Canvas->Brush->Style = bsSolid;
	// imag->Canvas->Brush->Color = tela.cor_fundo;
	ImageFasor->Canvas->FillRect(r);
	/*
	 //salva configuração de Brush
	 ImageFasor->Canvas->Brush->Color = brush.color;
	 ImageFasor->Canvas->Brush->Style = brush.style;
	 */
}

// ---------------------------------------------------------------------------
void __fastcall TFormFasometro::PreencheListView(void) // FKM 2015.12.01
{ // variaveis locais
	VTRedeCC *redeCC = (VTRedeCC*)apl->GetObject(__classid(VTRedeCC));
	complex<double>fasor_a, fasor_b, fasor_c;
	VTFasor *fasor;
	TListItem *valoresFasor;
	double valor;
	double multInicial;
	// AnsiString        nomeFasor;
	// estrutura do prefixo
	struct
	{
		bool allow;
		AnsiString unidade, simbolo;
	} prefix;

	// inicializa prefix
	prefix.allow = true;
	prefix.unidade = "";
	prefix.simbolo = "";
	// limpa o listView
	lvValores->Clear();
	// percorre todos vetores
	for (int n = 0; n < CLBoxFasor->Items->Count; n++)
	{ // verifica se o Fasor está habilitado
		if (!CLBoxFasor->Checked[n])
			continue;
		fasor = (VTFasor*)CLBoxFasor->Items->Objects[n];
		// salva o nome do fasor, caso seja necessário fazer modificacoes
		// nomeFasor = fasor->codigo;
		if (fasor->dim == 0)
			continue;
		// define a unidade
		switch (fasor->atr)
		{
		case lig_I_ABC:
			// o valor vem em ampere
			prefix.unidade = "A";
			prefix.simbolo = "";
			multInicial = 1;
			break;
		case lig_S:
			// o valor vem em MVA
			prefix.unidade = "VA";
			prefix.simbolo = "M";
			multInicial = 1e6;
			break;
		case bar_VFN_KV:
			prefix.unidade = "V";
			prefix.simbolo = "k";
			multInicial = 1e3;
			break;
		case bar_VFF_KV:
			prefix.unidade = "V";
			prefix.simbolo = "";
			multInicial = 1e3;
			break;
		default:
			// caso seja curto e barra
			if ((redeCC != NULL) && eqpto->TipoBarra())
			{
				prefix.unidade = "V";
				prefix.simbolo = "";
				multInicial = 1e3;
				// remove o "(pu)" do nome
				// nomeFasor = fasor->codigo.SubString(0,(fasor->codigo.Length() - 4));
			}
			// caso seja curto e ligação
			if ((redeCC != NULL) && eqpto->TipoLigacao())
			{
				prefix.unidade = "A";
				prefix.simbolo = "";
				multInicial = 1;
				// remove o "(kA)" do nome
				// nomeFasor = fasor->codigo.SubString(0,(fasor->codigo.Length() - 4));
			};
		}
		// começa a edição do listView
		// lvValores->Items->BeginUpdate();
		// salva o fasor na forma complexa
		if (fasor->a.enabled)
		{
			fasor_a = complex<double>(fasor->a.x, fasor->a.y);
			// caso o modulo seja nulo, não preenche adiciona ao listview
			// (erro ATAN no calculo do argumento)
			if (IsDoubleZero(abs(fasor_a)))
				continue;
			valoresFasor = lvValores->Items->Add();
			// caso seja S complexo é trifasico
			// if(fasor->atr == lig_S) valoresFasor->Caption = nomeFasor;
			// else                    valoresFasor->Caption = nomeFasor + "_a";
			valoresFasor->Caption = fasor->a.codigo;
			valor = abs(fasor_a);
			// descobre q unidade usar
			if (prefix.allow)
				prefix.simbolo = AvaliaPrefixo(&valor, multInicial);
			valoresFasor->SubItems->Add(DoubleToStr("%4.3f", valor));
			valoresFasor->SubItems->Add(prefix.simbolo + prefix.unidade);
			valoresFasor->SubItems->Add(DoubleToStr("%2.1f", arg(fasor_a)*180 / M_PI) + "º");
		}
		if (fasor->b.enabled)
		{
			fasor_b = complex<double>(fasor->b.x, fasor->b.y);
			// caso o modulo seja nulo, não preenche adiciona ao listview
			// (erro ATAN no calculo do argumento)
			if (IsDoubleZero(abs(fasor_b)))
				continue;
			valoresFasor = lvValores->Items->Add();
			// caso seja S complexo é trifasico
			// if(fasor->atr == lig_S) valoresFasor->Caption = nomeFasor;
			// else                    valoresFasor->Caption = nomeFasor + "_b";
			valoresFasor->Caption = fasor->b.codigo;
			valor = abs(fasor_b);
			// descobre q unidade usar
			if (prefix.allow)
				prefix.simbolo = AvaliaPrefixo(&valor, multInicial);
			valoresFasor->SubItems->Add(DoubleToStr("%4.3f", valor));
			valoresFasor->SubItems->Add(prefix.simbolo + prefix.unidade);
			valoresFasor->SubItems->Add(DoubleToStr("%2.1f", arg(fasor_b)*180 / M_PI) + "º");
		}
		if (fasor->c.enabled)
		{
			fasor_c = complex<double>(fasor->c.x, fasor->c.y);
			// caso o modulo seja nulo, não preenche adiciona ao listview
			// (erro ATAN no calculo do argumento)
			if (IsDoubleZero(abs(fasor_c)))
				continue;
			valoresFasor = lvValores->Items->Add();
			// caso seja S complexo é trifasico
			// if(fasor->atr == lig_S) valoresFasor->Caption = nomeFasor;
			// else                    valoresFasor->Caption = nomeFasor + "_c";
			valoresFasor->Caption = fasor->c.codigo;
			valor = abs(fasor_c);
			// descobre q unidade usar
			if (prefix.allow)
				prefix.simbolo = AvaliaPrefixo(&valor, multInicial);
			valoresFasor->SubItems->Add(DoubleToStr("%4.3f", valor));
			valoresFasor->SubItems->Add(prefix.simbolo + prefix.unidade);
			valoresFasor->SubItems->Add(DoubleToStr("%2.1f", arg(fasor_c)*180 / M_PI) + "º");
		}
	}
	// arruma o tamanho das colunas
	for (int nc = 0; nc < lvValores->Columns->Count; nc++)
	{
		lvValores->Columns->Items[nc]->Width = ColumnTextWidth;
		lvValores->Columns->Items[nc]->Width = ColumnHeaderWidth;
	}
	// lvValores->Items->EndUpdate();
}

// ---------------------------------------------------------------------------
void __fastcall TFormFasometro::TimerTimer(TObject *Sender)
{
	// variáveis locais
	int index;

	// protecao
	if (fasores == NULL)
		return;
	// seleciona próximo patamar
	if ((index = CBoxPatamar->ItemIndex + 1) >= CBoxPatamar->Items->Count)
		index = 0;
	CBoxPatamar->ItemIndex = index;
	// força atualizaçao do Fasor
	CBoxPatamarChange(NULL);
}

// ---------------------------------------------------------------------------
// eof
