//---------------------------------------------------------------------------
#ifndef TFormMedidorH
#define TFormMedidorH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <ImgList.hpp>
#include <Menus.hpp>
#include <ToolWin.hpp>
#include "TFORMEDT.h"
#include <System.Actions.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTEdt;
class VTEqpto;
class TFormEqpto;

//---------------------------------------------------------------------------
class TFormMedidor : public TFormEdt
{
__published:   // IDE-managed Components
   TAction *ActionSelEqpto;
   TPanel *Panel;
   TAction *ActionRedeChanged;
   TAction *ActionHelp;
   TAction *ActionRelatorio;
   TToolButton *ButRelatorio;
	TGroupBox *GroupBoxFiltro;
   TPanel *Panel1;
   TComboBox *CBoxTipoEqpto;
   TListBox *LBoxEqpto;
   TSplitter *Splitter1;
	TToolBar *ToolBar1;
	TToolButton *ToolButton1;
	TAction *ActionFiltra;
	TToolButton *ToolButton2;
	TToolButton *ToolButton3;
   void __fastcall ActionEdtExecute(TObject *Sender);
   void __fastcall ActionSelEqptoExecute(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall ActionRedeChangedExecute(TObject *Sender);
   void __fastcall ActionHelpExecute(TObject *Sender);
   void __fastcall ActionRelatorioExecute(TObject *Sender);
   void __fastcall LBoxEqptoClick(TObject *Sender);
   void __fastcall LBoxEqptoDblClick(TObject *Sender);
   void __fastcall LBoxEqptoMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
   void __fastcall CBoxTipoEqptoChange(TObject *Sender);
	void __fastcall ActionFiltraExecute(TObject *Sender);

public:      // User declarations
        __fastcall  TFormMedidor(TForm *form_owner, VTApl *apl_owner, TWinControl *parent);
        __fastcall ~TFormMedidor(void);
   void __fastcall  Ativo(void);
   void __fastcall  Atualiza(void);
   void __fastcall  DefineEqptoSelecionado(VTEqpto *eqpto);

private: //dados externos
   void __fastcall CBoxTipoEqptoAtualiza(VTEqpto *eqpto);
   void __fastcall ExibeEqptoSelecionado(VTEqpto *eqpto);
   void __fastcall FormIniciaPosicao(void);
   //void __fastcall GraficoMoldura(VTEqpto *eqpto);
   void __fastcall GraficoZoom(VTEqpto *eqpto);
   void __fastcall LBoxEqptoInicia(VTEqpto *eqpto_ref);

private: //dados externos
   VTEqpto *eqpto;

private: //dados locais
   VTApl      *apl;
   VTEdt      *edt;
   TFormEqpto *FormEqpto;
   TList      *lisEQP;
   struct{
         TShiftState shift_state;
         }zoom;
};

//---------------------------------------------------------------------------
#endif
//eof
