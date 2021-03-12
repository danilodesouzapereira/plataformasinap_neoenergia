//---------------------------------------------------------------------------
#ifndef TFormStkH
#define TFormStkH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
#include <ActnList.hpp>
#include <ImgList.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <Menus.hpp>
#include <Buttons.hpp>
#include <System.Actions.hpp>
#include <System.ImageList.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTElpa;
class VTElpaGD1;
class VTElpaGD3;
class VTElpaSUB1;


//---------------------------------------------------------------------------
class TFormStk : public TForm
{
__published:   // IDE-managed Components
   TTreeView *TreeView;
   TActionList *ActionListAlign;
   TAction *ActionAlignNone;
   TAction *ActionAlignLeft;
   TAction *ActionAlignRight;
   TImageList *ImageListStk;
   TImage *Image;
   void __fastcall ActionAlignNoneExecute(TObject *Sender);
   void __fastcall ActionAlignLeftExecute(TObject *Sender);
   void __fastcall ActionAlignRightExecute(TObject *Sender);
   void __fastcall TreeViewAdvancedCustomDrawItem(TCustomTreeView *Sender,
          TTreeNode *Node, TCustomDrawState State, TCustomDrawStage Stage,
          bool &PaintImages, bool &DefaultDraw);
   void __fastcall TreeViewClick(TObject *Sender);
   void __fastcall FormResize(TObject *Sender);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);

public:      // User declarations
        __fastcall  TFormStk(TComponent *Owner, VTApl *apl, TWinControl *parent);
        __fastcall ~TFormStk(void);
   void __fastcall TreeViewAdd(AnsiString caption, TActionList* ActionList, bool expand=false);
   void __fastcall TreeViewAdd(AnsiString caption, TList *lisAL, bool expand=false);
   void __fastcall TreeViewInicia(void);
   void __fastcall TreeViewRemove(TActionList* ActionList);

private:   //métodos
   void       __fastcall FormIniciaPosicao(void);
   void       __fastcall FormSalvaPosicao(void);
   void       __fastcall LimpaImage(void);
   void       __fastcall TreeViewAdd(TTreeNode *Node, TActionList* ActionList);
   TTreeNode* __fastcall TreeViewExisteNode(void *data);

private:   //objetos externos
   VTApl *apl;

private:   //dados locais
   int        plus_index;
   int        minus_index;
   struct   {//coordenadas p/ janela flutuante
            int top;
            int left;
            int width;
            int height;
            } janela;
};

//---------------------------------------------------------------------------
#endif
//eof
