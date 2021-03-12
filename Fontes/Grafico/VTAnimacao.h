//---------------------------------------------------------------------------
#ifndef VTAnimacaoH
#define VTAnimacaoH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTUnif;

//---------------------------------------------------------------------------
class VTAnimacao : public TObject
   {
   public:  //property
      __property bool Enabled = {read=PM_GetEnabled, write=PM_SetEnabled};

   public:		// User declarations
                   __fastcall VTAnimacao(void) {};
                   __fastcall ~VTAnimacao(void) {};
      virtual void __fastcall Start(VTUnif *unif) = 0;
      virtual void __fastcall Stop(void) = 0;

   protected: //métodos acessados via property
      virtual bool __fastcall PM_GetEnabled(void) = 0;
      virtual void __fastcall PM_SetEnabled(bool enabled) = 0;
   };

//-----------------------------------------------------------------------------
// função global para criar objeto da classe VTAnimacao
//-----------------------------------------------------------------------------
VTAnimacao* __fastcall NewObjAnimacao(void);

//---------------------------------------------------------------------------
#endif
//eof
