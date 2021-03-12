//---------------------------------------------------------------------------
#ifndef TFormEdtZrefH
#define TFormEdtZrefH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <DBCtrls.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
#include <ActnList.hpp>
#include <ImgList.hpp>
#include <System.Actions.hpp>

//---------------------------------------------------------------------------
class VTYref;
class VTApl;
//class TFormEdtPlan;

//---------------------------------------------------------------------------
class TFormEdtZref : public TForm
{
__published:	// IDE-managed Components
   TLabel *LabelX1;
   TLabel *LabelR1;
   TLabel *Label1;
   TEdit *edtR1;
   TEdit *edtX1;
   TEdit *EditCodigo;
   TActionList *ActionList;
   TAction *ActionCancela;
   TAction *ActionConfirma;
   TAction *ActionRetira;
   TAction *ActionSelFoto;
   TAction *ActionSelPadrao;
   TImageList *ImageList;
   TToolBar *ToolBar;
   TToolButton *butConfirma;
   TToolButton *butCancela;
   TToolButton *butRetira;
   TPanel *Panel1;
   TPanel *Panel2;
   void __fastcall butCancelaClick(TObject *Sender);
   void __fastcall butConfirmaClick(TObject *Sender);
   void __fastcall butRetiraClick(TObject *Sender);
   void __fastcall FormShow(TObject *Sender);

public:		// métodos
        __fastcall TFormEdtZref(TComponent* Owner, VTApl *apl, VTYref* yref);
        __fastcall ~TFormEdtZref(void);
   void __fastcall OpcaoRetira(bool enabled);

private:    //métodos
   void __fastcall DefValoresDefaultZref(void);
   void __fastcall ExibeDadosZref(void);
   void __fastcall FormIniciaPosicao(void);
   void __fastcall IniciaParteFixaTela(void);
   bool __fastcall LeDadosZref(void);

private:    // atributos
   //TFormEdtPlan *FormEdtPlan;
   VTYref       *yref;
   VTApl        *apl;
};

#endif
//---------------------------------------------------------------------------
//eof
