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
#include <System.ImageList.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.CheckLst.hpp>

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
   TGroupBox *gboxDataAtivacao;
   TDateTimePicker *DateTimePickerAtivacao;
   TGroupBox *gboxDataInicio;
   TDateTimePicker *DateTimePickerInicio;
   TGroupBox *GBoxTaxaFalha;
   TLabel *Label1;
   TLabel *Label2;
   TEdit *EditTaxaInsercao;
   TEdit *EditTaxaSubstituicao;
   TPanel *Panel1;
   TPanel *Panel2;
   TGroupBox *gboxPNT;
   TPanel *Panel3;
   TLabel *Label3;
   TEdit *EditPNT;
   TPanel *panelExtras;
	TGroupBox *GroupBox3;
	TToolBar *ToolBar2;
	TBitBtn *butSelAll;
	TBitBtn *butSelNone;
	TCheckListBox *CLBoxRede;
	TAction *ActionRedeSelAll;
	TAction *ActionRedeSelNone;
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall ActionConfirmaExecute(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall CheckBoxQualidadeClick(TObject *Sender);
   void __fastcall EditTaxaSubstituicaoClick(TObject *Sender);
   void __fastcall EditPNTClick(TObject *Sender);
   void __fastcall EditPNTChange(TObject *Sender);
   void __fastcall FormResize(TObject *Sender);
   void __fastcall EditTaxaInsercaoClick(TObject *Sender);
	void __fastcall ActionRedeSelAllExecute(TObject *Sender);
	void __fastcall ActionRedeSelNoneExecute(TObject *Sender);

public:      // User declarations
              __fastcall  TFormObra(TComponent* Owner, VTApl *apl_owner, VTObra *obra);
              __fastcall  TFormObra(TComponent* Owner);
              __fastcall ~TFormObra(void);

protected:   // User declarations
   void       __fastcall CLBoxRedeInicia(void);
   TDateTime  __fastcall GetDataInicio(void);
   TDateTime  __fastcall GetDataAtivacao(void);
   AnsiString __fastcall GetNome(void);
   double     __fastcall GetReducaoPNT(void);
   double     __fastcall GetValorDouble(TEdit *Edit, AnsiString descricao);

protected: //métodos
   bool       __fastcall ExisteObra(TList *lisOBRA, AnsiString nome);
   AnsiString __fastcall NomeProximaObra(void);
   bool       __fastcall SelecionaArquivoObraTipica(void);
   bool       __fastcall ValidaDataInicio(void);

protected: //objetos externos
   VTApl         *apl;
   VTObra        *obra_ref;

protected: //objetos próprios
	double   valorPNT;
    TList    *lisTMP;
};

//---------------------------------------------------------------------------
#endif
//eof
