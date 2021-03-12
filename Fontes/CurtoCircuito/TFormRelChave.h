//---------------------------------------------------------------------------
#ifndef TFormRelChaveH
#define TFormRelChaveH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ToolWin.hpp>
#include <Vcl.Dialogs.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTLigCC;

//---------------------------------------------------------------------------
class TFormRelChave : public TForm
{
__published:   // IDE-managed Components
   TActionList *ActionList;
   TToolBar *ToolBar1;
   TListView *LView;
   TToolButton *ToolButton1;
   TToolButton *ToolButton2;
   TAction *ActionClose;
   TAction *ActionExporta;
   TToolButton *ToolButton3;
   TSaveDialog *SaveDialog;
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall ActionCloseExecute(TObject *Sender);
   void __fastcall ActionExportaExecute(TObject *Sender);
   void __fastcall LViewColumnClick(TObject *Sender, TListColumn *Column);
   void __fastcall LViewCompare(TObject *Sender, TListItem *Item1, TListItem *Item2,
          int Data, int &Compare);

public:  // User declarations
        __fastcall  TFormRelChave(TComponent* Owner, VTApl *apl_owner);
        __fastcall ~TFormRelChave(void);
   void __fastcall  DefineCaminhoEletrico(TList *lisLIG);

private: //métodos
   int  __fastcall  CompareNum(AnsiString S1, AnsiString S2);
   bool __fastcall  SelecionaArquivo(void);

private: //objetos externos
   VTApl *apl;
   struct   {//estrutura p/ ordenar LView
            int    ColumnIndex;
            bool   OrdemCrescente;
            }ListViewSort;
};

//---------------------------------------------------------------------------
#endif
//eof

