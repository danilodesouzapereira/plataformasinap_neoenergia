//---------------------------------------------------------------------------
#ifndef VTDadoH
#define VTDadoH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTDado : public TObject
   {
   public:
                         __fastcall VTDado(void) {};
                         __fastcall ~VTDado(void) {};
      virtual AnsiString __fastcall DescUnid(void) = 0;
      virtual AnsiString __fastcall DescUnidValor(void) = 0;
      virtual AnsiString __fastcall DescValorUnid(void) = 0;
      virtual AnsiString __fastcall ValorUnid(void) = 0;

   public: //dados
      AnsiString descricao, valor, unidade;
   };

//---------------------------------------------------------------------------
//função global p/ criar objetos VTDado
//---------------------------------------------------------------------------
VTDado* __fastcall NewObjDado(AnsiString descricao);

//---------------------------------------------------------------------------
#endif
//eof
