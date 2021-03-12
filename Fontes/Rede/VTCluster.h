//---------------------------------------------------------------------------
#ifndef VTClusterH
#define VTClusterH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTEqpto.h"

//---------------------------------------------------------------------------
class VTRede;

//---------------------------------------------------------------------------
class VTCluster : public VTEqpto
   {
   public:  //property
      __property bool Carregado = {read=PM_GetCarregado};
      
   public:
                         __fastcall VTCluster(void) {};
      virtual            __fastcall ~VTCluster(void) {};
      virtual VTCluster* __fastcall Clone(void) = 0;
      virtual bool       __fastcall ExisteRede(VTRede *rede) = 0;
      virtual void       __fastcall InsereRede(VTRede *rede) = 0;
      virtual TList*     __fastcall LisRede(void) = 0;
      virtual void       __fastcall LisRede(TList *lisEQP) = 0;
      virtual int        __fastcall NumeroRedes(void) = 0;
      virtual void       __fastcall Reinicia(void) = 0;
      virtual void       __fastcall RemoveRede(VTRede *rede) = 0;

   protected:  //métodos acessados via property
      virtual bool       __fastcall PM_GetCarregado(void) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTCluster
//---------------------------------------------------------------------------
VTCluster* __fastcall NewObjCluster(void);

//---------------------------------------------------------------------------
#endif
//eof



 