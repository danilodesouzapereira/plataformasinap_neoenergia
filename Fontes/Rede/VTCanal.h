//---------------------------------------------------------------------------
#ifndef VTCanalH
#define VTCanalH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTEqpto.h"

//---------------------------------------------------------------------------
#define  TOLERANCIA_DEFAULT              1.0
#define  TOLERANCIA_MEDICAO_DJ           1.0
#define  TOLERANCIA_MEDICAO_CHV          5.0
#define  TOLERANCIA_MEDICAO_CARGA      -10.0
#define  TOLERANCIA_MEDICAO_CAPACITOR    2.0

//---------------------------------------------------------------------------
//enum tipoCANAL {canalV=1, canalVF, canalPQ, canalI, canalIF, canalP, canalPV,
//					 canalVFHRM, canalIFHRM};

//---------------------------------------------------------------------------
enum tipoCANAL {canalINDEF = -1,
                canalV     =  1,
                canalI,
                canalP,
                canalQ,
                canalS,
					 canalVFHRM,
                canalIFHRM};

//---------------------------------------------------------------------------
class VTBarra;
class VTCurva;
class VTEqpto;
class VTMedidor;
class VTPatamar;

//---------------------------------------------------------------------------
class VTCanal : public VTEqpto
   {
	public:  //property
		__property int        Numero                                    = {read=PM_GetNumero,               write=PM_SetNumero               };
		__property int        TipoCanal                                 = {read=PM_GetTipoCanal,            write=PM_SetTipoCanal            };
		__property int        Fases                                     = {read=PM_GetFases,                write=PM_SetFases                };
		__property AnsiString TipoCanalAsString                         = {read=PM_GetTipoCanalAsString                                      };
		__property bool       Descartavel                               = {read=PM_GetDescartavel,          write=PM_SetDescartavel          };
		__property bool       Enabled                                   = {read=PM_GetEnabled,              write=PM_SetEnabled              };
		__property VTMedidor* Medidor                                   = {read=PM_GetMedidor,              write=PM_SetMedidor              };
		__property VTEqpto*   Eqpto                                     = {read=PM_GetEqpto,                write=PM_SetEqpto                };
		__property double     Sigma_pu                                  = {read=PM_GetSigma_pu                                               };
		__property double     Tolerancia_perc                           = {read=PM_GetTolerancia_perc,      write=PM_SetTolerancia_perc      };
		__property VTCurva*   Curva                                     = {read=PM_GetCurva,                write=PM_SetCurva                };
		__property double     Erro_pu[VTPatamar *patamar]               = {read=PM_GetErro_pu                                                };
		__property double     ErroMaximo_pu                             = {read=PM_GetErroMaximo_pu                                          };
		__property double     FatorEstimadoMedido[VTPatamar *patamar]   = {read=PM_GetFatorEstimadoMedido                                    };
		__property double     FatorNominalMedido[VTPatamar *patamar]    = {read=PM_GetFatorNominalMedido                                     };
		__property double     ValorEstimado[VTPatamar *patamar]         = {read=PM_GetValorEstimado,        write=PM_SetValorEstimado        };
		__property double     ValorMedido[VTPatamar *patamar]           = {read=PM_GetValorMedido,          write=PM_SetValorMedido          };
		__property double     ValorNominal[VTPatamar *patamar]          = {read=PM_GetValorNominal,         write=PM_SetValorNominal };

   public:
                        __fastcall  VTCanal(void) {};
      virtual           __fastcall ~VTCanal(void) {};
      virtual VTCanal*  __fastcall  Clone(void) = 0;

	protected:  //métodos acessados via Property
		virtual VTCurva*   __fastcall PM_GetCurva(void)=0;
		virtual bool       __fastcall PM_GetDescartavel(void)=0;
		virtual bool       __fastcall PM_GetEnabled(void)=0;
		virtual VTEqpto*   __fastcall PM_GetEqpto(void)=0;
		virtual double     __fastcall PM_GetErro_pu(VTPatamar *patamar)=0;
		virtual double     __fastcall PM_GetErroMaximo_pu(void)=0;
		virtual int        __fastcall PM_GetFases(void)=0;
      virtual double     __fastcall PM_GetFatorEstimadoMedido(VTPatamar *patamar)=0;
      virtual double     __fastcall PM_GetFatorNominalMedido(VTPatamar *patamar)=0;
		virtual VTMedidor* __fastcall PM_GetMedidor(void)=0;
		virtual int        __fastcall PM_GetNumero(void)=0;
		virtual double     __fastcall PM_GetSigma_pu(void)=0;
		virtual int        __fastcall PM_GetTipoCanal(void)=0;
		virtual AnsiString __fastcall PM_GetTipoCanalAsString(void)=0;
		virtual double     __fastcall PM_GetTolerancia_perc(void)=0;
		virtual double     __fastcall PM_GetValorEstimado(VTPatamar *patamar)=0;
		virtual double     __fastcall PM_GetValorNominal(VTPatamar *patamar)=0;
		virtual double     __fastcall PM_GetValorMedido(VTPatamar *patamar)=0;
		//
		virtual void       __fastcall PM_SetCurva(VTCurva *curva)=0;
		virtual void       __fastcall PM_SetDescartavel(bool descartavel)=0;
		virtual void       __fastcall PM_SetEnabled(bool enabled)=0;
		virtual void       __fastcall PM_SetEqpto(VTEqpto *eqpto)=0;
		virtual void       __fastcall PM_SetFases(int fases)=0;
		virtual void       __fastcall PM_SetMedidor(VTMedidor *medidor)=0;
      virtual void       __fastcall PM_SetNumero(int numero)=0;
		virtual void       __fastcall PM_SetTipoCanal(int tipo_canal)=0;
		virtual void       __fastcall PM_SetTolerancia_perc(double tolerancia_perc)=0;
		virtual void       __fastcall PM_SetValorEstimado(VTPatamar *patamar, double valor)=0;
		virtual void       __fastcall PM_SetValorNominal(VTPatamar *patamar, double valor)=0;
		virtual void       __fastcall PM_SetValorMedido(VTPatamar *patamar, double valor)=0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe Canal
//---------------------------------------------------------------------------
VTCanal* __fastcall NewObjCanal(void);

//---------------------------------------------------------------------------
#endif
//eof

 