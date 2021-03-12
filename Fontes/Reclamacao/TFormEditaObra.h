//---------------------------------------------------------------------------
#ifndef TFormEditaObraH
#define TFormEditaObraH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <Dialogs.hpp>
#include <System.Actions.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTObra;

//---------------------------------------------------------------------------
class TFormEditaObra : public TForm
{
__published:   // IDE-managed Components
   TActionList *ActionList1;
   TAction *ActionCancela;
   TAction *ActionConfirma;
   TImageList *ImageList1;
   TToolBar *ToolBar1;
   TToolButton *butConfirma;
   TToolButton *butCancela;
   TGroupBox *GroupBox1;
   TEdit *EditNome;
   TGroupBox *GroupBox4;
   TDateTimePicker *DateTimePickerInicio;
   TOpenDialog *OpenDialog;
   TGroupBox *GroupBox2;
   TDateTimePicker *DateTimePickerAtivacao;
   TGroupBox *GroupBox3;
   TEdit *EditCusto;
	TToolButton *ToolButton1;
	TAction *ActionSelObraTipica;
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall ActionConfirmaExecute(TObject *Sender);
	void __fastcall ActionSelObraTipicaExecute(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormShow(TObject *Sender);

public:      // User declarations
              __fastcall  TFormEditaObra(TComponent* Owner, VTApl *apl_owner, VTObra *obra);
              __fastcall ~TFormEditaObra(void);

protected:   // User declarations
   bool       __fastcall GetCusto(double &custo);
   TDateTime  __fastcall GetDataInicio(void);
   TDateTime  __fastcall GetDataAtivacao(void);
   AnsiString __fastcall GetNome(void);

private: //métodos
   bool       __fastcall ExisteObra(TList *lisOBRA, AnsiString nome);
   AnsiString __fastcall NomeProximaObra(void);
   bool       __fastcall SelecionaArquivoObraTipica(void);
   bool       __fastcall ValidaDataInicio(void);

private: //objetos externos
   VTApl         *apl;
   VTObra        *obra_ref;
};

//---------------------------------------------------------------------------
#endif
//eof
