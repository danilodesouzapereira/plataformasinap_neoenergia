//---------------------------------------------------------------------------
#ifndef TPenFinder_h
#define TPenFinder_h

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTPenFinder.h"

//---------------------------------------------------------------------------
class TPenFinder : public VTPenFinder
   {
   public:
             __fastcall TPenFinder(void);
             __fastcall ~TPenFinder(void);
      TList* __fastcall Executa(void);
      VTPendrive* __fastcall ExistePendrive(AnsiString pen_id);

   private:
      void       __fastcall LimpaTList(TList *lisOBJ);
      void       __fastcall MLopes_GetPenDriveList(void);
      AnsiString __fastcall TrocaBarrasSimplesPorBarrasDuplas(AnsiString strin);

   private: //dados locais
      TList *lisPEN;
   };

//---------------------------------------------------------------------------
#endif
//eof

