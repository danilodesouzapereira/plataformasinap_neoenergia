//---------------------------------------------------------------------------
#ifndef TFormDistanciaH
#define TFormDistanciaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "TFormEdt.h"
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <ImgList.hpp>
#include <Menus.hpp>
#include <ToolWin.hpp>
#include <Dialogs.hpp>
#include <System.Actions.hpp>
#include <System.ImageList.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTArranjo;
class VTCaminho;
class VTEdt;
class VTEqpto;
class VTBarra;
class VTRede;

//---------------------------------------------------------------------------
class TFormDistancia : public TFormEdt
{
__published:   // IDE-managed Components
   TAction *ActionSelEqpto;
   TPageControl *PageControl;
   TTabSheet *TabSheetResumo;
   TTabSheet *TabSheetTrecho;
   TTabSheet *TabSheetTrafo;
   TTabSheet *TabSheetChave;
   TTabSheet *TabSheetCarga;
   TTabSheet *TabSheetEqpto;
   TListView *LViewResumo;
   TListView *LViewEqpto;
   TListView *LViewTrafo;
   TListView *LViewChave;
   TListView *LViewCarga;
   TListView *LViewTrecho;
   TToolButton *ToolButton1;
   TAction *ActionExporta;
   TSaveDialog *SaveDialog;
   TToolButton *ToolButton2;
   TToolButton *ToolButton4;
   TAction *ActionHelp;
   TTabSheet *TabSheetArranjo;
   TListView *LViewArranjo;
	TCheckBox *CheckBoxMontante;
   void __fastcall ActionEdtExecute(TObject *Sender);
   void __fastcall ActionSelEqptoExecute(TObject *Sender);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall LViewClick(TObject *Sender);
   void __fastcall LViewDblClick(TObject *Sender);
   void __fastcall LViewColumnClick(TObject *Sender, TListColumn *Column);
   void __fastcall LViewCompare(TObject *Sender, TListItem *Item1,
          TListItem *Item2, int Data, int &Compare);
   void __fastcall ActionExportaExecute(TObject *Sender);
   void __fastcall ActionHelpExecute(TObject *Sender);

public:      // User declarations
   __fastcall  TFormDistancia(TComponent *owner, VTApl *apl_owner, TWinControl *parent);
   __fastcall ~TFormDistancia(void);

private:   //métodos
   void    __fastcall DefineEqptoFinal(VTEqpto *eqpto);
   void    __fastcall DefineEqptoInicial(VTEqpto *eqpto);
   void    __fastcall DestacaEqpto(TList *lisEQP);
   void    __fastcall FormIniciaPosicao(void);
   void    __fastcall ExibeDados(TList *lisEQP);
   bool    __fastcall LisArranjo(TList *lisLIG, TList *lisARR);
   TList*  __fastcall LisBarra(TList *lisLIG);
   TList*  __fastcall LisEqbar(TList *lisLIG, int tipo_eqpto);
   TList*  __fastcall LisLigacao(TList *lisLIG, int tipo_eqpto);
   TList*  __fastcall LisRede(TList *lisLIG);
   TList*  __fastcall LisTrecho(TList *lisLIG, VTArranjo *arranjo);
   void    __fastcall LViewArranjoInicia(TList *lisLIG);
   void    __fastcall LViewCargaInicia(TList *lisLIG);
   void    __fastcall LViewChaveInicia(TList *lisLIG);
   void    __fastcall LViewEqptoInicia(TList *lisLIG);
   void    __fastcall LViewResumoInicia(TList *lisLIG);
   void    __fastcall LViewTrafoInicia(TList *lisLIG);
   void    __fastcall LViewTrechoInicia(TList *lisLIG);
   void    __fastcall Moldura(TList *lisEQP);
   void    __fastcall SelecionaMontante(VTEqpto *eqpto);
   VTRede* __fastcall RedeDaBarra(TList *lisLIG, VTBarra *barra);
   double  __fastcall SomaComprimentoTrecho_m(TList *lisTRE);
   void    __fastcall Zoom(VTEqpto *eqpto);

private:   //objetos externos
   VTApl *apl;

private:   //dados locais
   TList     *lisARR;
   TList     *lisEQP;
   TList     *lisLIG;
   VTCaminho *caminho;
   VTEdt     *edt_busca;
   struct   {//estrutura c/ Eqptos inicial e final do caminho elétrico
            VTEqpto *eqpto_ini;
            VTEqpto *eqpto_fim;
            } dist;
   struct   {//estrutura p/ controle da ordenação do LView
            TListColumn *Column;
            bool        OrdemCrescente;
            }LViewSort;
};

//---------------------------------------------------------------------------
#endif
//eof
