//---------------------------------------------------------------------------
#ifndef TFormCfgFlowH
#define TFormCfgFlowH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.ToolWin.hpp>
#include <Vcl.ExtCtrls.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTCurvas;
class VTFlow;
class VTMercado;
class VTRedes;

//---------------------------------------------------------------------------
class TFormCfgFlow : public TForm
{
__published:	// IDE-managed Components
   TToolBar *ToolBar;
   TToolButton *butConfirma;
   TToolButton *butCancela;
   TImageList *ImageList;
   TActionList *ActionList;
   TAction *ActionCancela;
   TAction *ActionConfirma;
   TRadioGroup *RadioGroupRelatorio;
   TGroupBox *GBoxTempo;
   TCheckBox *CheckBoxCronometro;
   TCheckBox *CheckBoxProgresso;
   TRadioGroup *RadioGroupModeloCalculo;
   TRadioGroup *RadioGroupModeloCarga;
   TGroupBox *GBoxTrafo;
   TCheckBox *CheckBoxPerdaFerro;
   TRadioGroup *RadioGroupCurvaTipica;
   TGroupBox *GBoxMercado;
   TComboBox *CBoxAno;
   TCheckBox *CheckBoxMercado;
   TLabel *Label1;
   TPanel *PanelAno;
   TRadioGroup *RadioGroupModeloRede;
   TGroupBox *GBoxProcessamento;
   TCheckBox *CheckBoxCalculoPorRede;
   TCheckBox *CheckBoxThread;
   TCheckBox *CheckBoxTramo;
   TCheckBox *CheckBoxAviso;
   TPanel *PanelLeft;
   TPanel *PanelRight;
   TGroupBox *GBoxDebug;
   TCheckBox *CheckBoxRelatorioBarDesc;
   void __fastcall CBoxAnoChange(TObject *Sender);
   void __fastcall CheckBoxMercadoClick(TObject *Sender);
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall ActionConfirmaExecute(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);

public:		// User declarations
   __fastcall  TFormCfgFlow(TComponent* Owner, VTApl *apl_owner);
   __fastcall ~TFormCfgFlow(void);

private:	//métodos
   void __fastcall ExibeConfiguracao(void);
   void __fastcall ExibeOpcaoCronometro(void);
   void __fastcall ExibeOpcaoCurvaTipica(void);
   void __fastcall ExibeOpcaoMercado(void);
   void __fastcall ExibeOpcaoModeloCarga(void);
   void __fastcall ExibeOpcaoPerdaFerroTrafo(void);
   void __fastcall ExibeOpcaoProcessamento(void);
   void __fastcall ExibeOpcaoProgresso(void);
   void __fastcall ExibeOpcaoRelatorio(void);
   void __fastcall ExibeOpcaoTipoFluxo(void);
   void __fastcall FormIniciaPosicao(void);
   bool __fastcall SalvaConfiguracao(void);
   bool __fastcall SalvaOpcaoCronometro(void);
   bool __fastcall SalvaOpcaoCurvaTipica(void);
   bool __fastcall SalvaOpcaoMercado(void);
   bool __fastcall SalvaOpcaoModeloCarga(void);
   bool __fastcall SalvaOpcaoPerdaFerroTrafo(void);
   bool __fastcall SalvaOpcaoProcessamento(void);
   bool __fastcall SalvaOpcaoProgresso(void);
   bool __fastcall SalvaOpcaoRelatorio(void);
   bool __fastcall SalvaOpcaoTipoFluxo(void);

private:   //objetos externos
   VTApl     *apl;
   VTCurvas  *curvas;
   VTFlow    *flow;
   VTMercado *mercado;
   VTRedes   *redes;

private:   //dados locais
   struct{
         int  ModeloCarga;
         int  TipoFluxo;
         int  TipoRelatorio;
         bool RelatorioBarDesc;
         bool AvisoEnabled;
         bool CronometroEnabled;
         bool MultiThreadEnabled;
         bool PorRedeEnabled;
         bool PerdaFerroEnabled;
         bool ProgressoEnabled;
         bool TramoEnabled;
         int  AnoMercado;
         int  DiaCurvaTipica;
         }cfg;
};

//---------------------------------------------------------------------------
#endif
//eof
