//---------------------------------------------------------------------------
#ifndef TFormEdtMotorH
#define TFormEdtMotorH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.ToolWin.hpp>
#include <VCLTee.Chart.hpp>
#include <VCLTee.Series.hpp>
#include <VclTee.TeeGDIPlus.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.ValEdit.hpp>
#include <System.ImageList.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTLegenda;
class VTPatamares;
class VTMotor;

//---------------------------------------------------------------------------
class TFormEdtMotor : public TForm
{
__published:	// IDE-managed Components
   TImageList *ImageList1;
   TActionList *ActionList1;
   TAction *ActionCancela;
   TAction *ActionConfirma;
   TAction *ActionRetira;
   TToolBar *ToolBar2;
   TToolButton *butConfirma;
   TToolButton *butCancela;
	TToolButton *butRetira;

	TPanel *pnlMotorBaseInfo;
	TLabel *lblCodigo;
   TEdit *EditCodigo;
	TPanel *pnlMotorTabs;
	TLabel *lblTipoMotor;
	TComboBox *cmbTipoMotor;
	TValueListEditor *vleMotor;

   void __fastcall ActionCancelaExecute(TObject *Sender);
	void __fastcall ActionConfirmaExecute(TObject *Sender);
   void __fastcall ActionRetiraExecute(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
public:		// métodos
        __fastcall TFormEdtMotor(TComponent* Owner, VTApl *apl, VTMotor* motor);
        __fastcall ~TFormEdtMotor(void);
   void __fastcall OpcaoRetira(bool enabled);

private:    //métodos
   void __fastcall ExibeDadosMotor(void);
   void __fastcall FormIniciaPosicao(void);
	bool __fastcall LeDadosMotor(void);
	void __fastcall IniciaVleMotor(void);
	void __fastcall IniciaCmbTipoMotor(void);

private: //objetos externos
   VTApl    *apl;
   VTMotor  *motor;

private:    // atributos
   //TFormEdtPlan *FormEdtPlan;
   VTLegenda   *legenda;
   VTPatamares *patamares;
   TList       *lisVETVAL;
   TList       *lisEQP;

};
//---------------------------------------------------------------------------
#endif
