//---------------------------------------------------------------------------
#ifndef TFormSuporteH
#define TFormSuporteH

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
class VTSuporte;

//---------------------------------------------------------------------------
class TFormSuporte : public TForm
{
__published:	// IDE-managed Components
   TToolBar *ToolBar1;
   TToolButton *butConfirma;
   TToolButton *butCancela;
   TImageList *ImageList1;
   TActionList *ActionList1;
   TAction *ActionCancela;
   TAction *ActionConfirma;
   TAction *ActionAltNumPto;
   TGroupBox *GroupBox1;
   TLabel *LabelCod;
   TLabel *LabelVmin;
   TLabel *LabelVmax;
   TEdit *EditCodigo;
   TEdit *EditVmin;
   TEdit *EditVmax;
   TRadioGroup *RGroupLocal;
   TGroupBox *GBoxPto;
   TStringGrid *SGridPto;
   TLabel *Label1;
   TComboBox *CBoxNumPto;
   TPanel *Panel1;
   TPanel *Panel2;
   TPanel *Panel3;
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall ActionConfirmaExecute(TObject *Sender);
   void __fastcall ActionAltNumPtoExecute(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormShow(TObject *Sender);

public:		// User declarations
   __fastcall TFormSuporte(TComponent* Owner, VTApl *apl, VTSuporte *suporte);
   __fastcall ~TFormSuporte(void);

private:	//métodos
   void       __fastcall AjustaNumPtoFixacao(int num_pto);
   void       __fastcall ExibeDados(void);
   void       __fastcall ExibePtoFixacao(void);
   VTSuporte* __fastcall ExisteSuporte(AnsiString codigo);
   bool       __fastcall LeDados(void);
   bool       __fastcall LeDadosPtoFixacao(int num_pto);

private:	//objetos externos
   VTApl     *apl;
   VTSuporte *suporte_orig;


private:	//locais
   VTSuporte *suporte;
};

//---------------------------------------------------------------------------
#endif
//eof
