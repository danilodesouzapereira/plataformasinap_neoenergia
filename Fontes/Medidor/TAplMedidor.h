//---------------------------------------------------------------------------
#ifndef TAplMedidorH
#define TAplMedidorH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "..\Apl\TApl.h"

//---------------------------------------------------------------------------
class TAplMedidor : public TApl
   {
   public:
      __fastcall TAplMedidor(TForm *form_owner, VTApl *apl_owner);
      __fastcall ~TAplMedidor(void);
   };

#endif
//---------------------------------------------------------------------------
//eof