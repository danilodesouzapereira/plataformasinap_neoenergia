//---------------------------------------------------------------------------
#ifndef InterfaceH
#define InterfaceH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <complex>
using namespace std;
#include "..\Constante\Fases.h"

//---------------------------------------------------------------------------
class VTEqpto;

//---------------------------------------------------------------------------
enum emcCFG_SUP {//tipos de Suprimento
                tiposupPV=1, //OBS: se mudar p/ 0 precisa acertar o SINAP
                tiposupPQ,
                tiposupVF
                };
//---------------------------------------------------------------------------
enum emcCFG_TRECHO {//tipos de arranjos de trecho/tramo
                   tipo_Z0Z1_Y0Y1=1, //Z
                   tipo_MATZ_MATC
                   };

//---------------------------------------------------------------------------
enum emcGRUPO_LIGACAO {// Grupo da ligação (somente trafos trifásicos DY, YD,
                       // DDY e DYY)
                      glINDEF = 0,
                      // Trafo2E
                      glDy1,
                      glDy11,
                      glYd1,
                      glYd11,
                      // Trafo3E
                      //    Configurações básicas (01 a 07)
                      glDD0Y1,     // Configuração 01
                      glDD0Y11,    // Configuração 02
                      glDY1Y1,     // Configuração 03
                      glDY1Y11,    // Configuração 04
                      glDY11Y1,    // Configuração 05
                      glDY11Y11,   // Configuração 06
                      glYY0Y0,     // Configuração 07
                      //    Configurações derivadas (08 a 19)
                      glDY1D0,     // Configuração 08
                      glYD11D11,   // Configuração 09
                      glDY11D0,    // Configuração 10
                      glYD1D1,     // Configuração 11
                      glYY0D11,    // Configuração 12
                      glYD11Y0,    // Configuração 13
                      glYY10D11,   // Configuração 14
                      glYD1Y2,     // Configuração 15
                      glYY2D1,     // Configuração 16
                      glYD11Y10,   // Configuração 17
                      glYY0D1,     // Configuração 18
                      glYD1Y0      // Configuração 19
                      };
//---------------------------------------------------------------------------
enum emcLIGACAO {//tipos de ligação dos eqptos
                // IMPORTANTE: manter coerência com 'enumLIGTRF' em
                //             Fontes\Rede\VTEqpto.h
                ligacaoESTRELA_ATERRADO = 0,
                ligacaoTRIANGULO,
                ligacaoESTRELA,
                ligacaoMONOFASICO,
                ligacaoDELTA_ABERTO,
                ligacaoDELTA_FECHADO,
                ligacaoINDEFINIDA
                };
//---------------------------------------------------------------------------
enum emcLTC_TIPO {//tipos de LTC
                 ltcNAO_USADOX=0, ltcPRIMARIOX, ltcSECUNDARIOX};

//---------------------------------------------------------------------------

//definição das estruturas de interface
//---------------------------------------------------------------------------
struct strENROLAMENTO
   {
   int             enrol_orig;  // Enrolamento original no Sinap (0, 1 ou 2)
   int             ligacao;     // ver enum emcLIGACAO
   double          snom_mva;
   double          vnom_kv;
   double          tap_kv;
   double          defasagem;
   complex<double> zater_ohm;
   };

//---------------------------------------------------------------------------
struct smcATERRAMENTO {
                      void           *barraCALC;
                      complex<double> z_ohm;
                      };

//---------------------------------------------------------------------------
struct smcBARRA {
                VTEqpto    *eqpto;
                AnsiString codigo;
                int        id;
                int        fases;
                int        tipo_zona; // cf. enum_TIPO_ZONA_CALCULO em VTData.h
                double     vff_kv;
                double     van_kv;
                double     vbn_kv;
                double     vcn_kv;
                double     fase_zona_rad[MAX_FASE];;
                bool       mantida;
                bool       sup_equivalente;
                };

//---------------------------------------------------------------------------
struct smcBATERIA {
                  // Dados nominais (fixos)
                  VTEqpto    *eqpto;
                  AnsiString codigo;
                  int        id;
                  void       *barraCALC;
                  int        fases;            // faseA, faseAN, etc.
                  int        tipo_operacao;    // Cf. 'enum_BATERIA_TIPO_OPERACAO' em VTGeralC.h
                  int        modelo_carga;     // Icte, Scte ou Zcte quando no estado "Carga"
                  int        modelo_desc;      // Icte, Scte ou Zcte quando no estado "Descarga"
                  double     pnom_kw;          // Potência ativa nominal (valor de referência)
                  double     enom_kwh;         // Energia nominal (capacidade)
                  double     scap_inv_kva;     // Potência aparente máxima do inversor (capacidade)
                  double     en_armaz_inic_pu; // Energia inicial armazenada (pu de 'enom_kwh')
                  double     pext_carga_kw;    // Potência ativa   externa absorvida durante a carga (só tipo AUTO)
                  double     qext_carga_kvar;  // Potência reativa externa absorvida durante a carga (só tipo AUTO)
                  double     pext_desc_kw;     // Potência ativa   externa fornecida durante a descarga (só tipo AUTO)
                  double     qext_desc_kvar;   // Potência reativa externa fornecida durante a descarga (só tipo AUTO)
                  double     p_ociosa_pu;      // Potência ativa   absorvida no modo "ociosa" (pu de 'pnom_kw')
                  double     q_ociosa_pu;      // Potência reativa absorvida no modo "ociosa" (pu de 'pnom_kw')
                  double     rend_carga_pu;    // Rendimento em carga
                  double     rend_desc_pu;     // Rendimento em descarga
                  double     gatilho_carga_pu; // Início do ciclo de carga    (pu de 'enom_kwh')
                  double     gatilho_desc_pu;  // Início do ciclo de descarga (pu de 'enom_kwh')
                  };

//---------------------------------------------------------------------------
struct smcCANAL {
                int        id;
                int        tipo;            // Ver enum tipoCANAL em Rede\VTCanal
                int        eqpto_id;
                void       *canal;
                AnsiString codigo;
                int        tipo_eqpto;      // Cf. enum_TIPO_EQPTO   em Fontes\Calculo\Comum13\VTGeralC.h
                int        tipo_medicao;    // Cf. enum_TIPO_MEDICAO em Fontes\Calculo\Comum13\VTGeralC.h
                int        fase_int;        // Fase medida: faseA, faseB ou faseC
                double     tolerancia_perc; // Tolerância da medição em %
                double     valor_fe;        // Valor de fundo de escala (kV, A, MVA, MW ou MVAR)
                // bool       descartavel;     // Pode ser excluído no processo de ajuste
                bool       enabled;         // Indica se o canal está habilitado ou não (tratamento de erros)
                };
//---------------------------------------------------------------------------
struct smcCAPACITOR {
                    VTEqpto    *eqpto;
                    AnsiString codigo;
                    int        id;
                    void       *barraCALC;
                    int        ligacao; //ver enum emcLIGACAO
                    };
//---------------------------------------------------------------------------
struct smcCAPSERIE {
                   VTEqpto    *eqpto;
                   AnsiString codigo;
                   int        id;
                   void       *barraCALC_1;
                   void       *barraCALC_2;
                   double     r_ohm;
                   double     x_ohm;
                   };
//---------------------------------------------------------------------------
struct smcCARGA {
                VTEqpto    *eqpto;
                AnsiString codigo;
                int        id;
                int        barra_id;   // Remover após acerto de FluxoH3
                void       *barraCALC;
                int        fases;      // Fontes\Constante\Fases.h
                };
//---------------------------------------------------------------------------
struct smcCHAVE {
                VTEqpto         *eqpto;
                AnsiString      codigo;
                int             id;
                void            *barraCALC_1;
                void            *barraCALC_2;
                int             corn_amp;
                int             estado;                   //{0: aberta, 1: fechada}
                int             fases;
                complex<double> z0_pu;                    //ohm
                complex<double> z1_pu;                    //ohm
                complex<double> matZ[MAX_FASE][MAX_FASE]; //ohm
                };

//---------------------------------------------------------------------------
#define MAX_HARMONICAS 100
struct smcCNL {
              VTEqpto    *eqpto;
              AnsiString codigo;
              int        id;
              void       *barraCALC;
              int        num_harmonicas;                     // Número de frequências harmônicas (inclui a fundamental)
              int        ordem[MAX_HARMONICAS];              // Ordem das harmônicas (1, 3, 5, ...)
              double     cos_fi;                             // Fator de potência na fundamental (pu, < 0 para capacitivo)
              double     pot_kva[MAX_HARMONICAS][MAX_FASE];  // Potência aparente das harmônicas por fase:
                                        // kVA para a fundamental, pu da fundamental para as demais frequências
              double     ang_grau[MAX_HARMONICAS][MAX_FASE]; // Ângulo das harmônicas por fase (grau)
              };
//---------------------------------------------------------------------------
#define MAX_FASE_DEM 50
struct smcDEMANDA {
                  int capacity;           //dimensão do vetor de estrutura
                  int count;              //número de elementos preenchidos
                  struct {
                         AnsiString fases_str;
                         int        fases_int;
                         int        modelo;
                         complex<double> s_mva[3]; // Icte, Scte, Zcte (NESTA ORDEM)
                         }dem[MAX_FASE_DEM];
                  };

//---------------------------------------------------------------------------
struct smcESTADO_BATERIA {
                         VTEqpto *eqpto;
                         int     ind_pat;
                         int     estado;
                         int     modo_operacao;
                         double  en_armaz_kwh; // Energia armazenada NO FIM DO PATAMAR
                         double  pext_kw;
                         double  qext_kvar;
                         };

//---------------------------------------------------------------------------
struct smcFILTRO {
                 VTEqpto         *eqpto;
                 AnsiString      codigo;
                 int             id;
                 void            *barraCALC;
                 double          vnom_kv;
                 double          snom_mva;
                 complex<double> z0_pu;
                 };
//---------------------------------------------------------------------------
struct smcFLUXO {
                bool            total;
                complex<double> stotal_mva;
                complex<double> perda_total_kva;
                complex<double> sfase_mva[MAX_FASE];
                complex<double> ifase_amp[MAX_FASE];
                };
//---------------------------------------------------------------------------
struct smcGERACAO {
                  int  capacity;             //dimensão do vetor de estrutura
                  int  count;                //número de elementos preenchidos
                  bool ativa;
                  bool zona_unica;
                  struct{
                        double p_mw;
                        double q_mvar;
                        double v_pu;
                        double t_rad;
                        }ger[MAX_FASE];
                  };
//---------------------------------------------------------------------------
struct smcGERADOR {
                  VTEqpto         *eqpto;
                  AnsiString      codigo;
                  int             id;
                  int             barra_id;    // Remover após acerto de FluxoH3
                  void            *barraCALC;
                  int             ligacao;     //ver enumMC_LIGACAO
                  int             fases;
                  double          vnom_kv;     //tensão nominal
                  double          snom_mva;    //potência nominal
                  double          smax_mva;    //potência máxima
                  double          fp_min;      //fator de potência mínimo
                  int             tipo_sup;    //ver enumMC_SUPRIMENTO
                  complex<double> z0_pu;       //Sbase = Snom
                  complex<double> z1_pu;       //Sbase = Snom
                  struct {//potências de curto
                         complex<double> fase_terra_mva;
                         complex<double> trifasico_mva;
                         }scc;
                  };
//---------------------------------------------------------------------------
struct smcMEDICAO {
   // Unidades para 'valor':
   //    tipo_medicao == medMOD_V  ==>  [pu]
   //                    medMOD_I  ==>  [A]
   //                    medP      ==>  [MW]
   //                    medQ      ==>  [MVAr]
   //                    medS      ==>  [MVA]
                  void   *canal;
                  double valor;
                  struct
                     {
                     double valor;
                     double desvio;
                     bool   flag;
                     } estimado;
                  };
//---------------------------------------------------------------------------
struct smcMEDIDOR {
                   #define SIZEOF_CANAL   100  // Alterado em 2015.11.07 (HPS)
                   void       *medidor;
                   AnsiString codigo;
                   int        id;
                   void       *barraCALC;
                   int        numero_canal;
                   smcCANAL   smc_canal[SIZEOF_CANAL];
                   };
//---------------------------------------------------------------------------
struct smcMUTUA {
                VTEqpto         *eqpto;
                AnsiString      codigo;
                int             id;
                int             trecho1_id;
                int             trecho2_id;
                complex<double> z0_ohm;
                complex<double> z1_ohm;
                struct{
                      int trecho1_bar_id;
                      int trecho2_bar_id;
                      }sentido;
                };

//---------------------------------------------------------------------------
struct smcPERDA_ENERGIA_GLOBAL
                     {
                     complex<double> perda_energia_diaria_mvah;
                     };

//---------------------------------------------------------------------------
struct smcPERDA_ENERGIA_LIGACAO
                     {
                     VTEqpto *eqpto;                   // Ponteiro para a ligação
                     double  perda_energia_diaria_kwh; // Total da ligação
                     double  perda_ferro_diaria_kwh;   // Somente para trafo ou regulador
                     };

//---------------------------------------------------------------------------
struct smcREATOR {
                 VTEqpto    *eqpto;
                 AnsiString codigo;
                 int        id;
                 void       *barraCALC;
                 int        ligacao;              //ver enum emcLIGACAO
                 double     q_mvar;
                 };

//---------------------------------------------------------------------------
struct smcRES_MEDIDOR {
   // Unidades para 'val_med' e 'val_est':
   //    TipoCanal == canalV  ==>  [pu]
   //                 canalI  ==>  [A]
   //                 canalP  ==>  [MW]     Serve para 'val_est_nom' também
   //                 canalQ  ==>  [MVAr]   Serve para 'val_est_nom' também
   //                 canalS  ==>  [MVA]
                      void   *canal;
                      int    ind_pat;
                      double val_med;
                      double val_est;
                      double val_est_nom;       // Valor nominal da potência de carga (v = 1 pu)
                      double erro_pu;           // Número de desvios-padrão do medidor: (val_med - val_est)/sigma
                      bool   excede_desvio_max; // Indica se o desvio (em núm. de DPs) excedeu o máximo
                      };

//---------------------------------------------------------------------------
struct smcSUPRIMENTO {
                     VTEqpto    *eqpto;
                     AnsiString codigo;
                     int        id;
                     int        barra_id;    // Remover após acerto de FluxoH3
                     void       *barraCALC;
                     double     smax_mva;
                     int        tipo_sup;    // ver emcCFG_SUP
                     int        ligacao;     // ver enumMC_LIGACAO
                     int        fases;
                     struct {//potências de curto
                            complex<double> fase_terra_mva;
                            complex<double> trifasico_mva;
                            }scc;
                     struct {//impedâncias equivalentes, Sb=100MVA
                            complex<double> z0_pu;
                            complex<double> z1_pu;
                            }eqv;
                     };
//---------------------------------------------------------------------------
struct smcTENSAO {
                 // 'existe_fase' indica se a fase existe (true) ou não (false)
                 // IMPORTANTE: end. 0 = faseA, 1 = faseB, 2 = faseC, 3 = faseN
                 bool            existe_fase[MAX_FASE];
                 complex<double> v_pu[MAX_FASE];
                 };
//---------------------------------------------------------------------------
struct smcTRAFO2E {
                  VTEqpto         *eqpto;
                  AnsiString      codigo;
                  int             id;
                  void            *barraCALC_1;
                  void            *barraCALC_2;
                  int             fase_comum;  // Para uso nos reguladores de 6 terminais
                  int             grupo_lig;   // Ver 'emcGRUPO_LIGACAO'
                  double          snom_mva;
                  double          perda_ferro; // % da Snom
                  bool            auto_trafo;
                  bool            monofasico;
                  bool            reducao;
                  complex<double> z0_pu;       // pu de Snom
                  complex<double> z1_pu;       // pu de Snom
                  struct {
                         int             ligacao; // ver enum emcLIGACAO
                         int             fase;
                         double          vnom_kv;
                         double          tap_kv;
                         double          defasagem;
                         complex<double> zater_ohm;
                         } pri, sec;
                  struct {
								 int    modelo;                 // Ver enum_MODELO_REGULADOR em VTData.h
								 int    sub_modelo;             // Ver enum_SUB_MODELO_REGULADOR em VTData.h
								 int    pri_sec;                // {0:fixo; 1:ajuste no tap primário; 2:ajuste no tap secundário}
								 int    barra_id_12;            // barra controlada (sentido 1 -> 2)
								 int    barra_id_21;            // barra controlada (sentido 2 -> 1)
								 double v_pu_12;                // tensão alvo da barra controlada (sentido 1 -> 2)
								 double v_pu_21;                // tensão alvo da barra controlada (sentido 2 -> 1)
                         double banda_morta_pu;         // "Dead band" para sentido direto  (não altera tap se a tensão estiver na faixa v_pu +/- toler_pu)
                         double banda_morta_inv_pu;     // "Dead band" para sentido reverso (não altera tap se a tensão estiver na faixa v_pu +/- toler_pu)
								 double var_tensao;             // % para baixo ou para cima
								 int    num_passo;              // passos para baixo ou para cima
								 int    passo_calc[MAX_FASE-1]; // passo calculado pelo Fluxo (1 valor por fase)
                         } ltc;
                  // Dados específicos para trafos equivalentes
                  int             modelo_eqv;     // 1: gerado por Eqv1 ; 3: gerado por Eqv3
                  complex<double> Ynodal0_ij_pu;  // Seq. zero,   elemento nodal ij
                  complex<double> Ynodal0_ji_pu;  // Seq. zero,   elemento nodal ji
                  complex<double> Ynodal1_ij_pu;  // Seq. direta, elemento nodal ij
                  complex<double> Ynodal1_ji_pu;  // Seq. direta, elemento nodal ji
                  complex<double> mat_ynodal_ij_S[MAX_FASE][MAX_FASE]; // Submatriz nodal ij [S]
                  complex<double> mat_ynodal_ji_S[MAX_FASE][MAX_FASE]; // Submatriz nodal ji [S]
                  };
//---------------------------------------------------------------------------
struct smcTRAFO3E {
                  VTEqpto         *eqpto;
                  AnsiString      codigo;
                  int             id;
                  int             grupo_lig;     // Ver 'emcGRUPO_LIGACAO'
                  void            *barraCALC_1;
                  void            *barraCALC_2;
                  void            *barraCALC_3;
                  double          perda_ferro;   //% da Snom máxima
                  complex<double> zps0_pu;       //pu Sb=Snom máxima
                  complex<double> zps1_pu;       //pu Sb=Snom máxima
                  complex<double> zpt0_pu;       //pu Sb=Snom máxima
                  complex<double> zpt1_pu;       //pu Sb=Snom máxima
                  complex<double> zst0_pu;       //pu Sb=Snom máxima
                  complex<double> zst1_pu;       //pu Sb=Snom máxima
                  strENROLAMENTO  pri, sec,ter;
                  };
//---------------------------------------------------------------------------
struct smcTRAFOZZ {
                  VTEqpto         *eqpto;
                  AnsiString      codigo;
                  int             id;
                  void            *barraCALC;
                  double          vnom_kv;
                  double          snom_mva;
                  complex<double> z0_pu;
                  };
//---------------------------------------------------------------------------
struct smcTRECHO {
                 VTEqpto         *eqpto;
                 AnsiString      codigo;
                 int             id;
                 int             barra1_id;  // Remover após acerto de FluxoH3
                 int             barra2_id;  // Remover após acerto de FluxoH3
                 void            *barraCALC_1;
                 void            *barraCALC_2;
                 int             fases;
                 int             arranjo_id;
                 double          comprimento_km;
                 double          vet_iadm[MAX_FASE]; //A
                 int             tipo_cfg;           //ver emcCFG_TRECHO
                 complex<double> z0_pu;
                 complex<double> z1_pu;
                 complex<double> matZ[MAX_FASE][MAX_FASE]; //ohm
                 double          matC[MAX_FASE][MAX_FASE]; //mho
                 struct {
                        double y0cap_pu;             //admitância paralela de seq.0 (pu)
                        double y1cap_pu;             //admitância paralela de seq.1 (pu)
                        } modelo_pi;
                 };
//---------------------------------------------------------------------------
#define smcTRAMO smcTRECHO
//---------------------------------------------------------------------------
struct smcYREF {
               VTEqpto         *eqpto;
               AnsiString      codigo;
               int             id;
               void            *barraCALC;
               complex<double> y0_pu;
               complex<double> y1_pu;
               complex<double> maty_S[MAX_FASE][MAX_FASE];
               };
#endif
//---------------------------------------------------------------------------
//eof
