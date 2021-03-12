//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <algorithm>
#include "TMNet.h"
#include "VTArea.h"
#include "VTCorte.h"
#include "VTCluster.h"
#include "VTMRede.h"
#include "VTRede.h"
#include "VTReducao.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
using namespace std;

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTMNet
//---------------------------------------------------------------------------
VTMNet* __fastcall NewObjMNet(void)
   {
   return(new TMNet());
   }

//---------------------------------------------------------------------------
__fastcall TMNet::TMNet(void)
   {
   //pbar_ini = NULL;
   //cria objetos
   lisMR    = new TList();
   lisREDU  = new TList();
   lisCORTE = new TList();
   lisTMP   = new TList();
   lisGER   = new TList();
   lisSUP   = new TList();
   //inicia dados
   Enabled = true;
   }

//---------------------------------------------------------------------------
__fastcall TMNet::~TMNet(void)
   {
   //destrói lista e seus objetos
   if (lisREDU)  {LimpaTList(lisREDU); delete lisREDU;  lisREDU  = NULL;}
   //destrói lista sem destruir seus objetos
   if (lisMR)    {delete lisMR;    lisMR    = NULL;}
   if (lisCORTE) {delete lisCORTE; lisCORTE = NULL;}
   if (lisTMP)   {delete lisTMP;   lisTMP   = NULL;}
   if (lisGER)   {delete lisGER;   lisGER   = NULL;}
   if (lisSUP)   {delete lisSUP;   lisSUP   = NULL;}
   }

//---------------------------------------------------------------------------
bool __fastcall TMNet::ExisteCorte(VTCorte *corte)
   {
   return(lisCORTE->IndexOf(corte) >= 0);
   }

//---------------------------------------------------------------------------
VTCorte* __fastcall TMNet::ExisteCorte(VTMNet *mnet)
   {
   //variáveis locais
   VTCorte *corte;
   TList   *lisEXT;

   //obtém lista de Cortes da MNet indicada
   lisEXT = mnet->LisCorte();
   //verifica se existe algum Corte comum c/ a MNet indicada
   for (int n = 0; n < lisCORTE->Count; n++)
      {
      corte = (VTCorte*)lisCORTE->Items[n];
      if (lisEXT->IndexOf(corte) >= 0) return(corte);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
bool __fastcall TMNet::ExisteMRede(VTMRede *mrede)
   {
   return(lisMR->IndexOf(mrede) >= 0);
   }

//---------------------------------------------------------------------------
VTMRede* __fastcall TMNet::ExisteMRede(VTRede *rede)
   {
   //variáveis locais
   VTMRede *mrede;

   for (int n = 0; n < lisMR->Count; n++)
      {
      mrede = (VTMRede*)lisMR->Items[n];
      if (mrede->ExisteRede(rede)) return(mrede);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
VTMRede* __fastcall TMNet::ExisteMRedeCarregada(void)
   {
   //variáveis locais
   VTMRede *mrede;

   for (int n = 0; n < lisMR->Count; n++)
      {
      mrede = (VTMRede*)lisMR->Items[n];
      if (mrede->Carregada) return(mrede);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
VTReducao* __fastcall TMNet::ExisteReducao(VTCorte *corte)
   {
   //variáveis locais
   VTReducao *reducao;

   //analisa o Corte de cada Reducao
   for (int nr = 0; nr < lisREDU->Count; nr++)
      {
      reducao = (VTReducao*)lisREDU->Items[nr];
      if (reducao->Corte == corte) return(reducao);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
void __fastcall TMNet::IncorporaMNet(TList *lisEXT)
   {
   //variáveis locais
   VTMNet *mnet;

   //loop p/ todos objetos MNet em lisMR
   for (int n = 0; n < lisEXT->Count; n++)
      {
      mnet = (VTMNet*)lisEXT->Items[n];
      //incorpora MNet
      IncorporaMNet(mnet);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMNet::IncorporaMNet(VTMNet *mnet)
   {
   //variáveis locais
   VTMRede   *mrede;
   VTReducao *reducao;
   struct   {
            TList *lisMR;
            TList *lisREDU;
            } ext;

   //insere as MRedes da MNet indicada
   ext.lisMR = mnet->LisMRede();
   for (int n = 0; n < ext.lisMR->Count; n++)
      {
      mrede = (VTMRede*)ext.lisMR->Items[n];
      InsereMRede(mrede);
      }
   //incorpora as Reducoes da MNet indicada
   ext.lisREDU = mnet->LisReducao();
   for (int nr = 0; nr < ext.lisREDU->Count; nr++)
      {
      reducao = (VTReducao*)ext.lisREDU->Items[nr];
      InsereReducao(reducao, reducao->Corte);
      }
   //limpa lista
   ext.lisREDU->Clear();
   }

//---------------------------------------------------------------------------
void __fastcall TMNet::IniciaLisCorte(void)
   {
   //variáveis locais
   VTCorte  *corte;
   VTMRede  *mrede, *mrede_viz;
   struct {TList *lisCOR;} ext;

   //reinicia lista lisCOR
   lisCORTE->Clear();
   //verifica se há apenas uma MRede em lisMR
   if (lisMR->Count == 1)
      {//os Cortes da MNet são os próprios Cortes da MRede
      mrede = (VTMRede*)lisMR->First();
      ext.lisCOR = mrede->LisCorte();
      for (int nc = 0; nc < ext.lisCOR->Count; nc++)
         {
         corte = (VTCorte*)ext.lisCOR->Items[nc];
         InsereCorte(corte);
         }
      return;
      }
   //loop p/ todas as MRede
   for (int n = 0; n < lisMR->Count; n++)
      {
      mrede = (VTMRede*)lisMR->Items[n];
      //obtém lista de Cortes da MRede
      ext.lisCOR = mrede->LisCorte();
      //loop p/ todos Cortes da MRede
      for (int nc = 0; nc < ext.lisCOR->Count; nc++)
         {
         corte = (VTCorte*)ext.lisCOR->Items[nc];
         //verifica se o Corte já existe em lisCORTE
         //if (lisCORTE->IndexOf(corte) >= 0) continue;
         //obtém lista de MRedes do Corte
         lisTMP->Clear();
         corte->LisMRede(lisTMP);
         //insere Corte somente se possuir um conjunto de MRedes diferente ao da MNet
         //JCG 2011.03.04
         //if (! Lista1IgualLista2(lisMR, lisTMP))
         if (! Lista1ContemLista2(lisMR, lisTMP))
            {//Corte possui MRede que pertence à outra MTRede: insere Corte
            InsereCorte(corte);
            }
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMNet::InsereCorte(VTCorte *corte)
   {
   //proteção
   if (corte == NULL) return;
   //verifica se o Corte já existe em lisCORTE
   if (lisCORTE->IndexOf(corte) < 0)
      {//insere Corte em lisCORTE
      lisCORTE->Add(corte);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMNet::InsereMRede(VTMRede *mrede)
   {
   //salva MRede na lista
   if (lisMR->IndexOf(mrede) < 0)
      {
      lisMR->Add(mrede);
      //associa MRede c/ esta MNet
      mrede->MNet = this;
      }
   //inicia lista de Cortes
   IniciaLisCorte();
   }

//---------------------------------------------------------------------------
void __fastcall TMNet::InsereMRede(TList *lisEXT)
   {
   //variáveis locais
   VTMRede *mrede;

   //loop p/ todas as MRedes
   for (int n = 0; n < lisEXT->Count; n++)
      {
      mrede = (VTMRede*)lisEXT->Items[n];
      InsereMRede(mrede);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMNet::InsereReducao(VTReducao *reducao, VTCorte *corte)
   {
   //proteção
   if ((reducao == NULL)||(corte == NULL)) return;
   //insere Corte
   InsereCorte(corte);
   //insere reducao em lisREDU
   if (lisREDU->IndexOf(reducao) < 0) lisREDU->Add(reducao);
   //associa Reducao c/ esta MNet
   //reducao->MNet = this;
   //associa Reducao c/ Corte
   reducao->Corte = corte;
   }

//---------------------------------------------------------------------------
void __fastcall TMNet::LisBarra(TList *lisEXT, bool barra_isolada)
   {
   //variáveis locais
   VTMRede    *mrede;

   //loop p/ todas as MRedes
   for (int n = 0; n < lisMR->Count; n++)
      {
      mrede = (VTMRede*)lisMR->Items[n];
      mrede->LisBarra(lisEXT, barra_isolada);
      }
   }

//---------------------------------------------------------------------------
TList* __fastcall TMNet::LisCorte(void)
   {
   return(lisCORTE);
   }

//---------------------------------------------------------------------------
void __fastcall TMNet::LisCorte(TList *lisEXT)
   {
   //variáveis locais
   VTCorte *corte;

   //loop p/ todos Cortes
   for (int n = 0; n < lisCORTE->Count; n++)
      {
      corte = (VTCorte*)lisCORTE->Items[n];
      if (lisEXT->IndexOf(corte) < 0) lisEXT->Add(corte);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMNet::LisEqbar(TList *lisEXT, int tipo, bool barra_isolada)
   {
   //variáveis locais
   VTMRede    *mrede;

   //loop p/ todas as MRedes
   for (int n = 0; n < lisMR->Count; n++)
      {
      mrede = (VTMRede*)lisMR->Items[n];
      mrede->LisEqbar(lisEXT, tipo, barra_isolada);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMNet::LisEqpto(TList *lisEXT, int tipo, bool barra_isolada)
   {
   //verifica tipo de eqpto solicitado
   if      (tipo == eqptoBARRA)   LisBarra(lisEXT, barra_isolada);
   else if (tipo == eqptoEQBAR)   LisEqbar(lisEXT, barra_isolada);
   else if (tipo == eqptoLIGACAO) LisLigacao(lisEXT);
   else if (tipo == eqptoMUTUA)   LisMutua(lisEXT);
   else if (tipo == -1)
      {//todos os equipamentos
      LisBarra(lisEXT, barra_isolada);
      LisEqbar(lisEXT, barra_isolada);
      LisLigacao(lisEXT);
      LisMutua(lisEXT);
      }
   else
      {//um tipo de equipamento específico (Eqbar ou Ligacao)
      LisEqbar(lisEXT, tipo, barra_isolada);
      LisLigacao(lisEXT, tipo);
      }
   }

//---------------------------------------------------------------------------
TList* __fastcall TMNet::LisGerador(void)
   {
   return lisGER;
   }

//---------------------------------------------------------------------------
void __fastcall TMNet::LisLigacao(TList *lisEXT, int tipo)
   {
   //variáveis locais
   VTMRede    *mrede;

   //loop p/ todas as MRedes
   for (int n = 0; n < lisMR->Count; n++)
      {
      mrede = (VTMRede*)lisMR->Items[n];
      mrede->LisLigacao(lisEXT, tipo);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMNet::LisMNetVizinha(TList *lisEXT)
   {
   //variáveis locais
   VTCorte *corte;

   //loop p/ todos Cortes
   for (int nc = 0; nc < lisCORTE->Count; nc++)
      {
      corte = (VTCorte*)lisCORTE->Items[nc];
      //obtém lista de MNets vizinhas neste Corte
      LisMNetVizinha(corte, lisEXT);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMNet::LisMNetVizinha(VTCorte *corte, TList *lisEXT)
   {
   //variáveis locais
   VTMNet  *mnet;
   VTMRede *mrede;

   //obtém lista de MRede do Corte
   lisTMP->Clear();
   corte->LisMRede(lisTMP);
   //insere em lisEXT as MNets das MRedes
   for (int n = 0; n < lisTMP->Count; n++)
      {
      mrede = (VTMRede*)lisTMP->Items[n];
      if ((mnet = mrede->MNet) == this) continue;
      //insere MNet em lisEXT
      if (lisEXT->IndexOf(mnet) < 0) lisEXT->Add(mnet);
      }
   }

//---------------------------------------------------------------------------
TList* __fastcall TMNet::LisMRede(void)
   {
   return(lisMR);
   }

//---------------------------------------------------------------------------
void __fastcall TMNet::LisMRede(VTCorte *corte, TList *lisEXT)
   {
   //variáveis locais
   VTMRede *mrede;

   for (int n = 0; n < lisMR->Count; n++)
      {
      mrede = (VTMRede*)lisMR->Items[n];
      //verifica se a MRede conecta o Corte
      if (mrede->ExisteCorte(corte)) lisEXT->Add(mrede);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMNet::LisMRedeCarregada(VTCorte *corte, TList *lisEXT)
   {
   //variáveis locais
   VTMRede *mrede;

   for (int n = 0; n < lisMR->Count; n++)
      {
      mrede = (VTMRede*)lisMR->Items[n];
      //verifica se MRede carregada
      if (mrede->Carregada)
         {//verifica se a MRede conecta o Corte
         if (mrede->ExisteCorte(corte)) lisEXT->Add(mrede);
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMNet::LisMutua(TList *lisEXT)
	{
	//variáveis locais
	VTMRede    *mrede;

	//loop p/ todas as MRedes
	for (int n = 0; n < lisMR->Count; n++)
		{
		mrede = (VTMRede*)lisMR->Items[n];
		mrede->LisMutua(lisEXT);
		}
	}

//---------------------------------------------------------------------------
void __fastcall TMNet::LisRede(TList *lisEXT)
	{
	//variáveis locais
	VTMRede    *mrede;

	//loop p/ todas as MRedes
	for (int n = 0; n < lisMR->Count; n++)
		{
		mrede = (VTMRede*)lisMR->Items[n];
		mrede->LisRede(lisEXT);
		}
	}

//---------------------------------------------------------------------------
TList* __fastcall TMNet::LisReducao(void)
   {
   return(lisREDU);
   }

//---------------------------------------------------------------------------
void __fastcall TMNet::LisReducaoVizinha(TList *lisEXT)
   {
   //variáveis locais
   VTCorte   *corte;
   VTMNet    *mnet;
   VTReducao *reducao;

   //obtém lista de MNet vizinhas
   //CUIDADO: o método LisMNetVizinha() utiliza lisTMP, por isso
   //         obtém lista de MNets vizinhas na própria lisEXT
   LisMNetVizinha(lisEXT);
   //transfere MNRet vizinhas de lisEXT p/ lisTMP
   lisTMP->Clear();
   CopiaTList(lisEXT, lisTMP);
   lisEXT->Clear();
   //loop p/ todos Cortes de MNet
   for (int nc = 0; nc < lisCORTE->Count; nc++)
      {
      corte = (VTCorte*)lisCORTE->Items[nc];
      //verifica se as MNet vizinhas possuem Reducao no Corte
      for (int n = 0; n < lisTMP->Count; n++)
         {
         mnet = (VTMNet*)lisTMP->Items[n];
         if ((reducao = mnet->ExisteReducao(corte)) == NULL) continue;
         //insere Reducao em lisEXT
         if (lisEXT->IndexOf(reducao) < 0) lisEXT->Add(reducao);
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMNet::LisReducaoVizinha(VTCorte *corte, TList *lisEXT)
   {
   //variáveis locais
   VTMNet    *mnet;
   VTReducao *reducao;

   //CUIDADO: o método LisMNetVizinha() utiliza lisTMP, por isso
   //         obtém lista de MNets vizinhas na própria lisEXT
   lisEXT->Clear();
   LisMNetVizinha(lisEXT);
   //transfere MNets vizinhas de lisEXT p/ lisTMP
   lisTMP->Clear();
   CopiaTList(lisEXT, lisTMP);
   lisEXT->Clear();
   for (int nr = 0; nr < lisTMP->Count; nr++)
      {
      mnet = (VTMNet*)lisTMP->Items[nr];
      if ((reducao = mnet->ExisteReducao(corte)) == NULL) continue;
      //insere Reducao em lisEXT
      if (lisEXT->IndexOf(reducao) < 0) lisEXT->Add(reducao);
      }
   }

//---------------------------------------------------------------------------
TList* __fastcall TMNet::LisSuprimento(void)
   {
   return lisSUP;
   }
/*
//---------------------------------------------------------------------------
void __fastcall TMNet::OrdenaMRedes(VTCorte *corte)
   {
   //variáveis locais
   VTMRede *mrede, *mrede_viz;

   //proteção
   if (lisMR->Count <= 1) return;
   //transfere as MRedes de lisMR p/ lisTMP
   lisTMP->Clear();
   CopiaTList(lisMR, lisTMP);
   lisMR->Clear();
   //transfere de lisTMP p/ lisMR as MRedes que conectam o Corte indicado
   for (int n = lisTMP->Count-1; n >= 0; n--)
      {
      mrede = (VTMRede*)lisTMP->Items[n];
      if (mrede->ExisteCorte(corte))
         {
         lisTMP->Delete(n);
         lisMR->Add(mrede);
         //define MRede pai
         mrede->Pai = NULL;
         }
      }
   //ordena demais MRede que restaram em lisTMP a partir das MRede já ordenadas em lisMR
   for (int n = 0; (n < lisMR->Count)&&(lisTMP->Count > 0); n++)
      {
      mrede = (VTMRede*)lisMR->Items[n];
      //loop p/ as MRedes em lisTMP
      for (int i = lisTMP->Count-1; i >= 0; i--)
         {
         mrede_viz = (VTMRede*)lisTMP->Items[i];
         //verifica se há um Corte entre a MRede e a MRede vizinha
         if (mrede->ExisteCorte(mrede_viz) != NULL)
            {//transfere MRede vizinha de lisTMP p/ lisMR
            lisTMP->Delete(i);
            lisMR->Add(mrede_viz);
            //define MRede pai
            mrede_viz->Pai = mrede;
            }
         }
      }
   }
*/
//---------------------------------------------------------------------------
void __fastcall TMNet::OrdenaMRedes(VTCorte *corte)
   {
   //variáveis locais
   VTMRede *mrede, *mrede_viz;
   VTMRede *mrede_pai = NULL;

   //verifica se esta MNet é de geração
   if (Geracao)
      {//cancela ordenação entre as MRedes
      for (int n = 0; n < lisMR->Count; n++)
         {
         mrede = (VTMRede*)lisMR->Items[n];
         mrede->Pai = NULL;
         }
      return;
      }
   //verifica se a MNet pai não é de geração
   if (! Pai->Geracao)
      {//verifica se existe uma única MRede da MNet pai que conecta o Corte
      lisTMP->Clear();
      Pai->LisMRedeCarregada(corte, lisTMP);
      if (lisTMP->Count == 1) mrede_pai = (VTMRede*)lisTMP->First();
      //verifica caso especial: MNet c/ uma única MRede
      if (lisMR->Count == 1)
         {
         mrede = (VTMRede*)lisMR->First();
         mrede->Pai = mrede_pai;
         return;
         }
      }
   //transfere as MRedes de lisMR p/ lisTMP
   lisTMP->Clear();
   CopiaTList(lisMR, lisTMP);
   lisMR->Clear();
   //transfere de lisTMP p/ lisMR as MRedes que conectam o Corte indicado
   for (int n = lisTMP->Count-1; n >= 0; n--)
      {
      mrede = (VTMRede*)lisTMP->Items[n];
      if (mrede->ExisteCorte(corte))
         {
         lisTMP->Delete(n);
         lisMR->Add(mrede);
         //define MRede pai
         //mrede->Pai = NULL;
         mrede->Pai = mrede_pai;
         }
      }
   //ordena demais MRede que restaram em lisTMP a partir das MRede já ordenadas em lisMR
   for (int n = 0; (n < lisMR->Count)&&(lisTMP->Count > 0); n++)
      {
      mrede = (VTMRede*)lisMR->Items[n];
      //loop p/ as MRedes em lisTMP
      for (int i = lisTMP->Count-1; i >= 0; i--)
         {
         mrede_viz = (VTMRede*)lisTMP->Items[i];
         //verifica se há um Corte entre a MRede e a MRede vizinha
         if (mrede->ExisteCorte(mrede_viz) != NULL)
            {//transfere MRede vizinha de lisTMP p/ lisMR
            lisTMP->Delete(i);
            lisMR->Add(mrede_viz);
            //define MRede pai
            mrede_viz->Pai = mrede;
            }
         }
      }
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TMNet::PM_GetCodigo(void)
   {
   //variáveis locais
   VTRede *rede;

   //obtém lista de Redes
   lisTMP->Clear();
   LisRede(lisTMP);
   //usa código da primeira rede
   for (int n = 0; n < lisTMP->Count; n++)
      {
      rede = (VTRede*)lisTMP->Items[n];
      if (! rede->Codigo.IsEmpty()) return(rede->Codigo);
      }
   return("SemNome");
   }

//---------------------------------------------------------------------------
bool __fastcall TMNet::PM_GetEnabled(void)
   {
   return(PD.enabled);
   }

//---------------------------------------------------------------------------
bool __fastcall TMNet::PM_GetGeracao(void)
	{
	//variáveis locais
	VTMRede    *mrede;

	//verifica se existe alguma MRede de geracao
	for (int n = 0; n < lisMR->Count; n++)
		{
		mrede = (VTMRede*)lisMR->Items[n];
		if (mrede->Geracao) return(true);
		}
	return(false);
	}

//---------------------------------------------------------------------------
VTNet* __fastcall TMNet::PM_GetNet(void)
   {
   return(PD.net);
   }

//---------------------------------------------------------------------------
int __fastcall TMNet::PM_GetNumeroGerSup(void)
	{
	//variáveis locais
	VTMRede    *mrede;
	int        count = 0;

	//verifica se existe alguma MRede de geracao
	for (int n = 0; n < lisMR->Count; n++)
		{
		mrede  = (VTMRede*)lisMR->Items[n];
		count += mrede->NumeroDeGerSup();
		}
	return(count);
	}

//---------------------------------------------------------------------------
int __fastcall TMNet::PM_GetNumeroMRede(void)
	{
	return(lisMR->Count);
	}

//---------------------------------------------------------------------------
TObject* __fastcall TMNet::PM_GetObj(void)
   {
   return(PD.obj);
   }

//---------------------------------------------------------------------------
VTMNet* __fastcall TMNet::PM_GetPai(void)
   {
   return(PD.pai);
   }

//---------------------------------------------------------------------------
void __fastcall TMNet::PM_SetEnabled(bool enabled)
   {
   PD.enabled = enabled;
   }

//---------------------------------------------------------------------------
void __fastcall TMNet::PM_SetNet(VTNet* net)
   {
   PD.net = net;
   }

//---------------------------------------------------------------------------
void __fastcall TMNet::PM_SetObj(TObject* obj)
   {
   PD.obj = obj;
   }

//---------------------------------------------------------------------------
void __fastcall TMNet::PM_SetPai(VTMNet *pai)
   {
   PD.pai = pai;
   }

//---------------------------------------------------------------------------
//eof

