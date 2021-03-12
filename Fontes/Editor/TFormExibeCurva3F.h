//---------------------------------------------------------------------------
#ifndef TFormExibeCurva3FH
#define TFormExibeCurva3FH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Grids.hpp>
#include <ExtCtrls.hpp>
#include <CheckLst.hpp>
#include <VCLTee.Chart.hpp>
#include <VCLTee.Series.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
#include <VclTee.TeeGDIPlus.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTEqpto;
class VTCurva;
class VTLegenda;
class VTVetval;

//---------------------------------------------------------------------------
class TFormExibeCurva : public TForm
{
__published:	// IDE-managed Components
   TChart *Chart;
   TFastLineSeries *Series1;
   TFastLineSeries *Series2;
   TFastLineSeries *Series3;
   TFastLineSeries *Series4;
   TRadioGroup *RGroupSerie;
   void __fastcall ChartClick(TObject *Sender);

public:		// User declarations
        __fastcall  TFormExibeCurva(TForm* form_owner, VTApl *apl_owner, TWinControl *parent);
        __fastcall ~TFormExibeCurva(void);
   void __fastcall  DefineCurva(VTEqpto *eqpto, VTCurva *curva);
   void __fastcall  HabilitaCurva(int atributo_id);

private:	//objetos externos
   VTApl       *apl;
   VTCurva     *curva;
   VTEqpto     *eqpto;

private:	//dados locais
   VTLegenda   *legenda;
   TList       *lisVETVAL;
};

//---------------------------------------------------------------------------
#endif
//eof
