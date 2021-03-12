//---------------------------------------------------------------------------
#ifndef TAplExplorerH
#define TAplExplorerH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "..\Apl\TApl.h"

//---------------------------------------------------------------------------
class TAplExplorer : public TApl
   {
   public:
      __fastcall TAplExplorer(TForm *form_owner, VTApl *apl_owner);
      __fastcall ~TAplExplorer(void);
   };

#endif
//---------------------------------------------------------------------------
//eof