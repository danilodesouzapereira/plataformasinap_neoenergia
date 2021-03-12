//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#pragma hdrstop
#include "TFasorAngulo.h"
#include "..\Complexo\Complexo.h"
#include "..\Constante\Fases.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTResflowBar.h"
#include "..\Zona\VTZona.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTFasor* __fastcall NewObjFasorAngulo(VTFasor *fasor1, VTFasor *fasor2)
   {
   try{//cria objeto TFasorAngulo
      return(new TFasorAngulo(fasor1, fasor2));
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TFasorAngulo::TFasorAngulo(VTFasor *fasor1, VTFasor *fasor2)
   {
   //salva ponteiros
   this->fasor1   = fasor1;
   this->fasor2   = fasor2;
   //insere parâmetros default
   color        = clGray;
   ind_pat      = 0;
   codigo       = "";
   enabled      = false;
   visible      = false;
   dim           = 0;
   raio          = 0;
   draw_seta     = false;
   fases         = faseINV;
   ind_seq       = -1;
   tipo_curto    = -1;
   coord.x       = 0.;
   coord.y       = 0.;
   //inicia Fasor p/ patamar
//   DefinePatamar(0);
   }

//---------------------------------------------------------------------------
__fastcall TFasorAngulo::~TFasorAngulo(void)
   {
   //nada a fazer
   }
   
//---------------------------------------------------------------------------
void __fastcall TFasorAngulo::AtualizaAngulo(double ang_novo)
   {
   //implementado somente para TFasorAjuste
   }

//---------------------------------------------------------------------------
void __fastcall TFasorAngulo::DefinePatamar(int ind_pat)
   {
   //salva patamar
   this->ind_pat = ind_pat;
   //inicia coordenadas de acordo c/tipo e unidade
   IniciaFasor();
   }

//---------------------------------------------------------------------------
void __fastcall TFasorAngulo::IniciaFasor(void)
   {
   //variáveis locais
   double ang_rad;

   //determina ângulo entre os dois fasores
   if (! ValorAngulo(ang_rad)) return;
   codigo.sprintf("Ang(%s)-Ang(%s)", fasor1->codigo.c_str(), fasor2->codigo.c_str());
   //define dimensão máxima
   dim       = 100;
   raio      = dim;
   //define coordenadas em função do angulo
   coord.x   = dim * cos(ang_rad);
   coord.y   = dim * sin(ang_rad);
   //habilita o fasor somente se os dois fasores estiverem habilitados
   enabled   = (fasor1->enabled && fasor2->enabled);
   }

//---------------------------------------------------------------------------
void __fastcall TFasorAngulo::RedefineBarraReferencia(VTBarra *barra)
   {
   //reinicia o Fasor, pois fasor1 e fasor2 podem ter sido alterados
   IniciaFasor();
   }

//---------------------------------------------------------------------------
int __fastcall TFasorAngulo::Tipo(void)
   {
   return(fasorANGULO);
   }

//---------------------------------------------------------------------------
bool __fastcall TFasorAngulo::Valor(complex<double> &valor, bool pu)
   {
   //variáveis locais
   double ang_rad;

   if (! ValorAngulo(ang_rad)) return(false);
   valor = polar(raio, ang_rad);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFasorAngulo::ValorAngulo(double &ang_rad)
   {
   //variáveis locais
   complex<double> valor1, valor2;

   //determina valores dos dois fasores
   if (! fasor1->Valor(valor1)) return(false);
   if (! fasor2->Valor(valor2)) return(false);
   //determina ângulo entre os dois fasores
   ang_rad = Arg(valor1) - Arg(valor2);
   return(true);
   }

//---------------------------------------------------------------------------
//eof
