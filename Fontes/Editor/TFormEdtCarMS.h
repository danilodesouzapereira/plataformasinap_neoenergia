//---------------------------------------------------------------------------
#ifndef TFormEdtCarMSH
#define TFormEdtCarMSH

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
class VTCarga;
class VTCurva;
class VTDemanda;

//---------------------------------------------------------------------------
class TFormEdtCarMS : public TForm
{
__published:   // IDE-managed Components
   TToolBar *ToolBar1;
   TLabel *Label1;
   TEdit *EditCodigo;
   TLabel *LabelPotAtiva;
   TEdit *EditPotAtiva;
   TImageList *ImageList1;
   TActionList *ActionList1;
   TAction *ActionCancela;
   TAction *ActionConfirma;
   TAction *ActionRetira;
   TToolButton *butConfirma;
   TToolButton *butCancela;
   TToolButton *butRetira;
   TPanel *Panel1;
   TPanel *PanelPotAtiva;
   TPanel *Panel3;
   TPanel *Panel4;
   TLabel *Label3;
   TEdit *EditNumCons;
   TCheckBox *CheckBoxVip;
   TPanel *Panel2;
   TLabel *LabelPotReativa;
   TEdit *EditPotReativa;
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall ActionConfirmaExecute(TObject *Sender);
   void __fastcall ActionRetiraExecute(TObject *Sender);
   
public:      // User declarations
        __fastcall  TFormEdtCarMS(TComponent* Owner, VTApl *apl, VTCarga *carga, bool op_retira);
        __fastcall ~TFormEdtCarMS(void);

private: //métodos
   VTCurva* __fastcall CriaCurva(void);
   void     __fastcall DefValoresDefaultCarga(void);
   VTCarga* __fastcall ExisteCarga(AnsiString codigo);
   void     __fastcall ExibeDadosCarga(void);
   void     __fastcall FormIniciaPosicao(void);
   void     __fastcall IniciaLisCarga(void);
   bool     __fastcall LeDadosCarga(void);

private: //objeto externos
   VTApl        *apl;
   VTCarga      *carga;
   VTDemanda    *demanda;

private: //dados locais
   TList *lisCARGA;

};

#endif
//---------------------------------------------------------------------------
//eof

