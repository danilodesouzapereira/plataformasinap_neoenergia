//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TBlocos.h"
#include "VTBloco.h"
#include "VTElo.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe
//---------------------------------------------------------------------------
VTBlocos* __fastcall NewObjBlocos(void)
   {
   return(new TBlocos());
   }

//---------------------------------------------------------------------------
__fastcall TBlocos::TBlocos(void)
   {
   //cria listas
   lisBAR = new TList();
   lisBLO = new TList();
   lisCHV = new TList();
   lisELO = new TList();
   lisLIG = new TList();
   lisTMP = new TList();
   lisVIZ = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TBlocos::~TBlocos(void)
   {
   //destrói listas sem destruir seus objetos
   if (lisBAR) {delete lisBAR; lisBAR = NULL;}
   if (lisCHV) {delete lisCHV; lisCHV = NULL;}
   if (lisLIG) {delete lisLIG; lisLIG = NULL;}
   if (lisTMP) {delete lisTMP; lisTMP = NULL;}
   if (lisVIZ) {delete lisVIZ; lisVIZ = NULL;}
   //destrói listas e seus objetos
   if (lisBLO) {LimpaTList(lisBLO); delete lisBLO; lisBLO = NULL;}
   if (lisELO) {LimpaTList(lisELO); delete lisELO; lisELO = NULL;}
   }

//------------------------------------------------------------------------------
bool __fastcall TBlocos::EliminaBloco(VTBloco *bloco)
   {
   //variáveis locais
   TList *lisTMP_ELO;
   VTElo *elo;

   //proteção
   if (bloco == NULL) return(false);
   //cria lista para armazenar Elos do Bloco
   lisTMP_ELO = new TList();
   //obtém lista de Elos do Bloco
   LisElo(bloco, lisTMP_ELO);
   //remove de lisELO todos os Elos que estão em lisTMP_ELO
   for (int n = 0; n < lisTMP_ELO->Count; n++)
      {
      elo = (VTElo*)lisTMP_ELO->Items[n];
      //remove Elo de lisELO
      lisELO->Remove(elo);
      }
   //destrói todos os Elos em lisTMP_ELO
   LimpaTList(lisTMP_ELO);
   //destrói lista lisTMP_ELO
   delete lisTMP_ELO;
   //remove o Bloco de lisBLO e destrói o objeto
   lisBLO->Remove(bloco);
   delete bloco;

   return(true);
   }

//------------------------------------------------------------------------------
bool __fastcall TBlocos::EliminaBlocos(TList *lisEXT)
   {
   //variáveis locais
   VTBloco *bloco;

   //loop para todos os Blocos em lisEXT
   for (int n = 0; n < lisEXT->Count; n++)
      {
      bloco = (VTBloco*)lisEXT->Items[n];
      //elimina o Bloco e seus Elos
      EliminaBloco(bloco);
      }
   //limpa a lista lisEXT sem destruir seus objetos pois os Blocos já foram destruídos
   lisEXT->Clear();

   return(true);
   }

//------------------------------------------------------------------------------
bool __fastcall TBlocos::EliminaBlocosNaoEnergizados(void)
   {
   //variáveis locais
   TList *lisBNE;

   //cria lista de Blocos não energizados
   lisBNE = new TList();
   //obtém lista de Blocos não energizados
   LisBlocoNaoEnergizado(lisBNE);
   //elimina os Blocos em lisBNE
   if (lisBNE->Count > 0)
      {
      EliminaBlocos(lisBNE);
      }
   //destrói lista lisBNE sem destruir seus objetos (que já foram destruídos)
   delete lisBNE;

   return(true);
   }
//------------------------------------------------------------------------------
bool __fastcall TBlocos::EliminaElosComBlocosIguais(void)
   {
   //variáveis locais
   VTElo *elo;

   //remove de lisELO todo Elo que conecta o mesmo bloco
   for (int n = lisELO->Count - 1; n >= 0; n--)
      {
      elo = (VTElo*)lisELO->Items[n];
      if (elo->Bloco1 == elo->Bloco2)
         {//insere Chave do Elo no Bloco
         elo->Bloco1->InsereLigacao(elo->Chave);
         //remove Elo da Lista
         lisELO->Delete(n);
         //destrói objeto
         delete elo;
         }
      }
   return(true);
   }

//------------------------------------------------------------------------------
bool __fastcall TBlocos::EliminaElosComChaveAberta(void)
   {
   //variáveis locais
   VTElo *elo;

   //remove de lisELO todo Elo com Chave aberta
   for (int n = lisELO->Count - 1; n >= 0; n--)
      {
      elo = (VTElo*)lisELO->Items[n];
      if (elo->Chave->Aberta)
         {//remove Elo da Lista
         lisELO->Delete(n);
         //destrói objeto
         delete elo;
         }
      }
   return(true);
   }

//------------------------------------------------------------------------------
bool __fastcall TBlocos::Executa(VTEqpto *eqpto)
   {
   return(true);
   }

//------------------------------------------------------------------------------
bool __fastcall TBlocos::Executa(VTRede *rede)
   {
   return(true);
   }

//------------------------------------------------------------------------------
bool __fastcall TBlocos::Executa(VTRedes *redes)
   {
   //destrói conjuntos de objetos Elo e Bloco criados anteriormente
   LimpaTList(lisELO);
   LimpaTList(lisBLO);
   //reinicia conjunto de objetos Blocos a partir das Barras e Ligacoes
   lisBAR->Clear(); redes->LisBarra(lisBAR);
   lisLIG->Clear(); redes->LisLigacao(lisLIG);
   MontaBloco(lisBAR, lisLIG);
   //monta conjunto de objetos Elo a partir dos Blocos e das Chaves
   lisCHV->Clear();
   redes->LisEqpto(lisCHV, eqptoCHAVE);
   MontaElo(lisBLO, lisCHV);
   return(true);
   }

//------------------------------------------------------------------------------
VTBloco* __fastcall TBlocos::ExisteBloco(VTBarra *barra)
   {
   //variáveis locais
   VTBloco *bloco;

   //procura Bloco que contém a Barra
   for (int n = 0; n < lisBLO->Count; n++)
      {
      bloco = (VTBloco*)lisBLO->Items[n];
      if (bloco->ExisteBarra(barra)) return(bloco);
      }
   return(NULL);
   }

//------------------------------------------------------------------------------
VTBloco* __fastcall TBlocos::ExisteBloco(VTLigacao *ligacao)
   {
   //variáveis locais
   VTBloco *bloco;

   //verifica se a Ligacao é uma Chave
   if (ligacao->Tipo() == eqptoCHAVE) return(NULL);
   //procura Bloco que contém a Ligacao
   for (int n = 0; n < lisBLO->Count; n++)
      {
      bloco = (VTBloco*)lisBLO->Items[n];
      if (bloco->ExisteLigacao(ligacao)) return(bloco);
      }
   return(NULL);
   }

//------------------------------------------------------------------------------
VTElo* __fastcall TBlocos::ExisteElo(VTBloco* bloco1, VTBloco* bloco2)
   {
   //variáveis locais
   VTElo *elo;

   //procura Elo que contém a Chave
   for (int n = 0; n < lisELO->Count; n++)
      {
      elo = (VTElo*)lisELO->Items[n];
      if (elo->ConectaBloco(bloco1) && elo->ConectaBloco(bloco2)) return(elo);
      }
   return(NULL);
   }

//------------------------------------------------------------------------------
VTElo* __fastcall TBlocos::ExisteElo(VTChave *chave)
   {
   //variáveis locais
   VTElo *elo;

   //procura Elo que contém a Chave
   for (int n = 0; n < lisELO->Count; n++)
      {
      elo = (VTElo*)lisELO->Items[n];
      if (elo->Chave == chave) return(elo);
      }
   return(NULL);
   }

//------------------------------------------------------------------------------
bool __fastcall TBlocos::FundeBloco2noBloco1(VTBloco* bloco1, VTBloco* bloco2)
   {
   //variáveis locais
   TList     *lisEXT;
   VTElo     *elo_viz;
   VTBarra   *barra;
   VTLigacao *ligacao;

   //proteção: verifica os blocos são iguais
   if (bloco1 == bloco2) return(true);
   //transfere Barras do Bloco2 para o Bloco1
   lisEXT = bloco2->LisBarra();
   for (int n = 0; n < lisEXT->Count; n++)
      {
      barra = (VTBarra*)lisEXT->Items[n];
      bloco1->InsereBarra(barra);
      }
   //transfere Ligacoes (e redes) do Bloco2 para o Bloco1
   lisEXT = bloco2->LisLigacao();
   for (int n = 0; n < lisEXT->Count; n++)
      {
      ligacao = (VTLigacao*)lisEXT->Items[n];
      bloco1->InsereLigacao(ligacao);
      }
   //obtém lista de Elos do Bloco2
   lisTMP->Clear();
   LisElo(bloco2, lisTMP);
   //incorpora as chaves dos elos do bloco2 que passaram a ser elos do bloco1
   for(int nl = lisTMP->Count -1 ; nl >= 0; nl--)
      {
      elo_viz = (VTElo*)lisTMP->Items[nl];
      //Elos internos
      if (elo_viz->ConectaBloco(bloco1))
         {
         lisELO->Remove(elo_viz);
         bloco1->InsereLigacao(elo_viz->Chave);
         delete elo_viz;
         }
      //transfere blocos vizinhos ao bloco 2 para bloco 1 //DVK 2016.03.16
      if(elo_viz->Bloco1 == bloco2) elo_viz->Bloco1 = bloco1;
      if(elo_viz->Bloco2 == bloco2) elo_viz->Bloco2 = bloco1;
      }
   //destrói o Bloco2
   lisBLO->Remove(bloco2);
   delete bloco2;
   return(true);
   }
/*
//------------------------------------------------------------------------------
bool __fastcall TBlocos::FundeBlocos(VTBloco* bloco, TList* lisEXT)
   {
   //Nova funcao HK
   VTBloco* bloco_aux;
   VTElo*   elo;
   bool    flag_funde = false;
   TList*  lisAux;
   TList*  lisLiga;
   TList*  lisChave;
   TList*  lisBarra;
   TList*  lisBloco;
   bool    flag_viz;
   AnsiString aux;


   ijk++;

   if(lisBLO->IndexOf(bloco) == -1) return(false);
   try
   {
   lisAux = new TList();
   //lisAux->Clear();
   //Funde apenas blocos vizinhos
   //LisBlocoVizinho1(bloco, lisAux);
   //lisEXT->Assign(lisAux, laAnd);

   for(int nb = lisEXT->Count - 1; nb >= 0; nb--)
      {
      bloco_aux = (VTBloco*)lisEXT->Items[nb];
      if(bloco_aux == bloco)
        int kkk = 0;

      if(bloco_aux == NULL)
        continue;
      (bloco->LisBarra())->Assign(bloco_aux->LisBarra(), laOr);
      lisLiga  = bloco_aux->LisLigacao();
      for(int nl = 0; nl < lisLiga->Count; nl++)
         bloco->InsereLigacao((VTLigacao*)lisLiga->Items[nl]);

      //hkk lisChave = bloco_aux->LisChave();
      //hkk for(int nl = 0; nl < lisChave->Count; nl++)
      //hkk    bloco->InsereLigacao((VTLigacao*)lisChave->Items[nl]);



      lisAux->Clear();
      LisElo(bloco_aux,lisAux);
      //LisEloFronteira(bloco_aux,lisAux);

      //Remove elos internos
      flag_viz = false;
      for(int nl = lisAux->Count -1 ; nl >= 0; nl--)
         {
         elo = (VTElo*)lisAux->Items[nl];
         //Elos internos
         if (elo->ConectaBloco(bloco))
            {
            lisELO->Remove(elo);
            lisAux->Remove(elo);
            bloco->InsereLigacao(elo->Chave);
            flag_viz = true;//É vizinho de bloco
            delete elo;
            continue;
            }
         //Elos de bloco_aux e não de bloco
         if(elo->Bloco1 == bloco_aux) elo->Bloco1 = bloco;
         if(elo->Bloco2 == bloco_aux) elo->Bloco2 = bloco;
         }
      if(!flag_viz)
         {int ii = 0;
          //aux.sprintf("flag_viz = false (%i)", ijk);
          //Aviso(aux);
         }
      lisBLO->Remove(bloco_aux);
      lisEXT->Remove(bloco_aux);
      delete bloco_aux;
      bloco_aux = NULL;
      flag_funde = true;
      }
   delete lisAux;
      }catch(Exception &)
         {
         return(false);
         }
   return(flag_funde);
   }
*/
//------------------------------------------------------------------------------
bool __fastcall TBlocos::FundeBlocos(VTBloco* bloco, TList* lisEXT)
   {
   //variáveis locais
   VTBloco *bloco_aux;

   //proteção
   if(lisBLO->IndexOf(bloco) == -1) return(false);
   //funde cada bloco da lista no Bloco
   for(int nb = lisEXT->Count - 1; nb >= 0; nb--)
      {
      bloco_aux = (VTBloco*)lisEXT->Items[nb];
      if(! FundeBloco2noBloco1(bloco, bloco_aux)) return(false);
      }
   return(true);
   }

//------------------------------------------------------------------------------
bool __fastcall TBlocos::FundeBlocos(VTElo *elo)
   {
   //proteção
   if (elo == NULL) return(false);
   //proteção: verifica se o Elo conecta o mesmo Bloco
   if (elo->Bloco1 == elo->Bloco2)
      {//insere Chave do Elo nas Ligações do Bloco único
      elo->Bloco1->InsereLigacao(elo->Chave);
      //destrói o Elo indicado
      lisELO->Remove(elo);
      delete elo;
      return(true);
      }
   //insere bloco 2 no bloco 1
   return(FundeBloco2noBloco1(elo->Bloco1, elo->Bloco2));
   }

//------------------------------------------------------------------------------
TList* __fastcall TBlocos::LisBloco(void)
   {
   return(lisBLO);
   }

//------------------------------------------------------------------------------
void __fastcall TBlocos::LisBlocoEnergizado(TList *lisEXT)
   {
   //variáveis locais
   VTBloco *bloco;
   TList   *lisENE;

   //cria lista temporária p/ armazenar Blocos energizados
   lisENE = new TList();
   //inicia lisENE c/ os Blocos c/ geração
   LisBlocoGeracao(lisENE);
   //loop p/ todos Blocos em lisENE
   for (int nb = 0; nb < lisENE->Count; nb++)
      {
      bloco = (VTBloco*)lisENE->Items[nb];
      //obtém lista de Blocos vizinhos conectados
      LisBlocoVizinhoConectado(bloco, lisENE);
      }
   //transfere Blocos de lisENE p/ lisEXT
   for (int nb = 0; nb < lisENE->Count; nb++)
      {
      bloco = (VTBloco*)lisENE->Items[nb];
      //insere Bloco em lisEXT
      if (lisEXT->IndexOf(bloco) < 0) lisEXT->Add(bloco);
      }
   //destrói lista temporária
   delete lisENE;
   }

//------------------------------------------------------------------------------
void __fastcall TBlocos::LisBlocoGeracao(TList *lisEXT)
   {
   //variáveis locais
   VTBloco *bloco;

   //procura Bloco que contém geração
   for (int n = 0; n < lisBLO->Count; n++)
      {
      bloco = (VTBloco*)lisBLO->Items[n];
      //verifica se o Bloco possui geração
      if (bloco->ExisteGeracao())
         {//insere Bloco em lisEXT
         if (lisEXT->IndexOf(bloco) < 0) lisEXT->Add(bloco);
         }
      }
   }

//------------------------------------------------------------------------------
void __fastcall TBlocos::LisBlocoGeracao(VTRede*rede, TList *lisEXT)
   {
   //variáveis locais
   VTBloco *bloco;

   //procura Bloco que contém geração
   for (int n = 0; n < lisBLO->Count; n++)
      {
      bloco = (VTBloco*)lisBLO->Items[n];
      //verifica se o Bloco pertence à Rede indicada
      if (bloco->Rede != rede) continue;
      //verifica se o Bloco possui geração
      if (bloco->ExisteGeracao())
         {//insere Bloco em lisEXT
         if (lisEXT->IndexOf(bloco) < 0) lisEXT->Add(bloco);
         }
      }
   }

//------------------------------------------------------------------------------
void __fastcall TBlocos::LisBlocoNaoEnergizado(TList *lisEXT)
   {
   //variáveis locais
   VTBloco *bloco;
   TList   *lisENE;

   //cria lista temporária p/ armazenar Blocos energizados
   lisENE = new TList();
   //obtém lista de Blocos energizados
   LisBlocoEnergizado(lisENE);
   //determina conjunto de blocos isolados: Blocos que não estão em lisENE
   for (int nb = 0; nb < lisBLO->Count; nb++)
      {
      bloco = (VTBloco*)lisBLO->Items[nb];
      if (lisENE->IndexOf(bloco) < 0)
         {//insere Bloco isolado em lisEXT
         if (lisEXT->IndexOf(bloco) < 0) lisEXT->Add(bloco);
         }
      }
   //destrói lista temporária
   delete lisENE;
   }
/*
//------------------------------------------------------------------------------
void __fastcall TBlocos::LisBlocoRede(VTRede *rede, TList *lisEXT)
   {
   //variáveis locais
   VTBloco *bloco;

   //procura Bloco que contém Ligacoes da Rede
   for (int n = 0; n < lisBLO->Count; n++)
      {
      bloco = (VTBloco*)lisBLO->Items[n];
      if (bloco->ExisteLigacaoDaRede(rede))
         {//insere Bloco em lisEXT
         if (lisEXT->IndexOf(bloco) < 0) lisEXT->Add(bloco);
         }
      }
   }
*/
//------------------------------------------------------------------------------
void __fastcall TBlocos::LisBlocoRede(VTRede *rede, TList *lisEXT)
   {
   //variáveis locais
   VTBloco *bloco;
   VTChave *chave;
   VTElo   *elo;

   //inicia lisTMP c/ as Chaves da Rede
   lisTMP->Clear();
   rede->LisLigacao(lisTMP, eqptoCHAVE);
   //determina Blocos interligados pelas Chaves
   for (int n = 0; n < lisTMP->Count; n++)
      {
      chave = (VTChave*)lisTMP->Items[n];
      if ((elo = ExisteElo(chave)) == NULL) continue;
      //insere Blocos conectados pelo Elo em lisEXT
      if (lisEXT->IndexOf(elo->Bloco1) < 0) lisEXT->Add(elo->Bloco1);
      if (lisEXT->IndexOf(elo->Bloco2) < 0) lisEXT->Add(elo->Bloco2);
      }
   }

//------------------------------------------------------------------------------
void __fastcall TBlocos::LisBlocoVizinho(VTBloco *bloco, TList *lisEXT)
   {
   //variáveis locais
   VTElo *elo;

   //monta lisTMP c/ lista de Elos da fronteira do Bloco indicado
   lisTMP->Clear();
   LisElo(bloco, lisTMP);//JCG Mudanca LisEloFronteira(bloco, lisTMP);
   //loop p/ todos Elos da lista lisTMP
   for (int n = 0; n < lisTMP->Count; n++)
      {
      elo = (VTElo*)lisTMP->Items[n];
      //verifica se elo->Bloco1 é um Bloco diferente do Bloco indicado
      if (elo->Bloco1 != bloco)
         {//insere elo->Bloco1 em lisEXT
          if (lisEXT->IndexOf(elo->Bloco1) < 0) lisEXT->Add(elo->Bloco1);
         }
      //verifica se elo->Bloco2 é um Bloco diferente do Bloco indicado
      if (elo->Bloco2 != bloco)
         {//insere elo->Bloco1 em lisEXT
          if (lisEXT->IndexOf(elo->Bloco2) < 0) lisEXT->Add(elo->Bloco2);
         }
      }
   }

//------------------------------------------------------------------------------
void __fastcall TBlocos::LisBlocoVizinhoConectado(VTBloco *bloco, TList *lisEXT)
   {
   //variáveis locais
   VTElo *elo;

   //monta lisTMP c/ lista de Elos da fronteira do Bloco indicado
   lisTMP->Clear();
   //JCG FIX 2015.04.03
   //LisEloFronteira(bloco, lisTMP);
   LisElo(bloco, lisTMP);
   //loop p/ todos Elos da lista lisTMP
   for (int n = 0; n < lisTMP->Count; n++)
      {
      elo = (VTElo*)lisTMP->Items[n];
      //verifica se Chave do Elo está aberta
      if (elo->Chave->Aberta) continue;
      //verifica se elo->Bloco1 é um Bloco diferente do Bloco indicado
      if (elo->Bloco1 != bloco)
         {//insere elo->Bloco1 em lisEXT
          if (lisEXT->IndexOf(elo->Bloco1) < 0) lisEXT->Add(elo->Bloco1);
         }
      //verifica se elo->Bloco2 é um Bloco diferente do Bloco indicado
      if (elo->Bloco2 != bloco)
         {//insere elo->Bloco1 em lisEXT
          if (lisEXT->IndexOf(elo->Bloco2) < 0) lisEXT->Add(elo->Bloco2);
         }
      }
   }

//------------------------------------------------------------------------------
void __fastcall TBlocos::LisChaveBloco(VTBarra *barra, TList *lisEXT)
   {
   //variáveis locais
   VTBloco *bloco;

   //determina Bloco da Barra
   if ((bloco = ExisteBloco(barra)) != NULL)
      {//determina Chaves do Bloco
      LisChaveBloco(bloco, lisEXT);
      }
   }

//------------------------------------------------------------------------------
void __fastcall TBlocos::LisChaveBloco(VTBloco *bloco, TList *lisEXT)
   {
   //variáveis locais
   VTElo *elo;

   //monta lisTMP c/ lista de Elos do Bloco indicado
   lisTMP->Clear();
   LisElo(bloco, lisTMP);
   //insere em lisEXT as Chaves dos Elos da lista lisTMP
   for (int n = 0; n < lisTMP->Count; n++)
      {
      elo = (VTElo*)lisTMP->Items[n];
      if (lisEXT->IndexOf(elo->Chave) < 0) lisEXT->Add(elo->Chave);
      }
   }

//------------------------------------------------------------------------------
void __fastcall TBlocos::LisChaveBloco(VTLigacao *ligacao, TList *lisEXT)
   {
   //variáveis locais
   VTBloco *bloco;

   //determina Bloco da Ligacao
   if ((bloco = ExisteBloco(ligacao)) != NULL)
      {//determina Chaves do Bloco
      LisChaveBloco(bloco, lisEXT);
      }
   }

//------------------------------------------------------------------------------
void __fastcall TBlocos::LisChaveFronteira(VTBarra *barra, TList *lisEXT)
   {
   //variáveis locais
   VTBloco *bloco;

   //determina Bloco da Ligacao
   if ((bloco = ExisteBloco(barra)) != NULL)
      {//determina Chaves de fronteira do Bloco
      LisChaveFronteira(bloco, lisEXT);
      }
   }

//------------------------------------------------------------------------------
void __fastcall TBlocos::LisChaveFronteira(VTBloco *bloco, TList *lisEXT)
   {
   //variáveis locais
   VTElo *elo;

   //monta lisTMP c/ lista de Elos da fronteira do Bloco indicado
   lisTMP->Clear();
   LisEloFronteira(bloco, lisTMP);
   //insere em lisEXT as Chaves dos Elos da lista lisTMP
   for (int n = 0; n < lisTMP->Count; n++)
      {
      elo = (VTElo*)lisTMP->Items[n];
      if (lisEXT->IndexOf(elo->Chave) < 0) lisEXT->Add(elo->Chave);
      }
   }

//------------------------------------------------------------------------------
void __fastcall TBlocos::LisChaveFronteira(VTLigacao *ligacao, TList *lisEXT)
   {
   //variáveis locais
   VTBloco *bloco;

   //determina Bloco da Ligacao
   if ((bloco = ExisteBloco(ligacao)) != NULL)
      {//determina Chaves de fronteira do Bloco
      //LisChaveFronteira(bloco, lisEXT);
      LisChaveFronteira(bloco, lisEXT);
      }
   }

//------------------------------------------------------------------------------
void __fastcall TBlocos::LisChaveFronteira(VTRede *rede, TList *lisEXT)
   {
   //variáveis locais
   VTElo *elo;

   //monta lisTMP c/ lista de Elos da fronteira da Rede
   lisTMP->Clear();
   LisEloFronteira(rede, lisTMP);
   //insere em lisEXT as Chaves dos Elos da lista lisTMP
   for (int n = 0; n < lisTMP->Count; n++)
      {
      elo = (VTElo*)lisTMP->Items[n];
      if (lisEXT->IndexOf(elo->Chave) < 0) lisEXT->Add(elo->Chave);
      }
   }

//------------------------------------------------------------------------------
void __fastcall TBlocos::LisChaveInterna(VTRede *rede, TList *lisEXT)
   {
   //variáveis locais
   VTElo *elo;

   //monta lisTMP c/ lista de Elos internos da Rede
   lisTMP->Clear();
   LisEloInterno(rede, lisTMP);
   //insere em lisEXT as Chaves dos Elos da lista lisTMP
   for (int n = 0; n < lisTMP->Count; n++)
      {
      elo = (VTElo*)lisTMP->Items[n];
      if (lisEXT->IndexOf(elo->Chave) < 0) lisEXT->Add(elo->Chave);
      }
   }

//------------------------------------------------------------------------------
TList* __fastcall TBlocos::LisElo(void)
   {
   return(lisELO);
   }

//------------------------------------------------------------------------------
void __fastcall TBlocos::LisElo(VTBloco *bloco, TList *lisEXT)
   {
   //variáveis locais
   VTElo *elo;

   //procura Elo que cuja Chave conecta o Bloco
   for (int n = 0; n < lisELO->Count; n++)
      {
      elo = (VTElo*)lisELO->Items[n];
      if (elo->ConectaBloco(bloco))
         {//insere Elo em lisEXT
         if (lisEXT->IndexOf(elo) < 0) lisEXT->Add(elo);
         }
      }
   }

//------------------------------------------------------------------------------
void __fastcall TBlocos::LisEloFronteira(VTBloco *bloco, TList *lisEXT)
   {
   //variáveis locais
   VTElo *elo;
   VTRede *rede = bloco->Rede;

   //procura Elo que cuja Chave conecta o Bloco e um Bloco de uma Rede vizinha
   for (int n = 0; n < lisELO->Count; n++)
      {
      elo = (VTElo*)lisELO->Items[n];
      if (! elo->ConectaBloco(bloco)) continue;
      //verifica se o Elo conecta dois Blocos da mesma Rede
      if (! elo->ConectaRede1Rede2(rede, rede))
         {//insere Elo em lisEXT
         if (lisEXT->IndexOf(elo) < 0) lisEXT->Add(elo);
         }
      }
   }

//------------------------------------------------------------------------------
void __fastcall TBlocos::LisEloFronteira(VTRede *rede, TList *lisEXT)
   {
   //variáveis locais
   VTElo *elo;

   //procura Elo que cuja Chave conecta a Rede a outra Rede vizinha
   for (int n = 0; n < lisELO->Count; n++)
      {
      elo = (VTElo*)lisELO->Items[n];
      //verifica se o Elo conecta dois Blocos da Rede
      if (elo->ConectaRede1Rede2(rede, rede)) continue;
      //verifica se o Elo conecta a Rede
      if (elo->ConectaRede(rede))
         {//insere Elo em lisEXT
         if (lisEXT->IndexOf(elo) < 0) lisEXT->Add(elo);
         }
      }
   }
/*
//------------------------------------------------------------------------------
void __fastcall TBlocos::LisEloInterno(VTBloco *bloco, TList *lisEXT)
   {
   //variáveis locais
   VTElo  *elo;
   VTRede *rede = bloco->Rede;

   //procura Elo que cuja Chave conecta a mesma Rede
   for (int n = 0; n < lisELO->Count; n++)
      {
      elo = (VTElo*)lisELO->Items[n];
      if (! elo->ConectaBloco(bloco)) continue;
      //verifica se o Elo conecta dois Blocos da mesma Rede
      if (elo->ConectaRede1Rede2(rede, rede))
         {//insere Elo em lisEXT
         if (lisEXT->IndexOf(elo) < 0) lisEXT->Add(elo);
         }
      }
   }
*/
//------------------------------------------------------------------------------
void __fastcall TBlocos::LisEloInterno(VTBloco *bloco, TList *lisEXT)
   {
   //variáveis locais
   VTElo  *elo;

   //procura Elo que cuja Chave conecta Barras do mesmo Bloco
   for (int n = 0; n < lisELO->Count; n++)
      {
      elo = (VTElo*)lisELO->Items[n];
      if ((elo->Bloco1 == bloco)&&(elo->Bloco2 == bloco))
         {//insere Elo em lisEXT
         if (lisEXT->IndexOf(elo) < 0) lisEXT->Add(elo);
         }
      }
   }

//------------------------------------------------------------------------------
void __fastcall TBlocos::LisEloInterno(VTRede *rede, TList *lisEXT)
   {
   //variáveis locais
   VTElo *elo;

   //procura Elo que cuja Chave conecta Blocos da Rede
   for (int n = 0; n < lisELO->Count; n++)
      {
      elo = (VTElo*)lisELO->Items[n];
      //verifica se o Elo conecta dois Blocos da Rede
      if (elo->ConectaRede1Rede2(rede, rede))
         {//insere Elo em lisEXT
         if (lisEXT->IndexOf(elo) < 0) lisEXT->Add(elo);
         }
      }
   }

//------------------------------------------------------------------------------
void __fastcall TBlocos::LisRedeVizinha(VTBloco *bloco, TList *lisEXT)
   {
   //variáveis locais
   VTBloco *bloco_viz;
   VTRede  *rede_viz;

   //obtém lista de Blocos vizinhos
   lisVIZ->Clear();
   LisBlocoVizinho(bloco, lisVIZ);
   //identifica Blocos vizinhos associados à outras Redes
   for (int nb = 0; nb < lisVIZ->Count; nb++)
      {
      bloco_viz = (VTBloco*)lisVIZ->Items[nb];
      if ((rede_viz = bloco_viz->Rede) == NULL) continue;
      if (rede_viz != bloco->Rede)
         {//insere Rede do Bloco vizinho em lisEXT
         if (lisEXT->IndexOf(rede_viz) < 0) lisEXT->Add(rede_viz);
         }
      }
   }

//------------------------------------------------------------------------------
bool __fastcall TBlocos::MontaBloco(TList *lisBARRA, TList *lisLIGACAO)
   {
   //variáveis locais
   VTBarra   *barra, *bar_lig;
   VTBloco   *bloco;
   VTLigacao *ligacao;

   try{//monta Bloco a partir das Barras
      while (lisBARRA->Count > 0)
         {//retira primeira Barra da lista
         barra = (VTBarra*)lisBARRA->First();
         lisBARRA->Delete(0);
         //cria um novo Bloco
         if ((bloco = NewObjBloco()) == NULL) return(false);
         //insere Bloco em lisBLO
         lisBLO->Add(bloco);
         //utiliza lisTMP p/ armazenar a lista de Barras do Bloco
         lisTMP->Clear();
         lisTMP->Add(barra);
         //monta Bloco a partir das suas Barraa armazenadas em lisTMP
         for (int nb = 0; nb < lisTMP->Count; nb++)
            {
            barra = (VTBarra*)lisTMP->Items[nb];
            //insere Barra no Bloco
            bloco->InsereBarra(barra);
            //determina Ligacoes que conectam a Barra do Bloco
            for (int nl = lisLIGACAO->Count-1; nl >= 0; nl--)
               {
               ligacao = (VTLigacao*)lisLIGACAO->Items[nl];
               //descarta Ligacao que é Chave
               if (ligacao->Tipo() == eqptoCHAVE)
                  {
                  lisLIGACAO->Delete(nl);
                  continue;
                  }
               //verifica se a Ligacao conecta a Barra
               if (! ligacao->ConectaBarra(barra)) continue;
               //transfere a Ligacao de lisLIGACAO p/ o Bloco
               lisLIGACAO->Delete(nl);
               bloco->InsereLigacao(ligacao);
               //transfere as Barras da Ligacao de lisBARRA p/ lisTMP
               for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
                  {
                  if ((bar_lig = ligacao->Barra(ind_bar)) == NULL) continue;
                  if (bar_lig == barra) continue;
                  //retira Barra de lisBARRA
                  lisBARRA->Remove(bar_lig);
                  //insere Barra em lisTMP
                  if (lisTMP->IndexOf(bar_lig) < 0) lisTMP->Add(bar_lig);
                  }
               }
            }
         }
      }catch(Exception &)
         {
         return(false);
         }
   return(true);
/*
   //ALTERACOES HK variáveis locais
   VTBarra   *barra, *bar_lig;
   VTBloco   *bloco;
   VTLigacao *ligacao;
   try{//monta Bloco a partir das Barras
      while (lisBARRA->Count > 0)
         {//retira primeira Barra da lista
         barra = (VTBarra*)lisBARRA->First();
         lisBARRA->Delete(0);
         //cria um novo Bloco
         if ((bloco = NewObjBloco()) == NULL) return(false);
         //insere Bloco em lisBLO
         lisBLO->Add(bloco);
         //utiliza lisTMP p/ armazenar a lista de Barras do Bloco
         lisTMP->Clear();
         lisTMP->Add(barra);
         //monta Bloco a partir das suas Barraa armazenadas em lisTMP
         for (int nb = 0; nb < lisTMP->Count; nb++)
            {
            barra = (VTBarra*)lisTMP->Items[nb];
            //insere Barra no Bloco
            bloco->InsereBarra(barra);
            //determina Ligacoes que conectam a Barra do Bloco
            for (int nl = lisLIGACAO->Count-1; nl >= 0; nl--)
               {
               ligacao = (VTLigacao*)lisLIGACAO->Items[nl];
               //hkMudanca funcao existente
               //descarta Ligacao que é Chave
               //if (ligacao->Tipo() == eqptoCHAVE)
               //hk   {
               //   lisLIGACAO->Delete(nl);
               //   continue;
               //hk   }
               //verifica se a Ligacao conecta a Barra
               if (! ligacao->ConectaBarra(barra)) continue;
               if (ligacao->Tipo() == eqptoCHAVE)
                  {
                  bloco->InsereChave(ligacao);  //Nova funcao de bloco
                  //hklisLIGACAO->Delete(nl); chave do bloco vizinho
                  continue;
                  }
               //transfere a Ligacao de lisLIGACAO p/ o Bloco
               //hk lisLIGACAO->Delete(nl);
               bloco->InsereLigacao(ligacao);
               //transfere as Barras da Ligacao de lisBARRA p/ lisTMP
               for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
                  {
                  if ((bar_lig = ligacao->Barra(ind_bar)) == NULL) continue;
                  if (bar_lig == barra) continue;
                  //retira Barra de lisBARRA
                  lisBARRA->Remove(bar_lig);
                  //insere Barra em lisTMP
                  if (lisTMP->IndexOf(bar_lig) < 0) lisTMP->Add(bar_lig);
                  }
               }
            }
         }
      }catch(Exception &)
         {
         return(false);
         }
   return(true);
*/

   }

//------------------------------------------------------------------------------
bool __fastcall TBlocos::MontaElo(TList *lisBLOCO, TList *lisCHAVE)
   {
   //variáveis locais
   VTBloco *bloco1, *bloco2;
   VTChave *chave;
   VTElo   *elo;

   //loop p/ todas Chaves em lisCHAVE
   for (int n = 0; n < lisCHAVE->Count; n++)
      {
      chave = (VTChave*)lisCHAVE->Items[n];
      //determina Blocos conectados pela Chave
      if ((bloco1 = ExisteBloco(chave->pbarra1)) == NULL) return(false);
      if ((bloco2 = ExisteBloco(chave->pbarra2)) == NULL) return(false);
      //cria Elo
      if ((elo = NewObjElo()) == NULL) return(false);
      //define Chave e Blocos do Elo
      elo->Bloco1 = bloco1;
      elo->Bloco2 = bloco2;
      elo->Chave  = chave;
      //insere Elo em lisELO
      lisELO->Add(elo);
      //IMPORTANTE: Pode existir Bloco sem Ligacao, formado por apenas uma Barra.
      //            Isto ocorre quando há Barra conectada apenas por Chaves.
      //            A solução adotada é associar o Bloco à uma Rede (de uma das Chaves)
      //            e manter vazia sua lista de Ligacoes
      //tratamento p/ Bloco sem Ligacao (formado por uma única Barra)
//      elo->Bloco1->DefineRedeParaBlocoSemLigacao(elo->Chave->rede);
//      elo->Bloco2->DefineRedeParaBlocoSemLigacao(elo->Chave->rede);
      }
   return(true);
   }

//------------------------------------------------------------------------------
void __fastcall TBlocos::LisBlocoVizinho1(VTBloco *bloco, TList *lisEXT, TList* lisCHV)
   {
   //hk  Nova Funcao
   //variáveis locais
   VTElo *elo;


   //monta lisTMP c/ lista de Elos da fronteira do Bloco indicado
   lisTMP->Clear();
   LisElo(bloco, lisTMP);
   //LisEloFronteira(bloco, lisTMP);
   //loop p/ todos Elos da lista lisTMP
   for (int n = 0; n < lisTMP->Count; n++)
      {
      elo = (VTElo*)lisTMP->Items[n];

      //verifica se Chave do Elo está aberta
      if (elo->Chave->Aberta) continue;
      //verifica se elo pertence a lisCHV
      if(lisCHV != NULL)
        {if(!(lisCHV->IndexOf(elo->Chave) < 0)) continue;
        }
      //verifica se elo->Bloco1 é um Bloco diferente do Bloco indicado
      if (elo->Bloco1 != bloco)
         {//insere elo->Bloco1 em lisEXT
          if (lisEXT->IndexOf(elo->Bloco1) < 0) lisEXT->Add(elo->Bloco1);
         }
      //verifica se elo->Bloco2 é um Bloco diferente do Bloco indicado
      if (elo->Bloco2 != bloco)
         {//insere elo->Bloco1 em lisEXT
          if (lisEXT->IndexOf(elo->Bloco2) < 0) lisEXT->Add(elo->Bloco2);
         }
      }
   }

//------------------------------------------------------------------------------
void __fastcall TBlocos::LisEloFronteiraSuper(VTBloco *bloco, TList *lisEXT)
   {
   //HK lista de elos super blocos variáveis locais
   VTElo *elo;

   //procura Elo que cuja Chave conecta o Bloco
   for (int n = 0; n < lisELO->Count; n++)
      {
      elo = (VTElo*)lisELO->Items[n];
      if (elo->ConectaBloco(bloco))
         {//insere Elo em lisEXT
         if (lisEXT->IndexOf(elo) < 0) lisEXT->Add(elo);
         }
      }
   }
//------------------------------------------------------------------------------
// eof



