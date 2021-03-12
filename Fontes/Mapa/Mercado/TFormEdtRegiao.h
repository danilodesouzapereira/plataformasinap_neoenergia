//---------------------------------------------------------------------------

#ifndef TFormEdtRegiaoH
#define TFormEdtRegiaoH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "TFormEdt.h"
#include <Dialogs.hpp> //added
#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Graphics.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.ToolWin.hpp>
//---------------------------------------------------------------------------
#include "..\Rede\Estrutura.h"
//---------------------------------------------------------------------------
class VTApl;
class VTRegiao;
class VTRegioes;
class TEdtRegiao;
class VTMercado;
class VTSegmento;
//---------------------------------------------------------------------------
class TFormEdtRegiao : public TFormEdt
{
__published:	// IDE-managed Components
	TGroupBox *GroupBox1;
	TAction *ActionAddRegiao;
	TAction *ActionRemoveRegiao;
	TAction *ActionImportarRegiao;
	TToolBar *ToolBar1;
	TToolButton *butDesenha;
	TToolButton *butRemove;
	TToolButton *butFinaliza;
	TToolButton *butCancela;
	TTreeView *tvRegiao;
	TAction *ActionFinalizaRegiao;
   TAction *ActionOnDone;
	TAction *ActionSalvaRegiao;
	TAction *ActionCancelaRegiao;
   TToolButton *ToolButton1;
	TAction *ActionAlteraCor;
	TColorDialog *dlgSelColor;

	void __fastcall ActionAddRegiaoExecute(TObject *Sender);
	void __fastcall ActionAlteraCorExecute(TObject *Sender);
	void __fastcall ActionCancelaRegiaoExecute(TObject *Sender);
	void __fastcall ActionFinalizaRegiaoExecute(TObject *Sender);
	void __fastcall ActionEdtExecute(TObject *Sender);
	void __fastcall ActionOnDoneExecute(TObject *Sender);
	void __fastcall ActionRemoveRegiaoExecute(TObject *Sender);

	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormHide(TObject *Sender);

	void __fastcall tvRegiaoAdvancedCustomDrawItem(TCustomTreeView *Sender, TTreeNode *Node,
			 TCustomDrawState State, TCustomDrawStage Stage, bool &PaintImages,
			 bool &DefaultDraw);
	void __fastcall tvRegiaoDblClick(TObject *Sender);
	void __fastcall tvRegiaoEdited(TObject *Sender, TTreeNode *Node, UnicodeString &S);
	void __fastcall tvRegiaoEditing(TObject *Sender, TTreeNode *Node, bool &AllowEdit);

private:	// User declarations
	 void __fastcall CancelaEdicaoRegiao(void);
	 void __fastcall ConfirmaEdicaoRegiao(void);
    void __fastcall ExcluiSegmentosDaRegiao(VTRegiao *regiao);
	 void __fastcall IniciaTreeView(void);
	 void __fastcall RegioesVisible(bool visible);
    void __fastcall RemoveRegiao(VTRegiao *regiao);
	 bool __fastcall VerificaMinPontos(VTRegiao *regiao);

private:	// Dados externos
	VTApl 	*apl;
	TForm		*form_dono;

private:	// Dados internos
	TEdtRegiao		*edtRegiao;
	//VTRegioes		*regioes;
	VTRegiao			*regiao;
	strUTM			utm;
	strESQ_BARRA	*pixelUtm;
	TTreeNode		*nodeLastRegiao;
	int 				numVertices;
	VTMercado		*mercado;
	VTSegmento		*segmento;
   TList          *lisRegiao;
   TList          *listaRegioesNovas;
   TList          *listaRegioesEditaveis;
   TAction        *onDone;

public:		// User declarations
			__fastcall TFormEdtRegiao(TComponent* Owner, VTApl *apl, TWinControl *parent, TAction *OnDone,TList* lisRegiao);
			__fastcall ~TFormEdtRegiao(void);
	//void 	__fastcall InsereVerticeNoTreeView(int x, int y);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormEdtRegiao *FormEdtRegiao;
//---------------------------------------------------------------------------
#endif
