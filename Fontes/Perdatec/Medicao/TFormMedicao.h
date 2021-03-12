//---------------------------------------------------------------------------
#ifndef TFormMedicaoH
#define TFormMedicaoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <ActnList.hpp>
#include <Grids.hpp>
#include <ToolWin.hpp>
#include <Menus.hpp>
#include <Dialogs.hpp>
#include <ImgList.hpp>
#include <Buttons.hpp>
#include <System.Actions.hpp>
#include <Vcl.ExtDlgs.hpp>
#include <System.ImageList.hpp>

//---------------------------------------------------------------------------
class TFormPerda;
class VTApl;

//---------------------------------------------------------------------------
class TFormMedicao : public TForm
{
__published:   // IDE-managed Components
    TActionList *ActionList;
    TToolBar *ToolBar;
    TToolButton *ToolButton1;
    TAction *ActionFechar;
    TToolButton *ToolButton3;
    TAction *ActionCriaMedicaoTeste;
    TImageList *ImageList;
    TToolButton *ButMedicao;
    TAction *ActionMedicaoImporta;
    TGroupBox *GroupBox1;
    TListView *LViewMedicao;
    TToolButton *ButEditaMedicao;
    TAction *ActionMedicaoEdita;
    TAction *ActionMedicaoInsere;
    TAction *ActionMedicaoRetira;
    TToolButton *ButMedicaoInsere;
    TToolButton *ButMedicaoRetira;
    TOpenTextFileDialog *OpenTextFileDialog;
    TToolButton *ToolButton2;
    TAction *ActionPropCarga;
    TToolButton *ToolButton4;
    TAction *ActionDetalhe;
    TSaveDialog *SaveDialog;
    TToolButton *ButMedicaoExporta;
    TAction *ActionMedicaoExporta;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall ActionFecharExecute(TObject *Sender);
    void __fastcall ActionCriaMedicaoTesteExecute(TObject *Sender);
    void __fastcall ActionMedicaoImportaExecute(TObject *Sender);
    void __fastcall ActionMedicaoEditaExecute(TObject *Sender);
    void __fastcall ActionMedicaoExportaExecute(TObject *Sender);
    void __fastcall ActionMedicaoInsereExecute(TObject *Sender);
    void __fastcall ActionMedicaoRetiraExecute(TObject *Sender);
    void __fastcall LViewMedicaoColumnClick(TObject *Sender, TListColumn *Column);
    void __fastcall LViewMedicaoCompare(TObject *Sender, TListItem *Item1, TListItem *Item2,
          int Data, int &Compare);
    void __fastcall ActionPropCargaExecute(TObject *Sender);
    void __fastcall LViewMedicaoCustomDrawItem(TCustomListView *Sender, TListItem *Item,
          TCustomDrawState State, bool &DefaultDraw);
    void __fastcall ActionDetalheExecute(TObject *Sender);

public:  //property
    __property TDateTime Date = {read=PD.date, write=PD.date};

public:      // User declarations
    __fastcall  TFormMedicao(TComponent* Owner, VTApl *apl_owner);
    __fastcall ~TFormMedicao(void);

private:      //métodos
    void __fastcall FormIniciaPosicao(void);
    bool __fastcall LeArquivoMedicao(void);
    void __fastcall LViewMedicaoInicia(void);
    bool __fastcall ValidaMedicao(void);

private:   //objetos externos
    VTApl       *apl;

private:   //dados acessados via proeprty
    struct
    {
        TDateTime date;
    } PD;

private:   //dados locais
    struct
    {   //controle de ordenação de ListView
        TListColumn *column;
        bool         ascendente;
    } LViewSort;
};

//---------------------------------------------------------------------------
#endif
//eof
