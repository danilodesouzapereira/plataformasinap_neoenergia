//---------------------------------------------------------------------------
#ifndef TCanalH
#define TCanalH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTCanal.h"

//---------------------------------------------------------------------------
class TCanal : public VTCanal
   {
   public:
                 __fastcall  TCanal(void);
					  __fastcall ~TCanal(void);
      VTCanal*   __fastcall  Clone(void);
		void       __fastcall  CopiaAtributosDe(VTEqpto &ref);
      int        __fastcall  Tipo(void);
      AnsiString __fastcall  TipoAsString(void);

	public:  //métodos derivados de VTEqpto  redefinidos para a classe
      VTCurva*   __fastcall  ExisteCurva(void);

	protected:  //métodos acessados via Property
		VTCurva*   __fastcall PM_GetCurva(void);
		bool       __fastcall PM_GetDescartavel(void);
      bool       __fastcall PM_GetEnabled(void);
		VTEqpto*   __fastcall PM_GetEqpto(void);
		double     __fastcall PM_GetErro_pu(VTPatamar *patamar);
		double     __fastcall PM_GetErroMaximo_pu(void);
		int        __fastcall PM_GetFases(void);
      double     __fastcall PM_GetFatorEstimadoMedido(VTPatamar *patamar);
      double     __fastcall PM_GetFatorNominalMedido(VTPatamar *patamar);
		VTMedidor* __fastcall PM_GetMedidor(void);
      int        __fastcall PM_GetNumero(void);
		double     __fastcall PM_GetSigma_pu(void);
      int        __fastcall PM_GetTipoCanal(void);
      AnsiString __fastcall PM_GetTipoCanalAsString(void);
      double     __fastcall PM_GetTolerancia_perc(void);
		double     __fastcall PM_GetValorEstimado(VTPatamar *patamar);
      double     __fastcall PM_GetValorNominal(VTPatamar *patamar);
		double     __fastcall PM_GetValorMedido(VTPatamar *patamar);
		//
		void       __fastcall PM_SetCurva(VTCurva *curva);
		void       __fastcall PM_SetDescartavel(bool descartavel);
      void       __fastcall PM_SetEnabled(bool enabled);
		void       __fastcall PM_SetEqpto(VTEqpto *eqpto);
		void       __fastcall PM_SetFases(int fases);
      void       __fastcall PM_SetMedidor(VTMedidor *medidor);
      void       __fastcall PM_SetNumero(int numero);
		void       __fastcall PM_SetSigma(double sigma);
      void       __fastcall PM_SetTipoCanal(int tipo_canal);
      void       __fastcall PM_SetTolerancia_perc(double tolerancia_perc);
		void       __fastcall PM_SetValorEstimado(VTPatamar *patamar, double valor);
      void       __fastcall PM_SetValorNominal(VTPatamar *patamar, double valor);
		void       __fastcall PM_SetValorMedido(VTPatamar *patamar, double valor);

   private: //dados locais
      #define  IND_VAL_MEDIDO   0
      #define  IND_VAL_ESTIMADO 1
      #define  IND_VAL_NOMINAL  2
		struct   {
					bool       enabled;
					bool       descartavel;
					int        num_seq;
               float      tolerancia_perc;
					int        fases;
					int        tipo_canal;
               VTMedidor  *medidor;
               VTEqpto    *eqpto;
               VTCurva    *curva;
					}PD;
   };

//---------------------------------------------------------------------------
#endif
//eof

 