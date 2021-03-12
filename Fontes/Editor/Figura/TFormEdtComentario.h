//---------------------------------------------------------------------------
#ifndef TFormEdtComentarioH
#define TFormEdtComentarioH

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
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.Buttons.hpp>
#include <System.ImageList.hpp>
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Figura\VTFiguraComentario.h>

//---------------------------------------------------------------------------
class VTRede;

//---------------------------------------------------------------------------
class TFormEdtComentario : public TForm
{
__published:	// IDE-managed Components
   TToolBar *ToolBar;
   TToolButton *butConfirma;
   TToolButton *butCancela;
   TToolButton *butRetira;
   TImageList *ImageList;
   TActionList *ActionList;
   TAction *ActionCancela;
   TAction *ActionConfirma;
   TAction *ActionRetira;
   TGroupBox *GBoxTexto;
   TMemo *Memo;
   TFontDialog *FontDialog;
   TAction *ActionSelFonte;
   TGroupBox *GBoxRede;
   TComboBox *CBoxRede;
   TAction *ActionSelCorFundo;
   TColorDialog *ColorDialog;
   TPanel *PanelTop;
   TRadioGroup *RadioGroupFormato;
   TGroupBox *GBoxMoldura;
   TLabel *Label1;
   TLabel *Label2;
   TLabel *Label3;
   TLabel *Label4;
   TComboBox *CBoxBordaEspessura;
   TComboBox *CBoxBordaEstilo;
   TAction *ActionSelCorBorda;
   TPanel *PanelFundoCor;
   TPanel *PanelBordaCor;
   TPanel *Panel1;
   TBitBtn *ButFont;
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall ActionConfirmaExecute(TObject *Sender);
   void __fastcall ActionRetiraExecute(TObject *Sender);
   void __fastcall ActionSelFonteExecute(TObject *Sender);
   void __fastcall ActionSelCorFundoExecute(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall ActionSelCorBordaExecute(TObject *Sender);
   void __fastcall CBoxBordaEspessuraChange(TObject *Sender);
   void __fastcall CBoxBordaEstiloChange(TObject *Sender);

public:  // User declarations
   __fastcall  TFormEdtComentario(TComponent* Owner, VTApl *apl_owner, VTFiguraComentario *comentario);
   __fastcall ~TFormEdtComentario(void);

private: //métodos
   void __fastcall CBoxRedeInicia(VTRede *rede_ref);
   void __fastcall ExibeDados(void);
   bool __fastcall LeDados(void);

private: //objetos externos
   VTApl        *apl;
   VTFiguraComentario *comentario;

};

//---------------------------------------------------------------------------
#endif
//eof

