//---------------------------------------------------------------------------
#ifndef TAplExplorerH
#define TAplExplorerH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "..\Apl\TApl.h"

//---------------------------------------------------------------------------
class TAplCustoNodal : public TApl
   {
   public:
      __fastcall TAplCustoNodal(TForm *form_owner, VTApl *apl_owner);
      __fastcall ~TAplCustoNodal(void);
   };

#endif
//---------------------------------------------------------------------------
//eof