//---------------------------------------------------------------------------
#ifndef VTLocksH
#define VTLocksH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTChave;
class VTLock;
class VTLocked;

//---------------------------------------------------------------------------
class VTLocks : public TObject
   {
   public:  //property
      __property bool Enabled = {read=PM_GetEnabled, write=PM_SetEnabled};

   public:
                        __fastcall VTLocks(void) {};
      virtual           __fastcall ~VTLocks(void) {};
      virtual VTLock*   __fastcall ExisteLockComChaveMaster(VTChave *chave, int estado) = 0;
      virtual VTLock*   __fastcall InsereLock(void) = 0;
      virtual VTLocked* __fastcall InsereMaster(VTLock *lock, VTChave *chave, int estado) = 0;
      virtual VTLocked* __fastcall InsereSlave(VTLock *lock, VTChave *chave, int estado) = 0;
      virtual TList*    __fastcall LisLock(void) = 0;
      virtual void      __fastcall RetiraChave(VTChave *chave) = 0;
      virtual void      __fastcall RetiraLock(VTLock *lock) = 0;

   protected: //métodos acessados via property
      virtual bool __fastcall PM_GetEnabled(void) = 0;
      virtual void __fastcall PM_SetEnabled(bool enabled) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe
VTLocks* __fastcall NewObjLocks(VTApl *apl);

#endif
//---------------------------------------------------------------------------
//eof
