//---------------------------------------------------------------------------
#pragma hdrstop
#include ".\TTrafo3Radial.h"
#include "..\Calculo\Rede1\TTrafo3.h"
#include "..\Matriz\VTMatriz.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TTrafo3Radial::TTrafo3Radial(TTrafo3 *trafo3,
                                        int     sentido)
   {
   VTMatriz *YD2;

   // Inicializa��o
   this->sentido = -1;
   vet_cor_acm   = NULL;
   vet_deltav    = NULL;
   this->trafo3  = NULL;
   YD            = NULL;
   ZD            = NULL;

   // Verifica valor de 'sentido'
   if((sentido < 0) || (sentido > 2)) return;  // Erro

   // Salva dados
   this->sentido      = sentido;
   this->trafo3       = trafo3;
   trafo3->ptr_radial = this;  // Atualiza ponteiro no objeto original

   // Aloca mem�ria
   vet_cor_acm = new complex<double> [3];
   vet_deltav  = new complex<double> [2];
   YD          = NewObjMatrizC(3, 3);
   YD2         = NewObjMatrizC(2, 2);

   // Copia Ynodal do TTrafo3 (completa e reduzida)
   CopiaYnodalDCompleta();
   CopiaYnodalDReduzida(YD2);

   // Obt�m Znodal
   ZD = YD2->Inverte();

   // Deleta YD2
   delete YD2;
   }

//---------------------------------------------------------------------------
__fastcall TTrafo3Radial::~TTrafo3Radial(void)
   {
   // Libera mem�ria
   delete[] vet_cor_acm;
   delete[] vet_deltav;
   delete   YD;
   delete   ZD;
   }

//---------------------------------------------------------------------------
void __fastcall TTrafo3Radial::CopiaYnodalDCompleta(void)
   {
   YD->C2[0][0] = trafo3->ynodalD_pu[0];  // Linha do prim�rio
   YD->C2[0][1] = trafo3->ynodalD_pu[1];
   YD->C2[0][2] = trafo3->ynodalD_pu[2];
   YD->C2[1][0] = trafo3->ynodalD_pu[1];  // Linha do secund�rio
   YD->C2[1][1] = trafo3->ynodalD_pu[3];
   YD->C2[1][2] = trafo3->ynodalD_pu[4];
   YD->C2[2][0] = trafo3->ynodalD_pu[2];  // Linha do terci�rio
   YD->C2[2][1] = trafo3->ynodalD_pu[4];
   YD->C2[2][2] = trafo3->ynodalD_pu[5];
   }

//---------------------------------------------------------------------------
void __fastcall TTrafo3Radial::CopiaYnodalDReduzida(VTMatriz *YD2)
   {
   if(sentido == 0)  // O enrolamento fonte � o prim�rio
      {
      YD2->C2[0][0] = trafo3->ynodalD_pu[3];  // Linha do secund�rio
      YD2->C2[0][1] = trafo3->ynodalD_pu[4];  // Linha do secund�rio
      YD2->C2[1][0] = trafo3->ynodalD_pu[4];  // Linha do terci�rio
      YD2->C2[1][1] = trafo3->ynodalD_pu[5];  // Linha do terci�rio
      }
   else if(sentido == 1)  // O enrolamento fonte � o secund�rio
      {
      YD2->C2[0][0] = trafo3->ynodalD_pu[0];  // Linha do prim�rio
      YD2->C2[0][1] = trafo3->ynodalD_pu[2];  // Linha do prim�rio
      YD2->C2[1][0] = trafo3->ynodalD_pu[2];  // Linha do terci�rio
      YD2->C2[1][1] = trafo3->ynodalD_pu[5];  // Linha do terci�rio
      }
   else  // O enrolamento fonte � o terci�rio
      {
      YD2->C2[0][0] = trafo3->ynodalD_pu[0];  // Linha do prim�rio
      YD2->C2[0][1] = trafo3->ynodalD_pu[1];  // Linha do prim�rio
      YD2->C2[1][0] = trafo3->ynodalD_pu[1];  // Linha do secund�rio
      YD2->C2[1][1] = trafo3->ynodalD_pu[3];  // Linha do secund�rio
      }
   }

//---------------------------------------------------------------------------
void __fastcall TTrafo3Radial::ZeraCorrentes(void)
{
vet_cor_acm[0] = vet_cor_acm[1] = vet_cor_acm[2] = complex<double>(0., 0.);
}

//---------------------------------------------------------------------------


