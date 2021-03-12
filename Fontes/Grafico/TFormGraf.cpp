// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Math.hpp>
#include "TFormGraf.h"
#include "VTGrafico.h"
#include "VTQuad.h"
#include "VTIcone.h"
#include "VTIcones.h"
#include "TPadrao.h"
#include "..\Apl\VTApl.h"
#include "..\Constante\Const.h"
#include "..\Constante\Cursor.h"
#include "..\Diretorio\VTPath.h"
#include "..\Forms\TFormEdtTxt.h"
#include "..\Cartografia\VTCartografia.h"
#include "..\Cartografia\VTScg.h"
#include "..\Licenca\VTLicenca.h"
#include "..\Licenca\VTLicencas.h"
#include "..\Rede\VTArea.h"
#include "..\Redegraf\VTRedegraf.h"
#include "..\Wizard\VTWizard.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Cartografia.h"
#include "..\..\DLL_Inc\Rede.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
// TFormGraf::TFormGraf() - construtor
// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/18417101
__fastcall TFormGraf::TFormGraf(TForm *Owner, VTGrafico *graf, VTApl *apl) : TForm(Owner)
{
	// variáveis locais
	int         height, width, empresa;
//	VTLicencas *licencas;
//	VTLicenca  *licenca;

	// salva ponteiro p/ objeto
	this->apl  = apl;
	this->graf = graf;
	path       = (VTPath*)apl->GetObject(__classid(VTPath));

   //JCG 2020.03.30 - ateração para compatibilidade com SigPerdas
   //determina empresa
   empresa  = SINAPSIS;
//   licencas = (VTLicencas*)apl->GetObject(__classid(VTLicencas));
//   if(licencas != NULL)
//      {
//      licenca = licencas->ExisteLicenca();
//      empresa = licenca->Empresa;
//      }
	 // inicia tipo de coordenada default
    MenuItemGoogleRoadMap->Checked     = true;
    MenuItemGoogleSatellite->Checked   = false;
    MenuItemGoogleHybrid->Checked      = false;
    MenuItemGoogleTerrain->Checked     = false;
    MenuItemMapaServidorCopel->Checked = false;
    switch(empresa)
	 {
	 case COELBA:
	 case CELPE:
	 case COSERN:
		tipo_coord = coordLATLON;
		MenuItemLatLong->Checked = true;
		MenuItemUTM->Checked = false;
		MenuItemMapaServidorCopel->Visible = false;
		break;
	 case COPEL:
		tipo_coord = coordUTM;
      MenuItemGoogleRoadMap->Checked     = false;
      MenuItemMapaServidorCopel->Checked = true;
      MenuItemMapaServidorCopel->Visible = true;
		break;
	 default:
		MenuItemMapaServidorCopel->Visible = false;
		tipo_coord = coordUTM;
		break;
	 }
	// cria objetos
	rede.area = DLL_NewObjArea();
	rede.zoom = DLL_NewObjArea();
	tela.area = NewObjQuad();
	tela.zoom = NewObjQuad();
	google.area = DLL_NewObjArea();
	lisZOOM = new TList();
	lisBMP = new TList();
	// seta propriedade DoubleBuffered p/ evitar flicker
	DoubleBuffered = true;
	// determina dimensão do maior Monitor
	height = Screen->Monitors[0]->Height;
	width = Screen->Monitors[0]->Width;
	for (int n = 1; n < Screen->MonitorCount; n++)
	{
		if (Screen->Monitors[n]->Height > height)
			height = Screen->Monitors[n]->Height;
		if (Screen->Monitors[n]->Width > width)
			width = Screen->Monitors[n]->Width;
	}
	// limita tamanho do form
	Constraints->MaxHeight = height;
	Constraints->MaxWidth = width;
	// limita tamanho do gráfico
	imag->Constraints->MaxHeight = height;
	imag->Constraints->MaxWidth = width;
	// aloca Bitmap p/ TImage
	image_graph_bmp = new Graphics::TBitmap();
	image_graph_bmp->Height = height;
	image_graph_bmp->Width = width;
	imag->Picture->Graphic = image_graph_bmp;
	// aloca Bitmap p/ tela virtual
	graph_bmp = new Graphics::TBitmap();
	graph_bmp->Height = DIM_TELA_VIRTUAL * height;
	graph_bmp->Width = DIM_TELA_VIRTUAL * width;
	// salva ponteiro p/ o Canvas de graph_bmp
	graph_bmp_canvas = graph_bmp->Canvas;
	// inicia coordenadas da tela virtual
	tela_virtual.x1 = 0;
	tela_virtual.y1 = 0;
	tela_virtual.x2 = graph_bmp->Width - 1;
	tela_virtual.y2 = graph_bmp->Height - 1;
	// define ponto de referência p/ montagem da figura
	tela_virtual.x = (graph_bmp->Width - width) / 2;
	tela_virtual.y = (graph_bmp->Height - height) / 2;
	// assume coordenadas inválidas para área da rede e do zoom
	rede.area->x1 = rede.area->x2 = rede.area->y1 = rede.area->y2 = -1;
	rede.zoom->x1 = rede.zoom->x2 = rede.zoom->y1 = rede.zoom->y2 = -1;
	// limpa a tela do Gráfico, para assumir a cor de fundo
	// LimpaTela();
	// define área da tela
	DefAreaTela();
	// define fator de escala entre área de zoom e tela
	DefEscala();
	// limpa zoom da tela
	// ActionZoomResetExecute(NULL);
	// define operação de seleção de área do mouse
	DefMouseOp(mouseZOOM);
	// cria bitmaps p/ os eqptos
	IniciaBitMaps();
	// cria símbolos vetoriais
	icones = NewObjIcones(this, path->DirIcone());
	// desabilita timer
	Timer->Enabled = false;
}

// ---------------------------------------------------------------------------
// TFormGraf::TFormGraf() - destrutor
// ---------------------------------------------------------------------------
__fastcall TFormGraf::~TFormGraf(void)
{
	// destrói lista e seus objetos criados
	if (lisZOOM)
	{
		LimpaTList(lisZOOM);
		delete lisZOOM;
		lisZOOM = NULL;
	}
	if (lisBMP)
	{
		LimpaTList(lisBMP);
		delete lisBMP;
		lisBMP = NULL;
	}
	// destrói objetos criados
	if (rede.area)
	{
		delete rede.area;
		rede.area = NULL;
	}
	if (rede.area)
	{
		delete rede.area;
		rede.area = NULL;
	}
	if (rede.zoom)
	{
		delete rede.zoom;
		rede.zoom = NULL;
	}
	if (tela.area)
	{
		delete tela.area;
		tela.area = NULL;
	}
	if (tela.zoom)
	{
		delete tela.zoom;
		tela.zoom = NULL;
	}
	if (google.cartografia)
	{
		delete google.cartografia;
		google.cartografia = NULL;
	}
	if (google.area)
	{
		delete google.area;
		google.area = NULL;
	}
	// icones vetoriais dos eqptos
	if (icones)
	{
		delete icones;
		icones = NULL;
	}
	// memória alocada p/ bitmap da tela virtual
	if (graph_bmp)
	{
		delete graph_bmp;
		graph_bmp = NULL;
	}
	// memória alocada p/ bitmap de Timage
	if (image_graph_bmp)
	{
		delete image_graph_bmp;
		image_graph_bmp = NULL;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormGraf::ActionCfgMousePanExecute(TObject *Sender)
{
	DefMouseOp(mouseEXTERNO);
	// altera Action associada ao botão direito do mouse
	mouse.action = ActionCfgMousePan;
}

// ---------------------------------------------------------------------------
void __fastcall TFormGraf::ActionCfgMouseZoomAreaExecute(TObject *Sender)
{
	DefMouseOp(mouseEXTERNO);
	// altera Action associada ao botão direito do mouse
	mouse.action = ActionCfgMouseZoomArea;
}

// ---------------------------------------------------------------------------
void __fastcall TFormGraf::ActionCfgMouseZoomDyExecute(TObject *Sender)
{
	DefMouseOp(mouseEXTERNO);
	// altera Action associada ao botão direito do mouse
	mouse.action = ActionCfgMouseZoomDy;
}

// ---------------------------------------------------------------------------
void __fastcall TFormGraf::ActionTipoCoordenadaExecute(TObject *Sender)
{
	// verifica tipo de coordenada selecionada
	if (MenuItemLatLong->Checked)
	{
		tipo_coord = coordLATLON;
	}
	else
	{
		tipo_coord = coordUTM;
	}
	// atualiza o gráfico
	ZoomDefineArea(rede.zoom->x1, rede.zoom->y1, rede.zoom->x2, rede.zoom->y2);

}
/*
 // ---------------------------------------------------------------------------
 void __fastcall TFormGraf::ActionGravaExecute(TObject *Sender)
 {
 // variáveis locais
 AnsiString arq;
 int id = 1;

 // configura Dialog
 dlgGrava->DefaultExt = GraphicExtension(__classid(Graphics::TBitmap));
 dlgGrava->Filter = GraphicFilter(__classid(Graphics::TBitmap));
 // define diretório inicial
 if (dlgGrava->InitialDir.IsEmpty())
 dlgGrava->InitialDir = path->DirTmp();
 // define nome inicial do arquivo
 do
 {
 arq = "Rede_" + IntToStr(id++) + "." + dlgGrava->DefaultExt;
 }
 while (FileExists(dlgGrava->InitialDir + "\\" + arq));
 dlgGrava->FileName = arq;
 if (dlgGrava->Execute())
 imag->Picture->SaveToFile(dlgGrava->FileName);
 }
 */

// ---------------------------------------------------------------------------
void __fastcall TFormGraf::ActionGravaExecute(TObject *Sender)
{
	// variáveis locais
	AnsiString arq, filename;
	int id = 1;

	// configura Dialog
	dlgGrava->DefaultExt = GraphicExtension(__classid(Graphics::TBitmap));
	dlgGrava->Filter = GraphicFilter(__classid(Graphics::TBitmap));
	// define diretório inicial
	if (dlgGrava->InitialDir.IsEmpty())
		dlgGrava->InitialDir = path->DirTmp();
	// define nome inicial do arquivo
	do
	{
		arq = "Rede_" + IntToStr(id++) + "." + dlgGrava->DefaultExt;
	}
	while (FileExists(dlgGrava->InitialDir + "\\" + arq));
	dlgGrava->FileName = arq;
	if (dlgGrava->Execute())
	{
		filename = dlgGrava->FileName;
		if (FileExists(filename))
		{
			if (Confirma("O arquivo " + filename + " já existe.", "Deseja substitui-lo ?") == idYes)
			{
				ExportaBmp(filename);
			}
		}
		ExportaBmp(filename);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormGraf::ActionMapaExecute(TObject *Sender)
{
	// valores deste form
	// int w = tela.area->Width();
	// int h = tela.area->Height();

	// int w = imag->Width;
	// int h = imag->Height;

	// verifica se existe o objeto Mapa
	if (google.cartografia == NULL)
	{ // altera Hint da ActionMapa
		ActionMapa->Hint = "Não exibir cartografia";
		// cria objeto mapa
		google.cartografia = DLL_NewObjCartografia(apl, Parent, ActionMapaDisp);
		// google.cartografia->Redimensiona(w, h);
		// exibe StatusBar
		// sttbar->Visible = true;
		// aumenta espessura da linha p/ destacar traçado da rede
		graf->Padrao->ligacao.dim = 5;
		// muda cursor do mouse p/ ampulheta
		// ImageCursorMouse(crHourGlass);
		// OBS: Mapa executa ActionMapaDisp quando site estiver iniciado
		// simula alteração da área de zoom p/ carregar a área do Mapa
		ZoomDefineArea(rede.zoom->x1, rede.zoom->y1, rede.zoom->x2, rede.zoom->y2);
		// habilita PopupMenuMapa
		ButMapa->PopupMenu = PopupMenuMapa;
	}
	else
	{ // altera Hint
		ActionMapa->Hint = "Exibir cartografia";
		// elimina Mapa
		delete google.cartografia;
		google.cartografia = NULL;
		// esconde StatusBar
		// sttbar->Visible = false;
		// restaura espessura da linha p/ desenhar traçado da rede
		graf->Padrao->ligacao.dim = 1;
		// força atualização do gráfico
		FormResize(NULL);
		// desabilita PopupMenuMapa
		ButMapa->PopupMenu = NULL;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormGraf::ActionMapaDispExecute(TObject *Sender)
{
	// área do Mapa disponível: reapresenta a rede
	graf->Redraw();
	// mudar cursor do mouse p/ ampulheta
	imag->Cursor = crDefault;
}

// ---------------------------------------------------------------------------
void __fastcall TFormGraf::ActionMapaZonaUtmExecute(TObject *Sender)
{
	// variáveis locais
	TFormEdtTxt *FormEdtTxt;

	// proteção
	if (google.cartografia == NULL)
		return;
	// pede para usuário selecionar nova Zona
	FormEdtTxt = new TFormEdtTxt(this, "Defina Zona UTM");
	FormEdtTxt->Label->Caption = "Zona";
	FormEdtTxt->Texto = google.cartografia->ZonaUTM;
	if (FormEdtTxt->ShowModal() == mrOk)
	{ // redefine zona utm
		google.cartografia->ZonaUTM = FormEdtTxt->Texto;
		// atualiza o gráfico
		ZoomDefineArea(rede.zoom->x1, rede.zoom->y1, rede.zoom->x2, rede.zoom->y2);
	}
	// destrói FormEdtTxt
	delete FormEdtTxt;
}

// ---------------------------------------------------------------------------
void __fastcall TFormGraf::ActionOpcaoGoogleExecute(TObject *Sender)
{
	// altera atributo Checked
	// MenuItemMapaEscalaGoogle->Checked = ! MenuItemMapaEscalaGoogle->Checked;
	// atualiza o gráfico
	ZoomDefineArea(rede.zoom->x1, rede.zoom->y1, rede.zoom->x2, rede.zoom->y2);
	// graf->Redraw();
}

// ---------------------------------------------------------------------------
void __fastcall TFormGraf::ActionWizardExecute(TObject *Sender)
{
	// habilita/desabilita Wizard
	if (wizard != NULL)
	{
		wizard = NULL;
	}
	else
	{
		wizard = (VTWizard*)apl->GetObject(__classid(VTWizard));
	}
	ActionWizard->Hint = (wizard == NULL) ? "Habilitar 'Wizard'" : "Desabilitar 'Wizard'";
	ActionWizard->ImageIndex = (wizard != NULL) ? 7 : 8;
}

// ---------------------------------------------------------------------------
void __fastcall TFormGraf::ActionZoomAnteriorExecute(TObject *Sender)
{
	// variáveis locais
	VTArea *area;

	// proteção
	if (lisZOOM->Count == 0)
	{
		ActionZoomAnterior->Enabled = false;
		return;
	}
	// determina última área de zoom
	area = (VTArea*)lisZOOM->Last();
	lisZOOM->Delete(lisZOOM->Count - 1);
	// se último zoom, garante área da rede
	if (lisZOOM->Count == 0)
		area->CopiaAtributosDe(*rede.area);
	// controla habilitação de botão de zoom
	ActionZoomAnterior->Enabled = (lisZOOM->Count > 0);
	// redefine área de zoom
	ZoomDefineArea(area->x1, area->y1, area->x2, area->y2);
	// destrói objeto
	delete area;
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18230648
void __fastcall TFormGraf::ActionZoomInExecute(TObject *Sender)
{
	// variáveis locais
	double fat = 0.25;//0.005;
	int dx = (janela_virtual.x2 - janela_virtual.x1 + 1) * fat;
	int dy = (janela_virtual.y2 - janela_virtual.y1 + 1) * fat;

	// redefine coordenadas da janela virtual
	JanelaVirtualDefineArea(janela_virtual.x1 + dx, janela_virtual.y1 + dy, janela_virtual.x2 - dx,
		janela_virtual.y2 - dy);

}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18230648
void __fastcall TFormGraf::ActionZoomOutExecute(TObject *Sender)
{
	// variáveis locais
	double fat = 0.25;//0.005;
	int dx = (janela_virtual.x2 - janela_virtual.x1 + 1) * fat;
	int dy = (janela_virtual.y2 - janela_virtual.y1 + 1) * fat;

	// redefine coordenadas da janela virtual
	JanelaVirtualDefineArea(janela_virtual.x1 - dx, janela_virtual.y1 - dy, janela_virtual.x2 + dx,
		janela_virtual.y2 + dy);
}

// ---------------------------------------------------------------------------
void __fastcall TFormGraf::ActionZoomResetExecute(TObject *Sender)
{
	// limpa os últimos zoom
	LimpaTList(lisZOOM);
	// desabilita ActionZoomAnterior
	ActionZoomAnterior->Enabled = false;
	// redefine a área de zoom para ser igual à área total da rede
	ZoomDefineArea(rede.area->x1, rede.area->y1, rede.area->x2, rede.area->y2);
}
//---------------------------------------------------------------------------
void   __fastcall  TFormGraf::AlteraFuncaoBotaoDireito(void)
{
	if (ButMouseZoomArea->Down)
	{
		ButMouseZoomArea->Down = false;
		ButMouseZoomDy->Down = true;
	}
	else if (ButMouseZoomDy->Down)
	{
		ButMouseZoomDy->Down = false;
		ButMousePan->Down = true;
	}
	else if (ButMousePan->Down)
	{
		ButMousePan->Down = false;
		ButMouseZoomArea->Down = true;
	}
    Application->ProcessMessages();
}
 //---------------------------------------------------------------------------
/*
 //---------------------------------------------------------------------------
 void __fastcall TFormGraf::AltAreaRede(VTArea *area)
 {
 //IMPORTANTE:
 //ao editar a primeira barra, a nova área da rede pode não conter a área anterior

 //proteção: verifica se área á válida
 if ((area->x1 < 0)||(area->y1 < 0)) return;
 //proteção
 //if ((area->x1 == rede.area->x1)&&(area->x2 == rede.area->x2)&&
 //    (area->y1 == rede.area->y1)&&(area->y2 == rede.area->y2)) return;
 //verifica se a nova área foi aumentada em todos os lados
 if ((area->x1 < rede.area->x1)&&(area->x2 > rede.area->x2)&&
 (area->y1 < rede.area->y1)&&(area->y2 > rede.area->y2))
 {//cancela todos os Zoom anteriores
 LimpaTList(lisZOOM);
 ActionZoomAnterior->Enabled = false;
 //redefine área da rede
 *rede.area = *area;
 //redefine área de zoom igual à nova área
 ZoomDefineArea(rede.area->x1, rede.area->y1, rede.area->x2, rede.area->y2);
 }
 //verifica se a nova área contém a área anterior
 else if ((area->x1 <= rede.area->x1)&&(area->x2 >= rede.area->x2)&&
 (area->y1 <= rede.area->y1)&&(area->y2 >= rede.area->y2))
 {//área da rede foi aumentada: verifica se há zoom
 if (lisZOOM->Count == 0)
 {//não há zoom: salva zoom c/ a área atual
 ZoomSalva();
 }
 //redefine área da rede
 *rede.area = *area;
 //redefine mesmma área de zoom (apenas p/ redesenhar o gráfico)
 ZoomDefineArea(rede.zoom->x1, rede.zoom->y1, rede.zoom->x2, rede.zoom->y2);
 }
 else
 {//cancela todos os Zoom anteriores
 LimpaTList(lisZOOM);
 ActionZoomAnterior->Enabled = false;
 //redefine área da rede
 *rede.area = *area;
 //redefine área de zoom
 ZoomDefineArea(rede.area->x1, rede.area->y1, rede.area->x2, rede.area->y2);
 }
 }
 */
/*
 //---------------------------------------------------------------------------
 void __fastcall TFormGraf::AltAreaRede(VTArea *area)
 {
 //IMPORTANTE:
 //ao editar a primeira barra, a nova área da rede pode não conter a área anterior

 //proteção: verifica se área é válida
 if ((area->x1 < 0)||(area->y1 < 0)) return;
 //proteção
 //if ((area->x1 == rede.area->x1)&&(area->x2 == rede.area->x2)&&
 //    (area->y1 == rede.area->y1)&&(area->y2 == rede.area->y2)) return;
 //verifica se a nova área contém a área anterior
 if ((area->x1 <= rede.area->x1)&&(area->x2 >= rede.area->x2)&&
 (area->y1 <= rede.area->y1)&&(area->y2 >= rede.area->y2))
 {//verifica se área de zoom é igual à área da rede anterior
 if (*(rede.area) == *(rede.zoom))
 {//redefine área da rede
 *rede.area = *area;
 //redefine área de zoom igual à nova área
 ZoomDefineArea(rede.area->x1, rede.area->y1, rede.area->x2, rede.area->y2);
 }
 else
 {//calcula variação entre área nova e anterior
 int dx1 = abs(area->x1 - rede.area->x1);
 int dx2 = abs(area->x2 - rede.area->x2);
 int dy1 = abs(area->y1 - rede.area->y1);
 int dy2 = abs(area->y2 - rede.area->y2);
 //redefine área da rede
 *rede.area = *area;
 //verifica se a variação de área foi uniforme
 if ((dx1 == dx2) &&(dy1 == dy2))
 {//redefine área de zoom
 ZoomDefineArea(rede.zoom->x1 - dx1, rede.zoom->y1 - dy1,
 rede.zoom->x2 + dx2, rede.zoom->y2 + dy2);
 }
 else
 {//redesenha o gráfico
 graf->Redraw();
 }
 }
 }
 else
 {//cancela todos os Zoom anteriores
 LimpaTList(lisZOOM);
 ActionZoomAnterior->Enabled = false;
 //redefine área da rede
 *rede.area = *area;
 //redefine área de zoom
 ZoomDefineArea(rede.area->x1, rede.area->y1, rede.area->x2, rede.area->y2);
 }
 }
 */
/*
 //---------------------------------------------------------------------------
 void __fastcall TFormGraf::AltAreaRede(VTArea *area)
 {
 //IMPORTANTE:
 //ao editar a primeira barra, a nova área da rede pode não conter a área anterior

 //proteção: verifica se área é válida
 if ((area->x1 < 0)||(area->y1 < 0)) return;
 //proteção
 //if ((area->x1 == rede.area->x1)&&(area->x2 == rede.area->x2)&&
 //    (area->y1 == rede.area->y1)&&(area->y2 == rede.area->y2)) return;
 //verifica se a nova área contém a área anterior
 if ((area->x1 <= rede.area->x1)&&(area->x2 >= rede.area->x2)&&
 (area->y1 <= rede.area->y1)&&(area->y2 >= rede.area->y2))
 {//verifica se área de zoom é igual à área da rede anterior
 if (*(rede.area) == *(rede.zoom))
 {//redefine área da rede
 *rede.area = *area;
 //verifica se houve deslocamento da janela virtual (pan)
 if ((janela_virtual.x1 == tela_virtual.x)&&(janela_virtual.y1 == tela_virtual.y))
 {//não houve pan: redefine área de zoom igual à nova área
 ZoomDefineArea(rede.area->x1, rede.area->y1, rede.area->x2, rede.area->y2);
 }
 else
 {//houve pan: redesenha o gráfico
 graf->Redraw();
 }
 }
 else
 {//calcula variação entre área nova e anterior
 int dx1 = abs(area->x1 - rede.area->x1);
 int dx2 = abs(area->x2 - rede.area->x2);
 int dy1 = abs(area->y1 - rede.area->y1);
 int dy2 = abs(area->y2 - rede.area->y2);
 //verifica se a variação de área foi uniforme
 if ((dx1 == dx2) &&(dy1 == dy2))
 {//calcula variação proporcional p/ área de zoom
 double a = rede.area->Width()  / area->Width();
 double b = rede.area->Height() / area->Height();
 dx1 = dx2 = (rede.zoom->Width()  / 2.) * (1. - (rede.area->Width()  / area->Width()));
 dy1 = dy2 = (rede.zoom->Height() / 2.) * (1. - (rede.area->Height() / area->Height()));
 //redefine área da rede
 *rede.area = *area;
 //dx1 = dx2 = rede.zoom->Width() * 0.05;
 //dy1 = dy2 = rede.zoom->Height() * 0.05;
 //redefine área de zoom
 ZoomDefineArea(rede.zoom->x1 - dx1, rede.zoom->y1 - dy1,
 rede.zoom->x2 + dx2, rede.zoom->y2 + dy2);
 }
 else
 {//redefine área da rede
 *rede.area = *area;
 //redesenha o gráfico
 graf->Redraw();
 }
 }
 }
 else
 {//cancela todos os Zoom anteriores
 LimpaTList(lisZOOM);
 ActionZoomAnterior->Enabled = false;
 //redefine área da rede
 *rede.area = *area;
 //redefine área de zoom
 ZoomDefineArea(rede.area->x1, rede.area->y1, rede.area->x2, rede.area->y2);
 }
 }

 //---------------------------------------------------------------------------
 void __fastcall TFormGraf::AltAreaRede(VTArea *area)
 {
 //variáveis locais
 VTRedegraf *redegraf = (VTRedegraf*)apl->GetObject(__classid(VTRedegraf));
 //IMPORTANTE:
 //ao editar a primeira barra, a nova área da rede pode não conter a área anterior

 //proteção: verifica se área é válida
 if ((area->x1 < 0)||(area->y1 < 0)) return;
 //verifica o modo gráfico
 if (redegraf->ModoGrafico == modoGEOREFERENCIADO)
 {//calcula variação entre área nova e anterior
 int dx1 = abs(area->x1 - rede.area->x1);
 int dx2 = abs(area->x2 - rede.area->x2);
 int dy1 = abs(area->y1 - rede.area->y1);
 int dy2 = abs(area->y2 - rede.area->y2);
 //verifica se a variação de área foi uniforme
 if ((dx1 == dx2) &&(dy1 == dy2))
 {//redefine área da rede
 *rede.area = *area;
 //redefine área de zoom igual à nova área
 ZoomDefineArea(rede.area->x1, rede.area->y1, rede.area->x2, rede.area->y2);
 }
 else
 {//redefine área da rede
 *rede.area = *area;
 //redesenha o gráfico
 graf->Redraw();
 }
 }
 else
 {//redesenha o gráfico
 graf->Redraw();
 }
 }
 */
/*
 //---------------------------------------------------------------------------
 void __fastcall TFormGraf::AltAreaRede(VTArea *area)
 {
 //IMPORTANTE:
 //ao editar a primeira barra, a nova área da rede pode não conter a área anterior

 //proteção: verifica se área é válida
 if ((area->x1 < 0)||(area->y1 < 0)) return;
 //calcula variação entre área nova e anterior
 int dx1 = abs(area->x1 - rede.area->x1);
 int dx2 = abs(area->x2 - rede.area->x2);
 int dy1 = abs(area->y1 - rede.area->y1);
 int dy2 = abs(area->y2 - rede.area->y2);
 //verifica se a variação de área foi uniforme
 if ((dx1 > 0)&&(dx1 == dx2)&&
 (dy1 > 0)&&(dy1 == dy2))
 {//redefine área da rede
 *rede.area = *area;
 //redefine área de zoom igual à nova área
 ZoomDefineArea(rede.area->x1, rede.area->y1, rede.area->x2, rede.area->y2);
 }
 else
 {//redefine área da rede
 *rede.area = *area;
 //redesenha o gráfico
 graf->Redraw();
 }
 }
 */
/*
 //---------------------------------------------------------------------------
 void __fastcall TFormGraf::AreaRedeAltera(VTArea *area)
 {
 //IMPORTANTE:
 //ao editar a primeira barra, a nova área da rede pode não conter a área anterior

 //proteção: verifica se área é válida
 if ((area->x1 < 0)||(area->y1 < 0)) return;
 //redefine área da rede
 *rede.area = *area;
 //verifica se a área foi reiniciada
 if (reiniciada)
 {//redefine área de zoom igual à nova área
 ZoomDefineArea(rede.area->x1, rede.area->y1, rede.area->x2, rede.area->y2);
 }
 else
 {//redesenha o gráfico
 graf->Redraw();
 }
 }
 */
// ---------------------------------------------------------------------------
void __fastcall TFormGraf::AreaRedeAltera(VTArea *area)
{
	// IMPORTANTE:
	// ao editar a primeira barra, a nova área da rede pode não conter a área anterior

	// proteção: verifica se área é válida
	// bloqueado em 2013.07.11
	// if ((area->x1 < 0)||(area->y1 < 0)) return;
	// redefine área da rede
	rede.area->CopiaAtributosDe(*area);
	// redesenha o gráfico
	graf->Redraw();
}

// --------------------------------------------------------------------------
void __fastcall TFormGraf::AreaTotal(VTArea *pArea)
{
	// variáveis locais
	struct
	{
		int x1, y1, x2, y2;
	} area_zoom;

	// verifica se há uma área de zoom
	if (lisZOOM->Count == 0)
	{ // calcula área total
		AreaVisivel(pArea);
		return;
	}
	// salva coordenadas da área de zoom
	area_zoom.x1 = rede.zoom->x1;
	area_zoom.y1 = rede.zoom->y1;
	area_zoom.x2 = rede.zoom->x2;
	area_zoom.y2 = rede.zoom->y2;
	// redefine área de zoom igual à área da rede
	rede.zoom->CopiaAtributosDe(*rede.area);
	// reinicia a área da tela real e da janela virtual
	DefAreaTela();
	// redefine fator de escala entre área de zoom e tela
	DefEscala();
	// calcula área total
	AreaVisivel(pArea);
	// restaura área de zoom
	rede.zoom->x1 = area_zoom.x1;
	rede.zoom->y1 = area_zoom.y1;
	rede.zoom->x2 = area_zoom.x2;
	rede.zoom->y2 = area_zoom.y2;
	// reinicia a área da tela real e da janela virtual
	DefAreaTela();
	// redefine fator de escala entre área de zoom e tela
	DefEscala();
}

// ---------------------------------------------------------------------------
void __fastcall TFormGraf::AreaVisivel(VTArea *pArea)
{
	// variáveis locais
	int x1 = Xpixel_to_Xutm(tela.area->x1);
	int y1 = Ypixel_to_Yutm(tela.area->y1);
	int x2 = Xpixel_to_Xutm(tela.area->x2);
	int y2 = Ypixel_to_Yutm(tela.area->y2);

	pArea->x1 = Min(x1, x2);
	pArea->y1 = Min(y1, y2);
	pArea->x2 = Max(x1, x2);
	pArea->y2 = Max(y1, y2);
}

// ---------------------------------------------------------------------------
void __fastcall TFormGraf::AreaZoom(VTArea *pArea)
{
	pArea->CopiaAtributosDe(*rede.zoom);
}

// ---------------------------------------------------------------------------
void __fastcall TFormGraf::AreaRedeDefine(VTArea *area)
{
	// cancela zoom
	lisZOOM->Clear();
	// redefine área da rede
	rede.area->CopiaAtributosDe(*area);
	// redefine área de zoom da rede igual à área da rede
	ZoomDefineArea(area->x1, area->y1, area->x2, area->y2);
}

// ---------------------------------------------------------------------------
void __fastcall TFormGraf::DefAreaTela(void)
{
	// determina área da tela real (imag)
	tela.area->x1 = 0;
	tela.area->y1 = 0;
	tela.area->x2 = ClientWidth; // - 1;
	tela.area->y2 = ClientHeight - imag->Top; // - 1;
	// ajusta y2 em função de sttbar estar visível
	if (sttbar->Visible)
		tela.area->y2 -= sttbar->Height;
	// atualiza área da janela virtual mapeada na tela real
	janela_virtual.x1 = tela_virtual.x;
	janela_virtual.y1 = tela_virtual.y;
	janela_virtual.x2 = janela_virtual.x1 + tela.area->Width() - 1;
	janela_virtual.y2 = janela_virtual.y1 + tela.area->Height() - 1;
	// inicia fator de escala entre janela virtual e área da tela real
	janela_virtual.fatx = 1.;
	janela_virtual.faty = 1.;
}

// ---------------------------------------------------------------------------
void __fastcall TFormGraf::DefEscala(void)
{
	// variáveis locais
	double width, height;
	double xmin, xmax, ymin, ymax;

	// verifica se a área de zoom foi definida
	if (rede.zoom->x1 == -1)
	{ // área de zoom não foi definida
		escala.fatx = escala.faty = 1.0;
		return;
	}
	// área de zoom definida: proteção contra dimensão nula
	width = rede.zoom->Width();
	height = rede.zoom->Height();
	if (IsDoubleZero(width))
		width = 1;
	if (IsDoubleZero(height))
		height = 1;
	// calcula fator de escala
	escala.fatx = tela.area->Width() / width;
	escala.faty = tela.area->Height() / height;
	// proteção
	if (escala.fatx == 0)
		escala.fatx = 1.0;
	if (escala.faty == 0)
		escala.faty = 1.0;
	// assume a menor escala para x e y
	if (escala.fatx < escala.faty)
		escala.faty = escala.fatx;
	if (escala.faty < escala.fatx)
		escala.fatx = escala.faty;
	// determina coordenadas (x0,y0) do ponto de referência da tela
	tela.x0 = tela.area->x1 + ((tela.area->Width() - (width * escala.fatx)) / 2);
	tela.y0 = tela.area->y1 + ((tela.area->Height() - (height * escala.faty)) / 2);
	// verifica limite para zoom
	// determina coordenadas limites da área, mapeadas na tela
	xmin = Xutm_to_Xpixel(rede.area->x1);
	xmax = Xutm_to_Xpixel(rede.area->x2);
	ymin = Yutm_to_Ypixel(rede.area->y1);
	ymax = Yutm_to_Ypixel(rede.area->y2);
	if ((xmin > -LIMZOOM) && (xmax < LIMZOOM) && (ymin > -LIMZOOM) && (ymax < LIMZOOM))
		escala.overzoom = false;
	else
	{ // excedeu limite de zoom
		escala.overzoom = true;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormGraf::DefMouseOp(int op)
{
	// define modo de seleção mouse
	if ((op < mouseZOOM) || (op > mouseEXTERNO))
		op = mouseZOOM;
	mouse.op = op;
}

/*
 //---------------------------------------------------------------------------
 // TFormGraf::DefObjGraf()  -  public
 //---------------------------------------------------------------------------
 void __fastcall TFormGraf::DefObjGraf(VTGrafico *graf)
 {
 //salva ponteiro para objeto VTGrafico
 this->graf = graf;
 }
 */
// -----------------------------------------------------------------------------
void __fastcall TFormGraf::DefineParent(TWinControl *parent)
{
	// insere o Form em seu Parent
	Parent = parent;
	Align = alClient;
}

// ---------------------------------------------------------------------------
// TFormGraf::DrawBmpCap()  -  public
// ---------------------------------------------------------------------------
void __fastcall TFormGraf::DrawBitMap(int ind_bmp, int x, int y, int dx, int dy)
{
	// variáveis locais
	TPicture *bmp;

	// proteção
	if (ind_bmp >= lisBMP->Count)
		return;
	// determina bitmap
	bmp = (TPicture*)lisBMP->Items[ind_bmp];
	// converte coordenadas da barra para coordenadas da tela
	x = Xutm_to_Xpixel(x) + ((dx * bmp->Width) / 100);
	y = Yutm_to_Ypixel(y) + ((dy * bmp->Height) / 100);
	if (escala.overzoom) // zoom excedido: protege coordenadas
	{
		if ((x < -LIMZOOM) || (x > LIMZOOM))
			return;
		if ((y < -LIMZOOM) || (y > LIMZOOM))
			return;
	}
	// desenha o bitmap
	graph_bmp_canvas->Draw(x, y, bmp->Graphic);
}

// ---------------------------------------------------------------------------
// TFormGraf::DrawBmpCap()  -  public
// ---------------------------------------------------------------------------
void __fastcall TFormGraf::DrawBitMap(int ind_bmp, int x, int y, int dx, int dy, AnsiString txt,
	TColor cor)
{
	// variáveis locais
	TPicture *bmp;

	// proteção
	if (ind_bmp >= lisBMP->Count)
		return;
	// determina bitmap
	bmp = (TPicture*)lisBMP->Items[ind_bmp];
	// converte coordenadas da barra para coordenadas da tela
	x = Xutm_to_Xpixel(x) + ((dx * bmp->Width) / 100);
	y = Yutm_to_Ypixel(y) + ((dy * bmp->Height) / 100);
	if (escala.overzoom) // zoom excedido: protege coordenadas
	{
		if ((x < -LIMZOOM) || (x > LIMZOOM))
			return;
		if ((y < -LIMZOOM) || (y > LIMZOOM))
			return;
	}
	// desenha o bitmap
	// TRect r(x-5, y-5, x+5, y+5);
	// graph_bmp_canvas->StretchDraw(r, bmp->Graphic);
	graph_bmp_canvas->Draw(x, y, bmp->Graphic);
	// apresenta o texto abaixo do bitmap
	graph_bmp_canvas->Pen->Width = 1;
	graph_bmp_canvas->Pen->Color = cor;
	// graph_bmp_canvas->Brush->Style = bsSolid;
	graph_bmp_canvas->Brush->Style = bsClear;
	// graph_bmp_canvas->Brush->Color = tela.cor_fundo;
	graph_bmp_canvas->Font->Color = cor;
	graph_bmp_canvas->TextOut(x, y + bmp->Height + 3, txt);
	graph_bmp_canvas->Font->Color = tela.cor_fundo; // ATENÇÃO: NÃO ELIMINAR ESTA LINHA
}

// ---------------------------------------------------------------------------
void __fastcall TFormGraf::DrawBitMap_XYpixel(int ind_bmp, int x, int y, int dx, int dy)
{
	// variáveis locais
	TPicture *bmp;

	// proteção
	if (ind_bmp >= lisBMP->Count)
		return;
	// determina bitmap
	bmp = (TPicture*)lisBMP->Items[ind_bmp];
	// ajusta coordenadas
	x += ((dx * bmp->Width) / 100);
	y += ((dy * bmp->Height) / 100);
	if (escala.overzoom) // zoom excedido: protege coordenadas
	{
		if ((x < -LIMZOOM) || (x > LIMZOOM))
			return;
		if ((y < -LIMZOOM) || (y > LIMZOOM))
			return;
	}
	// desenha o bitmap
	// TRect r(x-5, y-5, x+5, y+5);
	// graph_bmp_canvas->StretchDraw(r, bmp->Graphic);
	graph_bmp_canvas->Draw(x, y, bmp->Graphic);
}

// ---------------------------------------------------------------------------
void __fastcall TFormGraf::DrawBitMap_XYpixel(int ind_bmp, int x, int y, int dx, int dy,
	AnsiString txt, TColor cor)
{
	// variáveis locais
	TPicture *bmp;

	// proteção
	if (ind_bmp >= lisBMP->Count)
		return;
	// determina bitmap
	bmp = (TPicture*)lisBMP->Items[ind_bmp];
	// ajusta coordenadas
	x += ((dx * bmp->Width) / 100);
	y += ((dy * bmp->Height) / 100);
	if (escala.overzoom) // zoom excedido: protege coordenadas
	{
		if ((x < -LIMZOOM) || (x > LIMZOOM))
			return;
		if ((y < -LIMZOOM) || (y > LIMZOOM))
			return;
	}
	// desenha o bitmap
	// TRect r(x-5, y-5, x+5, y+5);
	// graph_bmp_canvas->StretchDraw(r, bmp->Graphic);
	graph_bmp_canvas->Draw(x, y, bmp->Graphic);
	// apresenta o texto abaixo do bitmap
	graph_bmp_canvas->Pen->Width = 1;
	graph_bmp_canvas->Pen->Color = cor;
	// graph_bmp_canvas->Brush->Style = bsSolid;
	graph_bmp_canvas->Brush->Style = bsClear;
	// graph_bmp_canvas->Brush->Color = tela.cor_fundo;
	graph_bmp_canvas->Font->Color = cor;
	graph_bmp_canvas->TextOut(x, y + bmp->Height + 3, txt);
	graph_bmp_canvas->Font->Color = tela.cor_fundo; // ATENÇÃO: NÃO ELIMINAR ESTA LINHA
}

// ---------------------------------------------------------------------------
void __fastcall TFormGraf::DrawCirculo(int x, int y, int dim, TColor cor)
{
	// define cor da linha
	graph_bmp_canvas->Pen->Width = 1;
	graph_bmp_canvas->Pen->Color = cor;
	graph_bmp_canvas->Brush->Style = bsSolid;
	graph_bmp_canvas->Brush->Color = cor;

	// converte coordenadas da barra para coordenadas da tela
	x = Xutm_to_Xpixel(x);
	y = Yutm_to_Ypixel(y);
	if (escala.overzoom) // zoom excedido: protege coordenadas
	{
		if ((x < -LIMZOOM) || (x > LIMZOOM))
			return;
		if ((y < -LIMZOOM) || (y > LIMZOOM))
			return;
	}
	graph_bmp_canvas->Ellipse(x - dim, y - dim, x + dim, y + dim);
}

// ---------------------------------------------------------------------------
void __fastcall TFormGraf::DrawCirculo(int x, int y, int dim, TColor cor_ext, TColor cor_int)
{
	// define cor da linha
	graph_bmp_canvas->Pen->Width = 1;
	graph_bmp_canvas->Pen->Color = cor_ext;
	graph_bmp_canvas->Brush->Style = bsSolid;
	graph_bmp_canvas->Brush->Color = cor_int;

	// converte coordenadas da barra para coordenadas da tela
	x = Xutm_to_Xpixel(x);
	y = Yutm_to_Ypixel(y);
	if (escala.overzoom) // zoom excedido: protege coordenadas
	{
		if ((x < -LIMZOOM) || (x > LIMZOOM))
			return;
		if ((y < -LIMZOOM) || (y > LIMZOOM))
			return;
	}
	graph_bmp_canvas->Ellipse(x - dim, y - dim, x + dim, y + dim);
}

// ---------------------------------------------------------------------------
void __fastcall TFormGraf::DrawCirculo_XYpixel(int x, int y, int dim, TColor cor_ext,
	TColor cor_int)
{
	// define cor da linha
	graph_bmp_canvas->Pen->Width = 1;
	graph_bmp_canvas->Pen->Color = cor_ext;
	graph_bmp_canvas->Brush->Style = bsSolid;
	graph_bmp_canvas->Brush->Color = cor_int;
	if (escala.overzoom) // zoom excedido: protege coordenadas
	{
		if ((x < -LIMZOOM) || (x > LIMZOOM))
			return;
		if ((y < -LIMZOOM) || (y > LIMZOOM))
			return;
	}
	graph_bmp_canvas->Ellipse(x - dim, y - dim, x + dim, y + dim);
}

// ---------------------------------------------------------------------------
void __fastcall TFormGraf::DrawCirculoXor(int x, int y, int raio)
{
	// variáveis locais
	TPenMode orig_mode;

	// converte coordenadas p/ coordenadas da tela
	x = Xutm_to_Xpixel(x);
	y = Yutm_to_Ypixel(y);
	if (escala.overzoom) // zoom excedido: protege coordenadas
	{
		if ((x < -LIMZOOM) || (x > LIMZOOM))
			return;
		if ((y < -LIMZOOM) || (y > LIMZOOM))
			return;
	}
	// salva e altera PenMode
	orig_mode = graph_bmp_canvas->Pen->Mode;
	// define padrão da linha
	graph_bmp_canvas->Pen->Mode = pmXor;
	graph_bmp_canvas->Pen->Color = (tela.cor_fundo == clBlack) ? clWhite : clGray;
	graph_bmp_canvas->Pen->Width = 1;
	graph_bmp_canvas->Brush->Style = bsClear;
	// escreve o texto
	graph_bmp_canvas->Ellipse(x - raio, y - raio, x + raio, y + raio);
	// restaura PenMode
	graph_bmp_canvas->Pen->Mode = orig_mode;
}

// ---------------------------------------------------------------------------
void __fastcall TFormGraf::DrawCirculoXor(int x1, int y1, int x2, int y2, int pos, int raio)
{
	// variáveis locais
	int x, y;
	TPenMode orig_mode;

	// converte coordenadas p/ coordenadas da tela
	x1 = Xutm_to_Xpixel(x1);
	y1 = Yutm_to_Ypixel(y1);
	x2 = Xutm_to_Xpixel(x2);
	y2 = Yutm_to_Ypixel(y2);
	// calcula coord. do centro do círculo
	x = x1 + (((x2 - x1) * pos) / 100);
	y = y1 + (((y2 - y1) * pos) / 100);
	if (escala.overzoom) // zoom excedido: protege coordenadas
	{
		if ((x < -LIMZOOM) || (x > LIMZOOM))
			return;
		if ((y < -LIMZOOM) || (y > LIMZOOM))
			return;
	}
	// salva e altera PenMode
	orig_mode = graph_bmp_canvas->Pen->Mode;
	// define padrão da linha
	graph_bmp_canvas->Pen->Mode = pmXor;
	graph_bmp_canvas->Pen->Color = (tela.cor_fundo == clBlack) ? clWhite : clGray;
	graph_bmp_canvas->Pen->Width = 1;
	graph_bmp_canvas->Brush->Style = bsClear;
	// escreve o texto
	graph_bmp_canvas->Ellipse(x - raio, y - raio, x + raio, y + raio);
	// restaura PenMode
	graph_bmp_canvas->Pen->Mode = orig_mode;
}

// ---------------------------------------------------------------------------
// TFormGraf::DrawCircunferencia()  -  public
// ---------------------------------------------------------------------------
void __fastcall TFormGraf::DrawCircunferencia(int x, int y, int dim, TColor cor)
{
	// verifica se exibição de barras está bloqueada
	if (dim == 0)
		return;
	// define cor da linha
	graph_bmp_canvas->Pen->Width = 1;
	graph_bmp_canvas->Pen->Color = cor;
	graph_bmp_canvas->Brush->Style = bsClear;
	// OBS: Para desenhar apenas a circunferência, NÃO DEFINIR
	// graph_bmp_canvas->Brush->Color após definir
	// graph_bmp_canvas->Brush->Style
	// graph_bmp_canvas->Brush->Color

	// converte coordenadas da barra para coordenadas da tela
	x = Xutm_to_Xpixel(x);
	y = Yutm_to_Ypixel(y);
	if (escala.overzoom) // zoom excedido: protege coordenadas
	{
		if ((x < -LIMZOOM) || (x > LIMZOOM))
			return;
		if ((y < -LIMZOOM) || (y > LIMZOOM))
			return;
	}
	graph_bmp_canvas->Ellipse(x - dim, y - dim, x + dim, y + dim);
}

// ---------------------------------------------------------------------------
void __fastcall TFormGraf::DrawCruz(int x, int y, int dim, TColor cor)
{
	// converte coordenadas da barra para coordenadas da tela
	x = Xutm_to_Xpixel(x);
	y = Yutm_to_Ypixel(y);
	if (escala.overzoom) // zoom excedido: protege coordenadas
	{
		if ((x < -LIMZOOM) || (x > LIMZOOM))
			return;
		if ((y < -LIMZOOM) || (y > LIMZOOM))
			return;
	}
	// define cor da linha
	graph_bmp_canvas->Pen->Width = 2;
	graph_bmp_canvas->Pen->Color = cor;
	graph_bmp_canvas->Brush->Style = bsSolid;
	graph_bmp_canvas->Brush->Color = cor;
	// desenha cruz
	graph_bmp_canvas->MoveTo(x - dim, y - dim);
	graph_bmp_canvas->LineTo(x + dim, y + dim);
	graph_bmp_canvas->MoveTo(x - dim, y + dim);
	graph_bmp_canvas->LineTo(x + dim, y - dim);
}

// ---------------------------------------------------------------------------
bool __fastcall TFormGraf::DrawIcone_XYpixel(int eqpto_id, int index, int x, int y, int len, int dx,
	int dy, TColor cor)
{
	// variáveis locais
	double fat_escala;
	int x1, y1, x2, y2, x3, y3, x4, y4;
	int dim, dim_icone;
	VTIcone *icone;
	TForma *forma;

	// proteção
	if ((icone = icones->ExisteIcone(eqpto_id, index)) == NULL)
		return (false);
	// determina maior dimensão do Icone
	dim_icone = Max(icone->height, icone->width);
	// define dimensão disponível p/ o ícone
	dim = len / 2;
	// calcula fator de escala, relativo ao tamanho do ícone
	fat_escala = double(dim) / double(dim_icone);
	// ajusta coordenadas
	x += (dim * (dx / 100.));
	y += (dim * (dy / 100.));
	if (escala.overzoom) // zoom excedido: protege coordenadas
	{
		if ((x < -LIMZOOM) || (x > LIMZOOM))
			return (false);
		if ((y < -LIMZOOM) || (y > LIMZOOM))
			return (false);
	}
	// inicia Brush
	graph_bmp_canvas->Brush->Style = bsSolid;
	// graph_bmp_canvas->Brush->Color = icone->cor_fundo;
	graph_bmp_canvas->Brush->Color = tela.cor_fundo;
	// inicia Pen
	graph_bmp_canvas->Pen->Style = psSolid;
	// desenha formas do Icone
	dim = (fat_escala > 3) ? 1 : 0;
	for (int n = 0; n < icone->lisFORMA->Count; n++)
	{
		forma = (TForma*)icone->lisFORMA->Items[n];
		// ajusta dimensão e cor da forma
		graph_bmp_canvas->Pen->Style = psSolid;
		graph_bmp_canvas->Pen->Width = forma->width + dim;
		// graph_bmp_canvas->Pen->Color = (n == 0) ? cor : forma->cor;
		graph_bmp_canvas->Pen->Color = cor;
		// calcula coordenadas da forma
		x1 = fat_escala * forma->x1;
		y1 = fat_escala * forma->y1;
		x2 = fat_escala * forma->x2;
		y2 = fat_escala * forma->y2;
		x3 = fat_escala * forma->x3;
		y3 = fat_escala * forma->y3;
		x4 = fat_escala * forma->x4;
		y4 = fat_escala * forma->y4;
		switch (forma->tipo)
		{
		case formaELIPSE:
			graph_bmp_canvas->Ellipse(x + x1, y + y1, x + x2, y + y2);
			break;
		case formaLINHA:
			graph_bmp_canvas->MoveTo(x + x1, y + y1);
			graph_bmp_canvas->LineTo(x + x2, y + y2);
			graph_bmp_canvas->LineTo(x + x1, y + y1);
			break;
		case formaARCO:
			graph_bmp_canvas->Arc(x + x1, y + y1, x + x2, y + y2, x + x3, y + y3, x + x4, y + y4);
			break;
		default:
			break;
		}
	}
	return (true);
}

// ---------------------------------------------------------------------------
void __fastcall TFormGraf::DrawLinha(int x, int y, int dx, int dy, int lenx, int leny, int dim,
	TColor cor)
{
	// variáveis locais
	int x1, y1, x2, y2;

	// converte coordenadas do trecho para coordenadas da tela
	x1 = Xutm_to_Xpixel(x) + dx;
	x2 = x1 + lenx;
	y1 = Yutm_to_Ypixel(y) + dy;
	y2 = y1 + leny;
	if (escala.overzoom) // zoom excedido: protege coordenadas
	{
		if ((x1 < -LIMZOOM) || (x1 > LIMZOOM))
			return;
		if ((y1 < -LIMZOOM) || (y1 > LIMZOOM))
			return;
		if ((x2 < -LIMZOOM) || (x2 > LIMZOOM))
			return;
		if ((y2 < -LIMZOOM) || (y2 > LIMZOOM))
			return;
	}
	// define cor da linha
	graph_bmp_canvas->Pen->Width = dim;
	graph_bmp_canvas->Pen->Color = cor;
	// posiciona no primeiro ponto e traça linha até o segundo
	graph_bmp_canvas->MoveTo(x1, y1);
	if ((x1 == x2) && (y1 == y2))
	{ // VER HELP DO BORLAND
		graph_bmp_canvas->LineTo(x2 + 1, y2);
	}
	else
	{ // VER HELP DO BORLAND
		graph_bmp_canvas->LineTo(x2, y2);
		graph_bmp_canvas->LineTo(x1, y1);
	}
}

// ---------------------------------------------------------------------------
// TFormGraf::DrawLinha()  -  public
// ---------------------------------------------------------------------------
void __fastcall TFormGraf::DrawLinha(int x1, int y1, int x2, int y2, int dim, TColor cor,
	int estilo)
{
	// converte coordenadas do trecho para coordenadas da tela
	x1 = Xutm_to_Xpixel(x1);
	y1 = Yutm_to_Ypixel(y1);
	x2 = Xutm_to_Xpixel(x2);
	y2 = Yutm_to_Ypixel(y2);
	if (escala.overzoom) // zoom excedido: protege coordenadas
	{
		if ((x1 < -LIMZOOM) || (x1 > LIMZOOM))
			return;
		if ((y1 < -LIMZOOM) || (y1 > LIMZOOM))
			return;
		if ((x2 < -LIMZOOM) || (x2 > LIMZOOM))
			return;
		if ((y2 < -LIMZOOM) || (y2 > LIMZOOM))
			return;
	}
	// define cor da linha
	graph_bmp_canvas->Pen->Width = dim;
	graph_bmp_canvas->Pen->Color = cor;
	graph_bmp_canvas->Pen->Style = TPenStyle(estilo);
	graph_bmp_canvas->Brush->Style = bsClear;
	// posiciona no primeiro ponto e traça linha até o segundo
	graph_bmp_canvas->MoveTo(x1, y1);
	if ((x1 == x2) && (y1 == y2))
	{ // VER HELP DO BORLAND
		graph_bmp_canvas->LineTo(x2 + 1, y2);
	}
	else
	{ // VER HELP DO BORLAND
		graph_bmp_canvas->LineTo(x2, y2);
		if (graph_bmp_canvas->Pen->Style == psSolid)
			graph_bmp_canvas->LineTo(x1, y1);
	}
}

//---------------------------------------------------------------------------
void __fastcall TFormGraf::DrawLinha(StrPaint &paint)
   {
   //variáveis locais
   TPenMode pen_mode;

   //proteção
   if (paint.x1 < rede.area->x1) paint.x1 = rede.area->x1;
   if (paint.x2 > rede.area->x2) paint.x2 = rede.area->x2;
   if (paint.y1 < rede.area->y1) paint.y1 = rede.area->y1;
   if (paint.y2 > rede.area->y2) paint.y2 = rede.area->y2;
   //converte coordenadas do retangulo para coordenadas da tela
   paint.x1 = Xutm_to_Xpixel(paint.x1);  paint.y1 = Yutm_to_Ypixel(paint.y1);
   paint.x2 = Xutm_to_Xpixel(paint.x2);  paint.y2 = Yutm_to_Ypixel(paint.y2);
   //verifica se as coordenadas excedem capacidade de zoom
   if (escala.overzoom)
      {
      if ((paint.x1 < -LIMZOOM)||(paint.x1 > LIMZOOM)) return;
      if ((paint.y1 < -LIMZOOM)||(paint.y1 > LIMZOOM)) return;
      if ((paint.x2 < -LIMZOOM)||(paint.x2 > LIMZOOM)) return;
      if ((paint.y2 < -LIMZOOM)||(paint.y2 > LIMZOOM)) return;
      }
   //salva PenMode
   pen_mode = graph_bmp_canvas->Pen->Mode;
   //define cor da linha
   graph_bmp_canvas->Pen->Color   = paint.pen.color;
   graph_bmp_canvas->Pen->Width   = paint.pen.width;
   graph_bmp_canvas->Pen->Mode    = paint.pen.mode;
   graph_bmp_canvas->Pen->Style   = paint.pen.style;
   //graph_bmp_canvas->Brush->Style = paint.brush.style;
   graph_bmp_canvas->Brush->Color = paint.brush.color;
   //posiciona no primeiro ponto e traça linha até o segundo
   graph_bmp_canvas->MoveTo(paint.x1,  paint.y1);
   if (( paint.x1 ==  paint.x2)&&( paint.y1 ==  paint.y2))
      {//VER HELP DO BORLAND
      graph_bmp_canvas->LineTo(paint.x2+1,  paint.y2);
      }
   else
      {//VER HELP DO BORLAND
      graph_bmp_canvas->LineTo(paint.x2,  paint.y2);
      if (paint.pen.style == psSolid) graph_bmp_canvas->LineTo(paint.x1,  paint.y1);
      }
   //restaura PenMode
   graph_bmp_canvas->Pen->Mode = pen_mode;
   }

// ---------------------------------------------------------------------------
void __fastcall TFormGraf::DrawLinha_XYpixel(int x1, int y1, int x2, int y2, int dim, TColor cor,
	int estilo)
{
	if (escala.overzoom) // zoom excedido: protege coordenadas
	{
		if ((x1 < -LIMZOOM) || (x1 > LIMZOOM))
			return;
		if ((y1 < -LIMZOOM) || (y1 > LIMZOOM))
			return;
		if ((x2 < -LIMZOOM) || (x2 > LIMZOOM))
			return;
		if ((y2 < -LIMZOOM) || (y2 > LIMZOOM))
			return;
	}
	// define cor da linha
	graph_bmp_canvas->Pen->Width = dim;
	graph_bmp_canvas->Pen->Color = cor;
	graph_bmp_canvas->Pen->Style = TPenStyle(estilo);
	graph_bmp_canvas->Brush->Style = bsClear;
	// posiciona no primeiro ponto e traça linha até o segundo
	graph_bmp_canvas->MoveTo(x1, y1);
	if ((x1 == x2) && (y1 == y2))
	{ // VER HELP DO BORLAND
		graph_bmp_canvas->LineTo(x2 + 1, y2);
	}
	else
	{ // VER HELP DO BORLAND
		graph_bmp_canvas->LineTo(x2, y2);
		if (graph_bmp_canvas->Pen->Style == psSolid)
			graph_bmp_canvas->LineTo(x1, y1);
	}
}

// ---------------------------------------------------------------------------
// TFormGraf::DrawLinhaXor()  -  public
// ---------------------------------------------------------------------------
void __fastcall TFormGraf::DrawLinhaXor(int x1, int y1, int x2, int y2, int width)
{
	// variáveis locais
	TPenMode orig_mode;

	// converte coordenadas para pixel
	x1 = Xutm_to_Xpixel(x1);
	y1 = Yutm_to_Ypixel(y1);
	x2 = Xutm_to_Xpixel(x2);
	y2 = Yutm_to_Ypixel(y2);
	if (escala.overzoom) // zoom excedido: protege coordenadas
	{
		if ((x1 < -LIMZOOM) || (x1 > LIMZOOM))
			return;
		if ((y1 < -LIMZOOM) || (y1 > LIMZOOM))
			return;
		if ((x2 < -LIMZOOM) || (x2 > LIMZOOM))
			return;
		if ((y2 < -LIMZOOM) || (y2 > LIMZOOM))
			return;
	}
	// proteção: um único pixel não fica fisível
	if ((x1 == x2) && (y1 == y2))
		y2++;
	// salva e altera PenMode
	orig_mode = graph_bmp_canvas->Pen->Mode;
	// define padrão da linha
	graph_bmp_canvas->Pen->Mode = pmXor;
	graph_bmp_canvas->Pen->Color = (tela.cor_fundo == clBlack) ? clWhite : clGray;
	graph_bmp_canvas->Pen->Width = width;
	graph_bmp_canvas->MoveTo(x1, y1);
	graph_bmp_canvas->LineTo(x2, y2);
	// restaura PenMode
	graph_bmp_canvas->Pen->Mode = orig_mode;
}

// ---------------------------------------------------------------------------
void __fastcall TFormGraf::DrawMolduraArea(int x1, int y1, int x2, int y2, TPenMode pen_mode,
	TColor color)
{
	// variáveis locais
	TColor orig_pen_color;
	TPenMode orig_pen_mode;

	// proteção para cor igual à cor de fundo
	if (tela.cor_fundo == color)
		color = (color == clWhite) ? clBlack : clWhite;
	// converte coordenadas do retângulo para coordenadas da tela
	x1 = Xutm_to_Xpixel(x1);
	y1 = Yutm_to_Ypixel(y1);
	x2 = Xutm_to_Xpixel(x2);
	y2 = Yutm_to_Ypixel(y2);
	// salva PenMode
	orig_pen_mode = graph_bmp_canvas->Pen->Mode;
	orig_pen_color = graph_bmp_canvas->Pen->Color;
	// define padrão da linha
	graph_bmp_canvas->Pen->Mode = pen_mode;
	graph_bmp_canvas->Pen->Color = color;
	graph_bmp_canvas->Pen->Width = 1;
	// desenha retângulo
	graph_bmp_canvas->MoveTo(x1, y1);
	graph_bmp_canvas->LineTo(x2, y1);
	graph_bmp_canvas->LineTo(x2, y2);
	graph_bmp_canvas->LineTo(x1, y2);
	graph_bmp_canvas->LineTo(x1, y1);
	// restaura PenMode
	graph_bmp_canvas->Pen->Mode = orig_pen_mode;
	graph_bmp_canvas->Pen->Color = orig_pen_color;
}

// ---------------------------------------------------------------------------
void __fastcall TFormGraf::DrawMolduraCirculo(int xm, int ym, TPenMode pen_mode, TColor color)
{
	// variáveis locais
	int raio = 18;
	TColor orig_pen_color;
	TPenMode orig_pen_mode;

	// proteção para cor igual à cor de fundo
	if (tela.cor_fundo == color)
		color = (color == clWhite) ? clBlack : clWhite;
	// converte coordenadas xm,ym p/ coordenadas da tela
	xm = Xutm_to_Xpixel(xm);
	ym = Yutm_to_Ypixel(ym);
	// salva PenMode
	orig_pen_mode = graph_bmp_canvas->Pen->Mode;
	orig_pen_color = graph_bmp_canvas->Pen->Color;
	// define padrão da linha
	graph_bmp_canvas->Pen->Mode = pen_mode;
	graph_bmp_canvas->Pen->Color = color;
	graph_bmp_canvas->Pen->Width = 1;
	// desenha circunferência
	graph_bmp_canvas->Ellipse(xm - raio, ym - raio, xm + raio, ym + raio);
	// restaura PenMode
	graph_bmp_canvas->Pen->Mode = orig_pen_mode;
	graph_bmp_canvas->Pen->Color = orig_pen_color;
}

// ---------------------------------------------------------------------------
void __fastcall TFormGraf::DrawMolduraLinha(int x1, int y1, int x2, int y2, TPenMode pen_mode,
	TColor color)
{
	// variáveis locais
	TPenMode orig_pen_mode;
	TColor orig_pen_color;
	int xa, xb, xc, xd, ya, yb, yc, yd;
	int dx, dy;
	int len = 10;
	int len_x, len_y;
	double sen_teta, cos_teta, tan_teta;
	double len_x_cos_teta, len_x_sen_teta;
	double len_y_cos_teta, len_y_sen_teta;

	// proteção para cor igual à cor de fundo
	if (tela.cor_fundo == color)
		color = (color == clWhite) ? clBlack : clWhite;
	// converte coordenadas do retângulo para coordenadas da tela
	x1 = Xutm_to_Xpixel(x1);
	y1 = Yutm_to_Ypixel(y1);
	x2 = Xutm_to_Xpixel(x2);
	y2 = Yutm_to_Ypixel(y2);
	// calcula dx e dy
	dx = x2 - x1;
	dy = y2 - y1;
	// ajusta dx
	if (dx < -3)
	{
		len_x = -len;
	}
	else if (dx > 3)
	{
		len_x = len;
	}
	else
	{
		len_x = len;
		dx = 0;
	}
	// ajusta dy
	if (dy < -3)
	{
		len_y = -len;
	}
	else if (dy > 3)
	{
		len_y = len;
	}
	else
	{
		len_y = len;
		dy = 0;
	}
	// inicia coordenadas da moldura (pontos A(x1,y1), B(x2,y1), C(x2,y2), D(x1,y2)
	if (dx == 0)
	{
		xa = xd = x1 - len_x;
		xb = xc = x2 + len_x;
		ya = yb = y1 - len_y;
		yd = yc = y2 + len_y;
	}
	else if (dy == 0)
	{
		ya = yb = y1 - len_y;
		yd = yc = y2 + len_y;
		xa = xd = x1 - len_x;
		xb = xc = x2 + len_x;
	}
	else
	{
		tan_teta = double(dy) / double(dx);
		sen_teta = sin(atan(tan_teta));
		cos_teta = cos(atan(tan_teta));
		len_x_cos_teta = len_x * cos_teta;
		len_x_sen_teta = len_x * sen_teta;
		len_y_cos_teta = len_y * cos_teta;
		len_y_sen_teta = len_y * sen_teta;
		xa = x1 - len_x_cos_teta - len_y_sen_teta;
		xb = x2 + len_x_cos_teta - len_y_sen_teta;
		xc = x2 + len_x_cos_teta + len_y_sen_teta;
		xd = x1 - len_x_cos_teta + len_y_sen_teta;
		ya = y1 + len_y_cos_teta - len_x_sen_teta;
		yb = y2 + len_y_cos_teta + len_x_sen_teta;
		yc = y2 - len_y_cos_teta + len_x_sen_teta;
		yd = y1 - len_y_cos_teta - len_x_sen_teta;
	}
	// salva PenMode
	orig_pen_color = graph_bmp_canvas->Pen->Color;
	orig_pen_mode = graph_bmp_canvas->Pen->Mode;
	// define padrão da linha
	graph_bmp_canvas->Pen->Mode = pen_mode;
	graph_bmp_canvas->Pen->Color = color;
	graph_bmp_canvas->Pen->Width = 1;
	// desenha retângulo
	graph_bmp_canvas->MoveTo(xa, ya);
	graph_bmp_canvas->LineTo(xb, yb);
	graph_bmp_canvas->LineTo(xc, yc);
	graph_bmp_canvas->LineTo(xd, yd);
	graph_bmp_canvas->LineTo(xa, ya);
	// restaura PenMode
	graph_bmp_canvas->Pen->Mode = orig_pen_mode;
	graph_bmp_canvas->Pen->Color = orig_pen_color;
}

// ---------------------------------------------------------------------------
void __fastcall TFormGraf::DrawPie(int x, int y, int dx, int dy, int raio, double alfa, TColor cor)
{
	// variáveis locais
	int x1, y1, x2, y2, x3, y3, x4, y4;

	// converte coordenadas p/ coordenadas da tela
	x = Xutm_to_Xpixel(x) + dx;
	y = Yutm_to_Ypixel(y) + dy;
	// converte (x,y,raio,alfa) p/ coordenadas x1,y1,x2,y2,x3,y3,x4,y4
	x1 = x - raio;
	y1 = y - raio;
	x2 = x + raio + 1;
	y2 = y + raio + 1; // +1 é p/ que (x2-x1)e(y2-y1) seja um número par
	// x3 = x2;       y3 = (y1+y2)/2;
	x3 = x2;
	y3 = y;
	x4 = x + (raio * cos(alfa));
	y4 = y - (raio * sin(alfa));
	if (alfa < (M_PI + M_PI))
	{ // desenha circunferência
		graph_bmp_canvas->Brush->Style = bsClear;
		// graph_bmp_canvas->Pen->Color   = cor;
		graph_bmp_canvas->Pen->Color = clWhite;
		graph_bmp_canvas->Ellipse(x1, y1, x2, y2);
	}
	else
	{ // desenha círculo preenchido c/ a cor
		graph_bmp_canvas->Brush->Style = bsSolid;
		graph_bmp_canvas->Brush->Color = cor;
		// graph_bmp_canvas->Pen->Color   = cor;
		graph_bmp_canvas->Pen->Color = clWhite;
		graph_bmp_canvas->Ellipse(x1, y1, x2, y2);
	}
	// desenha a pizza
	graph_bmp_canvas->Brush->Style = bsSolid;
	graph_bmp_canvas->Brush->Color = cor;
	// graph_bmp_canvas->Pen->Color   = cor;
	graph_bmp_canvas->Pen->Color = clWhite;
	// proteção p/ alfa == 0
	if (alfa == 0)
	{ // desenha Line
		graph_bmp_canvas->MoveTo(x, y);
		graph_bmp_canvas->LineTo(x3, y3);
	}
	else
	{ // desenha Pie
		graph_bmp_canvas->Pie(x1, y1, x2, y2, x3, y3, x4, y4);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormGraf::DrawPolylineXor(TPoint *point, int count)
{
	// variáveis locais
	TPenMode orig_mode;
	// proteção
	if (escala.overzoom) // zoom excedido: protege coordenadas
	{
		for (int n = 0; n < count; n++)
		{
			if ((point[n].x < -LIMZOOM) || (point[n].x > LIMZOOM))
				return;
			if ((point[n].y < -LIMZOOM) || (point[n].y > LIMZOOM))
				return;
		}
	}
	// salva e altera PenMode
	orig_mode = graph_bmp_canvas->Pen->Mode;
	// define padrão da linha
	graph_bmp_canvas->Pen->Mode = pmXor;
	graph_bmp_canvas->Pen->Color = (tela.cor_fundo == clBlack) ? clWhite : clGray;
	graph_bmp_canvas->Pen->Width = 1;
	graph_bmp_canvas->Polyline(point, count - 1);
	// restaura PenMode
	graph_bmp_canvas->Pen->Mode = orig_mode;
}

// ---------------------------------------------------------------------------
void __fastcall TFormGraf::DrawPoligon_XYpixel(TPoint *point, int count, int dim_linha,
	TColor cor_linha, bool fill, TColor cor_fill)
{
	// proteção
	if (escala.overzoom) // zoom excedido: protege coordenadas
	{
		for (int n = 0; n < count; n++)
		{
			if ((point[n].x < -LIMZOOM) || (point[n].x > LIMZOOM))
				return;
			if ((point[n].y < -LIMZOOM) || (point[n].y > LIMZOOM))
				return;
		}
	}
	// define cor da linha
	graph_bmp_canvas->Pen->Width = dim_linha;
	graph_bmp_canvas->Pen->Color = cor_linha;
	graph_bmp_canvas->Pen->Style = psSolid;
	graph_bmp_canvas->Brush->Color = (fill) ? cor_fill : cor_linha;
	graph_bmp_canvas->Brush->Style = (fill) ? bsSolid : bsClear;
	graph_bmp_canvas->Polygon(point, count - 1);
}

// ---------------------------------------------------------------------------
void __fastcall TFormGraf::DrawPolyline_XYpixel(TPoint *point, int count, int dim, TColor cor)
{
	// proteção
	if (escala.overzoom) // zoom excedido: protege coordenadas
	{
		for (int n = 0; n < count; n++)
		{
			if ((point[n].x < -LIMZOOM) || (point[n].x > LIMZOOM))
				return;
			if ((point[n].y < -LIMZOOM) || (point[n].y > LIMZOOM))
				return;
		}
	}
	// define cor da linha
	graph_bmp_canvas->Pen->Width = dim;
	graph_bmp_canvas->Pen->Color = cor;
	graph_bmp_canvas->Pen->Style = psSolid;
	graph_bmp_canvas->Brush->Style = bsClear;
	graph_bmp_canvas->Polyline(point, count - 1);
}

// ---------------------------------------------------------------------------
void __fastcall TFormGraf::DrawQuadrado(int x, int y, int dim, TColor cor_ext, TColor cor_int)
{
	// define cor da linha
	graph_bmp_canvas->Pen->Width = 1;
	graph_bmp_canvas->Pen->Color = cor_ext;
	graph_bmp_canvas->Brush->Style = bsSolid;
	graph_bmp_canvas->Brush->Color = cor_int;

	// converte coordenadas da barra para coordenadas da tela
	x = Xutm_to_Xpixel(x);
	y = Yutm_to_Ypixel(y);
	if (escala.overzoom) // zoom excedido: protege coordenadas
	{
		if ((x < -LIMZOOM) || (x > LIMZOOM))
			return;
		if ((y < -LIMZOOM) || (y > LIMZOOM))
			return;
	}
	graph_bmp_canvas->Rectangle(x - dim, y - dim, x + dim, y + dim);
}

// ---------------------------------------------------------------------------
void __fastcall TFormGraf::DrawQuadradoXor(int x_utm, int y_utm, int dim_pixel)
{
	DrawRetanguloXor(x_utm, y_utm, dim_pixel, dim_pixel);
}

// ---------------------------------------------------------------------------
void __fastcall TFormGraf::DrawQuadrado_XYpixel(int x, int y, int dim, TColor cor_ext,
	TColor cor_int)
{
	// define cor da linha
	graph_bmp_canvas->Pen->Width = 1;
	graph_bmp_canvas->Pen->Color = cor_ext;
	graph_bmp_canvas->Brush->Style = bsSolid;
	graph_bmp_canvas->Brush->Color = cor_int;
	if (escala.overzoom) // zoom excedido: protege coordenadas
	{
		if ((x < -LIMZOOM) || (x > LIMZOOM))
			return;
		if ((y < -LIMZOOM) || (y > LIMZOOM))
			return;
	}
	graph_bmp_canvas->Rectangle(x - dim, y - dim, x + dim, y + dim);
}

// ---------------------------------------------------------------------------
void __fastcall TFormGraf::DrawRectZoom(void)
{
	// variáveis locais
	int x1, x2, y1, y2;

	// proteção
	if ((tela.zoom->x1 == tela.zoom->x2) && (tela.zoom->y1 == tela.zoom->y2))
		return;
	// define o retângulo de zoom
	x1 = (tela.zoom->x1 < tela.zoom->x2) ? tela.zoom->x1 : tela.zoom->x2;
	x2 = (tela.zoom->x1 < tela.zoom->x2) ? tela.zoom->x2 : tela.zoom->x1;
	y1 = (tela.zoom->y1 < tela.zoom->y2) ? tela.zoom->y1 : tela.zoom->y2;
	y2 = (tela.zoom->y1 < tela.zoom->y2) ? tela.zoom->y2 : tela.zoom->y1;
	// mapeia coordenadas na tela virtual
	x1 = (janela_virtual.fatx * (x1 - tela.area->x1) + 0.5) + janela_virtual.x1;
	y1 = (janela_virtual.faty * (y1 - tela.area->y1) + 0.5) + janela_virtual.y1;
	x2 = (janela_virtual.fatx * (x2 - tela.area->x1) + 0.5) + janela_virtual.x1;
	y2 = (janela_virtual.faty * (y2 - tela.area->y1) + 0.5) + janela_virtual.y1;
	TRect r(x1, y1, x2, y2);
	// configura Brush
	graph_bmp_canvas->Brush->Color = (tela.cor_fundo == clBlack) ? clWhite : clGray;
	// desenha o retângulo na tela virtual
	graph_bmp_canvas->DrawFocusRect(r);
	// atualiz TImage
	RedrawTelaReal();
}

// ---------------------------------------------------------------------------
void __fastcall TFormGraf::DrawRetangulo(int x1, int y1, int x2, int y2, TColor cor)
{
	// define cor da linha
	graph_bmp_canvas->Pen->Width = 1;
	graph_bmp_canvas->Pen->Color = cor;
	graph_bmp_canvas->Brush->Style = bsSolid;
	graph_bmp_canvas->Brush->Color = cor;
	// proteção
	if (x1 < rede.area->x1)
		x1 = rede.area->x1;
	if (x2 > rede.area->x2)
		x2 = rede.area->x2;
	if (y1 < rede.area->y1)
		y1 = rede.area->y1;
	if (y2 > rede.area->y2)
		y2 = rede.area->y2;
	// converte coordenadas do retangulo para coordenadas da tela
	x1 = Xutm_to_Xpixel(x1);
	y1 = Yutm_to_Ypixel(y1);
	x2 = Xutm_to_Xpixel(x2);
	y2 = Yutm_to_Ypixel(y2);
	// verifica se as coordenadas excedem capacidade de zoom
	if (escala.overzoom)
	{
		if ((x1 < -LIMZOOM) || (x1 > LIMZOOM))
			return;
		if ((y1 < -LIMZOOM) || (y1 > LIMZOOM))
			return;
		if ((x2 < -LIMZOOM) || (x2 > LIMZOOM))
			return;
		if ((y2 < -LIMZOOM) || (y2 > LIMZOOM))
			return;
	}
	// desenha o retângulo
	graph_bmp_canvas->Rectangle(x1, y1, x2, y2);
}

//---------------------------------------------------------------------------
void __fastcall TFormGraf::DrawRetangulo(int x1, int y1, int x2, int y2, TColor cor_linha, TColor cor_interna)
   {
   //define cor da linha
   graph_bmp_canvas->Pen->Width   = 1;
   graph_bmp_canvas->Pen->Color   = cor_linha;
   graph_bmp_canvas->Brush->Style = bsSolid;
   graph_bmp_canvas->Brush->Color = cor_interna;

   //proteção
   if (x1 < rede.area->x1) x1 = rede.area->x1;
   if (x2 > rede.area->x2) x2 = rede.area->x2;
   if (y1 < rede.area->y1) y1 = rede.area->y1;
   if (y2 > rede.area->y2) y2 = rede.area->y2;
   //converte coordenadas do retangulo para coordenadas da tela
   x1 = Xutm_to_Xpixel(x1);  y1 = Yutm_to_Ypixel(y1);
   x2 = Xutm_to_Xpixel(x2);  y2 = Yutm_to_Ypixel(y2);
   //verifica se as coordenadas excedem capacidade de zoom
   if (escala.overzoom)
      {
      if ((x1 < -LIMZOOM)||(x1 > LIMZOOM)) return;
      if ((y1 < -LIMZOOM)||(y1 > LIMZOOM)) return;
      if ((x2 < -LIMZOOM)||(x2 > LIMZOOM)) return;
	  if ((y2 < -LIMZOOM)||(y2 > LIMZOOM)) return;
      }
   graph_bmp_canvas->Rectangle(x1, y1, x2, y2);
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16014668
void __fastcall TFormGraf::DrawRetangulo(StrPaint &paint)
   {
   //variáveis locais
   TPenMode pen_mode;

   //proteção
//   if (paint.x1 < rede.area->x1) paint.x1 = rede.area->x1;
//   if (paint.x2 > rede.area->x2) paint.x2 = rede.area->x2;
//   if (paint.y1 < rede.area->y1) paint.y1 = rede.area->y1;
//   if (paint.y2 > rede.area->y2) paint.y2 = rede.area->y2;
   //converte coordenadas do retangulo para coordenadas da tela
   paint.x1 = Xutm_to_Xpixel(paint.x1);  paint.y1 = Yutm_to_Ypixel(paint.y1);
   paint.x2 = Xutm_to_Xpixel(paint.x2);  paint.y2 = Yutm_to_Ypixel(paint.y2);
   //verifica se as coordenadas excedem capacidade de zoom
   if (escala.overzoom)
      {
      if ((paint.x1 < -LIMZOOM)||(paint.x1 > LIMZOOM)) return;
      if ((paint.y1 < -LIMZOOM)||(paint.y1 > LIMZOOM)) return;
      if ((paint.x2 < -LIMZOOM)||(paint.x2 > LIMZOOM)) return;
      if ((paint.y2 < -LIMZOOM)||(paint.y2 > LIMZOOM)) return;
      }
   //salva PenMode
   pen_mode = graph_bmp_canvas->Pen->Mode;
   //define cor da linha
   graph_bmp_canvas->Pen->Color   = paint.pen.color;
   graph_bmp_canvas->Pen->Width   = paint.pen.width;
   graph_bmp_canvas->Pen->Mode    = paint.pen.mode;
   graph_bmp_canvas->Pen->Style   = paint.pen.style;
   graph_bmp_canvas->Brush->Style = paint.brush.style;
   graph_bmp_canvas->Brush->Color = paint.brush.color;
   graph_bmp_canvas->Rectangle(paint.x1, paint.y1, paint.x2, paint.y2);
   //restaura PenMode
   graph_bmp_canvas->Pen->Mode = pen_mode;
   }

// ---------------------------------------------------------------------------
void __fastcall TFormGraf::DrawRetanguloTransparente(int x1, int y1, int x2, int y2, TColor cor)
{
	// variáveis locais
	int dx, dy;
	TRect r1, r2;

	// define cor da linha
	graph_bmp_canvas->Pen->Width = 1;
	graph_bmp_canvas->Pen->Color = cor;
	graph_bmp_canvas->Brush->Style = bsSolid;
	graph_bmp_canvas->Brush->Color = cor;
	// proteção
	if (x1 < rede.area->x1)
		x1 = rede.area->x1;
	if (x2 > rede.area->x2)
		x2 = rede.area->x2;
	if (y1 < rede.area->y1)
		y1 = rede.area->y1;
	if (y2 > rede.area->y2)
		y2 = rede.area->y2;
	// converte coordenadas do retangulo para coordenadas da tela
	x1 = Xutm_to_Xpixel(x1);
	y1 = Yutm_to_Ypixel(y1);
	x2 = Xutm_to_Xpixel(x2);
	y2 = Yutm_to_Ypixel(y2);
	// verifica se as coordenadas excedem capacidade de zoom
	if (escala.overzoom)
	{
		if ((x1 < -LIMZOOM) || (x1 > LIMZOOM))
			return;
		if ((y1 < -LIMZOOM) || (y1 > LIMZOOM))
			return;
		if ((x2 < -LIMZOOM) || (x2 > LIMZOOM))
			return;
		if ((y2 < -LIMZOOM) || (y2 > LIMZOOM))
			return;
	}
	// salva bitmap atual do retângualo que será preenchido
	r1 = TRect(x1, y1, x2, y2);
	dx = r1.Width();
	dy = r1.Height();
	if (dy < 0)
		r2 = TRect(0, -dy, dx, 0);
	else
		r2 = TRect(0, 0, dx, dy);
	imag->Canvas->CopyRect(r2, graph_bmp_canvas, r1);
	// desenha o retângulo
	graph_bmp_canvas->Rectangle(x1, y1, x2, y2);
	// copia bitmap salvo, fazendo merge
	int copy_mode_orig = graph_bmp_canvas->CopyMode;
	graph_bmp_canvas->CopyMode = cmMergePaint;
	graph_bmp_canvas->CopyRect(r1, imag->Canvas, r2);
	graph_bmp_canvas->CopyMode = copy_mode_orig;
}

//---------------------------------------------------------------------------
void __fastcall TFormGraf::DrawRetanguloTransparente(StrPaint &paint)
   {
   //variáveis locais
   TPenMode pen_mode;

   //proteção
   if (paint.x1 < rede.area->x1) paint.x1 = rede.area->x1;
   if (paint.x2 > rede.area->x2) paint.x2 = rede.area->x2;
   if (paint.y1 < rede.area->y1) paint.y1 = rede.area->y1;
   if (paint.y2 > rede.area->y2) paint.y2 = rede.area->y2;
   //converte coordenadas do retangulo para coordenadas da tela
   paint.x1 = Xutm_to_Xpixel(paint.x1);  paint.y1 = Yutm_to_Ypixel(paint.y1);
   paint.x2 = Xutm_to_Xpixel(paint.x2);  paint.y2 = Yutm_to_Ypixel(paint.y2);
   //verifica se as coordenadas excedem capacidade de zoom
   if (escala.overzoom)
      {
      if ((paint.x1 < -LIMZOOM)||(paint.x1 > LIMZOOM)) return;
      if ((paint.y1 < -LIMZOOM)||(paint.y1 > LIMZOOM)) return;
      if ((paint.x2 < -LIMZOOM)||(paint.x2 > LIMZOOM)) return;
      if ((paint.y2 < -LIMZOOM)||(paint.y2 > LIMZOOM)) return;
      }
   //salva PenMode
   pen_mode = graph_bmp_canvas->Pen->Mode;
   //define cor da linha
   graph_bmp_canvas->Pen->Color   = paint.pen.color;
   graph_bmp_canvas->Pen->Width   = paint.pen.width;
   graph_bmp_canvas->Pen->Mode    = paint.pen.mode;
   graph_bmp_canvas->Pen->Style   = paint.pen.style;
   //desenha linhas do retângulo
   graph_bmp_canvas->MoveTo(paint.x1, paint.y1);
   graph_bmp_canvas->LineTo(paint.x2, paint.y1);
   graph_bmp_canvas->LineTo(paint.x2, paint.y2);
   graph_bmp_canvas->LineTo(paint.x1, paint.y2);
   graph_bmp_canvas->LineTo(paint.x1, paint.y1);
   //restaura PenMode
   graph_bmp_canvas->Pen->Mode = pen_mode;
   }

// ---------------------------------------------------------------------------
void __fastcall TFormGraf::DrawRetanguloX1Y1(int x1, int y1, int dx_pixel, int dy_pixel, TColor cor)
{
	// variáveis locais
	int x2, y2;

	// define cor da linha
	graph_bmp_canvas->Pen->Width = 1;
	graph_bmp_canvas->Pen->Color = cor;
	graph_bmp_canvas->Brush->Style = bsSolid;
	graph_bmp_canvas->Brush->Color = cor;
	// converte coordenadas (x1,y1) do retangulo para coordenadas da tela
	x1 = Xutm_to_Xpixel(x1);
	y1 = Yutm_to_Ypixel(y1);
	x2 = x1 + dx_pixel;
	y2 = y1 + dy_pixel;
	// verifica se as coordenadas excedem capacidade de zoom
	if (escala.overzoom)
	{
		if ((x1 < -LIMZOOM) || (x1 > LIMZOOM))
			return;
		if ((y1 < -LIMZOOM) || (y1 > LIMZOOM))
			return;
		if ((x2 < -LIMZOOM) || (x2 > LIMZOOM))
			return;
		if ((y2 < -LIMZOOM) || (y2 > LIMZOOM))
			return;
	}
	// desenha o retângulo
	graph_bmp_canvas->Rectangle(x1, y1, x2, y2);
}

// ---------------------------------------------------------------------------
void __fastcall TFormGraf::DrawRetanguloXor(int x_utm, int y_utm, int dx_pixel, int dy_pixel)
{
	// variáveis locais
	int x, y; // coordenadas em pixel
	TPenMode orig_mode;

	// converte coordenadas da barra para coordenadas da tela
	x = Xutm_to_Xpixel(x_utm);
	y = Yutm_to_Ypixel(y_utm);
	if (escala.overzoom) // zoom excedido: protege coordenadas
	{
		if ((x < -LIMZOOM) || (x > LIMZOOM))
			return;
		if ((y < -LIMZOOM) || (y > LIMZOOM))
			return;
	}
	// salva PenMode
	orig_mode = graph_bmp_canvas->Pen->Mode;
	// define padrão da linha
	graph_bmp_canvas->Pen->Mode = pmXor;
	graph_bmp_canvas->Pen->Color = (tela.cor_fundo == clBlack) ? clWhite : clGray;
	graph_bmp_canvas->Pen->Width = 1;
	graph_bmp_canvas->Brush->Style = bsClear;
	// divide dimensões p/ o retângulo
	dx_pixel /= 2;
	dy_pixel /= 2;
	// desenha o retangulo
	graph_bmp_canvas->Rectangle(x - dx_pixel, y - dy_pixel, x + dx_pixel, y + dy_pixel);
	// restaura PenMode
	graph_bmp_canvas->Pen->Mode = orig_mode;
}

//---------------------------------------------------------------------------
void __fastcall TFormGraf::DrawRetanguloX1Y1_X2Y2_Xor(int x1_utm, int y1_utm, int x2_utm, int y2_utm)
   {
   //variáveis locais
   int      x1, y1, x2, y2;  //coordenadas em pixel
   TPenMode orig_mode;

   //converte coordenadas da barra para coordenadas da tela
   x1 = Xutm_to_Xpixel(x1_utm);
   y1 = Yutm_to_Ypixel(y1_utm);
   x2 = Xutm_to_Xpixel(x2_utm);
   y2 = Yutm_to_Ypixel(y2_utm);
   if (escala.overzoom) //zoom excedido: protege coordenadas
      {
      if ((x1 < -LIMZOOM)||(x1 > LIMZOOM)) return;
      if ((y1 < -LIMZOOM)||(y1 > LIMZOOM)) return;
      if ((x2 < -LIMZOOM)||(x2 > LIMZOOM)) return;
      if ((y2 < -LIMZOOM)||(y2 > LIMZOOM)) return;
      }
   //salva PenMode
   orig_mode = graph_bmp_canvas->Pen->Mode;
   //define padrão da linha
   graph_bmp_canvas->Pen->Mode = pmXor;
   graph_bmp_canvas->Pen->Color = (tela.cor_fundo == clBlack) ? clWhite : clGray;
   graph_bmp_canvas->Pen->Width = 1;
   graph_bmp_canvas->Brush->Style = bsClear;
   //desenha o retangulo
   graph_bmp_canvas->Rectangle(x1, y1, x2, y2);
   //restaura PenMode
   graph_bmp_canvas->Pen->Mode = orig_mode;
   }

// ---------------------------------------------------------------------------
// TFormGraf::DrawTrecho()  -  public
// ---------------------------------------------------------------------------
void __fastcall TFormGraf::DrawTexto(AnsiString txt, int x, int y, TColor cor)
{
	// define cor da linha
	graph_bmp_canvas->Pen->Width = 1;
	graph_bmp_canvas->Pen->Color = cor; // (tela.cor_fundo == clWhite) ? clBlack : clWhite;
	graph_bmp_canvas->Brush->Style = bsClear; // bsSolid;
	graph_bmp_canvas->Brush->Color = tela.cor_fundo;
	// converte coordenadas da barra para coordenadas da tela
	x = Xutm_to_Xpixel(x);
	y = Yutm_to_Ypixel(y);
	if (escala.overzoom) // zoom excedido: protege coordenadas
	{
		if ((x < -LIMZOOM) || (x > LIMZOOM))
			return;
		if ((y < -LIMZOOM) || (y > LIMZOOM))
			return;
	}
	// ajusta coordenadas e escreve o texto
	x -= graph_bmp_canvas->TextWidth(txt) / 2;
	y -= graph_bmp_canvas->TextHeight(txt) / 2;
	graph_bmp_canvas->Font->Color = cor;
	graph_bmp_canvas->TextOut(x, y, txt);
}

// ---------------------------------------------------------------------------
void __fastcall TFormGraf::DrawTexto_XYpixel(AnsiString txt, int x, int y, TColor cor)
{
	// define cor da linha
	graph_bmp_canvas->Pen->Width = 1;
	graph_bmp_canvas->Pen->Color = cor;
	graph_bmp_canvas->Brush->Style = bsClear; // bsSolid;
	graph_bmp_canvas->Brush->Color = tela.cor_fundo;
	if (escala.overzoom) // zoom excedido: protege coordenadas
	{
		if ((x < -LIMZOOM) || (x > LIMZOOM))
			return;
		if ((y < -LIMZOOM) || (y > LIMZOOM))
			return;
	}
	// ajusta coordenadas e escreve o texto
	x -= graph_bmp_canvas->TextWidth(txt) / 2;
	y -= graph_bmp_canvas->TextHeight(txt) / 2;
	graph_bmp_canvas->Font->Color = cor;
	graph_bmp_canvas->TextOut(x, y, txt);
}

//---------------------------------------------------------------------------
void __fastcall TFormGraf::DrawTexto_XYpixel(StrPaint &paint)
   {
   //proteção
   if (escala.overzoom) //zoom excedido: protege coordenadas
      {
      if ((paint.x < -LIMZOOM)||(paint.x >LIMZOOM)) return;
      if ((paint.y < -LIMZOOM)||(paint.y >LIMZOOM)) return;
      }
   //redefine Font de graph_bmp_canvas
   graph_bmp_canvas->Font->Assign(paint.font);
   //escreve o texto
   graph_bmp_canvas->TextOut(paint.x, paint.y, paint.texto);
   //restaura Font de graph_bmp_canvas
   graph_bmp_canvas->Font->Assign(image_graph_bmp->Canvas->Font);
   }

/*
 //---------------------------------------------------------------------------
 void __fastcall TFormGraf::DrawTexto(TStringList *StrList, int x, int y, TColor cor)
 {
 //variáveis locais
 int x1, y1, x2, y2, width = 0;

 //define cor do texto (sem cor de fundo)
 graph_bmp_canvas->Font->Color  = cor;
 graph_bmp_canvas->Brush->Style = bsClear;
 //graph_bmp_canvas->Brush->Color = tela.cor_fundo;
 //converte coordenadas para coordenadas da tela
 x1 = x2 = Xutm_to_Xpixel(x);
 y1 = y2 = Yutm_to_Ypixel(y);
 if (escala.overzoom) //zoom excedido: protege coordenadas
 {
 if ((x < -LIMZOOM)||(x >LIMZOOM)) return;
 if ((y < -LIMZOOM)||(y >LIMZOOM)) return;
 }
 for (int n = 0; n < StrList->Count; n++)
 {//exibe o string
 graph_bmp_canvas->TextOut(x1, y2, StrList->Strings[n]);
 //ajusta coordenada y2 p/ próximo string
 y2 += graph_bmp_canvas->TextHeight(StrList->Strings[n]);
 //determina maior dimensão dos Strings
 width = max(width, graph_bmp_canvas->TextWidth(StrList->Strings[n]));
 }
 //desenha uma moldura em torno dos string
 graph_bmp_canvas->Pen->Width   = 1;
 graph_bmp_canvas->Pen->Color   = cor;
 graph_bmp_canvas->Brush->Style = bsSolid;
 x2 = x1 + width;
 graph_bmp_canvas->FrameRect(Rect(x1-1, y1-1, x2+1, y2+1));
 }
 */
// ---------------------------------------------------------------------------
void __fastcall TFormGraf::DrawTexto(TStringList *StrList, int x, int y, TColor cor)
{
	// define cor do texto (sem cor de fundo)
	graph_bmp_canvas->Font->Color = cor;
	graph_bmp_canvas->Brush->Style = bsClear;
	// graph_bmp_canvas->Brush->Color = tela.cor_fundo;
	// converte coordenadas para coordenadas da tela
	x = Xutm_to_Xpixel(x);
	y = Yutm_to_Ypixel(y);
	if (escala.overzoom) // zoom excedido: protege coordenadas
	{
		if ((x < -LIMZOOM) || (x > LIMZOOM))
			return;
		if ((y < -LIMZOOM) || (y > LIMZOOM))
			return;
	}
	for (int n = 0; n < StrList->Count; n++)
	{ // exibe o string
		graph_bmp_canvas->TextOut(x, y, StrList->Strings[n]);
		// ajusta coordenada y p/ próximo string
		y += graph_bmp_canvas->TextHeight(StrList->Strings[n]);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormGraf::DrawTexto(AnsiString txt, int x, int y, int dx, int dy, TColor cor)
{
	// converte coordenadas da barra para coordenadas da tela
	x = Xutm_to_Xpixel(x);
	y = Yutm_to_Ypixel(y);
	// ajusta coordenadas
	x -= (graph_bmp_canvas->TextWidth(txt) * dx) / 100;
	y -= (graph_bmp_canvas->TextHeight(txt) *dy) / 100;
	if (escala.overzoom) // zoom excedido: protege coordenadas
	{
		if ((x < -LIMZOOM) || (x > LIMZOOM))
			return;
		if ((y < -LIMZOOM) || (y > LIMZOOM))
			return;
	}
	// define cor da linha
	graph_bmp_canvas->Pen->Width = 1;
	graph_bmp_canvas->Pen->Color = cor;
	// graph_bmp_canvas->Brush->Style = bsSolid;
	graph_bmp_canvas->Brush->Style = bsClear;
	// graph_bmp_canvas->Brush->Color = tela.cor_fundo;
	graph_bmp_canvas->Font->Color = cor;
	graph_bmp_canvas->TextOut(x, y, txt);
	graph_bmp_canvas->Font->Color = tela.cor_fundo; // ATENÇÃO: NÃO ELIMINAR ESTA LINHA
}

// ---------------------------------------------------------------------------
void __fastcall TFormGraf::DrawTexto_XYpixel(AnsiString txt, int x, int y, int dx, int dy,
	TColor cor)
{
	// ajusta coordenadas
	x -= (graph_bmp_canvas->TextWidth(txt) * dx) / 100;
	y -= (graph_bmp_canvas->TextHeight(txt) * dy) / 100;
	if (escala.overzoom) // zoom excedido: protege coordenadas
	{
		if ((x < -LIMZOOM) || (x > LIMZOOM))
			return;
		if ((y < -LIMZOOM) || (y > LIMZOOM))
			return;
	}
	// define cor da linha
	graph_bmp_canvas->Pen->Width = 1;
	graph_bmp_canvas->Pen->Color = cor;
	// graph_bmp_canvas->Brush->Style = bsSolid;
	graph_bmp_canvas->Brush->Style = bsClear;
	// graph_bmp_canvas->Brush->Color = tela.cor_fundo;
	graph_bmp_canvas->Font->Color = cor;
	graph_bmp_canvas->TextOut(x, y, txt);
	graph_bmp_canvas->Font->Color = tela.cor_fundo; // ATENÇÃO: NÃO ELIMINAR ESTA LINHA
}

// ---------------------------------------------------------------------------
void __fastcall TFormGraf::DrawTextoAt(AnsiString txt, int x, int y, int dx, int dy, TColor cor)
{
	// converte coordenadas da barra para coordenadas da tela
	x = Xutm_to_Xpixel(x) + dx;
	y = Yutm_to_Ypixel(y) + dy;
	if (escala.overzoom) // zoom excedido: protege coordenadas
	{
		if ((x < -LIMZOOM) || (x > LIMZOOM))
			return;
		if ((y < -LIMZOOM) || (y > LIMZOOM))
			return;
	}
	// ajusta coordenadas e escreve o texto
	// define cor da linha
	graph_bmp_canvas->Pen->Width = 1;
	graph_bmp_canvas->Pen->Color = cor;
	// graph_bmp_canvas->Brush->Style = bsSolid;
	graph_bmp_canvas->Brush->Style = bsClear;
	// graph_bmp_canvas->Brush->Color = tela.cor_fundo;
	graph_bmp_canvas->Font->Color = cor;
	graph_bmp_canvas->TextOut(x, y, txt);
	graph_bmp_canvas->Font->Color = tela.cor_fundo; // ATENÇÃO: NÃO ELIMINAR ESTA LINHA
}

// ---------------------------------------------------------------------------
void __fastcall TFormGraf::DrawTextoAt_XYpixel(AnsiString txt, int x, int y, int dx, int dy,
	TColor cor)
{
	if (escala.overzoom) // zoom excedido: protege coordenadas
	{
		if ((x < -LIMZOOM) || (x > LIMZOOM))
			return;
		if ((y < -LIMZOOM) || (y > LIMZOOM))
			return;
	}
	// ajusta coordenadas e escreve o texto
	// define cor da linha
	graph_bmp_canvas->Pen->Width = 1;
	graph_bmp_canvas->Pen->Color = cor;
	// graph_bmp_canvas->Brush->Style = bsSolid;
	graph_bmp_canvas->Brush->Style = bsClear;
	// graph_bmp_canvas->Brush->Color = tela.cor_fundo;
	graph_bmp_canvas->Font->Color = cor;
	graph_bmp_canvas->TextOut(x, y, txt);
	graph_bmp_canvas->Font->Color = tela.cor_fundo; // ATENÇÃO: NÃO ELIMINAR ESTA LINHA
}

// ---------------------------------------------------------------------------
// TFormGraf::DrawTextoCirc()  -  public
// ---------------------------------------------------------------------------
void __fastcall TFormGraf::DrawTextoCirc(AnsiString txt, int x, int y, TColor cor)
{
	// variáveis locais
	int dx, dy, raio;

	// converte coordenadas da barra para coordenadas da tela
	x = Xutm_to_Xpixel(x);
	y = Yutm_to_Ypixel(y);
	if (escala.overzoom) // zoom excedido: protege coordenadas
	{
		if ((x < -LIMZOOM) || (x > LIMZOOM))
			return;
		if ((y < -LIMZOOM) || (y > LIMZOOM))
			return;
	}
	// determina dimensão p/ conter o texto
	dx = (graph_bmp_canvas->TextWidth(txt) + 1) / 2;
	dy = (graph_bmp_canvas->TextHeight(txt) + 1) / 2;
	raio = Max(dx, dy) + 2;
	// desenha o círculo
	graph_bmp_canvas->Pen->Width = 1;
	graph_bmp_canvas->Pen->Color = cor;
	graph_bmp_canvas->Brush->Style = bsSolid;
	graph_bmp_canvas->Brush->Color = cor;
	graph_bmp_canvas->Ellipse(x - raio, y - raio, x + raio, y + raio);
	// escreve o texto
	graph_bmp_canvas->Brush->Style = bsClear;
	graph_bmp_canvas->Font->Color = clBlack;
	graph_bmp_canvas->TextOut(x - dx + 1, y - dy + 1, txt);
}

// ---------------------------------------------------------------------------
void __fastcall TFormGraf::DrawTextoRect(AnsiString txt, int x, int y, TColor cor)
{
	// variáveis locais
	int dx, dy, borda;

	// converte coordenadas da barra para coordenadas da tela
	x = Xutm_to_Xpixel(x);
	y = Yutm_to_Ypixel(y);
	if (escala.overzoom) // zoom excedido: protege coordenadas
	{
		if ((x < -LIMZOOM) || (x > LIMZOOM))
			return;
		if ((y < -LIMZOOM) || (y > LIMZOOM))
			return;
	}
	// determina dimensões p/ conter o texto
	dx = ((graph_bmp_canvas->TextWidth(txt) + 1) / 2) + 1;
	dy = ((graph_bmp_canvas->TextHeight(txt) + 1) / 2) + 1;
	// define borda p/ garantir uma dimensão mínima do retângulo
	borda = 5;
	if (dx < 30)
		borda = 35 - dx;
	// define cor da linha
	graph_bmp_canvas->Pen->Width = 1;
	graph_bmp_canvas->Pen->Color = cor;
	graph_bmp_canvas->Brush->Color = tela.cor_fundo; // cor;
	graph_bmp_canvas->Brush->Style = bsSolid; // bsClear;         //
	// desenho o retângulo
	graph_bmp_canvas->Rectangle(x - dx - borda, y - dy, x + dx + borda, y + dy);
	// escreve o texto
	graph_bmp_canvas->Font->Color = cor; // clBlack;
	graph_bmp_canvas->TextOut(x - dx + 1, y - dy + 1, txt);
}

// ---------------------------------------------------------------------------
void __fastcall TFormGraf::DrawTextoRect(AnsiString txt, int x, int y, int width, TColor cor)
{
	// variáveis locais
	int dx, dy;

	// converte width p/ pixel e divide por 2
	// width = (Xutm_to_Xpixel(x+width) - Xutm_to_Xpixel(x))/2;
	// converte coordenadas da barra para coordenadas da tela
	x = Xutm_to_Xpixel(x);
	y = Yutm_to_Ypixel(y);
	if (escala.overzoom) // zoom excedido: protege coordenadas
	{
		if ((x < -LIMZOOM) || (x > LIMZOOM))
			return;
		if ((y < -LIMZOOM) || (y > LIMZOOM))
			return;
	}
	// determina dimensões p/ conter o texto
	dx = ((graph_bmp_canvas->TextWidth(txt) + 8) / 2) + 1;
	dy = ((graph_bmp_canvas->TextHeight(txt) + 1) / 2) + 1;
	// verifica se o texto cabe dentro de width definido
	// if (dx > width) dx = width;
	// define retângulo p/ limitar apresentação do texto
	TRect r(x - dx, y - dy, x + dx, y + dy);
	// define cor da linha
	graph_bmp_canvas->Pen->Width = 1;
	graph_bmp_canvas->Pen->Color = cor;
	graph_bmp_canvas->Brush->Style = bsSolid;
	graph_bmp_canvas->Brush->Color = cor;
	// desenho o retângulo
	graph_bmp_canvas->Rectangle(x - dx, y - dy, x + dx, y + dy);
	// escreve o texto
	graph_bmp_canvas->Font->Color = clBlack;
	graph_bmp_canvas->TextRect(r, x - dx + 5, y - dy + 1, txt);
}

// ---------------------------------------------------------------------------
// TFormGraf::Escala()  -  public
// retorna escala sendo utilizada (pixel/unid.rede)
// ---------------------------------------------------------------------------
double __fastcall TFormGraf::Escala(void)
{
	return (escala.fatx);
}

// ---------------------------------------------------------------------------
void __fastcall TFormGraf::ExibeCoordMouse(void)
{
	// variáveis locais
	AnsiString txt;

	// panel 4: Coord. do mouse
	txt.sprintf("mouse:(%3.2f,%3.2f)", Xpixel_to_Xutm(mouse.x) / 100.,
		Ypixel_to_Yutm(mouse.y) / 100.);
	sttbar->Panels->Items[4]->Text = txt;
}

// ---------------------------------------------------------------------------
void __fastcall TFormGraf::ExibeDimRede(void)
{
	// variáveis locais
	AnsiString txt;
	int dx, dy, esc;

	// proteção: rede pode não ter sido definida
	if (rede.zoom->x1 < 0)
	{
		sttbar->Panels->Items[0]->Text = "";
		sttbar->Panels->Items[1]->Text = "";
		sttbar->Panels->Items[2]->Text = "";
		sttbar->Panels->Items[3]->Text = "";
		return;
	}
	// atualiza StatusBar
	// panel 0: Coord. da rede
	txt.sprintf("coord.rede:(%d,%d) (%d,%d)", int(rede.area->x1 / 100), int(rede.area->y1 / 100),
		int(rede.area->x2 / 100), int(rede.area->y2 / 100));
	sttbar->Panels->Items[0]->Text = txt;
	// panel 1: Coord. de zoom
	txt.sprintf("coord.zoom:(%d,%d) (%d,%d)", int(rede.zoom->x1 / 100), int(rede.zoom->y1 / 100),
		int(rede.zoom->x2 / 100), int(rede.zoom->y2 / 100));
	sttbar->Panels->Items[1]->Text = txt;
	// panel 2: Área da rede
	dx = (rede.area->x2 - rede.area->x1) / 100;
	dy = (rede.area->y2 - rede.area->y1) / 100;
	txt.sprintf("dim.rede:(%d) (%d)", dx, dy);
	sttbar->Panels->Items[2]->Text = txt;
	// panel 3: Área de zoom
	dx = (rede.zoom->x2 - rede.zoom->x1) / 100;
	dy = (rede.zoom->y2 - rede.zoom->y1) / 100;
	txt.sprintf("dim.zoom:(%d) (%d)", dx, dy);
	sttbar->Panels->Items[3]->Text = txt;
}

// ---------------------------------------------------------------------------
void __fastcall TFormGraf::FormClose(TObject *Sender, TCloseAction &Action)
{
	Action = caHide;
}

/*
 //-----------------------------------------------------------------------------
 bool __fastcall TFormGraf::Exporta(TCanvas *canvas, TRect &rect)
 {
 //redefine Canvas usado p/ desenhar
 graph_bmp_canvas = canvas;
 //redefine dimensão da tela real igual à área do TRect indicado
 tela.area->x1 = 0;
 tela.area->y1 = 0;
 tela.area->x2 = rect.Width()  - 1;
 tela.area->y2 = rect.Height() - 1;
 //redefine coordenadas da tela virtual igual à área do TRect indicado
 tela_virtual.x1 = 0;
 tela_virtual.y1 = 0;
 tela_virtual.x2 = rect.Width()  - 1;
 tela_virtual.y2 = rect.Height() - 1;
 //redefine ponto de referência p/ montagem da figura
 tela_virtual.x = 0;
 tela_virtual.y = 0;
 //redefine escala (que faz com que o gráfico seja redesenhado)
 DefEscala();
 //desenha a rede no Canvas externo
 graf->Redraw();
 //restaura Canvas próprio
 graph_bmp_canvas = graph_bmp->Canvas;
 //inicia coordenadas da tela virtual
 tela_virtual.x1 = 0;
 tela_virtual.y1 = 0;
 tela_virtual.x2 = graph_bmp->Width - 1;
 tela_virtual.y2 = graph_bmp->Height -1;
 //define ponto de referência p/ montagem da figura
 tela_virtual.x = (graph_bmp->Width  - imag->Constraints->MaxWidth)  / 2;
 tela_virtual.y = (graph_bmp->Height - imag->Constraints->MaxHeight) / 2;
 //define área da tela
 DefAreaTela();
 //redefine escala
 DefEscala();
 //reapresenta a rede
 graf->Redraw();

 return(true);
 }
 */
// -----------------------------------------------------------------------------
bool __fastcall TFormGraf::Exporta(TCanvas *canvas, TRect &rect)
{
	// redefine Canvas usado p/ desenhar
	graph_bmp_canvas = canvas;
	// redefine dimensão da tela real igual à área do TRect indicado
	tela.area->x1 = rect.Left;
	tela.area->y1 = rect.Top;
	tela.area->x2 = rect.Right;
	tela.area->y2 = rect.Bottom;
	// redefine coordenadas da tela virtual igual à área do TRect indicado
	tela_virtual.x1 = rect.Left;
	tela_virtual.y1 = rect.Top;
	tela_virtual.x2 = rect.Right;
	tela_virtual.y2 = rect.Bottom;
	// redefine ponto de referência p/ montagem da figura
	tela_virtual.x = rect.Left;
	tela_virtual.y = rect.Top;
	// redefine escala (que faz com que o gráfico seja redesenhado)
	DefEscala();
	// desenha a rede no Canvas externo
	graf->Redraw();
	// restaura Canvas próprio
	graph_bmp_canvas = graph_bmp->Canvas;
	// inicia coordenadas da tela virtual
	tela_virtual.x1 = 0;
	tela_virtual.y1 = 0;
	tela_virtual.x2 = graph_bmp->Width - 1;
	tela_virtual.y2 = graph_bmp->Height - 1;
	// define ponto de referência p/ montagem da figura
	tela_virtual.x = (graph_bmp->Width - imag->Constraints->MaxWidth) / 2;
	tela_virtual.y = (graph_bmp->Height - imag->Constraints->MaxHeight) / 2;
	// define área da tela
	DefAreaTela();
	// redefine escala
	DefEscala();
	// reapresenta a rede
	graf->Redraw();

	return (true);
}

// -----------------------------------------------------------------------------
void __fastcall TFormGraf::ExportaBmp(AnsiString filename)
{
	// variaveis locais
	TRect rect;
	TImage *imagem;
	int width = imag->Width; // 1280
	int height = imag->Height; // 720

	try
	{ // cria objeto
		imagem = new TImage(NULL);
		// determina dimensao atual da tela
		rect.init(imag->Left, imag->Top, width, height);
		imagem->Width = width;
		imagem->Height = height;
		// redefine Canvas usado p/ desenhar
		graph_bmp_canvas = imagem->Canvas;
		// redefine dimensao da tela real igual a area do TRect indicado
		tela.area->x1 = rect.Left;
		tela.area->y1 = rect.Top;
		tela.area->x2 = rect.Right;
		tela.area->y2 = rect.Bottom;
		// redefine coordenadas da tela virtual igual a area do TRect indicado
		tela_virtual.x1 = rect.Left;
		tela_virtual.y1 = rect.Top;
		tela_virtual.x2 = rect.Right;
		tela_virtual.y2 = rect.Bottom;
		// redefine ponto de referencia p/ montagem da figura
		tela_virtual.x = rect.Left;
		tela_virtual.y = rect.Top;
		// redefine escala (que faz com que o grafico seja redesenhado)
		DefEscala();
		// desenha a rede no Canvas externo
		graf->Redraw();
		// grava imagem no arquivo indicado
		imagem->Picture->SaveToFile(filename);
		// destroi objeto criado temporariamente para redesenhar imagem
		delete imagem;
		Aviso("A imagem foi exportada com sucesso.");
	}
	catch (Exception &e)
	{
		Aviso("Erro ao exportar imagem");
		if (imagem)
			delete imagem;
	}

	// restaura Canvas proprio
	graph_bmp_canvas = graph_bmp->Canvas;
	// inicia coordenadas da tela virtual
	tela_virtual.x1 = 0;
	tela_virtual.y1 = 0;
	tela_virtual.x2 = graph_bmp->Width - 1;
	tela_virtual.y2 = graph_bmp->Height - 1;
	// define ponto de referencia p/ montagem da figura
	tela_virtual.x = (graph_bmp->Width - imag->Constraints->MaxWidth) / 2;
	tela_virtual.y = (graph_bmp->Height - imag->Constraints->MaxHeight) / 2;
	// define area da tela
	DefAreaTela();
	// redefine escala
	DefEscala();
	// reapresenta a rede
	graf->Redraw();
}

// ---------------------------------------------------------------------------
void __fastcall TFormGraf::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	// verifica se tratamento externo
	if (mouse.op != mouseEXTERNO)
		return;
	switch (Key)
	{
	case VK_DELETE: // envia evento mouseDELETE p/ Grafico
		graf->Mouse(mouseDELETE, mbLeft, Shift, 0, 0, 0, 0);
		break;
	case VK_RETURN: // envia evento mouseENTER p/ Grafico
		graf->Mouse(mouseENTER, mbLeft, Shift, 0, 0, 0, 0);
		break;
	case VK_ESCAPE: // envia evento mouseESCAPE p/ Grafico
		graf->Mouse(mouseESCAPE, mbLeft, Shift, 0, 0, 0, 0);
		break;
	case 'A': // envia evento mouseSELECT_ALL p/ Grafico //DVK 2014.08.01
	case 'T':
		if (Shift.Contains(ssCtrl))
		{
			graf->Mouse(mouseSELECT_ALL, mbLeft, Shift, 0, 0, 0, 0);
		}
		break;
	default: // criar evento teclado p/ ser enviado p/ Grafico
		break;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormGraf::FormMouseWheelDown(TObject *Sender, TShiftState Shift, TPoint &MousePos,
	bool &Handled)
{
	// proteção
	if (mouse.action != NULL)
		return;
	// excuta ZoomOut
	ActionZoomOut->Execute();
}

// ---------------------------------------------------------------------------
void __fastcall TFormGraf::FormMouseWheelUp(TObject *Sender, TShiftState Shift, TPoint &MousePos,
	bool &Handled)
{
	// proteção
	if (mouse.action != NULL)
		return;
	// excuta ZoomIn
	ActionZoomIn->Execute();
}

// ---------------------------------------------------------------------------
void __fastcall TFormGraf::FormResize(TObject *Sender)
{
	// proteção
	// JCG 2016.02.11
	// if ((rede.area->x1 < 0)||(! Visible))
	if (!Visible)
	{
		DefAreaTela();
		LimpaTela(tela.cor_fundo, false);
		RedrawTelaReal();
		return;
	}
	// redefine área de zoom
	ZoomDefineArea(rede.zoom->x1, rede.zoom->y1, rede.zoom->x2, rede.zoom->y2);
}

// -----------------------------------------------------------------------------
void __fastcall TFormGraf::ImageCursorMouse(TCursor cursor)
{
	// atualiza formato do cursor do mouse
	imag->Cursor = cursor;
	// truque p/ que o formato seja atualizado
	Mouse->CursorPos = Mouse->CursorPos;
}

// ---------------------------------------------------------------------------
// TFormGraf::imagMouseDown()  -  public
// ---------------------------------------------------------------------------
void __fastcall TFormGraf::imagMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
	int X, int Y)
{
	// salva coordenadas do mouse
	mouse.x = X;
	mouse.y = Y;
	// salva coordenadas para área de zoom
	tela.zoom->x1 = tela.zoom->x2 = X;
	tela.zoom->y1 = tela.zoom->y2 = Y;
	// verifica se botão direito
	if (Button == mbRight)
	{ // salva formato atual do mouse
		mouse.cursor = imag->Cursor;
		// identifica função: pan, zoom_área ou zoom_dy
		if (Shift.Contains(ssAlt))
		{ // pan
			mouse.action = ActionCfgMousePan;
		}
		else if (Shift.Contains(ssShift))
		{ // zoom_dy
			mouse.action = ActionCfgMouseZoomDy;
		}
		else if (Shift.Contains(ssCtrl))
		{ // zoom_area
			mouse.action = ActionCfgMouseZoomArea;
		}
		else
		{ // Action definida por um dos botões de cfg de mouse
			if (ButMouseZoomArea->Down)
				mouse.action = ActionCfgMouseZoomArea;
			else if (ButMouseZoomDy->Down)
				mouse.action = ActionCfgMouseZoomDy;
			else if (ButMousePan->Down)
				mouse.action = ActionCfgMousePan;
			else
				mouse.action = ActionCfgMouseZoomArea;
		}
		// altera formato do mouse de acordo c/ mouse.action
		if (mouse.action == ActionCfgMousePan)
			ImageCursorMouse(crHandPoint);
		else if (mouse.action == ActionCfgMouseZoomDy)
			ImageCursorMouse(TCursor(curZoom)); // curZoomdy
		else if (mouse.action == ActionCfgMouseZoomArea)
			ImageCursorMouse(crDrag); // curZoomarea
		return;
	}
	//verifica botao meio
	if (Button == mbMiddle)
	{
        AlteraFuncaoBotaoDireito();
	}
	// verifica se tratamento externo
	if ((mouse.op == mouseEXTERNO) && (Button == mbLeft))
	{ // envia evento p/ graf indicando as coordenadas UTM e coordenadas da tela
		graf->Mouse(mouseDOWN, Button, Shift, Xpixel_to_Xutm(X), Ypixel_to_Yutm(Y), X, Y);
		return;
	}
}

// ---------------------------------------------------------------------------
// TFormGraf::imagMouseMove()  -  public
// ---------------------------------------------------------------------------
void __fastcall TFormGraf::imagMouseMove(TObject *Sender, TShiftState Shift, int X, int Y)
{
	// variáveis locais
	int dy;

	// garante Focus em TFormGraf
	if (!Focused())
		SetFocus();
	// proteção: verifica se o mouse foi movimentado
	if ((mouse.x == X) && (mouse.y == Y))
		return;
	// salva novas coordenadas do mouse
	mouse.x = X;
	mouse.y = Y;
	// verifica se deve apresentar coordenadas do mouse
	if (sttbar->Visible)
	{ // exibe coordenadas
		ExibeCoordMouse();
	}
	// verifica se está fazendo pan/zoom c/ botão direito
	if (Shift.Contains(ssRight))
	{ // pan ou zoom: verifica se está fazendo zoom por seleção de área
		if (mouse.action == ActionCfgMouseZoomArea)
		{ // apaga retângulo da área anterior
			DrawRectZoom();
			// salva novo ponto da área de zoom
			tela.zoom->x2 = X;
			tela.zoom->y2 = Y;
			// redesenha retângulo para nova área de zoom
			DrawRectZoom();
		}
		else if (mouse.action == ActionCfgMouseZoomDy)
		{ // zoom por deslocamento vertical
			dy = Y - tela.zoom->y1;
			if (dy < 0)
				ActionZoomInExecute(NULL);
			else if (dy > 0)
				ActionZoomOutExecute(NULL);
			// salva coordenadas atuais do mouse
			tela.zoom->x1 = X;
			tela.zoom->y1 = Y;
		}
		else if (mouse.action == ActionCfgMousePan)
		{ // pan
			TrataPan(X - tela.zoom->x1, Y - tela.zoom->y1);
			// salva coordenadas atuais do mouse
			tela.zoom->x1 = X;
			tela.zoom->y1 = Y;
		}
		return;
	}
	// verifica se Wizard está ativo
	if (wizard)
	{
		if (wizard->Executa(PanelWizard, Xpixel_to_Xutm(X), Ypixel_to_Yutm(Y), tela.cor_fundo))
		{
			PanelWizard->Left = X + 12;
			PanelWizard->Top = Y;
			if (!PanelWizard->Visible)
				PanelWizard->Visible = true;
		}
		else
		{
			PanelWizard->Visible = false;
		}
	}
	// verifica se seleção de área ou tratamento externo
	if (mouse.op == mouseAREA)
	{ // apaga retângulo anterior
		DrawRectZoom();
		// salva novo ponto da área de zoom
		tela.zoom->x2 = X;
		tela.zoom->y2 = Y;
		// redesenha retângulo para nova área de zoom
		DrawRectZoom();
		return;
	}
	// verifica se tratamento externo
	if (mouse.op == mouseEXTERNO)
	{ // envia evento p/ graf indicando as coordenadas UTM e coordenadas da tela
		if (Shift.Contains(ssLeft))
			graf->Mouse(mouseMOVE, mbLeft, Shift, Xpixel_to_Xutm(X), Ypixel_to_Yutm(Y), X, Y);
		else
			graf->Mouse(mouseMOVE, TMouseButton(-1), Shift, Xpixel_to_Xutm(X),
			Ypixel_to_Yutm(Y), X, Y);
	}
}

// ---------------------------------------------------------------------------
// TFormGraf::imagMouseUp()  -  public
// ---------------------------------------------------------------------------
void __fastcall TFormGraf::imagMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
	int X, int Y)
{
	// variáveis locais
	int x1, y1, x2, y2;

	// converte as coordenadas da tela para área de zoom da rede
	x1 = Xpixel_to_Xutm(tela.zoom->x1); // CUIDADO: esta função usa rede.zoom
	y1 = Ypixel_to_Yutm(tela.zoom->y1);
	x2 = Xpixel_to_Xutm(tela.zoom->x2); // CUIDADO: esta função usa rede.zoom
	y2 = Ypixel_to_Yutm(tela.zoom->y2);
	// proteção necessária: imagem especular coord.Tela e as coord.UTM
	// if (x1 < 0) x1 = 0;
	// if (x2 < 0) x2 = 0;
	// if (y1 < 0) y1 = 0;
	// if (y2 < 0) y2 = 0;
	if (x2 < x1)
	{
		int aux = x1;
		x1 = x2;
		x2 = aux;
	}
	if (y2 < y1)
	{
		int aux = y1;
		y1 = y2;
		y2 = aux;
	}
	// verifica se está fazendo pan/zoom
	if (Button == mbRight)
	{ // pan ou zoom: restaura formato do mouse
		ImageCursorMouse(mouse.cursor);
		// verifica se estava fazendo zoom por deslocamento vertical
		if (mouse.action == ActionCfgMouseZoomDy)
		{ // redefine área de zoom c/ a área mostrada na janela virtual
			ZoomExibeJanelaVirtual();
		}
		// verifica se estava fazendo zoom por seleção de área
		else if (mouse.action == ActionCfgMouseZoomArea)
		{ // apaga retângulo apresentaddo
			DrawRectZoom();
			// proteção contra click espúreo sem seleção de área
			if ((tela.zoom->x1 == tela.zoom->x2) && (tela.zoom->y1 == tela.zoom->y2))
				return;
			// salva área de zooom atual
			ZoomSalva();
			// redefine área de zoom
			ZoomDefineArea(x1, y1, x2, y2);
		}
		// redefine função do botâo direito do mouse
		// mouse.action = ButCfgMouse->Action;
		mouse.action = NULL;
		return;
	}
	// verifica se seleção de área ou tratamento externo
	if (mouse.op == mouseAREA)
	{ // tratamento externo
		DrawRectZoom();
		graf->SelArea(x1, y1, x2, y2);
		return;
	}
	if (mouse.op == mouseEXTERNO)
	{ // envia evento p/ graf indicando as coordenadas UTM e coordenadas da tela
		graf->Mouse(mouseUP, Button, Shift, Xpixel_to_Xutm(X), Ypixel_to_Yutm(Y), X, Y);
	}
}

// ---------------------------------------------------------------------------
// TFormGraf::IniciaBitMaps()  -  private
// ---------------------------------------------------------------------------
void __fastcall TFormGraf::IniciaBitMaps(void)
{
	// variáveis locais
	AnsiString arq_bmp;
	TPicture *bmp;
	int bmp_id[bmpMAX] =
	{bmpCAPACITOR, bmpCAPSERIE, bmpCARGA, bmpFILTRO, bmpGERADOR, bmpMEDIDOR, bmpMUTUA, bmpREATOR,
		bmpREGULADOR, bmpSUPRIMENTO, bmpTRAFO_H, bmpTRAFO_V, bmpTRAFO3E_H, bmpTRAFO3E_V, bmpTRAFOZZ,
		bmpZREF};
	// IMPORTANTE: os bitmaps devem ser carregados na mesma ordem do enumerador enumBMP
	for (int n = 0; n < (sizeof(bmp_id) / sizeof(int)); n++)
	{ // verifica se existe o arquivo de bitmap
		arq_bmp = path->ArqBitmap(bmp_id[n]);
		if (arq_bmp.IsEmpty())
			continue;
		if (!FileExists(arq_bmp))
			continue;
		// cria novo objeto TPicture e insere em lisBMP
		lisBMP->Add(bmp = new TPicture());
		bmp->LoadFromFile(arq_bmp);
		// bmp->Icon->Transparent = true;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormGraf::JanelaVirtualDefineArea(int x1, int y1, int x2, int y2)
{
	// verifica se é um Pan inicial
	if ((janela_virtual.fatx == 1) && (janela_virtual.faty == 1))
	{ // início de pan: verifica necessidade de "engrossar linhas"
		if (graf->Padrao->ligacao.dim == 1)
		{
			graf->Padrao->ligacao.dim = 2;
			graf->Redraw();
			graf->Padrao->ligacao.dim = 1;
		}
	}
	// proteção
	if (x1 < tela_virtual.x1)
		x1 = tela_virtual.x1;
	if (y1 < tela_virtual.y1)
		y1 = tela_virtual.y1;
	if (x2 > tela_virtual.x2)
		x2 = tela_virtual.x2;
	if (y2 > tela_virtual.y2)
		y2 = tela_virtual.y2;
	// redefine área da janela virtual
	janela_virtual.x1 = x1;
	janela_virtual.y1 = y1;
	janela_virtual.x2 = x2;
	janela_virtual.y2 = y2;
	// recalcula fator de escala entre janela virtual e tela real
	janela_virtual.fatx = double((janela_virtual.x2 - janela_virtual.x1 + 1)) / double
		(tela.area->Width());
	janela_virtual.faty = double((janela_virtual.y2 - janela_virtual.y1 + 1)) / double
		(tela.area->Height());
	// verifica necessidade de redesenhar a rede
	if ((janela_virtual.fatx < 0.6) || (janela_virtual.fatx > 1.4) || (janela_virtual.faty < 0.6) ||
		(janela_virtual.faty > 1.4))
	{ // redefine área de zoom c/ a área mostrada na janela virtual
		ZoomExibeJanelaVirtual();
	}
	else
	{ // atualiza tela real
		RedrawTelaReal();
		// sinaliza alteração de zoom
		graf->AreaZoomAlterada();
	}
}

// ---------------------------------------------------------------------------
// OBS: ESTA IMPLEMENTAÇÃO É MELHOR, POIS EVITA A TELA PISCAR. MAS PARA TRABALHAR
// SEM BORDA, PRECISA DE MAIS UM PARÂMETRO
// ---------------------------------------------------------------------------
void __fastcall TFormGraf::LimpaTela(TColor cor, bool borda)
{
	// salva cor na varíavel da classe cor_fundo, necessária quando esta função é
	// chamada internamente
	tela.cor_fundo = cor;
	// pinta toda a área da tela
	// TRect r(0, 0, graph_bmp->Width, graph_bmp->Height);
	// TRect r(0, 0, tela_virtual.x2 - tela_virtual.x1, tela_virtual.y2 - tela_virtual.y1);
	TRect r(tela_virtual.x1, tela_virtual.y1, tela_virtual.x2, tela_virtual.y2);
	graph_bmp_canvas->Brush->Style = bsSolid;
	graph_bmp_canvas->Brush->Color = tela.cor_fundo;
	graph_bmp_canvas->FillRect(r);
	// RedrawTelaReal();
	// carrega bitmap de cartografia
	if (google.cartografia)
	{
		AreaVisivel(google.area);
		google.cartografia->CopiaBitmap(google.area, graph_bmp_canvas, janela_virtual.x1,
			janela_virtual.y1, janela_virtual.x2, janela_virtual.y2, escala.fatx, tipo_coord);
	}
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/15000938
void __fastcall TFormGraf::MapaCarregaArea(void)
{
	// variáveis locais
	bool area_disponivel = false;
	int map_type;
	VTRedegraf *redegraf = (VTRedegraf*)apl->GetObject(__classid(VTRedegraf));

	// proteção
	if (google.cartografia == NULL)
		return;
	// determina área visível da rede (em cm)
	AreaVisivel(google.area);
	// define tipo de mapa
	if (MenuItemGoogleRoadMap->Checked)
		map_type = mapaROADMAP;
	else if (MenuItemGoogleSatellite->Checked)
		map_type = mapaSATELLITE;
	else if (MenuItemGoogleHybrid->Checked)
		map_type = mapaHYBRID;
	else if (MenuItemGoogleTerrain->Checked)
		map_type = mapaTERRAIN;
	else if (MenuItemMapaServidorCopel->Checked)
		map_type = mapaCOPEL;
	else
		map_type = mapaROADMAP;

	//se o mapa for da Copel, força cor de fundo branca
	if(map_type == mapaCOPEL)
	{	// troca configuração de cor de fundo
		redegraf->CorFundo = clWhite;
//		graf->Redraw();
	}
	else
	{   //volta ao default
		redegraf->CorFundo = clSilver;
//		graf->Redraw();
    }
	// converte área da rede de cm => m
	if (google.cartografia->CarregaArea_utm(map_type, google.area, tela.area, area_disponivel,
		tipo_coord))
	{ // verifica se área já está disponível
		if (area_disponivel)
		{ // sinaliza que o mapa já está disponível
			ActionMapaDispExecute((TObject*)1);
		}
		else
		{ // muda cursor do mouse p/ ampulheta
			ImageCursorMouse(crHourGlass);
		}
	}
}

/*
 //---------------------------------------------------------------------------
 bool __fastcall TFormGraf::MapaDefineEscala(void)
 {
 //proteção
 if (google.cartografia == NULL) return(false);
 if (! google.cartografia->DefineEscala(rede.zoom, tela.area)) return(false);
 return(true);
 }
 */
// ---------------------------------------------------------------------------
void __fastcall TFormGraf::MapaEnabled(bool enabled)
{
	// habilita/desabilita Action
	ActionMapa->Enabled = enabled;
	// exibe/esconde sttbar
	sttbar->Visible = enabled;
	// verifica se o Mapa está sendo exibido
	if ((google.cartografia) && (!enabled))
	{ // destrói objeto
		delete google.cartografia;
		google.cartografia = NULL;
		// restaura espessura da linha p/ desenhar traçado da rede
		graf->Padrao->ligacao.dim = 1;
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TFormGraf::MapaVisible(void)
{
	return (google.cartografia != NULL);
}
// -----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16014737
TCanvas* __fastcall TFormGraf::PM_GetCanvas(void)
{
	return(graph_bmp->Canvas);
}
// ---------------------------------------------------------------------------
void __fastcall TFormGraf::RedrawTelaReal(void)
{
	// variáveis locais
	TRect OrigRect(janela_virtual.x1, janela_virtual.y1, janela_virtual.x2, janela_virtual.y2);
	TRect DestRec(tela.area->x1, tela.area->y1, tela.area->x2, tela.area->y2);

	// proteção
	if (graph_bmp_canvas != graph_bmp->Canvas)
		return;
	// copia área visível de Image1 p/ Image2
	imag->Canvas->CopyRect(DestRec, graph_bmp_canvas, OrigRect);
}

// ---------------------------------------------------------------------------
int __fastcall TFormGraf::SizeOfBitmap(int ind_bmp)
{
	// variáveis locais
	TPicture *bmp;

	// proteção
	if (ind_bmp >= lisBMP->Count)
		return (-1);
	// determina bitmap
	bmp = (TPicture*)lisBMP->Items[ind_bmp];
	return (Max(bmp->Height, bmp->Width));
}

// ---------------------------------------------------------------------------
int __fastcall TFormGraf::TextHeight(AnsiString txt)
{
	return (graph_bmp_canvas->TextHeight(txt));
}

// ---------------------------------------------------------------------------
int __fastcall TFormGraf::TextWidth(AnsiString txt)
{
	return (graph_bmp_canvas->TextWidth(txt));
}

// ---------------------------------------------------------------------------
void __fastcall TFormGraf::TimerTimer(TObject *Sender)
{ // DVK 2017.02.01
	// desabilita o Timer
	Timer->Enabled = false;
	// atualiza o mapa carregado
	MapaCarregaArea();
}

// ---------------------------------------------------------------------------
void __fastcall TFormGraf::TrataPan(int dx, int dy)
{
	// variáveis locais
	int x1, y1, x2, y2;
	bool redefine_area_zoom = false;

	// determina novas coordenadas p/ janela virtual
	if ((x1 = janela_virtual.x1 - dx) < tela_virtual.x1)
	{
		x1 = tela_virtual.x1;
		redefine_area_zoom = true;
	}
	if ((y1 = janela_virtual.y1 - dy) < tela_virtual.y1)
	{
		y1 = tela_virtual.y1;
		redefine_area_zoom = true;
	}
	if ((x2 = janela_virtual.x2 - dx) > tela_virtual.x2)
	{
		x2 = tela_virtual.x2;
		redefine_area_zoom = true;
	}
	if ((y2 = janela_virtual.y2 - dy) > tela_virtual.y2)
	{
		y2 = tela_virtual.y2;
		redefine_area_zoom = true;
	}
	// proteção: verifica se a janela virtual ultrapassa tela virtual
	if (redefine_area_zoom)
	{ // redefine área de zoom: área da rede correspondente à janela virtual
		ZoomExibeJanelaVirtual();
	}
	else
	{ // desloca imagem movendo a janela virtual p/ posição contrária
		JanelaVirtualDefineArea(x1, y1, x2, y2);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormGraf::VerificaHeap(void)
{
}

// ---------------------------------------------------------------------------
int __fastcall TFormGraf::Xpixel_to_Xutm(int x_pixel)
{
	// variáveis locais
	double x_utm;

	// corrige coordenada x em função do fator de escala entre janela virtual e tela real
	x_pixel = (janela_virtual.fatx * x_pixel) - tela.x0;
	// corrige coordenada x em função de janela_virtual.x1
	x_pixel = x_pixel + janela_virtual.x1;
	// corrige coordenada x em função de tela_virtual.x
	x_pixel = x_pixel - tela_virtual.x;
	// converte coordenada x da tela para área de zoom
	x_utm = (x_pixel / escala.fatx) + rede.zoom->x1;
	(x_utm < 0) ? x_utm -= .5 : x_utm += .5;
	return (int(x_utm));
}

// ---------------------------------------------------------------------------
int __fastcall TFormGraf::Ypixel_to_Yutm(int y_pixel)
{
	// variáveis locais
	double y_utm;

	// corrige coordenada y em função do fator de escala entre janela virtual e tela real
	y_pixel = (janela_virtual.faty * y_pixel);
	// corrige coordenada y em função de janela_virtual.y1
	y_pixel = y_pixel + janela_virtual.y1;
	// corrige coordenada y em função de tela_virtual.y
	y_pixel = y_pixel - tela_virtual.y;
	// inverte ordenada video/UTM (imagem especular)
	y_pixel = tela.area->y1 + (tela.area->y2 - y_pixel);
	// corrige coordenada y em função de tela.y0
	y_pixel = y_pixel - tela.y0;
	// converte coordenada y da tela para área de zoom
	y_utm = (y_pixel / escala.faty) + rede.zoom->y1;
	(y_utm < 0) ? y_utm -= .5 : y_utm += .5;
	return (int(y_utm));
}

// ---------------------------------------------------------------------------
int __fastcall TFormGraf::Xutm_to_Xpixel(int x_utm)
{
	// variáveis locais
	double x_pixel;

	// tmp = tela.area->x1 + ((x - rede.zoom->x1) * escala.fatx);
	x_pixel = tela.x0 + ((x_utm - rede.zoom->x1) * escala.fatx);
	(x_pixel < 0) ? x_pixel -= .5 : x_pixel += .5;
	return (tela_virtual.x +int(x_pixel));
}

// ---------------------------------------------------------------------------
int __fastcall TFormGraf::Yutm_to_Ypixel(int y_utm)
{
	// variáveis locais
	double y_pixel;

	// converte utm p/ pixel
	y_pixel = (y_utm - rede.zoom->y1) * escala.faty;
	(y_pixel < 0) ? y_pixel -= .5 : y_pixel += .5;
	// desloca y_pixel em relação tela.y0
	y_pixel = tela.y0 + y_pixel;
	// inverte ordenada video/UTM (imagem especular)
	y_pixel = tela.area->y1 + (tela.area->y2 - y_pixel);
	return (tela_virtual.y + (int)y_pixel);
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/15000938
// https://sinapsisenergia.teamwork.com/#/tasks/16681470
void __fastcall TFormGraf::ZoomDefineArea(int x1, int y1, int x2, int y2)
{
	// variáveis locais
	int dx, dy, iteracao = 0;
	double fat_escala, width, height;

	// proteção
	if (graf == NULL)
		return;
	// if ((rede.zoom->x1 == x1)&&(rede.zoom->y1 == y1)&&
	// (rede.zoom->x2 == x2)&&(rede.zoom->y2 == y2)) return;
	// reinicia a área da tela real e da janela virtual
	DefAreaTela();
	// redefine a área de zoom
	rede.zoom->x1 = x1;
	rede.zoom->y1 = y1;
	rede.zoom->x2 = x2;
	rede.zoom->y2 = y2;

	// verifica se está usando Mapa
	if (google.cartografia != NULL)
	{ // redefine dimensoes do formMapa
		int w = tela.area->Width();
		int h = tela.area->Height();
		google.cartografia->Redimensiona(w, h);
		// calcula fator de escala de Mapa
//		google.cartografia->DefineEscala(rede.zoom, tela.area, tipo_coord);
		google.cartografia->DefineEscala(rede.zoom, tela.area);
		// verifica se usuário selecionou opção de utilizar mesma escala de zoom do Google
		if (MenuItemMapaEscalaGoogle->Checked)
		{
//jss20191014
/*
			do
			{ // lê escala do mapa no site
				fat_escala = google.cartografia->Escala_pixel_cm;
				// proteção
				if (IsDoubleZero(fat_escala))
				{
					// fat_escala = 1.;
					break;
				}
				// acerta zoom da rede p/ garantir mesma escala c/ site
				width = tela.area->Width() / fat_escala;
				height = tela.area->Height() / fat_escala;
				dx = (rede.zoom->Width() - width) / 2;
				dy = (rede.zoom->Height() - height) / 2;
				// redefine área de zoom da rede
				rede.zoom->x1 += dx;
				rede.zoom->x2 -= dx;
				rede.zoom->y1 += dy;
				rede.zoom->y2 -= dy;
				// redefine fator de escala de Mapa
				google.cartografia->DefineEscala(rede.zoom, tela.area, tipo_coord);
			}
			while ((dx != 0) && (dy != 0) && (iteracao++ < 3));
*/
			// redefine dimensoes do formMapa
//			int w = tela.area->Width();
//			int h = tela.area->Height();
//			google.cartografia->Redimensiona(w, h);
		}
		// redefine fator de escala entre área de zoom e tela
		DefEscala();
		// sinaliza alteração de zoom
		graf->AreaZoomAlterada();
		// define tipo de mapa e carrega área antes e depois do timer
		MapaCarregaArea();
		// habilita timer //DVK 2017.02.01
		Timer->Interval = 3000;
		Timer->Enabled = true;
		// NÃO reapresenta a rede: aguarda evento p/ reapresentar a rede
		// graf->Redraw();
	}
	else
	{ // redefine fator de escala entre área de zoom e tela
		DefEscala();
		// sinaliza alteração de zoom
		graf->AreaZoomAlterada();
		// reapresenta a rede
		graf->Redraw();
	}
	// atualiza statusBar
	ExibeDimRede();
}

/* //carrega 1x
 //---------------------------------------------------------------------------
 void __fastcall TFormGraf::ZoomDefineArea(int x1, int y1, int x2, int y2)
 {
 //variáveis locais
 int    dx, dy, iteracao = 0;
 double fat_escala, width, height;

 //proteção
 if (graf == NULL) return;
 //if ((rede.zoom->x1 == x1)&&(rede.zoom->y1 == y1)&&
 //    (rede.zoom->x2 == x2)&&(rede.zoom->y2 == y2)) return;
 //reinicia a área da tela real e da janela virtual
 DefAreaTela();
 //redefine a área de zoom
 rede.zoom->x1 = x1;
 rede.zoom->y1 = y1;
 rede.zoom->x2 = x2;
 rede.zoom->y2 = y2;
 //verifica se está usando Mapa
 if (google.cartografia != NULL)
 {//calcula fator de escala de Mapa
 google.cartografia->DefineEscala(rede.zoom, tela.area);
 //verifica se usuário selecionou opção de utilizar mesma escala de zoom do Google
 if (MenuItemMapaEscalaGoogle->Checked)
 {
 //lê escala do mapa no site
 fat_escala = google.cartografia->Escala_pixel_cm;
 //proteção
 if (IsDoubleZero(fat_escala))
 {
 fat_escala = 1.;
 //break;
 }
 //acerta zoom da rede p/ garantir mesma escala c/ site
 width  = tela.area->Width()  / fat_escala;
 height = tela.area->Height() / fat_escala;
 dx     = (rede.zoom->Width()  - width)  / 2;
 dy     = (rede.zoom->Height() - height) / 2;
 //redefine área de zoom da rede
 rede.zoom->x1 += dx;
 rede.zoom->x2 -= dx;
 rede.zoom->y1 += dy;
 rede.zoom->y2 -= dy;
 //redefine fator de escala de Mapa
 google.cartografia->DefineEscala(rede.zoom, tela.area);
 }
 //redefine fator de escala entre área de zoom e tela
 DefEscala();
 //sinaliza alteração de zoom
 graf->AreaZoomAlterada();
 //define tipo de mapa e carrega área
 MapaCarregaArea();
 //NÃO reapresenta a rede: aguarda evento p/ reapresentar a rede
 //graf->Redraw();
 }
 else
 {//redefine fator de escala entre área de zoom e tela
 DefEscala();
 //sinaliza alteração de zoom
 graf->AreaZoomAlterada();
 //reapresenta a rede
 graf->Redraw();
 }
 //atualiza statusBar
 ExibeDimRede();
 }
 */
// ---------------------------------------------------------------------------
void __fastcall TFormGraf::ZoomExibeJanelaVirtual(void)
{
	// variáveis locais
	int x1, y1, x2, y2;

	// calcula coordenadas p/ área de zoom
	x1 = Xpixel_to_Xutm(tela.area->x1 - 3); // OBS: -3 => número 'mágico' p/ evitar salto na imagem
	y1 = Ypixel_to_Yutm(tela.area->y1);
	x2 = Xpixel_to_Xutm(tela.area->x2 - 3); // OBS: -3 => número 'mágico' p/ evitar salto na imagem
	y2 = Ypixel_to_Yutm(tela.area->y2);
	if (x2 < x1)
	{
		int aux = x1;
		x1 = x2;
		x2 = aux;
	}
	if (y2 < y1)
	{
		int aux = y1;
		y1 = y2;
		y2 = aux;
	}
	// reinicia a área da tela real e da janela virtual
	// DefAreaTela();
	// redefine a área de zoom
	ZoomDefineArea(x1, y1, x2, y2);
}

// ---------------------------------------------------------------------------
void __fastcall TFormGraf::ZoomSalva(void)
{
	// variáveia locais
	VTArea *area;

	// proteção:
	if (rede.zoom->Width() <= 0)
		return;
	// cria novo objeto VTArea e salva área atual de zoom
	area = DLL_NewObjArea();
	area->CopiaAtributosDe(*rede.zoom);
	// salva área do zoom atual na lista
	lisZOOM->Add(area);
	// habilita opção de menu
	ActionZoomAnterior->Enabled = (lisZOOM->Count > 0);
}

// ---------------------------------------------------------------------------
// eof
