//---------------------------------------------------------------------------
#ifndef VTCapserieH
#define VTCapserieH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "TLigacao.h"

//---------------------------------------------------------------------------
class VTCapserie : public TLigacao
   {
   public:
                          __fastcall VTCapserie(void) {};
      virtual             __fastcall ~VTCapserie(void) {};
      virtual VTCapserie* __fastcall Clone(void) = 0;
      virtual void        __fastcall Z0_pu(double Sbase, double Vbase, double &r0, double &x0) = 0;
      virtual void        __fastcall Z1_pu(double Sbase, double Vbase, double &r1, double &x1) = 0;

   public: //dados
      float  vnom;     //kVA
      float  snom;     //MVA
      strIMP z;        //ohm
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe Capserie
//---------------------------------------------------------------------------
VTCapserie* __fastcall NewObjCapserie(int num_pat);

//---------------------------------------------------------------------------
#endif
//eof

 