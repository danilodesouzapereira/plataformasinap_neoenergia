//---------------------------------------------------------------------------
#ifndef TFormCentroCargaH
#define TFormCentroCargaH

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

//---------------------------------------------------------------------------
class VTApl;

//---------------------------------------------------------------------------
class TFormCentroCarga : public TForm
{
__published:   // IDE-managed Components
   TToolBar *ToolBar;
   TToolButton *butFecha;
   TImageList *ImageList;
   TActionList *ActionList;
   TAction *ActionFecha;
   TGroupBox *GroupBox1;
   TCheckListBox *CLBoxRegiao;
   TToolBar *ToolBarAIS;
   TBitBtn *ButSelAll;
   TBitBtn *ButSelNone;
   TAction *ActionSelAll;
   TAction *ActionSelNone;
   TColorDialog *ColorDialog;
   TTimer *Timer;
   TAction *ActionAnimacao;
   TGroupBox *GroupBox4;
   TBitBtn *ButTimer;
   TCheckBox *CheckBoxAno;
   TCheckBox *CheckBoxPatamar;
   TGroupBox *GroupBox2;
   TComboBox *CBoxPatamar;
   TComboBox *CBoxAno;
   TAction *ActionCalculaCentroCarga;
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall ActionFechaExecute(TObject *Sender);
   void __fastcall ActionSelAllExecute(TObject *Sender);
   void __fastcall ActionSelNoneExecute(TObject *Sender);
   void __fastcall CLBoxRegiaoDblClick(TObject *Sender);
   void __fastcall CBoxPatamarChange(TObject *Sender);
   void __fastcall ActionAnimacaoExecute(TObject *Sender);
   void __fastcall TimerTimer(TObject *Sender);
   void __fastcall CLBoxRegiaoClickCheck(TObject *Sender);
   void __fastcall CLBoxRegiaoDrawItem(TWinControl *Control, int Index,
          TRect &Rect, TOwnerDrawState State);
   void __fastcall ActionCalculaCentroCargaExecute(TObject *Sender);

public:      // User declarations
        __fastcall  TFormCentroCarga(TComponent* Owner, VTApl *apl_owner, TWinControl *parent);
        __fastcall ~TFormCentroCarga(void);
   void __fastcall  Atualiza(void);

public:   //dados
   TAction *ActionOnClose;

private:   //métodos
   void __fastcall CBoxAnoInicia(void);
   void __fastcall CBoxPatamarInicia(void);
   void __fastcall CLBoxRegiaoInicia(void);
   void __fastcall ExibeCentroCarga(void);
	int  __fastcall IndicePatamarSelecionado(void);
   void __fastcall IniciaCcs(void);
   void __fastcall MapaTematicoCancela(void);
	void __fastcall MapaTematicoHabilita(bool enabled);
   void __fastcall MapaTematicoMonta(void);
   void __fastcall RecalculaCentroCarga(void);
   void __fastcall SelecionaModoGrafico(int modo);

private:   //objetos externos
   VTApl   *apl;

private:   //dados locais
};

//---------------------------------------------------------------------------
#endif
//eof

