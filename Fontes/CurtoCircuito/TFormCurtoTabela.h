// ---------------------------------------------------------------------------
#ifndef TFormCurtoTabelaH
#define TFormCurtoTabelaH

// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <Dialogs.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>

// ---------------------------------------------------------------------------
class VTApl;
class VTBarra;
class VTEqpto;
class VTRedeCC;
class VTLigCC;

// ---------------------------------------------------------------------------
class TFormCurtoTabela : public TForm
{
__published: // IDE-managed Components

	TComboBox *CBoxDefeito;
	TPageControl *PageControl;
	TTabSheet *TabSheetPtoDefeito;
	TTabSheet *TabSheetContribuicao;
	TTabSheet *TabSheetTensao;
	TTabSheet *TabSheetCorrente;
	TListView *LViewDefIcc;
	TListView *LViewDefVcc;
	TComboBox *CBoxUnidTensao;
	TComboBox *CBoxUnidCorrente;
	TListView *LViewBarVcc;
	TListView *LViewSupIcc;
	TListView *LViewLigIcc;
	TSaveDialog *SaveDialog;
	TPanel *Panel;
	TBitBtn *ButRelatorio;
	TTabSheet *TabSheetPotencias;
	TListView *LViewLigScc;
	TComboBox *CBoxUnidPotencia;

	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall PageControlChange(TObject *Sender);
	void __fastcall LViewChange(TObject *Sender, TListItem *Item, TItemChange Change);
	void __fastcall CBoxChange(TObject *Sender);
	void __fastcall LviewSelecionaZoom(TObject *Sender);
	void __fastcall ButRelatorioClick(TObject *Sender);
	void __fastcall SaveDialogTypeChange(TObject *Sender);

public: // User declarations
	__fastcall TFormCurtoTabela(TComponent* Owner, TWinControl *parent, VTApl *apl);
	__fastcall ~TFormCurtoTabela(void);
	void __fastcall EqptoDefeito(VTEqpto *eqpto);
	void __fastcall EqptoSelecionado(VTEqpto *eqpto);
	int __fastcall WidthMinimo(void);

private: // métodos
	TListItem* __fastcall ExisteEqpto(TListView *LView, VTEqpto *eqpto);
	void __fastcall ExportaRelatorio(void);
	void __fastcall ExportaRelatorioExcel(AnsiString arq);
	void __fastcall ExportaRelatorioTexto(AnsiString arq);
	void __fastcall IniciaCBoxDefeito(void);
	void __fastcall IniciaLView(void);
	void __fastcall IniciaLViewBarVcc(void);
	void __fastcall IniciaLViewDefIcc(void);
	void __fastcall IniciaLViewDefVcc(void);
	void __fastcall IniciaLViewLigIcc(void);
	void __fastcall IniciaLViewLigIcc(VTLigCC *ligCC, int ind_def, int ind_bar);
	void __fastcall IniciaLViewLigScc(void);
	void __fastcall IniciaLViewLigScc(VTLigCC *ligCC, int ind_def, int ind_bar);
	void __fastcall Moldura(VTEqpto *eqpto = NULL);
	AnsiString __fastcall NomeDefeito(int ind_def);
	void __fastcall IniciaLViewSupIcc(void);

	// objetos externos
	VTApl *apl;
	VTRedeCC *redeCC;

	// dados locais
	int width_min;
};

// ---------------------------------------------------------------------------
#endif
// eof
