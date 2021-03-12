//---------------------------------------------------------------------------
#ifndef VTPerdasH
#define VTPerdasH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTPerda;

//---------------------------------------------------------------------------
class VTPerdas : public TObject
   {
   public:
                       __fastcall VTPerdas(void) {};
      virtual          __fastcall ~VTPerdas(void) {};
      virtual bool     __fastcall Executa(void) = 0;
      virtual VTPerda* __fastcall ExistePerda(TObject *object) = 0;
      virtual TList*   __fastcall LisPerda(void) = 0;
      virtual void     __fastcall Reinicia(void) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTPerdas
//---------------------------------------------------------------------------
VTPerdas* __fastcall NewObjPerdas(VTApl *apl);
VTPerdas* __fastcall NewObjAgregadas(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof



 