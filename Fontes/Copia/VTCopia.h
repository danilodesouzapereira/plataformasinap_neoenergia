//---------------------------------------------------------------------------
#ifndef VTCopiaH
#define VTCopiaH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTRede;
class VTRedes;

//---------------------------------------------------------------------------
class VTCopia : public TObject
   {
   public:
                       __fastcall  VTCopia(void) {};
      virtual          __fastcall ~VTCopia(void) {};
      virtual bool     __fastcall  CopiaLisRedeParaRedes(TList *lisREDE, VTRedes *redes) = 0;
      virtual bool     __fastcall  CopiaRedes1ParaRedes2(VTRedes *redes1, VTRedes *redes2) = 0;
      virtual bool     __fastcall  DuplicaCargas(TList *lisCARGA) = 0;
      virtual bool     __fastcall  DuplicaCargas(VTRedes *redes, TList *lisCARGA) = 0;
      virtual VTRedes* __fastcall  DuplicaRedes(VTRedes *redes) = 0;
      virtual VTRedes* __fastcall  DuplicaUmaRede(VTRedes *redes, VTRede *rede) = 0;
   };

//---------------------------------------------------------------------------
//função para criar objeto VTCopia
VTCopia* __fastcall NewObjCopia(VTApl *apl);

#endif
//---------------------------------------------------------------------------
//eof
