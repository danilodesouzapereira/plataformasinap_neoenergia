//---------------------------------------------------------------------------
#ifndef TFormCarroH
#define TFormCarroH

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
#include <System.ImageList.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTCarga;
class VTCurva;

//---------------------------------------------------------------------------
class TFormCarro : public TForm
{
__published:   // IDE-managed Components
   TToolBar *ToolBar2;
   TToolButton *butConfirma;
   TToolButton *butCancela;
   TImageList *ImageList1;
   TActionList *ActionList1;
   TAction *ActionCancela;
   TAction *ActionConfirma;
   TLabel *Label1;
   TLabel *Label2;
   TLabel *Label3;
   TComboBox *CBoxQtde;
   TComboBox *CBoxHoraIni;
   TLabel *Label4;
   TComboBox *CBoxNumHora;
   TToolButton *ButRetira;
   TToolButton *ToolButton2;
   TAction *ActionRetira;
	TEdit *edtPotencia;
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall ActionConfirmaExecute(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall ActionRetiraExecute(TObject *Sender);

public:      // User declarations
   __fastcall  TFormCarro(TComponent* Owner, VTApl *apl);
   __fastcall ~TFormCarro(void);

private:   //métodos
   VTCarga* __fastcall CriaCargaCarro(VTCarga *carga_res);
   VTCurva* __fastcall CriaCurvaCarga(void);
   bool     __fastcall InsereCargaCarro(void);
   bool     __fastcall LeDados(void);
   void     __fastcall RetiraCargaCarro(void);
   bool     __fastcall ValidaDados(void);

private:   //objetos externos
   VTApl *apl;

private:   //dados locais
   TList *lisEQP;
   TList *lisCAR;
   int    hora_ini, num_hora;
   double qtde_carro;
   double pot_kw;
};

//---------------------------------------------------------------------------
#endif
//eof

