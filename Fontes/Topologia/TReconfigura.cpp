//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TReconfigura.h"
#include "..\Apl\VTApl.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTCorte.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTReducao.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Rede.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTReconfigura* __fastcall NewObjReconfigura(VTApl *apl)
   {
   return(new TReconfigura(apl));
   }

//---------------------------------------------------------------------------
__fastcall TReconfigura::TReconfigura(VTApl *apl)
   {
   //salva ponteiro p/ objetos
   this->apl = apl;
   //cria objetos
   lisL   = new TList();
   lisBAR = new TList();
   lisLIG = new TList();
   lisCHV = new TList();
   lisISO = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TReconfigura::~TReconfigura(void)
   {
   //destrói lista sem destruir seus objetos
   if (lisL)   {delete lisL;   lisL   = NULL;}
   if (lisBAR) {delete lisBAR; lisBAR = NULL;}
   if (lisLIG) {delete lisLIG; lisLIG = NULL;}
   if (lisCHV) {delete lisCHV; lisCHV = NULL;}
   if (lisISO) {delete lisISO; lisISO = NULL;}
   }

//---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/18261839
bool __fastcall TReconfigura::Executa(VTRedes *redes, TList *lisREDE)
   {
   //variáveis locais
   VTRede  *rede;

	//salva ponteiro para objeto
	this->redes   = redes;
	this->lisREDE = lisREDE;
   //reinicia lista de Ligacoes isoladas
   lisISO->Clear();
   rede = redes->RedeIsolada();
   CopiaTList(rede->LisLigacao(), lisISO);
   // reinicia todas as lisLIG de todas as barras
   // (pois todas as redes serão ordenadas e irão inserir as ligações
   // em suas respectivas barras)
   ReiniciaLisLigBarras(redes->LisBarra());

   //reordena cada uma das Redes atendendo as seguintes condições:
   //1) inicia ordenação a partir da Barra inicial
   //2) trata somente suas próprias Ligacoes
   //3) Ligacoes interligadas permanecem na Rede
   //4) Ligacoes isoladas são transferidas p/ lisISO
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      //ordena a Rede
      Ordena(rede);
      }
   //faz uma segunda ordenação p/ transferir Ligacoes isoladas p/ as Redes
   for (int n = 0; (n < lisREDE->Count)&&(lisISO->Count > 0); n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      //ordena a Rede
      OrdenaLigacaoIsolada(rede);
      }
   //salva Ligacoes isoladas que permaneceram em lisISO na Rede isolada
   rede = redes->RedeIsolada();
   rede->IniciaLisLigacao(lisISO);
   //limpa todas listas
   LimpaListas();
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TReconfigura::LimpaListas(void)
   {
   lisL->Clear();
   lisBAR->Clear();
   lisLIG->Clear();
   lisCHV->Clear();
   lisISO->Clear();
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TReconfigura::Ordena(VTRede *rede)
   {
   //variáveis locais
   VTBarra   *pbarra, *pbar[NBAR_LIG];
   VTLigacao *pliga;
   VTRede    *rede_viz;

   //reinicia listas lisL, lisBAR, lisCHV, lisLIG
   lisL->Clear();
   lisCHV->Clear();
   lisBAR->Clear();
   lisLIG->Clear();
   //inicia lisL c/ todas Ligacoes da Rede
   CopiaTList(rede->LisLigacao(), lisL);
   //inicia lisBAR c/ Barra inicial da Rede
   pbarra = rede->BarraInicial();
   if (pbarra) lisBAR->Add(pbarra);
   //determina Ligacoes em lisL a partir das Barras em lisBAR
   for (int n = 0; n < lisBAR->Count; n++)
      {
      pbarra = (VTBarra*)lisBAR->Items[n];
      //loop p/ todas Ligacoes em lisL
      for (int nl = lisL->Count-1; nl >= 0; nl--)
         {
         pliga = (VTLigacao*)lisL->Items[nl];
         //verifica se Chave aberta
         if (pliga->Tipo() == eqptoCHAVE)
            {
            if (((VTChave*)pliga)->Aberta)
               {//transfere a Ligacao de lisL p/ lisCHV
               lisL->Remove(pliga);
               if (lisCHV->IndexOf(pliga) < 0) lisCHV->Add(pliga);
               continue;
               }
            }
         //verifica se a Ligacao conecta a Barra
         if ((pliga->pbarra1 == pbarra)||(pliga->pbarra2 == pbarra)||(pliga->pbarra3 == pbarra))
            {//determina Barras da Ligacao diferentes de pbarra
            pbar[0] = (pliga->pbarra1 == pbarra) ? NULL : pliga->pbarra1;
            pbar[1] = (pliga->pbarra2 == pbarra) ? NULL : pliga->pbarra2;
            pbar[2] = (pliga->pbarra3 == pbarra) ? NULL : pliga->pbarra3;
            //transfere a Ligacao de lisL p/ lisLIG
            lisL->Remove(pliga);
            if (lisLIG->IndexOf(pliga) < 0) lisLIG->Add(pliga);
            //insere Barras em lisBAR
            for (int nb = 0; nb < NBAR_LIG; nb++)
               {
               if (pbar[nb] != NULL)
                  {
                  if (lisBAR->IndexOf(pbar[nb]) < 0) lisBAR->Add(pbar[nb]);
                  }
               }
            }
         }
      }
   //reintroduz as Chaves abertas em lisLIG
   CopiaTList(lisCHV, lisLIG);
   //reinicia listas de Barras e Ligacoes da Rede
   rede->IniciaLisLigacao(lisLIG);
   //salva em lisISO as Ligacoes isoladas da Rede que permaneceram em lisL
   CopiaTList(lisL, lisISO);
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TReconfigura::OrdenaLigacaoIsolada(VTRede *rede)
   {
   //variáveis locais
   VTBarra   *pbarra, *pbar[NBAR_LIG];
   VTLigacao *pliga;

   //reinicia listas lisBAR c/ Barras da Rede
   lisBAR->Clear();
   CopiaTList(rede->LisBarra(), lisBAR);
   //reinicia listas lisLIG c/ Ligacoes da Rede
   lisLIG->Clear();
   CopiaTList(rede->LisLigacao(), lisLIG);
   //determina Ligacoes em lisISO que se conectam às Barras em lisBAR
   for (int n = 0; n < lisBAR->Count; n++)
      {
      pbarra = (VTBarra*)lisBAR->Items[n];
      //loop p/ todas Ligacoes em lisL
      for (int nl = lisISO->Count-1; nl >= 0; nl--)
         {
         pliga = (VTLigacao*)lisISO->Items[nl];
         //OBS: em lisISO não há chave aberta
         //verifica se a Ligacao conecta a Barra
         if ((pliga->pbarra1 == pbarra)||(pliga->pbarra2 == pbarra)||(pliga->pbarra3 == pbarra))
            {//determina Barras da Ligacao diferentes de pbarra
            pbar[0] = (pliga->pbarra1 == pbarra) ? NULL : pliga->pbarra1;
            pbar[1] = (pliga->pbarra2 == pbarra) ? NULL : pliga->pbarra2;
            pbar[2] = (pliga->pbarra3 == pbarra) ? NULL : pliga->pbarra3;
            //transfere a Ligacao de lisISO p/ lisLIG
            lisISO->Remove(pliga);
            if (lisLIG->IndexOf(pliga) < 0) lisLIG->Add(pliga);
            //insere Barras em lisBAR
            for (int nb = 0; nb < NBAR_LIG; nb++)
               {
               if (pbar[nb] != NULL)
                  {
                  if (lisBAR->IndexOf(pbar[nb]) < 0) lisBAR->Add(pbar[nb]);
                  }
               }
            }
         }
      }
   //reinicia listas de Barras e Ligacoes da Rede
   rede->IniciaLisLigacao(lisLIG);
   return(true);
   }
*/

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/14569628
bool __fastcall TReconfigura::Ordena(VTRede *rede)
   {
   //variáveis locais
   VTBarra   *pbarra, *pbar[NBAR_LIG];
   VTLigacao *pliga;
   VTRede    *rede_viz;

   //reinicia listas lisL, lisBAR, lisCHV, lisLIG
   lisL->Clear();
   lisCHV->Clear();
   lisBAR->Clear();
   lisLIG->Clear();
   //inicia lisL c/ todas Ligacoes da Rede
   CopiaTList(rede->LisLigacao(), lisL);
   //inicia lisBAR c/ Barra inicial da Rede
   pbarra = rede->BarraInicial();
   if (pbarra) lisBAR->Add(pbarra);
   //determina Ligacoes em lisL a partir das Barras em lisBAR
   for (int n = 0; n < lisBAR->Count; n++)
      {
      pbarra = (VTBarra*)lisBAR->Items[n];
      //loop p/ todas Ligacoes em lisL
      for (int nl = lisL->Count-1; nl >= 0; nl--)
         {
         pliga = (VTLigacao*)lisL->Items[nl];
         //verifica se a Ligacao conecta a Barra
         if ((pliga->pbarra1 == pbarra)||(pliga->pbarra2 == pbarra)||(pliga->pbarra3 == pbarra))
            {//verifica se Chave aberta
            if (pliga->Tipo() == eqptoCHAVE)
               {
               if (((VTChave*)pliga)->Aberta)
                  {//transfere a Ligacao de lisL p/ lisLIG mas não inclui as Barras em lisBAR
                  lisL->Remove(pliga);
                  if (lisLIG->IndexOf(pliga) < 0) lisLIG->Add(pliga);
                  continue;
                  }
               }
            //determina Barras da Ligacao diferentes de pbarra
            pbar[0] = (pliga->pbarra1 == pbarra) ? NULL : pliga->pbarra1;
            pbar[1] = (pliga->pbarra2 == pbarra) ? NULL : pliga->pbarra2;
            pbar[2] = (pliga->pbarra3 == pbarra) ? NULL : pliga->pbarra3;
            //transfere a Ligacao de lisL p/ lisLIG
            lisL->Remove(pliga);
            if (lisLIG->IndexOf(pliga) < 0) lisLIG->Add(pliga);
            //insere Barras em lisBAR
            for (int nb = 0; nb < NBAR_LIG; nb++)
               {
               if (pbar[nb] != NULL)
                  {
                  if (lisBAR->IndexOf(pbar[nb]) < 0)
                     {
                     lisBAR->Add(pbar[nb]);
                     }
                  else
                     {//existência de malha: verifica se rede radial
                     if (rede->Radial)
                        {//gera alerta para usuário
//                        Alerta(rede->Codigo + ": existência de malha em rede definida como radial");
						AlertaMalha(rede);
						}
                     }
                  }
               }
            }
         }
      }
   //reintroduz as Chaves abertas em lisLIG
   CopiaTList(lisCHV, lisLIG);
   //reinicia listas de Barras e Ligacoes da Rede
   rede->IniciaLisLigacao(lisLIG);
   //salva em lisISO as Ligacoes isoladas da Rede que permaneceram em lisL
   CopiaTList(lisL, lisISO);
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TReconfigura::OrdenaLigacaoIsolada(VTRede *rede)
   {
   //variáveis locais
   bool       barra_chvNA = false;
   VTBarra   *pbarra, *pbar[NBAR_LIG];
   VTLigacao *pliga;

   //reinicia listas lisBAR c/ Barras da Rede
   lisBAR->Clear();
   //CopiaTList(rede->LisBarra(), lisBAR);
   rede->LisBarra(lisBAR, barra_chvNA);
   //reinicia listas lisLIG c/ Ligacoes da Rede
   lisLIG->Clear();
   CopiaTList(rede->LisLigacao(), lisLIG);
   //determina Ligacoes em lisISO que se conectam às Barras em lisBAR
   for (int n = 0; n < lisBAR->Count; n++)
      {
      pbarra = (VTBarra*)lisBAR->Items[n];
      //loop p/ todas Ligacoes em lisL
      for (int nl = lisISO->Count-1; nl >= 0; nl--)
         {
         pliga = (VTLigacao*)lisISO->Items[nl];
         //verifica se a Ligacao conecta a Barra
         if ((pliga->pbarra1 == pbarra)||(pliga->pbarra2 == pbarra)||(pliga->pbarra3 == pbarra))
            {//verifica se Chave aberta
            if (pliga->Tipo() == eqptoCHAVE)
               {//verifica se é uma Chave aberta
               if (((VTChave*)pliga)->Aberta)
                  {//transfere a Ligacao de lisISO p/ lisLIG mas não inclui as Barras em lisBAR
                  lisISO->Remove(pliga);
                  if (lisLIG->IndexOf(pliga) < 0) lisLIG->Add(pliga);
                  continue;
                  }
               }
            //determina Barras da Ligacao diferentes de pbarra
            pbar[0] = (pliga->pbarra1 == pbarra) ? NULL : pliga->pbarra1;
            pbar[1] = (pliga->pbarra2 == pbarra) ? NULL : pliga->pbarra2;
            pbar[2] = (pliga->pbarra3 == pbarra) ? NULL : pliga->pbarra3;
            //transfere a Ligacao de lisISO p/ lisLIG
            lisISO->Remove(pliga);
            if (lisLIG->IndexOf(pliga) < 0) lisLIG->Add(pliga);
            //insere Barras em lisBAR
            for (int nb = 0; nb < NBAR_LIG; nb++)
               {
               if (pbar[nb] != NULL)
                  {
                  if (lisBAR->IndexOf(pbar[nb]) < 0) lisBAR->Add(pbar[nb]);
                  }
               }
            }
         }
      }
   //reinicia listas de Barras e Ligacoes da Rede
   rede->IniciaLisLigacao(lisLIG);
   return(true);
   }

//---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/18261839
void __fastcall TReconfigura::ReiniciaLisLigBarras(TList *lisBARRA)
{
    //variaveis locais
    VTBarra *barra;

    for(int n = 0; n < lisBARRA->Count; n++)
    {
        barra = (VTBarra*)lisBARRA->Items[n];
        barra->LisLigacao()->Clear();
    }
}

//---------------------------------------------------------------------------
//eof

