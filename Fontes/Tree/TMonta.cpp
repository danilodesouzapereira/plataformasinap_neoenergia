//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TMonta.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCorte.h"
#include "..\Rede\VTNet.h"
#include "..\Rede\VTNo.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTReducao.h"
#include "..\DLL_Stub\Rede\Rede.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTMonta
//---------------------------------------------------------------------------
VTMonta* __fastcall NewObjMonta(VTApl *apl)
   {
   return(new TMonta(apl));
   }

//---------------------------------------------------------------------------
__fastcall TMonta::TMonta(VTApl *apl)
   {
   //salva ponteiro p/ objeto
   this->apl = apl;
   //cria lista
   lisCORTE = new TList();
   lisREDU  = new TList();
   lisRR    = new TList();
   lisTMP   = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TMonta::~TMonta(void)
   {
   //destrói lista sem destruir seus objetos
   if (lisCORTE) {delete lisCORTE; lisCORTE = NULL;}
   if (lisREDU)  {delete lisREDU;  lisREDU  = NULL;}
   if (lisRR)    {delete lisRR;    lisRR    = NULL;}
   if (lisTMP)   {delete lisTMP;   lisTMP   = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TMonta::FiltraReducaoComSuprimento(TList *lisREDU)
   {
   //variáveis locais
   VTRede    *rede;
   VTReducao *reducao, *reducao_sup = NULL;

   //verifica se existe alguma Reducao de Rede carregada c/ Suprimento
   for (int nr = lisREDU->Count-1; nr >= 0; nr--)
      {
      reducao = (VTReducao*)lisREDU->Items[nr];
      rede    = reducao->Rede();
      if (! reducao->ExisteGeracao()) continue;
      //verifica se já foi encontrada uma Reducao de Rede carregada c/ Suprimento
      if (reducao_sup != NULL)
         {//retira Reducao c/ Suprimento da lista
         lisREDU->Delete(nr);
         }
      else
         {//verifica se Reducao de uma Rede carregada
         if (rede->Carregada()&&(reducao_sup == NULL))
            {//encontrou primeira Reducao de Rede carregada c/ Suprimento: verifica se é a primeira
            reducao_sup = reducao;
            }
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMonta::InsereEqptoRede(VTNet *net, VTRede *rede)
   {
   //insere em VTnet as Barras da Rede
   net->InsereLisBarra(rede->LisBarra());
   //insere em VTnet os Eqbar da Rede
   lisTMP->Clear();
   rede->LisEqbar(lisTMP);
   net->InsereLisEqbar(lisTMP);
   //insere em VTnet as Ligacoes da Rede
   net->InsereLisLigacao(rede->LisLigacao());
   //limpa lista temporaria
   lisTMP->Clear();
   }

//---------------------------------------------------------------------------
void __fastcall TMonta::InsereEqptoReducao(VTNet *net, VTReducao *reducao)
   {
   net->InsereLisBarra(reducao->LisBarra());
   net->InsereLisEqbar(reducao->LisEqbar());
   net->InsereLisLigacao(reducao->LisLigacao());
   }

//---------------------------------------------------------------------------
void __fastcall TMonta::MontaLisRedeParaReducao(VTRede *rede, VTCorte *corte)
   {
   //variáveis locais
   VTCorte   *corte_aux;
   VTRede    *rede_aux;
   VTReducao *reducao;
   TList     *lisREDE;

   //reinicia lisRR c/ a Rede
   lisRR->Clear();
   lisRR->Add(rede);
   //loop p/ todas Redes em lisRR
   for (int n = 0; n < lisRR->Count; n++)
      {
      rede = (VTRede*)lisRR->Items[n];
      //loop p/ todos Cortes da Rede
      lisCORTE->Clear();
      rede->LisCorte(lisCORTE);
      for (int nc = 0; nc < lisCORTE->Count; nc++)
         {
         corte_aux = (VTCorte*)lisCORTE->Items[nc];
         //desconsidera Redes conectadas no Corte da Reducao
         if (corte_aux == corte) continue;
         //obtém lista de Redes do Corte aux
         lisREDE = corte_aux->LisRede();
         //loop p/ todas Redes do Corte
         for (int i = 0; i < lisREDE->Count; i++)
            {
            rede_aux = (VTRede*)lisREDE->Items[i];
            if (rede_aux == rede) continue;
            //verifica se a Rede aux já existe em lisRR
            if (lisRR->IndexOf(rede_aux) >= 0) continue;
            //determina Reducao da Rede aux no Corte
            if ((reducao = rede_aux->ExisteReducao(corte_aux)) == NULL) continue;
            //verifica se a Reducao da Rede aux já foi montada
            if ((reducao->LisEqbar())->Count == 0)
               {//Reducao vazia: insere Rede na lista lisRR
               lisRR->Add(rede_aux);
               }
            }
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMonta::MontaLisReducaoParaReducao(TList *lisREDE, VTCorte *corte)
   {
   //variáveis locais
   VTRede    *rede, *rede_viz;
   VTReducao *reducao_viz;

   //monta lisREDU c/ todas Reducoes das Redes vizinhas às Redes em lisREDE,
   //exceto as reducoes das Redes conectadas ao Corte
   lisREDU->Clear();
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      //obtém lista de Reducoes das Redes vizinhas
      lisTMP->Clear();
      rede->LisReducaoVizinha(lisTMP);
      for (int nr = 0; nr < lisTMP->Count; nr++)
         {
         reducao_viz = (VTReducao*)lisTMP->Items[nr];
         rede_viz    = reducao_viz->Rede();
         //verifica se a Rede vizinha está em lisREDE
         if (lisREDE->IndexOf(rede_viz) >= 0) continue;
         //verifica se a Rede vizinha conecta-se ao Corte
         if (corte->ConectaRede(rede_viz)) continue;
         //verifica se a Reducao da Rede vizinha está em lisREDU
         if (lisREDU->IndexOf(reducao_viz) < 0)
            {//insere Reducao da Rede vizinha em lisREDU
            lisREDU->Add(reducao_viz);
            }
         }
      }
   }

//---------------------------------------------------------------------------
VTNet* __fastcall TMonta::Net(TList *lisREDE)
   {
   //variáveis locais
   VTNet     *net;
   VTRede    *rede, *rede_viz;
   VTReducao *reducao_viz;

   //cria objeto VTNet
   if ((net = DLL_NewObjNet()) == NULL) return(NULL);
   //monta lisREDU c/ todas Reducoes das Redes vizinhas
   lisREDU->Clear();
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      //obtém lista de Reducoes das Redes vizinhas
      lisTMP->Clear();
      rede->LisReducaoVizinha(lisTMP);
      for (int nr = 0; nr < lisTMP->Count; nr++)
         {
         reducao_viz = (VTReducao*)lisTMP->Items[nr];
         rede_viz    = reducao_viz->Rede();
         //verifica se a Rede vizinha está em lisREDE
         if (lisREDE->IndexOf(rede_viz) >= 0) continue;
         //verifica se a Reducao da Rede vizinha está em lisREDU
         if (lisREDU->IndexOf(reducao_viz) < 0)
            {//insere Reducao da Rede vizinha em lisREDU
            lisREDU->Add(reducao_viz);
            }
         }
      }
   //verifica se todas as Redes são simples
   if (TodasRedesSimples(lisREDE))
      {//filtra as Reducoes c/ Suprimento: se houver uma Reducao c/ Suprimento
      //relativa a uma Rede carregada DESCARTA todas as demais Reducoes c/ Suprimento
      FiltraReducaoComSuprimento(lisREDU);
      }
   //monta Net c/ Eqptos de todas Redes em lisREDE
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      //insere Eqptos da Rede em Net
      InsereEqptoRede(net, rede);
      }
   //inclui em Net os Eqptos das Reducoes das Redes vizinhas
   for (int nr = 0; nr < lisREDU->Count; nr++)
      {
      reducao_viz = (VTReducao*)lisREDU->Items[nr];
      //insere Eqptos da Redução da Rede vizinha
      InsereEqptoReducao(net, reducao_viz);
      }
   //limpa listas temporárias
   lisTMP->Clear();
   lisREDU->Clear();
   return(net);
   }

//---------------------------------------------------------------------------
VTNet* __fastcall TMonta::Net(VTRede *rede)
   {
   //variáveis locais
   VTNet     *net;
   VTRede    *rede_viz;
   VTReducao *reducao_viz;

   //cria objeto VTNet
   if ((net = DLL_NewObjNet()) == NULL) return(NULL);
   //insere Eqptos da Rede em Net
   InsereEqptoRede(net, rede);
   //obtém Reducoes das Redes vizinhas
   lisREDU->Clear();
   rede->LisReducaoVizinha(lisREDU);
   //PROTEÇÃO:
   if (rede->Simples())
      {//filtra as Reducoes c/ Suprimento: se houver uma Reducao c/ Suprimento
      //relativa a uma Rede carregada DESCARTA todas as demais Reducoes c/ Suprimento
      FiltraReducaoComSuprimento(lisREDU);
      }
   //insere Eqptos das Reducoes das Redes vizinhas
   for (int nr = 0; nr < lisREDU->Count; nr++)
      {
      reducao_viz = (VTReducao*)lisREDU->Items[nr];
      if (reducao_viz->Nula()) continue;
      InsereEqptoReducao(net, reducao_viz);
      }
   lisREDU->Clear();
   return(net);
   }

//---------------------------------------------------------------------------
VTNet* __fastcall TMonta::Net(TList *lisREDE, TList *lisREDU)
   {
   //variáveis locais
   VTNet     *net;
   VTRede    *rede;
   VTReducao *reducao;

   //cria objeto VTNet
   if ((net = DLL_NewObjNet()) == NULL) return(NULL);
   //monta Net c/ Eqptos de todas Redes em lisREDE
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      //insere Eqptos da Rede em Net
      InsereEqptoRede(net, rede);
      }
   //inclui em Net os Eqptos das Reducoes em lisREDU
   for (int nr = 0; nr < lisREDU->Count; nr++)
      {
      reducao = (VTReducao*)lisREDU->Items[nr];
      //insere Eqptos da Redução da Rede vizinha
      InsereEqptoReducao(net, reducao);
      }
   return(net);
   }

//---------------------------------------------------------------------------
VTNet* __fastcall TMonta::NetParaReducao(VTRede *rede, VTCorte *corte)
   {
   //variáveis locais
   VTNet *net;

   //monta lista lisRR c/ as Redes que irão compor a Net
   MontaLisRedeParaReducao(rede, corte);
   //monta lista lisREDU c/ as Reducoes que irão compor a Net
   MontaLisReducaoParaReducao(lisRR, corte);
   //cria VTNet a partir da lista lisRR
   net = Net(lisRR, lisREDU);
   //limpa listas
   lisRR->Clear();
   lisREDU->Clear();
   return(net);
   }

//---------------------------------------------------------------------------
bool __fastcall TMonta::TodasRedesSimples(TList *lisREDE)
   {
   //variáveis locais
   VTRede    *rede;

   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      if (! rede->Simples()) return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
//eof

