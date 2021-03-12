//---------------------------------------------------------------------------
#ifndef TAplCurtoH
#define TAplCurtoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "..\Apl\TApl.h"

//---------------------------------------------------------------------------
class TAplCurto : public TApl
   {
   public:
      __fastcall TAplCurto(TForm *form_owner, VTApl *apl_owner);
      __fastcall ~TAplCurto(void);
   };

#endif
//---------------------------------------------------------------------------
//eof