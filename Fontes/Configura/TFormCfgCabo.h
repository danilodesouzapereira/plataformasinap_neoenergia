//---------------------------------------------------------------------------

#ifndef TFormCfgCaboH
#define TFormCfgCaboH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <DBGrids.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <Db.hpp>
#include <ActnList.hpp>
#include <DBTables.hpp>
#include <DBCtrls.hpp>
#include <Dialogs.hpp>
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
#include <DB.hpp>
#include <ImgList.hpp>

//---------------------------------------------------------------------------
class VTBDado;
class TCabo;
class TFormParCabo;

//---------------------------------------------------------------------------
class TformCfgCabo : public TForm
{
__published:	// IDE-managed Components
   TPanel *panelFundo;
   TListView *lvwCabos;
        TToolBar *ToolBar1;
        TToolButton *butDefault;
        TToolButton *butInclui;
        TToolButton *butSair;
        TImageList *ImageList;
        TActionList *ActionList1;
        TAction *ActionCancela;
        TAction *ActionConfirma;
   TAction *ActionDesfaz;
   TToolButton *ToolButton1;
   TAction *ActionEdita;
   TToolButton *ToolButton2;
   TToolButton *ToolButton3;
   TToolButton *ToolButton4;
   TAction *ActionAdiciona;
   TAction *ActionRemove;
   void __fastcall ActionAdicionaExecute(TObject *Sender);
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall ActionConfirmaExecute(TObject *Sender);
   void __fastcall ActionDesfazExecute(TObject *Sender);
   void __fastcall ActionEditaExecute(TObject *Sender);
   void __fastcall ActionRemoveExecute(TObject *Sender);
   void __fastcall FormActivate(TObject *Sender);
   void __fastcall lvwCabosAdvancedCustomDrawSubItem(
          TCustomListView *Sender, TListItem *Item, int SubItem,
          TCustomDrawState State, TCustomDrawStage Stage,
          bool &DefaultDraw);

public:		// User declarations
   __fastcall TformCfgCabo(TComponent* Owner, AnsiString dirSISPLAN, AnsiString dirREDE);
   __fastcall ~TformCfgCabo(void);

private:	// User declarations
   VTBDado *bdado;
   TList   *lisCabos;

private:	// User declarations
   void __fastcall loadCabos(void);

};

#endif
//---------------------------------------------------------------------------
//eof

