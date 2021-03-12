//---------------------------------------------------------------------------
#ifndef VTRebuildH
#define VTRebuildH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTRedes;

//---------------------------------------------------------------------------
class VTRebuild : public TObject
   {
   public:
                   __fastcall  VTRebuild(void) {};
                   __fastcall ~VTRebuild(void) {};
		virtual bool __fastcall  Executa(VTRedes *redes, TList *lisREDE, TList *lisBARRA, TList *lisLIGACAO) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTRebuild
//---------------------------------------------------------------------------
VTRebuild* __fastcall NewObjRebuild(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof



 