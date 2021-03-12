//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TMontaCurto.h"
#include "..\Apl\VTApl.h"
#include "..\Consulta\VTConsulta.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTEqbar.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTNet.h"
#include "..\Rede\VTNo.h"
#include "..\Rede\VTRede.h"
#include "..\Tree\VTTree.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Rede.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTMontaCurto
//---------------------------------------------------------------------------
VTMontaCurto* __fastcall NewObjMontaCurto(VTApl *apl)
   {
   return(new TMontaCurto(apl));
   }

//---------------------------------------------------------------------------
__fastcall TMontaCurto::TMontaCurto(VTApl *apl)
   {
   //salva ponteiro p/ objeto
   this->apl = apl;
   //cria lista
   lisEQP = new TList();
   lisBAR = new TList();
   lisLIG = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TMontaCurto::~TMontaCurto(void)
   {
   //destrói lista sem destruir seus objetos
   if (lisEQP) {delete lisEQP; lisEQP = NULL;}
   if (lisBAR) {delete lisBAR; lisBAR = NULL;}
   if (lisLIG) {delete lisLIG; lisLIG = NULL;}
   }

//---------------------------------------------------------------------------
VTNet* __fastcall TMontaCurto::Executa(VTRedes *redes, VTBarra *barra)
   {
   //variáveis locais
   VTTree     *tree;
   VTNet      *net;

   //salva ponteiro p/ objetos
   this->redes = redes;
   this->barra = barra;
   //monta Tree c/ Barras e Ligacoes das Redes a partir dos Suprimentos e Geradores
   if ((tree = IniciaTree(redes)) == NULL) return(NULL);
   //mantém em Tree somente a parte da Rede que conecta as Barras de Gerador e de
   //Suprimento c/ a Barra indicada
   tree->Filtra(barra);
   //monta Net
   net = MontaNet(tree->LisBarra(), tree->LisLigacao());
   //destrói objeto Tree
   delete tree;
   return(net);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaCurto::IniciaLisBarraGeracao(VTRedes *redes, TList *lisBAR)
   {
   //variáveis locais
   VTEqbar    *eqbar;
   VTConsulta *consulta = (VTConsulta*)apl->GetObject(__classid(VTConsulta));

   //monta lista de Geradores e Suprimentos das Redes e das Reducoes
   lisEQP->Clear();
   CopiaTList(consulta->LisEqptoRedeReducao(redes, eqptoGERADOR),    lisEQP);
   CopiaTList(consulta->LisEqptoRedeReducao(redes, eqptoSUPRIMENTO), lisEQP);
   //monta lista de Barras dos Geradores e Suprimentos
   lisBAR->Clear();
   for (int n = 0; n < lisEQP->Count; n++)
      {
      eqbar = (VTEqbar*)lisEQP->Items[n];
      lisBAR->Add(eqbar->pbarra);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaCurto::IniciaLisLigacao(VTRedes *redes, TList *lisLIG)
   {
   //variáveis locais
   VTConsulta *consulta = (VTConsulta*)apl->GetObject(__classid(VTConsulta));

   //inicia lisLIG c/ todas as Ligacoes das Redes e das Reducoes
   lisLIG->Clear();
   CopiaTList(consulta->LisEqptoRedeReducao(redes, eqptoLIGACAO), lisLIG);
   }

//---------------------------------------------------------------------------
VTTree* __fastcall TMontaCurto::IniciaTree(VTRedes *redes)
   {
   //variáveis locais
   VTBarra   *barra1, *barra2;
   VTLigacao *ligacao;
   VTTree    *tree;

   //cria objeto Tree
   if ((tree = NewObjTree()) == NULL) return(NULL);
   //inicia lista lisBAR c/ as Barras dos Geradores e Suprimentos
   IniciaLisBarraGeracao(redes, lisBAR);
   //inicia lista lisLIG c/ todas as Ligacoes
   IniciaLisLigacao(redes, lisLIG);
   //monta Tree
   for (int nb = 0; nb < lisBAR->Count; nb++)
      {
      barra1 = (VTBarra*)lisBAR->Items[nb];
      //insere Barra em Tree
      tree->Add(barra);
      //determina Ligacoes que se conectam à Barra
      for (int nl = lisLIG->Count-1; nl >= 0; nl--)
         {
         ligacao = (VTLigacao*)lisLIG->Items[nl];
         if (! ligacao->ConectaBarra(barra1)) continue;
         //determina Barras da Ligacao diferentes da Barra
         for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
            {
            barra2 = ligacao->Barra(ind_bar);
            if ((barra2 == NULL)||(barra2 == barra1)) continue;
            //insere Barra2 em lisBAR
            if (lisBAR->IndexOf(barra2) < 0) lisBAR->Add(barra2);
            //insere Barra2 e Ligacao em Tree
            tree->Add(barra2, barra1, ligacao);
            }
         //retira Ligacao de lisLIG
         lisLIG->Delete(nl);
         }
      }
   return(tree);
   }

//---------------------------------------------------------------------------
VTNet* __fastcall TMontaCurto::MontaNet(TList *lisBAR, TList *lisLIG)
   {
   //variáveis locais
   VTBarra *barra;
   VTNet   *net;

   //cria objeto Net
   if ((net = DLL_NewObjNet()) == NULL) return(NULL);
   //insere em Net a lista de Barras
   net->InsereLisBarra(lisBAR);
   //monta lisEQP c/ Geradores e Suprimentos
   lisEQP->Clear();
   for (int nb = 0; nb < lisBAR->Count; nb++)
      {
      barra = (VTBarra*)lisBAR->Items[nb];
      barra->LisEqbar(lisEQP, eqptoGERADOR);
      barra->LisEqbar(lisEQP, eqptoSUPRIMENTO);
      }
   //insere em Net a lista Geradores e Suprimentos
   net->InsereLisEqbar(lisEQP);
   //insere em Net a lista Ligacoes
   net->InsereLisLigacao(lisLIG);
   return(net);
   }

//---------------------------------------------------------------------------
//eof

