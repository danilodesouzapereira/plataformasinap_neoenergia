//---------------------------------------------------------------------------
#ifndef VTEsqVizH
#define VTEsqVizH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTRede;

//---------------------------------------------------------------------------
class VTEsqViz : public TObject
   {
   public:
                   __fastcall VTEsqViz(void) {};
      virtual      __fastcall ~VTEsqViz(void) {};
      virtual void __fastcall Executa(VTRede *rede) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTEsqViz
//---------------------------------------------------------------------------
VTEsqViz* __fastcall NewObjEsqViz(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof



 