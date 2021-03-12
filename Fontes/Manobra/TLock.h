//---------------------------------------------------------------------------
#ifndef TLockH
#define TLockH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTLock.h"

//---------------------------------------------------------------------------
class TLock : public VTLock
   {
   public:
                __fastcall TLock(void);
                __fastcall ~TLock(void);
      VTLocked* __fastcall ExisteMaster(VTChave *chave, int estado);
      VTLocked* __fastcall ExisteSlave(VTChave *chave, int estado);
      VTLocked* __fastcall InsereMaster(VTChave *chave, int estado);
      VTLocked* __fastcall InsereSlave(VTChave *chave, int estado);
      TList*    __fastcall LisMaster(void);
      TList*    __fastcall LisSlave(void);
      void      __fastcall RetiraLocked(VTLocked *locked);
      void      __fastcall RetiraMasterSlave(VTChave *chave);
      bool      __fastcall VerificaDisparo(void);
      bool      __fastcall VerificaDisparo(VTChave *chave, int estado);

   protected:  //métodos acessados via property
      AnsiString __fastcall PM_GetCodigo(void);
      int        __fastcall PM_GetId(void);
      bool       __fastcall PM_GetStatus(unsigned bit);
      void       __fastcall PM_SetCodigo(AnsiString codigo);
      void       __fastcall PM_SetId(int id);
      void       __fastcall PM_SetStatus(unsigned bit, bool enabled);

   private: // métodos
      VTLocked* __fastcall ExisteLocked(TList *lisLOCKED, VTChave *chave);

   public: // dados
      TList *lisMASTER;  //lista de objetos Locked das Chaves master
      TList *lisSLAVE;   //lista de objetos Locked das Chaves slave
      struct   {
               int        id;
               int        status;
               AnsiString codigo;
               } PD;
   };

#endif
//---------------------------------------------------------------------------
//eof
