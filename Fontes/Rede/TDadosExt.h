//---------------------------------------------------------------------------
#ifndef TDadosExtH
#define TDadosExtH

//arquivos inclu�dos-----------------------------------------------------------
#include <Classes.hpp>
#include "Estrutura.h"
#include "VTDadosExt.h"

//---------------------------------------------------------------------------
class VTCurva;

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16666485
class TDadosExt : public VTDadosExt
{
    public:
                    __fastcall  TDadosExt(void);
                    __fastcall ~TDadosExt(void);
        VTDadosExt* __fastcall  Clone(void);
        void        __fastcall  CopiaAtributosDe(VTDadosExt &ref);
        void        __fastcall  DefineCurva(VTCurva *curva);

    protected: // m�todos acessados via property
        VTCurva*   __fastcall PM_GetCurva(void);

    private:
        VTCurva *curvaDemanda;
   };

//---------------------------------------------------------------------------
#endif
//eof
