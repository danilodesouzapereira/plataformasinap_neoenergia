//---------------------------------------------------------------------------
#ifndef TFormEqvWiseH
#define TFormEqvWiseH

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

//---------------------------------------------------------------------------
class VTApl;

//---------------------------------------------------------------------------
class TFormEqvWise : public TForm
{
__published:   // IDE-managed Components
   TActionList *ActionList;
   TImageList *ImageList;
   TToolBar *ToolBar1;
   TAction *ActionEqv;
   TAction *ActionClose;
   TToolButton *ButClose;
   TToolButton *ButEqv;
   void __fastcall ActionCloseExecute(TObject *Sender);
   void __fastcall ActionEqvExecute(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);

public:      // User declarations
   __fastcall  TFormEqvWise(TComponent* Owner, VTApl *apl_owner, TWinControl *parent);
   __fastcall ~TFormEqvWise(void);

private:   //métodos
   bool __fastcall ExisteMedicao(void);

private:   //objetos externos
  VTApl *apl;

private:   //dados locais
   TList *lisEQP;
};

//---------------------------------------------------------------------------
#endif
//eof

