//---------------------------------------------------------------------------
#ifndef TAplPerdaregH
#define TAplPerdaregH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "..\Apl\TApl.h"

//---------------------------------------------------------------------------
class TAplPerdareg : public TApl
   {
   public:
      __fastcall  TAplPerdareg(TForm *form_owner, VTApl *apl_owner);
      __fastcall ~TAplPerdareg(void);
   };

#endif
//---------------------------------------------------------------------------
//eof