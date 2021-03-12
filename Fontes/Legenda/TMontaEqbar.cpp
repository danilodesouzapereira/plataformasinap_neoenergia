//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#pragma hdrstop
#include "TMontaEqbar.h"
#include "TAtributo.h"
#include "VTDado.h"
#include "VTVetval.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTEqbar.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TMontaEqbar::TMontaEqbar(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
__fastcall TMontaEqbar::~TMontaEqbar(void)
   {
   //nada a fazer
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TMontaEqbar::LisAtributos(VTEqpto *eqpto, TList *lisATR)
   {
   //salva parâmetros
   this->eqbar = (VTEqbar*)eqpto;
   //insere atributos de Eqpto
   TMontaEqpto::LisAtributosBasico(eqbar, lisATR);
   TMontaEqpto::LisAtributosEqpto(eqbar, lisATR);
   LisAtributosEqpto(eqbar, lisATR);
   //insere atributos do Fluxo
   LisAtributosFluxo(eqpto, lisATR);
   //insere atributos de fluxo
   return(true);
   }
*/
//---------------------------------------------------------------------------
bool __fastcall TMontaEqbar::LisAtributosEqpto(VTEqpto *eqpto, TList *lisATR)
   {
   //salva parâmetros
   this->eqbar = (VTEqbar*)eqpto;
   //insere atributos de Eqpto
   TMontaEqpto::LisAtributosEqpto(eqpto, lisATR);
   //insere atributos de Eqpbar
   lisATR->Add(new TAtributo(eqbar, eqb_BARRA, true,  false, "Barra"));
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaEqbar::LisAtributosFluxo(VTEqpto *eqpto, TList *lisATR)
   {
   //nada a fazer
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaEqbar::LisValoresEqpto(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat)
   {
   //salva parâmetros
   this->eqbar = (VTEqbar*)eqpto;
   //monta lista de valores do Eqpto
   TMontaEqpto::LisValoresEqpto(eqpto, lisEXT, ano, num_pat);
   //monta lista de valores do Eqbar
   try{//dados do Eqbar
      if (eqbar->Show[eqb_BARRA])  ValorCodigoBarra();
      }catch(Exception &e)
         {
         }
   return(lisEXT->Count > 0);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaEqbar::LisVetValEqpto(VTEqpto *eqpto, TList *lisEXT, int ind_atrib)
   {
   //salva parâmetros
   this->eqbar  = (VTEqbar*)eqpto;
   //insere dados do Eqpto
   TMontaEqpto::LisVetValEqpto(eqpto, lisEXT, ind_atrib);
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaEqbar::LisVetValFluxo(VTEqpto *eqpto, TList *lisEXT, int ind_atrib, int ano)
   {
   //salva parâmetros
   this->eqbar  = (VTEqbar*)eqpto;
   //insere dados do Eqpto
   TMontaEqpto::LisVetValFluxo(eqpto, lisEXT, ind_atrib, ano);
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaEqbar::ValorCodigoBarra(void)
   {
   //variáveis locais
   VTDado *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Barra"));
   //define unidade e valor
   dado->valor = eqbar->pbarra->Codigo;
   }

//---------------------------------------------------------------------------
//eof


