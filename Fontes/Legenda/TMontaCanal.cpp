///---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#pragma hdrstop
#include "TMontaCanal.h"
#include "TAtributo.h"
#include "VTDado.h"
#include "VTGrupo.h"
#include "VTVetval.h"
#include "..\Apl\VTApl.h"
#include "..\Complexo\Complexo.h"
#include "..\Constante\Const.h"
#include "..\Curvas\VTCurva.h"
#include "..\Potencia\VTDemanda.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Rede\VTCanal.h"
#include "..\Rede\VTFases.h"
#include "..\Rede\VTSuprimento.h"
#include "..\Rede\VTGerador.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTMonta* __fastcall NewObjMontaCanal(VTApl *apl)
   {
   try{
      return (new TMontaCanal(apl));
      }catch (Exception &e)
         {//nada a fazer
         }
   return(NULL);
   }

//---------------------------------------------------------------------------
__fastcall TMontaCanal::TMontaCanal(VTApl *apl)
   {
   //salva ponteiro p/ objetos
   this->apl = apl;
   }

//---------------------------------------------------------------------------
__fastcall TMontaCanal::~TMontaCanal(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaCanal::LisVetValCanal(VTCanal *canal, TList *lisEXT, int ind_atrib, int ano)
   {
   //salva parâmetros
   this->canal   = canal;
   this->lisEXT    = lisEXT;
   this->ano       = ano;
   //verifica atributo selecionado
   switch (canal->Eqpto->Tipo())
      {
      case eqptoCARGA:
         switch (ind_atrib)
            {
            case car_P:    VetValorPotAtiva(ind_atrib);        break;
            case car_Q:    VetValorPotReativa(ind_atrib);      break;
            case car_S:    VetValorPotAparente(ind_atrib);     break;
            }
         break;
      case eqptoSUPRIMENTO:
         switch (ind_atrib)
            {
            case sup_P:    VetValorPotAtiva(ind_atrib);        break;
            case sup_Q:    VetValorPotReativa(ind_atrib);      break;
            case sup_S:    VetValorPotAparente(ind_atrib);     break;
            case sup_PQVF: VetValorCurvaPQ_PV_VF(ind_atrib);   break;
            }
         break;
      case eqptoGERADOR:
         switch (ind_atrib)
            {
            case sup_P:    VetValorPotAtiva(ind_atrib);        break;
            case sup_Q:    VetValorPotReativa(ind_atrib);      break;
            case sup_S:    VetValorPotAparente(ind_atrib);     break;
            case sup_PQVF: VetValorCurvaPQ_PV_VF(ind_atrib);   break;
            default: return (false);
            }
         break;
      case eqptoTRECHO:
         switch (ind_atrib)
            {
            case lig_P:    VetValorPotAtiva(ind_atrib);        break;
            case lig_Q:    VetValorPotReativa(ind_atrib);      break;
            case lig_S:    VetValorPotAparente(ind_atrib);     break;
            default: return (false);
            }
         break;
      case eqptoTRAFO:
         switch (ind_atrib)
            {
            case lig_P:    VetValorPotAtiva(ind_atrib);        break;
            case lig_Q:    VetValorPotReativa(ind_atrib);      break;
            case lig_S:    VetValorPotAparente(ind_atrib);     break;
            default: return (false);
            }
         break;
      case eqptoCHAVE:
         switch (ind_atrib)
            {
            case lig_P:    VetValorPotAtiva(ind_atrib);        break;
            case lig_Q:    VetValorPotReativa(ind_atrib);      break;
            case lig_S:    VetValorPotAparente(ind_atrib);     break;
            default: return (false);
            }
         break;
      case eqptoBARRA:
         switch (ind_atrib)
            {
            case bar_VFF_PU_MIN:    VetValorTensao(ind_atrib); break;
            default: return (false);
            }
         break;
      default: return (false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaCanal::VetValorPotAparente(int ind_atrib)
   {
   //variáveis locais
   AnsiString  val_str;
   int         max_pat, num_pat;
   int         ini, fim;
   strHM       hm_ini, hm_fim;
   VTVetval    *vetval;
   unionVALOR  valor;
   double      p_mw, q_mvar;
   VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   //determina número de patamares
   num_pat = canal->Curva->NumeroPontos();
   max_pat = patamares->NumPatamar();
   //cria um novo Vetval e insere em lisEXT
   lisEXT->Add(vetval = NewObjVetval(ind_atrib, "S", max_pat));
   //preenche valores
   for (int np = 0; np < max_pat; np++)
      {
      p_mw = 0;
      q_mvar = 0;
      vetval->Valor[np] = 0;
      ini = int(double(np) * 24 / double(max_pat));
      fim = int((double(np)+1) * 24 / double(max_pat));
      for (int p = 0 ; p < num_pat ; p++)
         if (canal->Curva->GetPonto(p, hm_ini, hm_fim, valor.vet, sizeof(valor)/sizeof(double)))
            if ((ini >= hm_ini.hora) && (fim <= hm_fim.hora+1))
               {
                  p_mw = valor.var.p;
                  q_mvar = valor.var.q;
                  vetval->Valor[np] = abs(complex<double>(p_mw, q_mvar));
               }
      //preenche valor como string
      aue.PotAparente_MVA(complex<double>(p_mw, q_mvar), val_str, vetval->unidade);
      vetval->ValorAsStr[np] = val_str;
      }
   //ajusta valores de acordo c/ a UE mais conveniente
   //vetval->AjustaUE_MVA();
   }

//---------------------------------------------------------------------------
void __fastcall TMontaCanal::VetValorPotAtiva(int ind_atrib)
   {
   //variáveis locais
   AnsiString  val_str;
   int         max_pat, num_pat;
   int         ini, fim;
   strHM       hm_ini, hm_fim;
   VTVetval    *vetval;
   unionVALOR  valor;
   VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   //determina número de patamares
   num_pat = canal->Curva->NumeroPontos();
   max_pat = patamares->NumPatamar();
   //cria um novo Vetval e insere em lisEXT
   lisEXT->Add(vetval = NewObjVetval(ind_atrib, "P", max_pat));
   //preenche valores
   for (int np = 0; np < max_pat; np++)
      {
      vetval->Valor[np] = 0;
      ini = int(double(np) * 24 / double(max_pat));
      fim = int((double(np)+1) * 24 / double(max_pat));
      for (int p = 0 ; p < num_pat ; p++)
         if (canal->Curva->GetPonto(p, hm_ini, hm_fim, valor.vet, sizeof(valor)/sizeof(double)))
            if ((ini >= hm_ini.hora) && (fim <= hm_fim.hora+1))
               vetval->Valor[np] = valor.var.p;
      //preenche valor como string
      aue.PotAtiva_MW(vetval->Valor[np], val_str, vetval->unidade);
      vetval->ValorAsStr[np] = val_str;
      }
   //ajusta valores de acordo c/ a UE mais conveniente
   //vetval->AjustaUE_MW();
   }

//---------------------------------------------------------------------------
void __fastcall TMontaCanal::VetValorPotReativa(int ind_atrib)
   {
   //variáveis locais
   AnsiString  val_str;
   unionVALOR  valor;
   int         max_pat, num_pat;
   int         ini, fim;
   strHM       hm_ini, hm_fim;
   VTVetval    *vetval;
   VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   //determina número de patamares
   num_pat = canal->Curva->NumeroPontos();
   max_pat = patamares->NumPatamar();
   //cria um novo Vetval e insere em lisEXT
   lisEXT->Add(vetval = NewObjVetval(ind_atrib, "Q", max_pat));
   //preenche valores
   for (int np = 0; np < max_pat; np++)
      {
      vetval->Valor[np] = 0;
      ini = int(double(np) * 24 / double(max_pat));
      fim = int((double(np)+1) * 24 / double(max_pat));
      for (int p = 0 ; p < num_pat ; p++)
         if (canal->Curva->GetPonto(p, hm_ini, hm_fim, valor.vet, sizeof(valor)/sizeof(double)))
            if ((ini >= hm_ini.hora) && (fim <= hm_fim.hora+1))
               vetval->Valor[np] = valor.var.q;
      //preenche valor como string
      vetval->Valor[np]      = aue.PotReativa_MVAR(vetval->Valor[np], val_str, vetval->unidade);
      vetval->ValorAsStr[np] = val_str;
      }
   //ajusta valores de acordo c/ a UE mais conveniente
   //vetval->AjustaUE_MVAR();
   }

//---------------------------------------------------------------------------
void __fastcall TMontaCanal::VetValorTensao(int ind_atrib)
   {
   //variáveis locais
   AnsiString  val_str;
   int         max_pat, num_pat;
   int         ini, fim;
   strHM       hm_ini, hm_fim;
   VTVetval    *vetval;
   unionVALOR  valor;
   double      v_pu, t_rad;
   VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   //determina número de patamares
   num_pat = canal->Curva->NumeroPontos();
   max_pat = patamares->NumPatamar();
   //cria um novo Vetval e insere em lisEXT
   lisEXT->Add(vetval = NewObjVetval(ind_atrib, "V", max_pat));
   //preenche valores
   for (int np = 0; np < max_pat; np++)
      {
      v_pu = 0;
      t_rad = 0;
      vetval->Valor[np] = 0;
      ini = int(double(np) * 24 / double(max_pat));
      fim = int((double(np)+1) * 24 / double(max_pat));
      for (int p = 0 ; p < num_pat ; p++)
         if (canal->Curva->GetPonto(p, hm_ini, hm_fim, valor.vet, sizeof(valor)/sizeof(double)))
            if ((ini >= hm_ini.hora) && (fim <= hm_fim.hora+1))
               {
               v_pu = valor.var.v;
               t_rad = (valor.var.t / 360.0) * 2 * M_PI;
               vetval->Valor[np] = v_pu;
               }
      //preenche valor como string
      aue.Tensao_PU(polar(v_pu, t_rad), val_str, vetval->unidade);
      vetval->ValorAsStr[np] = val_str;
      }
   }
//---------------------------------------------------------------------------
void __fastcall TMontaCanal::VetValorCurvaPQ_PV_VF(int ind_atrib)
   {

   //variáveis locais
   AnsiString     val_str;
   int            max_pat, num_pat;
   double         v_pu, t_rad, p_mw, q_mvar;
   VTVetval       *vetval;
   VTPatamares    *patamares    = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   int            tiposup;
   int            ini, fim;
   strHM          hm_ini, hm_fim;
   VTSuprimento   *suprimento;
   VTGerador      *gerador;
   unionVALOR  valor;
   //determina número de patamares
   max_pat = patamares->NumPatamar();
   num_pat = canal->Curva->NumeroPontos();
   //return caso nao seja suprimento ou gerador
   switch (canal->Eqpto->Tipo())
      {
      case eqptoSUPRIMENTO:
         suprimento = (VTSuprimento*)canal->Eqpto;
         tiposup = suprimento->tiposup;
         break;
      case eqptoGERADOR:
         gerador = (VTGerador*)canal->Eqpto;
         tiposup = gerador->tiposup;
         break;
      default: return;
      }
   //verifica se suprimento do tipo supPQ
   if (tiposup == supVF)
      {//cria Vetval p/ tensão (módulo e fase)
      lisEXT->Add(vetval = NewObjVetval(ind_atrib, "(V/teta)", max_pat));
      //preenche valores
      for (int np = 0; np < max_pat; np++)
         {
         v_pu = 0;
         t_rad = 0;
         vetval->Valor[np] = 0;
         ini = int(double(np) * 24 / double(max_pat));
         fim = int((double(np)+1) * 24 / double(max_pat));
         for (int p = 0 ; p < num_pat ; p++)
            if (canal->Curva->GetPonto(p, hm_ini, hm_fim, valor.vet, sizeof(valor)/sizeof(double)))
               if ((ini >= hm_ini.hora) && (fim <= hm_fim.hora+1))
                  {
                  v_pu = valor.var.v;
                  t_rad = (valor.var.t / 360.0) * 2 * M_PI;
                  vetval->Valor[np] = v_pu;
                  }
         //preenche valor como string
         aue.Tensao_PU(polar(v_pu, t_rad), val_str, vetval->unidade);
         vetval->ValorAsStr[np] = val_str;
         }
      }
   //verifica se suprimento do tipo supPV
   if (tiposup == supPV)
      {//cria um  Vetval p/ P e outro Vetval p/ V
      lisEXT->Add(vetval = NewObjVetval(ind_atrib, "(P)", max_pat));
      //preenche valores
      for (int np = 0; np < max_pat; np++)
         {
         vetval->Valor[np] = 0;
         ini = int(double(np) * 24 / double(max_pat));
         fim = int((double(np)+1) * 24 / double(max_pat));
         for (int p = 0 ; p < num_pat ; p++)
            if (canal->Curva->GetPonto(p, hm_ini, hm_fim, valor.vet, sizeof(valor)/sizeof(double)))
               if ((ini >= hm_ini.hora) && (fim <= hm_fim.hora+1))
                  vetval->Valor[np] = valor.var.p;
         //preenche valor como string
         aue.PotAtiva_MW(vetval->Valor[np], val_str, vetval->unidade);
         vetval->ValorAsStr[np] = val_str;
         }
      //ajusta valores de acordo c/ a UE mais conveniente
      //vetval->AjustaUE_MW();
      //cria Vetval p/ V
      lisEXT->Add(vetval = NewObjVetval(ind_atrib, "(V)", max_pat));
      //preenche valores
      for (int np = 0; np < max_pat; np++)
         {
         v_pu = 0;
         t_rad = 0;
         vetval->Valor[np] = 0;
         ini = int(double(np) * 24 / double(max_pat));
         fim = int((double(np)+1) * 24 / double(max_pat));
         for (int p = 0 ; p < num_pat ; p++)
            if (canal->Curva->GetPonto(p, hm_ini, hm_fim, valor.vet, sizeof(valor)/sizeof(double)))
               if ((ini >= hm_ini.hora) && (fim <= hm_fim.hora+1))
                  {
                  v_pu = valor.var.v;
                  t_rad = (valor.var.t / 360.0) * 2 * M_PI;
                  vetval->Valor[np] = v_pu;
                  }
         //preenche valor como string
         aue.Tensao_PU(polar(v_pu, t_rad), val_str, vetval->unidade);
         vetval->ValorAsStr[np] = val_str;
         }
      }
   //verifica se suprimento do tipo supPQ
   if (suprimento->tiposup == supPQ)
      {//cria Vetval p/ P/Q
      lisEXT->Add(vetval = NewObjVetval(ind_atrib, "(P/Q)", max_pat));
      //vetval->unidade = "MVA";
      //preenche valores
      for (int np = 0; np < max_pat; np++)
         {
         vetval->Valor[np] = 0;
         ini = int(double(np) * 24 / double(max_pat));
         fim = int((double(np)+1) * 24 / double(max_pat));
         for (int p = 0 ; p < num_pat ; p++)
            if (canal->Curva->GetPonto(p, hm_ini, hm_fim, valor.vet, sizeof(valor)/sizeof(double)))
               if ((ini >= hm_ini.hora) && (fim <= hm_fim.hora+1))
                  {
                  p_mw = valor.var.p;
                  q_mvar = valor.var.q;
                  vetval->Valor[np] = abs(complex<double>(p_mw, q_mvar));
                  }
         //preenche valor como string
         aue.PotAparente_MVA(complex<double>(p_mw, q_mvar), val_str, vetval->unidade);
         vetval->ValorAsStr[np] = val_str;
         }
      //ajusta valores de acordo c/ a UE mais conveniente
      //vetval->AjustaUE_MVA();
      }
   }

//---------------------------------------------------------------------------
//eof

