//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#include <algorithm>
#pragma hdrstop
#include "TFasorLigCC.h"
#include "..\Complexo\Complexo.h"
#include "..\Constante\Fases.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTResflowLig.h"
#include "..\RedeCC\ResCurto.h"
#include "..\RedeCC\VTLigCC.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTFasor* __fastcall NewObjFasorCurto(VTLigacao *ligacao, VTBarra *bar_ref)
   {
   try{//cria objeto TFasorLigCC
      return(new TFasorLigCC(ligacao, bar_ref));
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TFasorLigCC::TFasorLigCC(VTLigacao *ligacao, VTBarra *bar_ref)
   {
   //salva Ligacao
   this->ligacao = ligacao;
   //determina índice da Barra de referência
   ind_bar       = ligacao->IndiceBarra(bar_ref);
   //insere parâmetros default
   color        = clRed;
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
__fastcall TFasorLigCC::~TFasorLigCC(void)
   {
   //nada a fazer
   }
   
//---------------------------------------------------------------------------
void __fastcall TFasorLigCC::AtualizaAngulo(double ang_novo)
   {
   //implementado somente para TFasorAjuste
   }

//---------------------------------------------------------------------------
void __fastcall TFasorLigCC::DefinePatamar(int ind_pat)
   {
   //salva patamar
   this->ind_pat = ind_pat;
   //inicia coordenadas de acordo c/tipo e unidade
   if      (fases != faseINV)                             IniciaFasorIfas();
   else if ((ind_seq >= indSEQ_0)&&(ind_seq <= indSEQ_2)) IniciaFasorIseq();
   }
/*
//---------------------------------------------------------------------------
double __fastcall TFasorLigCC::Ibase_amp(void)
   {
   //variáveis locais
   double i_base;

   i_base  = ligCC->Ligacao->resflow->If_amp_max[ind_bar][ind_pat];
   i_base  = max(i_base, ligCC->Ifas_amp_max[defeito3F][ind_bar]);
   i_base  = max(i_base, ligCC->Ifas_amp_max[defeitoFT][ind_bar]);

   return(i_base);
   }
*/

//---------------------------------------------------------------------------
double __fastcall TFasorLigCC::Ibase_amp(void)
   {
   //variáveis locais
   double   i_base;
   VTLigCC *ligCC = (VTLigCC*)(ligacao->Obj);

   i_base  = ligCC->Ifas_amp_max[defeito3F][ind_bar];
   i_base  = max(i_base, ligCC->Ifas_amp_max[defeitoFT][ind_bar]);

   return(i_base);
   }

//---------------------------------------------------------------------------
void __fastcall TFasorLigCC::IniciaFasorIfas(void)
   {
   //variáveis locais
   VTLigCC *ligCC = (VTLigCC*)(ligacao->Obj);

   //habilita exibição
   //enabled = true;
   //define código do Fasor
   codigo = NomeDefeito(tipo_curto) + ": ";
   //define dimensão máxima
   dim  = Ibase_amp();
   switch(fases)
      {
      case faseA:
         codigo       = codigo + "Ia";
         corrente_amp = ligCC->Ifas_amp[tipo_curto][ind_bar][indFASE_A];
         break;
      case faseB:
         codigo       = codigo + "Ib";
         corrente_amp = ligCC->Ifas_amp[tipo_curto][ind_bar][indFASE_B];
         break;
      case faseC:
         codigo       = codigo + "Ic";
         corrente_amp = ligCC->Ifas_amp[tipo_curto][ind_bar][indFASE_C];
         break;
      case faseN:
         codigo       = codigo + "In";
         corrente_amp = ligCC->Ifas_amp[tipo_curto][ind_bar][indFASE_A] +
                        ligCC->Ifas_amp[tipo_curto][ind_bar][indFASE_B] +
                        ligCC->Ifas_amp[tipo_curto][ind_bar][indFASE_C];
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
void __fastcall TFasorLigCC::IniciaFasorIseq(void)
   {
   //variáveis locais
   VTLigCC *ligCC = (VTLigCC*)(ligacao->Obj);

   //habilita exibição
   //enabled = true;
   //define código do Fasor
   codigo = NomeDefeito(tipo_curto) + ": ";
   //define dimensão máxima
   dim          = Ibase_amp();
   corrente_amp = ligCC->Iseq_amp[tipo_curto][ind_bar][ind_seq];
   raio         = Abs(corrente_amp);
   coord.x      = corrente_amp.real();
   coord.y      = corrente_amp.imag();
   enabled      = (raio > 0.);
   switch(ind_seq)
      {
      case indSEQ_0:  codigo  = codigo + "I0"; break;
      case indSEQ_1:  codigo  = codigo + "I1"; break;
      case indSEQ_2:  codigo  = codigo + "I2"; break;
      default: //desabilita exibição
         corrente_amp = CZero();
         enabled      = false;
      }
   }

//-----------------------------------------------------------------------------
AnsiString __fastcall TFasorLigCC::NomeDefeito(int ind_def)
   {
   //variáveis locais
   char *tipo_def[] = {"3F", "2F", "1FT", "1FTz", "2FT"};

   return(AnsiString("Curto ") + tipo_def[ind_def]);
   }

//---------------------------------------------------------------------------
void __fastcall TFasorLigCC::RedefineBarraReferencia(VTBarra *bar_ref)
   {
   //redefine índice da barra na Ligacao
   ind_bar = ligacao->IndiceBarra(bar_ref);
   //reinicia o fasor
   DefinePatamar(ind_pat);
   }

//---------------------------------------------------------------------------
int __fastcall TFasorLigCC::Tipo(void)
   {
   if (fases   != faseINV)                           return(fasorCURTO_Ifas);
   if ((ind_seq >= indSEQ_0)&&(ind_seq <= indSEQ_2)) return(fasorCURTO_Iseq);
   return(fasorINDEFINIDO);
   }

//---------------------------------------------------------------------------
bool __fastcall TFasorLigCC::Valor(complex<double> &valor, bool pu)
   {
   valor = corrente_amp;

   return(true);
   }

//---------------------------------------------------------------------------
//eof
