// ---------------------------------------------------------------------------

#ifndef TFormMostraDefeitoH
#define TFormMostraDefeitoH
// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <ActnList.hpp>
#include <System.Actions.hpp>
// ---------------------------------------------------------------------------
class VTApl;
// n14 class VTCalcIndCont;
class VTChave;
class VTDefeito;

// ---------------------------------------------------------------------------
class TFormMostraDefeito : public TForm
{
__published: // IDE-managed Components

	TTreeView *tvVisDefeito;
	TSplitter *Splitter5;
	TTreeView *tvDefeito;
	TActionList *ActionList1;
	TAction *ActionMostraDefeito;
	TAction *ActionMostraDefeitoVis;
	TAction *ActionMostraBlocoDefeito;
	TTreeView *tvDefeito1;
	TSplitter *Splitter1;
	TAction *ActionMostraDefeito1;

	void __fastcall ActionMostraDefeitoExecute(TObject *Sender);
	void __fastcall ActionMostraDefeitoVisExecute(TObject *Sender);
	void __fastcall ActionMostraBlocoDefeitoExecute(TObject *Sender);
	void __fastcall ActionMostraDefeito1Execute(TObject *Sender);

public: // User declarations
	__fastcall TFormMostraDefeito(TComponent* Owner, TWinControl *parent, VTApl *apl,
		VTDefeito *pdef, TList *lisChavesAuto);
	__fastcall ~TFormMostraDefeito(void);

private: // User declarations
	bool __fastcall ChaveAutomatica(VTChave *pchave);
	void __fastcall DestacaEqptos(TList *lisBlocos, TColor color = clBlack);
	void __fastcall IniciaTreeView(void);
	void __fastcall IniciaTreeView00(void);

	VTApl *apl;
	VTDefeito *pdef;
	TList *lisChavesAuto;
	// n14 VTCalcIndCont *calcIndCont;

};

// ---------------------------------------------------------------------------
extern PACKAGE TFormMostraDefeito *FormMostraDefeito;
// ---------------------------------------------------------------------------
#endif
