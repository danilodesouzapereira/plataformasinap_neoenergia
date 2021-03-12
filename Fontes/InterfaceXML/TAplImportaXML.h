//---------------------------------------------------------------------------
#ifndef TAplImportaXMLH
#define TAplImportaXMLH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "..\Apl\TApl.h"

//---------------------------------------------------------------------------
class TAplImportaXML : public TApl
   {
   public:
      __fastcall TAplImportaXML(TForm *form_owner, VTApl *apl_owner);
      __fastcall ~TAplImportaXML(void);
   };

#endif
//---------------------------------------------------------------------------
//eof