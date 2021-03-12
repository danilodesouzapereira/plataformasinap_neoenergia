//---------------------------------------------------------------------------
#ifndef VTLigacaoG1H
#define VTLigacaoG1H

#include <complex>
using namespace std;

class TBarra;
class VTGeralC;

//---------------------------------------------------------------------------
class VTLigacaoG1 : public TObject
   {
   public:  // Métodos
                              __fastcall  VTLigacaoG1() {};
      virtual                 __fastcall ~VTLigacaoG1(void) {};
      virtual int             __fastcall  Barras(TBarra **bar1,
                                                 TBarra **bar2,
                                                 TBarra **bar3) = 0;
      virtual int             __fastcall  IndiceGlobal(int indice_local) = 0;
      virtual int             __fastcall  NumBarras(void) = 0;
      virtual int             __fastcall  TipoLigacao(void) = 0;
      virtual complex<double> __fastcall  Y1par_pu(VTGeralC *geralC,
                                                   TBarra   *bar1) = 0; // METADE do total da linha
      virtual complex<double> __fastcall  Z1ser2_pu(void) = 0;          // TOTAL da linha

   public:  // Dados
      VTLigacaoG1 *pai_radial;    // Ligação pai na rede radial
      int         sentido_radial; // Sentido do fluxo na ligação radial:
                                  //    0 = bar1-bar2 ou bar1; 1 = bar2-bar1 ou bar2; 2 = bar3
      int         indice_radial;  // Índice deste objeto em 'lisLIG_RADIAL'
   };

#endif
//---------------------------------------------------------------------------

