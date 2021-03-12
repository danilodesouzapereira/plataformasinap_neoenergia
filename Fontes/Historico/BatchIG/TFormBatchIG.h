//---------------------------------------------------------------------------
#ifndef TFormBatchIGH
#define TFormBatchIGH

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
#include <Fontes\ImportaRedeGIS\VTImportaRedeGIS.h>
//---------------------------------------------------------------------------
class TArqMedicao;
class TArqRede;
class TFormSelArqMedicao;
class TFormSelArqRede;
class VTArquivos;
class VTAjusta;
class VTApl;
class VTBatchIG;

//---------------------------------------------------------------------------
class TFormBatchIG : public TForm
{
__published:	// IDE-managed Components
   TActionList *ActionList;
   TAction *ActionExecutar;
   TAction *ActionFecha;
   TAction *ActionSelDiretorio;
   TAction *ActionCriaDiretorio;
   TImageList *ImageList;
   TOpenDialog *OpenDialog;
   TToolBar *ToolBar;
   TToolButton *ButFecha;
   TToolButton *ToolButton5;
   TToolButton *ToolButton2;
   TGroupBox *GBoxPeriodo;
   TDateTimePicker *DateTimePickerIni;
   TLabel *Label1;
   TLabel *Label2;
   TDateTimePicker *DateTimePickerFim;
   TRadioGroup *RadioGroupBaseDado;
   TPanel *PanelPeriodo;
   TPageControl *PageControl;
   TTabSheet *TabSheetOpcao;
   TTabSheet *TabSheetArqRede;
   TGroupBox *GBoxAjusteDemanda;
   TCheckBox *CheckBoxAjusteDemanda;
   TSaveDialog *SaveDialog;
   TPanel *PanelArqRede;
   TTabSheet *TabSheetProgresso;
   TToolButton *ButExecuta;
   TListBox *ListBox;
   TToolButton *ButCfgAjusteDemanda;
   TAction *ActionCfgAjusteDemanda;
   TTabSheet *TabSheetArqMedicao;
	TToolButton *ToolButton1;
	TAction *ActionCfgImporta;
	TToolButton *ToolButton3;
	TAction *ActionHelp;
   void __fastcall PageControlChanging(TObject *Sender, bool &AllowChange);
   void __fastcall DateTimePickerIniChange(TObject *Sender);
   void __fastcall DateTimePickerFimChange(TObject *Sender);
   void __fastcall CheckBoxAjusteDemandaClick(TObject *Sender);
   void __fastcall ActionCriaDiretorioExecute(TObject *Sender);
   void __fastcall ActionSelDiretorioExecute(TObject *Sender);
   void __fastcall ActionExecutarExecute(TObject *Sender);
   void __fastcall ActionCfgAjusteDemandaExecute(TObject *Sender);
   void __fastcall ActionFechaExecute(TObject *Sender);
	void __fastcall ActionCfgImportaExecute(TObject *Sender);
	void __fastcall ActionHelpExecute(TObject *Sender);
   void __fastcall PageControlChange(TObject *Sender);


public:		// User declarations
   __fastcall  TFormBatchIG(TComponent* Owner, VTApl *apl_owner);
   __fastcall ~TFormBatchIG(void);

private:	//métodos
   //void       __fastcall CriaDirRede(void);
//	bool       __fastcall DefineDiretorioRede(AnsiString &dir_rede);
	void       __fastcall FormIniciaPosicao(void);
   void       __fastcall IniciaArquivos(void);
   void       __fastcall ReiniciaArquivos(void);
   //void       __fastcall SelecionaDirRede(void);
   bool       __fastcall ValidaArquivosSelecionados(void);
   bool       __fastcall ValidaLicenca(void);
   bool       __fastcall VerificaOpcoes(void);

private:	//objetos externos

private:	//dados locais
   TFormSelArqMedicao *FormSelArqMedicao;
   TFormSelArqRede    *FormSelArqRede;
   VTApl              *apl;
   VTArquivos         *arquivos;
   VTAjusta           *ajusta;
   VTBatchIG          *batch_ig;
   strOPImporta        opcao;
   bool               executando;
   struct{
         TTreeNode *rede, *medicao;
         }node_raiz;
};

//---------------------------------------------------------------------------
#endif
//eof
