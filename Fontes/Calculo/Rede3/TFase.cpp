//---------------------------------------------------------------------------
#pragma hdrstop
#include ".\TFase.h"
#include "..\Comum13\VTGeralC.h"
#include "..\..\Constante\Fases.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TFase::TFase(int  fases_int,
                        bool flag_ordena_fases,
                        bool flag_linhas)
   {
   // Aloca memória para vetores
   vet_fases = new int [MAX_FASES_G3];
   if(flag_linhas)  // Cria e inicializa vetor de linhas em Ynodal
      {
      vet_linhas = new int [MAX_FASES_G3];
      ReiniciaLinhas();
      }
   else  // Não precisa deste vetor
      {
      vet_linhas = NULL;
      }

   // Monta vetor de fases
   MontaVetorFases(fases_int, flag_ordena_fases);
   }

//---------------------------------------------------------------------------
__fastcall TFase::~TFase(void)
   {
   // Deleta objetos locais
   delete[] vet_fases;
   if(vet_linhas != NULL) delete[] vet_linhas;
   }

//---------------------------------------------------------------------------
bool __fastcall TFase::AddFase(int  fase,
                               bool flag_ordena_fases)
{
int jm1;

// Verifica fase fornecida
if((fase != faseA) && (fase != faseB) && (fase != faseC) &&
   (fase != faseN)) return(false);  // Erro

// Verifica se a fase fornecida já existe
if(ExisteFase(fase)) return(true);

// Adiciona fase
if(num_fases >= MAX_FASES_G3) return(false);  // Erro
vet_fases[num_fases] = fase;
num_fases++;

// Ordena fases na ordem A-B-C-N, se indicado pelo flag
if(flag_ordena_fases)
   {
   jm1 = num_fases - 1;
   for(int j=(num_fases-2); j >= 0; j--)
      {
      if(fase > vet_fases[j]) break;
      vet_fases[jm1] = vet_fases[j];
      jm1            = j;
      }
   vet_fases[jm1] = fase;
   }

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TFase::AddFaseVetor(int  fase,
                                    int  *num_fases_aux,
                                    int  *vet_fases_aux,
                                    bool flag_ordena_fases)
{
int jm1;

// Verifica fase fornecida
if((fase != faseA) && (fase != faseB) && (fase != faseC) &&
   (fase != faseN)) return(false);  // Erro

// Verifica se a fase fornecida já existe
for(int n=0; n < *num_fases_aux; n++)
   {
   if(vet_fases_aux[n] == fase) return(true);  // A fase já existe
   }

// Adiciona fase
if(*num_fases_aux >= MAX_FASES_G3) return(false);  // Erro
vet_fases_aux[*num_fases_aux] = fase;
(*num_fases_aux)++;

// Ordena fases na ordem A-B-C-N, se indicado pelo flag
if(flag_ordena_fases)
   {
   jm1 = (*num_fases_aux) - 1;
   for(int j=((*num_fases_aux)-2); j >= 0; j--)
      {
      if(fase > vet_fases_aux[j]) break;
      vet_fases_aux[jm1] = vet_fases_aux[j];
      jm1                = j;
      }
   vet_fases_aux[jm1] = fase;
   }

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TFase::Copia(TFase *fases2)
{
fases2->num_fases = num_fases;
for(int n=0; n < num_fases; n++)
   {
   fases2->vet_fases[n] = vet_fases[n];
   if(fases2->vet_linhas != NULL) fases2->vet_linhas[n] = -1;  // Inicialização
   }

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TFase::ExisteFase(int fase_int)
{
for(int nf=0; nf < num_fases; nf++)
   {
   if(vet_fases[nf] == fase_int) return(true);
   }

return(false);
}

//---------------------------------------------------------------------------
bool __fastcall TFase::ExisteFaseNumero(int numero)
{
int fase;

if     (numero == 0) fase = faseA;
else if(numero == 1) fase = faseB;
else if(numero == 2) fase = faseC;
else if(numero == 3) fase = faseN;
else return(false);

return(ExisteFase(fase));
}

//---------------------------------------------------------------------------
bool __fastcall TFase::ExisteN(void)
{
return(ExisteFase(faseN));
}

//---------------------------------------------------------------------------
int __fastcall TFase::Fase(int indice)
{
if((indice >= 0) && (indice < num_fases)) return(vet_fases[indice]);
else                                      return(faseINV);
}

//---------------------------------------------------------------------------
char __fastcall TFase::FaseAsChar(int fase_int)
{
if     (fase_int == faseA) return('A');
else if(fase_int == faseB) return('B');
else if(fase_int == faseC) return('C');
else if(fase_int == faseN) return('N');
else if(fase_int == faseT) return('T');
else                       return('*');
}

//---------------------------------------------------------------------------
char __fastcall TFase::FaseAsCharIndice(int indice)
{
if((indice >= 0) && (indice < num_fases)) return(FaseAsChar(vet_fases[indice]));
else                                      return('*');
}

//---------------------------------------------------------------------------
int __fastcall TFase::FaseNumero(int numero)
{
if     (numero == 0) return(faseA);
else if(numero == 1) return(faseB);
else if(numero == 2) return(faseC);
else if(numero == 3) return(faseN);
else                 return(faseINV);
}

//---------------------------------------------------------------------------
int __fastcall TFase::FasesAsInt(void)
{
// Variáveis locais
int fases_int;

// Calcula 'fases_int'
fases_int = 0;
for(int nfa=0; nfa < num_fases; nfa++)
   {
   fases_int += vet_fases[nfa];
   }

// Retorna
return(fases_int);
}

//---------------------------------------------------------------------------
AnsiString __fastcall TFase::FasesAsString(void)
{
AnsiString string1;

string1 = "";
for(int nfa=0; nfa < num_fases; nfa++)
   {
   string1 += FaseAsCharIndice(nfa);
   }

return(string1);
}

//---------------------------------------------------------------------------
bool __fastcall TFase::FasesIguais(TFase *fases2)
   {
   if(num_fases != fases2->num_fases) return(false);
   // IMPORTANTE: a comparação abaixo pressupõe que as fases estão ordenadas
   //             em ambos objetos
   for(int n=0; n < num_fases; n++)
      {
      if(vet_fases[n] != fases2->vet_fases[n]) return(false);
      }

   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TFase::Impoe4Fases(void)
   {
   num_fases    = MAX_FASES_G3;
   vet_fases[0] = faseA;
   vet_fases[1] = faseB;
   vet_fases[2] = faseC;
   vet_fases[3] = faseN;
   ReiniciaLinhas();
   }

//---------------------------------------------------------------------------
int __fastcall TFase::Indice(int fase_int)
{
// Determina o endereço (offset) da fase fornecida, neste objeto
for(int n=0; n < num_fases; n++)
   {
   if(vet_fases[n] == fase_int) return(n);
   }

return(-1);
}

//---------------------------------------------------------------------------
int __fastcall TFase::IndiceA(void)
{
return(Indice(faseA));
}

//---------------------------------------------------------------------------
int __fastcall TFase::IndiceABCN(int indice)
{
if((indice < 0) || (indice >= num_fases)) return(-1);
else return(IndiceABCN_Fase(vet_fases[indice]));
}

//---------------------------------------------------------------------------
int __fastcall TFase::IndiceABCN_Fase(int fase_int)
{
if     (fase_int == faseA) return(0);
else if(fase_int == faseB) return(1);
else if(fase_int == faseC) return(2);
else if(fase_int == faseN) return(3);
else                       return(-1);
}

//---------------------------------------------------------------------------
int __fastcall TFase::IndiceB(void)
{
return(Indice(faseB));
}

//---------------------------------------------------------------------------
int __fastcall TFase::IndiceC(void)
{
return(Indice(faseC));
}

//---------------------------------------------------------------------------
int __fastcall TFase::IndiceLinear(int i,
                                   int j)
{
return((i * MAX_FASES_G3) + j);
}

//---------------------------------------------------------------------------
int __fastcall TFase::IndiceN(void)
{
return(Indice(faseN));
}

//---------------------------------------------------------------------------
bool __fastcall TFase::IsFaseABC(int fase_int)
{
if((fase_int == faseA) ||
   (fase_int == faseB) ||
   (fase_int == faseC)) return(true);
else                    return(false);
}

//---------------------------------------------------------------------------
int __fastcall TFase::LinhaFase(int fase_int)
{
if(vet_linhas == NULL) return(-1);

for(int nf=0; nf < num_fases; nf++)
   {
   if(vet_fases[nf] == fase_int) return(vet_linhas[nf]);
   }

return(-1);
}

//---------------------------------------------------------------------------
int __fastcall TFase::LinhaIndice(int indice)
{
if(vet_linhas == NULL)                         return(-1);
else if((indice < 0) || (indice >= num_fases)) return(-1);
else                                           return(vet_linhas[indice]);
}

//---------------------------------------------------------------------------
void __fastcall TFase::MontaVetorFases(int  fases_int,
                                       bool flag_ordena_fases)
{
// 'flag_ordena_fases' serve para controlar a ordenação automática das fases
// em cada chamada de AddFase(). Isto é necessário no caso de trafos
// monofásicos com fases CA ou CAN: a ordenação das fases alteraria a
// polaridade do enrolamento, logo as fases não devem ser ordenadas
// (flag_ordena_fases == false).
// Também serve para suprimentos bifásicos CAN.

// Inicialização
Reinicia();

// Adiciona fase(s) em função do valor inteiro fornecido
switch(fases_int)
   {
   case faseA:
      AddFase(faseA);
      break;

   case faseB:
      AddFase(faseB);
      break;

   case faseC:
      AddFase(faseC);
      break;

   case faseN:
   case faseNT:
   case faseT:
      AddFase(faseN);
      break;

   case faseAN:
   case faseANT:
   case faseAT:
      AddFase(faseA);
      AddFase(faseN);
      break;

   case faseBN:
   case faseBNT:
   case faseBT:
      AddFase(faseB);
      AddFase(faseN);
      break;

   case faseCN:
   case faseCNT:
   case faseCT:
      AddFase(faseC);
      AddFase(faseN);
      break;

   case faseAB:
      AddFase(faseA);
      AddFase(faseB);
      break;

   case faseBC:
      AddFase(faseB);
      AddFase(faseC);
      break;

   case faseCA:
      AddFase(faseC, flag_ordena_fases); // CA: necessário incluir 'flag_ordena_fases'
      AddFase(faseA, flag_ordena_fases); // CA: necessário incluir 'flag_ordena_fases'
      break;

   case faseABN:
   case faseABNT:
   case faseABT:
      AddFase(faseA);
      AddFase(faseB);
      AddFase(faseN);
      break;

   case faseBCN:
   case faseBCNT:
   case faseBCT:
      AddFase(faseB);
      AddFase(faseC);
      AddFase(faseN);
      break;

   case faseCAN:
   case faseCANT:
   case faseCAT:
      AddFase(faseC, flag_ordena_fases); // CAN: necessário incluir 'flag_ordena_fases'
      AddFase(faseA, flag_ordena_fases); // CAN: necessário incluir 'flag_ordena_fases'
      AddFase(faseN, flag_ordena_fases); // CAN: necessário incluir 'flag_ordena_fases'
      break;

   case faseABC:
      AddFase(faseA);
      AddFase(faseB);
      AddFase(faseC);
      break;

   case faseABCN:
   case faseABCNT:
      AddFase(faseA);
      AddFase(faseB);
      AddFase(faseC);
      AddFase(faseN);
      break;

   default:  // Fases não fornecidas (nada a fazer)
      break;
   }
}

//---------------------------------------------------------------------------
int __fastcall TFase::NumFases(void)
{
return(num_fases);
}

//---------------------------------------------------------------------------
int __fastcall TFase::NumFasesSemNT(void)
{
int f1, num_fases_semNT;

num_fases_semNT = 0;
for(int nf=0; nf < num_fases; nf++)
   {
   f1 = vet_fases[nf];
   if((f1 != faseN) && (f1 != faseT)) num_fases_semNT++;
   }

return(num_fases_semNT);
}

//---------------------------------------------------------------------------
int __fastcall TFase::NumFasesQ(void)
{
return(num_fases * num_fases);
}

//---------------------------------------------------------------------------
void __fastcall TFase::Reinicia(void)
{
num_fases = 0;
}

//---------------------------------------------------------------------------
void __fastcall TFase::ReiniciaLinhas(void)
{
if(vet_linhas != NULL)  // Existe o vetor
   {
   for(int n=0; n < MAX_FASES_G3; n++)
      {
      vet_linhas[n] = -1;
      }
   }
}

//---------------------------------------------------------------------------
bool __fastcall TFase::RemoveFase(int fase_int)
{
int numaux;

numaux = 0;
for(int n=0; n < num_fases; n++)
   {
   if(vet_fases[n] == fase_int)
      {
      if((n+1) < num_fases)
         {
         vet_fases[n] = vet_fases[n+1];
         if(vet_linhas != NULL) vet_linhas[n] = vet_linhas[n+1];
         }
      }
   else
      {
      numaux++;
      }
   }
num_fases = numaux;

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TFase::SetFase(int indice,
                               int fase_int)
{
if((indice < 0) || (indice >= num_fases)) return(false);
if((fase_int != faseA) && (fase_int != faseB) && (fase_int != faseC) &&
   (fase_int != faseN)) return(false);  // Erro
vet_fases[indice] = fase_int;

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TFase::SetLinhaIndice(int indice,
                                      int linha)
{
if((vet_linhas != NULL) &&
   (indice >= 0)        &&
   (indice < num_fases))
   {
   vet_linhas[indice] = linha;  // Não há problema se 'linha' for negativo
   return(true);
   }
else  // Erro
   {
   return(false);
   }
}

//---------------------------------------------------------------------------
void __fastcall TFase::SetNumFases(int num_fases)
{
if((num_fases >= 0) && (num_fases <= this->num_fases)) this->num_fases = num_fases;
}

//---------------------------------------------------------------------------
int __fastcall TFase::Uniao(TFase *fases2,
                            int   *vet_fases_aux)
{
int fase, num_fases_aux;

// Inicialização
num_fases_aux = 0;

// Inclui fases deste objeto ('this')
for(int n=0; n < num_fases; n++)
   {
   fase = vet_fases[n];
   if(! AddFaseVetor(fase, &num_fases_aux, vet_fases_aux)) return(0);
   }

// Inclui fases do objeto 'fases2'
for(int n=0; n < fases2->num_fases; n++)
   {
   fase = fases2->vet_fases[n];
   if(! AddFaseVetor(fase, &num_fases_aux, vet_fases_aux)) return(0);
   }

return(num_fases_aux);
}

//---------------------------------------------------------------------------
bool __fastcall TFase::VerificaFases(int num_fases,
                                     int *vet_fases)
   {
   int fase, num_vezes;

   num_vezes = 0;
   for(int nf=0; nf < num_fases; nf++)
      {
      fase = vet_fases[nf];
      for(int nf2=0; nf2 < this->num_fases; nf2++)
         {
         if(fase == this->vet_fases[nf2])
            {
            num_vezes++;
            break;
            }
         }
      }

   if(num_vezes == num_fases) return(true);
   else                       return(false);
   }

//---------------------------------------------------------------------------

