//---------------------------------------------------------------------------
#ifndef VTCriptH
#define VTCriptH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
//classe para prote��o de arquivos
class VTCript : public TObject
    {
    public://fun��es p�blicas
                     __fastcall VTCript(void) {};
        virtual      __fastcall ~VTCript(void) {};
        virtual void __fastcall DefineChave(AnsiString txt) = 0;
        virtual bool __fastcall Executa(char *buf, int sizebuf) = 0;
    };

//---------------------------------------------------------------------------
//fun��o global p/ criar objeto da classe
//---------------------------------------------------------------------------
VTCript* __fastcall NewObjCript(void);

//---------------------------------------------------------------------------
#endif

