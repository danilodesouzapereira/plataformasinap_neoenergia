//---------------------------------------------------------------------------
#ifndef TFormGoogleH
#define TFormGoogleH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <OleCtrls.hpp>
#include <ExtCtrls.hpp>
#include <SHDocVw.hpp>
#include <ComCtrls.hpp>
#include <Dialogs.hpp>
#include <Menus.hpp>
#include "cgrid.h"
#include <ActnColorMaps.hpp>
#include <ActnMan.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <System.ImageList.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTArvore;
class VTScg;

//---------------------------------------------------------------------------
class TFormGoogle : public TForm
{
__published:   // IDE-managed Components
   TWebBrowser *WebBrowser;
   TGroupBox *GBoxRede;
   TTreeView *TView;
   TButton *BotaoKML;
   TSaveDialog *SaveDialog1;
   TSplitter *Splitter1;
	TToolBar *ToolBar1;
	TToolButton *maiszoom;
	TImageList *ImageList1;
	TToolButton *menoszoom;
	TToolButton *ToolButton3;
	TToolButton *ToolButton4;
	TToolButton *ToolButton1;
	TToolButton *ToolButton2;


   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall WebBrowserDocumentComplete(TObject *ASender, const IDispatch *pDisp, const OleVariant &URL);
   void __fastcall BotaoKMLClick(TObject *Sender);
   void __fastcall TViewDblClick(TObject *Sender);
	void __fastcall maiszoomClick(TObject *Sender);
	void __fastcall menoszoomClick(TObject *Sender);
   void __fastcall FormShow(TObject *Sender);


public:      // User declarations
        __fastcall  TFormGoogle(TComponent* Owner, VTApl *apl, TWinControl *parent);
        __fastcall ~TFormGoogle(void);

private:   //métodos
	void       __fastcall AtualizaMapa(void);
   void       __fastcall DesenhaRede(void);
   void       __fastcall FormIniciaPosicao(void);
   AnsiString __fastcall HTML_Ponto(double lat, double lon);
	void       __fastcall IniciaMapa(void);
   bool       __fastcall LoadMapa(AnsiString map_type, double lat, double lon, int zoom, bool draw_rede);
   void       __fastcall TViewInicia(void);


private:   //objetos externos
   VTApl   *apl;

private:   //dados locais
   int          map_type;
	int 		    zoom;
   double       z0;  //relação pixel/grau do zoom 0 do google, empírico = 0,713
   TStringList  *Lines;
	VTArvore     *arvore;
   VTScg        *scg;
};

//---------------------------------------------------------------------------
#endif
//eof
