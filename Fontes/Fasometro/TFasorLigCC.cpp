//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#pragma hdrstop
#include "TFasorLigCC.h"
#include "..\Complexo\Complexo.h"
#include "..\Rede\VTTrafo.h"
#include "..\RedeCC\VTLigCC.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTFasor* __fastcall NewObjFasor(VTLigCC *ligCC, TColor color, int atr, int ind_bar, bool visible)
   {
   try{//cria objeto TFasorLigCC
      return(new TFasorLigCC(ligCC, color, atr, ind_bar, visible));
      }catch(Exception &e)
         {
         return(NULL);
         }
   }
/*
//---------------------------------------------------------------------------
VTFasorLigCC* __fastcall NewObjFasorLigCCVff(VTEqpto *eqpto, TColor color, int atr, int ind_bar, bool visible)
   {
   try{//cria objeto TFasorLigCC
      return(new TFasorLigCC(eqpto, color, atr, ind_bar, visible));
      }catch(Exception &e)
         {
         return(NULL);
         }
   }
*/
//---------------------------------------------------------------------------
__fastcall TFasorLigCC::TFasorLigCC(VTLigCC *ligCC, TColor color, int atr, int ind_bar, bool visible)
   {
   //salva parâmetros
   this->ligCC   = ligCC;
   this->color   = color;
   this->atr     = atr;
   this->ind_bar = ind_bar;
   this->visible = visible;
   //inicia FasorLigCC p/ primeiro patamar
   DefinePatamar(0);
   }

//---------------------------------------------------------------------------
__fastcall TFasorLigCC::~TFasorLigCC(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
void __fastcall TFasorLigCC::DefinePatamar(int ind_pat)
   {
   //salva patamar
   this->ind_pat = ind_pat;
   //inicia coordenadas de acordo c/tipo do Eqpto
   IniciaFasorLigCC_I();
   }

//---------------------------------------------------------------------------
//void __fastcall TFasorLigCC::IniciaFasorLigCC_I(void)
//   {
//   //variáveis locais
//   int        ind_def = atr;
//   AnsiString pri_sec = "";
//   VTTrafo    *trafo;
//
//   //define código do Fasor
//   codigo = NomeDefeito(ind_def) + ": ";
//   if (ligCC->Ligacao->Tipo() == eqptoTRAFO)
//      {
//      trafo = (VTTrafo*)ligCC->Ligacao;
//      if (trafo->IndiceBarra(trafo->BarraPrimario()) == ind_bar)
//         {codigo = "Ipri(kA)";}
//      else
//         {codigo = "Isec(kA)";}
//      }
//   else
//      {
//      codigo += "I(kA)";
//      }
//   //define extensão p/ o código de cada fase
//   pri_sec.sprintf("%d:", ind_bar+1);
//   //define dimensões
//   dim       = raio = ligCC->Ifas_amp_max[ind_def][ind_bar];
//   orig.x    = orig.y = 0.;
//   a.codigo  = pri_sec + "Ia";
//   a.x       = ligCC->Ifas_amp[ind_def][ind_bar][0].real();
//   a.y       = ligCC->Ifas_amp[ind_def][ind_bar][0].imag();
//   a.enabled = (fabs(a.x) + fabs(a.y)) > 0.;
//   b.codigo  = pri_sec + "Ib";
//   b.x       = ligCC->Ifas_amp[ind_def][ind_bar][1].real();
//   b.y       = ligCC->Ifas_amp[ind_def][ind_bar][1].imag();
//   b.enabled = (fabs(b.x) + fabs(b.y)) > 0.;
//   c.codigo  = pri_sec + "Ic";
//   c.x       = ligCC->Ifas_amp[ind_def][ind_bar][2].real();
//   c.y       = ligCC->Ifas_amp[ind_def][ind_bar][2].imag();
//   c.enabled = (fabs(c.x) + fabs(c.y)) > 0.;
//   }
//-----------------------------------------------------------------------------
void __fastcall TFasorLigCC::IniciaFasorLigCC_I(void) //FKM 2015.12.02
   {
   //variáveis locais
   int        ind_def = atr;
   AnsiString pri_sec = "";
   VTTrafo    *trafo;

   //define código do Fasor
   codigo = NomeDefeito(ind_def) + ": ";
   if (ligCC->Ligacao->Tipo() == eqptoTRAFO)
      {
      trafo = (VTTrafo*)ligCC->Ligacao;
      if (trafo->IndiceBarra(trafo->BarraPrimario()) == ind_bar)
         {codigo = "Ipri(kA)";}
      else
         {codigo = "Isec(kA)";}
      }
   else
      {
      codigo += "I(kA)";
      }
   //define extensão p/ o código de cada fase
   pri_sec.sprintf("%d:", ind_bar+1);    /* TODO : O q é isso?? */
   //define dimensões
   dim       = raio = ligCC->Ifas_amp_max[ind_def][ind_bar];
   orig.x    = orig.y = 0.;
   a.codigo  = NomeDefeito(ind_def) + ": " + pri_sec + "Ia";
   a.x       = ligCC->Ifas_amp[ind_def][ind_bar][0].real();
   a.y       = ligCC->Ifas_amp[ind_def][ind_bar][0].imag();
   a.enabled = (fabs(a.x) + fabs(a.y)) > 0.;
   b.codigo  = NomeDefeito(ind_def) + ": " + pri_sec + "Ib";
   b.x       = ligCC->Ifas_amp[ind_def][ind_bar][1].real();
   b.y       = ligCC->Ifas_amp[ind_def][ind_bar][1].imag();
   b.enabled = (fabs(b.x) + fabs(b.y)) > 0.;
   c.codigo  = NomeDefeito(ind_def) + ": " + pri_sec + "Ic";
   c.x       = ligCC->Ifas_amp[ind_def][ind_bar][2].real();
   c.y       = ligCC->Ifas_amp[ind_def][ind_bar][2].imag();
   c.enabled = (fabs(c.x) + fabs(c.y)) > 0.;
   }
//-----------------------------------------------------------------------------
AnsiString __fastcall TFasorLigCC::NomeDefeito(int ind_def)
   {
   //variáveis locais
   char *tipo_def[] = {"3F(abc)", "2F(bc)", "1FT(at)", "1FTz(at)", "2FT(bct)"};

   return(tipo_def[ind_def]);
   }

//---------------------------------------------------------------------------
bool __fastcall TFasorLigCC::PM_GetTipoCurto(void)
   {
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFasorLigCC::PM_GetTipoFluxo(void)
   {
   return(false);
   }

//---------------------------------------------------------------------------
//eof
