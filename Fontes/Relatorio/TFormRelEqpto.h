//---------------------------------------------------------------------------
#ifndef TFormRelEqptoH
#define TFormRelEqptoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
#include <ExtCtrls.hpp>
#include <ADODB.hpp>
#include <DB.hpp>
#include <Menus.hpp>
#include <System.Actions.hpp>
#include <Vcl.ImgList.hpp>
#include <System.ImageList.hpp>

//---------------------------------------------------------------------------
class TFase;
class VTApl;
class VTBarra;
class VTChave;
class VTClasse;
class VTRede;
class VTPatamar;
class VTTrecho;
class TFormPlanilha;
class VTProgresso;
//---------------------------------------------------------------------------
enum tipo_rel {tipoRelBarra = 0,
               tipoRelCapacitor,
               tipoRelCarga,
               tipoRelChave,
               tipoRelFluxoBarra,
               tipoRelFluxoRede,
               tipoRelFluxoRegulador,
               tipoRelFluxoTrafo,
               tipoRelFluxoTrecho,
               tipoRelGerador,
               tipoRelPerfilCarga,
               tipoRelReator,
               tipoRelRegulador,
               tipoRelRede,
               tipoRelSuprimento,
               tipoRelTrafo,
               tipoRelTrafo3E,
               tipoRelTrecho,
               tipoRelBarCC,
               tipoRelLigCC,
               tipoRelCOUNT};
//---------------------------------------------------------------------------
class TFormRelEqpto : public TForm
{
__published:	// IDE-managed Components
   TToolBar *ToolBar;
   TToolButton *butRelBarra;
   TToolButton *butRelRede;
   TPanel *panelRel;
   TActionList *ActionList1;
   TAction *ActionRelBarra;
   TAction *ActionRelRede;
   TToolButton *butRelTrecho;
   TToolButton *butRelTrafo;
   TToolButton *butRelChave;
   TAction *ActionRelChave;
   TAction *ActionRelTrafo;
   TAction *ActionRelTrecho;
   TAction *ActionRelSuprimento;
   TAction *ActionRelCarga;
   TToolButton *butRelCarga;
   TToolButton *butRelSuprimento;
   TToolButton *butRelGerador;
   TAction *ActionRelGerador;
   TAction *ActionRelFluxoBarra;
   TADOTable *table;
   TAction *ActionRelFluxoTrecho;
   TAction *ActionRelFluxoTrafo;
   TToolButton *butRelCapacitor;
   TToolButton *butRelReator;
   TToolButton *butRelTrafo3E;
   TAction *ActionRelCapacitor;
   TAction *ActionRelReator;
   TAction *ActionRelTrafo3E;
   TADOConnection *Connection;
   TADOQuery *query;
   TToolButton *ToolButton1;
   TPopupMenu *PopupMenuRelatorio;
   TToolButton *ButRelatorio;
   TMenuItem *EquipamentosAtributos1;
   TMenuItem *FluxodePotncia1;
   TMenuItem *Patamar01;
   TMenuItem *Patamar11;
   TAction *ActionRelEqpto;
	TAction *ActionRelFluxo;
   TAction *ActionRelFluxoCapacitor;
   TAction *ActionRelFluxoCarga;
   TAction *ActionRelFluxoChave;
   TAction *ActionRelFluxoGerador;
   TAction *ActionRelFluxoReator;
   TAction *ActionRelFluxoRede;
   TAction *ActionRelFluxoSuprimento;
   TAction *ActionRelFluxoTrafo3E;
   TToolButton *butRelPerfilCarga;
   TAction *ActionRelPerfilCarga;
   TAction *ActionRelFluxoPerfilCarga;
	TToolButton *butRelRegulador;
	TAction *ActionRelRegulador;
	TAction *ActionRelFluxoRegulador;
   TImageList *ImageList;
   TAction *ActionHelp;
   TToolButton *ToolButton2;
   TToolButton *ToolButton3;
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall ActionRelRedeExecute(TObject *Sender);
   void __fastcall ActionRelBarraExecute(TObject *Sender);
   void __fastcall ActionRelCargaExecute(TObject *Sender);
   void __fastcall ActionRelChaveExecute(TObject *Sender);
   void __fastcall ActionRelTrafoExecute(TObject *Sender);
   void __fastcall ActionRelTrechoExecute(TObject *Sender);
   void __fastcall ActionRelGeradorExecute(TObject *Sender);
   void __fastcall ActionRelFluxoBarraExecute(TObject *Sender);
   void __fastcall ActionRelFluxoTrechoExecute(TObject *Sender);
   void __fastcall ActionRelFluxoTrafoExecute(TObject *Sender);
   void __fastcall ActionRelCapacitorExecute(TObject *Sender);
   void __fastcall ActionRelReatorExecute(TObject *Sender);
   void __fastcall ActionRelSuprimentoExecute(TObject *Sender);
   void __fastcall ActionRelTrafo3EExecute(TObject *Sender);
   void __fastcall ActionRelEqptoExecute(TObject *Sender);
   void __fastcall ActionRelFluxoExecute(TObject *Sender);
   void __fastcall ActionRelPerfilCargaExecute(TObject *Sender);
   void __fastcall ActionRelFluxoRedeExecute(TObject *Sender);
	void __fastcall ActionRelReguladorExecute(TObject *Sender);
   void __fastcall ActionHelpExecute(TObject *Sender);
//	void __fastcall ActionRelFluxoReguladorExecute(TObject *Sender);


public:		// User declarations
		  __fastcall  TFormRelEqpto(TForm *Owner, VTApl *apl_owner, TWinControl *parent);
		  __fastcall ~TFormRelEqpto(void);
	void __fastcall  Atualiza(void);

private:	//métodos
   double     __fastcall CapacidadeTrechoMVA(VTTrecho *trecho);
   AnsiString __fastcall ConnectionString(AnsiString arq_db);
   void       __fastcall ConectaBaseDadoRelatorio(void);
   AnsiString __fastcall Carga_Curva(int tipica);
   AnsiString __fastcall Carga_Especial(int vip);
   AnsiString __fastcall Carga_Modelo(int modelo);
   AnsiString __fastcall Chave_Estado(VTChave *chave);
   void       __fastcall DeconectaBaseDadoRelatorio(void);
   void       __fastcall DesabilitaRelatorio(void);
   VTRede*    __fastcall ExisteRede(VTBarra *pbarra);
   void       __fastcall FormIniciaPosicao(void);
   int        __fastcall NumeroDeCarga(TList *lisCARGA, VTClasse *classe, TFase *fase);
	int        __fastcall NumeroDePatamarComFluxoCalculado(void);
	TMenuItem* __fastcall PopupMenuIniciaMenuItem(void);
   void       __fastcall PopupMenuRelatorioInicia(void);
   void       __fastcall PreencheRelBarra(void);
   void       __fastcall PreencheRelCapacitor(void);
   void       __fastcall PreencheRelCarga(void);
   void       __fastcall PreencheRelChave(void);
   void       __fastcall PreencheRelFluxoBarra(void);
	void       __fastcall PreencheRelFluxoRede(void);
//	void       __fastcall PreencheRelFluxoRegulador(void);
	void       __fastcall PreencheRelFluxoTrafo(void);
   void       __fastcall PreencheRelFluxoTrecho(void);
   void       __fastcall PreencheRelGerador(void);
   void       __fastcall PreencheRelPerfilCarga(void);
	void       __fastcall PreencheRelReator(void);
	void       __fastcall PreencheRelRegulador(void);
   void       __fastcall PreencheRelRede(void);
   void       __fastcall PreencheRelSuprimento(void);
   void       __fastcall PreencheRelTrafo(void);
   void       __fastcall PreencheRelTrafo3E(void);
   void       __fastcall PreencheRelTrecho(void);
   void       __fastcall PreencheRelBarCC(void);
   void       __fastcall PreencheRelLigCC(void);
   void       __fastcall ReiniciaTabela(AnsiString tab_name);
   AnsiString __fastcall TipoDeLigacao(int tipo_lig);
   AnsiString __fastcall TipoDeSuprimento(int suprimento);
   void       __fastcall ToolBarRedefineAction(bool rel_eqpto);
   AnsiString __fastcall TraduzModeloRegulador(int tipo);


private:	//objetos externos
   VTApl     *apl;

private:	//dados locais
   struct   {
            VTPatamar *patamar;
            } rel_fluxo;
   TList         *lisEQP;
   TFormPlanilha *FormPlanilha;
   VTProgresso *progresso;
   bool  preencheu[tipoRelCOUNT];
};

//---------------------------------------------------------------------------
#endif
//eof
