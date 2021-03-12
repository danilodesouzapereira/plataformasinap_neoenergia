//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TSegmento.h"
#include "..\VTResumo.h"
#include "..\..\Rede\VTTipoRede.h"
#include "..\..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TSegmento::TSegmento(int tiporede_id)
   {
   //salva o tipo de rede
   TipoRedeId = tiporede_id;
   //cria objetos
   Resumo      = NewObjResumo();
   lisSEGMENTO = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TSegmento::~TSegmento(void)
   {
   //destrói objetos
   if (Resumo) {delete Resumo; Resumo = NULL;}
   //destrói lista e seus objetos
   if (lisSEGMENTO) {LimpaTList(lisSEGMENTO); delete lisSEGMENTO; lisSEGMENTO = NULL;}
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TSegmento::AgregaResumos(TList *lisRESUMO)
   {
   //agrega dados dos Resumos
   Resumo->Agrega(lisRESUMO);
   //verifica se Segmento é de um grupo de redes
   if (TipoRedeId < 0)
	  {//cancela energias internas do balanco
	  Resumo->balanco.EnergiaEntregueInt = 0.;
	  Resumo->balanco.EnergiaRecebidaInt = 0.;
	  }
   return(true);
   }
*/
//---------------------------------------------------------------------------
bool __fastcall TSegmento::AgregaResumos(TList *lisRESUMO)
   {
   //agrega dados dos Resumos
   Resumo->Agrega(lisRESUMO);
   //verifica se Segmento é de um grupo de redes
   if (TipoRedeId < 0)
	  {//cancela energias internas do balanco
	  Resumo->balanco.EnergiaEntregueInt = 0.;
	  Resumo->balanco.EnergiaRecebidaInt = 0.;
	  //corrige entrada
	  Resumo->balanco.EnergiaEntrada = Resumo->balanco.EnergiaSuprimento + Resumo->balanco.EnergiaGerador;
	  }
   return(true);
   }

//---------------------------------------------------------------------------
TSegmento* __fastcall TSegmento::ExisteSegmento(int tiporede_id)
   {
   //variáveis locais
   TSegmento *segmento;

   for (int i = 0; i < lisSEGMENTO->Count; i++)
      {
      segmento = (TSegmento*)lisSEGMENTO->Items[i];
      if (segmento->TipoRedeId == tiporede_id) return(segmento);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
TSegmento* __fastcall TSegmento::InsereSegmento(int tiporede_id)
   {
   //variáveis locais
   TSegmento *segmento;

   //proteção: verifica se existe o Segmento indicado apenas neste Segmento
   if ((segmento = ExisteSegmento(tiporede_id)) == NULL)
      {//cria um novo Segmento
      lisSEGMENTO->Add(segmento = new TSegmento(tiporede_id));
      }
   return(segmento);
   }

//---------------------------------------------------------------------------
TList* __fastcall TSegmento::LisSegmento(void)
   {
   return(lisSEGMENTO);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TSegmento::TipoRedeAsString(void)
   {
   switch(TipoRedeId)
      {
      case -1:        return("grupo");
      case redeETT:   return("ETT");
      case redeSUB:   return("SDAT");
      case redeETD:   return("SED");
      case redePRI:   return("SDMT");
      case redeSEC:   return("SDBT");
      case redeTRANS: return("STAT");
      case redeINDEF: return("indef");
      default:        return("indef");
      }
   }
//---------------------------------------------------------------------------
//eof

