//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TCrescimentoJSON.h"
#include "TPeriodo.h"
#include "..\Classes\VTClasse.h"
#include "..\Constante\Const.h"
#include "..\Rede\VTFases.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTCrescimentoJSON* __fastcall NewObjCrescimentoJSON(void)
   {
   try{
      return(new TCrescimentoJSON());
      }catch(Exception &e)
         {
         }
   return(NULL);
   }

//---------------------------------------------------------------------------
__fastcall TCrescimentoJSON::TCrescimentoJSON(void)
   {
   try{//cria lista de Pontos
	  lisTAXA_JSON = new TList();
      }catch(Exception &e)
         {
         }
   }

//---------------------------------------------------------------------------
__fastcall TCrescimentoJSON::~TCrescimentoJSON(void)
   {
   //destrói lista e seus objetos
   if (lisTAXA_JSON) {LimpaTList(lisTAXA_JSON); delete lisTAXA_JSON; lisTAXA_JSON = NULL;}
   }


////---------------------------------------------------------------------------
//TPeriodo* __fastcall TCrescimentoJSON::ExistePeriodo(int ano)
//   {
//   //variáveis locais
//   TPeriodo *periodo;
//
//   //procura Periodo que contém ano indicado
//   for (int n = 0; n < lisPERIODO->Count; n++)
//      {
//      periodo = (TPeriodo*)lisPERIODO->Items[n];
//      if (periodo->ContemAno(ano)) return(periodo);
//      }
//   return(NULL);
//   }
//
//---------------------------------------------------------------------------
bool __fastcall TCrescimentoJSON::InsereTaxa(VTTaxaJSON *taxaJSON)
   {
   //proteção
   if (taxaJSON == NULL) return(false);
   //insere Crescimento na lista
   if (lisTAXA_JSON->IndexOf(taxaJSON) < 0)
	{
		lisTAXA_JSON->Add(taxaJSON);
	}
   return(true);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TCrescimentoJSON::PM_GetClasse(void)
   {
   return(classeNome);
   }

   //---------------------------------------------------------------------------
TList* __fastcall TCrescimentoJSON::PM_GetListTaxa(void)
   {
   return(lisTAXA_JSON);
   }


//---------------------------------------------------------------------------
void __fastcall TCrescimentoJSON::PM_SetClasse(AnsiString classe)
   {
   classeNome = classe;
   }


////---------------------------------------------------------------------------
//void __fastcall TCrescimentoJSON::RedefinePeriodo(int ano_ini, int ano_fim)
//   {
//   //variáveis locais
//   VTPeriodo *periodo;
//
//   //elimina Periodos fora do novo intervalo
//   for (int n = lisPERIODO->Count - 1; n >= 0; n--)
//      {
//      periodo = (VTPeriodo*)lisPERIODO->Items[n];
//      if ((ano_fim < periodo->AnoInicial)||(ano_ini > periodo->AnoFinal))
//         {//elimina o Periodo
//         lisPERIODO->Delete(n);
//         }
//      }
//   //loop p todos anos
//   for (int ano = ano_ini; ano <= ano_fim; ano++)
//      {//verifica se existe o Periodo p/ o ano
//      if ((periodo = ExistePeriodo(ano)) != NULL) continue;
//      //insere um novo Periodo com taxa nula
//      InserePeriodo(ano, ano, 0.);
//      }
//	//cancela valores de taxa acumulada
//   if (taxa_acum.Length > 0) {taxa_acum.Length = 0;}
//   }
//
////---------------------------------------------------------------------------
//void __fastcall TCrescimentoJSON::Reinicia(void)
//   {
//   //reinicia lisPERIODO
//   LimpaTList(lisPERIODO);
//	//cancela valores de taxa acumulada
//   if (taxa_acum.Length > 0) {taxa_acum.Length = 0;}
//   }
//

//---------------------------------------------------------------------------
//eof
