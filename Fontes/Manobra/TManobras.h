//---------------------------------------------------------------------------
#ifndef TManobrasH
#define TManobrasH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTManobras.h"

//---------------------------------------------------------------------------
class TManobras : public VTManobras
   {
   public:
                 __fastcall TManobras(VTApl *apl);
                 __fastcall ~TManobras(void);
      void       __fastcall ExecutaManobra(VTManobra *manobra);
      void       __fastcall ExecutaManobraComplementar(VTManobra *manobra);
      VTManobra* __fastcall ExisteManobra(AnsiString codigo);
      VTManobra* __fastcall InsereManobra(void);
      TList*     __fastcall LisManobra(void);
      void       __fastcall Reinicia(void);
      void       __fastcall RestauraEstadoOriginalChaves(VTManobra *manobra);
      void       __fastcall RetiraChave(VTChave *chave);
      void       __fastcall RetiraManobra(VTManobra *manobra);

   private: // objetos externos
      VTApl *apl;
      
   private: // dados
      TList  *lisCHAVE;
      TList  *lisMANOBRA;    //lista de objetos Manobra
   };

#endif
//---------------------------------------------------------------------------
//eof
