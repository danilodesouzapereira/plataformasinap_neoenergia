//---------------------------------------------------------------------------
#ifndef TFluxoG1H
#define TFluxoG1H
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
class VTGauss1;
//class VTMontaZ;
//class VTDemFase;
//class VTNo;

//---------------------------------------------------------------------------
class TFluxoG1 : public VTFluxo
   {
   public:
								 __fastcall TFluxoG1(VTApl *apl);
		virtual            __fastcall ~TFluxoG1(void);
		virtual void       __fastcall CfgAvisoErro(bool enabled);
		virtual bool       __fastcall Executa(VTNet *net, double Sbase,
                                            AnsiString str1, AnsiString str2);
		virtual AnsiString __fastcall Modelo(void);
		virtual bool       __fastcall UtilizaBarraPV(void);
		virtual bool       __fastcall ValidaAderencia(VTNet *net, VTLog *plog);

   protected:  //métodos acessados via property
      bool       __fastcall  PM_GetEquilibrado(void);

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
		VTData         *data;
		VTGauss1  *gauss1;
		//VTMontaZ  *montaZ;
		//TList     *lisEQP;
   };

#endif
//---------------------------------------------------------------------------
//eof
