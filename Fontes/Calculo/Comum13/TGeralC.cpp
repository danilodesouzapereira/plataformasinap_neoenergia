//---------------------------------------------------------------------------
#pragma hdrstop
#include ".\TGeralC.h"
#include ".\VTTempoCPU.h"
#include ".\Estruturas.h"

#include "..\..\Apl\VTApl.h"
#include "..\..\Calculo\VTData.h"
#include "..\..\Complexo\Complexo.h"
#include "..\..\Diretorio\VTPath.h"
#include "..\..\Eqv\VTEqv.h"
#include "..\..\Estimador\VTEstimador.h"
#include "..\..\Geral\VTGeral.h"
#include "..\..\Geral\CfgFluxoH3\VTCfgFluxoH3.h"
#include "..\..\Patamar\VTpatamar.h"
#include "..\..\Patamar\VTpatamares.h"
#include "..\..\SisLin\VTSisLin.h"

#include "..\..\..\DLL_Inc\Funcao.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTGeralC * __fastcall NewObjGeralC(int    prog,
                                   int    tipo_relatorio,
                                   VTApl  *apl,
                                   VTData *data,
                                   bool   flag_imprime_bar_desc)
{
return(new TGeralC(prog, tipo_relatorio, apl, data, flag_imprime_bar_desc));
}

//---------------------------------------------------------------------------
__fastcall TGeralC::TGeralC(int    prog,
                            int    tipo_relatorio,
                            VTApl  *apl,
                            VTData *data,
                            bool   flag_imprime_bar_desc)
{
// Variáveis locais
double       toler_MEM;
AnsiString   cod_prog, dados, log, log2, result, string1;
VTCfgFluxoH3 *cfg_fluxoH3;
VTGeral      *geral     = (VTGeral *)     apl->GetObject(__classid(VTGeral));
VTPatamares  *patamares = (VTPatamares *) apl->GetObject(__classid(VTPatamares));
VTPath       *path      = (VTPath *)      apl->GetObject(__classid(VTPath));

// Determina plataforma (32 ou 64 bits)
#if _WIN64
   Plataforma = "Win64";
#elif _WIN32
   Plataforma = "Win32";
#else
   Plataforma = "*****";
#endif

// Determina configuração (Debug ou Release)
Configuracao = "Release";
#ifdef _DEBUG
   Configuracao = "Debug";
#endif

// Salva valores fornecidos na criação deste objeto
Programa1                   = prog;
this->tipo_relatorio        = tipo_relatorio;
this->apl                   = apl;
this->data                  = data;
this->flag_imprime_bar_desc = flag_imprime_bar_desc;

// Cria objetos
tCPU        = NewObjTempoCPU();
strFilename = new Filename;

// Inicialização
CodigoBarraFicticia    = AnsiString("FICTICIA").SubString(0, 15); // Garante tamanho máximo
CodigoTrechoFicticio   = AnsiString("Fictício").SubString(0, 15); // Garante tamanho máximo
ModeloCargaImposto     = mcNaoImposto;

// DEBUG
// ModeloCargaImposto = Zcte;  // Impõe modelo de carga

TipoAtualizacaoTensoes = atINIC_1_PU;
GravaYnodal            = false;
czero                  = complex<double>(0., 0.);
comp_chave_m           = 0.2;
zchave_ohm             = 0.001 * comp_chave_m * complex<double>(0., 0.4); // Cabo 336,4 MCM
NumMaxHarmonicas       = 0;
num_harmonicas         = 0;
vet_ordem_harmonicas   = NULL;
vet_duracao_pat_h      = NULL;
for(int n=0; n < DIM_ITERACOES; n++)
   {
   MaxIteracao[n] = 0;
   NumIteracao[n] = 0;
   Precisao[n]    = 0.;
   }

// Inicializa flags internos de controle (tipicamente, definições
// que não devem ser disponibilizadas aos usuários)
flag_inclui_mutuas           = true;
flag_TRAFO5_Como_Trafo3E     = true;
flag_utiliza_bancoDF         = true;
remonta_y_completa_delta_tap = true;

// Obtém e salva definições externas
flag_ypar_trafos = data->PerdaFerroEnabled;
if(data->RedeComCarga)
   {
   if(data->Cargas_Ynodal) TipoCarga = tcTODAS_YNODAL;
   else                    TipoCarga = tcINJECOES_EXT;
   }
else
   {
   TipoCarga = tcSEM_CARGA;
   }
num_tot_pat             = patamares->NumPatamar();
NomeRede                = ChangeFileExt(ExtractFileName(path->ArqRede), "");
Sbase                   = data->Sbase;
Sbase_fase              = data->Sbase / 3.;
toler_tap_pu            = 1.e-10;   // Problemas de convergência em NR1/DeltaYnodal para valores > 1e-7
frequencia_hz           = data->FrequenciaHz;
MaxIteracaoDefault      = data->MaxIteracao;
MaxIteracao[0]          = data->MaxIteracao;
Precisao[0]             = data->Precisao;
flag_ater_comuns        = data->AterramentosComuns;
flag_capac_LT           = data->CapacitanciaLinhas;
flag_inclui_CNLs        = data->IncluiCNL;
todos_sup_barra_interna = data->BarraInternaSuprimentos;
usa_pi_equiv            = data->UsaPiEquivalente;
usa_vdnrm               = data->UsaVDNRM;
// reat_regul_DEMAIS_NR_pu = 0.;    // Valor para todos os módulos exceto NR1/NR3
// zreg_AUTODF_NR_pu       = czero; // Valor para todos os módulos exceto NR1/NR3

// Inicializa variáveis que poderão ser alteradas pela execução
Programa2             = progINVALIDO;
// metodo_resolucao      = mrSisLin;
modelo_eqv            = modeloEQV_INDEF;
metodoEE              = meeQR_GIVENS;
criterio_proc_colunas = slMIN_NLIG;
num_barras_PQ         = 0;
num_barras_EXT        = 0;
num_barras_PQV        = 0;
num_barras_PV         = 0;
num_barras_SW         = 0;
num_barras_INT        = 0;
num_barras_carga      = 0;
num_barras_geracao    = 0;
num_chaves            = 0;
num_lin_submatriz     = 0;
num_tot_linhas        = 0;
num_pat_calc          = 0;
num_pat_OK            = 0;
num_iter_min          = 1000000;  // Desabilita bloqueio de taps
ZeraEnergia();
ZeraTotaisPatamar();

// Inicializa nome e controle dos arquivos
dados           = "Dados.txt";
log             = "Log.txt";
log2            = "Log2.txt";
result          = "Resultados.txt";
num_open_dados  = 0;
num_open_log    = 0;
num_open_log2   = 0;
num_open_result = 0;

// Inicializa vetor de código dos módulos de cálculo
// (manter coerência com 'enum_MODULO_CALCULO' em VTGeralC.h)
codigo_programa[0]  = "Curto-circ.CS";   codigo_programa[1]  = "Curto-circ.MA";
codigo_programa[2]  = "Eqv1";            codigo_programa[3]  = "Eqv3";  ;
codigo_programa[4]  = "Estabilidade";    codigo_programa[5]  = "Estimador1";
codigo_programa[6]  = "Estimador3";      codigo_programa[7]  = "F.Radial1";
codigo_programa[8]  = "F.Radial3";       codigo_programa[9]  = "Gauss1";
codigo_programa[10] = "Gauss3";          codigo_programa[11] = "Harmonicos3";
codigo_programa[12] = "NR1";             codigo_programa[13] = "NR3";
codigo_programa[14] = "Partida Motores"; codigo_programa[15] = "Ynodal+Zcte";

// Inicializa vetor de código dos tipos de trafos de 3 enrolamentos
// (manter coerência com 'enum_TIPO_TRAFO33' em VTGeralC.h)
codigo_tipo_33[0] = "DDY1   "; codigo_tipo_33[1] = "DDY11  ";
codigo_tipo_33[2] = "DY1Y1  "; codigo_tipo_33[3] = "DY1Y11 ";
codigo_tipo_33[4] = "DY11Y1 "; codigo_tipo_33[5] = "DY11Y11";
codigo_tipo_33[6] = "YYY    ";

// Inicializa vetor de código dos tipos de barra
// (manter coerência com 'enum_TIPO_BARRA' em VTGeralC.h)
codigo_tipo_barra[0] = "PQ   "; codigo_tipo_barra[1] = "G.Ext";
codigo_tipo_barra[2] = "PQV  "; codigo_tipo_barra[3] = "PV   ";
codigo_tipo_barra[4] = "Swing"; codigo_tipo_barra[5] = "G.Int";

// Inicializa vetor de código dos tipos de medição
// (manter coerência com 'enum_TIPO_MEDICAO' em VTGeralC.h)
codigo_tipo_medicao[0] = 'V'; codigo_tipo_medicao[1] = 'I';
codigo_tipo_medicao[2] = 'P'; codigo_tipo_medicao[3] = 'Q';
codigo_tipo_medicao[4] = 'S';

// Inicializa vetor de código dos modelos de regulador de tensão
// (manter coerência com 'enum_MODELO_REGULADOR' em VTData.h)
codigo_modelo_regu[0] = "INDEF   ";
codigo_modelo_regu[1] = "PADRÃO  ";
codigo_modelo_regu[2] = "ENERQCT1";
codigo_modelo_regu[3] = "ENERQCT2";
codigo_modelo_regu[4] = "ENERQCT3";
codigo_modelo_regu[5] = "2_SENTID";

// Inicializa vetor de código dos sub-modelos de regulador de tensão
// (só para modelo '2_SENTID') (manter coerência com
// 'enum_SUB_MODELO_REGULADOR' em VTData.h)
codigo_sub_modelo_regu[0] = "INDEF    ";
codigo_sub_modelo_regu[1] = "AUTO_BI  ";
codigo_sub_modelo_regu[2] = "AUTO_UNI ";
codigo_sub_modelo_regu[3] = "AUTO_FIXA";

// Inicializações específicas de cada módulo
switch(prog)
   {
   case progCURTO_CS:
      cod_prog = "CCS";
      // Impõe cargas de Zcte em Ynodal, quando elas forem consideradas
      if(! CargaNula) TipoCarga = tcTODAS_YNODAL;
      break;

   case progCURTO_MA:
      cod_prog                = "CMA";
      todos_sup_barra_interna = true;
      // Impõe cargas de Zcte em Ynodal, quando elas forem consideradas
      if(! CargaNula) TipoCarga = tcTODAS_YNODAL;
      break;

   case progEQV1:
   case progEQV3:
      if(prog == progEQV1) cod_prog = "EQV1";
      else                 cod_prog = "EQV3";
      flag_inclui_CNLs = false;
      break;

   case progESTAB:
      cod_prog        = "ESTAB";
      num_instantes   =  0;
      num_bar_result  =  0;
      num_lig_result  =  0;
      num_geradores   =  0;
      tempo_ms        = -1;
      ctemp_min       =  0.;
      usa_vdnrm       = true;
      DescricaoEvento = "Rede sem contingência e sem barras internas";
      Programa2       = progNR1;
      MaxIteracao[0]  = 50;                 // Em estudo (Gauss1 e NR1)
      MaxIteracao[1]  = 5;                  // Loop de controles (tap de reg. ENERQCT e Qmax PV)
      MaxIteracao[2]  = data->MaxIteracao;  // 'CalculaRedeSimples()'
      MaxIteracao[3]  = data->MaxIteracao;  // Loop do MEM
      MaxIteracao[4]  = data->MaxIteracao;  // Loop de cálculo de 'gama'
      toler_MEM       = 1.e-4;              // MEM - em estudo
      Precisao[2]     = data->Precisao;     // 'CalculaRedeSimples()'
      Precisao[3]     = toler_MEM;
      Precisao[4]     = 1.e-6;              // 'gama' - em estudo
      log             = "Resumido.txt";
      if(tipo_relatorio != trSEM_RELATORIO) this->flag_imprime_bar_desc = true;
      break;

   case progESTIM1:
   case progESTIM3:
      if(prog == progESTIM1) cod_prog = "ESTIM1";
	   else                   cod_prog = "ESTIM3";
      offset_bar_ref           = -1;
      TipoBarraRef             = brFIXOS_1THETA_0V; // Default (estimação de estado convencional)
      num_med_reais            =  0;
      num_med_desab            =  0;
      num_var_est              =  0;
      FatMultMedChave          =  1.;
      DesvPadCargaAjustavel    =  0.;
      DesvPadCargaNaoAjustavel =  0.;
      DesvPadMedV              =  0.;
      desvio_max               = 10.;
      MaxIteracao[0]           = 25;     // Em estudo
      MaxIteracao[1]           =  2;     // Número máximo de montagens da matriz [H] (com VDNM, em estudo)
      Precisao[0]              =  1.e-3; // Em estudo
      nivel_confianca          =  0.90;  // Em estudo
      usa_vdnrm                = true;
      OrdenacaoLinhasQR        = qrlMENOR_NUM_ELEM_FD;
      LimitaSigmas             = false;
      SigmaMinNominal_pu       =  1.e-3;  // Em estudo
      SigmaMaxNominal_pu       =  1.e-2;  // Em estudo
      SigmaMinAtual_pu         =  0.;
      SigmaMaxAtual_pu         =  0.;
      log                      = "Excel.txt";
      if     (IsDoubleZero(nivel_confianca - 0.90)) semi_intervalo_normal = 1.645;
      else if(IsDoubleZero(nivel_confianca - 0.95)) semi_intervalo_normal = 1.960;
      else if(IsDoubleZero(nivel_confianca - 0.99)) semi_intervalo_normal = 2.575;
      else                                          semi_intervalo_normal = 3.; // 99.74%
      break;

   case progFR1:
      cod_prog = "FR1";
      break;

   case progG1:
   case progG3:
      if(prog == progG1) cod_prog = "G1";
      else               cod_prog = "G3";
      num_iter_min   = 5;  // Em estudo
      MaxIteracao[2] = MaxIteracao[0];
      break;

   case progH3:
      cod_prog          = "H3";
      cfg_fluxoH3       = geral->CfgFluxoH3;
      flag_grava_ynodal = cfg_fluxoH3->GravaYnodal;
      log               = "Ynodal.txt";
      // Alocação inicial do vetor de ordens harmônicas
      NumMaxHarmonicas     = 100;  // Valor inicial (ajustado automaticamente)
      vet_ordem_harmonicas = new int [NumMaxHarmonicas];
      break;

   case progNR1:
   case progNR3:
      if(prog == progNR1) cod_prog = "NR1";
      else                cod_prog = "NR3";
      versao_NR1     = AnsiString("");
      versao_NR3     = AnsiString("");
      flag_qmax_pv   = true;
      MaxIteracao[1] = 10;                     // Número máximo de iterações de controle (tap de reg. DOIS_SENTIDOS/ENERQCT e Qmax PV)
      MaxIteracao[2] = data->MaxIteracaoVDNRM; // Número máximo de montagens do Jacobiano
      num_iter_min   = 3;                      // Habilita bloqueio de taps (< MaxIteracao[1] !!!)
      ZeraContadoresNR();
      // reat_regul_DEMAIS_NR_pu = 0.02; // pu na base nominal do regulador (todos exceto AUTOTRAFO_DF)
      // zreg_AUTODF_NR_pu       = complex<double>(0.0005, 0.0050); // pu na base nominal do regulador (só AUTOTRAFO_DF)
      break;

   case progPM:
      cod_prog        = "PM";
      num_instantes   =  0;
      num_bar_result  =  0;
      num_lig_result  =  0;
      num_motores     =  0;
      tempo_ms        = -1;
      DescricaoEvento = "";
      Programa2       = progYNODAL_ZCTE;
      MaxIteracao[1]  = 5;                  // Loop de controles (tap de reg. ENERQCT e Qmax PV)
      MaxIteracao[2]  = data->MaxIteracao;  // 'CalculaRedeSimples()'
      MaxIteracao[3]  = data->MaxIteracao;  // Loop do MEM
      toler_MEM       = 1.e-4;              // MEM - em estudo
      Precisao[2]     = data->Precisao;     // 'CalculaRedeSimples()'
      Precisao[3]     = toler_MEM;
      log             = "Resumido.txt";
      log2            = "Ixs_Cxs.txt";
      // Impõe cargas de Zcte em Ynodal, quando elas forem consideradas
      if(! CargaNula) TipoCarga = tcTODAS_YNODAL;
      break;

   default:
      return;  // Erro
   }

// Inicializa 'strFilename'
string1             = DirTmp() + "\\" + NomeRede + "_" + cod_prog + "_";
strFilename->dados  = string1 + dados;
strFilename->log    = string1 + log;
strFilename->log2   = string1 + log2;
strFilename->result = string1 + result;
}

//---------------------------------------------------------------------------
__fastcall TGeralC::~TGeralC(void)
   {
   // Libera memória
   delete[] vet_ordem_harmonicas;
   delete[] vet_duracao_pat_h;

   // Destroi objetos
   delete tCPU;
   delete strFilename;
   }

//---------------------------------------------------------------------------
FILE * __fastcall TGeralC::AbreArquivo(int codigo)
{
FILE *arq;

switch(codigo)
   {
   case arqDADOS:
      if(num_open_dados == 0) arq = fopen(strFilename->dados.c_str(), "w");
      else                    arq = fopen(strFilename->dados.c_str(), "a");
      if(arq != NULL) num_open_dados++;
      break;

   case arqLOG:
      if(num_open_log == 0) arq = fopen(strFilename->log.c_str(), "w");
      else                  arq = fopen(strFilename->log.c_str(), "a");
      if(arq != NULL) num_open_log++;
      break;

   case arqLOG2:
      if(num_open_log2 == 0) arq = fopen(strFilename->log2.c_str(), "w");
      else                   arq = fopen(strFilename->log2.c_str(), "a");
      if(arq != NULL) num_open_log2++;
      break;

   case arqRESULT:
      if(num_open_result == 0) arq = fopen(strFilename->result.c_str(), "w");
      else                     arq = fopen(strFilename->result.c_str(), "a");
      if(arq != NULL) num_open_result++;
      break;

   default:  // Erro
      arq = NULL;
      break;
   }

return(arq);
}

//---------------------------------------------------------------------------
void __fastcall TGeralC::CalculaPerdaTotal(void)
{
// perda_total_1 = FiltraPerdaAtivaNegativa(geracao_total - carga_total);
// perda_total_2 = FiltraPerdaAtivaNegativa(perda_total_2);

perda_total_1 = geracao_total - carga_total;
}

//---------------------------------------------------------------------------
void __fastcall TGeralC::DefineParametrosEstimador(void)
{
// Esta função serve aos módulos Estimador1 e Estimador3

// Método convencional: impõe não-utilização do VDNM
// Decomposição QR:     impõe não-limitação  de sigmas
if(metodoEE == meeCONVENCIONAL) usa_vdnrm    = false;
else                            LimitaSigmas = false;

// Iguala o número máximo de iterações do segundo contador ao número máximo
// de iterações do primeiro contador, se o VDNM não é usado
if(! usa_vdnrm) MaxIteracao[1] = MaxIteracao[0];
}

/*
//---------------------------------------------------------------------------
AnsiString __fastcall TGeralC::DirTmp(void)
{
// Implementação anterior (remove substring do path)

// Variáveis locais
int        comprimento, inicio, fim;
AnsiString string1, string2, str_inicio, str_fim;
VTPath     *path = (VTPath *) apl->GetObject(__classid(VTPath));

// Valor inicial de DirTmp
string1 = path->DirTmp();

// Versão Sinapsis: habilitar   return abaixo
// Versão HPS:      desabilitar return abaixo
// return(string1);

// Remove substring \{...}
str_inicio = AnsiString("\\{");
str_fim    = AnsiString("}");
inicio     = string1.AnsiPos(str_inicio);
fim        = string1.AnsiPos(str_fim);
if((inicio == 0) || (fim == 0)) return(string1);  // Não existe o substring
if(fim < inicio)                return(string1);  // String "estranho"
string2     = string1.SubString(1, (inicio - 1));
comprimento = string1.Length();
if(fim < comprimento)
   {
   string2 += AnsiString("\\") + string1.SubString((fim + 1), comprimento);
   }

// Retorna
return(string2);
}
*/

//---------------------------------------------------------------------------
AnsiString __fastcall TGeralC::DirTmp(void)
{
// Implementação nova (string montado nesta função)

// Variáveis locais
AnsiString string1;
VTPath     *path = (VTPath *) apl->GetObject(__classid(VTPath));

// Monta e retorna diretório Tmp completo
string1 = path->DirBase + "\\Tmp";
return(string1);
}

//---------------------------------------------------------------------------
double __fastcall TGeralC::DuracaoPatamar(int np)
   {
   // Monta vetor de duração dos patamares, se isso não foi feito antes
   if(vet_duracao_pat_h == NULL)
      {
      VTPatamar   *patamar;
      VTPatamares *patamares = (VTPatamares *) apl->GetObject(__classid(VTPatamares));
      vet_duracao_pat_h      = new double [num_tot_pat];
      for(int n=0; n < num_tot_pat; n++)
         {
         patamar              = patamares->Patamar[n];
         vet_duracao_pat_h[n] = patamar->Duracao;
         }
      }

   // Retorna duração do patamar solicitado
   if((np >= 0) || (np < num_tot_pat)) return(vet_duracao_pat_h[np]);
   else                                return(0.);
   }

/*
//---------------------------------------------------------------------------
complex<double> __fastcall TGeralC::FiltraPerdaAtivaNegativa(complex<double> perda)
{
// Variáveis locais
double real1;

real1 = perda.real();
if(real1 < 0.) real1 = 0.;

return(complex<double>(real1, perda.imag()));
}
*/

//---------------------------------------------------------------------------
complex<double> __fastcall TGeralC::FiltraZero(complex<double> valor,
                                               double          precisao)
{
if(Abs(valor, precisao) > 0.) return(valor);
else                          return(czero);
}

//---------------------------------------------------------------------------
void __fastcall TGeralC::HabilitaSoPrimeiroPatamar(void)
{
// Variáveis locais
VTPatamar   *patamar;
VTPatamares *patamares = (VTPatamares *) apl->GetObject(__classid(VTPatamares));

// Loop de patamares
for(int n=0; n < num_tot_pat; n++)
   {
   patamar = patamares->Patamar[n];
   if(n == 0) patamar->Enabled = true;
   else       patamar->Enabled = false;
   }
}

//---------------------------------------------------------------------------
bool __fastcall TGeralC::InsereOrdemHarmonica(int ordem)
{
// Variáveis locais
int jinic, jm1, ordem2;

// Proteção
if(ordem <= 0) return(false);  // Erro

// Verifica se ordem dada já existe
for(int nfreq=0; nfreq < num_harmonicas; nfreq++)
   {
   if(vet_ordem_harmonicas[nfreq] == ordem) return(true);  // Ordem já existe
   }

// Verifica capacidade do vetor de harmônicas, realocando memória se necessário
if(num_harmonicas >= NumMaxHarmonicas)
   {
   NumMaxHarmonicas += NumMaxHarmonicas;  // Dobra a capacidade
   int *vet_aux      = new int [NumMaxHarmonicas];
   for(int n=0; n < num_harmonicas; n++)
      {
      vet_aux[n] = vet_ordem_harmonicas[n];
      }
   delete[] vet_ordem_harmonicas;
   vet_ordem_harmonicas = vet_aux;
   }

// Adiciona ordem harmônica fornecida, mantendo a ordem crescente
jinic = num_harmonicas - 1;
jm1   = num_harmonicas;
num_harmonicas++;
for(int j=jinic; j >= 0; j--)
   {
   ordem2 = vet_ordem_harmonicas[j];
   if(ordem >= ordem2) break;
   vet_ordem_harmonicas[jm1] = ordem2;
   jm1                       = j;
   }
vet_ordem_harmonicas[jm1] = ordem;

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TGeralC::IsProgramaNR1(int indice)
{
if(indice == 1)
   {
   if(Programa1 == progNR1) return(true);
   }
else if(indice == 2)
   {
   if(Programa2 == progNR1) return(true);
   }

return(false);
}

//---------------------------------------------------------------------------
void __fastcall TGeralC::MaxIter(int programa,
                                 int *num_max1,
                                 int *num_max2)
{
// Analisa programa fornecido
switch(programa)
   {
   case progESTIM1:
   case progESTIM3:
      *num_max1 = NumIteracao[0];
      if(NumIteracao[1] > *num_max1) *num_max1 = NumIteracao[1];
      *num_max2 = MaxIteracao[0];
      if(MaxIteracao[1] > *num_max2) *num_max2 = MaxIteracao[1];
      break;

   case progG1:
   case progG3:
   case progCURTO_MA:
      *num_max1 = NumIteracao[0];
      if(NumIteracao[2] > *num_max1) *num_max1 = NumIteracao[2];
      *num_max2 = MaxIteracao[0];
      if(MaxIteracao[2] > *num_max2) *num_max2 = MaxIteracao[2];
      break;

   case progNR1:
   case progNR3:
      *num_max1 = NumIteracao[0];
      if(NumIteracao[1] > *num_max1) *num_max1 = NumIteracao[1];
      if(NumIteracao[2] > *num_max1) *num_max1 = NumIteracao[2];
      *num_max2 = MaxIteracao[0];
      if(MaxIteracao[1] > *num_max2) *num_max2 = MaxIteracao[1];
      if(MaxIteracao[2] > *num_max2) *num_max2 = MaxIteracao[2];
      break;

   default:  // Demais módulos
      *num_max1 = NumIteracao[0];
      *num_max2 = MaxIteracao[0];
      break;
   }
}

//---------------------------------------------------------------------------
int __fastcall TGeralC::OrdemHarmonica(int indice)
{
if((indice < 0) || (indice >= num_harmonicas)) return(-1);
else                                           return(vet_ordem_harmonicas[indice]);
}

//---------------------------------------------------------------------------
bool __fastcall TGeralC::PatamarHabilitado(int   np,
                                           TList *lisBATERIA)
{
// Variáveis locais
VTPatamar   *patamar;
VTPatamares *patamares;
TList       *lisPATAMAR;

// Verifica existência de baterias, caso no qual todos os patamares são
// considerados habilitados (para contabilizar corretamente a variação da
// energia armazenada nas baterias em cada patamar)
if(lisBATERIA != NULL)
   {
   if(lisBATERIA->Count > 0) return(true);
   }

// Não há baterias, verifica definição do usuário
patamares  = (VTPatamares *) apl->GetObject(__classid(VTPatamares));
lisPATAMAR = patamares->LisPatamar();
patamar    = (VTPatamar *) lisPATAMAR->Items[np];
return(patamar->Enabled);
}

//---------------------------------------------------------------------------
bool __fastcall TGeralC::PM_GetCargaEmYnodal(void)
   {
   return(tipo_carga == tcTODAS_YNODAL);
   }

//---------------------------------------------------------------------------
bool __fastcall TGeralC::PM_GetCargaInjecoesExt(void)
   {
   return(tipo_carga == tcINJECOES_EXT);
   }

//---------------------------------------------------------------------------
bool __fastcall TGeralC::PM_GetCargaNula(void)
   {
   return(tipo_carga == tcSEM_CARGA);
   }

//---------------------------------------------------------------------------
bool __fastcall TGeralC::PM_GetGravaYnodal(void)
{
return(flag_grava_ynodal);
}

//---------------------------------------------------------------------------
int __fastcall TGeralC::PM_GetModeloCargaImposto(void)
{
return(modelo_carga_imposto);
}

//---------------------------------------------------------------------------
int __fastcall TGeralC::PM_GetNumHarmonicas(void)
{
return(num_harmonicas);
}

//---------------------------------------------------------------------------
int __fastcall TGeralC::PM_GetTipoAtualizacaoTensoes(void)
{
return(tipo_atualizacao_tensoes);
}

//---------------------------------------------------------------------------
int __fastcall TGeralC::PM_GetTipoCarga(void)
{
return(tipo_carga);
}

//---------------------------------------------------------------------------
void __fastcall TGeralC::PM_SetGravaYnodal(bool flag)
{
flag_grava_ynodal = flag;
}

//---------------------------------------------------------------------------
void __fastcall TGeralC::PM_SetModeloCargaImposto(int modelo)
{
if     (modelo < 0)    modelo_carga_imposto = mcNaoImposto;
else if(modelo > Zcte) modelo_carga_imposto = Icte; // Default em caso de dado inválido
else                   modelo_carga_imposto = modelo;
}

//---------------------------------------------------------------------------
void __fastcall TGeralC::PM_SetTipoAtualizacaoTensoes(int tipo)
{
if     (tipo == atRECUPERA_ANTERIOR) tipo_atualizacao_tensoes =  atRECUPERA_ANTERIOR;
else if(tipo == atINIC_1_PU)         tipo_atualizacao_tensoes =  atINIC_1_PU;
else                                 tipo_atualizacao_tensoes =  atNAO_ATUALIZA; // Default
}

//---------------------------------------------------------------------------
void __fastcall TGeralC::PM_SetTipoCarga(int tipo)
{
// Verifica tipo fornecido
if((tipo != tcSEM_CARGA)     &&
   (tipo != tcTODAS_YNODAL)  &&
   (tipo != tcINJECOES_EXT)) return;  // Valor inválido (nada a fazer)

// Executa atribuição
tipo_carga = tipo;
}

//---------------------------------------------------------------------------
void __fastcall TGeralC::ZeraContadoresNR(void)
   {
   num_lin_Jacob  = 0;
   num_eqs_Ir     = 0;
   num_eqs_Im     = 0;
   num_eqs_P3f    = 0;
   num_vars_Vr    = 0;
   num_vars_Vm    = 0;
   num_vars_Theta = 0;
   num_vars_Tap   = 0;
   }

//---------------------------------------------------------------------------
void __fastcall TGeralC::ZeraEnergia(void)
   {
   energia_carga_mvah   = czero;
   energia_geracao_mvah = czero;
   energia_perdas_mvah  = czero;
   }

//---------------------------------------------------------------------------
void __fastcall TGeralC::ZeraTotaisPatamar(void)
   {
   geracao_total     = czero;
   carga_total       = czero;
   perda_total_1     = czero;
   perda_total_2     = czero;
   perda_geradores   = czero;
   perda_admit_reduc = czero;
   perda_ypar_trafos = czero;
   }

//---------------------------------------------------------------------------

