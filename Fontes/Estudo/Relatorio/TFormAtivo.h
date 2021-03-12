//---------------------------------------------------------------------------
#ifndef TFormAtivoH
#define TFormAtivoH

//---------------------------------------------------------------------------
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
class VTEstudo;
class VTAlternativa;
class VTAtivo;
class VTApl;

//---------------------------------------------------------------------------
class TFormAtivo : public TForm
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

public:      // User declarations
   __fastcall  TFormAtivo(TComponent* Owner, VTEstudo *estudo, VTApl* apl_owner);
   __fastcall ~TFormAtivo(void);

private: //métodos
   VTAlternativa* __fastcall AlternativaSelecionada(void);
   void           __fastcall CLBoxAlternativaInicia(void);
   void           __fastcall IniciaAlternativas(void);
   void           __fastcall LViewInicia(void);

private: //objetos externos
   VTEstudo *estudo;
   VTApl    *apl;

private: //dados locais
   int  ano_ini, ano_fim;

};

//---------------------------------------------------------------------------
#endif
//eof

