//---------------------------------------------------------------------------
#ifndef VTTrafoZZH
#define VTTrafoZZH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "TEqbar.h"
#include "Estrutura.h"
#include "..\Constante\Const.h"

//---------------------------------------------------------------------------
class VTTrafoZZ : public TEqbar
   {
   public:
                         __fastcall VTTrafoZZ(void) {};
      virtual            __fastcall ~VTTrafoZZ(void) {};
		virtual VTTrafoZZ* __fastcall Clone(void) = 0;
      virtual bool       __fastcall Z0_pu(double Sbase, strIMP &z0_pu) = 0;

   public: //dados
      float  vnom, snom;
      strIMP z0;  // pu de Snom
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe TrafoZZ
//---------------------------------------------------------------------------
VTTrafoZZ* __fastcall NewObjTrafoZZ(void);

//---------------------------------------------------------------------------
#endif
//eof

 