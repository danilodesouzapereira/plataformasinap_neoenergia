//---------------------------------------------------------------------------
#ifndef TCampoH
#define TCampoH

//arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>
#include "VTCampo.h"

//---------------------------------------------------------------------------
class TCampo : public VTCampo
   {
   public :
      __fastcall TCampo(AnsiString nome);
      __fastcall ~TCampo(void);

   protected:   //métodos acessados via property
      bool       __fastcall PM_GetAsBoolean(void);
      TDate      __fastcall PM_GetAsDate(void);
      double     __fastcall PM_GetAsDouble(void);
      int        __fastcall PM_GetAsInteger(void);
      AnsiString __fastcall PM_GetAsString(void);
      AnsiString __fastcall PM_GetNome(void);
      void       __fastcall PM_SetAsBoolean(bool valor);
      void       __fastcall PM_SetAsDate(TDate valor);
      void       __fastcall PM_SetAsDouble(double valor);
      void       __fastcall PM_SetAsInteger(int valor);
      void       __fastcall PM_SetAsString(AnsiString valor);

   private: //dados
      struct   {
               AnsiString nome;
               AnsiString valor;
               }PD;
   };

//---------------------------------------------------------------------------
#endif
//eof
