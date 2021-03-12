//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#include <algorithm>
#pragma hdrstop
#include "TFasor.h"
#include "..\Arranjo\VTArranjo.h"
#include "..\Complexo\Complexo.h"
#include "..\Constante\Fases.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTTrafo.h"
#include "..\Rede\VTTrecho.h"
#include "..\Rede\VTResflowBar.h"
#include "..\Rede\VTResflowLig.h"
#include "..\Zona\VTZona.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTFasor* __fastcall NewObjFasor(VTEqpto *eqpto, TColor color, int atr, int ind_bar, bool visible)
   {
   try{//cria objeto TFasor
      return(new TFasor(eqpto, color, atr, ind_bar, visible));
      }catch(Exception &e)
         {
         return(NULL);
         }
   }
/*
//---------------------------------------------------------------------------
VTFasor* __fastcall NewObjFasorVff(VTEqpto *eqpto, TColor color, int atr, int ind_bar, bool visible)
   {
   try{//cria objeto TFasor
      return(new TFasor(eqpto, color, atr, ind_bar, visible));
      }catch(Exception &e)
         {
         return(NULL);
         }
   }
*/
//---------------------------------------------------------------------------
__fastcall TFasor::TFasor(VTEqpto *eqpto, TColor color, int atr, int ind_bar, bool visible)
   {
   //salva parâmetros
   this->eqpto   = eqpto;
   this->color   = color;
   this->atr     = atr;
   this->ind_bar = ind_bar;
   this->visible = visible;
   //fixa valor da origem
   orig.x = orig.y = 0;
   //desabilita as três fases
   a.enabled = false;
   b.enabled = false;
   c.enabled = false;
   //inicia Fasor p/ primeiro patamar
   DefinePatamar(0);
   }

//---------------------------------------------------------------------------
__fastcall TFasor::~TFasor(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
void __fastcall TFasor::DefinePatamar(int ind_pat)
   {
   //salva patamar
   this->ind_pat = ind_pat;
   //desabilita as 3 fases
   a.enabled = b.enabled = c.enabled = false;
   //inicia coordenadas de acordo c/tipo do Eqpto
   switch(eqpto->Tipo())
      {
      case eqptoBARRA: IniciaFasorBarra();   break;
      case eqptoCHAVE:
      case eqptoTRAFO:
      case eqptoTRECHO:IniciaFasorLigacao(); break;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFasor::IniciaFasorBarra(void)
   {
   switch(atr)
      {
      case bar_VFN_KV: IniciaFasorBarraVfn_kv(); break;
      case bar_VFF_KV: IniciaFasorBarraVff_kv(); break;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFasor::IniciaFasorBarraVff_kv(void)
   {
   //variáveis locais
   VTBarra *pbarra = (VTBarra*)eqpto;

   //proteção
   if (pbarra->zona == NULL) return;
   codigo  = "Vff";
   dim     = pbarra->vnom;
   //raio = pbarra->vnom;
   raio = pbarra->zona->Vff_kv;
   //verifica se existe a fase AB
   if ((pbarra->resflow->Fases & faseAB) == faseAB)
      {
      a.enabled = true;
      a.codigo  = "Vab";
      a.x = pbarra->resflow->Vab_kv[ind_pat].real();
      a.y = pbarra->resflow->Vab_kv[ind_pat].imag();
      }
   //verifica se existe a fase BC
   if ((pbarra->resflow->Fases & faseBC) == faseBC)
      {
      b.enabled = true;
      b.codigo  = "Vbc";
      b.x = pbarra->resflow->Vbc_kv[ind_pat].real();
      b.y = pbarra->resflow->Vbc_kv[ind_pat].imag();
      }
   //verifica se existe a fase CA
   if ((pbarra->resflow->Fases & faseCA) == faseCA)
      {
      c.enabled = true;
      c.codigo  = "Vca";
      c.x = pbarra->resflow->Vca_kv[ind_pat].real();
      c.y = pbarra->resflow->Vca_kv[ind_pat].imag();
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFasor::IniciaFasorBarraVfn_kv(void)
   {
   //variáveis locais
   VTBarra *pbarra = (VTBarra*)eqpto;

   //proteção
   if (pbarra->zona == NULL) return;
   codigo  = "Vfn";
   dim     = pbarra->vnom;
   //raio = pbarra->vnom / sqrt(3.);
   //verifica se existe a fase A
   if ((pbarra->resflow->Fases & faseA) == faseA)
      {
      a.enabled = true;
      a.codigo  = "Van";
      raio = pbarra->zona->Vfn_kv[indFASE_A];
      a.x  = pbarra->resflow->Van_kv[ind_pat].real();
      a.y  = pbarra->resflow->Van_kv[ind_pat].imag();
      }
   //verifica se existe a fase B
   if ((pbarra->resflow->Fases & faseB) == faseB)
      {
      b.enabled = true;
      b.codigo  = "Vbn";
      raio = pbarra->zona->Vfn_kv[indFASE_B];
      b.x  = pbarra->resflow->Vbn_kv[ind_pat].real();
      b.y  = pbarra->resflow->Vbn_kv[ind_pat].imag();
      }
   //verifica se existe a fase B
   if ((pbarra->resflow->Fases & faseC) == faseC)
      {
      c.enabled = true;
      c.codigo  = "Vcn";
      raio = pbarra->zona->Vfn_kv[indFASE_C];
      c.x  = pbarra->resflow->Vcn_kv[ind_pat].real();
      c.y  = pbarra->resflow->Vcn_kv[ind_pat].imag();
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFasor::IniciaFasorLigacao(void)
   {
   switch(atr)
      {
      case lig_I_ABC: IniciaFasorLigacaoI(); break;
      case lig_S:     IniciaFasorLigacaoS(); break;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFasor::IniciaFasorLigacaoI(void)
   {
   //variáveis locais
   int        fases = faseABC;
   double     fatmult, val_base;
   AnsiString pri_sec = "";
   VTLigacao  *ligacao = (VTLigacao*)eqpto;
   VTTrafo    *trafo   = (VTTrafo*)eqpto;
   VTTrecho   *trecho  = (VTTrecho*)ligacao;

   //identifica fases e define código do Fasor
   if (eqpto->Tipo() == eqptoTRAFO)
      {
      if (trafo->IndiceBarra(trafo->BarraPrimario()) == ind_bar)
         {
         fases   = trafo->pri.fase;
         codigo  = "Ipri";
         pri_sec = "Ipri";
        }
      else
         {
         fases   = trafo->sec.fase;
         codigo  = "Isec";
         pri_sec = "Isec";
         }
      }
   else if (eqpto->Tipo() == eqptoTRECHO)
      {//determina fases
      fases = trecho->arranjo->Fases;
      //define código
      if (ligacao->resflow->SentidoP[ind_bar][ind_pat] == fluxoPOS)
         {
         codigo  = "Iini";
         pri_sec = "Iini";
         }
      else
         {
         codigo  = "Ifim";
         pri_sec = "Ifim";
         }
      }
   else
      {//define fases
      fases = faseABC;
      if (ligacao->resflow->SentidoP[ind_bar][ind_pat] == fluxoPOS)
         {
         codigo  = "Iini";
         pri_sec = "Iini";
         }
      else
         {
         codigo  = "Ifim";
         pri_sec = "Ifim";
         }
      }
   //define dim e raio
   dim  = raio = max(ligacao->resflow->If_amp_max[ind_bar][ind_pat], ligacao->resflow->If_amp_max[1-ind_bar][ind_pat]);
   //determina valor de base para converter a dimensão do vetor de corrente
   val_base = ligacao->resflow->If_amp_max[ind_bar][ind_pat];
   if (IsDoubleZero(val_base)) val_base = 1;
   //verifica se fluxo c/ sentido negativo
   fatmult = (ligacao->resflow->SentidoP[ind_bar][ind_pat] == fluxoNEG) ? -1. : 1.;
   //fase A
   if ((fases & faseA) == faseA)
      {
      a.enabled = true;
      a.codigo  = pri_sec + "_a";
      a.x = fatmult * ligacao->resflow->Ia_amp[ind_bar][ind_pat].real() * (dim / val_base);
      a.y = fatmult * ligacao->resflow->Ia_amp[ind_bar][ind_pat].imag() * (dim / val_base);
      }
   if ((fases & faseB) == faseB)
      {
      b.enabled = true;
      b.codigo  = pri_sec + "_b";
      b.x = fatmult * ligacao->resflow->Ib_amp[ind_bar][ind_pat].real() * (dim / val_base);
      b.y = fatmult * ligacao->resflow->Ib_amp[ind_bar][ind_pat].imag() * (dim / val_base);
      }
   if ((fases & faseC) == faseC)
      {
      c.enabled = true;
      c.codigo  = pri_sec + "_c";
      c.x = fatmult * ligacao->resflow->Ic_amp[ind_bar][ind_pat].real() * (dim / val_base);
      c.y = fatmult * ligacao->resflow->Ic_amp[ind_bar][ind_pat].imag() * (dim / val_base);
      }
   /*
   //verifica se o fasor é p/ a menor corrente do Trafo
   if ((eqpto->Tipo() == eqptoTRAFO) && (i1 < i2))
      {//TRUQUE p/ o gráfico: define raio como 1/5 da corrente maior
      raio = dim /5;
      }
   */
   }

//---------------------------------------------------------------------------
void __fastcall TFasor::IniciaFasorLigacaoS(void)
   {
   //variáveis locais
   double     fatmult;
   AnsiString pri_sec = "";
   VTLigacao  *ligacao = (VTLigacao*)eqpto;
   VTTrafo    *trafo   = (VTTrafo*)eqpto;

   //define código do Fasor
   if (eqpto->Tipo() == eqptoTRAFO)
      {
      if (trafo->IndiceBarra(trafo->BarraPrimario()) == ind_bar)
         {
         codigo  = "Spri";
         pri_sec = "Spri";
         }
      else
         {
         codigo  = "Ssec";
         pri_sec = "Ssec";
         }
      }
   else
      {
      if (ligacao->resflow->SentidoP[ind_bar][ind_pat] == fluxoPOS)
         {
         codigo  = "Sini";
         pri_sec = "Sini";
         }
      else
         {
         codigo  = "Sfim";
         pri_sec = "Sfim";
         }
      }
   //define dimensões
   raio = 0;
   dim  = max(Abs(ligacao->resflow->S_mva[ind_bar][ind_pat]),
              Abs(ligacao->resflow->S_mva[1-ind_bar][ind_pat]));
   //verifica se fluxo c/ sentido negativo
   fatmult   = (ligacao->resflow->SentidoP[ind_bar][ind_pat] == fluxoNEG) ? -1. : 1.;
   a.enabled = true;
   a.codigo  = pri_sec;
   a.x = fatmult * ligacao->resflow->S_mva[ind_bar][ind_pat].real();
   a.y = fatmult * ligacao->resflow->S_mva[ind_bar][ind_pat].imag();
   b.enabled = true;
   c.enabled = true;
   }

//---------------------------------------------------------------------------
bool __fastcall TFasor::PM_GetTipoCurto(void)
   {
   return(false);
   }

//---------------------------------------------------------------------------
bool __fastcall TFasor::PM_GetTipoFluxo(void)
   {
   return(true);
   }

//---------------------------------------------------------------------------
//eof
