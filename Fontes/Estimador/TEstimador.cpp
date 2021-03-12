//---------------------------------------------------------------------------
#pragma hdrstop
#include ".\TEstimador.h"
#include "..\Apl\VTApl.h"
#include "..\Calculo\Comum13\Constantes_Calculo.h"
#include "..\Calculo\Comum13\TMedidor.h"
#include "..\Calculo\Comum13\VTGeralC.h"
#include "..\Log\VTLog.h"
#include "..\Patamar\VTpatamar.h"
#include "..\Patamar\VTPatamares.h"
#include "..\SisLin\VTSisLin.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TEstimador::TEstimador(VTApl *apl)
   {
   // Salva ponteiro para 'apl'
   this->apl = apl;

   // Inicializa ponteiros
   data              = NULL;
   Rm1               = NULL;
   vet_desv          = NULL;
   vet_desv_iter     = NULL;
   vet_TC            = NULL;
   vet_tensoes       = NULL;
   vet_col_arq       = NULL;
   vet_col_ordenadas = NULL;
   A                 = NULL;
   H                 = NULL;
   HtRm1             = NULL;
   lisBAR_ESTADO     = NULL;
   lisBAR_REF        = NULL;
   lisYE             = NULL;
   geralC            = NULL; // Será criado em TEstimador1 ou TEstimador3 (classes derivadas)

   // Define constantes
   raiz3 = sqrt(3.);
   czero = complex<double>(0., 0.);
   cum   = complex<double>(1., 0.);

   // Inicializa variáveis
   flag_grava_matriz_vetor = false;
   flag_montar_Rm1         = true;
   grava_arq_ordem_col     = false;
   gravou_arq_ordem_col    = false;
   le_arq_ordem_col        = false;
   leu_arq_ordem_col       = false;
   num_col_arq             = 0;
   num_col_ordenadas       = 0;
   fat_mult_med_desab      = 100.;              // Em estudo
   desv_tot                = 0.;
   desv_tot_ant            = 0.;
   TipoBarraRef            = brFIXOS_1THETA_0V; // Estimação de estado convencional
   LimitaSigmas            = false;
   OrdenacaoLinhasQR       = qrlMENOR_NUM_ELEM_FD;
   UtilizaVDNM             = false;
   FatMultMedChave         = 1.;
   SigmaMinimoPu           = 1.e-5;
   SigmaMaximoPu           = 1.e-2;
   MetodoEE                = meeQR_GIVENS;
   NumMaxMontH             = 2;
   TipoRelatorio           = trSEM_RELATORIO;
   }

//---------------------------------------------------------------------------
__fastcall TEstimador::~TEstimador(void)
   {
   DeletaMatrizes();
   LiberaMemoria();
   delete geralC;  // Criado em Estimador1 ou Estimador3 (classes derivadas)
   }

//---------------------------------------------------------------------------
void __fastcall TEstimador::AtualizaMedidoresReais(TList *lisMED)
{
// Variáveis locais
bool     flag_PQS;
double   sigma_pu_maximo, sigma_pu_minimo;
TMedidor *med1;

// Inicialização
flag_PQS              = false;  // Não há medidores de P, Q ou S em princípio
sigma_pu_minimo       =  1.e30;
sigma_pu_maximo       = -1.e30;
geralC->num_med_desab = 0;

// Loop de medidores reais
for(int nm=0; nm < geralC->num_med_reais; nm++)
   {
   med1 = (TMedidor *) lisMED->Items[nm];

   // Atualiza o contador de medidores desabilitados
   if(! med1->enabled) geralC->num_med_desab++;

   // Exclui medidor com tolerância <= 0.
   // Tolerância negativa é definida pelo Estimador de Demanda no caso de
   // medidores de barras que terão sua demanda ajustada, os quais serão
   // tratados logo a seguir.
   // IMPORTANTE: a comparação deve ser "<=".
   if(med1->toler_pu <= 0.) continue;


// TEMPORÁRIO: considera que o valor de fundo de escala é a própria
//             medição. Esta atribuição deverá ser removida quando
//             TData passar o valor correto do fundo de escala
//             (atualmente passa o valor zero).
med1->val_fe_pu = fabs(med1->val_med_pu);


   // Calcula desvio padrão do medidor (pode depender do valor medido)
   med1->CalculaDesvioPadrao(geralC->semi_intervalo_normal);

   // Atualiza valores mínimo e máximo (só medidores de corrente e potência)
   if(med1->TipoMedicaoCorrente() || med1->TipoMedicaoPotencia())
      {
	   if(med1->sigma1_pu < sigma_pu_minimo) sigma_pu_minimo = med1->sigma1_pu;
      if(med1->sigma1_pu > sigma_pu_maximo) sigma_pu_maximo = med1->sigma1_pu;
      if(med1->TipoMedicaoPotencia()) flag_PQS = true;
      }
   } // for(nm)

// Acerta valores gerais de desvio padrão
if(! flag_PQS)  // Não há medidores de P, Q ou S
   {
   sigma_pu_minimo = SIGMA_PU_MINIMO;
   sigma_pu_maximo = SIGMA_PU_MAXIMO;
   }
geralC->DesvPadCargaAjustavel    =       20.  * sigma_pu_maximo;
geralC->DesvPadCargaNaoAjustavel = (1. / 50.) * sigma_pu_minimo;
geralC->DesvPadMedV              = (1. / 50.) * sigma_pu_minimo;

// Acerto final do desvio padrão
for(int nm=0; nm < geralC->num_med_reais; nm++)
   {
   med1 = (TMedidor *) lisMED->Items[nm];

   // IMPORTANTE: a comparação deve ser "<=" (cf. acima)
   if(med1->toler_pu <= 0.)
      {
      // Trecho de código comentado: HPS, 2018.02.02
      // Nó com demanda a ser ajustada
      // Modificação abaixo implementada em 2017.11.24 (cargas com P < 0)
      // med1->sigma1_pu = geralC->DesvPadVirtualComCarga;
      // if(fabs(med1->val_med_pu) < ZEROV) med1->sigma1_pu = geralC->DesvPadVirtualSemCarga;
      // else                               med1->sigma1_pu = geralC->DesvPadVirtualComCarga;
      // Trecho de código implementado: HPS, 2018.02.02
      // Medidor real com toler_pu <= 0 deve ter SEMPRE desvio padrão GRANDE,
      // MESMO QUANDO SUA MEDIÇÃO É NULA (para que o Estimador atribua um valor
      // de demanda)
	   med1->sigma1_pu = geralC->DesvPadCargaAjustavel;
      }
   else
      {
      if(fabs(med1->val_med_pu) < ZEROV)
         {
	      // Nó sem demanda a ser ajustada e com carga nula (se tiver carga, será
         // usado o desvio padrão fornecido)
         med1->sigma1_pu = geralC->DesvPadCargaNaoAjustavel;
         }
      }
   }

// Atualiza flag de montagem da matriz 'Rm1'
flag_montar_Rm1 = true;
}

//---------------------------------------------------------------------------
void __fastcall TEstimador::DeletaLista(TList **lista)
   {
   TObject *ptr;

   if((*lista) == NULL) return;

   for(int n=0; n < (*lista)->Count; n++)
      {
      ptr = (TObject *) (*lista)->Items[n];
      delete ptr;
      }
   delete (*lista);
   (*lista) = NULL;
   }

//---------------------------------------------------------------------------
void __fastcall TEstimador::DeletaMatrizes(void)
{
delete A;     A     = NULL;
delete H;     H     = NULL;
delete HtRm1; HtRm1 = NULL;
}

//---------------------------------------------------------------------------
bool __fastcall TEstimador::GravaArquivoOrdemColunas(void)
{
// Variáveis locais
AnsiString nome_arq;
FILE       *arq;

// Verifica se já gravou o arquivo antes
if(gravou_arq_ordem_col) return(true);  // Já gravou o arquivo antes

// Abre arquivo
nome_arq = geralC->DirTmp() + "\\OrdemColunas.txt";
arq      = fopen(nome_arq.c_str(), "w");
if(arq == NULL) return(false);  // Erro

// Grava arquivo
fprintf(arq, " %6d", num_col_ordenadas);
for(int n=0; n < num_col_ordenadas; n++)
   {
   fprintf(arq, "\n %6d", vet_col_ordenadas[n]);
   }

// Fecha arquivo e liga flag de gravação
fclose(arq);
gravou_arq_ordem_col = true;

// Retorna
return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TEstimador::GravaMatriz(VTSisLin   *mat,
										          AnsiString nome_matriz)
   {
   // Grava matriz do sistema linear de equações

   // Variáveis locais
   int        jinic, jm1, num_colunas, num_linhas, num_nao_nulos;
   AnsiString nome_arq;
   FILE       *file1;
   TList      *lisELEMS;
   TElemD     *elem_i, *elem_j;

   // Define nome do arquivo;
   nome_arq = geralC->DirTmp() + "\\" + nome_matriz + ".txt";

   // Conta elementos não-nulos, descartando colunas irrelevantes
   num_colunas   = mat->NumLinhasSubmatriz();
   num_linhas    = mat->NumTotalLinhas();
   num_nao_nulos = 0;
   for(int nl=0; nl < num_linhas; nl++)
      {
      lisELEMS = mat->ElementosLinha(nl);
      for(int i=0; i < lisELEMS->Count; i++)
         {
         elem_i = (TElemD *) lisELEMS->Items[i];
         if(elem_i->coluna < num_colunas) num_nao_nulos++;
         }
      }

   // Abre arquivo e grava o primeiro registro
   file1 = fopen(nome_arq.c_str(), "w");
   fprintf(file1, "%d %d %d\n", num_linhas, num_colunas, num_nao_nulos);

   // Loop de linhas
   for(int nl=0; nl < num_linhas; nl++)
      {
      lisELEMS = mat->ElementosLinha(nl);
      // Ordena elementos em 'lisELEMS' em ordem crescente da coluna
      for(int i=1; i < lisELEMS->Count; i++)
         {
         elem_i = (TElemD *) lisELEMS->Items[i];
         jinic  = i - 1;
         jm1    = i;
         for(int j=jinic; j >= 0; j--)
            {
            elem_j = (TElemD *) lisELEMS->Items[j];
            if(elem_j->coluna <= elem_i->coluna) break;
            lisELEMS->Items[jm1] = elem_j;
            jm1                  = j;
            }
         lisELEMS->Items[jm1] = elem_i;
         }
      // Grava matriz, descartando colunas irrelevantes
      for(int ne=0; ne < lisELEMS->Count; ne++)
         {
         elem_i = (TElemD *) lisELEMS->Items[ne];
         if(elem_i->coluna < num_colunas)
            {
            fprintf(file1, "%d %d %19.12e\n", nl, elem_i->coluna, elem_i->valor);
            }
         }
      }
   fclose(file1);

   // Retorna
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TEstimador::GravaVetor(int        num_elems,
									            double     *vetor,
                                       AnsiString nome_vetor)
   {
   // Grava termo conhecido ou vetor solução do sistema linear de equações

   // Variáveis locais
   AnsiString nome_arq;
   FILE       *file1;

   // Define nome do arquivo
   nome_arq = geralC->DirTmp() + "\\" + nome_vetor + ".txt";

   // Grava vetor
   file1 = fopen(nome_arq.c_str(), "w");
   fprintf(file1, "%d\n", num_elems);
   for(int nl=0; nl < num_elems; nl++)
      {
      fprintf(file1, "%19.12e\n", vetor[nl]);
      }
   fclose(file1);

   // Retorna
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TEstimador::LeArquivoOrdemColunas(void)
{
// Variáveis locais
AnsiString nome_arq;
FILE       *arq;

// Libera memória alocada anteriormente
delete[] vet_col_arq;
vet_col_arq = NULL;

// Abre arquivo
nome_arq = geralC->DirTmp() + "\\OrdemColunas.txt";
arq      = fopen(nome_arq.c_str(), "r");
if(arq == NULL) return(false);  // Erro

// Lê número de colunas e aloca memória para 'vet_col_arq'
fscanf(arq, "\n%d", &num_col_arq);
vet_col_arq = new int [num_col_arq];

// Lê colunas
for(int n=0; n < num_col_arq; n++)
   {
   fscanf(arq, "\n%d", (vet_col_arq+n));
   }

// Fecha arquivo e liga flag de leitura
fclose(arq);
leu_arq_ordem_col = true;

// Retorna
return(true);
}

//---------------------------------------------------------------------------
void __fastcall TEstimador::LiberaMemoria(void)
{
DeletaLista(&lisBAR_ESTADO);
delete lisBAR_REF; // Os elementos desta lista são os mesmos de rede1->lisBAR ou rede3->lisBAR
DeletaLista(&lisYE);

delete[] Rm1;
delete[] vet_desv;
delete[] vet_desv_iter;
delete[] vet_TC;
delete[] vet_tensoes;
delete[] vet_col_arq;
delete[] vet_col_ordenadas;
}

//---------------------------------------------------------------------------
void __fastcall TEstimador::Log(AnsiString txt, bool data)
   {
   //var locais
   AnsiString texto = txt;
   TDateTime  dt;
   VTLog *log  = (VTLog*)apl->GetObject(__classid(VTLog));

   if (log)
		{
		if(data)
			{
			dt = Now();
			texto = dt.DateTimeString() + "\t" + txt;
			}
		log->Add(texto);
		}
   }

//---------------------------------------------------------------------------
bool __fastcall TEstimador::RecuperaOrdemColunas(VTSisLin *mat)
{
// Esta função admite duas situações, controladas por 'le_arq_ordem_col':
//    1. Leitura da ordem das colunas de arquivo (uma única vez), ou
//    2. Recuperação da ordem das colunas determinada em decomposição anterior

// Variáveis locais
bool sucesso;

// Procura ler o arquivo se sua leitura foi solicitada e isso não foi
// feito antes
if(le_arq_ordem_col)
   {
   if(! leu_arq_ordem_col)  // Nunca leu o arquivo antes
      {
      sucesso = LeArquivoOrdemColunas();
      if(sucesso)  // Leitura do arquivo OK
         {
         if(num_col_arq != mat->NumLinhasSubmatriz()) return(false); // Erro
         if(vet_col_ordenadas == NULL)  // Aloca memória pela primeira vez
            {
            num_col_ordenadas = num_col_arq;
            vet_col_ordenadas = new int [num_col_ordenadas];
            }
         // Transfere conteúdo do arquivo a 'vet_col_ordenadas'
         for(int n=0; n < num_col_ordenadas; n++)
            {
            vet_col_ordenadas[n] = vet_col_arq[n];
            }
         } // if(sucesso)
      } // if(! leu_arq_ordem_col)
   } // if(le_arq_ordem_col)

// Redefine ordenação de colunas, se o vetor estiver disponível
if(vet_col_ordenadas == NULL) return(true); // O vetor não existe ainda (nada a fazer)
if(num_col_ordenadas != mat->NumLinhasSubmatriz()) return(false); // Erro
mat->DefineOrdemColunas(num_col_ordenadas, vet_col_ordenadas);

// Retorna
return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TEstimador::SalvaOrdemColunas(VTSisLin *mat)
{
// Retorna se o vetor já foi montado antes (nada a fazer)
if(vet_col_ordenadas != NULL) return(true);

// Monta vetor com a ordem das colunas (determinada na primeira decomposição QR)
num_col_ordenadas = mat->NumLinhasSubmatriz();
vet_col_ordenadas = new int [num_col_ordenadas];
for(int n=0; n < num_col_ordenadas; n++)
   {
   vet_col_ordenadas[n] = mat->Linha(n);
   }

// Grava arquivo, se indicado pelo flag
if(grava_arq_ordem_col) return(GravaArquivoOrdemColunas());
else                    return(true);
}

//---------------------------------------------------------------------------
//eof

