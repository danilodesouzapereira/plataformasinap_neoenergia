//---------------------------------------------------------------------------
#ifndef TFormCurtoSimultaneoH
#define TFormCurtoSimultaneoH

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
#include <Grids.hpp>
#include <Buttons.hpp>
#include <Dialogs.hpp>
#include <System.Actions.hpp>
#include "..\Forms\TFormEdt.h"
#include "..\RedeCC\ResCurto.h"

//---------------------------------------------------------------------------
//class TFormCurtoArvore;
//class TFormCurtoTabela;
class TFormRelTexto;
class VTApl;
class VTEdt;
class VTBarra;
class VTChave;
class VTEqpto;
class VTRedeCC;
class VTTrafo;
class VTTrecho;
class VTGrupo;
class VTCurtocircuito;
class TDefeito;
class TDefeitos;

//---------------------------------------------------------------------------
class TFormCurtoSimultaneo : public TFormEdt
{
__published:   // IDE-managed Components
   TAction *ActionSelEqpto;
   TAction *ActionSelEqptoDefeito;
   TToolButton *ButRelCurto;
   TAction *ActionRelatorioCurto;
   TComboBox *CBoxDef;
   TPanel *PanelOpcao;
   TGroupBox *GBoxPrefalta;
   TGroupBox *GBoxGrafico;
   TLabel *Label1;
   TAction *ActionEditaZdef;
   TCheckBox *CheckBoxCaminho;
   TPanel *PanelColor;
   TColorDialog *ColorDialog;
   TToolButton *ButProtecao;
   TToolButton *ToolButton2;
   TAction *ActionProtecao;
   TGroupBox *gboxZdef;
   TListView *ListViewZdef;
   TToolButton *ToolButton4;
   TToolButton *ToolButton5;
   TAction *ActionHelp;
   TComboBox *CBoxPatamar;
   TToolButton *ButRelChave;
   TAction *ActionRelatorioChave;
   TGroupBox *GroupBox1;
   TToolBar *ToolBar;
   TListView *LViewBarDefeito;
   TToolButton *ToolButton1;
   TAction *ActionClearBarDefeito;
   TToolButton *ButCalculaCurto;
   TToolButton *ButEditaZdef;
   TAction *ActionCalculaCurto;
   void __fastcall ActionEdtExecute(TObject *Sender);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall ActionSelEqptoExecute(TObject *Sender);
   void __fastcall ActionSelEqptoDefeitoExecute(TObject *Sender);
   void __fastcall ActionRelatorioCurtoExecute(TObject *Sender);
   void __fastcall CBoxDefChange(TObject *Sender);
   void __fastcall CBoxPatamarChange(TObject *Sender);
   void __fastcall ActionEditaZdefExecute(TObject *Sender);
   void __fastcall CheckBoxCaminhoClick(TObject *Sender);
   void __fastcall PanelColorDblClick(TObject *Sender);
   void __fastcall ActionProtecaoExecute(TObject *Sender);
   void __fastcall RadioGroupAlgoritmoClick(TObject *Sender);
   void __fastcall ActionHelpExecute(TObject *Sender);
   void __fastcall ActionRelatorioChaveExecute(TObject *Sender);
   void __fastcall ActionCalculaCurtoExecute(TObject *Sender);
   void __fastcall LViewBarDefeitoClick(TObject *Sender);
   void __fastcall ActionClearBarDefeitoExecute(TObject *Sender);

public:      // User declarations
        __fastcall  TFormCurtoSimultaneo(TComponent *Owner, VTApl *apl_owner, TWinControl *parent);
        __fastcall ~TFormCurtoSimultaneo(void);

private:   //métodos
   void __fastcall CBoxPatamarInicia(void);
   bool __fastcall ConfiguraDefeito(TDefeito *defeito);
   int  __fastcall DeterminaPatamarSelecionado(void);
   bool __fastcall ExecutaCalculo(void);
   bool __fastcall ExecutaCalculoFluxo(bool pre_falta);
   void __fastcall ExibeCaminhoCurto(void);
   void __fastcall ExibeRelTexto(void);
   void __fastcall ExibeZdef(void);
   void __fastcall FormIniciaPosicao(void);
   void __fastcall GraficoMoldura(VTEqpto *eqpto=NULL);
   void __fastcall GraficoZoom(VTEqpto *eqpto);
   void __fastcall IniciaZdef(void);
   void __fastcall LeArqCfgCurto(void);
   void __fastcall LViewBarDefeitoInicia(void);
   void __fastcall MontaCaminhoCurto(void);
   void __fastcall SalvaArqCfgCurto(void);
   bool __fastcall SelecionaTipoDefeito(VTBarra *barra);
   bool __fastcall ValidaRedeParaCurto(void);

private:   //objetos externos
   VTApl    *apl;
   VTRedeCC *redeCC;

private:   //dados locais
   bool             curto_calculado;
   //TFormCurtoArvore *FormCurtoArvore;
   //TFormCurtoTabela *FormCurtoTabela;
   TFormRelTexto    *FormRelTexto;
   VTEdt            *edt_curto;
   VTCurtocircuito  *curtocircuito;
   TDefeitos        *defeitos;
   TList            *lisBAR;
   TList            *lisLIG;
   STR_ZDef         Zdef;        //impedâncias de defeito, em ohm
};

//---------------------------------------------------------------------------
#endif
//eof
