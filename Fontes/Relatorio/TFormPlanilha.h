//---------------------------------------------------------------------------
#ifndef TFormPlanilhaH
#define TFormPlanilhaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Graphics.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <DB.hpp>
#include <ADODB.hpp>
#include <ExtCtrls.hpp>
#include <ActnList.hpp>
#include <CheckLst.hpp>
#include <ComCtrls.hpp>
#include <DBCtrls.hpp>
#include <DBGrids.hpp>
#include <Grids.hpp>
#include <ToolWin.hpp>
#include <Buttons.hpp>
#include <ImgList.hpp>
#include <Mask.hpp>
#include <Menus.hpp>
#include <Dialogs.hpp>
#include <System.Actions.hpp>
#include <System.ImageList.hpp>

//---------------------------------------------------------------------------
class VTApl;

//---------------------------------------------------------------------------
class TFormPlanilha : public TForm
{
__published:	// IDE-managed Components
   TDataSource *DataSource;
   TToolBar *ToolBar;
   TToolButton *butAtributo;
   TActionList *ActionList1;
   TAction *ActionConfigura;
   TDBNavigator *DBNavigator;
   TADOQuery *query;
   TSplitter *Splitter1;
   TAction *ActionAtributoSelAll;
   TAction *ActionAtributoSelNone;
   TImageList *ImageList1;
   TAction *ActionAtributoMoveDown;
   TAction *ActionAtributoMoveUp;
   TToolButton *ToolButton3;
   TToolButton *ToolButton4;
   TToolButton *butLocaliza;
   TAction *ActionLocaliza;
   TComboBox *cboxField;
   TRichEdit *redLocaliza;
   TToolButton *ToolButton5;
   TToolButton *ButRelTexto;
   TAction *ActionExportaRelatorio;
   TPageControl *PageControl;
   TTabSheet *TabSheetRede;
   TTabSheet *TabSheetAtributo;
   TGroupBox *gboxField;
   TCheckListBox *CheckListBoxField;
   TToolBar *ToolBar2;
   TBitBtn *butSelAll;
   TBitBtn *butSelNone;
   TToolButton *ToolButton1;
   TToolButton *ToolButton2;
   TGroupBox *GroupBox1;
   TCheckListBox *CheckListBoxRede;
   TToolBar *ToolBar1;
   TBitBtn *BitBtn1;
   TBitBtn *BitBtn2;
   TAction *ActionRedeSelAll;
   TAction *ActionRedeSelNone;
	TAction *ActionZoom;
   TSaveDialog *SaveDialog;
	TDBGrid *dbgrid;
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall ActionConfiguraExecute(TObject *Sender);
   void __fastcall CheckListBoxFieldClickCheck(TObject *Sender);
   void __fastcall ActionAtributoSelAllExecute(TObject *Sender);
   void __fastcall ActionAtributoSelNoneExecute(TObject *Sender);
   void __fastcall ActionAtributoMoveDownExecute(TObject *Sender);
   void __fastcall ActionAtributoMoveUpExecute(TObject *Sender);
   void __fastcall ActionLocalizaExecute(TObject *Sender);
   void __fastcall ActionExportaRelatorioExecute(TObject *Sender);
   void __fastcall dbgridCellClick(TColumn *Column);
   void __fastcall dbgridTitleClick(TColumn *Column);
   void __fastcall dbgridDrawColumnCell(TObject *Sender, const TRect &Rect,
          int DataCol, TColumn *Column, TGridDrawState State);
   void __fastcall ActionRedeSelAllExecute(TObject *Sender);
   void __fastcall ActionRedeSelNoneExecute(TObject *Sender);
   void __fastcall CheckListBoxRedeClickCheck(TObject *Sender);
	void __fastcall ActionZoomExecute(TObject *Sender);

public:		// User declarations
        __fastcall  TFormPlanilha(TComponent* Owner, TWinControl *parent, VTApl *apl,
                                  TADOConnection *Connection);
        __fastcall ~TFormPlanilha(void);
   void __fastcall  ExibeTabela(AnsiString tab_name);

private:	//métodos
   void __fastcall IniciaComboBoxField(AnsiString sel_txt="");
   void __fastcall IniciaCheckListBoxField(AnsiString sel_txt="");
   void __fastcall IniciaCheckListBoxRede(void);
   void __fastcall IniciaDBGrid(void);
   int  __fastcall NumeroDeCheckedItems(TCheckListBox *clbox);
   bool __fastcall SelecionaArquivo(void);

private: //objetos externos
   VTApl *apl;

private: //dados
   AnsiString tab_name;
   struct   {
            TColumn *column;
            bool     ascendente;
            }ordenacao;
};

//---------------------------------------------------------------------------
#endif
//eof
