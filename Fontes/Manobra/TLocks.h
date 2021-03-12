//---------------------------------------------------------------------------
#ifndef TLocksH
#define TLocksH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTLocks.h"

//---------------------------------------------------------------------------
class TLocks : public VTLocks
   {
   public:
                __fastcall TLocks(VTApl *apl);
                __fastcall ~TLocks(void);
      VTLock*   __fastcall ExisteLockComChaveMaster(VTChave *chave, int estado);
      VTLock*   __fastcall InsereLock(void);
      VTLocked* __fastcall InsereMaster(VTLock *lock, VTChave *chave, int estado);
      VTLocked* __fastcall InsereSlave(VTLock *lock, VTChave *chave, int estado);
      TList*    __fastcall LisLock(void);
      void      __fastcall RetiraChave(VTChave *chave);
      void      __fastcall RetiraLock(VTLock *lock);

   protected: //métodos acessados via property
      bool __fastcall PM_GetEnabled(void);
      void __fastcall PM_SetEnabled(bool enabled);

   private: // objetos externos
      VTApl *apl;
      
   private: // dados
      TList  *lisLOCK;    //lista de objetos Lock
      struct   {
               bool enabled;
               }PD;
   };

#endif
//---------------------------------------------------------------------------
//eof
