//---------------------------------------------------------------------------
#ifndef TFormCurvaH
#define TFormCurvaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <Grids.hpp>
#include "..\Curvas\VTCurva.h"
#include <ExtCtrls.hpp>
#include <System.Actions.hpp>
#include <System.ImageList.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTCurva;

//---------------------------------------------------------------------------
class TFormCurva : public TForm
{
__published:	// IDE-managed Components
   TToolBar *ToolBar1;
   TToolButton *butConfirma;
   TToolButton *butCancela;
   TImageList *ImageList1;
   TActionList *ActionList1;
   TAction *ActionCancela;
   TAction *ActionConfirma;
   TGroupBox *GroupBoxCurva;
   TStringGrid *SGrid;
   TGroupBox *GroupBox2;
   TComboBox *CBoxNumPto;
   TGroupBox *GroupBox1;
   TComboBox *CBoxTipoCurva;
   TGroupBox *GBoxFaixaConsumo;
   TLabel *Label1;
   TLabel *Label2;
   TEdit *EditLimiteInf;
   TEdit *EditLimiteSup;
   TGroupBox *GroupBox3;
   TEdit *EditCodigo;
	TGroupBox *GroupBox4;
	TComboBox *CBoxUnidade;
   TPanel *Panel;
   TGroupBox *GBoxDiaMedicao;
   TComboBox *CBoxDiaMedicao;
   TPanel *Panel1;
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall ActionConfirmaExecute(TObject *Sender);
   void __fastcall CBoxNumPtoChange(TObject *Sender);
	void __fastcall CBoxTipoCurvaChange(TObject *Sender);
	void __fastcall CBoxUnidadeChange(TObject *Sender);

public:		// User declarations
        __fastcall  TFormCurva(TComponent* Owner, VTApl *apl);
        __fastcall ~TFormCurva(void);
   void __fastcall EditaCurva(VTCurva *curva);

private:
   void __fastcall CBoxMedicaoInicia(void);
   void __fastcall CBoxNumPtoInicia(void);
	void __fastcall CBoxTipoCurvaInicia(void);
	void __fastcall CBoxUnidadeInicia(void);
	void __fastcall FaixaConsumoInicia(void);
   bool __fastcall FaixaConsumoLe(void);
   bool __fastcall SGridExistePeriodo(int nr);
	void __fastcall SGridInicia(void);
   //void __fastcall SGridIniciaCurvaPQ(int num_pto);
   void __fastcall SGridIniciaCurvaPQVT(int num_pto);
   void __fastcall SGridIniciaCurvaPQ_MDP(int num_pto);
   void __fastcall SGridIniciaCurvaPFP(int num_pto);
   void __fastcall SGridIniciaCurvaPFP_MDP(int num_pto);
   void __fastcall SGridIniciaCurvaLD(int num_pto);
   bool __fastcall SGridLeCurva(void);
   //bool __fastcall SGridLeCurvaPQ(void);
   bool __fastcall SGridLeCurvaPQVT(void);
   bool __fastcall SGridLeCurvaPQ_MDP(void);
   bool __fastcall SGridLeCurvaPFP(void);
   bool __fastcall SGridLeCurvaPFP_MDP(void);
   bool __fastcall SGridLeCurvaLD(void);
   bool __fastcall SGridLeCurvaPatamarFixo(double *valor, int dim_valor);
   bool __fastcall SGridLeCurvaPatamarVariavel(double *valor, int dim_valor);

private:	//objetos externos
   VTApl   *apl;
   VTCurva *curva;
	int tipo_curva, unidade;

private:	//dados locais
   VTCurva *curva_nova;
};

//---------------------------------------------------------------------------
#endif
//eof

