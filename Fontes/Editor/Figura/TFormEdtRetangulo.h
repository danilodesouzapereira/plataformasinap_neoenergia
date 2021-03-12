//---------------------------------------------------------------------------
#ifndef TFormEdtRetanguloH
#define TFormEdtRetanguloH

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
#include <Fontes\Figura\VTRetangulo.h>

//---------------------------------------------------------------------------
class VTRede;

//---------------------------------------------------------------------------
class TFormEdtRetangulo : public TForm
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
   TAction *ActionSelCorFundo;
   TColorDialog *ColorDialog;
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
   TCheckBox *CheckBoxTransparente;
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall ActionConfirmaExecute(TObject *Sender);
   void __fastcall ActionRetiraExecute(TObject *Sender);
   void __fastcall ActionSelCorFundoExecute(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall ActionSelCorBordaExecute(TObject *Sender);
   void __fastcall CBoxBordaEstiloChange(TObject *Sender);
   void __fastcall CBoxBordaEspessuraChange(TObject *Sender);

public:  // User declarations
   __fastcall  TFormEdtRetangulo(TComponent* Owner, VTApl *apl_owner, VTRetangulo *retangulo);
   __fastcall ~TFormEdtRetangulo(void);

private: //métodos
   void __fastcall ExibeDados(void);
   bool __fastcall LeDados(void);

private: //objetos externos
   VTApl        *apl;
   VTRetangulo *retangulo;

};

//---------------------------------------------------------------------------
#endif
//eof

