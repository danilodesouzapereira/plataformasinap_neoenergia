//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DateUtils.hpp>
#include "TMercadoJSON.h"
#include "VTSegmentoJSON.h"
#include "VTCrescimentoJSON.h"
#include "VTTaxaJSON.h"
#include "..\Apl\VTApl.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTMercadoJSON* __fastcall NewObjMercadoJSON(VTApl *apl)
   {
   try{
		return(new TMercadoJSON(apl));
      }catch(Exception &e)
         {
         }
   return(NULL);
   }

//---------------------------------------------------------------------------
__fastcall TMercadoJSON::TMercadoJSON(VTApl *apl)
   {
   //salva objeto externo
   this->apl = apl;
   AnoFinal   = AnoInicial + 1;
   try{//cria objetos
	  lisSEGMENTO_JSON = new TList();
      }catch(Exception &e)
         {
         }
   }

//---------------------------------------------------------------------------
__fastcall TMercadoJSON::~TMercadoJSON(void)
   {
   //destrói lista e seus objetos
	if (lisSEGMENTO_JSON)	{LimpaTList(lisSEGMENTO_JSON);	delete lisSEGMENTO_JSON; lisSEGMENTO_JSON = NULL;}
   }

//---------------------------------------------------------------------------
bool __fastcall TMercadoJSON::InsereSegmentoJSON(VTSegmentoJSON *segmentoJSON)
   {
   //proteção
   if (segmentoJSON == NULL) return(false);
	if (lisSEGMENTO_JSON->IndexOf(segmentoJSON) < 0)
      {//insere Segmento na lista
		lisSEGMENTO_JSON->Add(segmentoJSON);
	  }
   return(true);
   }

//---------------------------------------------------------------------------
TList* __fastcall TMercadoJSON::PM_GetSegmentoJSON(void)
   {
	return(lisSEGMENTO_JSON);
   }


//---------------------------------------------------------------------------
int __fastcall TMercadoJSON::PM_GetAnoFinal(void)
   {
   return(PD.ano_final);
   }

//---------------------------------------------------------------------------
int __fastcall TMercadoJSON::PM_GetAnoInicial(void)
   {
   return(PD.ano_inicial);
   }

//---------------------------------------------------------------------------
void __fastcall TMercadoJSON::PM_SetAnoFinal(int ano_fim)
   {
   PD.ano_final = ano_fim;
   }

//---------------------------------------------------------------------------
void __fastcall TMercadoJSON::PM_SetAnoInicial(int ano_ini)
   {
   PD.ano_inicial = ano_ini;
   }

/*
//---------------------------------------------------------------------------
void __fastcall TMercadoJSON::RedefinePeriodo(int ano_ini, int ano_fim)
   {
   //variáveis locais
   VTSegmento *segmento;

   //salva ano inicial e final
   AnoInicial = ano_ini;
   AnoFinal   = ano_fim;
   //redefine período de todas Regioes
	for (int n = 0; n < lisSEGMENTO->Count; n++)
		{
		segmento = (VTSegmento*)lisSEGMENTO->Items[n];
		segmento->RedefinePeriodo(ano_ini, ano_fim);
		}
   }

//---------------------------------------------------------------------------
void __fastcall TMercadoJSON::Reinicia(void)
   {
   //destrói todas as Regioes
	LimpaTList(lisSEGMENTO);
   PD.segmento_default = NULL;
   }

//---------------------------------------------------------------------------
void __fastcall TMercadoJSON::RetiraCenario(VTCenario *cenario)
   {
   if (cenario != NULL)
      {
      lisCENARIO->Remove(cenario);
      delete cenario;
		}
   }

//---------------------------------------------------------------------------
void __fastcall TMercadoJSON::RetiraSegmento(VTSegmento *segmento)
   {
   if (segmento != NULL)
		{//verifica se Segmento default
      if (segmento == PD.segmento_default) PD.segmento_default = NULL;
      if(lisSEGMENTO->IndexOf(segmento)>= 0)lisSEGMENTO->Remove(segmento);
		//lisSEGMENTO->Remove(segmento);
		delete segmento;
      }
   }
 */
//---------------------------------------------------------------------------
//eof
