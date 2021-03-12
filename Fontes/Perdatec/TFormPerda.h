//---------------------------------------------------------------------------
#ifndef TFormPerdaH
#define TFormPerdaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <Dialogs.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include <Menus.hpp>
#include <Grids.hpp>
#include <System.Actions.hpp>
#include <System.ImageList.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTAgregada;
class VTResumo;
class VTNode;
class TSegmento;
class TRelRedePerda;
class TRelRedeEqpto;
class TRelRedeBalanco;
class TRelBalAgregado;
class TRelEqptoAgregado;
class TRelPerdaAgregado;

//---------------------------------------------------------------------------
class TFormPerda : public TForm
{
__published:   // IDE-managed Components
   TActionList *ActionList;
   TGroupBox *GBoxAgregadoEqpto;
   TGroupBox *GBoxAgregadoBalanco;
   TListView *LViewAgregadoEqpto;
   TListView *LViewAgregadoBalanco;
   TGroupBox *GBoxRedePerda;
   TGroupBox *GBoxRedeEqpto;
   TListView *LViewRedeEqpto;
   TGroupBox *GBoxRedeBalanco;
   TListView *LViewRedeBalanco;
   TListView *LViewRedePerda;
   TAction *ActionMnuGrupos_RegiaoAdiciona;
   TAction *ActionMnuGrupos_RegiaoRemove;
   TAction *ActionMnuGrupos_RegiaoRenomeia;
   TAction *ActionMnuGrupos_LocalAdiciona;
   TAction *ActionMnuGrupos_LocalRenomeia;
   TAction *ActionMnuGrupos_LocalRemove;
   TSaveDialog *SaveDialog;
   TGroupBox *GBoxBalancoTotal;
   TListView *LViewTotalBalanco;
   TGroupBox *GBoxRede;
   TTreeView *TView;
   TPanel *PanelLocaliza;
   TBitBtn *ButLocaliza;
   TRichEdit *RichEditLocaliza;
   TAction *ActionLocaliza;
   TImageList *ImageList;
   TListView *LViewAgregadoPerda;
   TPanel *PanelLeft;
   TPanel *PanelClient;
   TCheckBox *CheckBoxBalancoTotal;
   TComboBox *CBoxRelatorio;
   TGroupBox *GBoxAgregadoPerda;
   TGroupBox *GBoxRelatorio;
   TGroupBox *GBoxResumo;
   TListView *LViewResumo;
   TSplitter *Splitter;
   TBitBtn *ButHelp;
   TAction *ActionHelp;
   TGroupBox *GBoxPerdaSegmento;
   TListView *LViewPerdaSegmento;
   TAction *ActionAlteraUnidade;
   TToolBar *ToolBar;
   TToolButton *ButFecha;
   TToolButton *ButUnidade;
   TToolButton *ButRelatorio;
   TAction *ActionFecha;
   TPopupMenu *PopupMenuUnidade;
   TMenuItem *MenuItem_kwh;
   TMenuItem *MenuItem_kwh_30dias;
   TMenuItem *MenuItem_kwh_diasmes;
   TMenuItem *N1;
   TMenuItem *MenuItem_mwh;
   TMenuItem *MenuItem_mwh_30dias;
   TMenuItem *MenuItem_mwh_diasmes;
   TAction *ActionSalvaRelatorio;
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall LViewCompare(TObject *Sender, TListItem *Item1,
          TListItem *Item2, int Data, int &Compare);
   void __fastcall LViewColumnClick(TObject *Sender, TListColumn *Column);
   void __fastcall SaveDialogTypeChange(TObject *Sender);
   void __fastcall TViewChange(TObject *Sender, TTreeNode *TreeNode);
   void __fastcall ActionLocalizaExecute(TObject *Sender);
   void __fastcall RichEditLocalizaKeyPress(TObject *Sender, char &Key);
   void __fastcall CBoxRelatorioChange(TObject *Sender);
   void __fastcall CheckBoxBalancoTotalClick(TObject *Sender);
   void __fastcall ActionHelpExecute(TObject *Sender);
   void __fastcall ActionAlteraUnidadeExecute(TObject *Sender);
   void __fastcall ActionFechaExecute(TObject *Sender);
   void __fastcall ActionSalvaRelatorioExecute(TObject *Sender);
   void __fastcall FormShow(TObject *Sender);

public:      // User declarations
              __fastcall  TFormPerda(TComponent *Owner, VTApl *apl_owner, TWinControl *parent);
              __fastcall ~TFormPerda(void);
   void       __fastcall  Inicia(TDateTime date);
   void       __fastcall  CalculaAgregadaTotal(void);
   void       __fastcall  DefineUnidade(AnsiString unidade, double fat_mult);
   void       __fastcall  ExportaRelatorio(void);
   void       __fastcall  TViewInicia(int level=-1);

private:   //métodos
   void       __fastcall  AtualizaCaptions(void);
   void       __fastcall  ExportaRelatorioTexto(AnsiString arq);
   AnsiString __fastcall  FiltraSeparador(AnsiString valor, char separador='.');
   AnsiString __fastcall  FormataEnergia(double energia_mwh);
   AnsiString __fastcall  FormataPorcentagem(double dividendo, double divisor);
   void       __fastcall  FormIniciaPosicao(void);
   void       __fastcall  LViewAddItemBalanco(TListView *LView, VTResumo *resumo_rede, VTResumo *resumo_jus = NULL);
   void       __fastcall  LViewAddItemEqpto(TListView *LView, VTResumo *resumo_rede, VTResumo *resumo_jus=NULL);
   void       __fastcall  LViewAddItemPerda(TListView *LView, VTResumo *resumo_rede, VTResumo *resumo_jus=NULL);
   void       __fastcall  LViewAddItemSegmento(TListView *LView, TSegmento *segmento);
   void       __fastcall  LViewRedeBalancoInicia(TList *lisRESUMO);
   void       __fastcall  LViewRedeEqptoInicia(TList *lisRESUMO);
   void       __fastcall  LViewRedePerdaInicia(TList *lisRESUMO);
   void       __fastcall  MontaRelatorioBalanco(TList *lisRESUMO);
   void       __fastcall  MontaRelatorioBalancoAgregado(TList *lisRESUMO);
   void       __fastcall  MontaRelatorioBalancoEmpresa(void);
   void       __fastcall  MontaRelatorioEnergiaPassante(void);
   void       __fastcall  MontaRelatorioEqpto(TList *lisRESUMO);
   void       __fastcall  MontaRelatorioEqptoAgregado(TList *lisRESUMO);
   void       __fastcall  MontaRelatorioPerda(TList *lisRESUMO);
   void       __fastcall  MontaRelatorioPerdaAgregado(TList *lisRESUMO);
   void       __fastcall  MontaRelatorioPerdaSegmento(void);
   void       __fastcall  MontaRelatorioPerdaSegmento(TList *lisRESUMO);
   void       __fastcall  MontaRelatorioResumo(void);
   VTResumo*  __fastcall  MontaResumoAgregado(int tiporede_id);
   VTResumo*  __fastcall  MontaResumoAgregado(TList *lisRESUMO, int tiporede_id);
   TTreeNode* __fastcall  TViewInsereNode(TTreeView *TView, int level, VTNode *node, TTreeNode *treenode_pai=NULL);
   void       __fastcall  TViewSelectedResumo(TList *lisEXT, int tiporede_id=-1);

private:   //dados locais
   TDateTime  date;
   double     fat_mult;
   TList      *lisRES;
   TList      *lisTMP;
   VTApl      *apl;
   VTResumo   *resumo_agregado;
   VTAgregada *agregada;
   TRelRedePerda *relRedePerda;
   TRelRedeEqpto *relRedeEqpto;
   TRelRedeBalanco *relRedeBalanco;
   TRelBalAgregado *relBalAgregado;
   TRelEqptoAgregado *relEqptoAgregado;
   TRelPerdaAgregado *relPerdaAgregado;

   struct{//controle de ordenação de ListView
         TListView   *lview;
         TListColumn *column;
         bool         ascendente;
         }LViewSort;
   struct{
         int last_level;
         }tview;
   //enum levelREDE{levelETD=2, levelPRI=3};
   enum levelREDE{levelETD=1, levelPRI=2, levelSEC=3}; //FKM - 2015.09.21 mudei

private: //dados externos
   //TComponent *owner;       //FKM -2015.09.21 Teste
};

//---------------------------------------------------------------------------
#endif
//eof
