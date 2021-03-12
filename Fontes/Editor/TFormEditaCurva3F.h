//---------------------------------------------------------------------------

#ifndef TFormEditaCurva3FH
#define TFormEditaCurva3FH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Grids.hpp>
#include <ExtCtrls.hpp>
//#include <Chart.hpp>
//#include <TeEngine.hpp>
//#include <TeeProcs.hpp>
//#include <Series.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTCurva;
class VTPatamares;
class VTSuprimento;
class TFormEdtSup3;
//---------------------------------------------------------------------------
class TFormEditaCurva3F : public TForm
{
__published:	// IDE-managed Components
   TPanel *PanelCombos;
   TGroupBox *gbPatamares;
   TComboBox *CBoxNumPto;
   TStringGrid *SGrid;
   TGroupBox *gbGrandeza;
   TComboBox *CBoxTipoSup;
   TGroupBox *gbPontos;
   TStatusBar *sbUnidades;
   void __fastcall CBoxNumPtoChange(TObject *Sender);
   void __fastcall CBoxTipoSupChange(TObject *Sender);

public:		// User declarations
        __fastcall  TFormEditaCurva3F(TForm* form_owner, VTApl *apl_owner, TWinControl *parent);
        __fastcall ~TFormEditaCurva3F(void);
   void __fastcall  DefineCurva(VTCurva *curva);
	void __fastcall  InsereColuna(int ind_curva, AnsiString caption, double valor_min=-1e30, double valor_max=1e30);
	bool __fastcall  LePontosCurva(void);
	void __fastcall  SetSup(VTSuprimento *sup);
   void __fastcall  SGridInicia(void); //TIREI DO private



private:	//dados locais
   struct strCOLUNA  {
                     AnsiString caption;
                     int        ind_curva;
                     double     valor_min;
                     double     valor_max;
                     };

private:	//métodos
   void __fastcall CBoxNumPtoInicia(void);
   //void __fastcall ChartExibeCurva(VTVetval *vetval);
   void __fastcall ExibeDados(void);
   //bool __fastcall IsNumber(AnsiString string);
   bool __fastcall LeDadosPatamarFixo(void);
   bool __fastcall LeDadosPatamarVariavel(void);
   bool __fastcall SGridExistePeriodo(int nr);
   void __fastcall SGridIniciaPatamarFixo(void);
   void __fastcall SGridIniciaPatamarVariavel(void);
	bool __fastcall ValidaHora(int np, int hora);
	bool __fastcall ValidaTensao(double modA, double argA,double modB, double argB,double modC, double argC);
	bool __fastcall ValidaValor(int np, double valor, strCOLUNA &coluna);

private:	//objetos externos
	VTApl       *apl;
	VTCurva     *curva;
	TFormEdtSup3 *formEdtSup3;

private:	//dados locais
	VTPatamares *patamares;
	DynamicArray<strCOLUNA> coluna;
	VTSuprimento *psup;
	enum enumIND_GRANDFASE {//índices p/ as grandezas PQVT por fase
						indPa=0, indQa, indVa, indTa,
						indPb, indQb, indVb, indTb,
						indPc, indQc, indVc, indTc,
						indPn, indQn, indVn, indTn
						};


};

//---------------------------------------------------------------------------
#endif
//eof
