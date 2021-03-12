//-----------------------------------------------------------------------------
#ifndef VTValPerdafeH
#define VTValPerdafeH

//arquivos inclu�dos-----------------------------------------------------------
#include <Classes.hpp>

//-----------------------------------------------------------------------------
class VTApl;

//-----------------------------------------------------------------------------
class VTValPerdafe : public TObject
   {
   public:
                      __fastcall  VTValPerdafe(void) {};
      virtual         __fastcall ~VTValPerdafe(void) {};
      virtual bool    __fastcall  Executa(void)=0;
      virtual int     __fastcall  ExisteTrafoComPerdaFeInvalida(void)=0;
      virtual TList*  __fastcall  LisTrafoComPerdaFeInvalida(void)=0;
   };

//-----------------------------------------------------------------------------
// fun��o global para criar objeto VTMonta
//-----------------------------------------------------------------------------
VTValPerdafe* __fastcall NewObjValPerdafe(VTApl *apl);

#endif
//-----------------------------------------------------------------------------
// eof
