//---------------------------------------------------------------------------
#ifndef TFormCfgAGH
#define TFormCfgAGH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <ActnList.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include "..\Geral\VTGeral.h"

//---------------------------------------------------------------------------
class VTBDgeral;
class VTGeral;

//---------------------------------------------------------------------------
class TformCfgAG : public TForm
{
__published:	// IDE-managed Components
   TToolBar *ToolBar1;
   TToolButton *butDefault;
   TToolButton *butInclui;
   TToolButton *butSair;
   TActionList *ActionList1;
   TAction *ActionConfirma;
   TAction *ActionCancela;
   TAction *ActionDefault;
   TImageList *ImageList;
   TPanel *Panel1;
        TPageControl *PageControl1;
        TTabSheet *TabSheet1;
        TTabSheet *TabSheet2;
        TGroupBox *gboxGeral;
        TLabel *Label1;
        TLabel *Label2;
        TLabel *Label3;
        TLabel *Label4;
        TEdit *edtnger;
        TEdit *edtnindiv;
        TEdit *edtprobMut;
        TEdit *edtProbCruz;
        TGroupBox *gboxAvalia;
        TLabel *Label13;
        TLabel *Label14;
        TLabel *Label15;
        TEdit *edtkCusto;
        TEdit *edtkCar;
        TEdit *edtkDV;
        TGroupBox *gboxGrafico;
        TGroupBox *gboxPatamar;
        TComboBox *cboxPatamar;
        TRadioGroup *rgroupTipoGraf;
        TRadioGroup *rgroupExibeIndiv;
        TComboBox *cboxVisu;
        TRadioGroup *rgroupTipoFObj;
        TGroupBox *gboxCruzamento;
        TLabel *Label5;
        TEdit *edtProbCruzUnif;
        TCheckBox *ckboxCruzUnif;
        TLabel *Label12;
        TEdit *edtGama;
        TEdit *edtkInvest;
        TLabel *Label9;
        TGroupBox *gboxSubPop;
        TLabel *Label6;
        TEdit *edtnindivSub;
        TGroupBox *gboxPondera;
        TLabel *Label16;
        TLabel *Label17;
        TLabel *lbTrack;
        TTrackBar *TrackBarTensao;
        TRadioGroup *rgroupSelecao;
        TGroupBox *gboxInterface;
        TLabel *Label7;
        TCheckBox *ckboxRecondut;
        TGroupBox *gboxRanking;
        TLabel *Label8;
        TEdit *edtIndUnif;
        TGroupBox *gboxCritParada;
        TLabel *Label10;
        TCheckBox *ckboxCriterioParada;
        TEdit *edtngerParada;
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall ActionConfirmaExecute(TObject *Sender);
   void __fastcall ActionDefaultExecute(TObject *Sender);
   void __fastcall cboxVisuChange(TObject *Sender);
   void __fastcall ckboxCriterioParadaClick(TObject *Sender);
   void __fastcall ckboxCruzUnifClick(TObject *Sender);
   void __fastcall edtngerKeyPress(TObject *Sender, char &Key);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall rgroupSelecaoClick(TObject *Sender);
   void __fastcall rgroupTipoGrafClick(TObject *Sender);
   void __fastcall rgroupTipoFObjClick(TObject *Sender);
   void __fastcall TrackBarTensaoChange(TObject *Sender);

public:		// User declarations
         __fastcall TformCfgAG(TComponent* Owner, AnsiString dirBase, AnsiString dirRede);
         __fastcall TformCfgAG(TComponent* Owner, VTBDgeral *bdGeral, VTGeral *geral);
         __fastcall ~TformCfgAG(void);

private:	// métodos
   void __fastcall Consistencia(void);
   void __fastcall ExibeDados(void);
   bool __fastcall LeDados(void);
   void __fastcall LeDadosVisu(void);
   void __fastcall PreencheComboPat(void);
   void __fastcall PreencheComboVisu(void);
   void __fastcall SalvaInfoVisu(void);

private:	// dados
   strAG     AG;
   struct    Visualiza : public TObject  {int tipoGrafico; int npatExibe; int exibeIndiv;};
   VTBDgeral *bdGeral;
   VTGeral   *geral;
   TList     *listaVISU;
   Visualiza *visuRef;
   bool      obj_locais;
};

#endif
//---------------------------------------------------------------------------
//eof
