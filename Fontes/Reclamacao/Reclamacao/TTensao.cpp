//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TTensao.h"
#include <Fontes\Complexo\Complexo.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTTensao*  __fastcall NewObjTensao(int num_pat)
   {
   try{
      return(new TTensao(num_pat));
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TTensao::TTensao(int num_pat)
   {
   //salva dimens�o
   PD.num_pat = num_pat;
   //cria e inicia o vetor de tens�es com dimens�o num_pat
   tensao = new strTENSAO[num_pat];
   for (int ind_pat = 0; ind_pat < num_pat; ind_pat++)
      {
      tensao[ind_pat].enabled = false;
      for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++)
         {
         tensao[ind_pat].vfn_kv[ind_fase] = CZero();
         }
      }
   }

//---------------------------------------------------------------------------
__fastcall TTensao::~TTensao(void)
   {
   //destr�i objetos
   delete[] tensao;
   }

//---------------------------------------------------------------------------
bool  __fastcall TTensao::InsereTensao(int ind_pat, double vfn_kv[MAX_FASE])
   {
   //prote��o
   if ((ind_pat < 0)||(ind_pat >= PD.num_pat)) return(false);
   //salva valores das tens�es de fase
   tensao[ind_pat].enabled = true;
   for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++)
      {
      tensao[ind_pat].vfn_kv[ind_fase] = complex<double>(vfn_kv[ind_fase], 0.);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool  __fastcall TTensao::InsereTensao(int ind_pat, complex<double>vfn_kv[MAX_FASE])
   {
   //prote��o
   if ((ind_pat < 0)||(ind_pat >= PD.num_pat)) return(false);
   //salva valores das tens�es de fase
   tensao[ind_pat].enabled = true;
   for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++)
      {
      tensao[ind_pat].vfn_kv[ind_fase] = vfn_kv[ind_fase];
      }
   return(true);
   }

//---------------------------------------------------------------------------
TDateTime __fastcall TTensao::PM_GetData(void)
   {
   return(TDateTime(PD.ano, PD.mes, PD.dia));
   //return(TDateTime(PD.ano, PD.mes, PD.dia, PD.hora, PD.min, 0, 0));  //*
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TTensao::PM_GetDataAsString(void)
   {
   //vari�veis locais
   AnsiString txt;

   return(txt.sprintf("%02d/%02d/%4d", Dia, Mes, Ano));
   }

//---------------------------------------------------------------------------
bool __fastcall TTensao::PM_GetEnabled(int ind_pat)
   {
   //prote��o
   if ((ind_pat < 0)||(ind_pat >= PD.num_pat)) return(false);
   //retorna se o patamar est� habilitado
   return(tensao[ind_pat].enabled);
   }

//---------------------------------------------------------------------------
double __fastcall TTensao::PM_GetModVfn_kv(int ind_pat, int ind_fase)
   {
   //prote��o
   if ((ind_pat < 0)||(ind_pat >= PD.num_pat)) return(false);
   //retorna m�dulo da tens�o da fase no patamar
   return(Abs(tensao[ind_pat].vfn_kv[ind_fase]));
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TTensao::PM_GetVfn_kv(int ind_pat, int ind_fase)
   {
   //prote��o
   if ((ind_pat < 0)||(ind_pat >= PD.num_pat)) return(false);
   //retorna tens�o da fase no patamar
   return(tensao[ind_pat].vfn_kv[ind_fase]);
   }

//---------------------------------------------------------------------------
//eof

