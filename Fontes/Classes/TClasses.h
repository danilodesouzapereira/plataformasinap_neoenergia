//---------------------------------------------------------------------------
#ifndef TClassesH
#define TClassesH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTClasses.h"

//---------------------------------------------------------------------------
class TClasses : public VTClasses
   {
   public:
                __fastcall  TClasses(void);
                __fastcall ~TClasses(void);
      void      __fastcall  Clear(void);
      VTClasse* __fastcall  ExisteClasse(AnsiString codigo);
      VTClasse* __fastcall  ExisteClasse(int classe_id);
      VTClasse* __fastcall  ExisteClasseTag(int tag);
      VTClasse* __fastcall  First(void);
      void      __fastcall  InsereClasse(VTClasse *classe);
      TList*    __fastcall  LisClasse(void);

   private: //dados locais
      TList *lisCLASSE;
   };

#endif
//---------------------------------------------------------------------------
//eof

