//---------------------------------------------------------------------------
#ifndef TFormAnaliseGeralH
#define TFormAnaliseGeralH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <Dialogs.hpp>
#include <System.Actions.hpp>
#include <System.ImageList.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTAlternativa;
class VTMulta;
class VTPerfilAlt;

//---------------------------------------------------------------------------
class TFormAnaliseGeral : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *GroupBox1;
	TStringGrid *SGridAnalise;
	TToolBar *ToolBar1;
	TToolButton *ToolButtonExporta;
	TActionList *ActionList1;
	TImageList *ImageList1;
	TAction *ActionExporta;
	TSaveDialog *SaveDialogAnalise;
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall ActionExportaExecute(TObject *Sender);

public:		// User declarations
	__fastcall TFormAnaliseGeral(TComponent* Owner, VTApl *apl);
	__fastcall ~TFormAnaliseGeral(void);

private:	//objetos externos
	VTApl *apl;
	
private:	// User declarations
	void     __fastcall Cabecalho(void);
	void     __fastcall CalculaCustoAlternativa(VTAlternativa* alternativa);
	void     __fastcall Inicia(void);
   double   __fastcall MultaTotal(VTMulta *multa, VTPerfilAlt* perfil_alt);
	void     __fastcall RecalculaPreencheGrid(int linha, VTAlternativa *alternativa);
};

//---------------------------------------------------------------------------
#endif
//eof

