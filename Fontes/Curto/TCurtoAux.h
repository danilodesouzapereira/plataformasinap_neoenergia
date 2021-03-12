//---------------------------------------------------------------------------
#ifndef TCurtoAuxH
#define TCurtoAuxH

//---------------------------------------------------------------------------
#include ".\VTCurtoAux.h"
#include <Classes.hpp>
#include <complex>
using namespace std;

//---------------------------------------------------------------------------
class TCurtoAux : public VTCurtoAux
   {
   public:  // Métodos
                           __fastcall  TCurtoAux(VTApl *apl);
                           __fastcall ~TCurtoAux(void);
           double          __fastcall  FatorAssimetria(complex<double> zent);
           bool            __fastcall  ReiniciaImpedanciasDefeito(int tipo=-1);

   protected:  //Dados
      VTApl           *apl;
      complex<double> calfa, calfa2, cum, czero;
   };

#endif
//---------------------------------------------------------------------------

