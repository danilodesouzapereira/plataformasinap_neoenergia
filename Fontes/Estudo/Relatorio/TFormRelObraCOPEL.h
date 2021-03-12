//---------------------------------------------------------------------------
#ifndef TFormRelObraCOPELH
#define TFormRelObraCOPELH
#include <System.Actions.hpp>
#include <System.Classes.hpp>
#include <System.ImageList.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.CheckLst.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.ToolWin.hpp>
//---------------------------------------------------------------------------
#include "TRelObraTO.h"
//---------------------------------------------------------------------------
#include <map>
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <CheckLst.hpp>
#include <ExtCtrls.hpp>
#include <Dialogs.hpp>
#include <System.Actions.hpp>
#include <System.ImageList.hpp>

//---------------------------------------------------------------------------
class VTArvore;
class VTEstudo;
class VTAlternativa;
class VTAtivo;
class VTApl;
class TRelObra;

//---------------------------------------------------------------------------
class TFormRelObraCOPEL : public TForm
{
__published:   // IDE-managed Components
   TToolBar *ToolBar;
   TToolButton *ButImpressao;
   TToolButton *BuFecha;
   TActionList *ActionList;
   TAction *ActionConfirma;
   TAction *ActionImprime;
   TImageList *ImageList;
   TToolButton *ToolButton1;
   TGroupBox *GBoxAlternativa;
   TCheckListBox *CLBoxAlternativa;
   TSplitter *Splitter;
   TListView *LView;
   TGroupBox *GBoxAtivo;
   TSaveDialog *SaveDialogExporta;
   void __fastcall ActionImprimeExecute(TObject *Sender);
   void __fastcall ActionConfirmaExecute(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall CLBoxAlternativaClick(TObject *Sender);
   void __fastcall CLBoxAlternativaClickCheck(TObject *Sender);
	void __fastcall LViewAdvancedCustomDrawItem(TCustomListView *Sender, TListItem *Item,
          TCustomDrawState State, TCustomDrawStage Stage, bool &DefaultDraw);



public:      // User declarations
   __fastcall  TFormRelObraCOPEL(TComponent* Owner, VTEstudo *estudo, VTApl* apl_owner);
   __fastcall ~TFormRelObraCOPEL(void);

private: //métodos
   VTAlternativa* __fastcall AlternativaSelecionada(void);
   void           __fastcall CLBoxAlternativaInicia(void);
//   void           __fastcall IniciaAlternativas(void);
   void           __fastcall LViewInicia(void);


private: //objetos externos
   VTEstudo *estudo;
   VTApl    *apl;

private: //dados locais
    TRelObra *relObra;
	int  ano_ini, ano_fim;

};

//---------------------------------------------------------------------------
#endif
//eof

