//---------------------------------------------------------------------------

#ifndef TFormEnergiaPassanteH
#define TFormEnergiaPassanteH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <System.Actions.hpp>
#include <System.ImageList.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.ToolWin.hpp>
//---------------------------------------------------------------------------
class VTAgregada;
//---------------------------------------------------------------------------
class TFormEnergiaPassante : public TForm
{
	__published:	// IDE-managed Components
	 TImageList *ImageList;
	 TActionList *ActionList;
	 TAction *ActionFechar;
	 TAction *ActionSalvaRelatorio;
	 TSaveDialog *SaveDialog;
	 TToolBar *ToolBar;
	 TToolButton *ToolButton1;
	 TToolButton *ButExporta;
	 TListView *LViewEnergiaPassante;
	void __fastcall ActionSalvaRelatorioExecute(TObject *Sender);
	void __fastcall ActionFecharExecute(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall LViewEnergiaPassanteColumnClick(TObject *Sender, TListColumn *Column);
	void __fastcall LViewEnergiaPassanteCompare(TObject *Sender, TListItem *Item1, TListItem *Item2, int Data, int &Compare);

//    public:  //property
//	 __property TDateTime Date = {read=PD.date, write=PD.date};

	private:	// User declarations
	public:      // User declarations
	 __fastcall  TFormEnergiaPassante(TComponent *Owner, VTApl *apl_owner, TWinControl *parent);
	 __fastcall ~TFormEnergiaPassante(void);
	 void __fastcall MontaRelatorioEnergiaPassante(TDateTime date);

	private:      //métodos
    AnsiString __fastcall  FormataEnergia(double energia_mwh);
	void __fastcall FormIniciaPosicao(void);
	bool __fastcall LeArquivoMedicao(void);
	void __fastcall LViewMedicaoInicia(void);
	bool __fastcall ValidaMedicao(void);

	private:   //objetos externos
	 VTApl       *apl;

	private:   //dados acessados via proeprty
	 struct{
		  TDateTime date;
	 }PD;

	private:   //dados locais
	 struct{//controle de ordenação de ListView
		TListColumn *column;
		bool         ascendente;
	 }LViewSort;
	 double     fat_mult;
	 VTAgregada *agregada;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormEnergiaPassante *FormEnergiaPassante;
//---------------------------------------------------------------------------
#endif
