//---------------------------------------------------------------------------
#ifndef TAplPerdatecH
#define TAplPerdatecH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "..\Apl\TApl.h"

//---------------------------------------------------------------------------
class TAplPerdatec : public TApl
   {
   public:
      __fastcall  TAplPerdatec(TForm *form_owner, VTApl *apl_owner);
      __fastcall ~TAplPerdatec(void);
   };

#endif
//---------------------------------------------------------------------------
//eof