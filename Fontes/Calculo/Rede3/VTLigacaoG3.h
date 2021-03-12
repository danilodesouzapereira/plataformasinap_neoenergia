//---------------------------------------------------------------------------
#ifndef VTLigacaoG3H
#define VTLigacaoG3H

#include <complex>
using namespace std;

class TBarra;

//---------------------------------------------------------------------------
class VTLigacaoG3 : public TObject
   {
   public:  // Métodos
                              __fastcall  VTLigacaoG3() {};
      virtual                 __fastcall ~VTLigacaoG3(void) {};
      virtual int             __fastcall  Barras(TBarra **bar1,
                                                 TBarra **bar2,
                                                 TBarra **bar3) = 0;
      virtual int             __fastcall  IndiceGlobal(int indice_local) = 0;
      virtual int             __fastcall  NumBarras(void) = 0;
      virtual int             __fastcall  TipoLigacao(void) = 0;
   };

#endif
//---------------------------------------------------------------------------

