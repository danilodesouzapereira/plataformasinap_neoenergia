//---------------------------------------------------------------------------
#ifndef TRegistroH
#define TRegistroH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTRegistro.h"

//---------------------------------------------------------------------------
class VTApl;

//---------------------------------------------------------------------------
class TRegistro : public VTRegistro
   {
   protected: //dados locais
      TList *lisINFO;

      class TInfo
         {
         public:
             TInfo(void) {};
            ~TInfo(void) {};
         public:  //dados
            AnsiString nome;
            AnsiString valor;
         };

   protected: //métodos
      void   __fastcall Clear(void);
      TInfo* __fastcall ExisteInfo(AnsiString nome);

   public:
           __fastcall  TRegistro(void);
           __fastcall ~TRegistro(void);
      bool __fastcall  AddInfo(AnsiString nome, AnsiString valor);
      bool __fastcall  AddInfo(AnsiString nome, double     valor);
      bool __fastcall  AddInfo(AnsiString nome, float      valor);
      bool __fastcall  AddInfo(AnsiString nome, int        valor);
	  bool __fastcall  AddInfo(AnsiString nome, unsigned   valor);
	  bool __fastcall  AddInfo(AnsiString nome, bool       valor);
      bool __fastcall  GetAllInfoAsString(AnsiString &valor);
      bool __fastcall  GetInfo(AnsiString nome, AnsiString &valor);
      bool __fastcall  GetInfo(AnsiString nome, double     &valor);
      bool __fastcall  GetInfo(AnsiString nome, float      &valor);
      bool __fastcall  GetInfo(AnsiString nome, int        &valor);
	  bool __fastcall  GetInfo(AnsiString nome, unsigned   &valor);
	  bool __fastcall  GetInfo(AnsiString nome, bool       &valor);
   };

#endif
//---------------------------------------------------------------------------
//eof

