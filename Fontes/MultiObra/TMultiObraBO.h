//---------------------------------------------------------------------------
#ifndef TMultiObraBOH
#define TMultiObraBOH

//---------------------------------------------------------------------------
#include <map>
#include <Classes.hpp>
#include "VTMultiObraBO.h"
//---------------------------------------------------------------------------
//class VTApl;
class VTMultiObra;
class VTObra;
//---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/17073894
class TMultiObraBO : public VTMultiObraBO
{
    public:  //métodos
        __fastcall  TMultiObraBO(VTApl *apl_owner);
        __fastcall ~TMultiObraBO(void);

        VTMultiObra* __fastcall AnalisaAcoesObra(VTObra *obra);
        void         __fastcall DestroiMultiObra(VTObra *obra);
        VTMultiObra* __fastcall ExisteMultiObra(VTObra *obra);
        void         __fastcall ReiniciaMultiObra(VTObra *obra);
        bool         __fastcall IsObraFinalizada(VTObra *obra);

    private:
        bool         __fastcall PossuiIpeAtributo(VTObra *obra, int tipoAtributo);

    private:  //dados locais
        //map para relacionar a obra ao multiObra
        std::map<VTObra*, VTMultiObra*> mapMultiObra;
};

//---------------------------------------------------------------------------
#endif
//eof
