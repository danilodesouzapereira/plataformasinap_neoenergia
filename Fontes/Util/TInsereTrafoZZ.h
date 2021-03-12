// ---------------------------------------------------------------------------
#ifndef TInsereTrafoZZH
#define TInsereTrafoZZH

// arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>

class VTApl;
class VTBarra;

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18417177
class TInsereTrafoZZ
{
public:
    __fastcall TInsereTrafoZZ(VTApl *apl);
    __fastcall ~TInsereTrafoZZ(void);
    bool __fastcall Executa(TList *lisTRAFO_SED);

private: // métodos
    bool __fastcall ExisteTrafoZZ(VTBarra *barra);
    void __fastcall InsereZZ(VTBarra *barra);

    // dados locais
    VTApl *apl;
};

// ---------------------------------------------------------------------------
#endif
// eof
