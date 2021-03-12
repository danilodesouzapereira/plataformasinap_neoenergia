//---------------------------------------------------------------------------
#ifndef TFormCurtoArvoreH
#define TFormCurtoArvoreH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTBarra;
class VTChave;
class VTEqpto;
class VTRedeCC;
class VTTrafo;
class VTTrafo3E;
class VTTrecho;
class VTResCC;
class VTGrupo;

//---------------------------------------------------------------------------
class TFormCurtoArvore : public TForm
{
__published:	// IDE-managed Components
	TTreeView *TView;
	THeaderControl *TViewHeader;
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall TViewAdvancedCustomDrawItem(TCustomTreeView *Sender,
          TTreeNode *Node, TCustomDrawState State, TCustomDrawStage Stage,
          bool &PaintImages, bool &DefaultDraw);
	void __fastcall TViewChange(TObject *Sender, TTreeNode *Node);
	void __fastcall TViewHeaderSectionResize(THeaderControl *HeaderControl,
          THeaderSection *Section);
	void __fastcall TViewDblClick(TObject *Sender);

public:		// User declarations
		  __fastcall  TFormCurtoArvore(TComponent* Owner, TWinControl *parent, VTApl *apl);
		  __fastcall ~TFormCurtoArvore(void);
	void __fastcall  EqptoDefeito(VTEqpto *eqpto);
	void __fastcall  EqptoSelecionado(VTEqpto *eqpto);
	int  __fastcall  WidthMinimo(void);

private:	//métodos
   VTEqpto*   __fastcall TViewEqptoSelecionado(void);
	TTreeNode* __fastcall TViewExisteNode(AnsiString text);
   TTreeNode* __fastcall TViewExisteNode(VTEqpto *eqpto);
   TTreeNode* __fastcall TViewExisteNode(TTreeNode *node_pai, VTEqpto *eqpto);
   void       __fastcall TViewInicia(VTEqpto *eqpto);
   TTreeNode* __fastcall TViewInsereBarra(VTBarra *barra);
   TTreeNode* __fastcall TViewInsereChave(VTChave *chave);
   void       __fastcall TViewInsereEqpto(VTEqpto *eqpto);
   TTreeNode* __fastcall TViewInsereGrupo(TTreeNode *node_pai, VTGrupo *grupo);
   void       __fastcall TViewInsereLisDado(TTreeNode *node_pai, TList *lisDADO);
   void       __fastcall TViewInsereLisGrupo(TTreeNode *node_pai, TList *lisGRUPO);
   void       __fastcall TViewInsereLisVetval(TTreeNode *node_pai, TList *lisVETVAL);
   void       __fastcall TViewInserePtoDefeito(TTreeNode *node, VTEqpto *eqpto);
   TTreeNode* __fastcall TViewInsereTrafo(VTTrafo *trafo);
   TTreeNode* __fastcall TViewInsereTrafo3E(VTTrafo3E *trafo);
   TTreeNode* __fastcall TViewInsereTrecho(VTTrecho *trecho);

private:	//objetos externos
	VTApl    *apl;
	VTRedeCC *redeCC;
   VTResCC  *resCC;

private:	//dados locais
	int    width_min;
	TList *lisGRUPO;
	TList *lisEQP_SEL;
};

//---------------------------------------------------------------------------
#endif
//eof

