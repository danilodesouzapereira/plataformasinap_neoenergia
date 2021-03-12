//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TLegenda.h"
#include "VTMonta.h"
#include "..\Rede\VTEqpto.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//-----------------------------------------------------------------------------
// função global
//-----------------------------------------------------------------------------
VTLegenda* NewObjLegenda(VTApl *apl)
   {
   try{
      return(new TLegenda(apl));
      }catch(Exception &e)
         {
         }
   return(NULL);
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18686973
__fastcall TLegenda::TLegenda(VTApl *apl)
   {
   try{//cria lista p/ montar objetos VTMonta
      lisMONTA = new TList();
      //cria objetos VTMonta
	  lisMONTA->Add(monta_barra       = NewObjMontaBarra(apl));
	  lisMONTA->Add(monta_bateria	  = NewObjMontaBateria(apl));
      lisMONTA->Add(monta_capacitor   = NewObjMontaCapacitor(apl));
      lisMONTA->Add(monta_capserie    = NewObjMontaCapserie(apl));
      lisMONTA->Add(monta_carga       = NewObjMontaCarga(apl));
      lisMONTA->Add(monta_chave       = NewObjMontaChave(apl));
      lisMONTA->Add(monta_gerador     = NewObjMontaGerador(apl));
      //lisMONTA->Add(monta_canal       = NewObjMontaCanal(apl));
      lisMONTA->Add(monta_medidor     = NewObjMontaMedidor(apl));
      lisMONTA->Add(monta_mutua       = NewObjMontaMutua(apl));
      lisMONTA->Add(monta_reator      = NewObjMontaReator(apl));
      lisMONTA->Add(monta_regulador   = NewObjMontaRegulador(apl));
      lisMONTA->Add(monta_reducao     = NewObjMontaReducao(apl));
      lisMONTA->Add(monta_suprimento  = NewObjMontaSuprimento(apl));
      lisMONTA->Add(monta_trafo       = NewObjMontaTrafo(apl));
      lisMONTA->Add(monta_trafoZZ     = NewObjMontaTrafoZZ(apl));
      lisMONTA->Add(monta_trafo3E     = NewObjMontaTrafo3E(apl));
      lisMONTA->Add(monta_trecho      = NewObjMontaTrecho(apl));
      lisMONTA->Add(monta_yref        = NewObjMontaYref(apl));
      }catch (Exception &e)
         {}
   }

//---------------------------------------------------------------------------
__fastcall TLegenda::~TLegenda(void)
   {
   //destrói lista e seus objetos
   if (lisMONTA) {LimpaTList(lisMONTA); delete lisMONTA; lisMONTA = NULL;}
   }

//---------------------------------------------------------------------------
VTGrupo* __fastcall TLegenda::IniciaGrupo(VTEqpto *eqpto)
   {
   //variáveis locais
    VTMonta *monta;
    VTGrupo *grupo = NULL;

   //determina TMonta<eqpto> em função do Eqpto
   if ((monta = SelecionaMonta(eqpto)) != NULL)
      {//monta Grupo
      grupo = monta->IniciaGrupo(eqpto);
      }
   return(grupo);
   }

//---------------------------------------------------------------------------
bool __fastcall TLegenda::LisAtributos(VTEqpto *eqpto, TList *lisATR)
   {
   //variáveis locais
    VTMonta *monta;

   //reinicia lisATR
   LimpaTList(lisATR);
   //determina TMonta<eqpto> em função do Eqpto
   if ((monta = SelecionaMonta(eqpto)) != NULL)
      {//monta lista de atributos
      monta->LisAtributos(eqpto, lisATR);
      }
   return(lisATR->Count > 0);
   }

//---------------------------------------------------------------------------
bool __fastcall TLegenda::LisAtributosEqpto(VTEqpto *eqpto, TList *lisATR)
   {
   //variáveis locais
    VTMonta *monta;

   //reinicia lisATR
   LimpaTList(lisATR);
   //determina TMonta<eqpto> em função do Eqpto
   if ((monta = SelecionaMonta(eqpto)) != NULL)
      {//monta lista de atributos
      monta->LisAtributosEqpto(eqpto, lisATR);
      }
   return(lisATR->Count > 0);
   }

//---------------------------------------------------------------------------
bool __fastcall TLegenda::LisValores(VTEqpto *eqpto, TList *lisDADO, int ano, int num_pat)
   {
   //variáveis locais
    VTMonta *monta;

   //reinicia lisDADO
   LimpaTList(lisDADO);
   //determina TMonta<eqpto> em função do Eqpto
   if ((monta = SelecionaMonta(eqpto)) != NULL)
      {//monta lista de valores
      monta->LisValores(eqpto, lisDADO, ano, num_pat);
      }
   return(lisDADO->Count > 0);
   }

//---------------------------------------------------------------------------
bool __fastcall TLegenda::LisValoresEqpto(VTEqpto *eqpto, TList *lisDADO, int ano, int num_pat)
   {
   //variáveis locais
    VTMonta *monta;

   //reinicia lisDADO
   LimpaTList(lisDADO);
   //determina TMonta<eqpto> em função do Eqpto
   if ((monta = SelecionaMonta(eqpto)) != NULL)
      {//monta lista de valores do Eqpto
      monta->LisValoresEqpto(eqpto, lisDADO, ano, num_pat);
      }
   return(lisDADO->Count > 0);
   }

//---------------------------------------------------------------------------
bool __fastcall TLegenda::LisValoresFluxo(VTEqpto *eqpto, TList *lisDADO, int ano, int num_pat)
   {
   //variáveis locais
    VTMonta *monta;

   //reinicia lisDADO
   LimpaTList(lisDADO);
   //determina TMonta<eqpto> em função do Eqpto
   if ((monta = SelecionaMonta(eqpto)) != NULL)
      {//monta lista de valores de fluxo
      monta->LisValoresFluxo(eqpto, lisDADO, ano, num_pat);
      }
   return(lisDADO->Count > 0);
   }

//---------------------------------------------------------------------------
bool __fastcall TLegenda::LisVetValEqpto(VTEqpto *eqpto, TList *lisVET, int ind_atrib)
   {
   //variáveis locais
    VTMonta *monta;

   //reinicia lisDADO
   LimpaTList(lisVET);
   //determina TMonta<eqpto> em função do Eqpto
   if ((monta = SelecionaMonta(eqpto)) != NULL)
      {//monta lista de valores de fluxo
	  monta->LisVetValEqpto(eqpto, lisVET, ind_atrib);
      }
   return(lisVET->Count > 0);
   }

//---------------------------------------------------------------------------
bool __fastcall TLegenda::LisVetValFluxo(VTEqpto *eqpto, TList *lisVET, int ind_atrib, int ano)
   {
   //variáveis locais
    VTMonta *monta;

   //reinicia lisDADO
   LimpaTList(lisVET);
   //determina TMonta<eqpto> em função do Eqpto
   if ((monta = SelecionaMonta(eqpto)) != NULL)
      {//monta lista de valores de fluxo
      monta->LisVetValFluxo(eqpto, lisVET, ind_atrib, ano);
      }
   return(lisVET->Count > 0);
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TLegenda::LisVetValCanal(VTCanal *canal, TList *lisVET, int ind_atrib, int ano)
   {
   //reinicia lisDADO
   LimpaTList(lisVET);
   //monta lista de valores de acordo c/ o tipo de eqpto
   monta_canal->LisVetValCanal(canal, lisVET, ind_atrib, ano);
   return(lisVET->Count > 0);
   }
*/
//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18686973
VTMonta* __fastcall TLegenda::SelecionaMonta(VTEqpto *eqpto)
   {
   switch(eqpto->Tipo())
      {
	  case eqptoBARRA:      return(monta_barra);
	  case eqptoBATERIA:	return(monta_bateria);
	  case eqptoCANAL:      return(monta_medidor);
      case eqptoCAPACITOR:  return(monta_capacitor);
      case eqptoCAPSERIE:   return(monta_capserie);
      case eqptoCARGA:      return(monta_carga);
      case eqptoCHAVE:      return(monta_chave);
      case eqptoGERADOR:    return(monta_gerador);
      case eqptoMEDIDOR:    return(monta_medidor);
      case eqptoMUTUA:      return(monta_mutua);
      case eqptoREATOR:     return(monta_reator);
      case eqptoREGULADOR:  return(monta_regulador);
      case eqptoREDUCAO:    return(monta_reducao);
      case eqptoSUPRIMENTO: return(monta_suprimento);
      case eqptoTRAFO:      return(monta_trafo);
      case eqptoTRAFOZZ:    return(monta_trafoZZ);
      case eqptoTRAFO3E:    return(monta_trafo3E);
      case eqptoTRECHO:     return(monta_trecho);
	  case eqptoYREF:       return(monta_yref);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TLegenda::ValorAtributo(VTEqpto *eqpto, int ind_atrib)
   {
   //variáveis locais
   AnsiString valor;
   VTMonta *monta;

   //determina TMonta<eqpto> em função do Eqpto
   if ((monta = SelecionaMonta(eqpto)) != NULL)
      {//determina valor do atributo
      valor = monta->ValorAtributo(eqpto, ind_atrib);
      }
   return(valor);
   }

//---------------------------------------------------------------------------
//eof


