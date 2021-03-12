//---------------------------------------------------------------------------
#ifndef VTResultadosH3H
#define VTResultadosH3H

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <complex>
using namespace std;

class VTGeralC;
class VTRede3;
class VTRedeFH;

//---------------------------------------------------------------------------
class VTResultadosH3 : public TObject
   {
   public:  // Métodos
                   __fastcall  VTResultadosH3() {};
      virtual      __fastcall ~VTResultadosH3(void) {};
      virtual bool __fastcall  SalvaResultadosPatamar(int   np,
                                                      TList *lisBAR_H3) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VResultadosH3
//---------------------------------------------------------------------------
VTResultadosH3 * __fastcall NewObjResultadosH3(VTGeralC *geralC,
                                               VTRede3  *rede3,
                                               VTRedeFH *redeFH);

#endif
//---------------------------------------------------------------------------

