//---------------------------------------------------------------------------
#ifndef VTRegistroH
#define VTRegistroH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTRegistro : public TObject
   {
   public:
                   __fastcall  VTRegistro(void) {};
                   __fastcall ~VTRegistro(void) {};
      virtual bool __fastcall  AddInfo(AnsiString nome, AnsiString valor) = 0;
      virtual bool __fastcall  AddInfo(AnsiString nome, double     valor) = 0;
      virtual bool __fastcall  AddInfo(AnsiString nome, float      valor) = 0;
      virtual bool __fastcall  AddInfo(AnsiString nome, int        valor) = 0;
      virtual bool __fastcall  AddInfo(AnsiString nome, unsigned   valor) = 0;
	  virtual bool __fastcall  GetAllInfoAsString(AnsiString &valor) = 0;
      virtual bool __fastcall  GetInfo(AnsiString nome, AnsiString &valor) = 0;
      virtual bool __fastcall  GetInfo(AnsiString nome, double     &valor) = 0;
      virtual bool __fastcall  GetInfo(AnsiString nome, float      &valor) = 0;
      virtual bool __fastcall  GetInfo(AnsiString nome, int        &valor) = 0;
	  virtual bool __fastcall  GetInfo(AnsiString nome, unsigned   &valor) = 0;
   };

//---------------------------------------------------------------------------
//função global para criar objetos VTRegistro
//---------------------------------------------------------------------------
VTRegistro* __fastcall NewObjRegistro(void);

#endif
//---------------------------------------------------------------------------
//eof

