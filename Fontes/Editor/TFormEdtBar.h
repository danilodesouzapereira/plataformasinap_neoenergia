//---------------------------------------------------------------------------
#ifndef TFormEdtBarH
#define TFormEdtBarH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
#include <ActnList.hpp>
#include <ImgList.hpp>
#include <System.Actions.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <System.ImageList.hpp>

//---------------------------------------------------------------------------
class VTBarra;
class VTApl;

//---------------------------------------------------------------------------
class TFormEdtBar : public TForm
{
__published:   // IDE-managed Components
   TToolBar *ToolBar1;
   TLabel *Label1;
   TEdit *EditCodigo;
   TLabel *labelVnom;
   TEdit *EditVnom;
   TImageList *ImageList1;
   TActionList *ActionList1;
   TAction *ActionCancela;
   TAction *ActionConfirma;
   TAction *ActionRetira;
   TToolButton *butConfirma;
   TToolButton *butCancela;
   TToolButton *butRetira;
   TGroupBox *GroupBox1;
   TEdit *EditCoordX;
   TEdit *EditCoordY;
   TLabel *Label2;
   TLabel *Label3;
   TStatusBar *StatusBar;
   TPanel *Panel1;
   TPanel *Panel2;
   TPanel *Panel3;
   TPanel *Panel4;
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall ActionConfirmaExecute(TObject *Sender);
   void __fastcall ActionRetiraExecute(TObject *Sender);
   
public:      // User declarations
        __fastcall  TFormEdtBar(TComponent* Owner, VTApl *apl, VTBarra *pbarra, bool op_retira);
        __fastcall ~TFormEdtBar(void);

private: //métodos
   bool __fastcall ExisteCorte(VTBarra *barra);
   void __fastcall DefValoresDefaultBarra(void);
   void __fastcall ExibeDadosBarra(void);
   bool __fastcall ExisteBarraSemCoordUtm(void);
   void __fastcall FormIniciaPosicao(void);
   void __fastcall IniciaParteFixaTela(void);
   bool __fastcall LeDadosBarra(void);

private: //dados
   VTApl        *apl;
   VTBarra      *pbarra;
};

#endif
//---------------------------------------------------------------------------
//eof

