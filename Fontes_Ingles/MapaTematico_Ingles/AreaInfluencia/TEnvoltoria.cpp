//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <math.h>
#include "TEnvoltoria.h"
#include "TPtoCarga.h"
//#include <stdio.h>
//#include <PlataformaSinap\Fontes\Rede\VTBarra.h>
#include "..\..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTEnvoltoria* __fastcall NewObjEnvoltoria(void)
   {
   try{
      return(new TEnvoltoria());
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TEnvoltoria::TEnvoltoria(void)
   {
   //cria objetos
   local.lisPTC = new TList();
   local.lisENV = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TEnvoltoria::~TEnvoltoria(void)
   {
   //destrói listas sem destruir seus objetos
   if (local.lisPTC) {delete local.lisPTC; local.lisPTC = NULL;}
   if (local.lisENV) {delete local.lisENV; local.lisENV = NULL;}
   }

//---------------------------------------------------------------------------
TPtoCarga* __fastcall TEnvoltoria::BuscaPtoCargaInicial(void)
   {
   TPtoCarga *ptcIni, *ptcAux;

   if(local.lisPTC->Count == 0) return(NULL);

   ptcIni = (TPtoCarga*)local.lisPTC->First();
   for(int n = 1; n < local.lisPTC->Count; n++)
      {
      ptcAux = (TPtoCarga*)local.lisPTC->Items[n];
      if(ptcAux->X_cm < ptcIni->X_cm) ptcIni = ptcAux;
      }
   return(ptcIni);
   }

//---------------------------------------------------------------------------
TPtoCarga* __fastcall TEnvoltoria::BuscaSeg(TPtoCarga *ptc1, TPtoCarga *ptc2)
   {
   double    xA, yA, x1, y1, x2, y2;
   double    xm, ym;
   double    modA;
   double    detM, tetaMin, tetaAux;
   TPtoCarga *ptcAux, *ptcM;
   //double pi = 3.141592654;

   //proteção
   if(local.lisPTC->Count == 0) return(NULL);
   if(ptc1 == NULL) return(NULL);

   x1 = ptc1->X_cm;
   y1 = ptc1->Y_cm;
   //Define vetor A
   if(ptc2 == NULL)
     {x2 = ptc1->X_cm;
      y2 = ptc1->Y_cm;
      xA = 0.0;
      yA =-1.0;
     }
   else
     {x2 = ptc2->X_cm;
      y2 = ptc2->Y_cm;
      xA = x2 - x1;
      yA = y2 - y1;
     }
   //Cálcula o módulo de A
   modA = sqrt(xA * xA + yA * yA);


   ptcM = (TPtoCarga*)local.lisPTC->First();
   xm = ptcM->X_cm - x2;
   ym = ptcM->Y_cm - y2;
   tetaMin = CalcAng(xA, yA, xm, ym, modA);


   for(int n = 1; n < local.lisPTC->Count; n++)
      {
      ptcAux = (TPtoCarga*)local.lisPTC->Items[n];
      xm = ptcAux->X_cm - x2;
      ym = ptcAux->Y_cm - y2;
      //Calcula o angulo
      tetaAux = CalcAng(xA, yA, xm, ym, modA);
      if(tetaAux < tetaMin)
         {tetaMin = tetaAux;
          ptcM    = ptcAux;
         }
      }

   return(ptcM);
   }

//---------------------------------------------------------------------------
double __fastcall TEnvoltoria::CalcAng(double xA, double yA, double xm, double ym, double modA)
   {
   double modM, prEsc;
   double detM, tetaAux;
   double pi = 3.141592654;
   //Calcula orientação de M
   detM = xA * ym - yA * xm;
   if(!IsDoubleZero(detM))
     {
     //Calcula o módulo de m
     modM = sqrt(xm * xm + ym * ym);
     //Calcula o produto escalar
     prEsc = (xA * xm + yA * ym) / (modA * modM);
     //Calcula o angulo
     tetaAux = acos(prEsc);
     if(detM < 0) tetaAux = 2 * pi - tetaAux;
     }
   else
     {
     if((xA * xm < 0.0)||(yA * ym < 0.0)) tetaAux = pi;
     else
		{//Calcula o módulo de m
		modM = sqrt(xm * xm + ym * ym);
		if(modM >= modA)
			tetaAux = 0.0;
		else
			tetaAux = 2 * pi;
		//tetaAux = 0.0; //Talvez de pau se houver mais de 3 pontos LD
		}
	  }
	return(tetaAux);
	}

//---------------------------------------------------------------------------
TList* __fastcall TEnvoltoria::Executa(TList *lisPTC)
	{
	//variáveis locais
	TPtoCarga *ptcA, *ptcB, *ptc1, *ptcAux;
	double  dAng0, dAng1, dAngAux;

	//protecao
   if (lisPTC->Count <= 2) return(NULL);
   //copia lisPTC p/ local.lisPTC, descartando pontos de mesma coordenada
   //CopiaTList(lisPTC, local.lisPTC);
   for (int n = 0; n < lisPTC->Count; n++)
      {
      ptcA = (TPtoCarga*)lisPTC->Items[n];
      if (ExistePtoCarga(local.lisPTC, ptcA->X_cm, ptcA->Y_cm) == NULL)
         {//insere PtoCarga em local.lisPTC
         local.lisPTC->Add(ptcA);
         }
      }
   //reinicia lista com envoltoria
   local.lisENV->Clear();
   //busca PtoCarga inicial
   if ((ptcA = BuscaPtoCargaInicial()) == NULL) return(NULL);
   //transfere ptcA de local.lisPTC p/ local.lisENV
   local.lisENV->Add(ptcA);
   local.lisPTC->Remove(ptcA);
   //define o angulo inicial
   if ((ptcB = BuscaSeg(ptcA, NULL)) == NULL) return(NULL);
   //reinsere PtoCarga inicial
   local.lisPTC->Add(ptcA);
   //constrói a envoltória
   while(local.lisENV->IndexOf(ptcB) == -1)
      {//transfere ptcB de local.lisPTC p/ local.lisENV
      local.lisENV->Add(ptcB);
      local.lisPTC->Remove(ptcB);
      //determina próximo PtoCarga
      if ((ptc1 = BuscaSeg(ptcA, ptcB)) == NULL)
         {return(NULL);}
      ptcA = ptcB;
      ptcB = ptc1;
      }
   return(local.lisENV);
   }

//---------------------------------------------------------------------------
TPtoCarga* __fastcall TEnvoltoria::ExistePtoCarga(TList *lisPTC, int x_cm, int y_cm)
   {
   //variáveis locais
   TPtoCarga *ptc;

   //verifica se existe um PtoCarga c/ as coordenadas indicadas
   for (int n = 0; n < lisPTC->Count; n++)
      {
      ptc = (TPtoCarga*)lisPTC->Items[n];
      if ((ptc->X_cm == x_cm)&&(ptc->Y_cm == y_cm)) return(ptc);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof

