//---------------------------------------------------------------------------
#ifndef TFlowThreadH
#define TFlowThreadH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <SyncObjs.hpp>
#include "VTFlowThread.h"

//---------------------------------------------------------------------------
class VTApl;
class VTLog;
class VTMNet;
class VTNet;
class VTRede;
class VTRedes;

//---------------------------------------------------------------------------
class TFlowThreadEvent : public VTFlowThreadEvent
   {
   public:
           __fastcall  TFlowThreadEvent(void);
           __fastcall ~TFlowThreadEvent(void);

   private: //métodos

   private: //objetos externos

   private: //dados locais
   };

//---------------------------------------------------------------------------
class TFlowThread : public VTFlowThread
   {
   public:
           __fastcall TFlowThread(VTApl* apl_owner, bool bolCreateSuspended, VTFlowThreadEvent* thrEvent);
           __fastcall ~TFlowThread(void);
      void __fastcall Execute();

   private: //métodos
      bool   __fastcall ExecutaCalculoMNet(VTMNet *mnet);

   private: //objetos externos
      VTApl            *apl;
      TCriticalSection *g_cs;

   private: //dados locais
      bool               aviso_enabled;
      bool               tramo_enabled;
      VTLog              *plog;
      VTFlowThreadEvent  *thrEvent; // Evento para sinalização de conclusão da thread
   };

#endif
//---------------------------------------------------------------------------
//eof

