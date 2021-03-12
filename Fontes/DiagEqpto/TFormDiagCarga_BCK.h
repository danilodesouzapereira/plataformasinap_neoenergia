//---------------------------------------------------------------------------

#ifndef TFormDiagCargaH
#define TFormDiagCargaH
#include <System.Actions.hpp>
#include <System.Classes.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.ToolWin.hpp>
//---------------------------------------------------------------------------
class VTApl;
class TDiagCarga;
class VTDiagEqpto;
class DIAG_CARGA;
//---------------------------------------------------------------------------
enum colNUM {colNOME = 0, colLIMINF, colLIMSUP};
//---------------------------------------------------------------------------
class TFormDiagCarga : public TForm
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
   TListBox *lbTipoCarga;
   TAction *ActionDefault;
   void __fastcall lbTipoCargaClick(TObject *Sender);
   void __fastcall ActionDefaultExecute(TObject *Sender);
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall ActionConfirmaExecute(TObject *Sender);
   void __fastcall sgLimitesSetEditText(TObject *Sender, int ACol, int ARow, const UnicodeString Value);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);

private:	// Dados externos
   VTApl                *apl;
   //TList                *LisCfgDiagCarga;
   //TDiagEqptoCarga      *diagEqptoCarga;
   VTDiagEqpto      *diagEqpto;
   int                  ultimoTipoSelecionado;

private:	// Dados privados
   bool   editado;

private:	// User declarations
   void  __fastcall ListBoxInicia(void);
   void  __fastcall PreencheSGrids(DIAG_CARGA *diagCarga);
   bool  __fastcall SalvaSGrids(DIAG_CARGA *diagCarga);
   void  __fastcall SGridsEditado(bool editado);
   void  __fastcall SGridsInicia(void);
   bool  __fastcall VerificaLimites(double inf, double sup);
   bool  __fastcall VerificaLimitesSGrids(void);

public:  // User declarations
         __fastcall TFormDiagCarga(TComponent* Owner, VTApl *apl);
         __fastcall ~TFormDiagCarga(void);
   //void  __fastcall Inicia(TList *LisCfgDiagCarga);
   void  __fastcall Inicia(TDiagCarga *diagCarga);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormDiagCarga *FormDiagCarga;
//---------------------------------------------------------------------------
#endif
