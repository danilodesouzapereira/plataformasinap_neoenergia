//---------------------------------------------------------------------------
#ifndef TFormBalancoH
#define TFormBalancoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <Dialogs.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <ExtCtrls.hpp>

//---------------------------------------------------------------------------
class VTApl;

//---------------------------------------------------------------------------
class TFormBalanco : public TForm
{
__published:   // IDE-managed Components
   TToolBar *ToolBar;
   TToolButton *ButFecha;
   TToolButton *ButExporta;
   TImageList *ImageList;
   TActionList *ActionList;
   TAction *ActionExporta;
   TAction *ActionFecha;
   TSaveDialog *SaveDialog;
   TGroupBox *GBoxRede;
   TGroupBox *GBoxResumo;
   TPanel *Panel1;
   TGroupBox *GBoxData;
   TGroupBox *GBoxEnergia;
   TGroupBox *GBoxPerda;
   TListView *LViewPerda;
   TListView *ListView1;
   TListView *ListView2;
   void __fastcall ActionExportaExecute(TObject *Sender);
   void __fastcall ActionFechaExecute(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);

public:      // User declarations
   __fastcall  TFormBalanco(TComponent *Owner, VTApl *apl_owner, TAction *ActionOnClose);
   __fastcall ~TFormBalanco(void);

private:   //objetos externos
   TAction *ActionOnClose;
   VTApl   *apl;

private:   //dados locais
   struct   {
            TColumn *column;
            bool     ascendente;
            }ordenacao;
};

//---------------------------------------------------------------------------
#endif
//eof
