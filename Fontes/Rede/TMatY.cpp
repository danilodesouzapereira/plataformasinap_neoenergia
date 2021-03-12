//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TMatY.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe TMatY
//---------------------------------------------------------------------------
VTMatY* __fastcall NewObjMatY(void)
   {
   return(new TMatY());
   }

//---------------------------------------------------------------------------
__fastcall TMatY::TMatY(void)
	{
	//assume dimensão default
	Dimensao = MAX_FASE;
   //inicia com valores nulos
   Zera();
   //assume que não está montada
   Montada = false;
   }

//---------------------------------------------------------------------------
__fastcall TMatY::~TMatY(void)
   {
   //nada fazer
   }

//---------------------------------------------------------------------------
VTMatY* __fastcall TMatY::Clone(void)
   {
   //variáveis locais
   VTMatY *matY;

   //cria uma nova MatZ
   if ((matY = NewObjMatY()) != NULL)
      {//copia dados desta Barra p/ a nova Barra
      matY->CopiaAtributosDe(*this);
      }
   return(matY);
   }

//---------------------------------------------------------------------------
void __fastcall TMatY::CopiaAtributosDe(VTMatY &ref)
   {
   //variáveis locais
   TMatY &matyRef = (TMatY&)ref;

   //copia atributos de MatY
   PD = matyRef.PD;
   }

//---------------------------------------------------------------------------
double __fastcall TMatY::PM_GetB(int i, int j)
   {
   return(Y[i][j].imag());
   }

//---------------------------------------------------------------------------
int __fastcall TMatY::PM_GetDimensao(void)
   {
	return(PD.dim);
   }

//---------------------------------------------------------------------------
double __fastcall TMatY::PM_GetG(int i, int j)
   {
   return(Y[i][j].real());
   }

//---------------------------------------------------------------------------
int __fastcall TMatY::PM_GetModeloEqv(void)
   {
   return(PD.modelo_eqv);
   }

//---------------------------------------------------------------------------
bool __fastcall TMatY::PM_GetMontada(void)
   {
   return(PD.montada);
   }

//---------------------------------------------------------------------------
bool __fastcall TMatY::PM_GetSimetrica(void)
   {
	for (int i = 0; i < PD.dim; i++)
      {
		for (int j = i+1; j < PD.dim; j++)
         {
         if (PD.mat_y[i][j] != PD.mat_y[j][i]) return(false);
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TMatY::PM_GetY(int i, int j)
   {
   //proteção
	if ((i < 0)||(i >= MAX_FASE)) throw "TMatY::PM_GetZ(): índice inválido";
	if ((j < 0)||(j >= MAX_FASE)) throw "TMatY::PM_GetZ(): índice inválido";
	if ((i >= PD.dim)||(j >= PD.dim)) return(complex<double>(0., 0.));
	return(PD.mat_y[i][j]);
   }

//---------------------------------------------------------------------------
void __fastcall TMatY::PM_SetB(int i, int j, double b_ij)
   {
   //proteção
	if ((i < 0)||(i >= MAX_FASE)) throw "TMatY::PM_SetB(): índice inválido";
	if ((j < 0)||(j >= MAX_FASE)) throw "TMatY::PM_SetB(): índice inválido";
	//atualiza apenas a reatância da impedância
   PD.mat_y[i][j] = complex<double>(G[i][j], b_ij);
   }

//---------------------------------------------------------------------------
void __fastcall TMatY::PM_SetDimensao(int dim)
	{
	PD.dim = dim;
   }

//---------------------------------------------------------------------------
void __fastcall TMatY::PM_SetG(int i, int j, double g_ij)
   {
   //proteção
	if ((i < 0)||(i >= MAX_FASE)) throw "TMatY::PM_SetG(): índice inválido";
	if ((j < 0)||(j >= MAX_FASE)) throw "TMatY::PM_SetG(): índice inválido";
   //atualiza apenas a resistência da impedância
   PD.mat_y[i][j] = complex<double>(g_ij, B[i][j]);
   }

//---------------------------------------------------------------------------
void __fastcall TMatY::PM_SetModeloEqv(int modelo_eqv)
	{
   //salva modeloEQV
	PD.modelo_eqv = modelo_eqv;
   }

//---------------------------------------------------------------------------
void __fastcall TMatY::PM_SetMontada(bool montada)
	{
	PD.montada = montada;
   }

//---------------------------------------------------------------------------
void __fastcall TMatY::PM_SetY(int i, int j, complex<double> y_ij)
   {
   //proteção
	if ((i < 0)||(i >= MAX_FASE)) throw "TMatY::PM_SetY(): índice inválido";
	if ((j < 0)||(j >= MAX_FASE)) throw "TMatY::PM_SetY(): índice inválido";
	PD.mat_y[i][j] = y_ij;
   }

//---------------------------------------------------------------------------
void __fastcall TMatY::Zera(void)
   {
   for (int i = 0; i < MAX_FASE; i++)
      {
      for (int j = 0; j < MAX_FASE; j++)
         {
         PD.mat_y[i][j] = complex<double>(0., 0.);
         }
      }
   }

//---------------------------------------------------------------------------
//eof
