//---------------------------------------------------------------------------
#ifndef TFormBatchDCH
#define TFormBatchDCH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.ToolWin.hpp>
#include <System.ImageList.hpp>

//---------------------------------------------------------------------------
class TArqMedicao;
class TArqRede;
class TFormSelArqMedicao;
class TFormSelArqRede;
class VTArquivos;
class VTApl;
class VTAjusta;
class VTBatchDC;

//---------------------------------------------------------------------------
class TFormBatchDC : public TForm
{
__published:	// IDE-managed Components
	TToolBar *ToolBar;
	TToolButton *ButFecha;
	TToolButton *ToolButton5;
	TToolButton *ButCfgAjusteDemanda;
	TToolButton *ButExecuta;
	TToolButton *ToolButton1;
	TPageControl *PageControl;
	TTabSheet *TabSheetOpcao;
	TToolButton *ToolButton2;
	TPanel *PanelPeriodo;
	TRadioGroup *RadioGroupTopologia;
	TGroupBox *GBoxBaseDado;
	TBitBtn *BitBtn1;
	TBitBtn *ButSelBasePerda;
	TEdit *EditBaseHistorico;
	TGroupBox *GBoxAjusteDemanda;
	TCheckBox *CheckBoxAjusteDemanda;
	TGroupBox *GBoxPeriodo;
	TLabel *Label1;
	TLabel *Label2;
	TDateTimePicker *DateTimePickerIni;
	TDateTimePicker *DateTimePickerFim;
	TTabSheet *TabSheetArqRede;
	TTabSheet *TabSheetArqMedicao;
	TTabSheet *TabSheetProgresso;
	TListBox *ListBox;
	TActionList *ActionList;
	TAction *ActionExecutar;
	TAction *ActionFecha;
	TAction *ActionRemoverArqTxt;
	TAction *ActionCriaBaseHistorico;
	TAction *ActionSelBaseHistorico;
	TAction *ActionStop;
	TAction *ActionCfgAjusteDemanda;
	TAction *ActionClearMedicao;
	TAction *ActionClearRedes;
	TAction *ActionRemoverMedicao;
	TAction *ActionRemoverRede;
	TAction *ActionHelp;
	TImageList *ImageList;
	TOpenDialog *OpenDialog;
	TSaveDialog *SaveDialog;
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall ActionCfgAjusteDemandaExecute(TObject *Sender);
	void __fastcall ActionCriaBaseHistoricoExecute(TObject *Sender);
	void __fastcall ActionExecutarExecute(TObject *Sender);
	void __fastcall ActionFechaExecute(TObject *Sender);
	void __fastcall ActionHelpExecute(TObject *Sender);
	void __fastcall ActionSelBaseHistoricoExecute(TObject *Sender);
	void __fastcall CheckBoxAjusteDemandaClick(TObject *Sender);
	void __fastcall PageControlChanging(TObject *Sender, bool &AllowChange);
	void __fastcall RadioGroupTopologiaClick(TObject *Sender);
   void __fastcall PageControlChange(TObject *Sender);

public:		// User declarations
   __fastcall  TFormBatchDC(TComponent* Owner, VTApl *apl_owner);
	__fastcall ~TFormBatchDC(void);

private: //métodos
	void __fastcall CriaBaseAccessHistorico(void);
	void __fastcall DateTimePickerFimChange(TObject *Sender);
	void __fastcall DateTimePickerIniChange(TObject *Sender);
	bool __fastcall ExisteArvoreNaBase(int tipo, TDateTime data_ini, TDateTime data_fim);
	void __fastcall FormIniciaPosicao(void);
	void __fastcall IniciaArquivos(void);
	void __fastcall ReiniciaArquivos(void);
	void __fastcall SelecionaBaseHistorico(void);
	bool __fastcall ValidaArquivosSelecionados(void);
	bool __fastcall ValidaDatasSelecionadas(void);
	bool __fastcall ValidaLicenca(void);
	bool __fastcall VerificaOpcoes(void);

private:	//dados locais
   TFormSelArqMedicao *FormSelArqMedicao;
   TFormSelArqRede    *FormSelArqRede;
   VTApl              *apl;
   VTArquivos         *arquivos;
   VTAjusta           *ajusta;
   VTBatchDC          *batch_dc;
   bool               executando;
   struct{
         TTreeNode *rede, *medicao;
			}node_raiz;
};

//---------------------------------------------------------------------------
#endif
//eof

