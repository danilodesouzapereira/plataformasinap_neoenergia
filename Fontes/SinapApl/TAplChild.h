//---------------------------------------------------------------------------
#ifndef TAplChildH
#define TAplChildH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <StdCtrls.hpp>
#include "..\Apl\TApl.h"

//---------------------------------------------------------------------------
class TAplChild : public TApl
   {
   public:
      __fastcall  TAplChild(TForm *form_owner, VTApl *apl_owner, TAction *ActionInsRede);
      __fastcall ~TAplChild(void);

   private:
	  void __fastcall IniciaObjetosBasicos(void);
	  void __fastcall CriarArqRede(AnsiString arq_rede);
   };

#endif
//---------------------------------------------------------------------------
//eof