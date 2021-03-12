//---------------------------------------------------------------------------
#ifndef TFormEditaAlternativaH
#define TFormEditaAlternativaH

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
#include <System.ImageList.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTAlternativa;
class VTEstudo;
//---------------------------------------------------------------------------
class TFormEditaAlternativa : public TForm
{
__published:	// IDE-managed Components
	TToolBar *ToolBar1;
	TToolButton *butConfirma;
	TToolButton *butCancela;
	TGroupBox *GroupBox1;
	TEdit *EditNome;
	TGroupBox *GBoxJustificativa;
	TActionList *ActionList1;
	TAction *ActionCancela;
	TAction *ActionConfirma;
	TImageList *ImageList1;
	TRichEdit *RichEditJustificativa;
	void __fastcall ActionCancelaExecute(TObject *Sender);
	void __fastcall ActionConfirmaExecute(TObject *Sender);
    void __fastcall EditKeyPress(TObject *Sender, char &Key);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);

public:		// User declarations
	__fastcall TFormEditaAlternativa(TComponent* Owner, VTApl *apl_owner, VTAlternativa *alternativa);
	__fastcall ~TFormEditaAlternativa(void);
	__property AnsiString Nome 			= {read=PM_GetNome, write=PM_SetNome};
	__property AnsiString Justificativa = {read=PM_GetJustificativa, write=PM_SetJustificativa};

private: // User declarations
   AnsiString __fastcall PM_GetNome(void);
   void       __fastcall PM_SetNome(AnsiString texto);
   AnsiString __fastcall PM_GetJustificativa(void);
   void       __fastcall PM_SetJustificativa(AnsiString texto);
   VTEstudo*  __fastcall EstudoAtual(void);
   bool       __fastcall LicencaCopel(void);

private: //obj externos
	VTAlternativa *alternativa;
	VTApl *apl;

};

//---------------------------------------------------------------------------
#endif
