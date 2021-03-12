//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#pragma hdrstop
#include "TMontaEqpto.h"
#include "TAtributo.h"
#include "VTDado.h"
#include "..\Rede\VTEqpto.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TMontaEqpto::TMontaEqpto(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
__fastcall TMontaEqpto::~TMontaEqpto(void)
   {
   //nada a fazer
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TMontaEqpto::LisAtributos(VTEqpto *eqpto, TList *lisATR)
   {
   //salva parâmetros
   this->eqpto  = eqpto;
   //insere atributos básicos
   LisAtributosBasico(eqpto, lisATR);
   //insere atributos do Eqpto
   LisAtributosEqpto(eqpto, lisATR);
   //insere atributos de fluxo
   LisAtributosFluxo(eqpto, lisATR);
   return(true);
   }
*/
//---------------------------------------------------------------------------
bool __fastcall TMontaEqpto::LisAtributosBasico(VTEqpto *eqpto, TList *lisATR)
   {
   //salva parâmetros
   this->eqpto  = eqpto;
   lisATR->Add(new TAtributo(eqpto, eqp_VISIBLE, false, false, "Visível"));
   lisATR->Add(new TAtributo(eqpto, eqp_FIXO,    false, false, "Exibir sempre"));
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaEqpto::LisAtributosEqpto(VTEqpto *eqpto, TList *lisATR)
   {
   //salva parâmetros
   this->eqpto  = eqpto;
   lisATR->Add(new TAtributo(eqpto, eqp_ID,      true,  false, "ID"));
	lisATR->Add(new TAtributo(eqpto, eqp_CODIGO,  true,  false, "Código"));
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaEqpto::LisAtributosFluxo(VTEqpto *eqpto, TList *lisATR)
   {
   //nada a fazer
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaEqpto::LisValoresCurva(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat)
   {
   //método a ser definido nas classes derivadas
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaEqpto::LisValoresEqpto(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat)
   {
   //salva parâmetros
   this->eqpto  = eqpto;
   this->lisEXT = lisEXT;
   this->ano     = ano;
   this->num_pat = num_pat;
   //monta lista de valores do Eqpto
   try{//dados do Eqpto
      if (eqpto->Show[eqp_ID])       ValorId();
      if (eqpto->Show[eqp_CODIGO])   ValorCodigo();
      }catch(Exception &e)
         {
         }
   return(lisEXT->Count > 0);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaEqpto::LisValoresFluxo(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat)
   {
   //salva parâmetros
   this->eqpto  = eqpto;
   this->lisEXT = lisEXT;
   this->ano     = ano;
   this->num_pat = num_pat;
   //nada a fazer
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaEqpto::LisVetValEqpto(VTEqpto *eqpto, TList *lisEXT, int ind_atrib)
   {
   //salva parâmetros
   this->eqpto  = eqpto;
   this->lisEXT = lisEXT;
   this->ano    = 0;
   //nada a fazer
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaEqpto::LisVetValFluxo(VTEqpto *eqpto, TList *lisEXT, int ind_atrib, int ano)
   {
   //salva parâmetros
   this->eqpto  = eqpto;
   this->lisEXT = lisEXT;
   this->ano    = ano;
   //nada a fazer
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaEqpto::ValorCodigo(void)
   {
   //variáveis locais
   VTDado *dado;

   //cria um novo Dado e insere em lisEXT
	//lisEXT->Add(dado = NewObjDado("Código"));
	lisEXT->Add(dado = NewObjDado(""));
	//define unidade e valor
   dado->valor   = eqpto->Codigo;
   dado->unidade = "";
   }

//---------------------------------------------------------------------------
void __fastcall TMontaEqpto::ValorId(void)
   {
   //variáveis locais
   VTDado *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Id"));
   //define unidade e valor
   dado->valor   = IntToStr(eqpto->Id);
   dado->unidade = "";
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TMontaEqpto::ValorAtributo(VTEqpto *eqpto, int ind_atrib)
   {
   //variáveis locais
   AnsiString valor;
   
   switch(ind_atrib)
      {
      case eqp_ID:     valor = IntToStr(eqpto->Id); break;
      case eqp_CODIGO: valor = eqpto->Codigo;       break;
      }
   return(valor);
   }

//---------------------------------------------------------------------------
//eof


