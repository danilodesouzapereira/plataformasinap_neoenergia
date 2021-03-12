//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TCelula.h"
#include "VTLayer.h"
#include "..\Rede\VTArea.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTReducao.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Rede.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTCelula* NewObjCelula()
   {
   try{
      return(new TCelula());
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TCelula::TCelula(void)
   {
   try{//cria objetos
      PD.area       = DLL_NewObjArea();
      lisLAYER_REDE = new TList();
      lisLAYER_REDU = new TList();
      } catch (Exception &e)
         {//nada fazer
         }
   }

//---------------------------------------------------------------------------
__fastcall TCelula::~TCelula(void)
   {
   //destrói objetos
   if (PD.area) {delete PD.area; PD.area = NULL;}
   //destrói lista e seus objetos
   if (lisLAYER_REDE) {LimpaTList(lisLAYER_REDE); delete lisLAYER_REDE; lisLAYER_REDE = NULL;}
   if (lisLAYER_REDU) {LimpaTList(lisLAYER_REDU); delete lisLAYER_REDU; lisLAYER_REDU = NULL;}
   }

//---------------------------------------------------------------------------
VTLayer* __fastcall TCelula::ExisteLayerRede(VTRede *rede, int tipo_layer)
   {
   //variáveis locais
   VTLayer *layer;

   //verifica se último Layer é o da Rede indicada
   if ((ult_layer != NULL)&&(ult_layer->Rede == rede)&&(ult_layer->Tipo == tipo_layer)) return(ult_layer);
   //procura o Layer da Rede indicada
   for (int n = 0; n < lisLAYER_REDE->Count; n++)
      {
      layer = (VTLayer*)lisLAYER_REDE->Items[n];
      if ((layer->Rede == rede)&&(layer->Tipo == tipo_layer)) return(layer);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
VTLayer* __fastcall TCelula::ExisteLayerReducao(VTReducao *reducao, int tipo_layer)
   {
   //variáveis locais
   VTLayer *layer;

   //verifica se último Layer é o da Reducao indicada
   if ((ult_layer != NULL)&&(ult_layer->Reducao == reducao)&&(ult_layer->Tipo == tipo_layer)) return(ult_layer);
   //procura o Layer da Reducao indicada
   for (int n = 0; n < lisLAYER_REDU->Count; n++)
      {
      layer = (VTLayer*)lisLAYER_REDU->Items[n];
      if ((layer->Reducao == reducao)&&(layer->Tipo == tipo_layer)) return(layer);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
void __fastcall TCelula::InsereEqptoRede(VTRede *rede, int tipo_layer, VTEqpto *eqpto)
   {
   //verifica se já existe um objeto Layer p/ a Rede
   if ((ult_layer = ExisteLayerRede(rede, tipo_layer)) == NULL)
      {//cria um novo objeto Layer e associa c/ a Rede
      lisLAYER_REDE->Add(ult_layer = NewObjLayer(tipo_layer));
      ult_layer->Rede = rede;
      }
   //insere Eqpto no objeto Layer
   ult_layer->InsereEqpto(eqpto);
   }

//---------------------------------------------------------------------------
void __fastcall TCelula::InsereEqptoReducao(VTReducao *reducao, int tipo_layer, VTEqpto *eqpto)
   {
   //verifica se já existe um objeto Layer p/ a Reducao
   if ((ult_layer = ExisteLayerReducao(reducao, tipo_layer)) == NULL)
      {//cria um novo objeto Layer e associa c/ a Reducao
      lisLAYER_REDU->Add(ult_layer = NewObjLayer(tipo_layer));
      ult_layer->Reducao = reducao;
      }
   //insere Eqpto no objeto Layer
   ult_layer->InsereEqpto(eqpto);
   }

//---------------------------------------------------------------------------
void __fastcall TCelula::LisEqpto(TList *lisEXT, int tipo)
   {
   //variáveis locais
   VTLayer *layer;

   //obtém Eqptos de todas Rede
   LisEqptoRedeAllLayer(lisEXT, layerREDE, tipo);
   //obtém Eqptos de Reducao cujos Eqptos estão sendo mostrados
   for (int n = 0; n < lisLAYER_REDU->Count; n++)
      {
      layer = (VTLayer*)lisLAYER_REDU->Items[n];
      if ((layer->Tipo == layerREDU_EQPTO)&&(layer->Reducao->Show[rdu_EQPTOS]))
         {
         layer->LisEqpto(lisEXT, tipo);
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TCelula::LisEqptoRede(TList *lisEXT, int tipo_layer, VTRede *rede, int tipo)
   {
   //variáveis locais
   VTLayer *layer;

   //verifica se foi definida a Rede
   if (rede == NULL)
      {//obtém Eqpto do tipo indicado de todos Layer da Celula
      LisEqptoRedeAllLayer(lisEXT, tipo_layer, tipo);
      }
   else
      {//determina Layer da Rede indicada
      if ((layer = ExisteLayerRede(rede, tipo_layer)) == NULL) return;
      //copia Eqptos do Layer
      layer->LisEqpto(lisEXT, tipo);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TCelula::LisEqptoRedeAllLayer(TList *lisEXT, int tipo_layer, int tipo)
   {
   //variáveis locais
   VTLayer *layer;

   //loop p/ todos Layer
   for (int n = 0; n < lisLAYER_REDE->Count; n++)
      {
      layer = (VTLayer*)lisLAYER_REDE->Items[n];
      if (layer->Tipo == tipo_layer)
         {
         layer->LisEqpto(lisEXT, tipo);
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TCelula::LisEqptoReducao(TList *lisEXT, int tipo_layer, VTReducao *reducao, int tipo)
   {
   //variáveis locais
   VTLayer *layer;

   //verifica se foi definida a Reducao
   if (reducao == NULL)
      {//obtém Eqpto do tipo indicado de todos Layer da Celula
      LisEqptoReducaoAllLayer(lisEXT, tipo_layer, tipo);
      }
   else
      {//determina Layer da Reducao indicada
      if ((layer = ExisteLayerReducao(reducao, tipo_layer)) == NULL) return;
      //copia Eqptos do Layer
      layer->LisEqpto(lisEXT, tipo);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TCelula::LisEqptoReducaoAllLayer(TList *lisEXT, int tipo_layer, int tipo)
   {
   //variáveis locais
   VTLayer *layer;

   //loop p/ todos Layer
   for (int n = 0; n < lisLAYER_REDU->Count; n++)
      {
      layer = (VTLayer*)lisLAYER_REDU->Items[n];
      if (layer->Tipo == tipo_layer)
         {
         layer->LisEqpto(lisEXT, tipo);
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TCelula::LisEqptoVisivel(TList *lisEXT, int tipo)
   {
   //variáveis locais
   VTLayer *layer;

   //loop p/ todos Layer de Rede
   for (int n = 0; n < lisLAYER_REDE->Count; n++)
      {
      layer = (VTLayer*)lisLAYER_REDE->Items[n];
      if (layer->Rede->Visible())
         {
         layer->LisEqpto(lisEXT, tipo);
         }
      }
   //obtém Eqptos de Reducao cujos Eqptos estão sendo mostrados
   for (int n = 0; n < lisLAYER_REDU->Count; n++)
      {
      layer = (VTLayer*)lisLAYER_REDU->Items[n];
      if (layer->Reducao->Visible())
         {
         if ((layer->Tipo == layerREDU_EQPTO)&&(layer->Reducao->Show[rdu_EQPTOS]))
            {
            layer->LisEqpto(lisEXT, tipo);
            }
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TCelula::LisRede(TList *lisEXT)
   {
   //variáveis locais
   VTLayer *layer;

   //loop p/ todos Layer
   for (int n = 0; n < lisLAYER_REDE->Count; n++)
      {
      layer = (VTLayer*)lisLAYER_REDE->Items[n];
      if (lisEXT->IndexOf(layer->Rede) < 0) lisEXT->Add(layer->Rede);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TCelula::LisReducao(TList *lisEXT)
   {
   //variáveis locais
   VTLayer *layer;

   //loop p/ todos Layer
   for (int n = 0; n < lisLAYER_REDU->Count; n++)
      {
      layer = (VTLayer*)lisLAYER_REDU->Items[n];
      if ((layer->Tipo == layerREDU_EQPTO)&&(layer->Reducao->Show[rdu_EQPTOS]))
         {
         if (lisEXT->IndexOf(layer->Reducao) < 0) lisEXT->Add(layer->Reducao);
         }
      else if ((layer->Tipo == layerREDU_ESQ)&&(! layer->Reducao->Show[rdu_EQPTOS]))
         {
         if (lisEXT->IndexOf(layer->Reducao) < 0) lisEXT->Add(layer->Reducao);
         }
      }
   }

//---------------------------------------------------------------------------
int __fastcall TCelula::QtdeEqptoRede(int tipo_layer, VTRede *rede, int tipo)
   {
   //variáveis locais
   int     count = 0;
   VTLayer *layer;

   //verifica se foi definida a Rede
   if (rede == NULL)
      {//obtém Eqpto do tipo indicado de todos Layer da Celula
      count = QtdeEqptoRedeAllLayer(tipo_layer, tipo);
      }
   else
      {//determina Layer da Rede indicada
      if ((layer = ExisteLayerRede(rede, tipo_layer)) != NULL)
         {//copia Eqptos do Layer
         count = layer->QtdeEqpto(tipo);
         }
      }
   return(count);
   }

//---------------------------------------------------------------------------
int __fastcall TCelula::QtdeEqptoRedeAllLayer(int tipo_layer, int tipo)
   {
   //variáveis locais
   int     count = 0;
   VTLayer *layer;

   //loop p/ todos Layer
   for (int n = 0; n < lisLAYER_REDE->Count; n++)
      {
      layer = (VTLayer*)lisLAYER_REDE->Items[n];
      if (layer->Tipo == tipo_layer)
         {
         count += layer->QtdeEqpto(tipo);
         }
      }
   return(count);
   }

//---------------------------------------------------------------------------
//eof


