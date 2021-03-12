//---------------------------------------------------------------------------
#ifndef TFormSobreSINAPrntH
#define TFormSobreSINAPrntH

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

//---------------------------------------------------------------------------
class VTApl;

//---------------------------------------------------------------------------
class TFormSobreSINAPrnt : public TForm
{
__published:	// IDE-managed Components
   TToolBar *ToolBar;
   TToolButton *butFecha;
   TPanel *Panel1;
   TLabel *LabelRevisao;
	TImage *Image;
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall butFechaClick(TObject *Sender);
   void __fastcall ButSiteSinapsisClick(TObject *Sender);

public:		// User declarations
   __fastcall TFormSobreSINAPrnt(TComponent* Owner, VTApl *apl);
   __fastcall ~TFormSobreSINAPrnt(void);

private:	// User declarations};
   };

//---------------------------------------------------------------------------
#endif
//eof
