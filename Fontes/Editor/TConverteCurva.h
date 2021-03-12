//---------------------------------------------------------------------------

#ifndef TConverteCurvaH
#define TConverteCurvaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "..\Curvas\VTCurva.h"

//---------------------------------------------------------------------------
class VTApl;
class VTCurva;
class VTEqpto;

//---------------------------------------------------------------------------
class TConverteCurva : public TObject
   {
   public:
	  __fastcall TConverteCurva(VTApl *apl);
	  __fastcall ~TConverteCurva(void);
	  void __fastcall DefineCurva(VTEqpto *eqpto, VTCurva *curva);
	  VTCurva* __fastcall FaseToTrifasica(void);
	  VTCurva* __fastcall TrifasicaToFase(void);
	  double __fastcall Valor(int ind_fase, double valor_tri, int fase_tag);

	private:  //métodos
		int __fastcall DeterminaFases(void);
		int __fastcall NumValores(int tipo);
        double __fastcall ValorPtri(double valor[nvCURVA_RRCAR]);
		double __fastcall ValorQtri(double valor[nvCURVA_RRCAR]);

	private:
		VTApl   *apl;
		VTCurva *curva_tri, *curva_fase;
		VTEqpto *eqpto;

   };
//---------------------------------------------------------------------------
#endif
