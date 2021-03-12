//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TMalha.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCorte.h"
#include "..\Rede\VTMRede.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTReducao.h"
//#include "..\Rede\VTRedes.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Rede.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTMalha
//---------------------------------------------------------------------------
VTMalha* __fastcall NewObjMalha(void)
   {
   return(new TMalha());
   }

//---------------------------------------------------------------------------
__fastcall TMalha::TMalha(void)
   {
   //cria listas p/ rede em análise
   tmp.lisCORTE = new TList();
   tmp.lisORD   = new TList();
   tmp.lisVIZ   = new TList();
   tmp.lisMRM   = new TList();
   tmp.lisMR    = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TMalha::~TMalha(void)
   {
   if (tmp.lisCORTE) {delete tmp.lisCORTE; tmp.lisCORTE = NULL;}
   if (tmp.lisORD)   {delete tmp.lisORD;   tmp.lisORD   = NULL;}
   if (tmp.lisVIZ)   {delete tmp.lisVIZ;   tmp.lisVIZ   = NULL;}
   if (tmp.lisMRM)   {delete tmp.lisMRM;   tmp.lisMRM   = NULL;}
   if (tmp.lisMR)    {delete tmp.lisMR;    tmp.lisMR    = NULL;}
   }

//---------------------------------------------------------------------------
VTMRede* __fastcall TMalha::Executa(TList *lisMR)
   {
   //variáveis locais
   VTMRede *mrede;

   //loop de busca de MRedes em malha
   if (ExisteMalha(lisMR, tmp.lisMRM))
      {//funde as MRedes em malha, excluindo as MRedes fundidas de lisMR
      //seleciona uma MRede em malha que irá incorporar as demais
      mrede = (VTMRede*)tmp.lisMRM->Last();
      //retira MRede selecionada de tmp.lisMRM
      tmp.lisMRM->Delete(tmp.lisMRM->Count - 1);
      //retira as MRedes em tmp.lisMRM de lisMR
      RetiraLis1DeLis2(tmp.lisMRM, lisMR);
      //incorpora MRedes em tmp.lisMRM na MTRede selecionada
      mrede->IncorporaMRede(tmp.lisMRM);
      //destrói objetos MRede em tmp.lisMRM
      LimpaTList(tmp.lisMRM);
      return(mrede);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
bool __fastcall TMalha::ExisteMalha(TList *lisMR, TList *lisMALHA)
   {
   //variáveis locais
   VTCorte *corte;
   VTMRede *mrede, *mrede_viz;
   TList   *lisCORTE;

   //copia lisMR p/ tmp.lisMR
   tmp.lisMR->Clear();
   CopiaTList(lisMR, tmp.lisMR);
   while (tmp.lisMR->Count > 0)
      {//retira primeira MRede de tmp.lisMR p/ iniciar busca de malha
      mrede = (VTMRede*)tmp.lisMR->First();
      tmp.lisMR->Delete(0);
      //cancela MRede pai da MRede selecionada
      mrede->Pai = NULL;
      //inicia tmp.lisORD c/ a MRede selecionada
      tmp.lisORD->Clear();
      tmp.lisORD->Add(mrede);
      //limpa tmp.lisCORTE
      tmp.lisCORTE->Clear();
      //loop p/ todas as MRede ordenadas em lisORD
      for (int n = 0; n < tmp.lisORD->Count; n++)
         {
         mrede = (VTMRede*)tmp.lisORD->Items[n];
         //obtém lista de Cortes da MRede
         lisCORTE = mrede->LisCorte();
         //loop p/ todo Corte da MRede
         for (int nc = 0; nc < lisCORTE->Count; nc++)
            {
            corte = (VTCorte*)lisCORTE->Items[nc];
            //verifica se o Corte já existe em tmp.lisCORTE
            if (tmp.lisCORTE->IndexOf(corte) >= 0) continue;
            //insere Corte em tmp.lisCORTE
            tmp.lisCORTE->Add(corte);
            //obtém lista de MRedes vizinhas no Corte
            tmp.lisVIZ->Clear();
            mrede->LisMRedeVizinha(corte, tmp.lisVIZ);
            if (tmp.lisVIZ->Count == 0) continue;
            //verifica se alguma MRede vizinha fecha uma malha
            for (int i = 0; i < tmp.lisVIZ->Count; i++)
               {
               mrede_viz = (VTMRede*)tmp.lisVIZ->Items[i];
               //verifica se a MRede vizinha já existe na lista tmp.lisORD
               if (tmp.lisORD->IndexOf(mrede_viz) < 0)
                  {//define MRede como pai da MRede vizinha
                  mrede_viz->Pai = mrede;
                  //insere MRede vizinha em tmp.lisORD
                  tmp.lisORD->Add(mrede_viz);
                  //retira MRede vizinha de tmp.lisMR
                  tmp.lisMR->Remove(mrede_viz);
                  }
               else
                  {//MRede vizinha já existe em tmp.lisORD: malha
                  //inicia lisMALHA c/ as MRede que formam a malha
                  MontaListaMRedeEmMalha(mrede, mrede_viz, lisMALHA);
                  return(true);
                  }
               }
            }
         }
      }
   return(false);
   }

/*
//---------------------------------------------------------------------------
void __fastcall TMalha::MontaListaCorte(TList *lisMR, TList *lisCORTE)
   {
   //variáveis locais
   VTCorte *corte;
   VTMRede *mrede;

   //reinicia lisCORTE
   lisCORTE->Clear();
   //loop p/ todas MRede em lisMR
   for (int n = 0; n < lisMR->Count; n++)
      {
      mrede = (VTMRede*)lisMR->Items[n];
      //obtém lista de Cortes da MRede
      tmp.lisCORTE->Clear();
      mrede->LisCorte(tmp.lisCORTE);
      //insere os Cortes de tmp.lisCORTE em lisCORTE
      for (int nc = 0; nc < tmp.lisCORTE->Count; nc++)
         {
         corte = (VTCorte*)tmp.lisCORTE->Items[nc];
         //verifica se o Corte já existe em  lisCORTE
         if (lisCORTE->IndexOf(corte) >= 0) continue;
         //verifica se existem MRedes vizinhas carregadas no Corte
         tmp.lisVIZ->Clear();
         mrede->LisMRedeVizinhaCarregada(corte, tmp.lisVIZ);
         if (tmp.lisVIZ->Count > 0)
            {//insere Corte em lisCORTE
            lisCORTE->Add(corte);
            }
         }
      }
   //limpa listas usadas
   tmp.lisCORTE->Clear();
   tmp.lisVIZ->Clear();
   }
*/
//---------------------------------------------------------------------------
void __fastcall TMalha::MontaListaMRedeEmMalha(VTMRede *mrede1, VTMRede *mrede2, TList *lisMALHA)
   {
   //variáveis locais
   VTMRede *mrede, *mrede_pai;

   //inicia lisMALHA c/ as MRedes pai da rede1
   lisMALHA->Clear();
   mrede = mrede1;
   while (mrede != NULL)
      {
      lisMALHA->Add(mrede);
      mrede = mrede->Pai;
      }
   //determina primeira MRede pai comum entre as duas MRedes
   mrede = mrede2;
   while (mrede != NULL)
      {
      if (lisMALHA->IndexOf(mrede) < 0) mrede = mrede->Pai;
      else
         {//encontrou primeria MRede pai comum
         mrede_pai = mrede;
         break;
         }
      }
   //reinicia lisMALHA
   lisMALHA->Clear();
   //proteção: verifica se encontrou a Rede pai
   if (mrede_pai == NULL) return;
   //insere em lisMALHA as duas MRedes
   lisMALHA->Add(mrede1);
   lisMALHA->Add(mrede2);
   //insere em lisMALHA as demais MRedes que formam a malha entre as duas MRedes
   mrede = mrede1;
   while (mrede != mrede_pai)
      {//define que a MRede forma malha
      mrede->Malha = true;
      //insere MRede na malha
      if (lisMALHA->IndexOf(mrede) < 0) lisMALHA->Add(mrede);
      //salva em mrede1 a última Rede inserida na malha
      mrede1 = mrede;
      mrede  = mrede->Pai;
      }
   mrede = mrede2;
   while (mrede != mrede_pai)
      {//define que a MRede forma malha
      mrede->Malha = true;
      //insere Rede na malha
      if (lisMALHA->IndexOf(mrede) < 0) lisMALHA->Add(mrede);
      //salva em mrede2 a última MRede inserida na malha
      mrede2 = mrede;
      mrede  = mrede->Pai;
      }
   //verifica a MRede pai faz parte da malha
   if (mrede1->ExisteCorte(mrede_pai) != mrede2->ExisteCorte(mrede_pai))
      {//define que a MRede pai forma malha
      mrede_pai->Malha = true;
      //insere MRede pai
      if (lisMALHA->IndexOf(mrede_pai) < 0) lisMALHA->Add(mrede_pai);
      }
   }

//---------------------------------------------------------------------------
//eof


