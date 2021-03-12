//---------------------------------------------------------------------------
#ifndef TSinap2H
#define TSinap2H

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTSinap2.h"

//---------------------------------------------------------------------------
class TSinap2 : public VTSinap2
   {
   public:
            __fastcall  TSinap2(TComponent *Owner, VTApl *apl_owner);
            __fastcall ~TSinap2(void);
      bool  __fastcall  AbreRedeCompleta(AnsiString dir_base, AnsiString dir_rede);

   private: //dados locais métodos
      bool __fastcall CarregaRedeCompleta(void);
      void __fastcall IniciaObjetosBasicos(void);

   private: //dados locais
      VTApl *apl;
   };

#endif
//---------------------------------------------------------------------------
//eof

