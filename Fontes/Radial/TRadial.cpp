//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TRadial.h"
#include "VTArvore.h"
#include "VTNode.h"
#include "VTPrimario.h"
#include "VTSecundario.h"
#include "VTReticulado.h"
#include "VTSubestacao.h"
#include "..\Apl\VTApl.h"
#include "..\Rede\VTArea.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTCluster.h"
#include "..\Rede\VTCorte.h"
#include "..\Rede\VTMNet.h"
#include "..\Rede\VTMRede.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTTipoRede.h"
#include "..\Rede\VTTipos.h"
#include "..\..\DLL_Inc\Funcao.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTRadial* __fastcall NewObjRadial(VTApl *apl)
   {
   try{
      return (new TRadial(apl));
      }catch (Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TRadial::TRadial(VTApl *apl)
   {
   //salva ponteiro p/ objeto apl
   this->apl = apl;
   //cria objetos locais
   lisEQP        = new TList();
   lisBAR        = new TList();
   lisVIZ        = new TList();
   lisSUBESTACAO = new TList();
   lisPRIMARIO   = new TList();
   lisRETICULADO = new TList();
   lisSECUNDARIO = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TRadial::~TRadial(void)
   {
   //destrói objeto
   Reinicia();
   //destrói lista sem destruir seus objetos
   if (lisEQP) {delete lisEQP; lisEQP = NULL;}
   if (lisBAR) {delete lisBAR; lisBAR = NULL;}
   if (lisVIZ) {delete lisVIZ; lisVIZ = NULL;}
   //destrói lista e seus objetos
   if (lisSUBESTACAO) {LimpaTList(lisSUBESTACAO); delete lisSUBESTACAO; lisSUBESTACAO = NULL;}
   if (lisPRIMARIO  ) {LimpaTList(lisPRIMARIO  ); delete lisPRIMARIO  ; lisPRIMARIO   = NULL;}
   if (lisRETICULADO) {LimpaTList(lisRETICULADO); delete lisRETICULADO; lisRETICULADO = NULL;}
   if (lisSECUNDARIO) {LimpaTList(lisSECUNDARIO); delete lisSECUNDARIO; lisSECUNDARIO = NULL;}
   }
   
//---------------------------------------------------------------------------
void __fastcall TRadial::AssociaPrimario(void)
   {
   //variáveis locais
   VTTipoRede   *tiporede;
   VTRede       *rede_etd;
   VTPrimario   *primario;
   VTSubestacao *subestacao;

   //determina TipoRede p/ ETD
   //if ((tiporede = redes->ExisteTipoRede("ETD")) == NULL)  return;
   if ((tiporede = tipos->ExisteTipoRede("ETD", redeETD)) == NULL)  return;
   //loop p/ todo Primario
   for (int n = 0; n < lisPRIMARIO->Count; n++)
      {
      primario = (VTPrimario*)lisPRIMARIO->Items[n];
      //determina Rede ETD vizinha ao Primario
      if ((rede_etd = ExisteRedeVizinha(primario->Rede, tiporede)) != NULL)
         {//determina Subestacao associada ao Primario
         if ((subestacao = ExisteSubestacao(rede_etd)) != NULL)
            {//insere Primario na Subestacao (que faz a associação Primario/Subestacao)
            subestacao->InserePrimario(primario);
            }
         }
      //determina Barra inicial do Primario
      //primario->BarraInicial;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TRadial::AssociaReticulado(void)
   {
   //variáveis locais
   TList        *lisRET_PRI;
   VTPrimario   *primario;
   VTReticulado *reticulado;

   //loop p/ todo Reticulado
   for (int n = 0; n < lisRETICULADO->Count; n++)
      {
      reticulado = (VTReticulado*)lisRETICULADO->Items[n];
      lisRET_PRI = reticulado->LisPrimario();
      //loop p/ todos Primarios do Reticulado
      for (int i = 0; i < lisRET_PRI->Count; i++)
         {
         primario = (VTPrimario*)lisRET_PRI->Items[i];
         //verifica se o Primario está associado a uma Subestacao
         if (primario->Subestacao != NULL)
            {//insere Reticulado na Subestacao (que faz a associação Reticulado/Subestacao)
            primario->Subestacao->InsereReticulado(reticulado);
            }
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TRadial::AssociaSecundario(void)
   {
   //variáveis locais
   VTArea       *area_pri, *area_sec;
   VTCorte      *corte;
   VTRede       *rede_viz;
   VTPrimario   *primario;
   VTReticulado *reticulado;
   VTSecundario *secundario;

   //loop p/ todos Hibridos
   for (int ns = 0; ns < lisSECUNDARIO->Count; ns++)
      {
      secundario = (VTSecundario*)lisSECUNDARIO->Items[ns];
      //obtém lista de Redes vizinhas
      lisEQP->Clear();
      secundario->Rede->LisRedeVizinha(lisEQP);
      //loop p/ todas Redes vizinhas
      for (int nr = 0; nr < lisEQP->Count; nr++)
         {
         rede_viz = (VTRede*)lisEQP->Items[nr];
         if ((primario = ExistePrimario(rede_viz)) == NULL) continue;
         //verifica se o Primario pertence a um Reticulado
         if ((reticulado = ExisteReticulado(primario->Rede)) != NULL)
            {//insere Secundario no Reticulado
            reticulado->InsereSecundario(secundario);
            }
         else
            {//insere Secundario no Primario
            primario->InsereSecundario(secundario);
			//determina Corte
			if ((corte = ExisteCorte(secundario->Rede, rede_viz)) != NULL)
			   {//determina Barra inicial
			   secundario->BarraInicial = (VTBarra*)(corte->LisBarra())->First();
			   //verifica se a Barra inicial da rede é diferente
			   if (secundario->BarraInicial != secundario->Rede->BarraInicial())
				  {//atualiza Barra inicial da Rede
				  secundario->Rede->DefineBarraInicial(secundario->BarraInicial);
				  }
			   }
			}
         break;
         }
      }
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16017631
void __fastcall TRadial::AssociaSubestacaoPrimarioSecundario(VTArvore *arvore)
   {
   //variáveis locais
   TList *lisREDE;
   VTMNet *mnet;
   VTNode *node;
   VTRede *rede;
   VTSecundario *secundario;
   VTPrimario *primario;
   VTSubestacao *subestacao;
   VTCorte      *corte;
   VTRede       *rede_viz;

   for(int n = 0; n < lisSECUNDARIO->Count; n++)
	  {
	  secundario = (VTSecundario*)lisSECUNDARIO->Items[n];
	  node = arvore->ExisteNode(secundario->Rede);
	  if(node == NULL) continue;
	  if(node->Pai == NULL) continue;
	  lisREDE = node->Pai->LisObjectNode();
	  for(int nr = 0; nr < lisREDE->Count; nr++)
		 {
		 rede = (VTRede*)lisREDE->Items[nr];
		 primario = ExistePrimario(rede);
		 if(primario == NULL) continue;
		 if ((corte = ExisteCorte(secundario->Rede, rede)) != NULL)
			{//determina Barra inicial
			secundario->BarraInicial = (VTBarra*)(corte->LisBarra())->First();
			//verifica se a Barra inicial da rede é diferente
			if (secundario->BarraInicial != secundario->Rede->BarraInicial())
				{//atualiza Barra inicial da Rede
				secundario->Rede->DefineBarraInicial(secundario->BarraInicial);
				}
			}
		 primario->InsereSecundario(secundario);
		 break;
		 }
	  }
   for(int n = 0; n < lisPRIMARIO->Count; n++)
	  {
	  primario = (VTPrimario*)lisPRIMARIO->Items[n];
	  node = arvore->ExisteNode(primario->Rede);
	  if(node == NULL) continue;
	  if(node->Pai == NULL) continue;
	  lisREDE = node->Pai->LisObjectNode();
	  for(int nr = 0; nr < lisREDE->Count; nr++)
		 {
		 rede = (VTRede*)lisREDE->Items[nr];
		 subestacao = ExisteSubestacao(rede);
		 if(subestacao == NULL) continue;
		 subestacao->InserePrimario(primario);
		 break;
		 }
	  }
   }

//---------------------------------------------------------------------------
VTCorte* __fastcall TRadial::ExisteCorte(VTRede *rede, VTRede *rede_viz)
   {
   //variáveis locais
   TList   *lisREDE_CORTE;
   VTCorte *corte;
   TList   *lisCORTE = rede->LisCorte();

   //determina Corte com Rede vizinha
   for (int n = 0; n < lisCORTE->Count; n++)
      {
      corte         = (VTCorte*)lisCORTE->Items[n];
      lisREDE_CORTE = corte->LisRede();
      if (lisREDE_CORTE->IndexOf(rede_viz) >= 0) return(corte);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
VTPrimario* __fastcall TRadial::ExistePrimario(VTRede *rede)
   {
   //variáveis locais
   VTPrimario *primario;

   for (int n = 0; n < lisPRIMARIO->Count; n++)
      {
      primario = (VTPrimario*)lisPRIMARIO->Items[n];
      if (primario->Rede == rede) return(primario);
      }
   return(NULL);
   }
/*
//---------------------------------------------------------------------------
VTRede* __fastcall TRadial::ExisteRedeVizinha(VTRede *rede, VTTipoRede *tiporede)
   {
   //variáveis locais
   VTRede *rede_viz;

   //obtém lista de Redes vizinhas da Rede indicada
   lisVIZ->Clear();
   rede->LisRedeVizinha(lisVIZ);
   //procura Rede vizinha do TipoRede indicado
   for (int n = 0; n < lisVIZ->Count; n++)
      {
      rede_viz = (VTRede*)lisVIZ->Items[n];
      if (rede_viz->TipoRede == tiporede) return(rede_viz);
      }
   return(NULL);
   }
*/

//---------------------------------------------------------------------------
VTRede* __fastcall TRadial::ExisteRedeVizinha(VTRede *rede, VTTipoRede *tiporede)
   {
   //variáveis locais
   VTMRede *mrede_viz;
   VTRede  *rede_viz;
   TList   *lisREDE_VIZ;

   //obtém lista de Redes vizinhas da Rede indicada
   lisVIZ->Clear();
   rede->MRede->LisMRedeVizinha(lisVIZ);
   //procura MRede vizinha do TipoRede indicado
   for (int n = 0; n < lisVIZ->Count; n++)
      {
      mrede_viz = (VTMRede*)lisVIZ->Items[n];
      lisREDE_VIZ = mrede_viz->LisRede();
      for (int i = 0; i < lisREDE_VIZ->Count; i++)
         {
         rede_viz = (VTRede*)lisREDE_VIZ->Items[i];
         if (rede_viz->TipoRede == tiporede) return(rede_viz);
         }
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
VTReticulado* __fastcall TRadial::ExisteReticulado(VTRede *rede)
   {
   //variáveis locais
   VTPrimario   *primario;
   VTSecundario *secundario;
   VTReticulado *reticulado;

   //loop p/ todos Reticulados
   for (int n = 0; n < lisRETICULADO->Count; n++)
      {
      reticulado = (VTReticulado*)lisRETICULADO->Items[n];
      if (rede->TipoRede->Segmento == redePRI)
         {
         if ((primario = ExistePrimario(rede)) == NULL) continue;
         if ((reticulado->ExistePrimario(primario)) != NULL) return(reticulado);
         }
      else if (rede->TipoRede->Segmento == redeSEC)
         {
         if ((secundario = ExisteSecundario(rede)) == NULL) continue;
         if ((reticulado->ExisteSecundario(secundario)) != NULL) return(reticulado);
         }
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
VTSecundario* __fastcall TRadial::ExisteSecundario(VTRede *rede)
   {
   //variáveis locais
   VTSecundario *secundario;

   for (int n = 0; n < lisSECUNDARIO->Count; n++)
      {
      secundario = (VTSecundario*)lisSECUNDARIO->Items[n];
      if (secundario->Rede == rede) return(secundario);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
VTSubestacao* __fastcall TRadial::ExisteSubestacao(VTRede *rede)
   {
   //variáveis locais
   VTSubestacao *subestacao;

   for (int n = 0; n < lisSUBESTACAO->Count; n++)
      {
      subestacao = (VTSubestacao*)lisSUBESTACAO->Items[n];
      if (subestacao->Rede == rede) return(subestacao);
      }
   return(NULL);
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TRadial::Inicia(VTRedes *redes)
   {
   //salva ponteiro p/ objeto
   this->redes = redes;
   tipos       = (VTTipos*)apl->GetObject(__classid(VTTipos));
   //destrói eventuais objetos pré-existentes
   Reinicia();
   //cria conjunto de objetos Subestacao, Primario e Secundario
   MontaSubestacao();
   MontaPrimario();
   MontaReticulado();
   MontaSecundario();
   //associa Primario c/ Subestacao
   AssociaPrimario();
   //associa Reticulado c/ Subestacao
   AssociaReticulado();
   //associa Secundario c/ Primario
   AssociaSecundario();
   return(true);
   }
*/
//---------------------------------------------------------------------------
bool __fastcall TRadial::Inicia(VTRedes *redes)
   {
   //variáveis locais
   VTArvore *arvore;

   //salva ponteiro p/ objeto
   this->redes = redes;
   tipos       = (VTTipos*)apl->GetObject(__classid(VTTipos));
   //destrói eventuais objetos pré-existentes
   Reinicia();
   //cria conjunto de objetos Subestacao, Primario e Secundario
   MontaSubestacao();
   MontaPrimario();
   MontaReticulado();
   MontaSecundario();
   try
	  {//monta árvore de redes
	  arvore = NewObjArvore(apl);
	  arvore->TipoArvore = arvoreREDE;
	  if(arvore->Inicia(redes->LisMRede()))
		 {//implementação baseada na árvore de MRedes sem considerar gerador
		 AssociaSubestacaoPrimarioSecundario(arvore);
		 //associa Reticulado c/ Subestacao
		 AssociaReticulado();
		 }
	  else
		 {//implementação antiga
		 //associa Primario c/ Subestacao
		 AssociaPrimario();
		 //associa Reticulado c/ Subestacao
		 AssociaReticulado();
		 //associa Secundario c/ Primario
		 AssociaSecundario();
         }
	  delete arvore;
	  }
   catch(Exception &e)
	  {
	  if(arvore) delete arvore;
	  return(false);
	  }
   return(true);
   }

//---------------------------------------------------------------------------
TList* __fastcall TRadial::LisPrimario(void)
   {
   return(lisPRIMARIO);
   }

//---------------------------------------------------------------------------
void __fastcall TRadial::LisRede(TList *lisEXT, int segmento)
   {
   //variáveis locais
   VTSubestacao *subestacao;

   switch(segmento)
      {
      case redeETD:
         for (int n = 0; n < lisSUBESTACAO->Count; n++)
            {
            subestacao = (VTSubestacao*)lisSUBESTACAO->Items[n];
            if (lisEXT->IndexOf(subestacao->Rede) < 0) lisEXT->Add(subestacao->Rede);
            }
         break;
      case redePRI:
      case redeSEC:
         for (int n = 0; n < lisSUBESTACAO->Count; n++)
            {
            subestacao = (VTSubestacao*)lisSUBESTACAO->Items[n];
            subestacao->LisRede(lisEXT, segmento);
            }
         break;
      }
   }

//---------------------------------------------------------------------------
TList* __fastcall TRadial::LisSecundario(void)
   {
   return(lisSECUNDARIO);
   }

//---------------------------------------------------------------------------
TList* __fastcall TRadial::LisSubestacao(void)
   {
   return(lisSUBESTACAO);
   }

//---------------------------------------------------------------------------
void __fastcall TRadial::MontaPrimario(void)
   {
   //variáveis locais
   VTTipoRede *tiporede;
   VTRede     *rede;
   VTPrimario *primario;

   try{//obtém TipoRede p/ Rede primária
      //if ((tiporede = tipos->ExisteTipoRede("RedePri")) == NULL) return;
      if ((tiporede = tipos->ExisteTipoRede("RedePri", redePRI)) == NULL) return;
      //obtém lista de Redes do TipoRede primária
      lisEQP->Clear();
      redes->LisRede(lisEQP, tiporede->Id);
      //cria um Primario p/ cada Rede primária
      for (int n = 0; n < lisEQP->Count; n++)
         {
         rede = (VTRede*)lisEQP->Items[n];
         //descarta Rede primária que está associada a um Cluster
         //if (rede->Cluster != NULL) continue;
         //cria objeto Primario e associa c/ Rede
         primario       = NewObjPrimario(apl);
         primario->Rede = rede;
         //insere Primario em lisPRIMARIO
         lisPRIMARIO->Add(primario);
         }
      }catch(Exception &e)
         {
         }
   }

//---------------------------------------------------------------------------
void __fastcall TRadial::MontaReticulado(void)
   {
   //variáveis locais
   VTCluster    *cluster;
   VTRede       *rede;
   VTPrimario   *primario;
   VTReticulado *reticulado;
   VTSecundario *secundario;
   TList        *lisREDE;
   TList        *lisCLUSTER = redes->LisCluster();

   try{//cria um Reticulado p/ cada Cluster
      for (int n = 0; n < lisCLUSTER->Count; n++)
         {
         cluster = (VTCluster*)lisCLUSTER->Items[n];
         //cria objeto Reticulado e associa c/ Cluster
         reticulado          = NewObjReticulado();
         reticulado->Cluster = cluster;
         //insere Redes do Cluster no Reticulado
         lisREDE = cluster->LisRede();
         for (int nr = 0; nr < lisREDE->Count; nr++)            
            {
            rede = (VTRede*)lisREDE->Items[nr];
            if ((primario = ExistePrimario(rede)) != NULL) 
               {//insere Primario no Reticulado
               reticulado->InserePrimario(primario);
               }
            else if ((secundario = ExisteSecundario(rede)) != NULL) 
               {//insere Secundario no Reticulado
               reticulado->InsereSecundario(secundario);
               }
            }
         //insere Reticulado em lisRETICULADO
         lisRETICULADO->Add(reticulado);
         }
      }catch(Exception &e)
         {
         }
   }


//---------------------------------------------------------------------------
void __fastcall TRadial::MontaSecundario(void)
   {
   //variáveis locais
   VTRede       *rede;
   VTSecundario *secundario;
   VTTipoRede   *tiporede;

   try{//obtém TipoRede p/ Rede secundária
      //if ((tiporede = tipos->ExisteTipoRede("RedeSec")) == NULL) return;
      if ((tiporede = tipos->ExisteTipoRede("RedeSec", redeSEC)) == NULL) return;
      lisEQP->Clear();
      redes->LisRede(lisEQP, tiporede->Id);
      //cria um Secundario p/ cada Rede
      for (int n = 0; n < lisEQP->Count; n++)
         {
         rede = (VTRede*)lisEQP->Items[n];
         //cria objeto Secundario e associa c/ Rede
         secundario       = NewObjSecundario(apl);
         secundario->Rede = rede;
         //insere Secundario em lisSECUNDARIO
         lisSECUNDARIO->Add(secundario);
         }
      }catch(Exception &e)
         {
         }
   }

//---------------------------------------------------------------------------
void __fastcall TRadial::MontaSubestacao(void)
   {
   //variáveis locais
   VTTipoRede    *tiporede;
   VTRede       *rede;
   VTSubestacao *subestacao;

   try{//obtém TipoRede p/ ETD
      //if ((tiporede = tipos->ExisteTipoRede("ETD")) == NULL) return;
      if ((tiporede = tipos->ExisteTipoRede("ETD", redeETD)) == NULL) return;
      //obtém lista de Redes do TipoRede ETD
      lisEQP->Clear();
      redes->LisRede(lisEQP, tiporede->Id);
      //cria uma Subestacao p/ cada Rede ETD
      for (int n = 0; n < lisEQP->Count; n++)
         {
         rede       = (VTRede*)lisEQP->Items[n];
         subestacao = NewObjSubestacao(apl);
         subestacao->Rede = rede;
		 //insere Subestacao em lisSUBESTACAO
         lisSUBESTACAO->Add(subestacao);
         }
      }catch(Exception &e)
         {
         }
   }

//---------------------------------------------------------------------------
void __fastcall TRadial::Reinicia(void)
   {
   //destrói objetos
   LimpaTList(lisPRIMARIO);
   LimpaTList(lisSECUNDARIO);
   LimpaTList(lisSUBESTACAO);
   LimpaTList(lisRETICULADO);
   }

//---------------------------------------------------------------------------
//eof
