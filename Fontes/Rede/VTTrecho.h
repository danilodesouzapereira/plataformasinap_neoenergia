//---------------------------------------------------------------------------
#ifndef VTTrechoH
#define VTTrechoH
using namespace std;

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include <complex>
#include "TLigacao.h"

//---------------------------------------------------------------------------
class VTArranjo;
class VTMatC;
class VTMatZ;

//---------------------------------------------------------------------------
class VTTrecho : public TLigacao
   {
   public:  //property
      __property bool            ArranjoTipico              = {read=PM_GetArranjoTipico};
		__property bool            RamalLigacao               = {read=PM_GetRamalLigacao};
		__property bool            Rural                      = {read=PM_GetRural};
		__property bool            UtilizaNeutro              = {read=PM_GetUtilizaNeutro};
		__property double          Capacidade_amp             = {read=PM_GetCapacidade_amp};
      __property double          Comprimento_m              = {read=PM_GetComprimento_m};
      __property double          Comprimento_km             = {read=PM_GetComprimento_km, write=PM_SetComprimento_km};
      __property bool            Equilibrado                = {read=PM_GetEquilibrado};
      __property double          Iadm_amp[int nf]           = {read=PM_GetIadm_amp};
      __property VTMatC*         MatC                       = {read=PM_GetMatC};
      __property double          MatC_siemens[int i][int j] = {read=PM_GetMatC_siemens};
      __property VTMatZ*         MatZ                       = {read=PM_GetMatZ};
      __property complex<double> MatZ_ohm[int i][int j]     = {read=PM_GetMatZ_ohm};
      __property complex<double> MatZ_ohm_km[int i][int j]  = {read=PM_GetMatZ_ohm_km};

   public:
                        __fastcall VTTrecho(void) {};
      virtual           __fastcall ~VTTrecho(void) {};
      virtual VTTrecho* __fastcall Clone(void) = 0;
      virtual void      __fastcall DefineObjArranjo(VTArranjo *arranjo)=0;
      virtual void      __fastcall LisMutua(TList *lisMUTUA)=0;
      virtual bool      __fastcall Z0_ohm(double &r0, double &x0, double &yc0) = 0;
      virtual bool      __fastcall Z0_pu(double Sbase, double Vbase, double &r0, double &x0, double &yc0) = 0;
      virtual bool      __fastcall Z1_ohm(double &r1, double &x1, double &yc1) = 0;
      virtual bool      __fastcall Z1_pu(double Sbase, double Vbase, double &r1, double &x1, double &yc1) = 0;

   protected: //métodos acessados via property
      virtual bool            __fastcall PM_GetArranjoTipico(void) = 0;
      virtual double          __fastcall PM_GetCapacidade_amp(void) = 0;
      virtual double          __fastcall PM_GetComprimento_m(void) = 0;
      virtual double          __fastcall PM_GetComprimento_km(void) = 0;
      virtual bool            __fastcall PM_GetEquilibrado(void) = 0;
      virtual double          __fastcall PM_GetIadm_amp(int nf) = 0;
      virtual VTMatC*         __fastcall PM_GetMatC(void) = 0;
      virtual double          __fastcall PM_GetMatC_siemens(int i, int j) = 0;
      virtual VTMatZ*         __fastcall PM_GetMatZ(void) = 0;
      virtual complex<double> __fastcall PM_GetMatZ_ohm(int i, int j) = 0;
		virtual complex<double> __fastcall PM_GetMatZ_ohm_km(int i, int j) = 0;
		virtual bool            __fastcall PM_GetRamalLigacao(void) = 0;
		virtual bool            __fastcall PM_GetRural(void) = 0;
		virtual bool            __fastcall PM_GetUtilizaNeutro(void) = 0;
		virtual void            __fastcall PM_SetComprimento_km(double comp_km) = 0;

   public:  //dados locais
      VTArranjo *arranjo;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTTrecho
//---------------------------------------------------------------------------
VTTrecho* __fastcall NewObjTrecho(int num_pat);

//---------------------------------------------------------------------------
#endif
//eof

