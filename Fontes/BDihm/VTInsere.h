//---------------------------------------------------------------------------
#ifndef VTInsereH
#define VTInsereH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTReducao;

//---------------------------------------------------------------------------
class VTInsere : public TObject
    {
    public://funções públicas
                     __fastcall  VTInsere(void) {};
        virtual      __fastcall ~VTInsere(void) {};
        virtual bool __fastcall  Executa(AnsiString cod_rede) = 0;
        virtual bool __fastcall  Executa(VTReducao *reducao) = 0;
    };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe
//---------------------------------------------------------------------------
VTInsere* __fastcall NewObjInsere(VTApl *apl_owner, TAction *ActionInsRede);

//---------------------------------------------------------------------------
#endif

