//---------------------------------------------------------------------------
#ifndef VTInfosetH
#define VTInfosetH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;

//---------------------------------------------------------------------------
class VTInfoset : public TObject
   {
   public:  //property
      __property int  Count               = {read=PM_GetCount};

   public:
                   __fastcall  VTInfoset(void) {};
                   __fastcall ~VTInfoset(void) {};
      virtual bool __fastcall  AddInfo(AnsiString nome, AnsiString valor) = 0;
      virtual bool __fastcall  AddInfo(AnsiString nome, double     valor) = 0;
      virtual bool __fastcall  AddInfo(AnsiString nome, float      valor) = 0;
      virtual bool __fastcall  AddInfo(AnsiString nome, int        valor) = 0;
	  virtual bool __fastcall  AddInfo(AnsiString nome, unsigned   valor) = 0;
	  virtual bool __fastcall  AddInfo(AnsiString nome, bool   	   valor) = 0;
      virtual bool __fastcall  InsertNewLine(void) = 0;
      virtual void __fastcall  Clear(void) = 0;
      virtual bool __fastcall  GetInfo(AnsiString nome, AnsiString &valor) = 0;
      virtual bool __fastcall  GetInfo(AnsiString nome, double     &valor) = 0;
      virtual bool __fastcall  GetInfo(AnsiString nome, float      &valor) = 0;
	  virtual bool __fastcall  GetInfo(AnsiString nome, int        &valor) = 0;
	  virtual bool __fastcall  GetInfo(AnsiString nome, unsigned   &valor) = 0;
	  virtual bool __fastcall  GetInfo(AnsiString nome, bool       &valor) = 0;
      virtual bool __fastcall  LoadFromFile(AnsiString arquivo) = 0;
      virtual bool __fastcall  LoadFromFile(TForm *form) = 0;
      virtual bool __fastcall  SaveToFile(AnsiString arquivo) = 0;
      virtual bool __fastcall  SaveToFile(TForm *form) = 0;
      virtual bool __fastcall  SelectLine(int index) = 0;

      protected:  //metodos acessados via property
         virtual int __fastcall PM_GetCount(void) = 0;
   };

//---------------------------------------------------------------------------
//função global para criar objetos VTInfoset
//---------------------------------------------------------------------------
VTInfoset* __fastcall NewObjInfoset(VTApl *apl);

#endif
//---------------------------------------------------------------------------
//eof

