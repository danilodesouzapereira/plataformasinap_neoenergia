//---------------------------------------------------------------------------
#ifndef TFormCfgPatH
#define TFormCfgPatH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ActnList.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
#include <Grids.hpp>
#include <ImgList.hpp>
#include "..\Geral\VTGeral.h"

//---------------------------------------------------------------------------
class VTGeral;
class VTBDado;

//---------------------------------------------------------------------------
class TformCfgPat : public TForm
{
__published:	// IDE-managed Components
   TActionList *ActionList1;
   TAction *ActionCancela;
   TAction *ActionConfirma;
   TAction *ActionDesfaz;
   TImageList *ImageList;
   TToolBar *ToolBar1;
   TToolButton *butDefault;
   TToolButton *butInclui;
   TToolButton *butSair;
   TPanel *panelPat;
   TLabel *lblNumPat;
   TComboBox *cboxNumPat;
   TStringGrid *gridPat;
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall ActionConfirmaExecute(TObject *Sender);
   void __fastcall ActionDesfazExecute(TObject *Sender);
   void __fastcall ActionEditaExecute(TObject *Sender);
   void __fastcall cboxNumPatClick(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall gridPatKeyPress(TObject *Sender, char &Key);

public:		// User declarations
   __fastcall TformCfgPat(TComponent* Owner, AnsiString dirBase, AnsiString dirRede);
   __fastcall TformCfgPat::~TformCfgPat(void);
   void __fastcall DefObjGeral(VTGeral *geral);

private:	// métodos
   void __fastcall ExibeDados(void);
   bool __fastcall LeDados(void);
   bool __fastcall ValHora(int hora);
   bool __fastcall ValNome(AnsiString nome);

   private:	// dados
   VTGeral     *geral;
   VTBDado     *bdado;
   strPAT      PAT;
   AnsiString  dirSISPLAN;
   bool        autorizado;
};

#endif
//---------------------------------------------------------------------------
//eof
