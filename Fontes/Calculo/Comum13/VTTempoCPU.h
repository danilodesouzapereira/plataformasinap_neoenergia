//---------------------------------------------------------------------------
#ifndef VTTempoCPUH
#define VTTempoCPUH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <time.h>

//---------------------------------------------------------------------------

// IMPORTANTE: manter coerência com a inicialização de 'vetor' no construtor de TTempoCPU
enum enum_TEMPO_CPU {ttATRIB_LIN_Y=0,       ttATUALIZA,          ttBACK,                ttCALC_COND_INIC,
                     ttCALC_REDE,           ttCALC_TC,           ttCORR_GERADORES,      ttCORR_PERDAS,
                     ttESTIM1,              ttESTIM3,            ttEXEC_ESTIM,          ttGAUSS_FALHA,
                     ttGAUSS_ITER,          ttGAUSS1,            ttGAUSS3,              ttINIC_J,
                     ttINIC_TENSOES,        ttINIC_Y,            ttMONTA_JACOB,         ttMONTA_MATRIZES,
                     ttMONTA_REDE,          ttMONTA_Y,           ttNR_FALHA,            ttNR_ITER,
                     ttNR1,                 ttNR3,               ttORDENA_BARRAS,       ttREMONTA_REDE,
                     ttRESULTADOS,          ttTRIANG,            ttVERIF_CONECT,
                     // Curto-circuito (Componentes Simétricas)
                     ttCS_ACERTA_TENSOES,   ttCS_COLS_Z,         ttCS_COND_INIC,        ttCS_CURTO_3F_MAX,
                     ttCS_CURTO_CIRC_1,     ttCS_EXECUTA,        ttCS_RES_ICC_BDEF,     ttCS_RES_ICC_BSUP,
                     ttCS_RES_ICC_LIGACAO,  ttCS_RES_VBARRA,     ttCS_SALVA_RES,
                     // Eqv1 / Eqv3
                     ttEQV_MONTA_LIS_BAR,   ttEQV_MONTA_LIS_LIG, ttEQV_MONTA_REDE_ORIG, ttEQV_REDUZ_REDE,
                     ttEQV_RES_BARRA,       ttEQV_RES_LIGACAO,
                     // Estabilidade
                     ttEST_CALC_REDE,       ttEST_CRS,           ttEST_CRS_A,           ttEST_CRS_B,
                     ttEST_CRS_B1,          ttEST_CRS_B2,        ttEST_CRS_B3,          ttEST_CRS_B4,
                     ttEST_CRS_C,           ttEST_CRS_FALHA,     ttEST_CRS_ITER,        ttEST_EXECUTA,
                     ttEST_GAMA_FALHA,      ttEST_GAMA_ITER,     ttEST_MEM_FALHA,       ttEST_MEM_ITER,
                     // Matriz
                     ttMAT_BACK,            ttMAT_INVERTE,       ttMAT_PROD_MAT,        ttMAT_TRIANG,
                     // PartidaMotor
                     ttPM_CALC_REDE,        ttPM_CRS,            ttPM_EXECUTA,          ttPM_EXT_FALHA,
                     ttPM_EXT_ITER,         ttPM_MEM_FALHA,      ttPM_MEM_ITER,
                     // TData
                     ttDATA01,              ttDATA02,            ttDATA03,              ttDATA04,
                     ttDATA05,              ttDATA06,            ttDATA07,              ttDATA08,
                     ttDATA09,              ttDATA10,            ttDATA11,              ttDATA12,
                     // SisLin
                     ttSL_ATUAL_GR,         ttSL_BACK,           ttSL_COPIA,            ttSL_INS_ELEM_FD,
                     ttSL_INS_LIN,          ttSL_INVERTE,        ttSL_LOC_GR,           ttSL_MONTA_GR,
                     ttSL_PROD_MAT,         ttSL_PROD_MAT_CHEIA, ttSL_PROD_MAT_DIAG,    ttSL_PROD_MAT_SIMET,
                     ttSL_PROD_TRANSP_DIAG, ttSL_PRODUTO_AtBA,   ttSL_QR_BACK,          ttSL_QR_FAST_GIVENS,
                     ttSL_QR_GIVENS,        ttSL_QR_HOUSEHOLDER, ttSL_REM_LIN,          ttSL_TRANSPOSTA,
                     ttSL_TRIANG,
                     // Auxiliares
                     ttZ01,                 ttZ02,               ttZ03,                 ttZ04,
                     ttZ05,                 ttZ06,               ttZ07,                 ttZ08,
                     ttZ09,                 ttZ10,               ttZ11,                 ttZ12,
                     // Último identificador (não pode haver nenhum depois deste!)
                     ttULTIMO};

//---------------------------------------------------------------------------
class VTTempoCPU : public TObject
   {
   public:  // Métodos
                         __fastcall  VTTempoCPU(void) {};
      virtual            __fastcall ~VTTempoCPU(void) {};
      virtual void       __fastcall  Acumula(int     indice,
                                             clock_t inicio,
                                             clock_t fim=-1) = 0;
      virtual void       __fastcall  AcumulaC(int                    indice,
                                              unsigned long long int num_vezes) = 0;
      virtual int        __fastcall  NumCategorias(void) = 0;
      virtual void       __fastcall  Reinicia(void) = 0;
      virtual AnsiString __fastcall  Valores(int                    indice,
                                             bool                   *flag_tempo,
                                             unsigned long long int *num_chamadas,
                                             double                 *tempo_total_seg,
                                             double                 *tempo_medio_seg) = 0;
   };

//---------------------------------------------------------------------------
// Função global para criar objeto da classe VTTempoCPU
//---------------------------------------------------------------------------
VTTempoCPU * NewObjTempoCPU(void);

#endif
//---------------------------------------------------------------------------

