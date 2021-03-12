//---------------------------------------------------------------------------
#ifndef VTProteswH
#define VTProteswH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
//classe para proteção de arquivos
class VTProtesw : public TObject
    {
    public://funções públicas
        __fastcall VTProtesw(void) {};
        virtual __fastcall ~VTProtesw(void) {};
        virtual bool __fastcall Gera(AnsiString dirBase, AnsiString arq_texto) = 0;
        virtual bool __fastcall Valida(AnsiString dirBase) = 0;
    };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe
//---------------------------------------------------------------------------
VTProtesw* NewObjProtesw(void);

//---------------------------------------------------------------------------
#endif

