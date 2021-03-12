//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TMThread.h"
#include "VTFlowThread.h"
#include "..\..\Apl\VTApl.h"
#include "..\..\Rede\VTMNet.h"
#include "..\..\Rede\VTNet.h"
#include "..\..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTMThread* NewObjMThread(VTApl* apl_owner)
   {
   try{
      return(new TMThread(apl_owner));
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TMThread::TMThread(VTApl* apl_owner)
   {
	//cria objeto Apl local
   apl = NewObjApl(NULL, apl_owner);
   //cria objeto TCriticalSection e insere em Apl local
   apl->Add(g_cs = new TCriticalSection());
   //cria listas
   lisEVENT_ALL = new TList();
   lisEVENT_DIS = new TList();
   lisEVENT_USO = new TList();
   lisMNET_CALC = new TList();
   //cria objetos FlowThreadEvent
   CriaObjetosEvent();
   }

//---------------------------------------------------------------------------
__fastcall TMThread::~TMThread(void)
   {
   //destrói vetor de ponteiros alocado
   if (vet_handle) {delete[] vet_handle; vet_handle = NULL;}
   //destrói listas sem destruir seus objetos
   if (lisEVENT_DIS) {delete lisEVENT_DIS; lisEVENT_DIS = NULL;}
   if (lisEVENT_USO) {delete lisEVENT_USO; lisEVENT_USO = NULL;}
   if (lisMNET_CALC) {delete lisMNET_CALC; lisMNET_CALC = NULL;}
   //destrói listas e seus objetos
   if (lisEVENT_ALL) {LimpaTList(lisEVENT_ALL); delete lisEVENT_ALL; lisEVENT_ALL = NULL;}
   //destrói objetos criados
   if (apl) {delete apl; apl = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TMThread::CriaObjetosEvent(void)
   {
   //variáveis locais
   SYSTEM_INFO        sysInfo;
   VTFlowThreadEvent *event;

   //reinicia listas
   lisEVENT_DIS->Clear();
   lisEVENT_USO->Clear();
   LimpaTList(lisEVENT_ALL);
   //Controle das threads
   GetSystemInfo(&sysInfo);
   //obtém número de processadores disponíveis
   intThrMax = sysInfo.dwNumberOfProcessors;
   // O construtor da classe garante a iniciação correta dos objetos
   vet_handle = new HANDLE[intThrMax];
   //cria um objeto VTFlowThreadEvent para cada processador disponível
   for (int i = 0; i < intThrMax; i++)
      {//o construtor da classe garante a iniciação correta dos objetos
      event = NewObjFlowThreadEvent();
      //insere objeto em lisEVENT_ALL e em lisEVENT_DIS
      lisEVENT_ALL->Add(event);
      lisEVENT_DIS->Add(event);
      //copia o handle existente em VTFlowThreadEvent p/ o vetor de handles
      vet_handle[i] = event->Handle;
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TMThread::ExecutaFluxoPorThread(VTMNet *mnet)
   {
   //variáveis locais
   VTFlowThreadEvent *event;

   //verifica se há um objeto VTFlowThreadEvent livre
   while (lisEVENT_DIS->Count == 0)
      {//aguarda término de uma Thread, sinalizado pelo handle do VTFlowThreadEvent
      WaitForMultipleObjects(intThrMax, vet_handle, FALSE, INFINITE);
      //identifica VTFlowThreadEvent liberados
      VerificaObjetosEventLiberados();
      }
   //bloqueia acesso concorrente
   g_cs->Acquire();
   //seleciona primeiro objeto VTFlowThreadEvent disponível
   event = (VTFlowThreadEvent*)lisEVENT_DIS->First();
   //transfere o objeto VTFlowThreadEvent de lisEVENT_USO para lisEVENT_DIS
   lisEVENT_DIS->Delete(0);
   lisEVENT_USO->Add(event);
   //atualiza estado de cálculo da MNet e associa ao objeto VTFlowThreadEvent
   mnet->Net->StatusFluxo = fluxoCALCULANDO;
   event->MNet = mnet;
   //desbloqueia acesso concorrente
   g_cs->Release();
   //cria e excuta objeto VTThreadFlow
   NewObjFlowThread(apl, false, event);

   return(true);
   }

//---------------------------------------------------------------------------
int  __fastcall TMThread::LisMNetCalculada(TList *lisEXT, bool clear)
   {
   //variáveis locais
   int count;

   //verifica se há MNets calculadas em lisMNET_CALC
   if ((lisMNET_CALC->Count == 0) && (lisEVENT_DIS->Count == 0))
      {//aguarda término de uma Thread, sinalizado pelo handle do VTFlowThreadEvent
      WaitForMultipleObjects(intThrMax, vet_handle, FALSE, INFINITE);
      }
   //identifica VTFlowThreadEvent liberados
   VerificaObjetosEventLiberados();
   //copia lisMNET_CALC para lisEXT
   if ((count = lisMNET_CALC->Count) > 0)
      {
      CopiaTList(lisMNET_CALC, lisEXT);
      //verifica se deve limpar lisMNET_CALC
      if (clear) lisMNET_CALC->Clear();
      }
   return(count);
   }

//---------------------------------------------------------------------------
int  __fastcall TMThread::NumeroThreadAtiva(void)
   {
   return(lisEVENT_USO->Count);
   }

//---------------------------------------------------------------------------
void __fastcall TMThread::VerificaObjetosEventLiberados(void)
   {
   //variáveis locais
   VTFlowThreadEvent *event;

   //bloqueia acesso concorrente
   g_cs->Acquire();
   //loop para todos objetos VTFlowThreadEvent em lisEVENT_USO
   for (int n = lisEVENT_USO->Count - 1; n >= 0; n--)
      {
      event = (VTFlowThreadEvent*)lisEVENT_USO->Items[n];
      //verifica o status de cálculo de fluxo da Net associada
      if (event->MNet->Net->StatusFluxo == fluxoCALCULANDO) continue;
      //salva MNet associado em lisMNET_CALC
      lisMNET_CALC->Add(event->MNet);
      //cancela MNet associada ao objeto
      event->MNet = NULL;
      ResetEvent(event->Handle);
      //transfere o objeto VTFlowThreadEvent de lisEVENT_USO para lisEVENT_DIS
      lisEVENT_USO->Delete(n);
      lisEVENT_DIS->Add(event);
      }
   //desbloqueia acesso concorrente
   g_cs->Release();
   }

//---------------------------------------------------------------------------
//eof

