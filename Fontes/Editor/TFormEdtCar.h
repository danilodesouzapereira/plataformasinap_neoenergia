//---------------------------------------------------------------------------
#ifndef TFormEdtCarH
#define TFormEdtCarH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
#include <ActnList.hpp>
#include <ImgList.hpp>
#include <ExtCtrls.hpp>
#include <System.Actions.hpp>
#include <System.ImageList.hpp>

//---------------------------------------------------------------------------
class VTCarga;
class VTCurva;
class VTApl;
class TFormEditaCurvaCarga3F;
class TFormExibeCurva;
//class TFormEdtCurvaCar;
//class TFormEdtPlan;

//---------------------------------------------------------------------------
class TFormEdtCar : public TForm
{
__published:   // IDE-managed Components
   TPageControl *PageControl;
   TTabSheet *TabSheetGrafico;
   TTabSheet *TabSheetCarga;
   TTabSheet *TabSheetEdicao;
   TEdit *EditCodigo;
   TLabel *Label1;
   TComboBox *CBoxClasse;
   TLabel *Label2;
   TLabel *Label5;
   TComboBox *CBoxMBT;
   TComboBox *CBoxFase;
   TLabel *Label6;
   TComboBox *CBoxModCarga;
   TToolBar *ToolBar2;
   TToolButton *butConfirma;
   TToolButton *butCancela;
   TToolButton *butRetira;
   TImageList *ImageList1;
   TActionList *ActionList1;
   TAction *ActionCancela;
   TAction *ActionConfirma;
   TAction *ActionRetira;
   TAction *ActionImporta;
   TToolButton *ButImporta;
   TCheckBox *CheckBoxVIC;
   TLabel *Label4;
   TEdit *EditEnergia;
   TComboBox *CBoxUnidEnergia;
   TRadioGroup *RadioGroupTipoCurva;
   TGroupBox *GBoxTipica;
   TPanel *Panel1;
   TPanel *Panel2;
   TPanel *Panel3;
   TPanel *Panel4;
   TPanel *Panel5;
   TCheckBox *CheckBoxGrupoA;
   TTabSheet *TabSheetQtdCons;
   TLabel *Label3;
   TLabel *Label7;
   TLabel *Label8;
   TLabel *Label9;
   TLabel *Label10;
   TLabel *Label11;
   TEdit *EditNres;
   TEdit *EditNcom;
   TEdit *EditNind;
   TEdit *EditNrur;
   TEdit *EditNout;
   TEdit *EditNa4;
   TGroupBox *GroupBox1;
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall PageControlChange(TObject *Sender);
   void __fastcall CBoxUnidEnergiaChange(TObject *Sender);
   void __fastcall PageControlChanging(TObject *Sender, bool &AllowChange);
   void __fastcall CBoxMBTChange(TObject *Sender);
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall ActionConfirmaExecute(TObject *Sender);
   void __fastcall ActionRetiraExecute(TObject *Sender);
   void __fastcall ActionImportaExecute(TObject *Sender);
   void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
   void __fastcall RadioGroupTipoCurvaClick(TObject *Sender);
   void __fastcall CBoxClasseChange(TObject *Sender);
	void __fastcall CBoxFaseChange(TObject *Sender);

public:      // métodos
         __fastcall  TFormEdtCar(TComponent* Owner, VTApl *apl, VTCarga *pcarga, bool op_retira);
         __fastcall ~TFormEdtCar(void);
//VTCarga* __fastcall  CargaAtualizada(void);

private:
   VTCurva* __fastcall CriaCurva(void);
   void     __fastcall DefineCrescimento(void);
   void     __fastcall ExibeDadosCarga(void);
   void     __fastcall ExibeNomeCurvaTipica(bool enabled=true);
   VTCurva* __fastcall ExisteCurvaTipica(void);
   void     __fastcall FormIniciaPosicao(void);
   void     __fastcall IniciaCBoxClasse(void);
   void     __fastcall IniciaCBoxFase(void);
   void     __fastcall IniciaDadosCargaNova(void);
   void     __fastcall IniciaParteFixaTela(void);
   void     __fastcall IniciaQuantidadeConsumidores(void);
   bool     __fastcall LeDadosCarga(void);
   bool     __fastcall LeDadosPlaneja(void);
   bool     __fastcall LeFases(void);
   void     __fastcall LeQuantidadeConsumidoresCarga(void);
   bool     __fastcall LeValorEnergia(bool validar);
   bool     __fastcall LeValorModCarga(void);
   bool     __fastcall SelecionaClasse(void);
   bool     __fastcall SelecionaCurva(void);

private:    //objetos externos
   VTApl   *apl;
   VTCarga *pcarga;

private:    //dados locais
   TFormEditaCurvaCarga3F *FormEditaCurvaCarga3F;
   TFormExibeCurva *FormExibeCurva;
   //TFormEdtCurvaCar *FormEdtCurva;
   //TFormEdtPlan *FormEdtPlan;
   //TList *lisTMP;
   struct   {
			VTCurva *propria;
            }curva;
};

#endif
//---------------------------------------------------------------------------
//eof

