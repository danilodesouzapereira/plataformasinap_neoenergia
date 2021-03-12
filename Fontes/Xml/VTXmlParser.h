//---------------------------------------------------------------------------
#ifndef VTXmlParserH
#define VTXmlParserH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTXmlHandler;

//---------------------------------------------------------------------------
class VTXmlParser : public TObject
   {
   public:
      __fastcall VTXmlParser(void) {};
      __fastcall ~VTXmlParser(void) {};
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTXmlParser
//---------------------------------------------------------------------------
VTXmlParser* __fastcall NewObjXmlParser(VTXmlHandler *handler);

//---------------------------------------------------------------------------
#endif
//eof



 