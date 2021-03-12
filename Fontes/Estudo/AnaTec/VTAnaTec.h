//---------------------------------------------------------------------------
#ifndef VTAnaTecH
#define VTAnaTecH

//----------------------------------------------------------------------------
#include <Classes.hpp>

//----------------------------------------------------------------------------
class VTApl;
class VTAlternativa;

//----------------------------------------------------------------------------
class VTAnaTec : public TObject
   {
   public:
                     __fastcall  VTAnaTec(void) {};
      virtual        __fastcall ~VTAnaTec(void) {};
      virtual bool   __fastcall Executa(void) = 0;
      virtual bool   __fastcall Executa(VTAlternativa *alternativa, int ano) = 0;
   };

//---------------------------------------------------------------------------
//função p/ criar objeto da classe VTAnaTec
//---------------------------------------------------------------------------
VTAnaTec* __fastcall NewObjAnaTec(VTApl *apl_owner);

//---------------------------------------------------------------------------
#endif
//eof
