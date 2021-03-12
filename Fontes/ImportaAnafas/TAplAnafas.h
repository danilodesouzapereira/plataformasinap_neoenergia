//---------------------------------------------------------------------------
#ifndef TAplAnafasH
#define TAplAnafasH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "..\Apl\TApl.h"

//---------------------------------------------------------------------------
class TAplAnafas : public TApl
   {
   public:
      __fastcall TAplAnafas(TForm *form_owner, VTApl *apl_owner);
      __fastcall ~TAplAnafas(void);
   };

#endif
//---------------------------------------------------------------------------
//eof