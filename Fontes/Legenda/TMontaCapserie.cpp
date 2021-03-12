//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#pragma hdrstop
#include "TMontaCapserie.h"
#include "TAtributo.h"
#include "VTDado.h"
#include "VTGrupo.h"
#include "..\Arranjo\VTArranjo.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCapserie.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTMonta* __fastcall NewObjMontaCapserie(VTApl *apl)
   {
   try{
      return (new TMontaCapserie(apl));
      }catch (Exception &e)
         {//nada a fazer
         }
   return(NULL);
   }

//---------------------------------------------------------------------------
__fastcall TMontaCapserie::TMontaCapserie(VTApl *apl)
                        :TMontaLigacao(apl)
   {
   //cria objetos
   monta_arranjo = NewObjMontaArranjo(apl);
   }

//---------------------------------------------------------------------------
__fastcall TMontaCapserie::~TMontaCapserie(void)
   {
   //destrói objetos
   if (monta_arranjo) {delete monta_arranjo; monta_arranjo = NULL;}
   }

//---------------------------------------------------------------------------
VTGrupo* __fastcall TMontaCapserie::IniciaGrupo(VTEqpto *eqpto)
   {
   //monta grupo c/ dados do Capserie
   return(TMontaLigacao::IniciaGrupo(eqpto));
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaCapserie::LisAtributos(VTEqpto *eqpto, TList *lisATR)
   {
   //salva ponteiro p/ Capserie
   capserie = (VTCapserie*)eqpto;
   //insere atributos do Eqpto
   TMontaEqpto::LisAtributosBasico(eqpto, lisATR);
   LisAtributosEqpto(eqpto, lisATR);
   //insere atributos de fluxo
   LisAtributosFluxo(eqpto, lisATR);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaCapserie::LisAtributosEqpto(VTEqpto *eqpto, TList *lisATR)
   {
   //salva ponteiro p/ Capserie
   capserie = (VTCapserie*)eqpto;
   //atributos de Eqpto
   TMontaLigacao::LisAtributosEqpto(eqpto, lisATR);
   //atributos de Capserie
   lisATR->Add(new TAtributo(capserie, caps_SNOM, true, false, "Capacidade", "(MVA)"));
   lisATR->Add(new TAtributo(capserie, caps_X,    true, false, "Reatância", "(ohm)"));

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaCapserie::LisAtributosFluxo(VTEqpto *eqpto, TList *lisATR)
   {
   //salva ponteiro p/ Capserie
   capserie = (VTCapserie*)eqpto;
   //atributos de fluxo da Ligacao
   TMontaLigacao::LisAtributosFluxo(eqpto, lisATR);
   //atributos de fluxo de Capserie

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaCapserie::LisValores(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat)
   {
   //monta lista de valores do Eqpto
   LisValoresEqpto(eqpto, lisEXT, ano, num_pat);
   //monta lista de valores de Fluxo
   LisValoresFluxo(eqpto, lisEXT, ano, num_pat);
   return(lisEXT->Count > 0);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaCapserie::LisValoresEqpto(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat)
   {
   //salva parâmetros
   this->capserie     = (VTCapserie*)eqpto;
   try{//dados do Eqpto
      TMontaLigacao::LisValoresEqpto(eqpto, lisEXT, ano, num_pat);
      //dados gerais do capserie
      if (capserie->Show[caps_SNOM])     ValorCapacidade();
      if (capserie->Show[caps_X])        ValorReatancia();
      }catch(Exception &e)
         {
         }
   return(lisEXT->Count > 0);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall  TMontaCapserie::ValorAtributo(VTEqpto *eqpto, int ind_atrib)
   {
   //variáveis locais
   AnsiString  valor, unidade;
   VTCapserie *capserie = (VTCapserie*)eqpto;

   //determina valor de atributo
   switch(ind_atrib)
      {
      case eqp_ID:     valor = IntToStr(eqpto->Id);                         break;
      case eqp_CODIGO: valor = eqpto->Codigo;                               break;
      case caps_SNOM:  aue.PotAparente_MVA(capserie->snom, valor, unidade); break;
      case caps_X:     aue.Impedancia_OHM(capserie->z.x, valor, unidade);   break;
      }
   return(valor);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaCapserie::ValorCapacidade(void)
   {
   //variáveis locais
   VTDado *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Capacidade"));
   //define unidade e valor
   aue.PotAparente_MVA(capserie->snom, dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaCapserie::ValorReatancia(void)
   {
   //variáveis locais
   VTDado *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("X"));
   aue.Impedancia_OHM(capserie->z.x, dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
//eof



