// ---------------------------------------------------------------------------
#ifndef TFormEditaObraH
#define TFormEditaObraH
// ---------------------------------------------------------------------------
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

// ---------------------------------------------------------------------------
class VTApl;
class VTObra;
class VTPadrao;
// ---------------------------------------------------------------------------
class TFormEditaObra : public TForm
{
__published: // IDE-managed Components
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
	TPanel *PanelDatas;
	TGroupBox *GroupBoxAtiv;
	TDateTimePicker *DateTimePickerAtivacao;
	TGroupBox *GroupBoxIni;
	TDateTimePicker *DateTimePickerInicio;
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
	void __fastcall ActionRedeSelAllExecute(TObject *Sender);
	void __fastcall ActionRedeSelNoneExecute(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);

public: // User declarations
	__fastcall TFormEditaObra(TComponent* Owner, VTApl *apl_owner, VTObra *obra);
	__fastcall ~TFormEditaObra(void);

protected: // User declarations
	TDateTime __fastcall GetDataInicio(void);
	TDateTime __fastcall GetDataAtivacao(void);
	AnsiString __fastcall GetNome(void);
//	VTPadrao*  __fastcall GetPadrao(bool insere);
	int        __fastcall GetTipo(void);
	double __fastcall GetValorDouble(TEdit *Edit, AnsiString descricao);

private: // métodos
    void __fastcall CBoxTipoInicia(void);
	void __fastcall CLBoxRedeInicia(void);
	bool __fastcall ExisteObra(TList *lisOBRA, AnsiString nome);
	AnsiString __fastcall NomeProximaObra(void);
	bool __fastcall NomeObraExist(TList *lisOBRA, AnsiString nome);
	bool __fastcall SelecionaArquivoObraTipica(void);
	bool __fastcall ValidaDataInicio(void);

	// objetos externos
	VTApl *apl;
	VTObra *obra_ref;
	// objetos internos
	TList *lisTMP;
};

// ---------------------------------------------------------------------------
#endif
// eof
