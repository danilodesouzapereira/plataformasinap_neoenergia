//---------------------------------------------------------------------------
#ifndef TFluxoTramoH
#define TFluxoTramoH
using namespace std;

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <complex>
#include "VTFluxoTramo.h"

//---------------------------------------------------------------------------
class VTBarra;
class VTFases;
class VTPatamares;
class VTTramo;
class VTTrecho;

//---------------------------------------------------------------------------
class TFluxoTramo : public VTFluxoTramo
   {
   public:
            __fastcall TFluxoTramo(VTApl *apl);
            __fastcall ~TFluxoTramo(void);
       bool __fastcall Executa(VTNet *net, double Sbase);

   private: //métodos
      void __fastcall CalculaFluxoTrecho(VTTramo *tramo);
      void __fastcall CalculaPerdaTrecho(VTTramo *tramo);
      void __fastcall CalculaTensaoBarra(VTTramo *tramo);

   private: //objetos externos
      VTApl       *apl;
      VTFases     *fases;
      VTNet       *net;
      VTPatamares *patamares;

   private: //dados
      double Sbase;
      TList  *lisBAR;    //lista de Barras de um No
   };

#endif
//---------------------------------------------------------------------------
//eof

 