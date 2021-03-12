//---------------------------------------------------------------------------
#ifndef TFormPerdatecH
#define TFormPerdatecH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ToolWin.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.ImgList.hpp>
#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ExtDlgs.hpp>
#include <Vcl.Menus.hpp>
#include <System.ImageList.hpp>

//---------------------------------------------------------------------------
class TFormPerda;
class VTApl;
class VTRedes;

//---------------------------------------------------------------------------
class TFormPerdatec : public TForm
{
__published:	// IDE-managed Components
   TToolBar *ToolBar;
   TPanel *Panel;
   TPanel *PanelTitulo;
   TPanel *PanelToolBar;
   TLabel *LabelTitulo;
   TPanel *PanelData;
   TLabel *Label1;
   TDateTimePicker *DateTimePicker;
   TImageList *ImageList;
   TToolButton *ToolButton1;
   TToolButton *ToolButton3;
   TToolButton *ToolButton4;
   TToolButton *ButRelatorio;
   TToolButton *ToolButton7;
   TPanel *PanelRight;
   TBitBtn *ButFecha;
   TToolButton *ToolButton5;
   TActionList *ActionList;
   TAction *ActionCfgParametro;
   TAction *ActionFechar;
   TAction *ActionCalculaPerdas;
   TAction *ActionHelp;
   TOpenTextFileDialog *OpenTextFileDialog;
   TPopupMenu *PopupMenuRelatorio;
   TMenuItem *MenuItemRelPerdaTecnica;
   TAction *ActionRelPerdaTecnica;
   TAction *ActionSalvaBase;
   TToolButton *ButSalvaBase;
   TToolButton *ToolButton2;
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall ActionCfgParametroExecute(TObject *Sender);
   void __fastcall ActionFecharExecute(TObject *Sender);
   void __fastcall ActionCalculaPerdasExecute(TObject *Sender);
   void __fastcall ActionHelpExecute(TObject *Sender);
   void __fastcall DateTimePickerDropDown(TObject *Sender);
   void __fastcall DateTimePickerCloseUp(TObject *Sender);
   void __fastcall ActionRelPerdaTecnicaExecute(TObject *Sender);
   void __fastcall ActionSalvaBaseExecute(TObject *Sender);
   void __fastcall ToolButton2Click(TObject *Sender);

public:		// User declarations
   __fastcall  TFormPerdatec(TComponent* Owner, VTApl *apl_owner, TWinControl *parent);
   __fastcall ~TFormPerdatec(void);

private:      //métodos
   void __fastcall CalculaPerdasTecnicas(void);
   bool __fastcall ConfiguraParametros(void);
   bool __fastcall CopiaRedes1ParaRedes2(VTRedes *redes1, VTRedes *redes2);
   bool __fastcall DataSelecionada(void);
   void __fastcall DateTimePickerInicia(void);

private:   //objetos externos

private:   //dados locais
   VTApl      *apl;
   VTApl      *mApl_owner;
   TFormPerda *FormPerda;
   VTRedes    *redes_local;
   //controle de alteração de data no TDateTimePicker
   TDateTime date, last_date;
//   bool editor_enabled;
};

//---------------------------------------------------------------------------
#endif
//eof

