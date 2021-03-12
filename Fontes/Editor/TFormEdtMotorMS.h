//---------------------------------------------------------------------------
#ifndef TFormEdtMotorMSH
#define TFormEdtMotorMSH

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
class VTMotor;

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/index.cfm#tasks/14887876
class TFormEdtMotorMS : public TForm
{
__published:   // IDE-managed Components
   TImageList *ImageList1;
   TActionList *ActionList1;
   TAction *ActionCancela;
   TAction *ActionConfirma;
   TAction *ActionRetira;
   TToolBar *ToolBar2;
   TToolButton *butConfirma;
   TToolButton *butCancela;
   TToolButton *butRetira;
   TLabel *Label1;
   TLabel *Label2;
   TLabel *Label3;
   TEdit *EditCorPartida;
   TEdit *EditCorNominal;
   TEdit *EditCodigo;
   TLabel *Label4;
   TEdit *EditFatPot;

   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall ActionConfirmaExecute(TObject *Sender);
   void __fastcall ActionRetiraExecute(TObject *Sender);

public:      // métodos
        __fastcall  TFormEdtMotorMS(TComponent* Owner, VTApl *apl, VTMotor* motor);
        __fastcall ~TFormEdtMotorMS(void);
   void __fastcall  OpcaoRetira(bool enabled);

private:    //métodos
   void __fastcall ExibeDadosMotor(void);
   void __fastcall FormIniciaPosicao(void);
   bool __fastcall LeDadosMotor(void);

private: //objetos externos
   VTApl    *apl;
   VTMotor  *motor;

private:    // dados locais

};
//---------------------------------------------------------------------------
#endif
