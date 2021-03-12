//---------------------------------------------------------------------------
#pragma hdrstop
#include ".\TLigacaoEquiv.h"
#include ".\TBarra.h"
#include ".\TFase.h"
#include "..\..\Complexo\Complexo.h"
#include "..\..\Constante\Fases.h"
#include "..\..\Matriz\VTMatriz.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
// Função para criar objeto da classe
//---------------------------------------------------------------------------
TLigacaoEquiv * __fastcall NewObjLigacaoEquiv(TBarra   *bar1,
                                              TBarra   *bar2,
                                              int      id,
                                              int      fases_int_pri,
                                              int      fases_int_sec,
                                              VTMatriz *mat_ij,
                                              VTMatriz *mat_ji)
   {
   return(new TLigacaoEquiv(bar1, bar2, id, fases_int_pri, fases_int_sec,
                            mat_ij, mat_ji));
   }

//---------------------------------------------------------------------------
__fastcall TLigacaoEquiv::TLigacaoEquiv(TBarra   *bar1,
                                        TBarra   *bar2,
                                        int      id,
                                        int      fases_int_pri,
                                        int      fases_int_sec,
                                        VTMatriz *mat_ij,
                                        VTMatriz *mat_ji)
   {
   // Variáveis locais
   int ind_i, ind_j, num_fases1, num_fases2;

   // Salva dados fornecidos
   this->bar1 = bar1;
   this->bar2 = bar2;
   this->id   = id;
   codigo     = "Treduc" + AnsiString(id);

   // Aloca memória
   fases_pri        = new TFase(fases_int_pri);
   fases_sec        = new TFase(fases_int_sec);
   num_fases1       = fases_pri->NumFases();
   num_fases2       = fases_sec->NumFases();
   mat_Ynodal_ij_pu = NewObjMatrizC(num_fases1, num_fases2);
   mat_Ynodal_ji_pu = NewObjMatrizC(num_fases2, num_fases1);

   // Copia matrizes
   for(int i=0; i < num_fases1; i++)
      {
      ind_i = fases_pri->IndiceABCN(i);
      for(int j=0; j < num_fases2; j++)
         {
         ind_j = fases_sec->IndiceABCN(j);
         mat_Ynodal_ij_pu->C2[i][j] = mat_ij->C2[ind_i][ind_j];
         mat_Ynodal_ji_pu->C2[j][i] = mat_ji->C2[ind_j][ind_i];
         }
      }
   }

//---------------------------------------------------------------------------
__fastcall TLigacaoEquiv::~TLigacaoEquiv(void)
   {
   // Libera memória
   delete fases_pri;
   delete fases_sec;
   delete mat_Ynodal_ij_pu;
   delete mat_Ynodal_ji_pu;
   }

//---------------------------------------------------------------------------
void __fastcall TLigacaoEquiv::ImprimeMatriz(AnsiString nome_arq,
                                             int        ordem)
{
if(ordem == 0) mat_Ynodal_ij_pu->Imprime(nome_arq, 0);
else           mat_Ynodal_ji_pu->Imprime(nome_arq, 0);
}

//---------------------------------------------------------------------------
complex<double> __fastcall TLigacaoEquiv::PM_GetMatYnodal_ij_pu(int i,
                                                                int j)
{
if(mat_Ynodal_ij_pu == NULL) return(complex<double>(0., 0.));

return(mat_Ynodal_ij_pu->C2[i][j]);
}

//---------------------------------------------------------------------------
complex<double> __fastcall TLigacaoEquiv::PM_GetMatYnodal_ji_pu(int i,
                                                                int j)
{
if(mat_Ynodal_ji_pu == NULL) return(complex<double>(0., 0.));

return(mat_Ynodal_ji_pu->C2[i][j]);
}

//---------------------------------------------------------------------------
void __fastcall TLigacaoEquiv::PM_SetMatYnodal_ij_pu(int             i,
                                                     int             j,
                                                     complex<double> valor)
{
if(mat_Ynodal_ij_pu == NULL) return;

mat_Ynodal_ij_pu->C2[i][j] = valor;
}

//---------------------------------------------------------------------------
void __fastcall TLigacaoEquiv::PM_SetMatYnodal_ji_pu(int             i,
                                                     int             j,
                                                     complex<double> valor)
{
if(mat_Ynodal_ji_pu == NULL) return;

mat_Ynodal_ji_pu->C2[i][j] = valor;
}

//---------------------------------------------------------------------------

