//---------------------------------------------------------------------------
#ifndef TFormLog2H
#define TFormLog2H

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
class TFormLog2 : public TForm
{
__published:   // IDE-managed Components
   TToolBar *ToolBar;
   TToolButton *ButFecha;
   TToolButton *ToolButton4;
   TToolButton *ButExportaExcel;
   TImageList *ImageList;
   TActionList *ActionList;
   TAction *ActionFecha;
   TAction *ActionExportaTxt;
   TAction *ActionExportaExcel;
   TToolButton *ButExportaTxt;
   TListView *LView;
   TToolButton *ButConfigura;
   TAction *ActionConfigura;
   TGroupBox *GBoxConfigura;
   TToolButton *ToolButton1;
   TSaveDialog *SaveDialog;
   void __fastcall ActionFechaExecute(TObject *Sender);
   void __fastcall LViewColumnClick(TObject *Sender, TListColumn *Column);
   void __fastcall LViewCompare(TObject *Sender, TListItem *Item1,
          TListItem *Item2, int Data, int &Compare);
   void __fastcall ActionConfiguraExecute(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall ActionExportaTxtExecute(TObject *Sender);
   void __fastcall ActionExportaExcelExecute(TObject *Sender);

public:      //métodos
        __fastcall  TFormLog2(TComponent* Owner, VTApl *apl);
        __fastcall ~TFormLog2(void);
   void __fastcall  ExibeLog(AnsiString arq_log, TStrings *colunas);
   void __fastcall  ExibeLog(TStrings *Lines, TStrings *colunas);

private:  //métodos
   void __fastcall ExtraiCampos(AnsiString txt, TStrings *campos);
   void __fastcall LViewConfigura(TStrings *colunas);
   void __fastcall LViewInicia(TStrings *lines);

private: //objetos externos
   VTApl *apl;

private:  //dados locais
   TStringList *campos;
   TStringList *lines;
   struct   {
            int    ColumnIndex;
            bool   OrdemCrescente;
            }ListViewSort;
};
//---------------------------------------------------------------------------
#endif
