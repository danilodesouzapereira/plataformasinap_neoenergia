//---------------------------------------------------------------------------
#ifndef VTAtributoH
#define VTAtributoH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTEqpto;

//---------------------------------------------------------------------------
class VTAtributo : public TObject
   {
   public:
                         __fastcall VTAtributo(void) {};
                         __fastcall ~VTAtributo(void) {};
      virtual int        __fastcall Count(void)=0;
      virtual void       __fastcall Executa(VTEqpto *eqpto)=0;
      virtual void       __fastcall NomeValor(int index, AnsiString &nome, AnsiString &valor)=0;
      virtual AnsiString __fastcall Valor(AnsiString nome)=0;
   };

//---------------------------------------------------------------------------
//função global p/ criar obbjeto da classe
//---------------------------------------------------------------------------
VTAtributo* __fastcall NewObjAtributo(VTApl *apl);

#endif
//---------------------------------------------------------------------------
//eof

