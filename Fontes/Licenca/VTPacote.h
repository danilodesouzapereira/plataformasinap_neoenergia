//---------------------------------------------------------------------------
#ifndef VTPacoteH
#define VTPacoteH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTCampo;

//---------------------------------------------------------------------------
class VTPacote : public TObject
   {
   public:  //propery
      __property AnsiString CamposAsStr = {read=PM_GetCamposAsStr, write=PM_SetCamposAsStr};

   public:
                         __fastcall VTPacote(void) {};
      virtual            __fastcall ~VTPacote(void) {};
      virtual VTCampo*   __fastcall Campo(AnsiString nome) = 0;
      virtual void       __fastcall Clear(void) = 0;

   protected:  //métodos acessados via propery
      virtual AnsiString __fastcall PM_GetCamposAsStr(void) = 0;
      virtual void       __fastcall PM_SetCamposAsStr(AnsiString txt) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe
VTPacote* __fastcall NewObjPacote(void);

#endif
//---------------------------------------------------------------------------//eof

