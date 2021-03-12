//---------------------------------------------------------------------------
#ifndef TFormClientH
#define TFormClientH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
#include <ExtCtrls.hpp>
#include <System.Win.ScktComp.hpp>
#include <ImgList.hpp>
#include "..\Protocolo\VTProtocolo.h"
#include <System.Actions.hpp>
#include <System.ImageList.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTSinapClient;
class VTSinapUser;

//---------------------------------------------------------------------------
class TFormClient : public TForm
{
__published:	// IDE-managed Components
   TToolBar *ToolBar;
   TToolButton *ButStart;
   TToolButton *ButStop;
   TActionList *ActionList;
   TAction *ActionStart;
   TAction *ActionStop;
   TGroupBox *GBoxTX;
   TGroupBox *GBoxRx;
   TSplitter *Splitter1;
   TRichEdit *RichEditTx;
   TStatusBar *StatusBar;
   TToolButton *ToolButton1;
   TToolButton *ButClearLog;
   TAction *ActionClearLog;
   TPanel *Panel;
   TLabel *label;
   TComboBox *CBoxIP;
   TImageList *ImageList;
   TGroupBox *GroupBox1;
   TLabel *Label1;
   TComboBox *CBoxEqptoTipo;
   TLabel *Label2;
   TEdit *EditEqptoCodigo;
   TButton *Button1;
   TAction *ActionSolicitaRegistro;
   TDateTimePicker *DateTimePicker;
   TLabel *Label3;
   TCheckBox *CheckBoxTempoReal;
   TTimer *Timer;
   TButton *Button2;
   TAction *ActionChaveAbre;
   TAction *ActionChaveFecha;
   TButton *Button3;
   TButton *Button4;
   TAction *ActionSimulaFalta;
   TButton *Button5;
   TAction *ActionEliminaFalta;
   TPanel *PanelToolBar;
	TAction *ActionTrafoIncTap;
	TAction *ActionTrafoDecTap;
	TButton *Button6;
	TButton *Button7;
	TToolButton *ButSupervisaoAuto;
	TAction *ActionSupervisaoAuto;
	TButton *Button8;
	TButton *Button9;
	TAction *ActionCapacitorOff;
	TAction *ActionCapacitorOn;
	TRichEdit *RichEditRx;
   void __fastcall ActionStartExecute(TObject *Sender);
   void __fastcall ActionStopExecute(TObject *Sender);
   void __fastcall ActionClearLogExecute(TObject *Sender);
   void __fastcall ActionSolicitaRegistroExecute(TObject *Sender);
   void __fastcall CheckBoxTempoRealClick(TObject *Sender);
   void __fastcall TimerTimer(TObject *Sender);
   void __fastcall ActionChaveAbreExecute(TObject *Sender);
   void __fastcall ActionChaveFechaExecute(TObject *Sender);
	void __fastcall ActionSimulaFaltaExecute(TObject *Sender);
   void __fastcall ActionEliminaFaltaExecute(TObject *Sender);
	void __fastcall ActionTrafoDecTapExecute(TObject *Sender);
	void __fastcall ActionTrafoIncTapExecute(TObject *Sender);
	void __fastcall ActionSupervisaoAutoExecute(TObject *Sender);
	void __fastcall ActionCapacitorOffExecute(TObject *Sender);
	void __fastcall ActionCapacitorOnExecute(TObject *Sender);

public:		// SinapUser declarations
   __fastcall  TFormClient(TComponent* Owner, VTApl *apl_owner, TWinControl *parent);
   __fastcall ~TFormClient(void);

private:	//métodos
	void  __fastcall AlteraCapacitorEstado(bool ligado);
	void  __fastcall AlteraTrafoTap(int variacao_tap);
	void  __fastcall CBoxIP_Inicia(void);
	void  __fastcall ManobraChave(bool abrir);

private:	//objetos externos
   VTApl *apl;

private:	//dados locais
	bool          supervisao_auto;
   VTSinapClient *client;
   VTSinapUser   *user;
   };

//---------------------------------------------------------------------------
extern PACKAGE TFormClient *FormClient;

//---------------------------------------------------------------------------
#endif
//eof

