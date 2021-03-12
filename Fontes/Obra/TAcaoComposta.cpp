//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TAcaoComposta.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTAcao* __fastcall NewObjAcaoComposta(void)
   {
   try{
      return(new TAcaoComposta());
      }catch(Exception &e)
         {
         }
   return(NULL);
   }

//---------------------------------------------------------------------------
__fastcall TAcaoComposta::TAcaoComposta(void)
   {
   //define ID inválido
   Id = -1;
   //cria lista de Acoes
   lisACAO = new TList();
   //inicia demais dados
   Pai    = NULL;
   Fixa   = false;
   Moving = false;
   //acerta o ponteiro do item obra
   ItemObra = NULL;
   AcaoClone = NULL;
   }

//---------------------------------------------------------------------------
__fastcall TAcaoComposta::~TAcaoComposta(void)
   {
   //destrói lista e seus objetos
   if (lisACAO) {LimpaTList(lisACAO); delete lisACAO; lisACAO = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TAcaoComposta::Add(VTAcao *acao)
   {
   //insere Acao na lista
   acao->Index = lisACAO->Add(acao);
   //redefine Pai da Acao
   acao->Pai = this;
   }

//---------------------------------------------------------------------------
VTEqpto* __fastcall TAcaoComposta::Eqpto(void)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
VTEqpto* __fastcall TAcaoComposta::CopiaDoEqptoOriginal(void)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
VTEqpto* __fastcall TAcaoComposta::CopiaDoEqptoAlterado(void)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
TList* __fastcall TAcaoComposta::LisAcao(void)
   {
   return(lisACAO);
   }
   
//---------------------------------------------------------------------------
VTAcao* __fastcall TAcaoComposta::PM_GetPai(void)
   {
   return(PD.pai);
   }

//---------------------------------------------------------------------------
void __fastcall TAcaoComposta::PM_SetPai(VTAcao *pai)
   {
   PD.pai = pai;
   }

//---------------------------------------------------------------------------
int __fastcall TAcaoComposta::Tipo(void)
   {
   return(acaoCOMPOSTA);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TAcaoComposta::TipoAsString(void)
   {
   return("Composta");
   }

//---------------------------------------------------------------------------
//eof

