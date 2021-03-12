//---------------------------------------------------------------------------
#ifndef TChaveH
#define TChaveH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTChave.h"

//---------------------------------------------------------------------------
class TChave : public VTChave
   {
   public:
                 __fastcall  TChave(int num_pat);
                 __fastcall ~TChave(void);
      void       __fastcall  Abre(void);
      VTChave*   __fastcall  Clone(void);
      void       __fastcall  CopiaAtributosDe(VTEqpto &ref);
      void       __fastcall  Fecha(void);
      void       __fastcall  MudaEstado(void);
      int        __fastcall  Tipo(void);
      AnsiString __fastcall  TipoAsString(void);

   protected: //métodos virtuais redefinidos
      int        __fastcall Fases(VTBarra *barra);

   protected: //métodos acessados via property
      bool         __fastcall PM_GetAberta(void);
      bool         __fastcall PM_GetBloqueada(void);
      double       __fastcall PM_GetCapDisruptiva_amp(void);
      AnsiString   __fastcall PM_GetEloFusivel(void);
      unsigned     __fastcall PM_GetEstado(void);
      unsigned     __fastcall PM_GetEstadoNormal(void);
      bool         __fastcall PM_GetFechada(void);
      double       __fastcall PM_GetInom_amp(void);
      //AnsiString   __fastcall PM_GetPadrao(void);
      bool         __fastcall PM_GetProtecaoAtiva(void);
      int          __fastcall PM_GetTecnoComunicacao(void);
      bool         __fastcall PM_GetTelecomandada(void);
      bool         __fastcall PM_GetTipoBaseFusivel(void);
      VTTipoChave* __fastcall PM_GetTipoChave(void);
      bool         __fastcall PM_GetTipoDisjuntor(void);
      AnsiString   __fastcall PM_GetTipoGIS(void);
      bool         __fastcall PM_GetTipoReligadora(void);
      bool         __fastcall PM_GetTipoSeccionadora(void);
      void         __fastcall PM_SetBloqueada(bool bloqueada);
      void         __fastcall PM_SetCapDisruptiva_amp(double i_amp);
      void         __fastcall PM_SetEloFusivel(AnsiString elo);
      void         __fastcall PM_SetEstado(unsigned estado);
      void         __fastcall PM_SetEstadoNormal(unsigned estado);
      void         __fastcall PM_SetInom_amp(double i_amp);
      //void         __fastcall PM_SetPadrao(AnsiString padrao);
      void         __fastcall PM_SetProtecaoAtiva(bool protecao_ativa);
      void         __fastcall PM_SetTecnoComunicacao(int tecno_comunicacao);
      void         __fastcall PM_SetTelecomandada(bool telecomandada);
      void         __fastcall PM_SetTipoChave(VTTipoChave *tipochave);
      void         __fastcall PM_SetTipoGIS(AnsiString tipo_gis);

   protected: //dados acessados via property
      struct   {
               VTTipoChave *tipochave;
               AnsiString   elo_fusivel;
               AnsiString   tipo_gis;
               unsigned     estado;
               unsigned     estado_normal;
               float        corn_amp;
               float        cap_disruptiva_amp;
               bool         protecao_ativa;
               bool         telecomandada;
               int          tecno_comunicacao;
               }PD;
   };

//---------------------------------------------------------------------------
#endif
//eof

