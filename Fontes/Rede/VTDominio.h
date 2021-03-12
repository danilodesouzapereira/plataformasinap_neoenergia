//---------------------------------------------------------------------------
#ifndef VTDominioH
#define VTDominioH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTEqpto.h"

//---------------------------------------------------------------------------
class VTRede;

//---------------------------------------------------------------------------
class VTDominio : public VTEqpto
   {
   public: //property
      __property TDateTime DataCriacao = {read=PM_GetDataCriacao, write=PM_SetDataCriacao};

   public:
                         __fastcall  VTDominio(void) {};
      virtual            __fastcall ~VTDominio(void) {};
      virtual VTDominio* __fastcall  Clone(void) = 0;
      virtual bool       __fastcall  ExisteRede(VTRede *rede) = 0;
      virtual void       __fastcall  InsereRede(VTRede *rede) = 0;
      virtual TList*     __fastcall  LisRede(void) = 0;
      virtual void       __fastcall  LisRede(TList *lisEXT) = 0;
      virtual void       __fastcall  Reinicia(void) = 0;
      virtual void       __fastcall  RemoveRede(VTRede *rede) = 0;

   protected: //métodos acessados via property
      virtual TDateTime __fastcall PM_GetDataCriacao(void) = 0;
      virtual void      __fastcall PM_SetDataCriacao(TDateTime dttDataCriacao) = 0;
  };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTDominio
//---------------------------------------------------------------------------
VTDominio* __fastcall NewObjDominio(void);

//---------------------------------------------------------------------------
#endif
//eof



 