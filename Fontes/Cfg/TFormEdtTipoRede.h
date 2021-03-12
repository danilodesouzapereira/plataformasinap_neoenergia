//---------------------------------------------------------------------------

#ifndef TFormEdtTipoRedeH
#define TFormEdtTipoRedeH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <System.Actions.hpp>
//---------------------------------------------------------------------------
class VTTipoRede;
class VTApl;

//---------------------------------------------------------------------------
class TFormEdtTipoRede : public TForm
{
__published:	// IDE-managed Components
   TLabel *Label1;
   TLabel *Label2;
   TLabel *Label3;
   TToolBar *ToolBar1;
   TToolButton *butConfirma;
   TToolButton *butCancela;
   TPanel *PanelColor;
   TEdit *EditTipoRede;
   TActionList *ActionList1;
   TAction *ActionCancela;
   TAction *ActionConfirma;
   TAction *ActionCor;
   TImageList *ImageList1;
   TColorDialog *ColorDialog;
   TEdit *EditRedeDescr;
   TPanel *Panel1;
   TPanel *Panel2;
   TPanel *Panel3;
   TPanel *Panel4;
   TPanel *Panel5;
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall ActionConfirmaExecute(TObject *Sender);
   void __fastcall ActionCorExecute(TObject *Sender);

private:	// User declarations

public:		// User declarations
         __fastcall TFormEdtTipoRede(TComponent* Owner, VTApl *apl, VTTipoRede *tiporede);
         __fastcall ~TFormEdtTipoRede(void);
   void  __fastcall Inicia(void);
   void  __fastcall LeDados(void);

private:	//objetos externos
   VTApl      *apl;

private:	//dados locais
   VTTipoRede *TipoRedeOrig, *tiporede;
};

//---------------------------------------------------------------------------
#endif
