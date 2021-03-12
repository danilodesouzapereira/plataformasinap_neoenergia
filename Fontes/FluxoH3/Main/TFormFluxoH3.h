//---------------------------------------------------------------------------
#ifndef TFormFluxoH3H
#define TFormFluxoH3H

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <ImgList.hpp>
#include <Menus.hpp>
#include <ToolWin.hpp>
#include <CheckLst.hpp>
#include <VCLTee.Chart.hpp>
#include <VCLTee.Series.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
#include <VclTee.TeeGDIPlus.hpp>
#include "TFormEdt.h"
#include <System.Actions.hpp>
#include <System.ImageList.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTEdt;
class VTEqpto;
class VTFluxo;
class VTBarra;
class VTLigacao;
class VTVetval;
class VTBarFH;
class VTLigFH;
class VTRedeFH;
class TImporta;


//---------------------------------------------------------------------------
class TFormFluxoH3 : public TFormEdt
{
__published:	// IDE-managed Components
   TAction *ActionFluxoHarmonico;
   TAction *ActionSelEqpto;
   TGroupBox *GBoxEqpto;
   TTreeView *TView;
   TGroupBox *GBoxChart;
   TSplitter *Splitter1;
   TChart *Chart;
   TFastLineSeries *Series1;
   TFastLineSeries *Series2;
   TFastLineSeries *Series3;
   TFastLineSeries *Series4;
   TFastLineSeries *Series5;
   TFastLineSeries *Series6;
   TFastLineSeries *Series7;
   TFastLineSeries *Series8;
   TToolBar *ToolBarFluxoH3;
   TToolButton *ButFluxoHarmonico;
   TToolButton *ButImportaArquivo;
   TAction *ActionImportaArquivo;
   TAction *ActionDBEscrita;
   TAction *ActionDBLeitura;
   TToolButton *ToolButton1;
   TToolButton *ToolButton2;
   TAction *ActionAjuda;
   TToolButton *ButRetiraCargaNL;
   TAction *ActionRetiraCargaNL;
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall ActionFluxoHarmonicoExecute(TObject *Sender);
   void __fastcall ActionEdtExecute(TObject *Sender);
   void __fastcall ActionSelEqptoExecute(TObject *Sender);
   void __fastcall TViewChange(TObject *Sender, TTreeNode *Node);
   void __fastcall ActionDBEscritaExecute(TObject *Sender);
   void __fastcall ActionDBLeituraExecute(TObject *Sender);
   void __fastcall ActionImportaArquivoExecute(TObject *Sender);
   void __fastcall ActionAjudaExecute(TObject *Sender);
   void __fastcall ActionRetiraCargaNLExecute(TObject *Sender);

public:		// User declarations
        __fastcall  TFormFluxoH3(TForm *form_owner, VTApl *apl_owner, TWinControl *parent);
        __fastcall ~TFormFluxoH3(void);

private:	//métodos
   void      __fastcall ChartInicia(VTEqpto *eqpto, TList *lisVET);
   void      __fastcall ChartInsereCurva(VTVetval *vetval, TChartSeries *serie);
   void      __fastcall Moldura(VTEqpto *eqpto=NULL);
   void      __fastcall FormIniciaPosicao(void);
   VTVetval* __fastcall MontaVetvalCorrenteEficaz(VTLigFH *ligFH, int ind_fase);
   VTVetval* __fastcall MontaVetvalCorrenteHarmonica(VTLigFH *ligFH, int ind_fase, int ind_freq);
   VTVetval* __fastcall MontaVetvalTensaoEficaz(VTBarFH *barFH, int ind_fase);
   VTVetval* __fastcall MontaVetvalTensaoHarmonica(VTBarFH *barFH, int ind_fase, int ind_freq);
   VTVetval* __fastcall MontaVetvalDHTensao(VTBarFH *barFH, int ind_fase, int ind_freq);
   VTVetval* __fastcall MontaVetvalDHTTensao(VTBarFH *barFH, int ind_fase);
   int       __fastcall RetiraCargaNL(bool confirma);
   void      __fastcall TViewIniciaBarra(VTBarra *barra);
   void      __fastcall TViewIniciaLigacao(VTLigacao *ligacao);
   void      __fastcall TViewInsereVetval(TTreeNode *node_pai, VTVetval *vetval);
   bool      __fastcall TViewSelecionAlterado(void);

private:	//objetos externos
   VTFluxo *fluxoH3;

private:	//dados locais
   #define   NodeVetvalLevel  2
   TList    *lisCargaNL;
   TList    *lisEQP;
   TList    *lisVETSEL;
   TList    *lisVETVAL;
   VTApl    *apl;
   VTEdt    *edt;
   VTRedeFH *redeFH;
   TImporta *importa;
   double *vetorDHTensaoTotal[];
};

//---------------------------------------------------------------------------
#endif
//eof
