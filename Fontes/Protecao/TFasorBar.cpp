//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#pragma hdrstop
#include "TFasorBar.h"
#include "..\Complexo\Complexo.h"
#include "..\Constante\Fases.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTResflowBar.h"
#include "..\Zona\VTZona.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTFasor* __fastcall NewObjFasorFluxo(VTBarra *barra)
   {
   try{//cria objeto TFasorBar
      return(new TFasorBar(barra));
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TFasorBar::TFasorBar(VTBarra *barra)
   {
   //salva barra
   this->barra   = barra;
   //insere parâmetros default
   color        = clBlue;
   ind_pat      = 0;
   codigo       = "";
   enabled      = false;
   visible      = false;
   dim           = 0;
   raio          = 0;
   draw_seta     = true;
   fases         = faseINV;
   ind_seq       = -1;
   tipo_curto    = -1;
   coord.x       = 0.;
   coord.y       = 0.;
   tensao_kv     = CZero();
   //inicia Fasor p/ patamar
//   DefinePatamar(0);
   }

//---------------------------------------------------------------------------
__fastcall TFasorBar::~TFasorBar(void)
   {
   //nada a fazer
   }
   
//---------------------------------------------------------------------------
void __fastcall TFasorBar::AtualizaAngulo(double ang_novo)
   {
   //implementado somente para TFasorAjuste
   }

//---------------------------------------------------------------------------
void __fastcall TFasorBar::DefinePatamar(int ind_pat)
   {
   //salva patamar
   this->ind_pat = ind_pat;
   //inicia coordenadas de acordo c/tipo e unidade
   if      (fases != faseINV)                             IniciaFasorVfas();
   else if ((ind_seq >= indSEQ_0)&&(ind_seq <= indSEQ_2)) IniciaFasorVSeq();
   }

//---------------------------------------------------------------------------
void __fastcall TFasorBar::IniciaFasorVfas(void)
   {
   //habilita exibição
   //enabled = true;
   //define código
   codigo  = "Fluxo: ";
   //define dimensão máxima
   dim = barra->zona->Vff_kv;
   //inicia coordenadas de acordo as fases
   switch(fases)
      {
      case faseAN:
         codigo   += "Van";
         tensao_kv = barra->resflow->Van_kv[ind_pat];
         tensao_pu = barra->resflow->Van_pu[ind_pat];
         break;
      case faseBN:
         codigo   += "Vbn";
         tensao_kv = barra->resflow->Vbn_kv[ind_pat];
         tensao_pu = barra->resflow->Vbn_pu[ind_pat];
         break;
      case faseCN:
         codigo   += "Vcn";
         tensao_kv = barra->resflow->Vcn_kv[ind_pat];
         tensao_pu = barra->resflow->Vcn_pu[ind_pat];
         break;
      case faseAB:
         codigo   += "Vab";
         tensao_kv = barra->resflow->Vab_kv[ind_pat];
         tensao_pu = barra->resflow->Vab_pu[ind_pat];
         break;
      case faseBC:
         codigo   += "Vbc";
         tensao_kv = barra->resflow->Vbc_kv[ind_pat];
         tensao_pu = barra->resflow->Vbc_pu[ind_pat];
         break;
      case faseCA:
         codigo   += "Vca";
         tensao_kv = barra->resflow->Vca_kv[ind_pat];
         tensao_pu = barra->resflow->Vca_pu[ind_pat];
         break;
      default: //desabilita exibição
         tensao_kv = CZero();
         tensao_pu = CZero();
      }
   //verifica se valor nulo
   //if (IsDoubleZero(Abs(tensao_kv, V_KV_NULA))) tensao_kv = CZero();
   //if (IsDoubleZero(Abs(tensao_pu, V_PU_NULA))) tensao_pu = CZero();
   //verifica se valor nulo
   if (IsDoubleZero(Abs(tensao_kv), V_KV_NULA))
      {
      tensao_kv = CZero();
      tensao_pu = CZero();
      }
   //define informações para gráfico
   raio    = Abs(tensao_kv);
   coord.x = tensao_kv.real();
   coord.y = tensao_kv.imag();
   enabled = (raio > 0.);
   }

//---------------------------------------------------------------------------
void __fastcall TFasorBar::IniciaFasorVSeq(void)
   {
   //variáveis locais
   complex<double> van_kv, vbn_kv, vcn_kv;
   complex<double> van_pu, vbn_pu, vcn_pu;

   //habilita exibição
   //enabled = true;
   //determina tensões de fases em kV
   van_kv = barra->resflow->Van_kv[ind_pat];
   vbn_kv = barra->resflow->Vbn_kv[ind_pat];
   vcn_kv = barra->resflow->Vcn_kv[ind_pat];
   //determina tensões de fases em pu
   van_pu = barra->resflow->Van_pu[ind_pat];
   vbn_pu = barra->resflow->Vbn_pu[ind_pat];
   vcn_pu = barra->resflow->Vcn_pu[ind_pat];
   codigo  = "Fluxo: ";
   //define dimensão máxima
   dim = barra->zona->Vff_kv;
   //inicia coordenadas de acordo as fases
   switch(ind_seq)
      {
      case indSEQ_0:
         codigo   += "V0";
         tensao_kv = (van_kv + vbn_kv + vcn_kv) / 3.;
         tensao_pu = (van_pu + vbn_pu + vcn_pu) / 3.;
        break;
      case indSEQ_1:
         codigo   += "V1";
         tensao_kv = (van_kv + (Alfa() * vbn_kv) + (Alfa2() * vcn_kv)) / 3.;
         tensao_pu = (van_pu + (Alfa() * vbn_pu) + (Alfa2() * vcn_pu)) / 3.;
         break;
      case indSEQ_2:
         codigo   += "V2";
         tensao_kv = (van_kv + (Alfa2() * vbn_kv) + (Alfa() * vcn_kv)) / 3.;
         tensao_pu = (van_pu + (Alfa2() * vbn_pu) + (Alfa() * vcn_pu)) / 3.;
         break;
      default: //desabilita exibição
         tensao_kv = CZero();
         tensao_pu = CZero();
      }
   //verifica se valor nulo
   //if (IsDoubleZero(Abs(tensao_kv, V_KV_NULA))) tensao_kv = CZero();
   //if (IsDoubleZero(Abs(tensao_pu, V_PU_NULA))) tensao_pu = CZero();
   //verifica se valor nulo
   if (IsDoubleZero(Abs(tensao_kv), V_KV_NULA))
      {
      tensao_kv = CZero();
      tensao_pu = CZero();
      }
   //define informações para gráfico
   raio    = Abs(tensao_kv);
   coord.x = tensao_kv.real();
   coord.y = tensao_kv.imag();
   enabled = (raio > 0.);
   }

//---------------------------------------------------------------------------
void __fastcall TFasorBar::RedefineBarraReferencia(VTBarra *barra)
   {
   //salva barra
   this->barra = barra;
   //reinicia o fasor
   DefinePatamar(ind_pat);
   }

//---------------------------------------------------------------------------
int __fastcall TFasorBar::Tipo(void)
   {
   if (fases   != faseINV)                           return(fasorFLUXO_Vfas);
   if ((ind_seq >= indSEQ_0)&&(ind_seq <= indSEQ_2)) return(fasorFLUXO_Vseq);
   return(fasorINDEFINIDO);
   }

//---------------------------------------------------------------------------
bool __fastcall TFasorBar::Valor(complex<double> &valor, bool pu)
   {
   if (pu) {valor = tensao_pu;}
   else    {valor = tensao_kv;}

   return(true);
   }

//---------------------------------------------------------------------------
//eof
