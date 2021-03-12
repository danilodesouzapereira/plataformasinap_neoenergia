//---------------------------------------------------------------------------
#ifndef TMontaBarraH
#define TMontaBarraH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TMontaEqpto.h"

//---------------------------------------------------------------------------
class VTBarra;
class VTFases;

//---------------------------------------------------------------------------
class TMontaBarra : public TMontaEqpto
   {
   public:
                 __fastcall  TMontaBarra(VTApl *apl);
                 __fastcall ~TMontaBarra(void);
      VTGrupo*   __fastcall  IniciaGrupo(VTEqpto *eqpto);
      bool       __fastcall  LisAtributos(VTEqpto *eqpto, TList *lisATR);
      bool       __fastcall  LisAtributosEqpto(VTEqpto *eqpto, TList *lisATR);
      bool       __fastcall  LisAtributosFluxo(VTEqpto *eqpto, TList *lisATR);
      bool       __fastcall  LisValores(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat);
      bool       __fastcall  LisValoresEqpto(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat);
      bool       __fastcall  LisValoresFluxo(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat);
      bool       __fastcall  LisVetValFluxo(VTEqpto *eqpto, TList *lisEXT, int ind_atrib, int ano);
      AnsiString __fastcall  ValorAtributo(VTEqpto *eqpto, int ind_atrib);

   private:
      void __fastcall ValorCoordenada_utm(void);
      void __fastcall ValorDesequilibrio(void);
      void __fastcall ValorTensaoFaseNeutro_kv(void);
      void __fastcall ValorTensaoFaseNeutro_kv_max(void);
      void __fastcall ValorTensaoFaseNeutro_kv_min(void);
      void __fastcall ValorTensaoFaseNeutro_pu(void);
      void __fastcall ValorTensaoFaseNeutro_pu_max(void);
      void __fastcall ValorTensaoFaseNeutro_pu_min(void);
      void __fastcall ValorTensaoFaseTerra_kv(void);
      void __fastcall ValorTensaoFaseTerra_kv_max(void);
      void __fastcall ValorTensaoFaseTerra_kv_min(void);
      void __fastcall ValorTensaoFaseTerra_pu(void);
      void __fastcall ValorTensaoFaseTerra_pu_max(void);
      void __fastcall ValorTensaoFaseTerra_pu_min(void);
      void __fastcall ValorTensaoLinha_kv(void);
      void __fastcall ValorTensaoLinha_kv_max(void);
      void __fastcall ValorTensaoLinha_kv_min(void);
      void __fastcall ValorTensaoLinha_pu(void);
      void __fastcall ValorTensaoLinha_pu_max(void);
      void __fastcall ValorTensaoLinha_pu_min(void);
      void __fastcall ValorTensaoNominal(void);
      void __fastcall VetValorDesequilibrio(int ind_atrib);
      void __fastcall VetValorTensaoFaseNeutro_kv(int ind_atrib);
      void __fastcall VetValorTensaoFaseNeutro_kv_max(int ind_atrib);
      void __fastcall VetValorTensaoFaseNeutro_kv_min(int ind_atrib);
      void __fastcall VetValorTensaoFaseNeutro_pu(int ind_atrib);
      void __fastcall VetValorTensaoFaseNeutro_pu_min(int ind_atrib);
      void __fastcall VetValorTensaoFaseNeutro_pu_max(int ind_atrib);
      void __fastcall VetValorTensaoFaseTerra_kv(int ind_atrib);
      void __fastcall VetValorTensaoFaseTerra_kv_max(int ind_atrib);
      void __fastcall VetValorTensaoFaseTerra_kv_min(int ind_atrib);
      void __fastcall VetValorTensaoFaseTerra_pu(int ind_atrib);
      void __fastcall VetValorTensaoFaseTerra_pu_min(int ind_atrib);
      void __fastcall VetValorTensaoFaseTerra_pu_max(int ind_atrib);
      void __fastcall VetValorTensaoLinha_kv(int ind_atrib);
      void __fastcall VetValorTensaoLinha_kv_max(int ind_atrib);
      void __fastcall VetValorTensaoLinha_kv_min(int ind_atrib);
      void __fastcall VetValorTensaoLinha_pu(int ind_atrib);
      void __fastcall VetValorTensaoLinha_pu_max(int ind_atrib);
      void __fastcall VetValorTensaoLinha_pu_min(int ind_atrib);

   private: //objetos externos
      VTApl   *apl;
      VTBarra *pbarra;
      VTFases *fases;
   };

//---------------------------------------------------------------------------
#endif
//eof
