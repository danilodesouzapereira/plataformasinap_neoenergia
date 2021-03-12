//---------------------------------------------------------------------------
#ifndef VTFiltroH
#define VTFiltroH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "TEqbar.h"
#include "Estrutura.h"
#include "..\Constante\Const.h"

//---------------------------------------------------------------------------
class VTFiltro : public TEqbar
   {
   public:
                        __fastcall VTFiltro(void) {};
      virtual           __fastcall ~VTFiltro(void) {};
      virtual VTFiltro* __fastcall Clone(void) = 0;
      virtual bool      __fastcall Z0_pu(double Sbase, strIMP &z0_pu)=0;

   public: //dados
      float  vnom, snom;
      strIMP z0;  //pu se Snom
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe Filtro
//---------------------------------------------------------------------------
VTFiltro* __fastcall NewObjFiltro(void);

//---------------------------------------------------------------------------
#endif
//eof

 