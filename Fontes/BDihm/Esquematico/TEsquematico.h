//---------------------------------------------------------------------------
#ifndef TEsquematicoH
#define TEsquematicoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTEsquematico.h"

//---------------------------------------------------------------------------
class TEsquematico : public VTEsquematico
   {
   public:
              __fastcall TEsquematico(VTApl *apl);
              __fastcall ~TEsquematico(void);
      VTArea* __fastcall  Area(void);
      void    __fastcall  Clear(void);
      void    __fastcall  Coordenada(VTRede *rede, int &x, int &y);
      void    __fastcall  Coordenada(VTLiga *liga, int &x1, int &y1, int &x2, int &y2);
      void    __fastcall  Inicia(VTRede *rede, TList *lisREDE_VIZ, bool clear);
      TList*  __fastcall  LisLiga(void);
      TList*  __fastcall  LisRede(void);
      void    __fastcall  LisRede(TList *lisEXT);

   private: //métodos
      void    __fastcall CalculaArea(void);
      VTLiga* __fastcall ExisteLiga(VTRede *rede1, VTRede *rede2);

   private: //dados externos
      VTApl  *apl;

   private: //dados locais
      TList  *lisLIGA;
      TList  *lisREDE;
      VTArea *area;
   };

//---------------------------------------------------------------------------
#endif
//eof

