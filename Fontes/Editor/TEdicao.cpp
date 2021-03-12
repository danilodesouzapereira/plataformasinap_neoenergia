//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TEdicao.h"
#include "TFormEdtCap.h"
#include "TFormEdtCaps.h"
//#include "TFormEdtCar.h"
//#include "TFormEdtCarIP.h"
#include "TFormEdtChv.h"
#include "TFormEdtEqptos.h"
#include "TFormEdtFiltro.h"
#include "TFormEdtGer.h"
#include "TFormEdtMedidor.h"
#include "TFormEdtMutua.h"
#include "TFormEdtReator.h"
#include "TFormEdtRegulador.h"
#include "TFormEdtSup.h"
#include "TFormEdtTre.h"
#include "TFormEdtTrf.h"
#include "TFormEdtTrfMono.h"
#include "TFormEdtTrf3E.h"
#include "TFormEdtTrfZZ.h"
#include "TFormEdtZref.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTTrafo.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTEdicao* __fastcall NewObjEdicao(VTApl *apl)
   {
   try{//cria objeto
      return(new TEdicao(apl));
      }catch(Exception &e)
         {
         return(NULL);
         }
   }


//---------------------------------------------------------------------------
__fastcall TEdicao::TEdicao(VTApl *apl)
   {
   //salva ponteiro para objeto
   this->apl = apl;
   //cria lista local
   lisEQP = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TEdicao::~TEdicao(void)
   {
   //destrói lista sem destruir seus objetos
   if (lisEQP) {delete lisEQP; lisEQP = NULL;}
   }
/*
//---------------------------------------------------------------------------
int __fastcall TEdicao::EditaEqpto(TList *lisEQP)
   {
   //variáveis locais
   int     resp;
   VTEqpto *eqpto;

   //proteção
   if (lisEQP->Count == 0) return(mrCancel);
   //verifica se todos os Eqptos são do mesmo tipo
   if (! ValidaLisEqpto(lisEQP)) return(mrCancel);
   //edita eqpto selecionado
   eqpto = (VTEqpto*)lisEQP->First();
   switch(eqpto->Tipo())
      {//barra
      case eqptoBARRA:      resp = EditaBarra(lisEQP);        break;
      //eqbar
      case eqptoCAPACITOR:  resp = EditaCapacitor(lisEQP);    break;
      case eqptoCARGA:      resp = EditaCarga(lisEQP);        break;
      case eqptoFILTRO:     resp = EditaFiltro(lisEQP);       break;
      case eqptoGERADOR:    resp = EditaGerador(lisEQP);      break;
      case eqptoMEDIDOR:    resp = EditaMedidor(lisEQP);      break;
      case eqptoREATOR:     resp = EditaReator(lisEQP);       break;
      case eqptoSUPRIMENTO: resp = EditaSuprimento(lisEQP);   break;
      case eqptoTRAFOZZ:    resp = EditaTrafoZZ(lisEQP);      break;
      case eqptoYREF:       resp = EditaYref(lisEQP);         break;
      //ligacao
      case eqptoCAPSERIE:   resp = EditaCapserie(lisEQP);     break;
      case eqptoCHAVE:      resp = EditaChave(lisEQP);        break;
      case eqptoREGULADOR:  resp = EditaRegulador(lisEQP);    break;
      case eqptoTRAFO:      resp = EditaTrafo(lisEQP);        break;
      case eqptoTRAFO3E:    resp = EditaTrafo3E(lisEQP);      break;
      case eqptoTRECHO:     resp = EditaTrecho(lisEQP);       break;
      //mutua
      case eqptoMUTUA:      resp = EditaMutua(lisEQP);        break;
      //Rede/Reducao
      case eqptoREDE:       resp = mrCancel;                  break;
      case eqptoREDUCAO:    resp = mrCancel;                  break;
      default:              resp = mrCancel;                  break;
      }
   return(resp);
   }
*/
//---------------------------------------------------------------------------
int __fastcall TEdicao::EditaEqpto(TList *lisEQP)
   {
   //variáveis locais
   int     resp = mrCancel;
   VTEqpto *eqpto;

   //proteção
   if (lisEQP->Count == 0) return(mrCancel);
   //verifica se todos os Eqptos são do mesmo tipo
   if (! ValidaLisEqpto(lisEQP)) return(mrCancel);
   //edita eqpto selecionado
   eqpto = (VTEqpto*)lisEQP->First();
   if (eqpto->Tipo() == eqptoTRAFO) resp = EditaTrafo(lisEQP);
   return(resp);
   }

//---------------------------------------------------------------------------
int __fastcall TEdicao::EditaTrafo(TList *lisEQP)
   {
   //variáveis locais
   int        resp;
   TForm      *FormEdicao;
   VTTrafo    *trafo;

   try{//verifica se Trafo monofásico
      trafo = (VTTrafo*)lisEQP->First();
      if (trafo->Monofasico)
         {//cria TFormEdtTrfMono
         FormEdicao = new TFormEdtTrfMono(NULL, apl, lisEQP, criar_copia);
         }
      else
         {//cria TFormEdtTrf
         FormEdicao = new TFormEdtTrf(NULL, apl, lisEQP, criar_copia);
         }
      resp = FormEdicao->ShowModal();
      //destrói o Form
      delete FormEdicao;
      } catch(Exception &e)
         {
         resp = mrCancel;
         }
   return(resp);
   }

//---------------------------------------------------------------------------
int __fastcall TEdicao::Executa(VTEqpto *eqpto, bool criar_copia)
   {
   //salva opção de criar cópia
   this->criar_copia = criar_copia;
   //insere Eqpto em lisEQP
   lisEQP->Clear();
   lisEQP->Add(eqpto);
   //executa edição
   return(EditaEqpto(lisEQP));
   }

//---------------------------------------------------------------------------
int __fastcall TEdicao::Executa(TList *lisEQP, bool criar_copia)
   {
   //salva opção de criar cópia
   this->criar_copia = criar_copia;
   //executa edição
   return(EditaEqpto(lisEQP));
   }

//---------------------------------------------------------------------------
bool __fastcall TEdicao::ValidaLisEqpto(TList *lisEQP)
   {
   //variáveis locais
   VTEqpto *eqpto1, *eqpto2;
   VTCarga *carga1, *carga2;
   VTTrafo *trafo1, *trafo2;

   //proteção
   if (lisEQP->Count == 0) return(false);
   //verific ase há um único Eqpto
   if (lisEQP->Count == 1) return(true);
   //verifica se todos Eqptos são do mesmo tipo
   eqpto1 = (VTEqpto*)lisEQP->First();
   for (int n = 1; n < lisEQP->Count; n++)
      {
      eqpto2 = (VTEqpto*)lisEQP->Items[n];
      //verifica tipos iguais
      if (eqpto1->Tipo() != eqpto2->Tipo())
         {
         Aviso("Não é possível editar o conjunto de equipamentos.\nOs equipamentos não são do mesmo tipo");
         return(false);
         }
      //verifica Carga e CargaIP
      if (eqpto1->Tipo() == eqptoCARGA)
         {
         carga1 = (VTCarga*)eqpto1;
         carga2 = (VTCarga*)eqpto2;
         if (carga1->IP && carga2->IP) continue;
         if ((! carga1->IP) && (! carga2->IP)) continue;
         Aviso("Não é possível editar o conjunto de equipamentos.\nHá carga e carga de IP");
         return(false);
         }
      //verifica Trafo e Trafo monofásico
      if (eqpto1->Tipo() == eqptoTRAFO)
         {
         trafo1 = (VTTrafo*)eqpto1;
         trafo2 = (VTTrafo*)eqpto2;
         if (trafo1->Monofasico && trafo2->Monofasico) continue;
         if ((! trafo1->Monofasico) && (! trafo2->Monofasico)) continue;
         Aviso("Não é possível editar o conjunto de equipamentos.\nHá transformadores trifásicos e monofásicos");
         return(false);
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
//eof
