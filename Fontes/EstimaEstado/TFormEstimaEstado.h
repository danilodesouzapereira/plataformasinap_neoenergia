//---------------------------------------------------------------------------
#ifndef TFormEstimaEstadoH
#define TFormEstimaEstadoH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "TFormEdt.h"
#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Graphics.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.ToolWin.hpp>
#include <System.ImageList.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTEditor;
class VTEstimaEstado;
class VTMedidor;

//---------------------------------------------------------------------------
class TFormEstimaEstado : public TFormEdt
{
__published:	// IDE-managed Components
   TToolBar *ToolBar1;
   TToolButton *ButFecha;
   TToolButton *ToolButton1;
   TToolButton *ButExecuta;
   TToolButton *ButConfigura;
   TToolButton *ButTeste;
   TToolButton *ButRetAUG;
   TAction *ActionCfgParametro;
   TAction *ActionEstimaEstado;
   TAction *ActionFecha;
   TGroupBox *GroupBox1;
   TRichEdit *RichEdit;
   TGroupBox *GroupBox2;
   TListView *LView;
   TToolButton *ButEdtMedidor;
   TAction *ActionEdtMedicao;
   TToolButton *ToolButton2;
   TAction *ActionRedeChanged;
   TToolButton *ButEquivalente;
   TAction *ActionRedutor;
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall ActionCfgParametroExecute(TObject *Sender);
   void __fastcall ActionEstimaEstadoExecute(TObject *Sender);
   void __fastcall ActionFechaExecute(TObject *Sender);
   void __fastcall ButTesteClick(TObject *Sender);
   void __fastcall ButRetAUGClick(TObject *Sender);
   void __fastcall LViewColumnClick(TObject *Sender, TListColumn *Column);
   void __fastcall LViewCompare(TObject *Sender, TListItem *Item1, TListItem *Item2,
          int Data, int &Compare);
   void __fastcall ActionEdtExecute(TObject *Sender);
   void __fastcall ActionEdtMedicaoExecute(TObject *Sender);
   void __fastcall ActionRedeChangedExecute(TObject *Sender);
   void __fastcall ActionRedutorExecute(TObject *Sender);

public:		// User declarations
   __fastcall  TFormEstimaEstado(TComponent* Owner, VTApl *apl_owner, TWinControl *parent);
   __fastcall ~TFormEstimaEstado(void);

private:	//métodos
   void __fastcall ExibeMedidores(bool visible);
   void __fastcall ExibeOpcoes(void);
   void __fastcall LViewInicia(void);

private:	//objetos externos

private:	//dados locais
   TList          *lisEQP;
   VTApl          *apl;
   VTEditor       *editor;
   VTEstimaEstado *estima;
   struct{//controle de ordenação de ListView
         TListColumn *column;
         bool         ascendente;
         }LViewSort;
};

//---------------------------------------------------------------------------
#endif
//eof

