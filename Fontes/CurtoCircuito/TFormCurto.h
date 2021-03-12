//---------------------------------------------------------------------------
#ifndef TFormCurtoH
#define TFormCurtoH

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
#include <System.ImageList.hpp>

//---------------------------------------------------------------------------
class TFormCurtoArvore;
class TFormCurtoTabela;
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
class VTProgresso;

//---------------------------------------------------------------------------
class TFormCurto : public TFormEdt
{
__published:   // IDE-managed Components
   TPanel *PanelCurto;
   TAction *ActionSelEqpto;
   TAction *ActionSelEqptoDefeito;
   TToolButton *ButRelCurto;
   TAction *ActionRelatorioCurto;
   TAction *ActionEditaDefTrecho;
   TToolButton *ButEditaDefTrecho;
   TComboBox *CBoxDef;
   TPanel *PanelOpcao;
   TGroupBox *GBoxPrefalta;
   TGroupBox *GBoxGrafico;
   TLabel *Label1;
   TAction *ActionEditaZdef;
   TCheckBox *CheckBoxRelatorio;
   TCheckBox *CheckBoxCaminho;
   TPanel *PanelColor;
   TColorDialog *ColorDialog;
   TToolButton *ButProtecao;
   TToolButton *ToolButton2;
   TAction *ActionProtecao;
   TRadioGroup *RadioGroupAlgoritmo;
   TGroupBox *gboxZdef;
   TListView *ListViewZdef;
   TToolButton *ButEditaZdef;
   TToolButton *ToolButton4;
   TToolButton *ToolButton5;
   TAction *ActionHelp;
   TComboBox *CBoxPatamar;
   TToolButton *ButRelChave;
   TAction *ActionRelatorioChave;
	TPanel *PanelRelZent;
	TListView *lviewZent;
	TToolBar *ToolBar1;
	TToolButton *ToolButton3;
	TSaveDialog *SaveDialog;
	TAction *ActionRelatorioZentrada;
	TAction *ActionExportRelZent;
	TToolButton *ToolButton1;
   void __fastcall ActionEdtExecute(TObject *Sender);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall ActionSelEqptoExecute(TObject *Sender);
   void __fastcall ActionSelEqptoDefeitoExecute(TObject *Sender);
   void __fastcall ActionRelatorioCurtoExecute(TObject *Sender);
   void __fastcall ActionEditaDefTrechoExecute(TObject *Sender);
   void __fastcall CBoxDefChange(TObject *Sender);
   void __fastcall CBoxPatamarChange(TObject *Sender);
   void __fastcall ActionEditaZdefExecute(TObject *Sender);
   void __fastcall CheckBoxCaminhoClick(TObject *Sender);
   void __fastcall PanelColorDblClick(TObject *Sender);
   void __fastcall ActionProtecaoExecute(TObject *Sender);
   void __fastcall RadioGroupAlgoritmoClick(TObject *Sender);
   void __fastcall ActionHelpExecute(TObject *Sender);
   void __fastcall ActionRelatorioChaveExecute(TObject *Sender);
	void __fastcall ActionExportRelZentExecute(TObject *Sender);
	void __fastcall ActionRelatorioZentradaExecute(TObject *Sender);
	void __fastcall lviewZentClick(TObject *Sender);
	void __fastcall lviewZentDblClick(TObject *Sender);

public:      // User declarations
        __fastcall  TFormCurto(TComponent *Owner, VTApl *apl_owner, TWinControl *parent);
        __fastcall ~TFormCurto(void);
   void __fastcall  AtualizaCalculo(void);

private:   //métodos
   void __fastcall CBoxPatamarInicia(void);
   void __fastcall ControlSelEquipamento(void);
   int  __fastcall DeterminaPatamarSelecionado(void);
   bool __fastcall ExecutaCalculo(void);
   bool __fastcall ExecutaCalculoFluxo(bool pre_falta);
   void __fastcall ExibeCaminhoCurto(bool visible);
   void __fastcall ExibeZdef(void);
   void __fastcall FormIniciaPosicao(void);
   void __fastcall FormRelZent(bool showRelZent);
   void __fastcall IniciaZdef(void);
   void __fastcall LeArqCfgCurto(void);
   void __fastcall MontaCaminhoCurto(void);
   void __fastcall PreencheRelZent(void);
   void __fastcall SalvaArqCfgCurto(void);
   void __fastcall SelecionaEqptoDefeito(VTEqpto *eqpto);
   complex<double> __fastcall StrToComplex(AnsiString dadoValor);
   void __fastcall UsuarioPrivilegiado(void);
   bool __fastcall ValidaRedeParaCurto(void);

private:   //objetos externos
   VTApl    *apl;
   VTRedeCC *redeCC;

private:   //dados locais
   TFormCurtoArvore *FormCurtoArvore;
   TFormCurtoTabela *FormCurtoTabela;
   VTEdt            *edt_curto;
   VTCurtocircuito  *curtocircuito;
   TList            *lisBAR;
   TList            *lisLIG;
   VTProgresso      *progresso;
	int barras_isoladas;
   struct   {
            bool     rede_alterada;
            bool     fluxo_calculado;
            VTEqpto  *eqpto;
            double   dist_bar1;   //km
            STR_ZDef Zdef;        //impedâncias de defeito, em ohm
           } defeito;
};

//---------------------------------------------------------------------------
#endif
//eof
