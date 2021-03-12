//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TMontaArvore.h"
#include "VTArvore.h"
#include "VTNode.h"
#include "..\Apl\VTApl.h"
#include "..\Bloco\VTBloco.h"
#include "..\Bloco\VTBlocos.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTMNet.h"
#include "..\Rede\VTMRede.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Topologia\VTMontaMNet.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Topologia.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
static int __fastcall ComparaCodRede(void *item1, void *item2)
   {//DVK 2017.02.16
   VTRede *rede1 = (VTRede*) item1;
   VTRede *rede2 = (VTRede*) item2;

   if(CompareText(rede1->Codigo, rede2->Codigo) > 0) return ( 1);
   if(CompareText(rede1->Codigo, rede2->Codigo) < 0) return (-1);
   return (0);
   }

//---------------------------------------------------------------------------
__fastcall TMontaArvore::TMontaArvore(VTApl *apl)
   {
   //salva ponteiro p/ objeto apl
   this->apl = apl;
   //cria objetos locais
   lisBLO_ORD  = new TList();
//   lisBLO_VIZ  = new TList();
   lisMNET_ORD = new TList();
   lisMNET_ISO = new TList();
   lisTMP = new TList();
   lisORD = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TMontaArvore::~TMontaArvore(void)
   {
   //destrói lista sem destruir seus objetos
   if (lisBLO_ORD) {delete lisBLO_ORD; lisBLO_ORD = NULL;}
   if (lisTMP)     {delete lisTMP;     lisTMP = NULL;}
   if (lisORD)     {delete lisORD;     lisORD = NULL;}
//   if (lisBLO_VIZ) {delete lisBLO_VIZ; lisBLO_VIZ = NULL;}
   //destrói lista e seus objetos
   if (lisMNET_ORD) {LimpaTList(lisMNET_ORD); delete lisMNET_ORD; lisMNET_ORD = NULL;}
   if (lisMNET_ISO) {LimpaTList(lisMNET_ISO); delete lisMNET_ISO; lisMNET_ISO = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TMontaArvore::DefineCodigoNode(VTNode *node)
   {
   //variáveis locais
   TList  *lisREDE = node->LisObjectNode();
   VTRede *rede;

   //verifica se existe apenas uma Rede em lisREDE
   if (lisREDE->Count == 0)
      {
      node->Codigo = "Vazia";
      }
   else if (lisREDE->Count == 1)
      {
      node->Codigo = ((VTRede*)lisREDE->First())->Codigo;
      }
   else
      {//insere código da primeira rede da lista, em ordem alfabética
      //DVK 2017.02.16
      lisREDE->Sort(ComparaCodRede);
      rede = ((VTRede*)lisREDE->First());
      node->Codigo = "Grupo " + IntToStr(lisREDE->Count) + " redes " +
                     "(" + rede->Codigo + "...)";
      }
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TMontaArvore::Executa(VTArvore *arvore, VTBlocos *blocos)
   {
   //variáveis locais
   VTNode      *node;
   VTBloco     *bloco;

   //proteção
   if ((arvore == NULL)||(blocos == NULL)) return(false);
   //salva ponteiro p/ objetos
   this->arvore = arvore;
   this->blocos = blocos;
   //reinicia Arvore já existente
   arvore->Clear();
   //obtém conjunto de Blocos com geração
   lisBLO_ORD->Clear();
   blocos->LisBlocoGeracao(lisBLO_ORD);
   //loop p/ todos Blocos em lisBLO_ORD, em ordem inversa porque nessa lista
   //também inseridos os demais Blocos filhos, à medida que a Arvore é montada
   for (int n = lisBLO_ORD->Count - 1; n >= 0; n--)
      {
      bloco = (VTBloco*)lisBLO_ORD->Items[n];
      //insere um ramo completo da Arvore a partir do Bloco com geração
      node = arvore->InsereNode(NULL);
      InsereNode(node, bloco);
      }
   return(arvore);
   }
*/
//---------------------------------------------------------------------------
bool __fastcall TMontaArvore::Executa(VTArvore *arvore, VTBlocos *blocos)
   {//DVK 2015.05.18 reescrito, considerando condição de blocos com gerador
   //variáveis locais
   VTNode      *node;
   VTBloco     *bloco;
   TList         *lisGER, *lisSUP;

   //proteção
   if ((arvore == NULL)||(blocos == NULL)) return(false);
   //salva ponteiro p/ objetos
   this->arvore = arvore;
   this->blocos = blocos;
   //cria lista
   lisGER = new TList();
   lisSUP = new TList();
   //reinicia Arvore já existente
   arvore->Clear();
   //obtém conjunto de Blocos com geração
   lisBLO_ORD->Clear();
   blocos->LisBlocoGeracao(lisBLO_ORD);
   //ordena blocos: primeiro com suprimento, depois com gerador //DVK 2016.01.12
   Ordena(lisBLO_ORD);
   //loop p/ todos Blocos em lisBLO_ORD, em ordem inversa porque nessa lista
   //também inseridos os demais Blocos filhos, à medida que a Arvore é montada
   for (int n = lisBLO_ORD->Count - 1; n >= 0; n--)
      {
      bloco = (VTBloco*)lisBLO_ORD->Items[n];
      lisGER->Clear();
      lisSUP->Clear();
      //node raiz composto somente por blocos com suprimento
      bloco->LisEqbar(lisGER, eqptoGERADOR);
	  bloco->LisEqbar(lisSUP, eqptoSUPRIMENTO);
      if((lisSUP->Count == 0) && (lisGER->Count > 0))
         {//ignora bloco sem suprimento e com gerador
         lisBLO_ORD->Remove(bloco);
         continue;
         }
      //insere um ramo completo da Arvore a partir do Bloco com geração
      node = arvore->InsereNode(NULL);
	  InsereNode(node, bloco);
	  }
   //destroi lista
   delete lisGER;
   delete lisSUP;
   return(arvore);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaArvore::Executa(VTArvore *arvore, VTRedes *redes, bool incluir_rede_isolada)
   {
   //variáveis locais
   VTNode      *node, *node_pai, *node_conexa, *node_isolada;
   VTMNet      *mnet;

   //proteção
   if ((arvore == NULL)||(redes == NULL)) return(false);
   //salva ponteiro p/ objeto
   this->arvore = arvore;
   this->redes  = redes;
   //reinicia Arvore já existente
   arvore->Clear();
   //obtém lista de MNets ordenadas e isoladas
   if (! IniciaLisMNet()) return(false);
   //cria um objeto Node p/ redes conexas
   node_conexa = arvore->InsereNode(NULL);
   //define código p/ o Node
   node_conexa->Codigo = "Redes";
   //inicia estrutura hierárquica, c/ as MNet de geração
   for (int n = 0; n < lisMNET_ORD->Count; n++)
      {
      mnet = (VTMNet*)lisMNET_ORD->Items[n];
      //verifica se MNet de Geracao
      if (! mnet->Geracao) break;
      //cria um objeto Node p/ a MNet e insere em Node conexa
      node_conexa->InsereNode(node = NewObjNode());
      //associa MNet com Node e vice-versa
      mnet->Obj    = node;
      node->Object = mnet;
      //node->Pai    = NULL;
      //insere Redes da MNet no objeto Node
      mnet->LisRede(node->LisObjectNode());
      //define código p/ o Node
      DefineCodigoNode(node);
      }
   //completa estrutura hierárquica, c/ as MNet sem geração
   for (int n = 0; n < lisMNET_ORD->Count; n++)
      {
      mnet = (VTMNet*)lisMNET_ORD->Items[n];
      //verifica se MNet de Geracao
      if (mnet->Geracao) continue;
      //cria um objeto Node p/ a MNet e associa os dois objetos
      node         = NewObjNode();
      mnet->Obj    = node;
      node->Object = mnet;
      //insere Redes da MNet no objeto Node
      mnet->LisRede(node->LisObjectNode());
      //define código p/ o Node
      DefineCodigoNode(node);
      //verifica se a MNet possui MNet pai
      if (mnet->Pai != NULL)
         {//determina Node da MNet Pai
         node_pai = (VTNode*)(mnet->Pai->Obj);
         }
      else
         {//MNet não possui MNet pai (OBS: isto nunca deve ocorrer)
         node_pai = NULL;
         }
      //verifica se existe Node pai
      if (node_pai != NULL)
         {//insere Node no Node pai
         node_pai->InsereNode(node);
         }
      else
         {//insere Node na lista de Node desconexa  (OBS: isto nunca deve ocorrer)
         node_conexa->InsereNode(node);
         }
      }
   if (incluir_rede_isolada)
      {//insere novo Node raiz para as redes das MNets isoladas
      if (lisMNET_ISO->Count > 0)
         {//cria um objeto Node p/ redes isoladas
         node_isolada = arvore->InsereNode(NULL);
         //define código p/ o Node
         node_isolada->Codigo = "Redes Isoladas";
         for (int n = 0; n < lisMNET_ISO->Count; n++)
            {
            mnet = (VTMNet*)lisMNET_ISO->Items[n];
            //NÃO associa MNet com Node e vice-versa
            mnet->Obj            = NULL;
            node_isolada->Object = NULL;
            //node->Pai    = NULL;
            //insere Redes da MNet no objeto Node
            mnet->LisRede(node_isolada->LisObjectNode());
            }
		 }
      }
   //define IDs dos Nos
   arvore->DefineNodeId();
   return(arvore);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaArvore::Executa(VTArvore *arvore, TList *lisMREDE)
   {
   //variáveis locais
   VTNode      *node, *node_pai, *node_conexa, *node_isolada;
   VTMRede     *mrede;
   TList       *lisMREDESUP;

   //proteção
   if (arvore == NULL) return(false);
   //salva ponteiro p/ objeto
   this->arvore = arvore;
   //reinicia Arvore já existente
   arvore->Clear();
   //cria lista
   lisMREDESUP = new TList();
   lisORD->Clear();

   //obtém lista de MRedes somente com suprimento (sem considerar gerador)
   for(int n = 0; n < lisMREDE->Count; n++)
	  {
	  mrede = (VTMRede*)lisMREDE->Items[n];
	  lisTMP->Clear();
	  mrede->LisEqpto(lisTMP, eqptoSUPRIMENTO, false);
	  if(lisTMP->Count > 0)
		 {
		 OrdenaMRede(mrede, lisMREDE);
		 lisMREDESUP->Add(mrede);
		 }
	  }
   //cria um objeto Node p/ redes conexas
   node_conexa = arvore->InsereNode(NULL);
   //define código p/ o Node
   node_conexa->Codigo = "Redes";
   //inicia estrutura hierárquica, c/ as MRede de geração
   for (int n = 0; n < lisMREDESUP->Count; n++)
	  {
	  mrede = (VTMRede*)lisMREDESUP->Items[n];
	  //cria um objeto Node p/ a MRede e insere em Node conexa
	  node_conexa->InsereNode(node = NewObjNode());
	  //associa MRede com Node e vice-versa
	  mrede->Obj    = node;
	  node->Object = mrede;
	  //node->Pai    = NULL;
	  //insere Redes da MNet no objeto Node
	  mrede->LisRede(node->LisObjectNode());
	  //define código p/ o Node
	  DefineCodigoNode(node);
	  }
   //completa estrutura hierárquica, c/ as MRede sem geração
   for(int n = 0; n < lisORD->Count; n++)
	  {
	  mrede = (VTMRede*)lisORD->Items[n];
	  //verifica se MRede de Geracao
	  if (lisMREDESUP->IndexOf(mrede) >= 0) continue;
	  //cria um objeto Node p/ a MRede e associa os dois objetos
      node         = NewObjNode();
	  mrede->Obj   = node;
	  node->Object = mrede;
	  //insere Redes da MRede no objeto Node
	  mrede->LisRede(node->LisObjectNode());
      //define código p/ o Node
      DefineCodigoNode(node);
	  //verifica se a MRede possui MRede pai
	  if (mrede->Pai != NULL)
		 {//determina Node da MRede Pai
		 node_pai = (VTNode*)(mrede->Pai->Obj);
		 }
      else
		 {//MRede não possui MRede pai (OBS: isto nunca deve ocorrer)
         node_pai = NULL;
         }
      //verifica se existe Node pai
	  if (node_pai != NULL)
         {//insere Node no Node pai
         node_pai->InsereNode(node);
         }
      else
         {//insere Node na lista de Node desconexa  (OBS: isto nunca deve ocorrer)
         node_conexa->InsereNode(node);
         }
      }
//   if (incluir_rede_isolada)
//	  {//insere novo Node raiz para as redes das MNets isoladas
//	  if (lisMNET_ISO->Count > 0)
//		 {//cria um objeto Node p/ redes isoladas
//		 node_isolada = arvore->InsereNode(NULL);
//		 //define código p/ o Node
//		 node_isolada->Codigo = "Redes Isoladas";
//		 for (int n = 0; n < lisMNET_ISO->Count; n++)
//			{
//			mrede = (VTMRede*)lisMNET_ISO->Items[n];
//			//NÃO associa MRede com Node e vice-versa
//			mrede->Obj            = NULL;
//			node_isolada->Object = NULL;
//			//node->Pai    = NULL;
//			//insere Redes da MRede no objeto Node
//			mrede->LisRede(node_isolada->LisObjectNode());
//			}
//		 }
//      }
   //define IDs dos Nos
   arvore->DefineNodeId();

   delete lisMREDESUP;
   return(true);
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/14217067
bool __fastcall TMontaArvore::Executa(VTArvore *arvore, TList *lisLIG, VTBarra *barra_ini)
   {//extraido do relatorio a jusante
   //variáveis locais
   VTBarra   *barra;
   VTChave   *chave;
   VTLigacao *ligacao_filha, *ligacao_pai;
   TList 	  *lisEQP = lisLIG;
   VTNode	  *node_lig_pai;//, *node_bar_pai;
   VTNode	  *node_lig_filho;//, *node_bar_filho;
   TList *lisEXT_BAR;
   TList *lisEXT_LIG;

   //determina Barra inicial
   if ((barra = barra_ini) == NULL)
	  {return(false);}
      //proteção
   if (arvore == NULL) return(false);
   //salva ponteiro p/ objeto
   this->arvore = arvore;
   //reinicia Arvore já existente
   arvore->Clear();
   //cria listas
   lisEXT_BAR = new TList();
   lisEXT_LIG = new TList();
   //inicia lisEXT_BAR com a Barra inicial
   lisEXT_BAR->Clear();
   lisEXT_BAR->Add(barra);
//   node_bar_pai = arvore_bar->InsereNode(NULL);
//   node_bar_pai->InsereObject(barra);
   //define a Barra iniciou sem Ligacao pai que a conectou
   barra->Obj = NULL;
   //inicia lisEXT_LIG com as Ligacoes que conectam a Barra inicial
   lisEXT_LIG->Clear();
   //percorre todas ligacoes
   for (int nl = lisEQP->Count-1; nl >= 0; nl--)
	  {
      ligacao_filha = (VTLigacao*)lisEQP->Items[nl];
      //verifica se é uma Chave aberta
      if (ligacao_filha->Tipo() == eqptoCHAVE)
         {
         chave = (VTChave*)ligacao_filha;
		 if (chave->Aberta)
            {//retira Ligacao de lisEQP
            lisEQP->Delete(nl);
            continue;
            }
         }
      //verifica se a Ligacao conecta a Barra
	  if (ligacao_filha->ConectaBarra(barra))
         {//transfere Ligacao filha de lisEQP p/ lisEXT_LIG
         lisEQP->Delete(nl);
		 lisEXT_LIG->Add(ligacao_filha);
         //define Ligacao pai como nula
		 ligacao_filha->ligaPai = NULL;
		 //cria o node da ligacao
		 node_lig_filho = arvore->InsereNode(NULL);
		 node_lig_filho->InsereObject(ligacao_filha);
         }
	  }
   //loop p/ todas Ligacoes pai que estão em lisEXT_LIG
   for (int nlp = 0; nlp < lisEXT_LIG->Count; nlp++)
      {
	  ligacao_pai = (VTLigacao*)lisEXT_LIG->Items[nlp];
	  //insere o node pai
	  node_lig_pai = arvore->ExisteNode(ligacao_pai);
	  //verifica qual barra ja foi analisada
      for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
	  {
		 if ((barra = ligacao_pai->Barra(ind_bar)) == NULL) continue;
		 //descarta Barras já analisadas
		 if (lisEXT_BAR->IndexOf(barra) >= 0)
		 {
//			node_bar_pai = arvore_bar->ExisteNode(barra);
			continue;
		 }
	  }
	  //loop p/ todas Barras da Ligacao pai
	  for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
	  {
		 if ((barra = ligacao_pai->Barra(ind_bar)) == NULL) continue;
		 //descarta Barras já analisadas
		 if (lisEXT_BAR->IndexOf(barra) >= 0)
		 	{continue;}
		 AnsiString nome_barra_pai, nome_barra_filha;
//		 nome_barra_pai =((VTBarra*)(node_bar_pai->LisObjectNode()->First()))->Codigo;
//		 node_bar_filho = arvore_bar->InsereNode(node_bar_pai);
//		 node_bar_filho->InsereObject(barra);
//		 nome_barra_filha =((VTBarra*)(node_bar_filho->LisObjectNode()->First()))->Codigo;
		 //insere Barra em lisEXT_BAR
		 lisEXT_BAR->Add(barra);
         //salva em Barra a Ligacao pai que a conectou
         barra->Obj = ligacao_pai;
         //determina Ligacoes em lisEQP que se conectam à Barra
		 for (int nl = lisEQP->Count-1; nl >= 0; nl--)
		 {//OBS: as chaves abertas foram excluídas de lisEQP no loop anterior
			ligacao_filha = (VTLigacao*)lisEQP->Items[nl];
            //verifica se a Ligacao conecta a Barra
            if (ligacao_filha->ConectaBarra(barra))
               {//transfere Ligacao filha de lisEQP p/ lisEXT_LIG
               lisEQP->Delete(nl);
               lisEXT_LIG->Add(ligacao_filha);
			   //associa Ligacao filha c/ Ligacao Pai
			   ligacao_filha->ligaPai = ligacao_pai;
			   //cria o node da ligacao
			   node_lig_filho = arvore->InsereNode(node_lig_pai);
			   node_lig_filho->InsereObject(ligacao_filha);
               }
            }
         }
      }
   //limpa listas locais
   lisEQP->Clear();
   delete lisEXT_BAR;
   delete lisEXT_LIG;
   return true;
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaArvore::IniciaLisMNet(void)
   {
   //variáveis locais
   VTMontaMNet *monta_mnet;

   try{//cria objeto MontaMNet
      monta_mnet = DLL_NewObjMontaMNet(apl);
      //cria conjunto ordenado de MNets
      monta_mnet->ExecutaLisMNetOrdenada(redes, lisMNET_ORD, lisMNET_ISO);
      //destrói objeto MontaMNet
      delete monta_mnet;
      }catch(Exception &e)
         {
         return(false);
         }
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaArvore::InsereNode(VTNode *node_pai, VTBloco *bloco_pai)
   {
   //variáveis locais
   VTNode  *node_filho;
   VTBloco *bloco_filho;
   TList   *lisBLO_VIZ = new TList();   //DVK 2015.05.08

   //associa Node pai com Bloco pai e insere Bloco pai no Node pai
   node_pai->Object = bloco_pai;
//   node_pai->InsereObject(bloco_pai);
   //obtém lista de Blocos vizinhos do Bloco pai
   lisBLO_VIZ->Clear();
   //JCG 2017.06.26
   //blocos->LisBlocoVizinho(bloco_pai, lisBLO_VIZ);
   blocos->LisBlocoVizinhoConectado(bloco_pai, lisBLO_VIZ);
   //loop p/ todos Blocos vizinhos
   for (int n = 0; n < lisBLO_VIZ->Count; n++)
	  {
	  bloco_filho = (VTBloco*)lisBLO_VIZ->Items[n];
	  //descarta Bloco vizinho que é pai (e não filho) do Bloco pai
      if (lisBLO_ORD->IndexOf(bloco_filho) >= 0) continue;
	  //insere Bloco vizinho em lisBLO_ORD
	  lisBLO_ORD->Add(bloco_filho);
      //insere um Node filho para o Bloco vizinho
      node_filho = arvore->InsereNode(node_pai);
	  //chamada recursiva para incluir demais Blocos filhos
      InsereNode(node_filho, bloco_filho);
      }
   if(lisBLO_VIZ) delete lisBLO_VIZ;  //DVK 2015.05.08
   }

//https://sinapsisenergia.teamwork.com/#/tasks/16586360
//---------------------------------------------------------------------------
void __fastcall  TMontaArvore::Ordena(TList *lisBLO_ORD)
   {//DVK 2016.01.12
   //variaveis locais
   VTBloco *bloco;
   TList   *lisORDENADA = new TList();
   TList   *lisGER      = new TList();
   TList   *lisSUP      = new TList();

   for(int nb = 0; nb < lisBLO_ORD->Count; nb++)
	  {//insere blocos com suprimento
      bloco = (VTBloco*)lisBLO_ORD->Items[nb];
      lisSUP->Clear();
      bloco->LisEqbar(lisSUP, eqptoSUPRIMENTO);
	  if(lisSUP->Count == 0) continue;
	  if(lisORDENADA->IndexOf(bloco) < 0) lisORDENADA->Add(bloco);
	  }
   for(int nb = 0; nb < lisBLO_ORD->Count; nb++)
	  {//insere blocos com gerador
	  bloco = (VTBloco*)lisBLO_ORD->Items[nb];
      lisGER->Clear();
	  bloco->LisEqbar(lisGER, eqptoGERADOR);
	  if(lisGER->Count == 0) continue;
	  if(lisORDENADA->IndexOf(bloco) < 0) lisORDENADA->Add(bloco);
	  }
   for(int nb = 0; nb < lisBLO_ORD->Count; nb++)
	  {//insere blocos com gerador
	  bloco = (VTBloco*)lisBLO_ORD->Items[nb];
	  if(bloco->ExisteGeracao())
		  {
			  if(lisORDENADA->IndexOf(bloco) < 0) lisORDENADA->Add(bloco);
		  }
	  }
   //transfere itens da lista ordenada
   lisBLO_ORD->Clear();
   CopiaTList(lisORDENADA,lisBLO_ORD);
   //destroi listas
   delete lisGER;
   delete lisSUP;
   delete lisORDENADA;
   }

//---------------------------------------------------------------------------
void __fastcall TMontaArvore::OrdenaMRede(VTMRede *mrede, TList *lisORIG)
   {
   //variáveis locais
   VTMRede *filha, *pai;

   //copia lisORIG p/ lisTMP
   lisTMP->Clear();
   CopiaTList(lisORIG, lisTMP);
   //monta lisORD c/ as Redes ordenadas a partir da MRede indicada
   mrede->Pai = NULL;
   lisORD->Add(mrede);
   //ordena MRedes que estão em lisORIG a partir da MRede pai até encontrar uma MRede de geração
   for (int i = 0; i < lisORD->Count; i++)
	  {
	  pai = (VTMRede*)lisORD->Items[i];
	  //procura MRedes de lisORIG que se conectam à MRede pai
	  for (int j = lisTMP->Count-1; j >= 0; j--)
		 {
		 filha = (VTMRede*)lisTMP->Items[j];
		 //verifica se a MRede filha se conecta c/ a MRede pai
		 if (Lista1IntersecaoLista2(pai->LisCorte(), filha->LisCorte()) == 1)
			{//verifica se a MRede já existe em lisORD
			if (lisORD->IndexOf(filha) < 0)
			   {//define MRede pai da MRede filha
			   filha->Pai = pai;
			   //transfere MRede filha de lisTMP p/ lisORD
			   lisTMP->Delete(j);
			   lisORD->Add(filha);
			   }
			}
		 }
	  }
   }

//---------------------------------------------------------------------------
void __fastcall  TMontaArvore::TreeViewInicia(VTArvore *arvore, TTreeView *TView, bool sort)
   {
   //variáveis locais
   TTreeNode *treenode;
   VTNode    *node;
   TList     *lisNODE = arvore->LisNodeRaiz();

   //salva ponteiro para Arvore
   this->arvore = arvore;
   //reinicia TView
   TView->Items->Clear();
   //loop p/ todos os Nodes em lisNODE
   for (int n = 0; n < lisNODE->Count; n++)
      {
      node  = (VTNode*)lisNODE->Items[n];
      //insere o Node em um TreeNode
      treenode = TViewInsereNode(TView, node);
      //ordena redes do tipo
      if (sort && (treenode != NULL)) treenode->AlphaSort(true);
      }
   }

//---------------------------------------------------------------------------
TTreeNode* __fastcall TMontaArvore::TViewInsereNode(TTreeView *TView, VTNode *node, TTreeNode *treenode_pai)
   {
   //variáveis locais
   TTreeNode *treenode, *treenode_grp;
   TList     *lisNODE;
   TList     *lisREDE;
   VTRede    *rede;

   //verifica se foi definido o TreeNode pai
   if (treenode_pai == NULL)
      {//insere um TreeNode raiz
      treenode = TView->Items->AddObject(NULL, node->Codigo, node);
      }
   else
      {
      treenode = TView->Items->AddChildObject(treenode_pai, node->Codigo, node);
      }
   //verifica número de Redes do Node
   lisREDE = node->LisObjectNode();
   if ((lisREDE->Count == 0)||(lisREDE->Count == 1))
      {//insere TreeNodes p/ os Nodes filhos
      lisNODE = node->LisNode();
      for (int n = 0; n < lisNODE->Count; n++)
         {
         node = (VTNode*)lisNODE->Items[n];
         TViewInsereNode(TView, node, treenode);
         }
      return(treenode);
      }
   else
      {//insere TreeNode c/ o título "Redes do grupo", associado ao Node
      treenode_grp = TView->Items->AddChildObject(treenode, "<Redes do grupo>", node);
      //insere TreeNode p/ cada rede do grupo
      for (int n = 0; n < lisREDE->Count; n++)
         {
         rede = (VTRede*)lisREDE->Items[n];
         //JCG 2016.09.07
         //TView->Items->AddChildObject(treenode_grp, rede->Codigo, node);
         TView->Items->AddChildObject(treenode_grp, rede->Codigo, rede);
         }
      //determina conjunto de Nodes filhos
      lisNODE = node->LisNode();
      if (lisNODE->Count > 0)
         {//insere TreeNodes p/ os Nodes filhos
         for (int n = 0; n < lisNODE->Count; n++)
            {
            node = (VTNode*)lisNODE->Items[n];
            TViewInsereNode(TView, node, treenode);
            }
         }
      return(treenode);
      }
   }

//---------------------------------------------------------------------------
//eof
