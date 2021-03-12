//---------------------------------------------------------------------------
#ifndef VTMapasH
#define VTMapasH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTMapa;

//---------------------------------------------------------------------------
class VTMapas : public TObject
   {
   public:
                        __fastcall  VTMapas(void) {};
      virtual           __fastcall ~VTMapas(void) {};
      virtual VTMapa*   __fastcall  CriaMapa(void) = 0;
      virtual VTMapa*   __fastcall  ExisteMapa(int mapa_id) = 0;
      virtual VTMapa*   __fastcall  ExisteMapa(AnsiString codigo) = 0;
      virtual void      __fastcall  InsereMapa(VTMapa *mapa) = 0;
      virtual TList*    __fastcall  LisMapa(void) = 0;
      virtual void      __fastcall  RetiraMapa(VTMapa *mapa) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTMapas
//---------------------------------------------------------------------------
VTMapas* __fastcall NewObjMapas(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof

 