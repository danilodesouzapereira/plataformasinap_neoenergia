//---------------------------------------------------------------------------
#ifndef TFormNIXELPAH
#define TFormNIXELPAH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ToolWin.hpp>
#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Mask.hpp>
#include <System.ImageList.hpp>
#include <map>
#include <iostream>
#include <cassert>

//---------------------------------------------------------------------------
class VTApl;
class VTInterfaceNIX;
class VTRedes;
class VTMalhas;


//---------------------------------------------------------------------------
class TFormNIXELPA : public TForm
{
__published:	// IDE-managed Components
   TRichEdit *RichEdit;
   TToolBar *ToolBar1;
   TCheckBox *clboxPatamar;
   TToolButton *ToolButton1;
   TToolButton *ToolButton2;
   TActionList *ActionList;
   TImageList *ImageList;
   TAction *ActionGeraDados;
   TAction *ActionSalvaArquivo;
   TToolButton *ToolButton3;
   TAction *ActionHelp;
   TGroupBox *GroupBox1;
   TCheckBox *cboxIAl;
   TSaveDialog *SaveDialog;
   TCheckBox *cboxIChave;
   TGroupBox *GroupBox2;
   TComboBox *comboPatamar;
   TGroupBox *gboxFatorDemanda;
   TCheckBox *cboxFDa4;
   TPanel *panelFDa4;
   TPanel *panelFDbt;
   TCheckBox *cboxFDbt;
   TEdit *editFDa4;
   TEdit *editFDbt;
	TToolButton *btnImportarNIX;
	TAction *ActionImportaDados;
	TOpenDialog *OpenDialog;
	TGroupBox *gboxELPA;
	TComboBox *cboxCarreg;
	TLabel *lblCarreg;
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);

   void __fastcall ActionHelpExecute(TObject *Sender);
   void __fastcall ActionGeraDadosExecute(TObject *Sender);
   void __fastcall ActionSalvaArquivoExecute(TObject *Sender);
   void __fastcall clboxPatamarClick(TObject *Sender);
   void __fastcall comboPatamarChange(TObject *Sender);
   void __fastcall cboxIAlClick(TObject *Sender);
   void __fastcall cboxFDbtClick(TObject *Sender);
   void __fastcall cboxFDa4Click(TObject *Sender);
   void __fastcall editFDbtMouseActivate(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y, int HitTest, TMouseActivate &MouseActivate);
   void __fastcall editFDa4MouseActivate(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y, int HitTest, TMouseActivate &MouseActivate);
	void __fastcall ActionImportaDadosExecute(TObject *Sender);

public:		// User declarations
   __fastcall  TFormNIXELPA(TComponent *Owner, VTApl *apl_owner, TWinControl *parent);
   __fastcall ~TFormNIXELPA(void);

private:	//métodos
	void  __fastcall ConfiguraInterfaceNix(void);
	bool  __fastcall ContemMalha(void);
	bool __fastcall ContemMalhaMT(VTMalhas *malhas);
	void  __fastcall DesabilitaCombo(void);
	bool  __fastcall ExisteRedeBTEquivalente(void);
	bool  __fastcall IniciaCBoxPatamar(void);
	//void  __fastcall IniciaForm(void);
	void  __fastcall IniciaMapaParametro(void);
	bool __fastcall SomenteMalhaDeTrafo(VTMalhas *malhas);
	bool  __fastcall ValidaFatorDemandaA4(void);
	bool  __fastcall ValidaFatorDemandaTrafo(void);
	bool  __fastcall ValidaInputForm(void);

private:	//objetos externos
   VTApl *apl;

private:	//dados
   VTInterfaceNIX *nix;
   TList          *lisCARREGADAS;
   int            index_patamar;
   int            index_pat_usado;
	double         fdA4, fdTrafo;
	std::map<int, AnsiString> mapa;
};

//---------------------------------------------------------------------------
#endif
//eof

