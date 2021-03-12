//---------------------------------------------------------------------------
#ifndef TAplDebugH
#define TAplDebugH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "..\Apl\TApl.h"

//---------------------------------------------------------------------------
class TAplDebug : public TApl
   {
   public:
      __fastcall TAplDebug(TForm *form_owner, VTApl *apl_owner);
      __fastcall ~TAplDebug(void);
   };

#endif
//---------------------------------------------------------------------------
//eof