//---------------------------------------------------------------------------
#ifndef TTrechoH
#define TTrechoH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTTrecho.h"

//---------------------------------------------------------------------------
class TTrecho : public VTTrecho
   {
   public:
                 __fastcall  TTrecho(int num_pat);
                 __fastcall ~TTrecho(void);
      VTTrecho*  __fastcall  Clone(void);
		void       __fastcall  CopiaAtributosDe(VTEqpto &ref);
		void       __fastcall  DefineObjArranjo(VTArranjo *arranjo);
		void       __fastcall  LisMutua(TList *lisMUTUA);
		int        __fastcall  Tipo(void);
		AnsiString __fastcall  TipoAsString(void);
		bool       __fastcall  Z0_ohm(double &r0, double &x0, double &yc0);
		bool       __fastcall  Z0_pu(double Sbase, double Vbase, double &r0, double &x0, double &yc0);
		bool       __fastcall  Z1_ohm(double &r1, double &x1, double &yc1);
		bool       __fastcall  Z1_pu(double Sbase, double Vbase, double &r1, double &x1, double &yc1);

	protected: //métodos virtuais redefinidos
		int        __fastcall Fases(VTBarra *barra);

	protected: //métodos acessados via property
      bool            __fastcall PM_GetArranjoTipico(void);
      double          __fastcall PM_GetCapacidade_amp(void);
      double          __fastcall PM_GetComprimento_m(void);
      double          __fastcall PM_GetComprimento_km(void);
      bool            __fastcall PM_GetEquilibrado(void);
      double          __fastcall PM_GetIadm_amp(int nf);
      VTMatC*         __fastcall PM_GetMatC(void);
      double          __fastcall PM_GetMatC_siemens(int i, int j);
      VTMatZ*         __fastcall PM_GetMatZ(void);
      complex<double> __fastcall PM_GetMatZ_ohm(int i, int j);
		complex<double> __fastcall PM_GetMatZ_ohm_km(int i, int j);
		bool            __fastcall PM_GetRamalLigacao(void);
		bool            __fastcall PM_GetRural(void);
		bool            __fastcall PM_GetUtilizaNeutro(void);
      void            __fastcall PM_SetComprimento_km(double comp_km);

   private:
      float   comp;     //km
   };

//---------------------------------------------------------------------------
#endif
//eof

