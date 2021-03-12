//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TCustos.h"
#include "VTCustoEqpto.h"
#include "VTCustoNodal.h"
//#include "..\BDado\VTBDcustonodal.h"
#include "..\Rede\VTEqpto.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTCustos* __fastcall NewObjCustos(VTApl *apl)
   {
   return(new TCustos(apl));
   }

//---------------------------------------------------------------------------
__fastcall TCustos::TCustos(VTApl *apl)
   {
   try{//salva ponteiro p/ objetos
      this->apl = apl;
      //cria listas
      lisCUSTO_EQP = new TList();
      lisCUSTO_NOD = new TList();
      }catch(Exception &e)
         {
         }
   }

//---------------------------------------------------------------------------
__fastcall TCustos::~TCustos(void)
   {
   //destrói somente Custos típicas
   if (lisCUSTO_EQP) {LimpaTList(lisCUSTO_EQP); delete lisCUSTO_EQP; lisCUSTO_EQP = NULL;}
   if (lisCUSTO_NOD) {LimpaTList(lisCUSTO_NOD); delete lisCUSTO_NOD; lisCUSTO_NOD = NULL;}
   }

//---------------------------------------------------------------------------
VTCustoEqpto* __fastcall TCustos::ExisteCustoEqpto(VTEqpto *eqpto)
   {
   //variáveis locais
   VTCustoEqpto *custo_eqpto;

   for (int n = 0; n < lisCUSTO_EQP->Count; n++)
      {
      custo_eqpto = (VTCustoEqpto*)lisCUSTO_EQP->Items[n];
      if (custo_eqpto->eqpto == eqpto) return(custo_eqpto);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
VTCustoNodal* __fastcall TCustos::ExisteCustoNodal(VTEqpto *eqpto)
   {
   //variáveis locais
   VTCustoNodal *custo_nodal;

   for (int n = 0; n < lisCUSTO_NOD->Count; n++)
      {
      custo_nodal = (VTCustoNodal*)lisCUSTO_NOD->Items[n];
      if (custo_nodal->eqpto == eqpto) return(custo_nodal);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
bool __fastcall TCustos::InsereCustoEqpto(VTCustoEqpto *custo_eqpto)
   {
   //proteção
   if (custo_eqpto == NULL) return(false);
   //insere CustoEqpto em lisCUSTO_EQP
   lisCUSTO_EQP->Add(custo_eqpto);
   return(true);
   }

//---------------------------------------------------------------------------
bool  __fastcall TCustos::InsereCustoNodal(VTCustoNodal *custo_nodal)
   {
   //proteção
   if (custo_nodal == NULL) return(false);
   //insere CustoNodal em lisCUSTO_NOD
   lisCUSTO_NOD->Add(custo_nodal);
   return(true);
   }

//---------------------------------------------------------------------------
TList* __fastcall TCustos::ListaCustoEqpto(void)
   {
   return(lisCUSTO_EQP);
   }

//---------------------------------------------------------------------------
TList* __fastcall TCustos::ListaCustoNodal(void)
   {
   return(lisCUSTO_NOD);
   }

//---------------------------------------------------------------------------
void __fastcall TCustos::Reinicia(void)
   {
   //reinicia lista de CustoNodal
   LimpaTList(lisCUSTO_NOD);
   //reinicia lista de CustoEqpto
   LimpaTList(lisCUSTO_EQP);
   }

//---------------------------------------------------------------------------
//eof



