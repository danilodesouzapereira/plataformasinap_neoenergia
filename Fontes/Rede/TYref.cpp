//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#pragma hdrstop
#include "TYref.h"
#include "VTBarra.h"
#include "VTFases.h"
#include "VTResFlowBar.h"
#include "..\Complexo\Complexo.h"
#include "..\Constante\Trifasico.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTYref* __fastcall NewObjYref(void)
   {
   return(new TYref());
   }
   
//---------------------------------------------------------------------------
__fastcall TYref::TYref(void)
   {
   Show[yref_Y_PU100] = true;
   }

//---------------------------------------------------------------------------
__fastcall TYref::~TYref(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
VTYref* __fastcall TYref::Clone(void)
   {
   //variáveis locais
   VTYref *yref;

   //cria uma nova Yref
   if ((yref = NewObjYref()) != NULL)
      {//copia dados desta Yref p/ a nova Yref
      yref->CopiaAtributosDe(*this);
      }
   return(yref);
   }

//---------------------------------------------------------------------------
void __fastcall TYref::CopiaAtributosDe(VTEqpto &ref)
   {
   //variáveis locais
   TYref &yref = (TYref&)ref;

   //proteção
   if (ref.Tipo() != eqptoYREF) return;
   //copia atributos de TEqbar
   TEqbar::CopiaAtributosDe(yref);
   //copia atributos de TYref
   PD = yref.PD;
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TYref::DemandaMVA(int num_pat)
   {
   //variáveis locais
   complex<double> y_s;
   complex<double> i_ka, vfn_kv;
   complex<double> s_mva = CZero();
   double Ybase;
   double Sbase=100.;
   double Vbase=pbarra->vnom;

   //proteção
   if ((Sbase == 0.)||(Vbase == 0.)) return(s_mva);
   //calcula Zbase em ohm
   Ybase = Sbase/(Vbase * Vbase);
   //calcula admitância de fase em siemens
   y_s = Y1_pu * Ybase;
   //totaliza potência nas 3 fases (supondo ligação Yisolado)
   for (int nf = 0; nf < MAX_FASE-1; nf++)
      {//obtém tensão de fase/terra em kV
      vfn_kv = pbarra->resflow->Vfn_kv[num_pat][nf];
      //calcula corrente em kA: I = (vf_kv * y_s)
      i_ka = vfn_kv * y_s;
      //acumula potência da fase em MVA
      s_mva += vfn_kv * conj(i_ka);
      }
   return(s_mva);
   }

//---------------------------------------------------------------------------
//FUNÇÃO TEMPORÁRIA ATÉ ALTERAR BASE DE DADOS
//---------------------------------------------------------------------------
void __fastcall TYref::Inicia(double r_pu, double x_pu)
   {
   //variáveis locais
   complex<float> z_pu = complex<float>(r_pu, x_pu);

   //inicia PD.y1_pu
   if (! Inverte(z_pu, PD.y1_pu)) PD.y1_pu = CZero();
   //inicia PD.y0_pu igual a PD.y1_pu
   PD.y0_pu = PD.y1_pu;
   //inicia PD.y_s com PD.y1_s na diagonal e 0 fora da diagnoal
   for (int i = 0; i < MAX_FASE; i++)
      {
      for (int j = 0; j < MAX_FASE; j++)
         {
         if (i == j) PD.y_pu[i][j] = PD.y1_pu;
         else        PD.y_pu[i][j] = CZero();
         }
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TYref::Inverte(complex<float> &valor, complex<float> &inverso)
   {
   //verifica se a é nulo
   if (IsDoubleZero(Abs(valor))) return(false);
   inverso = complex<float>(1., 0) / valor;

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TYref::PM_GetReducao(void)
   {
   return(PD.reducao);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TYref::PM_GetY0_pu(void)
   {
   return(PD.y0_pu);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TYref::PM_GetY1_pu(void)
   {
   return(PD.y1_pu);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TYref::PM_GetY_pu(int i, int j)
   {
   return(PD.y_pu[i][j]);
   }

//---------------------------------------------------------------------------
void __fastcall TYref::PM_SetReducao(bool reducao)
   {
   PD.reducao = reducao;
   }

//---------------------------------------------------------------------------
void __fastcall TYref::PM_SetY0_pu(complex<double> y0)
   {
   PD.y0_pu = y0;
   }

//---------------------------------------------------------------------------
void __fastcall TYref::PM_SetY1_pu(complex<double> y1)
   {
   PD.y1_pu = y1;
   }

//---------------------------------------------------------------------------
void __fastcall TYref::PM_SetY_pu(int i, int j, complex<double> y_ij)
   {
   PD.y_pu[i][j] = y_ij;
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TYref::Y0_s(double Sbase, double Vbase)
   {
   //variáveis locais
   double          Ybase;
   complex<double> y0_s;

   //proteção
   if ((Sbase == 0.)||(Vbase == 0.)) return(CZero());
   //calcula Ybase em siemen
   Ybase = Sbase / (Vbase * Vbase);
   //calcula y0 em pu
   //y0_s = PD.y0_pu * Ybase;
   y0_s  = PD.y0_pu;
   y0_s *=  Ybase;

   return(y0_s);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TYref::Y1_s(double Sbase, double Vbase)
   {
   //variáveis locais
   double          Ybase;
   complex<double> y1_s;

   //proteção
   if ((Sbase == 0.)||(Vbase == 0.)) return(CZero());
   //calcula Ybase em siemen
   Ybase = Sbase / (Vbase * Vbase);
   //calcula y1 em pu
   //y1_s = PD.y1_pu * Ybase;
   y1_s  = PD.y1_pu;
   y1_s *= Ybase;

   return(y1_s);
   }
/*
//---------------------------------------------------------------------------
void __fastcall TYref::Z_ohm(strIMP &z_ohm)
   {
   //variáveis locais
   complex<double> z1;

   //calcula z1 em ohm
   if (! Inverte(PD.y1_s, z1)) z1 = CZero();
   //atualiza z_ohm
   z_ohm.r = z1.real();
   z_ohm.x = z1.imag();
   }

//---------------------------------------------------------------------------
void __fastcall TYref::Z0_pu(double Sbase, double Vbase, strIMP &z0_pu)
   {
   //variáveis locais
   complex<double> y0, z0;

   //determina y0 em pu
   y0 = Y0_pu(Sbase, Vbase);
   //determina z0 em pu
   if (! Inverte(y0, z0)) z0 = CZero();
   //atualiza z0_pu
   z0_pu.r = z0.real();
   z0_pu.x = z0.imag();
   }

//---------------------------------------------------------------------------
void __fastcall TYref::Z1_pu(double Sbase, double Vbase, strIMP &z1_pu)
   {
   //variáveis locais
   complex<double> y1, z1;

   //determina y1 em pu
   y1 = Y1_pu(Sbase, Vbase);
   //determina z1 em pu
   if (! Inverte(y1, z1)) z1 = CZero();
   //atualiza z1_pu
   z1_pu.r = z1.real();
   z1_pu.x = z1.imag();
   }
*/

//---------------------------------------------------------------------------
void __fastcall TYref::Z0_pu(strIMP &z0_pu)
   {
   //variáveis locais
   complex<float> y0, z0;

   //determina y0 em pu
   y0 = Y0_pu;
   //determina z0 em pu
   if (! Inverte(y0, z0)) z0 = CZero();
   //atualiza z0_pu
   z0_pu.r = z0.real();
   z0_pu.x = z0.imag();
   }

//---------------------------------------------------------------------------
void __fastcall TYref::Z1_pu(strIMP &z1_pu)
   {
   //variáveis locais
   complex<float> y1, z1;

   //determina y1 em pu
   y1 = Y1_pu;
   //determina z1 em pu
   if (! Inverte(y1, z1)) z1 = CZero();
   //atualiza z1_pu
   z1_pu.r = z1.real();
   z1_pu.x = z1.imag();
   }

//---------------------------------------------------------------------------
int __fastcall TYref::Tipo(void)
   {
   return(eqptoYREF);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TYref::TipoAsString(void)
   {
   return("Yref");
   }

//---------------------------------------------------------------------------
//eof
