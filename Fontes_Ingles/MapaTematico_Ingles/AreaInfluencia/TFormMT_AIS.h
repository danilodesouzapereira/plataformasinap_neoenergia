//---------------------------------------------------------------------------
#ifndef TFormMT_AISH
#define TFormMT_AISH

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
class TFormMT_AIS : public TForm
{
__published:   // IDE-managed Components
   TToolBar *ToolBar;
   TToolButton *butFecha;
   TImageList *ImageList;
   TActionList *ActionList;
   TAction *ActionFecha;
   TGroupBox *GroupBox1;
   TToolBar *ToolBarAIS;
   TBitBtn *ButSelAll;
   TBitBtn *ButSelNone;
   TAction *ActionSelAll;
   TAction *ActionSelNone;
   TColorDialog *ColorDialog;
   TGroupBox *GBoxPatamar;
   TComboBox *CBoxPatamar;
   TTimer *Timer;
   TBitBtn *BitBtn1;
   TAction *ActionAnimacao;
   TListView *LViewAis;
   TRadioGroup *RGroupCarga;
   TRadioGroup *RGroupCapacidade;
   TGroupBox *GBoxCargtoMaximo;
   TComboBox *CBoxCargto;
   TAction *ActionOptionChange;
   TPanel *PanelOpcao;
   TAction *ActionHelp;
   TToolButton *ToolButton1;
   TToolButton *ToolButton2;
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall ActionFechaExecute(TObject *Sender);
   void __fastcall ActionSelAllExecute(TObject *Sender);
   void __fastcall ActionSelNoneExecute(TObject *Sender);
   void __fastcall ActionAnimacaoExecute(TObject *Sender);
   void __fastcall TimerTimer(TObject *Sender);
   void __fastcall LViewAisDblClick(TObject *Sender);
   void __fastcall LViewAisClick(TObject *Sender);
   void __fastcall ActionOptionChangeExecute(TObject *Sender);
   void __fastcall LViewAisCustomDrawSubItem(TCustomListView *Sender,
          TListItem *Item, int SubItem, TCustomDrawState State,
          bool &DefaultDraw);
   void __fastcall ActionHelpExecute(TObject *Sender);

public:      // User declarations
        __fastcall  TFormMT_AIS(TComponent* Owner, VTApl *apl_owner, TWinControl *parent);
        __fastcall ~TFormMT_AIS(void);
   void __fastcall  Atualiza(void);

private:   //métodos
   void __fastcall CancelaMapaTematicoAtual(void);
   void __fastcall CBoxPatamarInicia(void);
   void __fastcall ExibeAisSelecionada(void);
   void __fastcall FormIniciaPosicao(void);
	void __fastcall HabilitaMapaTematico(bool enabled);
	int  __fastcall IndicePatamarSelecionado(void);
   void __fastcall IniciaAis(void);
   void __fastcall LViewAisInicia(void);
   void __fastcall RecalculaAreaInfluencia(void);
   void __fastcall SelecionaModoGrafico(int modo);

private:   //objetos externos

private:   //dados locais
   VTApl      *apl;
   VTConsulta *consulta;
   VTRadial   *radial;
   TList      *lisAIS;
};

//---------------------------------------------------------------------------
#endif
//eof

