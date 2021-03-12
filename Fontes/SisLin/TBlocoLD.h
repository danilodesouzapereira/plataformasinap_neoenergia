
//---------------------------------------------------------------------------
#ifndef TBlocoLDH
#define TBlocoLDH

//---------------------------------------------------------------------------
#include <Classes.hpp>
using namespace std;
#include ".\LinkedListDados.h"

//---------------------------------------------------------------------------
class TBlocoLD : public TObject
   {
   public:
                        __fastcall  TBlocoLD(int tamanho_bloco);
                        __fastcall ~TBlocoLD(void);
      MatrixElementLD * __fastcall  AlocaElemento(void);

   private: // Dados
      int             tamanho_bloco;  // Tamanho do bloco (posi��es)
      int             ultimo;         // Offset do �ltimo elemento alocado
      MatrixElementLD *bloco;
   };

#endif
//---------------------------------------------------------------------------
//eof
