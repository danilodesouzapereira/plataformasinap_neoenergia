//---------------------------------------------------------------------------
#ifndef TFormPerdaregH
#define TFormPerdaregH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ToolWin.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.ImgList.hpp>
#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ExtDlgs.hpp>
#include <Vcl.Menus.hpp>
#include <System.ImageList.hpp>

//---------------------------------------------------------------------------
class TFormPerda;
class TFormPerdaComercial;
class TFormEnergiaPassante;
class VTApl;
class VTRedes;
class VTCfgPerdaTec;
class PERDA_MEDIDOR;
class PERDA_RAMAL;
class PERDA_TRAFO;
//---------------------------------------------------------------------------
class TFormPerdareg : public TForm
{
__published:	// IDE-managed Components
   TToolBar *ToolBar;
   TPanel *Panel;
   TPanel *PanelTitulo;
   TPanel *PanelToolBar;
   TLabel *LabelTitulo;
   TPanel *PanelData;
   TLabel *Label1;
   TDateTimePicker *DateTimePicker;
   TImageList *ImageList;
   TToolButton *ButParametros;
   TToolButton *ButMedicoes;
   TToolButton *ButCalcula;
   TToolButton *ToolButton4;
   TToolButton *ButRelatorio;
   TToolButton *ToolButton7;
   TPanel *PanelRight;
   TBitBtn *ButFecha;
   TToolButton *ToolButton5;
   TActionList *ActionList;
   TAction *ActionCfgParametro;
   TAction *ActionFechar;
   TAction *ActionAlteraUnidade;
   TAction *ActionCalculaPerdas;
   TAction *ActionHelp;
   TOpenTextFileDialog *OpenTextFileDialog;
   TAction *ActionCfgMedicao;
   TPopupMenu *PopupMenuRelatorio;
   TMenuItem *MenuItemRelPerdaComercial;
   TMenuItem *MenuItemRelPerdaTecnica;
   TAction *ActionRelPerdaComercial;
   TAction *ActionRelPerdaTecnica;
   TAction *ActionSalvaBase;
   TToolButton *ButSalvaBase;
	TAction *ActionRelEnergiaPassante;
	TMenuItem *RelatriodeEnergiaPassante1;
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall ActionCfgParametroExecute(TObject *Sender);
   void __fastcall ActionFecharExecute(TObject *Sender);
   void __fastcall ActionAlteraUnidadeExecute(TObject *Sender);
   void __fastcall ActionCalculaPerdasExecute(TObject *Sender);
   void __fastcall ActionHelpExecute(TObject *Sender);
   void __fastcall DateTimePickerDropDown(TObject *Sender);
   void __fastcall DateTimePickerCloseUp(TObject *Sender);
   void __fastcall ActionCfgMedicaoExecute(TObject *Sender);
   void __fastcall ActionRelPerdaComercialExecute(TObject *Sender);
   void __fastcall ActionRelPerdaTecnicaExecute(TObject *Sender);
   void __fastcall ActionSalvaBaseExecute(TObject *Sender);
   void __fastcall DateTimePickerChange(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall ActionRelEnergiaPassanteExecute(TObject *Sender);

public:		// User declarations
   __fastcall  TFormPerdareg(TComponent* Owner, VTApl *apl_owner, TWinControl *parent);
   __fastcall ~TFormPerdareg(void);

private:      //métodos
   void __fastcall AtualizaCaptions(void);
   bool __fastcall CalculaPerdasRegulatorias(void);
   void __fastcall ConfiguraMedicao(void);
   bool __fastcall ConfiguraParametros(void);
   void __fastcall CopiaCfgPerdas(void);
   bool __fastcall CopiaRedes1ParaRedes2(VTRedes *redes1, VTRedes *redes2);
   bool __fastcall DataSelecionada(void);
   void __fastcall DateTimePickerInicia(void);
   void __fastcall FormPerdasCalculado(bool calculado);
   void __fastcall ReiniciaFormPerdas(void);
   void __fastcall ReiniciaMedicao(void);
   bool __fastcall ValidaMedicao(void);
   bool __fastcall VerificaCfgPerdasAlterado(void);
   bool __fastcall VerificaRedesCarregadas(void);

private:   //objetos externos

private:   //dados locais
   VTApl                *apl;
   VTApl      			*mApl_owner;
   TFormPerda           *FormPerda;
   TFormPerdaComercial  *FormPerdaComercial;
   TFormEnergiaPassante  *FormEnergiaPassante;
   VTRedes              *redes_local;
   //controle de alteração de data no TDateTimePicker
   TDateTime date, last_date;
   //teste
   bool  redeAlterada;
//   bool editor_enabled;
   //VTCfgPerdaTec        *CfgPerdaTec_anterior;
   struct
      {
      bool primeira_valida;
      struct   {
               int perda_medidor;
               int perda_ramal;
               int perda_trafo;
               int perda_trafo_sed;
               } opcao;
      struct   {
               PERDA_MEDIDOR *perda_medidor;
               PERDA_RAMAL   *perda_ramal;
               PERDA_TRAFO   *perda_trafo;
               } unico;
      struct   {
               bool   ferro;
               bool   medidor;
               bool   ramal;
               } PorFora;
      struct   {
               double adicional_perc;  //%
               } perda;
      struct   {
               int    tipo;
               int    modelo_carga;
               bool   com_capacitor;
               bool   com_neutro;
               bool   zat_mrt;
               double zat_ohm;
               } fluxo;
      }strCfgPerdaTec_anterior;
};

//---------------------------------------------------------------------------
#endif
//eof

