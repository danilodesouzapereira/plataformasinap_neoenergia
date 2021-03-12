//---------------------------------------------------------------------------

#ifndef TFormConexoesRedesH
#define TFormConexoesRedesH

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
#include <Vcl.Dialogs.hpp>
//---------------------------------------------------------------------------
class VTApl;
class VTEqpto;
class VTProgresso;

//---------------------------------------------------------------------------
class TFormConexoesRedes : public TForm
{
__published:	// IDE-managed Components
	TImageList *ImageList;
	TToolBar *ToolBar;
	TToolButton *butEdt;
	TToolButton *ToolButton5;
	TActionList *ActionList;
	TAction *ActionFechar;
	TAction *ActionExportar;
	TGroupBox *GroupBox1;
	TListView *LViewConexoes;
	TSaveDialog *SaveDialog;
	void __fastcall ActionExportarExecute(TObject *Sender);
	void __fastcall ActionFecharExecute(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall LViewConexoesDblClick(TObject *Sender);
	void __fastcall LViewConexoesSelectItem(TObject *Sender, TListItem *Item, bool Selected);
	void __fastcall LViewConexoesColumnClick(TObject *Sender, TListColumn *Column);
	void __fastcall LViewConexoesCompare(TObject *Sender, TListItem *Item1, TListItem *Item2, int Data, int &Compare);


private:	// User declarations
	void __fastcall Executa(void);
	void __fastcall Moldura(VTEqpto *eqpto);
	void __fastcall Zoom(VTEqpto *eqpto);

public:		// User declarations
	__fastcall TFormConexoesRedes(TComponent* Owner, VTApl *apl_owner);
	__fastcall ~TFormConexoesRedes(void);


private: //obj externo
	VTApl *apl;

private: //obj local
	TList *lisEQP, *lisLIG;
	VTProgresso *progresso;
	struct{
		 TShiftState shift_state;
		 }zoom;
	struct{//estrutura p/ controle da ordenação do LView
		  TListColumn *Column;
		  bool        OrdemCrescente;
		  }LViewSort;
};

//---------------------------------------------------------------------------
#endif
