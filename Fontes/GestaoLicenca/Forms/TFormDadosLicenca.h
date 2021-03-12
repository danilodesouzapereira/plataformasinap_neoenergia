//---------------------------------------------------------------------------
#ifndef TFormDadosLicencaH
#define TFormDadosLicencaH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.ToolWin.hpp>
#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>
#include <System.ImageList.hpp>

//---------------------------------------------------------------------------
class VTApl;

//---------------------------------------------------------------------------
class TFormDadosLicenca : public TForm
{
__published:	// IDE-managed Components
	TToolBar *ToolBar;
	TToolButton *ButFecha;
	TImageList *ImageList;
	TActionList *ActionList1;
	TAction *ActionFecha;
	TListView *ListViewLicenca;
	TPageControl *PageControl;
	TTabSheet *TabSheetLicenca;
	TTabSheet *TabSheetOperacoes;
	TListView *ListViewOperacao;
	void __fastcall ActionFechaExecute(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);

public:		// User declarations
	__fastcall TFormDadosLicenca(TComponent* Owner, VTApl *apl);
	__fastcall ~TFormDadosLicenca(void);

private:	//métodos
   void __fastcall DadosDaLicenca(void);
   void __fastcall DadosOperacoes(void);
   AnsiString __fastcall DefineValidade(int validade);

private:	//objetos externos
   VTApl *apl;

};

//---------------------------------------------------------------------------
#endif
