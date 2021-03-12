//---------------------------------------------------------------------------
#ifndef TFormDebugH
#define TFormDebugH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <ActnList.hpp>
#include <ToolWin.hpp>
#include <System.Actions.hpp>
#include <Vcl.Menus.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTArea;
class VTAtributo;
class VTConsulta;
class VTCorte;
class VTEqpto;
class VTMRede;
class VTRede;
class VTReducao;
class VTZona;
class VTBloco;

//---------------------------------------------------------------------------
class TFormDebug : public TForm
{
__published:	// IDE-managed Components
   TSplitter *Splitter1;
   TGroupBox *gboxEqpto;
   TStringGrid *sgrid;
   TGroupBox *GroupBox1;
   TToolBar *ToolBar1;
   TTreeView *TView;
   TToolButton *butOrdenaPorCorte;
   TToolButton *butOrdenaPorRede;
   TActionList *ActionList;
   TAction *ActionOrdenaPorRede;
   TAction *ActionOrdenaPorCorte;
   TAction *ActionOrdenaPorMRede;
   TToolButton *butOrdenaPorMRede;
   TToolButton *ButZona;
   TAction *ActionZonas;
   TPanel *Panel1;
   TCheckBox *CheckBoxChaveAberta;
   TToolButton *ButBlocos;
   TAction *ActionBlocos;
   TPopupMenu *PopupMenuBlocos;
   TActionList *ActionListBlocos;
   TAction *ActionEliminaBlocosNaoEnergizados;
   TAction *ActionEliminaElosBlocosIguais;
   TAction *ActionEliminaElosComChaveAberta;
   TMenuItem *Eliminablocosnoenergizados1;
   TMenuItem *Eliminaelosqueligamummesmobloco1;
   TMenuItem *Eliminaeloscomchaveaberta1;
   void __fastcall TViewChange(TObject *Sender, TTreeNode *Node);
   void __fastcall ActionOrdenaPorCorteExecute(TObject *Sender);
   void __fastcall ActionOrdenaPorRedeExecute(TObject *Sender);
   void __fastcall ActionOrdenaPorMRedeExecute(TObject *Sender);
   void __fastcall ActionZonasExecute(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall ActionEliminaBlocosNaoEnergizadosExecute(TObject *Sender);
   void __fastcall ActionEliminaElosBlocosIguaisExecute(TObject *Sender);
   void __fastcall ActionEliminaElosComChaveAbertaExecute(TObject *Sender);
   void __fastcall ActionBlocosExecute(TObject *Sender);
   void __fastcall TViewDblClick(TObject *Sender);

public:		// User declarations
   __fastcall  TFormDebug(TForm* Owner, VTApl *apl_owner, TWinControl *parent);
   __fastcall ~TFormDebug(void);

private:	//métodos
   void __fastcall CalculaAreaEsq(TList *lisBAR);
   void __fastcall CalculaAreaUtm(TList *lisBAR);
   void __fastcall DestacaLigacao(TList *lisLIG=NULL);
   void __fastcall GraficoMoldura(VTEqpto *eqpto=NULL);
   void __fastcall GraficoMoldura(TList *lisEQP);
   void __fastcall GraficoZoom(VTEqpto *eqpto);
   void __fastcall GraficoZoom(TList *lisBAR);
   void __fastcall SGridAtualiza(VTEqpto *eqpto=NULL);
   void __fastcall SGridAtualizaBloco(VTBloco *bloco);
   void __fastcall SGridAtualizaZona(VTZona *zona);
   void __fastcall TreeViewInsereBarraDoCorte(VTCorte *corte, TTreeNode *node_corte);
   void __fastcall TreeViewInsereCorte(TTreeNode *node_pai);
   void __fastcall TreeViewInsereCorteDaMRede(VTMRede *rede, TTreeNode *node_mrede);
   void __fastcall TreeViewInsereCorteDaRede(VTRede *rede, TTreeNode *node_rede);
   void __fastcall TreeViewInsereEqptoDaReducao(VTReducao *reducao, TTreeNode *node_reducao);
   void __fastcall TreeViewInsereMRede(TTreeNode *node_pai);
   void __fastcall TreeViewInsereRede(TTreeNode *node_pai);
   void __fastcall TreeViewInsereRedeDoCorte(VTCorte *corte, TTreeNode *node_corte);
   void __fastcall TreeViewInsereRedeDaMRede(VTMRede *mrede, TTreeNode *node_mrede);
   void __fastcall TreeViewInsereReducaoDoCorte(VTCorte *corte, TTreeNode *node_corte);
   void __fastcall TreeViewOrdenaBloco(void);
   void __fastcall TreeViewOrdenaPorCorte(void);
   void __fastcall TreeViewOrdenaPorMRede(void);
   void __fastcall TreeViewOrdenaPorRede(void);
   void __fastcall TreeViewOrdenaZona(void);

private:	//dados locais
   VTApl      *apl;
   VTArea     *area_utm, *area_esq;
   VTAtributo *atributo;
   VTConsulta *consulta;
   TList      *lisEQP;
   TList      *lisREDU;
};

//---------------------------------------------------------------------------
#endif
//eof
