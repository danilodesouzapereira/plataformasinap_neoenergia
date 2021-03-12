//---------------------------------------------------------------------------
#ifndef VTEdicaoH
#define VTEdicaoH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTEqpto;

//---------------------------------------------------------------------------
class VTEdicao : public TObject
   {
   public:
                   __fastcall  VTEdicao(void) {};
      virtual      __fastcall ~VTEdicao(void) {};
      virtual int  __fastcall  Executa(VTEqpto *eqpto, bool criar_copia)= 0;
      virtual int  __fastcall  Executa(TList *lisEQP, bool criar_copia)= 0;
   };

//---------------------------------------------------------------------------
VTEdicao* __fastcall NewObjEdicao(VTApl *apl);

#endif
//---------------------------------------------------------------------------
//eof
