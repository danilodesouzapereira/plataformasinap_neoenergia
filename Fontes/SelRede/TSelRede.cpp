//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TSelRede.h"
#include "..\Rede\VTCorte.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTReducao.h"
#include "..\Rede\VTRedes.h"
#include "..\DLL_Stub\Funcao\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTSelRede
//---------------------------------------------------------------------------
VTSelRede* __fastcall NewObjSelRede(void)
   {
   return(new TSelRede());
   }

//---------------------------------------------------------------------------
__fastcall TSelRede::TSelRede()
   {
   //cria listas
   lisRND   = new TList();
   lisREDE  = new TList();
   lisREDU  = new TList();
   lisORD   = new TList();
   lisVIZ   = new TList();
   lisPAI   = new TList();
   lisFILHA = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TSelRede::~TSelRede(void)
   {
   //destrói listas sem destruir seus objetos
   if (lisRND)   {delete lisRND;   lisRND   = NULL;}
   if (lisREDE)  {delete lisREDE;  lisREDE  = NULL;}
   if (lisREDU)  {delete lisREDU;  lisREDU  = NULL;}
   if (lisORD)   {delete lisORD;   lisORD   = NULL;}
   if (lisVIZ)   {delete lisVIZ;   lisVIZ   = NULL;}
   if (lisPAI )  {delete lisPAI;   lisPAI   = NULL;}
   if (lisFILHA) {delete lisFILHA; lisFILHA = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TSelRede::Executa(VTRedes *redes, TList *lisSEL, int nivel_dependencia)
   {
   //variáveis locais
   VTRede  *rede;

   //salva ponteiros
   this->redes  = redes;
   this->lisSEL = lisSEL;
   //marca as Redes selecionadas como carregadas
   for (int n = 0; n < lisSEL->Count; n++)
      {
      rede = (VTRede*)lisSEL->Items[n];
      //marca que VTRede deve ser carregada
      rede->Carregada(true);
      }
   //verifica se não deve tratar nenhum nível de dependência
   if (nivel_dependencia == 0) return;
   //monta lisREDE c/ o conjunto de Redes definido pelo nível de dependência
   IdentificaRedesDoNivelDependencia(nivel_dependencia);
   //loop p/ todas Redes selecionadas
   for (int n = 0; n < lisSEL->Count; n++)
      {
      rede = (VTRede*)lisSEL->Items[n];
      IdentificaRedesInterligadas(rede);
      }
   //inclui Redes vizinhas cujas Reducoes são inválidas
   InsereRedeVizinhaComReducaoInvalida();
   //elimina Redes e Reducoes desnecessárias
   ExcluiRedesReducoes();
   //elimina Cortes sem Rede
   ExcluiCortes();
   }

//---------------------------------------------------------------------------
void __fastcall TSelRede::ExcluiCortes(void)
   {
   //variáveis locais
   VTCorte *corte;
   TList   *lisCORTE = redes->LisCorte();

   //elimina todos os Cortes que não tenham nenhuma Rede carregada
   for (int nc = lisCORTE->Count-1; nc >= 0; nc--)
      {
      corte = (VTCorte*)lisCORTE->Items[nc];
      if (corte->NumeroRedesCarregadas() == 0)
         {//elimina o Corte da Rede
         redes->RetiraCorte(corte);
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TSelRede::ExcluiRedesReducoes(void)
   {
   //variáveis locais
   VTRede    *rede;
   VTReducao *reducao;
   TList     *lisEQP;

   //monta lisREDE c/ as Redes que devem ser carregadas
   lisREDE->Clear(); redes->LisRedeCarregada(lisREDE);
   //monta lisREDU c/ as Reducoes das Redes vizinhas das Redes a serem carregadas
   lisREDU->Clear();
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      rede->LisReducaoVizinha(lisREDU);
      }
   //monta lisVIZ c/ as Redes das Reducoes vizinhas
   lisVIZ->Clear();
   for (int n = 0; n < lisREDU->Count; n++)
      {
      reducao = (VTReducao*)lisREDU->Items[n];
      rede = reducao->Rede();
      if (lisVIZ->IndexOf(rede) < 0) lisVIZ->Add(rede);
      }
   //elimina todas Redes não carregadas que não estão em lisVIZ
   lisEQP = redes->LisRede();
   for (int n = lisEQP->Count-1; n >= 0; n--)
      {
      rede = (VTRede*)lisEQP->Items[n];
      if (rede->Carregada()) continue;
      if (lisVIZ->IndexOf(rede) >= 0) continue;
      //retira Rede
      redes->RetiraRede(rede);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TSelRede::IdentificaRedesDoNivelDependencia(int nivel_dependencia)
   {
   //variáveis locais
   VTRede *rede, *rede_viz;

   //inicia lisRND
   lisRND->Clear();
   //verifica se deve considerar todos níveis de dependência
   if (nivel_dependencia < 0)
      {//insere todoas as Redes em lisRND
      CopiaTList(redes->LisRede(), lisRND);
      return;
      }
   //insere em lisRND as Redes selecionadas pelo usuário
   CopiaTList(lisSEL, lisRND);
   //inicia lisPAI c/ as Redes selecionadas
   lisPAI->Clear();
   CopiaTList(lisSEL, lisPAI);
   //seleciona Redes vizinhas de cada nível de dependência
   while((lisPAI->Count > 0)&&(nivel_dependencia-- > 0))
      {
      for (int n = 0; n < lisPAI->Count; n++)
         {
         rede = (VTRede*)lisPAI->Items[n];
         //obtém lista de Redes vizinhas da Rede
         lisVIZ->Clear();
         rede->LisRedeVizinha(lisVIZ);
         //loop p/ todas Redes vizinhas
         for (int nrv = 0; nrv < lisVIZ->Count; nrv++)
            {
            rede_viz = (VTRede*)lisVIZ->Items[nrv];
            //inclui Rede em lisFILHA
            if (lisFILHA->IndexOf(rede_viz) < 0) lisFILHA->Add(rede_viz);
            //inclui Rede em lisRND
            if (lisRND->IndexOf(rede_viz) < 0) lisRND->Add(rede_viz);
            }
         }
      //transfere Redes de lisFILHA p/ lisPAI
      lisPAI->Clear();
      CopiaTList(lisFILHA, lisPAI);
      lisFILHA->Clear();
      }
   }

//---------------------------------------------------------------------------
void __fastcall TSelRede::IdentificaRedesInterligadas(VTRede *rede)
   {
   //variáveis locais
   int     index;
   VTRede  *rede_viz;

   //reinicia lista lisORD
   lisORD->Clear();
   //reinicia lista  lisREDE c/ todas Redes selecionadas em lisRND
   lisREDE->Clear();
   CopiaTList(lisRND, lisREDE);
   //cancela eventual ordenação feita anteriormente
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede_viz = (VTRede*)lisREDE->Items[n];
      rede_viz->pai = NULL;
      }
   //transfere Rede indicada de lisREDE p/ lisORD
   lisREDE->Remove(rede);
   lisORD->Add(rede);
   //loop p/ todas as Redes em lisORD
   for (int n = 0; n < lisORD->Count; n++)
      {
      rede = (VTRede*)lisORD->Items[n];
      //obtém lista de Redes vizinhas da Rede
      lisVIZ->Clear();
      rede->LisRedeVizinha(lisVIZ);
      //loop p/ todas Redes vizinhas
      for (int nrv = 0; nrv < lisVIZ->Count; nrv++)
         {
         rede_viz = (VTRede*)lisVIZ->Items[nrv];
         //verifica se a Rede vizinha está presente em lisREDE
         if ((index = lisREDE->IndexOf(rede_viz)) < 0) continue;
         //define pai da Rede vizinha
         rede_viz->pai = rede;
         //verifica se a Rede vizinha é uma Rede carregada
         if (rede_viz->Carregada())
            {//insere Redes na lista de Redes selecionadas
            InsereRedesOrdenadas(rede_viz);
            }
         //transfere Rede vizinha de lisREDE p/ lisORD
         lisREDE->Delete(index);
         if (lisORD->IndexOf(rede_viz) < 0) lisORD->Add(rede_viz);
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TSelRede::InsereRedesOrdenadas(VTRede *rede)
   {
   //variáveis locais
   while (rede != NULL)
      {//marca Rede como carregada
      rede->Carregada(true);
      //insere Rede em lisSEL
      if (lisSEL->IndexOf(rede) < 0) lisSEL->Add(rede);
      rede = rede->pai;
      }
   }


//---------------------------------------------------------------------------
void __fastcall TSelRede::InsereRedeVizinhaComReducaoInvalida(void)
   {
   //variáveis locais
   VTCorte   *corte;
   VTRede    *rede, *rede_viz;
   VTReducao *reducao, *reducao_viz;

   //loop p/ todas Redes selecionadas
   for (int n = 0; n < lisSEL->Count; n++)
      {
      rede = (VTRede*)lisSEL->Items[n];
      //loop p/ todas Reducoes
      lisREDU->Clear(); rede->LisReducao(lisREDU);
      for (int nr = 0; nr < lisREDU->Count; nr++)
         {
         reducao = (VTReducao*)lisREDU->Items[nr];
         corte   = reducao->Corte();
         //obtém lista de Redes vizinhas
         lisVIZ->Clear(); corte->LisRedeVizinha(rede, lisVIZ);
         //loop p/ todas Redes vizinhas
         for (int nrv = 0; nrv < lisVIZ->Count; nrv++)
            {
            rede_viz = (VTRede*)lisVIZ->Items[nrv];
            //verifica se a Rede vizinha já é uma das Redes selecionadas
            if (lisSEL->IndexOf(rede_viz) >= 0) continue;
            //determina Reducao da Rede viznha no Corte
            if ((reducao_viz = rede_viz->ExisteReducao(corte)) == NULL) continue;
            if (! reducao_viz->Valida())
               {//insere Rede em lisSEL
               rede_viz->Carregada(true);
               lisSEL->Add(rede_viz);
               }
            }
         }
      }
   }

//---------------------------------------------------------------------------
//eof

