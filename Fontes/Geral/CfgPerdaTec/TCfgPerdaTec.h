//---------------------------------------------------------------------------
#ifndef TCfgPerdaTecH
#define TCfgPerdaTecH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTCfgPerdaTec.h"

//---------------------------------------------------------------------------
class VTInfoset;

//---------------------------------------------------------------------------
struct strCFG_CABO{
                  int id; char *material; char *codigo; double r_ohm_km;
                  };
//---------------------------------------------------------------------------
struct strCFG_RML {
                  double vmin_kv, vmax_kv, num_fases, comp_m, r_ohm_km;
                  };
//---------------------------------------------------------------------------
struct strCFG_TRF {
                  double vmin_kv, vmax_kv, snom_kva, perda_vazio_w, perda_total_w;
                  };
//---------------------------------------------------------------------------
struct strCFG_TRF_SED {
                     double snom_mva, perda_vazio_perc, perda_total_perc;
                     };

//---------------------------------------------------------------------------
class TCfgPerdaTec : public VTCfgPerdaTec
   {
   public:
                       __fastcall  TCfgPerdaTec(VTApl *apl);
                       __fastcall ~TCfgPerdaTec(void);
      PERDA_MEDIDOR*   __fastcall  ExistePerdaMedidor(void);
      PERDA_RAMAL*     __fastcall  ExistePerdaRamal(int num_fases, double v_kv);
      PERDA_TRAFO*     __fastcall  ExistePerdaTrafo(int tipo, double snom_kva, double v_kv);
      PERDA_TRAFO_SED* __fastcall  ExistePerdaTrafoSED(double snom_mva);
      void             __fastcall  IniciaConfiguracaoGis(int opcao=1);
      void             __fastcall  IniciaConfiguracaoProdist(void);
      void             __fastcall  LeCfgPerdas(void);
      void             __fastcall  SalvaCfgPerdas(void);

   protected: //métodos acessado via property
      TList* __fastcall PM_GetLisPerdasCabo(void);
      TList* __fastcall PM_GetLisPerdasMedidor(void);
      TList* __fastcall PM_GetLisPerdasRamal(void);
      TList* __fastcall PM_GetLisPerdasTrafoEmpresa(void);
      TList* __fastcall PM_GetLisPerdasTrafoProdist(void);
      TList* __fastcall PM_GetLisPerdasTrafoSED(void);

  private:
      PERDA_CABO* __fastcall ExistePerdaCabo(int id);
      void        __fastcall IniciaOpcaoValorUnico(void);
      void        __fastcall IniciaPerdasCabo(void);
      void        __fastcall IniciaPerdasMedidor(void);
      void        __fastcall IniciaPerdasRamal(void);
      void        __fastcall IniciaPerdasTrafo(void);
      void        __fastcall IniciaPerdasTrafo(int tipo, strCFG_TRF *cfg_trf, int count, TList *lisPerdaTrafo);
      void        __fastcall IniciaPerdasTrafoSED(void);
      void        __fastcall LeCfgPerdasCabo(void);
      void        __fastcall LeCfgPerdasMedidor(void);
      void        __fastcall LeCfgPerdasRamal(void);
      void        __fastcall LeCfgPerdasTrafo(AnsiString ArqCfgPerdaTrafo, TList *lisPerdaTrafo);
      void        __fastcall LeCfgPerdasTrafoSED(AnsiString ArqCfgPerdaTrafoSED, TList *lisPerdaTrafoSED);
      void        __fastcall SalvaCfgPerdasCabo(void);
      void        __fastcall SalvaCfgPerdasMedidor(void);
      void        __fastcall SalvaCfgPerdasRamal(void);
      void        __fastcall SalvaCfgPerdasTrafo(AnsiString ArqCfgPerdaTrafo, TList *lisPerdaTrafo);
      void        __fastcall SalvaCfgPerdasTrafoSED(AnsiString ArqCfgPerdaTrafoSED, TList *lisPerdaTrafoSED);

   private: //objetos externos
      VTApl *apl;

   private: //objetos locais
      #define ARQ_CFG_PERDA_CABO           "CfgPerdaCabo"
      #define ARQ_CFG_PERDA_MEDIDOR        "CfgPerdaMedidor"
      #define ARQ_CFG_PERDA_RAMAL          "CfgPerdaRamal"
      #define ARQ_CFG_PERDA_TRAFO_EMPRESA  "CfgPerdaTrafoEmpresa"
      #define ARQ_CFG_PERDA_TRAFO_PRODIST  "CfgPerdaTrafoProdist"
      #define ARQ_CFG_PERDA_TRAFO_SED      "CfgPerdaTrafoSED"
      TList     *lisPerdaCabo;
      TList     *lisPerdaRamal;
      TList     *lisPerdaMedidor;
      TList     *lisPerdaTrafoEmpresa;
      TList     *lisPerdaTrafoProdist;
      TList     *lisPerdaTrafoSED;
      TList     *lisTMP;
      VTInfoset *infoset;
   };

#endif
//---------------------------------------------------------------------------
//eof
