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
#include <Vcl.CheckLst.hpp>
#include <System.ImageList.hpp>
//---------------------------------------------------------------------------
class VTApl;
class VTRegiao;
class VTRegioes;
class TEdtRegiao;
//class VTMercado;
//class VTSegmento;
//---------------------------------------------------------------------------
class TFormEdtRegiao : public TFormEdt
{
__published:	// IDE-managed Components
	TToolBar *ToolBar1;
   TToolButton *butRemove;
   TToolButton *butCancela;
   TToolButton *butDesenha;
   TToolButton *butFinaliza;
   TColorDialog *dlgSelColor;
   TAction *ActionCriarRegiao;
   TAction *ActionRemoveRegiao;
   TAction *ActionImportarRegiao;
   TAction *ActionFinalizarRegiao;
   TAction *ActionSalvaRegiao;
   TAction *ActionCancelarRegiao;
   TAction *ActionAlteraCor;
   TTreeView *tvRegiao;
   TGroupBox *GroupBox1;
   TAction *ActionOnDone;
   TAction *Action1;

	void __fastcall ActionCriarRegiaoExecute(TObject *Sender);
	void __fastcall ActionAlteraCorExecute(TObject *Sender);
	void __fastcall ActionCancelaRegiaoExecute(TObject *Sender);
	void __fastcall ActionFinalizarRegiaoExecute(TObject *Sender);
	void __fastcall ActionEdtExecute(TObject *Sender);
	void __fastcall ActionOnDoneExecute(TObject *Sender);
	void __fastcall ActionRemoveRegiaoExecute(TObject *Sender);

	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormHide(TObject *Sender);
	void __fastcall tvRegiaoDblClick(TObject *Sender);
	void __fastcall tvRegiaoEdited(TObject *Sender, TTreeNode *Node, UnicodeString &S);
	void __fastcall tvRegiaoEditing(TObject *Sender, TTreeNode *Node, bool &AllowEdit);
   void __fastcall tvRegiaoAdvancedCustomDrawItem(TCustomTreeView *Sender, TTreeNode *Node,
          TCustomDrawState State, TCustomDrawStage Stage, bool &PaintImages,
          bool &DefaultDraw);
   void __fastcall FormShow(TObject *Sender);

private:	// User declarations
	 void __fastcall CancelaEdicaoRegiao(void);
	 void __fastcall ConfirmaEdicaoRegiao(void);
	 void __fastcall IniciaTreeView(void);
	 void __fastcall RegioesVisible(bool visible);
    void __fastcall RemoveRegiao(VTRegiao *regiao);
    void __fastcall Set_ActionOnDone(TAction *OnDone);
	 bool __fastcall VerificaMinPontos(VTRegiao *regiao);


private:	// Dados externos
	VTApl 	*apl;
	TForm		*form_dono;

private:	// Dados internos
	TEdtRegiao		*edtRegiao;
	VTRegioes		*regioes;
	VTRegiao			*regiao;
	strUTM			utm;
	strESQ_BARRA	*pixelUtm;
	TTreeNode		*nodeLastRegiao;
	int 				numVertices;
   TList          *lisRegiao;
   TList          *listaRegioesNovas;
   TList          *listaRegioesEditaveis;
   TAction        *onDone;
	//VTMercado		*mercado;
	//VTSegmento		*segmento;

public:		// User declarations
			__fastcall TFormEdtRegiao(TComponent* Owner, VTApl *apl, TWinControl *parent, TAction *OnDone,TList* lisRegiao);
			__fastcall ~TFormEdtRegiao(void);
	//void 	__fastcall InsereVerticeNoTreeView(int x, int y);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormEdtRegiao *FormEdtRegiao;
//---------------------------------------------------------------------------
#endif