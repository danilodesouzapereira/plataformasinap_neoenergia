//---------------------------------------------------------------------------
#ifndef TMutuaH
#define TMutuaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTMutua.h"

//---------------------------------------------------------------------------
class TMutua : public VTMutua
   {
   public:
                 __fastcall  TMutua(void);
                 __fastcall ~TMutua(void);
      VTMutua*   __fastcall  Clone(void);
      void       __fastcall  CoordenadasEsquematico(int &x1, int &y1, int &x2, int &y2);
      void       __fastcall  CoordenadasEsquematico(int &xm, int &ym, int &x1, int &y1,
                                                    int &x2, int &y2, int &x3, int &y3);
      bool       __fastcall  CoordenadasUtm_cm(int &x, int &y);
      bool       __fastcall  CoordenadasUtm_cm(int &x1, int &y1, int &x2, int &y2);
      bool       __fastcall  CoordenadasUtm_cm(int &xm, int &ym, int &x1, int &y1,
                                               int &x2, int &y2, int &x3, int &y3);
      bool       __fastcall  CoordenadasUtm_m(int &x, int &y);
      bool       __fastcall  CoordenadasUtm_m(int &x1, int &y1, int &x2, int &y2);
      bool       __fastcall  CoordenadasUtm_m(int &xm, int &ym, int &x1, int &y1,
                                              int &x2, int &y2, int &x3, int &y3);
      void       __fastcall  CopiaAtributosDe(VTEqpto &ref);
      void       __fastcall  DefineObjBarTrecho(VTTrecho *ptre, VTBarra *pbarra);
      void       __fastcall  DefineObjTrecho(VTTrecho *ptre1, VTTrecho *ptre2);
      void       __fastcall  DefinePosTre(VTTrecho *ptre, int x, int y);
      bool       __fastcall  ExisteTrecho(VTTrecho* trecho);
      bool       __fastcall  ExisteTrechoComum(VTMutua *mutua);
      VTRede*    __fastcall  Rede(void);
      int        __fastcall  Tipo(void);
      AnsiString __fastcall  TipoAsString(void);
      bool       __fastcall  Z0_pu(complex<double> &z0_pu, double sbase);
      bool       __fastcall  Z1_pu(complex<double> &z1_pu, double sbase);
   };

#endif
//---------------------------------------------------------------------------
//eof

