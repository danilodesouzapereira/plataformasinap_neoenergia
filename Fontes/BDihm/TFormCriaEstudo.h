//---------------------------------------------------------------------------
#ifndef TFormCriaEstudoH
#define TFormCriaEstudoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <System.Actions.hpp>

//---------------------------------------------------------------------------
class VTApl;

//---------------------------------------------------------------------------
class TFormCriaEstudo : public TForm
{
__published:   // IDE-managed Components
   TToolBar *ToolBar1;
   TToolButton *butConfirma;
   TToolButton *butCancela;
   TImageList *ImageList1;
   TActionList *ActionList1;
   TAction *ActionCancela;
   TAction *ActionConfirma;
   TEdit *EditCodigo;
   TLabel *Label1;
   TLabel *Label2;
   TLabel *Label3;
   TDateTimePicker *DateTimePicker;
   TEdit *EditAutor;
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall ActionConfirmaExecute(TObject *Sender);
   void __fastcall EditCodigoKeyPress(TObject *Sender, char &Key);

public:   //property
   __property AnsiString Autor  = {read=PM_GetAutor};
   __property AnsiString Codigo = {read=PM_GetCodigo};
   __property TDateTime  Data   = {read=PM_GetData};

public:      // User declarations
   __fastcall  TFormCriaEstudo(TComponent* Owner, VTApl *apl);
   __fastcall ~TFormCriaEstudo(void);

private:   // métodos
   AnsiString __fastcall DefineCodigoEstudo(void);
   AnsiString __fastcall PM_GetAutor(void);
   AnsiString __fastcall PM_GetCodigo(void);
   TDateTime  __fastcall PM_GetData(void);

private:  //objetos externos
   VTApl *apl;
};

//---------------------------------------------------------------------------
#endif
//eof

