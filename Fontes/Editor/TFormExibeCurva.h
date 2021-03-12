//---------------------------------------------------------------------------
#ifndef TFormExibeCurvaH
#define TFormExibeCurvaH

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
#include "..\Curvas\VTCurva.h"

//---------------------------------------------------------------------------
class VTApl;
class VTEqpto;
class VTCurva;
class VTLegenda;
class VTVetval;
class TConverteCurva;

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
	TGroupBox *gbTipo;
	TComboBox *CBoxTipoCurva;
	TFastLineSeries *Series5;
	TFastLineSeries *Series6;
   void __fastcall ChartClick(TObject *Sender);
	void __fastcall CBoxTipoCurvaChange(TObject *Sender);

public:		// User declarations
        __fastcall  TFormExibeCurva(TForm* form_owner, VTApl *apl_owner, TWinControl *parent);
        __fastcall ~TFormExibeCurva(void);
   void __fastcall  DefineCurva(VTEqpto *eqpto, VTCurva *curva);
   void __fastcall  HabilitaCurva(int atributo_id);
   void __fastcall  HabilitaCurvaPorFase(void);

private: //metodos
	void __fastcall  HabilitaPorFase(void);
	void __fastcall  HabilitaTrifasico(void);
	AnsiString __fastcall Unidade(int unid_curva);

private:	//objetos externos
   VTApl       *apl;
   VTCurva     *curva;
   VTEqpto     *eqpto, *clone;

private:	//dados locais
   VTLegenda   *legenda;
   TList       *lisVETVAL;
   TConverteCurva *converte;

};

//---------------------------------------------------------------------------
#endif
//eof
