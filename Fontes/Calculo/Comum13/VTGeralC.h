//---------------------------------------------------------------------------
#ifndef VTGeralCH
#define VTGeralCH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <complex>
using namespace std;

//---------------------------------------------------------------------------
#define DIM_ITERACOES              5  // Tamanho dos vetores de controle dos processos iterativos
#define ID_INVALIDO        -99999999  // ID tempor�rio
#define MAX_FASES_G3               4  // N�mero m�ximo de fases nos equipamentos
#define NUM_ADMITANCIAS_H3         6  // N�mero de admit�ncias para as cargas equivalentes (AN, BN, CN, AB, BC, CA)
#define NUM_LIG_CARGA             15  // N�mero de liga��es poss�veis para a carga (cf. 'enum_LIG_CARGA')
#define NUM_LIG_ESTATICO           3  // N�mero de liga��es poss�veis para est�tico de barra (cf. 'enum_LIG_ESTATICO')
#define NUM_MODELO_CARGA           3  // N�mero de modelos da carga (cf 'enum_MODELO_CARGA')
#define NUM_MODELO_REGULADOR       6  // N�mero de modelos de regulador de tens�o (cf. 'enum_MODELO_REGULADOR' em VTData.h)
#define NUM_MODULO_CALCULO        16  // N�mero de m�dulos de c�lculo (cf. 'enum_MODULO_CALCULO')
#define NUM_SUB_MODELO_REGULADOR   4  // N�mero de sub-modelos de regulador de tens�o (cf. 'enum_SUB_MODELO_REGULADOR' em VTData.h)
#define NUM_TIPO_ATERR             3  // N�mero de tipos de aterramento (cf. 'enum_TIPO_ATERR')
#define NUM_TIPO_BARRA             6  // N�mero de tipos de barra (cf. 'enum_TIPO_BARRA')
#define NUM_TIPO_EQPTO             3  // N�mero de tipos de equipamento medidos (Estimador, cf. 'enum_TIPO_EQPTO')
#define NUM_TIPO_MEDICAO           5  // N�mero de tipos de medi��o (Estimador, cf. 'enum_TIPO_MEDICAO')
#define NUM_TIPO_TRAFO33           7  // N�mero de tipos de trafos de 3 enrolamentos (cf. 'enum_TIPO_TRAFO33')

enum enum_ATUALIZA_TENSOES {atNAO_ATUALIZA=0, atRECUPERA_ANTERIOR, atINIC_1_PU};

enum enum_BATERIA_ESTADO {estCARREGADA=0, estINTERMEDIARIO, estDESCARREGADA};

enum enum_BATERIA_MODO_OPERACAO {mopCARGA=0, mopDESCARGA, mopOCIOSA};

enum enum_BATERIA_TIPO_OPERACAO {topMANUAL=0, topAUTO};

enum enum_CONVERTE_TENSOES {ctPU_VOLT=0, ctVOLT_PU};

// IMPORTANTE: altera��es na enum abaixo dever�o ser feitas tamb�m na
//             defini��o de 'codigo_fases_curto[]', em TImpressao3.cpp
enum enum_FASES_CURTO {ctAN=0, ctBN, ctCN, ctAT, ctBT, ctCT, ctAB, ctBC, ctCA,
                       ctABN, ctBCN, ctCAN, ctABT, ctBCT, ctCAT, ctABC, ctABCN,
                       ctABCT};

// IMPORTANTE: altera��es na enum abaixo dever�o ser feitas tamb�m na
//             defini��o de 'codigo_fases[]', em TImpressao3.cpp
enum enum_LIG_CARGA {cgAN=0,   cgBN,   cgCN,     cgAT,  cgBT,  cgCT,
                     cgAB,     cgBC,   cgCA,     cgABN, cgBCN, cgCAN,
                     cgYaterr, cgYiso, cgTriang};

// IMPORTANTE: altera��es na enum abaixo dever�o ser feitas tamb�m na
//             defini��o de 'codigo_lig_enrol[]', em TImpressao3.cpp
enum enum_LIG_ENROL_TRIF {enTRIANGULO=0, enESTRELA_ISOLADA, enESTRELA_ATERRADA};

// IMPORTANTE: altera��es na enum abaixo dever�o ser feitas tamb�m na
//             defini��o de 'codigo_estatico[]', em TImpressao3.cpp
enum enum_LIG_ESTATICO {esYaterr=0, esYiso, esTriang};

// enum enum_METODO_RESOLUCAO {mrSisLin=0, mrBiCG_ILU};

enum enum_MODELO_CARGA {mcNaoImposto=-1, Icte=0, Scte, Zcte};

// IMPORTANTE: altera��es na enum abaixo dever�o ser feitas tamb�m na
//             macro 'NUM_MODULO_CALCULO' (acima) e na inicializa��o de
//             'codigo_programa[]' (TGeralC.cpp)
enum enum_MODULO_CALCULO {progINVALIDO=-1, progCURTO_CS=0, progCURTO_MA,
                          progEQV1, progEQV3, progESTAB, progESTIM1,
                          progESTIM3, progFR1, progFR3, progG1, progG3,
                          progH3, progNR1, progNR3, progPM, progYNODAL_ZCTE};

enum enum_TIPO_ARQUIVO {arqDADOS=0, arqLOG, arqLOG2, arqRESULT};

// IMPORTANTE: - N�O ALTERAR A ORDEM DOS TIPOS DE ATERRAMENTO ABAIXO!!!!
//               (eles est�o na ordem em que podem ser promovidos)
//             - altera��es na enum abaixo dever�o ser feitas tamb�m na
//               defini��o de 'codigo_ater[]', em TImpressao3.cpp
enum enum_TIPO_ATERR {atIsolado=0, atZ, atSolido};

// IMPORTANTE: altera��es na enum abaixo dever�o ser feitas tamb�m na
//             defini��o de 'codigo_tipo_barra[]', em TGeralC.cpp
enum enum_TIPO_BARRA {BAR_PQ=0, BAR_EXT, BAR_PQV, BAR_PV, BAR_SW, BAR_INT};

enum enum_TIPO_CARGA {tcSEM_CARGA=0, tcTODAS_YNODAL, tcINJECOES_EXT};

// Enum abaixo usada no Estimador
// IMPORTANTE: altera��es na enum dever�o ser feitas tamb�m em 'NUM_TIPO_EQPTO'
enum enum_TIPO_EQPTO {teBARRA=0, teTRECHO, teTRAFO};

enum enum_TIPO_LIGACAO_1 {CapSerie1=0, Reg2, Trafo2, Trafo3, TrafoEquiv1,
                          TrafoZZ1, Trecho1};

enum enum_TIPO_LIGACAO_3 {BancoDF=0, CapSerie3, Reg12, Reg32, Trafo12, Trafo32,
                          Trafo33, TrafoEquiv3, TrafoZZ3, Trecho3};

// Enum abaixo usada no Estimador
// IMPORTANTE: altera��es na enum dever�o ser feitas tamb�m em 'NUM_TIPO_MEDICAO'
enum enum_TIPO_MEDICAO {medMOD_V=0, medMOD_I, medP, medQ, medS};

// Enum abaixo usada no Estimador
enum enum_TIPO_MEDIDOR {medREAL=0, medVIRTUAL};

// Enum abaixo usada em NR1/NR3
enum enum_TIPO_MONTAGEM_JACOB {mjJACOB_FIXO=0, mjJACOB_NAO_FIXO};

// IMPORTANTE: altera��es na enum abaixo dever�o ser feitas tamb�m na
//             defini��o de 'codigo_tipo_motor[]' em TImpressaoPM.cpp
enum enum_TIPO_MOTOR {tmROTOR_GAIOLA=0, tmROTOR_BOBINADO};

// Enum usada em NR1/NR3
// O tipo de n� 'tnSW_INT' indica que se trata de n� de barra swing ou interna
enum enum_TIPO_NO {tnVR_VM=0, tnTHETA_TAP, tnPV_REF, tnPV_GER, tnSW_INT};

enum enum_TIPO_POTENCIA {tpPOTENCIA_ATIVA=0, tpPOTENCIA_REATIVA};

enum enum_TIPO_RELATORIO {trSEM_RELATORIO=0, trREL_RESUMIDO,
                          trREL_DETALHADO_1, trREL_DETALHADO_2};

// IMPORTANTE: altera��es na enum abaixo dever�o ser feitas tamb�m em:
//             - defini��o de 'codigo_tipo_12[]' em TImpressao3.cpp
//             - fun��o 'TTrafo12::TipoImplementadoNR3()' [EVENTUALMENTE]
enum enum_TIPO_TRAFO12 {AUTOTRAFO4=0, AUTOTRAFO6_1T, AUTOTRAFO6_2T, TRAFO4, TRAFO5};

// IMPORTANTE: altera��es na enum abaixo dever�o ser feitas tamb�m em:
//             - defini��o de 'codigo_lig_trafo[]' em TImpressao1.cpp
//             - fun��o 'TTrafo2::TipoImplementadoNR1()' [EVENTUALMENTE]
enum enum_TIPO_TRAFO2 {T2_AUTOTRAFO_DF=0, T2_AUTOTRAFO_Y, T2_DD, T2_DY, T2_YD,
                       T2_YY};

// IMPORTANTE: altera��es na enum abaixo dever�o ser feitas tamb�m em:
//             - defini��o de 'codigo_tipo_32[]' em TImpressao3.cpp
//             - fun��o 'TTrafo32::TipoImplementadoNR3()' [EVENTUALMENTE]
enum enum_TIPO_TRAFO32 {AUTOTRAFO_DF=0, AUTOTRAFO_Y, TRAFO_DD, TRAFO_DY1,
                        TRAFO_DY11, TRAFO_YD1, TRAFO_YD11, TRAFO_YY};

// IMPORTANTE: altera��es na enum abaixo dever�o ser feitas tamb�m na
//             defini��o de 'codigo_tipo_33[]', em TGeralC.cpp
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

public:  // M�todos
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
   bool flag_ater_comuns;             // Flag para considerar aterramentos ATZ comuns (true) ou n�o (false)
   bool flag_capac_LT;                // Flag para indicar inclus�o da capacit�ncia de linhas
   bool flag_imprime_bar_desc;        // Flag para imprimir (true) barras desconexas
   bool flag_inclui_CNLs;             // Flag para incluir (true) Cargas N�o-Lineares
   bool flag_inclui_mutuas;           // Flag para incluir (true) imped�ncias m�tuas
   bool flag_ypar_trafos;             // Flag para indicar inclus�o do ramo paralelo de trafos
   bool flag_TRAFO5_Como_Trafo3E;     // Flag para representar TRAFO5 com o modelo de Trafo3E (OpenDSS)
   bool flag_utiliza_bancoDF;         // Flag para utilizar (true) bancos em Delta-Fechado ou n�o (false)
   bool remonta_y_completa_delta_tap; // Flag para remontar [Ynodal] do zero quando h� altera��o de taps (s� NR1)
   bool usa_pi_equiv;                 // Flag para indicar uso do modelo pi-equivalente em linhas (em vez do modelo pi-nominal)
   bool usa_vdnrm;                    // Flag para indicar uso do Very Dishonest NR Method (NR1/NR3)
   bool todos_sup_barra_interna;      // Flag que indica se todos os suprimentos ter�o barra interna (true) ou n�o (false) (Gauss3/NR3, etc.)

   int  Programa1;             // Programa executado (principal) (cf. 'enum_PROGS' acima)
   int  Programa2;             // Programa executado (auxiliar - Estabilidade) (cf. 'enum_PROGS' acima)
   int  tipo_relatorio;        // Tipo de relat�rio (cf. enum_TIPO_RELATORIO acima)
   // int  metodo_resolucao;      // M�todo de resolu��o dos sistemas lineares (cf. 'enum_METODO_RESOLUCAO')
   int  criterio_proc_colunas; // Crit�rio para processamento de colunas (cf. 'enum_CRIT_SELEC_LINHAS_COLUNAS' em VTSisLin.h)
   int  num_barras_PQ;         // N�mero de barras PQ
   int  num_barras_EXT;        // N�mero de barras EXT
   int  num_barras_PQV;        // N�mero de barras PQV
   int  num_barras_PV;         // N�mero de barras PV
   int  num_barras_SW;         // N�mero de barras SW
   int  num_barras_INT;        // N�mero de barras INT
   int  num_barras_carga;      // N�mero de barras de carga (PQ + EXT + PQV)
   int  num_barras_geracao;    // N�mero de barras de gera��o (PV + SW + INT)
   int  num_chaves;            // N�mero de chaves (representadas por trechos de rede)
   int  num_lin_submatriz;     // N�mero de linhas / colunas da matriz Ycc
   int  num_tot_linhas;        // N�mero total de linhas / colunas da matriz Ynodal
   int  num_tot_pat;           // N�mero total de patamares
   int  num_pat_calc;          // N�mero de patamares j� calculados
   int  num_pat_OK;            // N�mero de patamares com converg�ncia OK
   int  num_iter_min;          // N�mero m�nimo de itera��es antes de bloquear ajuste de taps
   int  num_open_dados;        // Controle de abertura do arquivo de dados
   int  num_open_log;          // Controle de abertura do arquivo de log
   int  num_open_log2;         // Controle de abertura do arquivo de log (segundo arquivo)
   int  num_open_result;       // Controle de abertura do arquivo de resultados

   int  modelo_eqv; // modeloEQV_INDEF: a rede n�o cont�m elementos equivalentes
                    // modeloEQV1:      a rede cont�m elementos equivalentes
                    //                  gerados por Eqv1
                    // modeloEQV3:      a rede cont�m elementos equivalentes
                    //                  gerados por Eqv3
                    // cf. Fontes\Eqv\VTEqv.h

   double          frequencia_hz;           // Frequ�ncia da rede (Hz)
   double          Sbase;                   // Pot�ncia de base TRIF�SICA (MVA)
   double          Sbase_fase;              // Pot�ncia de base POR FASE  (MVA)
   double          toler_tap_pu;            // Toler�ncia para considerar 2 valores de tap iguais ou diferentes
   double          comp_chave_m;            // Comprimento do trecho que representa chave
   // double          reat_regul_DEMAIS_NR_pu; // Reat�ncia  de default para reguladores de tens�o (todos exceto AUTOTRAFO_DF) em NR1/NR3 (pu na base nominal)
   // complex<double> zreg_AUTODF_NR_pu;       // Imped�ncia de default para reguladores de tens�o AUTOTRAFO_DF em NR1/NR3 (pu na base nominal)
   complex<double> geracao_total;           // Pot�ncia total gerada (c�lculo mais recente) (kW, kVAr)
   complex<double> carga_total;             // Carga total (kW, kVAr)
   complex<double> perda_total_1;           // Perda total calculada atrav�s de (Ger_tot - Carga_tot) (kW, kVAr)
   complex<double> perda_total_2;           // Perda total calculada atrav�s de (ri2, xi2) (kW, kVAr)
   complex<double> perda_geradores;         // Perda total nos geradores (kW, kVAr)
   complex<double> perda_admit_reduc;       // Perda total nas admit�ncias para a refer�ncia (reduc. rede) (kW, kVAr)
   complex<double> perda_ypar_trafos;       // Perda total no ramo em paralelo de trafos (kW, kVAr)
   complex<double> energia_carga_mvah;      // Energia total de carga   (todos os patamares) (MWh, MVArh)
   complex<double> energia_geracao_mvah;    // Energia total de gera��o (todos os patamares) (MWh, MVArh)
   complex<double> energia_perdas_mvah;     // Energia total de perdas  (todos os patamares) (MWh, MVArh)
   complex<double> zchave_ohm;              // Imped�ncia para chaves fechadas

   AnsiString      Plataforma;                                       // 32 ou 64 bits
   AnsiString      Configuracao;                                     // Debug ou Release
   AnsiString      NomeRede;                                         // Nome do diret�rio da base de dados da rede
   AnsiString      CodigoBarraFicticia;                              // C�digo para barra  fict�cia (quando n�o h� nenhuma barra de carga)
   AnsiString      CodigoTrechoFicticio;                             // C�digo para trecho fict�cio (quando n�o h� nenhuma barra de carga)
   AnsiString      codigo_modelo_regu[NUM_MODELO_REGULADOR];         // C�digo dos modelos de regulador de tens�o
   AnsiString      codigo_programa[NUM_MODULO_CALCULO];              // C�digo dos m�dulos de c�lculo
   AnsiString      codigo_sub_modelo_regu[NUM_SUB_MODELO_REGULADOR]; // C�digo dos sub-modelos de regulador de tens�o
   AnsiString      codigo_tipo_33[NUM_TIPO_TRAFO33];                 // C�digo dos tipos de trafos de 3 enrolamentos (modelos equilibrados e desequilibrados)
   AnsiString      codigo_tipo_barra[NUM_TIPO_BARRA];                // C�digo dos tipos de barra
   char            codigo_tipo_medicao[NUM_TIPO_MEDICAO];            // C�digo dos tipos de medi��o

   // Chave para endere�os em MaxIteracao, NumIteracao, FlagConv e Precisao:
   // Estabilidade:     [0] - Processo iterativo do fluxo de pot�ncia (G1 ou NR1)
   //                   [1] - Loop de controles (tap de reg. ENERQCT e Qmax em barras PV - NR1)
   //                   [2] - Loop de c�lculo em 'CalculaRedeSimples()'
   //                   [3] - Loop do M�todo de Euler Modificado (integra��o eqs. dif.)
   //                   [4] - Loop do c�lculo de 'gama'
   // Estim1 / Estim3:  [0] - Processo iterativo do m�todo de Newton (estado da rede)
   //                   [1] - Com VDNM - "Very Dishonest Newton Method"
   // Gauss1 / Gauss3:  [0] - Processo iterativo �nico
   //                   [2] - Montagem de Ynodal
   // NR1 / NR3:        [0] - Processo iterativo Newton-Raphson
   //                   [1] - Loop de controles (tap de reg. DOIS_SENTIDOS/ENERQCT e Qmax em barras PV)
   //                   [2] - Montagem/fatora��o do Jacobiano (com ou sem VDNRM - "Very Dishonest Newton-Raphson Method")
   // PM:               [0] - Processo iterativo do fluxo de pot�ncia (G1 ou NR1)
   //                   [1] - Loop de controles (tap de reg. ENERQCT e Qmax em barras PV - NR1)
   //                   [2] - Loop de c�lculo em 'CalculaRedeSimples()'
   //                   [3] - Loop do M�todo de Euler Modificado (integra��o eqs. dif.)
   int    MaxIteracaoDefault;       // Para inicializa��o de 'MaxIteracao[]'
   int    MaxIteracao[DIM_ITERACOES];
   int    NumIteracao[DIM_ITERACOES];
   bool   FlagConv[DIM_ITERACOES];
   double Precisao[DIM_ITERACOES];  // Toler�ncia dos processos iterativos

   // Vari�veis utilizadas por Estabilidade / PartidaMotor
   int        num_instantes;   // N�mero de instantes do transit�rio
   int        num_bar_result;  // N�mero de barras   para exibi��o de resultados
   int        num_lig_result;  // N�mero de liga��es para exibi��o de resultados
   int        num_geradores;   // N�mero de unidades geradoras
   int        tempo_ms;        // Instante de c�lculo
   double     ctemp_min;       // Menor constante de tempo (s)
   AnsiString DescricaoEvento; // Descri��o dos eventos

   // Vari�veis utilizadas por ESTIM1 / ESTIM3
   bool   LimitaSigmas;             // Flag para controlar mapeamento de sigmas
   int    TipoBarraRef;             // Tipo da(s) barra(s) de refer�ncia (cf. 'enum_BARRA_REF' em VTEstimador.h)
   int    metodoEE;                 // M�todo de resolu��o da Estima��o de Estado (cf. 'enum_METODO_EE' em VTEstimador.h)
   int    offset_bar_ref;           // Offset da barra de refer�ncia
   int    OrdenacaoLinhasQR;        // Controle da ordena��o de linhas em QR_Givens() (cf. enum_CRIT_ORD_LINHAS_QR em VTSisLin.h)
   int    num_med_reais;            // N�mero de medidores reais
   int    num_med_desab;            // N�mero de medidores desabilitados
   int    num_var_est;              // N�mero de vari�veis de estado (Ns)
   double FatMultMedChave;          // Fator multiplicativo da medi��o de chaves (TMontaMedicaoChave.h)
   double DesvPadCargaAjustavel;    // Desvio-padr�o de medidor em barra com carga ajust�vel
   double DesvPadCargaNaoAjustavel; // Desvio-padr�o de medidor em barra com carga n�o ajust�vel
   double DesvPadMedV;              // Desvio-padr�o para medidores de m�dulo de V
   double desvio_max;               // N�mero m�ximo de desvios-padr�o para manter medidores ligados
   double nivel_confianca;          // N�vel de confian�a das medi��es (pu)
   double semi_intervalo_normal;    // Tamanho do semi-intervalo na distribui��o normal, em fun��o do n�vel de confian�a
   double SigmaMinNominal_pu;       // Valor m�nimo NOMINAL do desvio-padr�o para normaliza��o
   double SigmaMaxNominal_pu;       // Valor m�ximo NOMINAL do desvio-padr�o para normaliza��o
   double SigmaMinAtual_pu;         // Valor m�nimo ATUAL   do desvio-padr�o para normaliza��o
   double SigmaMaxAtual_pu;         // Valor m�ximo ATUAL   do desvio-padr�o para normaliza��o

   // Vari�veis utilizadas por NR1 / NR3
   bool       flag_qmax_pv;   // Flag para verificar (true) Qmax em barras PV
   AnsiString versao_NR1;
   AnsiString versao_NR3;
   int        num_lin_Jacob;  // N�mero total de linhas / colunas do Jacobiano (NR3)
   int        num_eqs_Ir;     // Barras PQ/EXT/PQV
   int        num_eqs_Im;     // Barras PQ/EXT/PQV
   int        num_eqs_P3f;    // Barras PV
   int        num_vars_Vr;    // Barras PQ/EXT/PQV
   int        num_vars_Vm;    // Barras PQ/EXT/PQV
   int        num_vars_Theta; // Barras PV
   int        num_vars_Tap;   // Barras PQV

   // Vari�veis utilizadas por PartidaMotor
   int num_motores; // N�mero de motores

   // Estruturas (todos os m�dulos)
   Filename *strFilename;

protected: // M�todos acessados via property
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
//fun��o global p/ criar objeto da classe VTGeralC
//---------------------------------------------------------------------------
VTGeralC * __fastcall NewObjGeralC(int    prog,
                                   int    tipo_relatorio,
                                   VTApl  *apl,
                                   VTData *data,
                                   bool   flag_imprime_bar_desc=false);

#endif
//---------------------------------------------------------------------------

