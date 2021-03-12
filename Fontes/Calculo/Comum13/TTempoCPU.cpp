//---------------------------------------------------------------------------
#pragma hdrstop
#include ".\TTempoCPU.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
// Função global para criar objeto da classe
//---------------------------------------------------------------------------
VTTempoCPU * NewObjTempoCPU(void)
   {
   return(new TTempoCPU());
   }

//---------------------------------------------------------------------------
__fastcall TTempoCPU::TTempoCPU(void)
   {
   // Variáveis locais
   strTempoCPU *ptr;

   // Define 'num_categorias'
   num_categorias = ttULTIMO + 1;

   // Aloca memória
   vetor = new strTempoCPU [num_categorias];

   // Inicializa dados
   for(int n=0; n < num_categorias; n++)
      {
      ptr               = vetor + n;
      ptr->flag_tempo   = false;
      ptr->num_chamadas = 0;
      ptr->ticks        = 0;
      ptr->nome_funcao  = "";
      }

   // Inicializa 'nome_funcao'
   // IMPORTANTE: manter coerência com 'enum_TEMPO_CPU' em VTTempoCPU.h
   // Nomes gerais
   vetor[ttATRIB_LIN_Y].nome_funcao         = "AtribuiLinhasY";
   vetor[ttATUALIZA].nome_funcao            = "Atualiza";
   vetor[ttBACK].nome_funcao                = "Back";
   vetor[ttCALC_COND_INIC].nome_funcao      = "CalculaCondiçãoInicial";
   vetor[ttCALC_REDE].nome_funcao           = "CalculaRede";
   vetor[ttCALC_TC].nome_funcao             = "CalculaTC";
   vetor[ttCORR_GERADORES].nome_funcao      = "CalculaCorrenteGeradores";
   vetor[ttCORR_PERDAS].nome_funcao         = "CalculaCorrentesPerdas";
   vetor[ttESTIM1].nome_funcao              = "Estimador1";
   vetor[ttESTIM3].nome_funcao              = "Estimador3";
   vetor[ttEXEC_ESTIM].nome_funcao          = "ExecutaEstimação";
   vetor[ttGAUSS_FALHA].nome_funcao         = "Falhas_Gauss";
   vetor[ttGAUSS_ITER].nome_funcao          = "Iterações_Gauss";
   vetor[ttGAUSS1].nome_funcao              = "Gauss1";
   vetor[ttGAUSS3].nome_funcao              = "Gauss3";
   vetor[ttINIC_J].nome_funcao              = "InicializaJacob";
   vetor[ttINIC_TENSOES].nome_funcao        = "InicializaTensões";
   vetor[ttINIC_Y].nome_funcao              = "InicializaYnodal";
   vetor[ttMONTA_JACOB].nome_funcao         = "MontaJacob";
   vetor[ttMONTA_MATRIZES].nome_funcao      = "MontaMatrizes";
   vetor[ttMONTA_REDE].nome_funcao          = "MontaRede";
   vetor[ttMONTA_Y].nome_funcao             = "MontaY";
   vetor[ttNR_FALHA].nome_funcao            = "Falhas_NR";
   vetor[ttNR_ITER].nome_funcao             = "Iterações_NR";
   vetor[ttNR1].nome_funcao                 = "NR1";
   vetor[ttNR3].nome_funcao                 = "NR3";
   vetor[ttORDENA_BARRAS].nome_funcao       = "OrdenaBarras";
   vetor[ttREMONTA_REDE].nome_funcao        = "RemontaRede";
   vetor[ttRESULTADOS].nome_funcao          = "Resultados";
   vetor[ttTRIANG].nome_funcao              = "Triang";
   vetor[ttVERIF_CONECT].nome_funcao        = "VerificaConectividade";
   // Curto-circuito (Componentes Simétricas)
   vetor[ttCS_ACERTA_TENSOES].nome_funcao   = "CS_AcertaTensões";
   vetor[ttCS_COLS_Z].nome_funcao           = "CS_ColunasZnodal";
   vetor[ttCS_COND_INIC].nome_funcao        = "CS_CondicaoInicial";
   vetor[ttCS_CURTO_3F_MAX].nome_funcao     = "CS_Curto_3F_Maximo";
   vetor[ttCS_CURTO_CIRC_1].nome_funcao     = "CS_CurtoCirc1";
   vetor[ttCS_EXECUTA].nome_funcao          = "CS_Executa";
   vetor[ttCS_RES_ICC_BDEF].nome_funcao     = "CS_ResBarraDefeito";
   vetor[ttCS_RES_ICC_BSUP].nome_funcao     = "CS_ResBarraSuprimento";
   vetor[ttCS_RES_ICC_LIGACAO].nome_funcao  = "CS_ResLigacao";
   vetor[ttCS_RES_VBARRA].nome_funcao       = "CS_ResTensaoBarras";
   vetor[ttCS_SALVA_RES].nome_funcao        = "CS_SalvaResCurto";
   // Eqv1 / Eqv3
   vetor[ttEQV_MONTA_LIS_BAR].nome_funcao   = "EQV_MontaListaBarras";
   vetor[ttEQV_MONTA_LIS_LIG].nome_funcao   = "EQV_MontaListaLigações";
   vetor[ttEQV_MONTA_REDE_ORIG].nome_funcao = "EQV_MontaRedeOriginal";
   vetor[ttEQV_REDUZ_REDE].nome_funcao      = "EQV_ReduzRede";
   vetor[ttEQV_RES_BARRA].nome_funcao       = "EQV_ResBarra";
   vetor[ttEQV_RES_LIGACAO].nome_funcao     = "EQV_ResLigação";
   // Estabilidade
   vetor[ttEST_CALC_REDE].nome_funcao       = "ES_CalculaRede_Estabilidade";
   vetor[ttEST_CRS].nome_funcao             = "ES_CalculaRedeSimples";
   vetor[ttEST_CRS_A].nome_funcao           = "ES_CalculaRedeSimples_A";
   vetor[ttEST_CRS_B].nome_funcao           = "ES_CalculaRedeSimples_B";
   vetor[ttEST_CRS_B1].nome_funcao          = "ES_CalculaRedeSimples_B1";
   vetor[ttEST_CRS_B2].nome_funcao          = "ES_CalculaRedeSimples_B2";
   vetor[ttEST_CRS_B3].nome_funcao          = "ES_CalculaRedeSimples_B3";
   vetor[ttEST_CRS_B4].nome_funcao          = "ES_CalculaRedeSimples_B4";
   vetor[ttEST_CRS_C].nome_funcao           = "ES_CalculaRedeSimples_C";
   vetor[ttEST_CRS_FALHA].nome_funcao       = "ES_CalculaRedeSimples_Falhas";
   vetor[ttEST_CRS_ITER].nome_funcao        = "ES_CalculaRedeSimples_Iter";
   vetor[ttEST_EXECUTA].nome_funcao         = "ES_ExecutaEstabilidade";
   vetor[ttEST_GAMA_FALHA].nome_funcao      = "ES_Falhas_Gama";
   vetor[ttEST_GAMA_ITER].nome_funcao       = "ES_Iterações_Gama";
   vetor[ttEST_MEM_FALHA].nome_funcao       = "ES_Falhas_MEM";
   vetor[ttEST_MEM_ITER].nome_funcao        = "ES_Iterações_MEM";
   // Matriz
   vetor[ttMAT_BACK].nome_funcao            = "Matriz_Back";
   vetor[ttMAT_INVERTE].nome_funcao         = "Matriz_Inverte";
   vetor[ttMAT_PROD_MAT].nome_funcao        = "Matriz_ProdutoMatriz";
   vetor[ttMAT_TRIANG].nome_funcao          = "Matriz_Triang";
   // PartidaMotor
   vetor[ttPM_CALC_REDE].nome_funcao        = "PM_CalculaRede_PartidaMotor";
   vetor[ttPM_CRS].nome_funcao              = "PM_CalculaRedeSimples";
   vetor[ttPM_EXECUTA].nome_funcao          = "PM_ExecutaPartidaMotor";
   vetor[ttPM_EXT_FALHA].nome_funcao        = "PM_Falhas_EXT";
   vetor[ttPM_EXT_ITER].nome_funcao         = "PM_Iterações_EXT";
   vetor[ttPM_MEM_FALHA].nome_funcao        = "PM_Falhas_MEM";
   vetor[ttPM_MEM_ITER].nome_funcao         = "PM_Iterações_MEM";
   // TData
   vetor[ttDATA01].nome_funcao              = "DATA01";
   vetor[ttDATA02].nome_funcao              = "DATA02";
   vetor[ttDATA03].nome_funcao              = "DATA03";
   vetor[ttDATA04].nome_funcao              = "DATA04";
   vetor[ttDATA05].nome_funcao              = "DATA05";
   vetor[ttDATA06].nome_funcao              = "DATA06";
   vetor[ttDATA07].nome_funcao              = "DATA07";
   vetor[ttDATA08].nome_funcao              = "DATA08";
   vetor[ttDATA09].nome_funcao              = "DATA09";
   vetor[ttDATA10].nome_funcao              = "DATA10";
   vetor[ttDATA11].nome_funcao              = "DATA11";
   vetor[ttDATA12].nome_funcao              = "DATA12";
   // SisLin
   vetor[ttSL_ATUAL_GR].nome_funcao         = "SL_AtualizaGrupos";
   vetor[ttSL_BACK].nome_funcao             = "SL_Back";
   vetor[ttSL_COPIA].nome_funcao            = "SL_Copia";
   vetor[ttSL_INS_ELEM_FD].nome_funcao      = "SL_InsereElementoFD";
   vetor[ttSL_INS_LIN].nome_funcao          = "SL_InsereLinha";
   vetor[ttSL_INVERTE].nome_funcao          = "SL_Inverte";
   vetor[ttSL_LOC_GR].nome_funcao           = "SL_LocalizaGrupo";
   vetor[ttSL_MONTA_GR].nome_funcao         = "SL_MontaGrupos";
   vetor[ttSL_PROD_MAT].nome_funcao         = "SL_ProdutoMatriz";
   vetor[ttSL_PROD_MAT_CHEIA].nome_funcao   = "SL_ProdutoMatrizCheia";
   vetor[ttSL_PROD_MAT_DIAG].nome_funcao    = "SL_ProdutoMatrizDiag";
   vetor[ttSL_PROD_MAT_SIMET].nome_funcao   = "SL_ProdutoMatrizSimetrico";
   vetor[ttSL_PROD_TRANSP_DIAG].nome_funcao = "SL_ProdutoTranspDiag";
   vetor[ttSL_PRODUTO_AtBA].nome_funcao     = "SL_ProdutoAtBA";
   vetor[ttSL_QR_BACK].nome_funcao          = "SL_QR_Back";
   vetor[ttSL_QR_FAST_GIVENS].nome_funcao   = "SL_QR_FastGivens";
   vetor[ttSL_QR_GIVENS].nome_funcao        = "SL_QR_Givens";
   vetor[ttSL_QR_HOUSEHOLDER].nome_funcao   = "SL_QR_Householder";
   vetor[ttSL_REM_LIN].nome_funcao          = "SL_RemoveLinha";
   vetor[ttSL_TRANSPOSTA].nome_funcao       = "SL_Transposta";
   vetor[ttSL_TRIANG].nome_funcao           = "SL_Triang";
   // Auxiliares
   vetor[ttZ01].nome_funcao                 = "Z01";
   vetor[ttZ02].nome_funcao                 = "Z02";
   vetor[ttZ03].nome_funcao                 = "Z03";
   vetor[ttZ04].nome_funcao                 = "Z04";
   vetor[ttZ05].nome_funcao                 = "Z05";
   vetor[ttZ06].nome_funcao                 = "Z06";
   vetor[ttZ07].nome_funcao                 = "Z07";
   vetor[ttZ08].nome_funcao                 = "Z08";
   vetor[ttZ09].nome_funcao                 = "Z09";
   vetor[ttZ10].nome_funcao                 = "Z10";
   vetor[ttZ11].nome_funcao                 = "Z11";
   vetor[ttZ12].nome_funcao                 = "Z12";
   }

//---------------------------------------------------------------------------
__fastcall TTempoCPU::~TTempoCPU(void)
   {
   // Libera memória
   delete[] vetor;
   }

//---------------------------------------------------------------------------
void __fastcall TTempoCPU::Acumula(int     indice,
                                   clock_t inicio,
                                   clock_t fim)
   {
   if((indice >= 0) && (indice < num_categorias))
      {
      if(fim < 0) fim = clock();  // Argumento não fornecido
      vetor[indice].flag_tempo = true;
      vetor[indice].num_chamadas++;
      vetor[indice].ticks += fim - inicio;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TTempoCPU::AcumulaC(int                    indice,
                                    unsigned long long int num_vezes)
   {
   if((indice >= 0) && (indice < num_categorias))
      {
      vetor[indice].num_chamadas += num_vezes;
      }
   }

//---------------------------------------------------------------------------
int __fastcall TTempoCPU::NumCategorias(void)
   {
   return(num_categorias);
   }

//---------------------------------------------------------------------------
void __fastcall TTempoCPU::Reinicia(void)
   {
   // Variáveis locais
   strTempoCPU *ptr;

   // Reinicia dados
   for(int n=0; n < num_categorias; n++)
      {
      ptr               = vetor + n;
      ptr->flag_tempo   = false;
      ptr->num_chamadas = 0;
      ptr->ticks        = 0;
      }
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TTempoCPU::Valores(int                    indice,
                                         bool                   *flag_tempo,
                                         unsigned long long int *num_chamadas,
                                         double                 *tempo_total_seg,
                                         double                 *tempo_medio_seg)
   {
   // Inicialização
   *flag_tempo      = false;
   *num_chamadas    = 0;
   *tempo_total_seg = 0.;
   *tempo_medio_seg = 0.;

   // Verifica índice fornecido
   if((indice < 0) || (indice >= num_categorias)) return("");  // Índice errado

   // Preenche valores
   *num_chamadas = vetor[indice].num_chamadas;
   if(vetor[indice].flag_tempo)
      {
      *flag_tempo      = true;
      *tempo_total_seg = vetor[indice].ticks / double(CLK_TCK);
      if(*num_chamadas > 0) *tempo_medio_seg = *tempo_total_seg /
                                                         double (*num_chamadas);
      }

   return(vetor[indice].nome_funcao);
   }

//---------------------------------------------------------------------------

