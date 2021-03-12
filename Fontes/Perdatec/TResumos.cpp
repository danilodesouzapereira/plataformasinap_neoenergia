//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DateUtils.hpp>
#include "TResumos.h"
#include "VTBalanco.h"
#include "VTBalancos.h"
#include "VTPerda.h"
#include "VTPerdas.h"
#include "VTResumo.h"
#include ".\EnergiaPassante\VTEnergiaPassante.h"
#include ".\EnergiaPassante\VTEnergiaPassantes.h"
#include "..\Apl\VTApl.h"
#include "..\Classes\VTClasse.h"
#include "..\Potencia\VTDemanda.h"
#include "..\Radial\VTArvore.h"
#include "..\Radial\VTNode.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTTipoRede.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTResumos* __fastcall NewObjResumos(VTApl *apl)
   {
   try{
      return (new TResumos(apl));
      }catch (Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TResumos::TResumos(VTApl *apl)
   {
   //salva ponteiros p/ objetos
   this->apl = apl;
   //cria objetos
   lisRESUMO = new TList();
   lisTMP    = new TList();
   //cria Resumo para totalizar os resumos de um mesmo tipo de rede
   resumo_tmp = NewObjResumo();
   }

//---------------------------------------------------------------------------
__fastcall TResumos::~TResumos(void)
   {
   //destrói objetos Resumo
   if (resumo_tmp) {delete resumo_tmp; resumo_tmp = NULL;}
   //destrói lista sem destruir seus objetos
   if (lisRESUMO) {delete lisRESUMO; lisRESUMO = NULL;}
   if (lisTMP)    {delete lisTMP;    lisTMP    = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TResumos::AtualizaTodosResumos(void)
   {
   //variáveis locais
   VTResumo *resumo;

   //obtém lista de todos Resumo da Arvore
   lisRESUMO->Clear();
   arvore->LisAllObject(lisRESUMO);
   //loop p/ todos Resumos
   for (int n = 0; n < lisRESUMO->Count; n++)
      {
      resumo = (VTResumo*)lisRESUMO->Items[n];
      IniciaResumoRede(resumo, (VTRede*)(resumo->eqpto));
      }
   }

//---------------------------------------------------------------------------
void __fastcall TResumos::Clear(void)
   {
   LimpaTList(lisRESUMO);
   }

//---------------------------------------------------------------------------
VTResumo* __fastcall TResumos::ExisteResumo(VTEqpto *eqpto)
   {
   //variáveis locais
   VTResumo *resumo;

   for (int n = 0; n < lisRESUMO->Count; n++)
      {
      resumo = (VTResumo*)lisRESUMO->Items[n];
      if (resumo->eqpto == eqpto) return(resumo);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/17858579
bool __fastcall TResumos::IniciaArvoreComResumo(void)
   {
   //variáveis locais
   VTNode *node;
   TList  *lisNODE;

   //obtém objetos
   balancos  = (VTBalancos*)apl->GetObject(__classid(VTBalancos));
   energiaPassantes  = (VTEnergiaPassantes*)apl->GetObject(__classid(VTEnergiaPassantes));
   perdas    = (VTPerdas*)apl->GetObject(__classid(VTPerdas));
   arvore    = (VTArvore*)apl->GetObject(__classid(VTArvore));
   lisNODE   = arvore->LisNodeRaiz();
   //redefine tipo da arvore
//   arvore->TipoArvore = arvorePERDATEC;
   //loop p/ todos Nodes da Arvore
   for (int n = 0; n < lisNODE->Count; n++)
      {
      node    = (VTNode*)lisNODE->Items[n];
      //cria um Resumo para cada Rede do Node
      IniciaNodeComResumo(node);
      }
   //insere em lisRESUMO todos os Resumo criados
   lisRESUMO->Clear();
   LisResumo(lisRESUMO);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TResumos::IniciaNodeComResumo(VTNode *node)
   {
   //variáveis locais
   TList    *lisNODE;
   TList    *lisREDE;
   VTRede   *rede;
   VTResumo *resumo;

   //cria um Resumo para cada Rede do Node e insere em lisRESUMO
   lisRESUMO->Clear();
   lisREDE = node->LisObjectNode();
   for (int nr = 0; nr < lisREDE->Count; nr++)
      {
      rede = (VTRede*)lisREDE->Items[nr];
      //cria e inicia um objeto Resumo para a Rede
      lisRESUMO->Add(resumo = NewObjResumo());
      //associa Resumo ao Node
      resumo->object = node;
      //inicia Resumo da Rede
      IniciaResumoRede(resumo, rede);
      }
   //reinicia Node com lista de Resumos
   if (lisREDE->Count > 0) node->ReiniciaLisObject(lisRESUMO);
   //chamada recursiva da função
   lisNODE = node->LisNode();
   for (int n = 0; n < lisNODE->Count; n++)
      {
      node    = (VTNode*)lisNODE->Items[n];
      //cria um Resumo para cada Rede do Node
      IniciaNodeComResumo(node);
      }
   return(true);
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TResumos::IniciaResumoRede(VTResumo *resumo, VTRede *rede)
   {
   //variáveis locais
   VTBalanco  *balanco;
   VTPerda    *perda;

   //associa Resumo com Rede
   resumo->eqpto = rede;
   //determina objetos Balanco e Perda da Rede
   if ((balanco = balancos->ExisteBalanco(rede)) == NULL) return(false);
   if ((perda   = perdas->ExistePerda(rede)) == NULL)     return(false);
   //dados do Resumo
   resumo->id                    = FalsoId();
   resumo->tipo_resumo           = resumoREDE;
   //dados da rede
   resumo->rede.Id               = rede->Id;
   resumo->rede.Data             = DateToStr(DateOf(Now()));
   resumo->rede.Codigo           = rede->Codigo;
   resumo->rede.TipoRede         = rede->TipoRede->Codigo;
   resumo->rede.TipoRedeId       = rede->TipoRede->Id;
   resumo->rede.QtdeRede         = 1;
   resumo->rede.QtdeTrafo        = rede->NumeroDeLigacoes(eqptoTRAFO);
   resumo->rede.QtdeCabo         = int(rede->Comprimento_km() * 1000.);
   resumo->rede.QtdeCapacitor    = rede->NumeroDeEqbar(eqptoCAPACITOR);
   resumo->rede.QtdeReator       = rede->NumeroDeEqbar(eqptoREATOR);
   resumo->rede.QtdeRegulador    = rede->NumeroDeLigacoes(eqptoREGULADOR);
   resumo->rede.QtdeConsA4       = rede->NumeroDeConsumidor(consA4);
   resumo->rede.QtdeConsRes      = rede->NumeroDeConsumidor(consRESIDENCIAL);
   resumo->rede.QtdeConsCom      = rede->NumeroDeConsumidor(consCOMERCIAL);
   resumo->rede.QtdeConsInd      = rede->NumeroDeConsumidor(consINDUSTRIAL);
   resumo->rede.QtdeConsRur      = rede->NumeroDeConsumidor(consRURAL);
   resumo->rede.QtdeConsOut      = rede->NumeroDeConsumidor(consOUTROS);
   resumo->rede.QtdeConsNulo     = rede->NumeroDeConsumidor(consNULO);
   //dados de perda
   resumo->perda.PerdaCabo       = perda->Energia_mwh[segCABO];
   resumo->perda.PerdaTrafoCobre = perda->Energia_mwh[segTRF_CU];
   resumo->perda.PerdaTrafoFerro = perda->Energia_mwh[segTRF_FE];
   resumo->perda.PerdaCapacitor  = 0.;
   resumo->perda.PerdaReator     = 0.;
   resumo->perda.PerdaRegulador  = 0.;
   resumo->perda.PerdaMedidor    = perda->Energia_mwh[segMEDIDOR];
   resumo->perda.PerdaRamal      = perda->Energia_mwh[segRAMAL];
   resumo->perda.PerdaOutros     = perda->Energia_mwh[segOUTROS];
   //dados de balanço de energia
   resumo->balanco.EnergiaEntrada    =  balanco->EnergiaSuprimento_mwh + balanco->EnergiaGerador_mwh;
   resumo->balanco.EnergiaSuprimento =  balanco->EnergiaSuprimento_mwh;
   resumo->balanco.EnergiaGerador    =  balanco->EnergiaGerador_mwh;
   resumo->balanco.EnergiaRecebida   =  balanco->EnergiaRecebida_mwh;
   resumo->balanco.EnergiaEntregue   = -balanco->EnergiaEntregue_mwh;
   resumo->balanco.EnergiaCarga      = -balanco->EnergiaCarga_mwh;
   resumo->balanco.EnergiaPerdaFluxo = -balanco->EnergiaPerdaFluxo_mwh;
   resumo->balanco.EnergiaPerdaTotal = -balanco->EnergiaPerdaTotal_mwh;

   return(true);
   }
*/
//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/17858579
bool __fastcall TResumos::IniciaResumoRede(VTResumo *resumo, VTRede *rede)
   {
   //variáveis locais
   VTBalanco  *balanco;
   VTEnergiaPassante *energiaPassante;
   VTPerda    *perda;

   //associa Resumo com Rede
   resumo->eqpto = rede;
   //dados do Resumo
   resumo->id                    = FalsoId();
   resumo->tipo_resumo           = resumoREDE;
   //dados da rede
   resumo->rede.Id               = rede->Id;
   resumo->rede.Data             = DateToStr(DateOf(Now()));
   resumo->rede.Codigo           = rede->Codigo;
   resumo->rede.TipoRede         = rede->TipoRede->Codigo;
   resumo->rede.TipoRedeId       = rede->TipoRede->Id;
   resumo->rede.QtdeRede         = 1;
   resumo->rede.QtdeTrafo        = rede->NumeroDeLigacoes(eqptoTRAFO);
   resumo->rede.QtdeCabo         = int(rede->Comprimento_km() * 1000.);
   resumo->rede.QtdeCapacitor    = rede->NumeroDeEqbar(eqptoCAPACITOR);
   resumo->rede.QtdeReator       = rede->NumeroDeEqbar(eqptoREATOR);
   resumo->rede.QtdeRegulador    = rede->NumeroDeLigacoes(eqptoREGULADOR);
   resumo->rede.QtdeConsA4       = rede->NumeroDeConsumidor(consA4);
   resumo->rede.QtdeConsRes      = rede->NumeroDeConsumidor(consRESIDENCIAL);
   resumo->rede.QtdeConsCom      = rede->NumeroDeConsumidor(consCOMERCIAL);
   resumo->rede.QtdeConsInd      = rede->NumeroDeConsumidor(consINDUSTRIAL);
   resumo->rede.QtdeConsRur      = rede->NumeroDeConsumidor(consRURAL);
   resumo->rede.QtdeConsOut      = rede->NumeroDeConsumidor(consOUTROS);
   resumo->rede.QtdeConsNulo     = rede->NumeroDeConsumidor(consNULO);
   resumo->rede.EnergiaMercado   = SomatoriaEnergiaCargas(rede);
   //dados de perda
   if ((perda = perdas->ExistePerda(rede)) != NULL)
      {
      resumo->perda.PerdaCabo       = perda->Energia_mwh[segCABO];
      resumo->perda.PerdaTrafoCobre = perda->Energia_mwh[segTRF_CU];
      resumo->perda.PerdaTrafoFerro = perda->Energia_mwh[segTRF_FE];
      resumo->perda.PerdaCapacitor  = 0.;
      resumo->perda.PerdaReator     = 0.;
      resumo->perda.PerdaRegulador  = 0.;
      resumo->perda.PerdaMedidor    = perda->Energia_mwh[segMEDIDOR];
      resumo->perda.PerdaRamal      = perda->Energia_mwh[segRAMAL];
      resumo->perda.PerdaOutros     = perda->Energia_mwh[segOUTROS];
      }
   //dados de balanço de energia
   if ((balanco = balancos->ExisteBalanco(rede)) != NULL)
      {
	  //resumo->balanco.EnergiaEntrada     =  balanco->EnergiaSuprimento_mwh + balanco->EnergiaGerador_mwh;
	  resumo->balanco.EnergiaEntrada     =  balanco->EnergiaEntrada_mwh;
      resumo->balanco.EnergiaSuprimento  =  balanco->EnergiaSuprimento_mwh;
      resumo->balanco.EnergiaGerador     =  balanco->EnergiaGerador_mwh;
      resumo->balanco.EnergiaRecebidaExt =  balanco->EnergiaRecebidaExt_mwh;
      resumo->balanco.EnergiaRecebidaInt =  balanco->EnergiaRecebidaInt_mwh;
      resumo->balanco.EnergiaEntregueExt = -balanco->EnergiaEntregueExt_mwh;
      resumo->balanco.EnergiaEntregueInt = -balanco->EnergiaEntregueInt_mwh;
      resumo->balanco.EnergiaCarga       = -balanco->EnergiaCarga_mwh;
      resumo->balanco.EnergiaPerdaFluxo  = -balanco->EnergiaPerdaFluxo_mwh;
	  resumo->balanco.EnergiaPerdaTotal  = -balanco->EnergiaPerdaTotal_mwh;
	  }
   //dados da energia passante nos trafos
   if ((energiaPassante = energiaPassantes->ExisteEnergiaPassante(rede)) != NULL)
      {
	  //resumo->balanco.EnergiaEntrada     =  balanco->EnergiaSuprimento_mwh + balanco->EnergiaGerador_mwh;
	  resumo->energiaPassante.A1_A2		= energiaPassante->EnergiaRecebidaExt_mwh[catptA1_A2];
	  resumo->energiaPassante.A1_A3		= energiaPassante->EnergiaRecebidaExt_mwh[catptA1_A3];
	  resumo->energiaPassante.A1_A3A	= energiaPassante->EnergiaRecebidaExt_mwh[catptA1_A3A];
	  resumo->energiaPassante.A1_A4		= energiaPassante->EnergiaRecebidaExt_mwh[catptA1_A4];
	  resumo->energiaPassante.A2_A3		= energiaPassante->EnergiaRecebidaExt_mwh[catptA2_A3];
	  resumo->energiaPassante.A2_A3A	= energiaPassante->EnergiaRecebidaExt_mwh[catptA2_A3A];
	  resumo->energiaPassante.A2_A4		= energiaPassante->EnergiaRecebidaExt_mwh[catptA2_A4];
	  resumo->energiaPassante.A3_A2		= energiaPassante->EnergiaRecebidaExt_mwh[catptA3_A2];
	  resumo->energiaPassante.A3_A3A	= energiaPassante->EnergiaRecebidaExt_mwh[catptA3_A3A];
	  resumo->energiaPassante.A3_A4		= energiaPassante->EnergiaRecebidaExt_mwh[catptA3_A4];
	  resumo->energiaPassante.A3A_A2	= energiaPassante->EnergiaRecebidaExt_mwh[catptA3A_A2];
	  resumo->energiaPassante.A3A_A4	= energiaPassante->EnergiaRecebidaExt_mwh[catptA3A_A4];
	  resumo->energiaPassante.A3A_B		= energiaPassante->EnergiaRecebidaExt_mwh[catptA3A_B];
	  resumo->energiaPassante.A4_A2		= energiaPassante->EnergiaRecebidaExt_mwh[catptA4_A2];
	  resumo->energiaPassante.A4_A3		= energiaPassante->EnergiaRecebidaExt_mwh[catptA4_A3];
	  resumo->energiaPassante.A4_A3A	= energiaPassante->EnergiaRecebidaExt_mwh[catptA4_A3A];
	  resumo->energiaPassante.A4_A4		= energiaPassante->EnergiaRecebidaExt_mwh[catptA4_A4];
	  resumo->energiaPassante.A4_B		= energiaPassante->EnergiaRecebidaExt_mwh[catptA4_B];
	  resumo->energiaPassante.B_A3A		= energiaPassante->EnergiaRecebidaExt_mwh[catptB_A3A];
	  resumo->energiaPassante.B_A4		= energiaPassante->EnergiaRecebidaExt_mwh[catptB_A4];
	  }
   return(true);
   }


//---------------------------------------------------------------------------
bool __fastcall TResumos::InsereResumo(int nodepai_id, int node_id, VTResumo *resumo)
   {
   //variáveis locais
   VTNode *node_pai, *node;

   //verifica se existe o Node
   if ((node = arvore->ExisteNode(node_id)) != NULL)
      {//insere Resumo no Node existente
      node->InsereObject(resumo);
      return(true);
      }
   //nodepai_id < 0 indica que o Node não possui Node pai
   node_pai = NULL;
   if (nodepai_id > 0)
      {//verifica se existe o Node pai
      if ((node_pai = arvore->ExisteNode(nodepai_id)) == NULL)
         {//cria um novo Node pai
         node_pai = arvore->InsereNode();
         node_pai->Id = node_id;
         }
      }
   //cria um novo Node
   node = arvore->InsereNode(node_pai);
   node->Id = node_id;
   //insere Resumo no Node novo
   node->InsereObject(resumo);
   return(true);
   }

//---------------------------------------------------------------------------
TList* __fastcall TResumos::LisResumo(void)
   {
   return(lisRESUMO);
   }

//---------------------------------------------------------------------------
void __fastcall TResumos::LisResumo(TList *lisEXT)
   {
   //obtém objeto Arvore
   arvore    = (VTArvore*)apl->GetObject(__classid(VTArvore));
   //monta lista com todos objetos armazenados nos Nodes
   arvore->LisAllObject(lisEXT);
   }

//---------------------------------------------------------------------------
void __fastcall TResumos::LisResumo(int tiporede_id, TList *lisEXT)
   {
   //variáveis locais
   VTNode   *node;
   VTResumo *resumo;
   TList    *lisNODE;

   //verifica se foi definido tiporede
   if (tiporede_id == -1)
      {
      LisResumo(lisEXT);
      return;
      }
   //obtém objetos
   arvore    = (VTArvore*)apl->GetObject(__classid(VTArvore));
   lisNODE   = arvore->LisNodeRaiz();
   //loop p/ todos Nodes da Arvore
   for (int n = 0; n < lisNODE->Count; n++)
      {
      node    = (VTNode*)lisNODE->Items[n];
      //obtém lista de Resumos a jusante do Node
      lisTMP->Clear();
      node->LisObjectJusante(lisTMP);
      //seleciona os Resumos associados a redes do tipo indicado
      SelecionaResumo(lisTMP, lisEXT, tiporede_id);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TResumos::LisResumoJusante(VTNode *node, TList *lisEXT)
   {
   node->LisObjectJusante(lisEXT);
   }

//---------------------------------------------------------------------------
void __fastcall TResumos::LisResumoJusante(VTNode *node, int tiporede_id, TList *lisEXT)
   {
   //variáveis locais
   VTResumo *resumo;

   //verifica se foi definido tiporede
   if (tiporede_id == -1)
      {
      LisResumoJusante(node, lisEXT);
      return;
      }
   //obtém lista de Resumos a jusante
   lisTMP->Clear();
   LisResumoJusante(node, lisTMP);
   //seleciona os Resumos associados a redes do tipo indicado
   SelecionaResumo(lisTMP, lisEXT, tiporede_id);
   }

//---------------------------------------------------------------------------
void __fastcall TResumos::LisResumoMontante(VTNode *node, TList *lisEXT)
   {
   node->LisObjectMontante(lisEXT);
   }

//---------------------------------------------------------------------------
void __fastcall TResumos::LisResumoNode(VTNode *node, TList *lisEXT)
   {
   node->LisObjectNode(lisEXT);
   }

//---------------------------------------------------------------------------
void __fastcall TResumos::LisResumoNode(VTNode *node, int tiporede_id, TList *lisEXT)
   {
   //seleciona os Resumos associados a redes do tipo indicado
   SelecionaResumo(node->LisObjectNode(), lisEXT, tiporede_id);
   }

//---------------------------------------------------------------------------
int __fastcall TResumos::SelecionaResumo(TList *lisORIG, TList *lisDEST, int tiporede_id)
   {
   //variáveis locais
   int      count = 0;
   VTResumo *resumo;

   //seleciona os Resumos associados a redes do tipo indicado
   for (int n = 0; n < lisORIG->Count; n++)
      {
      resumo = (VTResumo*)lisORIG->Items[n];
      if (resumo->tipo_resumo == resumoREDE)
         {
         if ((tiporede_id == -1)||(resumo->rede.TipoRedeId == tiporede_id))
            {//insere Resumo em lisEXT
            if (lisDEST->IndexOf(resumo) < 0)
               {
               lisDEST->Add(resumo);
               count++;
               }
            }
         }
      }
   return(count);
   }

//---------------------------------------------------------------------------
double __fastcall TResumos::SomatoriaEnergiaCargas(VTRede *rede)
   {
   //variáveis locais
   double     energia_mwh = 0;
   VTCarga   *carga;
   VTDemanda *demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));

   //obtém lista de todas Cargas da Rede
   lisTMP->Clear();
   rede->LisEqbar(lisTMP, eqptoCARGA);
   //acumula energia de todas as Crgas
   for (int n = 0; n < lisTMP->Count; n++)
      {
	  carga        = (VTCarga*)lisTMP->Items[n];
	  //não soma carga A4 se a rede for BT
	  if((carga->GrupoA) && (rede->TipoRede->Segmento == redeSEC))
		 {//e a carga estiver na barra inicial da rede BT
		 if(carga->pbarra == rede->BarraInicial()) continue;
         }
      energia_mwh += demanda->Edia_mwh[carga];
      }
   return(energia_mwh);
   }
/*
//---------------------------------------------------------------------------
void __fastcall TResumos::TrataResumoPaiFilho(VTResumo *resumo, TList *lisRESUMO)
   {
   //variáveis locais
   VTResumo *resumo_pai, *resumo_filho;
   TList *lisRESUMO_MON = new TList();

   for(int nr = 0; nr < lisRESUMO->Count; nr++)
	  {
	  resumo_filho = (VTResumo*)lisRESUMO->Items[nr];
	  lisRESUMO_MON->Clear();
	  LisResumoMontante((VTNode*)resumo_filho->object, lisRESUMO_MON);
	  for(int n = 0; n < lisRESUMO_MON->Count; n++)
		 {
		 resumo_pai = (VTResumo*)lisRESUMO_MON->Items[n];
		 //verifica se é do mesmo tipo de rede
		 if(resumo_pai->rede.TipoRedeId != resumo_filho->rede.TipoRedeId)
			continue;
		 //verifica se resumo_pai está na lista de resumos que foram agregados
		 if(lisRESUMO->IndexOf(resumo_pai) < 0)
			continue;
		 //subtrai energia entrada do resumo_filho, para não ser contada 2x
		 resumo->balanco.EnergiaEntrada -= resumo_filho->balanco.EnergiaEntrada;
		 break;
		 }
	  }

   delete lisRESUMO_MON;
   }
*/

//---------------------------------------------------------------------------
void __fastcall TResumos::TrataResumoPaiFilho(VTResumo *resumo, TList *lisRESUMO)
   {
   //variáveis locais
   VTResumo *resumo_pai, *resumo_filho;
   TList *lisRESUMO_JUS = new TList();

   for(int nr = 0; nr < lisRESUMO->Count; nr++)
	  {
	  resumo_pai = (VTResumo*)lisRESUMO->Items[nr];
	  lisRESUMO_JUS->Clear();
	  LisResumoJusante((VTNode*)resumo_pai->object, lisRESUMO_JUS);
	  for(int n = 0; n < lisRESUMO_JUS->Count; n++)
		 {
		 resumo_filho = (VTResumo*)lisRESUMO_JUS->Items[n];
		 //verifica se é do mesmo tipo de rede
		 if(resumo_pai->rede.TipoRedeId != resumo_filho->rede.TipoRedeId)
			continue;
		 //verifica se resumo_filho está na lista de resumos que foram agregados
		 if(lisRESUMO->IndexOf(resumo_filho) < 0)
			continue;
		 //subtrai energia entrada do resumo_filho, para não ser contada 2x
		 resumo->balanco.EnergiaEntrada -= resumo_filho->balanco.EnergiaEntrada;
		 //break;
		 }
	  }

   delete lisRESUMO_JUS;
   }

//---------------------------------------------------------------------------
//eof

