//---------------------------------------------------------------------------
#ifndef ImportaRedeGISH
#define ImportaRedeGISH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Export.h"
#include "..\Fontes\ImportaRedeGIS\VTImportaRedeGIS.h"

//---------------------------------------------------------------------------
class VTApl;
class VTImportaRedeGIS;

//-----------------------------------------------------------------------------
// funções da DLL
//-----------------------------------------------------------------------------
EXPORT VTImportaRedeGIS* __fastcall DLL_NewObjImportaRedeGIS(TComponent *Owner, VTApl *apl_owner);
EXPORT TForm*            __fastcall DLL_NewFormCfgImporta(TComponent *Owner, strOPImporta *opcao);

//---------------------------------------------------------------------------
#endif
//eof


