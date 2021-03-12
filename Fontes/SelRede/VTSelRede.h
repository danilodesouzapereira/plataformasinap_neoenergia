//---------------------------------------------------------------------------
#ifndef VTSelRedeH
#define VTSelRedeH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTRedes;

//---------------------------------------------------------------------------
class VTSelRede : public TObject
   {
   public:
                   __fastcall VTSelRede(void) {};
                   __fastcall ~VTSelRede(void) {};
      virtual void __fastcall Executa(VTRedes *redes, TList *lisSEL, int nivel_dependencia) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTSelRede
//---------------------------------------------------------------------------
VTSelRede* __fastcall NewObjSelRede(void);

//---------------------------------------------------------------------------
#endif
//eof



 