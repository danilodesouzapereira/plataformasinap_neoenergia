//---------------------------------------------------------------------------

#ifndef TFormAlteraFaixaH
#define TFormAlteraFaixaH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <System.Actions.hpp>
#include <System.ImageList.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.ToolWin.hpp>

//---------------------------------------------------------------------------
class VTApl;
class TNivelTensao;
class TRelatorioTensao;

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/16082236
class TFormAlteraFaixa : public TForm
{
__published:	// IDE-managed Components
	TImageList *ImageList1;
	TActionList *ActionList1;
	TAction *ActionConfirma;
	TAction *ActionCancela;
	TToolBar *ToolBar1;
	TToolButton *ToolButton2;
	TToolButton *ToolButton3;
	TRadioGroup *RGroupNivel;
	TStringGrid *SGridValor;
	void __fastcall ActionCancelaExecute(TObject *Sender);
	void __fastcall ActionConfirmaExecute(TObject *Sender);
	void __fastcall RGroupNivelClick(TObject *Sender);

public:		// User declarations
	__fastcall TFormAlteraFaixa(TComponent* Owner, VTApl *apl, TRelatorioTensao *relatorio_tensao);
	__fastcall ~TFormAlteraFaixa(void);

private:
	void __fastcall SGridInicia(TNivelTensao *nivel);
	bool __fastcall StringGridCellToDouble(int col, int row, double &valor);
	bool __fastcall ValidaValores(void);

private: // dados externos
	VTApl *apl;
	TRelatorioTensao *relatorio_tensao;

private: //dados locais
	TNivelTensao *nivel_ativo, *nivel_at, *nivel_mt, *nivel_bt;
    int ind_nivel;

};

//---------------------------------------------------------------------------
#endif
