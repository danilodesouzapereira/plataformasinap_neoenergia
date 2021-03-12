//---------------------------------------------------------------------------
#ifndef TValCaboH
#define TValCaboH

//arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>
#include "VTValCabo.h"

//---------------------------------------------------------------------------
class TValCabo : public VTValCabo
   {
   public:
              __fastcall  TValCabo(VTApl *apl);
				  __fastcall ~TValCabo(void);
      bool    __fastcall  ValidaCaboMonopolar(VTCabo *cabo);
      bool    __fastcall  ValidaCaboMultiplexado(VTCabo *cabo);

   private: //métodos
      VTCabo* __fastcall ExisteCabo(TList *lisCABO, AnsiString extern_id);
      int     __fastcall ExtraiCampos(AnsiString txt);
      void    __fastcall LeArquivoCaboMonopolar(void);
      void    __fastcall LeArquivoCaboMultiplexado(void);

   private: //objetos externos
      VTApl   *apl;

   private: //dados locais
      TStrings *campos;
      TStrings *lines;
      TList    *lisCABO_MONO;
      TList    *lisCABO_MULT;
   };

#endif
//-----------------------------------------------------------------------------
// eof
