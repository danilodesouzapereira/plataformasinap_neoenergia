//---------------------------------------------------------------------------
#ifndef TFormEdtCurvaMedH
#define TFormEdtCurvaMedH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Grids.hpp>
#include <ExtCtrls.hpp>
#include <VCLTee.Chart.hpp>
#include <VCLTee.Series.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
#include <VclTee.TeeGDIPlus.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTCurva;
class VTCanal;
class VTLegenda;
class VTPatamares;
class VTVetval;

//---------------------------------------------------------------------------
class TFormEdtCurvaMed : public TForm
{
__published:	// IDE-managed Components
   TPageControl *PageControl;
   TTabSheet *TabSheetPonto;
   TTabSheet *TabSheetChart;
   TGroupBox *GroupBox3;
   TStringGrid *SGrid;
   TChart *chart;
   TFastLineSeries *Series1;
   TPanel *Panel1;
   TGroupBox *GroupBox5;
   TEdit *EditCodigo;
   TGroupBox *GBoxTolerancia;
   TEdit *EditTolerancia;
   TGroupBox *GroupBox2;
   TComboBox *CBoxNumPto;
	TPanel *Panel2;
   void __fastcall CBoxNumPtoChange(TObject *Sender);
   void __fastcall PageControlChanging(TObject *Sender, bool &AllowChange);
   void __fastcall PageControlChange(TObject *Sender);
   void __fastcall FormShow(TObject *Sender);

public:  //property
   __property VTCanal* Canal = {read=PM_GetCanal, write=PM_SetCanal};

public:		// User declarations
        __fastcall TFormEdtCurvaMed(TForm* form_owner, VTApl *apl_owner, TWinControl *parent);
        __fastcall ~TFormEdtCurvaMed(void);
   bool __fastcall LeDadosCanal(void);

private:	//métodos acessados via property
   VTCanal* __fastcall PM_GetCanal(void);
   void     __fastcall PM_SetCanal(VTCanal *canal);

private:	//métodos
	void __fastcall CBoxBarraRefInicia(void);
	void __fastcall CBoxNumPtoInicia(void);
	void __fastcall CBoxTipoCanalInicia(void);
	void __fastcall ChartExibeCurva(VTVetval *vetval);
	void __fastcall ChartExibeCurva(void);
	bool __fastcall LeDadosPatamarFixo(void);
	bool __fastcall LeDadosPatamarVariavel(void);
	bool __fastcall LePontosCurva(void);
	bool __fastcall LeValidaPontosCurva(void);
	void __fastcall SGridAcertaColunas(void);
	bool __fastcall SGridExistePeriodo(int nr);
	void __fastcall SGridInicia(void);
	bool __fastcall ValidaHora(int np, int hora);
	bool __fastcall ValidaI_amp(int np, double i_amp);
   bool __fastcall ValidaP_mw(int np, double p_mw);
   bool __fastcall ValidaQ_mvar(int np, double q_mvar);
	bool __fastcall ValidaV_pu(int np, double v_pu);
	bool __fastcall ValidaS_MVA(int np, double s_mva);
//   bool __fastcall ValidaT_grau(int np, double t_grau);

private:	//objetos externos
   VTApl       *apl;
   VTCurva     *curva;

private:	//dados locais
   bool        pot_neg_enabled;
   VTLegenda   *legenda;
   VTPatamares *patamares;
   TList       *lisVETVAL;
//	union unionVALOR  {
//							double vet[5];
//							struct {double p, q, v, i, t;} var;
//                     };

public:
   VTCanal   *canal;
//   struct      {
//               bool ColunaHoraIni;
//               bool ColunaHoraFim;
//               bool ColunaP;
//               bool ColunaQ;
//					bool ColunaV;
//					bool ColunaI;
//					bool ColunaF;
//					bool ColunaS;
//					} Exibe;
};

//---------------------------------------------------------------------------
#endif
//eof
