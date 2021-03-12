//---------------------------------------------------------------------------
#ifndef VTXmlTagH
#define VTXmlTagH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTXmlAttr;

//---------------------------------------------------------------------------
class VTXmlTag : public TObject
   {
   public:
                   __fastcall VTXmlTag(void) {};
                   __fastcall ~VTXmlTag(void) {};
      virtual bool __fastcall DefineTagName(AnsiString tag_name) = 0;
      virtual bool __fastcall InsereXmlAttr(VTXmlAttr *attr) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTXmlTag
//---------------------------------------------------------------------------
VTXmlTag* __fastcall NewObjXmlTag(void);

//---------------------------------------------------------------------------
#endif
//eof



 