//---------------------------------------------------------------------------
#ifndef TFormEditaCurvaH
#define TFormEditaCurvaH

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

//---------------------------------------------------------------------------
class TFormEditaCurva : public TForm
{
__published:	// IDE-managed Components
   TGroupBox *GroupBox2;
   TComboBox *CBoxNumPto;
   TGroupBox *GroupBox3;
   TStringGrid *SGrid;
   void __fastcall CBoxNumPtoChange(TObject *Sender);

public:		// User declarations
        __fastcall  TFormEditaCurva(TForm* form_owner, VTApl *apl_owner, TWinControl *parent);
        __fastcall ~TFormEditaCurva(void);
   void __fastcall  DefineCurva(VTCurva *curva);
	void __fastcall  InsereColuna(int ind_curva, AnsiString caption, double valor_min=-1e30, double valor_max=1e30);
   bool __fastcall  LePontosCurva(void);

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
   bool __fastcall LeDadosPatamarFixo(void);
   bool __fastcall LeDadosPatamarVariavel(void);
   bool __fastcall SGridExistePeriodo(int nr);
   void __fastcall SGridInicia(void);
   void __fastcall SGridIniciaPatamarFixo(void);
   void __fastcall SGridIniciaPatamarVariavel(void);
   bool __fastcall ValidaHora(int np, int hora);
	bool __fastcall ValidaValor(int np, double valor, strCOLUNA &coluna);

private:	//objetos externos
   VTApl       *apl;
   VTCurva     *curva;

private:	//dados locais
   VTPatamares *patamares;
   DynamicArray<strCOLUNA> coluna;

};

//---------------------------------------------------------------------------
#endif
//eof
