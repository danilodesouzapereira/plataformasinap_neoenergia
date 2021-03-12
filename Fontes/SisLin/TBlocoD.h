
//---------------------------------------------------------------------------
#ifndef TBlocoDH
#define TBlocoDH

//---------------------------------------------------------------------------
#include <Classes.hpp>
using namespace std;
#include ".\LinkedListDados.h"

//---------------------------------------------------------------------------
class TBlocoD : public TObject
   {
   public:
                       __fastcall  TBlocoD(int tamanho_bloco);
                       __fastcall ~TBlocoD(void);
      MatrixElementD * __fastcall  AlocaElemento(void);

   private: // Dados
      int            tamanho_bloco;  // Tamanho do bloco (posições)
      int            ultimo;         // Offset do último elemento alocado
      MatrixElementD *bloco;
   };

#endif
//---------------------------------------------------------------------------
//eof
