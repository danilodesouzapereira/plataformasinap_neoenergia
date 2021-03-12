//---------------------------------------------------------------------------
#ifndef TCriptH
#define TCriptH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTCript.h"

//---------------------------------------------------------------------------
//classe para proteção de arquivos
class TCript : public VTCript
    {
    public://funções públicas
             __fastcall TCript(void);
             __fastcall ~TCript(void);
        void __fastcall DefineChave(AnsiString txt);
        bool __fastcall Executa(char *buf, int sizebuf);

    private://métodos

    private://dados
      AnsiString chave;
      int        sizekey;
      char       *key;
    };

//---------------------------------------------------------------------------
#endif

