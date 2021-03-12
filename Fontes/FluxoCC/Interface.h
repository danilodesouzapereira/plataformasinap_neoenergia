//---------------------------------------------------------------------------
#ifndef InterfaceH
#define InterfaceH

//---------------------------------------------------------------------------
using namespace std;

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <complex>

//---------------------------------------------------------------------------
enum enumMC_LIGACAO   {//tipos de ligação dos eqptos
                       ligacaoTRIANGULO=0,
                       ligacaoESTRELA,
                       ligacaoESTRELA_ATERRADO};
/*
//---------------------------------------------------------------------------
enum enumMC_FASE  {//identificação de fases
                   faseA=0,  faseB,     faseC,  faseN,
                   faseAN,   faseBN,    faseCN,
                   faseAT,   faseBT,    faseCT, faseNT,
                   faseAB,   faseBC,    faseCA,
                   faseABN,  faseBCN,   faseCAN,
                   faseABT,  faseBCT,   faseCAT,
                   vfaseABC, faseABCN,  faseABCNT, faseINV=-1};
*/
//---------------------------------------------------------------------------
enum enumMC_SUPRIMENTO  {//tipos de Suprimento
                         tiposupPV=1, //OBS: se mudar p/ 0 precisa acertar o ultraPlan
                         tiposupPQ,
                         tiposupVF};

//---------------------------------------------------------------------------
//definição das estruturas de interface
//---------------------------------------------------------------------------
struct smcBARRA      {
                     void                   *eqpto;
                     AnsiString             codigo;
                     int                    id;
                     struct {
                            double          vnom_kv;
                            double          fase_zona_rad;
                            }set;
                     struct {
                            complex<double> Van_pu;
                            complex<double> Vbn_pu;
                            complex<double> Vcn_pu;
                            complex<double> Vnt_pu;
                            }get;
                     };
//---------------------------------------------------------------------------
struct smcBASE       {
                     struct {
                            double          sbase_mva;            //potência de base
                            }set;
                     };
//---------------------------------------------------------------------------
struct smcCAPACITOR  {
                     void                   *eqpto;
                     int                    id;
                     int                    barra_id;
                     struct {
                            int             ligacao;              //ver enumMC_LIGACAO
                            //double        vnom_kv;
                            //double        snom_mvar;
                             double         x_ohm;
                            }set;
                     };
//---------------------------------------------------------------------------
struct smcCAPSERIE   {
                     void                  *eqpto;
                     int                    id;
                     int                    barra1_id;
                     int                    barra2_id;
                     struct {
                             double         vnom_kv;
                             //double       snom_mvar;
                             double         x_ohm;
                            }set;
                     struct {
                            complex<double> Sa_mva;
                            complex<double> Sb_mva;
                            complex<double> Sc_mva;
                            complex<double> Sn_mva;
                            }get;
                     };
//---------------------------------------------------------------------------
struct smcCARGA      {
                     void                   *eqpto;
                     int                    id;
                     int                    barra_id;
                     struct {
                            int             fase;                 //ver enumMC_FASE
                            complex<double> s_mva;
                            struct	{
                            	      double  icte_pu;              //pu
                            	      double  scte_pu;
                            	      double  zcte_pu;
                                     }modelo;
                            }set;
                     };
//---------------------------------------------------------------------------
struct smcFILTRO     {
                     void                   *eqpto;
                     int                    id;
                     int                    barra_id;
                     struct {
                            //double        snom_mva;
                            //double        vnom_kv;
                            complex<double> z0_pu;
                            }set;
                     };
//---------------------------------------------------------------------------
struct smcGERADOR    {
                     void                   *eqpto;
                     int                    id;
                     int                    barra_id;
                     struct {
                            int             ligacao;              //ver enumMC_LIGACAO
                            double          vnom_kv;
                            double          snom_mva;
                            double          qmax_mvar;
                            struct  {//fornecimento
                                    int     tipo;                 //ver enumMC_SUPRIMENTO
                                    double  p_mw;
                                    double  q_mvar;
                                    double  v_pu;
                                    double  t_rad;
                                    }sup;
                            struct  {//impedâncias de seq
                                    complex<double> z0_pu;       //Sbase = Snom
                                    complex<double> z1_pu;       //Sbase = Snom
                                    }scc;
                            }set;
                     struct {
                            complex<double> Sa_pu;
                            complex<double> Sb_pu;
                            complex<double> Sc_pu;
                            complex<double> Sn_pu;
                            }get;
                     };
//---------------------------------------------------------------------------
struct smcMUTUA      {
                     void                   *eqpto;
                     int                    id;
                     int                    trecho1_id;
                     int                    trecho2_id;
                     struct {
                            struct{
                                  int       trecho1_bar_id;
                                  int       trecho2_bar_id;
                                  }sentido;
                            complex<double> z0_ohm;
                            complex<double> z1_ohm;
                            }set;
                     };
//---------------------------------------------------------------------------
struct smcREATOR     {
                     void                   *eqpto;
                     int                    id;
                     int                    barra_id;
                     struct {
                            int             ligacao;              //ver enumMC_LIGACAO
                            //double          snom_mva;
                            //double          vnom_kv;
                            double          x_ohm;
                            }set;
                     };
//---------------------------------------------------------------------------
struct smcREGULADOR  {
                     void                   *eqpto;
                     int                    id;
                     int                    barra1_id;
                     int                    barra2_id;
                     double                 snom_mva;
                     struct {
                            int             bar_ref_id;
                            int             sentido_fluxo;        //{1:bar1 => bar2, 2:bar2 => bar1, 3:reversivel}
                            int             ligacao;              //ver enumMC_LIGACAO
                            double          var_tensao;           //% para baixo ou para cima
                            int             num_passo;            //passos para baixo ou para cima
                            bool            ajuste_auto;
                            struct   {
                                     int    passo;                //passo fixo selecionado
                                     }cfg_fixo;
                            struct   {
                                     double v_pu;                 //tensão alvo da barra controlada
                                     }cfg_auto;
                            }set;
                     struct {
                            complex<double> Sa1_pu;
                            complex<double> Sb1_pu;
                            complex<double> Sc1_pu;
                            complex<double> Sn1_pu;
                            complex<double> Sa2_pu;
                            complex<double> Sb2_pu;
                            complex<double> Sc2_pu;
                            complex<double> Sn2_pu;
                            }get;
                     };
//---------------------------------------------------------------------------
struct smcSUPRIMENTO {
                     void                   *eqpto;
                     int                    id;
                     int                    barra_id;
                     struct {
                            double          smax_mva;
                            struct   {//fornecimento
                                     int    tipo;                 //ver enumMC_SUPRIMENTO
                                     double p_mw;
                                     double q_mvar;
                                     double v_pu;
                                     double t_rad;
                                     }sup;
                            struct   {//potências de curto
                                     complex<double> fase_terra_mva;
                                     complex<double> trifasico_mva;
                                     }scc;
                            }set;
                     struct {
                            complex<double> Sa_pu;
                            complex<double> Sb_pu;
                            complex<double> Sc_pu;
                            complex<double> Sn_pu;
                            }get;
                     };
//---------------------------------------------------------------------------
struct smcTRAFO2E    {
                     void                  *eqpto;
                     int                   id;
                     int                   barra1_id;
                     int                   barra2_id;
                     struct {
                            double          snom_mva;
                            double          perda_ferro;          //pu da Snom
                            complex<double> z0_pu;                //pu da Snom
                            complex<double> z1_pu;                //pu da Snom
                            struct   {                            //ver enumMC_LIGACAO
                                     int             ligacao;
                                     double          vnom_kv;
                                     double          tap_kv;
                                     double          defasagem;
                                     complex<double> zater_ohm;
                                     } pri, sec;
                            struct   {
                                     int    pri_sec;              //{0:fixo; 1:ajuste no tap primário; 2:ajuste no tap secundário}
                                     int    barra_id;             //barra controlada
                                     double v_pu;                 //tensão alvo da barra controlada
                                     } ltc;
                            }set;
                     struct {
                            complex<double> Sa1_pu;
                            complex<double> Sb1_pu;
                            complex<double> Sc1_pu;
                            complex<double> Sn1_pu;
                            complex<double> Sa2_pu;
                            complex<double> Sb2_pu;
                            complex<double> Sc2_pu;
                            complex<double> Sn2_pu;
                            }get;
                     };
//---------------------------------------------------------------------------
struct smcTRAFO3E    {
//IMPORTANTE: restrição p/ o cálculo de curto
//1) barra1_id deve indicar a barra do primário
//2) primário deve ter ligação delta
//3) secundário e terciário devem ter ligação estrela
                     void                   *eqpto;
                     int                    id;
                     int                    barra1_id;
                     int                    barra2_id;
                     int                    barra3_id;
                     struct {
                            double          perda_ferro;          //% da Snom máxima
                            complex<double> zps0_pu;              //pu Sb=Snom máxima
                            complex<double> zps1_pu;              //pu Sb=Snom máxima
                            complex<double> zpt0_pu;              //pu Sb=Snom máxima
                            complex<double> zpt1_pu;              //pu Sb=Snom máxima
                            complex<double> zst0_pu;              //pu Sb=Snom máxima
                            complex<double> zst1_pu;              //pu Sb=Snom máxima
                            struct   {
                                     int             ligacao;     //ver enumMC_LIGACAO
                                     double          snom_mva;
                                     double          vnom_kv;
                                     double          tap_kv;
                                     double          defasagem;
                                     complex<double> zater_ohm;
                                     } pri, sec, ter;
                            }set;
                     };
//---------------------------------------------------------------------------
struct smcTRAFOZZ    {
                     void                   *eqpto;
                     int                    id;
                     int                    barra_id;
                     struct {
                            //double        snom_mva;
                            //double        vnom_kv;
                            complex<double> z0_pu;
                            }set;
                     };
//---------------------------------------------------------------------------
struct smcTRECHO     {
                     void                  *eqpto;
                     int                    id;
                     int                    barra1_id;
                     int                    barra2_id;
                     struct {
                            double          comprimento_km;
                            double          capacidade_amp;
                            double          custo_unitario;       //???????????
                            complex<double> z0_ohm;
                            complex<double> z1_ohm;
                            struct   {
                                     bool   capacitivo;
                                     double qcap_mvar;            //potência reativa TOTAL do trecho
                                     } modelo_pi;
                            }set;
                     struct {
                            complex<double> Sa1_pu;
                            complex<double> Sb1_pu;
                            complex<double> Sc1_pu;
                            complex<double> Sn1_pu;
                            complex<double> Sa2_pu;
                            complex<double> Sb2_pu;
                            complex<double> Sc2_pu;
                            complex<double> Sn2_pu;
                            }get;
                     };
//---------------------------------------------------------------------------
struct smcYREF       {
                     void                  *eqpto;
                     int                    barra_id;
                     struct {
                            complex<double> yater_mho;
                            }set;
                     };
//---------------------------------------------------------------------------
struct smcZREF       {
                     void                  *eqpto;
                     int                    barra_id;
                     struct {
                            complex<double> zater_ohm;
                            }set;
                     };

#endif
//---------------------------------------------------------------------------
//eof
