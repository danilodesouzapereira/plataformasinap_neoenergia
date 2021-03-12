//---------------------------------------------------------------------------
#ifndef TFormSelPadraoH
#define TFormSelPadraoH

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

//---------------------------------------------------------------------------
class VTApl;

//---------------------------------------------------------------------------
class TFormSelPadrao : public TForm
{
__published:   // IDE-managed Components
   TToolBar *ToolBar1;
   TToolButton *butConfirma;
   TToolButton *butCancela;
   TImageList *ImageList;
   TActionList *ActionList;
   TAction *ActionCancela;
   TAction *ActionConfirma;
   TListView *LViewPadrao;
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall ActionConfirmaExecute(TObject *Sender);

public:      // User declarations
               __fastcall  TFormSelPadrao(TComponent* Owner, VTApl *apl);
               __fastcall ~TFormSelPadrao(void);
   void        __fastcall  Inicia(int tipo_eqpto);
   bool        __fastcall  PadraoSelecionado(int &id, AnsiString &codigo);

private:   //métods
   void __fastcall ExtraiCampos(AnsiString txt, TStringList *campos);
   void __fastcall FormIniciaPosicao(void);
   void __fastcall LePadrao(int tipo_eqpto);
   void __fastcall LViewPadraoInicia(void);

private:   //objetos externos
   VTApl *apl;

private:   //dados locais
   TStringList *StrList;
   TStringList *campos;
};

//---------------------------------------------------------------------------
#endif
//eof

