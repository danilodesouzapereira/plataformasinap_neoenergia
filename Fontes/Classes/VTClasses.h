//---------------------------------------------------------------------------
#ifndef VTClassesH
#define VTClassesH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTClasse;

//---------------------------------------------------------------------------
class VTClasses : public TObject
   {
   public:
                        __fastcall  VTClasses(void) {};
      virtual           __fastcall ~VTClasses(void) {};
      virtual void      __fastcall  Clear(void) = 0;
      virtual VTClasse* __fastcall  ExisteClasse(AnsiString codigo) = 0;
      virtual VTClasse* __fastcall  ExisteClasse(int classe_id) = 0;
      virtual VTClasse* __fastcall  ExisteClasseTag(int tag) = 0;
      virtual VTClasse* __fastcall  First(void) = 0;
      virtual void      __fastcall  InsereClasse(VTClasse *classe) = 0;
      virtual TList*    __fastcall  LisClasse(void) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTClasses
//---------------------------------------------------------------------------
VTClasses* __fastcall NewObjClasses(void);

//---------------------------------------------------------------------------
#endif
//eof



 