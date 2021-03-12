//---------------------------------------------------------------------------
#ifndef VTCfgPerdaTecH
#define VTCfgPerdaTecH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTClasse;
class VTCabo;

//---------------------------------------------------------------------------
enum tpTRAFO {trfMONOFASICO=1, trfTRIFASICO=3};
//---------------------------------------------------------------------------
//opções para perda em medidor, ramal,
enum opPERDA_MEDIDOR {opMEDIDOR_NAO_USAR=0, opMEDIDOR_UNICO};
//---------------------------------------------------------------------------
//opções para perda em ramal
enum opPERDA_RAMAL {opRAMAL_NAO_USAR=0, opRAMAL_GIS, opRAMAL_UNICO, opRAMAL_SELECAO};
//---------------------------------------------------------------------------
//opções para perda em trafo
enum opPERDA_TRAFO {opTRAFO_GIS, opTRAFO_UNICO, opTRAFO_PRODIST, opTRAFO_EMPRESA};

//---------------------------------------------------------------------------
class PERDA_CABO : public TObject
   {
   public:
       int        id;
       AnsiString material;
       AnsiString codigo;
       double     r_ohm_km;
   };
//---------------------------------------------------------------------------
class PERDA_MEDIDOR : public TObject
   {
   public:
       AnsiString codigo;
       double     perda_w;
   };
//---------------------------------------------------------------------------
class PERDA_RAMAL : public TObject
   {
   public:
       int         num_fases;           // {1, 2, 3}}
//       int         num_fios;            // {1, 2, 3, 4}
       double      vmin_kv, vmax_kv;    //tensão mínima e máxima kV
       VTCabo      *cabo;
       double      comp_m;              //comprimento em m
       double      r_ohm_km;            //ohms/km
   };
//---------------------------------------------------------------------------
class  PERDA_TRAFO : public TObject
   {
   public:
       int         tipo;                //1: monofásico, 3: trifásico
       double      vmin_kv, vmax_kv;    //tensão mínima e máxima kV
       double      snom_kva;            //potência nominal em KVA
       double      perda_vazio_w;       //perdas no ferro em Watts
       double      perda_total_w;       //perdas total em Watts
   };

//---------------------------------------------------------------------------
class  PERDA_TRAFO_SED : public TObject
   {
   public:
       double      snom_mva;            //potência nominal em KVA
       double      perda_vazio_perc;    //perdas no ferro em percentual de Snom
       double      perda_total_perc;    //perdas total em percentual de Snom
   };

//---------------------------------------------------------------------------
class VTCfgPerdaTec : public TObject
   {
   public:  //property
      __property bool    Alterada              = {read=PD.alterada,  write=PD.alterada};
      __property TList*  LisPerdasCabo         = {read=PM_GetLisPerdasCabo};
      __property TList*  LisPerdasMedidor      = {read=PM_GetLisPerdasMedidor};
      __property TList*  LisPerdasRamal        = {read=PM_GetLisPerdasRamal};
      __property TList*  LisPerdasTrafoEmpresa = {read=PM_GetLisPerdasTrafoEmpresa};
      __property TList*  LisPerdasTrafoProdist = {read=PM_GetLisPerdasTrafoProdist};
      __property TList*  LisPerdasTrafoSED     = {read=PM_GetLisPerdasTrafoSED};
      __property int     TipoFluxo             = {read=fluxo.tipo,             write=fluxo.tipo};
      __property int     ModeloCarga           = {read=fluxo.modelo_carga,     write=fluxo.modelo_carga};
      __property bool    RedeComCapacitor      = {read=fluxo.com_capacitor,    write=fluxo.com_capacitor};
      __property bool    RedeComNeutro         = {read=fluxo.com_neutro,       write=fluxo.com_neutro};
      __property bool    Zat_mrt               = {read=fluxo.zat_mrt,          write=fluxo.zat_mrt};
      __property double  Zat_ohm               = {read=fluxo.zat_ohm,          write=fluxo.zat_ohm};
      __property bool    Progresso             = {read=fluxo.progresso,        write=fluxo.progresso};
      __property int     OpcaoPerdaMedidor     = {read=opcao.perda_medidor, write=opcao.perda_medidor};
      //opções de perdas
      __property int     OpcaoPerdaRamal       = {read=opcao.perda_ramal,     write=opcao.perda_ramal  };
      __property int     OpcaoPerdaTrafo       = {read=opcao.perda_trafo,     write=opcao.perda_trafo  };
      __property int     OpcaoPerdaTrafoSED    = {read=opcao.perda_trafo_sed, write=opcao.perda_trafo_sed };
      __property double  PerdaAdicional_perc   = {read=perda.adicional_perc,  write=perda.adicional_perc};
      //cálculo por fora
      __property bool   FerroPorFora   = {read=PorFora.ferro,   write=PorFora.ferro  };
      __property bool   MedidorPorFora = {read=PorFora.medidor, write=PorFora.medidor};
      __property bool   RamalPorFora   = {read=PorFora.ramal,   write=PorFora.ramal  };
      //únicos
      __property PERDA_MEDIDOR* PerdaMedidorUnico = {read=unico.perda_medidor, write=unico.perda_medidor};
      __property PERDA_RAMAL*   PerdaRamalUnico   = {read=unico.perda_ramal,   write=unico.perda_ramal  };
      __property PERDA_TRAFO*   PerdaTrafoUnico   = {read=unico.perda_trafo,    write=unico.perda_trafo };

   public:
                               __fastcall  VTCfgPerdaTec(void) {};
      virtual                  __fastcall ~VTCfgPerdaTec(void) {};
      virtual PERDA_MEDIDOR*   __fastcall  ExistePerdaMedidor(void) = 0;
      virtual PERDA_RAMAL*     __fastcall  ExistePerdaRamal(int num_fases, double v_kv) = 0;
      virtual PERDA_TRAFO*     __fastcall  ExistePerdaTrafo(int tipo, double snom_kva, double v_kv) = 0;
      virtual PERDA_TRAFO_SED* __fastcall  ExistePerdaTrafoSED(double snom_mva) = 0;
      virtual void             __fastcall  IniciaConfiguracaoGis(int opcao) = 0;
      virtual void             __fastcall  IniciaConfiguracaoProdist(void) = 0;
      virtual void             __fastcall  LeCfgPerdas(void) = 0;
      virtual void             __fastcall  SalvaCfgPerdas(void) = 0;

   protected:
      virtual TList* __fastcall PM_GetLisPerdasCabo(void)= 0;
      virtual TList* __fastcall PM_GetLisPerdasMedidor(void)= 0;
      virtual TList* __fastcall PM_GetLisPerdasRamal(void)= 0;
      virtual TList* __fastcall PM_GetLisPerdasTrafoEmpresa(void)= 0;
      virtual TList* __fastcall PM_GetLisPerdasTrafoProdist(void)= 0;
      virtual TList* __fastcall PM_GetLisPerdasTrafoSED(void)= 0;
      struct   {
               bool alterada;
               } PD;
      struct   {
               int perda_medidor;
               int perda_ramal;
               int perda_trafo;
               int perda_trafo_sed;
               } opcao;
      struct   {
               PERDA_MEDIDOR *perda_medidor;
               PERDA_RAMAL   *perda_ramal;
               PERDA_TRAFO   *perda_trafo;
               } unico;
      struct   {
               bool   ferro;
               bool   medidor;
               bool   ramal;
               } PorFora;
      struct   {
               double adicional_perc;  //%
               } perda;
      struct   {
               int    tipo;
               int    modelo_carga;
               bool   com_capacitor;
               bool   com_neutro;
               bool   zat_mrt;
               double zat_ohm;
               bool   progresso;
               } fluxo;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTCfgPerdaTec
//---------------------------------------------------------------------------
VTCfgPerdaTec* __fastcall NewObjCfgPerdaTec(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof


