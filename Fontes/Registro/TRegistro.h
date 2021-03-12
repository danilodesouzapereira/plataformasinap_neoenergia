//---------------------------------------------------------------------------
#ifndef TRegistroH
#define TRegistroH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Registry.hpp>
#include "VTRegistro.h"

//---------------------------------------------------------------------------
class TRegistro : public VTRegistro
   {
   public:
           __fastcall TRegistro(AnsiString arquivo);
           __fastcall ~TRegistro(void);
      bool __fastcall ExisteInfo(AnsiString secao, AnsiString info, AnsiString &valor);
      bool __fastcall GravaInfo(AnsiString secao, AnsiString info, AnsiString &valor);
      bool __fastcall RetiraSecao(AnsiString secao);
      
   private: //dados
      TRegistryIniFile *RegistryIniFile;
   };

#endif
//---------------------------------------------------------------------------
//eof

