//---------------------------------------------------------------------------
#ifndef TFormCaboH
#define TFormCaboH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <System.Actions.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTCabo;

//---------------------------------------------------------------------------
class TFormCabo : public TForm
{
__published:	// IDE-managed Components
   TToolBar *ToolBar1;
   TToolButton *butConfirma;
   TToolButton *butCancela;
   TImageList *ImageList1;
   TActionList *ActionList1;
   TAction *ActionCancela;
   TAction *ActionConfirma;
   TRadioGroup *RGroupTipo;
   TGroupBox *GBoxZ0Z1;
   TStringGrid *SGridZ;
   TAction *ActionAltCfg;
   TGroupBox *GroupBox2;
   TEdit *EditCodigo;
   TLabel *Label2;
   TEdit *EditIadm;
   TLabel *Label1;
   TLabel *Label3;
   TEdit *EditVmin;
   TLabel *Label4;
   TEdit *EditVmax;
   TGroupBox *GBoxR_GMR;
   TLabel *Label5;
   TEdit *EditR;
   TLabel *Label6;
   TEdit *EditGMR;
   TPanel *Panel1;
   TPanel *Panel2;
   TPanel *Panel3;
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall ActionConfirmaExecute(TObject *Sender);
   void __fastcall ActionAltCfgExecute(TObject *Sender);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);

public:		// User declarations
   __fastcall TFormCabo(TComponent* Owner, VTApl *apl, VTCabo *cabo);
   __fastcall ~TFormCabo(void);

private:	//métodos
   void    __fastcall ExibeDados(void);
   void    __fastcall ExibeDadosZ0_Z1(void);
   VTCabo* __fastcall ExisteCabo(AnsiString codigo);
   bool    __fastcall LeDados(void);
   bool    __fastcall LeDadosR_GMR(void);
   bool    __fastcall LeDadosZ0_Z1(void);

private:	//objetos externos
   VTApl  *apl;
   VTCabo *cabo_orig;

private:	//dados locais
   VTCabo *cabo;


};

//---------------------------------------------------------------------------
#endif
//eof
