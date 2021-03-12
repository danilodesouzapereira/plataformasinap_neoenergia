// ---------------------------------------------------------------------------

#ifndef TFormEditaParamProtJusH
#define TFormEditaParamProtJusH
// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
#include <stdio.h>
#include <ImgList.hpp>
#include <System.Actions.hpp>
#include <System.ImageList.hpp>
// ---------------------------------------------------------------------------
class VTApl;
class VTBloco;
class VTTipoChave;
class VTBlocoRad;
class VTRede;

// ---------------------------------------------------------------------------
class TFormEditaParamProtJus : public TForm
{
__published: // IDE-managed Components

	TToolBar *ToolBar;
	TActionList *ActionList1;
	TAction *ActionAtualizaParamEqpto;
	TAction *ActionMostraBlocos;
	TImageList *ImageList;
	TToolButton *ToolButton1;
	TPageControl *PageControl1;
	TTabSheet *TabSheet1;
	TTabSheet *TabSheet2;
	TStringGrid *sgridBlocosProt;
	TStringGrid *sgridCargasBT;
	TAction *ActionMostraCargas;
	TToolButton *ToolButton2;
	TAction *ActionMostraTaxas;
	TTabSheet *TabSheet3;
	TGroupBox *GroupBox1;
	TTreeView *tvReligadas;
	TAction *ActionMostraBlocoReliga;
	TStatusBar *statusBar;
	TAction *ActionMostraBlocosPiscadas;

	void __fastcall ActionAtualizaParamEqptoExecute(TObject *Sender);
	void __fastcall ActionMostraBlocosExecute(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall ActionMostraCargasExecute(TObject *Sender);
	void __fastcall ActionMostraTaxasExecute(TObject *Sender);
	void __fastcall ActionMostraBlocoReligaExecute(TObject *Sender);
	void __fastcall ActionMostraBlocosPiscadasExecute(TObject *Sender);

public: // User declarations
	__fastcall TFormEditaParamProtJus(TComponent* Owner, VTApl* apl_owner, TWinControl *parent);
	__fastcall ~TFormEditaParamProtJus(void);

private:
	void __fastcall IniciaTVReligadas(void);
	void __fastcall IniciaTVReligadasRede(TTreeNode *nodeRede, VTRede *rede);
	void __fastcall IniciaLisChavesProt(void);
	// void __fastcall IniciaLisParamEqpto(void);
	void __fastcall IniciaTListParamChaves(void);
	void __fastcall IniciaTListParamCargas(void);
	double __fastcall ComprimentoBloco(VTBloco *bloco);
	void __fastcall GravaArqParamEqpto(void);
	AnsiString __fastcall LeDado(FILE* fp, bool &flagEOF);
	int __fastcall NCons(TList *lisCargas);
	void __fastcall GeraPiscadas(VTBlocoRad* blocoRadReliga, double &nfalhasTot,
		double &nfalhasPisca);

	// dados externos
	VTApl *apl;
	TList *lisParamEqpto;
	TList *lisChavesProt;
};

// ---------------------------------------------------------------------------
extern PACKAGE TFormEditaParamProtJus *FormEditaParamProtJus;
// ---------------------------------------------------------------------------
#endif
