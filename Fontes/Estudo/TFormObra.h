//---------------------------------------------------------------------------
#ifndef TFormObraH
#define TFormObraH
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
#include <Vcl.ExtCtrls.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTObra;

//---------------------------------------------------------------------------
class TFormObra : public TForm
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
   TOpenDialog *OpenDialog;
   TPanel *PanelTipo;
   TGroupBox *GBoxTipoObra;
   TComboBox *CBoxTipo;
   TGroupBox *GBoxQualidade;
   TCheckBox *CheckBoxQualidade;
   TPanel *PanelDatas;
   TGroupBox *GroupBox2;
   TDateTimePicker *DateTimePickerAtivacao;
   TGroupBox *GroupBox4;
   TDateTimePicker *DateTimePickerInicio;
   TGroupBox *GBoxTaxaFalha;
   TLabel *Label1;
   TLabel *Label2;
   TEdit *EditTaxaInsercao;
   TEdit *EditTaxaSubstituicao;
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall ActionConfirmaExecute(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall CheckBoxQualidadeClick(TObject *Sender);

public:      // User declarations
              __fastcall  TFormObra(TComponent* Owner, VTApl *apl_owner, VTObra *obra);
              __fastcall ~TFormObra(void);

protected:   // User declarations
   TDateTime  __fastcall GetDataInicio(void);
   TDateTime  __fastcall GetDataAtivacao(void);
   AnsiString __fastcall GetNome(void);
   double     __fastcall GetValorDouble(TEdit *Edit, AnsiString descricao);

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
