//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TMatZ.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//fun��o global p/ criar objeto da classe TMatZ
//---------------------------------------------------------------------------
VTMatZ* __fastcall NewObjMatZ(void)
   {
   return(new TMatZ());
   }

//---------------------------------------------------------------------------
__fastcall TMatZ::TMatZ(void)
	{
	//assume dimens�o default
	Dimensao = MAX_FASE;
   //inicia com valores nulos
   Zera();
   //assume que n�o est� momtada
   Montada = false;
   }

//---------------------------------------------------------------------------
__fastcall TMatZ::~TMatZ(void)
   {
   //nada fazer
   }

//---------------------------------------------------------------------------
VTMatZ* __fastcall TMatZ::Clone(void)
   {
   //vari�veis locais
   VTMatZ *matZ;

   //cria uma nova MatZ
   if ((matZ = NewObjMatZ()) != NULL)
      {//copia dados desta Barra p/ a nova Barra
      matZ->CopiaAtributosDe(*this);
      }
   return(matZ);
   }

//---------------------------------------------------------------------------
void __fastcall TMatZ::CopiaAtributosDe(VTMatZ &ref)
   {
   //vari�veis locais
   TMatZ &matzRef = (TMatZ&)ref;

   //copia atributos de MatZ
   PD = matzRef.PD;
   }

//---------------------------------------------------------------------------
int __fastcall TMatZ::PM_GetDimensao(void)
   {
	return(PD.dim);
   }

//---------------------------------------------------------------------------
bool __fastcall TMatZ::PM_GetMontada(void)
   {
   return(PD.montada);
   }

//---------------------------------------------------------------------------
double __fastcall TMatZ::PM_GetR(int i, int j)
   {
   return(Z[i][j].real());
   }

//---------------------------------------------------------------------------
bool __fastcall TMatZ::PM_GetSimetrica(void)
   {
	for (int i = 0; i < PD.dim; i++)
      {
		for (int j = i+1; j < PD.dim; j++)
         {
         if (PD.mat_z[i][j] != PD.mat_z[j][i]) return(false);
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
double __fastcall TMatZ::PM_GetX(int i, int j)
   {
   return(Z[i][j].imag());
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TMatZ::PM_GetZ(int i, int j)
   {
   //prote��o
	if ((i < 0)||(i >= MAX_FASE)) throw "TMatZ::PM_GetZ(): �ndice inv�lido";
	if ((j < 0)||(j >= MAX_FASE)) throw "TMatZ::PM_GetZ(): �ndice inv�lido";
	if ((i >= PD.dim)||(j >= PD.dim)) return(complex<double>(0., 0.));
	return(PD.mat_z[i][j]);
   }

//---------------------------------------------------------------------------
void __fastcall TMatZ::PM_SetDimensao(int dim)
	{
	PD.dim = dim;
   }

//---------------------------------------------------------------------------
void __fastcall TMatZ::PM_SetMontada(bool montada)
	{
	PD.montada = montada;
   }

//---------------------------------------------------------------------------
void __fastcall TMatZ::PM_SetR(int i, int j, double r_ij)
   {
   //prote��o
	if ((i < 0)||(i >= MAX_FASE)) throw "TMatZ::PM_SetR(): �ndice inv�lido";
	if ((j < 0)||(j >= MAX_FASE)) throw "TMatZ::PM_SetR(): �ndice inv�lido";
   //atualiza apenas a resist�ncia da imped�ncia
   PD.mat_z[i][j] = complex<double>(r_ij, X[i][j]);
   }

//---------------------------------------------------------------------------
void __fastcall TMatZ::PM_SetX(int i, int j, double x_ij)
   {
   //prote��o
	if ((i < 0)||(i >= MAX_FASE)) throw "TMatZ::PM_SetX(): �ndice inv�lido";
	if ((j < 0)||(j >= MAX_FASE)) throw "TMatZ::PM_SetX(): �ndice inv�lido";
	//atualiza apenas a reat�ncia da imped�ncia
   PD.mat_z[i][j] = complex<double>(R[i][j], x_ij);
   }

//---------------------------------------------------------------------------
void __fastcall TMatZ::PM_SetZ(int i, int j, complex<double> z_ij)
   {
   //prote��o
	if ((i < 0)||(i >= MAX_FASE)) throw "TMatZ::PM_SetZ(): �ndice inv�lido";
	if ((j < 0)||(j >= MAX_FASE)) throw "TMatZ::PM_SetZ(): �ndice inv�lido";
	PD.mat_z[i][j] = z_ij;
   }

//---------------------------------------------------------------------------
void __fastcall TMatZ::Zera(void)
   {
   for (int i = 0; i < MAX_FASE; i++)
      {
      for (int j = 0; j < MAX_FASE; j++)
         {
         PD.mat_z[i][j] = complex<double>(0., 0.);
         }
      }
   }

//---------------------------------------------------------------------------
//eof
