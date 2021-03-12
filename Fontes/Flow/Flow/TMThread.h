//---------------------------------------------------------------------------
#ifndef TMThreadH
#define TMThreadH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <SyncObjs.hpp>
#include "VTMThread.h"

//---------------------------------------------------------------------------
//class ;

//---------------------------------------------------------------------------
class TMThread : public VTMThread
   {
   public:
             __fastcall  TMThread(VTApl* apl_owner);
             __fastcall ~TMThread(void);
      bool   __fastcall  ExecutaFluxoPorThread(VTMNet *mnet);
      int    __fastcall  LisMNetCalculada(TList *lisEXT, bool clear=true);
      int    __fastcall  NumeroThreadAtiva(void);
      //void __fastcall  WaitForThreadLivre();

   private: //métodos
      void __fastcall CriaObjetosEvent(void);
      void __fastcall VerificaObjetosEventLiberados(void);

   private: //objetos externos
      VTApl            *apl;
      TCriticalSection *g_cs;

   private: //dados locais
      int    intThrMax;
      HANDLE *vet_handle;
      TList  *lisEVENT_ALL;        //lista de todos objetos VTFlowThreadEvent disponíveis
      TList  *lisEVENT_DIS;        //lista de objetos VTFlowThreadEvent disponíveis
      TList  *lisEVENT_USO;        //lista de objetos VTFlowThreadEvent em uso
      TList  *lisMNET_CALC;        //lista de objetos VTMNet já calculados
   };

#endif
//---------------------------------------------------------------------------
//eof

