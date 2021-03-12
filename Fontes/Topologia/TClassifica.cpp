//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TClassifica.h"
#include "VTRecorta.h"
#include "VTMalha.h"
#include "..\Apl\VTapl.h"
#include "..\Rede\VTCluster.h"
#include "..\Rede\VTCorte.h"
#include "..\Rede\VTMRede.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTSuprimento.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Rede.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTClassifica
//---------------------------------------------------------------------------
VTClassifica* __fastcall NewObjClassifica(VTApl *apl)
   {
   return(new TClassifica(apl));
   }

//---------------------------------------------------------------------------
__fastcall TClassifica::TClassifica(VTApl *apl)
   {
   //salva ponteiro p/ objetos
   this->apl = apl;
   //cria objetos
   recorta    = NewObjRecorta(apl);
   malha      = NewObjMalha();
   lisMR      = new TList();
   lisMRC     = new TList();
   lisMRR     = new TList();
   lisMR_CAR  = new TList();
   lisMR_GER  = new TList();
   lisTMP     = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TClassifica::~TClassifica(void)
   {
   //destrói objetos criados
   if (recorta)    {delete recorta;  recorta = NULL;}
   if (malha)      {delete malha;    malha   = NULL;}
   //destrói lista sem destruir seus objetos
   if (lisMR    )  {delete lisMR    ;  lisMR     = NULL;}
   if (lisMRC   )  {delete lisMRC   ;  lisMRC    = NULL;}
   if (lisMRR   )  {delete lisMRR   ;  lisMRR    = NULL;}
   if (lisMR_CAR)  {delete lisMR_CAR;  lisMR_CAR = NULL;}
   if (lisMR_GER)  {delete lisMR_GER;  lisMR_GER = NULL;}
   if (lisTMP   )  {delete lisTMP   ;  lisTMP    = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TClassifica::CancelaMRedeDeRede(void)
   {
   //variáveis locais
   VTRede *rede;
   TList  *lisREDE = redes->LisRede();

   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      if (rede->Carregada) rede->MRede = NULL;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TClassifica::CriaMRedesCarregadas(void)
   {
   //variáveis locais
   VTCluster *cluster;
   VTRede    *rede;
   VTMRede   *mrede;
   TList     *lisREDE;
   TList     *lisCLUSTER;

   //reinicia lisMRC
   lisMRC->Clear();
   //cria uma MRede p/ cada Rede carregada
   lisREDE = redes->LisRede();
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      //verifica se a Rede está carregada
      if (! rede->Carregada) continue;
      //verifica se a Rede já está em uma MRede
      if (rede->MRede != NULL) continue;
      //cria um objeto MRede
      if ((mrede = DLL_NewObjMRede()) == NULL) continue;
      //verifica se a Rede pertence a um Cluster
      if (rede->Cluster)
         {//insere todas Redes do Cluster na MRede
         mrede->InsereRede(rede->Cluster->LisRede());
         }
        else
         {//insere Rede na MRede
         mrede->InsereRede(rede);
         }
      //atualiza estado (Carregada) e insere objeto MRede em lisMRC
      mrede->Carregada = true;
      lisMRC->Add(mrede);
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TClassifica::Executa(VTRedes *redes)
   {
   //salva ponteiro p/ objetos
   this->redes   = redes;
   //limpa listas locais
   LimpaListas();
   //cancela associação das Redes carregadas c/ as MRedes
   CancelaMRedeDeRede();
   //reinicia Cortes e Reducoes das Redes
   if (! recorta->Executa(redes)) return(false);
   //obtém lista de MRedes reduzidas
   lisMRR->Clear();
   redes->LisMRedeReduzida(lisMRR);
   //cria objetos MRede: 1 MRede p/ cada Cluster e 1 MRede p/ demais Redes carregadas
   CriaMRedesCarregadas();
   //inicia lisMR c/ MRedes reduzidas e MRedes carregadas
   lisMR->Clear();
   CopiaTList(lisMRR, lisMR);
   CopiaTList(lisMRC, lisMR);
   //verifica se há Clusters
   if (redes->LisCluster()->Count > 0)
      {//MRede criada p/ Clusters pode permitir a eliminação de Cortes (que ficaram com apenas 1 MRede)
      recorta->VerificaExclusaoDeCortes();
      //MRede criada p/ Clusters pode permitir a fusão de Cortes
      recorta->VerificaFusaoDeCortes(lisMR);
      }
   //identifica MRedes com geração própria
   IdentificaMRedesComGeracaoPropria();
   //identifica MRedes em malha
   FundeMRedesEmMalha();
   //atualiza MRedes do objeto VTRedes
   redes->ReiniciaLisMRede(lisMR);
   //limpa listas que não são necessárias
   LimpaListas();
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TClassifica::FundeMRedesEmMalha(void)
   {
   //funde grupos de MRede em malha em uma única MRede
   while (malha->Executa(lisMR) != NULL)
      {//a fusão de MRedes em Malha pode permitir a eliminação de Cortes (que ficaram com apenas 1 MRede)
      recorta->VerificaExclusaoDeCortes();
      //a fusão de MRedes em Malha pode permitir a fusão de Cortes entre MRedes
      recorta->VerificaFusaoDeCortes(lisMR);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TClassifica::IdentificaMRedesComGeracaoPropria(void)
   {
   //variáveis locais
   VTMRede *mrede;

   //loop p/ toda MRede em lisMR
   for (int n = lisMR->Count-1; n >= 0 ; n--)
      {
      mrede = (VTMRede*)lisMR->Items[n];
      //verifica se há Gerador/Suprimento na MRede
      if (mrede->Geracao)
         {//inser MRede em lisMR_GER
         lisMR_GER->Add(mrede);
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TClassifica::LimpaListas(void)
   {
   lisMR->Clear();
   lisMRC->Clear();
   lisMRR->Clear();
   lisMR_CAR->Clear();
   lisMR_GER->Clear();
   }

//---------------------------------------------------------------------------
//eof


