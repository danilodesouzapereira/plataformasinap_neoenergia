//---------------------------------------------------------------------------
#ifndef VTLockH
#define VTLockH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTChave;
class VTLocked;
/*
//---------------------------------------------------------------------------
class VTLocked : public TObject
   {
   public:
      VTChave *chave;
      int      estado;
   };
*/
//---------------------------------------------------------------------------
class VTLock : public TObject
   {
   public:	//property
      __property int        Id                   = {read=PM_GetId,     write=PM_SetId};
      __property AnsiString Codigo               = {read=PM_GetCodigo, write=PM_SetCodigo};
      __property bool       Status[unsigned bit] = {read=PM_GetStatus, write=PM_SetStatus};

   public:
                        __fastcall VTLock(void) {};
      virtual           __fastcall ~VTLock(void) {};
      virtual VTLocked* __fastcall ExisteMaster(VTChave *chave, int estado) = 0;
      virtual VTLocked* __fastcall ExisteSlave(VTChave *chave, int estado) = 0;
      virtual VTLocked* __fastcall InsereMaster(VTChave *chave, int estado) = 0;
      virtual VTLocked* __fastcall InsereSlave(VTChave *chave, int estado) = 0;
      virtual TList*    __fastcall LisMaster(void) = 0;
      virtual TList*    __fastcall LisSlave(void) = 0;
      virtual void      __fastcall RetiraLocked(VTLocked *locked) = 0;
      virtual void      __fastcall RetiraMasterSlave(VTChave *chave) = 0;
      virtual bool      __fastcall VerificaDisparo(void) = 0;
      virtual bool      __fastcall VerificaDisparo(VTChave *chave, int estado) = 0;

   protected:  //métodos acessados via property
      virtual AnsiString __fastcall PM_GetCodigo(void) = 0;
      virtual int        __fastcall PM_GetId(void) = 0;
      virtual bool       __fastcall PM_GetStatus(unsigned bit) = 0;
      virtual void       __fastcall PM_SetCodigo(AnsiString codigo) = 0;
      virtual void       __fastcall PM_SetId(int id) = 0;
      virtual void       __fastcall PM_SetStatus(unsigned bit, bool enabled) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe
VTLock* __fastcall NewObjLock(void);

#endif
//---------------------------------------------------------------------------
//eof
