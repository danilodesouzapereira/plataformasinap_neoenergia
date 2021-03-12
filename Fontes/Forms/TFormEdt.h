//---------------------------------------------------------------------------
#ifndef TFormEdtH
#define TFormEdtH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <ImgList.hpp>
#include <Menus.hpp>
#include <ToolWin.hpp>
#include <System.Actions.hpp>
#include <System.ImageList.hpp>

//---------------------------------------------------------------------------
class TFormEdt : public TForm
{
__published:   // IDE-managed Components
   TToolBar *ToolBarLeft;
   TPanel *panelMouse;
   TImage *ImageMouseOff;
   TImage *ImageMouseOn;
   TToolButton *butAlign;
   TToolButton *butEdt;
   TActionList *ActionList;
   TAction *ActionAlignDown;
   TAction *ActionAlignLeft;
   TAction *ActionAlignNone;
   TAction *ActionAlignRight;
   TAction *ActionAlignUp;
   TAction *ActionEdt;
   TAction *ActionMouseOff;
   TAction *ActionMouseOn;
   TImageList *ImageList;
   TPopupMenu *PopupMenu;
   TMenuItem *mnuAlignLeft;
   TMenuItem *mnuAlignRight;
   TMenuItem *mnuAlignDown;
   TMenuItem *mnuAlignUp;
   TMenuItem *mnuAlignNode;
   TPanel *PanelToolBar;
   TPanel *PanelLeft;
   TPanel *PanelRight;
   void __fastcall ActionAlignDownExecute(TObject *Sender);
   void __fastcall ActionAlignLeftExecute(TObject *Sender);
   void __fastcall ActionAlignNoneExecute(TObject *Sender);
   void __fastcall ActionAlignRightExecute(TObject *Sender);
   void __fastcall ActionAlignUpExecute(TObject *Sender);
   void __fastcall ActionEdtExecute(TObject *Sender);
   void __fastcall ActionMouseOffExecute(TObject *Sender);
   void __fastcall ActionMouseOnExecute(TObject *Sender);

public:      // User declarations
                __fastcall TFormEdt(TComponent* Owner, TWinControl *parent);
                __fastcall ~TFormEdt(void);
   virtual void __fastcall Ativo(void);
   virtual void __fastcall Atualiza(void);
           void __fastcall DefParent(TWinControl *parent);

protected: //métodos
   void __fastcall AtualizaIndicacaoEditorOff(void);
   void __fastcall AtualizaIndicacaoEditorOn(TToolButton *button);
   void __fastcall Centraliza(void);

protected: //dados locais
   TToolButton *button_down;
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

