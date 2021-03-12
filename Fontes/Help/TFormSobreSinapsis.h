//---------------------------------------------------------------------------
#ifndef TFormSobreSinapsisH
#define TFormSobreSinapsisH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <Graphics.hpp>
#include <jpeg.hpp>
#include <OleCtrls.hpp>
#include <SHDocVw.hpp>

//---------------------------------------------------------------------------
class VTApl;

//---------------------------------------------------------------------------
class TFormSobreSinapsis : public TForm
{
__published:	// IDE-managed Components
   TToolBar *ToolBar;
   TToolButton *butFecha;
	TWebBrowser *WebBrowser;
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall butFechaClick(TObject *Sender);

public:		// User declarations
   __fastcall TFormSobreSinapsis(TComponent* Owner, VTApl *apl);
   __fastcall ~TFormSobreSinapsis(void);

private:	// User declarations};
   };

//---------------------------------------------------------------------------
#endif
//eof
