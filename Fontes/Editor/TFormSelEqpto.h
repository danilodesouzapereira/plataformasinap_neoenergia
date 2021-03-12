//---------------------------------------------------------------------------
#ifndef TFormSelEqptoH
#define TFormSelEqptoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <Buttons.hpp>
#include <CheckLst.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include <System.Actions.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTEdt;
class VTEqpto;

//---------------------------------------------------------------------------
class TFormSelEqpto : public TForm
{
__published:	// IDE-managed Components
	TActionList *ActionList;
	TAction *ActionCancela;
	TAction *ActionConfirma;
	TImageList *ImageList;
	TToolBar *ToolBar;
	TToolButton *ButConfirma;
	TToolButton *ButCancela;
   TAction *ActionEdtBusca;
   TAction *ActionSelEqptoGrafico;
   TToolButton *ToolButton1;
   TToolButton *ToolButton2;
   TAction *ActionSelEqptoRede;
   TGroupBox *GBoxSelecao;
   TToolBar *ToolBar1;
   TCheckBox *CheckBoxPorRede;
   TAction *ActionCancelaSelecao;
   TToolButton *ToolButton3;
   TTreeView *TView;
   TPopupMenu *PopupMenu;
   TAction *ActionPopupEditaEqptos;
   TAction *ActionPopupExibeRelatorio;
   TAction *ActionPopupRetiraEqptos;
   TMenuItem *Editarequipamentos1;
   TMenuItem *Excluirequipamentodaseleo1;
   TMenuItem *Exibirrelatrio1;
   TPanel *PanelRetiraEqpto;
   TCheckBox *CheckBoxRetiraEqpto;
	void __fastcall ActionCancelaExecute(TObject *Sender);
	void __fastcall ActionConfirmaExecute(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall ActionEdtBuscaExecute(TObject *Sender);
   void __fastcall ActionSelEqptoGraficoExecute(TObject *Sender);
   void __fastcall ActionSelEqptoRedeExecute(TObject *Sender);
   void __fastcall ActionCancelaSelecaoExecute(TObject *Sender);
   void __fastcall TViewChange(TObject *Sender, TTreeNode *Node);
   void __fastcall TViewMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
   void __fastcall ActionPopupEditaEqptosExecute(TObject *Sender);
   void __fastcall ActionPopupExibeRelatorioExecute(TObject *Sender);
   void __fastcall ActionPopupRetiraEqptosExecute(TObject *Sender);
   void __fastcall CheckBoxRetiraEqptoClick(TObject *Sender);

public:		// User declarations
			 __fastcall  TFormSelEqpto(TComponent* Owner, VTApl *apl, TAction *ActionOnDone, TWinControl *parent);
			 __fastcall ~TFormSelEqpto(void);
	TList* __fastcall  LisEqpto(void);

private:	//métodos
   void       __fastcall EditaLisEqpto(TList *lisEDITA);
   void       __fastcall FormIniciaPosicao(void);
   void       __fastcall Moldura(TList *lisEQP_SEL=NULL);
   void       __fastcall TViewAtualizaContadorEqpto(void);
   void       __fastcall TViewAtualizaMoldura(void);
   TTreeNode* __fastcall TViewExisteTreeNodeTipoEqpto(VTEqpto *eqpto);
   void       __fastcall TViewInicia(void);
   void       __fastcall TViewInsereEqpto(VTEqpto *eqpto);
   int        __fastcall TViewLisEqptoNode(TTreeNode *Node, TList *lisEQP_NODE);
   void       __fastcall TViewRetiraEqpto(VTEqpto *eqpto);
   void       __fastcall TrataEqptosSelecionados(TList *lisEXT, bool inserir);

private:	//objetos externos
   TAction *ActionOnDone;
	VTApl   *apl;

private:	//dados locais
   VTEdt *edt;
	TList *lisEQP;
	TList *lisREDE;
   TList *lisTMP;
   struct{
         TTreeNode *arranjo, *barra,     *cabo,      *capacitor, *capserie,
                   *carga,   *chave,     *gerador,   *filtro,    *mutua,
                   *reator,  *rede,      *regulador, *suporte,   *suprimento,
                   *trafo,   *trafo_mono,*trafo3E,   *trafoZZ,   *trecho, //*yref,   
                   *cargaNL;
         }tree_node;
};

//---------------------------------------------------------------------------
#endif

