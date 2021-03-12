//-----------------------------------------------------------------------------
#ifndef VTValCaboH
#define VTValCaboH

//arquivos inclu�dos-----------------------------------------------------------
#include <Classes.hpp>

//-----------------------------------------------------------------------------
class VTApl;
class VTCabo;

//-----------------------------------------------------------------------------
class VTValCabo : public TObject
   {
   public:
                      __fastcall  VTValCabo(void) {};
      virtual         __fastcall ~VTValCabo(void) {};
      virtual bool    __fastcall  ValidaCaboMonopolar(VTCabo *cabo)=0;
      virtual bool    __fastcall  ValidaCaboMultiplexado(VTCabo *cabo)=0;
   };

//-----------------------------------------------------------------------------
// fun��o global para criar objeto VTValCabo
//-----------------------------------------------------------------------------
VTValCabo* __fastcall NewObjValCabo(VTApl *apl);

#endif
//-----------------------------------------------------------------------------
// eof
