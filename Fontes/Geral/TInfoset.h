//---------------------------------------------------------------------------
#ifndef TInfosetH
#define TInfosetH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTInfoset.h"

//---------------------------------------------------------------------------
class VTApl;
class VTRegistro;

//---------------------------------------------------------------------------
class TInfoset : public VTInfoset
   {
   public:
           __fastcall  TInfoset(VTApl *apl);
           __fastcall ~TInfoset(void);
      bool __fastcall  AddInfo(AnsiString nome, AnsiString valor);
      bool __fastcall  AddInfo(AnsiString nome, double     valor);
      bool __fastcall  AddInfo(AnsiString nome, float      valor);
      bool __fastcall  AddInfo(AnsiString nome, int        valor);
	  bool __fastcall  AddInfo(AnsiString nome, unsigned   valor);
	  bool __fastcall  AddInfo(AnsiString nome, bool       valor);
      void __fastcall  Clear(void);
      bool __fastcall  GetInfo(AnsiString nome, AnsiString &valor);
      bool __fastcall  GetInfo(AnsiString nome, double     &valor);
      bool __fastcall  GetInfo(AnsiString nome, float      &valor);
      bool __fastcall  GetInfo(AnsiString nome, int        &valor);
	  bool __fastcall  GetInfo(AnsiString nome, unsigned   &valor);
	  bool __fastcall  GetInfo(AnsiString nome, bool       &valor);
      bool __fastcall  InsertNewLine(void);
      bool __fastcall  LoadFromFile(AnsiString arquivo);
      bool __fastcall  LoadFromFile(TForm *form);
      bool __fastcall  SaveToFile(AnsiString arquivo);
      bool __fastcall  SaveToFile(TForm *form);
      bool __fastcall  SelectLine(int index);

   protected:  //metodos acessados via property
      int __fastcall PM_GetCount(void);

   protected: //objetos externos
      VTApl *apl;

   protected: //dados locais
      TList       *lisREG;
      TStringList *lines;
      TStringList *campos;
      VTRegistro  *registro;


   protected: //métodos
      int __fastcall LeCampos(AnsiString txt, TStringList *campos);
   };

#endif
//---------------------------------------------------------------------------
//eof

