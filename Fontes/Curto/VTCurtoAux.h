//---------------------------------------------------------------------------
#ifndef VTCurtoAuxH
#define VTCurtoAuxH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <complex>
using namespace std;

class VTApl;

//---------------------------------------------------------------------------
// IMPORTANTE: alterações na enum abaixo deverão ser feitas também na
//             definição de 'codigo_tipo_curto[]', em TImpressao3.cpp
enum enumCU {cuTRI=0, cu2F, cuFT, cuFTZ, cu2FT};

//---------------------------------------------------------------------------
class VTCurtoAux : public TObject
   {
   public:
                              __fastcall  VTCurtoAux() {};
      virtual                 __fastcall ~VTCurtoAux(void) {};
      virtual double          __fastcall  FatorAssimetria(complex<double> zent) = 0;
      virtual bool            __fastcall  ReiniciaImpedanciasDefeito(int classe=-1) = 0;

   public:  // Dados
      complex<double> zdef_fase_ohm;  // Impedância de defeito (valor para a fase,  ohm)
      complex<double> zdef_terra_ohm; // Impedância de defeito (valor para a terra, ohm)
      complex<double> ydef_fase_S;    // Admitância de defeito (valor para a fase,  S)
      complex<double> ydef_terra_S;   // Admitância de defeito (valor para a terra, S)
      complex<double> yfase_fase_S;   // Admitância entre fases        no triângulo equivalente (curto 2FT, S)
      complex<double> yfase_terra_S;  // Admitância entre fase e terra no triângulo equivalente (curto 2FT, S)
   };

//---------------------------------------------------------------------------
// Função global para criar objeto da classe VTCurtoAux
//---------------------------------------------------------------------------
VTCurtoAux * __fastcall NewObjCurtoAux(VTApl *apl);

#endif
//---------------------------------------------------------------------------

