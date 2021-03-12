//---------------------------------------------------------------------------
#ifndef VTCalculaDemH
#define VTCalculaDemH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "..\Rede\Estrutura.h"

//---------------------------------------------------------------------------
class VTApl;
class VTBateria;
class VTCapacitor;
class VTCarga;
class VTReator;

//---------------------------------------------------------------------------
class VTCalculaDem : public TObject
   {
   public:  //property
		__property TDateTime  DataReferencia = {read=PM_GetDataReferencia, write=PM_SetDataReferencia};

   public:
						 __fastcall  VTCalculaDem(void) {};
		virtual      __fastcall ~VTCalculaDem(void) {};
		virtual bool __fastcall  GetValorBateria(VTBateria *bateria, int np, strDEM *str_dem) = 0;
		virtual bool __fastcall  GetValorCapacitor(VTCapacitor *capacitor, int np, strDEM *str_dem) = 0;
		virtual bool __fastcall  GetValorCarga(VTCarga *carga, int np, strDEM *str_dem) = 0;
		virtual bool __fastcall  GetValorCargaReducao(VTCarga *carga, int np, strDEM *str_dem) = 0;
		virtual bool __fastcall  GetValorReator(VTReator *reator, int np, strDEM *str_dem) = 0;
		virtual bool __fastcall  SetValorCapacitor(VTCapacitor *capacitor, int np, strDEM *str_dem) = 0;
		virtual bool __fastcall  SetValorCarga(VTCarga *carga, int np, strDEM *str_dem) = 0;
		virtual bool __fastcall  SetValorCargaReducao(VTCarga *carga, int np, strDEM *str_dem) = 0;
		virtual bool __fastcall  SetValorReator(VTReator *reator, int np, strDEM *str_dem) = 0;

	protected: //métodos acessados via property
		virtual TDateTime __fastcall PM_GetDataReferencia(void) = 0;
		virtual void      __fastcall PM_SetDataReferencia(TDateTime data) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe
VTCalculaDem* __fastcall NewObjCalculaDem(VTApl *apl);

#endif
//---------------------------------------------------------------------------
//eof
