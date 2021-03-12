//---------------------------------------------------------------------------
#ifndef TAplProtecaoH
#define TAplProtecaoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "..\Apl\TApl.h"

//---------------------------------------------------------------------------
class TAplProtecao : public TApl
   {
   public:
      __fastcall  TAplProtecao(TForm *form_owner, VTApl *apl_owner);
      __fastcall ~TAplProtecao(void);
   };

#endif
//---------------------------------------------------------------------------
//eof