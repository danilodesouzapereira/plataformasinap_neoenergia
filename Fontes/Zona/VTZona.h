//---------------------------------------------------------------------------
#ifndef VTZonaH
#define VTZonaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "..\Constante\Fases.h"

//---------------------------------------------------------------------------
enum TIPO_ZONA {zonaTRIFASICA=1, zonaMONOFASICA, zonaDELTA_ABERTO, zonaDELTA_FECHADO, zonaINDEFINIDA};

//---------------------------------------------------------------------------
class VTBarra;

//---------------------------------------------------------------------------
class VTZona : public TObject
   {
   public:  //property
      __property int    Fases                = {read=PM_GetFases};
      __property int    Tipo                 = {read=PM_GetTipo};
      __property bool   Trifasica            = {read=PM_GetTrifasica};
      __property double T_grau[int ind_fase] = {read=PM_GetT_grau};
      __property double T_rad[int ind_fase]  = {read=PM_GetT_rad};
      __property double Vff_kv               = {read=PM_GetVff_kv};
      __property double Vfn_kv[int ind_fase] = {read=PM_GetVfn_kv};
      __property double Defasagem_grau       = {read=PM_GetDefasagem_grau, write=PM_SetDefasagem_grau};
      __property double Defasagem_rad        = {read=PM_GetDefasagem_rad,  write=PM_SetDefasagem_rad};

   public:
                     __fastcall  VTZona(void) {};
      virtual        __fastcall ~VTZona(void) {};
      virtual bool   __fastcall  AnguloDefinido(void) = 0;
      virtual void   __fastcall  CorrigeAnguloZonaDelta(void) = 0;
      virtual bool   __fastcall  DefineAngulo_grau(double angulo_grau[MAX_FASE]) = 0;
      virtual void   __fastcall  DefineNeutroAterrado(void) = 0;
      virtual bool   __fastcall  DefineTipoEqvDeltaAberto(int fases, double vff_kv) = 0;
      virtual bool   __fastcall  DefineTipoEqvDeltaFechado(int fases, double vff_kv) = 0;
      virtual bool   __fastcall  DefineTipoEqvMonofasica(int fases, double vff_kv) = 0;
      virtual bool   __fastcall  DefineTipoEqvTrifasica(int fases, double vff_kv) = 0;
      virtual bool   __fastcall  DefineTipoMonofasica(int fases, double vff_kv, int fases_pri, int fases_sec) = 0;
      virtual bool   __fastcall  DefineTipoTrifasica(int fases, double vff_kv) = 0;
      virtual bool   __fastcall  ExisteBarra(VTBarra *barra) = 0;
      virtual bool   __fastcall  ExisteFase(int fase) = 0;
      virtual void   __fastcall  InsereBarra(VTBarra *pbarra) = 0;
      virtual TList* __fastcall  LisBarra(void) = 0;
      virtual void   __fastcall  RedefineFases(int fases) = 0;
      virtual void   __fastcall  RedefineTensao(double vff_kv) = 0;
      virtual void   __fastcall  RotacionaT_grau(double t_grau) = 0;
      virtual void   __fastcall  UsadaEmFluxoEquilibrado(void) = 0;

   protected: //métodos acessados via property
      virtual double __fastcall PM_GetDefasagem_grau(void) = 0;
      virtual double __fastcall PM_GetDefasagem_rad(void) = 0;
      virtual int    __fastcall PM_GetFases(void) = 0;
      virtual int    __fastcall PM_GetTipo(void) = 0;
      virtual bool   __fastcall PM_GetTrifasica(void) = 0;
      virtual double __fastcall PM_GetT_grau(int ind_fase) = 0;
      virtual double __fastcall PM_GetT_rad(int ind_fase) = 0;
      virtual double __fastcall PM_GetVff_kv(void) = 0;
      virtual double __fastcall PM_GetVfn_kv(int ind_fase) = 0;
      virtual void   __fastcall PM_SetDefasagem_grau(double ang_grau) = 0;
      virtual void   __fastcall PM_SetDefasagem_rad(double ang_rad) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe
VTZona* __fastcall NewObjZona(void);

#endif
//---------------------------------------------------------------------------//eof

