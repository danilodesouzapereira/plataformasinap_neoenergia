// ---------------------------------------------------------------------------

#ifndef TFormEliminaFusivelH
#define TFormEliminaFusivelH
// ---------------------------------------------------------------------------
#include <stdio.h>
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>
// ---------------------------------------------------------------------------
class VTApl;
class VTCalcIndCont;
class VTBuscaProt;
// ---------------------------------------------------------------------------
class TFormEliminaFusivel : public TForm
{
__published: // IDE-managed Components
	TActionList *ActionList1;
	TAction *ActionAvaliaDefeito;
	TPageControl *PageControl1;
	TTabSheet *TabSheet1;
	TButton *Button2;
	TButton *Button1;
	TButton *Button3;
	TButton *butEliminaNADuplic;
	TButton *Button4;
	TRichEdit *rchRes;
	TTabSheet *TabSheet2;
	TTreeView *tvChvProt;
	TButton *Button5;
	TAction *ActionArvoreProt;
	TAction *ActionMostraRamoArvore;
	TAction *ActionImprimeResultados;
	TButton *Button6;

	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);
	void __fastcall butEliminaNADuplicClick(TObject *Sender);
	void __fastcall ActionArvoreProtExecute(TObject *Sender);
	void __fastcall ActionAvaliaDefeitoExecute(TObject *Sender);
	void __fastcall ActionMostraRamoArvoreExecute(TObject *Sender);
	void __fastcall ActionImprimeResultadosExecute(TObject *Sender);
private: // User declarations
		public : // User declarations
	__fastcall TFormEliminaFusivel(TComponent* Owner, VTApl* apl_owner, TWinControl *parent);
	__fastcall ~TFormEliminaFusivel(void);
	void __fastcall EliminaChaves(void);
	void __fastcall EliminaChaves1(void);
	void __fastcall IniciaLisChavesFunde(TList *lisChavesFunde);
	void __fastcall IniciaLisChavesNA(void);
	bool __fastcall InsereChavesNA(AnsiString Chave, AnsiString TIPO, AnsiString Circuito_1,
		AnsiString Circuito_2, AnsiString Barra_1, AnsiString Barra_2);
        void   __fastcall IniciaTipoProt(TList  *lisTipoEqProt);

	VTApl *apl;
	VTCalcIndCont *calcIndCont;
      VTBuscaProt   *buscaProt;
	TList *lisChavesFunde;
	TList *lisChavesNA;
};

// ---------------------------------------------------------------------------
extern PACKAGE TFormEliminaFusivel *FormEliminaFusivel;
// ---------------------------------------------------------------------------
#endif
