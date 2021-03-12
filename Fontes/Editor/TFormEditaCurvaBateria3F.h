//---------------------------------------------------------------------------
#ifndef TFormEditaCurvaCarga3FH
#define TFormEditaCurvaCarga3FH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Grids.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTCurva;
class VTCarga;
class VTPatamares;
class TFormEditaCurvaCarga;
class TFormEdtBateria;

//---------------------------------------------------------------------------
class TFormEditaCurvaBateria3F : public TForm
{
__published:	// IDE-managed Components
	TPanel *PanelSelect;
	TGroupBox *gbTipo;
	TGroupBox *gpPatamares;
	TGroupBox *gbPontos;
	TStatusBar *sbUnidades;
	TStringGrid *SGrid;
	TComboBox *CBoxTipoCarga;
	TComboBox *CBoxNumPto;
	void __fastcall CBoxNumPtoChange(TObject *Sender);
	void __fastcall CBoxTipoCargaChange(TObject *Sender);

	void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFormEditaCurvaBateria3F(TComponent* Owner, TForm* form_owner, VTApl *apl_owner, TWinControl *parent);
	__fastcall ~TFormEditaCurvaBateria3F(void);
	void __fastcall  DefineCurva(VTCurva *curva);
    bool __fastcall LePontosCurva(void);
	void __fastcall SGridInicia(void);

private:	//dados locais
   struct strCOLUNA  {
                     AnsiString caption;
                     int        ind_curva;
                     double     valor_min;
                     double     valor_max;
                     };

private:	//m�todos
   void __fastcall CBoxNumPtoInicia(void);
	void __fastcall ExibeDados(void);
	bool __fastcall LeDadosPatamarFixo(void);
	bool __fastcall LeDadosPatamarVariavel(void);
	void __fastcall SGridIniciaPatamarFixo(void);
	void __fastcall SGridIniciaPatamarVariavel(void) ;
	bool __fastcall SGridExistePeriodo(int nr) ;
	void __fastcall TrataPorFaseParaTrifasico(void);
	void __fastcall TrataTrifasicoParaPorFase(void);

private:	//objetos externos
   VTApl       *apl;
   VTCurva     *curva;
   TFormEdtBateria *formEdtBateria;

private:	//dados locais
   VTPatamares *patamares_local;
   int colunas, fase_tag;
   bool por_fase;

};

//---------------------------------------------------------------------------
#endif
