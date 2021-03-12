//---------------------------------------------------------------------------
#ifndef TCalculaDemH
#define TCalculaDemH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTCalculaDem.h"

//---------------------------------------------------------------------------
class VTMercado;
class VTPatamares;

//---------------------------------------------------------------------------
class TCalculaDem : public VTCalculaDem
   {
   public:
			  __fastcall  TCalculaDem(VTApl *apl);
			  __fastcall ~TCalculaDem(void);

   private: //métodos
	  bool __fastcall GetValorBateria(VTBateria *bateria, int np, strDEM *str_dem);
      bool __fastcall GetValorCapacitor(VTCapacitor *capacitor, int np, strDEM *str_dem);
      bool __fastcall GetValorCarga(VTCarga *carga, int np, strDEM *str_dem);
      bool __fastcall GetValorCargaIP(VTCarga *carga, int np, strDEM *str_dem);
      bool __fastcall GetValorCargaReducao(VTCarga *carga, int np, strDEM *str_dem);
      bool __fastcall GetValorReator(VTReator *reator, int np, strDEM *str_dem);
      bool __fastcall SetValorCapacitor(VTCapacitor *capacitor, int np, strDEM *str_dem);
      bool __fastcall SetValorCarga(VTCarga *carga, int np, strDEM *str_dem);
      bool __fastcall SetValorCargaReducao(VTCarga *carga, int np, strDEM *str_dem);
      bool __fastcall SetValorReator(VTReator *reator, int np, strDEM *str_dem);

	protected: //métodos acessados via property
		TDateTime __fastcall PM_GetDataReferencia(void);
		void      __fastcall PM_SetDataReferencia(TDateTime data);

	private: //objetos externos
		VTApl       *apl;
      VTMercado   *mercado;
      VTPatamares *patamares;

	private: //dados locais
		struct{
            TDateTime data_referencia;
            int       dias_mes;
            }PD;
	};

#endif
//---------------------------------------------------------------------------
//eof
