//---------------------------------------------------------------------------
#ifndef TZonaH
#define TZonaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTZona.h"

//---------------------------------------------------------------------------
class TZona : public VTZona
   {
   public:
             __fastcall  TZona(void);
             __fastcall ~TZona(void);
      bool   __fastcall  AnguloDefinido(void);
      void   __fastcall  CorrigeAnguloZonaDelta(void);
      bool   __fastcall  DefineAngulo_grau(double angulo_grau[MAX_FASE]);
      void   __fastcall  DefineNeutroAterrado(void);
      bool   __fastcall  DefineTipoEqvDeltaAberto(int fases, double vff_kv);
      bool   __fastcall  DefineTipoEqvDeltaFechado(int fases, double vff_kv);
      bool   __fastcall  DefineTipoEqvMonofasica(int fases, double vff_kv);
      bool   __fastcall  DefineTipoEqvTrifasica(int fases, double vff_kv);
      bool   __fastcall  DefineTipoMonofasica(int fases, double vff_kv, int fases_pri, int fases_sec);
      bool   __fastcall  DefineTipoTrifasica(int fases, double vff_kv);
      bool   __fastcall  ExisteBarra(VTBarra *barra);
      bool   __fastcall  ExisteFase(int fase);
      void   __fastcall  InsereBarra(VTBarra *pbarra);
      TList* __fastcall  LisBarra(void);
      void   __fastcall  RedefineFases(int fases);
      void   __fastcall  RedefineTensao(double vff_kv);
      void   __fastcall  RotacionaT_grau(double t_grau);
      void   __fastcall  UsadaEmFluxoEquilibrado(void);

   private: //métodos
      double __fastcall PM_GetDefasagem_grau(void);
      double __fastcall PM_GetDefasagem_rad(void);
      int    __fastcall PM_GetFases(void);
      int    __fastcall PM_GetTipo(void);
      bool   __fastcall PM_GetTrifasica(void);
      double __fastcall PM_GetT_grau(int ind_fase);
      double __fastcall PM_GetT_rad(int ind_fase);
      double __fastcall PM_GetVff_kv(void);
      double __fastcall PM_GetVfn_kv(int ind_fase);
      void   __fastcall PM_SetDefasagem_grau(double ang_grau);
      void   __fastcall PM_SetDefasagem_rad(double ang_rad);

   private: //métodos
      void   __fastcall SalvaFasesZonaDelta(int fases_pri, int fases_sec);

   private: //dados acessados via property
      struct   {
               int    tipo;   //ver enum TIPO_ZONA
               int    fases;
               float  defasagem_grau;
               float  Tan_grau;
               float  Tbn_grau;
               float  Tcn_grau;
               float  vff_kv;
               float  van_kv;
               float  vbn_kv;
               float  vcn_kv;
               } PD;

   private: //dados
      #define VALOR_INDEFINIDO -9999
      TList *lisBAR;
      struct   {
               struct{
                     int fases_pri, fases_sec;
                     }luz, forca1, forca2;
               }delta;
   };

#endif
//---------------------------------------------------------------------------
//eof

