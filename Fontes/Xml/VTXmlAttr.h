//---------------------------------------------------------------------------
#ifndef VTXmlAttrH
#define VTXmlAttrH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTXmlAttr : public TObject
   {
   public:
      __fastcall VTXmlAttr(void) {};
      __fastcall ~VTXmlAttr(void) {};
   };

//---------------------------------------------------------------------------
//fun??o global p/ criar objeto da classe VTXmlAttr
//---------------------------------------------------------------------------
VTXmlAttr* __fastcall NewObjXmlAttr(void);

//---------------------------------------------------------------------------
#endif
//eof



 