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
//fun��o global p/ criar objeto VTFlowThreadEvent
VTFlowThreadEvent* NewObjFlowThreadEvent(void)
   {
   return(new TFlowThreadEvent());
   }

//---------------------------------------------------------------------------
__fastcall TFlowThreadEvent::TFlowThreadEvent(void)
   {
   PD.mnet   = NULL;
   PD.handle = CreateEvent(NULL,  // Atributos de seguran�a padr�o
                           FALSE, // N�o reseta automaticamente o evento
                           FALSE, // Estado inicial do evento � inativo (non-signaled)
                           NULL   // N�o foi criado nome para o obejto do sistema
                          );
   }

//---------------------------------------------------------------------------
__fastcall TFlowThreadEvent::~TFlowThreadEvent(void)
   {
   // O n� simplesmente perde a associa��o
   PD.mnet = NULL;
   // O Handle � fechado automaticamente ao t�rmino do processo, mas melhor fechar antes
   // N�o se deve deletar o PD.handle, por�m.
   CloseHandle(PD.handle);
   PD.handle = NULL;
   }

//---------------------------------------------------------------------------
//fun��o global p/ criar objeto VTFlowThread
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
   // O estado inicial � n�o sinalizado. Para verificar se o c�lculo
   // foi executado, deve ser verificado o estado do n�.
   ResetEvent(this->thrEvent->Handle);
   //cria objetos locais
   apl->Add(plog  = DLL_NewObjLog(apl));
   apl->Add(DLL_NewObjDemanda(apl));
   apl->Add(DLL_NewObjFornecimento(apl));
   //inicia dados
   aviso_enabled     = false;

   // Assim que acabar os c�lculos, a thread morre automaticamente.
   // Verificar se n�o vai dar zebra depois.
   // OOPS: When FreeOnTerminate is true, the Execute method may run and then free
   // the thread before your application can execute the next line of code. Thus,
   // you should not call any methods of the thread object when FreeOnTerminate is
   // true unless you create the thread in a suspended state.
   FreeOnTerminate = true;
   }

//---------------------------------------------------------------------------
__fastcall TFlowThread::~TFlowThread(void)
   {
   //destr�i objetos criados
   if (apl) {delete apl; apl = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFlowThread::Execute()
   {
   VTMNet* mnet;
   TList* lstOBJ;

   // Ao come�ar, sinaliza
   ResetEvent(thrEvent->Handle);
   //bloqueia acesso � regi�o cr�tica e obt�m MNet associada ao objeto thrEvent
   if (g_cs) g_cs->Acquire();
   try
      {
      //thrEvent->Node->LisObjectNode(lstOBJ);
      mnet = thrEvent->MNet;
      //atualiza status de c�lculo de fluxo da Net
      mnet->Net->StatusFluxo = fluxoCALCULANDO;
      }
   __finally
      {//desbloqueia acesso � regi�o cr�tica
      if (g_cs) g_cs->Release();
      }
   //executa c�lculo da MNet
   mnet->Enabled = ExecutaCalculoMNet(mnet);
   //bloqueia acesso � regi�o cr�tica e obt�m MNet associada ao objeto thrEvent
   if (g_cs) g_cs->Acquire();
   try
      {
      //atualiza status de Net, indicando fim do c�lculo
      mnet->Net->StatusFluxo = fluxoCALCULADO;
      }
   __finally
      {//desbloqueia acesso � regi�o cr�tica
      if (g_cs) g_cs->Release();
      }
   //indica que a thread e o c�lculo foram fiznalizados
   SetEvent(thrEvent->Handle);
   }

//---------------------------------------------------------------------------
bool __fastcall TFlowThread::ExecutaCalculoMNet(VTMNet *mnet)
   {
   //vari�veis locais
   bool       sucesso;
   VTFlowNet  *flow_net;

	try{//cria objeto VTFlowNet para c�lculo de fluxo da MNet
		if ((flow_net = NewObjFlowNet(apl)) == NULL) return(false);
		//calcula Fluxo de Pot�ncia p/ VTNet
		sucesso = flow_net->Executa(mnet);
		}catch(Exception &e)
			{
			sucesso = false;
         }
	//destr�i objeto VTFlowNet
   if (flow_net) delete flow_net;
   return(sucesso);
   }

//---------------------------------------------------------------------------
//eof

