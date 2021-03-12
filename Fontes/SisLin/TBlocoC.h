
//---------------------------------------------------------------------------
#ifndef TBlocoCH
#define TBlocoCH

//---------------------------------------------------------------------------
#include <Classes.hpp>
using namespace std;
#include<complex>
#include ".\LinkedListDados.h"

//---------------------------------------------------------------------------
class TBlocoC : public TObject
   {
   public:
                       __fastcall  TBlocoC(int tamanho_bloco);
                       __fastcall ~TBlocoC(void);
      MatrixElementC * __fastcall  AlocaElemento(void);

   private: // Dados
      int            tamanho_bloco;  // Tamanho do bloco (posições)
      int            ultimo;         // Offset do último elemento alocado
      MatrixElementC *bloco;
   };

#endif
//---------------------------------------------------------------------------
//eof
