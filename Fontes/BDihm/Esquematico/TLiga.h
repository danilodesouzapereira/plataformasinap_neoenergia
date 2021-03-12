//---------------------------------------------------------------------------
#ifndef TLigaH
#define TLigaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTLiga.h"

//---------------------------------------------------------------------------
class TLiga : public VTLiga
   {
   public:
                __fastcall TLiga(void);
                __fastcall ~TLiga(void);
        void    __fastcall  DefineRede(VTRede *rede1, VTRede *rede2);
        VTRede* __fastcall  Rede(int index);

   private:
      VTRede *rede1, *rede2;
   };

//---------------------------------------------------------------------------
#endif
//eof

