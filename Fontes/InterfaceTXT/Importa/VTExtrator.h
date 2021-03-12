//-----------------------------------------------------------------------------
#ifndef VTExtratorH
#define VTExtratorH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "..\..\Curvas\VTCurva.h"
#include "..\..\Rede\Estrutura.h"

//-----------------------------------------------------------------------------
struct sdi_CURVA        {
                        int          id;
                        AnsiString   cod;
                        AnsiString   extern_id;
                        AnsiString   tipica;
                        int          dia;          //JCG 2014.01.21
                        int          tipo;
                        AnsiString   tipo_str;
                        int          unidade;       //DVK 2013.05.28
                        AnsiString   classe_cons;
                        double       consumo_min_kwhmes;
                        double       consumo_max_kwhmes;
                        int          num_ponto;
                        int          dim_valor;
                        struct{
                              strHM hm_ini;
                              strHM hm_fim;
                              double valor[32];
                              }ponto[144];
                        };
struct sdi_ARRANJO      {
                        int          id;
                        AnsiString   cod;
                        AnsiString   extern_id;
                        int          tipo;
                        double       iadm_amp;
                        AnsiString   fases;
                        //arranjo tipo arranjoZ0Z1
                        strIMP       z0_ohm_km;
                        strIMP       z1_ohm_km;
                        double       c0;
                        double       c1;
                        //arranjo tipo arranjoCABO_Z0Z1
                        int          id_cabo_Z0Z1;
                        //arranjo tipo arranjoCABOxFASE
                        int          id_suporte;
                        int          id_pto_caboA;
                        int          id_caboA;
                        int          id_pto_caboB;
                        int          id_caboB;
                        int          id_pto_caboC;
                        int          id_caboC;
                        int          id_pto_caboN;
                        int          id_caboN;
                        };
struct sdi_BARRA        {
                        int          id;
                        AnsiString   cod;
                        AnsiString   extern_id;
                        double       vnom_kv;
                        struct       {
                                     double x;
									 double y;
									 }utm_m;
						strESQ_BARRA esq;
						};
struct sdi_BATERIA      {
						int 		     id;
						AnsiString 	     cod;
						AnsiString	     extern_id;
						int 		     id_barra;
						AnsiString 		 fases;
						int			     tipo_operacao;
						double		     i_cte_pu;
						double		     s_cte_pu;
						double		     z_cte_pu;
						int			     modelo_carga;
						int			     modelo_desc;
						double		     p_nom_kw;
						double		     e_nom_kwh;
						double			 cap_inv_kva;
						double		     p_ociosa_pu;
						double		     q_ociosa_pu;
						double		     rend_carga_pu;
						double		     rend_desc_pu;
						double		     gatilho_carga_pu;
						double		     gatilho_desc_pu;
						sdi_CURVA    	 curva;
						strESQ_EQBAR   	 esq;
						};
struct sdi_CABO         {
						int          id;
						AnsiString   cod;
						AnsiString   extern_id;
                        int          tipo;
                        double       iadm_amp;
                        //cabo tipo caboR_GMR
                        double       raio_mm;
                        double       rmg_mm;
                        double       r_ohm_km;
                        //cabo caboZ0Z1
                        strIMP       z0_ohm_km;
                        strIMP       z1_ohm_km;
                        double       c0;
                        double       c1;
                        };
struct sdi_CANAL        {
                        int           id;
                        int           id_medidor;
                        int           eqpto_tipo;
                        int           eqpto_id;
                        AnsiString    cod;
                        int           numero;
                        int           tipocanal;
                        int           fases;
                        double        toler_perc;
                        sdi_CURVA     curva;
                        };
struct sdi_CAPACITOR    {
                        int          id;
                        int          id_barra;
                        AnsiString   cod;
                        AnsiString   extern_id;
                        double       vnom_kv;
                        double       snom_kva;
                        AnsiString   ligacao;
                        sdi_CURVA    curva;
                        strESQ_EQBAR esq;
                        };
struct sdi_CAPSERIE     {
                        int           id;
                        int           id_bar1;
                        int           id_bar2;
                        AnsiString    cod;
                        AnsiString    extern_id;
                        double        vnom_kv;
                        double        snom_kva;
                        strIMP        z_ohm;
                        strESQ_LIGA   esq;
                        };
struct sdi_CARGA        {
						int          id;
						int          id_barra;
						AnsiString   cod;
						AnsiString   extern_id;
						AnsiString   grupo_tarifa;
						AnsiString   classe;
						AnsiString   fases;
						double       i_cte_pu;
						double       s_cte_pu;
						double       z_cte_pu;
						double       consumo_kwhm;
						sdi_CURVA    curva;
						strESQ_EQBAR   esq;
                        struct
							{
								int residencial;
								int comercial;
								int industrial;
								int rural;
								int outros;
								int a4;
							}num_cons;
                        };
struct sdi_CARGA_IP     {
                        int          id;
                        int          id_barra;
                        AnsiString   cod;
                        AnsiString   extern_id;
                        AnsiString   fases;
                        int          num_lampada;
                        double       p_w;
                        double       fp_pu;
                        strESQ_EQBAR   esq;
                        };
struct sdi_CHAVE        {
                        int          id;
                        int          id_bar1;
                        int          id_bar2;
                        AnsiString   cod;
						AnsiString   extern_id;
						AnsiString   tipo_gis;
                        AnsiString   elo_fusivel;
                        double       iadm_amp;
                        int          estado;
                        struct{
                              int        id;
                              int        tipo;
                              AnsiString cod;
                              }tipochave;
                        bool         protecao_ativa;
                        bool         telecomandada;
                        int          tecno_comunicacao;
                        strESQ_LIGA   esq;
                        };
struct sdi_FILTRO       {
                        int           id;
                        int           id_bar;
                        AnsiString    cod;
                        AnsiString    extern_id;
                        double        vnom_kv;
                        double        snom_kva;
                        strIMP        z0_pu; 
                        strESQ_EQBAR  esq;
                        };
struct sdi_GERADOR      {
                        int           id;
                        int           id_bar;
                        AnsiString    cod;
                        AnsiString    fonte_energia;
						AnsiString    extern_id;
                        int           fases;
                        int           tipo_sup;
                        int           tipo_geracao;
                        int           ligacao;
                        double        vnom_kv;
                        double        snom_kva;;
                        double        fplim_pu;
                        strIMP        z0_pu;
                        strIMP        z1_pu;
                        sdi_CURVA    curva;
                        strESQ_EQBAR  esq;
                        };
struct sdi_MEDIDOR      {
                        int           id;
                        int           id_bar;
                        AnsiString    cod;
                        AnsiString    extern_id;
                        strESQ_EQBAR  esq;
                        };
struct sdi_MUTUA        {
                        int           id;
                        int           id_tre1;
                        int           id_bar_tre1;
                        int           id_tre2;
                        int           id_bar_tre2;
                        AnsiString    cod;
                        AnsiString    extern_id;
                        strIMP        z0;            //ohm/km
                        strIMP        z1;            //ohm/km
                        double        postre1;
                        double        postre2;
                        };
struct sdi_REATOR       {
                        int           id;
                        int           id_bar;
                        AnsiString    cod;
                        AnsiString    extern_id;
                        int           ligacao;
                        double        vnom_kv;
                        double        q_kvar;
                        sdi_CURVA     curva;
                        strESQ_EQBAR  esq;
                        };
struct sdi_REDE         {
                        int          id;
                        struct{
                              int          id;
                              int          seg;
                              AnsiString   cod;
                              }tiporede;
                        AnsiString   cod;
                        AnsiString   extern_id;
                        AnsiString   area;
                        AnsiString   cluster;
                        unsigned     color_r;
                        unsigned     color_g;
                        unsigned     color_b;
                        //atributos acrescentados em 2017.04.02
                        int          barini_id; //0=indefinido
                        int          radial;    //{0=indefinido, 1=radial,   2=não radial}
                        int          estatica;  //{0=indefinido, 1=estática, 2=não estática}
                        };
struct sdi_REGULADOR    {
                        int          id;
                        int          id_bar1;
                        int          id_bar2;
                        int          id_bar_ent;
						int          modelo;
						int          rtp;
                        AnsiString   cod;
                        AnsiString   extern_id;
                        double       snom_kva;
                        strIMP       z0_pu;
                        strIMP       z1_pu;
                        int          num_eqpto;
                        AnsiString   ligacao;
                        AnsiString   fases;
                        int          num_passo;
                        double       dv_perc;
                        struct{
                              int        id_bar_ref;
                              double     v_pu; //tensão em pu
                              int        id_bar_ref_inv;
							  double     vinv_pu; //tensão em pu no sentido inverso
							  double	 larg_bandamorta_pu;
							  double	 larg_bandamorta_inv_pu;
                              }ajuste_auto;
                        struct{
                              int        passo;
                              }ajuste_fixo;
                        strESQ_LIGA   esq;
                        };
struct sdi_SUPORTE      {
                        int          id;
                        AnsiString   cod;
                        AnsiString   extern_id;
                        AnsiString   tipo;
                        int          num_pto_fixacao;
                        struct{
                              int          id;
                              AnsiString   cod;
                              double       x_m;
                              double       y_m;
                              }pto_fixacao[24];
                        };
struct sdi_SUPRIMENTO   {
                        int          id;
                        int          id_bar;
                        AnsiString   cod;
                        AnsiString   extern_id;
                        AnsiString   fases;
                        double       vnom_kv;
                        double       smax_kva;
                        strIMP       z0_pu;
                        strIMP       z1_pu;
                        int          tipo_sup;
                        sdi_CURVA    curva;
                        strESQ_EQBAR esq;
                        };
struct sdi_TIPO_CHAVE   {
                        int          id;
                        int          tipo;
                        AnsiString   codigo;
                        AnsiString   extern_id;
                        AnsiString   operacao;
                        };
struct sdi_TIPO_CONS    {
                        int          id;
                        int          tag;
                        AnsiString   codigo;
                        unsigned     color_r;
                        unsigned     color_g;
                        unsigned     color_b;
                        };
struct sdi_TIPO_REDE    {
                        int          id;
                        int          segmento;
                        AnsiString   codigo;
                        AnsiString   extern_id;
                        AnsiString   descricao;
                        double       vmin_kv;
                        double       vmax_kv;
                        unsigned     color_r;
                        unsigned     color_g;
                        unsigned     color_b;
                        };
struct sdi_TRAFO2E      {
                        int          id;
                        AnsiString   cod;
                        AnsiString   extern_id;
                        double       snom_kva;
                        double       sbase_kva;
                        double       perda_fe_w;
                        strIMP       z0_pu;
                        strIMP       z1_pu;
                        struct{
                              int        id_bar;
                              AnsiString ligacao;
                              double     vnom_kv;
                              double     vtap_kv;
                              strIMP     zat_ohm;
                              } pri, sec;
                        struct{
                              AnsiString tipo;  //{0:não usado; 1:primário; 2:secundário}
                              double     v_pu; //tensão em pu
                              }ajuste;
                        strESQ_LIGA   esq;
                        };
struct sdi_TRAFO3E      {
                        int          id;
                        AnsiString   cod;
                        AnsiString   extern_id;
                        double       sbase_kva;
                        double       perda_fe_w;
                        strIMP       zps0_pu;
                        strIMP       zps1_pu;
                        strIMP       zpt0_pu;
                        strIMP       zpt1_pu;
                        strIMP       zst0_pu;
                        strIMP       zst1_pu;
                        struct{
                              int        id_bar;
                              AnsiString ligacao;
                              double     snom_kva;
                              double     vnom_kv;
                              double     vtap_kv;
                              strIMP     zat_ohm;
                              } pri, sec, ter;
                         strESQ_LIGA   esq;
                        };
struct sdi_TRAFO_MONO   {
                        int          id;
                        AnsiString   cod;
                        AnsiString   extern_id;
                        double       snom_kva;
                        double       sbase_kva;
                        double       perda_fe_w;
                        strIMP       z_pu;
                        struct{
                              int        id_bar;
                              AnsiString fases;
                              double     vnom_kv;
                              double     vtap_kv;
                              strIMP     zat_ohm;
                              } pri, sec;
                        struct{
                              AnsiString tipo;  //{0:não usado; 1:primário; 2:secundário}
                              double     v_pu; //tensão em pu
                              }ajuste;
                        strESQ_LIGA   esq;
                        };
struct sdi_TRAFOZZ    {
                     int           id;
                     int           id_bar;
                     AnsiString    cod;
                     AnsiString    extern_id;
                     double        vnom_kv;
                     double        snom_kva;
                     strIMP        z0_pu; 
                     strESQ_EQBAR  esq;
                     };
struct sdi_TRECHO       {
                        int          id;
                        int          id_bar1;
                        int          id_bar2;
                        AnsiString   cod;
                        AnsiString   extern_id;
                        double       comp_m;
                        struct{//arranjo
                              int id_arranjo;
                              }arranjo_tipico;
                        struct{//impedância própria
                              AnsiString   fases;
                              double       iadm_amp;
                              strIMP       z0;
                              strIMP       z1;
                              double       c0;
                              double       c1;
                              }arranjo_proprio;
                        strESQ_LIGA   esq;
                        };

//-----------------------------------------------------------------------------
class VTApl;

//-----------------------------------------------------------------------------
class VTExtrator : public TObject
   {                    
   public:  //property
      __property bool ArranjoEnabled       = {read=opcao.arranjo,     write=opcao.arranjo    };
      __property bool CurvaTipicaEnabled   = {read=opcao.curvatipica, write=opcao.curvatipica};
      __property bool TipoChaveEnabled     = {read=opcao.tipochave,   write=opcao.tipochave  };
      __property bool TipoConsumidorEnabled= {read=opcao.tipocons,    write=opcao.tipocons   };
      __property bool TipoRedeEnabled      = {read=opcao.tiporede,    write=opcao.tiporede   };
      __property bool RedeEnabled          = {read=opcao.rede,        write=opcao.rede       };

   public:
                   __fastcall  VTExtrator(void) {};
      virtual      __fastcall ~VTExtrator(void) {};
      //versao
	  virtual bool __fastcall  InsereVersao(TStrings *campos) = 0;
	  //diagnostico
	  virtual bool __fastcall  InsereDiagnostico(TStrings *campos) = 0;
      //tipos
      virtual bool __fastcall  InsereTipoChave(TStrings *campos)=0;
      virtual bool __fastcall  InsereTipoConsumidor(TStrings *campos)=0;
      virtual bool __fastcall  InsereTipoRede(TStrings *campos)=0;
      //suportes, cabos e arranjos
      virtual bool __fastcall  InsereSuporte(TStrings *campos)=0;
      virtual bool __fastcall  InsereCaboUnipolar(TStrings *campos)=0;
      virtual bool __fastcall  InsereCaboZ0Z1(TStrings *campos)=0;
      virtual bool __fastcall  InsereArranjoCaboPorfase(TStrings *campos)=0;
      virtual bool __fastcall  InsereArranjoCaboZ0Z1(TStrings *campos)=0;
      virtual bool __fastcall  InsereArranjoPUSB100(TStrings *campos)=0;
      virtual bool __fastcall  InsereArranjoZ0Z1(TStrings *campos)=0;
      //curvas típicas
      virtual bool __fastcall  InsereCurvaCapacitor(TStrings *campos)=0;
      virtual bool __fastcall  InsereCurvaCarga(TStrings *campos)=0;
      virtual bool __fastcall  InsereCurvaCargaIP(TStrings *campos)=0;
      virtual bool __fastcall  InsereCurvaGerador(TStrings *campos)=0;
      virtual bool __fastcall  InsereCurvaReator(TStrings *campos)=0;
      //redes
      virtual bool __fastcall  InsereBarra(TStrings *campos)=0;
      virtual void __fastcall  InsereBarraStart(void)=0;
	  virtual void __fastcall  InsereBarraStop(void)=0;
	  virtual bool __fastcall  InsereBateria(TStrings *campos)=0;
      virtual bool __fastcall  InsereCapacitor(TStrings *campos)=0;
      virtual bool __fastcall  InsereCapacitorSerie(TStrings *campos)=0;
      virtual bool __fastcall  InsereCargaComMedicao(TStrings *campos)=0;
      virtual bool __fastcall  InsereCargaIP(TStrings *campos)=0;
      virtual bool __fastcall  InsereCargaTipica(TStrings *campos)=0;
      virtual bool __fastcall  InsereChave(TStrings *campos)=0;
      virtual bool __fastcall  InsereFiltro(TStrings *campos)=0;
      virtual bool __fastcall  InsereGerador(TStrings *campos)=0;
      virtual bool __fastcall  InsereMedidor(TStrings *campos) = 0;
      virtual bool __fastcall  InsereMedidorCanal(TStrings *campos) = 0;
      virtual bool __fastcall  InsereMutua(TStrings *campos)=0;
      virtual bool __fastcall  InsereReator(TStrings *campos)=0;
      virtual bool __fastcall  InsereRede(TStrings *campos)=0;
      virtual bool __fastcall  InsereReguladorAutomatico(TStrings *campos)=0;
      virtual bool __fastcall  InsereReguladorTapFixo(TStrings *campos)=0;
      virtual bool __fastcall  InsereSuprimento(TStrings *campos)=0;
      virtual bool __fastcall  InsereTrafo2E(TStrings *campos)=0;
      virtual bool __fastcall  InsereTrafo3E(TStrings *campos)=0;
      virtual bool __fastcall  InsereTrafoMono(TStrings *campos)=0;
      virtual bool __fastcall  InsereTrafoZZ(TStrings *campos)=0;
      virtual bool __fastcall  InsereTrechoTipico(TStrings *campos)=0;
      virtual bool __fastcall  InsereTrechoZ0Z1(TStrings *campos)=0;
      //
      virtual bool __fastcall  Sucesso(void) = 0;
      virtual bool __fastcall  TrataFimDados(void) = 0;
      virtual bool __fastcall  TrataIniciaDados(AnsiString cod_dominio) = 0;

   protected:
      struct{
            bool arranjo;
            bool curvatipica;
            bool tipochave;
            bool tipocons;
            bool tiporede;
            bool rede;
            }opcao;

   };

//-----------------------------------------------------------------------------
// função global para criar objeto VTMonta
//-----------------------------------------------------------------------------
VTExtrator* NewObjExtrator(VTApl *apl);
VTExtrator* NewObjExtratorCaboArranjo(VTApl *apl);

#endif
//-----------------------------------------------------------------------------
// eof
