//---------------------------------------------------------------------------
#ifndef TFormSelRedeH
#define TFormSelRedeH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <CheckLst.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTBDrede;
class VTPath;
class VTRedes;
class VTSelRede;

//---------------------------------------------------------------------------
class TFormSelRede : public TForm
{
__published:	// IDE-managed Components
   TToolBar *ToolBar1;
   TToolButton *butInclui;
   TToolButton *butSair;
   TActionList *ActionList;
   TAction *ActionConfirma;
   TAction *ActionCancela;
   TImageList *ImageList;
   TGroupBox *GroupBox1;
   TTreeView *tviewRede;
   TToolBar *ToolBar2;
   TSplitter *Splitter1;
   TSplitter *Splitter2;
   TGroupBox *GroupBox2;
   TTreeView *tviewSel;
   TToolBar *ToolBar3;
   TPanel *Panel1;
   TCheckListBox *clboxRedeViz;
   TCheckListBox *clboxRede;
   TSplitter *Splitter4;
   TAction *ActionInsereRede;
   TPanel *Panel2;
   TBitBtn *butRetRede;
   TBitBtn *butInsRede;
   TAction *ActionRetiraRede;
   TToolButton *butReiniciaSel;
   TAction *ActionReiniciaSel;
   TToolButton *butSelAll;
   TAction *ActionSelAll;
   TPanel *Panel3;
   TLabel *Label1;
   TComboBox *cboxDependencia;
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall ActionConfirmaExecute(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall tviewRedeChange(TObject *Sender, TTreeNode *Node);
   void __fastcall clboxRedeClick(TObject *Sender);
   void __fastcall clboxRedeVizClick(TObject *Sender);
   void __fastcall ActionInsereRedeExecute(TObject *Sender);
   void __fastcall ActionRetiraRedeExecute(TObject *Sender);
   void __fastcall ActionReiniciaSelExecute(TObject *Sender);
   void __fastcall clboxRedeDblClick(TObject *Sender);
   void __fastcall ActionSelAllExecute(TObject *Sender);

public:		// User declarations
   __fastcall TFormSelRede(TComponent* Owner, VTApl *apl);
   __fastcall ~TFormSelRede(void);

private:	//métodos
   int  __fastcall AtualizaListaRedeSelecionada(void);
   void __fastcall IniciaCheckListBoxRede(void);
   void __fastcall IniciaCheckListBoxRedeViz(void);
   void __fastcall IniciaCheckListBoxSel(void);
   void __fastcall IniciaTViewRede(void);
   void __fastcall IniciaTViewSel(void);

private:	//dados externos
   VTBDrede *bdRede;
   VTPath   *path;
   VTRedes  *redes;

private:	//dados locais
   VTSelRede *selRede;
   TList     *lisSEL;
   TList     *lisEQP;
};

//---------------------------------------------------------------------------
#endif
//eof
