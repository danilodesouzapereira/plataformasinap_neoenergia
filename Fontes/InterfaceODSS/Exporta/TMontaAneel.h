//---------------------------------------------------------------------------
#ifndef TMontaAneelH
#define TMontaAneelH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TMonta.h"

//---------------------------------------------------------------------------
class VTEqpto;
class VTFases;

//---------------------------------------------------------------------------
class TMontaAneel : public TMonta
   {
   public:
					  __fastcall  TMontaAneel(TStrings *lines);
					  __fastcall ~TMontaAneel(void);
		AnsiString __fastcall  AlgoritmoDeCalculo(void);
		AnsiString __fastcall  ModeloDeRede(void);
//		bool       __fastcall  NewCapacitor(odssCAPACITOR &odss);
//		bool       __fastcall  NewCircuit(odssCIRCUIT &circuit);
//		bool       __fastcall  NewGeneratorPV(odssGENERATOR &generator);
		bool       __fastcall  NewLine_ComLineCode(odssLINE &line);
		bool       __fastcall  NewLine_SemLineCode(odssLINE &line);
		bool       __fastcall  NewLineCode_MatZ(odssLINECODE &odss);
//		bool       __fastcall  NewLineCode_Z0Z1(odssLINECODE &odss);
		bool       __fastcall  NewLoad(odssLOAD &load);
//		bool       __fastcall  NewLoadShape(odssLOADSHAPE &odss);
//		bool       __fastcall  NewLoadShape_pq(odssLOADSHAPE &odss);
//		bool       __fastcall  NewRegulador(odssREGCONTROL &odss);
//		bool       __fastcall  NewTransformer_1F2E(odssTRANSFORMER &odss);
//		bool       __fastcall  NewTransformer_1F3E(odssTRANSFORMER &odss);
//		bool       __fastcall  NewTransformer_3F2E(odssTRANSFORMER &odss);
//		bool       __fastcall  NewTransformer_3F3E(odssTRANSFORMER &odss);

	protected: //métodos
		//AnsiString __fastcall MontaBusNodeCapacitor(odssBUS bus2, int ligacao_sinap);
      AnsiString __fastcall MontaBusNodeCarga(odssBUS bus,  int fases_sinap);
      AnsiString __fastcall MontaBusNodeTrecho(odssBUS bus, int fases_sinap);
		//AnsiString __fastcall MontaBusNodeTrafo(odssBUS bus, int fases, bool polar_fase=true);
		//bool       __fastcall TraduzTipoEnrolamentoTrafo(int ligacao, int fases_sinap, AnsiString &conn);
	};

#endif
//---------------------------------------------------------------------------
//eof

