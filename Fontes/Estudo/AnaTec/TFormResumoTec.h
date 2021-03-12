//---------------------------------------------------------------------------
#ifndef TFormResumoTecH
#define TFormResumoTecH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <System.Actions.hpp>
#include <System.ImageList.hpp>

//---------------------------------------------------------------------------
class VTEstudo;

//---------------------------------------------------------------------------
class TFormResumoTec : public TForm
{
__published:   // IDE-managed Components
   TToolBar *ToolBar;
   TToolButton *ButImpressao;
   TToolButton *BuFecha;
   TActionList *ActionList;
   TAction *ActionFecha;
   TAction *ActionImprime;
   TImageList *ImageList;
   TToolButton *ToolButton1;
   TListView *LViewAnaTec;
   void __fastcall ActionImprimeExecute(TObject *Sender);
   void __fastcall ActionFechaExecute(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);

public:      // User declarations
   __fastcall  TFormResumoTec(TComponent* Owner, VTEstudo *estudo);
   __fastcall ~TFormResumoTec(void);

private: //métodos
   void __fastcall LViewInicia(VTEstudo *estudo);

private: //objetos externos

private: //dados locais

};

//---------------------------------------------------------------------------
#endif
//eof

