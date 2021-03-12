//---------------------------------------------------------------------------
#ifndef VTResFlowRedeH
#define VTResFlowRedeH

//---------------------------------------------------------------------------
using namespace std;
#include <Classes.hpp>
#include <complex>

//---------------------------------------------------------------------------
class VTApl;
class VTBarra;
class VTRede;

//---------------------------------------------------------------------------
class VTResFlowRede : public TObject
   {
   public:  //property
      __property VTRede*         Rede              = {read=PM_GetRede, write=PM_SetRede};
      //fornecimento dos suprimentos
      __property double          Psup_mw[int np]   = {read=PM_GetPsup_mw};
      __property double          Qsup_mvar[int np] = {read=PM_GetQsup_mvar};
      __property complex<double> Ssup_mva[int np]  = {read=PM_GetSsup_mva};
      //fornecimento dos geradores
      __property double          Pger_mw[int np]   = {read=PM_GetPger_mw};
      __property double          Qger_mvar[int np] = {read=PM_GetQger_mvar};
      __property complex<double> Sger_mva[int np]  = {read=PM_GetSger_mva};
      //demanda das cargas
      __property double          Pcar_mw[int np]   = {read=PM_GetPcar_mw};
      __property double          Qcar_mvar[int np] = {read=PM_GetQcar_mvar};
      __property complex<double> Scar_mva[int np]  = {read=PM_GetScar_mva};
      //reativos
      __property double          Qcap_mvar[int np] = {read=PM_GetQcap_mvar};
      __property double          Qrea_mvar[int np] = {read=PM_GetQrea_mvar};
      //perda em demanda
      __property double          Perda_kw[int np]   = {read=PM_GetPerda_kw};
      __property double          Perda_kvar[int np] = {read=PM_GetPerda_kvar};

   public:  //funções públicas
                             __fastcall  VTResFlowRede(void) {};
      virtual                __fastcall ~VTResFlowRede(void) {};
      virtual double          __fastcall Pcarga_mw(VTBarra *barra, int np) = 0;
      virtual double          __fastcall Pgerador_mw(VTBarra *barra, int np) = 0;
      virtual double          __fastcall Psuprimento_mw(VTBarra *barra, int np) = 0;
      virtual double          __fastcall Qcapacitor_mvar(VTBarra *barra, int np) = 0;
      virtual double          __fastcall Qcarga_mvar(VTBarra *barra, int np) = 0;
      virtual double          __fastcall Qgerador_mvar(VTBarra *barra, int np) = 0;
      virtual double          __fastcall Qreator_mvar(VTBarra *barra, int np) = 0;
      virtual double          __fastcall Qsuprimento_mvar(VTBarra *barra, int np) = 0;

   protected: //funções acessadas via property
      virtual double          __fastcall PM_GetPcar_mw(int np) = 0;
      virtual double          __fastcall PM_GetPerda_kw(int np) = 0;
      virtual double          __fastcall PM_GetPerda_kvar(int np) = 0;
      virtual double          __fastcall PM_GetPger_mw(int np) = 0;
      virtual double          __fastcall PM_GetPsup_mw(int np) = 0;
      virtual double          __fastcall PM_GetQcap_mvar(int np) = 0;
      virtual double          __fastcall PM_GetQcar_mvar(int np) = 0;
      virtual double          __fastcall PM_GetQger_mvar(int np) = 0;
      virtual double          __fastcall PM_GetQrea_mvar(int np) = 0;
      virtual double          __fastcall PM_GetQsup_mvar(int np) = 0;
      virtual VTRede*         __fastcall PM_GetRede(void) = 0;
      virtual complex<double> __fastcall PM_GetScar_mva(int np) = 0;
      virtual complex<double> __fastcall PM_GetSger_mva(int np) = 0;
      virtual complex<double> __fastcall PM_GetSsup_mva(int np) = 0;
      //
      virtual void            __fastcall PM_SetRede(VTRede *rede) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTResFlowRede
//---------------------------------------------------------------------------
VTResFlowRede* __fastcall NewObjResFlowRede(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof
