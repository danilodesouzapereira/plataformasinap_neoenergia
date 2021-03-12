//---------------------------------------------------------------------------
#ifndef TFormEstudoH
#define TFormEstudoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <Menus.hpp>
#include <System.Actions.hpp>
#include <Vcl.Buttons.hpp>
#include <System.ImageList.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTAlternativa;
class VTEstudo;
class VTPlanejamento;
class VTObra;
class VTAcao;
class VTEqpto;

//---------------------------------------------------------------------------
class TFormEstudo : public TForm
{
__published:   // IDE-managed Components
   TGroupBox *GroupBoxEstudo;
   TToolBar *ToolBarRede;
   TActionList *ActionList;
   TAction *ActionAlternativaInsere;
   TAction *ActionAlternativaRetira;
   TToolButton *ButAlternativa;
   TTreeView *TreeView;
   TToolButton *ButAnalise;
   TAction *ActionAnaliseDefault;
   TToolButton *ButObra;
   TAction *ActionObraInsere;
   TAction *ActionObraRetira;
   TPopupMenu *PopupMenuAlternativa;
   TPopupMenu *PopupMenuObra;
   TMenuItem *MenuAlternativaInsere;
   TMenuItem *MenuAlternativaRetira;
   TMenuItem *MenuObr;
   TMenuItem *MenuObraRetira;
   TToolButton *ButEditor;
   TAction *ActionEditor;
   TToolButton *ButAlign;
   TAction *ActionAlignNone;
   TAction *ActionAlignLeft;
   TAction *ActionAlignRight;
   TAction *ActionAlignUp;
   TAction *ActionAlignDown;
   TPopupMenu *PopupMenuAlign;
   TMenuItem *mnuAlignLeft;
   TMenuItem *mnuAlignRight;
   TMenuItem *mnuAlignDown;
   TMenuItem *mnuAlignUp;
   TMenuItem *mnuAlignNode;
   TAction *ActionObraAlterada;
   TMenuItem *MenuItemAlternativaEdita;
   TAction *ActionAlternativaEdita;
   TAction *ActionObraEdita;
   TMenuItem *MenuItemObraEdita;
   TAction *ActionAlternativaCopia;
   TMenuItem *MenuAlternativaCopia;
   TAction *ActionAnaliseCfgPeriodo;
   TPopupMenu *PopupMenuAnalise;
   TMenuItem *MenuItemAnaliseCfgPeriodo;
   TMenuItem *N1;
   TAction *ActionObraExibeAcao;
   TMenuItem *MenuItemObraExibeAcao;
   TMenuItem *N2;
   TAction *ActionRelatorio;
   TToolButton *ButRelatorio;
   TPopupMenu *PopupMenuRelatorio;
   TMenuItem *MenuItem4;
   TMenuItem *MenuItem5;
   TMenuItem *MenuItemAnaliseDefault;
   TPopupMenu *TViewPopupMenu;
   TMenuItem *MenuItem2;
   TMenuItem *MenuItem3;
   TMenuItem *MenuItem6;
   TMenuItem *VisualizarAlternativa1;
   TAction *ActionVisualAltAno;
   TGroupBox *GroupBox1;
   TLabel *LabelEstudo;
   TBitBtn *ButEstudoEdita;
   TImageList *ImageList;
   TAction *ActionEstudoEdita;
	TMenuItem *MenuItemObraColorEnabled;
	TAction *ActionObraColorEnabled;
	TPanel *PanelAviso;
	TLabel *LabelAviso;
   TAction *ActionAnaliseCfgConfiabilidade;
   TMenuItem *MenuItemAnaliseCfgConfiabilidade;
   TToolButton *ButSalvaAlt;
   TAction *ActionSalvaAlt;
	TToolButton *ToolButton1;
   void __fastcall ActionAlternativaInsereExecute(TObject *Sender);
   void __fastcall ActionAlternativaRetiraExecute(TObject *Sender);
   void __fastcall TreeViewChange(TObject *Sender, TTreeNode *Node);
   void __fastcall ActionAnaliseDefaultExecute(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall ActionObraInsereExecute(TObject *Sender);
   void __fastcall ActionObraRetiraExecute(TObject *Sender);
   void __fastcall ActionEditorExecute(TObject *Sender);
   void __fastcall ActionAlignDownExecute(TObject *Sender);
   void __fastcall ActionAlignNoneExecute(TObject *Sender);
   void __fastcall ActionAlignLeftExecute(TObject *Sender);
   void __fastcall ActionAlignRightExecute(TObject *Sender);
   void __fastcall ActionAlignUpExecute(TObject *Sender);
   void __fastcall TreeViewDblClick(TObject *Sender);
   void __fastcall ActionObraAlteradaExecute(TObject *Sender);
   void __fastcall ActionAlternativaEditaExecute(TObject *Sender);
   void __fastcall ActionObraEditaExecute(TObject *Sender);
   void __fastcall ActionAlternativaCopiaExecute(TObject *Sender);
   void __fastcall ActionAnaliseCfgPeriodoExecute(TObject *Sender);
   void __fastcall ActionObraExibeAcaoExecute(TObject *Sender);
   void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
   void __fastcall ActionRelatorioExecute(TObject *Sender);
   void __fastcall TreeViewMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
   void __fastcall ActionVisualAltAnoExecute(TObject *Sender);
   void __fastcall TreeViewChanging(TObject *Sender, TTreeNode *Node,
          bool &AllowChange);
   void __fastcall ActionEstudoEditaExecute(TObject *Sender);
	void __fastcall ActionObraColorEnabledExecute(TObject *Sender);
   void __fastcall ActionAnaliseCfgConfiabilidadeExecute(TObject *Sender);
   void __fastcall ActionSalvaAltExecute(TObject *Sender);
	void __fastcall ToolButton1Click(TObject *Sender);

public:      // User declarations
                  __fastcall  TFormEstudo(TComponent* Owner, VTApl *apl, TWinControl *parent);
                  __fastcall ~TFormEstudo(void);
   void           __fastcall  AtualizaEstado(bool gravando_base);

private:   //métodos
   //void           __fastcall AvisoLimiteDeBarras(void);
   bool           __fastcall ConfiguraOpcaoAnalise(void);
   bool           __fastcall EditaObra(VTObra *obra);
   bool           __fastcall ExistePlanejamentoEstudo(void);
   bool           __fastcall ExisteRedeEqvBT(void);
   void           __fastcall FormIniciaPosicao(void);
   void           __fastcall FundeAcoesDasAlternativas(void);
   void           __fastcall GraficoMoldura(VTEqpto *eqpto=NULL);
   void           __fastcall GraficoZoom(VTEqpto *eqpto);
   void           __fastcall HabilitaApresentacaoDasAcoes(bool enabled);
	void           __fastcall Inicia(void);
	VTAlternativa* __fastcall InsereAlternativa(AnsiString nome, bool padrao=false);
   AnsiString     __fastcall NomeProximaAlternativa(void);
   void           __fastcall PopupMenuInicia(TPopupMenu *PopupMenu, TList *lisACTION);
   bool 			__fastcall RelatorioObra(VTEstudo *estudo);
   void           __fastcall RetiraAlternativa(VTAlternativa *alternativa);
	VTAlternativa* __fastcall SelecionaAlternativaPadrao(void);
	VTAlternativa* __fastcall TreeViewAlternativaSelecionada(TTreeNode *node);
   VTAlternativa* __fastcall TreeViewAlternativaSelecionada(void);
   void           __fastcall TreeViewDescricaoAcao(VTAcao *acao, AnsiString &text);
   TTreeNode*     __fastcall TreeViewExisteTreeNode(void *data);
	void           __fastcall TreeViewInicia(void);
   void           __fastcall TreeViewInsereTreeNodeAcao(TTreeNode *node_obra,  VTAcao *acao);
   void           __fastcall TreeViewInsereTreeNodeAcao(TTreeNode *node_obra, VTObra *obra);
	void           __fastcall TreeViewInsereTreeNodeObra(TTreeNode *node_alter, VTAlternativa *alternativa);
	void           __fastcall TreeViewInsereTreeNodeObra(TTreeNode *node_alter, VTObra *obra);
	AnsiString     __fastcall TreeViewNomeDaObra(VTObra *obra);
   VTObra*        __fastcall TreeViewObraSelecionada(void);
   void           __fastcall TrocaRedeEqvBTporCarga(void);
   bool           __fastcall ValidaDataTodasObras(void);

private:   //objetos externos
   VTApl          *apl;
   VTEstudo       *estudo;
   VTPlanejamento *planejamento;

private:   //dados locais
   bool     usuario_definiu_opcao_analise;
   TList    *lisTMP;
   struct   {//coordenadas p/ janela flutuante
            int top;
            int left;
            int width;
            int height;
            } janela;
};

//---------------------------------------------------------------------------
#endif
//eof

