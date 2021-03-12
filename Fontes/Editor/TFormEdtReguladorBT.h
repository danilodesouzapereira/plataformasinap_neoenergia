//---------------------------------------------------------------------------
#ifndef TFormEdtReguladorBTH
#define TFormEdtReguladorBTH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include <ComCtrls.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <ExtCtrls.hpp>
#include <ActnList.hpp>
#include <System.Actions.hpp>
#include <System.ImageList.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTEqpto;
class VTRede;
class VTRegulador;

//---------------------------------------------------------------------------
class TFormEdtReguladorBT : public TForm
{
__published:   // IDE-managed Components
	TToolBar *ToolBar;
   TToolButton *butConfirma;
   TToolButton *butCancela;
   TToolButton *butRetira;
   TActionList *ActionList1;
   TAction *ActionCancela;
   TAction *ActionConfirma;
   TAction *ActionRetira;
   TImageList *ImageList1;
   TAction *ActionSelPassoFixo;
   TAction *ActionSelTipoAjuste;
   TGroupBox *GBoxRegu;
   TLabel *Label1;
   TLabel *Label8;
   TEdit *EditCodigo;
   TEdit *EditS_kva;
   TCheckBox *checkBypass;
   TComboBox *CBoxRede;
   TGroupBox *GroupBoxUnidades;
   TComboBox *CBoxConfiguracao;
   TGroupBox *GBoxBarEntrada;
   TLabel *Label10;
   TComboBox *CBoxBarEntrada;
   TGroupBox *GroupBoxTipoAjuste;
   TLabel *lbRegMaxima;
   TLabel *lbDegrau;
   TEdit *EditFaixaTensao;
   TUpDown *UpDownFaixaTensao;
   TEdit *EditFaixaPasso;
   TUpDown *UpDownFaixaPasso;
   TPanel *PanelFluxo;
   TComboBox *CBoxTipoPot;
   TLabel *Label5;
   TLabel *Label11;
   TPanel *Panel1;
   TPanel *Panel2;
   TPanel *Panel4;
   TPanel *Panel5;
   TPanel *PanelPotencia;
   TPanel *Panel8;
   TPanel *Panel7;
   TPanel *Panel9;
   TPanel *Panel10;
   TPanel *Panel13;
   TPanel *Panel14;
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall ActionConfirmaExecute(TObject *Sender);
   void __fastcall ActionRetiraExecute(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall UpDownFaixaPassoClick(TObject *Sender,
          TUDBtnType Button);
   void __fastcall UpDownFaixaTensaoClick(TObject *Sender,
          TUDBtnType Button);
   void __fastcall CBoxBarEntradaChange(TObject *Sender);
   void __fastcall CBoxBarEnter(TObject *Sender);
   void __fastcall CBoxTipoPotChange(TObject *Sender);

public: //construtor e destrutor
        __fastcall  TFormEdtReguladorBT(TComponent* Owner, VTApl *apl, VTRegulador *regulador);
        __fastcall ~TFormEdtReguladorBT(void);
   void __fastcall  OpcaoRetira(bool enabled);

private: //métodos
   void    __fastcall DefValoresDefaultRegulador(void);
   void    __fastcall ExibeDadosRegulador(void);
   void    __fastcall FormIniciaPosicao(void);
   void    __fastcall IniciaCBoxBarEntrada(void);
   void    __fastcall IniciaCBoxBarRef(void);
   void    __fastcall IniciaCBoxBarSaida(void);
   void    __fastcall IniciaCBoxModelo(void);
   void    __fastcall IniciaCBoxRede(void);
   bool    __fastcall LeDados(void);
   void    __fastcall Moldura(VTEqpto *eqpto=NULL);
   double  __fastcall SnomToSpassante(double Snom, double dv_perc);
   double  __fastcall SpassanteToSnom(double Spas, double dv_perc);
   void    __fastcall RedefineBarraReferencia(void);
   VTRede* __fastcall SelecionaRede(TList *lisREDE);
   bool    __fastcall ValidaDados(void);

private: //objetos externos
   VTApl       *apl;
   VTRegulador *regulador;

private: //atributos locais
   double spassante_kva;
   TList *lisEQP;
};

//---------------------------------------------------------------------------
#endif
//eof
