//---------------------------------------------------------------------------
#ifndef TAplFasometroH
#define TAplFasometroH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "..\Apl\TApl.h"

//---------------------------------------------------------------------------
class TAplFasometro : public TApl
   {
   public:
      __fastcall TAplFasometro(TForm *form_owner, VTApl *apl_owner);
      __fastcall ~TAplFasometro(void);
   };

#endif
//---------------------------------------------------------------------------
//eof