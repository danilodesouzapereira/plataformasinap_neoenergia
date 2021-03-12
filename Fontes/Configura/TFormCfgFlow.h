//---------------------------------------------------------------------------
#ifndef TFormCfgFlowH
#define TFormCfgFlowH

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
#include <ImgList.hpp>
#include "..\Geral\VTGeral.h"

//---------------------------------------------------------------------------
class VTGeral;
class VTBDado;
class TformCfgFlow : public TForm
{
__published:	// IDE-managed Components
   TActionList *ActionList1;
   TAction *ActionExibeLbl;
   TGroupBox *gbCarga;
   TLabel *lblPotCon;
   TLabel *lblCorConst;
   TLabel *lblImpConst;
   TEdit *edtImpCon;
   TEdit *edtCorCon;
   TEdit *edtPotCon;
   TGroupBox *gbPrecisao;
   TLabel *lblIterMax;
   TLabel *lblToler;
   TEdit *edtIterMax;
   TEdit *edtToler;
   TGroupBox *gbFaixaI;
   TLabel *Label9;
   TLabel *Label10;
   TEdit *edtI2;
   TEdit *edtI1;
   TGroupBox *gbGrafI;
   TLabel *lblI1;
   TLabel *lblI3;
   TLabel *lblI2;
   TPanel *panCorI1;
   TPanel *panCorI2;
   TPanel *panCorI3;
   TGroupBox *gbFaixaV;
   TGroupBox *gbFaixaA;
   TLabel *Label7;
   TLabel *Label1;
   TEdit *edtV1;
   TEdit *edtV2;
   TGroupBox *gbFaixaB;
   TLabel *Label2;
   TLabel *Label5;
   TEdit *edtV3;
   TEdit *edtV4;
   TGroupBox *gbGrafV;
   TLabel *lblV1;
   TLabel *lblV2;
   TLabel *lblV5;
   TLabel *lblV3;
   TLabel *lblV4;
   TPanel *panCorV1;
   TPanel *panCorV2;
   TPanel *panCorV3;
   TPanel *panCorV4;
   TPanel *panCorV5;
   TToolBar *ToolBar1;
   TToolButton *butDefault;
   TToolButton *butInclui;
   TToolButton *butSair;
   TImageList *ImageList;
   TActionList *ActionList2;
   TAction *ActionConfirma;
   TAction *ActionCancela;
   TAction *ActionDefault;
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall ActionExibeLblExecute(TObject *Sender);
   void __fastcall ActionDefaultExecute(TObject *Sender);
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall ActionConfirmaExecute(TObject *Sender);
   void __fastcall FormShow(TObject *Sender);

public:		// User declarations
   __fastcall TformCfgFlow(TComponent* Owner, AnsiString dirBase, AnsiString dirRede);
   __fastcall ~TformCfgFlow(void);

private:	// métodos
   void __fastcall ExibeDados(void);
   bool __fastcall LeDados(void);

private:	// dados
   VTGeral *geral;
   strFLOW FLOW;
   VTBDado *bdado;
};

#endif
//---------------------------------------------------------------------------
//eof
