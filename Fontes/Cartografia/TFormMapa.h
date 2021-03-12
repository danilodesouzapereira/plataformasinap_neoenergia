//---------------------------------------------------------------------------
#ifndef TFormMapaH
#define TFormMapaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <OleCtrls.hpp>
#include <ExtCtrls.hpp>
#include <SHDocVw.hpp>

//---------------------------------------------------------------------------
class VTApl;

//---------------------------------------------------------------------------
class TFormMapa : public TForm
{
__published:   // IDE-managed Components
   TTimer *Timer;
   TWebBrowser *WebBrowser;
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall TimerTimer(TObject *Sender);
   void __fastcall WebBrowserDocumentComplete(TObject *ASender,
          const IDispatch *pDisp, OleVariant &URL);

public:      // User declarations
        __fastcall  TFormMapa(TComponent* Owner, VTApl *apl, TWinControl *parent, TAction *ActionMapaDisp);
        __fastcall ~TFormMapa(void);
//jss20191014
//   bool __fastcall  LoadMapa(AnsiString map_type, double lat, double lon, int zoom);
   bool __fastcall  LoadMapa(AnsiString map_type, double lat, double lon, double zoom);
//   bool __fastcall  LoadMapaCopel(double lat1, double lon1, double lat2, double lon2);
   bool __fastcall  LoadMapaCopelUTM(double x1, double y1, double x2, double y2);
   void __fastcall  Start(void);
   void __fastcall  Stop(void);

private:   //objetos externos
   TAction *ActionMapaDisp;
   VTApl   *apl;

private:   //dados locais
   TStringList *Lines;
};

//---------------------------------------------------------------------------
#endif
//eof
