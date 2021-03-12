//---------------------------------------------------------------------------
#ifndef TFormMT_CCSH
#define TFormMT_CCSH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ActnList.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <CheckLst.hpp>
#include <Buttons.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <System.Actions.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTRadial;
class VTConsulta;

//---------------------------------------------------------------------------
class TFormMT_CCS : public TForm
{
__published:   // IDE-managed Components
   TToolBar *ToolBar;
   TToolButton *butFecha;
   TImageList *ImageList;
   TActionList *ActionList;
   TAction *ActionFecha;
   TGroupBox *GroupBox1;
   TCheckListBox *CLBoxCCS;
   TToolBar *ToolBarAIS;
   TBitBtn *ButSelAll;
   TBitBtn *ButSelNone;
   TAction *ActionSelAll;
   TAction *ActionSelNone;
   TColorDialog *ColorDialog;
   TGroupBox *GroupBox2;
   TComboBox *CBoxPatamar;
   TTimer *Timer;
   TBitBtn *BitBtn1;
   TAction *ActionAnimacao;
   TAction *ActionHelp;
   TToolButton *ToolButton1;
   TToolButton *ToolButton2;
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall ActionFechaExecute(TObject *Sender);
   void __fastcall ActionSelAllExecute(TObject *Sender);
   void __fastcall ActionSelNoneExecute(TObject *Sender);
   void __fastcall CLBoxCCSDblClick(TObject *Sender);
   void __fastcall CBoxPatamarChange(TObject *Sender);
   void __fastcall ActionAnimacaoExecute(TObject *Sender);
   void __fastcall TimerTimer(TObject *Sender);
   void __fastcall CLBoxCCSClickCheck(TObject *Sender);
   void __fastcall CLBoxCCSDrawItem(TWinControl *Control, int Index,
          TRect &Rect, TOwnerDrawState State);
   void __fastcall ActionHelpExecute(TObject *Sender);

public:      // User declarations
        __fastcall  TFormMT_CCS(TComponent* Owner, VTApl *apl_owner, TWinControl *parent);
        __fastcall ~TFormMT_CCS(void);
   void __fastcall  Atualiza(void);

private:   //métodos
   void __fastcall CancelaMapaTematicoAtual(void);
   void __fastcall CBoxPatamarInicia(void);
   void __fastcall CLBoxCcsInicia(void);
   void __fastcall ExibeCcsSelecionada(void);
   void __fastcall FormIniciaPosicao(void);
	void __fastcall HabilitaMapaTematico(bool enabled);
	int  __fastcall IndicePatamarSelecionado(void);
   void __fastcall IniciaCcs(void);
   void __fastcall RecalculaCentroCarga(void);
   void __fastcall SelecionaModoGrafico(int modo);

private:   //objetos externos

private:   //dados locais
   VTApl      *apl;
   VTConsulta *consulta;
   VTRadial   *radial;
   TList      *lisCCS;
};

//---------------------------------------------------------------------------
#endif
//eof

