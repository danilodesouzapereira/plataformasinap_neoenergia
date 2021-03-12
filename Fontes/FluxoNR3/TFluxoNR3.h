//---------------------------------------------------------------------------
#ifndef TFluxoNR3H
#define TFluxoNR3H
using namespace std;

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <complex>
#include "..\Fluxo\VTFluxo.h"

//---------------------------------------------------------------------------
//class VTBarra;
class VTData;
//class VTDemanda;
//class VTFases;
//class VTFornecimento;
class VTNR3;
//class VTMontaZ;
//class VTDemFase;
//class VTNo;

//---------------------------------------------------------------------------
class TFluxoNR3 : public VTFluxo
   {
   public:
								 __fastcall  TFluxoNR3(VTApl *apl);
		virtual            __fastcall ~TFluxoNR3(void);
		virtual void       __fastcall  CfgAvisoErro(bool enabled);
		virtual bool       __fastcall  Executa(VTNet *net, double Sbase,
                                             AnsiString str1, AnsiString str2);
		virtual AnsiString __fastcall  Modelo(void);
      virtual bool       __fastcall  PM_GetEquilibrado(void);
		virtual bool       __fastcall  UtilizaBarraPV(void);
		virtual bool       __fastcall  ValidaAderencia(VTNet *net, VTLog *plog);
/*
	protected: //métodos
		virtual int  __fastcall SentidoFluxo(double fluxo);
		virtual complex<double> __fastcall ValidaFluxo(complex<double> s_mva);
*/
	private:


	protected: //objetos externos
      VTApl          *apl;
		//VTNet          *net;
		//VTDemanda      *demanda;
		//VTFases        *fases;
		//VTFornecimento *fornecimento;

	protected: //dados locais
		int       ano;
		double    Sbase;
		bool      aviso_enabled;
		VTNR3     *nr3;
		VTData    *data;
		//VTMontaZ  *montaZ;
		TList     *lisEQP;
   };

#endif
//---------------------------------------------------------------------------
//eof
