//---------------------------------------------------------------------------
#ifndef VTReconfiguraH
#define VTReconfiguraH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTRedes;

//---------------------------------------------------------------------------
class VTReconfigura : public TObject
   {
   public:
						 __fastcall  VTReconfigura(void) {};
                   __fastcall ~VTReconfigura(void) {};
		virtual bool __fastcall  Executa(VTRedes *redes, TList *lisREDE) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTReconfigura
//---------------------------------------------------------------------------
VTReconfigura* __fastcall NewObjReconfigura(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof



 