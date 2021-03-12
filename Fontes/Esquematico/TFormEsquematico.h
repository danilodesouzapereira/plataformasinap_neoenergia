//---------------------------------------------------------------------------
#ifndef TFormEsquematicoH
#define TFormEsquematicoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "TFORMEDT.h"
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <ImgList.hpp>
#include <Menus.hpp>
#include <ToolWin.hpp>
#include <CheckLst.hpp>
#include "TFormEdt.h"
#include <System.Actions.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTCluster;
class VTEqpto;
class VTMRede;
class VTRede;
class VTEsquematico;

//---------------------------------------------------------------------------
class TFormEsquematico : public TFormEdt
{
__published:	// IDE-managed Components
   TGroupBox *GBoxRedes;
   TSplitter *Splitter1;
   TGroupBox *GBoxReducao;
   TTreeView *TViewRede;
   TCheckListBox *CLBoxReducao;
   TAction *ActionEsq2AlignHLR;
   TAction *ActionEsq2AlignHRL;
   TAction *ActionEsq2AlignVBT;
   TAction *ActionEsq2AlignVTB;
   TAction *ActionEsq2AlignNone;
   TToolButton *ButEsq1;
   TToolButton *ButGeo;
   TAction *ActionGeo;
   TToolButton *ButEsq2;
   TAction *ActionEsq2;
   TPopupMenu *PopupEsqAlign;
   TMenuItem *MenuEsq2HLR;
   TMenuItem *MenuEsq2HRL;
   TMenuItem *MenuEsq2VTB;
   TMenuItem *MenuEsq2VBT;
   TToolButton *ToolButton1;
   TAction *ActionEsq1;
   void __fastcall FormShow(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall TViewRedeChange(TObject *Sender, TTreeNode *Node);
   void __fastcall TViewRedeChanging(TObject *Sender, TTreeNode *Node,
          bool &AllowChange);
   void __fastcall ActionEsq2AlignHRLExecute(TObject *Sender);
   void __fastcall CLBoxReducaoClickCheck(TObject *Sender);
   void __fastcall ActionEsq2AlignHLRExecute(TObject *Sender);
   void __fastcall ActionEsq2AlignVBTExecute(TObject *Sender);
   void __fastcall ActionEsq2AlignVTBExecute(TObject *Sender);
   void __fastcall ActionEsq2AlignNoneExecute(TObject *Sender);
   void __fastcall ActionGeoExecute(TObject *Sender);
   void __fastcall ActionEsq2Execute(TObject *Sender);
   void __fastcall ActionEsq1Execute(TObject *Sender);

public:		// User declarations
   __fastcall TFormEsquematico(TForm *Owner, VTApl *apl, TWinControl *parent);
   __fastcall ~TFormEsquematico(void);

private:	//objetos externos
   void     __fastcall CLBoxReducaoInicia(void);
   void     __fastcall DefineVisualizacaoRedes(bool visible);
   void     __fastcall DefineVisualizacaoReducoes(bool visible);
   void     __fastcall DefineVisualizacaoReducoesVizinhas(VTRede *rede, bool visible);
   void     __fastcall EscondeGrafico(void);
   void     __fastcall ExibeEsquematicoGeral(void);
   void     __fastcall ExibeGrafico(void);
   void     __fastcall ExibeGraficoRede(VTRede *rede);
   int      __fastcall ExisteRedeSemCluster(TList *lisEXT, int tipo_rede);
   void     __fastcall SalvaCoordenadas(void);
   void     __fastcall TViewRedeInicia(void);
   VTRede*  __fastcall TViewRedeSelecionada(void);

private:	//objetos externos
   VTApl *apl;

private:	//dados locais
   VTEsquematico *esquematico;
   TList         *lisEQP;
   struct   {
            VTEqpto       *eqpto;
            TToolButton   *but_modo;
            }diag_ant;
};

//---------------------------------------------------------------------------
#endif
//eof

