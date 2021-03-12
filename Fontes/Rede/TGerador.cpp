//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#pragma hdrstop
#include "TGerador.h"
#include "VTResFlowSup.h"
#include "..\Complexo\Complexo.h"
#include "..\Curvas\VTCurva.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTGerador* __fastcall NewObjGerador(int num_pat)
   {
   return(new TGerador(num_pat));
   }

//---------------------------------------------------------------------------
__fastcall TGerador::TGerador(int num_pat)
   {
   //inicia dados
   tiposup      = supPV;
   FonteEnergia = gerVAPOR;
   Fases        = faseABC;
   //inicia atributos p/ visualização gráfica
   Show[eqp_FIXO]   = true;
   Show[eqp_CODIGO] = true;
   Show[ger_TIPO]   = true;
   //cria objetos
   resflow = NewObjResFlowSup(this, num_pat);
   }

//---------------------------------------------------------------------------
__fastcall TGerador::~TGerador(void)
   {
   //destrói objetos
   if (resflow) {delete resflow; resflow = NULL;}
   //destrói Curva somente se não for típica
   if ((curva != NULL)&&(! curva->Tipica)) delete curva;
   curva = NULL;
   }

//---------------------------------------------------------------------------
VTGerador* __fastcall TGerador::Clone(void)
   {
   //variáveis locais
   VTGerador *gerador;

   //cria umum novo Gerador
   if ((gerador = NewObjGerador(resflow->Max_pat)) != NULL)
      {//copia dados deste Gerador p/ o novo Gerador
      gerador->CopiaAtributosDe(*this);
      }
   return(gerador);
   }

//---------------------------------------------------------------------------
void __fastcall TGerador::CopiaAtributosDe(VTEqpto &ref)
   {
   //variáveis locais
   TGerador &gerador = (TGerador&)ref;

   //proteção
   if (ref.Tipo() != eqptoGERADOR) return;
   //copia atributos de TEqbar
   TEqbar::CopiaAtributosDe(gerador);
   //copia atributos de VTGerador
   PD      = gerador.PD;
   tiposup = gerador.tiposup;
   ligacao = gerador.ligacao;
   //vnom    = gerador.vnom;
   //snom    = gerador.snom;
   fplim   = gerador.fplim;
   z0      = gerador.z0;
   z1      = gerador.z1;
   //copia atributos de VTResFlowSup
   resflow->CopiaAtributosDe(*gerador.resflow);
   //cria cópia da Curva, destruindo eventual instância anterior
   if ((curva)&&(! curva->Tipica)) delete curva;
   //verifica se o gerador de referência possui Curva típica
   if (gerador.curva->Tipica)
      {//assume a mesma Curva típica
      curva = gerador.curva;
      }
   else
      {//cria clone da Curva
      curva = gerador.curva->Clone();
      curva->Eqpto = this;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TGerador::DefineCurva(VTCurva *nova_curva)
   {
   //verifica se já existe uma Curva própria
   if ((curva != NULL)&&(curva != nova_curva)&&(! curva->Tipica))
      {//destrói Curva anterioratual
      delete curva;
      }
   //salva ponteiro p/ nova curva
   curva = nova_curva;
   }

//---------------------------------------------------------------------------
VTCurva* __fastcall TGerador::ExisteCurva(void)
   {
   return(curva);
   }

//---------------------------------------------------------------------------
double __fastcall TGerador::FornecimentoMVA(int num_pat)
   {
   return(Abs(resflow->S_mva[num_pat]));
   }
   
//---------------------------------------------------------------------------
bool __fastcall TGerador::PM_GetDefinidoPorFase(void)
   {
   //verifica se a Curva é definida por fase
   return((curva != NULL)&&(curva->NumeroValores == nvCURVA_RRSUP));
   }

//---------------------------------------------------------------------------
int __fastcall TGerador::PM_GetFases(void)
   {
   return(PD.fases);
   }

//---------------------------------------------------------------------------
int __fastcall TGerador::PM_GetFonte(void)
   {
   return(PD.fonte);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TGerador::PM_GetFonteStr(void)
   {
   switch(FonteEnergia)
      {
      case gerDIESEL:     return("Diesel");
      case gerPCH:        return("PCH");
      case gerVAPOR:      return("Vapor");
      case gerEOLICA:     return("Eólica");
      case gerSOLAR:      return("Fotovoltaica");
      //case gerTGAS:       return("Gás");
      //case gerHIDRAULICA: return("Hidráulica");
      default:            return("Outros");
      }
   }

//---------------------------------------------------------------------------
double __fastcall TGerador::PM_GetSnom_mva(void)
   {
   return(PD.snom_mva);
   }

//---------------------------------------------------------------------------
double __fastcall TGerador::PM_GetVnom_kv(void)
   {
   return(PD.vnom_kv);
   }

//---------------------------------------------------------------------------
void __fastcall TGerador::PM_SetFases(int fases)
   {
   PD.fases = fases;
   }

//---------------------------------------------------------------------------
void __fastcall TGerador::PM_SetFonte(int tipo)
   {
   //proteção
   //if ((tipo < gerDIESEL) || (tipo > gerHIDRAULICA)) tipo = gerVAPOR;
   if ((tipo < gerDIESEL) || (tipo > gerSOLAR)) tipo = gerPCH;
   //salva tipo
   PD.fonte = tipo;
   }

//---------------------------------------------------------------------------
void __fastcall TGerador::PM_SetSnom_mva(double snom_mva)
   {
   PD.snom_mva = snom_mva;
   }

//---------------------------------------------------------------------------
void __fastcall TGerador::PM_SetVnom_kv(double vnom_kv)
   {
   PD.vnom_kv = vnom_kv;
   }

//---------------------------------------------------------------------------
int __fastcall TGerador::Tipo(void)
   {
   return(eqptoGERADOR);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TGerador::TipoAsString(void)
   {
   return("Gerador");
   }

//---------------------------------------------------------------------------
//eof
