//---------------------------------------------------------------------------
#ifndef TFormDemandaH
#define TFormDemandaH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "TFormEdt.h"
#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Graphics.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.ToolWin.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ExtDlgs.hpp>
#include <System.ImageList.hpp>

//---------------------------------------------------------------------------
class TBackup;
class VTApl;
class VTAjusta;
class VTCanal;
class VTChave;
class VTCurva;
class VTCopia;
class VTEqpto;
class VTImportaMedicao;
class VTLigacao;
class VTMedicao;
class VTMedicoes;
class VTMedidor;
class VTObra;
class VTRadial;
class VTRede;
class VTRedes;     //DVK 2014.09.17
class VTAjusteFator;

//---------------------------------------------------------------------------
class TFormDemanda : public TFormEdt
{
__published:   // IDE-managed Components
   TGroupBox *GBoxMedicao;
   TToolBar *ToolBarMedicao;
   TPanel *PanelFiltro;
   TComboBox *CBoxPrimario;
   TListView *LViewMedidores;
   TToolBar *ToolBar;
   TToolButton *ButFecha;
   TToolButton *ButSelArqMedicao;
   TToolButton *ToolButton3;
   TToolButton *ButAjustaDemanda;
   TToolButton *ToolButton4;
   TToolButton *ToolButton1;
   TToolButton *ButAjuda;
   TAction *ActionAjustaDemanda;
   TAction *ActionFecha;
   TAction *ActionHelp;
   TAction *ActionCfgAjuste;
   TAction *ActionLogAjusta;
   TAction *ActionImportaMedicao;
   TOpenTextFileDialog *OpenTextFileDialog;
   TToolButton *ButEdtMedicao;
   TAction *ActionEdtMedicao;
   TAction *ActionRedeChanged;
	TGroupBox *gboxTipoAjuste;
	TRadioButton *radioMedicao;
	TRadioButton *radioFator;
	TGroupBox *GBoxMultiplicadores;
	TGroupBox *GBoxFatores;
	TLabel *lblK2;
	TPanel *pnlK2;
	TPanel *pnlK1;
	TLabel *lblK1;
	TEdit *editK2;
	TEdit *editK1;
	TRadioGroup *radiogroupTipo;
	TGroupBox *gboxRedeFator;
	TComboBox *cboxRede;
	TToolButton *ToolButton2;
	TAction *ActionInserirMedicao;
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
   void __fastcall ActionAjustaDemandaExecute(TObject *Sender);
   void __fastcall ActionFechaExecute(TObject *Sender);
   void __fastcall ActionHelpExecute(TObject *Sender);
   void __fastcall ActionLogAjustaExecute(TObject *Sender);
   void __fastcall ActionCfgAjusteExecute(TObject *Sender);
   void __fastcall ActionImportaMedicaoExecute(TObject *Sender);
   void __fastcall CBoxPrimarioChange(TObject *Sender);
   void __fastcall LViewMedidoresClick(TObject *Sender);
   void __fastcall LViewMedidoresDblClick(TObject *Sender);
   void __fastcall ActionEdtMedicaoExecute(TObject *Sender);
   void __fastcall ActionEdtExecute(TObject *Sender);
   void __fastcall ActionRedeChangedExecute(TObject *Sender);
   void __fastcall LViewMedidoresColumnClick(TObject *Sender, TListColumn *Column);
   void __fastcall LViewMedidoresCompare(TObject *Sender, TListItem *Item1, TListItem *Item2,
          int Data, int &Compare);
	void __fastcall editK1Click(TObject *Sender);
	void __fastcall editK2Click(TObject *Sender);
	void __fastcall radiogroupTipoClick(TObject *Sender);
	void __fastcall radioFatorClick(TObject *Sender);
	void __fastcall radioMedicaoClick(TObject *Sender);
	void __fastcall cboxRedeChange(TObject *Sender);
	void __fastcall cboxRedeClick(TObject *Sender);
	void __fastcall cboxRedeDropDown(TObject *Sender);
	void __fastcall ActionInserirMedicaoExecute(TObject *Sender);



public:      // User declarations
   __fastcall  TFormDemanda(TComponent *owner, VTApl *apl_owner, TWinControl *parent);
   __fastcall ~TFormDemanda(void);

private:   //métodos
   void		   __fastcall AlteraTipoAjuste(void);
   void        __fastcall AtualizaFluxo(bool uptodate);
   bool        __fastcall CalculaFluxo(AnsiString msg_usuario="");
   void        __fastcall CBoxAnoInicia(void);
   void        __fastcall CBoxPrimarioInicia(void);
   void 	   __fastcall CBoxRedesFatorInicia(void);
   void        __fastcall ChartInicia(void);
   void        __fastcall ChartIniciaIxFASE(VTCurva *curva_flow, VTCurva *curva_med);
   void        __fastcall ChartIniciaP(VTCurva *curva_flow, VTCurva *curva_med);
   void        __fastcall ChartIniciaPQ(VTCurva *curva_flow, VTCurva *curva_med);
   void        __fastcall ChartIniciaPQxFASE(VTCurva *curva_flow, VTCurva *curva_med);
   void 	   __fastcall ConfiguraForm(void);
   void 	   __fastcall ConfiguraGBoxFatores(void);
   bool        __fastcall ConfirmaSelecaoArquivoMedicao(void);
   bool        __fastcall ContinuaMesmoComPrimariosNaoAjustaveis(TList *lisPRI_VAL);
   void        __fastcall CriaCurvaFluxo(VTCurva *curva, VTMedicao *medicao);
   void		   __fastcall CriaFatores(void);
   void        __fastcall CriaMedicaoDisjuntorTodosPrimarios(void);
   void        __fastcall CriaMedicoesMedidores(void);
   void        __fastcall CriaObjAjusta(void);
   void        __fastcall ExibeMedidores(bool visible);
   void        __fastcall ExibeResultadoAjuste(void);
   bool        __fastcall ExisteGeracao(void);
   VTLigacao*  __fastcall ExisteLigacao(VTMedidor *medidor);
   int         __fastcall ExisteMedidor(void);
   //int         __fastcall ExisteMedicaoSelecionada(void);
   //bool        __fastcall ExisteMedicaoSelecionadaInvalida(TList *lisMED_INV);
   bool        __fastcall ExisteRedeEquivalente(void);
//   bool        __fastcall ExisteVizinhaEquivalentada(VTPrimario *primario);
   void        __fastcall FatorDefault(void);
   void        __fastcall FormIniciaPosicao(void);
   bool        __fastcall ImportaMedicao(void);
   void        __fastcall InsereMedidoresAjustados(void);
   void        __fastcall InsereValor(TListItem* item, VTMedidor* medidor,
                                      int tipo_canal, int fase_tag);
   bool        __fastcall LeFatorAjusteUnico(double ajuste_max=0);
   bool		   __fastcall LeFatores(void);
   void        __fastcall LViewMedidoresInicia(void);
   void        __fastcall LViewMedidoresInicia(VTRede *rede);
   AnsiString  __fastcall MaiorErro(VTCanal *canal);
   VTMedicao*  __fastcall MedicaoSelecionada(void);
   void        __fastcall Moldura(VTEqpto *eqpto);
   void        __fastcall Moldura(TList *lisEqp);
   void        __fastcall MontaCurvaMedicaoFatcor(void);
   void        __fastcall MostraMedicoesOuPrimarios(void);
   void        __fastcall PreencheGBoxEnergia(VTCurva *curva);
   void        __fastcall ReiniciaChart(void);
   void        __fastcall ReiniciaDiagnostico(void);
   void        __fastcall RestauraCargasOriginais(void);
   void        __fastcall RestauraCargasOriginaisRedesNaoAjustadas(void);
   void        __fastcall RestauraFlowOriginal(void);
   void        __fastcall SalvaCopiaDasCargasOriginais(void);
   bool        __fastcall SalvaFator(void);
   bool        __fastcall SalvaFatores(void);
   bool        __fastcall SelecionaArquivo(void);
   bool		   __fastcall ValidaFator(void);
   bool		   __fastcall ValidaFatorEdit(AnsiString fator);

private:   //objetos externos
   VTObra *obra_original;

private: //dados locais
   enum opMEDICAO  {medINVALIDA=0, medNEGATIVA=1, medVALIDA=2};
   VTApl            *apl; //próprio
//   bool             ajuste_ja_executado;
   bool             ajuste_por_fase;
   double           fatcor_unico;
   double           val_max, val_min;
   VTAjusta         *ajusta_demanda;
   VTCopia          *copia;
   TStringList      *files;
   TList            *lisEQP;
   int				tipoFator;
   VTAjusteFator	*fatorAtivo;

   struct   {//estrutura p/ controle da ordenação do LView
            TListColumn *Column;
            bool        OrdemCrescente;
            }LViewSort;
};

//---------------------------------------------------------------------------
#endif
//eof

