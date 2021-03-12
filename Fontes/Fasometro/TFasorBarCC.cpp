//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#pragma hdrstop
#include "TFasorBarCC.h"
#include "..\Complexo\Complexo.h"
//#include "..\Rede\VTBarra.h"
//#include "..\RedeCC\ResCurto.h"
#include "..\RedeCC\VTBarCC.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTFasor* __fastcall NewObjFasor(VTBarCC *barCC, TColor color, int atr, int ind_bar, bool visible)
   {
   try{//cria objeto TFasorBarCC
      return(new TFasorBarCC(barCC, color, atr, ind_bar, visible));
      }catch(Exception &e)
         {
         return(NULL);
         }
   }
/*
//---------------------------------------------------------------------------
VTFasorBarCC* __fastcall NewObjFasorBarCCVff(VTEqpto *eqpto, TColor color, int atr, int ind_bar, bool visible)
   {
   try{//cria objeto TFasorBarCC
      return(new TFasorBarCC(eqpto, color, atr, ind_bar, visible));
      }catch(Exception &e)
         {
         return(NULL);
         }
   }
*/
//---------------------------------------------------------------------------
__fastcall TFasorBarCC::TFasorBarCC(VTBarCC *barCC, TColor color, int atr, int ind_bar, bool visible)
   {
   //salva parâmetros
   this->barCC   = barCC;
   this->color   = color;
   this->atr     = atr;
   this->ind_bar = ind_bar;
   this->visible = visible;
   //inicia FasorBarCC p/ primeiro patamar
   DefinePatamar(0);
   }

//---------------------------------------------------------------------------
__fastcall TFasorBarCC::~TFasorBarCC(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
void __fastcall TFasorBarCC::DefinePatamar(int ind_pat)
   {
   //salva patamar
   this->ind_pat = ind_pat;
   //inicia coordenadas de acordo c/tipo do Eqpto
   IniciaFasorBarCC_Vfn_pu();
   //IniciaFasorBarCC_Vff_pu()
   }

//---------------------------------------------------------------------------
void __fastcall TFasorBarCC::IniciaFasorBarCC_Vff_pu(void)
   {
   /*
   //variáveis locais
   VTBarra *pbarra = (VTBarra*)eqpto;

   codigo  = "Vff(kV)";
   dim     = pbarra->vnom;
   orig.x  = orig.y = 0.;
   raio = pbarra->vnom / sqrt(3.);
   raio = pbarra->vnom;
   a.codigo = "Vab";
   a.x = pbarra->resflow->Vab_kv[ind_pat].real();
   a.y = pbarra->resflow->Vab_kv[ind_pat].imag();
   b.codigo = "Vbc";
   b.x = pbarra->resflow->Vbc_kv[ind_pat].real(); // * 0.95;
   b.y = pbarra->resflow->Vbc_kv[ind_pat].imag(); // * 0.95;
   c.codigo = "Vca";
   c.x = pbarra->resflow->Vca_kv[ind_pat].real(); // * 0.90;
   c.y = pbarra->resflow->Vca_kv[ind_pat].imag(); // * 0.90;
   */
   }

//---------------------------------------------------------------------------
//void __fastcall TFasorBarCC::IniciaFasorBarCC_Vfn_pu(void)
//   {
//   //variáveis locais
//   int ind_def = atr;
//
//   codigo    = NomeDefeito(ind_def) + ": Vfn(pu)";
//   dim       = barCC->Barra->vnom;
//   orig.x    = orig.y = 0.;
//   raio      = barCC->Barra->vnom / sqrt(3.);
//   a.codigo  = "Van";
//   a.x       = barCC->Vfas_kv[ind_def][0].real();
//   a.y       = barCC->Vfas_kv[ind_def][0].imag();
//   a.enabled = (fabs(a.x) + fabs(a.y)) > 0.;
//   b.codigo  = "Vbn";
//   b.x       = barCC->Vfas_kv[ind_def][1].real();
//   b.y       = barCC->Vfas_kv[ind_def][1].imag();
//   b.enabled = (fabs(b.x) + fabs(b.y)) > 0.;
//   c.codigo  = "Vcn";
//   c.x       = barCC->Vfas_kv[ind_def][2].real();
//   c.y       = barCC->Vfas_kv[ind_def][2].imag();
//   c.enabled = (fabs(c.x) + fabs(c.y)) > 0.;
//   }
//---------------------------------------------------------------------------
void __fastcall TFasorBarCC::IniciaFasorBarCC_Vfn_pu(void)  //FKM 2015.12.02
   {
   //variáveis locais
   int ind_def = atr;

   codigo    = NomeDefeito(ind_def) + ": Vfn(pu)";
   dim       = barCC->Barra->vnom/ sqrt(3.);
   orig.x    = orig.y = 0.;
   raio      = barCC->Barra->vnom / sqrt(3.);
   a.codigo  = NomeDefeito(ind_def) + ": Van";
   a.x       = barCC->Vfas_kv[ind_def][0].real();
   a.y       = barCC->Vfas_kv[ind_def][0].imag();
   a.enabled = (fabs(a.x) + fabs(a.y)) > 0.;
   b.codigo  = NomeDefeito(ind_def) + ": Vbn";
   b.x       = barCC->Vfas_kv[ind_def][1].real();
   b.y       = barCC->Vfas_kv[ind_def][1].imag();
   b.enabled = (fabs(b.x) + fabs(b.y)) > 0.;
   c.codigo  = NomeDefeito(ind_def) + ": Vcn";
   c.x       = barCC->Vfas_kv[ind_def][2].real();
   c.y       = barCC->Vfas_kv[ind_def][2].imag();
   c.enabled = (fabs(c.x) + fabs(c.y)) > 0.;
   }
//-----------------------------------------------------------------------------
AnsiString __fastcall TFasorBarCC::NomeDefeito(int ind_def)
   {
   //variáveis locais
   char *tipo_def[] = {"3F(abc)", "2F(bc)", "1FT(at)", "1FTz(at)", "2FT(bct)"};

   return(tipo_def[ind_def]);
   }

//---------------------------------------------------------------------------
bool __fastcall TFasorBarCC::PM_GetTipoCurto(void)
   {
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFasorBarCC::PM_GetTipoFluxo(void)
   {
   return(false);
   }

//---------------------------------------------------------------------------
//eof
