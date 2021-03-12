//---------------------------------------------------------------------------
#ifndef TGrupoLTC12H
#define TGrupoLTC12H

//---------------------------------------------------------------------------
#include <Classes.hpp>

class TBarra;

//---------------------------------------------------------------------------
class TGrupoLTC12 : public TObject
{
public:  // Dados locais
   int    tipo;      // Tipo dos reguladores no grupo (cf. enum_TIPO_TRAFO12 em VTGeralC.h)
   int    fase_ref1; // 1a. fase de tensão controlada na barra 'bar_ref' (A, B ou C)
   int    fase_ref2; // 2a. fase de tensão controlada na barra 'bar_ref' (N, B, C ou A))
   int    fase_ref3; // 3a. fase (tensão não-controlada)
   TBarra *bar_ref;  // Barra controlada pelo grupo de LTCs
   TList  *lisLTC;   // Lista de LTCs do grupo (classe TReg12)

public:  // Métodos
                   __fastcall  TGrupoLTC12(TBarra *bar_ref,
                                           int    fase_ref1,
                                           int    fase_ref2,
                                           int    fase_ref3,
                                           int    tipo);
                   __fastcall ~TGrupoLTC12(void);
};

#endif
//---------------------------------------------------------------------------

