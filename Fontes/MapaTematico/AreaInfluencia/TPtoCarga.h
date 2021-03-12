//---------------------------------------------------------------------------
#ifndef TPtoCargaH
#define TPtoCargaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
//#include "VTPtoCarga.h"

//---------------------------------------------------------------------------
class VTCarga;

//---------------------------------------------------------------------------
class TPtoCarga : public TObject
   {
   public:  //property
      __property VTCarga* Carga        = {read=PD.carga        , write=PD.carga        };
      __property int      Distancia_cm = {read=PD.distancia_cm , write=PD.distancia_cm };
      __property int      X_cm         = {read=PD.x_cm         , write=PD.x_cm         };
      __property int      Y_cm         = {read=PD.y_cm         , write=PD.y_cm         };
      __property double   S_mva        = {read=PD.s_mva        , write=PD.s_mva        };
      __property bool     Incluido     = {read=PD.incluido     , write=PD.incluido     };

   public:  //métodos
              __fastcall  TPtoCarga(VTCarga *carga);
              __fastcall ~TPtoCarga(void);

   private:  //métodos

   private:  //dados locais
      struct   {
               VTCarga *carga;
               int     x_cm, y_cm;
               int     distancia_cm;
               double  s_mva;
               bool    incluido;
               }PD;
   };

//---------------------------------------------------------------------------
#endif
//eof
