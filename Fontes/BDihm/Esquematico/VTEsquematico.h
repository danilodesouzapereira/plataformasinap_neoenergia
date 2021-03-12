//---------------------------------------------------------------------------
#ifndef VTEsquematicoH
#define VTEsquematicoH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTArea;
class VTLiga;
class VTRede;

//---------------------------------------------------------------------------
class VTEsquematico : public TObject
   {
   public:
                      __fastcall  VTEsquematico(void) {};
                      __fastcall ~VTEsquematico(void) {};
      virtual VTArea* __fastcall  Area(void) = 0;
      virtual void    __fastcall  Clear(void) = 0;
      virtual void    __fastcall  Coordenada(VTRede *rede, int &x, int &y) = 0;
      virtual void    __fastcall  Coordenada(VTLiga *liga, int &x1, int &y1, int &x2, int &y2) = 0;
      virtual void    __fastcall  Inicia(VTRede *rede, TList *lisREDE_VIZ, bool clear) = 0;
      virtual TList*  __fastcall  LisLiga(void) = 0;
      virtual TList*  __fastcall  LisRede(void) = 0;
      virtual void    __fastcall  LisRede(TList *lisEXT) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTEsquematico
//---------------------------------------------------------------------------
VTEsquematico* __fastcall NewObjEsquematico(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof



 