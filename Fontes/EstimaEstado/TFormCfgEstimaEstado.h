//---------------------------------------------------------------------------
#ifndef TFormCfgEstimaEstadoH
#define TFormCfgEstimaEstadoH

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
#include <System.ImageList.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTEstimaEstado;

//---------------------------------------------------------------------------
class TFormCfgEstimaEstado : public TForm
{
__published:	// IDE-managed Components
   TToolBar *ToolBar1;
   TToolButton *butConfirma;
   TToolButton *butCancela;
   TImageList *ImageList;
   TActionList *ActionList;
   TAction *ActionCancela;
   TAction *ActionConfirma;
   TRadioGroup *RadioGroupTipo;
   TGroupBox *GroupBox1;
   TRadioGroup *RadioGroupAnalise;
   TRadioGroup *RadioGroupMetodoEE;
   TLabel *LabelPatamar;
   TEdit *EditPatamar;
   TCheckBox *CheckBoxRelFluxoPotencia;
   TRadioGroup *RadioGroupReferencia;
   TCheckBox *CheckBoxPerdaFerro;
   TGroupBox *GBoxVDNM;
   TLabel *LabelNumMaxMontH;
   TCheckBox *CheckBoxVDNM;
   TEdit *EdtNumMaxMontH;
   TRadioGroup *RadioGroupOrdenacaoLinhasQR;
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall ActionConfirmaExecute(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);

public:		// User declarations
   __fastcall  TFormCfgEstimaEstado(TComponent* Owner, VTApl *apl_owner, TWinControl *parent);
   __fastcall ~TFormCfgEstimaEstado(void);

private:	//métodos
   void __fastcall ExibeConfiguracao(void);
   bool __fastcall LeConfiguracao(void);

private:	//objetos externos
   VTApl          *apl;
   VTEstimaEstado *estima;

private:	//dados locais
};

//---------------------------------------------------------------------------
#endif
//eof

