//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TRecorta.h"
#include "..\Apl\VTApl.h"
#include "..\Cronometro\VTCronometro.h"
#include "..\Consulta\VTConsulta.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Rede\VTArea.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCorte.h"
#include "..\Rede\VTMRede.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTReducao.h"
#include "..\Rede\VTRedes.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Rede.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)


//---------------------------------------------------------------------------
//função static p/ comparar número de MRedes de 2 Cortes
static int __fastcall ComparaCortePorNumeroMRede(void *obj1, void* obj2)
   {
   //variáveis locais
   //int count1 = (((VTRede*)obj1)->LisBarra())->Count;
   //int count2 = (((VTRede*)obj2)->LisBarra())->Count;
   //return(count2 - count1);
   return((((VTCorte*)obj2)->NumeroMRedes()) - (((VTCorte*)obj1)->NumeroMRedes()));
   }

//---------------------------------------------------------------------------
//função static p/ comparar número de Barras de 2 redes
static int __fastcall ComparaRedePorNumeroBarra(void *obj1, void* obj2)
   {
   //variáveis locais
   //int count1 = (((VTRede*)obj1)->LisBarra())->Count;
   //int count2 = (((VTRede*)obj2)->LisBarra())->Count;
   //return(count2 - count1);
   return((((VTRede*)obj2)->LisBarra())->Count - (((VTRede*)obj1)->LisBarra())->Count);
   }

//---------------------------------------------------------------------------
VTRecorta* __fastcall NewObjRecorta(VTApl *apl)
   {
   return(new TRecorta(apl));
   }

//---------------------------------------------------------------------------
__fastcall TRecorta::TRecorta(VTApl *apl)
   {
   //salva ponteiro p/ objetos
   this->apl = apl;
   //cria objetos
   local.lisBR1   = new TList();
   local.lisBR2   = new TList();
   local.lisCORTE = new TList();
   local.lisEQP   = new TList();
   local.lisMR1   = new TList();
   local.lisMR2   = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TRecorta::~TRecorta(void)
   {
   //destrói lista sem destruir seus objetos
   if (local.lisBR1)   {delete local.lisBR1;   local.lisBR1   = NULL;}
   if (local.lisBR2)   {delete local.lisBR2;   local.lisBR2   = NULL;}
   if (local.lisCORTE) {delete local.lisCORTE; local.lisCORTE = NULL;}
   if (local.lisEQP)   {delete local.lisEQP;   local.lisEQP   = NULL;}
   if (local.lisMR1)   {delete local.lisMR1;   local.lisMR1   = NULL;}
   if (local.lisMR2)   {delete local.lisMR2;   local.lisMR2   = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TRecorta::EliminaReducaoMRedeCarregada(void)
   {
   //variáveis locais
   VTMRede *mrede;
   TList   *lisMR;

   //loop p/ todas MRedes carregadas
   lisMR = redes->LisMRede();
   for (int n = 0; n < lisMR->Count; n++)
      {//verifica se a MRede está carregada
      mrede = (VTMRede*)lisMR->Items[n];
      if (! mrede->Carregada) continue;
      //elimina Reducoes da MRede. Ao eliminar a Reducao a MRede é excluída do Corte
      mrede->ReiniciaReducao();
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TRecorta::Executa(VTRedes *redes)
   {
   //salva ponteiro p/ objetos
   this->redes = redes;
   cronometro = (VTCronometro*)apl->GetObject(__classid(VTCronometro));
   //reinicia Cortes:
   //a) limpa listas de Cortes de cada Rede carregada
   //b) limpa listas de Redes de cada Corte
   //c) reinicia Redes dos Cortes das MRedes reduzidas
   ReiniciaCortes();
   //elimina Reducoes de todas MRedes carregadas
   EliminaReducaoMRedeCarregada();
   //identifica novo conjunto de Cortes
   MontaNovoConjuntoCortes();
   //exibe tempos
   //if (cronometro) cronometro->Show();
   return(true);
   }
   
/*
//---------------------------------------------------------------------------
bool __fastcall TRecorta::ExisteBarraInterface(VTRede *rede1, VTRede *rede2, TList *lisBAR)
   {
   //variáveis locais
   VTBarra *pbarra;
//JCG 2011.10.27

   //obtém lista de Barras da Rede1, sem as Barras conectadas por chave aberta
   local.lisBR1->Clear(); rede1->LisBarra(local.lisBR1, false);
   //obtém lista de Barras da Rede2, sem as Barras conectadas por chave aberta
   local.lisBR2->Clear(); rede2->LisBarra(local.lisBR2, false);
   //reinicia lisBAR c/ as Barras presentes nas duas listas
   lisBAR->Clear();
   //loop p/ todas Barras em lisBR1 (lista de Barras da Rede1)
   for (int nb = 0; nb < local.lisBR1->Count; nb++)
      {
      pbarra = (VTBarra*)local.lisBR1->Items[nb];
      //verifica se a Barra existe em local.lisBR2 (lista de Barras da Rede2)
      if (local.lisBR2->IndexOf(pbarra) >= 0) lisBAR->Add(pbarra);
      }
   return(lisBAR->Count > 0);
   }
*/

//---------------------------------------------------------------------------
bool __fastcall TRecorta::ExisteBarraInterface(TList *lisBAR1, TList *lisBAR2, VTArea *area2, TList *lisBAR12)
   {
   //variáveis locais
   VTBarra *pbarra;
//JCG 2011.10.27

   //reinicia lisBAR12 c/ as Barras presentes nas duas listas
   lisBAR12->Clear();
   //loop p/ todas Barras em lisBAR1
   for (int nb = 0; nb < lisBAR1->Count; nb++)
      {
      pbarra = (VTBarra*)lisBAR1->Items[nb];
      //verifica se Area da Rede2 contem a Barra
      //if ((area2->x1 > 0)&&(area2->ContemPonto(pbarra->utm.x, pbarra->utm.y)))
      if (area2->x1 > 0)
         {
         if (! area2->ContemPonto(pbarra->utm.x, pbarra->utm.y)) continue;
         }
      //verifica se a Barra existe em lisBAR2
      if (lisBAR2->IndexOf(pbarra) >= 0) lisBAR12->Add(pbarra);
      }
   return(lisBAR12->Count > 0);
   }
/*
//---------------------------------------------------------------------------
void __fastcall TRecorta::MontaConjuntoBarrasCorteDaReducao(VTReducao *reducao)
   {
   //variáveis locais
   TList   *lisREDE;
   VTBarra *barra;
   VTMRede *mrede;
   VTRede  *rede;
   VTCorte *corte  = reducao->Corte;
   TList   *lisBAR = reducao->LisBarra();
   TList   *lisMR  = redes->LisMRede();

   //reinicia Barras do Corte
   corte->LisBarra()->Clear();
   //verifica se a Reducao possui apenas uma Barra
   if (lisBAR->Count == 1)
      {//insere Barra da Reducao no Corte
      corte->InsereLisBarra(lisBAR);
      return;
      }
   //loop p/ todas MRede carregadas
   for (int n = 0; n < lisMR->Count; n++)
      {
      mrede = (VTMRede*)lisMR->Items[n];
      if (! mrede->Carregada) continue;
      //verifica se a MRede conecta o Corte da Reducao
      if (! mrede->ExisteCorte(corte)) continue;
      //loop p/ todas Redes da MRede
      lisREDE = mrede->LisRede();
      for (int i = 0; i < lisREDE->Count; i++)
         {
         rede = (VTRede*)lisREDE->Items[i];
         //loop p/ todas Barras da Reducao
         for (int j = 0; j < lisBAR->Count; j++)
            {
            barra = (VTBarra*)lisBAR->Items[j];
            if (rede->ExisteBarra(barra))
               {//insere Barra no Corte da Reducao
               corte->InsereBarra(barra);
               }
            }
         }
      //basta analisar uma MRede vizinha
      return;
      }
   }
*/

//---------------------------------------------------------------------------
void __fastcall TRecorta::MontaConjuntoBarrasCorteDaReducao(VTReducao *reducao)
   {
   //variáveis locais
   VTArea  *area;
   VTBarra *barra;
   VTRede  *rede;
   VTCorte *corte   = reducao->Corte;
   TList   *lisBAR  = reducao->LisBarra();
   TList   *lisREDE = redes->LisRede();

   //reinicia Barras do Corte
   corte->LisBarra()->Clear();
   //verifica se a Reducao possui apenas uma Barra
   if (lisBAR->Count == 1)
      {//insere Barra da Reducao no Corte
      corte->InsereLisBarra(lisBAR);
      return;
      }
   //loop p/ todas Redes
   for (int nr = 0; nr < lisREDE->Count; nr++)
      {
      rede = (VTRede*)lisREDE->Items[nr];
      //desconsidera Rede não carregada
      if (! rede->Carregada) continue;
      //desconsidera MRede da Reducao
      if (rede->MRede == reducao->MRede) continue;
      //determina Area da Rede
      area = rede->AreaUtm_cm();
      //loop p/ todas Barras da Reducao
      for (int nb = 0; nb < lisBAR->Count; nb++)
         {
         barra = (VTBarra*)lisBAR->Items[nb];
         //verifica se a Barra da Reducao existe na Rede
         if (area->x1 > 0)
            {
            if (! area->ContemPonto(barra->utm.x, barra->utm.y)) continue;
            }
         if (rede->LisBarra()->IndexOf(barra) >= 0)
            {
            corte->InsereBarra(barra);
            }
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TRecorta::MontaNovoConjuntoCortes(void)
   {
   //variáveis locais
   VTArea    *areaI, *areaJ;
   VTBarra   *barra;
   VTCorte   *corte, *corteI, *corteJ;
   VTMRede   *mrede;
   VTRede    *rede, *redeI, *redeJ;
   VTReducao *reducao;
   TList     *lisBAR_CORTE;
   TList     *lisREDU;
   TList     *lisMR    = redes->LisMRede();
   TList     *lisREDE  = redes->LisRede();

   //reinicia lista de Cortes c/ os Cortes das MRedes reduzidas
   //if (cronometro) cronometro->Start("Corte 0", "Identifica Barras dos Cortes das MRedes reduzidas");
   local.lisCORTE->Clear();
   for (int n = 0; n < lisMR->Count; n++)
      {
      mrede = (VTMRede*)lisMR->Items[n];
      if (mrede->Carregada) continue;
      //OBS: uma MRede reduzida tem uma única Redução carregada
      lisREDU = mrede->LisReducao();
      for (int nr = 0; nr < lisREDU->Count; nr++)
         {
         reducao = (VTReducao*)lisREDU->Items[nr];
         corte   = reducao->Corte;
         if (local.lisCORTE->IndexOf(corte) < 0) local.lisCORTE->Add(corte);
         //verifica se as Barras do Corte estão definidas
         if (corte->NumeroBarras() == 0)
            {//identifica Barras do Corte envolvendo Reducao
            MontaConjuntoBarrasCorteDaReducao(reducao);
            }
         }
      }
   //if (cronometro) cronometro->Stop("Corte 0");
   //identifica Redes que conectam os Cortes das MRedes reduzidas
   //if (cronometro) cronometro->Start("Corte 1", "Identifica Redes que conectam os Cortes das MRedes reduzidas");
   //verifica se as Rede contém todas (ou parte de) as Barras do Corte
   for (int nr = 0; nr < lisREDE->Count; nr++)
      {
      rede = (VTRede*)lisREDE->Items[nr];
      //verifica se a Rede está carregada
      if (! rede->Carregada) continue;
      //determina Area da Rede
      areaI = rede->AreaUtm_cm();
//JCG 2011.10.27
      //obtém lista de Barras da Rede (sem as Barras das Chaves abertas)
      local.lisBR1->Clear(); rede->LisBarra(local.lisBR1, false);
      //loop p/ todos Cortes associados a MRedes reduzidas
      for (int nc = 0; nc < local.lisCORTE->Count; nc++)
         {
         corte = (VTCorte*)local.lisCORTE->Items[nc];
         //determina lista de Barras do Corte d
         lisBAR_CORTE = corte->LisBarra();
         for (int nb = 0; nb < lisBAR_CORTE->Count; nb++)
            {
            barra = (VTBarra*)lisBAR_CORTE->Items[nb];
            //verifica se a Area da Rede está definida e se contém a Barra do Corte
            if (areaI->x1 > 0)
               {
               if (! areaI->ContemPonto(barra->utm.x, barra->utm.y)) continue;
               }
            if (local.lisBR1->IndexOf(barra) >= 0)
               {//Rede faz interface c/ o Corte
               corte->InsereRede(rede);
               rede->InsereCorte(corte);
               break;
               }
            }
         }
      }
   //if (cronometro) cronometro->Stop("Corte 1");
   //cria Cortes para cada Barra comum a duas Redes carregadas
   //if (cronometro) cronometro->Start("Corte 2", "Cria Cortes para cada Barra comum a duas Redes carregadas");
   //ordena Redes por número decrescente de Barras p/ diminuir processamento
   lisREDE->Sort(ComparaRedePorNumeroBarra);
   for (int i = 0; i < lisREDE->Count; i++)
      {
      redeI = (VTRede*)lisREDE->Items[i];
      if (! redeI->Carregada) continue;
      //determina área da RedeI
      areaI = redeI->AreaUtm_cm();
      //obtém lista de Barras da RedeI, sem as Barras conectadas por chave aberta
      local.lisBR1->Clear(); redeI->LisBarra(local.lisBR1, false);
      //loop p/ as Redes a partir de i+1
      for (int j = i+1; j < lisREDE->Count; j++)
         {
         redeJ = (VTRede*)lisREDE->Items[j];
         if (! redeJ->Carregada) continue;
         //determina área da RedeJ
         areaJ = redeJ->AreaUtm_cm();
         //verifica se as duas Redes áreas definidas
         if ((areaI->x1 > 0)&&(areaJ->x1 > 0))
            {//filtro inicial: verifica se as Redes podem ser vizinhas pela suas áreas
            if (areaJ->x2 < areaI->x1) continue;
            if (areaJ->x1 > areaI->x2) continue;
            if (areaJ->y2 < areaI->y1) continue;
            if (areaJ->y1 > areaI->y2) continue;
            }
         //obtém lista de Barras da RedeJ, sem as Barras conectadas por chave aberta
         local.lisBR2->Clear(); redeJ->LisBarra(local.lisBR2, false);
         //verifica se as Redes são vizinhas através de um conjunto de Barras
         local.lisEQP->Clear();
         if (ExisteBarraInterface(local.lisBR1, local.lisBR2, areaJ, local.lisEQP))
            {//cria um novo Corte p/ as Barras comuns e insere em local.lisCORTE
            if ((corte = DLL_NewObjCorte()) == NULL) continue;
            local.lisCORTE->Add(corte);
            //insere Barras no Corte
            corte->InsereLisBarra(local.lisEQP);
            //insere o Corte nas duas Redes
            redeI->InsereCorte(corte);
            redeJ->InsereCorte(corte);
            //insere as duas Redes no Corte
            corte->InsereRede(redeI);
            corte->InsereRede(redeJ);
            }
         }
      }
   //if (cronometro) cronometro->Stop("Corte 2");
   //funde os Cortes que tem as mesmas Redes (mas envolvendo Barras distintas)
   //if (cronometro) cronometro->Start("Corte 3", "funde os Cortes que tem as mesmas Redes");
   for (int i = 0; i < local.lisCORTE->Count; i++)
      {
      corteI = (VTCorte*)local.lisCORTE->Items[i];
      for (int j = local.lisCORTE->Count-1; j > i; j--)
         {
         corteJ = (VTCorte*)local.lisCORTE->Items[j];
         //verifica se o conjunto de Redes é igual nos dois Cortes
         if (Lista1IgualLista2(corteI->LisRede(), corteJ->LisRede()))
            {//transfere Barras do CorteJ p/ o CorteI
            corteI->InsereLisBarra(corteJ->LisBarra());
            //retira CorteJ das Redes
            lisREDE = corteJ->LisRede();
            for (int nr = 0; nr < lisREDE->Count; nr++)
               {
               rede = (VTRede*)lisREDE->Items[nr];
               //atualiza Rede: retira CorteJ
               rede->RemoveCorte(corteJ);
               }
            //elimina CorteJ
            local.lisCORTE->Delete(j);
            delete corteJ;
            }
         }
      }
/*
   ////if (cronometro) cronometro->Stop("Corte 3");
   //funde Cortes que tem as mesmas Barras (mas envolvendo Redes distintas)
   ////if (cronometro) cronometro->Start("Corte 4", "funde Cortes que tem as mesmas Barras");
   for (int i = 0; i < local.lisCORTE->Count; i++)
      {
      corteI = (VTCorte*)local.lisCORTE->Items[i];
      for (int j = local.lisCORTE->Count-1; j > i; j--)
         {
         corteJ = (VTCorte*)local.lisCORTE->Items[j];
         if (corteJ == corteI) continue;
         //verifica se o conjunto de Barras é igual nos dois Cortes
         if (Lista1IgualLista2(corteI->LisBarra(), corteJ->LisBarra()))
            {//transfere Redes do CorteJ p/ o CorteI
            lisREDE = corteJ->LisRede();
            for (int nr = 0; nr < lisREDE->Count; nr++)
               {
               rede = (VTRede*)lisREDE->Items[nr];
               //insere Rede no CorteI
               corteI->InsereRede(rede);
               //atualiza Rede: troca CorteJ por CorteI
               rede->RemoveCorte(corteJ);
               rede->InsereCorte(corteI);
               }
            //elimina CorteJ
            local.lisCORTE->Delete(j);
            delete corteJ;
            }
         }
      }
*/
   //if (cronometro) cronometro->Stop("Corte 4");
   //funde Cortes que contém todas as Barras de outro Corte
   //if (cronometro) cronometro->Start("Corte 5", "Funde Cortes que tem as mesmas Barras");
   for (int i = 0; i < local.lisCORTE->Count; i++)
      {
      corteI = (VTCorte*)local.lisCORTE->Items[i];
      for (int j = local.lisCORTE->Count-1; j > i; j--)
         {
         corteJ = (VTCorte*)local.lisCORTE->Items[j];
         if (corteJ == corteI) continue;
         //verifica se o conjunto de Barras é igual nos dois Cortes
         if (Lista1ContemLista2(corteI->LisBarra(), corteJ->LisBarra()))
            {//transfere Redes do CorteJ p/ o CorteI
            lisREDE = corteJ->LisRede();
            for (int nr = 0; nr < lisREDE->Count; nr++)
               {
               rede = (VTRede*)lisREDE->Items[nr];
               //insere Rede no CorteI
               corteI->InsereRede(rede);
               //atualiza Rede: troca CorteJ por CorteI
               rede->RemoveCorte(corteJ);
               rede->InsereCorte(corteI);
               }
            //elimina CorteJ
            local.lisCORTE->Delete(j);
            delete corteJ;
            }
         }
      }
   //if (cronometro) cronometro->Stop("Corte 5");
   //reinicia lista de Cortes de Redes
   redes->ReiniciaLisCorte(local.lisCORTE);
   }

//---------------------------------------------------------------------------
void __fastcall TRecorta::ReiniciaCortes(void)
   {
   //variáveis locais
   VTCorte *corte;
   VTRede  *rede;
   TList   *lisCORTE = redes->LisCorte();
   TList   *lisREDE  = redes->LisRede();

   //reinicia lista de Cortes de cada Rede carregada
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      if (rede->Carregada) rede->ReiniciaLisCorte();
      }
   //reinicia listas de Redes de cada Corte
   for (int nc = 0; nc < lisCORTE->Count; nc++)
      {
      corte = (VTCorte*)lisCORTE->Items[nc];
      corte->ReiniciaLisRede();
      }
   //reinicia Redes dos Cortes das MRedes reduzidas
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      if (! rede->Carregada)
         {
         lisCORTE = rede->LisCorte();
         for (int nc = 0; nc < lisCORTE->Count; nc++)
            {
            corte = (VTCorte*)lisCORTE->Items[nc];
            corte->InsereRede(rede);
            }
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TRecorta::VerificaExclusaoDeCortes(void)
   {
   //variáveis locais
   bool    exclusao = false;
   TList   *lisREDE;
   VTCorte *corte;
   VTMRede *mrede;
   VTRede  *rede;

   //obtém lista de todos os Cortes
   local.lisCORTE->Clear();
   CopiaTList(redes->LisCorte(), local.lisCORTE);
   //elimina os Cortes que tem uma única MRede
   for (int i = local.lisCORTE->Count-1; i >= 0; i--)
      {
      corte = (VTCorte*)local.lisCORTE->Items[i];
      //obtém lista de MRede do Corte
      local.lisMR1->Clear();
      corte->LisMRede(local.lisMR1);
      if (local.lisMR1->Count == 1)
         {
         mrede = (VTMRede*)local.lisMR1->First();
         //verifica se MRede Carregada
         if (! mrede->Carregada) continue;
         //retira Corte da MRede
         mrede->RemoveCorte(corte);
         //retira Corte das Redes
         lisREDE = corte->LisRede();
         for (int n = 0; n < lisREDE->Count; n++)
            {
            rede = (VTRede*)lisREDE->Items[n];
            rede->RemoveCorte(corte);
            }
         //retira Corte da lista lisCORTE sem destruir objeto
         local.lisCORTE->Delete(i);
         exclusao = true;
         }
      }
   //atualiza lista de Cortes seomente se tiver havido exclusaão
   if (exclusao) redes->ReiniciaLisCorte(local.lisCORTE);
   }

//---------------------------------------------------------------------------
void __fastcall TRecorta::VerificaFusaoDeCortes(TList *lisMR)
   {
   //variáveis locais
   bool      fusao = false;
   VTCorte   *corte, *corteI, *corteJ;
   VTMRede   *mrede;
   VTRede    *rede;
   TList     *lisREDE;

   //obtém lista de todos os Cortes
   local.lisCORTE->Clear();
   CopiaTList(redes->LisCorte(), local.lisCORTE);
   //ordena Cortes em número decrescente de MRedes
   local.lisCORTE->Sort(ComparaCortePorNumeroMRede);
   //funde os Cortes que tem as mesmas MRede (mas envolvendo Barras distintas)
   for (int i = 0; i < local.lisCORTE->Count; i++)
      {
      corteI = (VTCorte*)local.lisCORTE->Items[i];
      for (int j = local.lisCORTE->Count-1; j > i; j--)
         {
         corteJ = (VTCorte*)local.lisCORTE->Items[j];
         //obtém conjunto de MRede do CorteI: IMPORTANTE ficar dento do loop j
         local.lisMR1->Clear();
         corteI->LisMRede(local.lisMR1);
         //obtém conjunto de MRede do CorteJ
         local.lisMR2->Clear();
         corteJ->LisMRede(local.lisMR2);
         //verifica se o conjunto de MRedes é igual nos dois Cortes
         //if (Lista1IgualLista2(local.lisMR1, local.lisMR2))
         //verifica se o conjunto de MRedes do CorteI contém todas as MREdes do CorteJ
         if (Lista1ContemLista2(local.lisMR1, local.lisMR2))
            {//transfere Barras do CorteJ p/ o CorteI
            corteI->InsereLisBarra(corteJ->LisBarra());
            //transfere Redes do CorteJ para CorteI
            corteI->InsereLisRede(corteJ->LisRede());
            //atualiza Redes do CorteJ: retira CorteJ e insere CorteI
            lisREDE = corteJ->LisRede();
            for (int n = 0; n < lisREDE->Count; n++)
               {
               rede = (VTRede*)lisREDE->Items[n];
               //atualiza Rede: retira CorteJ  e insere CorteI
               rede->RemoveCorte(corteJ);
               rede->InsereCorte(corteI);
               }
            //retira CorteJ das MRedes
            for (int n = 0; n < local.lisMR1->Count; n++)
               {
               mrede = (VTMRede*)local.lisMR1->Items[n];
               mrede->RemoveCorte(corteJ);
               }
            //retira CorteJ da lista lisCORTE sem destruir objeto
            local.lisCORTE->Delete(j);
            //delete corteJ;
            //marca que houve fusão de cortes
            fusao = true;
            }
         }
      }
   //atualiza lista de Cortes seomente se tiver havido fusao
   if (fusao) redes->ReiniciaLisCorte(local.lisCORTE);
   }
/*
//---------------------------------------------------------------------------
void __fastcall TRecorta::VerificaFusaoDeCortes(TList *lisMR)
   {
   //variáveis locais
   bool      fusao = false;
   VTCorte   *corte, *corteI, *corteJ;
   VTMRede   *mrede;
   VTRede    *rede;
   TList     *lisREDE;

   //obtém lista de todos os Cortes
   local.lisCORTE->Clear();
   CopiaTList(redes->LisCorte(), local.lisCORTE);
   //funde os Cortes em que TODAS as MRedes de um Corte também pertencem ao outro 
   for (int i = 0; i < local.lisCORTE->Count; i++)
      {
      corteI = (VTCorte*)local.lisCORTE->Items[i];
      //obtém conjunto de MRede do CorteI
      local.lisMR1->Clear();
      corteI->LisMRede(local.lisMR1);
      if (local.lisMR1->Count <= 1) continue;
      for (int j = local.lisCORTE->Count-1; j > i; j--)
         {
         corteJ = (VTCorte*)local.lisCORTE->Items[j];
         //obtém conjunto de MRede do CorteJ
         local.lisMR2->Clear();
         corteJ->LisMRede(local.lisMR2);
         //verifica se o conjunto de MRedes do CorteI contem todas as MRedes do CorteJ ou vice-versa
         if (Lista1ContemLista2(local.lisMR1, local.lisMR2)||
             Lista1ContemLista2(local.lisMR2, local.lisMR1))
            {//transfere Barras do CorteJ p/ o CorteI
            corteI->InsereLisBarra(corteJ->LisBarra());
            //transfere Redes do CorteJ para CorteI
            corteI->InsereLisRede(corteJ->LisRede());
            //atualiza Redes do CorteJ: retira CorteJ e insere CorteI
            lisREDE = corteJ->LisRede();
            for (int n = 0; n < lisREDE->Count; n++)
               {
               rede = (VTRede*)lisREDE->Items[n];
               //atualiza Rede: retira CorteJ  e insere CorteI
               rede->RemoveCorte(corteJ);
               rede->InsereCorte(corteI);
               }
            //retira CorteJ das MRedes de local.lisMR1
            for (int n = 0; n < local.lisMR1->Count; n++)
               {
               mrede = (VTMRede*)local.lisMR1->Items[n];
               mrede->RemoveCorte(corteJ);
               }
            //retira CorteJ das MRedes de local.lisMR2
            for (int n = 0; n < local.lisMR2->Count; n++)
               {
               mrede = (VTMRede*)local.lisMR2->Items[n];
               mrede->RemoveCorte(corteJ);
               }
            //insere CorteI nas MRedes de local.lisMR2
            for (int n = 0; n < local.lisMR2->Count; n++)
               {
               mrede = (VTMRede*)local.lisMR2->Items[n];
               mrede->InsereCorte(corteI);
               }
            //retira CorteJ da lista lisCORTE sem destruir objeto
            local.lisCORTE->Delete(j);
            //delete corteJ;
            //marca que houve fusão de cortes
            fusao = true;
            }
         }
      }
   //atualiza lista de Cortes seomente se tiver havido fusao
   if (fusao) redes->ReiniciaLisCorte(local.lisCORTE);
   }
*/

//---------------------------------------------------------------------------
//eof


