//---------------------------------------------------------------------------
#ifndef ImportaMedicaoH
#define ImportaMedicaoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Export.h"

//---------------------------------------------------------------------------
class VTApl;
class VTImportaMedicao;
class VTMedicoes;

//-----------------------------------------------------------------------------
// funções da DLL
//-----------------------------------------------------------------------------
EXPORT VTImportaMedicao* __fastcall DLL_NewObjImportaMedicao(TComponent *Owner, VTApl *apl_owner);
EXPORT VTMedicoes*       __fastcall DLL_NewObjMedicoes(VTApl *apl_owner);

//---------------------------------------------------------------------------
#endif
//eof


 