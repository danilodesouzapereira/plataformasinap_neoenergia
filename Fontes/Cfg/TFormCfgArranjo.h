//---------------------------------------------------------------------------
#ifndef TFormCfgArranjoH
#define TFormCfgArranjoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <Dialogs.hpp>
#include <System.Actions.hpp>
#include <System.ImageList.hpp>

//---------------------------------------------------------------------------
class TAplCfg;
class VTApl;
class VTArranjo;
class VTArranjos;
class VTCabo;
class VTSuporte;

//---------------------------------------------------------------------------
class TFormCfgArranjo : public TForm
{
__published:	// IDE-managed Components
   TPageControl *PageControl;
   TToolBar *ToolBar;
   TActionList *ActionList;
   TTabSheet *TabSheetCabo;
   TTabSheet *TabSheetSuporte;
   TTabSheet *TabSheetArranjo;
   TListView *LViewCabo;
   TStatusBar *StatusBarCabo;
   TListView *LViewSuporte;
   TStatusBar *StatusBarSuporte;
   TListView *LViewArranjo;
   TAction *ActionEdtCabo;
   TAction *ActionEdtArranjo;
   TAction *ActionEdtSuporte;
   TAction *ActionInsArranjo;
   TAction *ActionInsCabo;
   TAction *ActionInsSuporte;
   TToolButton *ButIns;
   TAction *ActionConfirma;
   TImageList *ImageList;
   TToolButton *ToolButton1;
   TToolButton *ButConfirma;
   TToolButton *ButCancela;
   TAction *ActionCancela;
   TStatusBar *StatusBarArranjo;
   TToolButton *ButEdt;
   TToolButton *ButRet;
   TAction *ActionRetArranjo;
   TAction *ActionRetCabo;
   TAction *ActionRetSuporte;
   TToolButton *ToolButton2;
   TAction *ActionSalvaOracle;
   TToolButton *ButImportaXML;
   TOpenDialog *OpenDialog;
   TAction *ActionImportaXML;
   TAction *ActionExportaXML;
   TAction *ActionImportaTXT;
   TAction *ActionExportaTXT;
   TSaveDialog *SaveDialog;
   TToolButton *ButExportaXML;
   TToolButton *ToolButton3;
   TToolButton *ButImportaTXT;
   TToolButton *ButExportaTXT;
	TToolButton *ButZ0Z1;
	TAction *ActionVisualizarZ0Z1;
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall ActionEdtArranjoExecute(TObject *Sender);
   void __fastcall ActionEdtCaboExecute(TObject *Sender);
   void __fastcall ActionEdtSuporteExecute(TObject *Sender);
   void __fastcall ActionInsArranjoExecute(TObject *Sender);
   void __fastcall ActionInsCaboExecute(TObject *Sender);
   void __fastcall ActionInsSuporteExecute(TObject *Sender);
   void __fastcall PageControlChange(TObject *Sender);
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall ActionConfirmaExecute(TObject *Sender);
   void __fastcall ActionRetArranjoExecute(TObject *Sender);
   void __fastcall ActionRetCaboExecute(TObject *Sender);
   void __fastcall ActionRetSuporteExecute(TObject *Sender);
   void __fastcall LViewArranjoCompare(TObject *Sender, TListItem *Item1,
          TListItem *Item2, int Data, int &Compare);
   void __fastcall LViewArranjoColumnClick(TObject *Sender,
          TListColumn *Column);
   void __fastcall LViewCaboCompare(TObject *Sender, TListItem *Item1,
          TListItem *Item2, int Data, int &Compare);
   void __fastcall LViewCaboColumnClick(TObject *Sender,
          TListColumn *Column);
   void __fastcall ActionExportaXMLExecute(TObject *Sender);
   void __fastcall ActionExportaTXTExecute(TObject *Sender);
   void __fastcall ActionImportaTXTExecute(TObject *Sender);
   void __fastcall ActionImportaXMLExecute(TObject *Sender);
	void __fastcall ActionVisualizarZ0Z1Execute(TObject *Sender);

public:		// User declarations
   __fastcall  TFormCfgArranjo(TForm *form_owner, VTApl *apl_owner, VTApl *apl_child);
   __fastcall ~TFormCfgArranjo(void);

private:	//métodos
   bool __fastcall ArranjoAlterado(VTArranjo *arranjo);
   bool __fastcall ArranjoUtilizaCabo(VTArranjo *arranjo, VTCabo *cabo);
   void __fastcall AtualizaBaseDado(void);
   void __fastcall CalculaMatZ(void);
   int  __fastcall CompareNum(AnsiString S1, AnsiString S2);
   void __fastcall DefineStatus(TList *lisEQP, int bit_stt, bool enabled);
   void __fastcall ExibeDados(void);
   void __fastcall ExibeDadosArranjo(void);
   void __fastcall ExibeDadosCabo(void);
   void __fastcall ExibeDadosSuporte(void);
   int  __fastcall ExisteArranjo(VTSuporte *suporte, TList *lisEXT);
   bool __fastcall ExistePlanejamento(void);
   void __fastcall ExportaArranjosCabosSuporte(AnsiString path);
   AnsiString __fastcall LinhaCsvHeader(TListView *lview);
   AnsiString __fastcall LinhaCsvListItem(TListItem *item);
   //void __fastcall VerificaModulos(void);

private: //externo
   VTApl *apl_child;

private:	//dados locais
   TList       *lisEQP;
   TAplCfg     *apl;
   VTArranjos  *arranjos;
   struct   {//estrutura p/ ordenar LView
            int    ColumnIndex;
            bool   OrdemCrescente;
            }ListViewSort;
};

//---------------------------------------------------------------------------
#endif
//eof
