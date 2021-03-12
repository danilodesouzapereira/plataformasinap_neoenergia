//---------------------------------------------------------------------------
#ifndef TFormEdtMedicaoH
#define TFormEdtMedicaoH

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
#include <System.ImageList.hpp>

//---------------------------------------------------------------------------
class TMedicao;
class VTApl;

//---------------------------------------------------------------------------
class TFormEdtMedicao : public TForm
{
__published:	// IDE-managed Components
   TToolBar *ToolBar1;
   TToolButton *butConfirma;
   TToolButton *butCancela;
   TImageList *ImageList;
   TActionList *ActionList;
   TAction *ActionCancela;
   TAction *ActionConfirma;
   TComboBox *CBoxRede;
   TEdit *EditEnergia;
   TTrackBar *TrackBar;
   TGroupBox *GBoxPercMT_BT;
   TLabel *Label3;
   TLabel *Label4;
   TLabel *LabelPercSDMT;
   TLabel *LabelPercSDBT;
   TGroupBox *GBoxRede;
   TGroupBox *GBoxEnergia;
   TComboBox *CBoxTipoRede;
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall ActionConfirmaExecute(TObject *Sender);
   void __fastcall TrackBarChange(TObject *Sender);
   void __fastcall CBoxTipoRedeChange(TObject *Sender);

public:		// User declarations
   __fastcall  TFormEdtMedicao(TComponent* Owner, VTApl *apl_owner, TMedicao *medicao);
   __fastcall ~TFormEdtMedicao(void);

private:	//métodos

   void __fastcall CBoxRedeInicia(void);
   void __fastcall CBoxTipoRedeInicia(void);
   double __fastcall EnergiaCarga_mwhmes(TList *lisCARGA);
   bool __fastcall LeDadosMedicao(void);
   void __fastcall ValidaDistribuicaoPerdas(void);

private:	//objetos externos
   VTApl    *apl;
   TMedicao *medicao;

private:	//dados locais

};

//---------------------------------------------------------------------------
#endif
//eof

