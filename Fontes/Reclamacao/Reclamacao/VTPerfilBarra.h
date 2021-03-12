//---------------------------------------------------------------------------
#ifndef VTPerfilBarraH
#define VTPerfilBarraH

//---------------------------------------------------------------------------
using namespace std;
#include <Classes.hpp>
#include <complex>
#include <Fontes\Constante\Fases.h>

//---------------------------------------------------------------------------
class VTBarra;
class VTMedicao;
class VTRes505;

//---------------------------------------------------------------------------
class VTPerfilBarra : public TObject
   {
   public:  //property
      __property VTBarra* Barra                     = {read=PD.barra};
      __property bool     FaseEnabled[int ind_fase] = {read=PM_GetFaseEnabled        , write=PM_SetFaseEnabled};

   public:
                     __fastcall  VTPerfilBarra(void) {};
                     __fastcall ~VTPerfilBarra(void) {};
      virtual bool   __fastcall  CalculaDRA_DRP_DRC(VTRes505 *res505, int limite_amostra=-1) = 0;
      virtual bool   __fastcall  CalculaIndicesQualidadeUmDia(VTRes505 *res505, int ind_dia) = 0;
      virtual int    __fastcall  DRA(int fases=faseABC) = 0;
      virtual double __fastcall  DRA_perc(int fases=faseABC) = 0;
      virtual int    __fastcall  DRC(int fases=faseABC) = 0;
      virtual double __fastcall  DRC_perc(int fases=faseABC) = 0;
      virtual int    __fastcall  DRP(int fases=faseABC) = 0;
      virtual double __fastcall  DRP_perc(int fases=faseABC) = 0;
      virtual int    __fastcall  NumeroAmostragem(void) = 0;
      virtual bool   __fastcall  SalvaTensao(TDateTime data, VTMedicao *medicao, bool padrao=false) = 0;
      virtual bool   __fastcall  Vfn_kv(int ind_dia, int ind_pat, double vfn_kv[MAX_FASE]) = 0;
      virtual bool   __fastcall  Vfn_kv(int ind_dia, int ind_pat, complex<double> vfn_kv[MAX_FASE]) = 0;

   protected:  //métodos acessados vias property
      virtual bool __fastcall  PM_GetFaseEnabled(int ind_fase) = 0;
      virtual void __fastcall  PM_SetFaseEnabled(int ind_fase, bool enabled) = 0;

   protected:  //dados acessados vias property
      struct   {
               VTBarra *barra;
               int     num_amostra;
               bool    fase_enabled[MAX_FASE];
               int     dra[MAX_FASE];
               int     drc[MAX_FASE];
               int     drp[MAX_FASE];
               }PD;
   };

//---------------------------------------------------------------------------
VTPerfilBarra* __fastcall NewObjPerfilBarra(VTBarra *barra);

//---------------------------------------------------------------------------
#endif
//eof


