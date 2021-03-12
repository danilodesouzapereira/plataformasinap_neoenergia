//---------------------------------------------------------------------------
#ifndef TGrupoLTC32H
#define TGrupoLTC32H

//---------------------------------------------------------------------------
#include <Classes.hpp>

class TBarra;

//---------------------------------------------------------------------------
class TGrupoLTC32 : public TObject
{
public:  // Dados locais
   int    tipo;     // Tipo dos reguladores no grupo (cf. enum_TIPO_TRAFO32 em VTGeralC.h)
   TBarra *bar_ref; // Barra controlada pelo grupo de LTCs
   TList  *lisLTC;  // Lista de LTCs do grupo (classe TReg32)

public:  // Métodos
                   __fastcall  TGrupoLTC32(TBarra *bar_ref,
                                           int     tipo);
                   __fastcall ~TGrupoLTC32(void);
};

#endif
//---------------------------------------------------------------------------

