//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#include <Math.hpp>
#pragma hdrstop
#include "TFasorAjuste.h"
#include "..\Complexo\Complexo.h"
#include "..\Constante\Fases.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTResflowBar.h"
#include "..\Zona\VTZona.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTFasor* __fastcall NewObjFasorAjuste(VTFasor *fasorI, VTFasor *fasorV, char classe)
   {
   try{//cria objeto TFasorAjuste
      return(new TFasorAjuste(fasorI, fasorV, classe));
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TFasorAjuste::TFasorAjuste(VTFasor *fasorI, VTFasor *fasorV, char classe)
   {
   //salva dados
   this->fasorI   = fasorI;
   this->fasorV   = fasorV;
   this->classe   = classe;
   //insere parâmetros default
   ang_ajuste_rad = 0.;
   color          = clGreen;
   ind_pat        = 0;
   codigo         = "";
   enabled        = false;
   visible        = false;
   dim            = 0;
   raio           = 0;
   draw_seta      = false;
   fases          = faseINV;
   //guarda ind_seq, para o caso seq0
   ind_seq        = fasorV->ind_seq;
   tipo_curto     = -1;
   coord.x        = 0.;
   coord.y        = 0.;
   //inicia Fasor p/ patamar
//   DefinePatamar(0);
   }

//---------------------------------------------------------------------------
__fastcall TFasorAjuste::~TFasorAjuste(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
char __fastcall TFasorAjuste::Classe(void)
   {
   return(classe);
   }

//---------------------------------------------------------------------------
void __fastcall TFasorAjuste::DefineAnguloAjuste_rad(double ang_rad)
   {
   //salva angulo
   ang_ajuste_rad = ang_rad;
   }

//---------------------------------------------------------------------------
void __fastcall TFasorAjuste::DefinePatamar(int ind_pat)
   {
   //salva patamar
   this->ind_pat = ind_pat;
   //inicia coordenadas de acordo c/tipo e unidade
   IniciaFasor();
   }

//---------------------------------------------------------------------------
void __fastcall TFasorAjuste::IniciaFasor(void)
   {
   //variáveis locais
   double ang_rad;

   //determina ângulo
   if (! ValorAngulo(ang_rad)) return;
   codigo.sprintf("Ajuste%c: (%s) (%s)", classe, fasorI->codigo.c_str(), fasorV->codigo.c_str());
   //define dimensão máxima
   dim       = 100;
   raio      = dim;
   //define coordenadas em função do angulo
   coord.x   = dim * cos(ang_rad);
   coord.y   = dim * sin(ang_rad);
   //habilita o fasor somente se o fasor de tensão estiver habilitado
   enabled   = (fasorV->enabled);
   }

//---------------------------------------------------------------------------
void __fastcall TFasorAjuste::RedefineBarraReferencia(VTBarra *barra)
   {
   //reinicia o Fasor, pois fasorI e fasorV podem ter sido alterados
   IniciaFasor();
   }

//---------------------------------------------------------------------------
int __fastcall TFasorAjuste::Tipo(void)
   {
   return(fasorAJUSTE);
   }

//---------------------------------------------------------------------------
bool __fastcall TFasorAjuste::Valor(complex<double> &valor, bool pu)
   {
   //variáveis locais
   double ang_rad;

   if (! ValorAngulo(ang_rad)) return(false);
   valor = polar(raio, ang_rad);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFasorAjuste::ValorAngulo(double &ang_rad)
   {
   //variáveis locais
   complex<double> valor;

   //determina valor do fasor de tensão
   if (! fasorV->Valor(valor)) return(false);
   //determina ângulo resultante
   ang_rad = Arg(valor) + DegToRad(ang_ajuste_rad);
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFasorAjuste::ValorAnguloIni_rad(double &ang_rad)
   {
   //determina valor do ângulo
   if (! ValorAngulo(ang_rad)) return(false);
   //subtrai subtrai noventa graus
   ang_rad -= (M_PI / 2.);
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFasorAjuste::ValorAnguloFim_rad(double &ang_rad)
   {
   //determina valor do ângulo
   if (! ValorAngulo(ang_rad)) return(false);
   //soma noventa graus
   ang_rad += (M_PI / 2.);
   return(true);
   }

//---------------------------------------------------------------------------
//eof
