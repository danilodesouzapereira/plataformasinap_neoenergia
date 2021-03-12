//---------------------------------------------------------------------------
#ifndef TMapasH
#define TMapasH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTMapas.h"

//---------------------------------------------------------------------------
class TMapas : public VTMapas
   {
   public:
                __fastcall TMapas(VTApl *apl);
                __fastcall ~TMapas(void);
      VTMapa*   __fastcall  CriaMapa(void);
      VTMapa*   __fastcall  ExisteMapa(int mapa_id);
      VTMapa*   __fastcall  ExisteMapa(AnsiString codigo);
      void      __fastcall  InsereMapa(VTMapa *mapa);
      TList*    __fastcall  LisMapa(void);
      void      __fastcall  RetiraMapa(VTMapa *mapa);

   private: //métodos

   private: //dados externos
      VTApl *apl;

   private: //dados locais
      TList *lisMAPA;
   };

//---------------------------------------------------------------------------
#endif
//eof
