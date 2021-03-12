//---------------------------------------------------------------------------
#ifndef TAplBatchH
#define TAplBatchH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <StdCtrls.hpp>
#include "..\Apl\TApl.h"

//---------------------------------------------------------------------------
class TAplBatch : public TApl
   {
   public:
      __fastcall  TAplBatch(TForm *form_owner, VTApl *apl_owner);   //, TAction *ActionInsRede);
		__fastcall ~TAplBatch(void);

   private:
      void __fastcall IniciaObjetosBasicos(void);
   };

#endif
//---------------------------------------------------------------------------
//eof
