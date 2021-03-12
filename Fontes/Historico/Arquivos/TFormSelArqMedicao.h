//---------------------------------------------------------------------------
#ifndef TFormSelArqMedicaoH
#define TFormSelArqMedicaoH

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

//---------------------------------------------------------------------------
class VTApl;
class TArquivos;
class TArqMedicao;

//---------------------------------------------------------------------------
class TFormSelArqMedicao : public TForm
{
__published:	// IDE-managed Components
   TToolBar *ToolBar1;
   TToolButton *butConfirma;
   TImageList *ImageList;
   TActionList *ActionList;
   TAction *ActionFecha;
   TAction *ActionRetArquivoSel;
   TToolButton *ButRetiraArquivo;
   TGroupBox *GBoxArqMedicao;
   TToolButton *ButSelArquivo;
   TAction *ActionInsArquivo;
   TOpenDialog *OpenDialog;
   TToolButton *ToolButton1;
   TPopupMenu *PopupMenuArqMedicao;
   TPopupMenu *PopupMenuArquivo;
   TAction *ActionRetArquivoAll;
   TMenuItem *MenuItemInsArquivo;
   TMenuItem *MenuItemRetArqAll;
   TMenuItem *MenuItemRetArqSel;
   TTreeView *TView;
   void __fastcall ActionInsArquivoExecute(TObject *Sender);
   void __fastcall ActionRetArquivoSelExecute(TObject *Sender);
   void __fastcall ActionFechaExecute(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall TViewMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
   void __fastcall ActionRetArquivoAllExecute(TObject *Sender);
   void __fastcall TViewChange(TObject *Sender, TTreeNode *Node);

public:		// User declarations
					 __fastcall  TFormSelArqMedicao(TComponent* Owner, VTApl *apl_owner, TWinControl *parent);
					 __fastcall ~TFormSelArqMedicao(void);
	void 			 __fastcall FormPai(TForm *form);
	void         __fastcall TViewInicia(void);

private:	//métodos
	TArqMedicao* __fastcall ExisteArqMedicaoSelecionado(void);
   TTreeNode*   __fastcall TViewExisteTreeNode(TTreeView *TView, void *data);
   TTreeNode*   __fastcall TViewInsereArqMedicao(TArqMedicao *arq_medicao);

private:	//objetos externos
   VTApl     *apl;
	TArquivos *arquivos;
	TForm		 *form_pai;

private:	//dados locais
   TTreeNode *node_raiz;
};

//---------------------------------------------------------------------------
#endif
//eof

