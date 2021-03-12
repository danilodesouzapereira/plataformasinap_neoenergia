//---------------------------------------------------------------------------
#ifndef VTIconesH
#define VTIconesH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTIcone;

//---------------------------------------------------------------------------
class VTIcones : public TObject
   {
   public:
                       __fastcall  VTIcones(void) {};
      virtual          __fastcall ~VTIcones(void) {};
      virtual VTIcone* __fastcall  ExisteIcone(AnsiString eqpto_cod, int index) = 0;
      virtual VTIcone* __fastcall  ExisteIcone(int eqpto_id, int index) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTIcones
//---------------------------------------------------------------------------
VTIcones* __fastcall NewObjIcones(TForm *Owner, AnsiString dir_icone);

//---------------------------------------------------------------------------
#endif
//eof



 