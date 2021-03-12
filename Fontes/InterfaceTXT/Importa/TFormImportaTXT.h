//---------------------------------------------------------------------------
#ifndef TFormImportaTXTH
#define TFormImportaTXTH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <ImgList.hpp>
#include <Menus.hpp>
#include <ToolWin.hpp>
#include <ExtCtrls.hpp>
#include <FileCtrl.hpp>
#include <Dialogs.hpp>
#include <ExtDlgs.hpp>
#include <System.Actions.hpp>

//---------------------------------------------------------------------------
class VTApl;

//---------------------------------------------------------------------------
class TFormImportaTXT : public TForm
{
__published:   // IDE-managed Components
   TToolBar *ToolBar;
   TImageList *ImageList;
   TActionList *ActionList;
   TAction *ActionSelArquivoTxt;
   TOpenTextFileDialog *OpenTextFileDialog;
   TToolButton *ToolButton1;
   TAction *ActionImporta;
   TToolButton *ButImporta;
   TAction *ActionCancelaSelecao;
   TAction *ActionFecha;
   TToolButton *ButFecha;
   TToolButton *ToolButton4;
   TToolButton *ToolButton5;
   TGroupBox *GroupBoxSelTxt;
   TRadioGroup *RadioGroupRedeSec;
   TToolButton *ToolButton6;
   TAction *ActionReiniciarRede;
   TListBox *LBoxArquivo;
   TToolButton *ButCancelaSelecao;
   TAction *ActionRetArquivo;
   TGroupBox *GroupBox2;
   TCheckBox *CheckBoxValidaFase;
   TCheckBox *CheckBoxCorrigeFase;
   TRadioGroup *RadioGroupTipoArquivo;
   TGroupBox *GroupBox1;
   TCheckBox *CheckBoxEtd;
   TCheckBox *CheckBoxCorrigePerdaFerro;
   TGroupBox *GBoxLog;
   TCheckBox *CheckBoxLog;
   TGroupBox *GBoxArqTXT;
   TCheckBox *CheckBoxArqTXT;
   TRadioGroup *RadioGroupProgresso;
   void __fastcall ActionSelArquivoTxtExecute(TObject *Sender);
   void __fastcall ActionImportaExecute(TObject *Sender);
   void __fastcall ActionFechaExecute(TObject *Sender);
   void __fastcall ActionReiniciarRedeExecute(TObject *Sender);
   void __fastcall LBoxArquivoKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
   void __fastcall ActionRetArquivoExecute(TObject *Sender);
   void __fastcall ActionCancelaSelecaoExecute(TObject *Sender);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);

public:      // User declarations
   __fastcall  TFormImportaTXT(TComponent* Owner, VTApl *apl);
   __fastcall ~TFormImportaTXT(void);

private:   // User declarations
   void       __fastcall FormIniciaPosicao(void);
   bool       __fastcall ImportaETD(void);
   bool       __fastcall ImportaRede(void);
   bool       __fastcall IntegraETDcompleta(void);
   void       __fastcall LBoxArquivoInicia(int num_colunas=2);
   //void       __fastcall LBoxArquivoInsere(AnsiString filename);
   int        __fastcall LeArquivos(void);
   bool       __fastcall ReiniciaRede(bool aviso = false);
   //void       __fastcall SelecionaArquivos(void);
   void       __fastcall SelecionaArqTxt(TStrings *files);
   void       __fastcall SelecionaLisArqTxt(TStrings *files);

private:   //objetos externos
   VTApl   *apl;

private:   //dados locais
   //bool        exportar_xml;
   TStringList *StrListAllFiles;
};

//---------------------------------------------------------------------------
#endif
//eof

