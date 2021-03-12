//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#pragma hdrstop
#include "TFasorLig.h"
#include "..\Arranjo\VTArranjo.h"
#include "..\Complexo\Complexo.h"
#include "..\Constante\Fases.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTTrafo.h"
#include "..\Rede\VTTrecho.h"
#include "..\Rede\VTResflowLig.h"
#include "..\RedeCC\ResCurto.h"
#include "..\RedeCC\VTLigCC.h"
#include "..\Zona\VTZona.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTFasor* __fastcall NewObjFasorFluxo(VTLigacao *ligacao, VTBarra *bar_ref)
   {
   try{//cria objeto TFasorLig
      return(new TFasorLig(ligacao, bar_ref));
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TFasorLig::TFasorLig(VTLigacao *ligacao, VTBarra *bar_ref)
   {
   //salva Ligacao
   this->ligacao = ligacao;
   //determina índice da Barra de referência
   ind_bar       = ligacao->IndiceBarra(bar_ref);
   //insere parâmetros default
   color        = clBlue;
   ind_pat      = 0;
   codigo       = "";
   enabled      = false;
   visible      = false;
   dim          = 0;
   raio         = 0;
   draw_seta    = true;
   fases        = faseINV;
   ind_seq      = -1;
   tipo_curto   = -1;
   coord.x      = 0.;
   coord.y      = 0.;
   corrente_amp = CZero();
   }

//---------------------------------------------------------------------------
__fastcall TFasorLig::~TFasorLig(void)
   {
   //nada a fazer
   }
   
//---------------------------------------------------------------------------
void __fastcall TFasorLig::AtualizaAngulo(double ang_novo)
   {
   //implementado somente para TFasorAjuste
   }

//---------------------------------------------------------------------------
void __fastcall TFasorLig::DefinePatamar(int ind_pat)
   {
   //salva patamar
   this->ind_pat = ind_pat;
   //inicia coordenadas de acordo c/tipo e unidade
   if      (fases != faseINV)                             IniciaFasorIfas();
   else if ((ind_seq >= indSEQ_0)&&(ind_seq <= indSEQ_2)) IniciaFasorIseq();
   }
/*
//---------------------------------------------------------------------------
double __fastcall TFasorLig::Ibase_amp(void)
   {
   //variáveis locais
   double i_base;
   VTLigCC *ligCC = (VTLigCC*)(ligacao->Obj);

   i_base  = ligacao->resflow->If_amp_max[ind_bar][ind_pat];
   i_base  = max(i_base, ligCC->Ifas_amp_max[defeito3F][ind_bar]);
   i_base  = max(i_base, ligCC->Ifas_amp_max[defeitoFT][ind_bar]);

   return(i_base);
   }
*/
//---------------------------------------------------------------------------
double __fastcall TFasorLig::Ibase_amp(void)
   {
   return(ligacao->resflow->If_amp_max[ind_bar][ind_pat]);
   }

//---------------------------------------------------------------------------
void __fastcall TFasorLig::IniciaFasorIfas(void)
   {
   //habilita exibição
   //enabled = true;
   //define código
   codigo  = "Fluxo: ";
   //define dimensão máxima
   dim  = Ibase_amp();
   //inicia coordenadas de acordo as fases
   switch(fases)
      {
      case faseA:
         codigo      += "Ia";
         corrente_amp = ligacao->resflow->Ia_amp[ind_bar][ind_pat];
         break;
      case faseB:
         codigo      += "Ib";
         corrente_amp = ligacao->resflow->Ib_amp[ind_bar][ind_pat];
         break;
      case faseC:
         codigo      += "Ic";
         corrente_amp = ligacao->resflow->Ic_amp[ind_bar][ind_pat];
         break;
      case faseN:
         codigo      += "In";
//         corrente_amp = ligacao->resflow->In_amp[ind_bar][ind_pat];
         corrente_amp = ligacao->resflow->Ia_amp[ind_bar][ind_pat] +
                        ligacao->resflow->Ib_amp[ind_bar][ind_pat] +
                        ligacao->resflow->Ic_amp[ind_bar][ind_pat];
         break;
      default: //desabilita exibição
         corrente_amp = CZero();
      }
   raio    = Abs(corrente_amp);
   coord.x = corrente_amp.real();
   coord.y = corrente_amp.imag();
   enabled = (raio > 0.);
   }

//---------------------------------------------------------------------------
void __fastcall TFasorLig::IniciaFasorIseq(void)
   {
   //variáveis locais
   complex<double> ia_amp, ib_amp, ic_amp;

   //habilita exibição
   //enabled = true;
   //determina correntes de fases
   ia_amp = ligacao->resflow->Ia_amp[ind_bar][ind_pat];
   ib_amp = ligacao->resflow->Ib_amp[ind_bar][ind_pat];
   ic_amp = ligacao->resflow->Ic_amp[ind_bar][ind_pat];;
   codigo  = "Fluxo: ";
   //define dimensão máxima
   dim  = Ibase_amp();
   //inicia coordenadas de acordo as fases
   switch(ind_seq)
      {
      case indSEQ_0:
         codigo      += "I0";
         corrente_amp = (ia_amp + ib_amp + ic_amp) / 3.;
         break;
      case indSEQ_1:
         codigo      += "I1";
         corrente_amp = (ia_amp + (Alfa() * ib_amp) + (Alfa2() * ic_amp)) / 3.;
         break;
      case indSEQ_2:
         codigo      += "I2";
         corrente_amp = (ia_amp + (Alfa2() * ib_amp) + (Alfa() * ic_amp)) / 3.;
         break;
      default: //desabilita exibição
         corrente_amp = CZero();
      }
   //verifica se valor nulo
   if (IsDoubleZero(Abs(corrente_amp))) corrente_amp = CZero();
   raio    = Abs(corrente_amp);
   coord.x = corrente_amp.real();
   coord.y = corrente_amp.imag();
   enabled = (raio > 0.);
   }

//---------------------------------------------------------------------------
void __fastcall TFasorLig::RedefineBarraReferencia(VTBarra *bar_ref)
   {
   //redefine índice da barra na Ligacao
   ind_bar = ligacao->IndiceBarra(bar_ref);
   //reinicia o fasor
   DefinePatamar(ind_pat);
   }

//---------------------------------------------------------------------------
int __fastcall TFasorLig::Tipo(void)
   {
   if (fases   != faseINV)                           return(fasorFLUXO_Ifas);
   if ((ind_seq >= indSEQ_0)&&(ind_seq <= indSEQ_2)) return(fasorFLUXO_Iseq);
   return(fasorINDEFINIDO);
   }

//---------------------------------------------------------------------------
bool __fastcall TFasorLig::Valor(complex<double> &valor, bool pu)
   {
   valor = corrente_amp;

   return(true);
   }

//---------------------------------------------------------------------------
//eof
