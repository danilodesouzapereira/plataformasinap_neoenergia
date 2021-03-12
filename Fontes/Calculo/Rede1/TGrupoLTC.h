//---------------------------------------------------------------------------
#ifndef TGrupoLTCH
#define TGrupoLTCH

//---------------------------------------------------------------------------
#include <Classes.hpp>

class TBarra;

//---------------------------------------------------------------------------
class TGrupoLTC : public TObject
{
public:  // Dados locais
   int    tipo;        // Tipo dos reguladores no grupo (cf. enum_TIPO_TRAFO2 em VTGeralC.h)
   TBarra *bar_ref;   // Barra controlada pelo grupo de LTCs
   TList  *lisLTC;    // Lista de LTCs do grupo (classe TReg2)

public:  // Métodos
                   __fastcall  TGrupoLTC(TBarra *bar_ref,
                                         int    tipo);
                   __fastcall ~TGrupoLTC(void);
};

#endif
//---------------------------------------------------------------------------

