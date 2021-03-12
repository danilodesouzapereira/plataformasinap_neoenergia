//---------------------------------------------------------------------------
#ifndef VTProteswH
#define VTProteswH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
//classe para prote��o de arquivos
class VTProtesw : public TObject
    {
    public://fun��es p�blicas
        __fastcall VTProtesw(void) {};
        virtual __fastcall ~VTProtesw(void) {};
        virtual bool __fastcall Gera(AnsiString dirBase, AnsiString arq_texto) = 0;
        virtual bool __fastcall Valida(AnsiString dirBase) = 0;
    };

//---------------------------------------------------------------------------
//fun��o global p/ criar objeto da classe
//---------------------------------------------------------------------------
VTProtesw* NewObjProtesw(void);

//---------------------------------------------------------------------------
#endif

