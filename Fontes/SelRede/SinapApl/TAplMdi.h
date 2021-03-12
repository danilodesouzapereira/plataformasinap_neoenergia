//---------------------------------------------------------------------------
#ifndef TAplMdiH
#define TAplMdiH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "..\Apl\TApl.h"

//---------------------------------------------------------------------------
class TAplMdi : public TApl
   {
   public:
           __fastcall  TAplMdi(TForm *form_owner, VTApl *apl_owner);
           __fastcall ~TAplMdi(void);
      void __fastcall IniciaObjetosBasicos(void);

   private:
      bool __fastcall DeleteObject(TMetaClass* meta_class);

   };

#endif
//---------------------------------------------------------------------------
//eof