//---------------------------------------------------------------------------
#ifndef VTTramoH
#define VTTramoH
using namespace std;

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include <complex>
#include "TLigacao.h"

//---------------------------------------------------------------------------
class VTArranjo;
class VTMatZ;
class VTTrecho;

//---------------------------------------------------------------------------
class VTTramo : public TLigacao
   {
   public:  //property
      __property double          Capacidade_amp         = {read=PM_GetCapacidade_amp};
      __property double          Comprimento_m          = {read=PM_GetComprimento_m};
      __property double          Comprimento_km         = {read=PM_GetComprimento_km};
      __property double          Iadm_amp[int nf]       = {read=PM_GetIadm_amp};
      __property complex<double> Z_ohm[int i][int j]    = {read=PM_GetZ_ohm};
      __property int             TipoCfg                = {read=PM_GetTipoCfg};
      __property bool            Equilibrado            = {read=PM_GetEquilibrado};
      __property bool            UtilizaNeutro          = {read=PM_GetUtilizaNeutro};
      __property VTMatZ*         MatZ                   = {read=PM_GetMatZ};

   public:
                        __fastcall VTTramo(void) {};
      virtual           __fastcall ~VTTramo(void) {};
      virtual VTTramo*  __fastcall Clone(void) = 0;
      virtual bool      __fastcall InsereTrecho(VTTrecho *trecho)=0;
      virtual TList*    __fastcall LisBarraInterna(void)=0;
      virtual TList*    __fastcall LisTrecho(void)=0;
      virtual void      __fastcall LisTrecho(TList *lisEXT)=0;
      virtual bool      __fastcall Z0_ohm(double &r0, double &x0, double &yc0) = 0;
      virtual bool      __fastcall Z0_pu(double Sbase, double Vbase, double &r0, double &x0, double &yc0) = 0;
      virtual bool      __fastcall Z1_ohm(double &r1, double &x1, double &yc1) = 0;
      virtual bool      __fastcall Z1_pu(double Sbase, double Vbase, double &r1, double &x1, double &yc1) = 0;

   protected: //métodos acessados via property
      virtual double          __fastcall PM_GetCapacidade_amp(void) = 0;
      virtual double          __fastcall PM_GetComprimento_m(void) = 0;
      virtual double          __fastcall PM_GetComprimento_km(void) = 0;
      virtual bool            __fastcall PM_GetEquilibrado(void) = 0;
      virtual double          __fastcall PM_GetIadm_amp(int nf) = 0;
      virtual VTMatZ*         __fastcall PM_GetMatZ(void) = 0;
      virtual int             __fastcall PM_GetTipoCfg(void) = 0;
      virtual bool            __fastcall PM_GetUtilizaNeutro(void) = 0;
      virtual complex<double> __fastcall PM_GetZ_ohm(int i, int j) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTTramo
//---------------------------------------------------------------------------
VTTramo* __fastcall NewObjTramo(int num_pat);

//---------------------------------------------------------------------------
#endif
//eof

