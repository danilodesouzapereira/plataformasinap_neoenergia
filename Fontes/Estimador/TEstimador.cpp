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
   geralC            = NULL; // Ser� criado em TEstimador1 ou TEstimador3 (classes derivadas)

   // Define constantes
   raiz3 = sqrt(3.);
   czero = complex<double>(0., 0.);
   cum   = complex<double>(1., 0.);

   // Inicializa vari�veis
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
   TipoBarraRef            = brFIXOS_1THETA_0V; // Estima��o de estado convencional
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
// Vari�veis locais
bool     flag_PQS;
double   sigma_pu_maximo, sigma_pu_minimo;
TMedidor *med1;

// Inicializa��o
flag_PQS              = false;  // N�o h� medidores de P, Q ou S em princ�pio
sigma_pu_minimo       =  1.e30;
sigma_pu_maximo       = -1.e30;
geralC->num_med_desab = 0;

// Loop de medidores reais
for(int nm=0; nm < geralC->num_med_reais; nm++)
   {
   med1 = (TMedidor *) lisMED->Items[nm];

   // Atualiza o contador de medidores desabilitados
   if(! med1->enabled) geralC->num_med_desab++;

   // Exclui medidor com toler�ncia <= 0.
   // Toler�ncia negativa � definida pelo Estimador de Demanda no caso de
   // medidores de barras que ter�o sua demanda ajustada, os quais ser�o
   // tratados logo a seguir.
   // IMPORTANTE: a compara��o deve ser "<=".
   if(med1->toler_pu <= 0.) continue;


// TEMPOR�RIO: considera que o valor de fundo de escala � a pr�pria
//             medi��o. Esta atribui��o dever� ser removida quando
//             TData passar o valor correto do fundo de escala
//             (atualmente passa o valor zero).
med1->val_fe_pu = fabs(med1->val_med_pu);


   // Calcula desvio padr�o do medidor (pode depender do valor medido)
   med1->CalculaDesvioPadrao(geralC->semi_intervalo_normal);

   // Atualiza valores m�nimo e m�ximo (s� medidores de corrente e pot�ncia)
   if(med1->TipoMedicaoCorrente() || med1->TipoMedicaoPotencia())
      {
	   if(med1->sigma1_pu < sigma_pu_minimo) sigma_pu_minimo = med1->sigma1_pu;
      if(med1->sigma1_pu > sigma_pu_maximo) sigma_pu_maximo = med1->sigma1_pu;
      if(med1->TipoMedicaoPotencia()) flag_PQS = true;
      }
   } // for(nm)

// Acerta valores gerais de desvio padr�o
if(! flag_PQS)  // N�o h� medidores de P, Q ou S
   {
   sigma_pu_minimo = SIGMA_PU_MINIMO;
   sigma_pu_maximo = SIGMA_PU_MAXIMO;
   }
geralC->DesvPadCargaAjustavel    =       20.  * sigma_pu_maximo;
geralC->DesvPadCargaNaoAjustavel = (1. / 50.) * sigma_pu_minimo;
geralC->DesvPadMedV              = (1. / 50.) * sigma_pu_minimo;

// Acerto final do desvio padr�o
for(int nm=0; nm < geralC->num_med_reais; nm++)
   {
   med1 = (TMedidor *) lisMED->Items[nm];

   // IMPORTANTE: a compara��o deve ser "<=" (cf. acima)
   if(med1->toler_pu <= 0.)
      {
      // Trecho de c�digo comentado: HPS, 2018.02.02
      // N� com demanda a ser ajustada
      // Modifica��o abaixo implementada em 2017.11.24 (cargas com P < 0)
      // med1->sigma1_pu = geralC->DesvPadVirtualComCarga;
      // if(fabs(med1->val_med_pu) < ZEROV) med1->sigma1_pu = geralC->DesvPadVirtualSemCarga;
      // else                               med1->sigma1_pu = geralC->DesvPadVirtualComCarga;
      // Trecho de c�digo implementado: HPS, 2018.02.02
      // Medidor real com toler_pu <= 0 deve ter SEMPRE desvio padr�o GRANDE,
      // MESMO QUANDO SUA MEDI��O � NULA (para que o Estimador atribua um valor
      // de demanda)
	   med1->sigma1_pu = geralC->DesvPadCargaAjustavel;
      }
   else
      {
      if(fabs(med1->val_med_pu) < ZEROV)
         {
	      // N� sem demanda a ser ajustada e com carga nula (se tiver carga, ser�
         // usado o desvio padr�o fornecido)
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
// Vari�veis locais
AnsiString nome_arq;
FILE       *arq;

// Verifica se j� gravou o arquivo antes
if(gravou_arq_ordem_col) return(true);  // J� gravou o arquivo antes

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

// Fecha arquivo e liga flag de grava��o
fclose(arq);
gravou_arq_ordem_col = true;

// Retorna
return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TEstimador::GravaMatriz(VTSisLin   *mat,
										          AnsiString nome_matriz)
   {
   // Grava matriz do sistema linear de equa��es

   // Vari�veis locais
   int        jinic, jm1, num_colunas, num_linhas, num_nao_nulos;
   AnsiString nome_arq;
   FILE       *file1;
   TList      *lisELEMS;
   TElemD     *elem_i, *elem_j;

   // Define nome do arquivo;
   nome_arq = geralC->DirTmp() + "\\" + nome_matriz + ".txt";

   // Conta elementos n�o-nulos, descartando colunas irrelevantes
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
   // Grava termo conhecido ou vetor solu��o do sistema linear de equa��es

   // Vari�veis locais
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
// Vari�veis locais
AnsiString nome_arq;
FILE       *arq;

// Libera mem�ria alocada anteriormente
delete[] vet_col_arq;
vet_col_arq = NULL;

// Abre arquivo
nome_arq = geralC->DirTmp() + "\\OrdemColunas.txt";
arq      = fopen(nome_arq.c_str(), "r");
if(arq == NULL) return(false);  // Erro

// L� n�mero de colunas e aloca mem�ria para 'vet_col_arq'
fscanf(arq, "\n%d", &num_col_arq);
vet_col_arq = new int [num_col_arq];

// L� colunas
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
delete lisBAR_REF; // Os elementos desta lista s�o os mesmos de rede1->lisBAR ou rede3->lisBAR
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
// Esta fun��o admite duas situa��es, controladas por 'le_arq_ordem_col':
//    1. Leitura da ordem das colunas de arquivo (uma �nica vez), ou
//    2. Recupera��o da ordem das colunas determinada em decomposi��o anterior

// Vari�veis locais
bool sucesso;

// Procura ler o arquivo se sua leitura foi solicitada e isso n�o foi
// feito antes
if(le_arq_ordem_col)
   {
   if(! leu_arq_ordem_col)  // Nunca leu o arquivo antes
      {
      sucesso = LeArquivoOrdemColunas();
      if(sucesso)  // Leitura do arquivo OK
         {
         if(num_col_arq != mat->NumLinhasSubmatriz()) return(false); // Erro
         if(vet_col_ordenadas == NULL)  // Aloca mem�ria pela primeira vez
            {
            num_col_ordenadas = num_col_arq;
            vet_col_ordenadas = new int [num_col_ordenadas];
            }
         // Transfere conte�do do arquivo a 'vet_col_ordenadas'
         for(int n=0; n < num_col_ordenadas; n++)
            {
            vet_col_ordenadas[n] = vet_col_arq[n];
            }
         } // if(sucesso)
      } // if(! leu_arq_ordem_col)
   } // if(le_arq_ordem_col)

// Redefine ordena��o de colunas, se o vetor estiver dispon�vel
if(vet_col_ordenadas == NULL) return(true); // O vetor n�o existe ainda (nada a fazer)
if(num_col_ordenadas != mat->NumLinhasSubmatriz()) return(false); // Erro
mat->DefineOrdemColunas(num_col_ordenadas, vet_col_ordenadas);

// Retorna
return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TEstimador::SalvaOrdemColunas(VTSisLin *mat)
{
// Retorna se o vetor j� foi montado antes (nada a fazer)
if(vet_col_ordenadas != NULL) return(true);

// Monta vetor com a ordem das colunas (determinada na primeira decomposi��o QR)
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

