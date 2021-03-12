//---------------------------------------------------------------------------
#ifndef TFormLogMedicaoH
#define TFormLogMedicaoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <Dialogs.hpp>
#include <System.Actions.hpp>

//---------------------------------------------------------------------------
class VTApl;

//---------------------------------------------------------------------------
class TFormLogMedicao : public TForm
{
__published:	// IDE-managed Components
   TToolBar *ToolBar1;
   TListView *LViewLog;
   TToolButton *ToolButton1;
   TToolButton *ToolButton2;
   TActionList *ActionList1;
   TAction *ActionFechar;
   TAction *ActionExportar;
   TImageList *ImageList;
   TSaveDialog *SaveDialogExporta;
   void __fastcall ActionFecharExecute(TObject *Sender);
	void __fastcall ActionExportarExecute(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormShow(TObject *Sender);

private:	// User declarations
	void __fastcall SalvaLog(bool append = true);

public:		// User declarations
			__fastcall TFormLogMedicao(TComponent* Owner, VTApl *apl_owner);
         __fastcall ~TFormLogMedicao(void);
	void  __fastcall Executa(TStringList* strDIAG_MED);

private: //objs externos
   VTApl *apl;

};

//---------------------------------------------------------------------------
#endif
