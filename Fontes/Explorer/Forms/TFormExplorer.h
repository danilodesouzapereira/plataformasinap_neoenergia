//---------------------------------------------------------------------------
#ifndef TFormExplorerH
#define TFormExplorerH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "TFORMEDT.h"
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <ImgList.hpp>
#include <Menus.hpp>
#include <ToolWin.hpp>
#include <Buttons.hpp>
#include <CheckLst.hpp>
#include <Dialogs.hpp>
#include <Grids.hpp>
#include "..\Forms\TFormEdt.h"
#include <System.Actions.hpp>
#include <System.ImageList.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTEdt;
class VTBarra;
class VTEqpto;
class VTLigacao;
class VTRede;
class VTLegenda;
class VTConsulta;
class VTEdt;

//---------------------------------------------------------------------------
class TFormExplorer : public TFormEdt
{
__published:	// IDE-managed Components
   TAction *ActionExibeRedePorTipo;
   TColorDialog *dlgSelColor;
   TAction *ActionRedeSel;
   TAction *ActionRedeSelAll;
   TAction *ActionRedeSelNone;
   TAction *ActionSelEqpto;
   TAction *ActionRedeChanged;
   TAction *ActionExibeRedePorHierarquia;
   TPopupMenu *PopupMenuExplorer;
   TMenuItem *MenuItemRedePorTipo;
   TMenuItem *MenuItemRedePorHierarquia;
   TMenuItem *MenuItemChavePorTipo;
   TAction *ActionExibeChavePorTipo;
   TAction *ActionExibeCargaPorTipo;
   TMenuItem *MenuItemCargaPorTipo;
   TAction *ActionExibeTrechoPorCargto;
   TMenuItem *MenuItemTrechoPorCargto;
   TAction *ActionExibeCapPorPotencia;
   TMenuItem *MenuItemCapPorPotencia;
   TAction *ActionHelp;
   TToolBar *ToolBarRight;
   TToolButton *ButExplorar;
   TToolButton *ButAjuda;
	TAction *ActionExibeReguladorPorTipoOp;
	TMenuItem *Reguladoresportipodeoperao1;
   void __fastcall ActionSelEqptoExecute(TObject *Sender);
   void __fastcall ActionRedeChangedExecute(TObject *Sender);
   void __fastcall ActionEdtExecute(TObject *Sender);
   void __fastcall ActionExibeRedePorHierarquiaExecute(TObject *Sender);
   void __fastcall ActionExibeRedePorTipoExecute(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall ActionExibeChavePorTipoExecute(TObject *Sender);
   void __fastcall ActionExibeCargaPorTipoExecute(TObject *Sender);
   void __fastcall ActionExibeTrechoPorCargtoExecute(TObject *Sender);
   void __fastcall ActionExibeCapPorPotenciaExecute(TObject *Sender);
   void __fastcall ActionHelpExecute(TObject *Sender);
	void __fastcall ActionExibeReguladorPorTipoOpExecute(TObject *Sender);

public:		// User declarations
        __fastcall  TFormExplorer(TForm* Owner, VTApl *apl_owner, TWinControl *parent);
        __fastcall ~TFormExplorer(void);
   void __fastcall  Ativo(void);
   void __fastcall  Atualiza(void);

private:	//métodos
   void       __fastcall CLBoxAtualizaRede(void);
   void       __fastcall CLBoxAtualizaReducao(void);
   TForm*     __fastcall ExisteForm(AnsiString class_name);
	void       __fastcall FormIniciaPosicao(void);
	void       __fastcall Moldura(VTEqpto *eqpto);
	void       __fastcall TreeViewOrdenaHierarquia(void);
	void       __fastcall TreeViewOrdenaTipo(void);
   void       __fastcall TreeViewIniciaTipoRede(void);
   TTreeNode* __fastcall TreeViewExisteNodeChild(TTreeNode *node_pai, AnsiString text);
   TTreeNode* __fastcall TreeViewExisteEqpto(VTEqpto *eqpto);
   TTreeNode* __fastcall TreeViewExisteTipoEqpto(VTEqpto *eqpto);
   TTreeNode* __fastcall TreeViewInsereEqpto(VTEqpto *eqpto);
	void       __fastcall Zoom(VTEqpto *eqpto);

private:	//objetos externos
   VTApl *apl;

private:	//dados locais
   VTEdt *edt;
   TForm *form_atual;
   TList *lisFORM;
   //TForm *FormChavePorTipo;
   //TForm *FormRedePorTipo;
   //TForm *FormRedePorHierarquia;
};

//---------------------------------------------------------------------------
#endif
//eof
