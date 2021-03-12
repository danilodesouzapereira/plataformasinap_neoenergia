//---------------------------------------------------------------------------
#ifndef TAplRntH
#define TAplRntH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <StdCtrls.hpp>
#include "..\Apl\TApl.h"

//---------------------------------------------------------------------------
class TAplRnt : public TApl
   {
   public:
      __fastcall  TAplRnt(TForm *form_owner, VTApl *apl_owner);   //, TAction *ActionInsRede);
		__fastcall ~TAplRnt(void);

   private:
      void __fastcall IniciaObjetosBasicos(void);
   };

#endif
//---------------------------------------------------------------------------
//eof