//---------------------------------------------------------------------------
#ifndef VTLigaH
#define VTLigaH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTRede;

//---------------------------------------------------------------------------
class VTLiga : public TObject
   {
   public:
                        __fastcall  VTLiga(void) {};
        virtual         __fastcall ~VTLiga(void) {};
        virtual void    __fastcall  DefineRede(VTRede *rede1, VTRede *rede2) = 0;
        virtual VTRede* __fastcall  Rede(int index) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTLiga
//---------------------------------------------------------------------------
VTLiga* __fastcall NewObjLiga(void);

//---------------------------------------------------------------------------
#endif
//eof



