//---------------------------------------------------------------------------
#ifndef TFormSelArqRedeH
#define TFormSelArqRedeH

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
#include <Vcl.Dialogs.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Menus.hpp>
#include <System.ImageList.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTImportaRedeGIS;
class TArquivos;
class TArqRede;

//---------------------------------------------------------------------------
class TFormSelArqRede : public TForm
{
__published:	// IDE-managed Components
   TToolBar *ToolBar1;
   TImageList *ImageList;
   TActionList *ActionList;
   TAction *ActionFecha;
   TAction *ActionRetArquivoSel;
   TToolButton *ButRetiraArquivo;
   TGroupBox *GBoxArqRede;
   TTreeView *TView;
   TToolButton *ButSelArquivo;
   TAction *ActionInsArquivo;
   TOpenDialog *OpenDialog;
   TRadioGroup *RadioGroupTipoArqRede;
   TToolButton *ToolButton1;
   TPopupMenu *PopupMenuArqRede;
   TPopupMenu *PopupMenuArquivo;
   TAction *ActionRetArquivoAll;
   TMenuItem *MenuItemInsArquivo;
   TMenuItem *MenuItemRetArqAll;
   TMenuItem *MenuItemRetArqSel;
	TToolButton *butConfirma;
   TSplitter *Splitter;
	TToolButton *butRede;
	TToolButton *ToolButton3;
	TAction *ActionSelRede;
   void __fastcall ActionInsArquivoExecute(TObject *Sender);
   void __fastcall ActionRetArquivoSelExecute(TObject *Sender);
   void __fastcall RadioGroupTipoArqRedeClick(TObject *Sender);
   void __fastcall ActionFechaExecute(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall TViewMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
   void __fastcall ActionRetArquivoAllExecute(TObject *Sender);
   void __fastcall TViewChange(TObject *Sender, TTreeNode *Node);
   void __fastcall ActionSelRedeExecute(TObject *Sender);

public:     // User declarations
		 __fastcall  TFormSelArqRede(TComponent* Owner, VTApl *apl_owner, TWinControl *parent);
         __fastcall ~TFormSelArqRede(void);
   void  __fastcall FormPai(TForm *form);
   void  __fastcall TViewInicia(void);

private: //métodos
   bool       __fastcall ArquivosValidos(TStrings *files);
   TArqRede*  __fastcall ExisteArqRedeSelecionado(void);
   void       __fastcall RadioGroupTipoArqRedeInicia(void);
   TTreeNode* __fastcall TViewExisteTreeNode(TTreeView *TView, void *data);
   TTreeNode* __fastcall TViewInsereArqRede(TArqRede *arq_rede);

private: //objetos externos
   VTApl     *apl;
   TArquivos *arquivos;
   TForm     *form_pai;

private: //dados locais
   TTreeNode *node_raiz;
   VTImportaRedeGIS *importa;

};

//---------------------------------------------------------------------------
#endif
//eof

