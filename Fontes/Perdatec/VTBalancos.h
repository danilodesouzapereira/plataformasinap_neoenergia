//---------------------------------------------------------------------------
#ifndef VTBalancosH
#define VTBalancosH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTBalanco;
class VTRede;

//---------------------------------------------------------------------------
class VTBalancos : public TObject
   {
   public:
                         __fastcall VTBalancos(void) {};
      virtual            __fastcall ~VTBalancos(void) {};
      virtual bool       __fastcall Executa(void) = 0;
      virtual VTBalanco* __fastcall ExisteBalanco(VTRede *rede) = 0;
      virtual TList*     __fastcall LisBalanco(void) = 0;
      virtual void       __fastcall Reinicia(void) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTBalancos
//---------------------------------------------------------------------------
VTBalancos* __fastcall NewObjBalancos(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof



 