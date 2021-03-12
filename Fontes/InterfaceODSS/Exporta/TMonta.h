//---------------------------------------------------------------------------
#ifndef TMontaH
#define TMontaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTMonta.h"

//---------------------------------------------------------------------------
class VTEqpto;
class VTFases;

//---------------------------------------------------------------------------
class TMonta : public VTMonta
   {
   public:
								 __fastcall  TMonta(TStrings *lines);
								 __fastcall ~TMonta(void);
		virtual AnsiString __fastcall  AlgoritmoDeCalculo(void);
		virtual AnsiString __fastcall  ModeloDeRede(void);
		virtual bool       __fastcall  NewCapacitor(odssCAPACITOR &odss);
		virtual bool       __fastcall  NewCircuit(odssCIRCUIT &circuit);
		virtual bool       __fastcall  NewFault(odssFAULT &fault);
		virtual bool       __fastcall  NewGeneratorPV(odssGENERATOR &generator);
		virtual bool       __fastcall  NewIndMach012(odssINDMACH012 &indmach012);
		virtual bool       __fastcall  NewLine_ComLineCode(odssLINE &line);
		virtual bool       __fastcall  NewLine_SemLineCode(odssLINE &line);
		virtual bool       __fastcall  NewLineCode_MatZ(odssLINECODE &odss);
		virtual bool       __fastcall  NewLineCode_Z0Z1(odssLINECODE &odss);
		virtual bool       __fastcall  NewLoad(odssLOAD &load);
		virtual bool       __fastcall  NewLoadShape(odssLOADSHAPE &odss);
		virtual bool       __fastcall  NewLoadShape_pq(odssLOADSHAPE &odss);
		virtual bool       __fastcall  NewMonitor(odssMONITOR &monitor);
		virtual bool       __fastcall  NewRegulador(odssREGCONTROL &odss);
		virtual bool       __fastcall  NewTransformer_1F2E(odssTRANSFORMER &odss);
		virtual bool       __fastcall  NewTransformer_1F3E(odssTRANSFORMER &odss);
		virtual bool       __fastcall  NewTransformer_3F2E(odssTRANSFORMER &odss);
		virtual bool       __fastcall  NewTransformer_3F3E(odssTRANSFORMER &odss);

	protected: //métodos
      virtual AnsiString __fastcall MontaBusNodeCapacitor(odssBUS bus2, int ligacao_sinap);
      virtual AnsiString __fastcall MontaBusNodeCarga(odssBUS bus,  int fases_sinap);
      virtual AnsiString __fastcall MontaBusNodeTrecho(odssBUS bus, int fases_sinap);
      virtual AnsiString __fastcall MontaBusNodeTrafo(odssBUS bus, int fases, bool polar_fase=true);
      virtual bool       __fastcall TraduzTipoEnrolamentoTrafo(int ligacao, int fases_sinap, AnsiString &conn);

	protected: //objetos externos
      TStrings *lines;
   };

#endif
//---------------------------------------------------------------------------
//eof

