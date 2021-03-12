//---------------------------------------------------------------------------
#ifndef TGrupoH
#define TGrupoH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTGrupo.h"

//---------------------------------------------------------------------------
class TGrupo : public VTGrupo
   {
   public:
             __fastcall  TGrupo(void);
             __fastcall ~TGrupo(void);
      bool   __fastcall  ExisteFasorEnabled(void);
      bool   __fastcall  InsereFasor(VTFasor *fasor);
      TList* __fastcall  LisFasor(void);
      void   __fastcall  LisFasor(TList *lisEXT);

   private: //dados locais
      TList *lisFASOR;
   };

//---------------------------------------------------------------------------
#endif
//eof
