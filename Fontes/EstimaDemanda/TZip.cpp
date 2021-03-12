//---------------------------------------------------------------------------
#pragma hdrstop
#include "TZip.h"
#include "..\Apl\VTApl.h"
#include "..\Arranjo\VTArranjo.h"
//#include "..\Edita\VTEdita.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTMatC.h"
#include "..\Rede\VTMatZ.h"
#include "..\Rede\VTMedidor.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTTipoChave.h"
#include "..\Rede\VTTipoRede.h"
#include "..\Rede\VTTrecho.h"
#include "..\..\DLL_Inc\Funcao.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TZip::TZip(VTApl *apl)
   {
   //salva ponteiro para objeto
   this->apl = apl;
   //cria lista
   lisCHV = new TList();
   lisDEL = new TList();
   lisEQP = new TList();
   lisLIG = new TList();
   lisMED = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TZip::~TZip(void)
   {
   //destrói lista sem destruir seus objetos
   if (lisCHV) {delete lisCHV; lisCHV = NULL;}
   if (lisDEL) {delete lisDEL; lisDEL = NULL;}
   if (lisEQP) {delete lisEQP; lisEQP = NULL;}
   if (lisLIG) {delete lisLIG; lisLIG = NULL;}
   if (lisMED) {delete lisMED; lisMED = NULL;}
   }

//---------------------------------------------------------------------------
bool __fastcall TZip::AssociaBarrasComSuasLigacoes(VTRedes *redes)
   {
   //variáveis locais
   #define   LIS_BARRA   ((TList*)barra->Obj)
   VTBarra   *barra;
   VTLigacao *ligacao;
   TList     *lisBAR = redes->LisBarra();

   //inicia lista de Ligacoes
   lisLIG->Clear();
   redes->LisLigacao(lisLIG);
   try{//inicia Tag de todas Barras com valor 0 e incia lista de Ligacoes da Barra
      for (int n = 0; n < lisBAR->Count; n++)
         {
         barra = (VTBarra*)lisBAR->Items[n];
         barra->Tag = 0;
         LIS_BARRA->Clear();
         }
      //atualiza Tag das Barras com número de Ligacoes que ela conecta
      for (int n = 0; n < lisLIG->Count; n++)
         {
         ligacao = (VTLigacao*)lisLIG->Items[n];
         for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
            {
            if ((barra = ligacao->Barra(ind_bar)) != NULL)
               {//incrementa Tag
               barra->Tag = barra->Tag + 1;
               //insere Ligacao na lista da Barra
               LIS_BARRA->Add(ligacao);
               }
            }
         }
      }catch(Exception &e)
         {
         return(false);
         }
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TZip::DeleteEqpto(TList *lisEXT)
   {
   //variáveis locais
   VTBarra     *barra;
   VTEqbar     *eqbar;
   VTEqpto     *eqpto;
   VTLigacao   *ligacao;
   VTRedes     *redes     = (VTRedes*)apl->GetObject(__classid(VTRedes));

   try{//elimina Eqptos em lisEXT
      for (int n = 0; n < lisEXT->Count; n++)
         {
         eqpto = (VTEqpto*)lisEXT->Items[n];
         if (eqpto->TipoBarra())
            {
            barra = (VTBarra*)eqpto;
            redes->RemoveBarra(barra);
            }
         else if (eqpto->TipoEqbar())
            {
            eqbar = (VTEqbar*)eqpto;
            eqbar->pbarra->RemoveEqbar(eqbar);
            //eqbar->pbarra = NULL;
            }
         else if (eqpto->TipoLigacao())
            {
            ligacao = (VTLigacao*)eqpto;
            ligacao->rede->RemoveLigacao(ligacao);
            //ligacao->rede = NULL;
            }
         }
      LimpaTList(lisEXT);
      }catch(Exception &e)
         {//nada a fazer
         }
   }

//---------------------------------------------------------------------------
bool __fastcall TZip::FundeTrechos(void)
   {
   //variáveis locais
   VTBarra     *barra;
   VTRede      *rede;
   VTRedes     *redes     = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList       *lisBAR    = redes->LisBarra();

   try{//reinicia lisDEL
      lisDEL->Clear();
      //cria uma lista para cada Barra
      for (int n = 0; n < lisBAR->Count; n++)
         {
         barra = (VTBarra*)lisBAR->Items[n];
         barra->Obj = new TList();
         }
      //funde os trechos de todas as redes
      FundeTrechosIntermediarios(redes);
      FundeTrechosTerminais(redes);
      //destrói lista associada a cada Barra
      for (int n = 0; n < lisBAR->Count; n++)
         {
         barra = (VTBarra*)lisBAR->Items[n];
         if (barra->Obj != NULL) {delete barra->Obj; barra->Obj = NULL;}
         }
      //elimina Eqptos em lisDEL
      DeleteEqpto(lisDEL);
      }catch(Exception &e)
         {
         }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TZip::FundeTrechosIntermediarios(VTRedes *redes)
   {
   //variáveis locais
   #define   LIS_BARRA   ((TList*)barra->Obj)
   VTBarra   *barra;
   VTLigacao *ligacao;
   VTRede    *rede;
   VTTrecho  *trecho1, *trecho2;
   TList     *lisBAR = redes->LisBarra();

   try{//associa Barras com as Ligacoes que as conectam
      AssociaBarrasComSuasLigacoes(redes);
      //inicia lista de Ligacoes
      lisLIG->Clear();
      redes->LisLigacao(lisLIG);
      //monta lista de Barras que atendem aos seguintes requisitos:
      //1) não possuem Eqbar
      //2) conectam 2 Trechos da mesma rede e de fases iguais
      lisEQP->Clear();
      for (int n = 0; n < lisBAR->Count; n++)
         {
         barra = (VTBarra*)lisBAR->Items[n];
         //verifica se Barra possui algum Eqbar
         if (barra->NumeroDeEqbar() > 0) continue;
         //verifica se a Barra conecta 2 Ligacoes
         else if (barra->Tag != 2) continue;
         //verifica se a primeira Ligacao é um trecho
         ligacao = (VTLigacao*)LIS_BARRA->Items[0];
         if (ligacao->Tipo() != eqptoTRECHO) continue;
         //determina primeiro Trecho
         trecho1 = (VTTrecho*)ligacao;
         //verifica se a segunda Ligacao é um trecho
         ligacao = (VTLigacao*)LIS_BARRA->Items[1];
         if (ligacao->Tipo() != eqptoTRECHO) continue;
         //determina segundo Trecho
         trecho2 = (VTTrecho*)ligacao;
         //proteção contra trechos de redes distintas
         if (trecho1->rede != trecho2->rede) continue;
         //proteção contra Trechos conectando as mesmas Barras
         if (trecho1->ConectaBarra(trecho2->pbarra1) &&
             trecho1->ConectaBarra(trecho2->pbarra2)) continue;
         //verifica se fases dos Trechos são iguais
         if (trecho1->arranjo->Fases == trecho2->arranjo->Fases)
            {//insere Barra em lisEQP
            lisEQP->Add(barra);
            }
         }
      //funde os dois Trechos de todas as Barras em lisEQP
      for (int n = 0; n < lisEQP->Count; n++)
         {
         barra   = (VTBarra*)lisEQP->Items[n];
         trecho1 = (VTTrecho*)LIS_BARRA->Items[0];
         trecho2 = (VTTrecho*)LIS_BARRA->Items[1];
         rede    = trecho1->rede;
         //incorpora trecho2 no trecho1
         FundeTrecho2NoTrecho1(trecho1, trecho2);
         //insere Barra e Trecho2 em lisDEL
         if (lisDEL->IndexOf(barra)   < 0) lisDEL->Add(barra);
         if (lisDEL->IndexOf(trecho2) < 0) lisDEL->Add(trecho2);
         //remove Ligacao de sua Rede
         rede->RemoveLigacao(trecho2);
         //remove barra de Redes
         redes->RemoveBarra(barra);
         //substitui trecho2 por trecho1 na Barra complementar do Trecho 2
         barra = (trecho2->pbarra1 == barra) ? trecho2->pbarra2 : trecho2->pbarra1;
         LIS_BARRA->Remove(trecho2);
         LIS_BARRA->Add(trecho1);
         }
      }catch(Exception &e)
         {
         return(false);
         }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TZip::FundeTrechosTerminais(VTRedes *redes)
   {
   //variáveis locais
   #define   LIS_BARRA   ((TList*)barra->Obj)
   bool      repita;
   VTBarra   *barra;
   VTLigacao *ligacao;
   VTRede    *rede;
   TList     *lisBAR = redes->LisBarra();

   try{//inicia lista de Ligacoes
      lisLIG->Clear();
      redes->LisLigacao(lisLIG);
      do{//repita até eliminar todos Trechos terminais
         repita = false;
         //associa Barras com as Ligacoes que as conectam
         AssociaBarrasComSuasLigacoes(redes);
         //elimina Barras que atendem aos seguintes requisitos:
         //1) não possuem Eqbar
         //2) conectam apenas 1 Trecho
         for (int n = lisBAR->Count-1; n >= 0; n--)
            {
            barra = (VTBarra*)lisBAR->Items[n];
            //verifica se Barra possui algum Eqbar
            if (barra->NumeroDeEqbar() > 0) continue;
            //verifica se a Barra é conectada por uma única Ligacao
            if (barra->Tag == 1)
               {//verifica se é um Trecho
               ligacao = (VTLigacao*)LIS_BARRA->Items[0];
               if (ligacao->Tipo() == eqptoTRECHO)
                  {//insere Barra e Ligacao (Trecho) em lisDEL
                  if (lisDEL->IndexOf(barra)   < 0) lisDEL->Add(barra);
                  if (lisDEL->IndexOf(ligacao) < 0) lisDEL->Add(ligacao);
                  //remove Ligacao da sua Rede
                  rede = ligacao->rede;
                  rede->RemoveLigacao(ligacao);
                  //remove Barra da Rede e de Redes
                  redes->RemoveBarra(barra);
                  //retira trecho da lista de ligações da Barra
                  LIS_BARRA->Remove(ligacao);
                  //retira trecho da lista de ligações da segunda Barra
                  if      (ligacao->pbarra1 == barra) barra = ligacao->pbarra2;
                  else if (ligacao->pbarra2 == barra) barra = ligacao->pbarra1;
                  LIS_BARRA->Remove(ligacao);
                  //seta indicação para nova iteração para remover ligações termnais
                  repita = true;
                  }
               }
            }
         }while(repita);
      }catch(Exception &e)
         {
         return(false);
         }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TZip::FundeTrecho2NoTrecho1(VTTrecho *trecho1, VTTrecho *trecho2)
   {
   //variáveis locais
   double    comp_km, comp1_km, comp2_km;
   double    r0_pu, x0_pu, yc0_pu;
   double    r1_pu, x1_pu, yc1_pu;
   VTArranjo *arranjo;
   VTBarra   *barra1, *barra2, *barra;

   try{//determina Barra comum
      if ((barra = trecho1->BarraComum(trecho2)) == NULL)  return(false);
      //determina barra1: Barra do trecho1 diferente da barra comum
      if      (trecho1->pbarra1 == barra) barra1 = trecho1->pbarra2;
      else if (trecho1->pbarra2 == barra) barra1 = trecho1->pbarra1;
      //determina barra2: Barra do trecho2 diferente da barra comum
      if      (trecho2->pbarra1 == barra) barra2 = trecho2->pbarra2;
      else if (trecho2->pbarra2 == barra) barra2 = trecho2->pbarra1;
      //redefine Barra do trecho1
      if      (trecho1->pbarra1 == barra) trecho1->pbarra1 = barra2;
      else if (trecho1->pbarra2 == barra) trecho1->pbarra2 = barra2;
      //verifica se Arranjo do trecho1 é típico
      arranjo = trecho1->arranjo;
      if (trecho1->ArranjoTipico)
         {//cria um Arranjo próprio para o trecho1
         arranjo = trecho1->arranjo->Clone();
         arranjo->Owner    = NULL;
         arranjo->suporte  = NULL;
         arranjo->caboZ0Z1 = NULL;
         //redefine Arranjo do trecho1
         trecho1->DefineObjArranjo(arranjo);
         }
      //redefine tipo do Arranjo
      arranjo->TipoCfg  = arranjoMAT_Z;
//TESTES: define arranjo do tipo Z0Z1
//arranjo->TipoCfg  = arranjoZ0Z1;
      //determina comprimentos dos trechos
      comp1_km = trecho1->Comprimento_km;
      comp2_km = trecho2->Comprimento_km;
      comp_km  = comp1_km + comp2_km;
      //atualiza impedâncias de sequência do Arranjo
      arranjo->z0.r = ((trecho1->arranjo->z0.r * comp1_km)  +
                       (trecho2->arranjo->z0.r * comp2_km)) / comp_km;
      arranjo->z0.x = ((trecho1->arranjo->z0.x * comp1_km)  + 
                       (trecho2->arranjo->z0.x * comp2_km)) / comp_km;
      arranjo->z1.r = ((trecho1->arranjo->z1.r * comp1_km)  + 
                       (trecho2->arranjo->z1.r * comp2_km)) / comp_km;
      arranjo->z1.x = ((trecho1->arranjo->z1.x * comp1_km)  + 
                       (trecho2->arranjo->z1.x * comp2_km)) / comp_km;
//TESTES: assume valores default
//arranjo->z0.r = 0.4;
//arranjo->z0.x = 0.8;
//arranjo->z1.r = 0.2;
//arranjo->z1.x = 0.4;
      //atualiza MatC do Arranjo
      for (int i = 0; i < arranjo->MatC->Dimensao; i++)
         {
         for (int j = 0; j < arranjo->MatC->Dimensao; j++)
            {
            arranjo->MatC->Cap[i][j] = ((trecho1->arranjo->MatC->Cap[i][j] * comp1_km)  +
                                                 (trecho2->arranjo->MatC->Cap[i][j] * comp2_km)) / comp_km;
            }
         }
      //atualiza MatZ
      for (int i = 0; i < arranjo->MatZ->Dimensao; i++)
         {
         for (int j = 0; j < arranjo->MatZ->Dimensao; j++)
            {
            arranjo->MatZ->Z[i][j] = ((trecho1->arranjo->MatZ->Z[i][j] * comp1_km)  +
                                               (trecho2->arranjo->MatZ->Z[i][j] * comp2_km)) / comp_km;
            }
         }
      //redefine comprimento do trecho1
//TESTES: assume comprimento mínimo
//if (comp_km < 0.100) comp_km = 0.100;
      trecho1->Comprimento_km = comp_km;
      }catch(Exception &e)
         {
         return(false);
         }
   return(true);
   }

//---------------------------------------------------------------------------
int __fastcall TZip::LisBarraInicialRede(VTRedes *redes, TList *lisEXT)
   {
   //variáveis locais
   VTBarra *barra;
   VTRede  *rede;
   TList   *lisREDE   = redes->LisRede();

   //monta lista com todas as Barras iniciais das redes
   lisEXT->Clear();
   //loop p/ todas Redes
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      if ((barra = rede->BarraInicial()) == NULL) continue;
      if (lisEXT->IndexOf(barra) < 0) {lisEXT->Add(barra);}
      }
   return(lisEXT->Count);
   }

//---------------------------------------------------------------------------
int __fastcall TZip::LisChaveComMedicao(VTRede *rede, TList *lisEXT)
   {
   //variáveis locais
   VTEqpto   *eqpto;
   VTMedidor *medidor;

   //obtém lista de Medidores da rede
   lisMED->Clear();
   rede->LisEqbar(lisMED, eqptoMEDIDOR);
   for (int n = 0; n < lisMED->Count; n++)
      {
      medidor = (VTMedidor*)lisMED->Items[n];
      //obtém lista de Eqptos associados aos Canais
      lisEQP->Clear();
      medidor->LisEqptoDosCanais(lisEQP);
      //loop p/ todos os Eqptos associados aos Canais
      for (int i = 0; i < lisEQP->Count; i++)
         {
         eqpto = (VTEqpto*)lisEQP->Items[i];
         if (eqpto->Tipo() == eqptoCHAVE)
            {//inclui Chave em lisEXT
            lisEXT->Add(eqpto);
            }
         }
      }
   return(lisEXT->Count);
   }

//---------------------------------------------------------------------------
int __fastcall TZip::LisChaveSemMedicao(VTRedes *redes, TList *lisEXT)
   {
   //variáveis locais
   VTEqpto   *eqpto;
   VTMedidor *medidor;

   //obtém lista de todas chaves da rede
   lisEXT->Clear();
   redes->LisLigacao(lisEXT, eqptoCHAVE);
   //obtém lista de Medidores da rede
   lisMED->Clear();
   redes->LisEqbar(lisMED, eqptoMEDIDOR);
   for (int n = 0; n < lisMED->Count; n++)
      {
      medidor = (VTMedidor*)lisMED->Items[n];
      //obtém lista de Eqptos associados aos Canais
      lisEQP->Clear();
      medidor->LisEqptoDosCanais(lisEQP);
      //loop p/ todos os Eqptos associados aos Canais
      for (int i = 0; i < lisEQP->Count; i++)
         {
         eqpto = (VTEqpto*)lisEQP->Items[i];
         if (eqpto->Tipo() == eqptoCHAVE)
            {//exclui Chave de lisEXT
            lisEXT->Remove(eqpto);
            }
         }
      }
   return(lisEXT->Count);
   }

//---------------------------------------------------------------------------
bool __fastcall TZip::RetiraChavesExcetoDJinicial(void)
   {
   //variáveis locais
   VTRede      *rede;
   //VTEdita     *edita     = (VTEdita*)apl->GetObject(__classid(VTEdita));
   VTRedes     *redes     = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList       *lisREDE   = redes->LisRede();

   //reinicia lisDEL
   lisDEL->Clear();
   //loop para todas redes
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      if (! rede->Carregada) continue;
      if (rede->TipoRede->Segmento == redePRI)
         {
         RetiraChavesExcetoDJinicial(rede);
         }
      }
   //elimina Eqptos em lisDEL
   //if (lisDEL->Count > 0)
   //   {
   //   edita->RetiraLisEqpto(lisDEL);
   //   lisDEL->Clear();
   //   }
   DeleteEqpto(lisDEL);
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TZip::RetiraChavesExcetoDJinicial(VTRede *rede)
   {
   //variáveis locais
   VTBarra   *barra1, *barra2;
   VTChave   *chave;
   VTLigacao *ligacao;
   TList     *lisLIG = rede->LisLigacao();

   //obtém lista de chaves da rede
   lisCHV->Clear();
   rede->LisLigacao(lisCHV, eqptoCHAVE);
   //retira todas as Chaves da Rede
   for (int nc = 0; nc < lisCHV->Count; nc++)
      {
      chave = (VTChave*)lisCHV->Items[nc];
      //não retira o disjuntor que conecta a Barra inicial da Rede
      if (chave->TipoDisjuntor)
         {
         if (chave->ConectaBarra(rede->BarraInicial())) continue;
         }
      //insere Chave em lisDEL
      lisDEL->Add(chave);
      //verifica se a Chave está aberta
      if (chave->Aberta) continue;
      //determina Barras da chave
      barra1 = chave->pbarra1;
      barra2 = chave->pbarra2;
      //redefine as Barras das Ligacoes que conectam a barra1
      for (int nl = 0; nl < lisLIG->Count; nl++)
         {
         ligacao = (VTLigacao*)lisLIG->Items[nl];
         //proteção
         if (ligacao == chave) continue;
         //verifica se a Ligacao conecta a Barra
         if (ligacao->ConectaBarra(barra1))
            {//troca Barra da Ligacao
            if      (ligacao->pbarra1 == barra1) ligacao->pbarra1 = barra2;
            else if (ligacao->pbarra2 == barra1) ligacao->pbarra2 = barra2;
            else if (ligacao->pbarra3 == barra1) ligacao->pbarra3 = barra2;
            }
         }
      //insere Barra1 em lisDEL
      if (lisDEL->IndexOf(barra1) < 0) lisDEL->Add(barra1);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TZip::RetiraChavesSemMedicao(void)
   {
   //variáveis locais
   VTBarra     *barra;
   VTRedes     *redes     = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList       *lisBAR    = redes->LisBarra();

   try{//reinicia lisDEL
      lisDEL->Clear();
      //cria uma lista para cada Barra
      for (int n = 0; n < lisBAR->Count; n++)
         {
         barra = (VTBarra*)lisBAR->Items[n];
         barra->Obj = new TList();
         }
      //retira as Chaves sem medição de todas as redes
      RetiraChavesSemMedicao(redes);
      //destrói lista associada a cada Barra
      for (int n = 0; n < lisBAR->Count; n++)
         {
         barra = (VTBarra*)lisBAR->Items[n];
         if (barra->Obj != NULL) {delete barra->Obj; barra->Obj = NULL;}
         }
      //elimina Eqptos em lisDEL
      DeleteEqpto(lisDEL);
      }catch(Exception &e)
         {
         }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TZip::RetiraChavesSemMedicao(VTRedes *redes)
   {
   //variáveis locais
   TList       *lisLIG_BAR1, *lisLIG_BAR2;
   VTBarra     *barra, *barra_del;
   VTChave     *chave;
   VTLigacao   *ligacao;
   VTRede      *rede;

   //associa Barras com as Ligacoes que as conectam
   AssociaBarrasComSuasLigacoes(redes);
   //monta lista com Barras iniciais das redes
   LisBarraInicialRede(redes, lisEQP);
   //monta lista com Chaves sem Medicao
   LisChaveSemMedicao(redes, lisCHV);
   //retira Chaves que estão em lisCHV, reconectando as Ligacoes vizinhas
   for (int nc = 0; nc < lisCHV->Count; nc++)
      {
      chave = (VTChave*)lisCHV->Items[nc];
      //verifica se a Chave está aberta
      if (chave->Aberta)
         {//insere Chave em lisDEL
         lisDEL->Add(chave);
         //retira Chave de sua rede
         chave->rede->RemoveLigacao(chave);
         //determina listas de Ligaçoes das duas Barras da Chave
         lisLIG_BAR1 = (TList*)chave->pbarra1->Obj;
         lisLIG_BAR2 = (TList*)chave->pbarra2->Obj;
         //remove a Chave das listas de Ligacoes das duas Barras
         lisLIG_BAR1->Remove(chave);
         lisLIG_BAR2->Remove(chave);
         continue;
         }
      //determina Barra da chave que será eliminada: que não tem Eqbar e que não é barra inicial de rede
      if ((chave->pbarra1->NumeroDeEqbar() == 0)&& (lisEQP->IndexOf(chave->pbarra1) < 0))
         {
         barra_del = chave->pbarra1;
         barra     = chave->pbarra2;
         }
      else if ((chave->pbarra2->NumeroDeEqbar() == 0) && (lisEQP->IndexOf(chave->pbarra2) < 0))
         {
         barra_del = chave->pbarra2;
         barra     = chave->pbarra1;
         }
      else
         {continue;}
      //retira Chave de sua Rede
      chave->rede->RemoveLigacao(chave);
      //determina listas de Ligaçoes das duas Barras da Chave
      lisLIG_BAR1 = (TList*)(barra->Obj);
      lisLIG_BAR2 = (TList*)(barra_del->Obj);
      //remove a Chave das listas de Ligacoes das duas Barras
      lisLIG_BAR1->Remove(chave);
      lisLIG_BAR2->Remove(chave);
      //loop para todas Ligacoes da Barra que será eliminada
      for (int nl = 0; nl < lisLIG_BAR2->Count; nl++)
         {
         ligacao = (VTLigacao*)lisLIG_BAR2->Items[nl];
         //troca Barra da Ligacao
         if      (ligacao->pbarra1 == barra_del) ligacao->pbarra1 = barra;
         else if (ligacao->pbarra2 == barra_del) ligacao->pbarra2 = barra;
         else if (ligacao->pbarra3 == barra_del) ligacao->pbarra3 = barra;
         //insere Ligacao na lista de Ligações da Barra que permanece
         lisLIG_BAR1->Add(ligacao);
         }
      //insere Chave em lisDEL
      if (lisDEL->IndexOf(chave) < 0) lisDEL->Add(chave);
      //insere Barra em lisDEL
      if (lisDEL->IndexOf(barra_del) < 0) lisDEL->Add(barra_del);
      }
   return(true);
   }

//---------------------------------------------------------------------------
//eof

