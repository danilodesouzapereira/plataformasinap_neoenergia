//---------------------------------------------------------------------------
#ifndef TFormTensaoH
#define TFormTensaoH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Graphics.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.ToolWin.hpp>
#include "TFormEdt.h"
#include <Vcl.Grids.hpp>
#include <System.ImageList.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTAtributo;
class VTEdt;
class VTEqpto;
class VTZona;
class VTZonas;

//---------------------------------------------------------------------------
class TFormTensao : public TFormEdt
{
__published:	// IDE-managed Components
	TGroupBox *gboxEqpto;
	TStringGrid *sgrid;
	TGroupBox *GroupBox1;
	TTreeView *TView;
	TPanel *Panel1;
	TCheckBox *CheckBoxChaveAberta;
	TSplitter *Splitter1;
	TToolButton *ButEdtTensao;
	TToolButton *BtuFecha;
	TAction *ActionFecha;
	TAction *ActionEdtTensao;
	TAction *ActionSelEqpto;
	void __fastcall TViewChange(TObject *Sender, TTreeNode *Node);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall ActionEdtTensaoExecute(TObject *Sender);
	void __fastcall ActionFechaExecute(TObject *Sender);
	void __fastcall ActionEdtExecute(TObject *Sender);
	void __fastcall ActionSelEqptoExecute(TObject *Sender);
	void __fastcall CheckBoxChaveAbertaClick(TObject *Sender);

public:		// User declarations
	__fastcall  TFormTensao(TComponent* Owner, VTApl *apl, TAction *ActionOnDone, TWinControl *parent);
	__fastcall ~TFormTensao(void);

private:	//métodos
	void       __fastcall ExibeMoldura(VTEqpto *eqpto=NULL);
	void       __fastcall ExibeMoldura(TList *lisEQP);
	bool       __fastcall RedefineTensao(VTZona *zona, double vff_kv);
	void       __fastcall SGridAtualiza(VTEqpto *eqpto=NULL);
	void       __fastcall SGridAtualizaZona(VTZona *zona, int zona_id);
	void       __fastcall TViewAtualizaTreeNodeText(TTreeNode *node, VTZona *zona, int zona_id);
	TTreeNode* __fastcall TViewExisteTreeNode(void *data);
	void       __fastcall TViewInicia(void);
	VTZona*    __fastcall TViewZonaSelecionada(void);

private:	//objetos externos
	VTApl   *apl;
	TAction *ActionOnDone;

private:	//dados locais
	TList      *lisEQP;
	VTAtributo *atributo;
	VTEdt      *edt;
	VTZonas    *zonas;
};

//---------------------------------------------------------------------------
#endif
