//---------------------------------------------------------------------------

#ifndef TFormEdtFaseTrechoH
#define TFormEdtFaseTrechoH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.ToolWin.hpp>
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------
class VTApl;
class VTArranjo;
class VTCabo;
class VTSuporte;
class VTRede;
struct strIMP;

//---------------------------------------------------------------------------
class TFormEdtFaseTrecho : public TForm
{
__published:	// IDE-managed Components
   TToolBar *ToolBar2;
   TToolButton *ToolButton1;
   TImageList *ImageList1;
   TActionList *ActionList2;
   TAction *ActionFecha;
   TTreeView *TViewRedes;
   TPanel *Panel1;
   TGroupBox *GroupBox1;
   TSplitter *Splitter1;
   TRadioGroup *RadioGroupNeutro;
   TGroupBox *GroupBox2;
   TComboBox *CBoxFases;
   TToolButton *ToolButton3;
   TAction *ActionAplicar;
   void __fastcall ActionFechaExecute(TObject *Sender);
   void __fastcall ActionAplicarExecute(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall TViewRedesClick(TObject *Sender);
   void __fastcall TViewRedesDblClick(TObject *Sender);

public:		// User declarations
   __fastcall TFormEdtFaseTrecho(TComponent* Owner, VTApl *apl_owner);
   __fastcall ~TFormEdtFaseTrecho(void);

private:	// User declarations
   void        __fastcall AlteraFases(TList  *lisTRECHO);
   void        __fastcall AlteraNeutro(TList  *lisTRECHO);
   void        __fastcall CBoxFasesInicia(void);
   VTArranjo*  __fastcall CriaArranjo(VTSuporte *suporte, VTCabo *caboA,
                                      VTCabo *caboB, VTCabo *caboC, VTCabo *caboN);
   VTArranjo*  __fastcall CriaArranjo(VTCabo *cabo, int fase_nova);
   VTArranjo*  __fastcall CriaArranjo(strIMP &z0, strIMP &z1, double c0, double c1, int fase_nova);
   VTArranjo*  __fastcall ExisteArranjo(VTArranjo *original, int fase_tag);
   VTRede*     __fastcall ExisteRede(AnsiString cod_rede, TList *lisREDE);
   void        __fastcall FormIniciaPosicao(void);
   void        __fastcall TViewRedesInicia(void);
   VTRede*     __fastcall TViewRedeSelecionada(void);

private: //obj externo
   VTApl *apl;

private: //obj local
   VTArranjo *arr_original;

};

//---------------------------------------------------------------------------
#endif
