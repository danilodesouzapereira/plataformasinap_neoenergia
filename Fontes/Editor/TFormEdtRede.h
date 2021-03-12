//---------------------------------------------------------------------------
#ifndef TFormEdtRedeH
#define TFormEdtRedeH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <ActnList.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <Dialogs.hpp>
#include <Menus.hpp>
#include <System.Actions.hpp>
#include <Vcl.Buttons.hpp>
#include <System.ImageList.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTCluster;
class VTEdt;
class VTEqpto;
class VTRede;
class VTTipoRede;

//---------------------------------------------------------------------------
class TFormEdtRede : public TForm
{
__published:	// IDE-managed Components
   TSplitter *Splitter1;
   TActionList *ActionList;
   TAction *ActionCancela;
   TAction *ActionConfirma;
   TAction *ActionInsere;
   TAction *ActionRetira;
   TToolBar *ToolBar1;
   TToolButton *butInclui;
   TToolButton *butSair;
   TToolButton *butInsere;
   TToolButton *butRetira;
   TToolButton *ToolButton1;
   TGroupBox *gboxTopologia;
   TCheckBox *checkRadial;
   TCheckBox *checkEstatica;
   TTreeView *TViewEqpto;
   TAction *ActionAtributoAlterado;
   TPanel *PanelRede;
   TGroupBox *GBoxLigacao;
   TAction *ActionPadrao;
   TGroupBox *GBoxRede;
   TRadioGroup *RadioGroupModo;
   TPanel *PanelColorRede;
   TAction *ActionSelColorRede;
   TColorDialog *ColorDialog;
   TPanel *Panel1;
   TGroupBox *GBoxColorDestaque;
   TCheckBox *CheckBoxColorDestaque;
   TPanel *PanelColorDestaque;
   TAction *ActionSelColorDestaque;
   TGroupBox *GBoxBarraInicial;
   TPopupMenu *PopupMenu;
   TMenuItem *MenuItemBarraInicial;
   TAction *ActionSelBarraInicial;
   TToolButton *ButCriaBarra;
   TAction *ActionEdtBarra;
   TToolButton *ToolButton2;
   TAction *ActionUsuarioEditouBarra;
   TComboBox *CBoxBarra;
   TToolButton *ButSelBarra;
   TAction *ActionEdtBusca;
   TAction *ActionUsuarioSelecionouBarra;
   TGroupBox *GBoxTensao;
   TBitBtn *ButAlteraVnom;
   TRichEdit *RichEditVnom;
   TImageList *ImageList;
   TAction *ActionVnomAltera;
   TTreeView *TViewRede;
   void __fastcall FormShow(TObject *Sender);
   void __fastcall ActionInsereExecute(TObject *Sender);
   void __fastcall ActionRetiraExecute(TObject *Sender);
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall ActionConfirmaExecute(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall TViewRedeChange(TObject *Sender, TTreeNode *Node);
   void __fastcall ActionAtributoAlteradoExecute(TObject *Sender);
   void __fastcall TViewRedeDragDrop(TObject *Sender, TObject *Source,
          int X, int Y);
   void __fastcall TViewRedeDragOver(TObject *Sender, TObject *Source,
          int X, int Y, TDragState State, bool &Accept);
   void __fastcall ActionPadraoExecute(TObject *Sender);
   void __fastcall TViewEqptoChange(TObject *Sender, TTreeNode *Node);
   void __fastcall TViewRedeDblClick(TObject *Sender);
   void __fastcall TViewEqptoDblClick(TObject *Sender);
   void __fastcall RadioGroupModoClick(TObject *Sender);
   void __fastcall ActionSelColorRedeExecute(TObject *Sender);
   void __fastcall ActionSelColorDestaqueExecute(TObject *Sender);
   void __fastcall CheckBoxColorDestaqueClick(TObject *Sender);
   void __fastcall TViewEqptoMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
   void __fastcall ActionEdtBarraExecute(TObject *Sender);
   void __fastcall ActionUsuarioEditouBarraExecute(TObject *Sender);
   void __fastcall CBoxBarraChange(TObject *Sender);
   void __fastcall ActionEdtBuscaExecute(TObject *Sender);
   void __fastcall ActionUsuarioSelecionouBarraExecute(TObject *Sender);
   void __fastcall ActionVnomAlteraExecute(TObject *Sender);
   void __fastcall TViewRedeEdited(TObject *Sender, TTreeNode *Node, UnicodeString &S);



public:		// User declarations
   __fastcall  TFormEdtRede(TComponent* Owner, VTApl *apl, TAction *ActionOnDone, TWinControl *parent);
   __fastcall ~TFormEdtRede(void);

private:	//métodos
   void       __fastcall ExibeBarraInicialRedeSelecionada(void);
   void       __fastcall CBoxBarraInicia(void);
   AnsiString __fastcall CodigoRede(VTRede *rede);
   void       __fastcall CriaBarraInicial(void);
   AnsiString __fastcall DefineCodigoCluster(void);
   AnsiString __fastcall DefineCodigoRede(void);
   void       __fastcall EqptoAlteradoAtualiza(TList *lisLOCAL, TList *lisORIG);
   void       __fastcall EqptoNovoInsere(TList *lisLOCAL, TList *lisORIG);
   void       __fastcall EqptoExcluidoRetira(TList *lisLOCAL, TList *lisORIG);
   //bool       __fastcall ExclusaoPodeSerFeita(VTCluster *cluster);
   bool       __fastcall ExclusaoPodeSerFeita(VTRede *rede);
   bool       __fastcall ExisteCluster(AnsiString codigo);
   bool       __fastcall ExisteRede(AnsiString codigo);
   bool       __fastcall ExisteRedeBTnova(void);
   VTEqpto*   __fastcall ExisteEqpto(TList *lisEQP, int eqpto_id);
   void       __fastcall FinalizaEdicaoTreeNodeText(void);
   void       __fastcall FormIniciaPosicao(void);
   void       __fastcall GraficoDestacaRede(VTRede *rede=NULL);
   void       __fastcall GraficoMoldura(VTEqpto *eqpto=NULL);
   void       __fastcall GraficoZoom(VTEqpto *eqpto=NULL);
   VTCluster* __fastcall HaClusterSelecionado(void);
   void       __fastcall InsereRedeNoCluster(VTRede *rede, VTCluster *cluster);
   VTRede*    __fastcall HaRedeSelecionada(void);
   TTreeNode* __fastcall IdentificaTreeNodeAgrupadas(void);
   TTreeNode* __fastcall IdentificaTreeNodeTipoRede(void);
   void       __fastcall IniciaLisCluster_Rede(void);
   void       __fastcall InsereCluster(TTreeNode *node);
   void       __fastcall InsereRede(TTreeNode *node_tipo);
   bool       __fastcall LeTensaoNominal(double &vnom_kv);
   void       __fastcall RedefineTipoRede(VTRede *rede, VTTipoRede *tipo_rede);
   void       __fastcall TViewEqptoInicia(VTRede *rede);
   TTreeNode* __fastcall TViewRedeExisteNodeTipo(int tipo_rede_id);
   void       __fastcall TViewRedeInicia(void);
   bool       __fastcall ValidaBarraInicialRedeRadial(void);

private:	//dados externos
   TAction *ActionOnDone;
   VTApl   *apl;

private:	//dados locais
   VTEdt      *edt_busca;
   TTreeNode  *node_raiz;
   struct   {
            TList *lisCLUSTER;  //lista temporária de Clustes
            TList *lisREDE;     //lista temporária de Redes
            }local, orig;
   TList      *lisEQP;
   TList      *lisVIZ;
};

//---------------------------------------------------------------------------
#endif
//eof

