//---------------------------------------------------------------------------
#ifndef PlanejamentoH
#define PlanejamentoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Export.h"

//-----------------------------------------------------------------------------
class VTApl;
class VTEstudo;
class VTPlanejamento;

//-----------------------------------------------------------------------------
EXPORT VTEstudo*       __fastcall DLL_NewObjEstudo(VTApl *apl_owner);
EXPORT VTPlanejamento* __fastcall DLL_NewObjPlanejamento(VTApl *apl_owner);

//---------------------------------------------------------------------------
#endif
//eof


