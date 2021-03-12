//---------------------------------------------------------------------------
#ifndef VTRotacionaH
#define VTRotacionaH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
//class VTCluster;
class VTRede;
//class VTRedes;

//---------------------------------------------------------------------------
class VTRotaciona : public TObject
   {
   public:
                   __fastcall VTRotaciona(void) {};
                   __fastcall ~VTRotaciona(void) {};
      virtual void __fastcall Executa(VTRede *rede, int align) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTRotaciona
//---------------------------------------------------------------------------
VTRotaciona* __fastcall NewObjRotaciona(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof



 