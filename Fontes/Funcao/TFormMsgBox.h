//---------------------------------------------------------------------------
#ifndef TFormMsgBoxH
#define TFormMsgBoxH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Imaging.jpeg.hpp>
#include <Vcl.Graphics.hpp>

//---------------------------------------------------------------------------
class TFormMsgBox : public TForm
{
__published:	// IDE-managed Components
   TPanel *PanelSimNao;
   TButton *ButSim;
   TButton *ButNao;
   TButton *ButCancela;
   TPanel *PanelAviso;
   TImage *ImageAviso;
   TRichEdit *RichEdit;
   TPanel *PanelTexto;
   TPanel *PanelTop;
   TPanel *PanelOK;
   TButton *ButOK;
   TPanel *PanelErro;
   TImage *ImageErro;
   TPanel *PanelBottom;
   TPanel *PanelConfirma;
   TPanel *PanelEspaco;
   void __fastcall ButSimClick(TObject *Sender);
   void __fastcall ButNaoClick(TObject *Sender);
   void __fastcall ButCancelaClick(TObject *Sender);
   void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);

public:		// User declarations
        __fastcall  TFormMsgBox(TComponent* Owner);
        __fastcall ~TFormMsgBox(void);
   void __fastcall  Aviso(AnsiString txt);
   void __fastcall  Confirma(AnsiString txt1, AnsiString txt2, int buttons);
   void __fastcall  Erro(AnsiString txt);

private:	// User declarations
   //void __fastcall   AcertaTamanho(AnsiString txt);
   void __fastcall   AcertaTamanho(int max_width);
   int  __fastcall   LarguraMax(AnsiString txt);
};

//---------------------------------------------------------------------------
#endif
//eof
