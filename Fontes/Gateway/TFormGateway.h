//---------------------------------------------------------------------------
#ifndef TFormGatewayH
#define TFormGatewayH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Data.DB.hpp>
#include <Data.Win.ADODB.hpp>
#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.ToolWin.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTArvore;
class VTNode;

//---------------------------------------------------------------------------
class TFormGateway : public TForm
{
__published:	// IDE-managed Components
   TToolBar *ToolBar1;
   TActionList *ActionList;
   TImageList *ImageList;
   TToolButton *ButRead;
   TToolButton *ButWrite;
   TAction *ActionReadAll;
   TAction *ActionWriteAll;
   TToolButton *ToolButton1;
   TAction *ActionClose;
   TGroupBox *GBoxRede;
   TTreeView *TViewRede;
   TToolButton *ToolButton2;
   TAction *ActionReadSel;
   TToolButton *ToolButton3;
   TAction *ActionClear;
   void __fastcall ActionReadAllExecute(TObject *Sender);
   void __fastcall ActionWriteAllExecute(TObject *Sender);
   void __fastcall ActionCloseExecute(TObject *Sender);
   void __fastcall ActionReadSelExecute(TObject *Sender);
   void __fastcall TViewRedeDblClick(TObject *Sender);
   void __fastcall ActionClearExecute(TObject *Sender);

public:		// User declarations
   __fastcall  TFormGateway(TComponent* Owner, VTApl *apl_owner);
   __fastcall ~TFormGateway(void);

private:	//métodos
   void       __fastcall ArvoreInicia(void);
   void       __fastcall LeNodesFilho(int node_pai_id);
   void       __fastcall LeRedesSelecionadas(void);
   void       __fastcall LeRedesSelecionadas(DynamicArray<int> &vet_rede_id);
   void       __fastcall LeTodasRedesDoGateway(void);
   void       __fastcall ReiniciaGateway(void);
   void       __fastcall SalvaRedesNoGateway(void);
   void       __fastcall TViewRedeInicia(void);
   TTreeNode* __fastcall TViewRedeInsereNode(VTNode *node, TTreeNode *treenode_pai=NULL);

private:	//objetos externos
    VTApl *apl_owner;

private:	//dados locais
   VTArvore *arvore;
   TList    *lisOBJ;
};

//---------------------------------------------------------------------------
#endif
//eof
