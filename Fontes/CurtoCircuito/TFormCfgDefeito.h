//---------------------------------------------------------------------------
#ifndef TFormCfgDefeitoH
#define TFormCfgDefeitoH

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

//---------------------------------------------------------------------------
class TDefeito;
class TDefeitos;

//---------------------------------------------------------------------------
class TFormCfgDefeito : public TForm
{
__published:	// IDE-managed Components
   TToolBar *ToolBar1;
   TToolButton *butConfirma;
   TToolButton *butCancela;
   TImageList *ImageList;
   TActionList *ActionList;
   TAction *ActionCancela;
   TAction *ActionConfirma;
   TAction *ActionRetira;
   TToolButton *butRetira;
   TGroupBox *GroupBox1;
   TEdit *EditBarra;
   TRadioGroup *RadioGroupTipoDefeito;
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall ActionConfirmaExecute(TObject *Sender);
   void __fastcall ActionRetiraExecute(TObject *Sender);

public:  // User declarations
   __fastcall  TFormCfgDefeito(TComponent* Owner, TDefeitos *defeitos, TDefeito *defeito);
   __fastcall ~TFormCfgDefeito(void);

private: // métodos
   void __fastcall ExibeCodigoBarra(void);
   void __fastcall ExibeTipoDefeito(void);

private: //objetos externos
   TDefeito  *defeito;
   TDefeitos *defeitos;
};

//---------------------------------------------------------------------------
#endif
//eof

