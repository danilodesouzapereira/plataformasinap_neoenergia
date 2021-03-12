//---------------------------------------------------------------------------

#ifndef TFormVisualizarZ0Z1H
#define TFormVisualizarZ0Z1H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.ToolWin.hpp>
#include <Vcl.Dialogs.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTArranjos;

//---------------------------------------------------------------------------
class TFormVisualizarZ0Z1 : public TForm
	{
	__published:	// IDE-managed Components
		TToolBar *ToolBar;
		TToolButton *ButConfirma;
		TActionList *ActionList;
		TAction *ActionFechar;
		TListView *LViewZ0Z1;
		TImageList *ImageList;
		TAction *ActionExporta;
		TToolButton *ToolButton1;
		TSaveDialog *SaveDialog;
		void __fastcall ActionFecharExecute(TObject *Sender);
		void __fastcall FormShow(TObject *Sender);
		void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
		void __fastcall ActionExportaExecute(TObject *Sender);

	private:	// User declarations
		void __fastcall LViewZ0Z1Inicia(void);

	public:		// User declarations
		__fastcall TFormVisualizarZ0Z1(TComponent* Owner, VTArranjos *arranjos, VTApl *apl);
		__fastcall ~TFormVisualizarZ0Z1(void);

	private:	// obj. externo
		VTApl		  *apl;
		VTArranjos *arranjos;
	};

//---------------------------------------------------------------------------
#endif
