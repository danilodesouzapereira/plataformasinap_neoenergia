//---------------------------------------------------------------------------
#ifndef VTMontaH
#define VTMontaH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
#define MAX_NODE 4

//---------------------------------------------------------------------------
//enum tipos de gerador
enum odssTIPOGER  {
                  gerP_FP = 1, //Generator injects a constant kW at specified power factor.
                  gerGB   = 2, //Generator is modeled as a constant admittance.
                  gerP_V  = 3, //Const kW, constant kV.  Somewhat like a conventional transmission power flow P-V generator.
                  gerP_Q  = 4, //Const kW, Fixed Q (Q never varies)
                  gerP_X  = 5, //Const kW, Fixed Q(as a constant reactance)
                  gerUWM  = 6, //Compute load injection from User-written Model.(see usage of Xd, Xdp)
                  gerCL   = 7  //Constant kW, kvar, but current-limited below Vminpu. Approximates a simple inverter. See also Balanced.
                  };
//---------------------------------------------------------------------------
//enum tipos de gerador
enum odssLOADMODEL   {
                     loadScte      = 1,  //Standard constant P+jQ load. (Default)
                     loadZcte      = 2,  //Constant impedance load.
                     loadPcteQquad = 3,  //Const P, Quadratic Q (like a motor).
                     loadPlinQquad = 4,  //Nominal Linear P, Quadratic Q (feeder mix). Use this with CVRfactor.
                     loadIcte      = 5,  //Constant Current Magnitude
                     loadPcteQfix  = 6,  //Const P, Fixed Q
                     loadPcteQZfix = 7,  //Const P, Fixed Impedance Q
                     loadZIPV      = 8,  //ZIPV (7 values)
                     loadDEFAULT   = loadScte
                     };

//---------------------------------------------------------------------------
struct odssBUS          {
                        int        sinap_id;
                        int        sinap_fases;
                        };
//---------------------------------------------------------------------------
struct odssCAPACITOR    {
                        AnsiString     codigo;
                        int            sinap_id;
                        int            sinap_ligacao;
                        odssBUS        bus1;
                        odssBUS        bus2;
                        int            phases;
                        AnsiString     conn;
                        double         q_kvar;
                        double         vff_kv;
                        };
//---------------------------------------------------------------------------
struct odssCIRCUIT      {
                        AnsiString     codigo;
                        int            sinap_id;
                        odssBUS        bus1;
                        int            phases;
                        double         vbase_kv;
                        double         vref_pu;
                        double         pcc3f_mw;
                        double         qcc3f_mvar;
                        double         pcc1f_mw;
                        double         qcc1f_mvar;
						};
//---------------------------------------------------------------------------
struct odssFAULT      	{
						AnsiString     codigo;
						odssBUS        bus1;
						odssBUS        bus2;
						int            phases;
						double         r;
						double		   onTime;
						};
//---------------------------------------------------------------------------
struct odssGENERATOR    {
                        AnsiString     codigo;
                        odssBUS        bus1;
						int            phases;
                        int            model;
                        double         vnom_kv;
                        double         vref_pu;
                        double         p_kw;
                        double         qmax_kvar;
                        double         qmin_kvar;
                        double         pvfactor;
						};
//---------------------------------------------------------------------------
struct odssINDMACH012    {
                        AnsiString     codigo;
                        odssBUS        bus1;
						int            sinap_ligacao;
						double         vnom_kv;
						double		   slip;
						double		   s_kva;
						double		   p_kw;
						double		   h_minercia;
						double		   rs_pu;
						double		   xs_pu;
						double		   rr_pu;
						double		   xr_pu;
						double		   xm_pu;
						};
//---------------------------------------------------------------------------
struct odssLINE         {
                        AnsiString     codigo;
                        odssBUS        bus1;
                        odssBUS        bus2;
                        AnsiString     linecode;
                        int            sinap_fases;
                        int            phases;
                        double         r1_ohm_km;
                        double         x1_ohm_km;
                        double         c1_nF_km;
                        double         r0_ohm_km;
                        double         x0_ohm_km;
                        double         c0_nF_km;
                        double         comp_km;
                        };
//---------------------------------------------------------------------------
struct odssLINECODE     {
                        int            id;
                        AnsiString     codigo;
                        AnsiString     units;
                        int            basefreq;
                        double         normamps;
                        double         r1_ohm_km;
                        double         x1_ohm_km;
                        double         c1_nF_km;
                        double         r0_ohm_km;
                        double         x0_ohm_km;
                        double         c0_nF_km;
                        int            nphases;
                        bool           kron;
                        int            neutral;
                        double         cmatrix[MAX_NODE][MAX_NODE];
                        double         rmatrix[MAX_NODE][MAX_NODE];
                        double         xmatrix[MAX_NODE][MAX_NODE];
                        };
//---------------------------------------------------------------------------
struct odssLOAD         {
                        AnsiString     codigo;
                        odssBUS        bus1;
                        int            loadshape_id;
                        int            sinap_numfase;
                        int            sinap_fases;
                        int            sinap_zona;
                        int            model;
                        double         zipv[7];
                        //int            ligacao;
                        double         vff_kv;
                        double         vfn_kv;
                        double         vmin_pu;
                        double         p_kw;
                        double         q_kvar;
                        };
//---------------------------------------------------------------------------
struct odssLOADSHAPE    {
                        int            id;
                        AnsiString     codigo;
                        int            npts;
                        struct{
                              bool   enabled;
                              double valor[1440]; //24 * 60 => 1 ponto por minuto
                              }curva_p, curva_q;
						};
//---------------------------------------------------------------------------
struct odssMONITOR    	{
						AnsiString     codigo;
						AnsiString     element;
						int			   terminal;
						int 		   mode;
						bool		   ppolar;
						};
//---------------------------------------------------------------------------
struct odssWINDING      {
                        odssBUS        bus;
						int            sinap_ligacao;
                        int            sinap_fases;
                        double         vnom_kv;
                        double         vtap_pu;
                        double         snom_kva;
                        double         r_perc;
                        };
//---------------------------------------------------------------------------
struct odssREGCONTROL  { //DVK 2014.09.19
                        AnsiString     codigo;
                        bool           automatico;
                        int            phases;
                        int            winding;
                        double         xhl;
                        double         loadloss;
                        double         noloadloss;
                        int            vreg;
                        double         ptratio;
                        double         min_tap;
                        double         max_tap;
                        double         num_tap;
                        odssWINDING    win1;
                        odssWINDING    win2;
                        };
//---------------------------------------------------------------------------
struct odssTRANSFORMER  {
                        AnsiString     codigo;
                        int            phases;
                        double         xhl;
                        double         xht;
                        double         xlt;
                        double         loadloss;
                        double         noloadloss;
                        odssWINDING    win1;
                        odssWINDING    win2;
                        odssWINDING    win3;
                        };

//---------------------------------------------------------------------------
class VTMonta : public TObject
   {
   public:
								 __fastcall  VTMonta(void) {};
								 __fastcall ~VTMonta(void) {};
		virtual AnsiString __fastcall  AlgoritmoDeCalculo(void) = 0;
		virtual AnsiString __fastcall  ModeloDeRede(void) = 0;
		virtual bool       __fastcall  NewCapacitor(odssCAPACITOR &odss) = 0;
		virtual bool       __fastcall  NewCircuit(odssCIRCUIT &circuit) = 0;
		virtual bool       __fastcall  NewFault(odssFAULT &fault)=0;
		virtual bool       __fastcall  NewGeneratorPV(odssGENERATOR &generator)=0;
		virtual bool       __fastcall  NewIndMach012(odssINDMACH012 &indmach012)=0;
		virtual bool       __fastcall  NewLine_ComLineCode(odssLINE &line) = 0;
		virtual bool       __fastcall  NewLine_SemLineCode(odssLINE &line) = 0;
		virtual bool       __fastcall  NewLineCode_MatZ(odssLINECODE &odss) = 0;
		virtual bool       __fastcall  NewLineCode_Z0Z1(odssLINECODE &odss) = 0;
		virtual bool       __fastcall  NewLoad(odssLOAD &load) = 0;
		virtual bool       __fastcall  NewLoadShape(odssLOADSHAPE &odss) = 0;
		virtual bool       __fastcall  NewLoadShape_pq(odssLOADSHAPE &odss) = 0;
		virtual bool       __fastcall  NewMonitor(odssMONITOR &monitor)=0;
		virtual bool       __fastcall  NewRegulador(odssREGCONTROL &odss) = 0;
		virtual bool       __fastcall  NewTransformer_1F2E(odssTRANSFORMER &odss) = 0;
		virtual bool       __fastcall  NewTransformer_1F3E(odssTRANSFORMER &odss) = 0;
		virtual bool       __fastcall  NewTransformer_3F2E(odssTRANSFORMER &odss) = 0;
		virtual bool       __fastcall  NewTransformer_3F3E(odssTRANSFORMER &odss) = 0;

   protected: //macros
      #define fmtPQ   "%9.8f"
      #define fmt3_2   "%3.2f"
      #define fmt4_3   "%4.3f"
      #define fmt5_4   "%5.4f"
      #define fmt6_5   "%6.5f"
      #define fmt10_9  "%10.9f"
   };

//---------------------------------------------------------------------------
VTMonta* __fastcall NewObjMonta(TStrings *lines);
VTMonta* __fastcall NewObjMontaAneel(TStrings *lines);

#endif
//---------------------------------------------------------------------------
//eof

