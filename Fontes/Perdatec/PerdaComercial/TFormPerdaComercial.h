//---------------------------------------------------------------------------
#ifndef TFormPerdaComercialH
#define TFormPerdaComercialH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <ActnList.hpp>
#include <Grids.hpp>
#include <ToolWin.hpp>
#include <Menus.hpp>
#include <Dialogs.hpp>
#include <ImgList.hpp>
#include <Buttons.hpp>
#include <System.Actions.hpp>
#include <Vcl.ExtDlgs.hpp>
#include <System.ImageList.hpp>

//---------------------------------------------------------------------------
class TFormPerda;
class VTApl;

//---------------------------------------------------------------------------
class TFormPerdaComercial : public TForm
{
__published:   // IDE-managed Components
   TActionList *ActionList;
   TToolBar *ToolBar;
   TToolButton *ToolButton1;
   TAction *ActionFechar;
   TImageList *ImageList;
   TListView *LViewMedicao;
   TAction *ActionSalvaRelatorio;
   TToolButton *ButExporta;
   TSaveDialog *SaveDialog;
   void __fastcall FormShow(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall ActionFecharExecute(TObject *Sender);
   void __fastcall ActionSalvaRelatorioExecute(TObject *Sender);
   void __fastcall LViewMedicaoColumnClick(TObject *Sender, TListColumn *Column);
   void __fastcall LViewMedicaoCompare(TObject *Sender, TListItem *Item1, TListItem *Item2,
          int Data, int &Compare);

public:  //property
   __property TDateTime Date = {read=PD.date, write=PD.date};

public:      // User declarations
        __fastcall  TFormPerdaComercial(TComponent* Owner, VTApl *apl_owner, TWinControl *parent);
        __fastcall ~TFormPerdaComercial(void);
   void __fastcall  Inicia(void);

private:      //métodos
   void __fastcall FormIniciaPosicao(void);
   bool __fastcall LeArquivoMedicao(void);
   void __fastcall LViewMedicaoInicia(void);
   bool __fastcall ValidaMedicao(void);

private:   //objetos externos
   VTApl       *apl;

private:   //dados acessados via proeprty
   struct{
		 TDateTime date;
		 }PD;

private:   //dados locais
   struct{//controle de ordenação de ListView
		 TListColumn *column;
		 bool         ascendente;
         }LViewSort;
};

//---------------------------------------------------------------------------
#endif
//eof
