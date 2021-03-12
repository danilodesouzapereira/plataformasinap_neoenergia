//---------------------------------------------------------------------------

#ifndef TFormEdtTipoChaveH
#define TFormEdtTipoChaveH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ActnList.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <ExtCtrls.hpp>
#include <Dialogs.hpp>
#include <System.Actions.hpp>

//---------------------------------------------------------------------------
class VTTipoChave;
class VTApl;

//---------------------------------------------------------------------------
class TFormEdtTipoChave : public TForm
{
__published:	// IDE-managed Components
   TLabel *Label1;
   TLabel *Label2;
   TLabel *Label3;
   TComboBox *CBoxOperacao;
   TToolBar *ToolBar1;
   TToolButton *butConfirma;
   TToolButton *butCancela;
   TActionList *ActionList1;
   TAction *ActionCancela;
   TAction *ActionConfirma;
   TImageList *ImageList1;
   TPanel *PanelColor;
   TEdit *EditTipoChv;
   TAction *ActionCor;
   TColorDialog *ColorDialog;
   TPanel *Panel1;
   TPanel *Panel2;
   TPanel *Panel3;
   TPanel *Panel4;
   TPanel *Panel5;
   void __fastcall ActionConfirmaExecute(TObject *Sender);
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall ActionCorExecute(TObject *Sender);

private:	// User declarations

public:		// User declarations
         __fastcall TFormEdtTipoChave(TComponent* Owner, VTApl *apl, VTTipoChave *tipochave);
         __fastcall ~TFormEdtTipoChave(void);
   void  __fastcall Inicia(void);
   void  __fastcall LeDados(void);

private:	//objetos externos
   VTApl      *apl;

private:	//dados locais
   VTTipoChave *TipoChaveOrig, *tipochave;
};

//---------------------------------------------------------------------------
#endif
