//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TMontaMNet.h"
#include "..\Apl\VTapl.h"
#include "..\Curvas\VTCurva.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Potencia\VTDemanda.h"
#include "..\Potencia\VTFornecimento.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCluster.h"
#include "..\Rede\VTCorte.h"
#include "..\Rede\VTMNet.h"
#include "..\Rede\VTMRede.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTReducao.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTSuprimento.h"
#include "..\..\DLL_Inc\Curvas.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Rede.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTMontaMNet
//---------------------------------------------------------------------------
VTMontaMNet* __fastcall NewObjMontaMNet(VTApl *apl)
   {
   return(new TMontaMNet(apl));
   }

//---------------------------------------------------------------------------
__fastcall TMontaMNet::TMontaMNet(VTApl *apl)
   {
   //salva ponteiro p/ objetos
   this->apl = apl;
   patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   //cria objetos
   lisMNET    = new TList();
   lisMR      = new TList();
   lisMR_CAR  = new TList();
   lisMR_GER  = new TList();
   lisORD     = new TList();
   lisTMP     = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TMontaMNet::~TMontaMNet(void)
   {
   //destrói lista sem destruir seus objetos
   if (lisMNET  )  {delete lisMNET  ;  lisMNET   = NULL;}
   if (lisMR    )  {delete lisMR    ;  lisMR     = NULL;}
   if (lisMR_CAR)  {delete lisMR_CAR;  lisMR_CAR = NULL;}
   if (lisMR_GER)  {delete lisMR_GER;  lisMR_GER = NULL;}
   if (lisORD   )  {delete lisORD   ;  lisORD    = NULL;}
   if (lisTMP   )  {delete lisTMP   ;  lisTMP    = NULL;}
   }

//---------------------------------------------------------------------------
VTCarga* __fastcall TMontaMNet::CriaCargaDeReducao(void)
   {
   //variáveis locais
   VTCarga  *carga;

   //cria objeto VTCarga
   if ((carga = DLL_NewObjCarga(patamares->NumPatamar())) != NULL)
      {//define Carga como sendo de uma Reducao
      carga->Id      = FalsoId();
      carga->Reducao = true;
      //cria Curva PQ p/ a Carga
      if (CriaCurva(carga))
         {//inicia Curva c/ demanda por fase nula em todos os patamares
         IniciaCurvaCargaNula(carga);
         }
      else
         {
         delete carga;
         carga = NULL;
         }
      }
   return(carga);
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/index.cfm#/tasks/7056570
bool __fastcall TMontaMNet::CriaCurva(VTCarga *carga)
   {
   //variáveis locais
   VTCurva *curva;

   //cria uma nova Curva
   if ((curva = DLL_NewObjCurva()) == NULL) return(false);
   //define dados da Curva
   curva->Codigo        = carga->Codigo;
   curva->Id            = carga->Id;
   curva->Tipo          = curvaPQ;
   curva->Unidade       = unidMVA;
   curva->Escala        = patamares->Escala;//patamares->NumPatamar();
   curva->Tipica        = false;
   curva->NumeroValores = nvCURVA_RRCAR;  //{p,q} por fase
   //associa Curva c/ a Carga
   carga->DefineCurva(curva);
   return(true);
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/index.cfm#/tasks/7056570
bool __fastcall TMontaMNet::CriaCurva(VTSuprimento *suprimento)
   {
   //variáveis locais
   VTCurva *curva;

   //cria uma nova Curva
   if ((curva = DLL_NewObjCurva()) == NULL) return(false);
   //define dados da Curva
   curva->Codigo        = suprimento->Codigo;
   curva->Id            = suprimento->Id;
   curva->Tipo          = curvaPQVT;
   curva->Unidade       = unidPU;
   curva->Escala        = patamares->Escala;//patamares->NumPatamar();
   curva->Tipica        = false;
   curva->NumeroValores = nvCURVA_RRSUP;  //{p,q,v,t} por fase
   //associa Suprimento c/ a Curva
   suprimento->DefineCurva(curva);
   //inicia Curva do Suprimento c/ 1 pu de tensão por fase em todos patamares
   IniciaCurvaSuprimento1pu(suprimento);
   return(true);
   }

//---------------------------------------------------------------------------
VTReducao* __fastcall TMontaMNet::CriaReducaoComCarga(VTMNet *mnet, VTCorte *corte)
   {
   //variáveis locais
   VTBarra   *barra;
   VTReducao *reducao;
   VTCarga   *carga;
   TList     *lisBAR = corte->LisBarra();

   //cria Reducao
   if ((reducao = DLL_NewObjReducao(patamares->NumPatamar())) == NULL) return(NULL);
   //cria uma Carga p/ cada Barra do Corte
   for (int nb = 0; nb < lisBAR->Count; nb++)
      {
      barra = (VTBarra*)lisBAR->Items[nb];
      if ((carga = CriaCargaDeReducao()) == NULL)
         {
         delete reducao;
         return(NULL);
         }
      //define código da Carga
      carga->Codigo = mnet->Codigo;
      //define modelo de carga como sendo Icte
      carga->Icte_pu = 1;
      carga->Scte_pu = 0;
      carga->Zcte_pu = 0;
      //insere Carga na Reducao
      carga->DefineObjBarra(barra);
      reducao->InsereEqbar(carga);
      }
   return(reducao);
   }

//---------------------------------------------------------------------------
VTReducao* __fastcall TMontaMNet::CriaReducaoComSuprimento(VTMNet *mnet, VTCorte *corte)
   {
   //variáveis locais
   VTBarra      *barra;
   VTReducao    *reducao;
   VTSuprimento *suprimento;
   TList        *lisBAR = corte->LisBarra();

   //cria Reducao
   if ((reducao = DLL_NewObjReducao(patamares->NumPatamar())) == NULL) return(NULL);
   //cria um Suprimento p/ cada Barra do Corte
   for (int nb = 0; nb < lisBAR->Count; nb++)
      {
      barra = (VTBarra*)lisBAR->Items[nb];
      if ((suprimento = CriaSuprimentoDeReducao()) == NULL)
         {
         delete reducao;
         return(NULL);
         }
      //define código do Suprimento
      suprimento->Codigo = mnet->Codigo;
      //insere Carga na Reducao
      suprimento->DefineObjBarra(barra);
      reducao->InsereEqbar(suprimento);
      }
   return(reducao);
   }

//---------------------------------------------------------------------------
VTSuprimento* __fastcall TMontaMNet::CriaSuprimentoDeReducao(void)
   {
   //variáveis locais
   VTSuprimento  *suprimento;

   //cria objeto VTCarga
   if ((suprimento = DLL_NewObjSuprimento(patamares->NumPatamar())) != NULL)
      {//define Carga como sendo de uma Reducao
      suprimento->Id      = FalsoId();
      suprimento->Reducao = true;
		suprimento->tiposup = supVF;
		suprimento->Fases   = faseABC;
      //cria Curva PQ p/ o Suprimento
      if (CriaCurva(suprimento))
         {//inicia Curva c/ 1pu de tensão por fase em todos os patamares
			IniciaCurvaSuprimento1pu(suprimento);
         }
      else
         {
         delete suprimento;
         suprimento = NULL;
         }
      }
   return(suprimento);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaMNet::Executa(VTRedes *redes)
   {
   //salva ponteiro p/ objetos
   this->redes = redes;
   //limpa listas locais
   LimpaListas();
   //obtém lista de MRedes
   redes->LisMRede(lisMR);
   //monta MNets p/ cálculo parcial de Fluxo (MRedes que interligam MRedes com geracao)
   MontaMNetFluxo();
   //cria Reducoes p/ MNets p/ cálculo parcial de Fluxo
   MontaReducaoMNet();
   //atualiza MNets do objeto VTRedes
   redes->ReiniciaLisMNetFluxo(lisMNET);
   //limpa listas que não são necessárias
   LimpaListas();
   }

//---------------------------------------------------------------------------
void __fastcall TMontaMNet::ExecutaLisMNetOrdenada(VTRedes *redes, TList *lisEXT_ORD, TList *lisEXT_ISO)
   {
   //salva ponteiro p/ objetos
   this->redes = redes;
   //limpa listas locais
   LimpaListas();
   //obtém lista de MRedes
   redes->LisMRede(lisMR);
   //monta conjunto de MNets ordenadas geração/carga (sem fusão desnecessária de MNets)
   MontaMNetOrdenada(lisEXT_ORD, lisEXT_ISO);
   //limpa listas locais
   LimpaListas();
   //JCG 2011.09.21
   //IMPORTANTE: torna a iniciar as MNets
   Executa(redes);
   }

 //---------------------------------------------------------------------------
VTMNet* __fastcall TMontaMNet::ExisteMNetSoDeMRedeReduzida(TList *lisMNET)
   {
   //variáveis locais
   VTMNet *mnet;

   //verifica se há alguma MNet sem nenhuma MRede carregada
   for (int n = 0; n < lisMNET->Count; n++)
      {
      mnet = (VTMNet*)lisMNET->Items[n];
      if (mnet->ExisteMRedeCarregada() == NULL) return(mnet);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
VTMRede* __fastcall TMontaMNet::ExisteMRedeDeGeracao(TList *lisORIG)
   {
   //variáveis locais
   VTMRede   *mrede;

   for (int n = 0; n < lisORIG->Count; n++)
      {
      mrede = (VTMRede*)lisORIG->Items[n];
      if (mrede->Geracao) return(mrede);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaMNet::ExisteReducaoComGeracao(TList *lisREDU)
   {
   //variáveis locais
   VTReducao *reducao;

   for (int nr = 0; nr < lisREDU->Count; nr++)
      {
      reducao = (VTReducao*)lisREDU->Items[nr];
      if (reducao->ExisteGeracao()) return(true);
      }
   return(false);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaMNet::FundeMNetsDoCorte(TList *lisMNET, VTCorte *corte)
   {
   //variáveis locais
   VTMNet *mnet;

   //monta lisTMP c/ as MNet do Corte
   lisTMP->Clear();
   for (int n = 0; n < lisMNET->Count; n++)
      {
      mnet = (VTMNet*)lisMNET->Items[n];
      if (mnet->ExisteCorte(corte)) lisTMP->Add(mnet);
      }
   //verifica se há MNets no Corte
   if (lisTMP->Count <= 1) return;
   //funde MNets que estão em lisTMP
   mnet = (VTMNet*)lisTMP->First();
   lisTMP->Delete(0);
   mnet->IncorporaMNet(lisTMP);
   //elimina MNet incorporadas
   RetiraLis1DeLis2(lisTMP, lisMNET);
   LimpaTList(lisTMP);
   }

//---------------------------------------------------------------------------
int __fastcall TMontaMNet::IdentificaMalhaDeGeracao(VTMRede *mrede, TList *lisORIG, TList *lisMRM)
   {
   //variáveis locais
   VTMRede *filha, *pai;

   //copia lisORIG p/ lisTMP
   lisTMP->Clear();
   CopiaTList(lisORIG, lisTMP);
   //monta lisORD c/ as Redes ordenadas a partir da MRede indicada
   lisORD->Clear();
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
   //monta lisMRM c/ as seqüências de MRede de lisORD iniciadas por MRede de geração
   lisMRM->Clear();
   for (int n = lisORD->Count-1; n >= 0; n--)
      {//retira MRede da lista
      filha = (VTMRede*)lisORD->Items[n];
      lisORD->Delete(n);
      if (! filha->Geracao) continue;
      //transfere MRede filha e suas MRede pai p/ lisMRM
      pai = filha;
      while (pai != NULL)
         {//verifica se MRede pai já existe em lisMRM
         if (lisMRM->IndexOf(pai) < 0)
            {//insere MRede em lisMRM
            lisMRM->Add(pai);
            //determina próximo pai
            if ((pai = pai->Pai) == mrede) pai = NULL;
            }
         else
            {//trunca inserção da sequencia de MRedes
            pai = NULL;
            }
         }
      }
   return(lisMRM->Count);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaMNet::IniciaCurvaCargaNula(VTCarga *carga)
   {
   //variáveis locais
   VTDemanda  *demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));

   //reinicia Curva da Carga
   demanda->ReiniciaCurva(carga);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaMNet::IniciaCurvaSuprimento1pu(VTSuprimento *suprimento)
   {
   //variáveis locais
   VTFornecimento *fornecimento  = (VTFornecimento*)apl->GetObject(__classid(VTFornecimento));

   //reinicia Curva do Suprimento c/ 1 pu em todos os patamares
   fornecimento->ReiniciaCurva(suprimento, 0.);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaMNet::LimpaListas(void)
   {
   lisMNET->Clear();
   lisMR->Clear();
   lisORD->Clear();
   lisMR_CAR->Clear();
   lisMR_GER->Clear();
   lisTMP->Clear();
   }

//---------------------------------------------------------------------------
void __fastcall TMontaMNet::MontaMNetFluxo(void)
   {
   //variáveis locais
   VTCorte *corte;
   VTMRede *mrede;
   VTMNet  *mnet, *mnet_viz;
   TList   *lisCOR;

   //inicia lisMNET
   lisMNET->Clear();
   //reinicia lista lisMR_CAR c/ todas as MRedes de lisMR
   lisMR_CAR->Clear();
   CopiaTList(lisMR, lisMR_CAR);
   //cria uma MNet para cada conjunto de MRede que interliga MRedes de geração
   while ((mrede = ExisteMRedeDeGeracao(lisMR_CAR)) != NULL)
      {//verifica se a MRede forma uma malha de geração
      lisMR_GER->Clear();
      IdentificaMalhaDeGeracao(mrede, lisMR_CAR, lisMR_GER);
      //retira as MRedes em lisMR_GER de lisMR_CAR
      RetiraLis1DeLis2(lisMR_GER, lisMR_CAR);
      //cria uma MNet c/ as MRedes em lisMR_GER
      if ((mnet = DLL_NewObjMNet()) == NULL) continue;
      mnet->InsereMRede(lisMR_GER);
	  lisMNET->Add(mnet);
      }
   //cria uma MNet p/ cada MRede sem geração que permaneceram em lisMR_CAR
   for (int n = 0; n < lisMR_CAR->Count; n++)
      {
      mrede = (VTMRede*)lisMR_CAR->Items[n];
      //cria um objeto VTNet e insere em lisMNET
      if ((mnet = DLL_NewObjMNet()) == NULL) continue;
      mnet->InsereMRede(mrede);
      lisMNET->Add(mnet);
      }
   /*
   //funde MNets formadas apenas c/ MRedes reduzidas
   while ((mnet = ExisteMNetSoDeMRedeReduzida(lisMNET)) != NULL)
      {//funde MNet c/ uma MNet vizinha
      lisTMP->Clear();
      mnet->LisMNetVizinha(lisTMP);
      if (lisTMP->Count == 0) break;
      mnet_viz = (VTMNet*)lisTMP->First();
      mnet->IncorporaMNet(mnet_viz);
      //retira de lisMNET a MNet vizinha incorporada e destrói objeto
      lisMNET->Remove(mnet_viz);
      delete mnet_viz;
      }
   */
   //funde MNets formadas apenas c/ MRedes reduzidas
   while ((mnet = ExisteMNetSoDeMRedeReduzida(lisMNET)) != NULL)
      {//funde MNet c/ uma MNet vizinha
      lisTMP->Clear();
      mnet->LisMNetVizinha(lisTMP);
      if (lisTMP->Count == 0) break;
      mnet_viz = (VTMNet*)lisTMP->First();
      mnet_viz->IncorporaMNet(mnet);
      //retira de lisMNET a MNet incorporada e destrói objeto
      lisMNET->Remove(mnet);
      delete mnet;
      }
   //funde MNets em que os Cortes são formados por mais de 1 barra
   lisCOR = redes->LisCorte();
   for (int nc = 0; nc < lisCOR->Count; nc++)
      {
      corte = (VTCorte*)lisCOR->Items[nc];
      if (corte->NumeroBarras() > 1)
         {
         FundeMNetsDoCorte(lisMNET, corte);
         }
      }
   //ordena MNets, colocando no início da lista as MNet com geração
   lisORD->Clear();
   for (int n = lisMNET->Count-1; n >= 0; n--)
      {
      mnet = (VTMNet*)lisMNET->Items[n];
      if (mnet->Geracao)
         {//transfere MNet de lisMNET p/ lisORD
         lisMNET->Delete(n);
         lisORD->Add(mnet);
         //inicia ponteiro p/ MNet pai
         mnet->Pai = NULL;
         //cancela ordenação entre as MRede da MNet de geração
         mnet->OrdenaMRedes(NULL);
         }
      }
   //ordena demais MNets sem geração a partir das MNet já ordenadas
   for (int n = 0; (n < lisORD->Count)&&(lisMNET->Count > 0); n++)
	  {
	  mnet = (VTMNet*)lisORD->Items[n];
      //determina MNet vizinhas através dos Cortes comuns
      for (int i = lisMNET->Count-1; i >= 0; i--)
         {
         mnet_viz = (VTMNet*)lisMNET->Items[i];
         //verifica se há Corte comum entre as 2 MNet
         //if (Lista1IntersecaoLista2(mnet->LisCorte(), mnet_viz->LisCorte()) > 0)
         //determina Corte comum entre as 2 MNet
         if ((corte = mnet->ExisteCorte(mnet_viz)) == NULL) continue;
         //transfere MNet vizinha de lisMNET p/ lisORD
         lisMNET->Delete(i);
         lisORD->Add(mnet_viz);
         //define MNet pai
         mnet_viz->Pai = mnet;
         //ordena as MRede da MNet vizinha a partir do Corte comum c/ a MNet pai
         mnet_viz->OrdenaMRedes(corte);
         }
      }
   //transfere lisORD p/ lisMNET
   lisMNET->Clear();
   CopiaTList(lisORD, lisMNET);
   //insere geradores e suprimentos na lista de cada mnet
   for (int n = 0; n < lisMNET->Count > 0; n++)
	  {
	  mnet = (VTMNet*)lisMNET->Items[n];
	  //insere em VTMNet os Geradores da MNet
	  lisTMP->Clear();
	  mnet->LisEqbar(lisTMP, eqptoGERADOR);
	  CopiaTList(lisTMP, mnet->LisGerador());
	  //insere em VTMNet os Suprimentos da MNet
	  lisTMP->Clear();
	  mnet->LisEqbar(lisTMP, eqptoSUPRIMENTO);
	  CopiaTList(lisTMP, mnet->LisSuprimento());
	  }
   lisTMP->Clear();
   }

//---------------------------------------------------------------------------
void __fastcall TMontaMNet::MontaMNetOrdenada(TList *lisEXT_ORD, TList *lisEXT_ISO)
   {
   //variáveis locais
   VTCorte *corte;
   VTMRede *mrede;
   VTMNet  *mnet, *mnet_viz;
   TList   *lisCOR;

   //inicia lisMNET
   lisMNET->Clear();
   //reinicia lista lisMR_CAR c/ todas as MRedes de lisMR
   lisMR_CAR->Clear();
   CopiaTList(lisMR, lisMR_CAR);
   //cria uma MNet para cada conjunto de MRede que interliga MRedes de geração
   while ((mrede = ExisteMRedeDeGeracao(lisMR_CAR)) != NULL)
      {//verifica se a MRede forma uma malha de geração
      lisMR_GER->Clear();
      IdentificaMalhaDeGeracao(mrede, lisMR_CAR, lisMR_GER);
      //retira as MRedes em lisMR_GER de lisMR_CAR
      RetiraLis1DeLis2(lisMR_GER, lisMR_CAR);
      //cria uma MNet c/ as MRedes em lisMNET
      if ((mnet = DLL_NewObjMNet()) == NULL) continue;
      mnet->InsereMRede(lisMR_GER);
      lisMNET->Add(mnet);
      }
   //cria uma MNet p/ cada MRede sem geração que permaneceram em lisMR_CAR
   for (int n = 0; n < lisMR_CAR->Count; n++)
      {
      mrede = (VTMRede*)lisMR_CAR->Items[n];
      //cria um objeto VTNet e insere em lisMNET
      if ((mnet = DLL_NewObjMNet()) == NULL) continue;
      mnet->InsereMRede(mrede);
      lisMNET->Add(mnet);
      }
   //ordena MNets, colocando no início da lista as MNet com geração
   lisORD->Clear();
   for (int n = lisMNET->Count-1; n >= 0; n--)
      {
      mnet = (VTMNet*)lisMNET->Items[n];
      if (mnet->Geracao)
         {//transfere MNet de lisMNET p/ lisORD
         lisMNET->Delete(n);
         lisORD->Add(mnet);
         //inicia ponteiro p/ MNet pai
         mnet->Pai = NULL;
         //cancela ordenação entre as MRede da MNet de geração
         mnet->OrdenaMRedes(NULL);
         }
      }
   //ordena demais MNets sem geração a partir das MNet já ordenadas
   for (int n = 0; (n < lisORD->Count)&&(lisMNET->Count > 0); n++)
      {
      mnet = (VTMNet*)lisORD->Items[n];
      //determina MNet vizinhas através dos Cortes comuns
      for (int i = lisMNET->Count-1; i >= 0; i--)
         {
         mnet_viz = (VTMNet*)lisMNET->Items[i];
         //verifica se há Corte comum entre as 2 MNet
         //if (Lista1IntersecaoLista2(mnet->LisCorte(), mnet_viz->LisCorte()) > 0)
         //determina Corte comum entre as 2 MNet
         if ((corte = mnet->ExisteCorte(mnet_viz)) == NULL) continue;
         //transfere MNet vizinha de lisMNET p/ lisORD
         lisMNET->Delete(i);
         lisORD->Add(mnet_viz);
         //define MNet pai
         mnet_viz->Pai = mnet;
         //ordena as MRede da MNet vizinha a partir do Corte comum c/ a MNet pai
         mnet_viz->OrdenaMRedes(corte);
         }
      }
   //transfere MNest ordenadas de lisORD p/ lisEXT_ORD
   CopiaTList(lisORD, lisEXT_ORD);
   //transfere MNest isoladas de lisMNET p/ lisEXT_ISO
   CopiaTList(lisMNET, lisEXT_ISO);
      //insere geradores e suprimentos na lista de cada mnet
   for (int n = 0; n < lisMNET->Count > 0; n++)
	  {
	  mnet = (VTMNet*)lisMNET->Items[n];
	  //insere em VTMNet os Geradores da MNet
	  lisTMP->Clear();
	  mnet->LisEqbar(lisTMP, eqptoGERADOR);
	  CopiaTList(lisTMP, mnet->LisGerador());
	  //insere em VTMNet os Suprimentos da MNet
	  lisTMP->Clear();
	  mnet->LisEqbar(lisTMP, eqptoSUPRIMENTO);
	  CopiaTList(lisTMP, mnet->LisSuprimento());
	  }
   lisTMP->Clear();
   }

//---------------------------------------------------------------------------
void __fastcall TMontaMNet::MontaReducaoMNet(void)
   {
   //variáveis locais
   VTCorte   *corte;
   VTMNet    *mnet;
   VTReducao *reducao;
   TList     *lisCORTE;

   //cria Reducoes para cada MNet em lisMNET
   for (int n = 0; n < lisMNET->Count; n++)
      {
      mnet     = (VTMNet*)lisMNET->Items[n];
      lisCORTE = mnet->LisCorte();
      if (mnet->Geracao)
         {//MNet de geração: cria Reducoes c/ Suprimentos
         for (int nc = 0; nc < lisCORTE->Count; nc++)
            {
            corte = (VTCorte*)lisCORTE->Items[nc];
            //cria Reducao da MNet no Corte
            reducao = CriaReducaoComSuprimento(mnet, corte);
            if (reducao) mnet->InsereReducao(reducao, corte);
            }
         }
      else
         {//MNet de carga: cria uma Reducao como Carga e as demais como Suprimentos
         for (int nc = 0; nc < lisCORTE->Count; nc++)
            {
            corte = (VTCorte*)lisCORTE->Items[nc];
            //obtém lista de Reducoes das VTMNet vizinhas
            lisTMP->Clear();
            mnet->LisReducaoVizinha(corte, lisTMP);
            //verifica se há alguma VTMNet vizinha que possui Reducao c/ geração
            if (ExisteReducaoComGeracao(lisTMP))
               {//cria Reducoes como Carga
               reducao = CriaReducaoComCarga(mnet, corte);
               }
            else
               {//cria Reducao c/ Suprimento
               reducao = CriaReducaoComSuprimento(mnet, corte);
               }
            if (reducao) mnet->InsereReducao(reducao, corte);
            }
         }
      }
   }

//---------------------------------------------------------------------------
//eof


