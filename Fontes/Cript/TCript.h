//---------------------------------------------------------------------------
#ifndef TCriptH
#define TCriptH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTCript.h"

//---------------------------------------------------------------------------
//classe para prote��o de arquivos
class TCript : public VTCript
    {
    public://fun��es p�blicas
             __fastcall TCript(void);
             __fastcall ~TCript(void);
        void __fastcall DefineChave(AnsiString txt);
        bool __fastcall Executa(char *buf, int sizebuf);

    private://m�todos

    private://dados
      AnsiString chave;
      int        sizekey;
      char       *key;
    };

//---------------------------------------------------------------------------
#endif

