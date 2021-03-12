//---------------------------------------------------------------------------
#ifndef VTTrafoH
#define VTTrafoH
using namespace std;

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include <complex>
#include "Estrutura.h"
#include "TLigacao.h"
#include "..\Constante\Fases.h"

//---------------------------------------------------------------------------
class TFase;
class VTMatY;
class VTMatZ;

//---------------------------------------------------------------------------
class VTTrafo : public TLigacao
   {
   public:  //property
      __property int      Ligacao     = {read=PM_GetLigacao};
      __property bool     Monofasico  = {read=PM_GetMonofasico, write=PM_SetMonofasico };
      __property bool     Reducao     = {read=PM_GetReducao,    write=PM_SetReducao    };
      __property int      ModeloEqv   = {read=PM_GetModeloEqv,  write=PM_SetModeloEqv  };
      __property int      Tap         = {read=PM_GetTap,        write=PM_SetTap        };
      __property VTMatY*  MatYij      = {read=PM_GetMatYij                             };
      __property VTMatY*  MatYji      = {read=PM_GetMatYji                             };
      __property VTMatZ*  MatZ        = {read=PM_GetMatZ                               };

   public:
                       __fastcall  VTTrafo(void) {};
      virtual          __fastcall ~VTTrafo(void) {};
      virtual bool     __fastcall  AngulosNoPrimario(double vet_ang_grau[MAX_FASE])=0;
      virtual bool     __fastcall  AngulosNoSecundario(double vet_ang_grau[MAX_FASE])=0;
      virtual VTBarra* __fastcall  BarraPrimario(void) = 0;
      virtual VTBarra* __fastcall  BarraSecundario(void) = 0;
      virtual VTTrafo* __fastcall  Clone(void) = 0;
      virtual double   __fastcall  DefasagemPriSec(void)=0;
      virtual double   __fastcall  DefasagemSecPri(void)=0;
      virtual double   __fastcall  PerdaNoFerroMW(void) = 0;
      virtual void     __fastcall  OrdenaBar12PriSec(void)=0;
      virtual void     __fastcall  RedefineBarraDoPrimario(VTBarra *pbar)=0;
      virtual void     __fastcall  RedefineVnom(VTBarra *pbar)=0;
      virtual bool     __fastcall  Z0_Z1_pu(double Sbase, strIMP &z0_pu, strIMP &z1_pu)=0;

   protected: //métodos acessados via property
      virtual int      __fastcall PM_GetLigacao(void) = 0;
      virtual VTMatY*  __fastcall PM_GetMatYij(void) = 0;
      virtual VTMatY*  __fastcall PM_GetMatYji(void) = 0;
      virtual VTMatZ*  __fastcall PM_GetMatZ(void) = 0;
      virtual int      __fastcall PM_GetModeloEqv(void) = 0;
      virtual bool     __fastcall PM_GetMonofasico(void) = 0;
      virtual bool     __fastcall PM_GetReducao(void) = 0;
      virtual int      __fastcall PM_GetTap(void) = 0;
      virtual void     __fastcall PM_SetModeloEqv(int modelo_eqv) = 0;
      virtual void     __fastcall PM_SetMonofasico(bool monofasico) = 0;
      virtual void     __fastcall PM_SetReducao(bool reducao) = 0;
      virtual void     __fastcall PM_SetTap(int tap) = 0;

   public: //dados
      bool   reducao;            //trafo de redução
      bool   auto_trafo;
      float  snom;               //MVA
      float  perda_ferro;        //% da Snom
      strIMP z0, z1;             //pu
      VTMatZ *matZ;              //matriz de impedâncias em pu
      struct {
             int    ligacao;
             int    fase;
             float  vnom, tap;    //kV
             float  defasagem;    //graus
             strIMP zterra;       //ohm
             } pri, sec;
      struct {
             int    tipo;          //{0:tap fixo; 1:auto primário; 2:auto secundário}
             float  dv_perc;       //variação % da tensão para baixo ou para cima
             int    num_passo;     //passos para baixo ou para cima
             struct{
                   int  passo;          //passo fixo selecionado
                   }ajuste_fixo;
             struct{
                   VTBarra *bar_ref;    //barra de referência
                   float    v_pu;       //tensão alvo (pu) da Barra de referência
                   float    dv_pu;      //Tolerância de tensão. Nenhum ajuste é feito no tap
                                        //quando a tensão está na faixa (v_pu-dv_pu) e (v_pu+dv_pu)
                   }ajuste_auto;
             } ltc;
      struct{
            int     modelo;
            strADM  y0_ij_pu ;
            strADM  y0_ji_pu ;
            strADM  y1_ij_pu ;
            strADM  y1_ji_pu ;
            VTMatY *matYij_s;       //matriz de admitâncias em siemens
            VTMatY *matYji_s;       //matriz de admitâncias em siemens
            } eqv;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTTrafo
//---------------------------------------------------------------------------
VTTrafo* __fastcall NewObjTrafo(int num_pat);
VTTrafo* __fastcall NewObjTrafoMono(int num_pat);

//---------------------------------------------------------------------------
#endif
//eof

