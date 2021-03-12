//---------------------------------------------------------------------------
#ifndef TZoomGoogleH
#define TZoomGoogleH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
struct AREA_LL  {
                double lat1, lon1, lat2, lon2;
                };

//---------------------------------------------------------------------------
class TZoomGoogle
   {
   public:
           __fastcall  TZoomGoogle(void);
           __fastcall ~TZoomGoogle(void);
      int  __fastcall  MelhorValor(double z0, AREA_LL &area_ll, int tela_height, int tela_width);

   private: //métodos
      int __fastcall ZoomHeight(void);
      int __fastcall ZoomWidth(void);

   private: //dados
      #define MAX_ZOOM  24
      double z0;
      double lat1, lat2;
      double lon1, lon2;
      struct {
             double height, width;
             }tela;

   };

#endif
//---------------------------------------------------------------------------
//eof

