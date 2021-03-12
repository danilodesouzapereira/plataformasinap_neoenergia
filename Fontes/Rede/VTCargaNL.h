//---------------------------------------------------------------------------
#ifndef VTCargaNLH
#define VTCargaNLH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "Estrutura.h"
#include "TEqbar.h"
#include "..\Constante\Const.h"

//---------------------------------------------------------------------------
struct strFUN  {
               bool   indutiva;
               double fatpot_pu;
               double sa_mod_kva;
               double sa_ang_grau;
               double sb_mod_kva;
               double sb_ang_grau;
               double sc_mod_kva;
               double sc_ang_grau;
               };

//---------------------------------------------------------------------------
struct strHRM  {
               int    ordem;
               double sa_mod_pu;
               double sa_ang_grau;
               double sb_mod_pu;
               double sb_ang_grau;
               double sc_mod_pu;
               double sc_ang_grau;
               };

//---------------------------------------------------------------------------
class VTCargaNL : public TEqbar
   {
   /*
   public:  //property
      __property double Sa_mod_kva    = {read=PD_CNL.sa_mod_kva  ,     write=PD_CNL.sa_mod_kva  };
      __property double Sa_ang_grau   = {read=PD_CNL.sa_ang_grau ,     write=PD_CNL.sa_ang_grau };
      __property double Sb_mod_kva    = {read=PD_CNL.sb_mod_kva  ,     write=PD_CNL.sb_mod_kva  };
      __property double Sb_ang_grau   = {read=PD_CNL.sb_ang_grau ,     write=PD_CNL.sb_ang_grau };
      __property double Sc_mod_kva    = {read=PD_CNL.sc_mod_kva  ,     write=PD_CNL.sc_mod_kva  };
      __property double Sc_ang_grau   = {read=PD_CNL.sc_ang_grau ,     write=PD_CNL.sc_ang_grau };

      __property strHRM& Harmonica[int ind_freq] = {read=PM_GetHarmonica ,  write=PM_SetSf_mod_kva  };
      __property double Sf_ang_grau[int ind_fase]           = {read=PM_GetSf_ang_grau,  write=PM_GetSf_ang_grau };
      __property double Sh_mod_pu  [int ind_fase][ind_freq] = {read=PM_GetSh_mod_pu  ,  write=PM_SetSh_mod_pu   };
      __property double Sh_ang_grau[int ind_fase][ind_freq] = {read=PM_GetSh_ang_grau,  write=PM_SetSh_ang_grau };
   */
   public:  //property
      __property int    NumHarmonicas  = {read=PM_GetNumHarmonicas};
      __property bool   Indutiva       = {read=PM_GetIndutiva   ,  write=PM_SetIndutiva    };
      __property double FatPot_pu      = {read=PM_GetFatPot_pu  ,  write=PM_SetFatPot_pu   };

   public:
                         __fastcall  VTCargaNL(void) {};
      virtual            __fastcall ~VTCargaNL(void) {};
      virtual VTCargaNL* __fastcall  Clone(void) = 0;
      virtual bool       __fastcall  DefineFundamental(strFUN *str_fun) = 0;
      virtual bool       __fastcall  InsereHarmonica(strHRM *str_hrm) = 0;
      virtual strFUN*    __fastcall  LeFundamental(void) = 0;
      virtual strHRM*    __fastcall  LeHarmonica(int index) = 0;

   protected: //métodos acessados via property
      virtual double __fastcall PM_GetFatPot_pu(void) = 0;
      virtual bool   __fastcall PM_GetIndutiva(void) = 0;
      virtual int    __fastcall PM_GetNumHarmonicas(void) = 0;
      //
      virtual void __fastcall PM_SetFatPot_pu(double fatpot_pu) = 0;
      virtual void __fastcall PM_SetIndutiva(bool indutiva) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTCargaNL
//---------------------------------------------------------------------------
VTCargaNL* __fastcall NewObjCargaNL(void);

//---------------------------------------------------------------------------
#endif
//eof

 