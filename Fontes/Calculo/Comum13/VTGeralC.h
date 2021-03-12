//---------------------------------------------------------------------------
#ifndef VTGeralCH
#define VTGeralCH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <complex>
using namespace std;

//---------------------------------------------------------------------------
#define DIM_ITERACOES              5  // Tamanho dos vetores de controle dos processos iterativos
#define ID_INVALIDO        -99999999  // ID temporário
#define MAX_FASES_G3               4  // Número máximo de fases nos equipamentos
#define NUM_ADMITANCIAS_H3         6  // Número de admitâncias para as cargas equivalentes (AN, BN, CN, AB, BC, CA)
#define NUM_LIG_CARGA             15  // Número de ligações possíveis para a carga (cf. 'enum_LIG_CARGA')
#define NUM_LIG_ESTATICO           3  // Número de ligações possíveis para estático de barra (cf. 'enum_LIG_ESTATICO')
#define NUM_MODELO_CARGA           3  // Número de modelos da carga (cf 'enum_MODELO_CARGA')
#define NUM_MODELO_REGULADOR       6  // Número de modelos de regulador de tensão (cf. 'enum_MODELO_REGULADOR' em VTData.h)
#define NUM_MODULO_CALCULO        16  // Número de módulos de cálculo (cf. 'enum_MODULO_CALCULO')
#define NUM_SUB_MODELO_REGULADOR   4  // Número de sub-modelos de regulador de tensão (cf. 'enum_SUB_MODELO_REGULADOR' em VTData.h)
#define NUM_TIPO_ATERR             3  // Número de tipos de aterramento (cf. 'enum_TIPO_ATERR')
#define NUM_TIPO_BARRA             6  // Número de tipos de barra (cf. 'enum_TIPO_BARRA')
#define NUM_TIPO_EQPTO             3  // Número de tipos de equipamento medidos (Estimador, cf. 'enum_TIPO_EQPTO')
#define NUM_TIPO_MEDICAO           5  // Número de tipos de medição (Estimador, cf. 'enum_TIPO_MEDICAO')
#define NUM_TIPO_TRAFO33           7  // Número de tipos de trafos de 3 enrolamentos (cf. 'enum_TIPO_TRAFO33')

enum enum_ATUALIZA_TENSOES {atNAO_ATUALIZA=0, atRECUPERA_ANTERIOR, atINIC_1_PU};

enum enum_BATERIA_ESTADO {estCARREGADA=0, estINTERMEDIARIO, estDESCARREGADA};

enum enum_BATERIA_MODO_OPERACAO {mopCARGA=0, mopDESCARGA, mopOCIOSA};

enum enum_BATERIA_TIPO_OPERACAO {topMANUAL=0, topAUTO};

enum enum_CONVERTE_TENSOES {ctPU_VOLT=0, ctVOLT_PU};

// IMPORTANTE: alterações na enum abaixo deverão ser feitas também na
//             definição de 'codigo_fases_curto[]', em TImpressao3.cpp
enum enum_FASES_CURTO {ctAN=0, ctBN, ctCN, ctAT, ctBT, ctCT, ctAB, ctBC, ctCA,
                       ctABN, ctBCN, ctCAN, ctABT, ctBCT, ctCAT, ctABC, ctABCN,
                       ctABCT};

// IMPORTANTE: alterações na enum abaixo deverão ser feitas também na
//             definição de 'codigo_fases[]', em TImpressao3.cpp
enum enum_LIG_CARGA {cgAN=0,   cgBN,   cgCN,     cgAT,  cgBT,  cgCT,
                     cgAB,     cgBC,   cgCA,     cgABN, cgBCN, cgCAN,
                     cgYaterr, cgYiso, cgTriang};

// IMPORTANTE: alterações na enum abaixo deverão ser feitas também na
//             definição de 'codigo_lig_enrol[]', em TImpressao3.cpp
enum enum_LIG_ENROL_TRIF {enTRIANGULO=0, enESTRELA_ISOLADA, enESTRELA_ATERRADA};

// IMPORTANTE: alterações na enum abaixo deverão ser feitas também na
//             definição de 'codigo_estatico[]', em TImpressao3.cpp
enum enum_LIG_ESTATICO {esYaterr=0, esYiso, esTriang};

// enum enum_METODO_RESOLUCAO {mrSisLin=0, mrBiCG_ILU};

enum enum_MODELO_CARGA {mcNaoImposto=-1, Icte=0, Scte, Zcte};

// IMPORTANTE: alterações na enum abaixo deverão ser feitas também na
//             macro 'NUM_MODULO_CALCULO' (acima) e na inicialização de
//             'codigo_programa[]' (TGeralC.cpp)
enum enum_MODULO_CALCULO {progINVALIDO=-1, progCURTO_CS=0, progCURTO_MA,
                          progEQV1, progEQV3, progESTAB, progESTIM1,
                          progESTIM3, progFR1, progFR3, progG1, progG3,
                          progH3, progNR1, progNR3, progPM, progYNODAL_ZCTE};

enum enum_TIPO_ARQUIVO {arqDADOS=0, arqLOG, arqLOG2, arqRESULT};

// IMPORTANTE: - NÃO ALTERAR A ORDEM DOS TIPOS DE ATERRAMENTO ABAIXO!!!!
//               (eles estão na ordem em que podem ser promovidos)
//             - alterações na enum abaixo deverão ser feitas também na
//               definição de 'codigo_ater[]', em TImpressao3.cpp
enum enum_TIPO_ATERR {atIsolado=0, atZ, atSolido};

// IMPORTANTE: alterações na enum abaixo deverão ser feitas também na
//             definição de 'codigo_tipo_barra[]', em TGeralC.cpp
enum enum_TIPO_BARRA {BAR_PQ=0, BAR_EXT, BAR_PQV, BAR_PV, BAR_SW, BAR_INT};

enum enum_TIPO_CARGA {tcSEM_CARGA=0, tcTODAS_YNODAL, tcINJECOES_EXT};

// Enum abaixo usada no Estimador
// IMPORTANTE: alterações na enum deverão ser feitas também em 'NUM_TIPO_EQPTO'
enum enum_TIPO_EQPTO {teBARRA=0, teTRECHO, teTRAFO};

enum enum_TIPO_LIGACAO_1 {CapSerie1=0, Reg2, Trafo2, Trafo3, TrafoEquiv1,
                          TrafoZZ1, Trecho1};

enum enum_TIPO_LIGACAO_3 {BancoDF=0, CapSerie3, Reg12, Reg32, Trafo12, Trafo32,
                          Trafo33, TrafoEquiv3, TrafoZZ3, Trecho3};

// Enum abaixo usada no Estimador
// IMPORTANTE: alterações na enum deverão ser feitas também em 'NUM_TIPO_MEDICAO'
enum enum_TIPO_MEDICAO {medMOD_V=0, medMOD_I, medP, medQ, medS};

// Enum abaixo usada no Estimador
enum enum_TIPO_MEDIDOR {medREAL=0, medVIRTUAL};

// Enum abaixo usada em NR1/NR3
enum enum_TIPO_MONTAGEM_JACOB {mjJACOB_FIXO=0, mjJACOB_NAO_FIXO};

// IMPORTANTE: alterações na enum abaixo deverão ser feitas também na
//             definição de 'codigo_tipo_motor[]' em TImpressaoPM.cpp
enum enum_TIPO_MOTOR {tmROTOR_GAIOLA=0, tmROTOR_BOBINADO};

// Enum usada em NR1/NR3
// O tipo de nó 'tnSW_INT' indica que se trata de nó de barra swing ou interna
enum enum_TIPO_NO {tnVR_VM=0, tnTHETA_TAP, tnPV_REF, tnPV_GER, tnSW_INT};

enum enum_TIPO_POTENCIA {tpPOTENCIA_ATIVA=0, tpPOTENCIA_REATIVA};

enum enum_TIPO_RELATORIO {trSEM_RELATORIO=0, trREL_RESUMIDO,
                          trREL_DETALHADO_1, trREL_DETALHADO_2};

// IMPORTANTE: alterações na enum abaixo deverão ser feitas também em:
//             - definição de 'codigo_tipo_12[]' em TImpressao3.cpp
//             - função 'TTrafo12::TipoImplementadoNR3()' [EVENTUALMENTE]
enum enum_TIPO_TRAFO12 {AUTOTRAFO4=0, AUTOTRAFO6_1T, AUTOTRAFO6_2T, TRAFO4, TRAFO5};

// IMPORTANTE: alterações na enum abaixo deverão ser feitas também em:
//             - definição de 'codigo_lig_trafo[]' em TImpressao1.cpp
//             - função 'TTrafo2::TipoImplementadoNR1()' [EVENTUALMENTE]
enum enum_TIPO_TRAFO2 {T2_AUTOTRAFO_DF=0, T2_AUTOTRAFO_Y, T2_DD, T2_DY, T2_YD,
                       T2_YY};

// IMPORTANTE: alterações na enum abaixo deverão ser feitas também em:
//             - definição de 'codigo_tipo_32[]' em TImpressao3.cpp
//             - função 'TTrafo32::TipoImplementadoNR3()' [EVENTUALMENTE]
enum enum_TIPO_TRAFO32 {AUTOTRAFO_DF=0, AUTOTRAFO_Y, TRAFO_DD, TRAFO_DY1,
                        TRAFO_DY11, TRAFO_YD1, TRAFO_YD11, TRAFO_YY};

// IMPORTANTE: alterações na enum abaixo deverão ser feitas também na
//             definição de 'codigo_tipo_33[]', em TGeralC.cpp
enum enum_TIPO_TRAFO33 {TRAFO_DDY1=0, TRAFO_DDY11, TRAFO_DY1Y1, TRAFO_DY1Y11,
                        TRAFO_DY11Y1, TRAFO_DY11Y11, TRAFO_YYY};

// 'ttrSUPRIM' indica trecho interno de suprimento (CurtoMA)
enum enum_TIPO_TRECHO {ttrNORMAL=0, ttrSUPRIM};

class  VTApl;
class  VTData;
class  VTTempoCPU;
struct Filename;

//---------------------------------------------------------------------------
class VTGeralC : public TObject
{
public:  // Properties
   __property bool CargaEmYnodal          = {read=PM_GetCargaEmYnodal};
   __property bool CargaInjecoesExt       = {read=PM_GetCargaInjecoesExt};
   __property bool CargaNula              = {read=PM_GetCargaNula};
   __property bool GravaYnodal            = {read=PM_GetGravaYnodal, write=PM_SetGravaYnodal};
   __property int  ModeloCargaImposto     = {read=PM_GetModeloCargaImposto, write=PM_SetModeloCargaImposto};
   __property int  TipoAtualizacaoTensoes = {read=PM_GetTipoAtualizacaoTensoes, write=PM_SetTipoAtualizacaoTensoes};
   __property int  TipoCarga              = {read=PM_GetTipoCarga, write=PM_SetTipoCarga};
   __property int  NumHarmonicas          = {read=PM_GetNumHarmonicas};

public:  // Métodos
                           __fastcall  VTGeralC() {};
   virtual                 __fastcall ~VTGeralC(void) {};
   virtual FILE *          __fastcall  AbreArquivo(int codigo) = 0;
   virtual void            __fastcall  CalculaPerdaTotal(void) = 0;
   virtual void            __fastcall  DefineParametrosEstimador(void) = 0;
   virtual AnsiString      __fastcall  DirTmp(void) = 0;
   virtual double          __fastcall  DuracaoPatamar(int np) = 0;
   virtual complex<double> __fastcall  FiltraZero(complex<double> valor,
                                                  double          precisao=1.e-8) = 0;
   virtual void            __fastcall  HabilitaSoPrimeiroPatamar(void) = 0;
   virtual bool            __fastcall  InsereOrdemHarmonica(int ordem) = 0;
   virtual bool            __fastcall  IsProgramaNR1(int indice) = 0;
   virtual void            __fastcall  MaxIter(int programa,
                                               int *num_max1,
                                               int *num_max2) = 0;
   virtual int             __fastcall  OrdemHarmonica(int indice) = 0;
   virtual bool            __fastcall  PatamarHabilitado(int   np,
														 TList *lisBATERIA=NULL) = 0;
   virtual void            __fastcall  ZeraContadoresNR(void) = 0;
   virtual void            __fastcall  ZeraEnergia(void) = 0;
   virtual void            __fastcall  ZeraTotaisPatamar(void) = 0;

public:  // Objetos externos
        VTApl      *apl;
        VTData     *data;
        VTTempoCPU *tCPU;

public:  // Dados
   bool flag_ater_comuns;             // Flag para considerar aterramentos ATZ comuns (true) ou não (false)
   bool flag_capac_LT;                // Flag para indicar inclusão da capacitância de linhas
   bool flag_imprime_bar_desc;        // Flag para imprimir (true) barras desconexas
   bool flag_inclui_CNLs;             // Flag para incluir (true) Cargas Não-Lineares
   bool flag_inclui_mutuas;           // Flag para incluir (true) impedâncias mútuas
   bool flag_ypar_trafos;             // Flag para indicar inclusão do ramo paralelo de trafos
   bool flag_TRAFO5_Como_Trafo3E;     // Flag para representar TRAFO5 com o modelo de Trafo3E (OpenDSS)
   bool flag_utiliza_bancoDF;         // Flag para utilizar (true) bancos em Delta-Fechado ou não (false)
   bool remonta_y_completa_delta_tap; // Flag para remontar [Ynodal] do zero quando há alteração de taps (só NR1)
   bool usa_pi_equiv;                 // Flag para indicar uso do modelo pi-equivalente em linhas (em vez do modelo pi-nominal)
   bool usa_vdnrm;                    // Flag para indicar uso do Very Dishonest NR Method (NR1/NR3)
   bool todos_sup_barra_interna;      // Flag que indica se todos os suprimentos terão barra interna (true) ou não (false) (Gauss3/NR3, etc.)

   int  Programa1;             // Programa executado (principal) (cf. 'enum_PROGS' acima)
   int  Programa2;             // Programa executado (auxiliar - Estabilidade) (cf. 'enum_PROGS' acima)
   int  tipo_relatorio;        // Tipo de relatório (cf. enum_TIPO_RELATORIO acima)
   // int  metodo_resolucao;      // Método de resolução dos sistemas lineares (cf. 'enum_METODO_RESOLUCAO')
   int  criterio_proc_colunas; // Critério para processamento de colunas (cf. 'enum_CRIT_SELEC_LINHAS_COLUNAS' em VTSisLin.h)
   int  num_barras_PQ;         // Número de barras PQ
   int  num_barras_EXT;        // Número de barras EXT
   int  num_barras_PQV;        // Número de barras PQV
   int  num_barras_PV;         // Número de barras PV
   int  num_barras_SW;         // Número de barras SW
   int  num_barras_INT;        // Número de barras INT
   int  num_barras_carga;      // Número de barras de carga (PQ + EXT + PQV)
   int  num_barras_geracao;    // Número de barras de geração (PV + SW + INT)
   int  num_chaves;            // Número de chaves (representadas por trechos de rede)
   int  num_lin_submatriz;     // Número de linhas / colunas da matriz Ycc
   int  num_tot_linhas;        // Número total de linhas / colunas da matriz Ynodal
   int  num_tot_pat;           // Número total de patamares
   int  num_pat_calc;          // Número de patamares já calculados
   int  num_pat_OK;            // Número de patamares com convergência OK
   int  num_iter_min;          // Número mínimo de iterações antes de bloquear ajuste de taps
   int  num_open_dados;        // Controle de abertura do arquivo de dados
   int  num_open_log;          // Controle de abertura do arquivo de log
   int  num_open_log2;         // Controle de abertura do arquivo de log (segundo arquivo)
   int  num_open_result;       // Controle de abertura do arquivo de resultados

   int  modelo_eqv; // modeloEQV_INDEF: a rede não contém elementos equivalentes
                    // modeloEQV1:      a rede contém elementos equivalentes
                    //                  gerados por Eqv1
                    // modeloEQV3:      a rede contém elementos equivalentes
                    //                  gerados por Eqv3
                    // cf. Fontes\Eqv\VTEqv.h

   double          frequencia_hz;           // Frequência da rede (Hz)
   double          Sbase;                   // Potência de base TRIFÁSICA (MVA)
   double          Sbase_fase;              // Potência de base POR FASE  (MVA)
   double          toler_tap_pu;            // Tolerância para considerar 2 valores de tap iguais ou diferentes
   double          comp_chave_m;            // Comprimento do trecho que representa chave
   // double          reat_regul_DEMAIS_NR_pu; // Reatância  de default para reguladores de tensão (todos exceto AUTOTRAFO_DF) em NR1/NR3 (pu na base nominal)
   // complex<double> zreg_AUTODF_NR_pu;       // Impedância de default para reguladores de tensão AUTOTRAFO_DF em NR1/NR3 (pu na base nominal)
   complex<double> geracao_total;           // Potência total gerada (cálculo mais recente) (kW, kVAr)
   complex<double> carga_total;             // Carga total (kW, kVAr)
   complex<double> perda_total_1;           // Perda total calculada através de (Ger_tot - Carga_tot) (kW, kVAr)
   complex<double> perda_total_2;           // Perda total calculada através de (ri2, xi2) (kW, kVAr)
   complex<double> perda_geradores;         // Perda total nos geradores (kW, kVAr)
   complex<double> perda_admit_reduc;       // Perda total nas admitâncias para a referência (reduc. rede) (kW, kVAr)
   complex<double> perda_ypar_trafos;       // Perda total no ramo em paralelo de trafos (kW, kVAr)
   complex<double> energia_carga_mvah;      // Energia total de carga   (todos os patamares) (MWh, MVArh)
   complex<double> energia_geracao_mvah;    // Energia total de geração (todos os patamares) (MWh, MVArh)
   complex<double> energia_perdas_mvah;     // Energia total de perdas  (todos os patamares) (MWh, MVArh)
   complex<double> zchave_ohm;              // Impedância para chaves fechadas

   AnsiString      Plataforma;                                       // 32 ou 64 bits
   AnsiString      Configuracao;                                     // Debug ou Release
   AnsiString      NomeRede;                                         // Nome do diretório da base de dados da rede
   AnsiString      CodigoBarraFicticia;                              // Código para barra  fictícia (quando não há nenhuma barra de carga)
   AnsiString      CodigoTrechoFicticio;                             // Código para trecho fictício (quando não há nenhuma barra de carga)
   AnsiString      codigo_modelo_regu[NUM_MODELO_REGULADOR];         // Código dos modelos de regulador de tensão
   AnsiString      codigo_programa[NUM_MODULO_CALCULO];              // Código dos módulos de cálculo
   AnsiString      codigo_sub_modelo_regu[NUM_SUB_MODELO_REGULADOR]; // Código dos sub-modelos de regulador de tensão
   AnsiString      codigo_tipo_33[NUM_TIPO_TRAFO33];                 // Código dos tipos de trafos de 3 enrolamentos (modelos equilibrados e desequilibrados)
   AnsiString      codigo_tipo_barra[NUM_TIPO_BARRA];                // Código dos tipos de barra
   char            codigo_tipo_medicao[NUM_TIPO_MEDICAO];            // Código dos tipos de medição

   // Chave para endereços em MaxIteracao, NumIteracao, FlagConv e Precisao:
   // Estabilidade:     [0] - Processo iterativo do fluxo de potência (G1 ou NR1)
   //                   [1] - Loop de controles (tap de reg. ENERQCT e Qmax em barras PV - NR1)
   //                   [2] - Loop de cálculo em 'CalculaRedeSimples()'
   //                   [3] - Loop do Método de Euler Modificado (integração eqs. dif.)
   //                   [4] - Loop do cálculo de 'gama'
   // Estim1 / Estim3:  [0] - Processo iterativo do método de Newton (estado da rede)
   //                   [1] - Com VDNM - "Very Dishonest Newton Method"
   // Gauss1 / Gauss3:  [0] - Processo iterativo único
   //                   [2] - Montagem de Ynodal
   // NR1 / NR3:        [0] - Processo iterativo Newton-Raphson
   //                   [1] - Loop de controles (tap de reg. DOIS_SENTIDOS/ENERQCT e Qmax em barras PV)
   //                   [2] - Montagem/fatoração do Jacobiano (com ou sem VDNRM - "Very Dishonest Newton-Raphson Method")
   // PM:               [0] - Processo iterativo do fluxo de potência (G1 ou NR1)
   //                   [1] - Loop de controles (tap de reg. ENERQCT e Qmax em barras PV - NR1)
   //                   [2] - Loop de cálculo em 'CalculaRedeSimples()'
   //                   [3] - Loop do Método de Euler Modificado (integração eqs. dif.)
   int    MaxIteracaoDefault;       // Para inicialização de 'MaxIteracao[]'
   int    MaxIteracao[DIM_ITERACOES];
   int    NumIteracao[DIM_ITERACOES];
   bool   FlagConv[DIM_ITERACOES];
   double Precisao[DIM_ITERACOES];  // Tolerância dos processos iterativos

   // Variáveis utilizadas por Estabilidade / PartidaMotor
   int        num_instantes;   // Número de instantes do transitório
   int        num_bar_result;  // Número de barras   para exibição de resultados
   int        num_lig_result;  // Número de ligações para exibição de resultados
   int        num_geradores;   // Número de unidades geradoras
   int        tempo_ms;        // Instante de cálculo
   double     ctemp_min;       // Menor constante de tempo (s)
   AnsiString DescricaoEvento; // Descrição dos eventos

   // Variáveis utilizadas por ESTIM1 / ESTIM3
   bool   LimitaSigmas;             // Flag para controlar mapeamento de sigmas
   int    TipoBarraRef;             // Tipo da(s) barra(s) de referência (cf. 'enum_BARRA_REF' em VTEstimador.h)
   int    metodoEE;                 // Método de resolução da Estimação de Estado (cf. 'enum_METODO_EE' em VTEstimador.h)
   int    offset_bar_ref;           // Offset da barra de referência
   int    OrdenacaoLinhasQR;        // Controle da ordenação de linhas em QR_Givens() (cf. enum_CRIT_ORD_LINHAS_QR em VTSisLin.h)
   int    num_med_reais;            // Número de medidores reais
   int    num_med_desab;            // Número de medidores desabilitados
   int    num_var_est;              // Número de variáveis de estado (Ns)
   double FatMultMedChave;          // Fator multiplicativo da medição de chaves (TMontaMedicaoChave.h)
   double DesvPadCargaAjustavel;    // Desvio-padrão de medidor em barra com carga ajustável
   double DesvPadCargaNaoAjustavel; // Desvio-padrão de medidor em barra com carga não ajustável
   double DesvPadMedV;              // Desvio-padrão para medidores de módulo de V
   double desvio_max;               // Número máximo de desvios-padrão para manter medidores ligados
   double nivel_confianca;          // Nível de confiança das medições (pu)
   double semi_intervalo_normal;    // Tamanho do semi-intervalo na distribuição normal, em função do nível de confiança
   double SigmaMinNominal_pu;       // Valor mínimo NOMINAL do desvio-padrão para normalização
   double SigmaMaxNominal_pu;       // Valor máximo NOMINAL do desvio-padrão para normalização
   double SigmaMinAtual_pu;         // Valor mínimo ATUAL   do desvio-padrão para normalização
   double SigmaMaxAtual_pu;         // Valor máximo ATUAL   do desvio-padrão para normalização

   // Variáveis utilizadas por NR1 / NR3
   bool       flag_qmax_pv;   // Flag para verificar (true) Qmax em barras PV
   AnsiString versao_NR1;
   AnsiString versao_NR3;
   int        num_lin_Jacob;  // Número total de linhas / colunas do Jacobiano (NR3)
   int        num_eqs_Ir;     // Barras PQ/EXT/PQV
   int        num_eqs_Im;     // Barras PQ/EXT/PQV
   int        num_eqs_P3f;    // Barras PV
   int        num_vars_Vr;    // Barras PQ/EXT/PQV
   int        num_vars_Vm;    // Barras PQ/EXT/PQV
   int        num_vars_Theta; // Barras PV
   int        num_vars_Tap;   // Barras PQV

   // Variáveis utilizadas por PartidaMotor
   int num_motores; // Número de motores

   // Estruturas (todos os módulos)
   Filename *strFilename;

protected: // Métodos acessados via property
   virtual bool __fastcall PM_GetCargaEmYnodal(void) = 0;
   virtual bool __fastcall PM_GetCargaInjecoesExt(void) = 0;
   virtual bool __fastcall PM_GetCargaNula(void) = 0;
   virtual bool __fastcall PM_GetGravaYnodal(void) = 0;
   virtual int  __fastcall PM_GetModeloCargaImposto(void) = 0;
   virtual int  __fastcall PM_GetNumHarmonicas(void) = 0;
   virtual int  __fastcall PM_GetTipoAtualizacaoTensoes(void) = 0;
   virtual int  __fastcall PM_GetTipoCarga(void) = 0;
   virtual void __fastcall PM_SetGravaYnodal(bool flag) = 0;
   virtual void __fastcall PM_SetModeloCargaImposto(int modelo) = 0;
   virtual void __fastcall PM_SetTipoAtualizacaoTensoes(int tipo) = 0;
   virtual void __fastcall PM_SetTipoCarga(int tipo) = 0;
};

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTGeralC
//---------------------------------------------------------------------------
VTGeralC * __fastcall NewObjGeralC(int    prog,
                                   int    tipo_relatorio,
                                   VTApl  *apl,
                                   VTData *data,
                                   bool   flag_imprime_bar_desc=false);

#endif
//---------------------------------------------------------------------------

