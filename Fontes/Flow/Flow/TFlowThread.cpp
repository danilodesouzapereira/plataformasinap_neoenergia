//---------------------------------------------------------------------------
#include <vcl.h>
#include <SyncObjs.hpp>
#pragma hdrstop
#include "TFlowThread.h"
#include "VTFlowNet.h"
#include "..\VTFlow.h"
#include "..\..\Apl\VTApl.h"
#include "..\..\Log\VTLog.h"
#include "..\..\Potencia\VTDemanda.h"
#include "..\..\Potencia\VTFornecimento.h"
#include "..\..\Rede\VTMNet.h"
#include "..\..\Rede\VTNet.h"
#include "..\..\Rede\VTMRede.h"
#include "..\..\Rede\VTRede.h"
#include "..\..\Rede\VTRedes.h"
#include "..\..\..\DLL_Inc\Log.h"
#include "..\..\..\DLL_Inc\Funcao.h"
#include "..\..\..\DLL_Inc\Potencia.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ criar objeto VTFlowThreadEvent
VTFlowThreadEvent* NewObjFlowThreadEvent(void)
   {
   return(new TFlowThreadEvent());
   }

//---------------------------------------------------------------------------
__fastcall TFlowThreadEvent::TFlowThreadEvent(void)
   {
   PD.mnet   = NULL;
   PD.handle = CreateEvent(NULL,  // Atributos de segurança padrão
                           FALSE, // Não reseta automaticamente o evento
                           FALSE, // Estado inicial do evento é inativo (non-signaled)
                           NULL   // Não foi criado nome para o obejto do sistema
                          );
   }

//---------------------------------------------------------------------------
__fastcall TFlowThreadEvent::~TFlowThreadEvent(void)
   {
   // O nó simplesmente perde a associação
   PD.mnet = NULL;
   // O Handle é fechado automaticamente ao término do processo, mas melhor fechar antes
   // Não se deve deletar o PD.handle, porém.
   CloseHandle(PD.handle);
   PD.handle = NULL;
   }

//---------------------------------------------------------------------------
//função global p/ criar objeto VTFlowThread
VTFlowThread* NewObjFlowThread(VTApl* apl, bool bolCreateSuspended, VTFlowThreadEvent* thrEvent)
   {
   return(new TFlowThread(apl,bolCreateSuspended, thrEvent));
   }

//---------------------------------------------------------------------------
__fastcall TFlowThread::TFlowThread(VTApl* apl_owner, bool bolCreateSuspended, VTFlowThreadEvent* thrEvent) : VTFlowThread(bolCreateSuspended)
   {
	//cria objeto Apl local
   apl = NewObjApl(NULL, apl_owner);
   //salva ponteiro p/ objetos externos
   this->thrEvent = thrEvent;
   g_cs           = (TCriticalSection*)apl->GetObject(__classid(TCriticalSection));
   // O estado inicial é não sinalizado. Para verificar se o cálculo
   // foi executado, deve ser verificado o estado do nó.
   ResetEvent(this->thrEvent->Handle);
   //cria objetos locais
   apl->Add(plog  = DLL_NewObjLog(apl));
   apl->Add(DLL_NewObjDemanda(apl));
   apl->Add(DLL_NewObjFornecimento(apl));
   //inicia dados
   aviso_enabled     = false;

   // Assim que acabar os cálculos, a thread morre automaticamente.
   // Verificar se não vai dar zebra depois.
   // OOPS: When FreeOnTerminate is true, the Execute method may run and then free
   // the thread before your application can execute the next line of code. Thus,
   // you should not call any methods of the thread object when FreeOnTerminate is
   // true unless you create the thread in a suspended state.
   FreeOnTerminate = true;
   }

//---------------------------------------------------------------------------
__fastcall TFlowThread::~TFlowThread(void)
   {
   //destrói objetos criados
   if (apl) {delete apl; apl = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFlowThread::Execute()
   {
   VTMNet* mnet;
   TList* lstOBJ;

   // Ao começar, sinaliza
   ResetEvent(thrEvent->Handle);
   //bloqueia acesso à região crítica e obtém MNet associada ao objeto thrEvent
   if (g_cs) g_cs->Acquire();
   try
      {
      //thrEvent->Node->LisObjectNode(lstOBJ);
      mnet = thrEvent->MNet;
      //atualiza status de cálculo de fluxo da Net
      mnet->Net->StatusFluxo = fluxoCALCULANDO;
      }
   __finally
      {//desbloqueia acesso à região crítica
      if (g_cs) g_cs->Release();
      }
   //executa cálculo da MNet
   mnet->Enabled = ExecutaCalculoMNet(mnet);
   //bloqueia acesso à região crítica e obtém MNet associada ao objeto thrEvent
   if (g_cs) g_cs->Acquire();
   try
      {
      //atualiza status de Net, indicando fim do cálculo
      mnet->Net->StatusFluxo = fluxoCALCULADO;
      }
   __finally
      {//desbloqueia acesso à região crítica
      if (g_cs) g_cs->Release();
      }
   //indica que a thread e o cálculo foram fiznalizados
   SetEvent(thrEvent->Handle);
   }

//---------------------------------------------------------------------------
bool __fastcall TFlowThread::ExecutaCalculoMNet(VTMNet *mnet)
   {
   //variáveis locais
   bool       sucesso;
   VTFlowNet  *flow_net;

	try{//cria objeto VTFlowNet para cálculo de fluxo da MNet
		if ((flow_net = NewObjFlowNet(apl)) == NULL) return(false);
		//calcula Fluxo de Potência p/ VTNet
		sucesso = flow_net->Executa(mnet);
		}catch(Exception &e)
			{
			sucesso = false;
         }
	//destrói objeto VTFlowNet
   if (flow_net) delete flow_net;
   return(sucesso);
   }

//---------------------------------------------------------------------------
//eof

