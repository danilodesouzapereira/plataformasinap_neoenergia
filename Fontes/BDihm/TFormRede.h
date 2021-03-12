//---------------------------------------------------------------------------
#ifndef TFormRedeH
#define TFormRedeH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Menus.hpp>
#include <ActnList.hpp>
#include <System.Actions.hpp>
#include <Vcl.ExtCtrls.hpp>

//---------------------------------------------------------------------------
class VTAbre;
class VTApl;
class VTDominio;
class VTEstudo;
class VTRede;
class VTTipoRede;

//---------------------------------------------------------------------------
class TFormRede : public TForm
{
__published:   // IDE-managed Components
   TTreeView *TView;
   TPopupMenu *PopupMenu;
   TMenuItem *Selecionar1;
   TMenuItem *IncluirRedes1;
   TActionList *ActionList1;
   TAction *ActionExclui;
   TAction *ActionInclui;
   TAction *ActionDeleteDominio;
   TMenuItem *MenuItemDeleteDominio;
   TMenuItem *N1;
   TAction *ActionDeleteRede;
   TMenuItem *MenuItemDeleteRede;
   TAction *ActionAlteraDominio;
   TMenuItem *MenuItemAlteraDominio;
   TPanel *PanelLog;
   void __fastcall TViewAdvancedCustomDrawItem(TCustomTreeView *Sender,
          TTreeNode *Node, TCustomDrawState State, TCustomDrawStage Stage,
          bool &PaintImages, bool &DefaultDraw);
   void __fastcall TViewChange(TObject *Sender, TTreeNode *Node);
   void __fastcall TViewDblClick(TObject *Sender);
   void __fastcall TViewExpanding(TObject *Sender, TTreeNode *Node,
          bool &AllowExpansion);
   void __fastcall ActionExcluiExecute(TObject *Sender);
   void __fastcall ActionIncluiExecute(TObject *Sender);
   void __fastcall TViewMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
   void __fastcall PopupMenuPopup(TObject *Sender);
   void __fastcall ActionDeleteDominioExecute(TObject *Sender);
   void __fastcall ActionDeleteRedeExecute(TObject *Sender);
   void __fastcall ActionAlteraDominioExecute(TObject *Sender);
   void __fastcall TViewChanging(TObject *Sender, TTreeNode *Node, bool &AllowChange);


public:      // User declarations
        __fastcall  TFormRede(TComponent* Owner, VTApl *apl, TWinControl *parent);
        __fastcall ~TFormRede(void);
   void __fastcall  CancelaSelecao(void);
   bool __fastcall  CarregaRedeAdicional(TList *lisREDE_PRE);
   bool __fastcall  CarregaRedeCompleta(void);
   bool __fastcall  CarregaRedeSelecionada(void);
   bool __fastcall  CriaEstudoPlanejamento(void);
   bool __fastcall  CriaEstudoReclamacaoBT(void);
   void __fastcall  Desabilita(void);
   void __fastcall  Habilita(void);
   void __fastcall  RedeSelecionadaPeloGrafico(VTRede *rede);
   void __fastcall  TViewInicia(void);

private:   // métodos
   void      __fastcall BD_LeListaRede(VTEstudo *estudo, VTDominio *dominio);
   void      __fastcall BD_LeListaRede(VTEstudo *estudo, VTTipoRede *tiporede);
   void      __fastcall BD_LeListaRedeVizinha(VTEstudo *estudo, VTRede *rede, TList *lisVIZ);
   VTEstudo* __fastcall CriaEstudo(int tipo);
   void      __fastcall IniciaLisRedeVizinha(VTEstudo *estudo, VTRede *rede, TList *lisVIZ);
   void      __fastcall ReiniciaEsquematico(VTEstudo *estudo, TList* lisREDE);
   void      __fastcall Seleciona(TTreeNode *Node);
   void      __fastcall SelecionaRedeMaisVizinhas(VTRede *rede, VTTipoRede *tiporede = NULL);
   void      __fastcall SelecionaTodasRedesDoDominio(TTreeNode *node_dominio);
   void      __fastcall SelecionaTodasRedesDoTipo(TTreeNode *node_tiporede);
   void      __fastcall SelecionaTodasRedesVizinhas(TTreeNode *node_viz);
   bool      __fastcall SelecionaUmaRede(VTRede *rede);
   bool      __fastcall SelecionaUmaRedeVizinha(VTRede *rede_viz);
   void      __fastcall TViewAtualizaNodeRedeSelecionada(void);
   void      __fastcall TViewInsereRedeDoDominio(TTreeNode *node_dominio);
   void      __fastcall TViewInsereRedeDoTiporede(TTreeNode *node_tiporede);
   void      __fastcall TViewInsereRedeVizinha(TTreeNode *node_rede);
   void      __fastcall TViewLisRedes(TTreeNode *node_ref, TList *lisEXT);

private:   // objetos externos
   VTAbre         *abre;

private:    //dados locais
   enum enumLEVEL {LevelDominio=0,
                   LevelTipoRede, LevelRede,
                   LevelVizinha,  LevelRedeVizinha,
                   LevelRedeSel = LevelDominio};
   bool      inserir;
   TList     *lisEQP;
   TTreeNode *node_sel;    //nó fixo p/ exibir todas as redes selecionadas
   TTreeNode *right_click_node;
   TTreeNode *node_changing;
   VTEstudo  *estudo_zero;
};

//---------------------------------------------------------------------------
#endif
//eof

