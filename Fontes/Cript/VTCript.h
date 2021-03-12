//---------------------------------------------------------------------------
#ifndef VTCriptH
#define VTCriptH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
//classe para proteção de arquivos
class VTCript : public TObject
    {
    public://funções públicas
                     __fastcall VTCript(void) {};
        virtual      __fastcall ~VTCript(void) {};
        virtual void __fastcall DefineChave(AnsiString txt) = 0;
        virtual bool __fastcall Executa(char *buf, int sizebuf) = 0;
    };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe
//---------------------------------------------------------------------------
VTCript* __fastcall NewObjCript(void);

//---------------------------------------------------------------------------
#endif

