//---------------------------------------------------------------------------
#ifndef VTFornecimentoH
#define VTFornecimentoH
using namespace std;

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <complex>
#include "..\Rede\Estrutura.h"
#include "..\Constante\Fases.h"

//---------------------------------------------------------------------------
class  VTApl;
class  VTEqpto;
class  VTListaSupFase;

//---------------------------------------------------------------------------
class VTFornecimento : public TObject
   {
   public:  //property
      __property bool            Ativo[VTEqpto  *eqpto][int np] = {read=PM_GetAtivo};
      __property double          P_mw[VTEqpto   *eqpto][int np] = {read=PM_GetP_mw,   write=PM_SetP_mw};
      __property double          Q_mvar[VTEqpto *eqpto][int np] = {read=PM_GetQ_mvar, write=PM_SetQ_mvar};
      __property complex<double> S_mva[VTEqpto  *eqpto][int np] = {read=PM_GetS_mva,  write=PM_SetS_mva};
      __property double          V_pu[VTEqpto   *eqpto][int np] = {read=PM_GetV_pu,   write=PM_SetV_pu};
      __property double          T_grau[VTEqpto *eqpto][int np] = {read=PM_GetT_grau, write=PM_SetT_grau};
      __property double          T_rad[VTEqpto *eqpto][int np]  = {read=PM_GetT_rad,  write=PM_SetT_rad};

   public:
                              __fastcall  VTFornecimento(void) {};
      virtual                 __fastcall ~VTFornecimento(void) {};
      virtual bool            __fastcall  Get_PQVT_porfase(VTEqpto *eqpto, int np, strSUP vet_sup[MAX_FASE]) = 0;
      virtual bool            __fastcall  Get_PQVT_trifasico(VTEqpto *eqpto, int np, strSUP *str_sup) = 0;
      virtual bool            __fastcall  Set_PQVT_porfase(VTEqpto *eqpto, int np, strSUP vet_sup[MAX_FASE]) = 0;
      virtual bool            __fastcall  Set_PQVT_trifasico(VTEqpto *eqpto, int np, strSUP *str_sup) = 0;
      virtual void            __fastcall  ReiniciaCurva(VTEqpto *eqpto, double defasagem_rad) = 0;

   protected: //métodos acessados via property
      virtual bool            __fastcall PM_GetAtivo(VTEqpto  *eqpto, int np) = 0;
      virtual double          __fastcall PM_GetP_mw(VTEqpto   *eqpto, int np) = 0;
      virtual double          __fastcall PM_GetQ_mvar(VTEqpto *eqpto, int np) = 0;
      virtual complex<double> __fastcall PM_GetS_mva(VTEqpto  *eqpto, int np) = 0;
      virtual double          __fastcall PM_GetV_pu(VTEqpto   *eqpto, int np) = 0;
      virtual double          __fastcall PM_GetT_grau(VTEqpto *eqpto, int np) = 0;
      virtual double          __fastcall PM_GetT_rad(VTEqpto  *eqpto, int np) = 0;
      virtual void            __fastcall PM_SetP_mw(VTEqpto   *eqpto, int np, double p) = 0;
      virtual void            __fastcall PM_SetQ_mvar(VTEqpto *eqpto, int np, double q) = 0;
      virtual void            __fastcall PM_SetS_mva(VTEqpto  *eqpto, int np, complex<double> s_mva) = 0;
      virtual void            __fastcall PM_SetV_pu(VTEqpto   *eqpto, int np, double v) = 0;
      virtual void            __fastcall PM_SetT_grau(VTEqpto *eqpto, int np, double t) = 0;
      virtual void            __fastcall PM_SetT_rad(VTEqpto  *eqpto, int np, double t) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe
VTFornecimento* __fastcall NewObjFornecimento(VTApl *apl);

#endif
//---------------------------------------------------------------------------
//eof
