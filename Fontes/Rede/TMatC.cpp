//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TMatC.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//fun��o global p/ criar objeto da classe TMatC
//---------------------------------------------------------------------------
VTMatC* __fastcall NewObjMatC(void)
   {
   return(new TMatC());
   }

//---------------------------------------------------------------------------
__fastcall TMatC::TMatC(void)
	{
	//assume dimens�o default
	PD.dim = MAX_FASE;
   //inicia com valores nulos
   Zera();
   //assume que n�o est� momtada
   Montada = false;
   }

//---------------------------------------------------------------------------
__fastcall TMatC::~TMatC(void)
   {
   //nada fazer
   }

//---------------------------------------------------------------------------
VTMatC* __fastcall TMatC::Clone(void)
   {
   //vari�veis locais
   VTMatC *matC;

   //cria uma nova MatZ
   if ((matC = NewObjMatC()) != NULL)
      {//copia dados desta Barra p/ a nova Barra
      matC->CopiaAtributosDe(*this);
      }
   return(matC);
   }

//---------------------------------------------------------------------------
void __fastcall TMatC::CopiaAtributosDe(VTMatC &ref)
   {
   //vari�veis locais
   TMatC &matc = (TMatC&)ref;

   //copia atributos de MatZ
   PD = matc.PD;
   }

//---------------------------------------------------------------------------
int __fastcall TMatC::PM_GetDimensao(void)
   {
   return(PD.dim);
   }

//---------------------------------------------------------------------------
double __fastcall TMatC::PM_GetCap(int i, int j)
   {
   //prote��o
	if ((i < 0)||(i >= MAX_FASE)) throw "TMatC::PM_GetCap(): �ndice inv�lido";
	if ((j < 0)||(j >= MAX_FASE)) throw "TMatC::PM_GetCap(): �ndice inv�lido";
	if ((i >= PD.dim)||(j >= PD.dim)) return(0.);
	return(PD.mat_c[i][j]);
   }

//---------------------------------------------------------------------------
bool __fastcall TMatC::PM_GetMontada(void)
   {
   return(PD.montada);
   }

//---------------------------------------------------------------------------
void __fastcall TMatC::PM_SetCap(int i, int j, double cap)
   {
   //prote��o
	if ((i < 0)||(i >= MAX_FASE)) throw "TMatC::PM_SetCap(): �ndice inv�lido";
	if ((j < 0)||(j >= MAX_FASE)) throw "TMatC::PM_SetCap(): �ndice inv�lido";
   //atualiza apenas a resist�ncia da imped�ncia
   PD.mat_c[i][j] = cap;
   }

//---------------------------------------------------------------------------
void __fastcall TMatC::PM_SetDimensao(int dim)
	{
	PD.dim = dim;
   }

//---------------------------------------------------------------------------
void __fastcall TMatC::PM_SetMontada(bool montada)
	{
	PD.montada = montada;
   }

//---------------------------------------------------------------------------
void __fastcall TMatC::Zera(void)
   {
   //inicia matriz
   for (int i = 0; i < MAX_FASE; i++)
      {
      for (int j = 0; j < MAX_FASE; j++)
         {
         PD.mat_c[i][j] = 0.;
         }
      }
   }

//---------------------------------------------------------------------------
//eof
