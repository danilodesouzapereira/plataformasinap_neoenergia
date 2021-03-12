//---------------------------------------------------------------------------
#ifndef VTMThreadH
#define VTMThreadH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTMNet;

//---------------------------------------------------------------------------
class VTMThread : public TObject
   {
   public:
                     __fastcall  VTMThread(void) {};
                     __fastcall ~VTMThread(void) {};
      virtual bool   __fastcall  ExecutaFluxoPorThread(VTMNet *mnet) = 0;
      virtual int    __fastcall  LisMNetCalculada(TList *lisEXT, bool clear=true) = 0;
      virtual int    __fastcall  NumeroThreadAtiva(void) = 0;
      //virtual void __fastcall  WaitForThreadLivre() = 0;
   };

//-----------------------------------------------------------------------------
// função global para criar objeto VTMThread
//-----------------------------------------------------------------------------
VTMThread* NewObjMThread(VTApl* apl_owner);

#endif
//---------------------------------------------------------------------------
//eof
