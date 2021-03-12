//---------------------------------------------------------------------------
#ifndef TAtributoH
#define TAtributoH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTEqpto;

//---------------------------------------------------------------------------
class TAtributo : public TObject
   {
   public:
      __fastcall TAtributo(VTEqpto *eqpto, int bit_index, bool do_eqpto, bool grafico,
                           AnsiString nome, AnsiString unidade="");
      __fastcall ~TAtributo(void);

   public: //dados
      VTEqpto    *eqpto;
      AnsiString nome;
      AnsiString unidade;
      int        bit_index;
      bool       do_eqpto;
      bool       grafico;
      bool       enabled;
   };


#endif
//---------------------------------------------------------------------------
//eof


