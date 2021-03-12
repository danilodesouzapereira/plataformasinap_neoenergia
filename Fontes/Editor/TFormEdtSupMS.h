//---------------------------------------------------------------------------
#ifndef TFormEdtSupMSH
#define TFormEdtSupMSH

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

//---------------------------------------------------------------------------
class VTApl;
class VTCurva;
class VTSuprimento;

//---------------------------------------------------------------------------
class TFormEdtSupMS : public TForm
{
__published:   // IDE-managed Components
   TToolBar *ToolBar1;
   TLabel *Label1;
   TEdit *EditCodigo;
   TLabel *labelVnom;
   TEdit *EditVpu;
   TImageList *ImageList1;
   TActionList *ActionList1;
   TAction *ActionCancela;
   TAction *ActionConfirma;
   TAction *ActionRetira;
   TToolButton *butConfirma;
   TToolButton *butCancela;
   TToolButton *butRetira;
   TPanel *Panel1;
   TPanel *Panel2;
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall ActionConfirmaExecute(TObject *Sender);
   void __fastcall ActionRetiraExecute(TObject *Sender);
   
public:      // User declarations
        __fastcall  TFormEdtSupMS(TComponent* Owner, VTApl *apl, VTSuprimento *suprimento, bool op_retira);
        __fastcall ~TFormEdtSupMS(void);

private: //métodos
   VTCurva*      __fastcall CriaCurva(void);
   void          __fastcall DefValoresDefaultSuprimento(void);
   VTSuprimento* __fastcall ExisteSuprimento(AnsiString codigo);
   void          __fastcall ExibeDadosSuprimento(void);
   void          __fastcall FormIniciaPosicao(void);
   void          __fastcall IniciaLisSuprimento(void);
   bool          __fastcall LeDadosSuprimento(void);

private: //objeto externos
   VTApl        *apl;
   VTSuprimento *suprimento;

private: //dados locais
   TList *lisEQP;

};

#endif
//---------------------------------------------------------------------------
//eof

