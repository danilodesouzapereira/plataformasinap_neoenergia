//---------------------------------------------------------------------------
#ifndef TFormBatchPRH
#define TFormBatchPRH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.CheckLst.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.ToolWin.hpp>
#include <Vcl.ButtonGroup.hpp>
#include <Vcl.ActnCtrls.hpp>
#include <Vcl.ActnMan.hpp>
#include "Estrutura.h"
#include <System.ImageList.hpp>

//---------------------------------------------------------------------------
class TArqMedicao;
class TArqRede;
class TFormSelArqMedicao;
class TFormSelArqRede;
class VTArquivos;
class VTApl;
class VTAjusta;
class VTBatchPR;

//---------------------------------------------------------------------------
class TFormBatchPR : public TForm
{
__published:	// IDE-managed Components
   TActionList *ActionList;
   TAction *ActionExecutar;
   TAction *ActionFecha;
   TAction *ActionRemoverArqTxt;
   TAction *ActionStop;
   TAction *ActionClearMedicao;
   TAction *ActionCfgPerda;
   TAction *ActionClearRedes;
   TAction *ActionRemoverMedicao;
   TAction *ActionSelBaseHistorico;
   TAction *ActionCriaBaseHistorico;
   TImageList *ImageList;
   TOpenDialog *OpenDialog;
   TToolBar *ToolBar;
   TToolButton *ButFecha;
   TToolButton *ToolButton5;
   TToolButton *ButCfgPerda;
   TAction *ActionRemoverRede;
   TSaveDialog *SaveDialog;
   TToolButton *ButExecuta;
   TToolButton *ToolButton1;
   TAction *ActionHelp;
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
   TListBox *LBoxLogRede;
   TListBox *LBoxLogPasso;
   TSplitter *Splitter;
   void __fastcall PageControlChanging(TObject *Sender, bool &AllowChange);
   void __fastcall DateTimePickerIniChange(TObject *Sender);
   void __fastcall DateTimePickerFimChange(TObject *Sender);
   void __fastcall CheckBoxAjusteDemandaClick(TObject *Sender);
   void __fastcall ActionCriaBaseHistoricoExecute(TObject *Sender);
   void __fastcall ActionSelBaseHistoricoExecute(TObject *Sender);
   void __fastcall ActionCfgPerdaExecute(TObject *Sender);
   void __fastcall ActionExecutarExecute(TObject *Sender);
   void __fastcall RadioGroupTopologiaClick(TObject *Sender);
   void __fastcall ActionHelpExecute(TObject *Sender);
   void __fastcall ActionFechaExecute(TObject *Sender);
   void __fastcall PageControlChange(TObject *Sender);

public:		// User declarations
   __fastcall  TFormBatchPR(TComponent* Owner, VTApl *apl_owner);
   __fastcall ~TFormBatchPR(void);

private:	//métodos
   void       __fastcall CriaBaseAccessHistorico(void);
   bool       __fastcall ExisteArvoreNaBase(int tipo, TDateTime data_ini, TDateTime data_fim);
	void       __fastcall FormIniciaPosicao(void);
	void       __fastcall IniciaArquivos(void);
   void       __fastcall ReiniciaArquivos(void);
   void       __fastcall SelecionaBaseHistorico(void);
   bool       __fastcall UsuarioConfigurouPerdas(void);
   bool       __fastcall ValidaArquivosSelecionados(void);
   bool       __fastcall ValidaDatasSelecionadas(void);
   bool       __fastcall ValidaLicenca(void);
   bool       __fastcall VerificaOpcoes(void);

private:	//objetos externos

private:	//dados locais
   TFormSelArqMedicao *FormSelArqMedicao;
   TFormSelArqRede    *FormSelArqRede;
   VTApl              *apl;
   VTArquivos         *arquivos;
   VTAjusta           *ajusta;
   VTBatchPR          *batch_pr;
   bool               executando;
   struct{
         TTreeNode *rede, *medicao;
         }node_raiz;
};

//---------------------------------------------------------------------------
#endif
//eof
