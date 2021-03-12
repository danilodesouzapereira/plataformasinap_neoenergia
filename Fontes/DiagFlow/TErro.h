//---------------------------------------------------------------------------
#ifndef TErroH
#define TErroH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTEqpto;

//---------------------------------------------------------------------------
enum tipoERRO {AVISO=0, ERRO};

//---------------------------------------------------------------------------
class TErro : public TObject
   {
   public:
                 __fastcall TErro(VTEqpto *eqpto, int tipo, AnsiString descricao);
                 __fastcall ~TErro(void);
      AnsiString __fastcall TipoDescricao(void);
      AnsiString __fastcall TipoEqptoDescricao(void);

   private:
      AnsiString __fastcall TipoAsString(void);

   public: //dados
      int        tipo;
      AnsiString descricao;
      VTEqpto    *eqpto;
   };

//---------------------------------------------------------------------------
#endif
//eof
 