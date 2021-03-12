//---------------------------------------------------------------------------
#ifndef TFormConfirmaMedicaoH
#define TFormConfirmaMedicaoH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>

class VTMedicoes;

//---------------------------------------------------------------------------
class TFormConfirmaMedicao : public TForm
{
__published:	// IDE-managed Components
   TGroupBox *GroupBox1;
   TPanel *Panel1;
   TGroupBox *GroupBox2;
   TRichEdit *RichEdit;
   TButton *ButSim;
   TButton *ButNao;
   TComboBox *CBoxArquivo;
   TLabel *Label1;
   void __fastcall ButSimClick(TObject *Sender);
   void __fastcall ButNaoClick(TObject *Sender);
   void __fastcall CBoxArquivoChange(TObject *Sender);

public:		// User declarations
		  __fastcall  TFormConfirmaMedicao(TComponent* Owner, VTMedicoes *medicoes);
        __fastcall ~TFormConfirmaMedicao(void);
   void __fastcall DefineArqMedicao(TStrings *files);
	void __fastcall DefineTipoMedicao(AnsiString tipo_medicao);

private:	// User declarations
	void __fastcall ExibeArquivoSelecionado(void);
	int  __fastcall TipoMedicaoSelecionado(void);

private:	//objetos externos
	TStrings *files;
	VTMedicoes *medicoes;
};

//---------------------------------------------------------------------------
#endif
//eof

