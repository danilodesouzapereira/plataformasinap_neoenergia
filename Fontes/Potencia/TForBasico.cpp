//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <math.hpp>
#include "TForBasico.h"
#include "VTCalculaSup.h"
#include "..\Apl\VTApl.h"
#include "..\Complexo\Complexo.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTFases.h"
#include "..\Rede\VTGerador.h"
#include "..\Rede\VTSuprimento.h"
#include "..\Zona\VTZona.h"
#include "..\..\DLL_Inc\Funcao.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TForBasico::TForBasico(VTApl *apl)
   {
   try{//salva ponteiro p/ objetos
      this->apl = apl;
      fases     = (VTFases*)apl->GetObject(__classid(VTFases));
      patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
      //cria objetos
      calcula = NewObjCalculaSup(apl);
      }catch (Exception &e)
         {
         }
   }

//---------------------------------------------------------------------------
__fastcall TForBasico::~TForBasico(void)
   {
   //destrói objetos
   if (calcula) {delete calcula; calcula = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TForBasico::CalculaPQVT_porfase(strSUP *str_sup, strSUP vet_sup[MAX_FASE], int fases_eqpto)
   {
   //variáveis locais
   int    ind_fase;
   double defasagem[MAX_FASE] = {0., -2*M_PI/3, 2*M_PI/3, 0.};

   //elimina neutro/terra das fases do eqpto
   fases_eqpto = fases_eqpto & faseABC;
   //zera todos os valores em vet_sup
   for(ind_fase = indFASE_A; ind_fase <= indNEUTRO; ind_fase++)
      {
      vet_sup[ind_fase].p   = 0.;
      vet_sup[ind_fase].q   = 0.;
      vet_sup[ind_fase].v   = 0.;
      vet_sup[ind_fase].t   = 0.;
      }
   //verifica o número de fases do eqpto
   switch(fases->NumeroDeFases(fases_eqpto))
      {
      case 1: //define PQVT da única fase
         ind_fase = fases->TagToIndex[fases_eqpto];
         vet_sup[ind_fase].p   = str_sup->p;
         vet_sup[ind_fase].q   = str_sup->q;
         vet_sup[ind_fase].v   = str_sup->v;
         vet_sup[ind_fase].t   = str_sup->t;
         break;
      case 2: //define PQV das 2 fases em vet_sup
         if ((fases_eqpto & faseA) == faseA)
            {
            vet_sup[indFASE_A].p   = str_sup->p / 2.;
            vet_sup[indFASE_A].q   = str_sup->q / 2.;
            vet_sup[indFASE_A].v   = str_sup->v;
            }
         if ((fases_eqpto & faseB) == faseB)
            {
            vet_sup[indFASE_B].p   = str_sup->p / 2.;
            vet_sup[indFASE_B].q   = str_sup->q / 2.;
            vet_sup[indFASE_B].v   = str_sup->v;
            }
         if ((fases_eqpto & faseC) == faseC)
            {
            vet_sup[indFASE_C].p   = str_sup->p / 2.;
            vet_sup[indFASE_C].q   = str_sup->q / 2.;
            vet_sup[indFASE_C].v   = str_sup->v;
            }
         //define T das duas fases
         if ((fases_eqpto & faseAB) == faseAB)
            {
            vet_sup[indFASE_A].t   = str_sup->t;
            vet_sup[indFASE_B].t   = str_sup->t + M_PI;
            }
         else if ((fases_eqpto & faseBC) == faseBC)
            {
            vet_sup[indFASE_B].t   = str_sup->t;
            vet_sup[indFASE_C].t   = str_sup->t + M_PI;
            }
         else if ((fases_eqpto & faseCA) == faseCA)
            {
            vet_sup[indFASE_C].t   = str_sup->t;
            vet_sup[indFASE_A].t   = str_sup->t + M_PI;
            }
         break;
      case 3: //define PQVT das 3 fases em vet_sup
         for(ind_fase = indFASE_A; ind_fase <= indFASE_C; ind_fase++)
            {
            vet_sup[ind_fase].p   = str_sup->p / 3.;
            vet_sup[ind_fase].q   = str_sup->q / 3.;
            vet_sup[ind_fase].v   = str_sup->v;
            vet_sup[ind_fase].t   = str_sup->t + defasagem[ind_fase];
            }
         break;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TForBasico::CalculaPQVT_trifasico(strSUP *str_sup, strSUP vet_sup[MAX_FASE])
   {
   //variáveis locais
   double defasagem[MAX_FASE] = {0., -2*M_PI/3, 2*M_PI/3, 0.};

   //inicia valores de  str_sup
   str_sup->p   = 0.;
   str_sup->q   = 0.;
   str_sup->v   = 1.;
   str_sup->t   = 0.;
   for(int ind_fase = indFASE_A; ind_fase <= indFASE_C; ind_fase++)
      {
      str_sup->p += vet_sup[ind_fase].p;
      str_sup->q += vet_sup[ind_fase].q;
      }
   //determina primeira fase com módulo de tensão não nula
   for(int ind_fase = indFASE_A; ind_fase <= indFASE_C; ind_fase++)
      {
      if (IsDoubleZero(vet_sup[ind_fase].v)) continue;
      //copia módulo da tensão
      str_sup->v = vet_sup[ind_fase].v;
      //define ângulo da tensão, descontando a defasagem
      str_sup->t = vet_sup[ind_fase].t - defasagem[ind_fase];
      break;
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TForBasico::Get_PQVT_porfase(VTEqpto *eqpto, int np, strSUP vet_sup[MAX_FASE])
   {
   // Variáveis locais
   bool         sucesso;
   strSUP       str_sup;
   VTGerador    *gerador;
   VTSuprimento *suprimento;

   switch(eqpto->Tipo())
      {
      case eqptoGERADOR:
         gerador = (VTGerador*)eqpto;
         if (gerador->DefinidoPorFase)
            {//lê valores por fase
            sucesso = GetPQVTGeradorDeseq(gerador, np, vet_sup);
            }
         else
            {//lê valores trifásico
            sucesso = GetPQVTGeradorEquil(gerador, np, &str_sup);
            //converte PQVT total em PQVT por fase
            CalculaPQVT_porfase(&str_sup, vet_sup, gerador->Fases);
            }
         break;

      case eqptoSUPRIMENTO:
         suprimento = (VTSuprimento*)eqpto;
         if (suprimento->DefinidoPorFase)
            {//lê valores por fase
            sucesso = GetPQVTSuprimentoDeseq(suprimento, np, vet_sup);
            }
         else
            {//lê valores trifásico
            sucesso = GetPQVTSuprimentoEquil(suprimento, np, &str_sup);
            //converte PQVT total em PQVT por fase
            CalculaPQVT_porfase(&str_sup, vet_sup, suprimento->Fases);
            }
         break;

      default:
         sucesso = false;
      }
   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TForBasico::Get_PQVT_trifasico(VTEqpto *eqpto, int np, strSUP *str_sup)
   {
   // Variáveis locais
   bool         sucesso;
   strSUP       vet_sup[MAX_FASE];
   VTGerador    *gerador;
   VTSuprimento *suprimento;

   switch(eqpto->Tipo())
      {
      case eqptoGERADOR:
         gerador = (VTGerador*)eqpto;
         if (gerador->DefinidoPorFase)
            {//lê valores por fase
            sucesso = GetPQVTGeradorDeseq(gerador, np, vet_sup);
            //converte PQVT por fase em PQVT trifásico
            CalculaPQVT_trifasico(str_sup, vet_sup);
            }
         else
            {//lê valores trifásico
            sucesso = GetPQVTGeradorEquil(gerador, np, str_sup);
            }
         break;

      case eqptoSUPRIMENTO:
         suprimento = (VTSuprimento*)eqpto;
         if (suprimento->DefinidoPorFase)
            {//lê valores por fase
            sucesso = GetPQVTSuprimentoDeseq(suprimento, np, vet_sup);
            //converte PQVT por fase em PQVT trifásico
            CalculaPQVT_trifasico(str_sup, vet_sup);
            }
         else
            {//lê valores trifásico
            sucesso = GetPQVTSuprimentoEquil(suprimento, np, str_sup);
            }
         break;

      default:
         sucesso = false;
      }
   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TForBasico::GetPQVTGeradorDeseq(VTGerador *gerador, int np, strSUP vet_sup[MAX_FASE])
   {
   //obtém valores PQVT do Gerador
   if (! calcula->GetValorGeradorReducao(gerador, np, vet_sup)) return(false);
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TForBasico::GetPQVTGeradorEquil(VTGerador *gerador, int np, strSUP *str_sup)
   {
   //obtém valores PQVT do Gerador
   if(! calcula->GetValorGerador(gerador, np, str_sup)) return(false);
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TForBasico::GetPQVTSuprimentoDeseq(VTSuprimento *suprimento, int np, strSUP vet_sup[MAX_FASE])
   {
   //obtém valores PQVT do Suprimento
   if (! calcula->GetValorSuprimentoReducao(suprimento, np, vet_sup)) return(false);
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TForBasico::GetPQVTSuprimentoEquil(VTSuprimento *suprimento, int np, strSUP *str_sup)
   {
   //obtém valores PQVT do Suprimento
   if(! calcula->GetValorSuprimento(suprimento, np, str_sup)) return(false);
   return(true);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TForBasico::GetSmvaGerador(VTGerador *gerador, int np)
	{
   //variáveis locais
   strSUP str_sup;

   //obtém valores PQVT do Gerador
   if (! calcula->GetValorGerador(gerador, np, &str_sup)) return(CZero());
   //retorna potência aparente do Gerador
   return(complex<double>(str_sup.p, str_sup.q));
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TForBasico::GetSmvaSuprimento(VTSuprimento *suprimento, int np)
   {
   //variáveis locais
   strSUP str_sup;

   //obtém valores PQVT do Suprimento
   if (! calcula->GetValorSuprimento(suprimento, np, &str_sup)) return(CZero());
   //retorna potência aparente do Gerador
   return(complex<double>(str_sup.p, str_sup.q));
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TForBasico::GetSmvaSuprimentoReducao(VTSuprimento *suprimento, int np)
   {
   //variáveis locais
   strSUP          str_sup[MAX_FASE];
   complex<double> s_mva = CZero();

   //obtém valores PQVT do Suprimento
   if (! calcula->GetValorSuprimentoReducao(suprimento, np, str_sup)) return(CZero());
   //totaliza potência aparente das fases do Suprimento
   for (int nf = indFASE_A; nf <= indNEUTRO; nf++)
      {
      s_mva += complex<double>(str_sup[nf].p, str_sup[nf].q);
      }
   return(s_mva);
   }

//---------------------------------------------------------------------------
bool __fastcall TForBasico::GetUmValorGerador(VTGerador *gerador, int np, int ind_valor, double &valor)
   {
   //variáveis locais
   strSUP str_sup;

   //obtém valores PQVT do Gerador
   if (! calcula->GetValorGerador(gerador, np, &str_sup)) return(false);
   //retorna parâmetro solicitado
   switch(ind_valor)
      {
      case indP: valor = str_sup.p; break;
      case indQ: valor = str_sup.q; break;
      case indV: valor = str_sup.v; break;
      case indT: valor = str_sup.t; break;
      default:   return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TForBasico::GetUmValorSuprimento(VTSuprimento *suprimento, int np, int ind_valor, double &valor)
   {
   //variáveis locais
   strSUP str_sup;

   //obtém valores PQVT do Suprimento
   if (! calcula->GetValorSuprimento(suprimento, np, &str_sup)) return(false);
   //retorna parâmetro solicitado
   switch(ind_valor)
      {
      case indP: valor = str_sup.p; break;
      case indQ: valor = str_sup.q; break;
      case indV: valor = str_sup.v; break;
      case indT: valor = str_sup.t; break;
      default:   return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TForBasico::GetUmValorSuprimentoReducao(VTSuprimento *suprimento, int np, int ind_valor, double &valor)
   {
   //variáveis locais
   double v_max;
   strSUP str_sup[MAX_FASE];

   //obtém valores PQVT do Suprimento
   if (! calcula->GetValorSuprimentoReducao(suprimento, np, str_sup)) return(false);
   //retorna parâmetro solicitado
   switch(ind_valor)
      {
      case indP: //somatória das potências ativas
         valor = 0.;
         for (int nf = indFASE_A; nf <= indNEUTRO; nf++)
            {valor += str_sup[nf].p;}
         break;
      case indQ: //somatória das potências reativas
         valor = 0.;
         for (int nf = indFASE_A; nf <= indNEUTRO; nf++)
            {valor += str_sup[nf].q;}
         break;

      case indV:  //módulo da maior tensão de fase
         valor = 0.;
         for (int nf = indFASE_A; nf <= indNEUTRO; nf++)
            {if (str_sup[nf].v > valor) valor = str_sup[nf].v;}
         break;
      case indT:  //ângulo da maior tensão de fase
         v_max = 0.; valor = 0.;
         for (int nf = indFASE_A; nf <= indNEUTRO; nf++)
            {
            if (str_sup[nf].v > v_max) {v_max = str_sup[nf].v; valor = str_sup[nf].t;}
            }
         break;
      default: return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TForBasico::PM_GetAtivo(VTEqpto *eqpto, int np)
   {
   //variáveis locais
   int    tipo_sup;
   double p_mw, q_mvar, v_pu;

   //determina tipo do suprimento
   if      (eqpto->Tipo() == eqptoGERADOR)    tipo_sup = ((VTGerador*)eqpto)->tiposup;
   else if (eqpto->Tipo() == eqptoSUPRIMENTO) tipo_sup = ((VTSuprimento*)eqpto)->tiposup;
   else                                       return(false);
   //verifica valores de acordo com o tipo de suprimento
   switch(tipo_sup)
      {
      case supPQ:
         if (! IsDoubleZero(P_mw[eqpto][np]))   return(true);
         if (! IsDoubleZero(Q_mvar[eqpto][np])) return(true);
         break;
      case supPV:
         if (! IsDoubleZero(P_mw[eqpto][np])) return(true);
         if (! IsDoubleZero(V_pu[eqpto][np])) return(true);
         break;
      case supVF:
         if (! IsDoubleZero(V_pu[eqpto][np])) return(true);
         break;
      }
   return(false);
   }

//---------------------------------------------------------------------------
double __fastcall TForBasico::PM_GetP_mw(VTEqpto *eqpto, int np)
   {
   return(S_mva[eqpto][np].real());
   }

//---------------------------------------------------------------------------
double __fastcall TForBasico::PM_GetQ_mvar(VTEqpto *eqpto, int np)
   {
   return(S_mva[eqpto][np].imag());
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TForBasico::PM_GetS_mva(VTEqpto *eqpto, int np)
   {
   //variáveis locais
   VTSuprimento *suprimento;

   switch(eqpto->Tipo())
      {
      case eqptoGERADOR:    return(GetSmvaGerador((VTGerador*)eqpto, np));
      case eqptoSUPRIMENTO:
         suprimento = (VTSuprimento*)eqpto;
         if (suprimento->Reducao) return(GetSmvaSuprimentoReducao(suprimento, np));
         else                     return(GetSmvaSuprimento(suprimento, np));
      default: break;
      }
   return(CZero());
   }

//---------------------------------------------------------------------------
double __fastcall TForBasico::PM_GetT_grau(VTEqpto *eqpto, int np)
   {
   //variáveis locais
   return(RadToDeg(PM_GetT_rad(eqpto, np)));
   }

//---------------------------------------------------------------------------
double __fastcall TForBasico::PM_GetT_rad(VTEqpto *eqpto, int np)
   {
   //variáveis locais
   double       t_rad = 0.;
   VTSuprimento *suprimento;

   switch(eqpto->Tipo())
      {
      case eqptoGERADOR:
         GetUmValorGerador((VTGerador*)eqpto, np, indT, t_rad);
         break;
      case eqptoSUPRIMENTO:
         suprimento = (VTSuprimento*)eqpto;
         if (suprimento->Reducao) GetUmValorSuprimentoReducao(suprimento, np, indT, t_rad);
         else                     GetUmValorSuprimento(suprimento, np, indT, t_rad);
         break;
      default: break;
      }
   return(t_rad);
   }

//---------------------------------------------------------------------------
double __fastcall TForBasico::PM_GetV_pu(VTEqpto *eqpto, int np)
   {
   //variáveis locais
   double       v_pu = 1.0;
   VTSuprimento *suprimento;

   switch(eqpto->Tipo())
      {
      case eqptoGERADOR:
         GetUmValorGerador((VTGerador*)eqpto, np, indV, v_pu);
         break;
      case eqptoSUPRIMENTO:
         suprimento = (VTSuprimento*)eqpto;
         if (suprimento->Reducao) GetUmValorSuprimentoReducao(suprimento, np, indV, v_pu);
         else                     GetUmValorSuprimento(suprimento, np, indV, v_pu);
         break;
      default: break;
      }
   return(v_pu);
   }

//---------------------------------------------------------------------------
void __fastcall TForBasico::PM_SetP_mw(VTEqpto *eqpto, int np, double p)
   {
   //variáveis locais
   VTSuprimento *suprimento;

   switch(eqpto->Tipo())
      {
      case eqptoGERADOR:
         SetUmValorGerador((VTGerador*)eqpto, np, indP, p);
         break;
      case eqptoSUPRIMENTO:
         suprimento = (VTSuprimento*)eqpto;
         if (suprimento->Reducao) SetUmValorSuprimentoReducao(suprimento, np, indP, p);
         else                     SetUmValorSuprimento(suprimento, np, indP, p);
         break;
      default: break;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TForBasico::PM_SetQ_mvar(VTEqpto *eqpto, int np, double q)
   {
   //variáveis locais
   VTSuprimento *suprimento;

   switch(eqpto->Tipo())
      {
      case eqptoGERADOR:
         SetUmValorGerador((VTGerador*)eqpto, np, indQ, q);
         break;
      case eqptoSUPRIMENTO:
         suprimento = (VTSuprimento*)eqpto;
         if (suprimento->Reducao) SetUmValorSuprimentoReducao(suprimento, np, indQ, q);
         else                     SetUmValorSuprimento(suprimento, np, indQ, q);
         break;
      default: break;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TForBasico::PM_SetS_mva(VTEqpto  *eqpto, int np, complex<double> s_mva)
   {
   //variáveis locais
   VTSuprimento *suprimento;

   switch(eqpto->Tipo())
      {
      case eqptoGERADOR:
         SetSmvaGerador((VTGerador*)eqpto, np, s_mva);
         break;
      case eqptoSUPRIMENTO:
         suprimento = (VTSuprimento*)eqpto;
         if (suprimento->Reducao) SetSmvaSuprimentoReducao(suprimento, np, s_mva);
         else                     SetSmvaSuprimento(suprimento, np, s_mva);
         break;
      default: break;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TForBasico::PM_SetV_pu(VTEqpto *eqpto, int np, double v)
   {
   //variáveis locais
   VTSuprimento *suprimento;

   switch(eqpto->Tipo())
      {
      case eqptoGERADOR:
         SetUmValorGerador((VTGerador*)eqpto, np, indV, v);
         break;
      case eqptoSUPRIMENTO:
         suprimento = (VTSuprimento*)eqpto;
         if (suprimento->Reducao) SetUmValorSuprimentoReducao(suprimento, np, indV, v);
         else                     SetUmValorSuprimento(suprimento, np, indV, v);
         break;
      default: break;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TForBasico::PM_SetT_grau(VTEqpto *eqpto, int np, double t)
   {
   //variáveis locais
   PM_SetT_rad(eqpto, np, DegToRad(t));
   }

//---------------------------------------------------------------------------
void __fastcall TForBasico::PM_SetT_rad(VTEqpto  *eqpto, int np, double t)
   {
   //variáveis locais
   VTSuprimento *suprimento;

   switch(eqpto->Tipo())
      {
      case eqptoGERADOR:
         SetUmValorGerador((VTGerador*)eqpto, np, indT, t);
         break;
      case eqptoSUPRIMENTO:
         suprimento = (VTSuprimento*)eqpto;
         if (suprimento->Reducao) SetUmValorSuprimentoReducao(suprimento, np, indT, t);
         else                     SetUmValorSuprimento(suprimento, np, indT, t);
         break;
      default: break;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TForBasico::ReiniciaCurva(VTEqpto *eqpto, double defasagem_rad)
   {
   //variáveis locais
   VTSuprimento *suprimento;

   switch(eqpto->Tipo())
      {
      case eqptoGERADOR:
         ReiniciaCurvaGerador((VTGerador*)eqpto, defasagem_rad);
         break;
      case eqptoSUPRIMENTO:
         suprimento = (VTSuprimento*)eqpto;
         if (suprimento->Reducao) ReiniciaCurvaSuprimentoReducao(suprimento, defasagem_rad);
         else                     ReiniciaCurvaSuprimento(suprimento, defasagem_rad);
         break;
      default: break;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TForBasico::ReiniciaCurvaGerador(VTGerador *gerador, double defasagem_rad)
   {
   //variáveis locais
   strSUP str_sup = {1., 0., 0., float(defasagem_rad)};
   int    max_pat = patamares->NumPatamar();

   //atualiza todos patamares da Curva
   for (int np = 0; np < max_pat; np++)
      {
      calcula->SetValorGerador(gerador, np, &str_sup);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TForBasico::ReiniciaCurvaSuprimento(VTSuprimento *suprimento, double defasagem_rad)
   {
   //variáveis locais
   strSUP str_sup = {1., 0., 0., float(defasagem_rad)};
   int    max_pat = patamares->NumPatamar();

   //atualiza todos patamares da Curva
   for (int np = 0; np < max_pat; np++)
      {
      calcula->SetValorSuprimento(suprimento, np, &str_sup);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TForBasico::ReiniciaCurvaSuprimentoReducao(VTSuprimento *suprimento, double defasagem_rad)
   {
   //variáveis locais
   int    max_pat = patamares->NumPatamar();
   strSUP str_sup[MAX_FASE];

   // Inicialização (o compilador 64 bits não deixa inicializar na própria declaração)
   str_sup[0].p = 0.;
   str_sup[0].q = 0.;
   str_sup[0].v = 1.;
   str_sup[0].t = float(defasagem_rad);
   str_sup[1].p = 0.;
   str_sup[1].q = 0.;
   str_sup[1].v = 1.;
   str_sup[1].t = -(2.*M_PI/3.) + float(defasagem_rad);
   str_sup[2].p = 0.;
   str_sup[2].q = 0.;
   str_sup[2].v = 1.;
   str_sup[2].t =  (2.*M_PI/3.) + float(defasagem_rad);
   str_sup[3].p = 0.;
   str_sup[3].q = 0.;
   str_sup[3].v = 0.;
   str_sup[3].t = 0.;

   //atualiza todos patamares da Curva
   for (int np = 0; np < max_pat; np++)
      {
      calcula->SetValorSuprimentoReducao(suprimento, np, str_sup);
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TForBasico::Set_PQVT_porfase(VTEqpto *eqpto, int np, strSUP vet_sup[MAX_FASE])
   {
   // Variáveis locais
   bool         sucesso;
   strSUP       str_sup;
   VTGerador    *gerador;
   VTSuprimento *suprimento;

   switch(eqpto->Tipo())
      {
      case eqptoGERADOR:
         gerador = (VTGerador*)eqpto;
         if (gerador->DefinidoPorFase)
            {
            sucesso = SetPQVTGeradorDeseq(gerador, np, vet_sup);
            }
         else
            {//calcula PQVT trifásico
            CalculaPQVT_trifasico(&str_sup, vet_sup);
            sucesso = SetPQVTGeradorEquil(gerador, np, &str_sup);
            }
         break;

      case eqptoSUPRIMENTO:
         suprimento = (VTSuprimento*)eqpto;
         if (suprimento->DefinidoPorFase)
            {
            sucesso = SetPQVTSuprimentoDeseq(suprimento, np, vet_sup);
            }
         else
            {//calcula PQVT trifásico
            CalculaPQVT_trifasico(&str_sup, vet_sup);
            sucesso = SetPQVTSuprimentoEquil(suprimento, np, &str_sup);
            }
         break;

      default:
         sucesso = false;
      }
   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TForBasico::Set_PQVT_trifasico(VTEqpto *eqpto, int np, strSUP *str_sup)
   {
   // Variáveis locais
   bool         sucesso;
   strSUP       vet_sup[MAX_FASE];
   VTGerador    *gerador;
   VTSuprimento *suprimento;

   switch(eqpto->Tipo())
      {
      case eqptoGERADOR:
         gerador = (VTGerador*)eqpto;
         if (gerador->DefinidoPorFase)
            {//calcula PQVT por fase
            CalculaPQVT_porfase(str_sup, vet_sup, gerador->Fases);
            sucesso = SetPQVTGeradorDeseq(gerador, np, vet_sup);
            }
         else
            {
            sucesso = SetPQVTGeradorEquil(gerador, np, str_sup);
            }
         break;

      case eqptoSUPRIMENTO:
         suprimento = (VTSuprimento*)eqpto;
         if (suprimento->DefinidoPorFase)
            {//calcula PQVT por fase
            CalculaPQVT_porfase(str_sup, vet_sup, suprimento->Fases);
            sucesso = SetPQVTSuprimentoDeseq(suprimento, np, vet_sup);
            }
         else
            {
            sucesso = SetPQVTSuprimentoEquil(suprimento, np, str_sup);
            }
         break;

      default:
         sucesso = false;
      }
   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TForBasico::SetPQVTGeradorDeseq(VTGerador *gerador, int np, strSUP vet_sup[MAX_FASE])
   {
   //atualiza Gerador
   return(calcula->SetValorGeradorReducao(gerador, np, vet_sup));
   }

//---------------------------------------------------------------------------
bool __fastcall TForBasico::SetPQVTGeradorEquil(VTGerador *gerador, int np, strSUP *str_sup)
   {
   //atualiza Gerador
   return(calcula->SetValorGerador(gerador, np, str_sup));
   }

//---------------------------------------------------------------------------
bool __fastcall TForBasico::SetPQVTSuprimentoDeseq(VTSuprimento *suprimento, int np, strSUP vet_sup[MAX_FASE])
   {
   //atualiza Suprimento
   return(calcula->SetValorSuprimentoReducao(suprimento, np, vet_sup));
   }

//---------------------------------------------------------------------------
bool __fastcall TForBasico::SetPQVTSuprimentoEquil(VTSuprimento *suprimento, int np, strSUP *str_sup)
   {
   //atualiza Suprimento
   return(calcula->SetValorSuprimento(suprimento, np, str_sup));
   }

//---------------------------------------------------------------------------
bool __fastcall TForBasico::SetSmvaGerador(VTGerador *gerador, int np, complex<double> s_mva)
   {
   //variáveis locais
   strSUP str_sup;

   //obtém valores atuais de PQVT do Gerador
   if (! calcula->GetValorGerador(gerador, np, &str_sup)) return(false);
   //altera P e Q
   str_sup.p = s_mva.real();
   str_sup.q = s_mva.imag();
   //atualiza valores do Gerador
   if (! calcula->SetValorGerador(gerador, np, &str_sup)) return(false);
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TForBasico::SetSmvaSuprimento(VTSuprimento *suprimento, int np, complex<double> s_mva)
   {
   //variáveis locais
   strSUP str_sup;

   //obtém valores atuais de PQVT do Suprimento
   if (! calcula->GetValorSuprimento(suprimento, np, &str_sup)) return(false);
   //altera P e Q
   str_sup.p = s_mva.real();
   str_sup.q = s_mva.imag();
   //atualiza valores do Suprimento
   if (! calcula->SetValorSuprimento(suprimento, np, &str_sup)) return(false);
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TForBasico::SetSmvaSuprimentoReducao(VTSuprimento *suprimento, int np, complex<double> s_mva)
   {
   //variáveis locais
   double p_mw, q_mvar;
   strSUP str_sup[MAX_FASE];

   //obtém valores PQVT do Suprimento
   if (! calcula->GetValorSuprimentoReducao(suprimento, np, str_sup)) return(false);
   //divide potência aparente entre as fases do Suprimento
   p_mw   = s_mva.real() / 3.;
   q_mvar = s_mva.imag() / 3.;
   for (int nf = indFASE_A; nf <= indFASE_C; nf++)
      {
      str_sup[nf].p = p_mw;
      str_sup[nf].q = q_mvar;
      }
   //zera potência aparente do neutro
   str_sup[indNEUTRO].p = 0.;
   str_sup[indNEUTRO].q = 0.;
   //atualiza valores do Suprimento da Reducao
   if (! calcula->SetValorSuprimentoReducao(suprimento, np, str_sup)) return(false);
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TForBasico::SetUmValorGerador(VTGerador *gerador, int np, int ind_valor, double &valor)
   {
   //variáveis locais
   strSUP str_sup;

   //obtém valores atuais de PQVT do Gerador
   if (! calcula->GetValorGerador(gerador, np, &str_sup)) return(false);
   //atualiza somente o valor indicado
   switch(ind_valor)
      {
      case indP: str_sup.p = valor; break;
      case indQ: str_sup.q = valor; break;
      case indV: str_sup.v = valor; break;
      case indT: str_sup.t = valor; break;
      default:   return(false);
      }
   //atualiza valores do Gerador
   if (! calcula->SetValorGerador(gerador, np, &str_sup)) return(false);
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TForBasico::SetUmValorSuprimento(VTSuprimento *suprimento, int np, int ind_valor, double &valor)
   {
   //variáveis locais
   strSUP str_sup;

   //obtém valores atuais de PQVT do Gerador
   if (! calcula->GetValorSuprimento(suprimento, np, &str_sup)) return(false);
   //atualiza somente o valor indicado
   switch(ind_valor)
      {
      case indP: str_sup.p = valor; break;
      case indQ: str_sup.q = valor; break;
      case indV: str_sup.v = valor; break;
      case indT: str_sup.t = valor; break;
      default:   return(false);
      }
   //atualiza valores do Suprimento
   if (! calcula->SetValorSuprimento(suprimento, np, &str_sup)) return(false);
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TForBasico::SetUmValorSuprimentoReducao(VTSuprimento *suprimento, int np, int ind_valor, double &valor)
   {
   //variáveis locais
   double defasagem[MAX_FASE] =  {0., -2.*M_PI/3., 2.*M_PI/3., 0.};
   strSUP str_sup[MAX_FASE];

   //obtém valores PQVT do Suprimento
   if (! calcula->GetValorSuprimentoReducao(suprimento, np, str_sup)) return(false);
   //retorna parâmetro solicitado
   switch(ind_valor)
      {
      case indP: //potências ativas
         for (int nf = indFASE_A; nf <= indFASE_C; nf++)
            {str_sup[nf].p = valor / 3.;}
         str_sup[indNEUTRO].p = 0.;
         break;
      case indQ: //potências reativas
         for (int nf = indFASE_A; nf <= indFASE_C; nf++)
            {str_sup[nf].q = valor / 3.;}
         str_sup[indNEUTRO].q = 0.;
         break;
      case indV:  //módulo da tensão
         for (int nf = indFASE_A; nf <= indFASE_C; nf++)
            {str_sup[nf].v = valor;}
         str_sup[indNEUTRO].v = 0.;
         break;
      case indT:  //ângulo da fase
         for (int nf = indFASE_A; nf <= indFASE_C; nf++)
            {str_sup[nf].t = valor + defasagem[nf];}
         str_sup[indNEUTRO].t = 0.;
         break;
      default: return(false);
      }
   //atualiza valores do Suprimento da Reducao
   if (! calcula->SetValorSuprimentoReducao(suprimento, np, str_sup)) return(false);
   return(true);
   }

//---------------------------------------------------------------------------
//eof
