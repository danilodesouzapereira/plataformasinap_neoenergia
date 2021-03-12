//---------------------------------------------------------------------------
#ifndef VTXmlHandlerH
#define VTXmlHandlerH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTXmlTag;

//---------------------------------------------------------------------------
class VTXmlHandler : public TObject
   {
   public:
                   __fastcall VTXmlHandler(void) {};
                   __fastcall ~VTXmlHandler(void) {};
      virtual bool __fastcall InsereTag(VTXmlTag *tag) = 0;
      virtual bool __fastcall TrataErro(AnsiString err_msg) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTXmlHandler
//---------------------------------------------------------------------------
VTXmlHandler* __fastcall NewObjXmlHandler(void);

//---------------------------------------------------------------------------
#endif
//eof



 