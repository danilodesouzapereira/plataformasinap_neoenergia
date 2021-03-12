//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TAgregada.h"
#include "TSegmento.h"
#include "..\VTResumo.h"
#include "..\..\Apl\VTApl.h"
#include "..\..\Radial\VTArvore.h"
#include "..\..\Radial\VTNode.h"
#include "..\..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTAgregada* __fastcall NewObjAgregada(VTApl *apl_owner)
   {
   try{
      return (new TAgregada(apl_owner));
      }catch (Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TAgregada::TAgregada(VTApl *apl_owner)
   {
   //salva ponteiro para objetos
   this->apl = apl_owner;
   //cria listas
   lisSEGMENTO     = new TList();
   lisTMP          = new TList();
   //cria Resumo para totalizar todos os segmentos
   resumo_total = NewObjResumo();
   }

//---------------------------------------------------------------------------
__fastcall TAgregada::~TAgregada(void)
   {
   //destrói objetos
   if (resumo_total) {delete resumo_total; resumo_total = NULL;}
   //destrói lista sem destruir seus objetos
   if (lisTMP) {delete lisTMP; lisTMP = NULL;}
   //destrói lista e seus objetos
   if (lisSEGMENTO) {LimpaTList(lisSEGMENTO); delete lisSEGMENTO; lisSEGMENTO = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TAgregada::AgregaResumoTotal(void)
   {
   //variáveis locais
   TSegmento *segmento;

   //destrói resumo_total caso já exista
   if (resumo_total == NULL)
	  {//cria Resumo p/ totalizar os Resumos de todos Segmetnos
	  resumo_total = NewObjResumo();
	  }
   //reinicia seus valores
   resumo_total->ZeraValores();
   resumo_total->tipo_resumo      = resumoAGREGADO;
   resumo_total->rede.TipoRede    = "Total";
   //monta lista com todos os Segmentos
   LisAllSegmento(lisTMP);
   //loop p/ todos Segmentos em lisTMP
   for (int n = 0; n < lisTMP->Count; n++)
	  {
	  segmento = (TSegmento*)lisTMP->Items[n];
	  resumo_total->Agrega(segmento->Resumo);
	  }
   //redefine valores do balanço para grupos de resumo
   resumo_total->balanco.EnergiaEntrada = resumo_total->balanco.EnergiaSuprimento +
										  resumo_total->balanco.EnergiaGerador;
   resumo_total->balanco.EnergiaRecebidaExt = 0;
   resumo_total->balanco.EnergiaRecebidaInt = 0;
   resumo_total->balanco.EnergiaEntregueExt = 0;
   resumo_total->balanco.EnergiaEntregueInt = 0;

   //limpa lista local
   lisTMP->Clear();
  }
/*
//---------------------------------------------------------------------------
TSegmento* __fastcall TAgregada::ExisteSegmento(int tiporede_id)
   {
   return(ExisteSegmento(lisSEGMENTO, tiporede_id));
   }

//---------------------------------------------------------------------------
//função recursiva para busca de Segmentos dentro dos Segmentos
//---------------------------------------------------------------------------
TSegmento* __fastcall TAgregada::ExisteSegmento(TList *lisSEG, int tiporede_id)
   {
   //variáveis locais
   TSegmento *segmento;

   //procura o Segmento dentro da lista indicada
   for (int n = 0; n < lisSEG->Count; n++)
      {
      segmento = (TSegmento*)lisSEG->Items[n];
      if (segmento->TipoRedeId == tiporede_id) return(segmento);
      //chamda recursiva
      if ((segmento = ExisteSegmento(segmento->LisSegmento(), tiporede_id)) != NULL) return(segmento);
      }
   return(NULL);
   }
*/

//---------------------------------------------------------------------------
TSegmento* __fastcall TAgregada::ExisteSegmentoRaiz(int tiporede_id)
   {
   //variáveis locais
   TSegmento *segmento;

   //verifica se existe o Segmento em lisSEGMENTO
   for (int n = 0; n < lisSEGMENTO->Count; n++)
      {
      segmento = (TSegmento*)lisSEGMENTO->Items[n];
      if (segmento->TipoRedeId == tiporede_id) return(segmento);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
void __fastcall TAgregada::Inicia(void)
   {
   //variáveis locais
   VTNode    *node_redes, *node_filho;
   VTArvore  *arvore  = (VTArvore*)apl->GetObject(__classid(VTArvore));
   TList     *lisNODE = arvore->LisNodeRaiz();

   //ATENÇÃO:
   // 1) Arvore montada pelo Perdatec pode possuir 1 ou 2 Nos raiz: "Redes" e "Redes isoladas"
   // 2) Arvore montada a partir da base de  histórico possui um único No raiz
   //    com a Data e depois os 2 Nos "Redes" e "Redes isoladas"

   //destrói Segmentos criados anteriormente e zera ResumoTotal
   Reinicia();
   //identifica Node qeu tem código "Redes"
   if ((node_redes = arvore->ExisteNode("Redes")) == NULL) return;
   //determina lista de Nodes do Node "Redes"
   lisNODE = node_redes->LisNode();
   for (int n = 0; n < lisNODE->Count; n++)
      {
      node_filho = (VTNode*)lisNODE->Items[n];
      //insere Segmento em lisSEGMENTO
      InsereSegmentoRaiz(node_filho);
      }
   //agrega em Resumo o total dos Resumos de cada Segmento
   AgregaResumoTotal();
   }

//---------------------------------------------------------------------------
void __fastcall TAgregada::InsereSegmentoFilho(VTNode *node, TSegmento *segmento_pai)
   {
   //variáveis locais
   int       index;
   TSegmento *segmento_filho;
   VTNode    *node_filho;
   VTResumo  *resumo;
   TList     *lisNODE   = node->LisNode();
   TList     *lisRESUMO = node->LisObjectNode();

   //proteção
   if (lisRESUMO->Count == 0) return;
   //verifica se lisRESUMO possui apenas um Resumo (rede radial)
   if (lisRESUMO->Count == 1)
      {
      resumo = (VTResumo*)lisRESUMO->First();
      //verifica se o Segmento pai já possui um Segmento para o tipo de rede
      if ((segmento_filho = segmento_pai->ExisteSegmento(resumo->rede.TipoRedeId)) == NULL)
         {//cria um novo Segmento e insere no Segmento pai
         segmento_filho = segmento_pai->InsereSegmento(resumo->rede.TipoRedeId);
         }
      }
   else
      {//cria um Segmento para o grupo de Resumos e insere no Segmento pai
      segmento_filho = segmento_pai->InsereSegmento(-1);
      }
   //proteção
   if (segmento_filho == NULL) return;
   //define level do Segmento
   index = segmento_pai->LisSegmento()->IndexOf(segmento_filho);
   segmento_filho->Level = segmento_pai->Level + "." + IntToStr(index + 1);
   //agrega todos os Resumos de lisRESUMO no Resumo do Segmento pai
   segmento_filho->AgregaResumos(lisRESUMO);
   //chamada recursiva: cria Segmentos filhos para os Nodes filhos
   for (int n = 0; n < lisNODE->Count; n++)
      {
      node_filho = (VTNode*)lisNODE->Items[n];
      InsereSegmentoFilho(node_filho, segmento_filho);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TAgregada::InsereSegmentoRaiz(VTNode *node)
   {
   //variáveis locais
   TSegmento *segmento;
   VTNode    *node_filho;
   VTResumo  *resumo;
   TList     *lisNODE   = node->LisNode();
   TList     *lisRESUMO = node->LisObjectNode();

   //proteção
   if (lisRESUMO->Count == 0) return;
   //verifica se lisRESUMO possui apenas um Resumo (rede radial)
   if (lisRESUMO->Count == 1)
      {//determina Resumo único
      resumo = (VTResumo*)lisRESUMO->First();
      //verifica se já existe um Segmento em lisSEGMENTO para o tipo da rede
      if ((segmento = ExisteSegmentoRaiz(resumo->rede.TipoRedeId)) == NULL)
         {//cria um novo Segmento e insere em lisSEGMENTO
         segmento = new TSegmento(resumo->rede.TipoRedeId);
         lisSEGMENTO->Add(segmento);
         }
      }
   else
      {//cria um Segmento para o grupo de Resumos e insere em lisSEGMENTO
      segmento = new TSegmento(-1);
      lisSEGMENTO->Add(segmento);
      }
   //proteção
   if (segmento == NULL) return;
   //define level do Segmento
   segmento->Level = IntToStr(lisSEGMENTO->IndexOf(segmento) + 1);
   //agrega todos os Resumos de lisRESUMO no Resumo do Segmento pai
   segmento->AgregaResumos(lisRESUMO);
   //cria Segmentos filhos para os Nodes filhos
   for (int n = 0; n < lisNODE->Count; n++)
      {
      node_filho = (VTNode*)lisNODE->Items[n];
      InsereSegmentoFilho(node_filho, segmento);
      }
   }

//---------------------------------------------------------------------------
int __fastcall TAgregada::LisAllSegmento(TList *lisEXT)
   {
   //variáveis locais
   TSegmento *segmento;

   //reinicia lisEXT
   lisEXT->Clear();
   //copia os Segmentos raiz
   CopiaTList(lisSEGMENTO, lisEXT);
   //loop p/ todos os Segmentos em lisEXT
   for (int n = 0; n < lisEXT->Count; n++)
      {
      segmento = (TSegmento*)lisEXT->Items[n];
      CopiaTList(segmento->LisSegmento(), lisEXT);
      }
   return(lisEXT->Count);
   }

//---------------------------------------------------------------------------
TList* __fastcall TAgregada::LisSegmentoRaiz(void)
   {
   return(lisSEGMENTO);
   }

//---------------------------------------------------------------------------
bool __fastcall TAgregada::PM_GetValida(void)
   {
   return(lisSEGMENTO->Count > 0);
   }

//---------------------------------------------------------------------------
void __fastcall TAgregada::Reinicia(void)
   {
   //reinicia ResumoTotal
   resumo_total->ZeraValores();
   //destrói todos objetos Resumo de dados agregados
   LimpaTList(lisSEGMENTO);
   }

//---------------------------------------------------------------------------
VTResumo* __fastcall TAgregada::ResumoTotal(void)
   {
   return(resumo_total);
   }

//---------------------------------------------------------------------------
//eof

