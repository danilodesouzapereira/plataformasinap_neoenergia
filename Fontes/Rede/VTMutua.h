//---------------------------------------------------------------------------
#ifndef VTMutuaH
#define VTMutuaH
using namespace std;

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <complex>
#include "Estrutura.h"
#include "VTEqpto.h"
#include "..\Constante\Const.h"

//---------------------------------------------------------------------------
class VTBarra;
class VTTrecho;
class VTRede;

//---------------------------------------------------------------------------
class VTMutua : public VTEqpto
   {
   public:
      virtual          __fastcall VTMutua(void) {};
      virtual          __fastcall ~VTMutua(void) {};
      virtual VTMutua* __fastcall Clone(void) = 0;
      virtual void     __fastcall DefineObjBarTrecho(VTTrecho *ptre, VTBarra *pbarra) = 0;
      virtual void     __fastcall DefineObjTrecho(VTTrecho *ptre1, VTTrecho *ptre2) = 0;
      virtual void     __fastcall DefinePosTre(VTTrecho *ptre, int x, int y) = 0;
      virtual bool     __fastcall ExisteTrecho(VTTrecho* trecho) = 0;
      virtual bool     __fastcall ExisteTrechoComum(VTMutua *mutua) = 0;
      virtual VTRede*  __fastcall Rede(void) = 0;
      virtual bool     __fastcall Z0_pu(complex<double> &z0_pu, double sbase) = 0;
      virtual bool     __fastcall Z1_pu(complex<double> &z1_pu, double sbase) = 0;

   public: //dados
      float     postre1, postre2;  //posicões nos trechos (%) para diag.esquemático
      strIMP    z0, z1;            //ohm/km
      VTTrecho  *ptre1, *ptre2;
      VTBarra   *pbar_tre1;        //barras de referência dos trechos p/ indicar
      VTBarra   *pbar_tre2;        //'sentido' da mútua
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe
VTMutua* __fastcall NewObjMutua(void);

#endif
//---------------------------------------------------------------------------
//eof

