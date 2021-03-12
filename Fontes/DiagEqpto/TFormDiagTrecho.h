//---------------------------------------------------------------------------

#ifndef TFormDiagTrechoH
#define TFormDiagTrechoH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.ToolWin.hpp>
#include <System.ImageList.hpp>
//---------------------------------------------------------------------------
class VTApl;
class VTDiagEqpto;
class TDiagTrecho;
class DIAG_TRECHO;
//---------------------------------------------------------------------------
enum colNUM {colNOME = 0, colLIMINF, colLIMSUP};
//---------------------------------------------------------------------------
class TFormDiagTrecho : public TForm
{
__published:	// IDE-managed Components
   TActionList *ActionList;
   TAction *ActionCancela;
   TAction *ActionConfirma;
   TImageList *ImageList;
   TToolBar *ToolBar1;
   TToolButton *butConfirma;
   TToolButton *butCancela;
   TGroupBox *GroupBox1;
   TGroupBox *GroupBox2;
   TSplitter *Splitter1;
   TStringGrid *sgLimites;
   TToolButton *ToolButton1;
   TToolButton *ToolButton2;
   TGroupBox *GroupBox3;
   TStringGrid *sgArranjo;
   TListBox *lbTipoRede;
   TAction *ActionDefault;
   void __fastcall lbTipoRedeClick(TObject *Sender);
   void __fastcall ActionDefaultExecute(TObject *Sender);
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall ActionConfirmaExecute(TObject *Sender);
   void __fastcall sgLimitesSetEditText(TObject *Sender, int ACol, int ARow, const UnicodeString Value);
   void __fastcall sgArranjoSetEditText(TObject *Sender, int ACol, int ARow, const UnicodeString Value);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);

private:	// Dados externos
   VTApl           *apl;
   //TList                *LisCfgDiagTrecho;
   //TDiagEqptoTrecho     *diagEqptoTrecho;
   VTDiagEqpto     *diagEqpto;
   int             ultimoTipoSelecionado;
   TFormDiagTrecho *formDiagTrecho;

private:	// Dados privados
   bool   editado;

private:	// User declarations
   void  __fastcall ListBoxInicia(void);
   void  __fastcall PreencheSGrids(DIAG_TRECHO *cfgDiagTrecho);
   bool  __fastcall SalvaSGrids(DIAG_TRECHO *cfgDiagTrecho);
   void  __fastcall SGridsEditado(bool editado);
   void  __fastcall SGridsInicia(void);
   bool  __fastcall VerificaLimites(double inf, double sup);
   bool  __fastcall VerificaLimitesSGrids(void);

public:  // User declarations
         __fastcall TFormDiagTrecho(TComponent* Owner, VTApl *apl);
         __fastcall ~TFormDiagTrecho(void);
   //void  __fastcall Inicia(TList *LisCfgDiagTrecho);
   void  __fastcall Inicia(TDiagTrecho *diagTrecho);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormDiagTrecho *FormDiagTrecho;
//---------------------------------------------------------------------------
#endif
