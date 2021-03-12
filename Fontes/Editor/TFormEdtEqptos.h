//---------------------------------------------------------------------------
#ifndef TFormEdtEqptosH
#define TFormEdtEqptosH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <ExtCtrls.hpp>
#include <FileCtrl.hpp>
#include <System.Actions.hpp>
#include <System.ImageList.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTArranjo;
class VTCarga;
class VTEqpto;
class VTRede;

//---------------------------------------------------------------------------
class TFormEdtEqptos : public TForm
{
__published:	// IDE-managed Components
   TImageList *ImageList1;
	TActionList *ActionList;
   TAction *ActionFecha;
   TPageControl *PageControl;
   TTabSheet *TabSheetLigacao;
   TListView *LViewLigacao;
   TToolBar *ToolBar2;
   TToolButton *ButDefRedeLigacao;
   TAction *ActionDefRedeLigacao;
   TTabSheet *TabSheetBarra;
	TToolBar *ToolBar3;
	TToolButton *ButDefineVnom;
   TAction *ActionDefVnomBarra;
	TListView *LViewBarra;
   TTabSheet *TabSheetTrecho;
   TTabSheet *TabSheetTrafo;
   TTabSheet *TabSheetChave;
   TToolBar *ToolBar4;
   TToolButton *ButDefArranjoTrecho;
   TListView *LViewTrecho;
   TToolBar *ToolBar5;
   TToolButton *ButDefSnomTrafo;
   TListView *LViewTrafo;
   TToolBar *ToolBar6;
   TToolButton *ButAbreChave;
   TListView *LViewChave;
   TAction *ActionDefArranjoTrecho;
   TAction *ActionDefSnomTrafo;
   TAction *ActionAbreChave;
   TEdit *EditBarraVnom;
   TAction *ActionFechaChave;
   TToolButton *ButFechaChave;
   TEdit *EditTrafoSnom;
   TComboBox *CBoxRede;
   TToolButton *ToolButton1;
   TToolButton *ToolButton2;
   TToolButton *ToolButton3;
   TToolButton *ToolButton4;
	TToolBar *ToolBar1;
	TToolButton *ButComprimento;
	TToolButton *ToolButton5;
	TEdit *EditCompr;
	TAction *ActionDefComprTrecho;
	TToolButton *ButZerarTaps;
	TAction *ActionZerarTaps;
	TTabSheet *TabSheetCarga;
	TListView *LViewCarga;
	TAction *ActionEdtCarga;
   TToolBar *ToolBarCargaOld;
	TToolButton *ButEdtCargas;
	TToolBar *ToolBarMain;
	TToolButton *ButFecha;
	TCheckBox *cbCodCarga;
	TToolButton *ToolButton6;
   TTabSheet *TabSheetModulos;
   TToolBar *ToolBar8;
   TToolButton *ToolButton7;
   TToolButton *ToolButton8;
   TActionList *ActionListModulo;
   TAction *ActionPastaIns;
   TAction *ActionPastaRet;
   TAction *ActionModuloIns;
   TAction *ActionModuloRet;
   TToolButton *ToolButton9;
   TToolButton *ToolButton10;
   TToolButton *ToolButton11;
   TTreeView *TViewGrupo;
   TAction *ActionCurva;
   TToolBar *ToolBar9;
   TToolButton *ToolButton12;
   TPanel *Panel1;
	TComboBox *CBoxFases;
	TPanel *Panel2;
	TComboBox *CBoxArranjo;
	TLabel *Label1;
	TTabSheet *TabSheetRegulador;
	TToolBar *ToolBar7;
	TToolButton *ToolButton13;
	TToolButton *ToolButton14;
	TToolButton *ToolButton15;
	TListView *LViewReg;
	TAction *ActionByPassTrue;
	TAction *ActionByPassFalse;
    TToolBar *ToolBar10;
    TToolButton *ToolButton16;
    TComboBox *CBoxFasePri;
    TAction *ActionMudaFasesPrimario;

   void __fastcall ActionFechaExecute(TObject *Sender);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall ActionDefRedeLigacaoExecute(TObject *Sender);
	void __fastcall ActionDefVnomBarraExecute(TObject *Sender);
   void __fastcall ActionDefArranjoTrechoExecute(TObject *Sender);
   void __fastcall ActionDefSnomTrafoExecute(TObject *Sender);
   void __fastcall ActionAbreChaveExecute(TObject *Sender);
   void __fastcall LViewDblClick(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall ActionFechaChaveExecute(TObject *Sender);
   void __fastcall PageControlChange(TObject *Sender);
	void __fastcall ButComprimentoClick(TObject *Sender);
	void __fastcall ActionZerarTapsExecute(TObject *Sender);
	void __fastcall ActionEdtCargaExecute(TObject *Sender);
   void __fastcall ActionPastaInsExecute(TObject *Sender);
   void __fastcall ActionPastaRetExecute(TObject *Sender);
   void __fastcall ActionModuloInsExecute(TObject *Sender);
   void __fastcall ActionModuloRetExecute(TObject *Sender);
   void __fastcall ActionCurvaExecute(TObject *Sender);
   AnsiString      __fastcall RemoveFaseString(AnsiString string);
	void __fastcall CBoxArranjoChange(TObject *Sender);
	void __fastcall ActionByPassFalseExecute(TObject *Sender);
	void __fastcall ActionByPassTrueExecute(TObject *Sender);
    void __fastcall ActionMudaFasesPrimarioExecute(TObject *Sender);
public:		// User declarations
	__fastcall  TFormEdtEqptos(TComponent* Owner, VTApl *apl, TList *lisEXT);
	__fastcall ~TFormEdtEqptos(void);

private:	//métodos
	void 		    __fastcall CBoxArranjoInicia(void);
	void 		    __fastcall CBoxFasesInicia(void);
	void 			__fastcall CBoxRedeInicia(void);
	void 			__fastcall FormIniciaPosicao(void);
	void 			__fastcall IniciaDadosBarra(void);
	void 			__fastcall IniciaDadosCarga(void);
	void 			__fastcall IniciaDadosChave(void);
	void 			__fastcall IniciaDadosLigacao(void);
	void            __fastcall IniciaDadosModulo(void);
	void            __fastcall IniciaDadosRegulador(void);
	void			__fastcall IniciaDadosTrafo(void);
	void 			__fastcall IniciaDadosTrecho(void);
	void 			__fastcall IniciaLisEqpto(TList *lisEQP, int eqpto_tipo);
	bool            __fastcall InsereDiretorio(AnsiString dir_name);
	bool            __fastcall InsereModuloConstrutivo(AnsiString dir_grupo, AnsiString arq_txt);
	void 			__fastcall Moldura(VTEqpto *eqpto=NULL);
	void 			__fastcall PreencheLisFamiliaArranjo(void);
	void 			__fastcall RedefineArranjoDosTrechos(VTArranjo *arranjo);
	void 			__fastcall RedefineCarga2IgualCarga1(VTCarga *carga1, VTCarga *carga2);
    void            __fastcall RedefineFasePriDosTrafos(int fase_tag);
	void 			__fastcall RedefineRedeDasLigacoes(VTRede *rede);
	void 			__fastcall RedefineSnomDosTrafos(double snom_kva);
	void 			__fastcall RedefineVnomDasBarras(double vnom_kv);
	void 			__fastcall RedefineComprTrechos(double compr);
	void			__fastcall SeparaArranjosEmFamilias(TList *lisArranjos, TList *lisFamilias);

private:	//objetos externos
   VTApl *apl;
   TList *lisEXT;
	TForm *FormEdtCar;

private:	//dados locais
   TList *lisEQP, *lisTMP;
   TList *lisFamiliaArranjo;

};

//---------------------------------------------------------------------------
#endif
//eof
