//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TCarreg.h"
#include <Fontes\Complexo\Complexo.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTCarreg*  __fastcall NewObjCarreg(int num_pat)
   {
   try{
      return(new TCarreg(num_pat));
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TCarreg::TCarreg(int num_pat)
   {
   //salva dimensão
   PD.num_pat = num_pat;
   //cria e inicia o vetor de carregamento com dimensão num_pat
   carreg = new strCARREG[num_pat];
   for (int ind_pat = 0; ind_pat < num_pat; ind_pat++)
      {
      carreg[ind_pat].enabled = false;
      carreg[ind_pat].Carreg = 0.0;
      for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++)
         {
         carreg[ind_pat].i_antes[ind_fase] = 0.0;
         carreg[ind_pat].i_depois[ind_fase] = 0.0;
         }
      }
   }

//---------------------------------------------------------------------------
__fastcall TCarreg::~TCarreg(void)
   {
   //destrói objetos
   delete[] carreg;
   }
                                                          
//---------------------------------------------------------------------------
bool  __fastcall TCarreg::InsereCarreg(int ind_pat, double Carreg)
   {
   //proteção
   if ((ind_pat < 0)||(ind_pat >= PD.num_pat)) return(false);
   //salva valores dos carregamentos
   carreg[ind_pat].enabled = true;
   carreg[ind_pat].Carreg = Carreg;
   return(true);
   }

//---------------------------------------------------------------------------
bool  __fastcall TCarreg::InsereCorrenteAntes(int ind_pat, double I_A[MAX_FASE])
   {
   //proteção
   if ((ind_pat < 0)||(ind_pat >= PD.num_pat)) return(false);
   //salva valores das correntes de fase
   carreg[ind_pat].enabled = true;
   for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++)
      {
      carreg[ind_pat].i_antes[ind_fase] = I_A[ind_fase];
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool  __fastcall TCarreg::InsereCorrenteDepois(int ind_pat, double I_A[MAX_FASE])
   {
   //proteção
   if ((ind_pat < 0)||(ind_pat >= PD.num_pat)) return(false);
   //salva valores das correntes de fase
   carreg[ind_pat].enabled = true;
   for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++)
      {
      carreg[ind_pat].i_depois[ind_fase] = I_A[ind_fase];
      }
   return(true);
   }
//---------------------------------------------------------------------------
double __fastcall TCarreg::PM_GetCarreg(int ind_pat)
   {
   //proteção
   if ((ind_pat < 0)||(ind_pat >= PD.num_pat)) return(false);
   //retorna carregamento no patamar
   return(carreg[ind_pat].Carreg);
   }

//---------------------------------------------------------------------------
TDateTime __fastcall TCarreg::PM_GetData(void)
   {
   return(TDateTime(PD.ano, PD.mes, PD.dia));
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TCarreg::PM_GetDataAsString(void)
   {
   //variáveis locais
   AnsiString txt;

   return(txt.sprintf("%02d/%02d/%4d", Dia, Mes, Ano));
   }

//---------------------------------------------------------------------------
bool __fastcall TCarreg::PM_GetEnabled(int ind_pat)
   {
   //proteção
   if ((ind_pat < 0)||(ind_pat >= PD.num_pat)) return(false);
   //retorna se o patamar está habilitado
   return(carreg[ind_pat].enabled);
   }

//---------------------------------------------------------------------------
double __fastcall TCarreg::PM_GetModI_Antes(int ind_pat, int ind_fase)
   {
   //proteção
   if ((ind_pat < 0)||(ind_pat >= PD.num_pat)) return(false);
   //retorna módulo da corrente da fase no patamar
   return(carreg[ind_pat].i_antes[ind_fase]);
   }

//---------------------------------------------------------------------------
double __fastcall TCarreg::PM_GetModI_Depois(int ind_pat, int ind_fase)
   {
   //proteção
   if ((ind_pat < 0)||(ind_pat >= PD.num_pat)) return(false);
   //retorna módulo da corrente da fase no patamar
   return(carreg[ind_pat].i_depois[ind_fase]);
   }

//---------------------------------------------------------------------------
//eof

