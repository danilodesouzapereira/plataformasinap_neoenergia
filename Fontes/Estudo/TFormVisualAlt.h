//---------------------------------------------------------------------------

#ifndef TFormVisualAltH
#define TFormVisualAltH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ActnList.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <ExtCtrls.hpp>
#include <System.Actions.hpp>
#include <System.ImageList.hpp>
//---------------------------------------------------------------------------
class VTApl;
class VTAlternativa;
class VTEstudo;
//---------------------------------------------------------------------------
class TFormVisualAlt : public TForm
{
__published:	// IDE-managed Components
   TGroupBox *GroupBox1;
   TGroupBox *GroupBox2;
   TComboBox *CBoxAno;
   TTreeView *TreeView;
   TToolBar *ToolBar1;
   TToolButton *ToolButton1;
   TActionList *ActionList1;
   TAction *ActionFechar;
   TImageList *ImageList1;
   TPanel *PanelAviso;
   TLabel *LabelAviso;
   void __fastcall TreeViewChange(TObject *Sender, TTreeNode *Node);
   void __fastcall CBoxAnoChange(TObject *Sender);
   void __fastcall ActionFecharExecute(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormShow(TObject *Sender);

private:	// métodos
   void           __fastcall FormIniciaPosicao(void);
   void           __fastcall Inicia(void);
   void           __fastcall Redraw(void);
   void           __fastcall TreeViewInicia(void);
   VTAlternativa* __fastcall TreeViewAlternativaSelecionada(void);
   TTreeNode*     __fastcall TreeViewExisteTreeNode(void *data);

public:      // User declarations
                  __fastcall  TFormVisualAlt(TForm* Owner, VTApl *apl, VTEstudo *estudo);
                  __fastcall ~TFormVisualAlt(void);

private:   //objetos externos
   VTApl          *apl;
   VTEstudo       *estudo;
   TForm          *Owner;
};

//---------------------------------------------------------------------------
#endif
