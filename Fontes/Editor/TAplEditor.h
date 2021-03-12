//---------------------------------------------------------------------------
#ifndef TAplEditorH
#define TAplEditorH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "..\Apl\TApl.h"

//---------------------------------------------------------------------------
class TAplEditor : public TApl
   {
   public:
      __fastcall TAplEditor(TForm *form_owner, VTApl *apl_owner);
      __fastcall ~TAplEditor(void);
   };

#endif
//---------------------------------------------------------------------------
//eof