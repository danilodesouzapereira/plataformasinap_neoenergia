//---------------------------------------------------------------------------
#ifndef TFormLogAjustaDemandaH
#define TFormLogAjustaDemandaH

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
#include <System.ImageList.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTAjusta;

//---------------------------------------------------------------------------
class TFormLogAjustaDemanda : public TForm
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
	TGroupBox *GroupBox1;
	TCheckBox *CboxTrifasico;
	TCheckBox *CboxFase;
	TToolButton *ToolButton3;
	TAction *ActionLimpa;
   void __fastcall ActionFecharExecute(TObject *Sender);
	void __fastcall ActionExportarExecute(TObject *Sender);
	void __fastcall ActionLimpaExecute(TObject *Sender);

private:	// User declarations
	void __fastcall SalvaLog(bool append = true);

public:		// User declarations
			__fastcall TFormLogAjustaDemanda(TComponent* Owner, VTApl *apl_owner);
         __fastcall ~TFormLogAjustaDemanda(void);
	void  __fastcall Executa(TList *lisMedicao, bool ajuste_por_fase, VTAjusta *ajusta);
	void  __fastcall Importa(AnsiString arquivo);

private: //objs externos
   VTApl *apl;

};

//---------------------------------------------------------------------------
#endif
