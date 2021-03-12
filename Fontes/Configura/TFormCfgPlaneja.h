//---------------------------------------------------------------------------
#ifndef TFormCfgPlanejaH
#define TFormCfgPlanejaH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ActnList.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
#include <ImgList.hpp>
#include "..\Geral\VTGeral.h"

//---------------------------------------------------------------------------
class  VTBDado;

//---------------------------------------------------------------------------
class TformCfgPlaneja : public TForm
{
__published:	// IDE-managed Components
   TPanel *Panel1;
   TGroupBox *gboxPeriodo;
   TLabel *Label15;
   TLabel *lblNumAnos;
   TComboBox *cmbAnoRef;
   TComboBox *cmbNumAno;
   TGroupBox *GroupBox11;
   TLabel *Label9;
   TLabel *Label10;
   TEdit *edtFPerda;
   TEdit *edtFSCarga;
   TGroupBox *gboxCusto;
   TLabel *Label7;
   TLabel *Label8;
   TLabel *Label17;
   TLabel *Label18;
   TEdit *edtCDeman;
   TEdit *edtCEnergia;
   TEdit *edtTaxaAno;
   TEdit *edtVidaUtil;
   TToolBar *ToolBar1;
   TToolButton *butInclui;
   TToolButton *butSair;
   TActionList *ActionList1;
   TAction *ActionConfirma;
   TAction *ActionCancela;
   TImageList *ImageList;
   TAction *ActionDefault;
   TToolButton *butDefault;
        TLabel *Label1;
        TEdit *edtCEnd;
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall ActionConfirmaExecute(TObject *Sender);
   void __fastcall ActionDefaultExecute(TObject *Sender);
   void __fastcall edtCDemanKeyPress(TObject *Sender, char &Key);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormShow(TObject *Sender);

public:		// User declarations
   __fastcall TformCfgPlaneja(TComponent* Owner, AnsiString dirBase, AnsiString dirRede);
   __fastcall ~TformCfgPlaneja(void);

private:	// métodos
   void __fastcall ExibeDados(void);
   bool __fastcall LeDados(void);

private:	// dados
   VTBDado *bdado;
   VTGeral *geral;
   strPLAN PLAN;
};

#endif
//---------------------------------------------------------------------------
//eof
