//---------------------------------------------------------------------------
#ifndef TAplAnaredeH
#define TAplAnaredeH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "..\Apl\TApl.h"

//---------------------------------------------------------------------------
class TAplAnarede : public TApl
   {
   public:
      __fastcall TAplAnarede(TForm *form_owner, VTApl *apl_owner);
      __fastcall ~TAplAnarede(void);
   };

#endif
//---------------------------------------------------------------------------
//eof