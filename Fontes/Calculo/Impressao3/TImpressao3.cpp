//---------------------------------------------------------------------------
#pragma hdrstop
#include <algorithm>
#include ".\TImpressao3.h"

#include "..\Comum13\Constantes_Calculo.h"
#include "..\Comum13\Estruturas.h"
#include "..\Comum13\TMedidor.h"
#include "..\Comum13\TTempoCPU.h"
#include "..\Comum13\TYnodalJ.h"
#include "..\Rede3\TBancoDF.h"
#include "..\Rede3\TBarra.h"
#include "..\Rede3\TBarraDesc.h"
#include "..\Rede3\TBarraVnomZero.h"
#include "..\Rede3\TCapSerie.h"
#include "..\Rede3\TCarga.h"
#include "..\Rede3\TCNL.h"
#include "..\Rede3\TFase.h"
#include "..\Rede3\TGrupoMutua.h"
#include "..\Rede3\TLigacaoC.h"
#include "..\Rede3\TLigacaoEquiv.h"
#include "..\Rede3\TMutua.h"
#include "..\Rede3\TReg12.h"
#include "..\Rede3\TReg32.h"
#include "..\Rede3\TSup.h"
#include "..\Rede3\TTrafo12.h"
#include "..\Rede3\TTrafo33.h"
#include "..\Rede3\TTrafoEquiv.h"
#include "..\Rede3\TTrafoZZ.h"
#include "..\Rede3\TTrecho.h"
#include "..\Rede3\VTRede3.h"

#include "..\..\Apl\VTApl.h"
#include "..\..\Complexo\Complexo.h"
#include "..\..\Constante\Fases.h"
#include "..\..\Curto\TBarraDefeito.h"
#include "..\..\Curto\TCurtoAux3.h"
#include "..\..\Eqv\VTEqv.h"
#include "..\..\Estimador\TBarraEstado.h"
#include "..\..\Estimador\TYnodalE.h"
#include "..\..\Estimador\VTEstimador.h"
#include "..\..\FluxoH3\RedeH3\TBarraH3.h"
#include "..\..\FluxoH3\RedeH3\TLigacaoCH3.h"
#include "..\..\Matriz\VTMatriz.h"
#include "..\..\NR3\TBarraJ.h"
#include "..\..\SisLin\VTSisLin.h"

#include "..\..\..\DLL_Inc\Funcao.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
TImpressao3 * __fastcall NewObjImpressao3(VTRede3    *rede3,
                                          VTGeralC   *geralC,
                                          TCurtoAux3 *curto_aux3)
   {
   return(new TImpressao3(rede3, geralC, curto_aux3));
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
// Função global para ordenação de TTrafo12
static int __fastcall EscolheTrafo12(void *obj1,
                                     void *obj2)
   {
   // Variáveis locais
   int        num1;
   AnsiString cod_bar1, cod_bar2;
   TBarra     *bar1, *bar2;
   TTrafo12   *trafo1, *trafo2;

   // Primeiro critério: ordem crescente do código da barra inicial
   trafo1   = (TTrafo12 *) obj1;
   trafo2   = (TTrafo12 *) obj2;
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
// Função global para ordenação de TTrafo32
static int __fastcall EscolheTrafo32(void *obj1,
                                     void *obj2)
   {
   // Variáveis locais
   int        num1;
   AnsiString cod_bar1, cod_bar2;
   TBarra     *bar1, *bar2;
   TTrafo32   *trafo1, *trafo2;

   // Primeiro critério: ordem crescente do código da barra inicial
   trafo1   = (TTrafo32 *) obj1;
   trafo2   = (TTrafo32 *) obj2;
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
   int     num1;
   TBarra  *bar1, *bar2;
   TTrecho *trec1, *trec2;

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

   // Terceiro critério: ordem crescente do código do trafo
   return(trec1->codigo.AnsiCompareIC(trec2->codigo));
   }

//---------------------------------------------------------------------------
__fastcall TImpressao3::TImpressao3(VTRede3    *rede3,
                                    VTGeralC   *geralC,
                                    TCurtoAux3 *curto_aux3)
                       :TImpressao(geralC)
   {
   // Salva ponteiros para objetos externos
   this->rede3      = rede3;
   this->curto_aux3 = curto_aux3;

   // Inicializa variáveis locais
   lisSUP_CODIGO  = NULL;
   lisTRAFO12_ORD = new TList;
   lisTRAFO32_ORD = new TList;

   // Manter coerência com 'enum_TIPO_ATERR' em VTGeralC.h
   codigo_ater[0] = 'I'; codigo_ater[1] = 'Z'; codigo_ater[2] = 'S';
   codigo_ater[3] = ' ';

   // Manter coerência com 'enumCU' em VTCurtoAux.h
   codigo_tipo_curto[0] = "TRI"; codigo_tipo_curto[1] = "2F";
   codigo_tipo_curto[2] = "FT";  codigo_tipo_curto[3] = "FTZ";
   codigo_tipo_curto[4] = "2FT";

   // Manter coerência com 'enum_FASES_CURTO' em VTGeralC.h
   codigo_fases_curto[0]  = "AN  "; codigo_fases_curto[1]  = "BN  ";
   codigo_fases_curto[2]  = "CN  "; codigo_fases_curto[3]  = "AT  ";
   codigo_fases_curto[4]  = "BT  "; codigo_fases_curto[5]  = "CT  ";
   codigo_fases_curto[6]  = "AB  "; codigo_fases_curto[7]  = "BC  ";
   codigo_fases_curto[8]  = "CA  "; codigo_fases_curto[9]  = "ABN ";
   codigo_fases_curto[10] = "BCN "; codigo_fases_curto[11] = "CAN ";
   codigo_fases_curto[12] = "ABT "; codigo_fases_curto[13] = "BCT ";
   codigo_fases_curto[14] = "CAT "; codigo_fases_curto[15] = "ABC ";
   codigo_fases_curto[16] = "ABCN"; codigo_fases_curto[17] = "ABCT";

   // Manter coerência com 'enum_LIG_ESTATICO' em VTGeralC.h
   codigo_estatico[0] = "Yaterr"; codigo_estatico[1] = "Yiso  ";
   codigo_estatico[2] = "Triang";

   // Manter coerência com 'enum_LIG_CARGA' em VTGeralC.h
   codigo_fases[0]  = "AN    "; codigo_fases[1]  = "BN    ";
   codigo_fases[2]  = "CN    "; codigo_fases[3]  = "AT    ";
   codigo_fases[4]  = "BT    "; codigo_fases[5]  = "CT    ";
   codigo_fases[6]  = "AB    "; codigo_fases[7]  = "BC    ";
   codigo_fases[8]  = "CA    ", codigo_fases[9]  = "ABN   ";
   codigo_fases[10] = "BCN   "; codigo_fases[11] = "CAN   ";
   codigo_fases[12] = "Yaterr"; codigo_fases[13] = "Yiso  ";
   codigo_fases[14] = "Triang";

   // Manter coerência com 'enum_LIG_ENROL_TRIF' em VTGeralC.h
   codigo_lig_enrol[0] = 'D'; codigo_lig_enrol[1] = 'Y';
   codigo_lig_enrol[2] = 'Y';

   // Manter coerência com 'enum_TIPO_TRAFO12' em VTGeralC.h
   codigo_tipo_12[0] = "AUTO4 "; codigo_tipo_12[1] = "AUTO61";
   codigo_tipo_12[2] = "AUTO62"; codigo_tipo_12[3] = "TRAFO4";
   codigo_tipo_12[4] = "TRAFO5";

   // Manter coerência com 'enum_TIPO_TRAFO32' em VTGeralC.h
   codigo_tipo_32[0] = "AUTODF   "; codigo_tipo_32[1] = "AUTOY    ";
   codigo_tipo_32[2] = "TRAFODD  "; codigo_tipo_32[3] = "TRAFODY1 ";
   codigo_tipo_32[4] = "TRAFODY11"; codigo_tipo_32[5] = "TRAFOYD1 ";
   codigo_tipo_32[6] = "TRAFOYD11"; codigo_tipo_32[7] = "TRAFOYY  ";
   }

//---------------------------------------------------------------------------
__fastcall TImpressao3::~TImpressao3(void)
   {
   // Libera memória
   delete lisSUP_CODIGO;  // Os elementos desta lista são os mesmos de 'rede3->lisSUP'
   delete lisTRAFO12_ORD; // Os elementos desta lista são os mesmos de 'rede3->lisTRAFO12'
   delete lisTRAFO32_ORD; // Os elementos desta lista são os mesmos de 'rede3->lisTRAFO32'
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TImpressao3::CodigoFasesCarga(int lig)
{
if((lig >= 0) && (lig < NUM_LIG_CARGA)) return(codigo_fases[lig]);
else                                    return("");
}

//---------------------------------------------------------------------------
bool __fastcall TImpressao3::FalhaRedundancia(int np)
   {
   // Variáveis locais
   FILE *arq;

   // Abre arquivo
   arq = geralC->AbreArquivo(arqRESULT);
   if(arq == NULL) return(false);
   fprintf(arq, "\n\n *** ATENÇÃO - REDUNDÂNCIA MÍNIMA NÃO "
                "ALCANÇADA NO PATAMAR %d "
                "(var. est.: %d / var. med.: %d) ***",
           (np+1), geralC->num_var_est, rede3->lisMED->Count);
   fclose(arq);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TImpressao3::ImprimeBarrasDesconexas(TList *lisBAR_DESC,
                                                     TList *lisBAR_CONEXAS)
{
int        kprint, num_barras_linha, num_print;
FILE       *arq;
TBarra     *bar1;
TBarraDesc *bar_desc;

// Abre arquivo
arq = geralC->AbreArquivo(arqRESULT);
if(arq == NULL) return(false);

// Inicialização
num_barras_linha = 5;

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
      bar_desc = (TBarraDesc *) lisBAR_DESC->Items[nb];
      bar1     = bar_desc->barra;
      kprint++;
      if(kprint > num_barras_linha)
         {
         fprintf(arq, "\n");
         kprint = 1;
         }
      fprintf(arq, "     %15s", bar1->codigo.c_str());
      for(int nfa=0; nfa < bar_desc->num_fases; nfa++)
         {
         fprintf(arq, " %c", bar1->fases->FaseAsChar(bar_desc->vet_fases[nfa]));
         }
      if(kprint >= num_barras_linha) continue;
      for(int nfa=bar_desc->num_fases; nfa < MAX_FASES_G3; nfa++)
         {
         fprintf(arq, "  ");
         }
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
         fprintf(arq, "     %15s", bar1->codigo.c_str());
         if(kprint < num_barras_linha) fprintf(arq, "        ");
         }
      } // if(num_print)
   } // if(lisBAR_CONEXAS)

// Fecha arquivo
fclose(arq);

// Finalização
ImprimeSeparador(arqRESULT, 1, 132);

// Retorna
return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TImpressao3::ImprimeBarrasSemConvergencia(int    num_bar_sem_conv,
                                                          TBarra **vet_bar_sem_conv)
{
FILE *arq;

// Abre arquivo
arq = geralC->AbreArquivo(arqRESULT);
if(arq == NULL) return(false);

fprintf(arq, "\n *** Barras sem convergência ***\n");
for(int nb=0; nb < num_bar_sem_conv; nb++)
   {
   fprintf(arq, "\n %-15s", vet_bar_sem_conv[nb]->codigo.c_str());
   }
fclose(arq);

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TImpressao3::ImprimeBarrasVnomZero(TList *lisBAR_VNZ)
{
int            kprint, num_barras_linha, num_print;
FILE           *arq;
TBarra         *bar1;
TBarraVnomZero *bar_vnz;

// Abre arquivo
arq = geralC->AbreArquivo(arqRESULT);
if(arq == NULL) return(false);

// Inicialização
num_barras_linha = 5;

// Imprime barras com tensão nominal errada
if(lisBAR_VNZ == NULL)
   {
   fprintf(arq, "\n *** Não há barras com tensão nominal igual a zero ***");
   }
else
   {
   num_print = lisBAR_VNZ->Count;
   if     (num_print == 0)   fprintf(arq, "\n *** Não há barras com tensão nominal igual a zero ***");
   else if(num_print == 1)   fprintf(arq, "\n *** 1 barra com tensão nominal igual a zero ***");
   else if(num_print <= 200) fprintf(arq, "\n *** %d barras com tensão nominal igual a zero ***", num_print);
   else
      {
      num_print = 200;
      fprintf(arq, "\n *** Primeiras %d de %d barras com tensão nominal igual a zero ***",
              num_print, lisBAR_VNZ->Count);
      }
   if(num_print > 0)
      {
      fprintf(arq, "\n");
      kprint = 100;
      for(int nb=0; nb < num_print; nb++)
         {
         bar_vnz = (TBarraVnomZero *) lisBAR_VNZ->Items[nb];
         kprint++;
         if(kprint > num_barras_linha)
            {
            kprint = 1;
            fprintf(arq, "\n");
            }
         fprintf(arq, "   %15s  %-5s",
                 bar_vnz->barra->codigo.c_str(), bar_vnz->fases_vnz.c_str());
         if(kprint < num_barras_linha) fprintf(arq, "   ");
         }
      } // if(num_print)
   } // if(lisBAR_VNZ)

// Fecha arquivo
fclose(arq);

// Finalização
ImprimeSeparador(arqRESULT, 1, 137);

// Retorna
return(true);
}

/*
//---------------------------------------------------------------------------
void __fastcall TImpressao3::ImprimeCargas(void)
{

// Função apenas para debug - 2012_05_24

complex<double> stot;
FILE            *arq;
TBarra          *bar1;
TCarga          *carg1;


// Abre arquivo
arq = AbreArquivo(arqDADOS);
if(arq == NULL) return;

fprintf(arq, "\n\n G3 - Número de cargas: %5d\n", rede3->lisCARGA->Count);
for(int nc=0; nc < rede3->lisCARGA->Count; nc++)
   {
   carg1 = (TCarga *) rede3->lisCARGA->Items[nc];
   bar1  = carg1->barra;
   stot  = complex<double>(0., 0.);
   for(int nl=0; nl < NUM_LIG_CARGA; nl++)
      {
      for(int nm=0; nm < NUM_MODELO_CARGA; nm++)
         {
         stot += bar1->mat_carga->EC2[nl][nm];
         }
      }
   fprintf(arq, "\n %15s   %20.10f %20.10f", bar1->codigo.c_str(),
           stot.real(), stot.imag());
   }
fclose(arq);
}
*/

//---------------------------------------------------------------------------
bool __fastcall TImpressao3::ImprimeDados(int   np,
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
   teste1 = (geralC->Programa1 != progESTIM3) &&
            (geralC->tipo_relatorio >= trREL_DETALHADO_1);
   teste2 = (geralC->Programa1 == progESTIM3) &&
            (geralC->tipo_relatorio >= trREL_DETALHADO_2);
   if(teste1 || teste2)  // Imprime dados completos da rede
      {
      imprimiu = ImprimeDados2(np, lisBAR_J, lisBAR_ESTADO);
      if(imprimiu) num_vezes++;
      }
   if(geralC->Programa1 == progESTIM3)
      {
      imprimiu = ImprimeDadosESTIM3(np, lisBAR_ESTADO);
      if(imprimiu) num_vezes++;
      }
   }

// Imprime separador de patamares
imprimiu = false;
if(num_vezes > 0)
   {
   imprimiu = true;
   ImprimeSeparador(arqDADOS, 1, 145);
   }

return(imprimiu);
}

//---------------------------------------------------------------------------
bool __fastcall TImpressao3::ImprimeDados1(TList *lisBAR_REF)
{
// Local variables
int        num1, num2, num3, num4, num_lig_mutua;
double     rel_dp;
AnsiString string0, string1, string2, string3, string4, string5, string6,
           string7, string8;
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
for(int nl=0; nl < rede3->lisTRECHO->Count; nl++)
   {
   trec1 = (TTrecho *) rede3->lisTRECHO->Items[nl];
   if(trec1->grupo_mutua >= 0) num_lig_mutua++;
   }

// Prints general data
string0 = geralC->codigo_programa[geralC->Programa1];
if(geralC->Programa1 == progNR3)     string0 += " V" + geralC->versao_NR3;
if     (geralC->CargaNula)           string1  = "Sem carga";
else if(geralC->CargaEmYnodal)       string1  = "Todas em Ynodal";
else if(geralC->CargaInjecoesExt)    string1  = "Injeções externas";
else                                 string1  = "";
if(geralC->flag_capac_LT)            string2  = "Sim";
else                                 string2  = "Não";
if(geralC->usa_pi_equiv)             string3  = "Sim";
else                                 string3  = "Não";
if(geralC->flag_ypar_trafos)         string4  = "Sim";
else                                 string4  = "Não";
if(geralC->flag_ater_comuns)         string5  = "Sim";
else                                 string5  = "Não";
if(geralC->todos_sup_barra_interna)  string6  = "Sim";
else                                 string6  = "Não";
if(geralC->flag_TRAFO5_Como_Trafo3E) string7  = "Sim";
else                                 string7  = "Não";
if(geralC->flag_utiliza_bancoDF)     string8  = "Sim";
else                                 string8  = "Não";
fprintf(arq,   "\n *** Dados Gerais ***"
             "\n\n Programa:                                                   %15s"
               "\n Plataforma:                                                           %5s"
               "\n Configuração:                                                       %7s"
               "\n Nome da rede:                           %35s",
        string0.c_str(), geralC->Plataforma.c_str(), geralC->Configuracao.c_str(),
        geralC->NomeRede.c_str());
if((geralC->Programa1 == progG3) || (geralC->Programa1 == progESTIM3) ||
   (geralC->Programa1 == progH3))
   {
   fprintf(arq, "\n Número máximo de iterações:                                          %6d",
           geralC->MaxIteracao[0]);
   }
else if(geralC->Programa1 == progNR3)
   {
   fprintf(arq, "\n Número máximo de iterações - NR:                                   %8d"
					 "\n                            - ajuste de Tap/Qmax:                   %8d"
                "\n                  montagens/fatorações do Jacobiano:                %8d",
           geralC->MaxIteracao[0], geralC->MaxIteracao[1], geralC->MaxIteracao[2]);
   }
fprintf(arq, "\n Tolerância (pu):                                                     %6.0e"
             "\n Potência de base (MVA):                                                %4.0f"
             "\n Representação da carga:                           %25s"
             "\n Capacitância de linhas:                                                 %3s"
             "\n Modelo pi-equivalente para linhas:                                      %3s"
             "\n Ramo paralelo de trafos:                                                %3s",
        geralC->Precisao[0], geralC->Sbase, string1.c_str(), string2.c_str(),
        string3.c_str(), string4.c_str());
if(geralC->Programa1 == progNR3)
   {
   if(geralC->usa_vdnrm) string1 = "Sim";
   else                  string1 = "Não";
   fprintf(arq, "\n Utiliza VDNRM:                                                          %3s",
           string1.c_str());
   }
if(geralC->modelo_eqv != modeloEQV_INDEF)
   {
   fprintf(arq, "\n Modelo da rede equivalente:                                               %1d",
           geralC->modelo_eqv);
   }
fprintf(arq, "\n Aterramentos comuns:                                                    %3s"
             "\n Barra interna de suprimentos:                                           %3s"
             "\n TRAFO5 como Trafo3E:                                                    %3s"
             "\n Utilização de bancos Delta Fechado:                                     %3s",
        string5.c_str(), string6.c_str(), string7.c_str(), string8.c_str());

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
             "\n\n Número de trafos Monofásicos de 2 enrol.:                            %6d"
               "\n                  Trifásicos  de 2 enrol.:                            %6d"
               "\n                  Trifásicos  de 3 enrol.:                            %6d"
               "\n                  Equivalentes:                                       %6d"
               "\n                  ZigZag:                                             %6d"
             "\n\n Número de bancos ligados em Delta Fechado:                           %6d"
             "\n\n Número de reguladores de tensão Monofásicos:                         %6d"
               "\n                                 Trifásicos:                          %6d"
             "\n\n Número de cargas não-lineares:                                       %6d",
        geralC->codigo_tipo_barra[BAR_PQ].c_str(),  geralC->num_barras_PQ,
        geralC->codigo_tipo_barra[BAR_EXT].c_str(), geralC->num_barras_EXT,
        geralC->codigo_tipo_barra[BAR_PQV].c_str(), geralC->num_barras_PQV,
        geralC->codigo_tipo_barra[BAR_PV].c_str(),  geralC->num_barras_PV,
        geralC->codigo_tipo_barra[BAR_SW].c_str(),  geralC->num_barras_SW,
        geralC->codigo_tipo_barra[BAR_INT].c_str(), geralC->num_barras_INT,
        rede3->lisBAR->Count, (rede3->lisTRECHO->Count-num_lig_mutua),
        rede3->lisGRUPO_MUTUA->Count, num_lig_mutua, rede3->lisTRECHO->Count,
        geralC->num_chaves, rede3->lisCAPS->Count, rede3->lisTRAFO12->Count,
        rede3->lisTRAFO32->Count, rede3->lisTRAFO33->Count,
        rede3->lisTRAFOEQUIV->Count, rede3->lisTRAFOZZ->Count,
        rede3->lisBANCO_DF->Count, rede3->lisREG12->Count,
        rede3->lisREG32->Count, rede3->lisCNL->Count);
if(geralC->Programa1 == progH3)
   {
   if(rede3->lisCNL->Count > 0)
      {
      fprintf(arq, "\n Número de harmônicas:                                                %6d"
                   "\n Ordem das harmônicas:  ",
              geralC->NumHarmonicas);
      num1 = 13;  // Número de frequências por linha
      num3 = 0;
      while(num3 < geralC->NumHarmonicas)
         {
         num2  = num3;
         num3 += num1;
         if(num3 > geralC->NumHarmonicas) num3 = geralC->NumHarmonicas;
         if(num2 == 0)  // Primeira linha; desloca offset de impressão
            {
            num4 = num1 - (num3 - num2);
            for(int i=0; i < num4; i++)
               {
               fprintf(arq, "    ");
               }
            }
         else  // Demais linhas; pula linha e define offset
            {
            fprintf(arq, "\n                        ");
            }
         for(int nh=num2; nh < num3; nh++)
            {
            fprintf(arq, " %3d", geralC->OrdemHarmonica(nh));
            }
         } // while()
      } // if(rede3->lisCNL->Count)
   } // if(geralC->Programa1)
if(geralC->Programa1 == progESTIM3)
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
           (rede3->lisMED->Count - geralC->num_med_reais),
           rede3->lisMED->Count, geralC->num_var_est,
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
fprintf(arq, "\n\n Número de linhas/colunas em Ycc:                                     %6d"
               "\n                             Ygg:                                     %6d"
               "\n                             Ynodal:                                  %6d",
        geralC->num_lin_submatriz,
        (geralC->num_tot_linhas - geralC->num_lin_submatriz),
        geralC->num_tot_linhas);
if(geralC->Programa1 == progNR3)
   {
   fprintf(arq, "\n\n Estrutura da matriz Jacobiana:"
                  "\n\n     Equações em Ir:                                                  %6d"
                    "\n                 Im:                                                  %6d"
                    "\n                 P3f:                                                 %6d"
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
bool __fastcall TImpressao3::ImprimeDados2(int   np,
                                           TList *lisBAR_J,
                                           TList *lisBAR_ESTADO)
{
// Local variables
bool            flag_achou;
char            char1, char2, char3, char4, char5, char6;
int             f1, f2, iap, kprint, naux, naux2, nfa, num1, num2, num3,
                num4, num_fases, num_fases_pri, num_fases_sec, num_linhas,
                ordem;
double          fmult, p1, q1, p3, q3, soma1, soma2, soma3, tap1, tap2,
                tap3, ybase;
AnsiString      charstr1, charstr2, charstr3, str1;
complex<double> *pc1, soma_I, soma_S, soma_Z, tensao, z1, zater;
FILE            *arq;
TBancoDF        *bancoDF;
TBarra          *barra1;
TBarraJ         *pbarJ;
TBarraDefeito   *bar_def;
TCapSerie       *caps1;
TCNL            *cnl1;
TFase           *fases, *fases_pri, *fases_sec;
TGrupoMutua     *grupo;
TMutua          *mutua;
TReg12          *reg12;
TReg32          *reg32;
TTrecho         *trec1;
TTrafo12        *trafo12;
TTrafo32        *trafo32;
TTrafo33        *trafo33;
TTrafoEquiv     *trafoequiv;
TTrafoZZ        *trafozz;
TSup            *sup1;

// Abre arquivo
arq = geralC->AbreArquivo(arqDADOS);
if(arq == NULL) return(false);

// Ordena barras, trechos e trafos, se isso não foi feito antes
OrdenaBarrasCodigo();
OrdenaTrechos();
OrdenaTrafo12();
OrdenaTrafo32();

// Monta lista de suprimentos ordenados por código, se necessário
MontaSuprimentosCodigo();

// Imprime dados para cálculo de curto-circuito
if(curto_aux3 != NULL)
   {
   fprintf(arq, "\n\n *** Dados para cálculo de curto-circuito ***");
   for(int nb=0; nb < curto_aux3->lisBAR_DEF->Count; nb++)
      {
      bar_def = (TBarraDefeito *) curto_aux3->lisBAR_DEF->Items[nb];
      barra1  = bar_def->barra;
      if(geralC->num_open_dados == 2) fprintf(arq, "\n");
      fprintf(arq, "\n\n     Barra:                                       %-15s"
                     "\n     Tipo / fases do defeito:                     %s / %s"
                     "\n     Impedância de defeito - FASE          (ohm): (%12.6e %12.6e)"
                     "\n     Impedância de defeito - TERRA         (ohm): (%12.6e %12.6e)"
                     "\n     Impedância de entrada - seq. ZERO     (ohm): (%12.6e %12.6e)"
                     "\n     Impedância de entrada - seq. DIRETA   (ohm): (%12.6e %12.6e)"
                     "\n     Potência de curto-circuito TRIFÁSICO  (MVA): (%12.6e %12.6e)"
                     "\n     Potência de curto-circuito FASE-TERRA (MVA): (%12.6e %12.6e)",
              barra1->codigo.c_str(),
              codigo_tipo_curto[bar_def->tipo_curto].c_str(),
              codigo_fases_curto[bar_def->fases_curto].c_str(),
              curto_aux3->zdef_fase_ohm.real(),  curto_aux3->zdef_fase_ohm.imag(),
              curto_aux3->zdef_terra_ohm.real(), curto_aux3->zdef_terra_ohm.imag(),
              curto_aux3->zent0_ohm[nb].real(),  curto_aux3->zent0_ohm[nb].imag(),
              curto_aux3->zent1_ohm[nb].real(),  curto_aux3->zent1_ohm[nb].imag(),
              curto_aux3->s3f_mva[nb].real(),    curto_aux3->s3f_mva[nb].imag(),
              curto_aux3->sft_mva[nb].real(),    curto_aux3->sft_mva[nb].imag());
      } // for(nb)
   } // if(curto_aux3)

// Prints bus data
if(curto_aux3 == NULL)
   {
   if(geralC->num_open_dados == 2) fprintf(arq, "\n");
   }
fprintf(arq, "\n\n *** Dados de barras - Patamar %d ***"
             "\n\n Código            Tipo      Tens. nom. (kV)     "
             "-Fases-    At.     Imped. aterram. (ohm)      Tipo carga"
             "     ----P(MW)--- ---Q(MVAr)--  Qr-Qc(MVAr)"
             "\n                              Fase     Linha",
        (np+1));
soma1  = 0.;
soma2  = 0.;
soma3  = 0.;
soma_I = czero;
soma_S = czero;
soma_Z = czero;
for(int nb=0; nb < lisBAR_CODIGO->Count; nb++)
   {
   barra1    = (TBarra *) lisBAR_CODIGO->Items[nb];
   fases     = barra1->fases;
   num_fases = fases->NumFases();
   fprintf(arq, "\n\n %-15s   %-5s   %8.3f %8.3f",
           barra1->codigo.c_str(),
           geralC->codigo_tipo_barra[barra1->tipo].c_str(),
           barra1->VnomFase_kv(barra1->fases->Fase(0)),
           barra1->VnomLinha_kv());
   iap      = 0;
   charstr1 = "";
   for(int nfa=0; nfa < num_fases; nfa++)
      {
      charstr1 += fases->FaseAsCharIndice(nfa);
      iap++;
      if(iap >= 7) break;
      charstr1 += " ";
      iap++;
      }
   for(int nfa=iap; nfa < 7; nfa++)
      {
      charstr1 += " ";
      }
   fprintf(arq, "     %7s     ", charstr1.c_str());
   if(barra1->tipo_ater == atZ)
      {
      zater = czero;
      tap1  = barra1->VnomFase_kv(faseN);
      if(tap1 != 0.)
         {
         ybase = geralC->Sbase_fase / (tap1 * tap1);
         zater = cum / (barra1->yater_pu * ybase);
         }
      fprintf(arq, "%c   %12.5e %12.5e   ",
              codigo_ater[barra1->tipo_ater], zater.real(), zater.imag());
      }
   else
      {
      if(fases->ExisteN()) naux = barra1->tipo_ater;
      else                 naux = NUM_TIPO_ATERR;  // A barra não tem neutro

      fprintf(arq, "%c                               ",
              codigo_ater[naux]);
      }
   kprint = 0;

   // Loop de tipos de carga
   for(int lig=0; lig < NUM_LIG_CARGA; lig++)
      {
      if(barra1->CargaNula(lig)) continue; // Não tem carga do tipo 'lig'
      z1      = barra1->mat_carga_mva->C2[lig][Icte];
      p1      = z1.real();
      q1      = z1.imag();
      soma1  += p1;
      soma2  += q1;
      soma_I += complex<double>(p1, q1);
      if(kprint != 0) fprintf(arq, "\n\n                              "
                                   "                                  "
                                   "                             ");
      kprint = 1;
      fprintf(arq, "    %6s     I %12.6f %12.6f",
              codigo_fases[lig].c_str(), p1, q1);
      z1      = barra1->mat_carga_mva->C2[lig][Scte];
      p1      = z1.real();
      q1      = z1.imag();
      soma1  += p1;
      soma2  += q1;
      soma_S += complex<double>(p1, q1);
      fprintf(arq, "\n                                                "
                   "                                                  "
                   "          "
                   "S %12.6f %12.6f", p1, q1);
      z1      = barra1->mat_carga_mva->C2[lig][Zcte];
      p1      = z1.real();
      q1      = z1.imag();
      soma1  += p1;
      soma2  += q1;
      soma_Z += complex<double>(p1, q1);
      fprintf(arq, "\n                                                "
                   "                                                  "
                   "          "
                   "Z %12.6f %12.6f", p1, q1);
      } // for(lig)

   // Loop de tipos de estático de barra
   for(int lig=0; lig < NUM_LIG_ESTATICO; lig++)
      {
      q1 = barra1->vet_qest_mvar[lig];
      if(IsDoubleZero(q1)) continue;  // Não há estático nesta ligação
      q1     = -q1;
      soma3 += q1;
      if(kprint != 0) fprintf(arq, "\n\n                              "
                                   "                                  "
                                   "                             ");
      kprint = 1;
      fprintf(arq, "    %6s                                 %12.6f",
              codigo_estatico[lig].c_str(), q1);
      }

   // Imprime matriz de admitâncias da redução, se houver algum
   // elemento diferente de zero
   if(geralC->num_open_dados == 2)
      {
      flag_achou = false;
      for(int nfa=0; nfa < num_fases; nfa++)
         {
         for(int nfa2=0; nfa2 < num_fases; nfa2++)
            {
            if(Abs(barra1->mat_ynodal_reduc_pu->C2[nfa][nfa2]) > 0.)
               {
               flag_achou = true;
               break;
               }
            }
         }
      if(flag_achou)
         {
         fprintf(arq, "\n\n   ---------------------------------Matriz de "
                      "admitâncias nodais (redução de rede) (pu)"
                      "---------------------------------"
                      "       -------Ynodal1/0 (pu)------\n");
         num1  = 0;
         for(int nfa=0; nfa < num_fases; nfa++)
            {
            fprintf(arq, "\n");
            for(int nfa2=0; nfa2 < num_fases; nfa2++)
               {
               z1 = barra1->mat_ynodal_reduc_pu->C2[nfa][nfa2];
               fprintf(arq, "   %13.6e %13.6e", z1.real(), z1.imag());
               }
            for(int nfa2=num_fases; nfa2 < MAX_FASES_G3; nfa2++)
               {
               fprintf(arq, "                              ");
               }
            if(nfa == 0)
               {
               num1++;
               z1 = barra1->Ynodal1_pu;
               fprintf(arq, "       %13.6e %13.6e", z1.real(), z1.imag());
               }
            else if(nfa == 1)
               {
               num1++;
               z1 = barra1->Ynodal0_pu;
               fprintf(arq, "       %13.6e %13.6e", z1.real(), z1.imag());
               }
            if((nfa >= 1) && (num1 < 2)) // Não imprimiu 'Ynodal0_pu', imprime agora
               {
               z1 = barra1->Ynodal0_pu;
               fprintf(arq, "       %13.6e %13.6e", z1.real(), z1.imag());
               }
            }
         }
      }
   } // for(nb)
fprintf(arq, "\n\n                                                    "
             "                         "
             "                                 ------------ ------------"
             " ------------"
             "\n                                                      "
             "                                                 "
             "Total: %12.6f %12.6f %12.6f"
             "\n\n                                                      "
             "                                                "
             "Soma_I: %12.6f %12.6f"
             "\n                                                      "
             "                                                "
             "Soma_S: %12.6f %12.6f"
             "\n                                                      "
             "                                                "
             "Soma_Z: %12.6f %12.6f",
        soma1, soma2, soma3, soma_I.real(), soma_I.imag(),
        soma_S.real(), soma_S.imag(), soma_Z.real(), soma_Z.imag());

// Imprime dados de geradores (1a. parte)
fprintf(arq, "\n\n *** Dados de geradores ***"
             "\n                                                              "
             "   -------------------------Tensões (pu / °)-------------------------"
             "\n Código                 Tipo      Pot.nom.(MVA)    -Fases-    "
             "      Mod.VAN   Ang.VAN      Mod.VBN   Ang.VBN      Mod.VCN   Ang.VCN\n");
num1 = 0;  // Número de suprimentos PQ e PV (impressão de potências, 2a. parte)
for(int ns=0; ns < lisSUP_CODIGO->Count; ns++)
   {
   sup1      = (TSup *) lisSUP_CODIGO->Items[ns];
   barra1    = sup1->bar_int;
   fases     = sup1->fases;
   num_fases = fases->NumFases();
   if((barra1->tipo == BAR_PQ) || (barra1->tipo == BAR_PV)) num1++;
   fprintf(arq, "\n %-15s        %5s     %11.6f     ",
           barra1->codigo.c_str(),
           geralC->codigo_tipo_barra[barra1->tipo].c_str(),
           sup1->snom_mva);
   for(int nfa=0; nfa < num_fases; nfa++)
      {
      fprintf(arq, " %c", fases->FaseAsCharIndice(nfa));
      }
   naux = 2 * num_fases;
   for(int nfa=naux; nfa < 8; nfa++)
      {
      fprintf(arq, " ");
      }
   fprintf(arq, "    ");
   iap = 0;
   for(nfa=0; nfa < MAX_FASES_G3; nfa++)
      {
      if     (nfa == 0) f1 = faseA;
      else if(nfa == 1) f1 = faseB;
      else if(nfa == 2) f1 = faseC;
      else              continue;  // Pula neutro
      if(fases->ExisteFase(f1))
         {
         if(barra1->tipo == BAR_PV)
            {
            char1 = ' ';
            if(lisBAR_J != NULL)
               {
               num1 = rede3->lisBAR->IndexOf(barra1);
               if(num1 >= 0)
                  {
                  pbarJ = (TBarraJ *) lisBAR_J->Items[num1];
                  if(pbarJ->fase_int_ref == f1) char1 = 'R';
                  }
               }
            tensao = geralC->FiltraZero(sup1->vet_tensao_pu[iap]);
            fprintf(arq, "   %c %8.6f          ", char1, Abs(tensao));
            }
         else if(barra1->tipo == BAR_SW)
            {
            tensao = geralC->FiltraZero(sup1->vet_tensao_pu[iap]);
            fprintf(arq, "     %8.6f %9.4f",
                    Abs(tensao), ArgGrau(tensao));
            }
         iap++;
         }
      else
         {
         fprintf(arq, "                       ");
         }
      }
   }

// Imprime dados de geradores (2a. parte)
if(num1 > 0)  // Há suprimentos PQ ou PV
   {
   fprintf(arq, "\n\n Código                 Tipo     PV->PQ        "
                "--------------------------Potências (MW / MVAr)"
                "--------------------------      F.pot.mín. (pu)\n");
   for(int ns=0; ns < lisSUP_CODIGO->Count; ns++)
      {
      sup1   = (TSup *) lisSUP_CODIGO->Items[ns];
      barra1 = sup1->bar_int;
      if(barra1->tipo == barra1->tipo_original) char1 = ' ';
      else                                      char1 = 'S';
      fprintf(arq, "\n %-15s        %5s       %c        ",
              barra1->codigo.c_str(),
              geralC->codigo_tipo_barra[barra1->tipo].c_str(), char1);
      iap = 0;
      for(nfa=0; nfa < MAX_FASES_G3; nfa++)
         {
         if     (nfa == 0) f1 = faseA;
         else if(nfa == 1) f1 = faseB;
         else if(nfa == 2) f1 = faseC;
         else              continue;  // Pula neutro
         if(sup1->fases->ExisteFase(f1))
            {
            if(barra1->tipo == BAR_PQ)
               {
               fprintf(arq, "  %11.4e %11.4e",
                       sup1->vet_sesp_mva[iap].real(), sup1->vet_sesp_mva[iap].imag());
               }
            else if(barra1->tipo == BAR_PV)
               {
               fprintf(arq, "  %11.4e           -",
                       sup1->vet_sesp_mva[iap].real());
               }
            else  // BAR_SW
               {
               fprintf(arq, "            -           -");
               }
            iap++;
            }
         else
            {
            fprintf(arq, "            -           -");
            }
         }
      if((barra1->tipo == BAR_PV) || (barra1->tipo == BAR_PQ)) // Pode haver mudança de tipo (QmaxPV)
         {
         fprintf(arq, "         %8.6f", sup1->fp_min);
         }
      else
         {
         fprintf(arq, "                -");
         }
      }
   }

// Imprime dados de geradores (3a. parte)
if((geralC->num_open_dados == 2) && (lisSUP_CODIGO->Count > 0))
   {
   fprintf(arq, "\n\n Código               Pot. cto.cto. FT (MVA)"
                "         Pot. cto.cto. 3F (MVA)"
                "           rt (pu)         xm (pu)               "
                "zp (pu)\n");
   for(int ns=0; ns < lisSUP_CODIGO->Count; ns++)
      {
      sup1   = (TSup *) lisSUP_CODIGO->Items[ns];
      barra1 = sup1->bar_int;
      switch(sup1->fases->NumFasesSemNT())
         {
         case 1:  // Suprimento monofásico
            p1 = sup1->pot_curto1_pu.real() * geralC->Sbase_fase;
            q1 = sup1->pot_curto1_pu.imag() * geralC->Sbase_fase;
            z1 = sup1->zp_pu;
            fprintf(arq, "\n %-15s   %13.6e %13.6e                      "
                         "                                              "
                         "%13.6e %13.6e",
                    barra1->codigo.c_str(), p1, q1, z1.real(), z1.imag());
            break;

         case 2:  // Suprimento bifásico
            p1 = sup1->pot_curto1_pu.real() * geralC->Sbase_fase;
            q1 = sup1->pot_curto1_pu.imag() * geralC->Sbase_fase;
            p3 = sup1->pot_curto3_pu.real() * geralC->Sbase_fase;
            q3 = sup1->pot_curto3_pu.imag() * geralC->Sbase_fase;
            z1 = sup1->zp_pu;
            fprintf(arq, "\n %-15s   %13.6e %13.6e    %13.6e %13.6e     %13.6e"
                         "   %13.6e   %13.6e %13.6e",
                    barra1->codigo.c_str(), p1, q1, p3, q3,
                    sup1->rt_pu, sup1->xm_pu, z1.real(), z1.imag());
            break;

         case 3:  // Suprimento trifásico
            p1 = sup1->pot_curto1_pu.real() * geralC->Sbase;
            q1 = sup1->pot_curto1_pu.imag() * geralC->Sbase;
            p3 = sup1->pot_curto3_pu.real() * geralC->Sbase;
            q3 = sup1->pot_curto3_pu.imag() * geralC->Sbase;
            z1 = sup1->zp_pu;
            fprintf(arq, "\n %-15s   %13.6e %13.6e    %13.6e %13.6e     %13.6e"
                         "   %13.6e   %13.6e %13.6e",
                    barra1->codigo.c_str(), p1, q1, p3, q3,
                    sup1->rt_pu, sup1->xm_pu, z1.real(), z1.imag());
         }
      }
   }

// Prints branch data
if((geralC->num_open_dados == 2) && (lisTRECHO_ORD->Count > 0))
   {
   // num1 = 3;  // Define valor/unidade a ser impresso (imped. ou admit. série)
   // num2 = 1;  // Define valor/unidade a ser impresso (admitância paralelo)
   num1 = 0;
   num2 = 0;
   fprintf(arq, "\n\n\n *** Dados de trechos ");
   if     (num1 == 0) fprintf(arq, "(Yserie (pu) / ");
   else if(num1 == 1) fprintf(arq, "(Zserie (pu) / ");
   else if(num1 == 2) fprintf(arq, "(Zserie (ohm) / ");
   else               fprintf(arq, "(Zserie (ohm/km) / ");
   if     (num2 == 0) fprintf(arq, "Ypar_tot_nodal (pu)) ***");
   else               fprintf(arq, "Ypar_tot_nodal (S/km)) ***");
   fprintf(arq, "\n\n De              Para            Código            "
                "Mút.    Compr.(km)     -Fases-     "
                "--------Corr. admissível (A)-------");
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
      fases     = trec1->fases;
      num_fases = fases->NumFases();
      if(! trec1->MontaYserie(1))  // Erro na montagem da matriz
         {
         fclose(arq);
         return(false);
         }
      if(trec1->grupo_mutua >= 0)
         {
         char1 = 'M';
         str1  = AnsiString(trec1->grupo_mutua);
         }
      else
         {
         char1 = ' ';
         str1  = "  ";
         }
      fprintf(arq, "\n\n %-15s %-15s %-15s    %c %2s  %10.5f     ",
              charstr1.c_str(),
              charstr2.c_str(),
              trec1->codigo.c_str(), char1, str1.c_str(),
              trec1->compr_km);
      for(int nfa=0; nfa < num_fases; nfa++)
         {
         fprintf(arq, " %c", fases->FaseAsCharIndice(nfa));
         }
      for(int nfa=num_fases; nfa < MAX_FASES_G3; nfa++)
         {
         fprintf(arq, "  ");
         }
      fprintf(arq, "    ");
      for(int nfa=0; nfa < num_fases; nfa++)
         {
         fprintf(arq, " %8.1f", trec1->vet_iadm_a[nfa]);
         }

      // Imprime Zser/Yser
      for(int nfa=0; nfa < num_fases; nfa++)
         {
         f1    = fases->Fase(nfa);
         fmult = geralC->Sbase_fase / trec1->bar1->VnomFase_kv(f1);
         fprintf(arq, "\n");
         for(int nfa2=0; nfa2 < num_fases; nfa2++)
            {
            f2    = fases->Fase(nfa2);
            ybase = fmult / trec1->bar1->VnomFase_kv(f2);
            if     (num1 == 0) z1 = trec1->MatYserie_pu[nfa][nfa2];
            else if(num1 == 1) z1 = trec1->MatZserie_pu[nfa][nfa2];
            else if(num1 == 2) z1 = trec1->MatZserie_pu[nfa][nfa2] / ybase;
            else               z1 = trec1->MatZserie_pu[nfa][nfa2] / (ybase * trec1->compr_km);
            fprintf(arq, "  %13.6e %13.6e", z1.real(), z1.imag());
            }
         }

      // Imprime matriz de admitâncias-paralelo de elementos do trecho,
      // se houver algum elemento diferente de zero
      if(geralC->flag_capac_LT)
         {
         flag_achou = false;
         for(int nfa=0; nfa < num_fases; nfa++)
            {
            for(int nfa2=0; nfa2 < num_fases; nfa2++)
               {
               if(Abs(trec1->MatYparalelo_pu[nfa][nfa2]) > 0.)
                  {
                  flag_achou = true;
                  break;
                  }
               }
            if(flag_achou) break;
            }
         if(flag_achou)
            {
            fprintf(arq, "\n");
            for(int nfa=0; nfa < num_fases; nfa++)
               {
               f1    = fases->Fase(nfa);
               fmult = geralC->Sbase_fase / trec1->bar1->VnomFase_kv(f1);
               fprintf(arq, "\n");
               for(int nfa2=0; nfa2 < num_fases; nfa2++)
                  {
                  f2    = fases->Fase(nfa2);
                  ybase = fmult / trec1->bar1->VnomFase_kv(f2);
                  if(num2 == 0) z1 = trec1->MatYparalelo_pu[nfa][nfa2];
                  else          z1 = trec1->MatYparalelo_pu[nfa][nfa2] * ybase / trec1->compr_km;
                  fprintf(arq, "  %13.6e %13.6e", z1.real(), z1.imag());
                  }
               }
            }
         }
      }
   }

// Imprime dados de mútuas
if(geralC->num_open_dados == 2)
   {
   num_linhas = 0;
   for(int ng=0; ng < rede3->lisGRUPO_MUTUA->Count; ng++)
      {
      grupo = (TGrupoMutua *) rede3->lisGRUPO_MUTUA->Items[ng];
      for(int nm=0; nm < grupo->lisMUTUA->Count; nm++)
         {
         mutua = (TMutua *) grupo->lisMUTUA->Items[nm];
         if(num_linhas == 0)
            {
            num_linhas = 1;
            fprintf(arq, "\n\n\n *** Dados de impedâncias mútuas ***"
                         "\n\n --------------------Trecho 1-------------------"
                         "     --------------------Trecho 2-------------------"
                         "       Grupo     Xm0 (ohm)   Xm1 (ohm)\n");
            }
         fprintf(arq, "\n %-15s %-15s %-15s     %-15s %-15s %-15s        "
                      "%3d     %9.4f   %9.4f",
                 mutua->trec1->bar1->codigo.c_str(),
                 mutua->trec1->bar2->codigo.c_str(),
                 mutua->trec1->codigo.c_str(),
                 mutua->trec2->bar1->codigo.c_str(),
                 mutua->trec2->bar2->codigo.c_str(),
                 mutua->trec2->codigo.c_str(),
                 ng, mutua->z0_ohm.imag(), mutua->z1_ohm.imag());
         }
      }
   }

// Imprime dados de capacitores série
if((geralC->num_open_dados == 2) && (rede3->lisCAPS->Count > 0))
   {
   fprintf(arq, "\n\n\n *** Dados de capacitores série ***"
                "\n\n De              Para            Código            "
                "    Impedância série (ohm)       -Fases-\n");
   for(int ncs=0; ncs < rede3->lisCAPS->Count; ncs++)
      {
      caps1 = (TCapSerie *) rede3->lisCAPS->Items[ncs];
      zater = czero;
      if(Abs(caps1->yser_s) > 0.) zater = cum / caps1->yser_s;
      fprintf(arq, "\n %-15s %-15s %-15s    %13.6e %13.6e    ",
              caps1->bar1->codigo.c_str(),
              caps1->bar2->codigo.c_str(),
              caps1->codigo.c_str(),
              zater.real(), zater.imag());
      for(int nfa=0; nfa < caps1->fases->NumFases(); nfa++)
         {
         fprintf(arq, " %c", caps1->fases->FaseAsCharIndice(nfa));
         }
      }
   }

// Imprime dados de transformadores monofásicos de 2 enrolamentos
if((geralC->num_open_dados == 2) && (lisTRAFO12_ORD->Count > 0))
   {
   fprintf(arq, "\n\n\n *** Dados de trafos monofásicos de 2 enrolamentos ***"
                "\n\n De              Para               Fases       "
                "Tensões nom. (kV)"
                "    P.nom.(MVA)        Imped. (pu)       At     Imp. aterr. "
                "(ohm)       Tap (pu)"
                "\n                                  Prim. Sec.      "
                "Prim.      Sec.\n");
   for(int nt=0; nt < lisTRAFO12_ORD->Count; nt++)
      {
      trafo12 = (TTrafo12 *) lisTRAFO12_ORD->Items[nt];
      if((trafo12->tipo == AUTOTRAFO4) || (trafo12->tipo == AUTOTRAFO6_1T) ||
			(trafo12->tipo == AUTOTRAFO6_2T))
         {
         soma1 = trafo12->snom_mva / (trafo12->vnom2_kv * trafo12->vnom2_kv); // Ref. secundário
         }
      else  // TRAFO4 ou TRAFO5
         {
         soma1 = trafo12->snom_mva / (trafo12->vnom1_kv * trafo12->vnom1_kv); // Ref. primário
         }
      tensao    = trafo12->ztrafo_ohm * soma1;
		tap1      = trafo12->vet_tap[0] * trafo12->vnom1_kv / trafo12->vnom2_kv;
      num_fases = trafo12->fases_pri->NumFases();
      char1     = trafo12->fases_pri->FaseAsCharIndice(0);
      char2     = ' ';
      char3     = ' ';
      if(num_fases >= 2) char2 = trafo12->fases_pri->FaseAsCharIndice(1);
      if(num_fases >= 3) char3 = trafo12->fases_pri->FaseAsCharIndice(2);
      num_fases = trafo12->fases_sec->NumFases();
      char4     = trafo12->fases_sec->FaseAsCharIndice(0);
      char5     = ' ';
      char6     = ' ';
      if(num_fases >= 2) char5 = trafo12->fases_sec->FaseAsCharIndice(1);
      if(num_fases >= 3) char6 = trafo12->fases_sec->FaseAsCharIndice(2);
      fprintf(arq, "\n %-15s %-15s   %c%c%c  %c%c%c   %8.3f   %8.3f     "
                   "%8.4f     %9.6f %9.6f    %c%c",
              trafo12->bar1->codigo.c_str(),
              trafo12->bar2->codigo.c_str(),
              char1, char2, char3, char4, char5, char6,
              trafo12->vnom1_kv, trafo12->vnom2_kv,
              trafo12->snom_mva, tensao.real(), tensao.imag(),
              codigo_ater[trafo12->tipo_ater1],
              codigo_ater[trafo12->tipo_ater2]);
      if((trafo12->tipo_ater1 == atZ) && (trafo12->tipo_ater2 == atZ) &&
         (! geralC->flag_ater_comuns))
         {
         soma2 = trafo12->bar1->VnomFase_kv(faseN);
         ybase = geralC->Sbase_fase / (soma2 * soma2);
         zater = cum / (trafo12->yater1_pu * ybase);
			fprintf(arq, "  %10.3e %10.3e     1:%8.6f",
					  zater.real(), zater.imag(), tap1);
			if(trafo12->NumTaps == 2) fprintf(arq, "  1:%8.6f", trafo12->vet_tap[1]);
         soma2 = trafo12->bar2->VnomFase_kv(faseN);
         ybase = geralC->Sbase_fase / (soma2 * soma2);
			zater = cum / (trafo12->yater2_pu * ybase);
         fprintf(arq, "\n                                  "
                      "                                    "
                      "                                    "
                      "    %10.3e %10.3e",
                 zater.real(), zater.imag());
         }
      else if(trafo12->tipo_ater1 == atZ)
         {
         soma2 = trafo12->bar1->VnomFase_kv(faseN);
         ybase = geralC->Sbase_fase / (soma2 * soma2);
         zater = cum / (trafo12->yater1_pu * ybase);
         fprintf(arq, "  %10.3e %10.3e     1:%8.6f",
					  zater.real(), zater.imag(), tap1);
			if(trafo12->NumTaps == 2) fprintf(arq, "  1:%8.6f", trafo12->vet_tap[1]);
         }
      else if(trafo12->tipo_ater2 == atZ)
         {
         soma2 = trafo12->bar2->VnomFase_kv(faseN);
         ybase = geralC->Sbase_fase / (soma2 * soma2);
         zater = cum / (trafo12->yater2_pu * ybase);
         fprintf(arq, "  %10.3e %10.3e     1:%8.6f",
					  zater.real(), zater.imag(), tap1);
			if(trafo12->NumTaps == 2) fprintf(arq, "  1:%8.6f", trafo12->vet_tap[1]);
			}
      else
			{
         fprintf(arq, "                            1:%8.6f",
					  tap1);
			if(trafo12->NumTaps == 2) fprintf(arq, "  1:%8.6f", trafo12->vet_tap[1]);
         }
      p1 = trafo12->ypar_S.real() / soma1;
		if((trafo12->tipo == AUTOTRAFO6_1T) || (trafo12->tipo == AUTOTRAFO6_2T))
         {
         p1 *= 2.;                 // COM ou SEM neutro (duas pernas)
         if(trafo12->NeutroSec())  // COM neutro: acerto pela tensão de fase
            {
            fmult  = trafo12->bar2->VnomFase_kv(trafo12->fases_sec->Fase(0))
                   / trafo12->vnom2_kv;
            p1    *= (fmult * fmult);
            }
         }
      str1 = " ";
      if(trafo12->bancoDF != NULL)
         {
         naux = rede3->lisBANCO_DF->IndexOf(trafo12->bancoDF) + 1;
         str1.sprintf("%d", naux);
         }
      fprintf(arq, "\n      %-15s     %6s     Pfe: %6.4f pu     Banco DF: %s",
              trafo12->codigo.c_str(), codigo_tipo_12[trafo12->tipo].c_str(),
              p1, str1.c_str());
      if(trafo12->fase_comum >= 0)
         fprintf(arq, "     Fase comum: %c",
                 trafo12->fases_pri->FaseAsChar(trafo12->fase_comum));
      }
   }

// Imprime dados de transformadores trifásicos de 2 enrolamentos
if((geralC->num_open_dados == 2) && (lisTRAFO32_ORD->Count > 0))
   {
   fprintf(arq, "\n\n\n *** Dados de trafos trifásicos de 2 "
                "enrolamentos ***"
                "\n\n De              Para             Lig.    "
                "Tensões nom. (kV)     "
                "P.nom.(MVA)      Imped. (pu)      At     Imp. aterr. (ohm)"
                "               Taps (pu)"
                "\n                                           "
                "Prim.      Sec.\n");
   for(int nt=0; nt < lisTRAFO32_ORD->Count; nt++)
      {
      trafo32 = (TTrafo32 *) lisTRAFO32_ORD->Items[nt];
      soma1   = trafo32->snom_mva / (trafo32->vnom2_kv * trafo32->vnom2_kv);
      tensao  = trafo32->ztrafo_ohm * soma1;
      p1      = trafo32->ypar_S.real() / soma1;
      soma1   = trafo32->vnom1_kv / trafo32->vnom2_kv;
      tap1    = trafo32->vet_tap[0] * soma1;
      tap2    = trafo32->vet_tap[1] * soma1;
      tap3    = trafo32->vet_tap[2] * soma1;
      fprintf(arq, "\n %-15s %-15s   %c%c   %8.3f   %8.3f  "
                   "    %8.4f   %9.6f %9.6f   %c%c",
              trafo32->bar1->codigo.c_str(),
              trafo32->bar2->codigo.c_str(),
              codigo_lig_enrol[trafo32->tipo_lig1],
              codigo_lig_enrol[trafo32->tipo_lig2],
              trafo32->vnom1_kv, trafo32->vnom2_kv,
              trafo32->snom_mva,
              tensao.real(), tensao.imag(),
              codigo_ater[trafo32->tipo_ater1],
              codigo_ater[trafo32->tipo_ater2]);
      if((trafo32->tipo_ater1 == atZ) && (trafo32->tipo_ater2 == atZ) &&
         (! geralC->flag_ater_comuns))
         {
         soma2 = trafo32->bar1->VnomFase_kv(faseN);
         ybase = geralC->Sbase_fase / (soma2 * soma2);
         zater = cum / (trafo32->yater1_pu * ybase);
         fprintf(arq, "  %10.3e %10.3e  1:%8.6f 1:%8.6f 1:%8.6f",
                 zater.real(), zater.imag(), tap1, tap2, tap3);
         soma2 = trafo32->bar2->VnomFase_kv(faseN);
         ybase = geralC->Sbase_fase / (soma2 * soma2);
         zater = cum / (trafo32->yater2_pu * ybase);
         fprintf(arq, "\n      %-15s"
                      "  %-9s   NF: %1d/%1d   Pfe: %6.4f pu    "
                      "                                       "
                      "%10.3e %10.3e",
                 trafo32->codigo.c_str(), codigo_tipo_32[trafo32->tipo].c_str(),
                 trafo32->fases_pri->NumFases(), trafo32->fases_sec->NumFases(),
                 p1, zater.real(), zater.imag());
         }
      else if(trafo32->tipo_ater1 == atZ)
         {
         soma2 = trafo32->bar1->VnomFase_kv(faseN);
         ybase = geralC->Sbase_fase / (soma2 * soma2);
         zater = cum / (trafo32->yater1_pu * ybase);
         fprintf(arq, "  %10.3e %10.3e  1:%8.6f 1:%8.6f 1:%8.6f",
                 zater.real(), zater.imag(), tap1, tap2, tap3);
         fprintf(arq, "\n      %-15s  %-9s   NF: %1d/%1d   Pfe: %6.4f pu",
                 trafo32->codigo.c_str(), codigo_tipo_32[trafo32->tipo].c_str(),
                 trafo32->fases_pri->NumFases(), trafo32->fases_sec->NumFases(),
                 p1);
         }
      else if(trafo32->tipo_ater2 == atZ)
         {
         soma2 = trafo32->bar2->VnomFase_kv(faseN);
         ybase = geralC->Sbase_fase / (soma2 * soma2);
         zater = cum / (trafo32->yater2_pu * ybase);
         fprintf(arq, "  %10.3e %10.3e  1:%8.6f 1:%8.6f 1:%8.6f",
                 zater.real(), zater.imag(), tap1, tap2, tap3);
         fprintf(arq, "\n      %-15s  %-9s   NF: %1d/%1d   Pfe: %6.4f pu",
                 trafo32->codigo.c_str(), codigo_tipo_32[trafo32->tipo].c_str(),
                 trafo32->fases_pri->NumFases(), trafo32->fases_sec->NumFases(),
                 p1);
         }
      else
         {
         fprintf(arq, "                         1:%8.6f 1:%8.6f 1:%8.6f",
                 tap1, tap2, tap3);
         fprintf(arq, "\n      %-15s  %-9s   NF: %1d/%1d   Pfe: %6.4f pu",
                 trafo32->codigo.c_str(), codigo_tipo_32[trafo32->tipo].c_str(),
                 trafo32->fases_pri->NumFases(), trafo32->fases_sec->NumFases(),
                 p1);
         }
      }
   }

// Imprime dados de transformadores trifásicos de 3 enrolamentos
if((geralC->num_open_dados == 2) && (rede3->lisTRAFO33->Count > 0))
   {
   fprintf(arq, "\n\n\n *** Dados de trafos trifásicos de 3 enrolamentos ***"
                "\n\n Barra              Código           Ref       Tipo    At"
                "   NF   Pot. nom.   Vnom/Vtap                               "
                "    Impedâncias                             Perda Fe"
                "\n                                               lig.        "
                "        (MVA)                         Trafo (pu base %5.1f MVA)"
                "           Aterramento (ohm)      (pu base %5.1f MVA)",
           geralC->Sbase, geralC->Sbase);
   for(int nt=0; nt < rede3->lisTRAFO33->Count; nt++)
      {
      fprintf(arq, "\n");
      trafo33 = (TTrafo33 *) rede3->lisTRAFO33->Items[nt];
      char1   = char2 = char3 = char4 = char5 = char6 = ' ';
      if     (trafo33->enrol_Sref == 0) char1 = 'S';
      else if(trafo33->enrol_Sref == 1) char3 = 'S';
      else                              char5 = 'S';
      if     (trafo33->enrol_Ypar == 0) char2 = 'Y';
      else if(trafo33->enrol_Ypar == 1) char4 = 'Y';
      else                              char6 = 'Y';
      fprintf(arq, "\n P %-15s  %-15s   %c%c         %c      %c    %1d   %6.1f"
                   "       %8.6f       PS %13.6e %13.6e",
              trafo33->bar1->codigo.c_str(),
              trafo33->codigo.c_str(),
              char1,
              char2,
              codigo_lig_enrol[trafo33->tipo_lig1],
              codigo_ater[trafo33->tipo_ater1],
              trafo33->fases_pri->NumFases(),
              trafo33->snom1_mva,
              (trafo33->vnom1_kv/trafo33->vtap1_kv),
              trafo33->zps1_pu.real(),
              trafo33->zps1_pu.imag());
      if(trafo33->tipo_ater1 == atZ)
         {
         soma2 = trafo33->bar1->VnomFase_kv(faseN);
         ybase = geralC->Sbase_fase / (soma2 * soma2);
         zater = cum / (trafo33->yater1_pu * ybase);
         fprintf(arq, "    %13.6e %13.6e",
                 zater.real(), zater.imag());
         }
      else
         {
         fprintf(arq, "                               ");
         }
      fprintf(arq, "      %9.6f", trafo33->ypar_pu.real());

      fprintf(arq, "\n S %-15s  %-7s           %c%c         %c      %c"
                   "    %1d   %6.1f       %8.6f       ST %13.6e %13.6e",
              trafo33->bar2->codigo.c_str(),
              geralC->codigo_tipo_33[trafo33->tipo].c_str(),
              char3,
              char4,
              codigo_lig_enrol[trafo33->tipo_lig2],
              codigo_ater[trafo33->tipo_ater2],
              trafo33->fases_sec->NumFases(),
              trafo33->snom2_mva,
              (trafo33->vnom2_kv/trafo33->vtap2_kv),
              trafo33->zst1_pu.real(),
              trafo33->zst1_pu.imag());
      if(trafo33->tipo_ater2 == atZ)
         {
         soma2 = trafo33->bar2->VnomFase_kv(faseN);
         ybase = geralC->Sbase_fase / (soma2 * soma2);
         zater = cum / (trafo33->yater2_pu * ybase);
         fprintf(arq, "    %13.6e %13.6e",
                 zater.real(), zater.imag());
         }

      fprintf(arq, "\n T %-15s                    %c%c         %c      %c"
                   "    %1d   %6.1f       %8.6f       TP %13.6e %13.6e",
              trafo33->bar3->codigo.c_str(),
              char5,
              char6,
              codigo_lig_enrol[trafo33->tipo_lig3],
              codigo_ater[trafo33->tipo_ater3],
              trafo33->fases_ter->NumFases(),
              trafo33->snom3_mva,
              (trafo33->vnom3_kv/trafo33->vtap3_kv),
              trafo33->zpt1_pu.real(),
              trafo33->zpt1_pu.imag());
      if(trafo33->tipo_ater3 == atZ)
         {
         soma2 = trafo33->bar3->VnomFase_kv(faseN);
         ybase = geralC->Sbase_fase / (soma2 * soma2);
         zater = cum / (trafo33->yater3_pu * ybase);
         fprintf(arq, "    %13.6e %13.6e",
                 zater.real(), zater.imag());
         }
      }
   }

// Imprime dados de transformadores equivalentes
if((geralC->num_open_dados == 2) && (rede3->lisTRAFOEQUIV->Count > 0))
   {
   fprintf(arq, "\n\n\n *** Dados de trafos equivalentes - "
                "redução da rede (submatrizes Ynodal(i,j) e Ynodal(j,i) (pu)) ***"
                "\n\n De              Para            Código          "
                "Def.P->S (°)     --Fases--          Ynodal1/0(i,j) (pu)      "
                "     Ynodal1/0(j,i) (pu)");
   for(int nt=0; nt < rede3->lisTRAFOEQUIV->Count; nt++)
      {
      trafoequiv    = (TTrafoEquiv *) rede3->lisTRAFOEQUIV->Items[nt];
      fases_pri     = trafoequiv->fases_pri;
      fases_sec     = trafoequiv->fases_sec;
      num_fases_pri = fases_pri->NumFases();
      num_fases_sec = fases_sec->NumFases();
      fprintf(arq, "\n\n %-15s %-15s %-15s  %7.1f         ",
              trafoequiv->bar1->codigo.c_str(),
              trafoequiv->bar2->codigo.c_str(),
              trafoequiv->codigo.c_str(),
              (trafoequiv->defasagem*radgrau));
      for(int nfa=0; nfa < num_fases_pri; nfa++)
         {
         fprintf(arq, "%c", fases_pri->FaseAsCharIndice(nfa));
         }
      fprintf(arq, " ");
      for(int nfa=0; nfa < num_fases_sec; nfa++)
         {
         fprintf(arq, "%c", fases_sec->FaseAsCharIndice(nfa));
         }
      for(int nfa=(num_fases_pri+num_fases_sec+1); nfa < ((2*MAX_FASES_G3)+1); nfa++)
         {
         fprintf(arq, " ");
         }
      fprintf(arq, "     %13.6e %13.6e   %13.6e %13.6e",
              trafoequiv->Ynodal1_ij_pu.real(), trafoequiv->Ynodal1_ij_pu.imag(),
              trafoequiv->Ynodal1_ji_pu.real(), trafoequiv->Ynodal1_ji_pu.imag());
      fprintf(arq, "\n                                       "
                   "                                         "
                   "%13.6e %13.6e   %13.6e %13.6e",
              trafoequiv->Ynodal0_ij_pu.real(), trafoequiv->Ynodal0_ij_pu.imag(),
              trafoequiv->Ynodal0_ji_pu.real(), trafoequiv->Ynodal0_ji_pu.imag());
      // Imprime submatrizes de admitâncias nodais
      for(int i=0; i < num_fases_pri; i++)
         {
         fprintf(arq, "\n");
         for(int j=0; j < num_fases_sec; j++)
            {
            z1 = trafoequiv->MatYnodal_ij_pu[i][j];
            fprintf(arq, "  %13.6e %13.6e", z1.real(), z1.imag());
            }
         }
      fprintf(arq, "\n");
      for(int i=0; i < num_fases_sec; i++)
         {
         fprintf(arq, "\n");
         for(int j=0; j < num_fases_pri; j++)
            {
            z1 = trafoequiv->MatYnodal_ji_pu[i][j];
            fprintf(arq, "  %13.6e %13.6e", z1.real(), z1.imag());
            }
         }
      }
   }

// Imprime dados de transformadores ZZ
if((geralC->num_open_dados == 2) && (rede3->lisTRAFOZZ->Count > 0))
   {
   fprintf(arq, "\n\n\n *** Dados de trafos ZigZag ***"
                "\n\n Barra              Código            P.nom.(MVA)"
                "               Z0 (pu)                  Admit. fase (S)\n");
   for(int nt=0; nt < rede3->lisTRAFOZZ->Count; nt++)
      {
      trafozz = (TTrafoZZ *) rede3->lisTRAFOZZ->Items[nt];
      ybase   = trafozz->snom_mva / (trafozz->vnom_kv * trafozz->vnom_kv);
      z1      = trafozz->z0_ohm * ybase;
      fprintf(arq, "\n %-15s    %-15s     %7.3f      %13.6e %13.6e   %13.6e %13.6e",
              trafozz->barra->codigo.c_str(),
              trafozz->codigo.c_str(),
              trafozz->snom_mva,
              z1.real(), z1.imag(),
              trafozz->admit_fase_S.real(), trafozz->admit_fase_S.imag());
      }
   }

// Imprime dados de reguladores de tensão monofásicos
if((geralC->num_open_dados == 2) && (rede3->lisREG12->Count > 0))
   {
   fprintf(arq, "\n\n\n *** Dados de reguladores de tensão monofásicos ***"
                "\n\n De              Para               Fases       "
                "Tensões nom. (kV)"
                "    P.nom.(MVA)        Imped. (pu)       At     Imp. aterr. "
                "(ohm)       Tap (pu)"
                "\n                                  Prim. Sec.      "
                "Prim.      Sec.\n");
   for(int nt=0; nt < rede3->lisREG12->Count; nt++)
      {
      reg12 = (TReg12 *) rede3->lisREG12->Items[nt];
      if((reg12->tipo == AUTOTRAFO4) || (reg12->tipo == AUTOTRAFO6_1T) ||
			(reg12->tipo == AUTOTRAFO6_2T))
         {
         soma1 = reg12->snom_mva / (reg12->vnom2_kv * reg12->vnom2_kv); // Ref. secundário
         }
      else  // TRAFO4 ou TRAFO5
         {
         soma1 = reg12->snom_mva / (reg12->vnom1_kv * reg12->vnom1_kv); // Ref. primário
         }
      tensao    = reg12->ztrafo_ohm * soma1;
      tap1      = reg12->vet_tap[0] * reg12->vnom1_kv / reg12->vnom2_kv;
      num_fases = reg12->fases_pri->NumFases();
      char1     = reg12->fases_pri->FaseAsCharIndice(0);
      char2     = ' ';
      char3     = ' ';
      if(num_fases >= 2) char2 = reg12->fases_pri->FaseAsCharIndice(1);
      if(num_fases >= 3) char3 = reg12->fases_pri->FaseAsCharIndice(2);
      num_fases = reg12->fases_sec->NumFases();
      char4     = reg12->fases_sec->FaseAsCharIndice(0);
      char5     = ' ';
      char6     = ' ';
      if(num_fases >= 2) char5 = reg12->fases_sec->FaseAsCharIndice(1);
      if(num_fases >= 3) char6 = reg12->fases_sec->FaseAsCharIndice(2);
      fprintf(arq, "\n %-15s %-15s   %c%c%c  %c%c%c   %8.3f   %8.3f     "
                   "%8.4f     %9.6f %9.6f    %c%c",
              reg12->bar1->codigo.c_str(),
              reg12->bar2->codigo.c_str(),
              char1, char2, char3, char4, char5, char6,
              reg12->vnom1_kv, reg12->vnom2_kv,
              reg12->snom_mva, tensao.real(), tensao.imag(),
              codigo_ater[reg12->tipo_ater1],
              codigo_ater[reg12->tipo_ater2]);
      if((reg12->tipo_ater1 == atZ) && (reg12->tipo_ater2 == atZ) &&
         (! geralC->flag_ater_comuns))
         {
         soma2 = reg12->bar1->VnomFase_kv(faseN);
         ybase = geralC->Sbase_fase / (soma2 * soma2);
         zater = cum / (reg12->yater1_pu * ybase);
         fprintf(arq, "  %10.3e %10.3e     1:%8.6f",
					  zater.real(), zater.imag(), tap1);
			if(reg12->NumTaps == 2) fprintf(arq, "  1:%8.6f", reg12->vet_tap[1]);
         soma2 = reg12->bar2->VnomFase_kv(faseN);
         ybase = geralC->Sbase_fase / (soma2 * soma2);
         zater = cum / (reg12->yater2_pu * ybase);
         fprintf(arq, "\n                                                "
                      "                           "
                      "                           "
                      "  %10.3e %10.3e",
                 zater.real(), zater.imag());
         }
      else if(reg12->tipo_ater1 == atZ)
         {
         soma2 = reg12->bar1->VnomFase_kv(faseN);
         ybase = geralC->Sbase_fase / (soma2 * soma2);
         zater = cum / (reg12->yater1_pu * ybase);
         fprintf(arq, "  %10.3e %10.3e     1:%8.6f",
					  zater.real(), zater.imag(), tap1);
			if(reg12->NumTaps == 2) fprintf(arq, "  1:%8.6f", reg12->vet_tap[1]);
         }
      else if(reg12->tipo_ater2 == atZ)
         {
         soma2 = reg12->bar2->VnomFase_kv(faseN);
         ybase = geralC->Sbase_fase / (soma2 * soma2);
         zater = cum / (reg12->yater2_pu * ybase);
         fprintf(arq, "  %10.3e %10.3e     1:%8.6f",
					  zater.real(), zater.imag(), tap1);
			if(reg12->NumTaps == 2) fprintf(arq, "  1:%8.6f", reg12->vet_tap[1]);
         }
      else
         {
         fprintf(arq, "                            1:%8.6f",
					  tap1);
			if(reg12->NumTaps == 2) fprintf(arq, "  1:%8.6f", reg12->vet_tap[1]);
         }
      f1 = reg12->fases_sec->Fase(0);  // Fase 1 do secundário
      f2 = reg12->fases_sec->Fase(1);  // Fase 2 do secundário
		// Para AUTOTRAFO6 e TRAFO5 considera as duas fases que não são comuns
      if((reg12->tipo == AUTOTRAFO6_1T) || (reg12->tipo == AUTOTRAFO6_2T) ||
			(reg12->tipo == TRAFO5)) // 3 terminais no sec.
         {
         if(f1 == reg12->fase_comum)
            {
            f1 = f2;
            f2 = reg12->fases_sec->Fase(2);
            }
         else if(f2 == reg12->fase_comum)
            {
            f2 = reg12->fases_sec->Fase(2);
            }
         }
      p1 = reg12->ypar_S.real() / soma1;
		if((reg12->tipo == AUTOTRAFO6_1T) || (reg12->tipo == AUTOTRAFO6_2T))
         {
         p1 *= 2.;               // COM ou SEM neutro (duas pernas)
         if(reg12->NeutroSec())  // COM neutro: acerto pela tensão de fase
            {
            fmult  = reg12->bar2->VnomFase_kv(reg12->fases_sec->Fase(0))
                   / reg12->vnom2_kv;
            p1    *= (fmult * fmult);
            }
         }
      str1 = "         ";
      if(reg12->IsDOIS_SENTIDOS())
         {
         str1 = geralC->codigo_sub_modelo_regu[reg12->sub_modelo].c_str();
         }
      if(reg12->NumTaps == 1)  // Todos os tipos exceto AUTOTRAFO6_2T
         {
         fprintf(arq, "\n      %-15s  %6s  Pfe(pu): %6.4f  FC: %c  "
                      "Mod.: %8s  Sub-mod.: %9s  "
                      "Var.tensão (pu): %5.3f  Núm.passos: %2d",
                 reg12->codigo.c_str(),
                 codigo_tipo_12[reg12->tipo].c_str(),
                 p1,
                 reg12->fases_pri->FaseAsChar(reg12->fase_comum),
                 geralC->codigo_modelo_regu[reg12->modelo].c_str(),
                 str1.c_str(),
                 reg12->var_tensao_pu,
                 reg12->num_passo);
         if(reg12->IsENERQCT())
            {
            fprintf(arq, "\n      B.ref.: %-15s  Fases bar.ref.: %c%c",
                    reg12->bar_ref_12->codigo.c_str(),
                    reg12->fases_pri->FaseAsChar(f1),
                    reg12->fases_pri->FaseAsChar(f2));
            }
         else if(reg12->IsPADRAO() || reg12->IsDOIS_SENTIDOS())
            {
            fprintf(arq, "\n      B.ref.12: %-15s  Fases bar.ref.12: %c%c  "
                         "Vref.12(pu): %6.4f  BM12(pu): %5.3f",
                    reg12->bar_ref_12->codigo.c_str(),
                    reg12->fases_pri->FaseAsChar(f1),
                    reg12->fases_pri->FaseAsChar(f2),
                    reg12->vref_12_pu,
                    reg12->banda_morta_pu);
            if(reg12->IsDOIS_SENTIDOS())
               {
               fprintf(arq, "      B.ref.21: %-15s  Fases bar.ref.21: %c%c  "
                            "Vref.21(pu): %6.4f  BM21(pu): %5.3f",
                       reg12->bar_ref_21->codigo.c_str(),
                       reg12->fases_pri->FaseAsChar(f1),
                       reg12->fases_pri->FaseAsChar(f2),
                       reg12->vref_21_pu,
                       reg12->banda_morta_inv_pu);
               }
            }
         }
      else  // AUTOTRAFO6_2T
         {
         char1 = reg12->fases_pri->FaseAsChar(f1);
         char2 = reg12->fases_pri->FaseAsChar(reg12->fase_comum);
         char3 = reg12->fases_pri->FaseAsChar(f2);
         char4 = char2;
         if(((char1 == 'A') && (char2 == 'C')) ||
            ((char1 == 'B') && (char2 == 'A')) ||
            ((char1 == 'C') && (char2 == 'B')))
            {
            char5 = char1; char1 = char2; char2 = char5;
            }
         if(((char3 == 'A') && (char4 == 'C')) ||
            ((char3 == 'B') && (char4 == 'A')) ||
            ((char3 == 'C') && (char4 == 'B')))
            {
            char5 = char3; char3 = char4; char4 = char5;
            }
         fprintf(arq, "\n      %-15s  %6s  Pfe(pu): %6.4f  FC: %c  "
                      "Mod.: %8s  Sub-mod.: %9s  "
                      "Var.tensão (pu): %5.3f  Núm.passos: %2d",
                 reg12->codigo.c_str(),
                 codigo_tipo_12[reg12->tipo].c_str(),
                 p1,
                 reg12->fases_pri->FaseAsChar(reg12->fase_comum),
                 geralC->codigo_modelo_regu[reg12->modelo].c_str(),
                 str1.c_str(),
                 reg12->var_tensao_pu,
                 reg12->num_passo);
         if(reg12->IsENERQCT())
            {
            fprintf(arq, "\n      B.ref.: %-15s  Fases bar.ref.: %c%c/%c%c",
                    reg12->bar_ref_12->codigo.c_str(),
                    char1,
                    char2,
                    char3,
                    char4);
            }
         else if(reg12->IsPADRAO() || reg12->IsDOIS_SENTIDOS())
            {
            fprintf(arq, "\n      B.ref.12: %-15s  Fases bar.ref.12: %c%c/%c%c"
                         "  Vref.12(pu): %6.4f  BM12(pu): %5.3f",
                    reg12->bar_ref_12->codigo.c_str(),
                    char1,
                    char2,
                    char3,
                    char4,
                    reg12->vref_12_pu,
                    reg12->banda_morta_pu);
            if(reg12->IsDOIS_SENTIDOS())
               {
               fprintf(arq, "      B.ref.21: %-15s  Fases bar.ref.21: %c%c/%c%c"
                            "  Vref.21(pu): %6.4f  BM21(pu): %5.3f",
                       reg12->bar_ref_21->codigo.c_str(),
                       char1,
                       char2,
                       char3,
                       char4,
                       reg12->vref_21_pu,
                       reg12->banda_morta_inv_pu);
               }
            }
         }
      }
   }

// Imprime dados de reguladores de tensão trifásicos
if((geralC->num_open_dados == 2) && (rede3->lisREG32->Count > 0))
   {
   fprintf(arq, "\n\n\n *** Dados de reguladores de tensão trifásicos ***"
                "\n\n De              Para             Lig.    "
                "Tensões nom. (kV)     "
                "P.nom.(MVA)      Imped. (pu)      At     Imp. aterr. (ohm)"
                "               Taps (pu)"
                "\n                                           "
                "Prim.      Sec.\n");
   for(int nt=0; nt < rede3->lisREG32->Count; nt++)
      {
      reg32  = (TReg32 *) rede3->lisREG32->Items[nt];
      soma1  = reg32->snom_mva / (reg32->vnom2_kv * reg32->vnom2_kv);
      tensao = reg32->ztrafo_ohm * soma1;
      p1     = reg32->ypar_S.real() / soma1;
      soma1  = reg32->vnom1_kv / reg32->vnom2_kv;
      tap1   = reg32->vet_tap[0] * soma1;
      tap2   = reg32->vet_tap[1] * soma1;
      tap3   = reg32->vet_tap[2] * soma1;
      fprintf(arq, "\n %-15s %-15s   %c%c   %8.3f   %8.3f  "
                   "    %8.4f   %9.6f %9.6f   %c%c",
              reg32->bar1->codigo.c_str(),
              reg32->bar2->codigo.c_str(),
              codigo_lig_enrol[reg32->tipo_lig1],
              codigo_lig_enrol[reg32->tipo_lig2],
              reg32->vnom1_kv, reg32->vnom2_kv,
              reg32->snom_mva,
              tensao.real(), tensao.imag(),
              codigo_ater[reg32->tipo_ater1],
              codigo_ater[reg32->tipo_ater2]);
      if((reg32->tipo_ater1 == atZ) && (reg32->tipo_ater2 == atZ) &&
         (! geralC->flag_ater_comuns))
         {
         soma2 = reg32->bar1->VnomFase_kv(faseN);
         ybase = geralC->Sbase_fase / (soma2 * soma2);
         zater = cum / (reg32->yater1_pu * ybase);
         fprintf(arq, "  %10.3e %10.3e  1:%8.6f 1:%8.6f 1:%8.6f",
                 zater.real(), zater.imag(), tap1, tap2, tap3);
         soma2 = reg32->bar2->VnomFase_kv(faseN);
         ybase = geralC->Sbase_fase / (soma2 * soma2);
         zater = cum / (reg32->yater2_pu * ybase);
         fprintf(arq, "\n                       %-15s"
                      "                                "
                      "                                "
                      "%10.3e %10.3e",
                 reg32->codigo.c_str(), zater.real(), zater.imag());
         }
      else if(reg32->tipo_ater1 == atZ)
         {
         soma2 = reg32->bar1->VnomFase_kv(faseN);
         ybase = geralC->Sbase_fase / (soma2 * soma2);
         zater = cum / (reg32->yater1_pu * ybase);
         fprintf(arq, "  %10.3e %10.3e  1:%8.6f 1:%8.6f 1:%8.6f",
                 zater.real(), zater.imag(), tap1, tap2, tap3);
         fprintf(arq, "\n                       %-15s",
                 reg32->codigo.c_str());
         }
      else if(reg32->tipo_ater2 == atZ)
         {
         soma2 = reg32->bar2->VnomFase_kv(faseN);
         ybase = geralC->Sbase_fase / (soma2 * soma2);
         zater = cum / (reg32->yater2_pu * ybase);
         fprintf(arq, "  %10.3e %10.3e  1:%8.6f 1:%8.6f 1:%8.6f",
                 zater.real(), zater.imag(), tap1, tap2, tap3);
         fprintf(arq, "\n                       %-15s",
                 reg32->codigo.c_str());
         }
      else
         {
         fprintf(arq, "                         1:%8.6f 1:%8.6f 1:%8.6f",
                 tap1, tap2, tap3);
         }
      str1 = "         ";
      if(reg32->IsDOIS_SENTIDOS())
         {
         str1 = geralC->codigo_sub_modelo_regu[reg32->sub_modelo].c_str();
         }
      fprintf(arq, "\n      %-15s  %-9s  NF: %1d/%1d  %8s  %9s  Pfe(pu): %6.4f"
                   "  Var.tensão (pu): %5.3f  Núm.passos: %2d",
              reg32->codigo.c_str(),
              codigo_tipo_32[reg32->tipo].c_str(),
              reg32->fases_pri->NumFases(),
              reg32->fases_sec->NumFases(),
              geralC->codigo_modelo_regu[reg32->modelo].c_str(),
              str1.c_str(),
              p1,
              reg32->var_tensao_pu,
              reg32->num_passo);
      if(reg32->IsENERQCT())
         {
         fprintf(arq, "\n      B.ref.: %-15s",
                 reg32->bar_ref_12->codigo.c_str());
         }
      else if(reg32->IsPADRAO() || reg32->IsDOIS_SENTIDOS())
         {
         fprintf(arq, "\n      B.ref.12: %-15s  Vref.12(pu): %6.4f  BM12(pu): %5.3f",
                 reg32->bar_ref_12->codigo.c_str(),
                 reg32->vref_12_pu,
                 reg32->banda_morta_pu);
         if(reg32->IsDOIS_SENTIDOS())
            {
            fprintf(arq, "      B.ref.21: %-15s  Vref.21(pu): %6.4f  BM21(pu): %5.3f",
                    reg32->bar_ref_21->codigo.c_str(),
                    reg32->vref_21_pu,
                    reg32->banda_morta_inv_pu);
            }
         }
      }
   }

// Imprime dados de cargas não-lineares
if((geralC->num_open_dados == 2) && (rede3->lisCNL->Count > 0))
   {
   fprintf(arq, "\n\n\n *** Dados de cargas não-lineares - Patamar %d ***", (np+1));
   // fprintf(arq, "\n\n Barra        F.pot.(pu)   Ordem freq.        "
   fprintf(arq, "\n\n Barra                     Ordem freq.        "
                      "--------Potências (kVA / ° para fund.) (pu fund. / ° demais)"
                      "---------"
                      "\n                                                   "
                      "Mód. A    Âng. A        Mód. B    Âng. B        Mód. C    Âng. C");
   for(int nc=0; nc < rede3->lisCNL->Count; nc++)
      {
      cnl1      = (TCNL *) rede3->lisCNL->Items[nc];
      barra1    = cnl1->barra;
      fases     = barra1->fases;
      num_fases = fases->NumFases();
      fmult     = barra1->vnom_linha_kv * raiz3_inverso;
      for(int indfreq=0; indfreq < cnl1->num_harmonicas; indfreq++)
         {
         // if(indfreq == 0) fprintf(arq, "\n\n %-10s    %7.4f        ",
         //                          cnl1->barra->codigo.SubString(0, 10).c_str(),
         //                          cnl1->cos_fi);
         if(indfreq == 0) fprintf(arq, "\n\n %-15s              ",
                                cnl1->barra->codigo.c_str());
         else             fprintf(arq, "\n                              ");
         ordem = cnl1->vet_ordem[indfreq];
         fprintf(arq, "%4d         ", ordem);
         charstr1.sprintf("                        ");
         charstr2.sprintf("                        ");
         charstr3.sprintf("                        ");
         for(int nfa=0; nfa < num_fases; nfa++)
            {
            if(fases->Fase(nfa) == faseN) continue;  // Descarta o neutro
            iap = fases->IndiceABCN(nfa);
            p1  = cnl1->mat_corrente->D2[indfreq][iap];
            if(ordem == 1) p1 *= fmult; // Frequência fundamental: potência em kVA
            q1 = cnl1->mat_ang_rad->D2[indfreq][iap] * radgrau;
            if     (iap == 0) charstr1.sprintf("   %11.4f %9.4f", p1, q1); // Fase A
            else if(iap == 1) charstr2.sprintf("   %11.4f %9.4f", p1, q1); // Fase B
            else              charstr3.sprintf("   %11.4f %9.4f", p1, q1); // Fase C
            }
         fprintf(arq, "%24s%24s%24s", charstr1.c_str(), charstr2.c_str(),
                                      charstr3.c_str());
         }
      }
   }

/*
// Imprime dados bancos de trafos monofásicos ligados em Delta-Fechado
if((geralC->num_open_dados == 2) && (rede3->lisBANCO_DF->Count > 0))
   {
   fprintf(arq, "\n\n\n *** Dados de bancos ligados em Delta-Fechado ***");
   for(int nb=0; nb < rede3->lisBANCO_DF->Count; nb++)
      {
      fprintf(arq, "\n");
      bancoDF = (TBancoDF *) rede3->lisBANCO_DF->Items[nb];
      for(int nt=0; nt < bancoDF->lisTRAFO->Count; nt++)
         {
         trafo12 = (TTrafo12 *) bancoDF->lisTRAFO->Items[nt];
         if(nt == 0)  // Primeiro trafo do banco
            {
            fprintf(arq, "\n Banco DF: %d"
                         "\n    %-15s - %-15s - codigo: %-15s - tipo: %-6s",
                    nb,
                    trafo12->bar1->codigo.c_str(),
                    trafo12->bar2->codigo.c_str(),
                    trafo12->codigo.c_str(),
                    codigo_tipo_12[trafo12->tipo].c_str());
            }
         else  // Demais trafos
            {
            fprintf(arq, "\n                                        "
                         "codigo: %-15s - tipo: %-6s",
                    trafo12->codigo.c_str(),
                    codigo_tipo_12[trafo12->tipo].c_str());
            }
         }
      }
   }
*/

// Imprime linhas das matrizes Ynodal e Jacob (só uma vez)
if(geralC->num_open_dados == 2) ImprimeLinhasYnodalJacob(arq, lisBAR_J);

// Finalização
fclose(arq);

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TImpressao3::ImprimeDadosESTIM3(int   np,
                                                TList *lisBAR_ESTADO)
{
// Variáveis locais
int          fase_int, ind1, num_vezes;
int          vet_fases[] = {faseA, faseB, faseC, faseN};
FILE         *arq;
TBarra       *bar1;
TBarraEstado *pbar_est;
TFase        *fases;
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
                "\n\n ------ Barra -------   Tipo"
                "          ---------------- Endereço de 'V' e 'Theta' em cada fase"
                " ------------------"
                "\n                                            Fase A            "
                "  Fase B              Fase C              Neutro\n",
           geralC->num_var_est);

   // Loop de barras
   for(int nb=0; nb < lisBAR_CODIGO->Count; nb++)
      {
      bar1  = (TBarra *) lisBAR_CODIGO->Items[nb];
      fases = bar1->fases;
      ind1  = bar1->offset;
      if(ind1 >= 0) pbar_est = (TBarraEstado *) lisBAR_ESTADO->Items[ind1];
      else          continue;

      // Imprime barra
      fprintf(arq, "\n %-15s        %-5s   ",
              bar1->codigo.c_str(), geralC->codigo_tipo_barra[bar1->tipo].c_str());
      for(int nfa=0; nfa < MAX_FASES_G3; nfa++)
         {
         fase_int = vet_fases[nfa];
         ind1     = fases->Indice(fase_int);
         if(ind1 < 0)  // Não existe a fase 'fase_int'
            {
            fprintf(arq, "           -       -");
            }
         else  // Existe a fase
            {
            if(fases->LinhaIndice(ind1) < 0)  // Neutro atSolido (não existe o nó)
               {
               fprintf(arq, "           -       -");
               }
            else  // Existe o nó
               {
               pYE = pbar_est->vet_YE[ind1];
               if(pYE == NULL)
                  {
                  fprintf(arq, "           -       -");
                  }
               else
                  {
                  fprintf(arq, "      %6d", pYE->vet_linha_E[0]);
                  if(pYE->num_var == 2) fprintf(arq, "  %6d", pYE->vet_linha_E[1]);
                  else                  fprintf(arq, "       -");
                  }
               }
            }
         } // for(nfa)
      } // for(nb)
   } // if(geralC->num_var_est)

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
bool __fastcall TImpressao3::ImprimeDadosMedidores(FILE *arq)
{
// Variáveis locais
char       char0, char1, char2, char2_prev, char3;
int        fase_int, id1, id1_prev;
AnsiString str1, str1_prev, str2, str2_prev, str3, str3_prev;
TBarra     *bar_ref, *bar1;
TMedidor   *med1;
TTrafo32   *trafo32;
TTrecho    *trec1;

// Imprime dados de medidores
fprintf(arq, "\n\n\n *** Dados de medidores (total: %d) ***"
               "\n\n    Núm. R/V H/D  Código           Tipo eqpto  "
                   "--------------Equipamento--------------  TM  Fase   "
                   "Valor medido   Desv. padrão   Toler.  (pu)"
                 "\n                                               Barra1"
                   "          Barra2              ID\n",
        rede3->lisMED->Count);
char2_prev = '*';
str1_prev  = "*";
str2_prev  = "*";
str3_prev  = "*";
id1_prev   = -10000000;
for(int nm=0; nm < rede3->lisMED->Count; nm++)
   {
   med1     = (TMedidor *) rede3->lisMED->Items[nm];
   id1      = med1->id_calc;
   bar_ref  = med1->bar_ref;
   fase_int = med1->fase_int;
   if(med1->tipo_medidor == medREAL) char0 = 'R';
   else                              char0 = 'V';
   if(med1->enabled)                 char1 = ' ';
   else                              char1 = 'D';
   char2 = geralC->codigo_tipo_medicao[med1->tipo_medicao];
   char3 = bar_ref->fases->FaseAsChar(fase_int);
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
         trafo32 = (TTrafo32 *) med1->eqpto;
         str1    = "Trafo32";
         if(trafo32->bar1 == med1->bar_ref)
            {
            str2 = trafo32->bar1->codigo;
            str3 = trafo32->bar2->codigo;
            }
         else
            {
            str2 = trafo32->bar2->codigo;
            str3 = trafo32->bar1->codigo;
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
   fprintf(arq, "    %c   %13.6e  %13.6e",
           char3, med1->val_med_pu, med1->sigma2_pu);
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
bool __fastcall TImpressao3::ImprimeDadosR3(void)
{
// Local variables
bool            flag_achou;
char            char1, char2, char3, char4, char5, char6;
int             f1, f2, iap, kprint, naux, naux2, nfa,
                num_fases, num_fases_pri, num_fases_sec, num_lig_mutua,
                num_linhas, ordem;
double          fmult, p1, q1, p3, q3, soma1, soma2, tap1, tap2, tap3,
                ybase;
complex<double> y1;
AnsiString      charstr1, charstr2, charstr3, str1, string1, string2, string3,
                string4, string5, string6, string7, string8;
complex<double> *pc1, tensao, z1, zater;
FILE            *arq;
TBancoDF        *bancoDF;
TBarra          *barra1;
TBarraDefeito   *bar_def;
TCapSerie       *caps1;
TCNL            *cnl1;
TFase           *fases, *fases_pri, *fases_sec;
TGrupoMutua     *grupo;
TMutua          *mutua;
TReg12          *reg12;
TReg32          *reg32;
TTrecho         *trec1;
TTrafo12        *trafo12;
TTrafo32        *trafo32;
TTrafo33        *trafo33;
TTrafoEquiv     *trafoequiv;
TTrafoZZ        *trafozz;
TSup            *sup1;

// Verifica se o relatório está habilitado
if(geralC->tipo_relatorio == trSEM_RELATORIO) return(false); // Não imprimiu nada

// Abre arquivo
arq = geralC->AbreArquivo(arqDADOS);
if(arq == NULL) return(false);

// Ordena barras, trechos e trafos, se isso não foi feito antes
OrdenaBarrasCodigo();
OrdenaTrechos();
OrdenaTrafo12();
OrdenaTrafo32();

// Conta ligações com mútua
num_lig_mutua = 0;
for(int nl=0; nl < rede3->lisTRECHO->Count; nl++)
   {
   trec1 = (TTrecho *) rede3->lisTRECHO->Items[nl];
   if(trec1->grupo_mutua >= 0) num_lig_mutua++;
   }

// Imprime aviso de rede parcial
if(geralC->num_open_dados > 1) fprintf(arq, "\n");
fprintf(arq, "\n *** ATENÇÃO: a rede a reduzir é PARCIAL (composta somente "
             "pelas barras e ligações diretamente envolvidas na redução) ***");

// Prints general data
if(geralC->num_open_dados == 1)
   {
   if     (geralC->CargaNula)           string1 = "Sem carga";
   else if(geralC->CargaEmYnodal)       string1 = "Todas em Ynodal";
   else if(geralC->CargaInjecoesExt)    string1 = "Injeções externas";
   else                                 string1 = "";
   if(geralC->flag_capac_LT)            string2 = "Sim";
   else                                 string2 = "Não";
   if(geralC->usa_pi_equiv)             string3 = "Sim";
   else                                 string3 = "Não";
   if(geralC->flag_ypar_trafos)         string4 = "Sim";
   else                                 string4 = "Não";
   if(geralC->flag_ater_comuns)         string5 = "Sim";
   else                                 string5 = "Não";
   if(geralC->todos_sup_barra_interna)  string6 = "Sim";
   else                                 string6 = "Não";
   if(geralC->flag_TRAFO5_Como_Trafo3E) string7 = "Sim";
   else                                 string7 = "Não";
   if(geralC->flag_utiliza_bancoDF)     string8 = "Sim";
   else                                 string8 = "Não";
   fprintf(arq, "\n\n *** REDE PARCIAL A REDUZIR - Dados Gerais ***"
                "\n\n Programa:                                                   %15s"
                  "\n Plataforma:                                                           %5s"
                  "\n Configuração:                                                       %7s"
                  "\n Nome da rede:                           %35s"
                  "\n Potência de base (MVA):                                                %4.0f"
                  "\n Representação da carga:                           %25s"
                  "\n Capacitância de linhas:                                                 %3s"
                  "\n Modelo pi-equivalente para linhas:                                      %3s"
                  "\n Ramo paralelo de trafos:                                                %3s"
                  "\n Aterramentos comuns:                                                    %3s"
                  "\n Barra interna de suprimentos:                                           %3s"
                  "\n TRAFO5 como Trafo3E:                                                    %3s"
                  "\n Utilização de bancos Delta Fechado:                                     %3s",
           geralC->codigo_programa[geralC->Programa1].c_str(),
           geralC->Plataforma.c_str(), geralC->Configuracao.c_str(),
           geralC->NomeRede.c_str(), geralC->Sbase, string1.c_str(),
           string2.c_str(), string3.c_str(), string4.c_str(), string5.c_str(),
           string6.c_str(), string7.c_str(), string8.c_str());
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
                "\n\n Número de trafos Monofásicos de 2 enrol.:                            %6d"
                  "\n                  Trifásicos  de 2 enrol.:                            %6d"
                  "\n                  Trifásicos  de 3 enrol.:                            %6d"
                  "\n                  Equivalentes:                                       %6d"
                  "\n                  ZigZag:                                             %6d"
                "\n\n Número de bancos ligados em Delta Fechado:                           %6d"
                "\n\n Número de reguladores de tensão Monofásicos:                         %6d"
                  "\n                                 Trifásicos:                          %6d"
                "\n\n Número de cargas não-lineares:                                       %6d"
                "\n\n Número de linhas/colunas em Ycc:                                     %6d"
                  "\n                             Ygg:                                     %6d"
                  "\n                             Ynodal:                                  %6d",
           geralC->codigo_tipo_barra[BAR_PQ].c_str(),  geralC->num_barras_PQ,
           geralC->codigo_tipo_barra[BAR_EXT].c_str(), geralC->num_barras_EXT,
           geralC->codigo_tipo_barra[BAR_PQV].c_str(), geralC->num_barras_PQV,
           geralC->codigo_tipo_barra[BAR_PV].c_str(),  geralC->num_barras_PV,
           geralC->codigo_tipo_barra[BAR_SW].c_str(),  geralC->num_barras_SW,
           geralC->codigo_tipo_barra[BAR_INT].c_str(), geralC->num_barras_INT,
           rede3->lisBAR->Count, (rede3->lisTRECHO->Count-num_lig_mutua),
           rede3->lisGRUPO_MUTUA->Count, num_lig_mutua, rede3->lisTRECHO->Count,
           rede3->lisCAPS->Count, rede3->lisTRAFO12->Count,
           rede3->lisTRAFO32->Count, rede3->lisTRAFO33->Count,
           rede3->lisTRAFOEQUIV->Count, rede3->lisTRAFOZZ->Count,
           rede3->lisBANCO_DF->Count, rede3->lisREG12->Count,
           rede3->lisREG32->Count, rede3->lisCNL->Count,
           geralC->num_lin_submatriz,
           (geralC->num_tot_linhas - geralC->num_lin_submatriz),
           geralC->num_tot_linhas);
   }

// Imprime dados para cálculo de curto-circuito
if((geralC->num_open_dados == 1) && (curto_aux3 != NULL))
   {
   fprintf(arq, "\n\n\n *** Dados para cálculo de curto-circuito ***");
   for(int nb=0; nb < curto_aux3->lisBAR_DEF->Count; nb++)
      {
      bar_def = (TBarraDefeito *) curto_aux3->lisBAR_DEF->Items[nb];
      barra1  = bar_def->barra;
      fprintf(arq, "\n\n     Barra:                                %-15s"
                     "\n     Tipo / fases do defeito:              %s / %s"
                     "\n     Impedância de defeito - FASE  (ohm):  (%12.6e %12.6e)"
                     "\n     Impedância de defeito - TERRA (ohm):  (%12.6e %12.6e)",
              barra1->codigo.c_str(),
              codigo_tipo_curto[bar_def->tipo_curto].c_str(),
              codigo_fases_curto[bar_def->fases_curto].c_str(),
              curto_aux3->zdef_fase_ohm.real(),  curto_aux3->zdef_fase_ohm.imag(),
              curto_aux3->zdef_terra_ohm.real(), curto_aux3->zdef_terra_ohm.imag());
      }
   }

// Prints bus data
if(geralC->num_open_dados == 1) fprintf(arq, "\n");
fprintf(arq, "\n\n *** REDE PARCIAL A REDUZIR - Dados de barras ***"
             "\n\n Código            Tipo      Tens. nom. (kV)     "
             "-Fases-    At.   Mant/Rem      Imped. aterram. (ohm)   "
             "   Tipo carga     ----P(MW)--- ---Q(MVAr)--"
             "\n                              Fase     Linha");
soma1 = 0.;
soma2 = 0.;
for(int nb=0; nb < lisBAR_CODIGO->Count; nb++)
   {
   barra1    = (TBarra *) lisBAR_CODIGO->Items[nb];
   fases     = barra1->fases;
   num_fases = fases->NumFases();
   if(barra1->mantida) char1 = ' ';
   else                char1 = 'R';
   fprintf(arq, "\n\n %-15s   %-5s   %8.3f %8.3f",
           barra1->codigo.c_str(),
           geralC->codigo_tipo_barra[barra1->tipo].c_str(),
           barra1->VnomFase_kv(barra1->fases->Fase(0)),
           barra1->VnomLinha_kv());
   iap      = 0;
   charstr1 = "";
   for(int nfa=0; nfa < num_fases; nfa++)
      {
      charstr1 += fases->FaseAsCharIndice(nfa);
      iap++;
      if(iap >= 7) break;
      charstr1 += " ";
      iap++;
      }
   for(int nfa=iap; nfa < 7; nfa++)
      {
      charstr1 += " ";
      }
   fprintf(arq, "     %7s     ", charstr1.c_str());
   if(barra1->tipo_ater == atZ)
      {
      zater = czero;
      tap1  = barra1->VnomFase_kv(faseN);
      if(tap1 != 0.)
         {
         ybase = geralC->Sbase_fase / (tap1 * tap1);
         zater = cum / (barra1->yater_pu * ybase);
         }
      fprintf(arq, "%c        %c    %12.5e %12.5e   ",
              codigo_ater[barra1->tipo_ater], char1,
              zater.real(), zater.imag());
      }
   else
      {
      if(fases->ExisteN()) naux = barra1->tipo_ater;
      else                 naux = NUM_TIPO_ATERR;  // A barra não tem neutro
      fprintf(arq, "%c        %c                                ",
              codigo_ater[naux], char1);
      }

   /*
   kprint = 0;
   for(int lig=0; lig < NUM_LIG_CARGA; lig++)
      {
      if(rede3->CargaNula(barra1->vet_carga[lig])) continue; // Não tem carga do tipo 'lig'
      z1     = barra1->vet_carga[lig][Icte];
      p1     = z1.real();
      q1     = z1.imag();
      soma1 += p1;
      soma2 += q1;
      if(kprint != 0) fprintf(arq, "\n\n                              "
                                   "                                  "
                                   "                             ");
      kprint = 1;
      fprintf(arq, "    %6s     I %12.6f %12.6f",
              codigo_fases[lig].c_str(), p1, q1);
      z1     = barra1->vet_carga[lig][Scte];
      p1     = z1.real();
      q1     = z1.imag();
      soma1 += p1;
      soma2 += q1;
      fprintf(arq, "\n                                                "
                   "                                                  "
                   "          "
                   "S %12.6f %12.6f", p1, q1);
      z1     = barra1->vet_carga[lig][Zcte];
      p1     = z1.real();
      q1     = z1.imag();
      soma1 += p1;
      soma2 += q1;
      fprintf(arq, "\n                                                "
                   "                                                  "
                   "          "
                   "Z %12.6f %12.6f", p1, q1);
      } // for(lig)
   */

   // Imprime matriz de admitâncias da redução, se houver algum
   // elemento diferente de zero
   if(geralC->num_open_dados == 1)
      {
      flag_achou = false;
      for(int nfa=0; nfa < num_fases; nfa++)
         {
         for(int nfa2=0; nfa2 < num_fases; nfa2++)
            {
            if(Abs(barra1->mat_ynodal_reduc_pu->C2[nfa][nfa2]) > 0.)
               {
               flag_achou = true;
               break;
               }
            }
         }
      if(flag_achou)
         {
         fprintf(arq, "\n\n   ---------------------------"
                      "Matriz de admitâncias nodais (redução de rede) (pu)"
                      "-----------------------------\n");
         for(int nfa=0; nfa < num_fases; nfa++)
            {
            fprintf(arq, "\n");
            for(int nfa2=0; nfa2 < num_fases; nfa2++)
               {
               z1 = barra1->mat_ynodal_reduc_pu->C2[nfa][nfa2];
               fprintf(arq, "   %11.6f %11.6f", z1.real(), z1.imag());
               }
            }
         }
      }
   } // for(nb)
fprintf(arq, "\n\n                                                    "
             "                                     "
             "                                 ------------ ------------"
             "\n                                                      "
             "                                     "
             "                        Total: %12.6f %12.6f",
        soma1, soma2);

// Prints branch data
if((geralC->num_open_dados == 1) && (lisTRECHO_ORD->Count > 0))
   {
   fprintf(arq, "\n\n\n *** REDE PARCIAL A REDUZIR - Dados de trechos "
                "(Yser_tot (pu) / Ypar_tot_nodal (pu)) ***"
                "\n\n De              Para            Código            "
                "Mút.    Compr.(km)     -Fases-     "
                "--------Corr. admissível (A)-------");
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
      fases     = trec1->fases;
      num_fases = fases->NumFases();
      if(trec1->grupo_mutua >= 0)
         {
         char1 = 'M';
         str1  = AnsiString(trec1->grupo_mutua);
         }
      else
         {
         char1 = ' ';
         str1  = "  ";
         }
      fprintf(arq, "\n\n %-15s %-15s %-15s    %c %2s    %8.3f     ",
              charstr1.c_str(),
              charstr2.c_str(),
              trec1->codigo.c_str(), char1, str1.c_str(),
              trec1->compr_km);
      for(int nfa=0; nfa < num_fases; nfa++)
         {
         fprintf(arq, " %c", fases->FaseAsCharIndice(nfa));
         }
      for(int nfa=num_fases; nfa < MAX_FASES_G3; nfa++)
         {
         fprintf(arq, "  ");
         }
      fprintf(arq, "    ");
      for(int nfa=0; nfa < num_fases; nfa++)
         {
         fprintf(arq, " %8.1f", trec1->vet_iadm_a[nfa]);
         }

     // Imprime matriz de admitâncias-série de elementos do trecho
      for(int nfa=0; nfa < num_fases; nfa++)
         {
         fprintf(arq, "\n");
         for(int nfa2=0; nfa2 < num_fases; nfa2++)
            {
            z1 = trec1->MatYserie_pu[nfa][nfa2];
            fprintf(arq, "  %13.6e %13.6e", z1.real(), z1.imag());
            }
         }

      // Imprime matriz de admitâncias-paralelo de elementos do trecho,
      // se houver algum elemento diferente de zero
      if(geralC->flag_capac_LT)
         {
         flag_achou = false;
         for(int nfa=0; nfa < num_fases; nfa++)
            {
            for(int nfa2=0; nfa2 < num_fases; nfa2++)
               {
               if(Abs(trec1->MatYparalelo_pu[nfa][nfa2]) > 0.)
                  {
                  flag_achou = true;
                  break;
                  }
               }
            }
         if(flag_achou)
            {
            fprintf(arq, "\n");
            for(int nfa=0; nfa < num_fases; nfa++)
               {
               fprintf(arq, "\n");
               for(int nfa2=0; nfa2 < num_fases; nfa2++)
                  {
                  y1 = trec1->MatYparalelo_pu[nfa][nfa2];
                  fprintf(arq, "  %13.6e %13.6e", y1.real(), y1.imag());
                  }
               }
            }
         }
      }
   }

// Imprime dados de mútuas
if(geralC->num_open_dados == 1)
   {
   num_linhas = 0;
   for(int ng=0; ng < rede3->lisGRUPO_MUTUA->Count; ng++)
      {
      grupo = (TGrupoMutua *) rede3->lisGRUPO_MUTUA->Items[ng];
      for(int nm=0; nm < grupo->lisMUTUA->Count; nm++)
         {
         mutua = (TMutua *) grupo->lisMUTUA->Items[nm];
         if(num_linhas == 0)
            {
            num_linhas = 1;
            fprintf(arq, "\n\n\n *** Dados de impedâncias mútuas ***"
                         "\n\n --------------------Trecho 1-------------------"
                         "     --------------------Trecho 2-------------------"
                         "       Grupo     Xm0 (ohm)   Xm1 (ohm)\n");
            }
         fprintf(arq, "\n %-15s %-15s %-15s     %-15s %-15s %-15s        "
                      "%3d     %9.4f   %9.4f",
                 mutua->trec1->bar1->codigo.c_str(),
                 mutua->trec1->bar2->codigo.c_str(),
                 mutua->trec1->codigo.c_str(),
                 mutua->trec2->bar1->codigo.c_str(),
                 mutua->trec2->bar2->codigo.c_str(),
                 mutua->trec2->codigo.c_str(),
                 ng, mutua->z0_ohm.imag(), mutua->z1_ohm.imag());
         }
      }
   }

// Imprime dados de capacitores série
if((geralC->num_open_dados == 1) && (rede3->lisCAPS->Count > 0))
   {
   fprintf(arq, "\n\n\n *** REDE PARCIAL A REDUZIR - Dados de capacitores série ***"
                "\n\n De              Para            Código            "
                "    Impedância série (ohm)       -Fases-\n");
   for(int ncs=0; ncs < rede3->lisCAPS->Count; ncs++)
      {
      caps1 = (TCapSerie *) rede3->lisCAPS->Items[ncs];
      zater = czero;
      if(Abs(caps1->yser_s) > 0.) zater = cum / caps1->yser_s;
      fprintf(arq, "\n %-15s %-15s %-15s    %13.6e %13.6e    ",
              caps1->bar1->codigo.c_str(),
              caps1->bar2->codigo.c_str(),
              caps1->codigo.c_str(),
              zater.real(), zater.imag());
      for(int nfa=0; nfa < caps1->fases->NumFases(); nfa++)
         {
         fprintf(arq, " %c", caps1->fases->FaseAsCharIndice(nfa));
         }
      }
   }

// Imprime dados de transformadores monofásicos de 2 enrolamentos
if((geralC->num_open_dados == 1) && (lisTRAFO12_ORD->Count > 0))
   {
   fprintf(arq, "\n\n\n *** REDE PARCIAL A REDUZIR - Dados de trafos "
                "monofásicos de 2 enrolamentos ***"
                "\n\n De              Para               Fases       "
                "Tensões nom. (kV)"
                "    P.nom.(MVA)        Imped. (pu)       At     Imp. aterr. "
                "(ohm)       Tap (pu)"
                "\n                                  Prim. Sec.      "
                "Prim.      Sec.\n");
   for(int nt=0; nt < lisTRAFO12_ORD->Count; nt++)
      {
		trafo12 = (TTrafo12 *) lisTRAFO12_ORD->Items[nt];
      if((trafo12->tipo == AUTOTRAFO4) || (trafo12->tipo == AUTOTRAFO6_1T) ||
			(trafo12->tipo == AUTOTRAFO6_2T))
         {
         soma1 = trafo12->snom_mva / (trafo12->vnom2_kv * trafo12->vnom2_kv); // Ref. secundário
         }
      else  // TRAFO4 ou TRAFO5
         {
         soma1 = trafo12->snom_mva / (trafo12->vnom1_kv * trafo12->vnom1_kv); // Ref. primário
         }
      tensao    = trafo12->ztrafo_ohm * soma1;
		tap1      = trafo12->vet_tap[0] * trafo12->vnom1_kv / trafo12->vnom2_kv;
      num_fases = trafo12->fases_pri->NumFases();
      char1     = trafo12->fases_pri->FaseAsCharIndice(0);
      char2     = ' ';
      char3     = ' ';
      if(num_fases >= 2) char2 = trafo12->fases_pri->FaseAsCharIndice(1);
      if(num_fases >= 3) char3 = trafo12->fases_pri->FaseAsCharIndice(2);
      num_fases = trafo12->fases_sec->NumFases();
      char4     = trafo12->fases_sec->FaseAsCharIndice(0);
      char5     = ' ';
      char6     = ' ';
      if(num_fases >= 2) char5 = trafo12->fases_sec->FaseAsCharIndice(1);
      if(num_fases >= 3) char6 = trafo12->fases_sec->FaseAsCharIndice(2);
      fprintf(arq, "\n %-15s %-15s   %c%c%c   %c%c%c   %8.3f   %8.3f     "
                   "%8.4f     %9.6f %9.6f    %c%c",
              trafo12->bar1->codigo.c_str(),
              trafo12->bar2->codigo.c_str(),
              char1, char2, char3, char4, char5, char6,
              trafo12->vnom1_kv, trafo12->vnom2_kv,
              trafo12->snom_mva, tensao.real(), tensao.imag(),
              codigo_ater[trafo12->tipo_ater1],
              codigo_ater[trafo12->tipo_ater2]);
		if((trafo12->tipo_ater1 == atZ) && (trafo12->tipo_ater2 == atZ) &&
         (! geralC->flag_ater_comuns))
         {
         soma2 = trafo12->bar1->VnomFase_kv(faseN);
         ybase = geralC->Sbase_fase / (soma2 * soma2);
         zater = cum / (trafo12->yater1_pu * ybase);
         fprintf(arq, "  %10.3e %10.3e     1:%8.6f",
					  zater.real(), zater.imag(), tap1);
			if(trafo12->NumTaps == 2) fprintf(arq, "  1:%8.6f", trafo12->vet_tap[1]);
         soma2 = trafo12->bar2->VnomFase_kv(faseN);
         ybase = geralC->Sbase_fase / (soma2 * soma2);
         zater = cum / (trafo12->yater2_pu * ybase);
         fprintf(arq, "\n                                  "
                      "                                    "
                      "                                    "
                      "    %10.3e %10.3e",
                 zater.real(), zater.imag());
         }
      else if(trafo12->tipo_ater1 == atZ)
         {
         soma2 = trafo12->bar1->VnomFase_kv(faseN);
         ybase = geralC->Sbase_fase / (soma2 * soma2);
         zater = cum / (trafo12->yater1_pu * ybase);
         fprintf(arq, "  %10.3e %10.3e     1:%8.6f",
					  zater.real(), zater.imag(), tap1);
			if(trafo12->NumTaps == 2) fprintf(arq, "  1:%8.6f", trafo12->vet_tap[1]);
         }
      else if(trafo12->tipo_ater2 == atZ)
         {
         soma2 = trafo12->bar2->VnomFase_kv(faseN);
         ybase = geralC->Sbase_fase / (soma2 * soma2);
         zater = cum / (trafo12->yater2_pu * ybase);
         fprintf(arq, "  %10.3e %10.3e     1:%8.6f",
					  zater.real(), zater.imag(), tap1);
			if(trafo12->NumTaps == 2) fprintf(arq, "  1:%8.6f", trafo12->vet_tap[1]);
         }
      else
         {
         fprintf(arq, "                            1:%8.6f",
					  tap1);
			if(trafo12->NumTaps == 2) fprintf(arq, "  1:%8.6f", trafo12->vet_tap[1]);
         }
      p1 = trafo12->ypar_S.real() / soma1;
		if((trafo12->tipo == AUTOTRAFO6_1T) || (trafo12->tipo == AUTOTRAFO6_2T))
         {
         p1 *= 2.;                 // COM ou SEM neutro (duas pernas)
         if(trafo12->NeutroSec())  // COM neutro: acerto pela tensão de fase
            {
            fmult  = trafo12->bar2->VnomFase_kv(trafo12->fases_sec->Fase(0))
                   / trafo12->vnom2_kv;
            p1    *= (fmult * fmult);
            }
         }
      str1 = " ";
      if(trafo12->bancoDF != NULL)
         {
         naux = rede3->lisBANCO_DF->IndexOf(trafo12->bancoDF) + 1;
         str1.sprintf("%d", naux);
         }
      fprintf(arq, "\n      %-15s     %6s     Pfe: %6.4f pu     Banco DF: %s",
              trafo12->codigo.c_str(), codigo_tipo_12[trafo12->tipo].c_str(),
              p1, str1.c_str());
      if(trafo12->fase_comum >= 0)
         fprintf(arq, "     Fase comum: %c",
                 trafo12->fases_pri->FaseAsChar(trafo12->fase_comum));
      }
   }

// Imprime dados de transformadores trifásicos de 2 enrolamentos
if((geralC->num_open_dados == 1) && (lisTRAFO32_ORD->Count > 0))
   {
   fprintf(arq, "\n\n\n *** REDE PARCIAL A REDUZIR - Dados de trafos trifásicos de 2 "
                "enrolamentos ***"
                "\n\n De              Para             Lig.    "
                "Tensões nom. (kV)     "
                "P.nom.(MVA)      Imped. (pu)      At     Imp. aterr. (ohm)"
                "               Taps (pu)"
                "\n                                           "
                "Prim.      Sec.\n");
   for(int nt=0; nt < lisTRAFO32_ORD->Count; nt++)
      {
      trafo32 = (TTrafo32 *) lisTRAFO32_ORD->Items[nt];
      soma1   = trafo32->snom_mva / (trafo32->vnom2_kv * trafo32->vnom2_kv);
      tensao  = trafo32->ztrafo_ohm * soma1;
      p1      = trafo32->ypar_S.real() / soma1;
      soma1   = trafo32->vnom1_kv / trafo32->vnom2_kv;
      tap1    = trafo32->vet_tap[0] * soma1;
      tap2    = trafo32->vet_tap[1] * soma1;
      tap3    = trafo32->vet_tap[2] * soma1;
      fprintf(arq, "\n %-15s %-15s   %c%c   %8.3f   %8.3f  "
                   "    %8.4f   %9.6f %9.6f   %c%c",
              trafo32->bar1->codigo.c_str(),
              trafo32->bar2->codigo.c_str(),
              codigo_lig_enrol[trafo32->tipo_lig1],
              codigo_lig_enrol[trafo32->tipo_lig2],
              trafo32->vnom1_kv, trafo32->vnom2_kv,
              trafo32->snom_mva,
              tensao.real(), tensao.imag(),
              codigo_ater[trafo32->tipo_ater1],
              codigo_ater[trafo32->tipo_ater2]);
      if((trafo32->tipo_ater1 == atZ) && (trafo32->tipo_ater2 == atZ) &&
         (! geralC->flag_ater_comuns))
         {
         soma2 = trafo32->bar1->VnomFase_kv(faseN);
         ybase = geralC->Sbase_fase / (soma2 * soma2);
         zater = cum / (trafo32->yater1_pu * ybase);
         fprintf(arq, "  %10.3e %10.3e  1:%8.6f 1:%8.6f 1:%8.6f",
                 zater.real(), zater.imag(), tap1, tap2, tap3);
         soma2 = trafo32->bar2->VnomFase_kv(faseN);
         ybase = geralC->Sbase_fase / (soma2 * soma2);
         zater = cum / (trafo32->yater2_pu * ybase);
         fprintf(arq, "\n      %-15s"
                      "  %-9s   NF: %1d/%1d   Pfe: %6.4f pu    "
                      "                                       "
                      "%10.3e %10.3e",
                 trafo32->codigo.c_str(), codigo_tipo_32[trafo32->tipo].c_str(),
                 trafo32->fases_pri->NumFases(), trafo32->fases_sec->NumFases(),
                 p1, zater.real(), zater.imag());
         }
      else if(trafo32->tipo_ater1 == atZ)
         {
         soma2 = trafo32->bar1->VnomFase_kv(faseN);
         ybase = geralC->Sbase_fase / (soma2 * soma2);
         zater = cum / (trafo32->yater1_pu * ybase);
         fprintf(arq, "  %10.3e %10.3e  1:%8.6f 1:%8.6f 1:%8.6f",
                 zater.real(), zater.imag(), tap1, tap2, tap3);
         fprintf(arq, "\n      %-15s  %-9s   NF: %1d/%1d   Pfe: %6.4f pu",
                 trafo32->codigo.c_str(), codigo_tipo_32[trafo32->tipo].c_str(),
                 trafo32->fases_pri->NumFases(), trafo32->fases_sec->NumFases(),
                 p1);
         }
      else if(trafo32->tipo_ater2 == atZ)
         {
         soma2 = trafo32->bar2->VnomFase_kv(faseN);
         ybase = geralC->Sbase_fase / (soma2 * soma2);
         zater = cum / (trafo32->yater2_pu * ybase);
         fprintf(arq, "  %10.3e %10.3e  1:%8.6f 1:%8.6f 1:%8.6f",
                 zater.real(), zater.imag(), tap1, tap2, tap3);
         fprintf(arq, "\n      %-15s  %-9s   NF: %1d/%1d   Pfe: %6.4f pu",
                 trafo32->codigo.c_str(), codigo_tipo_32[trafo32->tipo].c_str(),
                 trafo32->fases_pri->NumFases(), trafo32->fases_sec->NumFases(),
                 p1);
         }
      else
         {
         fprintf(arq, "                         1:%8.6f 1:%8.6f 1:%8.6f",
                 tap1, tap2, tap3);
         fprintf(arq, "\n      %-15s  %-9s   NF: %1d/%1d   Pfe: %6.4f pu",
                 trafo32->codigo.c_str(), codigo_tipo_32[trafo32->tipo].c_str(),
                 trafo32->fases_pri->NumFases(), trafo32->fases_sec->NumFases(),
                 p1);
         }
      }
   }

// Imprime dados de transformadores trifásicos de 3 enrolamentos
if((geralC->num_open_dados == 1) && (rede3->lisTRAFO33->Count > 0))
   {
   fprintf(arq, "\n\n\n *** REDE PARCIAL A REDUZIR - Dados de trafos trifásicos de 3 "
                "enrolamentos ***"
                "\n\n Barra              Código            Tipo    At   NF   "
                "Pot. nom.   Vnom/Vtap                                   "
                "Impedâncias"
                "\n                                      lig.                "
                "(MVA)                         Trafo (pu base %5.1f MVA)     "
                "      Aterramento (ohm)",
           geralC->Sbase);
   for(int nt=0; nt < rede3->lisTRAFO33->Count; nt++)
      {
      fprintf(arq, "\n");
      trafo33 = (TTrafo33 *) rede3->lisTRAFO33->Items[nt];
      char1   = char2 = char3 = char4 = char5 = char6 = ' ';
      if     (trafo33->enrol_Sref == 0) char1 = 'S';
      else if(trafo33->enrol_Sref == 1) char3 = 'S';
      else                              char5 = 'S';
      if     (trafo33->enrol_Ypar == 0) char2 = 'Y';
      else if(trafo33->enrol_Ypar == 1) char4 = 'Y';
      else                              char6 = 'Y';
      fprintf(arq, "\n P %-15s  %-15s   %c%c         %c      %c    %1d   %6.1f"
                   "       %8.6f       PS %13.6e %13.6e",
              trafo33->bar1->codigo.c_str(),
              trafo33->codigo.c_str(),
              char1,
              char2,
              codigo_lig_enrol[trafo33->tipo_lig1],
              codigo_ater[trafo33->tipo_ater1],
              trafo33->fases_pri->NumFases(),
              trafo33->snom1_mva,
              (trafo33->vnom1_kv/trafo33->vtap1_kv),
              trafo33->zps1_pu.real(),
              trafo33->zps1_pu.imag());
      if(trafo33->tipo_ater1 == atZ)
         {
         soma2 = trafo33->bar1->VnomFase_kv(faseN);
         ybase = geralC->Sbase_fase / (soma2 * soma2);
         zater = cum / (trafo33->yater1_pu * ybase);
         fprintf(arq, "    %13.6e %13.6e",
                 zater.real(), zater.imag());
         }
      else
         {
         fprintf(arq, "                               ");
         }
      fprintf(arq, "      %9.6f", trafo33->ypar_pu.real());

      fprintf(arq, "\n S %-15s  %-7s           %c%c         %c      %c"
                   "    %1d   %6.1f       %8.6f       ST %13.6e %13.6e",
              trafo33->bar2->codigo.c_str(),
              geralC->codigo_tipo_33[trafo33->tipo].c_str(),
              char3,
              char4,
              codigo_lig_enrol[trafo33->tipo_lig2],
              codigo_ater[trafo33->tipo_ater2],
              trafo33->fases_sec->NumFases(),
              trafo33->snom2_mva,
              (trafo33->vnom2_kv/trafo33->vtap2_kv),
              trafo33->zst1_pu.real(),
              trafo33->zst1_pu.imag());
      if(trafo33->tipo_ater2 == atZ)
         {
         soma2 = trafo33->bar2->VnomFase_kv(faseN);
         ybase = geralC->Sbase_fase / (soma2 * soma2);
         zater = cum / (trafo33->yater2_pu * ybase);
         fprintf(arq, "    %13.6e %13.6e",
                 zater.real(), zater.imag());
         }

      fprintf(arq, "\n T %-15s                    %c%c         %c      %c"
                   "    %1d   %6.1f       %8.6f       TP %13.6e %13.6e",
              trafo33->bar3->codigo.c_str(),
              char5,
              char6,
              codigo_lig_enrol[trafo33->tipo_lig3],
              codigo_ater[trafo33->tipo_ater3],
              trafo33->fases_ter->NumFases(),
              trafo33->snom3_mva,
              (trafo33->vnom3_kv/trafo33->vtap3_kv),
              trafo33->zpt1_pu.real(),
              trafo33->zpt1_pu.imag());
      if(trafo33->tipo_ater3 == atZ)
         {
         soma2 = trafo33->bar3->VnomFase_kv(faseN);
         ybase = geralC->Sbase_fase / (soma2 * soma2);
         zater = cum / (trafo33->yater3_pu * ybase);
         fprintf(arq, "    %13.6e %13.6e",
                 zater.real(), zater.imag());
         }
      }
   }

// Imprime dados de transformadores equivalentes
if((geralC->num_open_dados == 1) && (rede3->lisTRAFOEQUIV->Count > 0))
   {
   fprintf(arq, "\n\n\n *** REDE PARCIAL A REDUZIR - Dados de trafos "
                "equivalentes - redução da rede "
                "(Submatriz Ynodal(i,j) (pu)) ***"
                "\n\n De              Para            Código          "
                "Def.P->S (°)     --Fases--          Ynodal1/0(i,j) (pu)       "
                "    Ynodal1/0(j,i) (pu)");
   for(int nt=0; nt < rede3->lisTRAFOEQUIV->Count; nt++)
      {
      trafoequiv    = (TTrafoEquiv *) rede3->lisTRAFOEQUIV->Items[nt];
      fases_pri     = trafoequiv->fases_pri;
      fases_sec     = trafoequiv->fases_sec;
      num_fases_pri = fases_pri->NumFases();
      num_fases_sec = fases_sec->NumFases();
      fprintf(arq, "\n\n %-15s %-15s %-15s  %7.1f        ",
              trafoequiv->bar1->codigo.c_str(),
              trafoequiv->bar2->codigo.c_str(),
              trafoequiv->codigo.c_str(),
              (trafoequiv->defasagem*radgrau));
      for(int nfa=0; nfa < num_fases_pri; nfa++)
         {
         fprintf(arq, "%c", fases_pri->FaseAsCharIndice(nfa));
         }
      fprintf(arq, " ");
      for(int nfa=0; nfa < num_fases_sec; nfa++)
         {
         fprintf(arq, "%c", fases_sec->FaseAsCharIndice(nfa));
         }
      for(int nfa=(num_fases_pri+num_fases_sec+1); nfa < ((2*MAX_FASES_G3)+1); nfa++)
         {
         fprintf(arq, " ");
         }
      fprintf(arq, "     %13.6e %13.6e   %13.6e %13.6e",
              trafoequiv->Ynodal1_ij_pu.real(), trafoequiv->Ynodal1_ij_pu.imag(),
              trafoequiv->Ynodal1_ji_pu.real(), trafoequiv->Ynodal1_ji_pu.imag());
      fprintf(arq, "\n                                      "
                   "                                        "
                   "%13.6e %13.6e   %13.6e %13.6e",
              trafoequiv->Ynodal0_ij_pu.real(), trafoequiv->Ynodal0_ij_pu.imag(),
              trafoequiv->Ynodal0_ji_pu.real(), trafoequiv->Ynodal0_ji_pu.imag());
      for(int i=0; i < num_fases_pri; i++)
         {
         fprintf(arq, "\n");
         for(int j=0; j < num_fases_sec; j++)
            {
            z1 = trafoequiv->MatYnodal_ij_pu[i][j];
            fprintf(arq, "  %13.6e %13.6e", z1.real(), z1.imag());
            }
         }
      for(int i=0; i < num_fases_sec; i++)
         {
         fprintf(arq, "\n");
         for(int j=0; j < num_fases_pri; j++)
            {
            z1 = trafoequiv->MatYnodal_ji_pu[i][j];
            fprintf(arq, "  %13.6e %13.6e", z1.real(), z1.imag());
            }
         }
      }
   }

// Imprime dados de transformadores ZZ
if((geralC->num_open_dados == 1) && (rede3->lisTRAFOZZ->Count > 0))
   {
   fprintf(arq, "\n\n\n *** REDE PARCIAL A REDUZIR - Dados de trafos ZigZag ***"
                "\n\n Barra              Código            P.nom.(MVA)"
                "               Z0 (pu)                  Admit. fase (S)\n");
   for(int nt=0; nt < rede3->lisTRAFOZZ->Count; nt++)
      {
      trafozz = (TTrafoZZ *) rede3->lisTRAFOZZ->Items[nt];
      ybase   = trafozz->snom_mva / (trafozz->vnom_kv * trafozz->vnom_kv);
      z1      = trafozz->z0_ohm * ybase;
      fprintf(arq, "\n %-15s    %-15s     %7.3f      %13.6e %13.6e   %13.6e %13.6e",
              trafozz->barra->codigo.c_str(),
              trafozz->codigo.c_str(),
              trafozz->snom_mva,
              z1.real(), z1.imag(),
              trafozz->admit_fase_S.real(), trafozz->admit_fase_S.imag());
      }
   }

// Imprime dados de reguladores de tensão monofásicos
if((geralC->num_open_dados == 1) && (rede3->lisREG12->Count > 0))
   {
   fprintf(arq, "\n\n\n *** REDE PARCIAL A REDUZIR - Dados de reguladores de "
                "tensão monofásicos ***"
                "\n\n De              Para               Fases       "
                "Tensões nom. (kV)"
                "    P.nom.(MVA)        Imped. (pu)       At     Imp. aterr. "
                "(ohm)       Tap (pu)"
                "\n                                  Prim. Sec.      "
                "Prim.      Sec.\n");
   for(int nt=0; nt < rede3->lisREG12->Count; nt++)
      {
      reg12 = (TReg12 *) rede3->lisREG12->Items[nt];
      if((reg12->tipo == AUTOTRAFO4) || (reg12->tipo == AUTOTRAFO6_1T) ||
			(reg12->tipo == AUTOTRAFO6_2T))
			{
         soma1 = reg12->snom_mva / (reg12->vnom2_kv * reg12->vnom2_kv); // Ref. secundário
         }
      else  // TRAFO4 ou TRAFO5
         {
         soma1 = reg12->snom_mva / (reg12->vnom1_kv * reg12->vnom1_kv); // Ref. primário
         }
      tensao    = reg12->ztrafo_ohm * soma1;
		tap1      = reg12->vet_tap[0] * reg12->vnom1_kv / reg12->vnom2_kv;
      num_fases = reg12->fases_pri->NumFases();
      char1     = reg12->fases_pri->FaseAsCharIndice(0);
      char2     = ' ';
      char3     = ' ';
      if(num_fases >= 2) char2 = reg12->fases_pri->FaseAsCharIndice(1);
      if(num_fases >= 3) char3 = reg12->fases_pri->FaseAsCharIndice(2);
      num_fases = reg12->fases_sec->NumFases();
      char4     = reg12->fases_sec->FaseAsCharIndice(0);
      char5     = ' ';
      char6     = ' ';
      if(num_fases >= 2) char5 = reg12->fases_sec->FaseAsCharIndice(1);
      if(num_fases >= 3) char6 = reg12->fases_sec->FaseAsCharIndice(2);
      fprintf(arq, "\n %-15s %-15s   %c%c%c  %c%c%c   %8.3f   %8.3f     "
                   "%8.4f     %9.6f %9.6f    %c%c",
				  reg12->bar1->codigo.c_str(),
              reg12->bar2->codigo.c_str(),
              char1, char2, char3, char4, char5, char6,
              reg12->vnom1_kv, reg12->vnom2_kv,
              reg12->snom_mva, tensao.real(), tensao.imag(),
              codigo_ater[reg12->tipo_ater1],
              codigo_ater[reg12->tipo_ater2]);
      if((reg12->tipo_ater1 == atZ) && (reg12->tipo_ater2 == atZ) &&
         (! geralC->flag_ater_comuns))
         {
         soma2 = reg12->bar1->VnomFase_kv(faseN);
         ybase = geralC->Sbase_fase / (soma2 * soma2);
         zater = cum / (reg12->yater1_pu * ybase);
         fprintf(arq, "  %10.3e %10.3e     1:%8.6f",
					  zater.real(), zater.imag(), tap1);
			if(reg12->NumTaps == 2) fprintf(arq, "  1:%8.6f", reg12->vet_tap[1]);
         soma2 = reg12->bar2->VnomFase_kv(faseN);
         ybase = geralC->Sbase_fase / (soma2 * soma2);
         zater = cum / (reg12->yater2_pu * ybase);
         fprintf(arq, "\n                                                "
                      "                           "
                      "                           "
                      "  %10.3e %10.3e",
                 zater.real(), zater.imag());
         }
      else if(reg12->tipo_ater1 == atZ)
         {
         soma2 = reg12->bar1->VnomFase_kv(faseN);
         ybase = geralC->Sbase_fase / (soma2 * soma2);
         zater = cum / (reg12->yater1_pu * ybase);
         fprintf(arq, "  %10.3e %10.3e     1:%8.6f",
					  zater.real(), zater.imag(), tap1);
			if(reg12->NumTaps == 2) fprintf(arq, "  1:%8.6f", reg12->vet_tap[1]);
         }
      else if(reg12->tipo_ater2 == atZ)
         {
         soma2 = reg12->bar2->VnomFase_kv(faseN);
         ybase = geralC->Sbase_fase / (soma2 * soma2);
         zater = cum / (reg12->yater2_pu * ybase);
         fprintf(arq, "  %10.3e %10.3e     1:%8.6f",
					  zater.real(), zater.imag(), tap1);
			if(reg12->NumTaps == 2) fprintf(arq, "  1:%8.6f", reg12->vet_tap[1]);
			}
      else
         {
         fprintf(arq, "                            1:%8.6f",
					  tap1);
			if(reg12->NumTaps == 2) fprintf(arq, "  1:%8.6f", reg12->vet_tap[1]);
         }
      f1 = reg12->fases_sec->Fase(0);  // Fase 1 do secundário
      f2 = reg12->fases_sec->Fase(1);  // Fase 2 do secundário
		// Para AUTOTRAFO6 e TRAFO5 considera as duas fases que não são comuns
      if((reg12->tipo == AUTOTRAFO6_1T) || (reg12->tipo == AUTOTRAFO6_2T) ||
			(reg12->tipo == TRAFO5))  // 3 terminais no sec.
			{
         if(f1 == reg12->fase_comum)
            {
            f1 = f2;
            f2 = reg12->fases_sec->Fase(2);
            }
         else if(f2 == reg12->fase_comum)
            {
            f2 = reg12->fases_sec->Fase(2);
            }
         }
      p1 = reg12->ypar_S.real() / soma1;
      if((reg12->tipo == AUTOTRAFO6_1T) || (reg12->tipo == AUTOTRAFO6_2T))
         {
         p1 *= 2.;               // COM ou SEM neutro (duas pernas)
         if(reg12->NeutroSec())  // COM neutro: acerto pela tensão de fase
            {
            fmult  = reg12->bar2->VnomFase_kv(reg12->fases_sec->Fase(0))
                   / reg12->vnom2_kv;
            p1    *= (fmult * fmult);
            }
         }
      str1 = "         ";
      if(reg12->IsDOIS_SENTIDOS())
         {
         str1 = geralC->codigo_sub_modelo_regu[reg12->sub_modelo].c_str();
         }
      if(reg12->NumTaps == 1)  // Todos os tipos exceto AUTOTRAFO6_2T
         {
         fprintf(arq, "\n      %-15s  %6s  Pfe(pu): %6.4f  FC: %c  "
                      "Mod.: %8s  Sub-mod.: %9s  "
                      "Var.tensão (pu): %5.3f  Núm.passos: %2d",
                 reg12->codigo.c_str(),
                 codigo_tipo_12[reg12->tipo].c_str(),
                 p1,
                 reg12->fases_pri->FaseAsChar(reg12->fase_comum),
                 geralC->codigo_modelo_regu[reg12->modelo].c_str(),
                 str1.c_str(),
                 reg12->var_tensao_pu,
                 reg12->num_passo);
         if(reg12->IsENERQCT())
            {
            fprintf(arq, "\n      B.ref.: %-15s  Fases bar.ref.: %c%c",
                    reg12->bar_ref_12->codigo.c_str(),
                    reg12->fases_pri->FaseAsChar(f1),
                    reg12->fases_pri->FaseAsChar(f2));
            }
         else if(reg12->IsPADRAO() || reg12->IsDOIS_SENTIDOS())
            {
            fprintf(arq, "\n      B.ref.12: %-15s  Fases bar.ref.12: %c%c  "
                         "Vref.12(pu): %6.4f  BM12(pu): %5.3f",
                    reg12->bar_ref_12->codigo.c_str(),
                    reg12->fases_pri->FaseAsChar(f1),
                    reg12->fases_pri->FaseAsChar(f2),
                    reg12->vref_12_pu,
                    reg12->banda_morta_pu);
            if(reg12->IsDOIS_SENTIDOS())
               {
               fprintf(arq, "      B.ref.21: %-15s  Fases bar.ref.21: %c%c  "
                            "Vref.21(pu): %6.4f  BM21(pu): %5.3f",
                       reg12->bar_ref_21->codigo.c_str(),
                       reg12->fases_pri->FaseAsChar(f1),
                       reg12->fases_pri->FaseAsChar(f2),
                       reg12->vref_21_pu,
                       reg12->banda_morta_inv_pu);
               }
            }
         }
      else  // AUTOTRAFO6_2T
         {
         char1 = reg12->fases_pri->FaseAsChar(f1);
         char2 = reg12->fases_pri->FaseAsChar(reg12->fase_comum);
         char3 = reg12->fases_pri->FaseAsChar(f2);
         char4 = char2;
         if(((char1 == 'A') && (char2 == 'C')) ||
            ((char1 == 'B') && (char2 == 'A')) ||
            ((char1 == 'C') && (char2 == 'B')))
            {
            char5 = char1; char1 = char2; char2 = char5;
            }
         if(((char3 == 'A') && (char4 == 'C')) ||
            ((char3 == 'B') && (char4 == 'A')) ||
            ((char3 == 'C') && (char4 == 'B')))
            {
            char5 = char3; char3 = char4; char4 = char5;
            }
         fprintf(arq, "\n      %-15s  %6s  Pfe(pu): %6.4f  FC: %c  "
                      "Mod.: %8s  Sub-mod.: %9s  "
                      "Var.tensão (pu): %5.3f  Núm.passos: %2d",
                 reg12->codigo.c_str(),
                 codigo_tipo_12[reg12->tipo].c_str(),
                 p1,
                 reg12->fases_pri->FaseAsChar(reg12->fase_comum),
                 geralC->codigo_modelo_regu[reg12->modelo].c_str(),
                 str1.c_str(),
                 reg12->var_tensao_pu,
                 reg12->num_passo);
         if(reg12->IsENERQCT())
            {
            fprintf(arq, "\n      B.ref.: %-15s  Fases bar.ref.: %c%c/%c%c",
                    reg12->bar_ref_12->codigo.c_str(),
                    char1,
                    char2,
                    char3,
                    char4);
            }
         else if(reg12->IsPADRAO() || reg12->IsDOIS_SENTIDOS())
            {
            fprintf(arq, "\n      B.ref.12: %-15s  Fases bar.ref.12: %c%c/%c%c"
                         "  Vref.12(pu): %6.4f  BM12(pu): %5.3f",
                    reg12->bar_ref_12->codigo.c_str(),
                    char1,
                    char2,
                    char3,
                    char4,
                    reg12->vref_12_pu,
                    reg12->banda_morta_pu);
            if(reg12->IsDOIS_SENTIDOS())
               {
               fprintf(arq, "      B.ref.21: %-15s  Fases bar.ref.21: %c%c/%c%c"
                            "  Vref.21(pu): %6.4f  BM21(pu): %5.3f",
                       reg12->bar_ref_21->codigo.c_str(),
                       char1,
                       char2,
                       char3,
                       char4,
                       reg12->vref_21_pu,
                       reg12->banda_morta_inv_pu);
               }
            }
         }
      }
   }

// Imprime dados de reguladores de tensão trifásicos
if((geralC->num_open_dados == 1) && (rede3->lisREG32->Count > 0))
   {
   fprintf(arq, "\n\n\n *** REDE PARCIAL A REDUZIR - Dados de reguladores de tensão "
                "trifásicos ***"
                "\n\n De              Para             Lig.    "
                "Tensões nom. (kV)     "
                "P.nom.(MVA)      Imped. (pu)      At     Imp. aterr. (ohm)"
                "               Taps (pu)"
                "\n                                           "
                "Prim.      Sec.\n");
   for(int nt=0; nt < rede3->lisREG32->Count; nt++)
      {
      reg32  = (TReg32 *) rede3->lisREG32->Items[nt];
      soma1  = reg32->snom_mva / (reg32->vnom2_kv * reg32->vnom2_kv);
      tensao = reg32->ztrafo_ohm * soma1;
      p1     = reg32->ypar_S.real() / soma1;
      soma1  = reg32->vnom1_kv / reg32->vnom2_kv;
      tap1   = reg32->vet_tap[0] * soma1;
      tap2   = reg32->vet_tap[1] * soma1;
      tap3   = reg32->vet_tap[2] * soma1;
      fprintf(arq, "\n %-15s %-15s   %c%c   %8.3f   %8.3f  "
                   "    %8.4f   %9.6f %9.6f   %c%c",
              reg32->bar1->codigo.c_str(),
              reg32->bar2->codigo.c_str(),
              codigo_lig_enrol[reg32->tipo_lig1],
              codigo_lig_enrol[reg32->tipo_lig2],
              reg32->vnom1_kv, reg32->vnom2_kv,
              reg32->snom_mva,
              tensao.real(), tensao.imag(),
              codigo_ater[reg32->tipo_ater1],
              codigo_ater[reg32->tipo_ater2]);
      if((reg32->tipo_ater1 == atZ) && (reg32->tipo_ater2 == atZ) &&
         (! geralC->flag_ater_comuns))
         {
         soma2 = reg32->bar1->VnomFase_kv(faseN);
         ybase = geralC->Sbase_fase / (soma2 * soma2);
         zater = cum / (reg32->yater1_pu * ybase);
         fprintf(arq, "  %10.3e %10.3e  1:%8.6f 1:%8.6f 1:%8.6f",
                 zater.real(), zater.imag(), tap1, tap2, tap3);
         soma2 = reg32->bar2->VnomFase_kv(faseN);
         ybase = geralC->Sbase_fase / (soma2 * soma2);
         zater = cum / (reg32->yater2_pu * ybase);
         fprintf(arq, "\n                       %-15s"
                      "                                "
                      "                                "
                      "%10.3e %10.3e",
                 reg32->codigo.c_str(), zater.real(), zater.imag());
         }
      else if(reg32->tipo_ater1 == atZ)
         {
         soma2 = reg32->bar1->VnomFase_kv(faseN);
         ybase = geralC->Sbase_fase / (soma2 * soma2);
         zater = cum / (reg32->yater1_pu * ybase);
         fprintf(arq, "  %10.3e %10.3e  1:%8.6f 1:%8.6f 1:%8.6f",
                 zater.real(), zater.imag(), tap1, tap2, tap3);
         fprintf(arq, "\n                       %-15s",
                 reg32->codigo.c_str());
         }
      else if(reg32->tipo_ater2 == atZ)
         {
         soma2 = reg32->bar2->VnomFase_kv(faseN);
         ybase = geralC->Sbase_fase / (soma2 * soma2);
         zater = cum / (reg32->yater2_pu * ybase);
         fprintf(arq, "  %10.3e %10.3e  1:%8.6f 1:%8.6f 1:%8.6f",
                 zater.real(), zater.imag(), tap1, tap2, tap3);
         fprintf(arq, "\n                       %-15s",
                 reg32->codigo.c_str());
         }
      else
         {
         fprintf(arq, "                         1:%8.6f 1:%8.6f 1:%8.6f",
                 tap1, tap2, tap3);
         }
      str1 = "         ";
      if(reg32->IsDOIS_SENTIDOS())
         {
         str1 = geralC->codigo_sub_modelo_regu[reg32->sub_modelo].c_str();
         }
      fprintf(arq, "\n      %-15s  %-9s  NF: %1d/%1d  %8s  %9s  Pfe(pu): %6.4f"
                   "  Var.tensão (pu): %5.3f  Núm.passos: %2d",
              reg32->codigo.c_str(),
              codigo_tipo_32[reg32->tipo].c_str(),
              reg32->fases_pri->NumFases(),
              reg32->fases_sec->NumFases(),
              geralC->codigo_modelo_regu[reg32->modelo].c_str(),
              str1.c_str(),
              p1,
              reg32->var_tensao_pu,
              reg32->num_passo);
      if(reg32->IsENERQCT())
         {
         fprintf(arq, "\n      B.ref.: %-15s",
                 reg32->bar_ref_12->codigo.c_str());
         }
      else if(reg32->IsPADRAO() || reg32->IsDOIS_SENTIDOS())
         {
         fprintf(arq, "\n      B.ref.12: %-15s  Vref.12(pu): %6.4f  BM12(pu): %5.3f",
                 reg32->bar_ref_12->codigo.c_str(),
                 reg32->vref_12_pu,
                 reg32->banda_morta_pu);
         if(reg32->IsDOIS_SENTIDOS())
            {
            fprintf(arq, "      B.ref.21: %-15s  Vref.21(pu): %6.4f  BM21(pu): %5.3f",
                    reg32->bar_ref_21->codigo.c_str(),
                    reg32->vref_21_pu,
                    reg32->banda_morta_inv_pu);
            }
         }
      }
   }

// Imprime dados de cargas não-lineares
if((geralC->num_open_dados == 1) && (rede3->lisCNL->Count > 0))
   {
   fprintf(arq, "\n\n\n *** Dados de cargas não-lineares ***");
   // fprintf(arq, "\n\n Barra        F.pot.(pu)   Ordem freq.        "
   fprintf(arq, "\n\n Barra                     Ordem freq.        "
                      "-----Potências (kVA / ° para fund.) (pu fund. / ° demais)"
                      "------"
                      "\n                                                 "
                      "Mód. A    Âng. A      Mód. B    Âng. B      Mód. C    Âng. C");
   for(int nc=0; nc < rede3->lisCNL->Count; nc++)
      {
      cnl1      = (TCNL *) rede3->lisCNL->Items[nc];
      barra1    = cnl1->barra;
      fases     = barra1->fases;
      num_fases = fases->NumFases();
      fmult     = barra1->vnom_linha_kv * raiz3_inverso;
      for(int indfreq=0; indfreq < cnl1->num_harmonicas; indfreq++)
         {
         // if(indfreq == 0) fprintf(arq, "\n\n %-10s    %7.4f        ",
         //                          cnl1->barra->codigo.SubString(0, 10).c_str(),
         //                          cnl1->cos_fi);
         if(indfreq == 0) fprintf(arq, "\n\n %-15s              ",
                                cnl1->barra->codigo.c_str());
         else             fprintf(arq, "\n                              ");
         ordem = cnl1->vet_ordem[indfreq];
         fprintf(arq, "%4d         ", ordem);
         charstr1.sprintf("                      ");
         charstr2.sprintf("                      ");
         charstr3.sprintf("                      ");
         for(int nfa=0; nfa < num_fases; nfa++)
            {
            if(fases->Fase(nfa) == faseN) continue;  // Descarta o neutro
            iap = fases->IndiceABCN(nfa);
            p1  = cnl1->mat_corrente->D2[indfreq][iap];
            if(ordem == 1) p1 *= fmult; // Frequência fundamental: potência em kVA
            q1 = cnl1->mat_ang_rad->D2[indfreq][iap] * radgrau;
            if     (iap == 0) charstr1.sprintf("   %9.4f %9.4f", p1, q1); // Fase A
            else if(iap == 1) charstr2.sprintf("   %9.4f %9.4f", p1, q1); // Fase B
            else              charstr3.sprintf("   %9.4f %9.4f", p1, q1); // Fase C
            }
         fprintf(arq, "%22s%22s%22s", charstr1.c_str(), charstr2.c_str(),
                 charstr3.c_str());
         }
      }
   }

/*
// Imprime dados bancos de trafos monofásicos ligados em Delta-Fechado
if((geralC->num_open_dados == 1) && (rede3->lisBANCO_DF->Count > 0))
   {
   fprintf(arq, "\n\n\n *** Dados de bancos ligados em Delta-Fechado ***");
   for(int nb=0; nb < rede3->lisBANCO_DF->Count; nb++)
      {
      fprintf(arq, "\n");
      bancoDF = (TBancoDF *) rede3->lisBANCO_DF->Items[nb];
      for(int nt=0; nt < bancoDF->lisTRAFO->Count; nt++)
         {
         trafo12 = (TTrafo12 *) bancoDF->lisTRAFO->Items[nt];
         if(nt == 0)  // Primeiro trafo do banco
            {
            fprintf(arq, "\n Banco DF: %d"
                         "\n    %-15s - %-15s - codigo: %-15s - tipo: %-6s",
                    nb,
                    trafo12->bar1->codigo.c_str(),
                    trafo12->bar2->codigo.c_str(),
                    trafo12->codigo.c_str(),
                    codigo_tipo_12[trafo12->tipo].c_str());
            }
         else  // Demais trafos
            {
            fprintf(arq, "\n                                        "
                         "codigo: %-15s - tipo: %-6s",
                    trafo12->codigo.c_str(),
                    codigo_tipo_12[trafo12->tipo].c_str());
            }
         }
      }
   }
*/

// Imprime linhas da matriz Ynodal e do Jacobiano
if(geralC->num_open_dados == 1) ImprimeLinhasYnodalJacob(arq, NULL);
fclose(arq);

// Finalização
ImprimeSeparador(arqDADOS, 1, 89);

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TImpressao3::ImprimeLinhasYnodalJacob(FILE  *arq,
                                                      TList *lisBAR_J)
{
AnsiString mat_char[MAX_FASES_G3];
bool       flag_abriu;
int        ind, linY, nb2, num_fases;
TBarra     *barra1;
TBarraJ    *pbarJ;
TFase      *fases;
TYnodalJ   *pYJ;

// Proteção (só imprime dados de redes pequenas)
if(rede3->lisBAR->Count > 100) return(true);

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
if(lisBAR_J == NULL)  // Programa diferente de NR3
   {
   fprintf(arq, "\n\n\n *** Estrutura da matriz 'Ynodal' ***"
                "\n\n ------ Barra -------   Tipo     At"
                "    - Linhas/Cols. em Ynodal --"
                "\n                                            "
                "A      B      C      N\n");
   }
else  // NR3
   {
   fprintf(arq, "\n\n\n *** Estrutura das matrizes 'Ynodal' e 'Jacob' ***"
                "\n\n ------ Barra -------   Tipo     At"
                "    - Linhas/Cols. em Ynodal --    -- Linhas  em Ir / P3f ----"
                "    ------ Linhas  em Im ------");
   fprintf(arq, "\n                                                           "
                "           -- Colunas em Vr / Theta --"
                "    --- Colunas em Vm / Tap ---");
   fprintf(arq, "\n                                            "
                "A      B      C      N         A      B      C      N"
                "         A      B      C      N");
   fprintf(arq, "\n");
   }

// Loop de barras
for(int nb=0; nb < lisBAR_CODIGO->Count; nb++)
   {
   barra1    = (TBarra *) lisBAR_CODIGO->Items[nb];
   fases     = barra1->fases;
   num_fases = fases->NumFases();

   if(fases->ExisteN()) ind = barra1->tipo_ater;
   else                 ind = NUM_TIPO_ATERR;  // A barra não tem neutro

   // Imprime linhas em Ynodal
   fprintf(arq, "\n %-15s        %-5s     %c   ",
           barra1->codigo.c_str(),
           geralC->codigo_tipo_barra[barra1->tipo].c_str(),
           codigo_ater[ind]);
   for(int nfa=0; nfa < MAX_FASES_G3; nfa++)
      {
      mat_char[nfa] = "     -";
      }
   for(int nfa=0; nfa < num_fases; nfa++)
      {
      ind  = fases->IndiceABCN(nfa);
      linY = fases->LinhaIndice(nfa);
      if(linY < 0) mat_char[ind].sprintf("   REF"); // Referência ou barra desconexa
      else         mat_char[ind].sprintf("%6d", linY);
      }
   for(int nfa=0; nfa < MAX_FASES_G3; nfa++)
      {
      fprintf(arq, " %6s", mat_char[nfa].c_str());
      }

   // Verifica se o programa chamador é o NR3
   if(lisBAR_J == NULL) continue;  // Programa diferente do NR3, nada a fazer
   nb2   = rede3->lisBAR->IndexOf(barra1);
   pbarJ = (TBarraJ *) lisBAR_J->Items[nb2];

   // Imprime linhas em Jacob - equações em Ir ou P3f
   fprintf(arq, "   ");
   for(int nfa=0; nfa < MAX_FASES_G3; nfa++)
      {
      mat_char[nfa] = "     -";
      }
   for(int nfa=0; nfa < num_fases; nfa++)
      {
      pYJ = pbarJ->vet_YJ[nfa];
      if(pYJ == NULL) continue;
      ind = fases->IndiceABCN(nfa);
      if(pYJ->vet_linha_J[0] >= 0) mat_char[ind].sprintf("%6d", pYJ->vet_linha_J[0]); // Eq. em Ir / P3f
      }
   for(int nfa=0; nfa < MAX_FASES_G3; nfa++)
      {
      fprintf(arq, " %6s", mat_char[nfa].c_str());
      }

   // Imprime linhas em Jacob - equações em Im
   fprintf(arq, "   ");
   for(int nfa=0; nfa < MAX_FASES_G3; nfa++)
      {
      mat_char[nfa] = "     -";
      }
   for(int nfa=0; nfa < num_fases; nfa++)
      {
      pYJ = pbarJ->vet_YJ[nfa];
      if(pYJ == NULL) continue;
      ind = fases->IndiceABCN(nfa);
      if(pYJ->vet_linha_J[1] >= 0) mat_char[ind].sprintf("%6d", pYJ->vet_linha_J[1]); // Eq. em Im
      }
   for(int nfa=0; nfa < MAX_FASES_G3; nfa++)
      {
      fprintf(arq, " %6s", mat_char[nfa].c_str());
      }
   }

// Fecha arquivo, se necessário
if(flag_abriu) fclose(arq);

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TImpressao3::ImprimeLisYJ(TList *lisYJ)
{
// Variáveis locais
AnsiString codigo;
FILE       *arq;
TYnodalJ   *pYJ;

// Abre arquivo
arq = geralC->AbreArquivo(arqDADOS);
if(arq == NULL) return(false);

fprintf(arq, "\n\n\n *** lisYJ (%d elementos) ***\n", lisYJ->Count);
for(int ne=0; ne < lisYJ->Count; ne++)
   {
   pYJ = (TYnodalJ *) lisYJ->Items[ne];
   if(pYJ->barraJ == NULL) codigo = "NULL";
   else                    codigo = pYJ->barraJ->barra->codigo.SubString(0, 15);
   fprintf(arq, "\n   ne: %4d - barra: %-15s - tipo_no: %2d - fase_no: %2d - "
                "vet_linha_J: %4d %4d",
           ne, codigo.c_str(), pYJ->tipo_no, pYJ->fase_no, pYJ->vet_linha_J[0],
           pYJ->vet_linha_J[1]);
   }
fprintf(arq, "\n");
fclose(arq);

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TImpressao3::ImprimeResultados(int    np,
                                               TList  *lisBAR_SEM_CONV,
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
   imprimiu = ImprimeResultados1(np, lisBAR_SEM_CONV);   // Imprime resumo inicial
   if(imprimiu) num_vezes++;
   if(geralC->Programa1 != progESTIM3)                   // Todos menos ESTIM3
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
   else  // ESTIM3
      {
      if(geralC->tipo_relatorio >= trREL_DETALHADO_2)    // Imprime resultados por barra/ligação
         {
         imprimiu = ImprimeResultados2();
         if(imprimiu) num_vezes++;
         // ImprimeResultadosMedidoresExcel();              // Gera arquivo para Excel
         }
      imprimiu = ImprimeResultados3(np);                 // Imprime resumo final
      if(imprimiu) num_vezes++;
      imprimiu = ImprimeResultadosESTIM3(vet_desv_iter, lisBAR_REF);
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
bool __fastcall TImpressao3::ImprimeResultados1(int   np,
                                                TList *lisBAR_SEM_CONV)
{
// Local variables
bool            flag_3F, flag_FT;
int             fase, fases_curto, nelem, num_brancos, num_max1, num_max2;
AnsiString      string00, string01, string02, string03, string04, string05,
                string06, string07, string08, string09, string10, string11,
                string12;
complex<double> corrente;
FILE            *arq;
TBarra          *bar1;
TBarraDefeito   *bar_def;

// Abre arquivo
arq = geralC->AbreArquivo(arqRESULT);
if(arq == NULL) return(false);

// Impressão de resultados
string00 = geralC->codigo_programa[geralC->Programa1];
if(geralC->Programa1 == progNR3)     string00 += " V" + geralC->versao_NR3;
if(geralC->FlagConv[0])              string01  = "           OK";
else                                 string01  = "NÃO CONVERGIU";
if     (geralC->CargaNula)           string02  = "Sem carga";
else if(geralC->CargaEmYnodal)       string02  = "Todas em Ynodal";
else if(geralC->CargaInjecoesExt)    string02  = "Injeções externas";
else                                 string02  = "";
if(geralC->flag_capac_LT)            string03  = "Sim";
else                                 string03  = "Não";
if(geralC->usa_pi_equiv)             string04  = "Sim";
else                                 string04  = "Não";
if(geralC->flag_ypar_trafos)         string05  = "Sim";
else                                 string05  = "Não";
if(geralC->flag_ater_comuns)         string06  = "Sim";
else                                 string06  = "Não";
if(geralC->todos_sup_barra_interna)  string07  = "Sim";
else                                 string07  = "Não";
if(geralC->flag_TRAFO5_Como_Trafo3E) string08  = "Sim";
else                                 string08  = "Não";
if(geralC->flag_utiliza_bancoDF)     string09  = "Sim";
else                                 string09  = "Não";
if(geralC->num_open_result > 1) fprintf(arq, "\n");
fprintf(arq, "\n Programa:                                               %15s"
             "\n Plataforma:                                                       %5s"
             "\n Configuração:                                                   %7s"
             "\n Nome da rede:                       %35s"
             "\n Patamar:                                                           %4d",
        string00.c_str(), geralC->Plataforma.c_str(), geralC->Configuracao.c_str(),
        geralC->NomeRede.c_str(), (np+1));
if(geralC->Programa1 == progH3)
   {
   fprintf(arq, "\n Frequência:                                               "
                  "  FUNDAMENTAL");
   }
fprintf(arq, "\n Convergência:                                             %13s",
        string01.c_str());

if((geralC->Programa1 == progCURTO_MA) ||
   (geralC->Programa1 == progG3))
   {
   geralC->MaxIter(geralC->Programa1, &num_max1, &num_max2);
   string10    = MontaString(geralC->NumIteracao[0], num_max1,
                             geralC->MaxIteracao[0], num_max2);
   string11    = MontaString(geralC->NumIteracao[2], num_max1,
                             geralC->MaxIteracao[2], num_max2);
   num_brancos = 26 - string10.Length();
   fprintf(arq, "\n Número de iterações:                         ");
   ImprimeBrancos(num_brancos, arq);
   fprintf(arq, "%s", string10.c_str());
   fprintf(arq, "\n           montagens/fatorações de Ynodal:    ");
   ImprimeBrancos(num_brancos, arq);
   fprintf(arq, "%s", string11.c_str());
   }
else if(geralC->Programa1 == progESTIM3)
   {
   geralC->MaxIter(geralC->Programa1, &num_max1, &num_max2);
   string10    = MontaString(geralC->NumIteracao[0], num_max1,
                             geralC->MaxIteracao[0], num_max2);
   string11    = MontaString(geralC->NumIteracao[1], num_max1,
                             geralC->MaxIteracao[1], num_max2);
   num_brancos = 26 - string10.Length();
   fprintf(arq, "\n Número de iterações:                         ");
   ImprimeBrancos(num_brancos, arq);
   fprintf(arq, "%s", string10.c_str());
   fprintf(arq, "\n           montagens/fatorações de [H]:       ");
   ImprimeBrancos(num_brancos, arq);
   fprintf(arq, "%s", string11.c_str());
   }
else if(geralC->Programa1 == progH3)
   {
   geralC->MaxIter(geralC->Programa1, &num_max1, &num_max2);
   string10    = MontaString(geralC->NumIteracao[0], num_max1,
                             geralC->MaxIteracao[0], num_max2);
   num_brancos = 26 - string10.Length();
   fprintf(arq, "\n Número de iterações:                      ");
   ImprimeBrancos(num_brancos, arq);
   fprintf(arq, "%s", string10.c_str());
   }
else if(geralC->Programa1 == progNR3)
   {
   geralC->MaxIter(geralC->Programa1, &num_max1, &num_max2);
   string10    = MontaString(geralC->NumIteracao[0], num_max1,
                             geralC->MaxIteracao[0], num_max2);
   string11    = MontaString(geralC->NumIteracao[1], num_max1,
                             geralC->MaxIteracao[1], num_max2);
   string12    = MontaString(geralC->NumIteracao[2], num_max1,
                             geralC->MaxIteracao[2], num_max2);
   num_brancos = 26 - string10.Length();
   fprintf(arq, "\n Número de iterações - NR:                    ");
   ImprimeBrancos(num_brancos, arq);
   fprintf(arq, "%s", string10.c_str());
   fprintf(arq, "\n                     - ajuste de Tap/Qmax:    ");
   ImprimeBrancos(num_brancos, arq);
   fprintf(arq, "%s", string11.c_str());
   fprintf(arq, "\n           montagens/fatorações do Jacobiano: ");
   ImprimeBrancos(num_brancos, arq);
   fprintf(arq, "%s", string12.c_str());
   }

fprintf(arq, "\n Tolerância (pu):                                                 %6.0e"
             "\n Representação da carga:                       %25s"
             "\n Capacitância de linhas:                                             %3s"
             "\n Modelo pi-equivalente para linhas:                                  %3s"
             "\n Ramo paralelo de trafos:                                            %3s",
        geralC->Precisao[0], string02.c_str(), string03.c_str(), string04.c_str(),
        string05.c_str());
if(geralC->Programa1 == progNR3)
   {
   if(geralC->usa_vdnrm) string02 = "Sim";
   else                  string02 = "Não";
   fprintf(arq, "\n Utiliza VDNRM:                                                      %3s",
           string02.c_str());
   }
if(geralC->modelo_eqv != modeloEQV_INDEF)
   {
   fprintf(arq, "\n Modelo da rede equivalente:                                           %1d",
           geralC->modelo_eqv);
   }
fprintf(arq, "\n Aterramentos comuns:                                                %3s"
             "\n Barra interna de suprimentos:                                       %3s"
             "\n TRAFO5 como Trafo3E:                                                %3s"
             "\n Utilização de bancos Delta Fechado:                                 %3s",
        string06.c_str(), string07.c_str(), string08.c_str(), string09.c_str());

// Imprime barras sem convergência, quando houver
if(lisBAR_SEM_CONV != NULL)
   {
   if(lisBAR_SEM_CONV->Count > 0)
      {
      nelem = min(50, lisBAR_SEM_CONV->Count);
      fprintf(arq, "\n\n *** %d primeiras barras sem convergência"
                       " (total %d barras) ***\n", nelem, lisBAR_SEM_CONV->Count);
      for(int nb=0; nb < nelem; nb++)
         {
         bar1 = (TBarra *) lisBAR_SEM_CONV->Items[nb];
         fprintf(arq, "\n %-15s", bar1->codigo.c_str());
         }
      }
   }

// Imprime valores do cálculo de curto-circuito
if(curto_aux3 != NULL)
   {
   fprintf(arq, "\n\n\n *** Cálculo de curto-circuito ***");
   for(int nb=0; nb < curto_aux3->lisBAR_DEF->Count; nb++)
      {
      bar_def     = (TBarraDefeito *) curto_aux3->lisBAR_DEF->Items[nb];
      bar1        = bar_def->barra;
      fases_curto = bar_def->fases_curto;
      fprintf(arq, "\n\n     Barra:                                       %-15s"
                     "\n     Tipo / fases do defeito:                     %s / %s"
                     "\n     Impedância de defeito - FASE          (ohm): (%12.6e %12.6e)"
                     "\n     Impedância de defeito - TERRA         (ohm): (%12.6e %12.6e)"
                     "\n     Impedância de entrada - seq. ZERO     (ohm): (%12.6e %12.6e)"
                     "\n     Impedância de entrada - seq. DIRETA   (ohm): (%12.6e %12.6e)"
                     "\n     Potência de curto-circuito TRIFÁSICO  (MVA): (%12.6e %12.6e)"
                     "\n     Potência de curto-circuito FASE-TERRA (MVA): (%12.6e %12.6e)"
                     "\n     Correntes de defeito (A/°):               ",
              bar1->codigo.c_str(),
              codigo_tipo_curto[bar_def->tipo_curto].c_str(),
              codigo_fases_curto[bar_def->fases_curto].c_str(),
              curto_aux3->zdef_fase_ohm.real(),  curto_aux3->zdef_fase_ohm.imag(),
              curto_aux3->zdef_terra_ohm.real(), curto_aux3->zdef_terra_ohm.imag(),
              curto_aux3->zent0_ohm[nb].real(),  curto_aux3->zent0_ohm[nb].imag(),
              curto_aux3->zent1_ohm[nb].real(),  curto_aux3->zent1_ohm[nb].imag(),
              curto_aux3->s3f_mva[nb].real(),    curto_aux3->s3f_mva[nb].imag(),
              curto_aux3->sft_mva[nb].real(),    curto_aux3->sft_mva[nb].imag());
      for(int n=0; n < (MAX_FASES_G3+1); n++)
         {
         if(n > 0) fprintf(arq, "\n                                               ");
         if(n == 0)      fase = faseA;
         else if(n == 1) fase = faseB;
         else if(n == 2) fase = faseC;
         else if(n == 3) fase = faseN;
         else            fase = faseT;
         corrente = geralC->FiltraZero(curto_aux3->idef[nb][n]);
         fprintf(arq, "   %c  %12.4f %9.4f",
                 bar1->fases->FaseAsChar(fase),
                 Abs(corrente), ArgGrau(corrente));
         }
      flag_3F = IsComplexZero(curto_aux3->zdef_fase_ohm) &&
                ((fases_curto == ctABC)  || (fases_curto == ctABCN) ||
                 (fases_curto == ctABCT));
      flag_FT = IsComplexZero(curto_aux3->zdef_terra_ohm) &&
                ((fases_curto == ctAN)   || (fases_curto == ctAT));
      if(flag_3F || flag_FT)
         {
         fprintf(arq, "\n     Correntes assimétricas (A):                  ");
         for(int n=0; n < (MAX_FASES_G3-1); n++)
            {
            fprintf(arq, "   %12.4f", curto_aux3->idef_assim[nb][n]);
            }
         }
      } // for(nb)
   } // if(curto_aux3)
fclose(arq);

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TImpressao3::ImprimeResultados2(void)
{
// Local variables
int             fase_int, ind1, nfim, num_fases, num_fases_semNT, num_vezes,
					 offset;
double          tap1, vmod;
AnsiString      vet_cod_linha[MAX_FASES_G3],
                cod01, cod02, cod03, cod04, cod05, cod06, cod07,
                cod08, cod09, cod10;
complex<double> corrente, sfluxo, sperda, v1, v2,
                vet_tensao_aux_pu[2*MAX_FASES_G3];
AnsiString      charstr1, vet_fases;
FILE            *arq;
TList           *lisLIG;
TBancoDF        *bancoDF;
TBarra          *barra1;
TCapSerie       *caps1;
TFase           *fases, *fases_lig;
TLigacaoC       *ligacao;
TSup            *sup1;
TTrecho         *trec1;
TTrafo12        *trafo12;
TTrafo32        *trafo32;
TTrafo33        *trafo33;
TTrafoEquiv     *trafoequiv;
TTrafoZZ        *trafozz;

// Abre arquivo
arq = geralC->AbreArquivo(arqRESULT);
if(arq == NULL) return(false);

// Ordena barras por código, se isso não foi feito antes
OrdenaBarrasCodigo();

// Imprime resultados nas barras combinados com resultados nas ligações
// (modo padrão de apresentação de resultados de fluxo de potência)
fprintf(arq, "\n\n\n *** Resultados nas barras e ligações ***"
             "\n\n De                 Tipo     At.   Tensões nom. (kV)"
             "   F          ----Corrente (A/°)----    ---Vfase (pu/°)---    "
             "---Vlinha (pu/°)--   DeseqV(%%)"
             "\n                                     Fase    Linha"
             "\n\n      Para             Código       M/L  At  "
             "----Tap---- Pass  F  ----Corrente (A/°)----   ----Potência "
             "(kW/kVAr)---  -------Perda (kW/kVAr)-------");
for(int nb=0; nb < lisBAR_CODIGO->Count; nb++)
   {
   barra1          = (TBarra *) lisBAR_CODIGO->Items[nb];
   cod01           = barra1->codigo;
   fases           = barra1->fases;
   num_fases       = fases->NumFases();
   num_fases_semNT = fases->NumFasesSemNT();
   barra1->CalculaTensoesPU(vet_tensao_aux_pu, vet_cod_linha);
   fprintf(arq, "\n");
   for(int nfa=0; nfa < num_fases; nfa++)
      {
		fase_int = fases->Fase(nfa);
      if(nfa == 0)
         {
         if(fases->ExisteN()) nfim = barra1->tipo_ater;
         else                 nfim = NUM_TIPO_ATERR;  // A barra não tem neutro
         fprintf(arq, "\n %-15s    %-5s     %c  %8.3f %8.3f    ",
                 cod01.c_str(),
                 geralC->codigo_tipo_barra[barra1->tipo].c_str(),
                 codigo_ater[nfim],
					  barra1->VnomFase_kv(fase_int),
                 barra1->VnomLinha_kv());
         }
      else if(nfa != fases->IndiceN())
         {
         fprintf(arq, "\n                                 %8.3f   "
                      "          ",
					  barra1->VnomFase_kv(fase_int));
         }
      else
         {
         fprintf(arq, "\n                                         "
                      "             ");
         }
      v1       = geralC->FiltraZero(vet_tensao_aux_pu[nfa]);  // Tensão de fase
      corrente = geralC->FiltraZero(barra1->vet_corr_a[nfa]);
      vmod     = Abs(v1);
      fprintf(arq, " %c          %12.6f %9.4f   %9.6f %9.4f",
              fases->FaseAsCharIndice(nfa),
              Abs(corrente), ArgGrau(corrente),
              vmod, ArgGrau(v1));
      num_vezes = 0;
      if     (num_fases_semNT == 2) num_vezes = 1;
      else if(num_fases_semNT == 3) num_vezes = 3;
      if(nfa < num_vezes)  // Imprime Vlinha
         {
         v2   = geralC->FiltraZero(vet_tensao_aux_pu[num_fases + nfa]);
         vmod = Abs(v2);
         fprintf(arq, "   %9.6f %9.4f",
                 vmod, ArgGrau(v2));
         }
      if(nfa == 0)
         {
         charstr1.sprintf("        ");
         if(barra1->deseq_v_pu >= 0.) charstr1.sprintf("%8.4f", barra1->deseq_v_pu);
         fprintf(arq, "   %8s", charstr1.c_str());
         }
      } // for(nfa)
   if((fases->IndiceN() >= 0) && (barra1->tipo_ater != atIsolado))
      {// A barra tem neutro não-isolado - imprime corrente fluindo pelo aterramento
      corrente = geralC->FiltraZero(barra1->vet_corr_a[fases->NumFases()]);
      fprintf(arq, "\n                                         "
                   "              %c          %12.6f %9.4f",
              fases->FaseAsChar(faseT),
              Abs(corrente), ArgGrau(corrente));
      }

   // Loop de ligações da barra
   lisLIG = barra1->lisLIG;
   for(int nl=0; nl < lisLIG->Count; nl++)
      {
      ligacao = (TLigacaoC *) lisLIG->Items[nl];
      cod01   = ligacao->cod_bar2;
      cod02   = ligacao->cod_lig;
      cod03   = "  ";
      cod04   = "   ";
      switch(ligacao->tipo)
         {
         case BancoDF:
            bancoDF = (TBancoDF *) ligacao->plig;
            if(ligacao->sentido == 0)
					{
					fases_lig = bancoDF->fases_AT;
					offset    = 0;
					}
            else
					{
					fases_lig = bancoDF->fases_BT;
					offset    = bancoDF->fases_AT->NumFases();
					}
            break;

         case CapSerie3:
				caps1     = (TCapSerie *) ligacao->plig;
				fases_lig = caps1->fases;
				if(ligacao->sentido == 0) offset = 0;
            else                      offset = fases_lig->NumFases();
				break;

         case Reg12:
         case Trafo12:
            trafo12 = (TTrafo12 *) ligacao->plig;
            if(ligacao->sentido == 0)
					{
					fases_lig = trafo12->fases_pri;
					offset    = 0;
               cod04     = AnsiString(codigo_ater[trafo12->tipo_ater1]) +
                           AnsiString(codigo_ater[trafo12->tipo_ater2]);
					}
            else
					{
					fases_lig = trafo12->fases_sec;
					offset    = trafo12->fases_pri->NumFases();
               cod04     = AnsiString(codigo_ater[trafo12->tipo_ater2]) +
                           AnsiString(codigo_ater[trafo12->tipo_ater1]);
					}
            break;

         case Reg32:
         case Trafo32:
            trafo32 = (TTrafo32 *) ligacao->plig;
            if(ligacao->sentido == 0)
					{
					fases_lig = trafo32->fases_pri;
					offset    = 0;
               cod03     = AnsiString(codigo_lig_enrol[trafo32->tipo_lig1]) +
                           AnsiString(codigo_lig_enrol[trafo32->tipo_lig2]);
               cod04     = AnsiString(codigo_ater[trafo32->tipo_ater1]) +
                           AnsiString(codigo_ater[trafo32->tipo_ater2]);
					}
            else
					{
					fases_lig = trafo32->fases_sec;
					offset    = trafo32->fases_pri->NumFases();
               cod03     = AnsiString(codigo_lig_enrol[trafo32->tipo_lig2]) +
                           AnsiString(codigo_lig_enrol[trafo32->tipo_lig1]);
               cod04     = AnsiString(codigo_ater[trafo32->tipo_ater2]) +
                           AnsiString(codigo_ater[trafo32->tipo_ater1]);
               }
            break;

         case Trafo33:
            trafo33 = (TTrafo33 *) ligacao->plig;
            if(ligacao->sentido == 0)
					{
					fases_lig = trafo33->fases_pri;
               cod03     = AnsiString(codigo_lig_enrol[trafo33->tipo_lig1]);
               cod04     = AnsiString(codigo_ater[trafo33->tipo_ater1]);
					offset    = 0;
               tap1      = trafo33->vnom1_kv / trafo33->vtap1_kv;
					}
            else if(ligacao->sentido == 1)
					{
					fases_lig = trafo33->fases_sec;
               cod03     = AnsiString(codigo_lig_enrol[trafo33->tipo_lig2]);
               cod04     = AnsiString(codigo_ater[trafo33->tipo_ater2]);
					offset    = trafo33->fases_pri->NumFases();
               tap1      = trafo33->vnom2_kv / trafo33->vtap2_kv;
					}
            else
					{
					fases_lig = trafo33->fases_ter;
               cod03     = AnsiString(codigo_lig_enrol[trafo33->tipo_lig3]);
               cod04     = AnsiString(codigo_ater[trafo33->tipo_ater3]);
					offset    = trafo33->fases_pri->NumFases() + trafo33->fases_sec->NumFases();
               tap1      = trafo33->vnom3_kv / trafo33->vtap3_kv;
               }
            break;

         case TrafoEquiv3:
				trafoequiv = (TTrafoEquiv *) ligacao->plig;
				if(ligacao->sentido == 0)
               {
               fases_lig = trafoequiv->fases_pri;
               offset    = 0;
               }
            else
               {
               fases_lig = trafoequiv->fases_sec;
               offset    = trafoequiv->fases_pri->NumFases();
               }
				break;

         case TrafoZZ3:
            trafozz   = (TTrafoZZ *) ligacao->plig;
				fases_lig = trafozz->fases;
            break;

         case Trecho3:
				trec1     = (TTrecho *) ligacao->plig;
				fases_lig = trec1->fases;
            if(trec1->grupo_mutua >= 0)
               {
               cod03 = "M ";
               cod04.sprintf("%-3d", trec1->grupo_mutua);
               }
				if(ligacao->sentido == 0) offset = 0;
            else                      offset = fases_lig->NumFases();
				break;

         default:
            break;  // Erro - nada a fazer
         } // switch

      fprintf(arq, "\n\n      %-15s  %-11s  %2s  %3s",
              cod01.c_str(), cod02.c_str(), cod03.c_str(), cod04.c_str());

		// Loop de fases da ligação atual
      num_fases = fases_lig->NumFases();
		vet_fases = fases_lig->FasesAsString();
      for(int nfa=0; nfa < num_fases; nfa++)
         {
         cod05    = "           ";
			cod06    = "    ";
			cod07    = "            ";
			cod08    = "            ";
			cod09    = "              ";
			cod10    = "              ";
			fase_int = fases_lig->Fase(nfa);
         switch(ligacao->tipo)
            {
            case BancoDF:
               corrente = bancoDF->vet_corr_a[offset + nfa];
               sfluxo   = bancoDF->vet_s_kva[offset + nfa];
               sperda   = bancoDF->vet_perda_kva[nfa];
               cod07.sprintf("%12.3f", sfluxo.real());
               cod08.sprintf("%12.3f", sfluxo.imag());
               cod09.sprintf("%14.6f", sperda.real());
               cod10.sprintf("%14.6f", sperda.imag());
               break;

            case CapSerie3:
               corrente = caps1->vet_corr_a[offset + nfa];
               sfluxo   = caps1->vet_s_kva[offset + nfa];
               sperda   = caps1->vet_perda_kva[nfa];
               cod07.sprintf("%12.3f", sfluxo.real());
               cod08.sprintf("%12.3f", sfluxo.imag());
               cod09.sprintf("%14.6f", sperda.real());
               cod10.sprintf("%14.6f", sperda.imag());
               break;

            case Reg12:
            case Trafo12:
               corrente = trafo12->vet_corr_a[offset + nfa];
               sfluxo   = trafo12->vet_s_kva[offset + nfa];
               cod07.sprintf("%12.3f", sfluxo.real());
               cod08.sprintf("%12.3f", sfluxo.imag());
               if(nfa == 0)
                  {
						sperda = trafo12->perda_kva;
                  cod09.sprintf("%14.6f", sperda.real());
                  cod10.sprintf("%14.6f", sperda.imag());
                  }
               // AUTOTRAFO4 e AUTOTRAFO6: imprime os taps nas fases corretas
               // TRAFO4 e TRAFO5: imprime o tap na primeira fase (nfa == 0),
               //                  porque as fases do primário e do secundário
               //                  não são necessariamente as mesmas
               switch(trafo12->tipo)
                  {
                  case AUTOTRAFO4:
                     if(fase_int == trafo12->vet_fases_ord[2]) // O tap está nesta fase
                        {
                        tap1 = trafo12->vet_tap[0] * trafo12->vnom1_kv / trafo12->vnom2_kv;
                        if(ligacao->sentido == 0) cod05.sprintf(" 1:%8.6f", tap1);
                        else                      cod05.sprintf("-1:%8.6f", tap1);
                        if(trafo12->flag_reg12)
                           {
                           TReg12 *reg12 = (TReg12 *) trafo12;
                           if((geralC->Programa1 == progNR3) && reg12->IsPADRAO())
                              {
                              cod06 = "-   ";
                              }
                           else
                              {
                              cod06.sprintf("%3d ", reg12->vet_passo[0]);
                              }
                           }
                        }
							break;
                  case AUTOTRAFO6_1T:
							ind1 = -1;
                     if     (fase_int == trafo12->vet_fases_ord[3]) ind1 = 0;
                     else if(fase_int == trafo12->vet_fases_ord[4]) ind1 = 0;
                     if(ind1 >= 0)  // O tap está nesta fase
                        {
                        tap1 = trafo12->vet_tap[ind1] * trafo12->vnom1_kv / trafo12->vnom2_kv;
                        if(ligacao->sentido == 0) cod05.sprintf(" 1:%8.6f", tap1);
                        else                      cod05.sprintf("-1:%8.6f", tap1);
                        if(trafo12->flag_reg12)
                           {
                           TReg12 *reg12 = (TReg12 *) trafo12;
                           if((geralC->Programa1 == progNR3) && reg12->IsPADRAO())
                              {
                              cod06 = "-   ";
                              }
                           else
                              {
                              cod06.sprintf("%3d ", reg12->vet_passo[ind1]);
                              }
                           }
                        }
							break;
                  case AUTOTRAFO6_2T:
                     ind1 = -1;
                     if     (fase_int == trafo12->vet_fases_ord[3]) ind1 = 0;
                     else if(fase_int == trafo12->vet_fases_ord[4]) ind1 = 1;
                     if(ind1 >= 0)  // Um dos 2 taps está nesta fase
                        {
                        tap1 = trafo12->vet_tap[ind1] * trafo12->vnom1_kv / trafo12->vnom2_kv;
                        if(ligacao->sentido == 0) cod05.sprintf(" 1:%8.6f", tap1);
                        else                      cod05.sprintf("-1:%8.6f", tap1);
                        if(trafo12->flag_reg12)
                           {
                           TReg12 *reg12 = (TReg12 *) trafo12;
                           if((geralC->Programa1 == progNR3) && reg12->IsPADRAO())
                              {
                              cod06 = "-   ";
                              }
                           else
                              {
                              cod06.sprintf("%3d ", reg12->vet_passo[ind1]);
                              }
                           }
                        }
                     break;
                  default:  // TRAFO4 ou TRAFO5
                     if(nfa == 0)  // Impressão de tap na primeira linha (nfa == 0)
                        {
                        tap1 = trafo12->vet_tap[0] * trafo12->vnom1_kv / trafo12->vnom2_kv;
                        if(ligacao->sentido == 0) cod05.sprintf(" 1:%8.6f", tap1);
                        else                      cod05.sprintf("-1:%8.6f", tap1);
                        if(trafo12->flag_reg12)
                           {
                           TReg12 *reg12 = (TReg12 *) trafo12;
                           if((geralC->Programa1 == progNR3) && reg12->IsPADRAO())
                              {
                              cod06 = "-   ";
                              }
                           else
                              {
                              cod06.sprintf("%3d ", reg12->vet_passo[0]);
                              }
                           }
                        }
                     break;
						}
               break;

            case Reg32:
            case Trafo32:
               corrente = trafo32->vet_corr_a[offset + nfa];
               sfluxo   = trafo32->vet_s_kva[offset + nfa];
               cod07.sprintf("%12.3f", sfluxo.real());
               cod08.sprintf("%12.3f", sfluxo.imag());
               if(nfa < trafo32->NumTaps())
                  {
                  if(nfa == 0)
                     {
                     cod09.sprintf("%14.6f", trafo32->perda_kva.real());
                     cod10.sprintf("%14.6f", trafo32->perda_kva.imag());
                     }
                  tap1 = trafo32->vet_tap[nfa] * trafo32->vnom1_kv / trafo32->vnom2_kv;
                  if(ligacao->sentido == 0) cod05.sprintf(" 1:%8.6f", tap1);
                  else                      cod05.sprintf("-1:%8.6f", tap1);
                  if(trafo32->flag_reg32)
                     {
                     TReg32 *reg32 = (TReg32 *) trafo32;
                     if((geralC->Programa1 == progNR3) && reg32->IsPADRAO())
                        {
                        cod06 = "-   ";
                        }
                     else
                        {
                        cod06.sprintf("%3d ", reg32->vet_passo[nfa]);
                        }
                     }
                  }
               break;

            case Trafo33:
               corrente = trafo33->vet_corr_a[offset + nfa];
               sfluxo   = trafo33->vet_s_kva[offset + nfa];
               cod07.sprintf("%12.3f", sfluxo.real());
               cod08.sprintf("%12.3f", sfluxo.imag());
               if(nfa == 0)
                  {
                  cod05.sprintf(" 1:%8.6f", tap1);
                  sperda = trafo33->perda_kva;
                  cod09.sprintf("%14.6f", sperda.real());
                  cod10.sprintf("%14.6f", sperda.imag());
                  }
               break;

            case TrafoEquiv3:
               corrente = trafoequiv->vet_corr_a[offset + nfa];
               sfluxo   = trafoequiv->vet_s_kva[offset + nfa];
               cod07.sprintf("%12.3f", sfluxo.real());
               cod08.sprintf("%12.3f", sfluxo.imag());
               if(nfa == 0)
                  {
                  cod09.sprintf("%14.6f", trafoequiv->perda_kva.real());
                  cod10.sprintf("%14.6f", trafoequiv->perda_kva.imag());
                  }
               break;

            case TrafoZZ3:
               corrente = trafozz->vet_corr_a[nfa];
               sperda   = trafozz->vet_perda_kva[nfa];
               cod09.sprintf("%14.6f", sperda.real());
               cod10.sprintf("%14.6f", sperda.imag());
               break;

            case Trecho3:
               corrente = trec1->vet_corr_a[offset + nfa];
               sfluxo   = trec1->vet_s_kva[offset + nfa];
               sperda   = trec1->vet_perda_kva[nfa];
               cod07.sprintf("%12.3f", sfluxo.real());
               cod08.sprintf("%12.3f", sfluxo.imag());
               cod09.sprintf("%14.6f", sperda.real());
               cod10.sprintf("%14.6f", sperda.imag());
               break;

            default:
               break;  // Erro - nada a fazer
            } // switch

         corrente = geralC->FiltraZero(corrente);
         if(nfa > 0) fprintf(arq, "\n                                  "
                                         "         ");
         fprintf(arq, "  %11s %4s  %c  "
                      "%12.6f %9.4f   %12s %12s  %14s %14s",
                 cod05.c_str(), cod06.c_str(),
                 vet_fases[nfa+1], Abs(corrente), ArgGrau(corrente),
                 cod07.c_str(), cod08.c_str(), cod09.c_str(), cod10.c_str());
         } // for(nfa)
      } // for(nl)
   } // for(nb)
fclose(arq);

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TImpressao3::ImprimeResultados3(int np)
{
// Local variables
char            fase_vf_max, fase_vf_min;
bool            flag_deseq, flag_vlinha;
int             fase, num_fases, num_fases_semNT, num_vezes;
double          deseq_min, deseq_max, vf_max, vf_min, vl_max, vl_min,
                vmod, vn_max, vn_min;
AnsiString      bar_deseq_max, bar_deseq_min, bar_vf_max, bar_vf_min,
                bar_vl_max, bar_vl_min, bar_vn_max, bar_vn_min,
                cod01, fases_vl_max, fases_vl_min, vet_cod_linha[MAX_FASES_G3];
complex<double> corrente, v1, v2, vet_tensao_aux_pu[2*MAX_FASES_G3];
FILE            *arq;
TBarra          *barra1;
TFase           *fases;

// Abre arquivo
arq = geralC->AbreArquivo(arqRESULT);
if(arq == NULL) return(false);

// Loop de barras, determinando valores mínimo e máximo de
// tensão (fase e linha) e desequilíbrio
flag_vlinha = false;
flag_deseq  = false;
vl_min      = vf_min = vn_min = deseq_min =  1.e10;
vl_max      = vf_max = vn_max = deseq_max = -1.e10;
for(int nb=0; nb < rede3->lisBAR->Count; nb++)
   {
   barra1          = (TBarra *) rede3->lisBAR->Items[nb];
   cod01           = barra1->codigo;
   fases           = barra1->fases;
   num_fases       = fases->NumFases();
   num_fases_semNT = fases->NumFasesSemNT();
   barra1->CalculaTensoesPU(vet_tensao_aux_pu, vet_cod_linha);
   for(int nfa=0; nfa < num_fases; nfa++)
      {
      fase     = fases->Fase(nfa);
      v1       = geralC->FiltraZero(vet_tensao_aux_pu[nfa]);  // Tensão de fase
      corrente = geralC->FiltraZero(barra1->vet_corr_a[nfa]);
      vmod     = Abs(v1);
      if(fase == faseN)  // Neutro
         {
         if(vmod < vn_min)
            {
            bar_vn_min = cod01;
            vn_min     = vmod;
            }
         if(vmod > vn_max)
            {
            bar_vn_max  = cod01;
            vn_max      = vmod;
            }
         }
      else  // Fases A, B ou C
         {
         if(vmod < vf_min)
            {
            bar_vf_min  = cod01;
            fase_vf_min = fases->FaseAsChar(fase);
            vf_min      = vmod;
            }
         if(vmod > vf_max)
            {
            bar_vf_max  = cod01;
            fase_vf_max = fases->FaseAsChar(fase);
            vf_max      = vmod;
            }
         }
      num_vezes = 0;
      if     (num_fases_semNT == 2) num_vezes = 1;
      else if(num_fases_semNT == 3) num_vezes = 3;
      if(nfa < num_vezes)  // Imprime Vlinha
         {
         flag_vlinha = true;
         v2          = geralC->FiltraZero(vet_tensao_aux_pu[num_fases + nfa]);
         vmod        = Abs(v2);
         if(vmod < vl_min)
            {
            bar_vl_min   = cod01;
            fases_vl_min = vet_cod_linha[nfa];
            vl_min       = vmod;
            }
         if(vmod > vl_max)
            {
            bar_vl_max   = cod01;
            fases_vl_max = vet_cod_linha[nfa];
            vl_max       = vmod;
            }
         }
      if(nfa == 0)
         {
         if(barra1->deseq_v_pu >= 0.)
            {
            flag_deseq = true;
            if(barra1->deseq_v_pu < deseq_min)
               {
               bar_deseq_min = cod01;
               deseq_min     = barra1->deseq_v_pu;
               }
            if(barra1->deseq_v_pu > deseq_max)
               {
               bar_deseq_max = cod01;
               deseq_max     = barra1->deseq_v_pu;
               }
            }
         }
      } // for(nfa)
   } // for(nb)

// Imprime tensão mínima, tensão máxima e desequilíbrio
fprintf(arq, "\n\n     Tensão de fase   mínima:          %9.6f pu - barra: %-15s "
                 "- fase: %c"
               "\n                      máxima:          %9.6f pu - barra: %-15s "
                 "- fase: %c"
             "\n\n     Tensão de neutro mínima:          %9.6f pu - barra: %-15s"
               "\n                      máxima:          %9.6f pu - barra: %-15s",
        vf_min, bar_vf_min.c_str(), fase_vf_min,
        vf_max, bar_vf_max.c_str(), fase_vf_max,
        vn_min, bar_vn_min.c_str(),
        vn_max, bar_vn_max.c_str());
if(flag_vlinha)
   {
   fprintf(arq, "\n\n     Tensão de linha  mínima:          %9.6f pu - barra: %-15s "
                    "- fases: %-4s"
                  "\n                      máxima:          %9.6f pu - barra: %-15s "
                    "- fases: %-4s",
           vl_min, bar_vl_min.c_str(), fases_vl_min.c_str(),
           vl_max, bar_vl_max.c_str(), fases_vl_max.c_str());
   }
if(flag_deseq)
   {
   fprintf(arq, "\n\n     Desequilíbrio    mínimo:       %12.4f %%  - barra: %-15s "
                  "\n                      máximo:       %12.4f %%  - barra: %-15s ",
           deseq_min, bar_deseq_min.c_str(),
           deseq_max, bar_deseq_max.c_str());
   }

// Imprime potências totais e perda total calculada pelas ligações
fprintf(arq, "\n\n     Potência total de geração:  %15.6f kW   %15.6f kVAr"
                 "                  Perda total admit. ref.:  "
                 "%14.6f %14.6f"
               "\n     Potência total de carga:    %15.6f kW   %15.6f kVAr"
                 "                                            "
                 "-------------- --------------",
        geralC->geracao_total.real(),     geralC->geracao_total.imag(),
        geralC->perda_admit_reduc.real(), geralC->perda_admit_reduc.imag(),
        geralC->carga_total.real(),       geralC->carga_total.imag());
if(geralC->CargaEmYnodal)
   {
   fprintf(arq, "\n                                     "
                  "                                     "
                  "                  Perda total ligações:     "
                  "%14.6f %14.6f",
           geralC->perda_total_2.real(), geralC->perda_total_2.imag());
   }
else
   {
   fprintf(arq, "\n     Perda total (G-C):          %15.6f kW   %15.6f kVAr"
                  "                  Perda total ligações:     "
                  "%14.6f %14.6f",
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

// Imprime admitâncias equivalentes das CLs (FluxoH3)
if(geralC->Programa1 == progH3)
   {
   fprintf(arq, "\n\n\n *** Admitâncias das Cargas Lineares ***"
                  "\n\n Barra                   "
                      " Admitância da carga equivalente de Z constante (S)"
                      " (AN BN CN AB BC CA)\n");
   for(int nb=0; nb < geralC->num_barras_carga; nb++)
      {
      barra1 = (TBarra *) rede3->lisBAR->Items[nb];
      fprintf(arq, "\n %-15s   ", barra1->codigo.c_str());
      for(int na=0; na < NUM_ADMITANCIAS_H3; na++)
         {
         if(na == 3) fprintf(arq, "\n                   ");
         v1 = barra1->admit_carga_CL_s[na];
         fprintf(arq, "   %13.6e %13.6e", v1.real(), v1.imag());
         }
      }
   }
fclose(arq);

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TImpressao3::ImprimeResultadosESTIM3(double *vet_desv_iter,
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
        (rede3->lisMED->Count - geralC->num_med_reais),
        rede3->lisMED->Count, geralC->num_var_est,
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
bool __fastcall TImpressao3::ImprimeResultadosH3_Globais(int             np,
                                                         TList           *lisBAR_H3,
                                                         complex<double> *vet_perda_total_2)
{
// Local variables
char            fase_max_DHTv;
int             fase_int, ind, num_fases;
double          corrente, dht, max_DHTv, v1;
AnsiString      vet_fases, cod01, cod02, cod03, cod04, cod06, cod07;
FILE            *arq;
TBarra          *barra1, *barra_max_DHTv;
TBarraH3        *barH3;
TCapSerie       *caps1;
TFase           *fases;
TLigacaoC       *ligacao;
TLigacaoCH3     *ligH3;
TSup            *sup1;
TTrecho         *trec1;
TTrafo12        *trafo12;
TTrafo32        *trafo32;
TTrafo33        *trafo33;
TTrafoEquiv     *trafoequiv;
TTrafoZZ        *trafozz;

// Verifica se o relatório está habilitado
if(geralC->tipo_relatorio == trSEM_RELATORIO) return(false); // Não imprimiu nada

// Abre arquivo
arq = geralC->AbreArquivo(arqRESULT);
if(arq == NULL) return(false);

// Ordena barras por código, se isso não foi feito antes
OrdenaBarrasCodigo();

// Impressão de resultados
fprintf(arq, "\n Nome da rede:           %35s"
             "\n Patamar:                                               %4d"
             "\n                                          RESULTADOS GLOBAIS",
        geralC->NomeRede.c_str(), (np+1));

// Imprime resultados nas barras combinados com resultados nas ligações
// (modo padrão de apresentação de resultados de fluxo de potência)
fprintf(arq, "\n\n\n *** Resultados nas barras e ligações ***"
             "\n\n De                 Tipo     At.   Tensões nom. (kV)"
             "   F             ------- C O R R E N T E -------     "
             "-------- T E N S Ã O -------"
             "\n                                     Fase    Linha            "
             "           Módulo (A)         DHT (%%)      "
             "Módulo (pu)         DHT (%%)"
             "\n\n      Para             Código       M/L  At  "
             "----Tap---- Pass  F     ------- C O R R E N T E -------"
             "\n                                                              "
             "           Módulo (A)         DHT (%%)");
max_DHTv = -1.;
for(int nb=0; nb < lisBAR_CODIGO->Count; nb++)
   {
   barra1    = (TBarra *) lisBAR_CODIGO->Items[nb];
   ind       = rede3->lisBAR->IndexOf(barra1);
   barH3     = (TBarraH3 *) lisBAR_H3->Items[ind]; // Barras em lisBAR e lisBAR_H3 na mesma ordem...
   cod01     = barra1->codigo;
   fases     = barra1->fases;
   num_fases = fases->NumFases();
   fprintf(arq, "\n");
   for(int nfa=0; nfa < num_fases; nfa++)
      {
      fase_int = fases->Fase(nfa);
      dht      = barH3->dht_tensao[nfa];
      if((dht != DH_INFINITA) && (dht > max_DHTv))
         {
         barra_max_DHTv = barra1;
         max_DHTv       = dht;
         fase_max_DHTv  = fases->FaseAsChar(fase_int);
         }
      if(nfa == 0)
         {
         if(fases->ExisteN()) ind = barra1->tipo_ater;
         else                 ind = NUM_TIPO_ATERR;  // A barra não tem neutro
         fprintf(arq, "\n %-15s    %-5s     %c  %8.3f %8.3f    ",
                 cod01.c_str(),
                 geralC->codigo_tipo_barra[barra1->tipo].c_str(),
                 codigo_ater[ind],
                 barra1->VnomFase_kv(fase_int),
                 barra1->VnomLinha_kv());
         }
      else if(nfa != fases->IndiceN())
         {
         fprintf(arq, "\n                                 %8.3f   "
                      "          ",
                 barra1->VnomFase_kv(fase_int));
         }
      else
         {
         fprintf(arq, "\n                                         "
                      "             ");
         }
      v1       = barH3->vef_tensao_pu[nfa];
      corrente = barH3->vef_corrente_A[nfa];
      fprintf(arq, " %c             %14.6f     %12.4f      %10.8f     %12.4f",
              fases->FaseAsCharIndice(nfa), corrente,
              barH3->dht_corrente[nfa], v1, barH3->dht_tensao[nfa]);
      } // for(nfa)
   if((barra1->tipo_ater == atSolido) || (barra1->tipo_ater == atZ))
      {
      corrente = barH3->vef_corrente_A[num_fases];
      fprintf(arq, "\n                                                       %c"
                   "             %14.6f",
                   fases->FaseAsChar(faseT), corrente);
      }

   // Loop de ligações da barra
   for(int nl=0; nl < barH3->lisLIG->Count; nl++)
      {
      ligH3     = (TLigacaoCH3 *) barH3->lisLIG->Items[nl];
      ligacao   = ligH3->ligacao;
      cod01     = ligacao->cod_bar2;
      cod02     = ligacao->cod_lig;
      num_fases = ligH3->num_fases;
      switch(ligacao->tipo)
         {
         case CapSerie3:
            caps1     = (TCapSerie *) ligacao->plig;
            cod03     = "  ";
            cod04     = "   ";
            vet_fases = caps1->fases->FasesAsString();
            break;

         case Reg12:
         case Trafo12:
            trafo12 = (TTrafo12 *) ligacao->plig;
            cod03   = "  ";
            if(ligacao->sentido == 0)
               {
               cod04     = AnsiString(codigo_ater[trafo12->tipo_ater1]) +
                           AnsiString(codigo_ater[trafo12->tipo_ater2]);
               vet_fases = trafo12->fases_pri->FasesAsString();
               }
            else
               {
               cod04     = AnsiString(codigo_ater[trafo12->tipo_ater2]) +
                           AnsiString(codigo_ater[trafo12->tipo_ater1]);
               vet_fases = trafo12->fases_sec->FasesAsString();
               }
            break;

         case Reg32:
         case Trafo32:
            trafo32 = (TTrafo32 *) ligacao->plig;
            if(ligacao->sentido == 0)
               {
               cod03     = AnsiString(codigo_lig_enrol[trafo32->tipo_lig1]) +
                           AnsiString(codigo_lig_enrol[trafo32->tipo_lig2]);
               cod04     = AnsiString(codigo_ater[trafo32->tipo_ater1]) +
                           AnsiString(codigo_ater[trafo32->tipo_ater2]);
               vet_fases = trafo32->fases_pri->FasesAsString();
               }
            else
               {
               cod03     = AnsiString(codigo_lig_enrol[trafo32->tipo_lig2]) +
                           AnsiString(codigo_lig_enrol[trafo32->tipo_lig1]);
               cod04     = AnsiString(codigo_ater[trafo32->tipo_ater2]) +
                           AnsiString(codigo_ater[trafo32->tipo_ater1]);
               vet_fases = trafo32->fases_sec->FasesAsString();
               }
            break;

         case Trafo33:
            trafo33 = (TTrafo33 *) ligacao->plig;
            if(ligacao->sentido == 0)
               {
               cod03     = AnsiString(codigo_lig_enrol[trafo33->tipo_lig1]);
               cod04     = AnsiString(codigo_ater[trafo33->tipo_ater1]);
               vet_fases = trafo33->fases_pri->FasesAsString();
               }
            else if(ligacao->sentido == 1)
               {
               cod03     = AnsiString(codigo_lig_enrol[trafo33->tipo_lig2]);
               cod04     = AnsiString(codigo_ater[trafo33->tipo_ater2]);
               vet_fases = trafo33->fases_sec->FasesAsString();
               }
            else
               {
               cod03     = AnsiString(codigo_lig_enrol[trafo33->tipo_lig3]);
               cod04     = AnsiString(codigo_ater[trafo33->tipo_ater3]);
               vet_fases = trafo33->fases_ter->FasesAsString();
               }
            break;

         case TrafoEquiv3:
            trafoequiv = (TTrafoEquiv *) ligacao->plig;
            cod03      = "  ";
            cod04      = "   ";
            if(ligacao->sentido == 0) vet_fases = trafoequiv->fases_pri->FasesAsString();
            else                      vet_fases = trafoequiv->fases_sec->FasesAsString();
            break;

         case TrafoZZ3:
            trafozz   = (TTrafoZZ *) ligacao->plig;
            cod03     = "  ";
            cod04     = "   ";
            vet_fases = trafozz->fases->FasesAsString();
            break;

         case Trecho3:
            trec1 = (TTrecho *) ligacao->plig;
            if(trec1->grupo_mutua >= 0)
               {
               cod03 = "M ";
               cod04 = AnsiString(trec1->grupo_mutua);
               }
            else
               {
               cod03 = "  ";
               cod04 = "   ";
               }
            vet_fases = trec1->fases->FasesAsString();
            break;

         default:
            break;  // Erro - nada a fazer
         } // switch

      fprintf(arq, "\n\n      %-15s  %-11s  %2s  %3s",
              cod01.c_str(), cod02.c_str(), cod03.c_str(),  cod04.c_str());

      // Loop de fases da ligação atual
      for(int nfa=0; nfa < num_fases; nfa++)
         {
         corrente = ligH3->vef_corrente_A[nfa];
         dht      = ligH3->dht_corrente[nfa];
         cod06.sprintf("%14.6f", corrente);
         cod07.sprintf("%12.4f", dht);
         if(nfa > 0) fprintf(arq, "\n                                  "
                                   "         ");
         fprintf(arq, "                    %c     %14s     %12s",
                 vet_fases[nfa+1], cod06.c_str(), cod07.c_str());
         } // for(nfa)
      } // for(nl)
   } // for(nb)

// Imprime máxima DHTv
fprintf(arq, "\n\n *** Máxima DHTv: %6.4f %% - Barra: %s - Fase: %c ***",
        max_DHTv, barra_max_DHTv->codigo.c_str(), fase_max_DHTv);
fclose(arq);

// Finalização
ImprimeSeparador(arqRESULT, 1, 147);

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TImpressao3::ImprimeResultadosH3_Harmonica(int             np,
                                                           int             indfreq,
                                                           TList           *lisBAR_H3,
                                                           complex<double> *vet_perda_total_2)
{
// Local variables
char            fase_max_DHv;
int             fase_int, ind, num_fases, ordem;
double          dh, max_DHv;
complex<double> corrente, sperda, v1;
AnsiString      vet_fases, cod01, cod02, cod03, cod04, cod06, cod07,
                cod08, cod09, cod10;
FILE            *arq;
TList           *lisLIG;
TBarra          *barra1, *barra_max_DHv;
TBarraH3        *barH3;
TCapSerie       *caps1;
TFase           *fases;
TLigacaoC       *ligacao;
TLigacaoCH3     *ligH3;
TSup            *sup1;
TTrecho         *trec1;
TTrafo12        *trafo12;
TTrafo32        *trafo32;
TTrafo33        *trafo33;
TTrafoEquiv     *trafoequiv;
TTrafoZZ        *trafozz;

// Verifica se o relatório está habilitado
if(geralC->tipo_relatorio == trSEM_RELATORIO) return(false); // Não imprimiu nada

// Obtém ordem harmônica
ordem = geralC->OrdemHarmonica(indfreq);
if(ordem < 0) return(false);

// Abre arquivo
arq = geralC->AbreArquivo(arqRESULT);
if(arq == NULL) return(false);

// Ordena barras por código, se isso não foi feito antes
OrdenaBarrasCodigo();

// Impressão de resultados
fprintf(arq, "\n Nome da rede:           %35s"
             "\n Patamar:                                               %4d"
             "\n Ordem da harmônica:                                    %4d",
        geralC->NomeRede.c_str(), (np+1), ordem);

// Imprime resultados nas barras combinados com resultados nas ligações
// (modo padrão de apresentação de resultados de fluxo de potência)
fprintf(arq, "\n\n\n *** Resultados nas barras e ligações ***"
             "\n\n De                 Tipo     At.   Tensões nom. (kV)"
             "   F            ----------- C O R R E N T E -----------     "
             "------------ T E N S Ã O ------------"
             "\n                                     Fase    Linha            "
             "          Módulo (A)    Âng. (°)      DH (%%)       Módulo (pu)   "
             " Âng. (°)      DH (%%)"
             "\n\n      Para             Código       M/L  At  "
             "----Tap---- Pass  F    ----------- C O R R E N T E -----------   "
             "------ P E R D A (kVA) ------"
             "\n                                                              "
             "          Módulo (A)    Âng. (°)      DH (%%)         Ativa      "
             "   Reativa");
max_DHv = -1.;
for(int nb=0; nb < lisBAR_CODIGO->Count; nb++)
   {
   barra1    = (TBarra *) lisBAR_CODIGO->Items[nb];
   ind       = rede3->lisBAR->IndexOf(barra1);
   barH3     = (TBarraH3 *) lisBAR_H3->Items[ind]; // Barras em lisBAR e lisBAR_H3 na mesma ordem...
   cod01     = barra1->codigo;
   fases     = barra1->fases;
   num_fases = fases->NumFases();
   fprintf(arq, "\n");
   for(int nfa=0; nfa < num_fases; nfa++)
      {
      fase_int = fases->Fase(nfa);
      dh       = barH3->dh_tensao[indfreq][nfa];
      if((dh != DH_INFINITA) && (dh > max_DHv))
         {
         barra_max_DHv = barra1;
         max_DHv       = dh;
         fase_max_DHv  = fases->FaseAsChar(fase_int);
         }
      if(nfa == 0)
         {
         if(fases->ExisteN()) ind = barra1->tipo_ater;
         else                 ind = NUM_TIPO_ATERR;  // A barra não tem neutro
         fprintf(arq, "\n %-15s    %-5s     %c  %8.3f %8.3f    ",
                 cod01.c_str(),
                 geralC->codigo_tipo_barra[barra1->tipo].c_str(),
                 codigo_ater[ind],
                 barra1->VnomFase_kv(fase_int),
                 barra1->VnomLinha_kv());
         }
      else if(nfa != fases->IndiceN())
         {
         fprintf(arq, "\n                                 %8.3f   "
                      "          ",
                 barra1->VnomFase_kv(fase_int));
         }
      else
         {
         fprintf(arq, "\n                                         "
                      "             ");
         }
      v1       = barH3->tensao_pu[indfreq][nfa];
      corrente = barH3->corrente_A[indfreq][nfa];
      v1       = geralC->FiltraZero(v1);
      corrente = geralC->FiltraZero(corrente);
      fprintf(arq, " %c            %14.6f  %9.4f  %12.4f     %12.10f  %9.4f"
                   "  %12.4f",
              fases->FaseAsCharIndice(nfa), Abs(corrente),
              ArgGrau(corrente), barH3->dh_corrente[indfreq][nfa],
              Abs(v1), ArgGrau(v1),
              barH3->dh_tensao[indfreq][nfa]);
      } // for(nfa)
   if((barra1->tipo_ater == atSolido) || (barra1->tipo_ater == atZ))
      {
      corrente = geralC->FiltraZero(barH3->corrente_A[indfreq][num_fases]);
      fprintf(arq, "\n                                                       %c"
                   "            %14.6f  %9.4f",
                   fases->FaseAsChar(faseT),
                   Abs(corrente), ArgGrau(corrente));
      }

   // Loop de ligações da barra
   lisLIG = barH3->lisLIG;
   for(int nl=0; nl < lisLIG->Count; nl++)
      {
      ligH3     = (TLigacaoCH3 *) lisLIG->Items[nl];
      ligacao   = ligH3->ligacao;
      cod01     = ligacao->cod_bar2;
      cod02     = ligacao->cod_lig;
      num_fases = ligH3->num_fases;
      switch(ligacao->tipo)
         {
         case CapSerie3:
            caps1     = (TCapSerie *) ligacao->plig;
            cod03     = "  ";
            cod04     = "   ";
            vet_fases = caps1->fases->FasesAsString();
            break;

         case Reg12:
         case Trafo12:
            trafo12 = (TTrafo12 *) ligacao->plig;
            cod03   = "  ";
            if(ligacao->sentido == 0)
               {
               cod04     = AnsiString(codigo_ater[trafo12->tipo_ater1]) +
                           AnsiString(codigo_ater[trafo12->tipo_ater2]);
               vet_fases = trafo12->fases_pri->FasesAsString();
               }
            else
               {
               cod04     = AnsiString(codigo_ater[trafo12->tipo_ater2]) +
                           AnsiString(codigo_ater[trafo12->tipo_ater1]);
               vet_fases = trafo12->fases_sec->FasesAsString();
               }
            break;

         case Reg32:
         case Trafo32:
            trafo32 = (TTrafo32 *) ligacao->plig;
            if(ligacao->sentido == 0)
               {
               cod03     = AnsiString(codigo_lig_enrol[trafo32->tipo_lig1]) +
                           AnsiString(codigo_lig_enrol[trafo32->tipo_lig2]);
               cod04     = AnsiString(codigo_ater[trafo32->tipo_ater1]) +
                           AnsiString(codigo_ater[trafo32->tipo_ater2]);
               vet_fases = trafo32->fases_pri->FasesAsString();
               }
            else
               {
               cod03     = AnsiString(codigo_lig_enrol[trafo32->tipo_lig2]) +
                           AnsiString(codigo_lig_enrol[trafo32->tipo_lig1]);
               cod04     = AnsiString(codigo_ater[trafo32->tipo_ater2]) +
                           AnsiString(codigo_ater[trafo32->tipo_ater1]);
               vet_fases = trafo32->fases_sec->FasesAsString();
               }
            break;

         case Trafo33:
            trafo33 = (TTrafo33 *) ligacao->plig;
            if(ligacao->sentido == 0)
               {
               cod03     = AnsiString(codigo_lig_enrol[trafo33->tipo_lig1]);
               cod04     = AnsiString(codigo_ater[trafo33->tipo_ater1]);
               vet_fases = trafo33->fases_pri->FasesAsString();
               }
            else if(ligacao->sentido == 1)
               {
               cod03     = AnsiString(codigo_lig_enrol[trafo33->tipo_lig2]);
               cod04     = AnsiString(codigo_ater[trafo33->tipo_ater2]);
               vet_fases = trafo33->fases_sec->FasesAsString();
               }
            else
               {
               cod03     = AnsiString(codigo_lig_enrol[trafo33->tipo_lig3]);
               cod04     = AnsiString(codigo_ater[trafo33->tipo_ater3]);
               vet_fases = trafo33->fases_ter->FasesAsString();
               }
            break;

         case TrafoEquiv3:
            trafoequiv = (TTrafoEquiv *) ligacao->plig;
            cod03      = "  ";
            cod04      = "   ";
            if(ligacao->sentido == 0) vet_fases = trafoequiv->fases_pri->FasesAsString();
            else                      vet_fases = trafoequiv->fases_sec->FasesAsString();
            break;

         case TrafoZZ3:
            trafozz   = (TTrafoZZ *) ligacao->plig;
            cod03     = "  ";
            cod04     = "   ";
            vet_fases = trafozz->fases->FasesAsString();
            break;

         case Trecho3:
            trec1 = (TTrecho *) ligacao->plig;
            if(trec1->grupo_mutua >= 0)
               {
               cod03 = "M ";
               cod04 = AnsiString(trec1->grupo_mutua);
               }
            else
               {
               cod03 = "  ";
               cod04 = "   ";
               }
            vet_fases = trec1->fases->FasesAsString();
            break;

         default:
            break;  // Erro - nada a fazer
         } // switch

      fprintf(arq, "\n\n      %-15s  %-11s  %2s  %3s",
              cod01.c_str(), cod02.c_str(), cod03.c_str(),  cod04.c_str());

      // Loop de fases da ligação atual
      for(int nfa=0; nfa < num_fases; nfa++)
         {
         corrente = geralC->FiltraZero(ligH3->corrente_A->C2[indfreq][nfa]);
         dh       = ligH3->dh_corrente->D2[indfreq][nfa];
         cod06.sprintf("%14.6f", Abs(corrente));
         cod07.sprintf("%9.4f", ArgGrau(corrente));
         cod08.sprintf("%12.4f", dh);
         switch(ligacao->tipo)
            {
            case CapSerie3:
            case Reg32:
            case Trafo32:
            case TrafoEquiv3:
            case Trecho3:
            case TrafoZZ3:
               sperda = ligH3->perda_kva->C2[indfreq][nfa];  // Perda por fase
               cod09.sprintf("%14.6f", sperda.real());
               cod10.sprintf("%14.6f", sperda.imag());
               break;

            case Reg12:
            case Trafo12:
            case Trafo33:
               if(nfa == 0)
                  {
                  sperda = ligH3->perda_kva->C2[indfreq][0]; // Perda total do trafo
                  cod09.sprintf("%14.6f", sperda.real());
                  cod10.sprintf("%14.6f", sperda.imag());
                  }
               else
                  {
                  cod09 = "              ";
                  cod10 = "              ";
                  }
               break;

            default:  // Erro
               return(false);
            }
         if(nfa > 0) fprintf(arq, "\n                                  "
                                   "         ");
         fprintf(arq, "                    %c    %14s  %9s  %12s   %14s %14s",
                 vet_fases[nfa+1], cod06.c_str(), cod07.c_str(), cod08.c_str(),
                 cod09.c_str(), cod10.c_str());
         } // for(nfa)
      } // for(nl)
   } // for(nb)

// Imprime perda total calculada pelas ligações
fprintf(arq, "\n                                                       "
             "                                                       "
             "-------------- --------------"
             "\n                                          "
             "                                            "
             "  Perda total ligações: %14.6f %14.6f",
        vet_perda_total_2[indfreq].real(),
        vet_perda_total_2[indfreq].imag());

// Imprime máxima DHv
fprintf(arq, "\n\n *** Máxima DHv: %6.4f %% - Barra: %s - Fase: %c ***",
        max_DHv, barra_max_DHv->codigo.c_str(), fase_max_DHv);
fclose(arq);

// Finalização
ImprimeSeparador(arqRESULT, 1, 147);

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TImpressao3::ImprimeResultadosMedidores(FILE *arq)
{
// Variáveis locais
char       char0, char1, char2, char2_prev, char3;
int        fase_int, id1, id1_prev, med_desv_max, nm_max_abs_pneg, num_casas,
           num_med_pneg;
double     desv_max, desv1, max_abs_pneg;
AnsiString str1, str1_prev, str2, str2_prev, str3, str3_prev;
TBarra     *bar1, *bar_ref;
TMedidor   *med1;
TTrecho    *trec1;
TTrafo32   *trafo32;

// Temporárias
// int    med_desv_max2;
// double desv2, desv_max2 = -1.,
//        somapm = 0., somape = 0., somaqm = 0., somaqe = 0.;
// int    med_pmax, med_qmax;
// double pmax = -1., qmax = -1.;

// Localiza medidor com maior desvio
desv_max = -1.;
for(int nm=0; nm < rede3->lisMED->Count; nm++)
   {
   med1  = (TMedidor *) rede3->lisMED->Items[nm];
   desv1 = fabs(med1->desvio_ndp);
   if(desv1 > desv_max)
	  {
      med_desv_max = nm;
      desv_max     = desv1;
      }

/*
if(nm >= geralC->num_med_reais) continue;
if(fabs(med1->val_med_pu) > 0.)
   {
   // desv2 = fabs((med1->val_med_pu - med1->val_est_pu) / med1->val_med_pu);
   desv2 = fabs(med1->val_est_pu / med1->val_med_pu);
   if(desv2 > desv_max2)
      {
      med_desv_max2 = nm;
      desv_max2     = desv2;
      }
   }
*/
/*
   // if(med1->tipo_medidor != medREAL) continue;
   if(med1->tipo_eqpto   != teBARRA) continue;
   if(med1->tipo_medicao == medP)
      {
	  somapm += med1->val_med_pu;
      somape += med1->val_est_pu;
      if(fabs(med1->val_med_pu) > pmax)
         {
         pmax     = fabs(med1->val_med_pu);
         med_pmax = nm;
         }
      }
   else if(med1->tipo_medicao == medQ)
      {
      somaqm += med1->val_med_pu;
      somaqe += med1->val_est_pu;
      if(fabs(med1->val_med_pu) > qmax)
         {
         qmax     = fabs(med1->val_med_pu);
         med_qmax = nm;
         }
      }
*/

   } // for(nm)

/*
// DEBUG ESTIM3
int    numrp, numrq, numvp, numvq;
double somarp, somarq, somavp, somavq;
numrp  = numrq  = numvp  = numvq  = 0;
somarp = somarq = somavp = somavq = 0.;
*/

// Imprime resultados nos medidores
fprintf(arq, "\n\n\n *** Resultados nos medidores (total: %d) ***"
               "\n\n    Núm. R/V H/D  Código           Tipo eqpto  "
                   "--------------Equipamento--------------  TM  Fase   "
                   "Valor medido   Valor estim.   Desv. padrão  Desv.(núm.DP)"
                 "\n                                               Barra1"
                   "          Barra2              ID\n",
        rede3->lisMED->Count);
char2_prev = '*';
str1_prev  = "*";
str2_prev  = "*";
str3_prev  = "*";
id1_prev   = -10000000;
for(int nm=0; nm < rede3->lisMED->Count; nm++)
   {
   med1     = (TMedidor *) rede3->lisMED->Items[nm];
   id1      = med1->id_calc;
   bar_ref  = med1->bar_ref;
   fase_int = med1->fase_int;
   if(med1->tipo_medidor == medREAL) char0 = 'R';
   else                              char0 = 'V';
   if(med1->enabled)                 char1 = ' ';
   else                              char1 = 'D';
   char2 = geralC->codigo_tipo_medicao[med1->tipo_medicao];
   char3 = bar_ref->fases->FaseAsChar(fase_int);
   switch(med1->tipo_eqpto)
      {
      case teBARRA:
         bar1 = (TBarra *) med1->eqpto;
         str1 = "Barra";
         str2 = bar1->codigo;
		 str3 = " ";

/*
// DEBUG ESTIM3
if(med1->tipo_medicao == medP)
   {
   if(med1->tipo_medidor == medREAL) {numrp++; somarp += med1->val_est_pu;}
   else                              {numvp++; somavp += med1->val_est_pu;}
   }
else if(med1->tipo_medicao == medQ)
   {
   if(med1->tipo_medidor == medREAL) {numrq++; somarq += med1->val_est_pu;}
   else                              {numvq++; somavq += med1->val_est_pu;}
   }
*/

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
         trafo32 = (TTrafo32 *) med1->eqpto;
         str1    = "Trafo32";
         if(trafo32->bar1 == med1->bar_ref)
            {
            str2 = trafo32->bar1->codigo;
            str3 = trafo32->bar2->codigo;
            }
         else
            {
            str2 = trafo32->bar2->codigo;
            str3 = trafo32->bar1->codigo;
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
   fprintf(arq, "    %c   %13.6e  %13.6e  %13.6e   %8.2f",
		   char3, med1->val_med_pu, med1->val_est_pu, med1->sigma2_pu,
           med1->desvio_ndp);
   if(nm == med_desv_max) fprintf(arq, " MAIOR");
   char2_prev = char2;
   str1_prev  = str1;
   str2_prev  = str2;
   str3_prev  = str3;
   id1_prev   = id1;
   } // for(nm)
fprintf(arq, "\n");

// fprintf(arq, "\n\n desv_max2: %13.6e - med_desv_max2: %d",
//         desv_max2, (med_desv_max2+1));
// fprintf(arq, "\n\n somapm = %13.6e   somape = %13.6e"
//                "\n somaqm = %13.6e   somaqe = %13.6e",
//         somapm, somape, somaqm, somaqe);
// fprintf(arq, "\n\n pmax = %13.6e   med_pmax = %d"
//                "\n qmax = %13.6e   med_qmax = %d",
//         pmax, (med_pmax+1), qmax, (med_qmax+1));

/*
// DEBUG ESTIM3
somarp *= geralC->Sbase_fase;
somarq *= geralC->Sbase_fase;
somavp *= geralC->Sbase_fase;
somavq *= geralC->Sbase_fase;
fprintf(arq, "\n\n\n *** TImpressao3::ImprimeResultadosMedidores() ***"
			   "\n\n Npr = %6d   Pr = %10.6f   Nqr = %6d   Qr = %10.6f   (MVA)"
				 "\n Npv = %6d   Pv = %10.6f   Nqv = %6d   Qv = %10.6f   (MVA)\n\n",
        numrp, somarp, numrq, somarq, numvp, somavp, numvq, somavq);
*/

// Imprime medidores reais de barra com P_est < 0, se houver
num_med_pneg = 0;
for(int nm=0; nm < geralC->num_med_reais; nm++)
   {
   med1 = (TMedidor *) rede3->lisMED->Items[nm];
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
                   "--------------Equipamento--------------  TM  Fase   "
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
   med1 = (TMedidor *) rede3->lisMED->Items[nm];
   if(med1->tipo_eqpto   != teBARRA) continue;  // Medidor irrelevante
   if(med1->tipo_medicao != medP)    continue;  // Medidor irrelevante
   if(med1->val_est_pu >= 0.)        continue;  // Medidor irrelevante
   id1      = med1->id_calc;
   bar_ref  = med1->bar_ref;
   fase_int = med1->fase_int;
   if(med1->tipo_medidor == medREAL) char0 = 'R';
   else                              char0 = 'V';
   if(med1->enabled)                 char1 = ' ';
   else                              char1 = 'D';
   char2  = geralC->codigo_tipo_medicao[med1->tipo_medicao];
   char3  = bar_ref->fases->FaseAsChar(fase_int);
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
   fprintf(arq, "    %c   %13.6e  %13.6e  %13.6e   %8.2f",
           char3, med1->val_med_pu, med1->val_est_pu, med1->sigma2_pu,
           med1->desvio_ndp);
   char2_prev = char2;
   str1_prev  = str1;
   str2_prev  = str2;
   str3_prev  = str3;
   id1_prev   = id1;
   } // for(nm)
fprintf(arq, "\n\n     Medidor com maior potência ativa negativa: %-6d"
                 "   P = %13.6e pu = %13.6e kW",
             (nm_max_abs_pneg+1), (-max_abs_pneg),
             (-max_abs_pneg * 1000. * geralC->Sbase_fase));
if(num_med_pneg == 1) fprintf(arq, "\n     Potência negativa total (1 medidor):  ");
else                  fprintf(arq, "\n     Potência negativa total (%d medidores):",num_med_pneg);
num_casas = 5 - int(log10(double(num_med_pneg) + 0.0001));
for(int n=0; n < num_casas; n++) fprintf(arq, " ");
fprintf(arq, "         P = %13.6e pu = %13.6e kW\n",
        desv1, (desv1 * 1000. * geralC->Sbase_fase));

return(true);
}
/*
//---------------------------------------------------------------------------
bool __fastcall TImpressao3::ImprimeResultadosMedidoresExcel(void)
{
// Variáveis locais
char       char0, char1, char2, char3;
int        fase_int, id1;
AnsiString nome_arq, str1, str2, str3;
FILE       *arq;
TBarra     *bar1, *bar_ref;
TMedidor   *med1;
TTrecho    *trec1;
TTrafo32   *trafo32;

// Abre arquivo
arq = geralC->AbreArquivo(arqLOG);
if(arq == NULL) return(false);

// Loop de medidores
for(int nm=0; nm < rede3->lisMED->Count; nm++)
   {
   med1     = (TMedidor *) rede3->lisMED->Items[nm];
   id1      = med1->id_calc;
   bar_ref  = med1->bar_ref;
   fase_int = med1->fase_int;
   if(med1->tipo_medidor == medREAL) char0 = 'R';
   else                              char0 = 'V';
   if(med1->enabled)                 char1 = ' ';
   else                              char1 = 'D';
   char2 = geralC->codigo_tipo_medicao[med1->tipo_medicao];
   char3 = bar_ref->fases->FaseAsChar(fase_int);
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
         trafo32 = (TTrafo32 *) med1->eqpto;
         str1    = "Trafo32";
         if(trafo32->bar1 == med1->bar_ref)
            {
            str2 = trafo32->bar1->codigo;
            str3 = trafo32->bar2->codigo;
            }
         else
            {
            str2 = trafo32->bar2->codigo;
            str3 = trafo32->bar1->codigo;
            }
         break;

      default:  // erro
         return(false);
      }
   fprintf(arq, "\n   %6d   %c   %c   %-15s",
           (nm+1), char0, char1, med1->codigo.SubString(0, 15).c_str());
   fprintf(arq, "    %-7s   %-15s %-15s %5d   %c",
              str1.c_str(), str2.c_str(), str3.c_str(), id1, char2);
   fprintf(arq, "    %c   %13.6e  %13.6e   %8.2f",
           char3, med1->val_med_pu, med1->val_est_pu, med1->desvio_ndp);
   } // for(nm)

// Fecha arquivo
fclose(arq);

return(true);
}
*/

//---------------------------------------------------------------------------
bool __fastcall TImpressao3::ImprimeResultadosMedidoresExcel(void)
{
return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TImpressao3::ImprimeResultadosR3(int      np,
                                                 VTGeralC *geralC,
                                                 TList    *lisBAR,
                                                 TList    *lisLIGACAO)
{
// Local variables
int             iap, kprint, num_fases, num_fases_pri, num_fases_sec;
double          p1, q1, soma1, soma2, soma3, soma4;
complex<double> *pc1, v1, z1, vet_tensao_aux_pu[2*MAX_FASES_G3];
AnsiString      charstr1, charstr2, vet_cod_linha[MAX_FASES_G3];
FILE            *arq;
TBarra          *barra1;
TFase           *fases, *fases_pri, *fases_sec;
TLigacaoEquiv   *lig1;

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
           geralC->Plataforma.c_str(), geralC->Configuracao.c_str(),
           geralC->NomeRede.c_str(), geralC->Sbase);

   fprintf(arq, "\n\n Número de barras - de carga:                                         %6d",
           geralC->num_barras_carga);
   fprintf(arq,   "\n                  - de geração:                                       %6d",
           geralC->num_barras_geracao);
   fprintf(arq,   "\n                  - total:                                            %6d",
           lisBAR->Count);
   fprintf(arq, "\n\n Número de ligações:                                                  %6d",
           lisLIGACAO->Count);
   }

// Prints bus data
if(lisBAR->Count > 0)
   {
   fprintf(arq, "\n\n *** REDE REDUZIDA - Dados de barras - Patamar %d ***"
                "\n\n Código            Tipo      Tens. nom. (kV)     "
                "-Fases-    Tipo carga        ---Pot. absorvida final--"
                "\n                              Fase     Linha"
                "                                  ----P(MW)--- ---Q(MVAr)--",
           (np+1));
   soma1 = 0.;
   soma2 = 0.;
   soma3 = 0.;
   soma4 = 0.;
   for(int nb=0; nb < lisBAR->Count; nb++)
      {
      barra1    = (TBarra *) lisBAR->Items[nb];
      fases     = barra1->fases;
      num_fases = fases->NumFases();
      barra1->CalculaTensoesPU(vet_tensao_aux_pu, vet_cod_linha);
      kprint = 0;
      fprintf(arq, "\n\n %-15s   %-5s   %8.3f %8.3f",
              barra1->codigo.c_str(),
              geralC->codigo_tipo_barra[barra1->tipo].c_str(),
              barra1->VnomFase_kv(barra1->fases->Fase(0)),
              barra1->VnomLinha_kv());
      iap      = 0;
      charstr1 = "";
      for(int nfa=0; nfa < num_fases; nfa++)
         {
         charstr1 += fases->FaseAsCharIndice(nfa);
         iap++;
         if(iap >= 7) break;
         charstr1 += " ";
         iap++;
         }
      for(int nfa=iap; nfa < 7; nfa++)
         {
         charstr1 += " ";
         }
      fprintf(arq, "     %7s     ", charstr1.c_str());

      for(int lig=0; lig < NUM_LIG_CARGA; lig++)
         {
         if(barra1->CargaNula(lig)) continue; // Não tem carga do tipo 'lig'
         z1     = barra1->mat_carga_mva->C2[lig][Icte];
         p1     = z1.real();
         q1     = z1.imag();
         soma1 += p1;
         soma2 += q1;
         if(kprint != 0) fprintf(arq, "\n\n                             "
                                      "                                ");
         kprint = 1;
         fprintf(arq, "    %6s     I %12.6f %12.6f",
                 codigo_fases[lig].c_str(), p1, q1);
         z1     = barra1->mat_carga_mva->C2[lig][Scte];
         p1     = z1.real();
         q1     = z1.imag();
         soma1 += p1;
         soma2 += q1;
         fprintf(arq, "\n                                     "
                      "                                       "
                      "S %12.6f %12.6f", p1, q1);
         z1     = barra1->mat_carga_mva->C2[lig][Zcte];
         p1     = z1.real();
         q1     = z1.imag();
         soma1 += p1;
         soma2 += q1;
         fprintf(arq, "\n                                     "
                      "                                       "
                      "Z %12.6f %12.6f", p1, q1);
         } // for(lig)

      // Imprime DeltaCarga, tensões e matriz de admitâncias da redução
      fprintf(arq, "\n\n -Variação pot. absorvida-   "
                   "-----ModV/AngV----    "
                   "-------------------------------"
                   "Matriz de admitâncias nodais (redução de rede) (pu)"
                   "--------------------------------"
                   "\n ---DP(MW)--- --DQ(MVAr)--\n");
      for(int nfa=0; nfa < num_fases; nfa++)
         {
         v1     = geralC->FiltraZero(vet_tensao_aux_pu[nfa]);
         p1     = barra1->vet_scarga_mva[nfa].real();
         q1     = barra1->vet_scarga_mva[nfa].imag();
         soma3 += p1;
         soma4 += q1;
         fprintf(arq,"\n %12.6f %12.6f   %8.6f %9.4f  ",
                 p1, q1, Abs(v1), ArgGrau(v1));
         for(int nfa2=0; nfa2 < num_fases; nfa2++)
            {
            z1 = barra1->mat_ynodal_reduc_pu->C2[nfa][nfa2];
            fprintf(arq, "  %13.6e %13.6e", z1.real(), z1.imag());
            }
         }

      } // for(nb)
   fprintf(arq, "\n\n ------------ ------------            "
                "                       "
                "                 ------------ ------------"
                "\n %12.6f %12.6f  <- Total DP/DQ                     "
                "    Total P/Q: %12.6f %12.6f",
           soma3, soma4, soma1, soma2);
   }

// Prints branch data
if((geralC->num_open_result == 1) && (lisLIGACAO->Count > 0))
   {
   fprintf(arq, "\n\n\n *** REDE REDUZIDA - Dados de ligações equivalentes "
                "(submatrizes Ynodal(i,j) e Ynodal(j,i) (pu)) ***");
   fprintf(arq, "\n\n De              Para            Código            "
                "        --Fases--\n");
   for(int nt=0; nt < lisLIGACAO->Count; nt++)
      {
      lig1          = (TLigacaoEquiv *) lisLIGACAO->Items[nt];
      fases_pri     = lig1->fases_pri;
      fases_sec     = lig1->fases_sec;
      num_fases_pri = fases_pri->NumFases();
      num_fases_sec = fases_sec->NumFases();
      fprintf(arq, "\n %-15s %-15s %-15s           ",
              lig1->bar1->codigo.c_str(),
              lig1->bar2->codigo.c_str(),
              lig1->codigo.c_str());
      for(int nfa=0; nfa < num_fases_pri; nfa++)
         {
         fprintf(arq, "%c", fases_pri->FaseAsCharIndice(nfa));
         }
      fprintf(arq, " ");
      for(int nfa=0; nfa < num_fases_sec; nfa++)
         {
         fprintf(arq, "%c", fases_sec->FaseAsCharIndice(nfa));
         }
      fprintf(arq, "\n");

      // Imprime submatrizes nodais de admitâncias
      for(int i=0; i < num_fases_pri; i++)
         {
         fprintf(arq, "\n");
         for(int j=0; j < num_fases_sec; j++)
            {
            z1 = lig1->MatYnodal_ij_pu[i][j];
            fprintf(arq, "  %13.6e %13.6e", z1.real(), z1.imag());
            }
         }
      fprintf(arq, "\n");
      for(int i=0; i < num_fases_sec; i++)
         {
         fprintf(arq, "\n");
         for(int j=0; j < num_fases_pri; j++)
            {
            z1 = lig1->MatYnodal_ji_pu[i][j];
            fprintf(arq, "  %13.6e %13.6e", z1.real(), z1.imag());
            }
         }
      if(nt < (lisLIGACAO->Count-1)) fprintf(arq, "\n");
      }
   }
fclose(arq);

// Finalização
ImprimeSeparador(arqRESULT, 1, 116);

return(true);
}

//---------------------------------------------------------------------------
void __fastcall TImpressao3::MontaSuprimentosCodigo(void)
{
int        jinic, jm1;
AnsiString cod_i, cod_j;
TSup       *sup_i, *sup_j;

// Retorna se 'lisSUP_CODIGO' já foi montada anteriormente
if(lisSUP_CODIGO != NULL) return;

// Cria e monta 'lisSUP_CODIGO'
lisSUP_CODIGO = new TList;
for(int ns=0; ns < rede3->lisSUP->Count; ns++)
   {
   sup_i = (TSup *) rede3->lisSUP->Items[ns];
   lisSUP_CODIGO->Add(sup_i);
   }

// Ordena suprimentos em ordem crescente do código da barra INTERNA
for(int i=1; i < lisSUP_CODIGO->Count; i++)
   {
   sup_i = (TSup *) lisSUP_CODIGO->Items[i];
   cod_i = sup_i->bar_int->codigo;
   jinic = i - 1;
   jm1   = i;
   for(int j=jinic; j >= 0; j--)
      {
      sup_j = (TSup *) lisSUP_CODIGO->Items[j];
      cod_j = sup_j->bar_int->codigo;
      if(cod_i.AnsiCompareIC(cod_j) >= 0) break;
      lisSUP_CODIGO->Items[jm1] = sup_j;
      jm1                       = j;
      }
   lisSUP_CODIGO->Items[jm1] = sup_i;
   }
}

//---------------------------------------------------------------------------
void __fastcall TImpressao3::OrdenaBarrasCodigo(void)
{
// Local variables
TBarra *barra1;

// Retorna se a lista 'lisBAR_CODIGO' tiver o mesmo número de
// elementos da lista 'rede3->lisBAR' (já foi montada e não houve
// acréscimo de barras)
if(lisBAR_CODIGO->Count == rede3->lisBAR->Count) return;

// Inicializa 'lisBAR_CODIGO'
lisBAR_CODIGO->Clear();
for(int i=0; i < rede3->lisBAR->Count; i++)
   {
   barra1 = (TBarra *) rede3->lisBAR->Items[i];
   lisBAR_CODIGO->Add(barra1);
   }

// Ordena 'lisBAR_CODIGO'
lisBAR_CODIGO->Sort(EscolheBarraCodigo);
}

//---------------------------------------------------------------------------
void __fastcall TImpressao3::OrdenaTrafo12(void)
{
// Local variables
TTrafo12 *trafo12;

// Retorna se a lista 'lisTRAFO12_ORD' tiver o mesmo número de
// elementos da lista 'rede3->lisTRAFO12' (já foi montada e não
// houve alteração no número de trafos)
if(lisTRAFO12_ORD->Count == rede3->lisTRAFO12->Count) return;

// Inicializa 'lisTRAFO12_ORD'
lisTRAFO12_ORD->Clear();
for(int i=0; i < rede3->lisTRAFO12->Count; i++)
   {
   trafo12 = (TTrafo12 *) rede3->lisTRAFO12->Items[i];
   lisTRAFO12_ORD->Add(trafo12);
   }

// Ordena 'lisTRAFO12_ORD'
lisTRAFO12_ORD->Sort(EscolheTrafo12);
}

//---------------------------------------------------------------------------
void __fastcall TImpressao3::OrdenaTrafo32(void)
{
// Local variables
TTrafo32 *trafo32;

// Retorna se a lista 'lisTRAFO32_ORD' tiver o mesmo número de
// elementos da lista 'rede3->lisTRAFO32' (já foi montada e não
// houve alteração no número de trafos)
if(lisTRAFO32_ORD->Count == rede3->lisTRAFO32->Count) return;

// Inicializa 'lisTRAFO32_ORD'
lisTRAFO32_ORD->Clear();
for(int i=0; i < rede3->lisTRAFO32->Count; i++)
   {
   trafo32 = (TTrafo32 *) rede3->lisTRAFO32->Items[i];
   lisTRAFO32_ORD->Add(trafo32);
   }

// Ordena 'lisTRAFO32_ORD'
lisTRAFO32_ORD->Sort(EscolheTrafo32);
}

//---------------------------------------------------------------------------
void __fastcall TImpressao3::OrdenaTrechos(void)
{
// Local variables
TTrecho *trec1;

// Retorna se a lista 'lisTRECHO_ORD' tiver o mesmo número de
// elementos da lista 'rede3->lisTRECHO' (já foi montada e não
// houve alteração no número de trechos)
if(lisTRECHO_ORD->Count == rede3->lisTRECHO->Count) return;

// Inicializa 'lisTRECHO_ORD'
lisTRECHO_ORD->Clear();
for(int i=0; i < rede3->lisTRECHO->Count; i++)
   {
   trec1 = (TTrecho *) rede3->lisTRECHO->Items[i];
   lisTRECHO_ORD->Add(trec1);
   }

// Ordena 'lisTRECHO_ORD'
lisTRECHO_ORD->Sort(EscolheTrecho);
}

//---------------------------------------------------------------------------

