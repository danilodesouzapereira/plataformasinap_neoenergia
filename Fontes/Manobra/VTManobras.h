//---------------------------------------------------------------------------
#ifndef VTManobrasH
#define VTManobrasH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTChave;
class VTManobra;

//---------------------------------------------------------------------------
class VTManobras : public TObject
   {
   public:
                         __fastcall VTManobras(void) {};
      virtual            __fastcall ~VTManobras(void) {};
      virtual void       __fastcall ExecutaManobra(VTManobra *manobra) = 0;
      virtual void       __fastcall ExecutaManobraComplementar(VTManobra *manobra) = 0;
      virtual VTManobra* __fastcall ExisteManobra(AnsiString codigo) = 0;
      virtual VTManobra* __fastcall InsereManobra(void) = 0;
      virtual TList*     __fastcall LisManobra(void) = 0;
      virtual void       __fastcall Reinicia(void) = 0;
      virtual void       __fastcall RestauraEstadoOriginalChaves(VTManobra *manobra) = 0;
      virtual void       __fastcall RetiraChave(VTChave *chave) = 0;
      virtual void       __fastcall RetiraManobra(VTManobra *manobra) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe
VTManobras* __fastcall NewObjManobras(VTApl *apl);

#endif
//---------------------------------------------------------------------------
//eof
