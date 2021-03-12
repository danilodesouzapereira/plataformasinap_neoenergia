//---------------------------------------------------------------------------
#ifndef VTFlowThreadH
#define VTFlowThreadH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTMNet;
class VTNode;

//---------------------------------------------------------------------------
class VTFlowThreadEvent : public TObject
   {
   public:
      __property HANDLE  Handle           = {read=PD.handle,             write=PD.handle           };
      __property VTMNet* MNet             = {read=PD.mnet,               write=PD.mnet             };

      __fastcall  VTFlowThreadEvent(void)  {};
      __fastcall ~VTFlowThreadEvent(void) {};  // OOPS: Eu achava que esse deveria ser virtual

   protected:  //dados  acessados via property
      struct   {
               HANDLE  handle;
               VTMNet  *mnet;
               }PD;
   };

//---------------------------------------------------------------------------
class VTFlowThread : public TThread
   {
   public:
                   __fastcall  VTFlowThread(bool bolCreateSuspended) : TThread(bolCreateSuspended) {};
                   __fastcall ~VTFlowThread(void) {}; // OOPS: Eu achava que esse deveria ser virtual
      virtual void __fastcall  Execute() = 0;
   };

//-----------------------------------------------------------------------------
// função global para criar objeto VTFlowThread
//-----------------------------------------------------------------------------
VTFlowThread* NewObjFlowThread(VTApl* apl_owner, bool bolCreateSuspended, VTFlowThreadEvent* thrEvent);
VTFlowThreadEvent* NewObjFlowThreadEvent(void);

#endif
//---------------------------------------------------------------------------
//eof
