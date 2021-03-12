//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TEsqViz.h"
#include "VTSecao.h"
#include "..\Apl\VTApl.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCorte.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTTrecho.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTResFlowLig.h"
#include "..\..\DLL_Inc\Rede.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTEsqViz
//---------------------------------------------------------------------------
VTEsqViz* __fastcall NewObjEsqViz(VTApl *apl)
   {
   return(new TEsqViz(apl));
   }

//---------------------------------------------------------------------------
__fastcall TEsqViz::TEsqViz(VTApl *apl)
   {
   //salva ponteiro p/ objeto
   this->apl = apl;
   //cria objetos
   lisBAR = new TList();
   lisLIG = new TList();
   lisVIZ = new TList();
   lisTMP = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TEsqViz::~TEsqViz(void)
   {
   //destrói esquemáticos de redes carregadas
   ReiniciaEsquematicos();
   //destrói listas sem destruir seus objetos
   if (lisBAR) {delete lisBAR; lisBAR = NULL;}
   if (lisLIG) {delete lisLIG; lisLIG = NULL;}
   if (lisVIZ) {delete lisVIZ; lisVIZ = NULL;}
   if (lisTMP) {delete lisTMP; lisTMP = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TEsqViz::Executa(VTRede *rede)
   {
   //variáveis locais
   VTRede *rede_viz;

   //elimina esquemático de todas Redes carregadas
   ReiniciaEsquematicos();
   //inicia lisVIZ c/ as Redes vizinhas da Rede indicada
   lisVIZ->Clear();
   rede->LisRedeVizinha(lisVIZ);
   //loop p/ todas Redes vizinhas
   for (int nr = 0; nr < lisVIZ->Count; nr++)
      {
      rede_viz = (VTRede*)lisVIZ->Items[nr];
      //descarta Rede vizinha não carregada
      if (! rede_viz->Carregada) continue;
      //determina Barras comuns entre as duas Redes
      IniciaLisBarraComum(rede, rede_viz, lisBAR);
      //determina lista de Ligacoes da Rede vizinha que conectam as Barras comuns entres as duas Redes
      IniciaLisLigacao(rede_viz, lisBAR, lisLIG);
      //atualiza coordenadas do esquemático das Barras das Ligacoes da Rede vizinha
      MontaEsquematicoRede(rede_viz, lisBAR, lisLIG);
      }
   }

//---------------------------------------------------------------------------
VTBarra* __fastcall TEsqViz::ExisteBarraComum(VTLigacao *ligacao, TList *lisBAR)
   {
   //variáveis locais
   VTBarra *barra;

   //procura uma Barra da Ligacao que exista em lisBAR
   for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
      {
      barra = ligacao->Barra(ind_bar);
      if (barra == NULL) continue;
      if (lisBAR->IndexOf(barra) >= 0) return(barra);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
void __fastcall TEsqViz::IniciaLisBarraComum(VTRede *rede1, VTRede *rede2, TList *lisBAR)
   {
   //variáveis locais
   VTBarra *barra;
   VTCorte *corte;
   TList   *lisBAR_COR;
   TList   *lisCORTE1, *lisCORTE2;

   //limpa lisBAR
   lisBAR->Clear();
   //obtém lista de Cortes das duas Redes
   lisCORTE1 = rede1->LisCorte();
   lisCORTE2 = rede2->LisCorte();
   //loop p/ os Cortes em lisCORTE1
   for (int nc = 0; nc < lisCORTE1->Count; nc++)
      {
      corte = (VTCorte*)lisCORTE1->Items[nc];
      //verifica se o Corte existe em lisCORTE2
      if (lisCORTE2->IndexOf(corte) >= 0)
         {// Corte comum entre Redes: verifica se o Corte possui Barras comuns às duas redes
         lisBAR_COR = corte->LisBarra();
         for (int nb = 0; nb < lisBAR_COR->Count; nb++)
            {
            barra = (VTBarra*)lisBAR_COR->Items[nb];
            if (! rede1->ExisteBarra(barra)) continue;
            if (! rede2->ExisteBarra(barra)) continue;
            //insere Barra comum às duas Redes em lisBAR
            lisBAR->Add(barra);
            }
         }
      //verifica se encontrou Barras comuns às duas Redes
      if (lisBAR->Count > 0) return;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TEsqViz::IniciaLisLigacao(VTRede *rede, TList *lisBAR, TList *lisLIG)
   {
   //variáveis locais
   VTBarra   *barra;
   VTLigacao *ligacao;
   TList     *lisEXT;

   //limpa lisLIG
   lisLIG->Clear();
   //determina conjunto de Ligacoes da Rede que conectam as Barras em lisBAR
   for (int nb = 0; nb < lisBAR->Count; nb++)
      {
      barra = (VTBarra*)lisBAR->Items[nb];
      //loop p/ todas Ligacoes da Rede
      lisEXT = rede->LisLigacao();
      for (int nl = 0; nl < lisEXT->Count; nl++)
         {
         ligacao = (VTLigacao*)lisEXT->Items[nl];
         if (ligacao->ConectaBarra(barra)) lisLIG->Add(ligacao);
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TEsqViz::MontaEsquematicoRede(VTRede *rede, TList *lisBAR, TList *lisLIG)
   {
   //variáveis locais
   int         dx, dy;
   VTBarra     *bar1, *bar2, *bar_fic;
   VTLigacao   *ligacao, *liga_fic;
   VTPatamares *patamares =  (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   //proteção
   if (lisLIG->Count == 0) return;
   //limpa lisTMP
   lisTMP->Clear();
   //cria Barra fictícia p/ esquemático da Rede
   if ((bar_fic = DLL_NewObjBarra(patamares->NumPatamar())) == NULL) return;
   //loop p/ todas Ligacoes
   for (int nl = 0; nl < lisLIG->Count; nl++)
      {
      ligacao = (VTLigacao*)lisLIG->Items[nl];
      //determina Barra da Ligacao que conecta uma das Barras em lisBAR
      if ((bar1 = ExisteBarraComum(ligacao, lisBAR)) == NULL) continue;
      //determina segunda Barra da Ligacao
      bar2 = (ligacao->pbarra1 == bar1) ? ligacao->pbarra2 : ligacao->pbarra1;
      //determina deslocamento sofrido pela Barra bar1
      dx = bar1->esq.x1 - bar1->esq_orig.x1;
      dy = bar1->esq.y1 - bar1->esq_orig.y1;
      //cria Trecho fictício e insere em lisTMP
      liga_fic = DLL_NewObjTrecho(patamares->NumPatamar());
      lisTMP->Add(liga_fic);
      //inicia dados do Trecho fictício
      liga_fic->Codigo  = ligacao->Codigo;
      liga_fic->DefineObjBarra(bar1, bar_fic);
      liga_fic->ShowAsUnsigned    = 0;
      liga_fic->Show[eqp_VISIBLE] = true;
      liga_fic->Show[eqp_FIXO]    = false;
      liga_fic->resflow->CopiaAtributosDe(*ligacao->resflow);
      //define coordenadas da Barra fictícia
      bar_fic->esq.x1 = bar_fic->esq.x2 = bar2->esq_orig.x1 + dx;
      bar_fic->esq.y1 = bar_fic->esq.y2 = bar2->esq_orig.y1 + dy;
      //salva coordenadas esquemático da Barra fictícia
      bar_fic->esq_orig = bar_fic->esq;
      }
   //atualiza esquemático da Rede
   rede->ReiniciaEsquematico(bar_fic, lisTMP);
   }

//---------------------------------------------------------------------------
void __fastcall TEsqViz::ReiniciaEsquematicos(void)
   {
   //variáveis locais
   TList   *lisREDE;
   VTRede  *rede;
   VTRedes *redes =  (VTRedes*)apl->GetObject(__classid(VTRedes));

   //proteção
   if (redes == NULL) return;
   //reinicia esquemático de todas Redes
   lisREDE = redes->LisRede();
   for (int nr = 0; nr < lisREDE->Count; nr++)
      {
      rede = (VTRede*)lisREDE->Items[nr];
      if (rede->Carregada) rede->ReiniciaEsquematico();
      }
   }

//---------------------------------------------------------------------------
//eof


