//---------------------------------------------------------------------------
#ifndef TFormInsereMedicaoH
#define TFormInsereMedicaoH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <System.Actions.hpp>
#include <System.ImageList.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.ToolWin.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTAjusta;
class VTCanal;
class VTChave;
class VTMedidor;
class VTRede;

//---------------------------------------------------------------------------
class TFormInsereMedicao : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *gboxPQ;
	TStringGrid *sgridPQ;
	TGroupBox *GroupBox3;
	TComboBox *cboxRedeManual;
	TImageList *ImageList;
	TActionList *ActionList;
	TAction *ActionFecha;
	TAction *ActionConfirma;
	TToolBar *ToolBar1;
	TToolButton *ToolButton2;
	TToolButton *ToolButton1;
	TAction *ActionRedeChange;
	void __fastcall cboxRedeManualClick(TObject *Sender);
	void __fastcall ActionFechaExecute(TObject *Sender);
	void __fastcall ActionConfirmaExecute(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall ActionRedeChangeExecute(TObject *Sender);

public:		// User declarations
	__fastcall TFormInsereMedicao(TComponent* Owner, VTApl *apl, VTAjusta *ajusta);
	__fastcall ~TFormInsereMedicao(void);

private:	// User declarations
	bool __fastcall AtualizaMedidor(void);
	void __fastcall AtualizaSgrid(void);
	void __fastcall CBoxRedeManualInicia(void);
	void __fastcall CompletaDadosCanal(VTCanal *canal);
	void __fastcall CopiaFluxoParaMedido(void);
	bool __fastcall CurvaEstimadaIgualMedida(VTMedidor *medidor);
	AnsiString __fastcall DeterminaP(VTMedidor *medidor, int ind_pat);
	AnsiString __fastcall DeterminaQ(VTMedidor *medidor, int ind_pat);
	void __fastcall IniciaLisMed(void);
	void __fastcall IniciaMed(void);
	VTChave* __fastcall ExisteChave(VTRede *rede);
	bool __fastcall ExisteGerador(void);
	VTMedidor* __fastcall ExisteMedDJ(VTRede *rede);
	VTMedidor* __fastcall ExisteMedSemCanal(VTRede *rede);
	bool __fastcall RetiraMedidoresInalterados(void);
	void __fastcall ValidaCanais(VTMedidor *medidor, VTChave *chave);

private:  //obj externo
	VTApl *apl;
	VTAjusta *ajusta;
    VTRede *ult_rede;

private: //obj local
	TList *lisMED;

};

//---------------------------------------------------------------------------
#endif
