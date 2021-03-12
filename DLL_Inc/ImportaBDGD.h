//---------------------------------------------------------------------------
#ifndef ImportaBDGDH
#define ImportaBDGDH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Export.h"
#include "..\Fontes\ImportaRedeGis\VTImportaRedeGis.h"

//---------------------------------------------------------------------------
class VTApl;
class VTImportaRedeGIS;
class VTImportador;
//-----------------------------------------------------------------------------
// funções da DLL
//-----------------------------------------------------------------------------
EXPORT VTImportaRedeGIS* __fastcall DLL_NewObjImportaBDGD(TComponent *Owner, VTApl *apl_owner);
EXPORT VTImportador* __fastcall DLL_NewObjImportadorUgrid(VTApl *apl, UnicodeString filename);
//---------------------------------------------------------------------------
#endif
//eof


