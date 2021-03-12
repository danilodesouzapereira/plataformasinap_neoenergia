//---------------------------------------------------------------------------
#ifndef VTRegistroH
#define VTRegistroH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTRegistro : public TObject
   {
   public:
                   __fastcall VTRegistro(void) {};
      virtual      __fastcall ~VTRegistro(void) {};
      virtual bool __fastcall ExisteInfo(AnsiString secao, AnsiString info, AnsiString &valor) = 0;
      virtual bool __fastcall GravaInfo(AnsiString secao,  AnsiString info, AnsiString &valor) = 0;
      virtual bool __fastcall RetiraSecao(AnsiString secao) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe
VTRegistro* __fastcall NewObjRegistro(AnsiString arquivo);


#endif
//---------------------------------------------------------------------------//eof

