//---------------------------------------------------------------------------
#ifndef TFormOpAutoH
#define TFormOpAutoH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ToolWin.hpp>
#include <Vcl.ImgList.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTChave;
class VTEdita;
class VTGrafico;
class VTPatamar;
class VTSinapServer;

//---------------------------------------------------------------------------
class TFormOpAuto : public TForm
{
__published:   // IDE-managed Components
   TTimer *Timer;
   TToolBar *ToolBar;
   TComboBox *CBoxTempoPatamar;
   TRadioGroup *RadioGroupPatamar;
   TGroupBox *GroupBox2;
   TLabel *Label1;
   TComboBox *CBoxTempoRedeNormal;
   TComboBox *CBoxTempoAbertura2;
   TToolButton *ButFecha;
   TActionList *ActionList;
   TAction *ActionFecha;
   TAction *ActionStart;
   TToolButton *ToolButton1;
   TToolButton *butStart;
   TToolButton *ButStop;
   TAction *ActionStop;
   TImageList *ImageList;
   TLabel *Label3;
   TComboBox *CBoxTempoRedeContingencia;
   TPanel *PanelTop;
   TGroupBox *GBoxEvento;
   TRichEdit *RichEdit;
   TPanel *PanelPatamar;
   TLabel *LabelPatamar;
   TLabel *LabelChave;
   TLabel *Label4;
   TComboBox *CBoxTempoAbertura1;
   TLabel *Label5;
   TLabel *Label6;
   TLabel *Label7;
   TLabel *Label8;
   TLabel *Label2;
   TLabel *Label9;
   TLabel *Label10;
   TLabel *LabelTimer;
   TCheckBox *CheckBoxLockout;
   void __fastcall TimerTimer(TObject *Sender);
   void __fastcall ActionFechaExecute(TObject *Sender);
   void __fastcall ActionStartExecute(TObject *Sender);
   void __fastcall ActionStopExecute(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall RadioGroupPatamarClick(TObject *Sender);

public:      // User declarations
   __fastcall  TFormOpAuto(TComponent* Owner, VTApl *apl_owner, TWinControl *parent, TAction *ActionOnClose);
   __fastcall ~TFormOpAuto(void);

private:   //métodos
   bool __fastcall ChaveAbre(VTChave *chave, bool lockout);
   bool __fastcall ChaveFecha(VTChave *chave);
   bool __fastcall ExecutaTransicaoControleContingencia(void);
   bool __fastcall LeDados(void);
   void __fastcall Log(AnsiString texto);
   void __fastcall LogChave(AnsiString texto);
   void __fastcall LogClear(void);
   void __fastcall LogPatamar(AnsiString texto);
   bool __fastcall RestauraEstadoNormalChaves(void);
   bool __fastcall SelecionaChaveDefeito(void);
   void __fastcall SelecionaPatamar(void);

private:   //objetos externos
   TAction       *ActionOnClose;
   VTApl         *apl;
   VTEdita       *edita;
   VTGrafico     *grafico;
   VTSinapServer *server;

private:   //dados locais
   TList *lisCHV;
   TList *lisMAN;
   enum enumESTADO {estINICIO, estREDE_NORMAL, estREDE_CONTINGENCIA, estCHAVE_ABERTA1, estCHAVE_ABERTA2};
   struct{
         int        tempo; //seg
         int        timer_count;
         VTPatamar *patamar;
         }ctrl_pat;
   struct{
         bool       enabled;
         int        estado;
         int        timer_count;
         struct{
               int rede_normal;
               int rede_contingencia;
               int chave_aberta1;
               int chave_aberta2;
               }tempo;  //seg
         VTChave *chave;
         }ctrl_def;

};

//---------------------------------------------------------------------------
#endif
