//---------------------------------------------------------------------------
#pragma hdrstop
#include ".\TImpressao.h"
#include ".\TTempoCPU.h"
#include ".\VTGeralC.h"
#include "..\..\Complexo\Complexo.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TImpressao::TImpressao(VTGeralC *geralC)
   {
   // Salva dados
   this->geralC = geralC;

   // Inicializa constantes
   radgrau       = 180. / M_PI;
   raiz3         = sqrt(3.);
   raiz3_inverso = 1. / raiz3;
   cum           = complex<double>(1., 0.);
   czero         = complex<double>(0., 0.);

   // Inicializa variáveis locais
   lisBAR_CODIGO = new TList;
   lisTRECHO_ORD = new TList;

   // Manter coerência com 'enum_BARRA_REF' em VTEstimador.h
   codigo_barra_ref[0] = "FIXOS_1THETA_0V";
   codigo_barra_ref[1] = "FIXOS_1THETA_1V";
   codigo_barra_ref[2] = "FIXOS_nTHETA_0V";
   codigo_barra_ref[3] = "FIXOS_nTHETA_nV";

   // Manter coerência com 'enum_METODO_EE' em VTEstimador.h
   codigo_metodo_EE[0] = "Convencional";
   codigo_metodo_EE[1] = "QR Fast Givens";
   codigo_metodo_EE[2] = "QR Givens";
   codigo_metodo_EE[3] = "QR Householder";

   // Manter coerência com 'enum_CRIT_ORD_LINHAS_COLUNAS' em VTSisLin.h
   codigo_proc_linhas_colunas[0] = "MIN_NLIG";
   codigo_proc_linhas_colunas[1] = "MIN_NLIG_MAX_DIAG";
   codigo_proc_linhas_colunas[2] = "MAX_DIAG";
   codigo_proc_linhas_colunas[3] = "SEM_ORDENACAO";

   // Manter coerência com 'enum_CRIT_ORD_LINHAS_QR' em VTSisLin.h
   codigo_proc_linhas_qr[0] = "SEM_ORDENACAO";
   codigo_proc_linhas_qr[1] = "MENOR_NUM_ELEM_FD";
   codigo_proc_linhas_qr[2] = "MENOR_ULTIMA_COLUNA";
   }

//---------------------------------------------------------------------------
__fastcall TImpressao::~TImpressao(void)
   {
   delete lisBAR_CODIGO; // Os elementos desta lista são os mesmos de 'rede1->lisBAR'
   delete lisTRECHO_ORD; // Os elementos desta lista são os mesmos de 'rede1->lisTRECHO'
   }

//---------------------------------------------------------------------------
void __fastcall TImpressao::ImprimeBrancos(int  num_brancos,
                                           FILE *arq)
{
if(arq == NULL) return;  // Erro
for(int n=0; n < num_brancos; n++)
   {
   fprintf(arq, " ");
   }
}
//---------------------------------------------------------------------------
bool __fastcall TImpressao::ImprimeResumoProc(int arq_alt)
{
// Local variables
bool                   flag_imprimiu, flag_tempo, flag_tempo_aux;
int                    num_cat, tipo_arquivo;
unsigned long long int num_chamadas;
double                 tempo_total_seg, tempo_medio_seg;
AnsiString             str1;
FILE                   *arq;

// Verifica se o relatório está habilitado
if(geralC->tipo_relatorio == trSEM_RELATORIO) return(false); // Não imprimiu nada

// Abre arquivo
if(arq_alt < 0) tipo_arquivo = arqRESULT; // Arquivo default
else            tipo_arquivo = arq_alt;   // Arquivo especificado na chamada
arq = geralC->AbreArquivo(tipo_arquivo);
if(arq == NULL) return(false);

// Imprime cabeçalho
fprintf(arq, "\n\n *** Resumo do processamento ***"
             "\n\n          Plataforma:     %-5s"
               "\n          Configuração:   %-7s"
             "\n\n          Contador / Função                    Núm. cham.     "
             "Tempo tot.(s)      Tempo méd.(s)",
             geralC->Plataforma.c_str(), geralC->Configuracao.c_str());

// Loop de duas passagens: primeiro só os contadores e
// depois contadores + tempos
num_cat = geralC->tCPU->NumCategorias();
for(int nv=0; nv < 2; nv++)
   {
   if(nv == 0) flag_tempo_aux = false;
   else        flag_tempo_aux = true;

   // Loop de medições
   flag_imprimiu = false;
   for(int nm=0; nm < num_cat; nm++)
      {
      str1 = geralC->tCPU->Valores(nm, &flag_tempo, &num_chamadas, &tempo_total_seg,
                                   &tempo_medio_seg);
      if(flag_tempo != flag_tempo_aux) continue;  // Tipo irrelevante
      if(num_chamadas <= 0)            continue;  // Medição sem registro
      if(! flag_imprimiu) {fprintf(arq, "\n"); flag_imprimiu = true;}
      fprintf(arq, "\n     %-35s     %12llu", str1.c_str(), num_chamadas);
      if(flag_tempo_aux)
         {
         fprintf(arq, "       %9.3f        %13.6e",
                 tempo_total_seg, tempo_medio_seg);
         }
      }
   }
fclose(arq);

// Finalização
ImprimeSeparador(tipo_arquivo, 1, 94);

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TImpressao::ImprimeSeparador(int tipo_arquivo,
                                             int num_linhas_branco,
                                             int num_caracteres)
{
// Local variables
FILE *arq;

// Abre arquivo
arq = geralC->AbreArquivo(tipo_arquivo);
if(arq == NULL) return(false);

// Imprime linhas em branco
for(int n=0; n < (num_linhas_branco+1); n++)
   {
   fprintf(arq, "\n");
   }

// Imprime caracteres de separação
for(int n=0; n < num_caracteres; n++)
   {
   fprintf(arq, "=");
   }
fclose(arq);

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TImpressao::ImprimeVetorComplex(AnsiString      nome,
                                                int             count,
                                                complex<double> *vetor)
   {
   complex<double> cval;
   FILE            *arq;

   arq = geralC->AbreArquivo(arqRESULT);
   if(arq == NULL) return(false);

   fprintf(arq, "\n\n     *** %s ***\n", nome.c_str());
   for(int n=0; n < count; n++)
      {
      cval = vetor[n];
      // fprintf(arq, "\n     n: %-3d   valor: (%13.6e %13.6e) (%13.6e %13.6e)",
      fprintf(arq, "\n     n: %-3d   valor: (%14.6f %14.6f)   (%14.6f %9.4f)",
              n, cval.real(), cval.imag(), Abs(cval), ArgGrau(cval));
      }
   fprintf(arq, "\n");
   fclose(arq);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TImpressao::ImprimeVetorDouble(AnsiString nome,
                                               int        count,
                                               double     *vetor)
   {
   FILE *arq;

   arq = geralC->AbreArquivo(arqRESULT);
   if(arq == NULL) return(false);

   fprintf(arq, "\n\n     *** %s ***\n", nome.c_str());
   for(int n=0; n < count; n++)
      {
      fprintf(arq, "\n     n: %-3d   valor: %13.6e",
              n, vetor[n]);
      }
   fprintf(arq, "\n");
   fclose(arq);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TImpressao::ImprimeVetorInt(AnsiString nome,
                                            int        count,
                                            int        *vetor)
   {
   FILE *arq;

   arq = geralC->AbreArquivo(arqRESULT);
   if(arq == NULL) return(false);

   fprintf(arq, "\n\n     *** %s ***\n", nome.c_str());
   for(int n=0; n < count; n++)
      {
      fprintf(arq, "\n     n: %-3d   valor: %4d",
              n, vetor[n]);
      }
   fprintf(arq, "\n");
   fclose(arq);

   return(true);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TImpressao::MontaString(int num1,
                                              int num_max1,
                                              int num2,
                                              int num_max2)
{
// Variáveis locais
int        num_brancos;
AnsiString string1;

// Monta string
string1 = "";
num_brancos = NumDigitos(num_max1) - NumDigitos(num1);
for(int n=0; n < num_brancos; n++)
   {
   string1 += " ";
   }
string1     += IntToStr(num1);
string1     += " (";
num_brancos  = NumDigitos(num_max2) - NumDigitos(num2);
for(int n=0; n < num_brancos; n++)
   {
   string1 += " ";
   }
string1 += IntToStr(num2);
string1 += ")";

// Retorna
return(string1);
}

//---------------------------------------------------------------------------
int __fastcall TImpressao::NumDigitos(int num1)
{
// Variáveis locais
int    num_digitos;
double logx;

// Verifica caso particular
if(num1 == 0) return(1);

// Determina número de dígitos do número fornecido
logx        = log10(double(num1) + 0.0001);
num_digitos = int(logx) + 1;

// Retorna
return(num_digitos);
}

//---------------------------------------------------------------------------
//eof

