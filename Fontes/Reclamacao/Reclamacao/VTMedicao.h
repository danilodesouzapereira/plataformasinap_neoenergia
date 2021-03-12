//---------------------------------------------------------------------------
#ifndef VTMedicaoH
#define VTMedicaoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Fontes\Constante\Fases.h>
#include <Fontes\Complexo\Complexo.h>

//---------------------------------------------------------------------------
struct strREGISTRO{
                  int    num_seq;
                  int    dia;
                  int    mes;
                  int    ano;
                  int    hora;
						int    minuto;
						double v1;
						double v2;
						double v3;
						complex<double> v1_cmplx;
						complex<double> v2_cmplx;
						complex<double> v3_cmplx;
                  };

//---------------------------------------------------------------------------
class VTApl;
class VTCarga;
class VTTensao;

//---------------------------------------------------------------------------
class VTMedicao : public TObject
   {
   public:  //property
      __property TDateTime Data[int ind_dia]         = {read=PM_GetData                                        };
      __property int       NumeroDia                 = {read=PM_GetNumeroDia                                   };
      __property int       NumeroMedicaoPorDia       = {read=PM_GetNumeroMedicaoPorDia                         };
      __property int       NumeroMedicaoTotal        = {read=PM_GetNumeroMedicaoTotal                          };
      __property bool      FaseEnabled[int ind_fase] = {read=PM_GetFaseEnabled        , write=PM_SetFaseEnabled};
      __property int       FasesRede                 = {read=PM_GetFasesRede          , write=PM_SetFasesRede  };
      __property VTCarga*  Carga                     = {read=PM_GetCarga              , write=PM_SetCarga      };

   public:
                        __fastcall  VTMedicao(void) {};
                        __fastcall ~VTMedicao(void) {};
      virtual bool      __fastcall  AlteraMedicao(int ind_dia, int ind_pat, double vfn_kv[MAX_FASE]) = 0;
      virtual bool      __fastcall  ExisteMedicao(TDateTime data, int ind_pat) = 0;
      virtual bool      __fastcall  ExisteMedicao(TDateTime data, int ind_pat, double vfn_kv[MAX_FASE]) = 0;
      virtual bool      __fastcall  ExisteMedicao(int ind_dia, int ind_pat, double vfn_kv[MAX_FASE]) = 0;
      virtual VTTensao* __fastcall  ExisteTensao(TDateTime data) = 0;
      virtual int       __fastcall  InsereMedicao(strREGISTRO &reg) = 0;
      virtual void      __fastcall  Reinicia(void) = 0;

   protected:  //métodos acessados vias property
      virtual VTCarga*  __fastcall  PM_GetCarga(void) = 0;
      virtual TDateTime __fastcall  PM_GetData(int ind_dia) = 0;
      virtual bool      __fastcall  PM_GetFaseEnabled(int ind_fase) = 0;
      virtual int       __fastcall  PM_GetFasesRede(void) = 0;
      virtual int       __fastcall  PM_GetNumeroDia(void) = 0;
      virtual int       __fastcall  PM_GetNumeroMedicaoPorDia(void) = 0;
      virtual int       __fastcall  PM_GetNumeroMedicaoTotal(void) = 0;
      virtual void      __fastcall  PM_SetCarga(VTCarga* carga) = 0;
      virtual void      __fastcall  PM_SetFaseEnabled(int ind_fase, bool enabled) = 0;
      virtual void      __fastcall  PM_SetFasesRede(int fases_rede) = 0;

   protected:  //dados acessados vias property
   };
                                      
//---------------------------------------------------------------------------
VTMedicao*  __fastcall NewObjMedicao(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof


