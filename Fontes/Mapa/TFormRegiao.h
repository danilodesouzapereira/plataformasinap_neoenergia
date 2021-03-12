//---------------------------------------------------------------------------
#ifndef TFormRegiaoH
#define TFormRegiaoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ActnList.hpp>
#include <CheckLst.hpp>
#include <ComCtrls.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <Dialogs.hpp>
#include <System.Actions.hpp>
#include <System.ImageList.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTRegiao;
class VTRegioes;

//---------------------------------------------------------------------------
class TFormRegiao : public TForm
{
__published:	// IDE-managed Components
   TGroupBox *GBoxRegiao;
   TActionList *ActionList;
   TToolBar *ToolBar;
   TImageList *ImageList;
   TCheckListBox *CLBoxRegiao;
   TToolButton *ButImportaTxt;
   TAction *ActionImportaTxt;
   TOpenDialog *OpenDialog;
   TToolButton *ToolButton1;
   TToolButton *ButCentroCarga;
   TAction *ActionCentroCarga;
   TAction *ActionShow;
   void __fastcall ActionImportaTxtExecute(TObject *Sender);
   void __fastcall CLBoxRegiaoClick(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall ActionCentroCargaExecute(TObject *Sender);
   void __fastcall ActionShowExecute(TObject *Sender);

public:		// User declarations
        __fastcall  TFormRegiao(TComponent *Owner, VTApl *apl_owner, TWinControl *parent);
        __fastcall ~TFormRegiao(void);

private:	//métodos
   void __fastcall AtualizaGrafico(void);
   void __fastcall CLBoxRegiaoInicia(void);
   bool __fastcall LeArquivoLatLon(AnsiString arq_txt);

private:	//objetos externos
    VTApl *apl;

private: //dados locais
   VTRegioes *regioes;
};

//---------------------------------------------------------------------------
#endif
//eof
