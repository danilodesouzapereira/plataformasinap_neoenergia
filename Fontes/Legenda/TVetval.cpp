//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <math.h>
#include "TVetval.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTVetval* __fastcall NewObjVetval(int ind_atrib, AnsiString descricao, int num_pat)
   {
   try{
      return (new TVetval(ind_atrib, descricao, num_pat));
      }catch (Exception &e)
         {
         }
   return(NULL);
   }

//---------------------------------------------------------------------------
__fastcall TVetval::TVetval(int ind_atrib, AnsiString descricao, int num_pat)
   {
   //salva dados
   this->ind_atrib = ind_atrib;
   this->descricao = descricao;
   //aloca vetor de valores
   PD.len     = num_pat;
   PD.valor   = new double[num_pat];
   PD.val_str = new AnsiString[num_pat];
   }

//---------------------------------------------------------------------------
__fastcall TVetval::~TVetval(void)
   {
   //destrói vetor alocado
   if (PD.valor)   {delete[] PD.valor;   PD.valor   = NULL;}
   if (PD.val_str) {delete[] PD.val_str; PD.val_str = NULL;}
   }
/*
//---------------------------------------------------------------------------
void __fastcall TVetval::AjustaUE_A(void)
   {
   //verifica se deve converter valores de A p/ kA
   if (Valor[0] < 1000.)
      {//mantém unidade A
      unidade = "A";
      }
   else
      {//converte A p/ kA
      unidade = "kA";
      for (int np = 0; np < Len; np++) Valor[np] = Valor[np] / 1000.;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TVetval::AjustaUE_KV(void)
   {
   //verifica se deve converter valores de kV p/ V
   if (Valor[0] > 0.5)
      {//mantém unidade kV
      unidade = "kV";
      }
   else
      {//converte kV p/ V
      unidade = "V";
      for (int np = 0; np < Len; np++) Valor[np] = Valor[np] * 1000.;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TVetval::AjustaUE_MVA(void)
   {
   //verifica se deve converter valores de MVA p/ kVA
   if (fabs(Valor[0]) > 0.5)
      {//mantém unidade MVA
      unidade = "MVA";
      }
   else
      {//converte MVA p/ kVA
      unidade = "kVA";
      for (int np = 0; np < Len; np++) Valor[np] = Valor[np] * 1000.;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TVetval::AjustaUE_MVAR(void)
   {
   if (fabs(Valor[0]) > 0.5)
      {//mantém unidade em MVAr
      unidade = "MVAr";
      }
   else
      {//converte MVAr p/ kVAr
      unidade = "kVAr";
      for (int np = 0; np < Len; np++) Valor[np] = Valor[np] * 1000.;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TVetval::AjustaUE_MW(void)
   {
   if (fabs(Valor[0]) > 0.5)
      {//mantém unidade em MW
      unidade = "MW";
      }
   else
      {//converte MW p/ kW
      unidade = "kW";
      for (int np = 0; np < Len; np++) Valor[np] = Valor[np] * 1000.;
      }
   }
*/
//---------------------------------------------------------------------------
AnsiString __fastcall TVetval::DescUnid(void)
   {
   if (unidade.IsEmpty()) return(descricao);
   return(descricao + "(" + unidade + ")");
   }

//---------------------------------------------------------------------------
double __fastcall TVetval::PM_GetValor(int ind)
   {
   return(PD.valor[ind]);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TVetval::PM_GetValorAsStr(int ind)
   {
   return(PD.val_str[ind]);
   }

//---------------------------------------------------------------------------
void __fastcall TVetval::PM_SetValorAsStr(int ind, AnsiString val_str)
   {
   PD.val_str[ind] = val_str;
   }


//---------------------------------------------------------------------------
void __fastcall TVetval::PM_SetValor(int ind, double valor)
   {
   PD.valor[ind] = valor;
   }

//---------------------------------------------------------------------------
//eof
