//---------------------------------------------------------------------------
#ifndef TFormCfgFlowH
#define TFormCfgFlowH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ActnList.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
#include <ImgList.hpp>
#include "..\Geral\VTGeral.h"
#include "..\Rede\VTTipoRede.h"
#include <Dialogs.hpp>
#include <System.Actions.hpp>
#include <System.ImageList.hpp>
//---------------------------------------------------------------------------
//enum para as tabs
enum tabCFGV
{
	tabCFGV_REDESEC = 0,
	tabCFGV_REDEPRI,
	tabCFGV_REDESUB,
	tabCFGV_COUNT
};
//---------------------------------------------------------------------------
class VTApl;
class TFormChild;
class TFormMDI;

//---------------------------------------------------------------------------
class TFormCfgFlow : public TForm
{
__published:   // IDE-managed Components
   TActionList *ActionList1;
   TAction *ActionExibeLbl;
   TEdit *edtI2;
   TEdit *edtI1;
   TGroupBox *gbGrafI;
   TLabel *lblI1;
   TLabel *lblI3;
   TLabel *lblI2;
   TPanel *panCorI1;
   TPanel *panCorI2;
   TPanel *panCorI3;
   TEdit *edtV1;
   TEdit *edtV2;
   TEdit *edtV3;
   TEdit *edtV4;
   TGroupBox *gbGrafV;
   TLabel *lblV1;
   TLabel *lblV2;
   TLabel *lblV5;
   TLabel *lblV3;
   TLabel *lblV4;
   TPanel *panCorV1;
   TPanel *panCorV2;
   TPanel *panCorV3;
   TPanel *panCorV4;
   TPanel *panCorV5;
   TToolBar *ToolBar1;
   TToolButton *butInclui;
   TToolButton *butSair;
   TImageList *ImageList;
   TActionList *ActionList;
   TAction *ActionConfirma;
   TAction *ActionCancela;
   TAction *ActionDefault;
   TEdit *edtS2;
   TEdit *edtS1;
   TGroupBox *gbGrafS;
   TLabel *lblS1;
   TLabel *lblS3;
   TLabel *lblS2;
   TPanel *panCorS1;
   TPanel *panCorS2;
   TPanel *panCorS3;
   TColorDialog *ColorDialog;
   TGroupBox *GroupBox1;
   TLabel *Label6;
   TPanel *PanelCorDiagBom;
   TLabel *Label8;
   TPanel *PanelCorDiagRegular;
   TLabel *Label11;
   TPanel *PanelCorDiagRuim;
   TToolButton *ToolButton1;
	TLabel *Label12;
	TLabel *Label13;
	TPanel *Panel1;
	TPanel *pnlV1;
	TPanel *pnlV2;
	TPanel *pnlV3;
	TPanel *pnlV4;
	TPanel *pnlV5;
	TPanel *Panel2;
	TPanel *pnlGrafmin;
	TPanel *pnlGrafmed;
	TPanel *pnlGrafmax;
	TPanel *Panel3;
	TPanel *Panel4;
	TPanel *Panel5;
	TTabControl *tcFaixaV;
	TPanel *Panel6;
	TPanel *Panel7;
	TPanel *Panel8;
	TLabel *lblVlimsup;
	TLabel *lblClimsup;
	TLabel *lblSlimsup;
	TToolBar *ToolBar2;
	TToolButton *ToolButton5;
	TToolButton *ToolButton2;
	TAction *ActionCfgLimV;
	TComboBox *cboxBaseSelecionada;
	TGroupBox *gboxBaseSelecionada;
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall ActionExibeLblExecute(TObject *Sender);
   void __fastcall ActionDefaultExecute(TObject *Sender);
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall ActionConfirmaExecute(TObject *Sender);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall PanelCorDblClick(TObject *Sender);
	void __fastcall tcFaixaVChange(TObject *Sender);
	void __fastcall ActionCfgLimVExecute(TObject *Sender);
	void __fastcall cboxBaseSelecionadaChange(TObject *Sender);

public:      // User declarations
   __fastcall TFormCfgFlow(TComponent* Owner, VTApl *apl_owner);
   __fastcall ~TFormCfgFlow(void);

private:   // métodos
	void __fastcall AtualizaValoresTemp(int tab);
//   bool __fastcall BaseDadoLeGeral(void);
   bool __fastcall BaseDadoSalvaGeral(VTGeral *pGeral);
   void __fastcall CarregaValoresTemp(void);
   void __fastcall CBoxBaseSelecionadaInicia(void);
   void __fastcall ConfiguraGeral(VTApl *apl_de, VTApl *apl_para);
   void __fastcall ExibeCores(void);
   void __fastcall ExibeDados(VTGeral *pGeral);
   void __fastcall ExibeDados(void);
   void __fastcall ExibeDadosTensao(void);
   TFormChild* __fastcall FormChildSelecionado(void);
   bool __fastcall LeDados(void);
   AnsiString __fastcall NomeBase(TFormChild *pFormChild);
   void __fastcall SalvaValoresTemp(VTGeral *pGeral);
   void __fastcall SelecionaBaseFormChild(TFormChild *form);
   int  __fastcall TipoRedeTabTensao(void);
   bool __fastcall ValidaValoresTemp(void);
   //void __fastcall ReiniciaValoresDefault(void);

private:   //objetos externos
   VTApl   *apl;
   VTGeral *geral_RedeEmpresa;
   VTGeral *geral_mdi;
   VTGeral *geralOrigSel;
   TFormMDI *formMDI;

private:   // dados
   TList *lisFormChild;
   TFormChild *formChildEscolhido;
   VTGeral *geralTemp;
   double valoresTemp[tabCFGV_COUNT][4];  //valores temporarios
   int	   tabAnterior;
};

#endif
//---------------------------------------------------------------------------
//eof
