//---------------------------------------------------------------------------
#ifndef TFormEdtReguladorMTH
#define TFormEdtReguladorMTH

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
#include <map>
using namespace std;

//---------------------------------------------------------------------------
class VTApl;
class VTBarra;
class VTEqpto;
class VTRede;
class VTRegulador;

//---------------------------------------------------------------------------
class TFormEdtReguladorMT : public TForm
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
   TCheckBox *checkBypass;
   TComboBox *CBoxRede;
   TGroupBox *GBoxUnidades;
   TComboBox *CBoxConfiguracao;
   TGroupBox *GBoxBarEntrada;
   TLabel *Label10;
   TComboBox *CBoxBarEntrada;
   TLabel *Label6;
   TTrackBar *TrackBarPassoFixo;
   TEdit *EditFixoPasso;
   TPanel *PanelFluxo;
	TComboBox *CBoxTipoPotCorr;
   TPanel *Panel1;
   TPanel *PanelLabels;
   TPanel *PanelBarras;
   TPanel *Panel4;
   TPanel *Panel5;
   TPanel *PanelPotencia;
   TPanel *PanelPasso;
   TLabel *Label3;
   TComboBox *CBoxBarSaida;
   TGroupBox *GBoxEstado;
   TPanel *PanelV;
   TLabel *Label7;
   TEdit *EditFixoTensao;
   TPanel *PanelTap;
   TPanel *Panel7;
	TPanel *PanelTapFix;
	TAction *ActionBarrasDefault;
	TPageControl *PageControl;
	TTabSheet *psheetDados;
	TTabSheet *psheetAjustes;
	TGroupBox *gboxBMdir;
	TLabel *lblBandaMorta;
	TEdit *edtInsDireto;
	TComboBox *cboxUnidadeBM;
	TPanel *pnlBMDireto;
	TPanel *pnlBMInverso;
	TLabel *lblBandaMortaInv;
	TComboBox *cboxUnidadeBMInv;
	TEdit *edtInsInverso;
	TPanel *Panel2;
	TEdit *EditValor;
	TLabel *Label2;
	TPanel *Panel8;
	TLabel *Label5;
	TGroupBox *GBoxTipoAjuste;
	TPanel *Panel9;
	TLabel *lbRegMaxima;
	TComboBox *CBoxFaixaTensao;
	TPanel *Panel10;
	TLabel *lbDegrau;
	TComboBox *CBoxFaixaPassos;
	TPanel *Panel11;
	TLabel *Label9;
	TEdit *EditRTP;
	TPanel *PanelLimitesInv;
	TLabel *Label11;
	TEdit *EditBMinverso;
	TPanel *Panel18;
	TPanel *PanelLimitesDir;
	TLabel *Label12;
	TEdit *EditBMdireto;
	TPanel *Panel20;
	TGroupBox *gboxBMinv;
	TGroupBox *gboxTapFixo;
	TTabSheet *psheetReferencia;
	TGroupBox *GroupBoxVrefInv;
	TPanel *Panel21;
	TPanel *Panel25;
	TLabel *LabelBarraDireto;
	TComboBox *CBoxBarraInverso;
	TPanel *Panel26;
	TLabel *LabelBarraInverso;
	TPanel *Panel3;
	TLabel *Label15;
	TEdit *EditVinv;
	TEdit *EditNTinv;
	TGroupBox *GroupBoxVrefDir;
	TLabel *Label4;
	TPanel *Panel22;
	TPanel *Panel23;
	TLabel *Label13;
	TComboBox *CBoxBarraDireto;
	TEdit *EditNTdir;
	TPanel *Panel24;
	TLabel *Label14;
	TEdit *EditVdir;
	TRadioGroup *RadioGroupBarraRef;
	TGroupBox *GBoxTipoOperacao;
	TComboBox *ComboBoxTipoAjuste;
	TPanel *PanelNVdir;
	TPanel *Panel6;
	TPanel *PanelNVinv;
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall ActionConfirmaExecute(TObject *Sender);
   void __fastcall ActionRetiraExecute(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall ActionSelPassoFixoExecute(TObject *Sender);
   void __fastcall ActionSelTipoAjusteExecute(TObject *Sender);
   void __fastcall CBoxBarEntradaChange(TObject *Sender);
   void __fastcall CBoxBarSaidaChange(TObject *Sender);
   void __fastcall CBoxBarEnter(TObject *Sender);
   void __fastcall CBoxBarraDiretoChange(TObject *Sender);
   void __fastcall CBoxBarraInversoChange(TObject *Sender);
   void __fastcall ActionBarrasDefaultExecute(TObject *Sender);
	void __fastcall CBoxFaixaPassosChange(TObject *Sender);
	void __fastcall CBoxFaixaTensaoChange(TObject *Sender);
	void __fastcall CBoxTipoPotCorrChange(TObject *Sender);
	void __fastcall cboxUnidadeBMChange(TObject *Sender);
	void __fastcall cboxUnidadeBMInvChange(TObject *Sender);
	void __fastcall RadioGroupBarraRefClick(TObject *Sender);
	void __fastcall edtInsInversoKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall edtInsDiretoKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall EditVdirKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall EditVinvKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall EditRTPKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall edtInsDiretoExit(TObject *Sender);
	void __fastcall edtInsInversoExit(TObject *Sender);
	void __fastcall CBoxConfiguracaoChange(TObject *Sender);

public: //construtor e destrutor
        __fastcall  TFormEdtReguladorMT(TComponent* Owner, VTApl *apl, VTRegulador *regulador);
        __fastcall ~TFormEdtReguladorMT(void);
   void __fastcall  OpcaoRetira(bool enabled);

private: //métodos
   void    __fastcall AtualizaTrackBarPassofixo(void);
   void    __fastcall CarregaPassos(void);
   double  __fastcall ConverteInsensibilidadeVtoPerc(double ins_v, double nv_v);
   void    __fastcall DefValoresDefaultRegulador(void);
   double  __fastcall DeterminaCapacidadeMax(void);
   int     __fastcall DeterminaFaixaPassos(void);
   double  __fastcall DeterminaFaixaTensao(void);
   int     __fastcall DeterminaFaixaPassosInd(int valor);
   int     __fastcall DeterminaFaixaTensaoInd(double valor);
   void    __fastcall ExibeDadosRegulador(void);
   void    __fastcall FormIniciaPosicao(void);
   void    __fastcall IniciaCBoxBarControle(void);
   void    __fastcall IniciaCBoxBarEntrada(void);
   void    __fastcall IniciaCBoxBarSaida(void);
   void    __fastcall IniciaCBoxRede(void);
   void    __fastcall IniciaCBoxTipoPotCorr(void);
   void    __fastcall IniciaMaps(void);
   double  __fastcall InomToSnom_kva(double inom_a);
   double  __fastcall InomToSpassante_kva(double inom_a);
   bool    __fastcall LeDados(void);
   void    __fastcall Moldura(VTEqpto *eqpto=NULL);
   double  __fastcall SnomToSpassante(double Snom);
   double  __fastcall SnomToInom(double snom_kva);
   double  __fastcall SpassanteToSnom(double Spas_kva);
   double  __fastcall SpassanteToInom(double Spas_kva);
   VTRede* __fastcall SelecionaRede(TList *lisREDE);
   bool    __fastcall ValidaDados(void);
   double  __fastcall ValorBandaMorta_pu(void);
   double  __fastcall ValorBandaMortaInv_pu(void);
   double  __fastcall ValorNivelTensaoDir(void);
   double  __fastcall ValorNivelTensaoInv(void);
   double  __fastcall ValorSnom_mva(void);
   double  __fastcall ReguladorVnom_kv(void);

private: //objetos externos
   VTApl       *apl;
   VTRegulador *regulador;

private: //atributos locais
//   double spassante_kva;
//   double snom_kva;
   TList *lisEQP;
   int opPotCor;
   int passoTemp;
//   bool wasPassante;
   map <int, double> faixa_tensao_val; //indice (key) e valores para faixa de tensão (value)
   map <int, int>    faixa_passos_val; //indice (key) e valores para número máximo de passos (value)
   map <double, int> faixa_tensao_ind; //faixa de tensão (key) e indice (value)
   map <int, int>    faixa_passos_ind; //número máximo de passos (key) e indice (value)
   map <double, double> capacidade_max; //faixa de tensão (key) e capacidade (value)
//   int varMax;
};

//---------------------------------------------------------------------------
#endif
//eof
