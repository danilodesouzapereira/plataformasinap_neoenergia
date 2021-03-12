//---------------------------------------------------------------------------
#pragma hdrstop
#include <algorithm>
#include ".\TImpressao1.h"

#include "..\VTData.h"
#include "..\Comum13\Constantes_Calculo.h"
#include "..\Comum13\Estruturas.h"
#include "..\Comum13\TMedidor.h"
#include "..\Comum13\TTempoCPU.h"
#include "..\Comum13\TYnodalJ.h"
#include "..\Comum13\VTGeralC.h"
#include "..\Rede1\TBarra.h"
#include "..\Rede1\TCapSerie.h"
#include "..\Rede1\TCarga.h"
#include "..\Rede1\TCNL.h"
#include "..\Rede1\TFiltro.h"
#include "..\Rede1\TGrupoMutua.h"
#include "..\Rede1\TLigacaoC.h"
#include "..\Rede1\TMutua.h"
#include "..\Rede1\TReg2.h"
#include "..\Rede1\TSup.h"
#include "..\Rede1\TTrafo2.h"
#include "..\Rede1\TTrafo3.h"
#include "..\Rede1\TTrafoEquiv.h"
#include "..\Rede1\TTrafoZZ.h"
#include "..\Rede1\TTrecho.h"
#include "..\Rede1\VTRede1.h"

#include "..\..\Apl\VTApl.h"
#include "..\..\Complexo\Complexo.h"
#include "..\..\Eqv\VTEqv.h"
#include "..\..\Estimador\TBarraEstado.h"
#include "..\..\Estimador\TYnodalE.h"
#include "..\..\Estimador\VTEstimador.h"
#include "..\..\NR1\TBarraJ.h"
#include "..\..\RedeCC\ResCurto.h"
#include "..\..\SisLin\VTSisLin.h"

#include "..\..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
TImpressao1 * __fastcall NewObjImpressao1(VTRede1  *rede1,
                                          VTGeralC *geralC)
   {
   return(new TImpressao1(rede1, geralC));
   }

//---------------------------------------------------------------------------
// Função global para ordenação de barras
static int __fastcall EscolheBarraCodigo(void *obj1,
                                         void *obj2)
   {
   // Variáveis locais
   TBarra *bar1, *bar2;

   // Primeiro critério: ordem crescente do código da barra
   bar1 = (TBarra *) obj1;
   bar2 = (TBarra *) obj2;
   return(bar1->codigo.AnsiCompareIC(bar2->codigo));
   }

//---------------------------------------------------------------------------
// Função global para ordenação de trafos
static int __fastcall EscolheTrafo2(void *obj1,
                                    void *obj2)
   {
   // Variáveis locais
   int        num1;
   AnsiString cod_bar1, cod_bar2;
   TBarra     *bar1, *bar2;
   TTrafo2    *trafo1, *trafo2;

   // Primeiro critério: ordem crescente do código da barra inicial
   trafo1   = (TTrafo2 *) obj1;
   trafo2   = (TTrafo2 *) obj2;
   cod_bar1 = trafo1->bar1->codigo;
   cod_bar2 = trafo2->bar1->codigo;
   num1     = cod_bar1.AnsiCompareIC(cod_bar2);
   if     (num1 < 0) return(-1);
   else if(num1 > 0) return( 1);

   // Segundo critério: ordem crescente do código da barra terminal
   cod_bar1 = trafo1->bar2->codigo;
   cod_bar2 = trafo2->bar2->codigo;
   num1     = cod_bar1.AnsiCompareIC(cod_bar2);
   if     (num1 < 0) return(-1);
   else if(num1 > 0) return( 1);

   // Terceiro critério: ordem crescente do código do trafo
   return(trafo1->codigo.AnsiCompareIC(trafo2->codigo));
   }

//---------------------------------------------------------------------------
// Função global para ordenação de trechos
static int __fastcall EscolheTrecho(void *obj1,
                                    void *obj2)
   {
   // Variáveis locais
   int        num1;
   AnsiString cod_bar1, cod_bar2;
   TBarra     *bar1, *bar2;
   TTrecho    *trec1, *trec2;

   // Obtém trechos
   trec1 = (TTrecho *) obj1;
   trec2 = (TTrecho *) obj2;

   // Primeiro critério: ordem crescente do código da barra inicial
   bar1 = trec1->bar1;
   bar2 = trec2->bar1;
   if(trec1->bar2->codigo.AnsiCompareIC(bar1->codigo) < 0) bar1 = trec1->bar2;
   if(trec2->bar2->codigo.AnsiCompareIC(bar2->codigo) < 0) bar2 = trec2->bar2;
   num1 = bar1->codigo.AnsiCompareIC(bar2->codigo);
   if     (num1 < 0) return(-1);
   else if(num1 > 0) return( 1);

   // Segundo critério: ordem crescente do código da barra final
   if(bar1 == trec1->bar1) bar1 = trec1->bar2;
   else                    bar1 = trec1->bar1;
   if(bar2 == trec2->bar1) bar2 = trec2->bar2;
   else                    bar2 = trec2->bar1;
   num1 = bar1->codigo.AnsiCompareIC(bar2->codigo);
   if     (num1 < 0) return(-1);
   else if(num1 > 0) return( 1);

   // Terceiro critério: ordem crescente do código do trecho
   return(trec1->codigo.AnsiCompareIC(trec2->codigo));
   }

//---------------------------------------------------------------------------
__fastcall TImpressao1::TImpressao1(VTRede1  *rede1,
                                    VTGeralC *geralC)
                       :TImpressao(geralC)
   {
   // Salva ponteiros para objetos externos
   this->rede1  = rede1;
   this->geralC = geralC;

   // Inicializa variáveis locais
   lisTRAFO2_ORD = new TList;

   // Manter coerência com 'enumCU' em VTCurtoAux.h
   codigo_tipo_curto[0] = "Trifásico";  codigo_tipo_curto[1] = "Dupla fase";
   codigo_tipo_curto[2] = "Fase-terra"; codigo_tipo_curto[3] = "Fase-terraZ";
   codigo_tipo_curto[4] = "Dupla-faseT";

   // (Manter coerência com 'enum_TIPO_TRAFO2' em VTGeralC.h)
   codigo_lig_trafo[0] = "AUD"; codigo_lig_trafo[1] = "AUY";
   codigo_lig_trafo[2] = "DD "; codigo_lig_trafo[3] = "DY ";
   codigo_lig_trafo[4] = "YD "; codigo_lig_trafo[5] = "YY ";
   }

//---------------------------------------------------------------------------
__fastcall TImpressao1::~TImpressao1(void)
   {
   delete lisTRAFO2_ORD; // Os elementos desta lista são os mesmos de 'rede1->lisTRAFO2'
   }

//---------------------------------------------------------------------------
double __fastcall TImpressao1::CalculaFatorPotencia(bool       tipo_carga,
                                                    double     p,
                                                    double     q,
                                                    AnsiString *cod_nat)
   {
   double abs_p, abs_q, fpot;

   // Inicialização
   fpot     = 0.;
   *cod_nat = "    ";

   abs_p = fabs(p);
   abs_q = fabs(q);
   if(abs_q > 1.e-5)  // Q != 0
      {
      if(abs_p > 1.e-5) fpot = cos(atan(q/p));  // P != 0
      if(tipo_carga)  // Barra de carga
         {
         if(q > 0.) *cod_nat = "Ind.";
         else       *cod_nat = "Cap.";
         }
      else  // Barra de geração
         {
         if(q > 0.) *cod_nat = "Cap.";
         else       *cod_nat = "Ind.";
         }
      }
   else  // Q == 0
      {
      if(abs_p > 1.e-5) fpot = 1.;  // P != 0
      }

   return(fpot);
   }

//---------------------------------------------------------------------------
bool __fastcall TImpressao1::FalhaRedundancia(int np)
   {
   // Variáveis locais
   FILE *arq;

   // Abre arquivo
   arq = geralC->AbreArquivo(arqRESULT);
   if(arq == NULL) return(false);
   fprintf(arq, "\n\n *** ATENÇÃO - REDUNDÂNCIA MÍNIMA NÃO "
                "ALCANÇADA NO PATAMAR %d "
                "(var. est.: %d / var. med.: %d) ***",
           (np+1), geralC->num_var_est, rede1->lisMED->Count);
   fclose(arq);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TImpressao1::ImprimeBarrasDesconexas(TList *lisBAR_DESC,
                                                     TList *lisBAR_CONEXAS)
{
int    kprint, num_barras_linha, num_print;
FILE   *arq;
TBarra *bar1;

// Abre arquivo
arq = geralC->AbreArquivo(arqRESULT);
if(arq == NULL) return(false);

// Inicialização
num_barras_linha = 8;

// Imprime barras desconexas
if(lisBAR_DESC == NULL)
   {
   fprintf(arq, "\n *** Não há barras desconexas ***");
   }
else
   {
   if     (lisBAR_DESC->Count == 0) fprintf(arq, "\n *** Não há barras desconexas ***");
   else if(lisBAR_DESC->Count == 1) fprintf(arq, "\n *** 1 barra desconexa ***");
   else                             fprintf(arq, "\n *** %d barras desconexas ***",
                                            lisBAR_DESC->Count);
   fprintf(arq, "\n");
   kprint = 100;
   for(int nb=0; nb < lisBAR_DESC->Count; nb++)
      {
      bar1 = (TBarra *) lisBAR_DESC->Items[nb];
      kprint++;
      if(kprint > num_barras_linha)
         {
         fprintf(arq, "\n");
         kprint = 1;
         }
      fprintf(arq, " %-15s", bar1->codigo.c_str());
      }
   } // if(lisBAR_DESC)

// Imprime barras conexas
if(lisBAR_CONEXAS != NULL)
   {
   fprintf(arq, "\n");
   num_print = lisBAR_CONEXAS->Count;
   if     (num_print == 0)   fprintf(arq, "\n *** Não há barras conexas ***");
   else if(num_print == 1)   fprintf(arq, "\n *** 1 barra conexa ***");
   else if(num_print <= 100) fprintf(arq, "\n *** %d barras conexas ***", num_print);
   else
      {
      num_print = 100;
      fprintf(arq, "\n *** Primeiras %d de %d barras conexas ***",
              num_print, lisBAR_CONEXAS->Count);
      }
   if(num_print > 0)
      {
      fprintf(arq, "\n");
      kprint = 100;
      for(int nb=0; nb < num_print; nb++)
         {
         bar1 = (TBarra *) lisBAR_CONEXAS->Items[nb];
         kprint++;
         if(kprint > num_barras_linha)
            {
            kprint = 1;
            fprintf(arq, "\n");
            }
         fprintf(arq, " %-15s", bar1->codigo.c_str());
         }
      } // if(num_print)
   } // if(lisBAR_CONEXAS)

// Fecha arquivo
fclose(arq);

// Finalização
ImprimeSeparador(arqRESULT, 1, 128);

// Retorna
return(true);
}

/*
//---------------------------------------------------------------------------
void __fastcall TImpressao1::ImprimeCargas(void)
{

// Função apenas para debug - 2012_05_24

complex<double> stot;
FILE            *arq;
TBarra          *bar1;
TCarga          *carg1;


// Abre arquivo
arq = AbreArquivo(arqDADOS);
if(arq == NULL) return;

fprintf(arq, "\n\n G1 - Número de cargas: %5d\n", rede1->lisCARGA->Count);
for(int nc=0; nc < rede1->lisCARGA->Count; nc++)
   {
   carg1 = (TCarga *) rede1->lisCARGA->Items[nc];
   bar1  = carg1->barra;
   stot  = (bar1->vet_carga[Icte] + bar1->vet_carga[Scte] + bar1->vet_carga[Zcte]) *
           strGerais->Sbase;
   fprintf(arq, "\n %15s   %20.10f %20.10f", bar1->codigo.c_str(), stot.real(), stot.imag());
   }
fclose(arq);
}
*/

//---------------------------------------------------------------------------
bool __fastcall TImpressao1::ImprimeDados(bool  flag_imprime_tudo,
                                          int   np,
                                          TList *lisBAR_J,
                                          TList *lisBAR_ESTADO,
                                          TList *lisBAR_REF)
{
// Variáveis locais
bool imprimiu, teste1, teste2;
int  num_vezes;

// Inicialização
num_vezes = 0;

// Imprime dados, de acordo com o tipo de relatório
if(geralC->tipo_relatorio >= trREL_RESUMIDO)
   {
   imprimiu = ImprimeDados1(lisBAR_REF);  // Imprime dados gerais
   if(imprimiu) num_vezes++;
   teste1 = (geralC->Programa1 != progESTIM1) &&
            (geralC->tipo_relatorio >= trREL_DETALHADO_1);
   teste2 = (geralC->Programa1 == progESTIM1) &&
            (geralC->tipo_relatorio >= trREL_DETALHADO_2);
   if(teste1 || teste2)  // Imprime dados completos da rede
      {
      imprimiu = ImprimeDados2(flag_imprime_tudo, np, lisBAR_J, lisBAR_ESTADO);
      if(imprimiu) num_vezes++;
      }
   if(geralC->Programa1 == progESTIM1)
      {
      imprimiu = ImprimeDadosESTIM1(np, lisBAR_ESTADO);
      if(imprimiu) num_vezes++;
      }
   }

// Imprime separador de patamares
imprimiu = false;
if(num_vezes > 0)
   {
   imprimiu = true;
   ImprimeSeparador(arqDADOS, 1, 148);
   }

return(imprimiu);
}

//---------------------------------------------------------------------------
bool __fastcall TImpressao1::ImprimeDados1(TList *lisBAR_REF)
{
// Local variables
bool       teste;
int        num_lig_mutua, tempo_ms;
double     rel_dp;
AnsiString string0, string1, string2, string3, string4;
FILE       *arq;
TBarra     *bar1;
TTrecho    *trec1;

// Verifica necessidade de imprimir resumo
if(geralC->num_open_dados > 0) return(false);  // Não imprimiu nada

// Abre arquivo
arq = geralC->AbreArquivo(arqDADOS);
if(arq == NULL) return(false);

// Conta ligações com mútua
num_lig_mutua = 0;
for(int nl=0; nl < rede1->lisTRECHO->Count; nl++)
   {
   trec1 = (TTrecho *) rede1->lisTRECHO->Items[nl];
   if(trec1->grupo_mutua >= 0) num_lig_mutua++;
   }

// Prints general data
string0 = geralC->codigo_programa[geralC->Programa1];
if(geralC->Programa1 != progPM)
   {
   if(geralC->IsProgramaNR1(1)) string0 += " V" + geralC->versao_NR1;
   }
if     (geralC->CargaNula)        string1 = "Sem carga";
else if(geralC->CargaEmYnodal)    string1 = "Todas em Ynodal";
else if(geralC->CargaInjecoesExt) string1 = "Injeções externas";
else                              string1 = "";
if(geralC->flag_capac_LT)         string2 = "Sim";
else                              string2 = "Não";
if(geralC->usa_pi_equiv)          string3 = "Sim";
else                              string3 = "Não";
if(geralC->flag_ypar_trafos)      string4 = "Sim";
else                              string4 = "Não";
fprintf(arq,   "\n *** Dados Gerais ***"
             "\n\n Programa:                                                   %15s"
               "\n Plataforma:                                                           %5s"
               "\n Configuração:                                                       %7s",
        string0.c_str(), geralC->Plataforma.c_str(), geralC->Configuracao.c_str());
if((geralC->Programa1 == progESTAB) || (geralC->Programa1 == progPM))
   {
   if(geralC->IsProgramaNR1(2))
      {
      string0 = geralC->codigo_programa[geralC->Programa2] +
                " V" + geralC->versao_NR1;
      }
   else
      {
      string0 = geralC->codigo_programa[geralC->Programa2];
      }
   fprintf(arq, "\n Programa auxiliar:                                          %15s",
           string0.c_str());
   }
fprintf(arq, "\n Nome da rede:                           %35s",
        geralC->NomeRede.c_str());
if((geralC->Programa1 == progG1) || (geralC->Programa1 == progESTIM1) ||
   (geralC->Programa1 == progFR1))
   {
   fprintf(arq, "\n Número máximo de iterações:                                          %6d",
           geralC->MaxIteracao[0]);
   }
else if(geralC->IsProgramaNR1(1))
   {
   fprintf(arq, "\n Número máximo de iterações - NR:                                   %8d"
					 "\n                            - ajuste de Tap/Qmax:                   %8d"
                "\n                  montagens/fatorações do Jacobiano:                %8d",
           geralC->MaxIteracao[0], geralC->MaxIteracao[1], geralC->MaxIteracao[2]);
   }
else if(geralC->Programa1 == progESTAB)
   {
   if(geralC->IsProgramaNR1(2))
      {
      fprintf(arq, "\n Número máximo de iterações NR:                                     %8d"
						 "\n                            para ajuste de Tap/Qmax:                %8d"
                   "\n                  montagens do Jacobiano:                           %8d",
              geralC->MaxIteracao[0], geralC->MaxIteracao[1],
              geralC->MaxIteracao[2]);
      }
   else
      {
      fprintf(arq, "\n Número máximo de iterações Gauss:                                    %6d",
              geralC->MaxIteracao[0]);
      }
   }
else if(geralC->Programa1 == progPM)
   {
   fprintf(arq, "\n Número máximo de iterações:                                          %6d",
           geralC->MaxIteracao[0]);
   }
fprintf(arq, "\n Tolerância (pu):                                                     %6.0e"
             "\n Potência de base (MVA):                                                %4.0f"
             "\n Representação da carga:                           %25s"
             "\n Capacitância de linhas:                                                 %3s"
             "\n Modelo pi-equivalente para linhas:                                      %3s"
             "\n Ramo paralelo de trafos:                                                %3s",
        geralC->Precisao[0], geralC->Sbase, string1.c_str(), string2.c_str(),
        string3.c_str(), string4.c_str());
if(geralC->IsProgramaNR1(1) ||
   ((geralC->Programa1 == progESTAB) && geralC->IsProgramaNR1(2)))
   {
   if(geralC->remonta_y_completa_delta_tap) string1 = "Sim";
   else                                     string1 = "Não";
   if(geralC->usa_vdnrm)                    string2 = "Sim";
   else                                     string2 = "Não";
   fprintf(arq, "\n Remonta [Y] completa quando há ajuste de tap:                           %3s"
                "\n Utiliza VDNRM:                                                          %3s",
           string1.c_str(), string2.c_str());
   }
if(geralC->modelo_eqv != modeloEQV_INDEF)
   {
   fprintf(arq, "\n Modelo da rede equivalente:                                               %1d",
           geralC->modelo_eqv);
   }

fprintf(arq, "\n\n Número de barras %-5s                                               %6d"
               "\n                  %-5s                                               %6d"
               "\n                  %-5s                                               %6d"
               "\n                  %-5s                                               %6d"
               "\n                  %-5s                                               %6d"
               "\n                  %-5s                                               %6d"
               "\n                  Total                                               %6d"
             "\n\n Número de trechos sem mútua:                                         %6d"
               "\n Número de grupos de mútuas:                                          %6d"
               "\n Número de trechos com mútuas:                                        %6d"
               "\n Número total de trechos:                                             %6d"
             "\n\n Número de chaves:                                                    %6d"
             "\n\n Número de capacitores série:                                         %6d"
             "\n\n Número de trafos de 2 enrolamentos:                                  %6d"
               "\n                     3 enrolamentos:                                  %6d"
               "\n                  Equivalentes:                                       %6d"
             "\n\n Número de reguladores de tensão:                                     %6d"
             "\n\n Número de cargas não-lineares:                                       %6d",
        geralC->codigo_tipo_barra[BAR_PQ].c_str(),  geralC->num_barras_PQ,
        geralC->codigo_tipo_barra[BAR_EXT].c_str(), geralC->num_barras_EXT,
        geralC->codigo_tipo_barra[BAR_PQV].c_str(), geralC->num_barras_PQV,
        geralC->codigo_tipo_barra[BAR_PV].c_str(),  geralC->num_barras_PV,
        geralC->codigo_tipo_barra[BAR_SW].c_str(),  geralC->num_barras_SW,
        geralC->codigo_tipo_barra[BAR_INT].c_str(), geralC->num_barras_INT,
        rede1->lisBAR->Count, (rede1->lisTRECHO->Count-num_lig_mutua),
        rede1->lisGRUPO_MUTUA->Count, num_lig_mutua, rede1->lisTRECHO->Count,
        geralC->num_chaves, rede1->lisCAPS->Count, rede1->lisTRAFO2->Count,
        rede1->lisTRAFO3->Count, rede1->lisTRAFOEQUIV->Count,
        rede1->lisREG2->Count, rede1->lisCNL->Count);
if(geralC->Programa1 == progCURTO_CS)
   {
   // if(strGerais->flag_prefalta) str1 = "Sim";
   // else                         str1 = "Não";
   // fprintf(arq, "\n\n Cálculo de curto-circuito com pré-falta:                                %3s"
   //                "\n Número de filtros:                                                   %6d"
   //                "\n Número de transformadores ZZ:                                        %6d",
   //         str1.c_str(), rede1->lisFILTRO->Count, rede1->lisTRAFOZZ->Count);
   fprintf(arq, "\n\n Cálculo de curto-circuito:"
                  "\n    Número de filtros:                                                %6d"
                  "\n    Número de transformadores ZZ:                                     %6d",
           rede1->lisFILTRO->Count, rede1->lisTRAFOZZ->Count);
   }
else if((geralC->Programa1 == progESTAB) || (geralC->Programa1 == progPM))
   {
   if(geralC->tempo_ms >= 0) tempo_ms = geralC->tempo_ms;
   else                      tempo_ms = 0;
   fprintf(arq, "\n\n Evento:                       %45s"
                  "\n Instante (ms):                                                   %10d",
           geralC->DescricaoEvento.c_str(), tempo_ms);
   }
if(geralC->Programa1 == progESTIM1)
   {
   if(geralC->usa_vdnrm)                   string0 = "Sim";
   else                                    string0 = "Não";
   if(geralC->metodoEE == meeCONVENCIONAL) string1 = "linhas: ";
   else                                    string1 = "colunas:";
   if(geralC->LimitaSigmas)                string2 = "Sim";
   else                                    string2 = "Não";
   fprintf(arq, "\n\n Número de medidores reais habilitados:                               %6d"
                  "\n                           desabilitados (sigma elevado):             %6d"
                  "\n        total de medidores reais:                                     %6d"
                  "\n                           virtuais:                                  %6d"
                  "\n        total de medidores (Nm):                                      %6d"
                  "\n              de variáveis de estado (Ns):                            %6d"
                  "\n Método de resolução:                                         %14s"
                  "\n Utiliza VDNM:                                                           %3s",
           (geralC->num_med_reais - geralC->num_med_desab),
            geralC->num_med_desab, geralC->num_med_reais,
           (rede1->lisMED->Count - geralC->num_med_reais),
           rede1->lisMED->Count, geralC->num_var_est,
           codigo_metodo_EE[geralC->metodoEE].c_str(), string0.c_str());
   if(geralC->usa_vdnrm)
      {
      fprintf(arq, "\n    Número máximo de montagens de [H]:                                %6d",
              geralC->MaxIteracao[1]);
      }
   rel_dp = geralC->SigmaMaxAtual_pu / geralC->SigmaMinAtual_pu;
   fprintf(arq, "\n Critério para ordenação de %8s                       %17s"
                "\n Critério para ordenação de linhas (QR):                 %19s"
                "\n Limitação de valores de desvio padrão:                                  %3s"
                "\n Menor desvio padrão (pu):                                     %13.6e"
                "\n Maior desvio padrão (pu):                                     %13.6e"
                "\n Relação dp_max / dp_min:                                      %13.6e"
                "\n Fator multiplicativo da medição de chaves (pu):                     %7.4f"
                "\n Nível de confiança das medições:                                     %6.4f"
                "\n Tipo de referência:                                         %15s",
           string1.c_str(),
           codigo_proc_linhas_colunas[geralC->criterio_proc_colunas].c_str(),
           codigo_proc_linhas_qr[geralC->OrdenacaoLinhasQR].c_str(),
           string2.c_str(), geralC->SigmaMinAtual_pu, geralC->SigmaMaxAtual_pu,
           rel_dp, geralC->FatMultMedChave, geralC->nivel_confianca,
           codigo_barra_ref[geralC->TipoBarraRef].c_str());
   if(lisBAR_REF->Count == 1) fprintf(arq, "\n Barra de referência: ");
   else                       fprintf(arq, "\n Barras de referência:");
   bar1 = (TBarra *) lisBAR_REF->First();
   fprintf(arq, "                                       %15s", bar1->codigo.c_str());
   for(int nb=1; nb < lisBAR_REF->Count; nb++)
      {
      bar1 = (TBarra *) lisBAR_REF->Items[nb];
      fprintf(arq, "\n                                                             %15s",
              bar1->codigo.c_str());
      }
   }
if(geralC->Programa1 != progFR1)
   {
   fprintf(arq, "\n\n Número de linhas/colunas em Ycc:                                     %6d"
                  "\n                             Ygg:                                     %6d"
                  "\n                             Ynodal:                                  %6d",
           geralC->num_lin_submatriz,
           (geralC->num_tot_linhas - geralC->num_lin_submatriz),
           geralC->num_tot_linhas);
   }
teste = geralC->IsProgramaNR1(1) || geralC->IsProgramaNR1(2);
if(teste)
   {
   fprintf(arq, "\n\n Estrutura da matriz Jacobiana:"
                  "\n\n     Equações em Ir:                                                  %6d"
                    "\n                 Im:                                                  %6d"
                    "\n                 P:                                                   %6d"
                  "\n\n     Variáveis   Vr:                                                  %6d"
                    "\n                 Vm:                                                  %6d"
                    "\n                 Theta:                                               %6d"
                    "\n                 Tap:                                                 %6d"
                  "\n\n     Total:                                                           %6d",
           geralC->num_eqs_Ir, geralC->num_eqs_Im, geralC->num_eqs_P3f,
           geralC->num_vars_Vr, geralC->num_vars_Vm, geralC->num_vars_Theta,
           geralC->num_vars_Tap, geralC->num_lin_Jacob);
   }
fclose(arq);

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TImpressao1::ImprimeDados2(bool  flag_imprime_tudo,
                                           int   np,
                                           TList *lisBAR_J,
                                           TList *lisBAR_ESTADO)
{
// Local variables
bool            teste1, teste2, teste3, teste_impr;
char            char1, char2, char3, char4, char5, char6, char7, char8, char9;
int             iap, linha, num_linhas;
double          p1, q1, soma1, soma2, soma3, soma4, soma5, soma6, soma7,
                qcap1, qreat1, qtot, y1, y2;
AnsiString      charstr1, charstr2, charstr3, str1, str2, str3;
complex<double> soma_I, soma_S, soma_Z, tensao, z12c, z23c, z31c;
FILE            *arq;
TBarra          *barra1, *barra2;
TCapSerie       *caps1;
TCNL            *cnl1;
TFiltro         *filtro;
TGrupoMutua     *grupo;
TMutua          *mutua;
TReg2           *reg2;
TSup            *sup1;
TTrecho         *trec1;
TTrafo2         *trafo2;
TTrafo3         *trafo3;
TTrafoEquiv     *trafoequiv;
TTrafoZZ        *trafozz;

// Abre arquivo
arq = geralC->AbreArquivo(arqDADOS);
if(arq == NULL) return(false);

// Ordena barras, trechos e trafos, se isso não foi feito antes
OrdenaBarrasCodigo();
OrdenaTrechos();
OrdenaTrafo2();

// Determina 'teste_impr'
teste_impr = flag_imprime_tudo ||
             (! flag_imprime_tudo && (geralC->num_open_dados == 2));

// Prints bus data
if(teste_impr) fprintf(arq, "\n");
if(np >= 0) fprintf(arq, "\n\n *** Dados de barras - Patamar %d ***", (np+1));
else        fprintf(arq, "\n\n *** Dados de barras - Patamar indefinido ***");
fprintf(arq, "\n\n Código            D   Tipo      V.nom.(kV)      "
             "----P(MW)--- ---Q(MVAr)--      Qr-Qc(MVAr)    "
             "-Yref(reduc.) seq.1/0 (pu)-");
soma1  = 0.;
soma2  = 0.;
soma3  = 0.;
soma4  = 0.;
soma5  = 0.;
soma6  = 0.;
soma7  = 0.;
soma_I = czero;
soma_S = czero;
soma_Z = czero;
for(int nb=0; nb < lisBAR_CODIGO->Count; nb++)
   {
   barra1 = (TBarra *) lisBAR_CODIGO->Items[nb];
   char1  = ' ';
   if(! barra1->flag_conexa) char1 = 'D';
   fprintf(arq, "\n\n %-15s   %c   %-5s     %9.3f     ",
           barra1->codigo.c_str(),
           char1,
           geralC->codigo_tipo_barra[barra1->tipo].c_str(),
           barra1->vnom_kv);
   teste1 = barra1->CargaNula();
   teste2 = IsDoubleZero(barra1->best1_pu * geralC->Sbase); // Compatibilidade com TImpressao3.cpp
   teste3 = ((Abs(barra1->yref_reduc0_pu) == 0.) &&
             (Abs(barra1->yref_reduc1_pu) == 0.));
   if(teste1 && teste2 && teste3) continue;  // Todos os valores nulos
   p1      =  barra1->vet_carga_pu[Icte].real() * geralC->Sbase;
   q1      =  barra1->vet_carga_pu[Icte].imag() * geralC->Sbase;
   qtot    = -barra1->best1_pu                  * geralC->Sbase;
   soma1  += p1;
   soma2  += q1;
   soma3  += qtot;
   soma_I += complex<double>(p1, q1);
   fprintf(arq, "I %12.6f %12.6f     %12.6f", p1, q1, qtot);
   if(! teste3)
      {
      y1     = barra1->yref_reduc1_pu.real();
      y2     = barra1->yref_reduc1_pu.imag();
      soma4 += y1;
      soma5 += y2;
      fprintf(arq, "    %13.6e %13.6e", y1, y2);
      }
   p1      = barra1->vet_carga_pu[Scte].real() * geralC->Sbase;
   q1      = barra1->vet_carga_pu[Scte].imag() * geralC->Sbase;
   soma1  += p1;
   soma2  += q1;
   soma_S += complex<double>(p1, q1);
   fprintf(arq, "\n                                               "
                "S %12.6f %12.6f", p1, q1);
   if(! teste3)
      {
      y1     = barra1->yref_reduc0_pu.real();
      y2     = barra1->yref_reduc0_pu.imag();
      soma6 += y1;
      soma7 += y2;
      fprintf(arq, "                     %13.6e %13.6e", y1, y2);
      }
   p1      = barra1->vet_carga_pu[Zcte].real() * geralC->Sbase;
   q1      = barra1->vet_carga_pu[Zcte].imag() * geralC->Sbase;
   soma1  += p1;
   soma2  += q1;
   soma_Z += complex<double>(p1, q1);
   fprintf(arq, "\n                                               "
                "Z %12.6f %12.6f", p1, q1);
   } // for(nb)
fprintf(arq, "\n                                                 "
             "------------ ------------     ------------    "
             "------------- -------------"
             "\n                                          "
             "Total: %12.6f %12.6f     %12.6f    %13.6e %13.6e"
             "\n                                         "
             "                                           "
             "           %13.6e %13.6e"
             "\n                                         "
             "Soma_I: %12.6f %12.6f"
             "\n                                         "
             "Soma_S: %12.6f %12.6f"
             "\n                                         "
             "Soma_Z: %12.6f %12.6f",
        soma1, soma2, soma3, soma4, soma5, soma6, soma7,
        soma_I.real(), soma_I.imag(), soma_S.real(), soma_S.imag(),
        soma_Z.real(), soma_Z.imag());
fprintf(arq, "\n\n Código              Tipo     PV->PQ    Pot.nom.(MVA)"
             "      P (MW)       Q (MVAr)        Mod.V (pu)     Ang.V(°)"
             "       F.pot.mín. (pu)\n");

// Imprime dados de suprimentos
for(int nb=0; nb < lisBAR_CODIGO->Count; nb++)
   {
   barra1 = (TBarra *) lisBAR_CODIGO->Items[nb];
   sup1   = rede1->LocalizaSuprimentoBarra(barra1);
   if(sup1 == NULL) continue;  // Esta barra não tem suprimento
   tensao = geralC->FiltraZero(sup1->tensao_pu);
   if(barra1->tipo == barra1->tipo_original) char1 = ' ';
   else                                      char1 = 'S';
   fprintf(arq, "\n %-15s     %-5s       %c      %11.6f",
           barra1->codigo.c_str(),
           geralC->codigo_tipo_barra[barra1->tipo].c_str(),
           char1, sup1->snom_mva);
   if(barra1->tipo == BAR_PV)  // PV
      {
      p1 = sup1->sesp_pu.real() * geralC->Sbase;
      fprintf(arq, "     %11.4e                      %8.6f"
                   "                        %8.6f",
              p1, Abs(tensao), sup1->fp_min);
      }
   else if(barra1->tipo == BAR_PQ)  // PQ
      {
      p1 = sup1->sesp_pu.real() * geralC->Sbase;
      q1 = sup1->sesp_pu.imag() * geralC->Sbase;
      fprintf(arq, "     %11.4e   %11.4e"
                   "                                        %8.6f",
              p1, q1, sup1->fp_min);
      }
   else  // Barra Swing
      {
      fprintf(arq, "                             "
                   "         %8.6f     %9.4f",
              Abs(tensao), ArgGrau(tensao));
      }
   }

// Prints branch data
if(teste_impr && (lisTRECHO_ORD->Count > 0))
   {
   fprintf(arq, "\n\n\n *** Dados de trechos ***"
                  "\n\n De              Para            Código            "
                      "Chv   Mút.   Compr.(km)   C.adm.(A)       "
                      "     Yser1 (pu)                    Yser0 (pu)       "
                      "        Ypar_tot_nodal1 (pu)\n");
   for(int nt=0; nt < lisTRECHO_ORD->Count; nt++)
      {
      trec1    = (TTrecho *) lisTRECHO_ORD->Items[nt];
      charstr1 = trec1->bar1->codigo;
      charstr2 = trec1->bar2->codigo;
      if(charstr1.AnsiCompare(charstr2) > 0)
         {
         charstr1 = trec1->bar2->codigo;
         charstr2 = trec1->bar1->codigo;
         }
      char1 = ' ';
      if(! trec1->flag_chave_fechada) char1 = 'A';
      if(trec1->grupo_mutua >= 0)
         {
         char2 = 'M';
         str2  = AnsiString(trec1->grupo_mutua);
          }
      else
          {
          char2 = ' ';
          str2  = "  ";
          }
      fprintf(arq, "\n %-15s %-15s %-15s    %c    %c %2s  %10.5f"
                     "    %8.1f    %13.6e %13.6e   %13.6e %13.6e"
                     "   %13.6e %13.6e",
              charstr1.c_str(),
              charstr2.c_str(),
              trec1->codigo.c_str(), char1, char2, str2.c_str(),
              trec1->compr_km, trec1->iadm_A,
              trec1->Y1ser_pu.real(), trec1->Y1ser_pu.imag(),
              trec1->Y0ser_pu.real(), trec1->Y0ser_pu.imag(),
              trec1->Y1cap_pu.real(), trec1->Y1cap_pu.imag());
      }
   }

// Imprime dados de mútuas
if(teste_impr)
   {
   num_linhas = 0;
   for(int ng=0; ng < rede1->lisGRUPO_MUTUA->Count; ng++)
      {
      grupo = (TGrupoMutua *) rede1->lisGRUPO_MUTUA->Items[ng];
      for(int nm=0; nm < grupo->lisMUTUA->Count; nm++)
         {
         mutua = (TMutua *) grupo->lisMUTUA->Items[nm];
         if(num_linhas == 0)
            {
            num_linhas = 1;
            fprintf(arq, "\n\n\n *** Dados de impedâncias mútuas ***"
                         "\n\n --------------------Trecho 1-------------------"
                         "     --------------------Trecho 2-------------------"
                         "       Grupo     Xm (ohm)\n");
            }
         fprintf(arq, "\n %-15s %-15s %-15s     %-15s %-15s %-15s        "
                      "%3d    %9.4f",
                 mutua->trec1->bar1->codigo.c_str(),
                 mutua->trec1->bar2->codigo.c_str(),
                 mutua->trec1->codigo.c_str(),
                 mutua->trec2->bar1->codigo.c_str(),
                 mutua->trec2->bar2->codigo.c_str(),
                 mutua->trec2->codigo.c_str(),
                 ng, mutua->z1_ohm.imag());
         }
      }
   }

// Imprime dados de capacitores série
if(teste_impr && (rede1->lisCAPS->Count > 0))
   {
   fprintf(arq, "\n\n\n *** Dados de capacitores série ***");
   fprintf(arq, "\n\n De              Para            Código             "
                "   Impedância série (pu)\n");
   for(int ncs=0; ncs < rede1->lisCAPS->Count; ncs++)
      {
      caps1 = (TCapSerie *) rede1->lisCAPS->Items[ncs];
      soma1 = geralC->Sbase / (caps1->bar1->vnom_kv * caps1->bar1->vnom_kv); // Ybase
      p1    = caps1->r_ohm * soma1;
      q1    = caps1->x_ohm * soma1;
      fprintf(arq, "\n %-15s %-15s %-15s    %13.6e %13.6e",
              caps1->bar1->codigo.c_str(),
              caps1->bar2->codigo.c_str(),
              caps1->codigo.c_str(), p1, q1);
      }
   }

// Imprime dados de transformadores de 2 enrolamentos
if(teste_impr && (lisTRAFO2_ORD->Count > 0))
   {
   fprintf(arq, "\n\n\n *** Dados de transformadores de 2 enrolamentos ***"
                "\n\n De              Para            Código             "
                "Lig.    Tensões nom. (kV)    P.nom.(MVA)          z1 (pu)"
                "                 z0 (pu)            Tap (pu)      "
                "Perda Fe (pu)"
               "\n                                                      "
                      "       Prim.       Sec.\n");
   for(int nt=0; nt < lisTRAFO2_ORD->Count; nt++)
      {
      trafo2 = (TTrafo2 *) lisTRAFO2_ORD->Items[nt];
      fprintf(arq, "\n %-15s %-15s %-15s    %-3s   %8.3f   %8.3f"
                   "     %8.4f     %9.6f %9.6f     %9.6f %9.6f      1:%8.6f"
                   "       %7.4f",
              trafo2->bar1->codigo.c_str(),
              trafo2->bar2->codigo.c_str(),
              trafo2->codigo.c_str(),
              codigo_lig_trafo[trafo2->tipo].c_str(),
              trafo2->vnom1, trafo2->vnom2,
              trafo2->snom_mva,
              trafo2->z1_pu.real(), trafo2->z1_pu.imag(),
              trafo2->z0_pu.real(), trafo2->z0_pu.imag(),
              trafo2->tap, trafo2->y1par_pu.real());
      }
   }

// Imprime dados de reguladores de tensão
if(teste_impr && (rede1->lisREG2->Count > 0))
   {
   fprintf(arq, "\n\n\n *** Dados de reguladores de tensão ***"
                "\n\n De              Para            Código            Modelo"
                "    Sub-mod.    Lig.   Tensões nom. (kV)    P.nom.(MVA)      "
                "   Imped. (pu)          Tap (pu)      Perda Fe (pu)"
                "\n                                                           "
                "                       Prim.       Sec.\n");
   for(int nt=0; nt < rede1->lisREG2->Count; nt++)
      {
      reg2 = (TReg2 *) rede1->lisREG2->Items[nt];
      str1 = "         ";
      if(reg2->IsDOIS_SENTIDOS())
         {
         str1 = geralC->codigo_sub_modelo_regu[reg2->sub_modelo].c_str();
         }
      fprintf(arq, "\n %-15s %-15s %-15s   %8s  %9s   %-3s   %8.3f   %8.3f"
                   "     %8.4f     %9.6f %9.6f      1:%8.6f       %7.4f"
                   "\n      Var.tensão (pu): %5.3f  Núm.passos: %2d",
              reg2->bar1->codigo.c_str(),
              reg2->bar2->codigo.c_str(),
              reg2->codigo.c_str(),
              geralC->codigo_modelo_regu[reg2->modelo].c_str(),
              str1.c_str(),
              codigo_lig_trafo[reg2->tipo].c_str(),
              reg2->vnom1,
              reg2->vnom2,
              reg2->snom_mva,
              reg2->z1_pu.real(),
              reg2->z1_pu.imag(),
              reg2->tap,
              reg2->y1par_pu.real(),
              reg2->var_tensao_pu,
              reg2->num_passo);
      if(reg2->IsENERQCT())
         {
         fprintf(arq, "      B.ref.: %-15s",
                 reg2->bar_ref_12->codigo.c_str());
         }
      else if(reg2->IsPADRAO() || reg2->IsDOIS_SENTIDOS())
         {
         fprintf(arq, "      B.ref.12: %-15s  Vref.12(pu): %6.4f  BM12(pu): %5.3f",
                 reg2->bar_ref_12->codigo.c_str(),
                 reg2->vref_12,
                 reg2->banda_morta_pu);
         if(reg2->IsDOIS_SENTIDOS())
            {
            fprintf(arq, "      B.ref.21: %-15s  Vref.21(pu): %6.4f  BM21(pu): %5.3f",
                    reg2->bar_ref_21->codigo.c_str(),
                    reg2->vref_21,
                    reg2->banda_morta_inv_pu);
            }
         }
      }
   }

// Imprime dados de transformadores de 3 enrolamentos
if(teste_impr && (rede1->lisTRAFO3->Count > 0))
   {
   fprintf(arq, "\n\n\n *** Dados de transformadores de 3 enrolamentos ***"
                "\n\n Barra              Código            Ref     Lig    "
                "Pot. nom.      Tensões (kV)             Impedância      "
                "        Perda Fe"
                "\n                                                 "
                "      (MVA)      Nominal     Tap        (pu base %3.0f MVA)"
                "       (pu base %3.0f MVA)",
           geralC->Sbase, geralC->Sbase);
   for(int nt=0; nt < rede1->lisTRAFO3->Count; nt++)
      {
      trafo3 = (TTrafo3 *) rede1->lisTRAFO3->Items[nt];
      char1  = char2 = char4 = char5 = char7 = char8 = ' ';
      if     (trafo3->enrol_Sref == 0) char1 = 'S';
      else if(trafo3->enrol_Sref == 1) char4 = 'S';
      else                             char7 = 'S';
      if     (trafo3->enrol_Ypar == 0) char2 = 'Y';
      else if(trafo3->enrol_Ypar == 1) char5 = 'Y';
      else                             char8 = 'Y';
      if     (trafo3->IsDDY()) {char3 = 'D'; char6 = 'D'; char9 = 'Y';}
      else if(trafo3->IsDYY()) {char3 = 'D'; char6 = 'Y'; char9 = 'Y';}
      else                     {char3 = 'Y'; char6 = 'Y'; char9 = 'Y';}
      fprintf(arq, "\n\n P %-15s  %-15s    %c%c      %c     "
                   "%7.1f    %8.3f  %8.3f       PS %7.4f %7.4f          %7.4f",
              trafo3->bar1->codigo.c_str(),
              trafo3->codigo.c_str(), char1, char2, char3,
              trafo3->snom1_mva, trafo3->vnom1, trafo3->vtap1,
              trafo3->zps1_pu.real(), trafo3->zps1_pu.imag(),
              trafo3->ypar_pu.real());
      fprintf(arq, "\n S %-15s  %-7s            %c%c      %c     "
                   "%7.1f    %8.3f  %8.3f       ST %7.4f %7.4f",
              trafo3->bar2->codigo.c_str(), geralC->codigo_tipo_33[trafo3->tipo].c_str(),
              char4, char5, char6, trafo3->snom2_mva, trafo3->vnom2, trafo3->vtap2,
              trafo3->zst1_pu.real(), trafo3->zst1_pu.imag());
      fprintf(arq, "\n T %-15s                     %c%c      %c     "
                   "%7.1f    %8.3f  %8.3f       TP %7.4f %7.4f",
              trafo3->bar3->codigo.c_str(), char7, char8, char9,
              trafo3->snom3_mva, trafo3->vnom3, trafo3->vtap3,
              trafo3->zpt1_pu.real(), trafo3->zpt1_pu.imag());
      }
   }

// Imprime dados de transformadores equivalentes
if(teste_impr && (rede1->lisTRAFOEQUIV->Count > 0))
   {
   fprintf(arq, "\n\n\n *** Dados de trafos equivalentes - redução da "
                "rede ***"
                "\n\n De              Para            Código          "
                "   Def.P->S (°)"
                "        Ynodal1/0(i,j) (pu)             Ynodal1/0(j,i) (pu)\n");
   for(int nt=0; nt < rede1->lisTRAFOEQUIV->Count; nt++)
      {
      trafoequiv = (TTrafoEquiv *) rede1->lisTRAFOEQUIV->Items[nt];
      fprintf(arq, "\n %-15s %-15s %-15s     %7.1f",
              trafoequiv->bar1->codigo.c_str(),
              trafoequiv->bar2->codigo.c_str(),
              trafoequiv->codigo.c_str(),
              (trafoequiv->defasagem * radgrau));
      fprintf(arq, "       %13.6e %13.6e     %13.6e %13.6e",
              trafoequiv->Ynodal1_ij_pu.real(), trafoequiv->Ynodal1_ij_pu.imag(),
              trafoequiv->Ynodal1_ji_pu.real(), trafoequiv->Ynodal1_ji_pu.imag());
      fprintf(arq, "\n                                                            "
                   "       %13.6e %13.6e     %13.6e %13.6e",
              trafoequiv->Ynodal0_ij_pu.real(), trafoequiv->Ynodal0_ij_pu.imag(),
              trafoequiv->Ynodal0_ji_pu.real(), trafoequiv->Ynodal0_ji_pu.imag());
      }
   }

// Imprime dados de filtros (Curto)
if(teste_impr && (rede1->lisFILTRO->Count > 0))
   {
   fprintf(arq, "\n\n\n *** Dados de filtros ***"
                "\n\n Barra              Código            Tensão nom. (kV)"
                "       Pot. nom. (MVA)       Imped. seq. zero (pu)\n");
   for(int nf=0; nf < rede1->lisFILTRO->Count; nf++)
      {
      filtro = (TFiltro *) rede1->lisFILTRO->Items[nf];
      fprintf(arq, "\n %-15s    %-15s      %8.3f               %8.3f"
                   "         %10.4f %10.4f",
              filtro->barra->codigo.c_str(),
              filtro->codigo.c_str(),
              filtro->vnom_kv,
              filtro->snom_mva,
              filtro->z0_pu.real(),
              filtro->z0_pu.imag());
      }
   }

// Imprime dados de transformadores ZZ (Curto)
if(teste_impr && (rede1->lisTRAFOZZ->Count > 0))
   {
   fprintf(arq, "\n\n\n *** Dados de transformadores ZZ ***"
                "\n\n Barra              Código            Tensão nom. (kV)"
                "       Pot. nom. (MVA)       Imped. seq. zero (pu)\n");
   for(int nt=0; nt < rede1->lisTRAFOZZ->Count; nt++)
      {
      trafozz = (TTrafoZZ *) rede1->lisTRAFOZZ->Items[nt];
      fprintf(arq, "\n %-15s    %-15s      %8.3f               %8.3f"
                   "         %10.4f %10.4f",
              trafozz->barra->codigo.c_str(),
              trafozz->codigo.c_str(),
              trafozz->vnom_kv,
              trafozz->snom_mva,
              trafozz->z0_pu.real(),
              trafozz->z0_pu.imag());
      }
   }

// Imprime dados de cargas não-lineares
if(rede1->lisCNL->Count > 0)
   {
   fprintf(arq, "\n\n\n *** Dados de cargas não-lineares ***");
   // fprintf(arq, "\n\n Barra        F.pot.(pu)   Ordem freq.        "
   fprintf(arq, "\n\n Barra                     Ordem freq.        "
                "Pot.trifásica    Âng. potência     Fat. pot."
                "\n                                                  (kVA)"
                "            (grau)          (pu)\n");
   for(int nc=0; nc < rede1->lisCNL->Count; nc++)
      {
      cnl1   = (TCNL *) rede1->lisCNL->Items[nc];
      barra1 = cnl1->barra;
      p1     = raiz3 * barra1->vnom_kv * cnl1->corrente_a;
      q1     = cnl1->phi_rad * radgrau;
      soma1  = cos(cnl1->phi_rad);
      // fprintf(arq, "\n\n %-10s    %7.4f           1               %9.4f",
      //         cnl1->barra->codigo.SubString(0, 10).c_str(), cnl1->cos_fi, p1);
      fprintf(arq, "\n %-15s               1               %9.4f"
                   "        %9.4f         %6.4f",
              cnl1->barra->codigo.c_str(), p1, q1, soma1);
      }
   }

// Imprime linhas das matrizes Ynodal e Jacob (só uma vez)
if(geralC->num_open_dados == 2) ImprimeLinhasYnodalJacob(arq, lisBAR_J);

// Finalização
fclose(arq);

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TImpressao1::ImprimeDadosESTIM1(int   np,
                                                TList *lisBAR_ESTADO)
{
// Variáveis locais
int          ind1, num_vezes;
FILE         *arq;
TBarra       *bar1;
TBarraEstado *pbar_est;
TYnodalE     *pYE;

// Inicialização
num_vezes = 0;

// Abre arquivo
arq = geralC->AbreArquivo(arqDADOS);
if(arq == NULL) return(false);

// Ordena barras por código, se isso não foi feito antes
OrdenaBarrasCodigo();

// Imprime dados das variáveis de estado (só redes pequenas)
if(geralC->num_var_est <= 50)
   {
   num_vezes++;
   fprintf(arq, "\n\n\n *** Dados das variáveis de estado (total: %d) ***"
                "\n\n ------ Barra -------   Tipo            End.'V'     "
                "End.'Theta'\n",
           geralC->num_var_est);

   // Loop de barras
   for(int nb=0; nb < lisBAR_CODIGO->Count; nb++)
      {
      bar1 = (TBarra *) lisBAR_CODIGO->Items[nb];
      ind1 = bar1->offset;
      if(ind1 >= 0) pbar_est = (TBarraEstado *) lisBAR_ESTADO->Items[ind1];
      else          continue;

      // Imprime barra
      fprintf(arq, "\n %-15s        %-5s      ",
      bar1->codigo.c_str(), geralC->codigo_tipo_barra[bar1->tipo].c_str());
      pYE = pbar_est->vet_YE[0];  // Elemento único em TEstimador1
      if(pYE == NULL)
         {
         fprintf(arq, "          -            -");
         }
      else
         {
         fprintf(arq, "     %6d", pYE->vet_linha_E[0]);
         if(pYE->num_var == 2) fprintf(arq, "       %6d", pYE->vet_linha_E[1]);
         else                  fprintf(arq, "            -");
         }
      } // for(nb)
   }

// Imprime dados de medidores, se indicado pela chave de impressão
if(geralC->tipo_relatorio >= 2)
   {
   num_vezes++;
   ImprimeDadosMedidores(arq);
   }

// Fecha arquivo
fclose(arq);

// Retorna
if(num_vezes > 0) return(true);
else              return(false);
}

//---------------------------------------------------------------------------
bool __fastcall TImpressao1::ImprimeDadosMedidores(FILE *arq)
{
// Variáveis locais
char       char0, char1, char2, char2_prev;
int        id1, id1_prev;
AnsiString str1, str1_prev, str2, str2_prev, str3, str3_prev;
TBarra     *bar1;
TMedidor   *med1;
TTrafo2    *trafo2;
TTrecho    *trec1;

// Imprime dados de medidores
fprintf(arq, "\n\n\n *** Dados de medidores (total: %d) ***"
               "\n\n    Núm. R/V H/D  Código           Tipo eqpto  "
                   "--------------Equipamento--------------  TM         "
                   "Valor medido   Desv. padrão   Toler.  (pu)"
                 "\n                                               Barra1"
                   "          Barra2              ID\n",
        rede1->lisMED->Count);
char2_prev = '*';
str1_prev  = "*";
str2_prev  = "*";
str3_prev  = "*";
id1_prev   = -10000000;
for(int nm=0; nm < rede1->lisMED->Count; nm++)
   {
   med1 = (TMedidor *) rede1->lisMED->Items[nm];
   id1  = med1->id_calc;
   if(med1->tipo_medidor == medREAL) char0 = 'R';
   else                              char0 = 'V';
   if(med1->enabled)                 char1 = ' ';
   else                              char1 = 'D';
   char2 = geralC->codigo_tipo_medicao[med1->tipo_medicao];
   switch(med1->tipo_eqpto)
      {
      case teBARRA:
         bar1 = (TBarra *) med1->eqpto;
         str1 = "Barra";
         str2 = bar1->codigo;
         str3 = " ";
         break;

      case teTRECHO:
         trec1 = (TTrecho *) med1->eqpto;
         str1  = "Trecho";
         if(trec1->bar1 == med1->bar_ref)
            {
            str2 = trec1->bar1->codigo;
            str3 = trec1->bar2->codigo;
            }
         else
            {
            str2 = trec1->bar2->codigo;
            str3 = trec1->bar1->codigo;
            }
         break;

      case teTRAFO:
         trafo2 = (TTrafo2 *) med1->eqpto;
         str1   = "Trafo2";
         if(trafo2->bar1 == med1->bar_ref)
            {
            str2 = trafo2->bar1->codigo;
            str3 = trafo2->bar2->codigo;
            }
         else
            {
            str2 = trafo2->bar2->codigo;
            str3 = trafo2->bar1->codigo;
            }
         break;

      default:  // erro
         return(false);
      }
   fprintf(arq, "\n %6d   %c   %c   %-15s",
           (nm+1), char0, char1, med1->codigo.SubString(0, 15).c_str());
   if(str1.AnsiCompareIC(str1_prev) != 0)
      {
      fprintf(arq, "    %-7s   %-15s %-15s %7d   %c",
              str1.c_str(), str2.c_str(), str3.c_str(), id1, char2);
      }
   else if(id1 != id1_prev)
      {
      fprintf(arq, "             ");
      fprintf(arq, " %-15s %-15s %7d   %c",
              str2.c_str(), str3.c_str(), id1, char2);
      }
   else
      {
      fprintf(arq, "             ");
      if(str2.AnsiCompareIC(str2_prev) == 0) fprintf(arq, "                ");
      else                                   fprintf(arq, " %-15s", str2.c_str());
      if(str3.AnsiCompareIC(str3_prev) == 0) fprintf(arq, "                ");
      else                                   fprintf(arq, " %-15s", str3.c_str());
      if(id1 == id1_prev)                    fprintf(arq, "        ");
      else                                   fprintf(arq, " %7d", id1);
      if(char2 == char2_prev)                fprintf(arq, "    ");
      else                                   fprintf(arq, "   %c", char2);
      }
   fprintf(arq, "        %13.6e  %13.6e",
           med1->val_med_pu, med1->sigma2_pu);
   if(med1->tipo_medidor == medREAL) fprintf(arq, "  %13.6e", med1->toler_pu);
   char2_prev = char2;
   str1_prev  = str1;
   str2_prev  = str2;
   str3_prev  = str3;
   id1_prev   = id1;
   } // for(nm)

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TImpressao1::ImprimeDadosR1(void)
{
// Local variables
char            char1, char2, char3, char4, char5, char6, char7, char8, char9;
int             num_lig_mutua, num_linhas;
double          p1, q1, soma1, soma2, soma3, soma4, soma5, soma6, soma7,
                qtot, y1, y2;
AnsiString      charstr1, charstr2, string1, string2, string3, string4;
complex<double> soma_I, soma_S, soma_Z;
FILE            *arq;
TBarra          *barra1;
TCapSerie       *caps1;
TCNL            *cnl1;
TFiltro         *filtro;
TGrupoMutua     *grupo;
TMutua          *mutua;
TReg2           *reg2;
TTrecho         *trec1;
TTrafo2         *trafo2;
TTrafo3         *trafo3;
TTrafoEquiv     *trafoequiv;
TTrafoZZ        *trafozz;

// Verifica se o relatório está habilitado
if(geralC->tipo_relatorio == trSEM_RELATORIO) return(false); // Não imprimiu nada

// Abre arquivo
arq = geralC->AbreArquivo(arqDADOS);
if(arq == NULL) return(false);

// Ordena barras, trechos e trafos, se isso não foi feito antes
OrdenaBarrasCodigo();
OrdenaTrechos();
OrdenaTrafo2();

// Conta ligações com mútua
num_lig_mutua = 0;
for(int nl=0; nl < rede1->lisTRECHO->Count; nl++)
   {
   trec1 = (TTrecho *) rede1->lisTRECHO->Items[nl];
   if(trec1->grupo_mutua >= 0) num_lig_mutua++;
   }

// Imprime aviso de rede parcial
if(geralC->num_open_dados > 1) fprintf(arq, "\n");
fprintf(arq, "\n *** ATENÇÃO: a rede a reduzir é PARCIAL (composta somente "
             "pelas barras e ligações diretamente envolvidas na redução) ***");

// Prints general data
if(geralC->num_open_dados == 1)
   {
   if     (geralC->CargaNula)        string1 = "Sem carga";
   else if(geralC->CargaEmYnodal)    string1 = "Todas em Ynodal";
   else if(geralC->CargaInjecoesExt) string1 = "Injeções externas";
   else                              string1 = "";
   if(geralC->flag_capac_LT)         string2 = "Sim";
   else                              string2 = "Não";
   if(geralC->usa_pi_equiv)          string3 = "Sim";
   else                              string3 = "Não";
   if(geralC->flag_ypar_trafos)      string4 = "Sim";
   else                              string4 = "Não";
   fprintf(arq, "\n\n *** REDE PARCIAL A REDUZIR - Dados Gerais ***"
                "\n\n Programa:                                                   %15s"
                  "\n Plataforma:                                                           %5s"
                  "\n Configuração:                                                       %7s"
                  "\n Nome da rede:                           %35s",
           geralC->codigo_programa[geralC->Programa1].c_str(),
           geralC->Plataforma.c_str(),
           geralC->Configuracao.c_str(),
           geralC->NomeRede.c_str());

   fprintf(arq, "\n Potência de base (MVA):                                                %4.0f"
                "\n Representação da carga:                           %25s"
                "\n Capacitância de linhas:                                                 %3s"
                "\n Modelo pi-equivalente para linhas:                                      %3s"
                "\n Ramo paralelo de trafos:                                                %3s",
           geralC->Sbase, string1.c_str(), string2.c_str(), string3.c_str(),
           string4.c_str());

   fprintf(arq, "\n\n Número de barras %-5s                                               %6d"
                  "\n                  %-5s                                               %6d"
                  "\n                  %-5s                                               %6d"
                  "\n                  %-5s                                               %6d"
                  "\n                  %-5s                                               %6d"
                  "\n                  %-5s                                               %6d"
                  "\n                  Total                                               %6d"
                "\n\n Número de trechos sem mútua:                                         %6d"
                  "\n Número de grupos de mútuas:                                          %6d"
                  "\n Número de trechos com mútuas:                                        %6d"
                  "\n Número total de trechos:                                             %6d"
                "\n\n Número de capacitores série:                                         %6d"
                "\n\n Número de trafos de 2 enrolamentos:                                  %6d"
                  "\n                     3 enrolamentos:                                  %6d"
                  "\n                  Equivalentes:                                       %6d"
                "\n\n Número de reguladores de tensão:                                     %6d"
                "\n\n Número de cargas não-lineares:                                       %6d",
           geralC->codigo_tipo_barra[BAR_PQ].c_str(),  geralC->num_barras_PQ,
           geralC->codigo_tipo_barra[BAR_EXT].c_str(), geralC->num_barras_EXT,
           geralC->codigo_tipo_barra[BAR_PQV].c_str(), geralC->num_barras_PQV,
           geralC->codigo_tipo_barra[BAR_PV].c_str(),  geralC->num_barras_PV,
           geralC->codigo_tipo_barra[BAR_SW].c_str(),  geralC->num_barras_SW,
           geralC->codigo_tipo_barra[BAR_INT].c_str(), geralC->num_barras_INT,
           rede1->lisBAR->Count, (rede1->lisTRECHO->Count-num_lig_mutua),
           rede1->lisGRUPO_MUTUA->Count, num_lig_mutua, rede1->lisTRECHO->Count,
           rede1->lisCAPS->Count, rede1->lisTRAFO2->Count,
           rede1->lisTRAFO3->Count, rede1->lisTRAFOEQUIV->Count,
           rede1->lisREG2->Count, rede1->lisCNL->Count);

   fprintf(arq, "\n\n Número de linhas/colunas em Ynodal:                                  %6d",
           geralC->num_tot_linhas);
   fprintf(arq, "\n");
   }

// Prints bus data
fprintf(arq, "\n\n *** REDE PARCIAL A REDUZIR - Dados de barras ***\n\n"
                 " Código            D   Tipo      V.nom.(kV)      "
                 "----P(MW)--- ---Q(MVAr)--      Qr-Qc(MVAr)    "
                 "-Yref(reduc.) seq.1/0 (pu)-   Mant/Rem");
soma1  = 0.;
soma2  = 0.;
soma3  = 0.;
soma4  = 0.;
soma5  = 0.;
soma6  = 0.;
soma7  = 0.;
soma_I = czero;
soma_S = czero;
soma_Z = czero;
for(int nb=0; nb < lisBAR_CODIGO->Count; nb++)
   {
   barra1 = (TBarra *) lisBAR_CODIGO->Items[nb];
   char1  = ' ';
   char2  = ' ';
   if(! barra1->flag_conexa) char1 = 'D';
   if(! barra1->mantida)     char2 = 'R';
   fprintf(arq, "\n\n %-15s   %c   %-5s     %9.3f     ",
           barra1->codigo.c_str(),
           char1,
           geralC->codigo_tipo_barra[barra1->tipo].c_str(),
           barra1->vnom_kv);
   p1      =  barra1->vet_carga_pu[Icte].real() * geralC->Sbase;
   q1      =  barra1->vet_carga_pu[Icte].imag() * geralC->Sbase;
   qtot    = -barra1->best1_pu                  * geralC->Sbase;
   y1      = barra1->yref_reduc1_pu.real();
   y2      = barra1->yref_reduc1_pu.imag();
   soma1  += p1;
   soma2  += q1;
   soma3  += qtot;
   soma4  += y1;
   soma5  += y2;
   soma_I += complex<double>(p1, q1);
   fprintf(arq, "I %12.6f %12.6f     %12.6f    %13.6e %13.6e      %c",
           p1, q1, qtot, y1, y2, char2);
   p1      = barra1->vet_carga_pu[Scte].real() * geralC->Sbase;
   q1      = barra1->vet_carga_pu[Scte].imag() * geralC->Sbase;
   y1      = barra1->yref_reduc0_pu.real();
   y2      = barra1->yref_reduc0_pu.imag();
   soma1  += p1;
   soma2  += q1;
   soma6  += y1;
   soma7  += y2;
   soma_S += complex<double>(p1, q1);
   fprintf(arq, "\n                                               "
                "S %12.6f %12.6f                     %13.6e %13.6e",
           p1, q1, y1, y2);
   p1      = barra1->vet_carga_pu[Zcte].real() * geralC->Sbase;
   q1      = barra1->vet_carga_pu[Zcte].imag() * geralC->Sbase;
   soma1  += p1;
   soma2  += q1;
   soma_Z += complex<double>(p1, q1);
   fprintf(arq, "\n                                               "
                "Z %12.6f %12.6f", p1, q1);
   }
fprintf(arq, "\n                                                 "
             "------------ ------------     ------------    "
             "------------- -------------"
             "\n                                          "
             "Total: %12.6f %12.6f     %12.6f    %13.6e %13.6e"
             "\n                                         "
             "                                           "
             "           %13.6e %13.6e"
             "\n                                         "
             "Soma_I: %12.6f %12.6f"
             "\n                                         "
             "Soma_S: %12.6f %12.6f"
             "\n                                         "
             "Soma_Z: %12.6f %12.6f",
        soma1, soma2, soma3, soma4, soma5, soma6, soma7,
        soma_I.real(), soma_I.imag(), soma_S.real(), soma_S.imag(),
        soma_Z.real(), soma_Z.imag());

// Prints branch data
if((geralC->num_open_dados == 1) && (lisTRECHO_ORD->Count > 0))
   {
   fprintf(arq, "\n\n\n *** REDE PARCIAL A REDUZIR - Dados de trechos ***"
                  "\n\n De              Para            Código            "
                      "Chv   Mút.   Compr.(km)   C.adm.(A)      "
                      "Admitância série (pu)      Admit.paralelo (pu)\n");
   for(int nt=0; nt < lisTRECHO_ORD->Count; nt++)
      {
      trec1    = (TTrecho *) lisTRECHO_ORD->Items[nt];
      charstr1 = trec1->bar1->codigo;
      charstr2 = trec1->bar2->codigo;
      if(charstr1.AnsiCompare(charstr2) > 0)
         {
         charstr1 = trec1->bar2->codigo;
         charstr2 = trec1->bar1->codigo;
         }
      char1 = ' ';
      if(! trec1->flag_chave_fechada) char1 = 'A';
      if(trec1->grupo_mutua >= 0)
         {
         char2   = 'M';
         string2 = AnsiString(trec1->grupo_mutua);
          }
      else
          {
          char2    = ' ';
          string2  = "  ";
          }
      fprintf(arq, "\n %-15s %-15s %-15s    %c    %c %2s   %8.3f"
                     "     %8.1f    %13.6e %13.6e     %13.6e %13.6e",
              charstr1.c_str(),
              charstr2.c_str(),
              trec1->codigo.c_str(), char1, char2, string2.c_str(),
              trec1->compr_km, trec1->iadm_A,
              trec1->Y1ser_pu.real(), trec1->Y1ser_pu.imag(),
              trec1->Y1cap_pu.real(), trec1->Y1cap_pu.imag());
      }
   }

// Imprime dados de mútuas
if(geralC->num_open_dados == 1)
   {
   num_linhas = 0;
   for(int ng=0; ng < rede1->lisGRUPO_MUTUA->Count; ng++)
      {
      grupo = (TGrupoMutua *) rede1->lisGRUPO_MUTUA->Items[ng];
      for(int nm=0; nm < grupo->lisMUTUA->Count; nm++)
         {
         mutua = (TMutua *) grupo->lisMUTUA->Items[nm];
         if(num_linhas == 0)
            {
            num_linhas = 1;
            fprintf(arq, "\n\n\n *** Dados de impedâncias mútuas ***"
                         "\n\n --------------------Trecho 1-------------------"
                         "     --------------------Trecho 2-------------------"
                         "       Grupo     Xm (ohm)\n");
            }
         fprintf(arq, "\n %-15s %-15s %-15s     %-15s %-15s %-15s        "
                      "%3d    %9.4f",
                 mutua->trec1->bar1->codigo.c_str(),
                 mutua->trec1->bar2->codigo.c_str(),
                 mutua->trec1->codigo.c_str(),
                 mutua->trec2->bar1->codigo.c_str(),
                 mutua->trec2->bar2->codigo.c_str(),
                 mutua->trec2->codigo.c_str(),
                 ng, mutua->z1_ohm.imag());
         }
      }
   }

// Imprime dados de capacitores série
if((geralC->num_open_dados == 1) && (rede1->lisCAPS->Count > 0))
   {
   fprintf(arq, "\n\n\n *** REDE PARCIAL A REDUZIR - Dados de capacitores série ***");
   fprintf(arq, "\n\n De              Para            Código             "
                "   Impedância série (pu)\n");
   for(int ncs=0; ncs < rede1->lisCAPS->Count; ncs++)
      {
      caps1 = (TCapSerie *) rede1->lisCAPS->Items[ncs];
      soma1 = geralC->Sbase / (caps1->bar1->vnom_kv * caps1->bar1->vnom_kv); // Ybase
      p1    = caps1->r_ohm * soma1;
      q1    = caps1->x_ohm * soma1;
      fprintf(arq, "\n %-15s %-15s %-15s    %13.6e %13.6e",
              caps1->bar1->codigo.c_str(),
              caps1->bar2->codigo.c_str(),
              caps1->codigo.c_str(), p1, q1);
      }
   }

// Imprime dados de transformadores de 2 enrolamentos
if((geralC->num_open_dados == 1) && (lisTRAFO2_ORD->Count > 0))
   {
   fprintf(arq, "\n\n\n *** REDE PARCIAL A REDUZIR - Dados de transformadores de "
                "2 enrolamentos ***"
                "\n\n De              Para            Código             "
                "Lig.   Tensões nom. (kV)    P.nom.(MVA)        Imped. (pu)    "
                "      Tap (pu)      Perda Fe (pu)"
                "\n                                                      "
                "       Prim.       Sec.\n");
   for(int nt=0; nt < lisTRAFO2_ORD->Count; nt++)
      {
      trafo2 = (TTrafo2 *) lisTRAFO2_ORD->Items[nt];
      fprintf(arq, "\n %-15s %-15s %-15s    %-3s   %8.3f   %8.3f"
                   "     %8.4f     %9.6f %9.6f      1:%8.6f       %7.4f",
              trafo2->bar1->codigo.c_str(),
              trafo2->bar2->codigo.c_str(),
              trafo2->codigo.c_str(),
              codigo_lig_trafo[trafo2->tipo].c_str(),
              trafo2->vnom1, trafo2->vnom2,
              trafo2->snom_mva, trafo2->z1_pu.real(),
              trafo2->z1_pu.imag(), trafo2->tap, trafo2->y1par_pu.real());
      }
   }

// Imprime dados de reguladores de tensão
if((geralC->num_open_dados == 1) && (rede1->lisREG2->Count > 0))
   {
   fprintf(arq, "\n\n\n *** REDE PARCIAL A REDUZIR - Dados de reguladores de "
                "tensão ***"
                "\n\n De              Para            Código            Modelo"
                "    Sub-mod.    Lig.   Tensões nom. (kV)    P.nom.(MVA)      "
                "   Imped. (pu)          Tap (pu)      Perda Fe (pu)"
                "\n                                                           "
                "                       Prim.       Sec.\n");
   for(int nt=0; nt < rede1->lisREG2->Count; nt++)
      {
      reg2    = (TReg2 *) rede1->lisREG2->Items[nt];
      string1 = "         ";
      if(reg2->IsDOIS_SENTIDOS())
         {
         string1 = geralC->codigo_sub_modelo_regu[reg2->sub_modelo].c_str();
         }
      fprintf(arq, "\n %-15s %-15s %-15s   %8s  %9s   %-3s   %8.3f   %8.3f"
                   "     %8.4f     %9.6f %9.6f      1:%8.6f       %7.4f"
                   "\n      Var.tensão (pu): %5.3f  Núm.passos: %2d",
              reg2->bar1->codigo.c_str(),
              reg2->bar2->codigo.c_str(),
              reg2->codigo.c_str(),
              geralC->codigo_modelo_regu[reg2->modelo].c_str(),
              string1.c_str(),
              codigo_lig_trafo[reg2->tipo].c_str(),
              reg2->vnom1,
              reg2->vnom2,
              reg2->snom_mva,
              reg2->z1_pu.real(),
              reg2->z1_pu.imag(),
              reg2->tap,
              reg2->y1par_pu.real(),
              reg2->var_tensao_pu,
              reg2->num_passo);
      if(reg2->IsENERQCT())
         {
         fprintf(arq, "      B.ref.: %-15s",
                 reg2->bar_ref_12->codigo.c_str());
         }
      else if(reg2->IsPADRAO() || reg2->IsDOIS_SENTIDOS())
         {
         fprintf(arq, "      B.ref.12: %-15s  Vref.12(pu): %6.4f  BM12(pu): %5.3f",
                 reg2->bar_ref_12->codigo.c_str(),
                 reg2->vref_12,
                 reg2->banda_morta_pu);
         if(reg2->IsDOIS_SENTIDOS())
            {
            fprintf(arq, "      B.ref.21: %-15s  Vref.21(pu): %6.4f  BM21(pu): %5.3f",
                    reg2->bar_ref_21->codigo.c_str(),
                    reg2->vref_21,
                    reg2->banda_morta_inv_pu);
            }
         }
      }
   }

// Imprime dados de transformadores de 3 enrolamentos
if((geralC->num_open_dados == 1) && (rede1->lisTRAFO3->Count > 0))
   {
   fprintf(arq, "\n\n\n *** REDE PARCIAL A REDUZIR - Dados de transformadores de 3 "
                "enrolamentos ***"
                "\n\n Barra              Código            Ref     Lig    "
                "Pot. nom.      Tensões (kV)             Impedância      "
                "        Perda Fe"
                "\n                                                 "
                "      (MVA)      Nominal     Tap        (pu base %3.0f MVA)"
                "       (pu base %3.0f MVA)",
           geralC->Sbase, geralC->Sbase);
   for(int nt=0; nt < rede1->lisTRAFO3->Count; nt++)
      {
      trafo3 = (TTrafo3 *) rede1->lisTRAFO3->Items[nt];
      char1  = char2 = char4 = char5 = char7 = char8 = ' ';
      if     (trafo3->enrol_Sref == 0) char1 = 'S';
      else if(trafo3->enrol_Sref == 1) char4 = 'S';
      else                             char7 = 'S';
      if     (trafo3->enrol_Ypar == 0) char2 = 'Y';
      else if(trafo3->enrol_Ypar == 1) char5 = 'Y';
      else                             char8 = 'Y';
      if     (trafo3->IsDDY()) {char3 = 'D'; char6 = 'D'; char9 = 'Y';}
      else if(trafo3->IsDYY()) {char3 = 'D'; char6 = 'Y'; char9 = 'Y';}
      else                     {char3 = 'Y'; char6 = 'Y'; char9 = 'Y';}
      fprintf(arq, "\n\n P %-15s  %-15s    %c%c      %c     "
                   "%7.1f    %8.3f  %8.3f       PS %7.4f %7.4f          %7.4f",
              trafo3->bar1->codigo.c_str(),
              trafo3->codigo.c_str(), char1, char2, char3,
              trafo3->snom1_mva, trafo3->vnom1, trafo3->vtap1,
              trafo3->zps1_pu.real(), trafo3->zps1_pu.imag(),
              trafo3->ypar_pu.real());
      fprintf(arq, "\n S %-15s  %-7s              %c%c      %c     "
                   "%7.1f    %8.3f  %8.3f       ST %7.4f %7.4f",
              trafo3->bar2->codigo.c_str(), geralC->codigo_tipo_33[trafo3->tipo].c_str(),
              char4, char5, char6, trafo3->snom2_mva, trafo3->vnom2, trafo3->vtap2,
              trafo3->zst1_pu.real(), trafo3->zst1_pu.imag());
      fprintf(arq, "\n T %-15s                     %c%c      %c     "
                   "%7.1f    %8.3f  %8.3f       TP %7.4f %7.4f",
              trafo3->bar3->codigo.c_str(), char7, char8, char9,
              trafo3->snom3_mva, trafo3->vnom3, trafo3->vtap3,
              trafo3->zpt1_pu.real(), trafo3->zpt1_pu.imag());
      }
   }

// Imprime dados de transformadores equivalentes
if((geralC->num_open_dados == 1) && (rede1->lisTRAFOEQUIV->Count > 0))
   {
   fprintf(arq, "\n\n\n *** REDE PARCIAL A REDUZIR - Dados de trafos equivalentes"
                " - redução da rede ***"
                "\n\n De              Para            Código          "
                "   Def.P->S (°)"
                "    Ynodal1(i,j) (pu)   Ynodal0(i,j) (pu)\n");
   for(int nt=0; nt < rede1->lisTRAFOEQUIV->Count; nt++)
      {
      trafoequiv = (TTrafoEquiv *) rede1->lisTRAFOEQUIV->Items[nt];
      fprintf(arq, "\n %-15s %-15s %-15s  %7.1f",
              trafoequiv->bar1->codigo.c_str(),
              trafoequiv->bar2->codigo.c_str(),
              trafoequiv->codigo.c_str(),
              (trafoequiv->defasagem * radgrau));
      fprintf(arq, "   %13.6e %13.6e     %13.6e %13.6e",
              trafoequiv->Ynodal1_ij_pu.real(), trafoequiv->Ynodal1_ij_pu.imag(),
              trafoequiv->Ynodal0_ij_pu.real(), trafoequiv->Ynodal0_ij_pu.imag());
      }
   }

// Imprime dados de filtros
if((geralC->num_open_dados == 1) && (rede1->lisFILTRO->Count > 0))
   {
   fprintf(arq, "\n\n\n *** REDE PARCIAL A REDUZIR - Dados de filtros ***"
                "\n\n Barra              Código            Tensão nom. (kV)"
                "       Pot. nom. (MVA)       Imped. seq. zero (pu)\n");
   for(int nf=0; nf < rede1->lisFILTRO->Count; nf++)
      {
      filtro = (TFiltro *) rede1->lisFILTRO->Items[nf];
      fprintf(arq, "\n %-15s    %-15s      %8.3f               %8.3f"
                   "         %10.4f %10.4f",
              filtro->barra->codigo.c_str(),
              filtro->codigo.c_str(),
              filtro->vnom_kv,
              filtro->snom_mva,
              filtro->z0_pu.real(),
              filtro->z0_pu.imag());
      }
   }

// Imprime dados de transformadores ZZ
if((geralC->num_open_dados == 1) && (rede1->lisTRAFOZZ->Count > 0))
   {
   fprintf(arq, "\n\n\n *** REDE PARCIAL A REDUZIR - Dados de transformadores ZZ ***"
                "\n\n Barra              Código            Tensão nom. (kV)"
                "       Pot. nom. (MVA)       Imped. seq. zero (pu)\n");
   for(int nt=0; nt < rede1->lisTRAFOZZ->Count; nt++)
      {
      trafozz = (TTrafoZZ *) rede1->lisTRAFOZZ->Items[nt];
      fprintf(arq, "\n %-15s    %-15s      %8.3f               %8.3f"
                   "         %10.4f %10.4f",
              trafozz->barra->codigo.c_str(),
              trafozz->codigo.c_str(),
              trafozz->vnom_kv,
              trafozz->snom_mva,
              trafozz->z0_pu.real(),
              trafozz->z0_pu.imag());
      }
   }

// Imprime dados de cargas não-lineares
if((geralC->num_open_dados == 1) && (rede1->lisCNL->Count > 0))
   {
   fprintf(arq, "\n\n\n *** REDE PARCIAL A REDUZIR - Dados de cargas não-lineares ***");
   // fprintf(arq, "\n\n Barra        F.pot.(pu)   Ordem freq.        "
   fprintf(arq, "\n\n Barra                     Ordem freq.        "
                "Pot.trifásica    Âng. potência     Fat. pot."
                "\n                                                  (kVA)"
                "            (grau)          (pu)\n");
   for(int nc=0; nc < rede1->lisCNL->Count; nc++)
      {
      cnl1   = (TCNL *) rede1->lisCNL->Items[nc];
      barra1 = cnl1->barra;
      p1     = raiz3 * barra1->vnom_kv * cnl1->corrente_a;
      q1     = cnl1->phi_rad * radgrau;
      soma1  = cos(cnl1->phi_rad);
      // fprintf(arq, "\n\n %-10s    %7.4f           1               %9.4f",
      //         cnl1->barra->codigo.SubString(0, 10).c_str(), cnl1->cos_fi, p1);
      fprintf(arq, "\n %-15s               1               %9.4f"
                   "        %9.4f         %6.4f",
              cnl1->barra->codigo.c_str(), p1, q1, soma1);
      }
   }
fclose(arq);

// Finalização
ImprimeSeparador(arqDADOS, 1, 148);

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TImpressao1::ImprimeLigacoesFluxoRadial(FILE  *arq,
                                                        TList *lisLIG_RADIAL)
{
int         ind_pai;
AnsiString  cod1, cod2, cod3;
TBarra      *bar1, *bar2, *bar3;
VTLigacaoG1 *ligG;

// Proteção
if(lisLIG_RADIAL == NULL) return(false);  // Não imprimiu nada

// Loop de ligações radiais
fprintf(arq, "\n *** FluxoRadial1 - Ligações ***\n");
for(int ng=0; ng < lisLIG_RADIAL->Count; ng++)
   {
   ligG = (VTLigacaoG1 *) lisLIG_RADIAL->Items[ng];
   ligG->Barras(&bar1, &bar2, &bar3);
   cod1 = bar1->codigo;
   if(bar2 == NULL) cod2 = " ";
   else             cod2 = bar2->codigo;
   if(bar3 == NULL) cod3 = " ";
   else             cod3 = bar3->codigo;
   if(ligG->pai_radial == NULL) ind_pai = -1;
   else                         ind_pai = ligG->pai_radial->indice_radial;
   fprintf(arq, "\n ng: %2d     ind_pai: %2d     bar1: %-4s     bar2: %-4s     "
                "bar3: %-4s     sentido: %1d",
           ng, ind_pai, cod1.c_str(), cod2.c_str(), cod3.c_str(),
           ligG->sentido_radial);
   }
fprintf(arq, "\n\n");

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TImpressao1::ImprimeLinhasYnodalJacob(FILE *arq,
                                                      TList *lisBAR_J)
{
AnsiString string1;
bool       flag_abriu;
int        linJ1, linJ2, linY, nb2;
TBarra     *barra1;
TBarraJ    *pbarJ;
TYnodalJ   *pYJ;

// Proteção (só imprime dados de redes pequenas)
if(rede1->lisBAR->Count > 100) return(true);

// Proteção
if(geralC->IsProgramaNR1(1) && (lisBAR_J == NULL)) return(false); // Não imprimiu nada

// Inicialização
flag_abriu = false;

// Abre arquivo, se necessário
if(arq == NULL)
   {
   arq = geralC->AbreArquivo(arqDADOS);
   if(arq == NULL) return(false);
   flag_abriu = true;
   }

// Ordena barras por código, se isso não foi feito antes
OrdenaBarrasCodigo();

// Imprime cabeçalho
if(! geralC->IsProgramaNR1(1))  // Programa diferente de NR1
   {
   fprintf(arq, "\n\n\n *** Estrutura da matriz 'Ynodal' ***"
                "\n\n ------ Barra -------   Tipo         Linha/Coluna"
                "\n                                      em Ynodal\n");
   }
else  // NR1
   {
   fprintf(arq, "\n\n\n *** Estrutura das matrizes 'Ynodal' e 'Jacob' ***"
                "\n\n ------ Barra -------   Tipo         Linha/Coluna"
                "      Linha em Ir/P          Linha em Im");
   fprintf(arq, "\n                                      em Ynodal"
                "        Col.  em Vr/Theta      Col.  em Vm/Tap");
   fprintf(arq, "\n");
   }

// Loop de barras
for(int nb=0; nb < lisBAR_CODIGO->Count; nb++)
   {
   barra1 = (TBarra *) lisBAR_CODIGO->Items[nb];

   // Imprime linha em Ynodal
   fprintf(arq, "\n %-15s        %-5s         ",
           barra1->codigo.c_str(),
           geralC->codigo_tipo_barra[barra1->tipo].c_str());
   linY = barra1->linhaY;
   if(linY < 0) string1.sprintf("  DESC"); // Barra desconexa
   else         string1.sprintf("%6d", linY);
   fprintf(arq, " %6s", string1.c_str());

   // Verifica se o programa chamador é o NR1
   if(! geralC->IsProgramaNR1(1)) continue; // Programa diferente do NR1, nada a fazer

   // Obtém linhas
   linJ1 = linJ2 = -1;
   nb2   = rede1->lisBAR->IndexOf(barra1);
   pbarJ = (TBarraJ *) lisBAR_J->Items[nb2];
   pYJ   = pbarJ->YJ;
   if(pYJ != NULL)
      {
      linJ1 = pYJ->vet_linha_J[0];
      linJ2 = pYJ->vet_linha_J[1];
      }

   // Imprime linha em Jacob - equação em Ir / P
   if(linJ1 >= 0) string1.sprintf("%6d", linJ1);
   else           string1.sprintf("     -");
   fprintf(arq, "               %6s", string1.c_str());

   // Imprime linha em Jacob - equação em Im
   if(linJ2 >= 0) string1.sprintf("%6d", linJ2);
   else           string1.sprintf("     -");
   fprintf(arq, "              %6s", string1.c_str());
   }

// Fecha arquivo, se necessário
if(flag_abriu) fclose(arq);

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TImpressao1::ImprimeResultados(int    np,
                                               double *vet_desv_iter,
                                               TList  *lisBAR_REF)
{
// Variáveis locais
bool imprimiu;
int  num_vezes;

// Inicialização
num_vezes = 0;

// Imprime resultados, de acordo com o tipo de relatório
if(geralC->tipo_relatorio >= trREL_RESUMIDO)
   {
   imprimiu = ImprimeResultados1(np);                    // Imprime resumo inicial
   if(imprimiu) num_vezes++;
   if(geralC->Programa1 != progESTIM1)                   // Todos menos ESTIM1
      {
      if(geralC->FlagConv[0])
         {
         if(geralC->tipo_relatorio >= trREL_DETALHADO_1) // Imprime resultados por barra/ligação
            {
            imprimiu = ImprimeResultados2();
            if(imprimiu) num_vezes++;
            }
         imprimiu = ImprimeResultados3(np);              // Imprime resumo final
         if(imprimiu) num_vezes++;
         }
      }
   else  // ESTIM1
      {
      if(geralC->tipo_relatorio >= trREL_DETALHADO_2)    // Imprime resultados por barra/ligação
         {
         imprimiu = ImprimeResultados2();
         if(imprimiu) num_vezes++;
         }
      imprimiu = ImprimeResultados3(np);                 // Imprime resumo final
      if(imprimiu) num_vezes++;
      imprimiu = ImprimeResultadosESTIM1(vet_desv_iter, lisBAR_REF);
      if(imprimiu) num_vezes++;
      }
   }

// Imprime separador de patamares
imprimiu = false;
if(num_vezes > 0)
   {
   imprimiu = true;
   ImprimeSeparador(arqRESULT, 1, 147);
   }

return(imprimiu);
}

//---------------------------------------------------------------------------
bool __fastcall TImpressao1::ImprimeResultados1(int np)
{
// Local variables
int        num_max1, num_max2, num_brancos, tempo_ms;
AnsiString string0, string1, string2, string3, string4, string5, string6,
           string7, string8;
FILE       *arq;

// Abre arquivo
arq = geralC->AbreArquivo(arqRESULT);
if(arq == NULL) return(false);

// Impressão de resultados
string0 = geralC->codigo_programa[geralC->Programa1];
if(geralC->IsProgramaNR1(1))      string0 += " V" + geralC->versao_NR1;
if(geralC->FlagConv[0])           string1  = "           OK";
else                              string1  = "NÃO CONVERGIU";
if     (geralC->CargaNula)        string2  = "Sem carga";
else if(geralC->CargaEmYnodal)    string2  = "Todas em Ynodal";
else if(geralC->CargaInjecoesExt) string2  = "Injeções externas";
else                              string2  = "";
if(geralC->flag_capac_LT)         string3  = "Sim";
else                              string3  = "Não";
if(geralC->usa_pi_equiv)          string4  = "Sim";
else                              string4  = "Não";
if(geralC->flag_ypar_trafos)      string5  = "Sim";
else                              string5  = "Não";
if(geralC->num_open_result > 1) fprintf(arq, "\n");
fprintf(arq, "\n Programa:                                               %15s"
             "\n Plataforma:                                                       %5s"
             "\n Configuração:                                                   %7s",
        string0.c_str(), geralC->Plataforma.c_str(), geralC->Configuracao.c_str());
if((geralC->Programa1 == progESTAB) || (geralC->Programa1 == progPM))
   {
   string0 = geralC->codigo_programa[geralC->Programa2];
   if(geralC->IsProgramaNR1(2)) string0 += " V" + geralC->versao_NR1;
   fprintf(arq, "\n Programa auxiliar:                                      %15s",
           string0.c_str());
   }
fprintf(arq, "\n Nome da rede:                       %35s"
             "\n Patamar:                                                           %4d"
             "\n Convergência:                                             %13s",
        geralC->NomeRede.c_str(), (np+1), string1.c_str());

if(geralC->Programa1 == progESTIM1)
   {
   geralC->MaxIter(geralC->Programa1, &num_max1, &num_max2);
   string6     = MontaString(geralC->NumIteracao[0], num_max1,
                             geralC->MaxIteracao[0], num_max2);
   string7     = MontaString(geralC->NumIteracao[1], num_max1,
                             geralC->MaxIteracao[1], num_max2);
   num_brancos = 26 - string6.Length();
   fprintf(arq, "\n Número de iterações:                         ");
   ImprimeBrancos(num_brancos, arq);
   fprintf(arq, "%s", string6.c_str());
   fprintf(arq, "\n           montagens/fatorações de [H]:       ");
   ImprimeBrancos(num_brancos, arq);
   fprintf(arq, "%s", string7.c_str());
   }
else if(geralC->Programa1 == progFR1)
   {
   geralC->MaxIter(geralC->Programa1, &num_max1, &num_max2);
   string6     = MontaString(geralC->NumIteracao[0], num_max1,
                             geralC->MaxIteracao[0], num_max2);
   num_brancos = 26 - string6.Length();
   fprintf(arq, "\n Número de iterações:                      ");
   ImprimeBrancos(num_brancos, arq);
   fprintf(arq, "%s", string6.c_str());
   }
else if(geralC->Programa1 == progG1)
   {
   geralC->MaxIter(geralC->Programa1, &num_max1, &num_max2);
   string6     = MontaString(geralC->NumIteracao[0], num_max1,
                             geralC->MaxIteracao[0], num_max2);
   string7     = MontaString(geralC->NumIteracao[2], num_max1,
                             geralC->MaxIteracao[2], num_max2);
   num_brancos = 26 - string6.Length();
   fprintf(arq, "\n Número de iterações:                         ");
   ImprimeBrancos(num_brancos, arq);
   fprintf(arq, "%s", string6.c_str());
   fprintf(arq, "\n           montagens/fatorações de Ynodal:    ");
   ImprimeBrancos(num_brancos, arq);
   fprintf(arq, "%s", string7.c_str());
   }
else if(geralC->IsProgramaNR1(1))
   {
   geralC->MaxIter(progNR1, &num_max1, &num_max2);
   string6     = MontaString(geralC->NumIteracao[0], num_max1,
                             geralC->MaxIteracao[0], num_max2);
   string7     = MontaString(geralC->NumIteracao[1], num_max1,
                             geralC->MaxIteracao[1], num_max2);
   string8     = MontaString(geralC->NumIteracao[2], num_max1,
                             geralC->MaxIteracao[2], num_max2);
   num_brancos = 26 - string6.Length();
   fprintf(arq, "\n Número de iterações - NR:                    ");
   ImprimeBrancos(num_brancos, arq);
   fprintf(arq, "%s", string6.c_str());
   fprintf(arq, "\n                     - ajuste de Tap/Qmax:    ");
   ImprimeBrancos(num_brancos, arq);
   fprintf(arq, "%s", string7.c_str());
   fprintf(arq, "\n           montagens/fatorações do Jacobiano: ");
   ImprimeBrancos(num_brancos, arq);
   fprintf(arq, "%s", string8.c_str());
   }
else if(geralC->Programa1 == progESTAB)
   {
   if(geralC->IsProgramaNR1(2))
      {
      fprintf(arq, "\n Número de iterações executadas - NR:                           %8d"
						 "\n                                - ajuste de Tap/Qmax:           %8d"
                   "\n           montagens/fatorações do Jacobiano:                   %8d",
              geralC->NumIteracao[0], geralC->NumIteracao[1],
              geralC->NumIteracao[2]);
      }
   else
      {
      fprintf(arq, "\n Número de iterações executadas - Gauss:                          %6d",
              geralC->NumIteracao[0]);
      }
   }
else if(geralC->Programa1 == progPM)
   {
   fprintf(arq, "\n Número de iterações executadas:                                  %6d",
           geralC->NumIteracao[0]);
   }

fprintf(arq, "\n Tolerância (pu):                                                 %6.0e"
             "\n Representação da carga:                       %25s"
             "\n Capacitância de linhas:                                             %3s"
             "\n Modelo pi-equivalente para linhas:                                  %3s"
             "\n Ramo paralelo de trafos:                                            %3s",
        geralC->Precisao[0], string2.c_str(), string3.c_str(), string4.c_str(),
        string5.c_str());
if(geralC->IsProgramaNR1(1) ||
   ((geralC->Programa1 == progESTAB) && geralC->IsProgramaNR1(2)))
   {
   if(geralC->remonta_y_completa_delta_tap) string2 = "Sim";
   else                                     string2 = "Não";
   if(geralC->usa_vdnrm)                    string3 = "Sim";
   else                                     string3 = "Não";
   fprintf(arq, "\n Remonta [Y] completa quando há ajuste de tap:                       %3s"
                "\n Utiliza VDNRM:                                                      %3s",
           string2.c_str(), string3.c_str());
   }
if(geralC->modelo_eqv != modeloEQV_INDEF)
   {
   fprintf(arq, "\n Modelo da rede equivalente:                                           %1d",
           geralC->modelo_eqv);
   }
if((geralC->Programa1 == progESTAB) || (geralC->Programa1 == progPM))
   {
   if(geralC->tempo_ms >= 0) tempo_ms = geralC->tempo_ms;
   else                      tempo_ms = 0;
   fprintf(arq, "\n\n Evento:                   %45s"
                  "\n Instante (ms):                                               %10d",
           geralC->DescricaoEvento.c_str(), tempo_ms);
   }
fclose(arq);

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TImpressao1::ImprimeResultados2(void)
{
// Local variables
bool            teste;
char            char1;
double          fpot, p_kw, q_kvar, tap1, vmod;
AnsiString      cod_nat, cod1, cod2, cod3, cod4, cod5;
complex<double> corrente, sfluxo, s_kva, sperda, tensao;
FILE            *arq;
TList           *lisLIG;
TBarra          *barra1;
TCapSerie       *caps1;
TTrafo2         *trafo2;
TTrafo3         *trafo3;
TTrafoEquiv     *trafoequiv;
TTrecho         *trec1;
TLigacaoC       *ligacao;

// Abre arquivo
arq = geralC->AbreArquivo(arqRESULT);
if(arq == NULL) return(false);

// Ordena barras por código, se isso não foi feito antes
OrdenaBarrasCodigo();

// Imprime resultados nas barras combinados com resultados nas ligações
// (modo padrão de apresentação de resultados de fluxo de potência)
fprintf(arq, "\n\n\n *** Resultados nas barras e ligações ***"
             "\n\n De               D  Tipo    Vnom (kV)     "
             "---Tensão (pu/°)--     ----Corrente (A/°)----   "
             "----Potência (kW/kVAr)---    Fat.pot. (pu)"
             "\n\n      Para             Código            M    "
             "----Tap---- Pass    ----Corrente (A/°)----   ----Potência "
             "(kW/kVAr)---  -------Perda (kW/kVAr)-------");
for(int nb=0; nb < lisBAR_CODIGO->Count; nb++)
   {
   barra1   = (TBarra *) lisBAR_CODIGO->Items[nb];
   tensao   = barra1->tensao_pu;
   corrente = barra1->corr_a;
   s_kva    = 1000. * barra1->scalc_mva;
   p_kw     = s_kva.real();
   q_kvar   = s_kva.imag();
   fpot     = CalculaFatorPotencia(barra1->TipoCarga(), p_kw, q_kvar, &cod_nat);
   tensao   = geralC->FiltraZero(tensao);
   corrente = geralC->FiltraZero(corrente);
   vmod     = Abs(tensao);
   char1 = ' ';
   if(! barra1->flag_conexa) char1 = 'D';
   fprintf(arq, "\n\n %-15s  %c  %-5s  %9.3f      %8.6f %9.4f     "
                "%12.6f %9.4f   %12.3f %12.3f    %8.6f %4s\n",
           barra1->codigo.c_str(), char1,
           geralC->codigo_tipo_barra[barra1->tipo].c_str(),
           barra1->vnom_kv, vmod, ArgGrau(tensao),
           Abs(corrente), ArgGrau(corrente),
           p_kw, q_kvar, fpot, cod_nat.c_str());

   // Loop de ligações da barra
   lisLIG = barra1->lisLIG;
   for(int nl=0; nl < lisLIG->Count; nl++)
      {
      ligacao = (TLigacaoC *) lisLIG->Items[nl];
      cod1    = ligacao->cod_bar2;
      cod2    = ligacao->cod_lig;
      cod3    = "  ";
      cod4    = "           ";
      cod5    = "    ";
      switch(ligacao->tipo)
         {
         case CapSerie1:
            caps1    = (TCapSerie *) ligacao->plig;
            corrente = caps1->vet_corr_a[ligacao->sentido];
            sfluxo   = caps1->vet_s_kva[ligacao->sentido];
            sperda   = caps1->perda;
            break;

         case Reg2:
         case Trafo2:
            trafo2 = (TTrafo2 *) ligacao->plig;
            if(ligacao->sentido == 0) cod4.sprintf(" 1:%8.6f", trafo2->tap);
            else                      cod4.sprintf("-1:%8.6f", trafo2->tap);
            if(trafo2->flag_regulador)
               {
               TReg2 *reg2 = (TReg2 *) trafo2;
               teste       = reg2->IsPADRAO() &&
                             (geralC->IsProgramaNR1(1) || geralC->IsProgramaNR1(2));
               if(teste)
                  {
                  cod5 = "-   ";
                  }
               else
                  {
                  cod5.sprintf("%3d ", reg2->passo_int);
                  }
               }
            corrente = trafo2->vet_corr_a[ligacao->sentido];
            sfluxo   = trafo2->vet_s_kva[ligacao->sentido];
            sperda   = trafo2->perda_kva;
            break;

         case Trafo3:
            trafo3 = (TTrafo3 *) ligacao->plig;
            if     (ligacao->sentido == 0) tap1 = trafo3->vnom1 / trafo3->vtap1;
            else if(ligacao->sentido == 1) tap1 = trafo3->vnom2 / trafo3->vtap2;
            else                           tap1 = trafo3->vnom3 / trafo3->vtap3;
            cod4.sprintf(" 1:%8.6f", tap1);
            corrente = trafo3->vet_corr_a[ligacao->sentido];
            sfluxo   = trafo3->vet_s_kva[ligacao->sentido];
            sperda   = trafo3->perda_kva;
            break;

         case TrafoEquiv1:
            trafoequiv = (TTrafoEquiv *) ligacao->plig;
            corrente   = trafoequiv->vet_corr_a[ligacao->sentido];
            sfluxo     = trafoequiv->vet_s_kva[ligacao->sentido];
            sperda     = trafoequiv->perda_kva;
            break;

         case Trecho1:
            trec1 = (TTrecho *) ligacao->plig;
            if(trec1->grupo_mutua >= 0)
               {
               cod3 = "M ";
               cod4.sprintf("%-11d", trec1->grupo_mutua);
               }
            corrente = trec1->vet_corr_a[ligacao->sentido];
            sfluxo   = trec1->vet_s_kva[ligacao->sentido];
            sperda   = trec1->perda_kva;
            break;

         default:  // Erro - nada a fazer
            break;
         }

      corrente = geralC->FiltraZero(corrente);
      fprintf(arq, "\n      %-15s  %-15s   %2s   %11s %4s    "
                   "%12.6f %9.4f   %12.3f %12.3f  %14.6f %14.6f",
              cod1.c_str(), cod2.c_str(), cod3.c_str(), cod4.c_str(), cod5.c_str(),
              Abs(corrente), ArgGrau(corrente),
              sfluxo.real(), sfluxo.imag(), sperda.real(), sperda.imag());
      } // for(nl)
   } // for(nb)
fclose(arq);

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TImpressao1::ImprimeResultados3(int np)
{
// Local variables
double          vmax, vmod, vmin;
AnsiString      bar_vmax, bar_vmin;
complex<double> tensao;
FILE            *arq;
TBarra          *barra1;

// Abre arquivo
arq = geralC->AbreArquivo(arqRESULT);
if(arq == NULL) return(false);

// Loop de barras, determinando tensão mínima e máxima
vmin =  1.e10;
vmax = -1.e10;
for(int nb=0; nb < rede1->lisBAR->Count; nb++)
   {
   barra1 = (TBarra *) rede1->lisBAR->Items[nb];
   tensao = geralC->FiltraZero(barra1->tensao_pu);
   vmod   = Abs(tensao);
   if(barra1->flag_conexa)
      {
      if(vmod < vmin)
         {
         bar_vmin = barra1->codigo;
         vmin     = vmod;
         }
      if(vmod > vmax)
         {
         bar_vmax = barra1->codigo;
         vmax     = vmod;
         }
      }
   } // for(nb)

// Imprime tensão mínima e tensão máxima
fprintf(arq, "\n\n     Tensão mínima:                    %9.6f pu - barra: %-15s"
               "\n     Tensão máxima:                    %9.6f pu - barra: %-15s",
        vmin, bar_vmin.c_str(), vmax, bar_vmax.c_str());

// Imprime potências totais e perda total calculada pelas ligações
fprintf(arq, "\n\n     Potência total de geração:  %15.6f kW   %15.6f kVAr"
                 "                  Perda total admit. ref.:  %14.6f %14.6f"
               "\n     Potência total de carga:    %15.6f kW   %15.6f kVAr"
                 "                                            "
                 "-------------- --------------",
        geralC->geracao_total.real(),     geralC->geracao_total.imag(),
        geralC->perda_admit_reduc.real(), geralC->perda_admit_reduc.imag(),
        geralC->carga_total.real(),       geralC->carga_total.imag());
// Partida de motor: imprime as perdas da mesma forma que no caso de cargas
//                   representadas por injeções externas (os motores são
//                   tratados como injeções externas)
if((geralC->CargaEmYnodal) && (geralC->Programa1 != progPM))
   {
   fprintf(arq, "\n                                     "
                  "                                     "
                  "                  Perda total ligações:     %14.6f %14.6f",
           geralC->perda_total_2.real(), geralC->perda_total_2.imag());
   }
else
   {
   fprintf(arq, "\n     Perda total (G-C):          %15.6f kW   %15.6f kVAr"
                  "                  Perda total ligações:     %14.6f %14.6f",
           geralC->perda_total_1.real(), geralC->perda_total_1.imag(),
           geralC->perda_total_2.real(), geralC->perda_total_2.imag());
   }

// Imprime valor total da perda no ferro
fprintf(arq, "\n\n     Perda no ferro total:       %15.6f kW",
        geralC->perda_ypar_trafos.real());
if(geralC->flag_ypar_trafos) fprintf(arq, "   (valor atual, considerado no cálculo)");
else                         fprintf(arq, "   (valor nominal, não considerado no cálculo)");

if(np == (geralC->num_tot_pat - 1))
   {
   // Último patamar, imprime valores de energia
   // (geração total, carga total e perda total)
   fprintf(arq, "\n\n     Energia total de geração:   %15.6f MWh  %15.6f MVArh"
                  "\n     Energia total de carga:     %15.6f MWh  %15.6f MVArh"
                  "\n     Energia total perdida:      %15.6f MWh  %15.6f MVArh",
           geralC->energia_geracao_mvah.real(), geralC->energia_geracao_mvah.imag(),
           geralC->energia_carga_mvah.real(),   geralC->energia_carga_mvah.imag(),
           geralC->energia_perdas_mvah.real(),  geralC->energia_perdas_mvah.imag());
   }
fclose(arq);

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TImpressao1::ImprimeResultadosCurto(int             ind_pat,
                                                    complex<double> zent0_pu,
                                                    complex<double> zent1_pu,
                                                    complex<double> s3f_mva,
                                                    complex<double> sft_mva,
                                                    complex<double> **cf,
                                                    complex<double> **cs,
                                                    complex<double> **vaf,
                                                    complex<double> **vbf,
                                                    complex<double> **vcf,
                                                    complex<double> **cor_ga,
                                                    complex<double> **cor_gb,
                                                    complex<double> **cor_gc,
                                                    complex<double> **cor_g0,
                                                    complex<double> **cor_g1,
                                                    complex<double> **cor_g2,
                                                    TBarra          *bar_def)
{
int             nbd, nbs;
double          ibase, ibase_def;
complex<double> cf0, cf1, cf2, cs0, cs1, cs2, s3f, sft, va, vb, vc;
TBarra          *bar1;
TSup            *sup1;
FILE            *arq;

// Verifica se o relatório está habilitado
if(geralC->tipo_relatorio == trSEM_RELATORIO) return(false); // Não imprimiu nada

// Abre arquivo
arq = geralC->AbreArquivo(arqRESULT);
if(arq == NULL) return(false);

// Imprime cabeçalho
nbd       = bar_def->offset;
ibase_def = geralC->Sbase * raiz3_inverso / bar_def->vnom_kv;
if(geralC->CargaNula)
   {
   fprintf(arq, "\n            *** Defeito na barra %-s - SEM pré-falta ***",
           bar_def->codigo.c_str());
   }
else
   {
   fprintf(arq, "\n            *** Defeito na barra %-s - COM pré-falta "
                "(patamar %d) ***",
           bar_def->codigo.c_str(), (ind_pat+1));
   }

// Imprime impedâncias de entrada e potências de curto-circuito na barra de defeito
fprintf(arq, "\n\n Impedâncias de entrada na barra de defeito (r, x) (pu)"
             "\n\n    Z0:         %13.6f %13.6f"
               "\n    Z1:         %13.6f %13.6f",
             zent0_pu.real(), zent0_pu.imag(), zent1_pu.real(), zent1_pu.imag());
fprintf(arq, "\n\n Potências de curto-circuito na barra de defeito (MW, MVAr)"
             "\n\n    Trifásico:  %13.6f %13.6f"
               "\n    Fase-terra: %13.6f %13.6f",
             s3f_mva.real(), s3f_mva.imag(), sft_mva.real(), sft_mva.imag());

// Imprime valores de corrente e tensão na barra de defeito
fprintf(arq, "\n\n\n Tipo Defeito    --------- C o r r e n t e s  (kA / °)"
             " ----------       ---------- T e n s õ e s  (pu / °)"
             " ----------"
             "\n                      FASE A           FASE B           "
             "FASE C              FASE A          FASE B          FASE C\n");
for(int cu=0; cu < MAX_CU; cu++)
   {
   cf0 = RoundZero(cf[0][cu]);
   cf1 = RoundZero(cf[1][cu]);
   cf2 = RoundZero(cf[2][cu]);
   va  = RoundZero(vaf[nbd][cu]);
   vb  = RoundZero(vbf[nbd][cu]);
   vc  = RoundZero(vcf[nbd][cu]);
   fprintf(arq, "\n %12s    %7.3f %6.1f   %7.3f %6.1f   %7.3f %6.1f"
                    "       %6.4f %6.1f   %6.4f %6.1f   %6.4f %6.1f",
           codigo_tipo_curto[cu].c_str(),
           (Abs(cf0) * ibase_def), ArgGrau(cf0),
           (Abs(cf1) * ibase_def), ArgGrau(cf1),
           (Abs(cf2) * ibase_def), ArgGrau(cf2),
           Abs(va),                ArgGrau(va),
           Abs(vb),                ArgGrau(vb),
           Abs(vc),                ArgGrau(vc));
   }

// Imprime contribuição dos suprimentos
if(geralC->CargaNula)
   {
   fprintf(arq, "\n\n\n\n     *** Contribuição dos suprimentos para defeito na "
                "barra %-s - SEM pré-falta ***",
           bar_def->codigo.c_str());
   }
else
   {
   fprintf(arq, "\n\n\n\n     *** Contribuição dos suprimentos para defeito na "
                "barra %-s - COM pré-falta (patamar %d) ***",
           bar_def->codigo.c_str(), (ind_pat+1));
   }
fprintf(arq, "\n\n Tipo Defeito  Barra              ------------ Correntes Seq."
             " (kA / °) ------------  ------------ Correntes Fase (kA / °)"
             " ------------  ------------- Tensoes Fase (pu / °)"
             " -------------"
             "\n                                          IO               I1"
             "               I2               IA               IB         "
             "      IC               VA               VB               VC");
for(int cu=0; cu < MAX_CU; cu++)
   {
   cf0 = RoundZero(cf[0][cu]);
   cf1 = RoundZero(cf[1][cu]);
   cf2 = RoundZero(cf[2][cu]);
   cs0 = RoundZero(cs[0][cu]);
   cs1 = RoundZero(cs[1][cu]);
   cs2 = RoundZero(cs[2][cu]);
   va  = RoundZero(vaf[nbd][cu]);
   vb  = RoundZero(vbf[nbd][cu]);
   vc  = RoundZero(vcf[nbd][cu]);
   fprintf(arq, "\n\n %12s  %-15s    %8.3f %6.1f  %8.3f %6.1f  %8.3f %6.1f"
                                  "  %8.3f %6.1f  %8.3f %6.1f  %8.3f %6.1f"
                                  "  %8.3f %6.1f  %8.3f %6.1f  %8.3f %6.1f",
           codigo_tipo_curto[cu].c_str(), bar_def->codigo.c_str(),
           (Abs(cs0) * ibase_def), ArgGrau(cs0),
           (Abs(cs1) * ibase_def), ArgGrau(cs1),
           (Abs(cs2) * ibase_def), ArgGrau(cs2),
           (Abs(cf0) * ibase_def), ArgGrau(cf0),
           (Abs(cf1) * ibase_def), ArgGrau(cf1),
           (Abs(cf2) * ibase_def), ArgGrau(cf2),
           Abs(va), ArgGrau(va),
           Abs(vb), ArgGrau(vb),
           Abs(vc), ArgGrau(vc));
   for(int ns=0; ns < rede1->lisSUP->Count; ns++)
      {
      sup1  = (TSup *) rede1->lisSUP->Items[ns];
      bar1  = sup1->barra;
      nbs   = bar1->offset;
      ibase = geralC->Sbase * raiz3_inverso / bar1->vnom_kv;
      cs0   = RoundZero(cor_g0[ns][cu]);
      cs1   = RoundZero(cor_g1[ns][cu]);
      cs2   = RoundZero(cor_g2[ns][cu]);
      cf0   = RoundZero(cor_ga[ns][cu]);
      cf1   = RoundZero(cor_gb[ns][cu]);
      cf2   = RoundZero(cor_gc[ns][cu]);
      va    = RoundZero(vaf[nbs][cu]);
      vb    = RoundZero(vbf[nbs][cu]);
      vc    = RoundZero(vcf[nbs][cu]);
      if(ns == 0) fprintf(arq, "\n");
      fprintf(arq, "\n             Sup. %-15s %8.3f %6.1f  %8.3f %6.1f  %8.3f %6.1f"
                                           "  %8.3f %6.1f  %8.3f %6.1f  %8.3f %6.1f"
                                           "  %8.3f %6.1f  %8.3f %6.1f  %8.3f %6.1f",
              bar1->codigo.c_str(),
              (Abs(cs0) * ibase), ArgGrau(cs0),
              (Abs(cs1) * ibase), ArgGrau(cs1),
              (Abs(cs2) * ibase), ArgGrau(cs2),
              (Abs(cf0) * ibase), ArgGrau(cf0),
              (Abs(cf1) * ibase), ArgGrau(cf1),
              (Abs(cf2) * ibase), ArgGrau(cf2),
              Abs(va), ArgGrau(va),
              Abs(vb), ArgGrau(vb),
              Abs(vc), ArgGrau(vc));
      }
   if((rede1->lisSUP->Count == 1) && (cu < (MAX_CU-1))) fprintf(arq, "\n");
   }
fclose(arq);

// Finalização
ImprimeSeparador(arqRESULT, 1, 134);

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TImpressao1::ImprimeResultadosESTIM1(double *vet_desv_iter,
                                                     TList  *lisBAR_REF)
{
// Variáveis locais
bool       sucesso;
int        iter_max;
double     rel_dp;
AnsiString str0, str1, str2, str3;
FILE       *arq;
TBarra     *bar1;

// Verifica se o relatório está habilitado
if(geralC->tipo_relatorio == trSEM_RELATORIO) return(false); // Não imprimiu nada

// Inicialização
sucesso = true;

// Abre arquivo
arq = geralC->AbreArquivo(arqRESULT);
if(arq == NULL) return(false);

// Imprime dados gerais
if(geralC->usa_vdnrm)                   str0 = "Sim";
else                                    str0 = "Não";
if(geralC->metodoEE == meeCONVENCIONAL) str1 = "linhas: ";
else                                    str1 = "colunas:";
if(geralC->LimitaSigmas)                str2 = "Sim";
else                                    str2 = "Não";
fprintf(arq, "\n\n\n *** Resultados da Estimação de Estado ***"
               "\n\n Número de medidores reais habilitados:                                 %6d"
                 "\n                           desabilitados (sigma elevado):               %6d"
                 "\n        total de medidores reais:                                       %6d"
                 "\n                           virtuais:                                    %6d"
                 "\n        total de medidores (Nm):                                        %6d"
                 "\n              de variáveis de estado (Ns):                              %6d"
                 "\n Método de resolução:                                           %14s"
                 "\n Número de iterações executadas:                                        %6d"
                 "\n Utiliza VDNM:                                                             %3s",
        (geralC->num_med_reais - geralC->num_med_desab),
         geralC->num_med_desab, geralC->num_med_reais,
        (rede1->lisMED->Count - geralC->num_med_reais),
        rede1->lisMED->Count, geralC->num_var_est,
        codigo_metodo_EE[geralC->metodoEE].c_str(),
        geralC->NumIteracao[0], str0.c_str());
if(geralC->usa_vdnrm)
   {
   fprintf(arq, "\n    Número máximo  de montagens de [H]:                                 %6d"
                "\n           efetivo de montagens de [H]:                                 %6d",
           geralC->MaxIteracao[1], geralC->NumIteracao[1]);
   }
rel_dp = geralC->SigmaMaxAtual_pu / geralC->SigmaMinAtual_pu;
fprintf(arq, "\n Critério para ordenação de %8s                         %17s"
             "\n Critério para ordenação de linhas (QR):                   %19s"
             "\n Limitação de valores de desvio padrão:                                    %3s"
             "\n Menor desvio padrão (pu):                                       %13.6e"
             "\n Maior desvio padrão (pu):                                       %13.6e"
             "\n Relação dp_max / dp_min:                                        %13.6e"
             "\n Fator multiplicativo da medição de chaves (pu):                       %7.4f"
             "\n Nível de confiança das medições:                                       %6.4f"
             "\n Tipo de referência:                                           %15s",
        str1.c_str(),
        codigo_proc_linhas_colunas[geralC->criterio_proc_colunas].c_str(),
        codigo_proc_linhas_qr[geralC->OrdenacaoLinhasQR].c_str(),
        str2.c_str(), geralC->SigmaMinAtual_pu,
        geralC->SigmaMaxAtual_pu, rel_dp, geralC->FatMultMedChave,
        geralC->nivel_confianca, codigo_barra_ref[geralC->TipoBarraRef].c_str());
if(lisBAR_REF->Count == 1) fprintf(arq, "\n Barra de referência: ");
else                       fprintf(arq, "\n Barras de referência:");
bar1 = (TBarra *) lisBAR_REF->First();
fprintf(arq, "                                         %15s", bar1->codigo.c_str());
for(int nb=1; nb < lisBAR_REF->Count; nb++)
   {
   bar1 = (TBarra *) lisBAR_REF->Items[nb];
   fprintf(arq, "\n                                                               %15s",
           bar1->codigo.c_str());
   }

// Imprime resultados nos medidores, se indicado pelo tipo de relatório
if(geralC->tipo_relatorio >= trREL_DETALHADO_1)
                                      sucesso = ImprimeResultadosMedidores(arq);

// Imprime resumo das iterações
// Determina número de valores a imprimir (necessário fazer a comparação
// para incluir o caso de não haver convergência)
iter_max = geralC->NumIteracao[0] + 1;
fprintf(arq, "\n\n *** Desvio total ao longo das iterações ***"
             "\n\n        Iteração     Desvio total\n");
for(int iter=0; iter < iter_max; iter++)
   {
   fprintf(arq, "\n         %4d       %13.6e", iter, vet_desv_iter[iter]);
   }

// Fecha arquivo
fclose(arq);

return(sucesso);
}

//---------------------------------------------------------------------------
bool __fastcall TImpressao1::ImprimeResultadosMedidores(FILE *arq)
{
// Variáveis locais
char       char0, char1, char2, char2_prev;
int        id1, id1_prev, med_desv_max, nm_max_abs_pneg, num_casas,
           num_med_pneg;
double     desv_max, desv1, max_abs_pneg;
AnsiString str1, str1_prev, str2, str2_prev, str3, str3_prev;
TBarra     *bar1;
TMedidor   *med1;
TTrecho    *trec1;
TTrafo2    *trafo2;

// Localiza medidor com maior desvio
desv_max = -1.;
for(int nm=0; nm < rede1->lisMED->Count; nm++)
   {
   med1  = (TMedidor *) rede1->lisMED->Items[nm];
   desv1 = fabs(med1->desvio_ndp);
   if(desv1 > desv_max)
      {
      med_desv_max = nm;
      desv_max     = desv1;
      }
   }

// Imprime resultados nos medidores
fprintf(arq, "\n\n\n *** Resultados nos medidores (total: %d) ***"
               "\n\n    Núm. R/V H/D  Código           Tipo eqpto  "
                   "--------------Equipamento--------------  TM         "
                   "Valor medido   Valor estim.   Desv. padrão  Desv.(núm.DP)"
                 "\n                                               Barra1"
                   "          Barra2              ID\n",
        rede1->lisMED->Count);
char2_prev = '*';
str1_prev  = "*";
str2_prev  = "*";
str3_prev  = "*";
id1_prev   = -10000000;
for(int nm=0; nm < rede1->lisMED->Count; nm++)
   {
   med1 = (TMedidor *) rede1->lisMED->Items[nm];
   id1  = med1->id_calc;
   if(med1->tipo_medidor == medREAL) char0 = 'R';
   else                              char0 = 'V';
   if(med1->enabled)                 char1 = ' ';
   else                              char1 = 'D';
   char2 = geralC->codigo_tipo_medicao[med1->tipo_medicao];
   switch(med1->tipo_eqpto)
      {
      case teBARRA:
         bar1 = (TBarra *) med1->eqpto;
         str1 = "Barra";
         str2 = bar1->codigo;
         str3 = " ";
         break;

      case teTRECHO:
         trec1 = (TTrecho *) med1->eqpto;
         str1  = "Trecho";
         if(trec1->bar1 == med1->bar_ref)
            {
            str2 = trec1->bar1->codigo;
            str3 = trec1->bar2->codigo;
            }
         else
            {
            str2 = trec1->bar2->codigo;
            str3 = trec1->bar1->codigo;
            }
         break;

      case teTRAFO:
         trafo2 = (TTrafo2 *) med1->eqpto;
         str1   = "Trafo2";
         if(trafo2->bar1 == med1->bar_ref)
            {
            str2 = trafo2->bar1->codigo;
            str3 = trafo2->bar2->codigo;
            }
         else
            {
            str2 = trafo2->bar2->codigo;
            str3 = trafo2->bar1->codigo;
            }
         break;

      default:  // erro
         return(false);
      }
   fprintf(arq, "\n %6d   %c   %c   %-15s",
           (nm+1), char0, char1, med1->codigo.SubString(0, 15).c_str());
   if(str1.AnsiCompareIC(str1_prev) != 0)
      {
      fprintf(arq, "    %-7s   %-15s %-15s %7d   %c",
              str1.c_str(), str2.c_str(), str3.c_str(), id1, char2);
      }
   else if(id1 != id1_prev)
      {
      fprintf(arq, "             ");
      fprintf(arq, " %-15s %-15s %7d   %c",
              str2.c_str(), str3.c_str(), id1, char2);
      }
   else
      {
      fprintf(arq, "             ");
      if(str2.AnsiCompareIC(str2_prev) == 0) fprintf(arq, "                ");
      else                                   fprintf(arq, " %-15s", str2.c_str());
      if(str3.AnsiCompareIC(str3_prev) == 0) fprintf(arq, "                ");
      else                                   fprintf(arq, " %-15s", str3.c_str());
      if(id1 == id1_prev)                    fprintf(arq, "        ");
      else                                   fprintf(arq, " %7d", id1);
      if(char2 == char2_prev)                fprintf(arq, "    ");
      else                                   fprintf(arq, "   %c", char2);
      }
   fprintf(arq, "        %13.6e  %13.6e  %13.6e   %8.2f",
           med1->val_med_pu, med1->val_est_pu, med1->sigma2_pu, med1->desvio_ndp);
   if(nm == med_desv_max) fprintf(arq, " MAIOR");
   char2_prev = char2;
   str1_prev  = str1;
   str2_prev  = str2;
   str3_prev  = str3;
   id1_prev   = id1;
   } // for(nm)
fprintf(arq, "\n");

// Imprime medidores reais de barra com P_est < 0, se houver
num_med_pneg = 0;
for(int nm=0; nm < geralC->num_med_reais; nm++)
   {
   med1 = (TMedidor *) rede1->lisMED->Items[nm];
   if(med1->tipo_eqpto   != teBARRA) continue;  // Medidor irrelevante
   if(med1->tipo_medicao != medP)    continue;  // Medidor irrelevante
   if(med1->val_est_pu >= 0.)        continue;  // Medidor irrelevante
   num_med_pneg++;
   }
if     (num_med_pneg == 0) return(true);  // Não há medidores com P_est < 0
else if(num_med_pneg == 1) fprintf(arq, "\n\n\n *** 1 medidor real");
else                       fprintf(arq, "\n\n\n *** %d medidores reais", num_med_pneg);
fprintf(arq, " de barra com P_est < 0 ***\n"
                 "\n      Núm. R/V H/D  Código           Tipo eqpto  "
                   "--------------Equipamento--------------  TM         "
                   "Valor medido   Valor estim.   Desv. padrão  Desv.(núm.DP)"
                 "\n                                                 Barra1"
                   "          Barra2              ID\n");
char2_prev   = '*';
str1_prev    = "*";
str2_prev    = "*";
str3_prev    = "*";
id1_prev     = -10000000;
desv1        =  0.;  // Potência negativa total
max_abs_pneg = -1.;
for(int nm=0; nm < geralC->num_med_reais; nm++)
   {
   med1 = (TMedidor *) rede1->lisMED->Items[nm];
   if(med1->tipo_eqpto   != teBARRA) continue;  // Medidor irrelevante
   if(med1->tipo_medicao != medP)    continue;  // Medidor irrelevante
   if(med1->val_est_pu >= 0.)        continue;  // Medidor irrelevante
   id1  = med1->id_calc;
   if(med1->tipo_medidor == medREAL) char0 = 'R';
   else                              char0 = 'V';
   if(med1->enabled)                 char1 = ' ';
   else                              char1 = 'D';
   char2  = geralC->codigo_tipo_medicao[med1->tipo_medicao];
   bar1   = (TBarra *) med1->eqpto;
   str1   = "Barra";
   str2   = bar1->codigo;
   str3   = " ";
   desv1 += med1->val_est_pu;
   if(fabs(med1->val_est_pu) > max_abs_pneg)
      {
      nm_max_abs_pneg = nm;
      max_abs_pneg    = fabs(med1->val_est_pu);
      }
   fprintf(arq, "\n   %6d   %c   %c   %-15s",
           (nm+1), char0, char1, med1->codigo.SubString(0, 15).c_str());
   if(str1.AnsiCompareIC(str1_prev) != 0)
      {
      fprintf(arq, "    %-7s   %-15s %-15s %7d   %c",
              str1.c_str(), str2.c_str(), str3.c_str(), id1, char2);
      }
   else if(id1 != id1_prev)
      {
      fprintf(arq, "             ");
      fprintf(arq, " %-15s %-15s %7d   %c",
              str2.c_str(), str3.c_str(), id1, char2);
      }
   else
      {
      fprintf(arq, "             ");
      if(str2.AnsiCompareIC(str2_prev) == 0) fprintf(arq, "                ");
      else                                   fprintf(arq, " %-15s", str2.c_str());
      if(str3.AnsiCompareIC(str3_prev) == 0) fprintf(arq, "                ");
      else                                   fprintf(arq, " %-15s", str3.c_str());
      if(id1 == id1_prev)                    fprintf(arq, "        ");
      else                                   fprintf(arq, " %7d", id1);
      if(char2 == char2_prev)                fprintf(arq, "    ");
      else                                   fprintf(arq, "   %c", char2);
      }
   fprintf(arq, "        %13.6e  %13.6e  %13.6e   %8.2f",
           med1->val_med_pu, med1->val_est_pu, med1->sigma2_pu, med1->desvio_ndp);
   if(nm == med_desv_max) fprintf(arq, " MAIOR");
   char2_prev = char2;
   str1_prev  = str1;
   str2_prev  = str2;
   str3_prev  = str3;
   id1_prev   = id1;
   } // for(nm)
fprintf(arq, "\n\n     Medidor com maior potência ativa negativa: %-6d"
                 "   P = %13.6e pu = %13.6e kW",
             (nm_max_abs_pneg+1), (-max_abs_pneg),
             (-max_abs_pneg * 1000. * geralC->Sbase));
if(num_med_pneg == 1) fprintf(arq, "\n     Potência negativa total (1 medidor):  ");
else                  fprintf(arq, "\n     Potência negativa total (%d medidores):", num_med_pneg);
num_casas = 5 - int(log10(double(num_med_pneg) + 0.0001));
for(int n=0; n < num_casas; n++) fprintf(arq, " ");
fprintf(arq, "         P = %13.6e pu = %13.6e kW\n",
        desv1, (desv1 * 1000. * geralC->Sbase));

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TImpressao1::ImprimeResultadosR1(int      np,
                                                 VTGeralC *geralC,
                                                 TList    *lisBAR,
                                                 TList    *lisTRECHO)
{
// Local variables
double          p1, q1, p2, q2, soma1, soma2, soma3, soma4, soma5, soma6,
                soma7, soma8, y1, y2;
complex<double> v1;
FILE            *arq;
TBarra          *barra1;
TTrecho         *ligacao1;

// Verifica se o relatório está habilitado
if(geralC->tipo_relatorio == trSEM_RELATORIO) return(false); // Não imprimiu nada

// Abre arquivo
arq = geralC->AbreArquivo(arqRESULT);
if(arq == NULL) return(false);

// Imprime aviso de rede parcial
if(geralC->num_open_result == 1)
   {
   fprintf(arq, "\n *** ATENÇÃO: a rede reduzida é PARCIAL (composta somente "
             "pelas barras diretamente envolvidas na redução) ***");
   }

// Prints general data
if(geralC->num_open_result == 1)
   {
   fprintf(arq, "\n\n *** REDE REDUZIDA - Dados Gerais ***"
                "\n\n Programa:                                                   %15s"
                  "\n Plataforma:                                                           %5s"
                  "\n Configuração:                                                       %7s"
                  "\n Nome da rede:                           %35s"
                  "\n Potência de base (MVA):                                                %4.0f",
           geralC->codigo_programa[geralC->Programa1].c_str(),
           geralC->Plataforma.c_str(),
           geralC->Configuracao.c_str(),
           geralC->NomeRede.c_str(), geralC->Sbase);

   fprintf(arq, "\n\n Número de barras - de carga:                                         %6d",
           geralC->num_barras_carga);
   fprintf(arq,   "\n                  - de geração:                                       %6d",
           geralC->num_barras_geracao);
   fprintf(arq,   "\n                  - total:                                            %6d",
           lisBAR->Count);
   fprintf(arq, "\n\n Número de ligações:                                                  %6d",
           lisTRECHO->Count);
   }

// Prints bus data
fprintf(arq, "\n\n *** REDE REDUZIDA - Dados de barras - Patamar %d ***\n\n"
             " Código             Tipo barra   V.nom.(kV)     "
             "---Pot. absorvida final--   -Variação pot. absorvida-   "
             "-Yref(reduc.) seq.1/0 (pu)-   -----ModV/AngV----"
             "\n                                                "
             "----P(MW)--- ---Q(MVAr)--   ---DP(MW)--- --DQ(MVAr)--",
        (np+1));
soma1 = 0.;
soma2 = 0.;
soma3 = 0.;
soma4 = 0.;
soma5 = 0.;
soma6 = 0.;
soma7 = 0.;
soma8 = 0.;
for(int nb=0; nb < lisBAR->Count; nb++)
   {
   barra1 = (TBarra *) lisBAR->Items[nb];
   fprintf(arq, "\n\n %-15s       %-5s     %9.3f    ",
           barra1->codigo.c_str(),
           geralC->codigo_tipo_barra[barra1->tipo].c_str(),
           barra1->vnom_kv);
   v1     = geralC->FiltraZero(barra1->tensao_pu);
   p1     = barra1->vet_carga_pu[0].real() * geralC->Sbase;
   q1     = barra1->vet_carga_pu[0].imag() * geralC->Sbase;
   p2     = barra1->scalc_mva.real()       * geralC->Sbase;
   q2     = barra1->scalc_mva.imag()       * geralC->Sbase;
   y1     = barra1->yref_reduc1_pu.real();
   y2     = barra1->yref_reduc1_pu.imag();
   soma1 += p1;
   soma2 += q1;
   soma3 += p2;
   soma4 += q2;
   soma5 += y1;
   soma6 += y2;
   fprintf(arq, "I %12.6f %12.6f   %12.6f %12.6f   %13.6e %13.6e"
                "   %8.6f %9.4f",
           p1, q1, p2, q2, y1, y2, Abs(v1), ArgGrau(v1));
   p1     = barra1->vet_carga_pu[1].real() * geralC->Sbase;
   q1     = barra1->vet_carga_pu[1].imag() * geralC->Sbase;
   y1     = barra1->yref_reduc0_pu.real();
   y2     = barra1->yref_reduc0_pu.imag();
   soma1 += p1;
   soma2 += q1;
   soma7 += y1;
   soma8 += y2;
   fprintf(arq, "\n                                              "
                "S %12.6f %12.6f                               %13.6e %13.6e",
           p1, q1, y1, y2);
   p1     = barra1->vet_carga_pu[2].real() * geralC->Sbase;
   q1     = barra1->vet_carga_pu[2].imag() * geralC->Sbase;
   soma1 += p1;
   soma2 += q1;
   fprintf(arq, "\n                                              "
                "Z %12.6f %12.6f", p1, q1);
   }
fprintf(arq, "\n                                                "
             "------------ ------------   ------------ ------------"
             "   ------------- -------------"
             "\n                                         "
             "Total: %12.6f %12.6f   %12.6f %12.6f   "
             "%13.6e %13.6e"
             "\n                                                   "
             "                                                     "
             "%13.6e %13.6e",
        soma1, soma2, soma3, soma4, soma5, soma6, soma7, soma8);

// Prints branch data
if(geralC->num_open_result == 1)
   {
   fprintf(arq, "\n\n *** REDE REDUZIDA - Dados de ligações equivalentes ***");
   fprintf(arq, "\n\n De              Para            Código            "
                "         Ynodal1/0(i,j) (pu)             "
                "Ynodal1/0(j,i) (pu)\n");
   for(int nl=0; nl < lisTRECHO->Count; nl++)
      {
      ligacao1 = (TTrecho *) lisTRECHO->Items[nl];
      fprintf(arq, "\n %-15s %-15s %-15s"
                   "       %13.6e %13.6e     %13.6e %13.6e"
                   "\n                                        "
                   "               %13.6e %13.6e     %13.6e %13.6e",
              ligacao1->bar1->codigo.c_str(),
              ligacao1->bar2->codigo.c_str(),
              ligacao1->codigo.c_str(),
              ligacao1->Y1ser_pu.real(),      ligacao1->Y1ser_pu.imag(),
              ligacao1->Ynodal1_ji_pu.real(), ligacao1->Ynodal1_ji_pu.imag(),
              ligacao1->Y0ser_pu.real(),      ligacao1->Y0ser_pu.imag(),
              ligacao1->Ynodal0_ji_pu.real(), ligacao1->Ynodal0_ji_pu.imag());
      }
   }
fclose(arq);

// Finalização
ImprimeSeparador(arqRESULT, 1, 125);

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TImpressao1::ImprimeTensaoBarras(void)
   {
   complex<double> cval;
   FILE            *arq;
   TBarra          *bar1;

   arq = geralC->AbreArquivo(arqRESULT);
   if(arq == NULL) return(false);

   fprintf(arq, "\n\n Indice     Barra       tensao (pu)\n");
   for(int n=0; n < rede1->lisBAR->Count; n++)
      {
      bar1 = (TBarra *) rede1->lisBAR->Items[n];
      cval = bar1->tensao_pu;
      fprintf(arq, "\n   %4d     %-15s     (%9.6f %9.6f)   (%9.6f %9.4f)",
              n, bar1->codigo.c_str(), cval.real(), cval.imag(),
              Abs(cval), ArgGrau(cval));
      }
   fclose(arq);

   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TImpressao1::OrdenaBarrasCodigo(void)
{
// Local variables
TBarra *barra1;

// Retorna se a lista 'lisBAR_CODIGO' tiver o mesmo número de
// elementos da lista 'rede1->lisBAR' (já foi montada e não houve
// acréscimo de barras (Estabilidade))
if(lisBAR_CODIGO->Count == rede1->lisBAR->Count) return;

// Inicializa 'lisBAR_CODIGO'
lisBAR_CODIGO->Clear();
for(int i=0; i < rede1->lisBAR->Count; i++)
   {
   barra1 = (TBarra *) rede1->lisBAR->Items[i];
   lisBAR_CODIGO->Add(barra1);
   }

// Ordena 'lisBAR_CODIGO'
lisBAR_CODIGO->Sort(EscolheBarraCodigo);
}

//---------------------------------------------------------------------------
void __fastcall TImpressao1::OrdenaTrafo2(void)
{
// Local variables
TTrafo2 *trafo2;

// Retorna se a lista 'lisTRAFO2_ORD' tiver o mesmo número de
// elementos da lista 'rede1->lisTRAFO2' (já foi montada e não
// houve alteração no número de trafos)
if(lisTRAFO2_ORD->Count == rede1->lisTRAFO2->Count) return;

// Inicializa 'lisTRAFO2_ORD'
lisTRAFO2_ORD->Clear();
for(int i=0; i < rede1->lisTRAFO2->Count; i++)
   {
   trafo2 = (TTrafo2 *) rede1->lisTRAFO2->Items[i];
   lisTRAFO2_ORD->Add(trafo2);
   }

// Ordena 'lisTRAFO2_ORD'
lisTRAFO2_ORD->Sort(EscolheTrafo2);
}

//---------------------------------------------------------------------------
void __fastcall TImpressao1::OrdenaTrechos(void)
{
// Local variables
TTrecho *trec1;

// Retorna se a lista 'lisTRECHO_ORD' tiver o mesmo número de
// elementos da lista 'rede1->lisTRECHO' (já foi montada e não
// houve alteração no número de trechos)
if(lisTRECHO_ORD->Count == rede1->lisTRECHO->Count) return;

// Inicializa 'lisTRECHO_ORD'
lisTRECHO_ORD->Clear();
for(int i=0; i < rede1->lisTRECHO->Count; i++)
   {
   trec1 = (TTrecho *) rede1->lisTRECHO->Items[i];
   lisTRECHO_ORD->Add(trec1);
   }

// Ordena 'lisTRECHO_ORD'
lisTRECHO_ORD->Sort(EscolheTrecho);
}

//---------------------------------------------------------------------------
//eof

