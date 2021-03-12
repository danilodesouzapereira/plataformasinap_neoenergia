//---------------------------------------------------------------------------
#ifndef VTArranjoH
#define VTArranjoH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include <complex>
#include "..\Rede\Estrutura.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTFases.h"
using namespace std;

//---------------------------------------------------------------------------
//enum que define configurações do Arranjo
enum enumCFG_ARRANJO {arranjoCABOxFASE=0, arranjoCABO_Z0Z1,  arranjoZ0Z1,
                      arranjoMAT_Z,       arranjoPU_SB100,   arranjoMUTUA};

//---------------------------------------------------------------------------
#define RES_TERRA_DEFAULT  100

//---------------------------------------------------------------------------
class VTCabo;
class VTMatC;
class VTMatZ;
class VTPtoFixacao;
class VTSuporte;

//---------------------------------------------------------------------------
class VTArranjo : public VTEqpto
   {
   public:  //property
      __property TColor          Color                        = {read=PM_GetColor,    write=PM_SetColor    };
      __property TDate           Data                         = {read=PM_GetData,     write=PM_SetData     };
      __property int             Fases                        = {read=PM_GetFases,    write=PM_SetFases    };
      __property double          Ifase_amp[int nf]            = {read=PM_GetIfase_amp                      };
      __property double          Iadm_amp                     = {read=PM_GetIadm_amp, write=PM_SetIadm_amp };
      __property VTMatC*         MatC                         = {read=PM_GetMatC};
      __property VTMatZ*         MatZ                         = {read=PM_GetMatZ};
      __property VTEqpto*        Owner                        = {read=PM_GetOwner,    write=PM_SetOwner    };
      __property double          ResTerra                     = {read=PM_GetResTerra, write=PM_SetResTerra };
      __property double          Vmax_KV                      = {read=PM_GetVmax_KV,  write=PM_SetVmax_KV  };
      __property double          Vmin_KV                      = {read=PM_GetVmin_KV,  write=PM_SetVmin_KV  };
      __property int             TipoCfg                      = {read=PM_GetTipoCfg,  write=PM_SetTipoCfg  };
      __property double          Ycap_siemens_km[int i][int j] = {read=PM_GetYcap_siemens_km};
      __property complex<double> Z_ohm_km[int i][int j]        = {read=PM_GetZ_ohm_km};
      __property int             NumeroCabos                   = {read=PM_GetNumeroCabos                    };

   public:
                           __fastcall  VTArranjo(void) {};
        virtual            __fastcall ~VTArranjo(void) {};
        virtual VTArranjo* __fastcall  Clone(void) = 0;
        virtual void       __fastcall  DefCaboFase(int ind_fase, VTCabo *cabo, int multiplicidade=1) = 0;
        virtual void       __fastcall  DefCaboZ0Z1(VTCabo *cabo) = 0;
        virtual void       __fastcall  DefC0C1_nF_km(double c0, double c1) = 0;
        virtual void       __fastcall  DefC0C1_pu(double Sbase, double Vbase, double comp, double yc0_pu, double yc1_pu) = 0;
        virtual void       __fastcall  DefPtoFixacaoFase(int ind_fase, VTPtoFixacao *pto_fixacao) = 0;
        virtual void       __fastcall  DefSuporte(VTSuporte *suporte) = 0;
        virtual void       __fastcall  DefZ0Z1_ohm_km(strIMP z0, strIMP z1) = 0;
        virtual void       __fastcall  DefZ0Z1_pu(double Sbase, double Vbase, double comp, strIMP z0_pu, strIMP z1_pu) = 0;
        virtual double     __fastcall  MaiorCapacitancia(int seq) = 0;
        virtual bool       __fastcall  TemCapacitancia(void) = 0;
        virtual AnsiString __fastcall  TipoDeArranjo(void) = 0;
        virtual bool       __fastcall  Z0_ohm(double comp, strIMP &z0_ohm, double &yc0_mho) = 0;
        virtual bool       __fastcall  Z0_pu (double Sbase, double Vbase, double comp, strIMP &z0_pu, double &yc0_pu) = 0;
        virtual bool       __fastcall  Z1_ohm(double comp, strIMP &z1_ohm, double &yc1_mho) = 0;
        virtual bool       __fastcall  Z1_pu (double Sbase, double Vbase, double comp, strIMP &z1_pu, double &yc1_pu) = 0;

   protected: //métodos acessados via property
      virtual TColor          __fastcall PM_GetColor(void) = 0;
      virtual TDate           __fastcall PM_GetData(void) = 0;
      virtual int             __fastcall PM_GetFases(void) = 0;
      virtual double          __fastcall PM_GetIadm_amp(void) = 0;
      virtual double          __fastcall PM_GetIfase_amp(int nf) = 0;
      virtual VTMatC*         __fastcall PM_GetMatC(void) = 0;
      virtual VTMatZ*         __fastcall PM_GetMatZ(void) = 0;
      virtual int             __fastcall PM_GetNumeroCabos(void) = 0;
      virtual VTEqpto*        __fastcall PM_GetOwner(void) = 0;
      virtual double          __fastcall PM_GetResTerra(void) = 0;
      virtual int             __fastcall PM_GetTipoCfg(void) = 0;
      virtual double          __fastcall PM_GetVmax_KV(void) = 0;
      virtual double          __fastcall PM_GetVmin_KV(void) = 0;
      virtual void            __fastcall PM_SetFases(int fases) = 0;
      virtual void            __fastcall PM_SetColor(TColor color) = 0;
      virtual void            __fastcall PM_SetData(TDate data) = 0;
      virtual void            __fastcall PM_SetIadm_amp(double i_amp) = 0;
      virtual void            __fastcall PM_SetOwner(VTEqpto *owner) = 0;
      virtual void            __fastcall PM_SetTipoCfg(int tipo_cfg) = 0;
      virtual void            __fastcall PM_SetResTerra(double res_terra) = 0;
      virtual void            __fastcall PM_SetVmax_KV(double vmax) = 0;
      virtual void            __fastcall PM_SetVmin_KV(double vmin) = 0;
      virtual complex<double> __fastcall PM_GetZ_ohm_km(int i, int j) = 0;
      virtual double          __fastcall PM_GetYcap_siemens_km(int i, int j) = 0;


   public: //objetos externos
      VTSuporte *suporte;
      VTCabo    *caboZ0Z1; //caboZ0Z1 ou caboPRE_REUNIDO

    public: //dados locais
        bool      ramal;
      strIMP    z0;            //impedância de seq.0    (ohm/km)
      strIMP    z1;            //impedância de seq.1    (ohm/km)
      double    c0;            //capacitânca de seq.0   (nF/km)
      double    c1;            //capacitânca de seq.1   (nF/km)
      struct   {
               VTPtoFixacao *pto_fixacao;
               VTCabo       *cabo;
               int          multiplicidade;
               }fase[MAX_FASE];   //ordenado por A,B,C,N
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTArranjo
//---------------------------------------------------------------------------
VTArranjo* __fastcall NewObjArranjo(VTEqpto *owner);

//---------------------------------------------------------------------------
#endif
//eof


